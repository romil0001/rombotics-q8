#include <ESP32Servo.h>

Servo myServo;  // Create a servo object

const int servoPin = 14;  // Define the pin for the servo

void setup() {
    myServo.attach(servoPin);  // Attach the servo to pin 14
}

void loop() {
    // Move servo from 0 to 180 degrees
    for (int pos = 0; pos <= 180; pos += 5) {
        myServo.write(pos);
        delay(20);  // Small delay to let the servo move
    }
    
    // Move servo from 180 to 0 degrees
    for (int pos = 180; pos >= 0; pos -= 5) {
        myServo.write(pos);
        delay(20);
    }
}
