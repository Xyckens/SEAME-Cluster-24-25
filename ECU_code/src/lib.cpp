#include "lib.hpp"
#include "FreeRTOSVariant.h"
#include "can.h"
#include "portable.h"
#include "projdefs.h"
#include <Arduino.h>
#include <ArduinoLog.h>
#include <Arduino_FreeRTOS.h>
#include <lib.hpp>
#include <mcp2515.h>

MCP2515 mcp(9); // specify the pin for chip select
t_state STATE{IDLE, OFF};
struct can_frame frame;
volatile uint16_t rotations = 0;

void incrementSpeedCount() { rotations++; }

void errorHalt()
{
    while (true)
    {
        digitalWrite(LEFT_BLINKER_PIN, HIGH);
        digitalWrite(RIGHT_BLINKER_PIN, HIGH);
        delay(200);
        digitalWrite(LEFT_BLINKER_PIN, LOW);
        digitalWrite(RIGHT_BLINKER_PIN, LOW);
        delay(200);
    }
}

void handleCanFrames(void* pvParameters)
{
    while (true)
    {
        // Retry reading CAN message
        MCP2515::ERROR result = mcp.readMessage(&frame);
        if (result == MCP2515::ERROR_OK)
        {
            // Process CAN frame
            switch (frame.can_id)
            {
            case 0x100:
                updateBlinkerState(frame.data[0]);
                break;
            case 0x300:
                sendSpeedReadings();
                break;
            default:
                Log.warningln("Unrecognized CAN frame ID: 0x%X", frame.can_id);
            }
        }
        vTaskDelay(pdMS_TO_TICKS(20));
    }
}

void updateBlinkerState(uint8_t cmd)
{
    switch (cmd)
    {
    case 0:
        STATE.blinker_state = IDLE;
        break;
    case 1:
        STATE.blinker_state = TURN_LEFT;
        break;
    case 2:
        STATE.blinker_state = TURN_RIGHT;
        break;
    case 3:
        STATE.blinker_state = WARNINGS;
        break;
    default:
        Log.warningln("Urecognised blinker state: 0x%X", cmd);
    }
    Log.verboseln("blinkers state updated to: %d", cmd);
}

void blinkBlinkers(void* pvParameters)
{
    while (true)
    {
        switch (STATE.blinker_state)
        {
        case IDLE:
            break;
        case TURN_LEFT:
            digitalWrite(LEFT_BLINKER_PIN, HIGH);
            vTaskDelay(pdMS_TO_TICKS(BLINK_DELAY));
            digitalWrite(LEFT_BLINKER_PIN, LOW);
            vTaskDelay(pdMS_TO_TICKS(BLINK_DELAY));
            break;
        case TURN_RIGHT:
            digitalWrite(RIGHT_BLINKER_PIN, HIGH);
            vTaskDelay(pdMS_TO_TICKS(BLINK_DELAY));
            digitalWrite(RIGHT_BLINKER_PIN, LOW);
            vTaskDelay(pdMS_TO_TICKS(BLINK_DELAY));
            break;
        case WARNINGS:
            digitalWrite(RIGHT_BLINKER_PIN, HIGH);
            digitalWrite(LEFT_BLINKER_PIN, HIGH);
            vTaskDelay(pdMS_TO_TICKS(BLINK_DELAY));
            digitalWrite(RIGHT_BLINKER_PIN, LOW);
            digitalWrite(LEFT_BLINKER_PIN, LOW);
            vTaskDelay(pdMS_TO_TICKS(BLINK_DELAY));
            break;
        case FULL:
            digitalWrite(RIGHT_BLINKER_PIN, HIGH);
            digitalWrite(LEFT_BLINKER_PIN, HIGH);
        }
        vTaskDelay(pdMS_TO_TICKS(10));
    }
}

void sendSpeedReadings()
{
    frame.can_id = 0x200;

    noInterrupts();
    uint16_t speed_reading = rotations;
    rotations = 0;
    interrupts();

    frame.data[0] = speed_reading & 0xFF;
    frame.data[1] = speed_reading >> 8;

    Log.verbose("rotations: %d\n", speed_reading);
    mcp.sendMessage(&frame);
}
