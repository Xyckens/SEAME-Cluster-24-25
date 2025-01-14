#include <custom_msgs/msg/display.hpp>
#include <geometry_msgs/msg/twist.hpp>
#include <rclcpp/rclcpp.hpp>
#include <std_msgs/msg/u_int8.hpp>

using namespace std::chrono_literals;

class TesterNode : public rclcpp::Node
{
    public:
        TesterNode() : Node("tester_node")
        {
            // Custom QoS profile for low latency and reliable delivery
            rclcpp::QoS qos_profile = rclcpp::QoS(rclcpp::KeepLast(10))
                                          .reliable()
                                          .durability_volatile();

            timer_servo = this->create_timer(
                1s, std::bind(&TesterNode::twistCallback, this));
            timer_display = this->create_timer(
                1s, std::bind(&TesterNode::displayCallback, this));

            twist_publisher_ =
                this->create_publisher<geometry_msgs::msg::Twist>("cmd_vel",
                                                                  qos_profile);
            display_publisher_ =
                this->create_publisher<custom_msgs::msg::Display>("cmd_display",
                                                                  qos_profile);

            RCLCPP_INFO(this->get_logger(), "starting test all peripherals");
        }

    private:
        rclcpp::TimerBase::SharedPtr timer_display;
        rclcpp::TimerBase::SharedPtr timer_servo;
        rclcpp::TimerBase::SharedPtr timer_dc_motors;

        float curr_linear_x = -1.0;
        float curr_angular_z = -1.0;

        rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr
            twist_publisher_;
        rclcpp::Publisher<custom_msgs::msg::Display>::SharedPtr
            display_publisher_;

        void displayCallback()
        {
            auto msg = custom_msgs::msg::Display();

            msg.line1 = "Hello world";
            msg.line2 = "this is a test";
            msg.line3 = "test";
            msg.line4 = "test";
            display_publisher_->publish(msg);
        }

        void twistCallback()
        {
            auto msg = geometry_msgs::msg::Twist();
            msg.linear.x = curr_linear_x;
            msg.angular.z = curr_angular_z;

            curr_angular_z += 0.1;
            curr_linear_x += 0.1;

            if (curr_linear_x > 1 && curr_angular_z > 1)
            {
                curr_angular_z = -1.0;
                curr_linear_x = -1.0;
            }

            twist_publisher_->publish(msg);
        }
};

int main(int argc, char** argv)
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<TesterNode>());
    rclcpp::shutdown();
    return 0;
}
