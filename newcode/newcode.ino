#define X_STEP 26
#define X_DIR  27
#define Y_STEP 14
#define Y_DIR  12
#define Z_STEP 25
#define Z_DIR  35
#define A_STEP 32
#define A_DIR  15
#define EN_PIN 13  // Enable pin (active LOW)

void setup() {
    pinMode(X_STEP, OUTPUT);
    pinMode(X_DIR, OUTPUT);
    pinMode(Y_STEP, OUTPUT);
    pinMode(Y_DIR, OUTPUT);
    pinMode(Z_STEP, OUTPUT);
    pinMode(Z_DIR, OUTPUT);
    pinMode(A_STEP, OUTPUT);
    pinMode(A_DIR, OUTPUT);
    pinMode(EN_PIN, OUTPUT);
    
    digitalWrite(EN_PIN, LOW);  // Enable all drivers
}

void loop() {
    testMotor(X_STEP, X_DIR);  // Test X motor
    testMotor(Y_STEP, Y_DIR);  // Test Y motor
    testMotor(Z_STEP, Z_DIR);  // Test Z motor
    testMotor(A_STEP, A_DIR);  // Test A motor
}

void testMotor(int stepPin, int dirPin) {
    digitalWrite(dirPin, HIGH);  // Move forward
    for (int i = 0; i < 200; i++) {  // 200 steps for 1 rotation (adjust if needed)
        digitalWrite(stepPin, HIGH);
        delayMicroseconds(1000);  // Step pulse width (1ms)
        digitalWrite(stepPin, LOW);
        delayMicroseconds(1000);
    }
    delay(1000);

    digitalWrite(dirPin, LOW);  // Move backward
    for (int i = 0; i < 200; i++) {
        digitalWrite(stepPin, HIGH);
        delayMicroseconds(1000);
        digitalWrite(stepPin, LOW);
        delayMicroseconds(1000);
    }
    delay(1000);
}