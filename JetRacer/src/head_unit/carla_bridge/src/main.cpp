#include "../include/CarlaBridgeNode.hpp"
#include <rclcpp/rclcpp.hpp>

int main(int argc, char** argv)
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<CarlaBridgeNode>());
    rclcpp::shutdown();
    return 0;
}
