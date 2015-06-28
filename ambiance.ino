#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP085_U.h>

int photocellPin = 0;     // the cell and 10K pulldown are connected to a0
int photocellReading;     // the analog reading from the analog resistor divider
Adafruit_BMP085_Unified bmp = Adafruit_BMP085_Unified(10085);

void setup(void) {
  Serial.begin(9600);

  /* Initialise bmp085/180 sensor */
  if(!bmp.begin())
  {
    Serial.print("No BMP085/180 detected");
    while(1);
  }
}

void loop(void) {
  getData();
  delay(1000);
}

void getData() {
  /* Get a new sensor event */
  sensors_event_t event;
  bmp.getEvent(&event);
  
  getPhotocell();
  
  if (event.pressure) {
    Serial.print("Pressure: "); Serial.print(getPressure(event)); Serial.println("hPa");
    Serial.print("Temperature: "); Serial.print(getTemperature(event)); Serial.println(" C");
    Serial.print("Altitude: "); Serial.print(getAltitude(event, getTemperature(event))); Serial.println("m");
    Serial.println("======================================");
  }
}


float getPressure(sensors_event_t event) {
  return event.pressure;
}

float getAltitude(sensors_event_t event, float temperature) {
  float seaLevelPressure = SENSORS_PRESSURE_SEALEVELHPA; // default mean pressure at sea level; can replace with current local value
  return bmp.pressureToAltitude(seaLevelPressure, event.pressure, temperature);
}

float getTemperature(sensors_event_t event) {
  float temperature;
  bmp.getTemperature(&temperature);
  return temperature;
}

void getPhotocell() {
  photocellReading = analogRead(photocellPin);

  Serial.print("Ambient Light: "); Serial.print(photocellReading);     // the raw analog reading

  if (photocellReading < 10) {
    Serial.println(" - Very Bright");
  } else if (photocellReading < 200) {
    Serial.println(" - Bright");
  } else if (photocellReading < 500) {
    Serial.println(" - Light");
  } else if (photocellReading < 800) {
    Serial.println(" - Dim");
  } else {
    Serial.println(" - Dark");
  }
}


