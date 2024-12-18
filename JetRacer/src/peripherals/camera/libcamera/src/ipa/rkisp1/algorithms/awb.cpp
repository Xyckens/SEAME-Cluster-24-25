/* SPDX-License-Identifier: LGPL-2.1-or-later */
/*
 * Copyright (C) 2021-2022, Ideas On Board
 *
 * AWB control algorithm
 */

#include "awb.h"

#include <algorithm>
#include <ios>

#include <libcamera/base/log.h>

#include <libcamera/control_ids.h>

#include <libcamera/ipa/core_ipa_interface.h>

#include "libipa/colours.h"

/**
 * \file awb.h
 */

namespace libcamera {

namespace ipa::rkisp1::algorithms {

/**
 * \class Awb
 * \brief A Grey world white balance correction algorithm
 */

LOG_DEFINE_CATEGORY(RkISP1Awb)

/* Minimum mean value below which AWB can't operate. */
constexpr double kMeanMinThreshold = 2.0;

Awb::Awb()
	: rgbMode_(false)
{
}

/**
 * \copydoc libcamera::ipa::Algorithm::configure
 */
int Awb::configure(IPAContext &context,
		   const IPACameraSensorInfo &configInfo)
{
	context.activeState.awb.gains.manual = RGB<double>{ 1.0 };
	context.activeState.awb.gains.automatic = RGB<double>{ 1.0 };
	context.activeState.awb.autoEnabled = true;

	/*
	 * Define the measurement window for AWB as a centered rectangle
	 * covering 3/4 of the image width and height.
	 */
	context.configuration.awb.measureWindow.h_offs = configInfo.outputSize.width / 8;
	context.configuration.awb.measureWindow.v_offs = configInfo.outputSize.height / 8;
	context.configuration.awb.measureWindow.h_size = 3 * configInfo.outputSize.width / 4;
	context.configuration.awb.measureWindow.v_size = 3 * configInfo.outputSize.height / 4;

	context.configuration.awb.enabled = true;

	return 0;
}

/**
 * \copydoc libcamera::ipa::Algorithm::queueRequest
 */
void Awb::queueRequest(IPAContext &context,
		       [[maybe_unused]] const uint32_t frame,
		       IPAFrameContext &frameContext,
		       const ControlList &controls)
{
	auto &awb = context.activeState.awb;

	const auto &awbEnable = controls.get(controls::AwbEnable);
	if (awbEnable && *awbEnable != awb.autoEnabled) {
		awb.autoEnabled = *awbEnable;

		LOG(RkISP1Awb, Debug)
			<< (*awbEnable ? "Enabling" : "Disabling") << " AWB";
	}

	const auto &colourGains = controls.get(controls::ColourGains);
	if (colourGains && !awb.autoEnabled) {
		awb.gains.manual.r() = (*colourGains)[0];
		awb.gains.manual.b() = (*colourGains)[1];

		LOG(RkISP1Awb, Debug)
			<< "Set colour gains to " << awb.gains.manual;
	}

	frameContext.awb.autoEnabled = awb.autoEnabled;

	if (!awb.autoEnabled)
		frameContext.awb.gains = awb.gains.manual;
}

/**
 * \copydoc libcamera::ipa::Algorithm::prepare
 */
void Awb::prepare(IPAContext &context, const uint32_t frame,
		  IPAFrameContext &frameContext, RkISP1Params *params)
{
	/*
	 * This is the latest time we can read the active state. This is the
	 * most up-to-date automatic values we can read.
	 */
	if (frameContext.awb.autoEnabled)
		frameContext.awb.gains = context.activeState.awb.gains.automatic;

	auto gainConfig = params->block<BlockType::AwbGain>();
	gainConfig.setEnabled(true);

	gainConfig->gain_green_b = std::clamp<int>(256 * frameContext.awb.gains.g(), 0, 0x3ff);
	gainConfig->gain_blue = std::clamp<int>(256 * frameContext.awb.gains.b(), 0, 0x3ff);
	gainConfig->gain_red = std::clamp<int>(256 * frameContext.awb.gains.r(), 0, 0x3ff);
	gainConfig->gain_green_r = std::clamp<int>(256 * frameContext.awb.gains.g(), 0, 0x3ff);

	/* If we have already set the AWB measurement parameters, return. */
	if (frame > 0)
		return;

	auto awbConfig = params->block<BlockType::Awb>();
	awbConfig.setEnabled(true);

	/* Configure the measure window for AWB. */
	awbConfig->awb_wnd = context.configuration.awb.measureWindow;

	/* Number of frames to use to estimate the means (0 means 1 frame). */
	awbConfig->frames = 0;

	/* Select RGB or YCbCr means measurement. */
	if (rgbMode_) {
		awbConfig->awb_mode = RKISP1_CIF_ISP_AWB_MODE_RGB;

		/*
		 * For RGB-based measurements, pixels are selected with maximum
		 * red, green and blue thresholds that are set in the
		 * awb_ref_cr, awb_min_y and awb_ref_cb respectively. The other
		 * values are not used, set them to 0.
		 */
		awbConfig->awb_ref_cr = 250;
		awbConfig->min_y = 250;
		awbConfig->awb_ref_cb = 250;

		awbConfig->max_y = 0;
		awbConfig->min_c = 0;
		awbConfig->max_csum = 0;
	} else {
		awbConfig->awb_mode = RKISP1_CIF_ISP_AWB_MODE_YCBCR;

		/* Set the reference Cr and Cb (AWB target) to white. */
		awbConfig->awb_ref_cb = 128;
		awbConfig->awb_ref_cr = 128;

		/*
		 * Filter out pixels based on luminance and chrominance values.
		 * The acceptable luma values are specified as a [16, 250]
		 * range, while the acceptable chroma values are specified with
		 * a minimum of 16 and a maximum Cb+Cr sum of 250.
		 */
		awbConfig->min_y = 16;
		awbConfig->max_y = 250;
		awbConfig->min_c = 16;
		awbConfig->max_csum = 250;
	}
}

/**
 * \copydoc libcamera::ipa::Algorithm::process
 */
void Awb::process(IPAContext &context,
		  [[maybe_unused]] const uint32_t frame,
		  IPAFrameContext &frameContext,
		  const rkisp1_stat_buffer *stats,
		  ControlList &metadata)
{
	const rkisp1_cif_isp_stat *params = &stats->params;
	const rkisp1_cif_isp_awb_stat *awb = &params->awb;
	IPAActiveState &activeState = context.activeState;
	RGB<double> rgbMeans;

	metadata.set(controls::AwbEnable, frameContext.awb.autoEnabled);
	metadata.set(controls::ColourGains, {
			static_cast<float>(frameContext.awb.gains.r()),
			static_cast<float>(frameContext.awb.gains.b())
		});
	metadata.set(controls::ColourTemperature, activeState.awb.temperatureK);

	if (!stats || !(stats->meas_type & RKISP1_CIF_ISP_STAT_AWB)) {
		LOG(RkISP1Awb, Error) << "AWB data is missing in statistics";
		return;
	}

	if (rgbMode_) {
		rgbMeans = {{
			static_cast<double>(awb->awb_mean[0].mean_y_or_g),
			static_cast<double>(awb->awb_mean[0].mean_cr_or_r),
			static_cast<double>(awb->awb_mean[0].mean_cb_or_b)
		}};
	} else {
		/* Get the YCbCr mean values */
		Vector<double, 3> yuvMeans({
			static_cast<double>(awb->awb_mean[0].mean_y_or_g),
			static_cast<double>(awb->awb_mean[0].mean_cb_or_b),
			static_cast<double>(awb->awb_mean[0].mean_cr_or_r)
		});

		/*
		 * Convert from YCbCr to RGB. The hardware uses the following
		 * formulas:
		 *
		 * Y  =  16 + 0.2500 R + 0.5000 G + 0.1094 B
		 * Cb = 128 - 0.1406 R - 0.2969 G + 0.4375 B
		 * Cr = 128 + 0.4375 R - 0.3750 G - 0.0625 B
		 *
		 * This seems to be based on limited range BT.601 with Q1.6
		 * precision.
		 *
		 * The inverse matrix is:
		 *
		 * [[1,1636, -0,0623,  1,6008]
		 *  [1,1636, -0,4045, -0,7949]
		 *  [1,1636,  1,9912, -0,0250]]
		 */
		static const Matrix<double, 3, 3> yuv2rgbMatrix({
			1.1636, -0.0623,  1.6008,
			1.1636, -0.4045, -0.7949,
			1.1636,  1.9912, -0.0250
		});
		static const Vector<double, 3> yuv2rgbOffset({
			16, 128, 128
		});

		rgbMeans = yuv2rgbMatrix * (yuvMeans - yuv2rgbOffset);

		/*
		 * Due to hardware rounding errors in the YCbCr means, the
		 * calculated RGB means may be negative. This would lead to
		 * negative gains, messing up calculation. Prevent this by
		 * clamping the means to positive values.
		 */
		rgbMeans = rgbMeans.max(0.0);
	}

	/*
	 * The ISP computes the AWB means after applying the colour gains,
	 * divide by the gains that were used to get the raw means from the
	 * sensor.
	 */
	rgbMeans /= frameContext.awb.gains;

	/*
	 * If the means are too small we don't have enough information to
	 * meaningfully calculate gains. Freeze the algorithm in that case.
	 */
	if (rgbMeans.r() < kMeanMinThreshold && rgbMeans.g() < kMeanMinThreshold &&
	    rgbMeans.b() < kMeanMinThreshold)
		return;

	activeState.awb.temperatureK = estimateCCT(rgbMeans);

	/* Metadata shall contain the up to date measurement */
	metadata.set(controls::ColourTemperature, activeState.awb.temperatureK);

	/*
	 * Estimate the red and blue gains to apply in a grey world. The green
	 * gain is hardcoded to 1.0. Avoid divisions by zero by clamping the
	 * divisor to a minimum value of 1.0.
	 */
	RGB<double> gains({
		rgbMeans.g() / std::max(rgbMeans.r(), 1.0),
		1.0,
		rgbMeans.g() / std::max(rgbMeans.b(), 1.0)
	});

	/*
	 * Clamp the gain values to the hardware, which expresses gains as Q2.8
	 * unsigned integer values. Set the minimum just above zero to avoid
	 * divisions by zero when computing the raw means in subsequent
	 * iterations.
	 */
	gains = gains.max(1.0 / 256).min(1023.0 / 256);

	/* Filter the values to avoid oscillations. */
	double speed = 0.2;
	gains = gains * speed + activeState.awb.gains.automatic * (1 - speed);

	activeState.awb.gains.automatic = gains;

	LOG(RkISP1Awb, Debug)
		<< std::showpoint
		<< "Means " << rgbMeans << ", gains "
		<< activeState.awb.gains.automatic << ", temp "
		<< activeState.awb.temperatureK << "K";
}

REGISTER_IPA_ALGORITHM(Awb, "Awb")

} /* namespace ipa::rkisp1::algorithms */

} /* namespace libcamera */