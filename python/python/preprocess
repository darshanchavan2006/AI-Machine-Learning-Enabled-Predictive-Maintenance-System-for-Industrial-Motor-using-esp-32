import pandas as pd
import os

os.makedirs("../data", exist_ok=True)

# Load raw dataset
data = pd.read_csv(
    "../data/raw_motor_data.csv"
)

# Remove missing values
data = data.dropna()

# Remove duplicate rows
data = data.drop_duplicates()

# Save processed dataset
data.to_csv(
    "../data/processed_motor_data.csv",
    index=False
)

print("Data preprocessing completed.")
print(data.head())
