import serial
import re
import tkinter as tk
from threading import Thread

# Define COM port settings
com_port = 'COM3'  # Change this to your specific COM port
baud_rate = 9600  # Baud rate of the COM port

def extractValues(input):
    # Use regular expression to find numeric values (positive and negative)
    numeric_values = re.findall(r'-?\d+\.\d+|-?\d+', input)
    
    # Convert strings to actual numeric values
    numeric_values = [int(value) for value in numeric_values]
    
    return numeric_values

def update_and_display_values():
    try:
        # Open the COM port
        ser = serial.Serial(com_port, baud_rate)
        
        while True:
            try:
                # Read data from the COM port
                data = ser.readline().decode('utf-8')
                
                # Split data by space
                data = data.split(" ")
                # Extract LeftDC and RightDC values
                LeftDC_values = extractValues(data[0])
                RightDC_values = extractValues(data[1])

                # Update the StringVar variables with the new values
                LeftDC.set(LeftDC_values)
                RightDC.set(RightDC_values)

            except UnicodeDecodeError:
                print("Error decoding data")
    except serial.SerialException as e:
        print(f"Error: {e}")
    finally:
        if 'ser' in locals() and ser.is_open:
            ser.close()

# Create the main window
root = tk.Tk()
root.title("LeftDC and RightDC Display GUI")

# Create StringVar variables for LeftDC and RightDC
LeftDC = tk.StringVar()
RightDC = tk.StringVar()

# Labels to display LeftDC and RightDC
dc_label = tk.Label(root, text="LeftDC:")
dc_label.pack(side="left")

dc_display = tk.Label(root, textvariable=LeftDC)
dc_display.pack(side="left")

dc_label = tk.Label(root, text="RightDC:")
dc_label.pack(side="left")

dc_display = tk.Label(root, textvariable=RightDC)
dc_display.pack(side="left")

# Create button to start updating and displaying values
start_button = tk.Button(root, text="Start Updating", command=lambda: Thread(target=update_and_display_values).start())
start_button.pack()

# Start the GUI event loop
root.mainloop()
