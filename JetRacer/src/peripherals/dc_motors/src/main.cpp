#include "DcMotorsNode.hpp"
#include <rclcpp/logging.hpp>
#include <rclcpp/rclcpp.hpp>

using namespace std::chrono_literals;
int main(int argc, char** argv)
{
    rclcpp::init(argc, argv);
    auto node = std::make_shared<DcMotorsNode>();
    node->initPCA9685();

    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}
