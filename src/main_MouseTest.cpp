#include <Arduino.h>
#include <CST816t_TouchWorker.h>
#include <MouseWorker.h>
#include "GuiWorker.h"


CST816t_TouchWorker touchWorker;
MouseWorker mouseWorker;
GuiWorker guiWorker;

void setup() {
    // Initialize serial communication at 115200 baud
    Serial.begin(115200);
    // Wait for serial port to connect (for native USB devices)
    while (!Serial) {
        ; // wait
    }

    touchWorker.init();
    mouseWorker.init();
    guiWorker.init();

    guiWorker.onFunctionChange([](String item, String action, String function) {
        Serial.println("Function changed: " + item + " " + action + " " + function);
        if (item == "screen" ){
            bool result = touchWorker.setActionFunction(action, mouseWorker.getFunction(function));
            if (result) {
                Serial.println("Action function set successfully");
            } else {
                Serial.println("Failed to set action function");
            }
        }
        // else if (item.startsWith("button")) {
        //     buttonWorker.setBtnActionFunction(item, action, mouseWorker.getFunction(function));
        // }
    });

    touchWorker.setRotation(CST816t_TouchWorker::USB_UP); // Set initial rotation to USB_DOWN
    mouseWorker.setMode(MouseWorker::JOYSTICK_MODE); // Set initial mode to JOYSTICK_MODE
    
    bool success = false;
    success = touchWorker.setActionFunction("singleClick", mouseWorker.getFunction("setOrigin"));
    if (!success) {
        Serial.println("Failed to set action function for singleClick");
    }

    success = touchWorker.setActionFunction("singleClickRelease", std::function<void(int, int)>([](int x, int y) {
        Serial.println("MouseWorker: Single click release action triggered at X: " + String(x) + ", Y: " + String(y));
    //     mouseWorker.getFunction("click")(x, y);
        mouseWorker.getFunction("resetOrigin")(x, y);
    }));
    if (!success) {
        Serial.println("Failed to set action function for singleClickRelease");
    }

    // success = touchWorker.setActionFunction("doubleClick", mouseWorker.getFunction("setOrigin"));
    // if (!success) {
    //     Serial.println("Failed to set action function for doubleClick");
    // }

    // success = touchWorker.setActionFunction("doubleClickRelease", std::function<void(int, int)>([](int x, int y) {
    //     Serial.println("MouseWorker: Double click release action triggered at X: " + String(x) + ", Y: " + String(y));
    //     mouseWorker.getFunction("click")(x, y);
    //     mouseWorker.getFunction("resetOrigin")(x, y);
    // }));
    // if (!success) {
    //     Serial.println("Failed to set action function for doubleClickRelease");
    // }

    success = touchWorker.setActionFunction("noGesture", mouseWorker.getFunction("move"));
    if (!success) {
        Serial.println("Failed to set action function for noGesture");
    }

    // // New action functions for drag and drop actions
    // success = touchWorker.setActionFunction("doubleClickHold", mouseWorker.getFunction("pressLeft"));
    // if (!success) {
    //     Serial.println("Failed to set action function for doubleClickHold");
    // }

    // success = touchWorker.setActionFunction("doubleClickRelease", mouseWorker.getFunction("releaseLeft"));
    // if (!success) {
    //     Serial.println("Failed to set action function for doubleClickRelease");
    // }
    // Serial.println("BLE Touch Mouse Test - Setup Complete");
}

void loop() {
    guiWorker.handleGui();
    touchWorker.handleTouch();
    delay(20); // Small delay to avoid overwhelming the loop
}