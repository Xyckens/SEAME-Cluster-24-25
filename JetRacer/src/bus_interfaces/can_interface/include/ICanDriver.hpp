#pragma once

#include <CanDriver.hpp>

class ICanDriver
{
    public:
        virtual ~ICanDriver() = default;
        virtual ssize_t sendMessage(const sockcanpp::CanMessage& msg,
                                    bool force_extended = false) = 0;
        virtual sockcanpp::CanMessage readMessage() = 0;
        virtual bool waitForMessages(sockcanpp::milliseconds timeout) = 0;
};
