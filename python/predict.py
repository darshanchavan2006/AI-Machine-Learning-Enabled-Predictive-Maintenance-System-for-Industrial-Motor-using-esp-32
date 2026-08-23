import joblib
import pandas as pd

# Load model
model = joblib.load(
    "../models/motor_condition_model.pkl"
)

# Example sensor readings
new_data = pd.DataFrame(
    [[
        32.5,   # Temperature
        0.20,   # Accel_X
        0.30,   # Accel_Y
        9.75,   # Accel_Z
        9.80    # Vibration
    ]],

    columns=[
        "Temperature",
        "Accel_X",
        "Accel_Y",
        "Accel_Z",
        "Vibration"
    ]
)

# Prediction
prediction = model.predict(new_data)

print(
    "Motor Condition:",
    prediction[0]
)
