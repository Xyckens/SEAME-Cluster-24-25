#include "BatteryNode.hpp"
#include <rclcpp/logging.hpp>
#include <rclcpp/rclcpp.hpp>

using namespace std::chrono_literals;
int main(int argc, char** argv)
{
    rclcpp::init(argc, argv);
    auto node = std::make_shared<BatteryNode>();
    if (node->initINA219() != EXIT_SUCCESS)
        return EXIT_FAILURE;

    rclcpp::spin(node);
    rclcpp::shutdown();
    return EXIT_SUCCESS;
}
