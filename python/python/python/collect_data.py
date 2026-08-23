import serial
import csv
import os

SERIAL_PORT = "COM3"   # Change this
BAUD_RATE = 115200

os.makedirs("../data", exist_ok=True)

ser = serial.Serial(SERIAL_PORT, BAUD_RATE)

with open("../data/raw_motor_data.csv",
          "a",
          newline="") as file:

    writer = csv.writer(file)

    print("Collecting data... Press Ctrl+C to stop.")

    try:
        while True:

            data = ser.readline().decode(
                "utf-8"
            ).strip()

            # Skip empty or invalid data
            if not data:
                continue

            values = data.split(",")

            if len(values) == 5:

                try:
                    values = [
                        float(value)
                        for value in values
                    ]

                    writer.writerow(values)
                    file.flush()

                    print(values)

                except ValueError:
                    pass

    except KeyboardInterrupt:
        print("\nData collection stopped.")

ser.close()
