# AI & Machine Learning Enabled Predictive Maintenance System for Industrial Motor using esp 32


An embedded IoT and machine-learning prototype for **condition monitoring and predictive maintenance of industrial motors** using multi-sensor data and an ML-based operating-state classifier.

The system collects **vibration, temperature, and gas/environmental measurements** through an ESP32, builds a labelled dataset, trains a Random Forest classifier, and predicts the motor condition as **Normal, Warning, or Fault**.

> **Project Status:** Working prototype. The current firmware performs real-time sensor acquisition and threshold-based local alerts, while the machine-learning model is trained and evaluated using Python/Jupyter. Direct TinyML inference on the ESP32 is planned as a future extension.

---

## Overview

Unexpected motor failures can result in equipment downtime, increased maintenance costs, and production losses. Predictive-maintenance systems monitor operating conditions and use historical data to identify abnormal behavior before a severe failure occurs.

This project implements an end-to-end prototype using a low-cost embedded platform:

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

The architecture separates **embedded data acquisition** from **machine-learning development**, allowing real ESP32 measurements to replace the initial development dataset.

---

## Key Features

- Real-time sensor acquisition using ESP32
- MPU6050-based vibration/acceleration monitoring
- DS18B20 temperature monitoring
- MQ-2 gas/smoke/environmental monitoring
- Vibration RMS calculation
- CSV-compatible sensor-data output
- Normal / Warning / Fault condition classification
- Random Forest machine-learning model
- Imbalanced dataset for realistic class distribution
- Model evaluation using:
  - Accuracy
  - Precision
  - Recall
  - F1-score
  - Confusion Matrix
  - Feature Importance
- LED-based condition indication
- Buzzer-based alert mechanism
- Python model serialization
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
        Vibration     Temperature     Gas /
       Acceleration                 Environment
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
| ESP32 | Embedded controller and sensor data acquisition |
| MPU6050 | Vibration/acceleration measurement |
| DS18B20 | Temperature measurement |
| MQ-2 | Gas/smoke/environmental signal |
| Green LED | Normal-state indication |
| Yellow LED | Warning indication |
| Red LED | Fault indication |
| Buzzer | Audible alert |

---

## ESP32 Pin Configuration

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

- MPU6050 communicates with the ESP32 using I2C.
- DS18B20 requires a pull-up resistor on its data line.
- MQ-series sensors commonly operate at 5 V. Their analog output must remain within the ESP32 ADC input range; appropriate voltage scaling should be used where necessary.
- High-power motors and loads must not be connected directly to ESP32 GPIOs.
- Development and testing should use a safe, low-voltage motor setup.

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

Example serial output:

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

The machine-learning workflow is implemented using Python and Jupyter Notebook.

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
Model Evaluation
      │
      ▼
Motor State Prediction
```

### Primary Model Features

```text
temperature_c
vibration_rms_ms2
gas_adc
```

Additional engineered features available in the dataset include:

```text
temperature_change
vibration_peak
gas_normalized
```

### Target Variable

```text
motor_state
```

Classes:

```text
NORMAL
WARNING
FAULT
```

---

## Dataset

The repository includes an intentionally **imbalanced synthetic dataset** for initial model development.

| Class | Samples | Approx. Share |
|---|---:|---:|
| NORMAL | 1,100 | 73.33% |
| WARNING | 320 | 21.33% |
| FAULT | 80 | 5.33% |
| **Total** | **1,500** | **100%** |

The imbalance reflects a realistic monitoring scenario where healthy operating conditions are expected to occur significantly more often than faults.

### Dataset Disclaimer

The included dataset is **synthetic development data** and does not represent measured industrial motor behavior.

For real-world evaluation, the dataset should be replaced or extended using sensor measurements collected from the target motor under safe, controlled, and appropriately labelled operating conditions.

---

## Machine Learning Model

### Random Forest Classifier

The initial machine-learning implementation uses a Random Forest classifier.

Random Forest was selected because it:

- Performs well on tabular numerical data
- Can capture nonlinear relationships
- Requires relatively little preprocessing
- Supports feature-importance analysis
- Is straightforward to train and evaluate

The dataset is split using a stratified train/test split:

```text
80% → Training
20% → Testing
```

Because the dataset is imbalanced, the model is evaluated using more than accuracy alone.

### Evaluation Metrics

- Accuracy
- Precision
- Recall
- F1-score
- Confusion Matrix
- Feature Importance

For predictive maintenance, **recall of the FAULT class** is particularly important because missed fault detections can be more significant than false alarms.

---

## Example Inference

Example sensor input:

```text
Temperature = 63 °C
Vibration  = 2.5 m/s²
Gas ADC    = 1900
```

Possible prediction:

```text
WARNING
```

Another example:

```text
Temperature = 82 °C
Vibration  = 5.0 m/s²
Gas ADC    = 3200
```

Possible prediction:

```text
FAULT
```

The actual prediction and class probabilities are generated by the trained model.

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
├── Documentation/
│
└── README.md
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

Install Python dependencies:

```bash
pip install pandas numpy matplotlib scikit-learn joblib jupyter
```

---

## Running the Machine Learning Pipeline

Place the Jupyter notebook and dataset in the same working directory.

Start Jupyter:

```bash
jupyter notebook
```

Open:

```text
motor_predictive_maintenance_ML.ipynb
```

Run the notebook cells in order.

The notebook performs:

1. Dataset loading
2. Data exploration
3. Class-distribution analysis
4. Feature selection
5. Stratified train/test split
6. Random Forest training
7. Prediction
8. Classification metrics
9. Confusion-matrix visualization
10. Feature-importance analysis
11. Model serialization
12. Custom sensor-value prediction

The trained model is saved as:

```text
motor_predictive_model.pkl
```

---

## Real Sensor Data Workflow

For a stronger version of the project, sensor readings should be collected directly from the ESP32.

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
Controlled Data Labelling
  │
  ▼
Machine Learning Training
  │
  ▼
Validation on Unseen Data
```

