#include "OledDisplayNode.hpp"
#include <rclcpp/logging.hpp>
#include <rclcpp/rclcpp.hpp>

int main(int argc, char** argv)
{
    rclcpp::init(argc, argv);
    auto node = std::make_shared<OledDisplayNode>();
    node->initSSD1306();

    rclcpp::spin(node);
    rclcpp::shutdown();
    return EXIT_SUCCESS;
}
