# AI & Machine Learning Enabled Predictive Maintenance System for Industrial Motor

[![Platform](https://img.shields.io/badge/Platform-ESP32-blue)](https://www.espressif.com/en/products/socs/esp32)
[![Language](https://img.shields.io/badge/Firmware-Arduino%20C%2B%2B-orange)](https://www.arduino.cc/)
[![ML](https://img.shields.io/badge/ML-Random%20Forest-green)](https://scikit-learn.org/)
[![Python](https://img.shields.io/badge/Python-3.x-yellow)](https://www.python.org/)

An embedded IoT and machine-learning prototype for **condition monitoring and predictive maintenance of industrial motors** using multi-sensor data and an ML-based operating-state classifier.

The system collects **vibration, temperature, and gas/environmental measurements** through an ESP32, builds a labelled dataset, trains a Random Forest classifier, and predicts the motor condition as **Normal, Warning, or Fault**.

> **Project status:** Working prototype. The current firmware supports real-time sensing and threshold-based local alerts; the ML model is trained and evaluated in Python/Jupyter. Direct TinyML inference on the ESP32 is a planned extension.

---

## Overview

Unexpected motor failures can cause downtime, maintenance costs, and production losses. Predictive maintenance approaches monitor operating conditions and use historical data to identify abnormal behavior before a severe failure occurs.

This project explores that workflow using a low-cost embedded platform:

```text
Sensors
   │
   ▼
ESP32 Data Acquisition
   │
   ▼
Feature / Dataset Generation
   │
   ▼
Python + Machine Learning
   │
   ▼
Motor Condition Classification
   │
   ├── NORMAL
   ├── WARNING
   └── FAULT
```

The design intentionally separates **embedded data acquisition** from **offline model development**, making it straightforward to replace the demonstration dataset with real measurements collected from the hardware.

---

## Key Features

- Real-time sensor acquisition using ESP32
- MPU6050-based vibration/acceleration monitoring
- DS18B20 temperature monitoring
- MQ-2 gas/smoke/environmental signal monitoring
- Vibration RMS feature calculation
- CSV-compatible sensor-data output
- Normal / Warning / Fault condition classification
- Random Forest machine-learning model
- Imbalanced dataset representing a monitoring scenario with more normal operation than faults
- Model evaluation using:
  - Accuracy
  - Precision
  - Recall
  - F1-score
  - Confusion matrix
  - Feature importance
- LED and buzzer alerts on the ESP32
- Saved Python model for repeatable inference
- Architecture suitable for future TinyML/Edge-AI deployment

---

## System Architecture

```text
                   INDUSTRIAL MOTOR
                          │
             ┌────────────┼────────────┐
             │            │            │
             ▼            ▼            ▼
         MPU6050       DS18B20       MQ-2
        Vibration     Temperature     Gas/
       Acceleration                  Environment
             │            │            │
             └────────────┼────────────┘
                          ▼
                        ESP32
                          │
                  Sensor Acquisition
                          │
                 RMS / Feature Data
                          │
                          ▼
                   Dataset (CSV)
                          │
                          ▼
                Python / Jupyter
                          │
                  Random Forest
                          │
                          ▼
             Motor Condition Prediction
                          │
          ┌───────────────┼───────────────┐
          ▼               ▼               ▼
       NORMAL          WARNING          FAULT
          │               │               │
      Green LED       Yellow LED       Red LED
                                          │
                                       Buzzer
```

---

## Hardware

| Component | Purpose |
|---|---|
| ESP32 | Embedded controller and data acquisition |
| MPU6050 | Vibration/acceleration measurement |
| DS18B20 | Temperature measurement |
| MQ-2 | Gas/smoke/environmental signal |
| Green LED | Normal-state indication |
| Yellow LED | Warning indication |
| Red LED | Fault indication |
| Buzzer | Audible alert |

### ESP32 Pin Configuration

| Device | Signal | ESP32 Pin |
|---|---|---:|
| MPU6050 | SDA | GPIO 21 |
| MPU6050 | SCL | GPIO 22 |
| DS18B20 | DATA | GPIO 4 |
| MQ-2 | AO | GPIO 34 |
| Green LED | Digital | GPIO 25 |
| Yellow LED | Digital | GPIO 26 |
| Red LED | Digital | GPIO 27 |
| Buzzer | Digital | GPIO 14 |

### Electrical Notes

- MPU6050 is interfaced over I2C.
- DS18B20 requires a pull-up resistor on its data line.
- MQ-series sensors commonly operate from 5 V. Its analog output must be kept within the ESP32 ADC input range; use suitable voltage scaling/level shifting where required.
- High-power loads and industrial motors should not be connected directly to ESP32 GPIOs.
- The prototype should be tested using a safe, low-voltage motor setup.

---

## Embedded Firmware

The Arduino firmware performs:

1. Sensor initialization
2. Temperature acquisition
3. MPU6050 acceleration sampling
4. Vibration RMS calculation
5. MQ-2 analog acquisition
6. Condition evaluation
7. LED/buzzer status indication
8. CSV-style serial output

Example output:

```text
Temperature : 41.20 °C
Vibration   : 0.82 m/s²
Gas Sensor  : 704
Motor State : NORMAL
```

CSV-style output can be collected for machine-learning dataset generation:

```text
DATA,1000,41.20,0.82,704,NORMAL
```

---

## Machine Learning Pipeline

The ML workflow is implemented in Python/Jupyter:

```text
Raw Sensor Data
      │
      ▼
Data Cleaning
      │
      ▼
Feature Selection
      │
      ▼
Train / Test Split
      │
      ▼
Random Forest Classifier
      │
      ▼
Evaluation
      │
      ▼
Motor State Prediction
```

### Input Features

The primary model uses:

```text
temperature_c
vibration_rms_ms2
gas_adc
```

Additional derived features available in the dataset include:

```text
temperature_change
vibration_peak
gas_normalized
```

### Target

```text
motor_state
```

with three classes:

```text
NORMAL
WARNING
FAULT
```

---

## Dataset

The repository uses an **imbalanced synthetic dataset** for initial model development.

| Class | Samples | Approx. Share |
|---|---:|---:|
| NORMAL | 1,100 | 73.33% |
| WARNING | 320 | 21.33% |
| FAULT | 80 | 5.33% |
| **Total** | **1,500** | **100%** |

The imbalance is intentional: a monitoring system would normally observe substantially more healthy operation than actual fault conditions.

### Important Dataset Disclaimer

The included dataset is **synthetic**, generated for development and demonstration. It should not be interpreted as measured industrial motor data.

For meaningful real-world evaluation, the dataset should be replaced or extended with sensor measurements collected from the target motor under controlled and safely labelled operating conditions.

---

## Machine Learning Model

### Random Forest Classifier

The first model uses a Random Forest classifier because it provides:

- Strong performance on tabular numerical data
- Nonlinear decision boundaries
- Minimal preprocessing requirements
- Feature-importance estimates
- Straightforward implementation and interpretation

The dataset is divided into training and test subsets using a stratified split.

```text
80% → Training
20% → Testing
```

Because the dataset is imbalanced, model performance should not be judged by accuracy alone.

The project evaluates:

- Accuracy
- Precision
- Recall
- F1-score
- Confusion matrix
- Feature importance

For predictive maintenance, **recall for the FAULT class** is particularly important because false negatives can represent missed abnormal conditions.

---

## Example Inference

A trained model can receive a new sensor sample:

```text
Temperature = 63 °C
Vibration  = 2.5 m/s²
Gas ADC    = 1900
```

and produce:

```text
Prediction: WARNING
```

Another sample:

```text
Temperature = 82 °C
Vibration  = 5.0 m/s²
Gas ADC    = 3200
```

may produce:

```text
Prediction: FAULT
```

The actual result and class probabilities are generated by the trained model.

---

## Repository Structure

```text
AI-ML-Industrial-Motor-Predictive-Maintenance/
│
├── Arduino/
│   └── predictive_maintenance_esp32.ino
│
├── ML/
│   ├── motor_predictive_maintenance_ML.ipynb
│   └── motor_predictive_model.pkl
│
├── Dataset/
│   └── motor_predictive_maintenance_imbalanced_dataset.csv
│
├── README.md
│
└── Documentation/
```

---

## Software Requirements

### Firmware

- Arduino IDE
- ESP32 board package
- Adafruit MPU6050
- Adafruit Unified Sensor
- OneWire
- DallasTemperature

### Machine Learning

- Python 3.x
- Jupyter Notebook
- pandas
- NumPy
- matplotlib
- scikit-learn
- joblib

Install the Python dependencies:

```bash
pip install pandas numpy matplotlib scikit-learn joblib jupyter
```

---

## Running the Machine Learning Pipeline

Place the dataset and notebook in the same working directory.

Start Jupyter:

```bash
jupyter notebook
```

Open:

```text
motor_predictive_maintenance_ML.ipynb
```

Run the cells in order.

The notebook:

1. Loads the dataset
2. Explores class distribution
3. Selects features
4. Splits training and testing data
5. Trains the Random Forest model
6. Generates predictions
7. Calculates evaluation metrics
8. Displays the confusion matrix
9. Displays feature importance
10. Saves the trained model as:

```text
motor_predictive_model.pkl
```

---

## Real Sensor Data Workflow

For a stronger real-world version, sensor readings should be collected directly from the ESP32.

Recommended workflow:

```text
ESP32
  │
  ▼
Real Sensor Measurements
  │
  ▼
CSV Data Logging
  │
  ▼
Data Cleaning
  │
  ▼
Feature Engineering
  │
  ▼
Manual / Controlled Labelling
  │
  ▼
ML Training
  │
  ▼
Validation on Unseen Data
```

Fault samples should be collected only through safe, controlled test conditions or appropriate recorded/replayed fault scenarios. The project should not intentionally create hazardous electrical, thermal, or mechanical faults.

---

## Current vs. Planned AI Architecture

### Current Prototype

```text
ESP32
  ├── Sensor acquisition
  ├── Vibration RMS calculation
  └── Threshold-based local alert

Python
  ├── Dataset processing
  ├── Random Forest training
  └── ML prediction
```

### Planned Edge-AI Version

```text
ESP32
  ├── Sensor acquisition
  ├── Feature extraction
  └── Lightweight ML inference
             │
             ▼
       NORMAL / WARNING / FAULT
```

The current Python `.pkl` model is intended for desktop/Python inference. It is **not directly executable on the ESP32**. A future version can convert or reimplement the trained inference pipeline using a TinyML-compatible approach.

---

## Future Improvements

### Hardware

- Add motor current sensing
- Use an industrial-grade vibration sensor
- Add OLED/local display
- Add SD-card data logging
- Improve sensor calibration
- Add isolated industrial interfaces

### Machine Learning

- Replace synthetic data with real measurements
- Increase fault-class coverage
- Cross-validation
- Hyperparameter optimization
- Class weighting / imbalance handling
- Compare Random Forest, SVM, KNN and gradient-boosting models
- Time-series feature extraction
- Anomaly detection
- Fault-type classification
- Remaining Useful Life (RUL) estimation

### IoT / Edge AI

- MQTT-based monitoring
- Web dashboard
- Cloud data logging
- Remote alerts
- TinyML inference on ESP32
- Local fault-history storage
- Real-time edge analytics

---

## Limitations

- The current dataset is synthetic and does not represent all industrial motor conditions.
- Sensor thresholds and feature distributions are application-dependent.
- MQ-2 is an environmental gas/smoke sensor, not a dedicated motor-fault diagnostic sensor.
- Real industrial deployment requires calibrated, appropriately rated sensors and electrical isolation.
- High model accuracy on synthetic data does not demonstrate equivalent real-world performance.
- The current ESP32 firmware uses threshold-based alerts rather than executing the trained Random Forest model locally.
- This repository represents a research/engineering prototype, not a certified industrial safety or protection system.

---

## Project Outcomes

This project demonstrates an end-to-end predictive-maintenance workflow:

```text
Embedded Sensing
       ↓
Data Acquisition
       ↓
Feature Engineering
       ↓
Dataset Development
       ↓
Machine Learning
       ↓
Model Evaluation
       ↓
Condition Classification
       ↓
Future Edge-AI Deployment
```

It combines:

**Embedded Systems + IoT + Sensor Fusion + Data Engineering + Machine Learning + Predictive Maintenance + Edge AI**

---

## Author

**Darshan Chavan**

Electronics & Telecommunication Engineering

---

## License

Add an appropriate open-source license before publishing the repository, such as MIT, Apache-2.0, or another license that matches your intended use.
