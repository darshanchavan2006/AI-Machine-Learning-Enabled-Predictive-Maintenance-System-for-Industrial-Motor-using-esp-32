/*
  AI-Enabled Predictive Maintenance System
  ESP32 + MPU6050 + DS18B20 + MQ-2

  Sensors:
  MPU6050 -> Vibration
  DS18B20 -> Temperature
  MQ-2    -> Gas level

  Output:
  NORMAL / WARNING / FAULT

  NOTE:
  This version uses threshold-based classification.
  It is a hardware prototype for collecting data.
  A trained ML/TinyML model can be added later.
*/

#include <Wire.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <math.h>

// ---------------- PIN DEFINITIONS ----------------

#define SDA_PIN 21
#define SCL_PIN 22

#define TEMP_PIN 4
#define GAS_PIN 34

// LEDs
#define GREEN_LED 25
#define YELLOW_LED 26
#define RED_LED 27

// Buzzer
#define BUZZER 14

// ---------------- SENSOR OBJECTS ----------------

Adafruit_MPU6050 mpu;

OneWire oneWire(TEMP_PIN);
DallasTemperature tempSensor(&oneWire);

// ---------------- THRESHOLDS ----------------

// Temperature
const float TEMP_WARNING = 60.0;
const float TEMP_FAULT = 75.0;

// Vibration RMS
const float VIB_WARNING = 2.0;
const float VIB_FAULT = 4.0;

// MQ-2 raw ADC values
// These values MUST be calibrated for your sensor.
const int GAS_WARNING = 1800;
const int GAS_FAULT = 2800;

// Sampling interval
const unsigned long SAMPLE_INTERVAL = 2000;

unsigned long previousMillis = 0;


// =================================================
// VIBRATION MEASUREMENT
// =================================================

float calculateVibrationRMS(int samples = 100)
{
  double sumSquares = 0;

  for (int i = 0; i < samples; i++)
  {
    sensors_event_t accel;
    sensors_event_t gyro;
    sensors_event_t temp;

    mpu.getEvent(&accel, &gyro, &temp);

    // Calculate total acceleration
    float magnitude = sqrt(
      accel.acceleration.x * accel.acceleration.x +
      accel.acceleration.y * accel.acceleration.y +
      accel.acceleration.z * accel.acceleration.z
    );

    // Remove approximate gravity
    float vibration = magnitude - 9.81;

    sumSquares += vibration * vibration;

    delay(5);
  }

  return sqrt(sumSquares / samples);
}


// =================================================
// NORMAL STATUS
// =================================================

void normalStatus()
{
  digitalWrite(GREEN_LED, HIGH);
  digitalWrite(YELLOW_LED, LOW);
  digitalWrite(RED_LED, LOW);

  noTone(BUZZER);
}


// =================================================
// WARNING STATUS
// =================================================

void warningStatus()
{
  digitalWrite(GREEN_LED, LOW);
  digitalWrite(YELLOW_LED, HIGH);
  digitalWrite(RED_LED, LOW);

  tone(BUZZER, 1200, 150);
}


// =================================================
// FAULT STATUS
// =================================================

void faultStatus()
{
  digitalWrite(GREEN_LED, LOW);
  digitalWrite(YELLOW_LED, LOW);
  digitalWrite(RED_LED, HIGH);

  tone(BUZZER, 2000, 500);
}


// =================================================
// MOTOR CONDITION CLASSIFICATION
// =================================================

String classifyMotor(
  float temperature,
  float vibration,
  int gasValue
)
{
  // Fault condition
  if (
    temperature >= TEMP_FAULT ||
    vibration >= VIB_FAULT ||
    gasValue >= GAS_FAULT
  )
  {
    return "FAULT";
  }

  // Warning condition
  if (
    temperature >= TEMP_WARNING ||
    vibration >= VIB_WARNING ||
    gasValue >= GAS_WARNING
  )
  {
    return "WARNING";
  }

  return "NORMAL";
}


// =================================================
// SETUP
// =================================================

void setup()
{
  Serial.begin(115200);

  delay(1000);

  // LEDs
  pinMode(GREEN_LED, OUTPUT);
  pinMode(YELLOW_LED, OUTPUT);
  pinMode(RED_LED, OUTPUT);

  // Buzzer
  pinMode(BUZZER, OUTPUT);

  // Gas sensor
  pinMode(GAS_PIN, INPUT);

  // Start I2C
  Wire.begin(SDA_PIN, SCL_PIN);

  // Start MPU6050
  if (!mpu.begin())
  {
    Serial.println("ERROR: MPU6050 not found!");

    digitalWrite(RED_LED, HIGH);

    while (1)
    {
      tone(BUZZER, 2000, 300);
      delay(1000);
    }
  }

  // MPU6050 configuration
  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);

  // Start temperature sensor
  tempSensor.begin();

  // Initial state
  normalStatus();

  Serial.println();
  Serial.println("==========================================");
  Serial.println(" INDUSTRIAL MOTOR PREDICTIVE MAINTENANCE");
  Serial.println("==========================================");

  Serial.println("ESP32 started successfully");

  Serial.println();
  Serial.println("Sensors:");
  Serial.println("1. MPU6050 - Vibration");
  Serial.println("2. DS18B20 - Temperature");
  Serial.println("3. MQ-2    - Gas");
  Serial.println();

  delay(2000);
}


// =================================================
// MAIN LOOP
// =================================================

void loop()
{
  if (millis() - previousMillis >= SAMPLE_INTERVAL)
  {
    previousMillis = millis();

    // ---------------- TEMPERATURE ----------------

    tempSensor.requestTemperatures();

    float temperature =
      tempSensor.getTempCByIndex(0);


    // ---------------- VIBRATION ----------------

    float vibration =
      calculateVibrationRMS(100);


    // ---------------- GAS ----------------

    int gasValue =
      analogRead(GAS_PIN);


    // ---------------- CLASSIFICATION ----------------

    String motorState =
      classifyMotor(
        temperature,
        vibration,
        gasValue
      );


    // ---------------- LED + BUZZER ----------------

    if (motorState == "NORMAL")
    {
      normalStatus();
    }

    else if (motorState == "WARNING")
    {
      warningStatus();
    }

    else
    {
      faultStatus();
    }


    // ---------------- SERIAL MONITOR ----------------

    Serial.println("------------------------------------------");

    Serial.print("Temperature : ");
    Serial.print(temperature);
    Serial.println(" °C");

    Serial.print("Vibration   : ");
    Serial.print(vibration);
    Serial.println(" m/s²");

    Serial.print("Gas Sensor  : ");
    Serial.println(gasValue);

    Serial.print("Motor State : ");
    Serial.println(motorState);

    Serial.println("------------------------------------------");


    // ---------------- CSV DATA ----------------

    Serial.print("DATA,");

    Serial.print(millis());
    Serial.print(",");

    Serial.print(temperature);
    Serial.print(",");

    Serial.print(vibration);
    Serial.print(",");

    Serial.print(gasValue);
    Serial.print(",");

    Serial.println(motorState);
  }
}
