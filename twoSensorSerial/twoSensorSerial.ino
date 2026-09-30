//Sensor variables
int sensor1Pin = A1;
int sensor1Value;

int sensor2Pin = A2;
int sensor2Value;

int led1Pin = A5;
int led2Pin = A0;
int led3Pin = 27;

const float PERIOD_1 = 3100.0; // ~3.1 seconds per cycle
const float PERIOD_2 = 4300.0; // ~4.3 seconds per cycle
const float PERIOD_3 = 5700.0; // ~5.7 seconds per cycle

const float PHASE_1 = 0.0;
const float PHASE_2 = 2.094;
const float PHASE_3 = 4.188;

const int YELLOW[3] = { 255, 255,   8 };
const int PINK[3]   = { 255,  8, 255 };

const int   PULSE_TIME = 100;   // milliseconds for one full yellow -> pink -> yellow cycle
const float BRIGHTNESS = 1.0;   // 0.0-1.0, kept low so it is easy on the eyes

void setup()
{
  Serial.begin(9600);

  pinMode(led1Pin, OUTPUT);
  pinMode(led2Pin, OUTPUT);
  pinMode(led3Pin, OUTPUT);

  // On the Feather ESP32 V2 the NeoPixel only gets power when pin 2 is HIGH
  pinMode(NEOPIXEL_I2C_POWER, OUTPUT);
  digitalWrite(NEOPIXEL_I2C_POWER, HIGH);

  // Serial.println("FeatherTest: hello from your Feather ESP32 V2!");
  // Serial.println("The NeoPixel should be pulsing between yellow and pink.");
}

void loop()
{
  unsigned long timeStep = millis();

  //read both sensors (0 - 4095)
  sensor1Value = analogRead(sensor1Pin);
  sensor2Value = analogRead(sensor2Pin);

  //print them on one line
  Serial.print(sensor1Value);
  Serial.print(", ");
  Serial.println(sensor2Value);

  // Where are we in the cycle? 0.0 at the start, 1.0 at the end, then it repeats.
  float phase = (millis() % PULSE_TIME) / (float) PULSE_TIME;
  // // Serial.print("   Phase: ");
  // // Serial.println(phase);

  float farFactor = (float)sensor1Value / 4095.0;       // High sensor1 -> closer to 1.0
  float closeFactor = 1.0 - ((float)sensor2Value / 4095.0); // High sensor2 -> closer to 0.0

  float maxBrightness = constrain(farFactor * closeFactor, 0.0, BRIGHTNESS);

  float breath1 = 0.5 * (1.0 + sin((2.0 * PI * timeStep / PERIOD_1) + PHASE_1));
  float breath2 = 0.5 * (1.0 + sin((2.0 * PI * timeStep / PERIOD_2) + PHASE_2));
  float breath3 = 0.5 * (1.0 + sin((2.0 * PI * timeStep / PERIOD_3) + PHASE_3));

  int pwm1 = (int)(breath1 * maxBrightness * 255.0);
  int pwm2 = (int)(breath2 * maxBrightness * 255.0);
  int pwm3 = (int)(breath3 * maxBrightness * 255.0);

  analogWrite(led1Pin, constrain(pwm1, 0, 255));
  analogWrite(led2Pin, constrain(pwm2, 0, 255));
  analogWrite(led3Pin, constrain(pwm3, 0, 255));

  // Turn that into a smooth 0 -> 1 -> 0 wave (cosine gives an easy in-and-out).
  float mix = (1.0 - cos(phase * 2.0 * PI)) / 2.0;   // 0 = yellow, 1 = pink

  // Blend each colour channel between yellow and pink, then dim it
  int r = (YELLOW[0] + (PINK[0] - YELLOW[0]) * mix) * BRIGHTNESS / phase;
  int g = (YELLOW[1] + (PINK[1] - YELLOW[1]) * mix) * BRIGHTNESS / phase;
  int b = (YELLOW[2] + (PINK[2] - YELLOW[2]) * mix) * BRIGHTNESS / phase;

  // RGB_BUILTIN is the on-board NeoPixel (already defined for this board)
  rgbLedWrite(RGB_BUILTIN, r, g, b);

  delay(PULSE_TIME / 10); //short pause so the monitor is readable
}
