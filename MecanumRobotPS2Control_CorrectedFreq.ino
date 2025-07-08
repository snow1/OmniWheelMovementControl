#include <PS2X_lib.h>  // PS2 Controller library

PS2X ps2x; // Create PS2 controller object

// Define PS2 pins
#define PS2_DAT 50
#define PS2_CMD 51
#define PS2_CLK 52
#define PS2_ATT 53 

#define PWMA 11    // Front Left Motor Speed
#define DIRA1 34
#define DIRA2 35  // Front Left Motor Direction
#define PWMB 7    // Front Right Motor Speed
#define DIRB1 37
#define DIRB2 36  // Front Right Motor Direction
#define PWMC 6   // Rear Left Motor Speed
#define DIRC1 43
#define DIRC2 42  // Rear Left Motor Direction
#define PWMD 4    // Rear Right Motor Speed
#define DIRD1 A4
#define DIRD2 A5  // Rear Right Motor Direction

// Speed control (PWM range 0-255)
#define SPEED 30  // Slow speed

void setup() {
    Serial.begin(9600);
  // Set Timer2 (Pin 11) and Timer4 (Pins 6, 7) to ~31kHz for quieter operation
  TCCR2B = TCCR2B & 0b11111000 | 0x01;
  TCCR4B = TCCR4B & 0b11111000 | 0x01;
    delay(2000); // Allow PS2 controller to fully power up

    Serial.println("Attempting to connect to PS2 Controller...");

    int error = -1;

    // Try multiple times to connect
    for (int i = 0; i < 5; i++) {  
        error = ps2x.config_gamepad(PS2_CLK, PS2_CMD, PS2_ATT, PS2_DAT, false, false); // Disable pressure & rumble
        if (error == 0) break; // Exit loop if connected
        Serial.println("Retrying...");
        delay(1000);  // Wait 1 second before retrying
    }

    if (error == 0) {
        Serial.println("✅ PS2 Controller Connected Successfully!");
    } else {
        Serial.println("❌ PS2 Controller Failed to Connect.");
        while(1); // Stop execution if no controller
    }

   
    pinMode(PWMA, OUTPUT);
    pinMode(DIRA1, OUTPUT);
    pinMode(DIRA2, OUTPUT);
    pinMode(PWMB, OUTPUT);
    pinMode(DIRB1, OUTPUT);
    pinMode(DIRB2, OUTPUT);
    pinMode(PWMC, OUTPUT);
    pinMode(DIRC1, OUTPUT);
    pinMode(DIRC2, OUTPUT);
    pinMode(PWMD, OUTPUT);
    pinMode(DIRD1, OUTPUT);
    pinMode(DIRD2, OUTPUT);
    
    stopMotors(); // Ensure motors are stopped initially
}

void loop() {
    ps2x.read_gamepad(false, false); // Read PS2 data

    if (ps2x.Button(PSB_PAD_UP)) {
        Serial.println("Moving Forward");
        moveRight();
    } else if (ps2x.Button(PSB_PAD_DOWN)) {
        Serial.println("Moving Backward");
        moveLeft();
    } else if (ps2x.Button(PSB_PAD_LEFT)) {
        Serial.println("Moving Left");
        moveBackward();
    } else if (ps2x.Button(PSB_PAD_RIGHT)) {
        Serial.println("Moving Right");
        moveForward();
    } else if (ps2x.Button(PSB_L1)) {
        Serial.println("Rotating Clockwise");
        rotateCounterclockwise();
    } else if (ps2x.Button(PSB_R1)) {
        Serial.println("Rotating Counterclockwise");
        rotateClockwise();
    } else {
        stopMotors();
    }

    delay(100);
}


void stopMotors() {
    analogWrite(PWMA, 0);
    analogWrite(PWMB, 0);
    analogWrite(PWMC, 0);
    analogWrite(PWMD, 0);
    digitalWrite(DIRA1, LOW);
    digitalWrite(DIRA2, LOW);
    digitalWrite(DIRB1, LOW);
    digitalWrite(DIRB2, LOW);
    digitalWrite(DIRC1, LOW);
    digitalWrite(DIRC2, LOW);
    digitalWrite(DIRD1, LOW);
    digitalWrite(DIRD2, LOW);
}

void rotationRight() {
    digitalWrite(DIRA1, LOW); digitalWrite(DIRA2, HIGH);
    digitalWrite(DIRB1, HIGH); digitalWrite(DIRB2, LOW);
    digitalWrite(DIRC1, LOW); digitalWrite(DIRC2, HIGH);
    digitalWrite(DIRD1, HIGH); digitalWrite(DIRD2, LOW);
    analogWrite(PWMA, SPEED);
    analogWrite(PWMB, SPEED);
    analogWrite(PWMC, SPEED);
    analogWrite(PWMD, SPEED);
}


