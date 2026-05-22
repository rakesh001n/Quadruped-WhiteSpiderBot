// PCA9685 — 8 Servo Sweep (Limited to 10° – 160°)
// Library: Adafruit PWM Servo Driver Library

#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

Adafruit_PWMServoDriver pca = Adafruit_PWMServoDriver(0x40);

#define SERVO_MIN_US  500    // pulse width at 0°
#define SERVO_MAX_US  2500   // pulse width at 180°
#define SERVO_FREQ    50     // 50 Hz

#define SWEEP_MIN     10     // minimum angle limit
#define SWEEP_MAX     160    // maximum angle limit
#define NUM_SERVOS    8      // channels 0 to 7
#define STEP_DELAY    10     // ms per degree (lower = faster)

uint16_t angleToPulse(float angle) {
  // Hard clamp to safe limits
  if (angle < SWEEP_MIN) angle = SWEEP_MIN;
  if (angle > SWEEP_MAX) angle = SWEEP_MAX;
  float us = map(angle, 0, 180, SERVO_MIN_US, SERVO_MAX_US);
  float ticksPerUs = 4096.0f / (1000000.0f / SERVO_FREQ);
  return (uint16_t)(us * ticksPerUs);
}

void setServoAngle(uint8_t ch, float angle) {
  pca.setPWM(ch, 0, angleToPulse(angle));
}

void setup() {
  Serial.begin(9600);
  Wire.begin();
  pca.begin();
  pca.setPWMFreq(SERVO_FREQ);

  Serial.println("Sweep limited to 10° – 160°");

  // Start all servos at minimum position
  for (uint8_t i = 0; i < NUM_SERVOS; i++) setServoAngle(i, SWEEP_MIN);
  delay(500);
}

void loop() {
  // Sweep 10° → 160°
  Serial.println("Sweeping 10° → 160°");
  for (float angle = SWEEP_MIN; angle <= SWEEP_MAX; angle += 1.0f) {
    for (uint8_t i = 0; i < NUM_SERVOS; i++) setServoAngle(i, angle);
    delay(STEP_DELAY);
  }

  delay(300); // pause at 160°

  // Sweep 160° → 10°
  Serial.println("Sweeping 160° → 10°");
  for (float angle = SWEEP_MAX; angle >= SWEEP_MIN; angle -= 1.0f) {
    for (uint8_t i = 0; i < NUM_SERVOS; i++) setServoAngle(i, angle);
    delay(STEP_DELAY);
  }

  delay(300); // pause at 10°
}
