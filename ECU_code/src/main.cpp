#include "Arduino.h"
#include "FreeRTOSVariant.h"
#include "lib.hpp"
#include "projdefs.h"
#include <ArduinoLog.h>
#include <Arduino_FreeRTOS.h>
#include <lib.hpp>
#include <mcp2515.h>

extern MCP2515 mcp;
extern struct can_frame frame;

void setup()
{
    Serial.begin(115200);
    Log.begin(LOG_LEVEL_VERBOSE, &Serial);

    pinMode(LEFT_BLINKER_PIN, OUTPUT);
    pinMode(RIGHT_BLINKER_PIN, OUTPUT);
    pinMode(SENSOR_PIN, INPUT);
    attachInterrupt(digitalPinToInterrupt(SENSOR_PIN), incrementSpeedCount,
                    FALLING);

    // MCP settings
    mcp.reset();
    if (mcp.setBitrate(CAN_125KBPS, MCP_16MHZ) != MCP2515::ERROR_OK)
    {
        Log.errorln("fail setting bitrate");
        errorHalt();
    }
    if (mcp.setNormalMode() != MCP2515::ERROR_OK)
    {
        Log.errorln("Fail setting normal mode");
        errorHalt();
    }

    // Set the CAN frame
    frame.can_id = 0x0F6;
    frame.can_dlc = 4;
    frame.data[0] = 0x00;
    frame.data[1] = 0x00;
    frame.data[2] = 0x00;
    frame.data[3] = 0x00;

    BaseType_t task1 =
        xTaskCreate(handleCanFrames, "CAN handler", 256, NULL, 2, nullptr);
    BaseType_t task2 =
        xTaskCreate(blinkBlinkers, "blinkers", 256, NULL, 1, nullptr);

    if (task1 != pdPASS || task2 != pdPASS)
    {
        Log.errorln("Failed to create tasks");
        errorHalt();
    }

    Log.infoln("Starting scheduler\n");
    vTaskStartScheduler();
    Log.infoln("program ended");
}

void loop() {}
