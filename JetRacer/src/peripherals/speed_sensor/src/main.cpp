#include <rclcpp/logging.hpp>
#include <rclcpp/rclcpp.hpp>

using namespace std::chrono_literals;
int main(int argc, char** argv)
{
    rclcpp::init(argc, argv);
    // rclcpp::spin(std::make_shared<ServoNode>());
    // rclcpp::shutdown();
    return 0;
}
