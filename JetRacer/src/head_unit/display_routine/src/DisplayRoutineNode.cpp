#include "DisplayRoutineNode.hpp"
#include <arpa/inet.h>
#include <ifaddrs.h>
#include <netdb.h>
#include <netinet/in.h>
#include <string>

using namespace std::chrono_literals;

DisplayRoutineNode::DisplayRoutineNode() : rclcpp::Node("display_routine_node")
{
    timer_ = this->create_wall_timer(
        60s, std::bind(&DisplayRoutineNode::updateDisplay, this));
    publisher_ = this->create_publisher<custom_msgs::msg::Display>(
        "cmd_display", NODE_QOS);
    battery_subscriber_ = this->create_subscription<std_msgs::msg::Float64>(
        "battery_percentage", NODE_QOS,
        [this](std_msgs::msg::Float64 battery_level) {
            battery_level_ =
                std::to_string(static_cast<int>(battery_level.data));
        });

    auto addresses = getIPv4Addresses();
    if (!addresses.empty())
        ip_address_ = addresses.at(0);

    updateDisplay();
}

/**
 * @brief Update the display with the current IP address and battery level.
 *
 * this callback is called every 60 seconds.
 */
void DisplayRoutineNode::updateDisplay() const
{
    auto msg = custom_msgs::msg::Display();

    msg.line1 = "ip: " + ip_address_;
    msg.line3 = "bat: " + battery_level_ + "%";
    publisher_->publish(msg);
}

/**
 * @brief Get the IPv4 addresses of the network interfaces.
 *
 * @return a vector of strings containing the IPv4 addresses.
 */
std::vector<std::string> DisplayRoutineNode::getIPv4Addresses() const
{
    std::vector<std::string> ip_addresses;

    struct ifaddrs* ifaddr;
    if (getifaddrs(&ifaddr) == -1)
    {
        RCLCPP_ERROR(this->get_logger(), "Fail to get network interfaces: %s",
                     std::string(std::strerror(errno)).c_str());
    }

    // Use a smart pointer to automatically free the linked list
    std::unique_ptr<struct ifaddrs, decltype(&freeifaddrs)> ifaddr_guard(
        ifaddr, freeifaddrs);

    // Iterate through the linked list of interfaces
    for (struct ifaddrs* ifa = ifaddr; ifa != nullptr; ifa = ifa->ifa_next)
    {
        if (ifa->ifa_addr == nullptr)
        {
            continue;
        }

        // Only consider IPv4 addresses
        if (ifa->ifa_addr->sa_family == AF_INET)
        {
            auto ipv4 = reinterpret_cast<struct sockaddr_in*>(ifa->ifa_addr);
            char ip[INET_ADDRSTRLEN];
            inet_ntop(AF_INET, &ipv4->sin_addr, ip, INET_ADDRSTRLEN);

            if (std::string(ip) != "127.0.0.1")
                ip_addresses.emplace_back(ip);
        }
    }

    return ip_addresses;
}
