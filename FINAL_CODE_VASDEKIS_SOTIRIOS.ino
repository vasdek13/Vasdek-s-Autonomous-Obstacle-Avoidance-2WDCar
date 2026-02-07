/**
 * Project: Autonomous Obstacle Avoidance Robot (Lafvin 2WD Kit)
 * Description: Control logic using Ultrasonic Sensor (mounted on Servo) 
 * and IR Obstacle sensors. Features audio warning via Buzzer.
 * * Hardware:
 * - Arduino UNO & V5 Sensor Shield
 * - L298N Motor Driver
 * - HC-SR04 Ultrasonic Sensor + SG90 Servo Motor
 * - 2x IR Obstacle Avoidance Sensors
 * - Active Buzzer
 */

#include <Servo.h> // Library for Servo Motor control

// --- PIN DEFINITIONS ---

// Ultrasonic Sensor (HC-SR04)
const int trigPin = 9;
const int echoPin = 10;

// Servo Motor (SG90)
const int servoPin = 11; // Connect Servo Signal to Pin 11

// IR Obstacle/Edge Sensors
const int irLeft = 7;
const int irRight = 8;

// Motor Driver L298N Pins
// Note: Ensure ENA/ENB jumpers are in place on L298N for full speed
const int motorLeftFwd = 5;   // IN1
const int motorLeftBack = 6;  // IN2
const int motorRightFwd = 3;  // IN3
const int motorRightBack = 4; // IN4

// Indicators
const int buzzer = 13; // Connect Buzzer to Pin 13

// --- GLOBAL VARIABLES ---
Servo headServo;       // Create servo object
long duration;
int distance;
const int safetyThreshold = 25; // Distance in cm to trigger avoidance

void setup() {
  // Initialize Serial Monitor for debugging
  Serial.begin(9600);

  // Initialize Servo
  headServo.attach(servoPin);
  headServo.write(76); // Set Servo to center position (looking forward)
  delay(500);

  // Initialize Motor Pins
  pinMode(motorLeftFwd, OUTPUT);
  pinMode(motorLeftBack, OUTPUT);
  pinMode(motorRightFwd, OUTPUT);
  pinMode(motorRightBack, OUTPUT);

  // Initialize Sensor Pins
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(irLeft, INPUT);
  pinMode(irRight, INPUT);

  // Initialize Buzzer
  pinMode(buzzer, OUTPUT);
  
  // Test Buzzer on startup
  tone(buzzer, 2000);
  delay(100);
  noTone(buzzer);
}

void loop() {
  // 1. READ SENSORS
  distance = getDistance();
  
  // Read IR sensors (LOW usually means obstacle detected for these modules)
  int leftCliff = digitalRead(irLeft);
  int rightCliff = digitalRead(irRight);

  // 2. LOGIC DECISION
  // If obstacle is close OR IR sensors detect something (e.g., edge/wall)
  if (distance < safetyThreshold || leftCliff == LOW || rightCliff == LOW) {
    stopRobot();
    triggerAlarm(); // Sound the alarm
    avoidObstacleSequence(); // Start the avoidance maneuver
  } else {
    moveForward(); // Path is clear
  }
  
  delay(10); // Short delay for stability
}

// --- HELPER FUNCTIONS ---

/**
 * Calculates distance using HC-SR04
 */
int getDistance() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(5);
  digitalWrite(trigPin, LOW);
  
  duration = pulseIn(echoPin, HIGH);
  return duration * 0.034 / 2;
}

/**
 * Complex Avoidance Maneuver with Servo Scanning
 */
void avoidObstacleSequence() {
  // 1. Stop and Look Around
  stopRobot();
  
  // Scan Right
  headServo.write(16); 
  delay(400);
  
  // Scan Left
  headServo.write(136);
  delay(400);
  
  // Look Forward again
  headServo.write(76);
  delay(300);
  
  // 2. Move Backward to clear the obstacle
  moveBackward();
  delay(500); 
  
  // 3. Turn Right to change direction
  turnRight();
  delay(100);
  
  stopRobot();
  delay(600);
}

void moveForward() {
  noTone(buzzer); 
  // Αντιστροφή: Το Back γίνεται HIGH και το Fwd γίνεται LOW
  digitalWrite(motorLeftFwd, LOW);
  analogWrite(motorLeftBack, 215);
  digitalWrite(motorRightFwd, LOW);
  analogWrite(motorRightBack, 150);
}

void moveBackward() {
  // Αντιστροφή: Το Fwd γίνεται HIGH και το Back γίνεται LOW
  analogWrite(motorLeftFwd, 215);
  digitalWrite(motorLeftBack, LOW);
  analogWrite(motorRightFwd, 150);
  digitalWrite(motorRightBack, LOW);
}

void turnRight() {
  // Differential steering: Left wheels FWD, Right wheels BACK
  analogWrite(motorLeftFwd, 215);
  digitalWrite(motorLeftBack, LOW);
  digitalWrite(motorRightFwd, LOW);
  analogWrite(motorRightBack, 150);
}

void stopRobot() {
  digitalWrite(motorLeftFwd, LOW);
  digitalWrite(motorLeftBack, LOW);
  digitalWrite(motorRightFwd, LOW);
  digitalWrite(motorRightBack, LOW);
}

void triggerAlarm() {
  // Beep at 1000Hz
  tone(buzzer, 1000);
}