#include "Bitling.h"

LEDConfig lcfg = {
  A1,
  1000,
};

LEDController lc(lcfg);

int BITLING_0 = A1;
int BITLING_0_DS = 0;
int BITLING_0_SENSOR_PIN = A2;
int BITLING_0_SENSOR_VALUE;
const int BITLING_0_SENSOR_VALUE_SAMPLES = 32;
int BITLING_0_SENSOR_VALUES[BITLING_0_SENSOR_VALUE_SAMPLES];
int BITLING_0_SENSOR_VALUES_CURRENT = 0;
int BITLING_0_SENSOR_VALUES_COUNT = 0;
int BITLING_0_SENSOR_THRESHOLD = 60000; // TODO: Make a moving average.

const int animation_pin_count = 3;
int ANIMATION_PINS[animation_pin_count];
int ANIMATION_TIME = 1000;
int animation_current_time = 0;
int animation_current_pin = 0;

bool state = true;

const int PULSE_TIME = 100;   // milliseconds
const int READ_RESOLUTION = 16; // bits
float SENSOR_THRESHOLD = 1.1f;

unsigned long lastTime = 0;

void setup()
{
  Serial.begin(9600);
  pinMode(BITLING_0, OUTPUT);

  ANIMATION_PINS[0] = A3;
  ANIMATION_PINS[1] = A4;
  ANIMATION_PINS[2] = A5;
  
  pinMode(ANIMATION_PINS[0], OUTPUT);
  pinMode(ANIMATION_PINS[1], OUTPUT);
  pinMode(ANIMATION_PINS[2], OUTPUT);

  analogReadResolution(READ_RESOLUTION);
  lastTime = micros();
}

void loop()
{
  unsigned long currentTime = micros();
  unsigned long delta = currentTime - lastTime;
  float deltaTime = delta / 1000000.0f;
  lastTime = currentTime;

  BITLING_0_SENSOR_VALUE = analogRead(BITLING_0_SENSOR_PIN);
  // Serial.print(BITLING_0_SENSOR_VALUE);

  BITLING_0_SENSOR_VALUES[BITLING_0_SENSOR_VALUES_CURRENT] = BITLING_0_SENSOR_VALUE;
  BITLING_0_SENSOR_VALUES_CURRENT = (BITLING_0_SENSOR_VALUES_CURRENT + 1) % BITLING_0_SENSOR_VALUE_SAMPLES;
  BITLING_0_SENSOR_VALUES_COUNT = min(BITLING_0_SENSOR_VALUES_COUNT + 1, BITLING_0_SENSOR_VALUE_SAMPLES);

  int sum = 0;

  for (int i = 0; i < BITLING_0_SENSOR_VALUES_COUNT; i++) {
    sum = sum + BITLING_0_SENSOR_VALUES[i];
  }

  int threshold = int((sum / BITLING_0_SENSOR_VALUES_COUNT) * SENSOR_THRESHOLD);
  
  animation_current_time += delta;
  if (animation_current_time > ANIMATION_TIME) {
    digitalWrite(ANIMATION_PINS[animation_current_pin], LOW);
    animation_current_time = 0;
    animation_current_pin = (animation_current_pin + 1) % animation_pin_count;
  }

  Serial.println(animation_current_pin);
  // Serial.print(animation_current_pin);

  digitalWrite(ANIMATION_PINS[animation_current_pin], HIGH);

  // float phase = (millis() % PULSE_TIME) / (float) PULSE_TIME;

  if (BITLING_0_SENSOR_VALUE > threshold) {
    // Serial.println('threshold hit');
    Serial.print(', ');
    Serial.print(threshold);
    state = false;
  }

  BITLING_0_DS = BITLING_0_DS + ((state ? 255 : -255) * deltaTime);

  if (BITLING_0_DS > 255) {
    BITLING_0_DS = 255;
    state = false;
  }

  if (BITLING_0_DS < 0) {
    BITLING_0_DS = 0;
    state = true;
  }

  Serial.println();
  analogWrite(BITLING_0, BITLING_0_DS);
  delay(PULSE_TIME);// - delta / 1000);
}

