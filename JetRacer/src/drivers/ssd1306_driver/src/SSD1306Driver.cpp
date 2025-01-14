#include "SSD1306Driver.hpp"
#include "font.hpp"
#include <cstdlib>
#include <ssd1306.hpp>

using namespace std::chrono_literals;

SSD1306Driver::SSD1306Driver(std::shared_ptr<rclcpp::Node> node)
    : ADriver(node, SSD1306_I2C_ADDR)
{
    this->ping();

    RCLCPP_INFO(node->get_logger(),
                "SSD1306 succefully initiated at address: 0x%02X",
                device_address_);
}

SSD1306Driver::~SSD1306Driver() {}

/*custom_msgs
 * @brief try to read and write dummy bytes to the display.
 *
 * @return
 */
void SSD1306Driver::initDisplay()
{
    auto write_request =
        std::make_shared<custom_msgs::srv::I2cService::Request>();
    auto read_request =
        std::make_shared<custom_msgs::srv::I2cService::Request>();

    write_request->set__read_request(false);
    write_request->set__device_address(SSD1306_I2C_ADDR);
    write_request->write_data.push_back(SSD1306_COMM_CONTROL_BYTE);

    read_request->set__read_request(true);
    read_request->set__device_address(SSD1306_I2C_ADDR);
    read_request->set__read_length(1);

    i2c_client_->async_send_request(write_request,
                                    std::bind(&SSD1306Driver::handleI2cResponse,
                                              this, std::placeholders::_1));
    i2c_client_->async_send_request(read_request,
                                    std::bind(&SSD1306Driver::handleI2cResponse,
                                              this, std::placeholders::_1));
}

void SSD1306Driver::onOffDisplay(uint8_t onoff)
{
    auto request = std::make_shared<custom_msgs::srv::I2cService::Request>();

    request->set__read_request(false);
    request->set__device_address(SSD1306_I2C_ADDR);
    request->write_data.push_back(SSD1306_COMM_CONTROL_BYTE);
    if (!onoff)
        request->write_data.push_back(SSD1306_COMM_DISPLAY_ON);
    else
        request->write_data.push_back(SSD1306_COMM_DISPLAY_OFF);

    i2c_client_->async_send_request(request,
                                    std::bind(&SSD1306Driver::handleI2cResponse,
                                              this, std::placeholders::_1));
}

void SSD1306Driver::setDefaultConfig()
{
    auto request = std::make_shared<custom_msgs::srv::I2cService::Request>();

    request->set__read_request(false);
    request->set__device_address(SSD1306_I2C_ADDR);

    request->write_data.push_back(SSD1306_COMM_CONTROL_BYTE); // ctrl byte
    request->write_data.push_back(SSD1306_COMM_DISPLAY_OFF);  // display on
    request->write_data.push_back(SSD1306_COMM_DISP_NORM);    // normal display
    request->write_data.push_back(SSD1306_COMM_CLK_SET);      // set clock div
    request->write_data.push_back(SSD1306_SCREEN_RATIO);      // ratio 0x80
    request->write_data.push_back(SSD1306_COMM_MULTIPLEX);    // set multiplex
    request->write_data.push_back(SSD1306_HEIGHT - 1);
    request->write_data.push_back(SSD1306_COMM_VERT_OFFSET); // offset
    request->write_data.push_back(0); // no horizontal offset
    request->write_data.push_back(SSD1306_COMM_START_LINE);
    request->write_data.push_back(SSD1306_COMM_CHARGE_PUMP);
    request->write_data.push_back(0x14); // turn on charge pump
    request->write_data.push_back(SSD1306_COMM_MEMORY_MODE);
    request->write_data.push_back(SSD1306_PAGE_MODE);
    request->write_data.push_back(SSD1306_COMM_HORIZ_NORM);
    request->write_data.push_back(SSD1306_COMM_SCAN_NORM);
    request->write_data.push_back(SSD1306_COMM_COM_PIN);
    request->write_data.push_back(0x02); // for display with 32 lines
    request->write_data.push_back(SSD1306_COMM_CONTRAST);
    request->write_data.push_back(0x7f); // default contrast value
    request->write_data.push_back(SSD1306_COMM_PRECHARGE);
    request->write_data.push_back(0xf1); // default precharge value
    request->write_data.push_back(SSD1306_COMM_DESELECT_LV);
    request->write_data.push_back(0x40); // default deselct val
    request->write_data.push_back(SSD1306_COMM_RESUME_RAM);
    request->write_data.push_back(SSD1306_COMM_DISP_NORM);
    request->write_data.push_back(SSD1306_COMM_DISPLAY_ON);
    request->write_data.push_back(SSD1306_COMM_DISABLE_SCROLL);

    i2c_client_->async_send_request(request,
                                    std::bind(&SSD1306Driver::handleI2cResponse,
                                              this, std::placeholders::_1));
}

