#include <Arduino.h>

void setup() {
    Serial.begin(115200);
    Serial.println("Hello, Arduino!");
}

void loop() {
    Serial.println("Running loop... 📁📁📁");
    delay(1000);
}
