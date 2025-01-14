#include "CanInterface.hpp"
#include <rclcpp/rclcpp.hpp>

int main(int argc, char** argv)
{
    rclcpp::init(argc, argv);
    try
    {
        rclcpp::spin(std::make_shared<CanInterface>());
        rclcpp::shutdown();
    }
    catch (const std::exception& e)
    {
    }
    return EXIT_SUCCESS;
}
