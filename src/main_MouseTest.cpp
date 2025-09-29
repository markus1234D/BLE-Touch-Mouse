#include <Arduino.h>
// #include <CST816t_TouchWorker.h>
#include <MouseWorker.h>
// #include "GuiWorker.h"
#include "touchworker_test.h"

TouchWorker_test touchWorker;
// CST816t_TouchWorker touchWorker;
MouseWorker mouseWorker;
// GuiWorker guiWorker;

void setup() {
    // Initialize serial communication at 115200 baud
    Serial.begin(115200);
    // Wait for serial port to connect (for native USB devices)
    while (!Serial) {
        ; // wait
    }

    touchWorker.init();
    mouseWorker.init();
    // guiWorker.init();

    // guiWorker.onFunctionChange([](String item, String action, String function) {
    //     Serial.println("Function changed: " + item + " " + action + " " + function);
    //     if (item == "screen" ){
    //         bool result = touchWorker.setActionFunction(action, mouseWorker.getFunction(function));
    //         if (result) {
    //             Serial.println("Action function set successfully");
    //         } else {
    //             Serial.println("Failed to set action function");
    //         }
    //     }
    //     // else if (item.startsWith("button")) {
    //     //     buttonWorker.setBtnActionFunction(item, action, mouseWorker.getFunction(function));
    //     // }
    // });

    touchWorker.setRotation(TouchWorker_test::USB_UP); // Set initial rotation to USB_DOWN
    mouseWorker.setMode(MouseWorker::JOYSTICK_MODE); // Set initial mode to JOYSTICK_MODE
    touchWorker.setMaxGestureTime(300); // Set max gesture time to 800ms

    touchWorker.setActionFunction("singleClickRelease", std::function<void(int, int)>([](int x, int y) {
        mouseWorker.getFunction("LeftClick")(x, y);
    }));
    touchWorker.setActionFunction("enterScreen", std::function<void(int, int)>([](int x, int y) {
        Serial.println("Enter screen at X: " + String(x) + ", Y: " + String(y));
        mouseWorker.getFunction("setOrigin")(x, y);
    }));
    touchWorker.setActionFunction("leaveScreen", std::function<void(int, int)>([](int x, int y) {
        Serial.println("Leave screen at X: " + String(x) + ", Y: " + String(y));
        mouseWorker.getFunction("resetOrigin")(x, y);
    }));
    touchWorker.setActionFunction("doubleClickHold", std::function<void(int, int)>([](int x, int y) {
        Serial.println("Double click and hold at X: " + String(x) + ", Y: " + String(y));
        mouseWorker.getFunction("pressLeft")(x, y);
    }));
    touchWorker.setActionFunction("doubleClickHoldRelease", std::function<void(int, int)>([](int x, int y) {
        Serial.println("Double click and hold release at X: " + String(x) + ", Y: " + String(y));
        mouseWorker.getFunction("releaseLeft")(x, y);
    }));
    touchWorker.setActionFunction("noGesture", std::function<void(int, int)>([](int x, int y) {
        mouseWorker.getFunction("move")(x, y);
    }));

}

void loop() {
    // guiWorker.handleGui();
    touchWorker.handleTouch();
    delay(20); // Small delay to avoid overwhelming the loop
}