void rotationLeft() {
    digitalWrite(DIRA1, HIGH); digitalWrite(DIRA2, LOW);
    digitalWrite(DIRB1, LOW); digitalWrite(DIRB2, HIGH);
    digitalWrite(DIRC1, HIGH); digitalWrite(DIRC2, LOW);
    digitalWrite(DIRD1, LOW); digitalWrite(DIRD2, HIGH);
    analogWrite(PWMA, SPEED);
    analogWrite(PWMB, SPEED);
    analogWrite(PWMC, SPEED);
    analogWrite(PWMD, SPEED);
}


// Function to move left
void moveRight() {
    digitalWrite(DIRA1, HIGH); digitalWrite(DIRA2, LOW);  // Front Left moves backward
    digitalWrite(DIRB1, LOW); digitalWrite(DIRB2, HIGH);  // Front Right moves forward
    digitalWrite(DIRC1, LOW); digitalWrite(DIRC2, HIGH);  // Rear Left moves forward
    digitalWrite(DIRD1, HIGH); digitalWrite(DIRD2, LOW);  // Rear Right moves backward
    analogWrite(PWMA, SPEED);
    analogWrite(PWMB, SPEED);
    analogWrite(PWMC, SPEED);
    analogWrite(PWMD, SPEED);
}

// Function to move right (Strafing Right) - FIXED
void moveForward() {
    digitalWrite(DIRA1, HIGH); digitalWrite(DIRA2, LOW);  // Front Left moves forward
    digitalWrite(DIRB1, HIGH); digitalWrite(DIRB2, LOW);  // Front Right moves backward
    digitalWrite(DIRC1, LOW); digitalWrite(DIRC2, HIGH);  // Rear Left moves backward
    digitalWrite(DIRD1, LOW); digitalWrite(DIRD2, HIGH);  // Rear Right moves forward
    analogWrite(PWMA, SPEED);
    analogWrite(PWMB, SPEED);
    analogWrite(PWMC, SPEED);
    analogWrite(PWMD, SPEED);
}


// Function to move left
void moveBackward() {
    digitalWrite(DIRA1, LOW); digitalWrite(DIRA2, HIGH);  // Front Left moves backward
    digitalWrite(DIRB1, LOW); digitalWrite(DIRB2, HIGH);  // Front Right moves forward
    digitalWrite(DIRC1, HIGH); digitalWrite(DIRC2, LOW);  // Rear Left moves forward
    digitalWrite(DIRD1, HIGH); digitalWrite(DIRD2, LOW);  // Rear Right moves backward
    analogWrite(PWMA, SPEED);
    analogWrite(PWMB, SPEED);
    analogWrite(PWMC, SPEED);
    analogWrite(PWMD, SPEED);
}

// Function to move right (Strafing Right) - FIXED
void moveLeft() {
    digitalWrite(DIRA1, LOW); digitalWrite(DIRA2, HIGH);  // Front Left moves forward
    digitalWrite(DIRB1, HIGH); digitalWrite(DIRB2, LOW);  // Front Right moves backward
    digitalWrite(DIRC1, HIGH); digitalWrite(DIRC2, LOW);  // Rear Left moves backward
    digitalWrite(DIRD1, LOW); digitalWrite(DIRD2, HIGH);  // Rear Right moves forward
    analogWrite(PWMA, SPEED);
    analogWrite(PWMB, SPEED);
    analogWrite(PWMC, SPEED);
    analogWrite(PWMD, SPEED);
}


void rotateClockwise() {
    digitalWrite(DIRA1, HIGH); digitalWrite(DIRA2, LOW);
    digitalWrite(DIRB1, HIGH); digitalWrite(DIRB2, LOW);
    digitalWrite(DIRC1, HIGH); digitalWrite(DIRC2, LOW);
    digitalWrite(DIRD1, HIGH); digitalWrite(DIRD2, LOW);
    analogWrite(PWMA, SPEED);
    analogWrite(PWMB, SPEED);
    analogWrite(PWMC, SPEED);
    analogWrite(PWMD, SPEED);
}


void rotateCounterclockwise() {
    digitalWrite(DIRA1, LOW); digitalWrite(DIRA2, HIGH);
    digitalWrite(DIRB1, LOW); digitalWrite(DIRB2, HIGH);
    digitalWrite(DIRC1, LOW); digitalWrite(DIRC2, HIGH);
    digitalWrite(DIRD1, LOW); digitalWrite(DIRD2, HIGH);
    analogWrite(PWMA, SPEED);
    analogWrite(PWMB, SPEED);
    analogWrite(PWMC, SPEED);
    analogWrite(PWMD, SPEED);
}
