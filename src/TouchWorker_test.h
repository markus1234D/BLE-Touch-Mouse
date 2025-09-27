#include <Arduino.h>
#include <Wire.h>
#include <cst816t.h>
#include <map>
#include "pin_config.h"


class TouchWorker_test {

public:
    enum Rotation {
        USB_UP = 0,
        USB_DOWN = 1,
        USB_LEFT = 2,
        USB_RIGHT = 3
    };

public:
    void init();
    void setRotation(int rotation) { this->rotation = rotation; }
    void setMaxGestureTime(unsigned long time) { maxGestureTime = time; }

    void handleTouch();
    
private:

    int rotation = USB_DOWN;
    cst816t touchpad = cst816t(Wire, PIN_TOUCH_RES, PIN_TOUCH_INT); //begin of Wire is in Libra

    uint16_t x = 0;
    uint16_t y = 0;
    uint16_t last_x = 0;
    uint16_t last_y = 0;
    u_int16_t rawX = 0;
    u_int16_t rawY = 0;
    unsigned long maxGestureTime = 1000;
    uint16_t gestureX = 0;
    uint16_t gestureY = 0;  
    unsigned long last_millis = 0; 
    bool swipe_read = false;
    bool gesture_timeout = true;
    bool clicked = false;
    bool doubleClicked = false;
    bool doubleLongPress = false;
    bool available = false;

    uint16_t xMin = 0;
    uint16_t xMax = 170;
    uint16_t yMin = 0;
    uint16_t yMax = 320;
    uint16_t real_xMin = 7;
    uint16_t real_xMax = 154;
    uint16_t real_yMin = 7;
    uint16_t real_yMax = 312;

private:
    void setXY(uint16_t x, uint16_t y);
    std::map<String, std::function<void(int, int)>> callbacks;
    std::function<void(int, int)> gestureCallbackBuffer = NULL;
    void checkGesture();
    void debugPrint(String str);
};

void TouchWorker_test::setXY(uint16_t x, uint16_t y) {
    rawX = x;
    rawY = y;
    switch (rotation) {
    case USB_UP:
        this->x = xMax - x;
        this->y = yMax - y;
        break;
    case USB_DOWN:
        this->x = x;
        this->y = y;
        break;
    case USB_LEFT:
        this->x = yMax - y;
        this->y = x;
        break;
    case USB_RIGHT:
        this->x = y;
        this->y = xMax - x;
        break;
    default:
        Serial.println("Unknown rotation");
        break;
    }
}

void TouchWorker_test::init() {
    touchpad.begin(mode_touch);

    this->callbacks["swipeLeft"] = NULL;
    this->callbacks["swipeRight"] = NULL;
    this->callbacks["swipeUp"] = NULL;
    this->callbacks["swipeDown"] = NULL;
    this->callbacks["singleClick"] = NULL;
    this->callbacks["singleClickRelease"] = NULL;
    this->callbacks["singleClickHold"] = NULL;
    this->callbacks["doubleClick"] = NULL;
    this->callbacks["doubleClickRelease"] = NULL;
    this->callbacks["doubleClickHold"] = NULL;
    this->callbacks["longPress"] = NULL;
    this->callbacks["longPressRelease"] = NULL;
    this->callbacks["noGesture"] = NULL;
}

