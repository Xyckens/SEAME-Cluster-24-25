#pragma once

#include <CanDriver.hpp>
#include <ICanDriver.hpp>
#include <gmock/gmock.h>

class MockCanDriver : public ICanDriver
{
    public:
        MockCanDriver() {}
        MOCK_METHOD(ssize_t, sendMessage,
                    (const sockcanpp::CanMessage& msg, bool force_extended),
                    (override));
        MOCK_METHOD(sockcanpp::CanMessage, readMessage, (), (override));
        MOCK_METHOD(bool, waitForMessages, (sockcanpp::milliseconds timeout),
                    (override));

        ~MockCanDriver() override{};
};
