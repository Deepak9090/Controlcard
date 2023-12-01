"""
IF Receiver GUI
Author: Deepak Kumar Panda
Copyright (c) 2023 Your Name. All rights reserved.
"""
import tkinter as tk
from tkinter import ttk
import serial.tools.list_ports
import logging


class SerialHandler:
    def __init__(self):
        self.ser = None

    def open_port(self, port):
        try:
            self.ser = serial.Serial(port, baudrate=9600, timeout=1)
            logging.info(f"Serial port {port} opened successfully.")
            return True
        except serial.SerialException as e:
            if isinstance(e.__cause__, PermissionError):
                logging.error(f"Permission error: Make sure you have the necessary permissions to access {port}.")
            else:
                logging.error(f"Error opening serial port: {e}")
            return False

    def close_port(self):
        if self.ser and self.ser.is_open:
            self.ser.close()
            logging.info("Serial port closed.")
        else:
            logging.info("No open serial port to close.")


class IFReceiverGUI:
    def __init__(self, root, serial_handler):
        self.root = root
        self.serial_handler = serial_handler
        self.ser = None
        self.root.title('IF RECEIVER GUI')
        self.root.geometry('320x450')
        self.connected = False  # Flag to track the connection status

        self.create_title_label()
        self.create_widgets()

    def create_title_label(self):
        title_label = ttk.Label(self.root, text="IF RECEIVER GUI", font=("Helvetica", 24, "bold"), foreground="blue")
        title_label.grid(column=0, row=0, columnspan=3, sticky='N', padx=0, pady=30)
        title_label.configure(style='Title.TLabel')  # Set background color for the title label

    def create_widgets(self):
        self.create_serial_frame()
        self.create_attenuator_box()
        self.create_phase_shifter_box()
        self.create_sent_text_box()
        self.create_buttons()

        # Disable phase shifter and attenuator comboboxes by default
        for i in range(3):
            self.phase_shifter_comboboxes[i]['state'] = 'disabled'
            self.attenuator_comboboxes[i]['state'] = 'disabled'

        # Disable send button by default
        self.send_button['state'] = 'disabled'

    def create_serial_frame(self):
        serial_frame = ttk.LabelFrame(self.root, text="Serial Communication")
        serial_frame.grid(column=0, row=1, columnspan=3, pady=10, padx=10, sticky='WE')
        serial_frame.configure(style='BW.TLabelframe')  # Set background color for the serial frame

        self.port_combobox = ttk.Combobox(serial_frame, text="COM", width=10)
        self.port_combobox.grid(column=0, row=0, padx=10, pady=5, sticky='W')

        self.refresh_button = ttk.Button(serial_frame, text="Refresh", command=self.refresh_ports, state='normal')
        self.refresh_button.grid(column=1, row=0, padx=10, pady=5)

        self.connect_button = ttk.Button(serial_frame, text="Connect", command=self.toggle_connect, state='normal')
        self.connect_button.grid(column=2, row=0, padx=10, pady=5)

    def create_attenuator_box(self):
        attenuator_box = ttk.LabelFrame(self.root, text="Attenuators (dB)")
        attenuator_box.grid(column=0, row=2, columnspan=3, padx=10, pady=10, sticky='WE')

        self.attenuator_comboboxes = []
        for i in range(3):
            attenuator_combobox = ttk.Combobox(attenuator_box, width=10)
            attenuator_combobox.grid(column=i, row=0, pady=10)
            attenuator_combobox['values'] = [str(j * 0.5) for j in range(61)]
            self.attenuator_comboboxes.append(attenuator_combobox)

    def create_phase_shifter_box(self):
        phase_shifter_box = ttk.LabelFrame(self.root, text="Phase Shifters (V)")
        phase_shifter_box.grid(column=0, row=3, columnspan=3, padx=10, pady=10, sticky='WE')

        self.phase_shifter_comboboxes = []
        for i in range(1, 4):
            phase_shifter_combobox = ttk.Combobox(phase_shifter_box, width=10)
            phase_shifter_combobox.grid(column=i, row=0, pady=10)
            phase_shifter_combobox['values'] = [str(j * 0.5) for j in range(31)]
            self.phase_shifter_comboboxes.append(phase_shifter_combobox)

    def create_sent_text_box(self):
        sent_text_box = ttk.LabelFrame(self.root, text="Sent Text")
        sent_text_box.grid(column=0, row=4, columnspan=3, padx=10, pady=10, sticky='WE')

        self.sent_label = ttk.Label(sent_text_box, text="", font=("Robot", 10))
        self.sent_label.grid(column=0, row=0, pady=10)

    def create_buttons(self):
        self.send_button = ttk.Button(self.root, text="Send", command=self.send_data)
        self.send_button.grid(column=1, row=5, pady=10)

    def close_serial_port(self):
        if self.ser and self.ser.is_open:
            self.ser.close()
            logging.info("Serial port closed.")
        else:
            logging.warning("No open serial port to close.")
        self.connected = False

        # Enable widgets when disconnected
        self.port_combobox['state'] = 'normal'
        self.connect_button['state'] = 'normal'
        self.refresh_button['state'] = 'normal'
        self.send_button['state'] = 'disable'
        for i in range(3):
            self.phase_shifter_comboboxes[i]['state'] = 'disabled'
            self.attenuator_comboboxes[i]['state'] = 'disabled'
        # Reset connection-related labels
        ttk.Label(self.root, text="", font=("Robot", 10)).grid(column=3, row=11, columnspan=2, rowspan=2, sticky='W',
                                                               padx=10, pady=25)
        # Update the button text to "Connect"
        self.connect_button.configure(text="Connect")

    def open_serial_port(self, port):
        try:
            self.ser = serial.Serial(port, baudrate=9600, timeout=1)
            logging.info(f"Serial port {port} opened successfully.")
            return True
        except serial.SerialException as e:
            if isinstance(e.__cause__, PermissionError):
                logging.error(f"Permission error: Make sure you have the necessary permissions to access {port}.")
            else:
                logging.error(f"Error opening serial port: {e}")
            return False

    def refresh_ports(self):
        ttk.Label(self.root, text="Refreshing serial ports...", font=("Robot", 10)).grid(column=3, row=1, columnspan=2,
                                                                                         rowspan=2, sticky='W', padx=10,
                                                                                         pady=25)
        self.root.update_idletasks()  # Force update to show the label
        self.port_combobox['values'] = [comport.device for comport in serial.tools.list_ports.comports()]
        logging.info("Serial ports refreshed.")
        ttk.Label(self.root, text="", font=("Robot", 10)).grid(column=3, row=1, columnspan=2, rowspan=2, sticky='W',
                                                               padx=10, pady=25)  # Clear the label

    def toggle_connect(self):
        if not self.connected:
            selected_port = self.port_combobox.get()
            if selected_port:
                if self.open_serial_port(selected_port):
                    ttk.Label(self.root, text="Connected to " + selected_port, font=("Robot", 10)).grid(column=3,
                                                                                                        row=11,
                                                                                                        columnspan=2,
                                                                                                        rowspan=2,
                                                                                                        sticky='W',
                                                                                                        padx=10,
                                                                                                        pady=25)
                    # Disable widgets when connected
                    self.port_combobox['state'] = 'disabled'
                    self.refresh_button['state'] = 'disabled'
                    self.send_button['state'] = 'enable'
                    for i in range(3):
                        self.phase_shifter_comboboxes[i]['state'] = 'enable'
                        self.attenuator_comboboxes[i]['state'] = 'enable'
                    self.connected = True
                    # Update the button text to "Disconnect"
                    self.connect_button.configure(text="Disconnect")
                else:
                    print(f"Failed to connect to {selected_port}")
        else:
            # Disconnect and release the serial port
            self.close_serial_port()

    def send_data(self):

        attenuator_values = [combobox.get() or '0' for combobox in self.attenuator_comboboxes]
        phase_shifter_values = [combobox.get() or '0' for combobox in self.phase_shifter_comboboxes]

        data = f"${','.join(attenuator_values + phase_shifter_values)}#"
        logging.info(f"Sending data: {data}")

        self.sent_label.configure(text=data)
        self.ser.write(data.encode())

        logging.info("Data sent over serial.")

if __name__ == "__main__":
    style = ttk.Style()
    style.configure('Title.TLabel', background='#D3D3D3')  # Set background color for the title label
    style.configure('BW.TLabelframe', background='#D3D3D3')  # Set background color for the serial frame

    logging.basicConfig(level=logging.DEBUG)  # Set the desired logging level

    serial_handler = SerialHandler()

    root = tk.Tk()
    app = IFReceiverGUI(root, serial_handler)
    root.mainloop()
