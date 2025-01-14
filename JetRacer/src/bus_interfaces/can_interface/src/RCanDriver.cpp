#include "RCanDriver.hpp"

RCanDriver::RCanDriver(const std::string& name, int can_mode)
    : driver_(std::make_unique<sockcanpp::CanDriver>(name, can_mode))
{
}

ssize_t RCanDriver::sendMessage(const sockcanpp::CanMessage& msg,
                                bool force_extended)
{
    return driver_->sendMessage(msg, force_extended);
}

sockcanpp::CanMessage RCanDriver::readMessage()
{
    return driver_->readMessage();
}

bool RCanDriver::waitForMessages(sockcanpp::milliseconds timeout)
{
    return driver_->waitForMessages(timeout);
}
