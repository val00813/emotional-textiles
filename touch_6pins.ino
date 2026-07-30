#include "TouchyTouch.h"

// Settings for each touch sensor and its corresponding TouchDesigner function.
struct TouchConfig {
  uint8_t pin;
  const char* functionName;
  float minValue;
  float maxValue;
  uint8_t decimalPlaces;
};

const TouchConfig TOUCH_CONFIG[] = {
  {6, "color1", 1.00, 200.00, 0},
  {7, "color2", 1.00, 200.00, 0},
  {8, "color3", 1.00, 200.00, 0},
  {3, "color4", 1.00, 200.00, 0},
  {4, "color5", 1.00, 200.00, 0},
  {5, "color6", 1.00, 200.00, 0}
};

// The number of sensors is calculated automatically from the configuration above.
const uint8_t CONFIG_COUNT =
  sizeof(TOUCH_CONFIG) / sizeof(TOUCH_CONFIG[0]);

TouchyTouch touchSensors[CONFIG_COUNT];
unsigned long lastOutputTime[CONFIG_COUNT] = {0};

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(9600);

  // Wait for the serial connection before starting.
  while (!Serial) {
    delay(10);
  }

  // Use an unconnected analogue input to create less predictable values.
  randomSeed(analogRead(A0));

  // Initialise each touch sensor using its assigned pin.
  for (uint8_t i = 0; i < CONFIG_COUNT; i++) {
    touchSensors[i].begin(TOUCH_CONFIG[i].pin);
    delay(100);
  }
}

void loop() {
  for (uint8_t i = 0; i < CONFIG_COUNT; i++) {
    touchSensors[i].update();

    bool isTouched = touchSensors[i].touched();

    if (isTouched) {
      digitalWrite(LED_BUILTIN, HIGH);

      // Limit each sensor to one message every 200 milliseconds.
      if (millis() - lastOutputTime[i] > 200) {
        float parameterValue = generateParameter(
          TOUCH_CONFIG[i].minValue,
          TOUCH_CONFIG[i].maxValue,
          TOUCH_CONFIG[i].decimalPlaces
        );

        /*
          Send the data as:
          function name, pin number, parameter value

          Example:
          color1,6,125
        */
        Serial.print(TOUCH_CONFIG[i].functionName);
        Serial.print(",");
        Serial.print(TOUCH_CONFIG[i].pin);
        Serial.print(",");
        Serial.println(
          parameterValue,
          TOUCH_CONFIG[i].decimalPlaces
        );

        lastOutputTime[i] = millis();
      }
    }
  }

  // Turn the LED off once every sensor has been released.
  if (allPinsReleased()) {
    digitalWrite(LED_BUILTIN, LOW);
  }

  // A short delay helps prevent unstable or accidental triggers.
  delay(50);
}

// Generate a random value within the range assigned to a sensor.
float generateParameter(
  float minValue,
  float maxValue,
  uint8_t decimalPlaces
) {
  int scale = pow(10, decimalPlaces);
  int scaledMin = minValue * scale;
  int scaledMax = maxValue * scale;

  int randomValue = random(scaledMin, scaledMax + 1);

  return static_cast<float>(randomValue) / scale;
}

// Check whether all touch sensors have been released.
bool allPinsReleased() {
  for (uint8_t i = 0; i < CONFIG_COUNT; i++) {
    if (touchSensors[i].touched()) {
      return false;
    }
  }

  return true;
}
