#ifndef _BITLING_H_
#define _BITLING_H_

#include <Arduino.h>

typedef struct LEDConfig {
  uint8_t pin;
  unsigned long dutyCycle;
} LEDConfig;

typedef struct SensorConfig {
  uint8_t pin;
  unsigned long dutyCycle;
} SensorConfig;

class ArdyObject {
  public:
    virtual void setup();
    virtual void update(float fDelta);
};

class Timer : public ArdyObject {
  private:
    float fTimeRemaining;
  public:
    Timer(float _fTimeRemaining = 0.0f) : fTimeRemaining(_fTimeRemaining) {};
    void setup () {};
    void update (float fDelta) {
      fTimeRemaining -= fDelta;
    };
};

class LEDController : public ArdyObject {
  private:
    LEDConfig config;
  public:
    LEDController(LEDConfig _config) : config(_config) {};
    void setup ();
    void update (float fDelta);
};

class PressureSensor : public ArdyObject {
  private:
    SensorConfig config;
  public:
    PressureSensor(SensorConfig _config) : config(_config) {};
    void setup ();
    void update (float fDelta);
};

class Bitling : public ArdyObject {
  private:
    PressureSensor ps;
    LEDController lc;
  public:
    Bitling(PressureSensor _ps, LEDController _lc) : ps(_ps), lc(_lc) {};
    void setup ();
    void update (float fDelta);
};

#endif
