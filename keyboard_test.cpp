#include <Arduino.h>
#include <BleCombo.h>


void setup() {
    Serial.begin(115200);
    Keyboard.begin();
}

void loop() {
    if (Keyboard.isConnected()) {
        Serial.println("BLE Keyboard connected");
        // Serial.println("Sending 'hello' over BLE Keyboard");
        // bleKeyboard.print("hello");
        Serial.println("Sending 'INSERT' key over BLE Keyboard");
        // Keyboard.print("hello");
        Keyboard.press(KEY_LEFT_CTRL);
        Keyboard.print('v'); // Use write instead of press for paste
        delay(100);
        Keyboard.releaseAll();
        delay(10000); // Wait 10 seconds before sending again
    } else {
        Serial.println("BLE Keyboard not connected");
    }
    delay(1000);
}