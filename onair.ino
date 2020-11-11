#include <Arduino.h>

#include "Adafruit_HT1632.h"

#define HT_DATA 2
#define HT_WR   3
#define HT_CS0  4
#define HT_CS1  5

#define MSG_PIN 6

Adafruit_HT1632LEDMatrix matrix = Adafruit_HT1632LEDMatrix(
    HT_DATA,
    HT_WR,
    HT_CS0,
    HT_CS1);

// *****************
// Program Constants
// *****************

#define HEIGHT matrix.height()
#define WIDTH matrix.width()

#define MSG_COUNT 6
char* messages[] = {
    "???",
    "THE BEST",
    "* LIVE *",
    "ON ZOOM",
    "OVER IT",
    "BUSY AF",
};

#ifndef HUMAN
#include "human.h"
#endif

// *************
// Program State
// *************

bool didSwitchMessage = false;
uint16_t messageIndex = 0;

// *****************
// Program Utilities
// *****************

void printHeader() {
    uint16_t length = strlen(HUMAN) + 3;
    uint16_t pixels = length * 5 + (length - 1);
    uint16_t indent = (WIDTH - pixels) / 2;

    matrix.setCursor(indent, 0);
    matrix.print(HUMAN);
    matrix.print(" is");
}

void printMessage() {
    char* message = messages[messageIndex];

    uint16_t length = strlen(message);
    uint16_t pixels = length * 5 + (length - 1);
    uint16_t indent = (WIDTH - pixels) / 2;

    matrix.setCursor(indent, HEIGHT - 7);
    matrix.print(message);
}

// ****************
// Program Handlers
// ****************

/**
 * Determine whether to switch the message and, if so, switch it.
 * This is called once per loop iteration.
 */
bool handleSwitchMessage() {
    int state = digitalRead(MSG_PIN);

    if (state == HIGH && !didSwitchMessage) {
        delay(50);
        didSwitchMessage = true;
        messageIndex += 1;
        if (messageIndex >= MSG_COUNT) {
            messageIndex = 0;
        }
        return true;
    }

    if (state == LOW && didSwitchMessage) {
        delay(50);
        didSwitchMessage = false;
    }

    return false;
}

// *********
// Main Loop
// *********

void setup() {
    matrix.begin(ADA_HT1632_COMMON_16NMOS);

    matrix.setBrightness(0);
    matrix.setTextWrap(false);
    matrix.clearScreen();

    printHeader();
    printMessage();
    matrix.writeScreen();
}

void loop() {
    bool shouldPrintMessage = handleSwitchMessage();

    if (shouldPrintMessage) {
        matrix.clearScreen();
        printHeader();
        printMessage();
        matrix.writeScreen();
    }
}

