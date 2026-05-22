// PCA9685 — Move All 8 Servos to 90° (Center Position)
// Useful for calibration, homing, or neutral position
// Library: Adafruit PWM Servo Driver Library

#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

Adafruit_PWMServoDriver pca = Adafruit_PWMServoDriver(0x40);

#define SERVO_MIN_US  500    // pulse width at 0°
#define SERVO_MAX_US  2500   // pulse width at 180°
#define SERVO_FREQ    50     // 50 Hz
#define NUM_SERVOS    8      // channels 0 to 7
#define CENTER_ANGLE  90     // target angle

uint16_t angleToPulse(float angle) {
  if (angle < 0)   angle = 0;
  if (angle > 180) angle = 180;
  float us = map(angle, 0, 180, SERVO_MIN_US, SERVO_MAX_US);
  float ticksPerUs = 4096.0f / (1000000.0f / SERVO_FREQ);
  return (uint16_t)(us * ticksPerUs);
}

void setServoAngle(uint8_t ch, float angle) {
  pca.setPWM(ch, 0, angleToPulse(angle));
  Serial.print("  Servo "); Serial.print(ch + 1);
  Serial.print(" → "); Serial.print(angle); Serial.println("°");
}

void setup() {
  Serial.begin(9600);
  Wire.begin();
  pca.begin();
  pca.setPWMFreq(SERVO_FREQ);

  Serial.println("Moving all 8 servos to 90° (center)...");

  for (uint8_t i = 0; i < NUM_SERVOS; i++) {
    setServoAngle(i, CENTER_ANGLE);
    delay(100); // small delay between each servo for stable movement
  }

  Serial.println("Done. All servos at 90°.");
}

void loop() {
  // Nothing — servos hold at 90° indefinitely
  // To change the target angle, edit CENTER_ANGLE above
}
