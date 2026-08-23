#include <Wire.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <OneWire.h>
#include <DallasTemperature.h>

#define ONE_WIRE_BUS 4

Adafruit_MPU6050 mpu;

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature temperatureSensor(&oneWire);

void setup() {
  Serial.begin(115200);

  Wire.begin();

  if (!mpu.begin()) {
    Serial.println("MPU6050 not found");
    while (1) {
      delay(10);
    }
  }

  temperatureSensor.begin();

  Serial.println(
    "Temperature,Accel_X,Accel_Y,Accel_Z,Vibration"
  );

  delay(1000);
}

void loop() {

  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  temperatureSensor.requestTemperatures();
  float motorTemperature =
      temperatureSensor.getTempCByIndex(0);

  float ax = a.acceleration.x;
  float ay = a.acceleration.y;
  float az = a.acceleration.z;

  // Calculate vibration magnitude
  float vibration =
      sqrt(ax * ax + ay * ay + az * az);

  // Send CSV data
  Serial.print(motorTemperature);
  Serial.print(",");

  Serial.print(ax);
  Serial.print(",");

  Serial.print(ay);
  Serial.print(",");

  Serial.print(az);
  Serial.print(",");

  Serial.println(vibration);

  delay(1000);
}
