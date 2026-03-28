#include <AFMotor.h>
#include <NewPing.h>
#include <Servo.h>

// ---------------- PIN DEFINITIONS ----------------
#define TRIG A0
#define ECHO A1

#define IR_RIGHT A2
#define IR_LEFT  A3

#define MAX_DISTANCE 200
#define HAND_DISTANCE 30

// ---------------- OBJECTS ----------------
NewPing sonar(TRIG, ECHO, MAX_DISTANCE);
Servo scanServo;

// Motors
AF_DCMotor m1(1); // Front Left
AF_DCMotor m2(2); // Back Left
AF_DCMotor m3(3); // Back Right
AF_DCMotor m4(4); // Front Right

// ---------------- SETUP ----------------
void setup() {
  pinMode(IR_LEFT, INPUT);
  pinMode(IR_RIGHT, INPUT);

  // ---------------- SERVO SWEEP (INITIAL CALIBRATION) ----------------
  scanServo.attach(10);

  for (int pos = 0; pos <= 180; pos += 2) {
    scanServo.write(pos);
    delay(10);
  }

  for (int pos = 180; pos >= 0; pos -= 2) {
    scanServo.write(pos);
    delay(10);
  }

  scanServo.write(90); // Center position

  // ---------------- MOTOR SPEED ----------------
  m1.setSpeed(200);
  m2.setSpeed(200);
  m3.setSpeed(200);
  m4.setSpeed(200);

  stopCar();
}

// ---------------- LOOP ----------------
void loop() {
  int leftIR = digitalRead(IR_LEFT);
  int rightIR = digitalRead(IR_RIGHT);
  int distance = sonar.ping_cm();

  // Priority: Direction first
  if (leftIR == LOW) {
    turnLeft();
    return;
  }

  if (rightIR == LOW) {
    turnRight();
    return;
  }

  // Forward movement
  if (distance > 0 && distance < HAND_DISTANCE) {
    moveForward();
  } else {
    stopCar();
  }
}

// ---------------- MOVEMENTS ----------------

void moveForward() {
  m1.run(BACKWARD);
  m2.run(FORWARD);
  m3.run(FORWARD);
  m4.run(BACKWARD);
}

void turnLeft() {
  m1.run(BACKWARD);
  m2.run(FORWARD);
  m3.run(BACKWARD);
  m4.run(FORWARD);
}

void turnRight() {
  m1.run(FORWARD);
  m2.run(BACKWARD);
  m3.run(FORWARD);
  m4.run(BACKWARD);
}

void stopCar() {
  m1.run(RELEASE);
  m2.run(RELEASE);
  m3.run(RELEASE);
  m4.run(RELEASE);
}