Fault conditions should be represented through safe and controlled experiments or recorded/replayed fault data rather than intentionally creating hazardous motor failures.

---

## Current AI/ML Architecture

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

The threshold-based firmware and Python ML model are intentionally separated in the current prototype.

This allows the hardware system to be tested independently while the machine-learning pipeline is developed and evaluated offline.

---

## Future Edge-AI Architecture

```text
ESP32
  ├── Sensor acquisition
  ├── Feature extraction
  └── Lightweight ML inference
             │
             ▼
      NORMAL / WARNING / FAULT
```

The current Python `.pkl` model is designed for Python-based inference and cannot simply be uploaded to an ESP32.

A future implementation can use a TinyML-compatible model or an embedded implementation of the trained classifier to perform inference directly on the ESP32.

---

## Future Improvements

### Hardware

- Add motor-current sensing
- Upgrade to industrial-grade vibration sensing
- Add OLED display
- Add SD-card data logging
- Improve sensor calibration
- Add electrical isolation and industrial interfaces

### Machine Learning

- Replace synthetic data with real sensor measurements
- Increase the number of fault samples
- Cross-validation
- Hyperparameter optimization
- Class weighting and imbalance-handling techniques
- Comparison of Random Forest, SVM, KNN and gradient-boosting models
- Time-series feature extraction
- Anomaly detection
- Fault-type classification
- Remaining Useful Life (RUL) estimation

### IoT / Edge AI

- MQTT communication
- Web dashboard
- Cloud-based data storage
- Remote fault notifications
- TinyML deployment on ESP32
- Local fault-history storage
- Real-time edge analytics

---

## Limitations

- The current dataset is synthetic and does not represent all industrial motor conditions.
- Sensor thresholds and feature behavior are application-dependent.
- MQ-2 is an environmental gas/smoke sensor rather than a dedicated motor-fault diagnostic sensor.
- Real industrial deployment requires calibrated, appropriately rated sensors and proper electrical isolation.
- High model accuracy on synthetic data does not imply equivalent real-world performance.
- The current ESP32 firmware does not execute the trained Random Forest model locally.
- This project is an engineering prototype and is not intended to replace certified industrial protection or safety systems.

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

**Embedded Systems · IoT · Sensor Fusion · Data Engineering · Machine Learning · Predictive Maintenance · Edge AI**

---

## Technology Stack

### Hardware
- ESP32
- MPU6050
- DS18B20
- MQ-2
- LEDs
- Buzzer

### Firmware
- Arduino IDE
- C/C++

### Machine Learning
- Python
- Jupyter Notebook
- Pandas
- NumPy
- Scikit-learn
- Random Forest
- Joblib

### Concepts
- Condition Monitoring
- Predictive Maintenance
- Sensor Fusion
- Classification
- Embedded Systems
- IoT
- Edge AI
- TinyML

---

## Author

**Darshan Chavan**  
Electronics & Telecommunication Engineering


