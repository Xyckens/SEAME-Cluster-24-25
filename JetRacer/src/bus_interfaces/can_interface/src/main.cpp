#include "CanInterface.hpp"
#include <rclcpp/rclcpp.hpp>

int main(int argc, char** argv)
{
    rclcpp::init(argc, argv);
    try
    {
        auto node = std::make_shared<CanInterface>();
        rclcpp::spin(std::make_shared<CanInterface>());
    }
    catch (const std::exception& e)
    {
    }
    rclcpp::shutdown();
    return 0;
}