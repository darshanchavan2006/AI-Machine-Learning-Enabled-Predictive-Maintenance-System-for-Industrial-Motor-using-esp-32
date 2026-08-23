import pandas as pd
import joblib

from sklearn.model_selection import train_test_split
from sklearn.ensemble import RandomForestClassifier
from sklearn.metrics import (
    accuracy_score,
    classification_report
)

# Load processed dataset
data = pd.read_csv(
    "../data/processed_motor_data.csv"
)

# Input features
X = data[
    [
        "Temperature",
        "Accel_X",
        "Accel_Y",
        "Accel_Z",
        "Vibration"
    ]
]

# Target
y = data["Condition"]

# Split data
X_train, X_test, y_train, y_test = (
    train_test_split(
        X,
        y,
        test_size=0.2,
        random_state=42,
        stratify=y
    )
)

# Create model
model = RandomForestClassifier(
    n_estimators=100,
    random_state=42
)

# Train model
model.fit(X_train, y_train)

# Prediction
predictions = model.predict(X_test)

# Accuracy
accuracy = accuracy_score(
    y_test,
    predictions
)

print("Accuracy:", accuracy)

print(
    classification_report(
        y_test,
        predictions
    )
)

# Save model
joblib.dump(
    model,
    "../models/motor_condition_model.pkl"
)

print("Model saved successfully.")