void SSD1306Driver::setCursor(uint8_t x, uint8_t y)
{
    if (x >= SSD1306_WIDTH || y >= (SSD1306_HEIGHT / 8))
    {
        RCLCPP_ERROR(node_->get_logger(), "setting cursor");
        return;
    }

    auto request = std::make_shared<custom_msgs::srv::I2cService::Request>();

    request->set__read_request(false);
    request->set__device_address(SSD1306_I2C_ADDR);
    request->write_data.push_back(SSD1306_COMM_CONTROL_BYTE);
    request->write_data.push_back(SSD1306_COMM_PAGE_NUMBER | (y & 0x0f));
    request->write_data.push_back(SSD1306_COMM_LOW_COLUMN | (x & 0x0f));
    request->write_data.push_back(SSD1306_COMM_HIGH_COLUMN | ((x >> 4) & 0x0f));

    i2c_client_->async_send_request(request,
                                    std::bind(&SSD1306Driver::handleI2cResponse,
                                              this, std::placeholders::_1));
}

void SSD1306Driver::flipDisplay(uint8_t flip)
{
    auto request = std::make_shared<custom_msgs::srv::I2cService::Request>();

    request->set__read_request(false);
    request->set__device_address(SSD1306_I2C_ADDR);
    request->write_data.push_back(SSD1306_COMM_CONTROL_BYTE);
    if (!flip)
        request->write_data.push_back(SSD1306_COMM_HORIZ_NORM);
    else
        request->write_data.push_back(SSD1306_COMM_HORIZ_FLIP);

    i2c_client_->async_send_request(request,
                                    std::bind(&SSD1306Driver::handleI2cResponse,
                                              this, std::placeholders::_1));
}

void SSD1306Driver::rotateDisplay(uint8_t degree)
{
    auto request = std::make_shared<custom_msgs::srv::I2cService::Request>();

    request->set__read_request(false);
    request->set__device_address(SSD1306_I2C_ADDR);

    if (degree == 0)
    {
        request->write_data.push_back(SSD1306_COMM_CONTROL_BYTE);
        request->write_data.push_back(SSD1306_COMM_HORIZ_FLIP);
        request->write_data.push_back(SSD1306_COMM_SCAN_REVS);
    }
    else if (degree == 180)
    {
        request->write_data.push_back(SSD1306_COMM_CONTROL_BYTE);
        request->write_data.push_back(SSD1306_COMM_HORIZ_NORM);
        request->write_data.push_back(SSD1306_COMM_SCAN_NORM);
    }
}

void SSD1306Driver::clearRow(uint8_t row)
{
    if (row >= (SSD1306_HEIGHT / 8))
    {
        RCLCPP_ERROR(node_->get_logger(), "invalid row number");
        return;
    }

    auto request = std::make_shared<custom_msgs::srv::I2cService::Request>();

    setCursor(0, row);
    request->set__read_request(false);
    request->set__device_address(SSD1306_I2C_ADDR);

    request->write_data.push_back(SSD1306_DATA_CONTROL_BYTE);
    for (uint8_t i = 0; i < SSD1306_WIDTH; i++)
        request->write_data.push_back(0x00);

    i2c_client_->async_send_request(request,
                                    std::bind(&SSD1306Driver::handleI2cResponse,
                                              this, std::placeholders::_1));
}

void SSD1306Driver::clearScreen()
{
    for (uint8_t i = 0; i < (SSD1306_HEIGHT / 8); i++)
        clearRow(i);
}

void SSD1306Driver::writeString(uint8_t size, const std::string& msg)
{
    uint8_t* font_table = 0;
    uint8_t font_table_width = 0;

    if (size == SSD1306_FONT_SMALL)
    {
        font_table = (uint8_t*)font5x7;
        font_table_width = 5;
    }
    else if (size == SSD1306_FONT_NORMAL)
    {
        font_table = (uint8_t*)font8x8;
        font_table_width = 8;

        auto request =
            std::make_shared<custom_msgs::srv::I2cService::Request>();

        request->set__read_request(false);
        request->set__device_address(SSD1306_I2C_ADDR);
        request->write_data.push_back(SSD1306_DATA_CONTROL_BYTE);

        // font table range in ascii table is from 0x20(space) to 0x7e(~)
        for (char c : msg)
        {
            if (c < ' ' || c > '~')
                return;

            uint8_t* font_ptr = &font_table[(c - 0x20) * font_table_width];
            uint8_t j = 0;
            for (j = 0; j < font_table_width; j++)
                request->write_data.push_back(font_ptr[j]);

            if (size == SSD1306_FONT_SMALL)
                request->write_data.push_back(0x00);
        }

        i2c_client_->async_send_request(
            request, std::bind(&SSD1306Driver::handleI2cResponse, this,
                               std::placeholders::_1));
    }
}