void TouchWorker_test::checkGesture(){
    uint8_t gesture_id = touchpad.gesture_id;

    switch (gesture_id) {
    case GESTURE_NONE:
        // Serial.println("NONE");
        break;
    case GESTURE_SWIPE_DOWN:
        // Serial.println("SWIPE DOWN");
        swipe_read = true;

        switch (rotation)
        {
        case USB_DOWN :
            if(callbacks["swipeUp"] != NULL){
                gestureCallbackBuffer = callbacks["swipeUp"];
                gestureX = this->x;
                gestureY = this->y;
            }
            debugPrint("Swipe Up detected");
            break;
        case USB_UP :
            if(callbacks["swipeDown"] != NULL){
                gestureCallbackBuffer = callbacks["swipeDown"];
                gestureX = this->x;
                gestureY = this->y;
            }
            debugPrint("Swipe Down detected");
            break;
        case USB_LEFT:
            if(callbacks["swipeRight"] != NULL){
                gestureCallbackBuffer = callbacks["swipeRight"];
                gestureX = this->x;
                gestureY = this->y;
            }
            debugPrint("Swipe Right detected");
            break;
        case USB_RIGHT:
            if(callbacks["swipeLeft"] != NULL){
                gestureCallbackBuffer = callbacks["swipeLeft"];
                gestureX = this->x;
                gestureY = this->y;
            }
            debugPrint("Swipe Left detected");
            break;
        default:
            break;
        }
        break;
    case GESTURE_SWIPE_UP:
        // Serial.println("SWIPE UP");
        swipe_read = true;
        switch (rotation)
        {
        case USB_DOWN :
            if(callbacks["swipeUp"] != NULL){
                gestureCallbackBuffer = callbacks["swipeUp"];
                gestureX = this->x;
                gestureY = this->y;
            }
            debugPrint("Swipe Up detected");
            break;
        case USB_UP :
            if(callbacks["swipeDown"] != NULL){
                gestureCallbackBuffer = callbacks["swipeDown"];
                gestureX = this->x;
                gestureY = this->y;
            }
            debugPrint("Swipe Down detected");
            break;
        case USB_LEFT:
            if(callbacks["swipeLeft"] != NULL){
                gestureCallbackBuffer = callbacks["swipeLeft"];
                gestureX = this->x;
                gestureY = this->y;
            }
            debugPrint("Swipe Left detected");
            break;
        case USB_RIGHT:
            if(callbacks["swipeRight"] != NULL){
                gestureCallbackBuffer = callbacks["swipeRight"];
                gestureX = this->x;
                gestureY = this->y;
            }
            debugPrint("Swipe Right detected");
            break;
        default:
            break;
        }
        break;
    case GESTURE_SWIPE_LEFT:             
        // Serial.println("SWIPE LEFT");
        swipe_read = true;
        switch (rotation)
        {
        case USB_DOWN :
            if(callbacks["swipeLeft"] != NULL){
                gestureCallbackBuffer = callbacks["swipeLeft"];
                gestureX = this->x;
                gestureY = this->y;
            }
            debugPrint("Swipe Left detected");
            break;
        case USB_UP :
            if(callbacks["swipeRight"] != NULL){
                gestureCallbackBuffer = callbacks["swipeRight"];
                gestureX = this->x;
                gestureY = this->y;
            }
            debugPrint("Swipe Right detected");
            break;
        case USB_LEFT:
            if(callbacks["swipeUp"] != NULL){
                gestureCallbackBuffer = callbacks["swipeUp"];
                gestureX = this->x;
                gestureY = this->y;
            }
            debugPrint("Swipe Up detected");
            break;
        case USB_RIGHT:
            if(callbacks["swipeDown"] != NULL){
                gestureCallbackBuffer = callbacks["swipeDown"];
                gestureX = this->x;
                gestureY = this->y;
            }
            debugPrint("Swipe Down detected");
            break;
        default:
            break;
        }
        break;
    case GESTURE_SWIPE_RIGHT:
        // Serial.println("SWIPE RIGHT");
        swipe_read = true;
        switch (rotation)
        {
        case USB_DOWN :
            if(callbacks["swipeRight"] != NULL){
                gestureCallbackBuffer = callbacks["swipeRight"];
                gestureX = this->x;
                gestureY = this->y;
            }
            debugPrint("Swipe Right detected");
            break;
        case USB_UP :
            if(callbacks["swipeLeft"] != NULL){
                gestureCallbackBuffer = callbacks["swipeLeft"];
                gestureX = this->x;
                gestureY = this->y;
            }
            debugPrint("Swipe Left detected");
            break;
        case USB_LEFT:
            if(callbacks["swipeDown"] != NULL){
                gestureCallbackBuffer = callbacks["swipeDown"];
                gestureX = this->x;
                gestureY = this->y;
            }
            debugPrint("Swipe Down detected");
            break;
        case USB_RIGHT:
            if(callbacks["swipeUp"] != NULL){
                gestureCallbackBuffer = callbacks["swipeUp"];
                gestureX = this->x;
                gestureY = this->y;
            }
            debugPrint("Swipe Up detected");
            break;
        default:
            break;
        }
        break;
    case GESTURE_LONG_PRESS:
        // Serial.println("LONG PRESS");
        if(callbacks["longPress"] != NULL){
            gestureCallbackBuffer = callbacks["longPress"];
            gestureX = this->x;
            gestureY = this->y;
        }
        break;
    default:
        debugPrint("?");
        break;
    }
}

void TouchWorker_test::debugPrint(String str) {
    Serial.println("[TW]: " + str);
}


void TouchWorker_test::handleTouch() {
    
    if(millis() - last_millis > maxGestureTime && !gesture_timeout){
        // only enter once
        debugPrint("Gesture timeout");
        gesture_timeout = true;

        if(clicked && available) {
            debugPrint("doublclick and hold detected");
            doubleLongPress = true;

        } else if (!clicked && available){
            debugPrint("touch and hold");
        }
        
        clicked = false;

        if(!clicked && !doubleLongPress && !available){
            checkGesture();
        }
    }
    
    if (touchpad.available()){
        available = true;
        // debugPrint("Touchpad available");
        setXY(touchpad.x, touchpad.y);
        // debugPrint("X: " + String(this->x) + ", Y: " + String(this->y));

        if(last_x == 0 && last_y == 0){
            // touch detected
            if(!clicked){
                // first touch
                debugPrint("First touch detected");
                // start to count gesture time
                last_millis = millis();
                gesture_timeout = false;
            } else {
                debugPrint("second touch detected");
                doubleLongPress = true;
            }
        }

        if(gesture_timeout){
            debugPrint("X: " + String(this->x) + ", Y: " + String(this->y));
        }

        last_x = this->x;
        last_y = this->y;

    } else { // touchpad not available
        available = false;
        // debugPrint("Touchpad not available");
        if (last_x != 0 || last_y != 0) {
            // touch released
            // debugPrint("Touch released");

            if(gesture_timeout){
                // checkGesture();
                if (doubleLongPress) {
                    // Handle double long press
                    debugPrint("Double long press released after timeout");
                    doubleLongPress = false;
                } else{
                    debugPrint("touch released after timeout");
                }

            } else { // not timeout
                if(doubleLongPress) {
                    debugPrint("Double long press released before timeout");
                    doubleLongPress = false;
                } else {
                    debugPrint("Touch released before timeout");
                    clicked = true;
                }
                
            }



            last_x = 0;
            last_y = 0;
        }
    }
}