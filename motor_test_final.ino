#include <AFMotor.h>
//start on left side of the track
const int IR_PIN = A0; 

AF_DCMotor motor1(1); // Left Motor
AF_DCMotor motor2(2); // Right Motor

// --- Speed Tuning Variables ---
const int speedFast = 180;  
const int speedSlow = 80;   
const int speedSearch = 130; // Medium speed for finding the line

// Track if the robot has successfully found the line for the first time
bool hasFoundLineYet = false;

void setup() {
  pinMode(IR_PIN, INPUT); 
  Serial.begin(9600);     
  
  motor1.run(FORWARD);
  motor2.run(FORWARD);
}

void loop() {
  int sensorState = digitalRead(IR_PIN); 

  // PHASE 1: INITIAL SEARCH (Runs only until the first time the line is touched)
  if (!hasFoundLineYet) {
    if (sensorState == LOW) { 
      // Found the line! Switch out of search mode permanently
      hasFoundLineYet = true; 
      Serial.println("Line Found! Entering zig-zag mode...");
    } 
    else {
      // Still looking for the line: Drive forward/right to sweep into it
      Serial.println("Searching for line...");
      motor1.setSpeed(speedSearch);
      motor2.setSpeed(speedSearch - 30); // Slight right curve to seek the edge
      return; // Skip the rest of the loop and keep searching
    }
  }

  // PHASE 2: ZIG-ZAG TRACKING (Only runs after the line has been found once)
  if (sensorState == LOW) {
    Serial.println("Line Detected! -> Veering Right");
    motor1.setSpeed(speedFast);
    motor2.setSpeed(speedSlow);
  } 
  else {
    Serial.println("Clear (No Line) -> Veering Left");
    motor1.setSpeed(speedSlow);
    motor2.setSpeed(speedFast);
  }
  
  delay(50); 
}
