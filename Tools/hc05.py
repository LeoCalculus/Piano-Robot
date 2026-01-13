import tkinter as tk
from tkinter import ttk, messagebox, scrolledtext
import threading
import time
import serial
import serial.tools.list_ports
import sys
import os

class HC05SerialGUI:
    def __init__(self, root):
        self.root = root
        self.root.title("HC05 Serial Testing Tool")
        self.root.geometry("800x600")
        self.root.resizable(True, True)
        
        # Set color scheme
        self.bg_color = "#f0f0f0"
        self.header_color = "#3498db"
        self.button_color = "#2980b9"
        self.text_color = "#2c3e50"
        self.highlight_color = "#e74c3c"
        
        self.root.configure(bg=self.bg_color)
        
        # Variables
        self.ports = {}
        self.selected_port = None
        self.scanning = False
        self.connected = False
        self.ser = None
        self.reading_thread = None
        self.keep_reading = False
        
        # Create styles
        self.style = ttk.Style()
        self.style.configure("TButton", font=("Arial", 10))
        self.style.configure("Primary.TButton", background=self.button_color, foreground="white")
        self.style.configure("TLabel", font=("Arial", 10))
        self.style.configure("Header.TLabel", font=("Arial", 12, "bold"), foreground=self.header_color)
        
        # Create the main frame
        self.main_frame = ttk.Frame(self.root, padding=10)
        self.main_frame.pack(fill=tk.BOTH, expand=True)
        
        # Create GUI elements
        self.create_header()
        self.create_notebook()
        
        # Populate ports initially
        self.refresh_ports()
        
        # Start periodic check for UI updates
        self.root.after(100, self.check_ui_updates)

    def create_header(self):
        header_frame = ttk.Frame(self.main_frame)
        header_frame.pack(fill=tk.X, pady=5)
        
        title_label = ttk.Label(header_frame, text="HC05 Serial Testing Tool", 
                               style="Header.TLabel")
        title_label.pack(side=tk.LEFT)
        
        self.status_var = tk.StringVar(value="Ready")
        status_label = ttk.Label(header_frame, textvariable=self.status_var, 
                                foreground=self.text_color)
        status_label.pack(side=tk.RIGHT)

    def create_notebook(self):
        """Create tabbed interface"""
        self.notebook = ttk.Notebook(self.main_frame)
        self.notebook.pack(fill=tk.BOTH, expand=True, pady=5)
        
        # Create tabs
        self.connection_tab = ttk.Frame(self.notebook)
        self.serial_testing_tab = ttk.Frame(self.notebook)
        self.at_command_tab = ttk.Frame(self.notebook)
        
        # Add tabs to notebook
        self.notebook.add(self.connection_tab, text="Connection")
        self.notebook.add(self.serial_testing_tab, text="Serial Testing")
        self.notebook.add(self.at_command_tab, text="AT Commands")
        
        # Set up each tab
        self.setup_connection_tab()
        self.setup_serial_testing_tab()
        self.setup_at_command_tab()
        
        # Disable tabs until connected
        self.notebook.tab(1, state="disabled")
        self.notebook.tab(2, state="disabled")
        
        # Bind tab change event
        self.notebook.bind("<<NotebookTabChanged>>", self.on_tab_change)

    def setup_connection_tab(self):
        """Setup the connection tab"""
        # Create port list
        self.create_port_list()
        
        # Create connection controls
        control_frame = ttk.Frame(self.connection_tab)
        control_frame.pack(fill=tk.X, pady=5)
        
        # Buttons
        self.scan_button = ttk.Button(control_frame, text="Auto-Scan for HC05", 
                                     command=self.auto_scan_for_hc05)
        self.scan_button.pack(side=tk.LEFT, padx=5)
        
        self.connect_button = ttk.Button(control_frame, text="Connect", 
                                       command=self.connect_port, state=tk.DISABLED)
        self.connect_button.pack(side=tk.LEFT, padx=5)
        
        self.refresh_button = ttk.Button(control_frame, text="Refresh Ports", 
                                       command=self.refresh_ports)
        self.refresh_button.pack(side=tk.LEFT, padx=5)
        
        self.disconnect_button = ttk.Button(control_frame, text="Disconnect", 
                                          command=self.disconnect_port, state=tk.DISABLED)
        self.disconnect_button.pack(side=tk.LEFT, padx=5)
        
        quit_button = ttk.Button(control_frame, text="Quit", 
                               command=self.on_closing)
        quit_button.pack(side=tk.RIGHT, padx=5)
        
        # Connection settings
        settings_frame = ttk.LabelFrame(self.connection_tab, text="Connection Settings")
        settings_frame.pack(fill=tk.X, pady=10, padx=5)
        
        # Baudrate
        baud_frame = ttk.Frame(settings_frame)
        baud_frame.pack(fill=tk.X, pady=5)
        
        ttk.Label(baud_frame, text="Baud Rate:").pack(side=tk.LEFT, padx=5)
        self.baudrate_var = tk.StringVar(value="9600")
        baudrate_combo = ttk.Combobox(baud_frame, textvariable=self.baudrate_var, 
                                     values=["9600", "38400", "57600", "115200"], width=10)
        baudrate_combo.pack(side=tk.LEFT, padx=5)
        
        # Line Endings
        ending_frame = ttk.Frame(settings_frame)
        ending_frame.pack(fill=tk.X, pady=5)
        
        ttk.Label(ending_frame, text="Line Ending:").pack(side=tk.LEFT, padx=5)
        self.line_ending_var = tk.StringVar(value="CRLF")  # Default to CRLF for AT commands
        ttk.Radiobutton(ending_frame, text="None", variable=self.line_ending_var, 
                       value="NONE").pack(side=tk.LEFT, padx=5)
        ttk.Radiobutton(ending_frame, text="CR", variable=self.line_ending_var, 
                       value="CR").pack(side=tk.LEFT, padx=5)
        ttk.Radiobutton(ending_frame, text="LF", variable=self.line_ending_var, 
                       value="LF").pack(side=tk.LEFT, padx=5)
        ttk.Radiobutton(ending_frame, text="CR+LF", variable=self.line_ending_var, 
                       value="CRLF").pack(side=tk.LEFT, padx=5)
        
        # Connection info
        self.conn_info_frame = ttk.LabelFrame(self.connection_tab, text="Connection Info")
        self.conn_info_frame.pack(fill=tk.X, pady=10, padx=5)
        
        info_frame = ttk.Frame(self.conn_info_frame)
        info_frame.pack(fill=tk.X, pady=5)
        
        ttk.Label(info_frame, text="Connected Port:").grid(row=0, column=0, sticky=tk.W, padx=5)
        self.conn_port_var = tk.StringVar(value="None")
        ttk.Label(info_frame, textvariable=self.conn_port_var).grid(row=0, column=1, sticky=tk.W)
        
        ttk.Label(info_frame, text="Status:").grid(row=1, column=0, sticky=tk.W, padx=5)
        self.conn_status_var = tk.StringVar(value="Disconnected")
        ttk.Label(info_frame, textvariable=self.conn_status_var).grid(row=1, column=1, sticky=tk.W)
        
        # Create log area
        log_frame = ttk.LabelFrame(self.connection_tab, text="Status Log")
        log_frame.pack(fill=tk.BOTH, expand=True, pady=5, padx=5)
        
        self.log_text = scrolledtext.ScrolledText(log_frame, height=6, width=80, wrap=tk.WORD)
        self.log_text.pack(fill=tk.BOTH, expand=True)
        self.log_text.config(state=tk.DISABLED)
        
        # Add initial log message
        self.log("HC05 Serial Testing Tool ready.")
        self.log("1. Pair your HC05 with Windows first before connecting")
        self.log("2. For AT commands, put your HC05 in AT mode (special setup required)")
        self.log("3. Click 'Auto-Scan for HC05' to automatically detect HC05 devices")

    def create_port_list(self):
        """Create the port list frame and tree"""
        list_frame = ttk.LabelFrame(self.connection_tab, text="Available COM Ports")
        list_frame.pack(fill=tk.BOTH, expand=True, pady=5, padx=5)
        
        # Create Treeview for port list
        columns = ("port", "description", "hwid", "device")
        self.port_tree = ttk.Treeview(list_frame, columns=columns, show="headings", selectmode="browse")
        
        # Define headings
        self.port_tree.heading("port", text="Port")
        self.port_tree.heading("description", text="Description")
        self.port_tree.heading("hwid", text="Hardware ID")
        self.port_tree.heading("device", text="Device")
        
        # Define columns
        self.port_tree.column("port", width=80)
        self.port_tree.column("description", width=200)
        self.port_tree.column("hwid", width=150)
        self.port_tree.column("device", width=100)
        
        # Add scrollbar
        scrollbar = ttk.Scrollbar(list_frame, orient=tk.VERTICAL, command=self.port_tree.yview)
        self.port_tree.configure(yscroll=scrollbar.set)
        
        # Pack widgets
        self.port_tree.pack(side=tk.LEFT, fill=tk.BOTH, expand=True)
        scrollbar.pack(side=tk.RIGHT, fill=tk.Y)
        
        # Bind selection event
        self.port_tree.bind("<<TreeviewSelect>>", self.on_port_select)

    def setup_serial_testing_tab(self):
        """Setup the serial testing tab for TX/RX testing"""
        # Create frames
        send_frame = ttk.LabelFrame(self.serial_testing_tab, text="Send Data")
        send_frame.pack(fill=tk.X, pady=5, padx=5)
        
        # Send text input
        input_frame = ttk.Frame(send_frame)
        input_frame.pack(fill=tk.X, pady=5, padx=5)
        
        self.data_entry = ttk.Entry(input_frame, width=60)
        self.data_entry.pack(side=tk.LEFT, fill=tk.X, expand=True, padx=5)
        
        self.send_button = ttk.Button(input_frame, text="Send", command=self.send_data)
        self.send_button.pack(side=tk.LEFT, padx=5)
        
        # Quick send buttons
        quick_frame = ttk.Frame(send_frame)
        quick_frame.pack(fill=tk.X, pady=5)
        
        ttk.Label(quick_frame, text="Quick Send:").pack(side=tk.LEFT, padx=5)
        
        test_messages = ["Hello", "Test", "OK", "1234", "ABCD"]
        for msg in test_messages:
            btn = ttk.Button(quick_frame, text=msg, 
                           command=lambda m=msg: self.quick_send(m))
            btn.pack(side=tk.LEFT, padx=2)
        
        # HEX input option
        hex_frame = ttk.Frame(send_frame)
        hex_frame.pack(fill=tk.X, pady=5)
        
        ttk.Label(hex_frame, text="Send Hex:").pack(side=tk.LEFT, padx=5)
        self.hex_entry = ttk.Entry(hex_frame, width=40)
        self.hex_entry.pack(side=tk.LEFT, fill=tk.X, expand=True, padx=5)
        self.hex_entry.insert(0, "48 65 6C 6C 6F")  # "Hello" in hex
        
        hex_send_button = ttk.Button(hex_frame, text="Send Hex", 
                                   command=self.send_hex_data)
        hex_send_button.pack(side=tk.LEFT, padx=5)
        
        # Received data display
        receive_frame = ttk.LabelFrame(self.serial_testing_tab, text="Received Data")
        receive_frame.pack(fill=tk.BOTH, expand=True, pady=5, padx=5)
        
        display_frame = ttk.Frame(receive_frame)
        display_frame.pack(fill=tk.X, pady=5)
        
        ttk.Label(display_frame, text="Display As:").pack(side=tk.LEFT, padx=5)
        self.display_mode_var = tk.StringVar(value="TEXT")
        ttk.Radiobutton(display_frame, text="Text", variable=self.display_mode_var, 
                       value="TEXT").pack(side=tk.LEFT, padx=5)
        ttk.Radiobutton(display_frame, text="Hex", variable=self.display_mode_var, 
                       value="HEX").pack(side=tk.LEFT, padx=5)
        ttk.Radiobutton(display_frame, text="Both", variable=self.display_mode_var, 
                       value="BOTH").pack(side=tk.LEFT, padx=5)
        
        clear_button = ttk.Button(display_frame, text="Clear", 
                                command=self.clear_receive_text)
        clear_button.pack(side=tk.RIGHT, padx=5)
        
        # Received text area
        self.receive_text = scrolledtext.ScrolledText(receive_frame, height=15, width=80, wrap=tk.WORD)
        self.receive_text.pack(fill=tk.BOTH, expand=True, pady=5, padx=5)

    def setup_at_command_tab(self):
        """Setup the AT command testing tab"""
        # Information frame for AT mode
        info_frame = ttk.LabelFrame(self.at_command_tab, text="AT Mode Information")
        info_frame.pack(fill=tk.X, pady=5, padx=5)
        
        info_text = (
            "HC05 AT Mode Instructions:\n\n"
            "1. To enter AT mode, power on HC05 with KEY/EN pin HIGH\n"
            "   - This often requires hardware modification or a special breakout board\n"
            "2. AT mode typically uses 38400 baud rate (change in Connection tab)\n"
            "3. All AT commands need to end with CR+LF (\\r\\n)\n"
            "4. Begin with simple 'AT' command to test connection\n\n"
            "Note: Some HC05 modules have a button to enter AT mode. Hold it while powering on."
        )
        
        info_label = ttk.Label(info_frame, text=info_text, justify=tk.LEFT, wraplength=700)
        info_label.pack(pady=10, padx=10)
        
        # Command input
        cmd_frame = ttk.LabelFrame(self.at_command_tab, text="Command Input")
        cmd_frame.pack(fill=tk.X, pady=5, padx=5)
        
        input_frame = ttk.Frame(cmd_frame)
        input_frame.pack(fill=tk.X, pady=5)
        
        ttk.Label(input_frame, text="AT Command:").pack(side=tk.LEFT, padx=5)
        self.at_command_entry = ttk.Entry(input_frame, width=50)
        self.at_command_entry.pack(side=tk.LEFT, fill=tk.X, expand=True, padx=5)
        self.at_command_entry.insert(0, "AT")
        
        at_send_button = ttk.Button(input_frame, text="Send", 
                                  command=self.send_at_command)
        at_send_button.pack(side=tk.LEFT, padx=5)
        
        # Common AT commands
        common_cmds_frame = ttk.LabelFrame(self.at_command_tab, text="Common AT Commands")
        common_cmds_frame.pack(fill=tk.X, pady=5, padx=5)
        
        # Define common AT commands with descriptions
        common_cmds = [
            ("AT", "Test connection"),
            ("AT+VERSION?", "Get firmware version"),
            ("AT+NAME?", "Get module name"),
            ("AT+ADDR?", "Get Bluetooth address"),
            ("AT+PSWD?", "Get pairing password"),
            ("AT+UART?", "Get UART parameters"),
            ("AT+NAME=HC05", "Set module name to HC05"),
            ("AT+PSWD=1234", "Set password to 1234"),
            ("AT+ROLE=0", "Set role as slave (0=slave, 1=master)"),
            ("AT+CMODE=0", "Set connection mode (0=fixed addr, 1=any addr)"),
            ("AT+UART=9600,0,0", "Set baud rate to 9600"),
            ("AT+RESET", "Reset module")
        ]
        
        # Create buttons for common commands in a grid
        row = 0
        col = 0
        for cmd, desc in common_cmds:
            cmd_button = ttk.Button(common_cmds_frame, text=cmd,
                                  command=lambda c=cmd: self.at_command_entry.delete(0, tk.END) or 
                                                      self.at_command_entry.insert(0, c))
            cmd_button.grid(row=row, column=col*2, padx=5, pady=2, sticky=tk.W)
            
            ttk.Label(common_cmds_frame, text=f"- {desc}").grid(row=row, column=col*2+1, 
                                                              padx=5, pady=2, sticky=tk.W)
            
            row += 1
            if row > 5:  # After 6 rows, start a new column
                row = 0
                col += 1
        
        # AT response area
        response_frame = ttk.LabelFrame(self.at_command_tab, text="AT Response")
        response_frame.pack(fill=tk.BOTH, expand=True, pady=5, padx=5)
        
        # Toolbar
        toolbar_frame = ttk.Frame(response_frame)
        toolbar_frame.pack(fill=tk.X)
        
        clear_button = ttk.Button(toolbar_frame, text="Clear Responses", 
                                command=self.clear_at_response)
        clear_button.pack(side=tk.RIGHT, padx=5)
        
        # Response text area
        self.at_response_text = scrolledtext.ScrolledText(response_frame, height=12, width=80, wrap=tk.WORD)
        self.at_response_text.pack(fill=tk.BOTH, expand=True, pady=5, padx=5)
        
    def on_tab_change(self, event):
        """Handle tab change event"""
        selected_tab = self.notebook.index(self.notebook.select())
        
        # If changed to Serial Testing tab, focus on data entry
        if selected_tab == 1 and self.connected:
            self.data_entry.focus()
        # If changed to AT Command tab, focus on AT command entry
        elif selected_tab == 2 and self.connected:
            self.at_command_entry.focus()

    def log(self, message):
        """Add a message to the log with timestamp"""
        timestamp = time.strftime("%H:%M:%S", time.localtime())
        log_message = f"[{timestamp}] {message}\n"
        
        self.log_text.config(state=tk.NORMAL)
        self.log_text.insert(tk.END, log_message)
        self.log_text.see(tk.END)
        self.log_text.config(state=tk.DISABLED)

    def refresh_ports(self):
        """Refresh the list of available COM ports"""
        # Clear existing ports
        for item in self.port_tree.get_children():
            self.port_tree.delete(item)
        self.ports = {}
        
        # Get all COM ports
        ports = list(serial.tools.list_ports.comports())
        
        # If no ports found
        if not ports:
            self.log("No COM ports found. Make sure your device is connected and paired.")
            return
            
        # Add ports to the list
        for port in ports:
            port_name = port.device
            description = port.description
            hwid = port.hwid
            
            # Try to identify if it's an HC05
            is_hc05 = False
            device_type = "Unknown"
            
            # Check for common HC05 identifiers in the description
            if any(x in description.lower() for x in ['hc05', 'hc-05', 'bt', 'bluetooth', 'serial']):
                is_hc05 = True
                device_type = "HC05 (Likely)"
            elif 'com' in port_name.lower():
                device_type = "Serial"
                
            # Store port info
            self.ports[port_name] = {
                'name': port_name,
                'description': description,
                'hwid': hwid,
                'is_hc05': is_hc05,
                'device_type': device_type
            }
            
            # Add to UI
            values = (port_name, description, hwid, device_type)
            
            # Use tags for HC05 devices
            tags = ("standard",)
            if is_hc05:
                tags = ("hc05",)
                
            self.port_tree.insert('', tk.END, port_name, values=values, tags=tags)
            
        # Configure tag for HC05 devices
        self.port_tree.tag_configure('hc05', background='#ffd700')
        
        self.log(f"Found {len(ports)} COM port(s)")

    def auto_scan_for_hc05(self):
        """Automatically scan for and identify HC05 devices by trying to connect"""
        if self.scanning:
            return
            
        self.scanning = True
        self.status_var.set("Scanning for HC05...")
        self.scan_button.config(state=tk.DISABLED)
        
        # Start scanning thread
        scan_thread = threading.Thread(target=self.auto_scan_thread)
        scan_thread.daemon = True
        scan_thread.start()

    def auto_scan_thread(self):
        """Thread function to scan for HC05 devices"""
        try:
            # Refresh ports first
            self.root.after(0, self.refresh_ports)
            time.sleep(1)
            
            # Get all ports
            ports = list(self.ports.keys())
            hc05_found = False
            
            # Try common baud rates for HC05
            baud_rates = [9600, 38400, 57600, 115200]
            
            self.log("Auto-scanning COM ports for HC05 devices...")
            
            # Try each port
            for port_name in ports:
                self.root.after(0, lambda p=port_name: self.update_scan_status(f"Trying {p}..."))
                
                port_info = self.ports[port_name]
                
                # Skip already identified HC05 devices
                if port_info['is_hc05']:
                    self.log(f"Port {port_name} is already identified as HC05")
                    hc05_found = True
                    continue
                    
                # Try each baud rate
                for baud in baud_rates:
                    try:
                        # Try to open the port
                        ser = serial.Serial(port_name, baud, timeout=1)
                        
                        # Send AT command to check if it's HC05
                        ser.write(b"AT\r\n")
                        time.sleep(0.5)
                        
                        # Read response
                        response = ser.read(10)
                        
                        # Check if it's HC05
                        if b"OK" in response:
                            # It's an HC05!
                            self.log(f"Found HC05 on {port_name} at {baud} baud!")
                            
                            # Update port info
                            port_info['is_hc05'] = True
                            port_info['device_type'] = f"HC05 ({baud} baud)"
                            
                            # Update UI in main thread
                            self.root.after(0, lambda p=port_name, d=f"HC05 ({baud} baud)": 
                                          self.update_port_as_hc05(p, d))
                            
                            hc05_found = True
                            
                        # Close the port
                        ser.close()
                        
                        # Small delay
                        time.sleep(0.5)
                        
                    except Exception as e:
                        # Move on to the next baud rate or port
                        continue
            
            # Scanning complete
            if hc05_found:
                self.root.after(0, lambda: self.update_scan_status("HC05 devices found!"))
            else:
                self.root.after(0, lambda: self.update_scan_status("No HC05 devices detected"))
                self.log("No HC05 devices found. Make sure your HC05 is paired with Windows.")
                self.log("If already paired, select the COM port manually and try to connect.")
            
        except Exception as e:
            self.log(f"Auto-scan error: {str(e)}")
            self.root.after(0, lambda: self.update_scan_status("Scan error"))
            
        finally:
            # Reset scanning state
            self.scanning = False
            self.root.after(0, lambda: self.scan_button.config(state=tk.NORMAL))

    def update_scan_status(self, status):
        """Update scan status (called from main thread)"""
        self.status_var.set(status)

    def update_port_as_hc05(self, port_name, device_type):
        """Update a port as HC05 in the UI (called from main thread)"""
        # Update the tree item
        self.port_tree.item(port_name, tags=("hc05",))
        
        # Update the device type in the tree
        values = list(self.port_tree.item(port_name, 'values'))
        values[3] = device_type
        self.port_tree.item(port_name, values=values)

    def on_port_select(self, event):
        """Handle port selection from the list"""
        selection = self.port_tree.selection()
        if selection:
            port_name = selection[0]
            self.selected_port = port_name
            self.status_var.set(f"Selected: {port_name}")
            self.connect_button.config(state=tk.NORMAL)
        else:
            self.selected_port = None
            self.connect_button.config(state=tk.DISABLED)

    def connect_port(self):
        """Connect to the selected serial port"""
        if not self.selected_port:
            return
            
        port_name = self.selected_port
        baud_rate = int(self.baudrate_var.get())
        
        # Start connection in a separate thread
        self.log(f"Connecting to {port_name} at {baud_rate} baud...")
        self.status_var.set(f"Connecting...")
        
        # Disable buttons during connection
        self.connect_button.config(state=tk.DISABLED)
        
        conn_thread = threading.Thread(target=self.connect_thread, args=(port_name, baud_rate))
        conn_thread.daemon = True
        conn_thread.start()

    def connect_thread(self, port_name, baud_rate):
        """Handle port connection in a separate thread"""
        try:
            # Try to open the serial port
            self.ser = serial.Serial(port_name, baud_rate, timeout=1)
            
            # If we get here, connection was successful
            self.connected = True
            
            # Start reading thread
            self.keep_reading = True
            self.reading_thread = threading.Thread(target=self.read_serial)
            self.reading_thread.daemon = True
            self.reading_thread.start()
            
            # Update UI from main thread
            self.root.after(0, self.connection_successful, port_name, baud_rate)
            
        except Exception as e:
            # Update UI from main thread
            self.root.after(0, self.connection_failed, str(e))
            
            if self.ser:
                try:
                    self.ser.close()
                except:
                    pass
                self.ser = None

    def connection_successful(self, port_name, baud_rate):
        """Handle successful connection (called from main thread)"""
        self.log(f"Successfully connected to {port_name} at {baud_rate} baud")
        self.status_var.set(f"Connected: {port_name}")
        
        # Update UI
        self.conn_port_var.set(f"{port_name} ({baud_rate} baud)")
        self.conn_status_var.set("Connected")
        
        # Enable other tabs
        self.notebook.tab(1, state="normal")
        self.notebook.tab(2, state="normal")
        
        # Update button states
        self.connect_button.config(state=tk.DISABLED)
        self.disconnect_button.config(state=tk.NORMAL)
        
        # Highlight connected port in the list
        self.port_tree.item(port_name, tags=("connected",))
        self.port_tree.tag_configure("connected", background="#aaffaa")
        
        # Clear receive areas
        self.clear_receive_text()
        self.clear_at_response()
        
        # Welcome message
        if self.ports[port_name]['is_hc05']:
            welcome_msg = (
                "Connected to HC05 device. You can now:\n"
                "1. Send/receive data in the 'Serial Testing' tab\n"
                "2. Test AT commands in the 'AT Commands' tab (requires HC05 to be in AT mode)"
            )
        else:
            welcome_msg = (
                "Connected to serial device. You can now:\n"
                "1. Send/receive data in the 'Serial Testing' tab\n"
                "2. If this is an HC05 in AT mode, use the 'AT Commands' tab"
            )
            
        self.log(welcome_msg)

    def connection_failed(self, error_msg):
        """Handle failed connection (called from main thread)"""
        self.log(f"Connection failed: {error_msg}")
        self.status_var.set("Connection failed")
        
        # Reset connection status
        self.connected = False
        self.ser = None
        
        # Update UI
        if self.selected_port:
            self.connect_button.config(state=tk.NORMAL)
            
        messagebox.showerror("Connection Error", 
                           f"Could not connect to the port.\n\nError: {error_msg}\n\n" + 
                           "For HC-05 modules, make sure:\n" +
                           "1. The module is powered on\n" +
                           "2. It's paired with your computer\n" +
                           "3. Try different baud rates (typically 9600 or 38400)\n" +
                           "4. The COM port is not in use by another application")

    def read_serial(self):
        """Read data from the serial port in a separate thread"""
        receive_buffer = bytearray()
        
        while self.keep_reading and self.ser and self.ser.is_open:
            try:
                # Check if there's data available
                if self.ser.in_waiting > 0:
                    # Read data
                    data = self.ser.read(self.ser.in_waiting)
                    receive_buffer.extend(data)
                    
                    # Process and display data in main thread
                    if receive_buffer:
                        # Make a copy of the buffer to send to the main thread
                        data_to_display = bytes(receive_buffer)
                        self.root.after(0, lambda d=data_to_display: self.display_received_data(d))
                        receive_buffer.clear()
                
                # Small delay to prevent CPU hogging
                time.sleep(0.1)
                
            except Exception as e:
                # Connection might be lost
                if self.connected:  # Check to avoid redundant error messages
                    self.root.after(0, lambda err=str(e): 
                                  self.log(f"Error reading from serial port: {err}"))
                    self.root.after(0, self.disconnect_port)
                break

    def display_received_data(self, data):
        """Display received data in the UI (called from main thread)"""
        if not data:
            return
            
        # Get current tab
        current_tab = self.notebook.index(self.notebook.select())
        
        # Get display mode
        display_mode = self.display_mode_var.get()
        
        # Convert data to string and hex
        try:
            text_data = data.decode('utf-8', errors='replace')
        except:
            text_data = "<?>"
            
        hex_data = ' '.join([f'{b:02X}' for b in data])
        
        # Format according to display mode
        if display_mode == "TEXT":
            display_text = text_data
        elif display_mode == "HEX":
            display_text = hex_data
        else:  # BOTH
            display_text = f"{text_data}\n[HEX: {hex_data}]"
        
        timestamp = time.strftime("%H:%M:%S", time.localtime())
        
        # Add to appropriate text area based on current tab
        if current_tab == 1:  # Serial Testing tab
            self.receive_text.insert(tk.END, f"[{timestamp}] {display_text}\n")
            self.receive_text.see(tk.END)
        elif current_tab == 2:  # AT Command tab
            self.at_response_text.insert(tk.END, f"[{timestamp}] {display_text}\n")
            self.at_response_text.see(tk.END)
        
        # Also log the data
        self.log(f"Received: {text_data}")

    def disconnect_port(self):
        """Disconnect from current port"""
        # Stop reading thread
        self.keep_reading = False
        if self.reading_thread:
            self.reading_thread.join(timeout=1.0)
            
        # Close serial port
        if self.ser:
            try:
                self.ser.close()
            except:
                pass
                
        self.ser = None
        self.connected = False
        
        # Update UI
        self.log("Disconnected from port")
        self.status_var.set("Disconnected")
        self.conn_port_var.set("None")
        self.conn_status_var.set("Disconnected")
        
        # Disable tabs
        self.notebook.tab(1, state="disabled")
        self.notebook.tab(2, state="disabled")
        
        # Switch to connection tab
        self.notebook.select(0)
        
        # Update button states
        self.disconnect_button.config(state=tk.DISABLED)
        
        # Reset port list highlighting
        if self.selected_port:
            # Determine the appropriate tag for the port
            if self.ports[self.selected_port]['is_hc05']:
                self.port_tree.item(self.selected_port, tags=("hc05",))
            else:
                self.port_tree.item(self.selected_port, tags=("standard",))
            self.connect_button.config(state=tk.NORMAL)

    def send_data(self):
        """Send data from the entry field"""
        if not self.connected or not self.ser:
            messagebox.showerror("Error", "Not connected to any port")
            return
            
        data = self.data_entry.get()
        if not data:
            messagebox.showwarning("Warning", "Please enter data to send")
            return
            
        # Send the data
        self.send_raw_data(data)
        
        # Clear entry field
        self.data_entry.delete(0, tk.END)
        
        # Set focus back to entry field
        self.data_entry.focus()

    def quick_send(self, message):
        """Send a predefined message"""
        if not self.connected or not self.ser:
            messagebox.showerror("Error", "Not connected to any port")
            return
            
        # Send the message
        self.send_raw_data(message)

    def send_hex_data(self):
        """Send data from the hex entry field"""
        if not self.connected or not self.ser:
            messagebox.showerror("Error", "Not connected to any port")
            return
            
        hex_data = self.hex_entry.get()
        if not hex_data:
            messagebox.showwarning("Warning", "Please enter hex data to send")
            return
            
        try:
            # Parse hex string
            hex_bytes = bytearray()
            for hex_str in hex_data.split():
                hex_bytes.append(int(hex_str, 16))
                
            # Send the data
            self.ser.write(hex_bytes)
            
            # Log the sent data
            self.log(f"Sent (hex): {' '.join([f'{b:02X}' for b in hex_bytes])}")
            
        except Exception as e:
            self.log(f"Error sending hex data: {str(e)}")
            messagebox.showerror("Send Error", f"Could not send hex data: {str(e)}")

    def send_at_command(self):
        """Send an AT command"""
        if not self.connected or not self.ser:
            messagebox.showerror("Error", "Not connected to any port")
            return
            
        cmd = self.at_command_entry.get()
        if not cmd:
            messagebox.showwarning("Warning", "Please enter an AT command")
            return
            
        # Always add CRLF for AT commands
        self.send_raw_data(cmd, ending="CRLF")
        
        # Set focus back to entry field
        self.at_command_entry.focus()

    def send_raw_data(self, data, ending=None):
        """Send raw data with specified line ending"""
        try:
            # Use specified ending or get from UI
            if not ending:
                ending = self.line_ending_var.get()
                
            # Add appropriate line ending
            if ending == "CR":
                data += "\r"
            elif ending == "LF":
                data += "\n"
            elif ending == "CRLF":
                data += "\r\n"
                
            # Send data as bytes
            self.ser.write(data.encode('utf-8'))
            
            # Log the sent data
            self.log(f"Sent: {data}")
            
        except Exception as e:
            self.log(f"Send error: {str(e)}")
            messagebox.showerror("Send Error", f"Could not send data: {str(e)}")
            
            # Connection might be lost
            if "device disconnected" in str(e).lower() or "port is closed" in str(e).lower():
                self.disconnect_port()

    def clear_receive_text(self):
        """Clear the receive text area"""
        self.receive_text.delete(1.0, tk.END)

    def clear_at_response(self):
        """Clear the AT response text area"""
        self.at_response_text.delete(1.0, tk.END)

    def check_ui_updates(self):
        """Periodically check for UI updates"""
        # Schedule the next check
        self.root.after(100, self.check_ui_updates)

    def on_closing(self):
        """Handle window closing"""
        # Disconnect if connected
        if self.connected and self.ser:
            self.disconnect_port()
                
        # Close window
        self.root.destroy()
        sys.exit(0)


if __name__ == "__main__":
    # Check requirements
    try:
        import serial
    except ImportError:
        print("ERROR: PySerial library not found. Please install it with:")
        print("pip install pyserial")
        sys.exit(1)
        
    # Create the main window
    root = tk.Tk()
    app = HC05SerialGUI(root)
    
    # Handle window close event
    root.protocol("WM_DELETE_WINDOW", app.on_closing)
    
    # Start the application
    root.mainloop()