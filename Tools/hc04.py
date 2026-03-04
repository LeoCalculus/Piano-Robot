import tkinter as tk
from tkinter import ttk, messagebox, scrolledtext, filedialog
import threading
import time
import serial
import serial.tools.list_ports
import sys
import os
import struct
import ctypes

# File Transfer Protocol Constants
FT_CMD_FILE_START = 0xF0
FT_CMD_FILE_DATA = 0xF1
FT_CMD_FILE_END = 0xF2
FT_CMD_FILE_ABORT = 0xF3

FT_RSP_ACK = 0xA0
FT_RSP_NAK = 0xA1
FT_RSP_READY = 0xA2
FT_RSP_ERROR = 0xA3
FT_RSP_SUCCESS = 0xA4

FT_CHUNK_SIZE = 64  # Match STM32 FT_MAX_CHUNK_SIZE

# RAM Transfer Sentinel Constants
RAM_SENTINEL_START = struct.pack('<f', 514.114)
RAM_SENTINEL_END = struct.pack('<f', 114.514)
RAM_BT_CHUNK_SIZE = 64  # bytes per BT write to avoid overflow


class HC04SerialGUI:
    def __init__(self, root):
        self.root = root
        self.root.title("HC-04 Bluetooth Serial Tool")
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
        self.connection_timeout = 3  # seconds

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

        title_label = ttk.Label(header_frame, text="HC-04 Bluetooth Serial Tool",
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

        # Create tabs (no AT Commands tab for HC-04)
        self.connection_tab = ttk.Frame(self.notebook)
        self.serial_testing_tab = ttk.Frame(self.notebook)
        self.file_upload_tab = ttk.Frame(self.notebook)
        self.control_tab = ttk.Frame(self.notebook)
        self.command_tab = ttk.Frame(self.notebook)

        # Add tabs to notebook
        self.notebook.add(self.connection_tab, text="Connection")
        self.notebook.add(self.serial_testing_tab, text="Serial Testing")
        self.notebook.add(self.file_upload_tab, text="File Upload")
        self.notebook.add(self.control_tab, text="Control")
        self.notebook.add(self.command_tab, text="Command")

        # Set up each tab
        self.setup_connection_tab()
        self.setup_serial_testing_tab()
        self.setup_file_upload_tab()
        self.setup_control_tab()
        self.setup_command_tab()

        # Disable tabs until connected
        self.notebook.tab(1, state="disabled")
        self.notebook.tab(2, state="disabled")
        self.notebook.tab(3, state="disabled")
        self.notebook.tab(4, state="disabled")

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
        self.baudrate_var = tk.StringVar(value="921600")
        baudrate_combo = ttk.Combobox(baud_frame, textvariable=self.baudrate_var,
                                     values=["9600", "38400", "57600", "115200", "460800", "921600"], width=10)
        baudrate_combo.pack(side=tk.LEFT, padx=5)

        # Connection timeout
        ttk.Label(baud_frame, text="Timeout (s):").pack(side=tk.LEFT, padx=15)
        self.timeout_var = tk.StringVar(value="3")
        timeout_spin = ttk.Spinbox(baud_frame, from_=1, to=10, width=5,
                                   textvariable=self.timeout_var)
        timeout_spin.pack(side=tk.LEFT, padx=5)

        # Line Endings
        ending_frame = ttk.Frame(settings_frame)
        ending_frame.pack(fill=tk.X, pady=5)

        ttk.Label(ending_frame, text="Line Ending:").pack(side=tk.LEFT, padx=5)
        self.line_ending_var = tk.StringVar(value="CRLF")
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
        self.log("HC-04 Bluetooth Serial Tool ready.")
        self.log("1. Pair your HC-04 with Windows first before connecting")
        self.log("2. Select the Bluetooth COM port from the list")
        self.log("3. HC-04 uses transparent serial mode (no AT commands)")

    def create_port_list(self):
        """Create the port list frame and tree"""
        list_frame = ttk.LabelFrame(self.connection_tab, text="Available COM Ports (Bluetooth Classic)")
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
        self.data_entry.bind("<Return>", lambda e: self.send_data())

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

    def setup_file_upload_tab(self):
        """Setup the file upload tab for TXT file transfer"""
        # File selection frame
        file_frame = ttk.LabelFrame(self.file_upload_tab, text="Select TXT File")
        file_frame.pack(fill=tk.X, pady=5, padx=5)

        select_frame = ttk.Frame(file_frame)
        select_frame.pack(fill=tk.X, pady=5, padx=5)

        self.file_path_var = tk.StringVar(value="No file selected")
        ttk.Label(select_frame, textvariable=self.file_path_var, width=60).pack(
            side=tk.LEFT, padx=5)

        ttk.Button(select_frame, text="Browse...",
                   command=self.browse_txt_file).pack(side=tk.LEFT, padx=5)

        # File info frame
        info_frame = ttk.LabelFrame(self.file_upload_tab, text="File Information")
        info_frame.pack(fill=tk.X, pady=5, padx=5)

        self.file_info_var = tk.StringVar(value="Select a TXT file to see details")
        ttk.Label(info_frame, textvariable=self.file_info_var, justify=tk.LEFT).pack(
            pady=10, padx=10)

        # Upload controls frame
        control_frame = ttk.LabelFrame(self.file_upload_tab, text="Upload Control")
        control_frame.pack(fill=tk.X, pady=5, padx=5)

        btn_frame = ttk.Frame(control_frame)
        btn_frame.pack(fill=tk.X, pady=5)

        self.upload_button = ttk.Button(btn_frame, text="Upload to SD",
                                         command=self.start_upload, state=tk.DISABLED)
        self.upload_button.pack(side=tk.LEFT, padx=5)

        self.ram_upload_button = ttk.Button(btn_frame, text="Upload to RAM",
                                             command=self.start_ram_upload, state=tk.DISABLED)
        self.ram_upload_button.pack(side=tk.LEFT, padx=5)

        self.abort_button = ttk.Button(btn_frame, text="Abort",
                                        command=self.abort_upload, state=tk.DISABLED)
        self.abort_button.pack(side=tk.LEFT, padx=5)

        ttk.Button(btn_frame, text="Refresh File List",
                   command=self.refresh_device_files).pack(side=tk.LEFT, padx=5)

        # Progress frame
        progress_frame = ttk.LabelFrame(self.file_upload_tab, text="Upload Progress")
        progress_frame.pack(fill=tk.X, pady=5, padx=5)

        self.progress_var = tk.DoubleVar(value=0)
        self.progress_bar = ttk.Progressbar(progress_frame, variable=self.progress_var,
                                             maximum=100, length=400)
        self.progress_bar.pack(pady=10, padx=10, fill=tk.X)

        self.progress_label_var = tk.StringVar(value="Ready")
        ttk.Label(progress_frame, textvariable=self.progress_label_var).pack(pady=5)

        # Device files frame
        device_frame = ttk.LabelFrame(self.file_upload_tab, text="Files on Device")
        device_frame.pack(fill=tk.BOTH, expand=True, pady=5, padx=5)

        columns = ("filename", "size")
        self.device_files_tree = ttk.Treeview(device_frame, columns=columns,
                                               show="headings", height=6)
        self.device_files_tree.heading("filename", text="Filename")
        self.device_files_tree.heading("size", text="Size (bytes)")
        self.device_files_tree.column("filename", width=200)
        self.device_files_tree.column("size", width=100)
        self.device_files_tree.pack(fill=tk.BOTH, expand=True, padx=5, pady=5)

        # Upload log
        log_frame = ttk.LabelFrame(self.file_upload_tab, text="Upload Log")
        log_frame.pack(fill=tk.BOTH, expand=True, pady=5, padx=5)

        self.upload_log = scrolledtext.ScrolledText(log_frame, height=5, width=80)
        self.upload_log.pack(fill=tk.BOTH, expand=True)

        # Initialize upload state
        self.selected_file_path = None
        self.upload_abort_flag = False

    def setup_control_tab(self):
        """Setup the control tab for menu navigation via W/S/D/A keys"""
        # Track which keys are currently held to prevent repeat
        self.control_keys_held = set()

        # Instructions
        info_frame = ttk.LabelFrame(self.control_tab, text="Menu Control")
        info_frame.pack(fill=tk.X, pady=10, padx=10)

        ttk.Label(info_frame, text="W = Up, S = Down, D = Enter/Select, A = Back",
                  font=("Arial", 11)).pack(pady=10, padx=10)
        ttk.Label(info_frame, text="Click this area first to capture keyboard input.",
                  foreground="gray").pack(pady=(0, 10), padx=10)

        # Status display
        self.control_status_var = tk.StringVar(value="Waiting for input...")
        status_label = ttk.Label(info_frame, textvariable=self.control_status_var,
                                 font=("Arial", 14, "bold"), foreground=self.header_color)
        status_label.pack(pady=10)

        # Visual buttons (also clickable)
        btn_frame = ttk.Frame(info_frame)
        btn_frame.pack(pady=10)

        up_btn = ttk.Button(btn_frame, text="W  Up", width=15,
                            command=lambda: self.control_send(":w", "Up"))
        up_btn.pack(pady=5)

        down_btn = ttk.Button(btn_frame, text="S  Down", width=15,
                              command=lambda: self.control_send(":s", "Down"))
        down_btn.pack(pady=5)

        enter_btn = ttk.Button(btn_frame, text="D  Enter/Select", width=15,
                               command=lambda: self.control_send(":d", "Enter"))
        enter_btn.pack(pady=5)

        back_btn = ttk.Button(btn_frame, text="A  Back", width=15,
                              command=lambda: self.control_send(":a", "Back"))
        back_btn.pack(pady=5)

        # Log for control actions
        log_frame = ttk.LabelFrame(self.control_tab, text="Control Log")
        log_frame.pack(fill=tk.BOTH, expand=True, pady=5, padx=10)

        self.control_log = scrolledtext.ScrolledText(log_frame, height=10, width=80)
        self.control_log.pack(fill=tk.BOTH, expand=True)

        # Bind key events to the root window; handler checks active tab
        self.root.bind("<KeyPress>", self.control_key_press)
        self.root.bind("<KeyRelease>", self.control_key_release)
        # Right Shift to toggle mouse tracking off from anywhere
        self.root.bind("<KeyPress-Shift_R>", lambda _: self._rshift_stop_mouse())

    def control_send(self, cmd, direction):
        """Send a control command over serial"""
        if not self.connected or not self.ser:
            return
        try:
            self.send_raw_data(cmd)
            self.control_status_var.set(f"Sent: {direction}")
            timestamp = time.strftime("%H:%M:%S")
            self.control_log.insert(tk.END, f"[{timestamp}] {direction} ({cmd})\n")
            self.control_log.see(tk.END)
        except Exception as e:
            self.control_status_var.set(f"Error: {e}")

    def control_key_press(self, event):
        """Handle key press — only fire once per physical press"""
        # Only respond when the Control tab is active
        if self.notebook.index(self.notebook.select()) != 3:
            return
        if not self.connected:
            return

        key = event.keysym.lower()
        if key in self.control_keys_held:
            return  # already held, ignore auto-repeat

        self.control_keys_held.add(key)

        if key == 'w':
            self.control_send(":w", "Up")
        elif key == 's':
            self.control_send(":s", "Down")
        elif key == 'd':
            self.control_send(":d", "Enter")
        elif key == 'a':
            self.control_send(":a", "Back")

    def control_key_release(self, event):
        """Handle key release — re-enable the key for next press"""
        key = event.keysym.lower()
        self.control_keys_held.discard(key)

    # ========== Command Tab ==========

    def setup_command_tab(self):
        """Setup the command tab with manual commands and mouse distance tracking"""
        # --- Upper Section: Manual Commands ---
        cmd_frame = ttk.LabelFrame(self.command_tab, text="Commands")
        cmd_frame.pack(fill=tk.X, pady=5, padx=10)

        # Raw command entry
        entry_frame = ttk.Frame(cmd_frame)
        entry_frame.pack(fill=tk.X, pady=5, padx=5)

        self.cmd_entry = ttk.Entry(entry_frame, width=40)
        self.cmd_entry.pack(side=tk.LEFT, fill=tk.X, expand=True, padx=5)
        self.cmd_entry.bind("<Return>", lambda e: self.cmd_send_raw())

        ttk.Button(entry_frame, text="Send", command=self.cmd_send_raw).pack(side=tk.LEFT, padx=5)

        # Quick command buttons
        quick_frame = ttk.Frame(cmd_frame)
        quick_frame.pack(fill=tk.X, pady=5, padx=5)

        ttk.Label(quick_frame, text="Quick:").pack(side=tk.LEFT, padx=5)
        ttk.Button(quick_frame, text=";E Halt",
                   command=lambda: self.cmd_quick(";E")).pack(side=tk.LEFT, padx=2)
        ttk.Button(quick_frame, text=";F Toggle PID",
                   command=lambda: self.cmd_quick(";F")).pack(side=tk.LEFT, padx=2)

        # ;D distance entry
        dist_frame = ttk.Frame(cmd_frame)
        dist_frame.pack(fill=tk.X, pady=5, padx=5)

        ttk.Label(dist_frame, text=";D Distance (cm):").pack(side=tk.LEFT, padx=5)
        self.cmd_dist_entry = ttk.Entry(dist_frame, width=10)
        self.cmd_dist_entry.pack(side=tk.LEFT, padx=5)
        self.cmd_dist_entry.bind("<Return>", lambda e: self.cmd_send_dist())
        ttk.Button(dist_frame, text="Send ;D", command=self.cmd_send_dist).pack(side=tk.LEFT, padx=5)

        # --- Command Output Log ---
        cmd_log_frame = ttk.LabelFrame(self.command_tab, text="Command Output")
        cmd_log_frame.pack(fill=tk.X, pady=5, padx=10)

        self.cmd_log = scrolledtext.ScrolledText(cmd_log_frame, height=6, width=80, wrap=tk.WORD)
        self.cmd_log.pack(fill=tk.BOTH, expand=True, padx=5, pady=5)

        cmd_log_btn_frame = ttk.Frame(cmd_log_frame)
        cmd_log_btn_frame.pack(fill=tk.X, padx=5, pady=(0, 5))
        ttk.Button(cmd_log_btn_frame, text="Clear", command=lambda: self.cmd_log.delete(1.0, tk.END)).pack(side=tk.RIGHT)

        # --- Lower Section: Mouse Distance Control ---
        mouse_frame = ttk.LabelFrame(self.command_tab, text="Mouse Distance Control")
        mouse_frame.pack(fill=tk.BOTH, expand=True, pady=5, padx=10)

        # Controls row
        ctrl_frame = ttk.Frame(mouse_frame)
        ctrl_frame.pack(fill=tk.X, pady=5, padx=5)

        self.mouse_track_btn = ttk.Button(ctrl_frame, text="Start Tracking",
                                          command=self.toggle_mouse_tracking)
        self.mouse_track_btn.pack(side=tk.LEFT, padx=5)

        self.mouse_dist_label_var = tk.StringVar(value="0.00 cm")
        ttk.Label(ctrl_frame, text="Current:").pack(side=tk.LEFT, padx=(20, 5))
        ttk.Label(ctrl_frame, textvariable=self.mouse_dist_label_var,
                  font=("Consolas", 16, "bold"), foreground="#e74c3c").pack(side=tk.LEFT)

        ttk.Label(ctrl_frame, text="Range: 0.00 to -672.00 cm",
                  foreground="gray").pack(side=tk.RIGHT, padx=5)

        # Settings row: sample rate + sensitivity
        settings_frame = ttk.Frame(mouse_frame)
        settings_frame.pack(fill=tk.X, pady=5, padx=5)

        ttk.Label(settings_frame, text="Sample Rate:").pack(side=tk.LEFT, padx=5)
        self.mouse_rate_var = tk.StringVar(value="100 Hz")
        rate_combo = ttk.Combobox(settings_frame, textvariable=self.mouse_rate_var,
                                  values=["1 Hz", "10 Hz", "100 Hz"], width=8, state="readonly")
        rate_combo.pack(side=tk.LEFT, padx=5)

        ttk.Label(settings_frame, text="Sensitivity:").pack(side=tk.LEFT, padx=(20, 5))
        self.mouse_sensitivity_var = tk.DoubleVar(value=1.0)
        sens_scale = ttk.Scale(settings_frame, from_=0.1, to=5.0,
                               variable=self.mouse_sensitivity_var, orient=tk.HORIZONTAL,
                               length=150)
        sens_scale.pack(side=tk.LEFT, padx=5)
        self.mouse_sens_label_var = tk.StringVar(value="1.00x")
        ttk.Label(settings_frame, textvariable=self.mouse_sens_label_var,
                  font=("Consolas", 10)).pack(side=tk.LEFT, padx=5)

        # Tracking canvas (mouse input area)
        canvas_frame = ttk.Frame(mouse_frame)
        canvas_frame.pack(fill=tk.X, pady=5, padx=5)

        ttk.Label(canvas_frame, text="Move mouse horizontally in the area below:",
                  foreground="gray").pack(anchor=tk.W)

        self.mouse_canvas = tk.Canvas(canvas_frame, height=60, bg="#ecf0f1",
                                      cursor="sb_h_double_arrow", relief=tk.SUNKEN, bd=2)
        self.mouse_canvas.pack(fill=tk.X, pady=2)

        # Trace graph
        trace_frame = ttk.LabelFrame(mouse_frame, text="Position Trace (last 3s)")
        trace_frame.pack(fill=tk.BOTH, expand=True, pady=5, padx=5)

        self.trace_canvas = tk.Canvas(trace_frame, height=150, bg="#1a1a2e")
        self.trace_canvas.pack(fill=tk.BOTH, expand=True, padx=2, pady=2)

        # Mouse tracking state
        self.mouse_tracking = False
        self.mouse_distance_cm = 0.0
        self.mouse_prev_screen_x = None  # for delta tracking
        self.mouse_trace_data = []  # list of (timestamp, distance) tuples
        self.mouse_poll_timer = None
        self.mouse_draw_counter = 0

        # No canvas-only bindings — mouse is tracked globally in _mouse_poll_tick

    def cmd_send_raw(self):
        """Send raw command from entry"""
        if not self.connected or not self.ser:
            return
        cmd = self.cmd_entry.get().strip()
        if not cmd:
            return
        self.send_raw_data(cmd)
        self.cmd_log_message(cmd)
        self.cmd_entry.delete(0, tk.END)

    def cmd_quick(self, cmd):
        """Send a quick command"""
        if not self.connected or not self.ser:
            return
        self.send_raw_data(cmd)
        self.cmd_log_message(cmd)

    def cmd_send_dist(self):
        """Send ;D <distance> command"""
        if not self.connected or not self.ser:
            return
        try:
            dist = float(self.cmd_dist_entry.get())
            cmd = f";D{dist:.2f}"
            self.send_raw_data(cmd)
            self.cmd_log_message(cmd)
        except ValueError:
            pass

    def cmd_log_message(self, cmd):
        """Log a sent command to the Command Output area"""
        timestamp = time.strftime("%H:%M:%S")
        self.cmd_log.insert(tk.END, f"[{timestamp}] Sent: {cmd}\n")
        # Trim to last 200 lines
        line_count = int(self.cmd_log.index('end-1c').split('.')[0])
        if line_count > 200:
            self.cmd_log.delete('1.0', f'{line_count - 200}.0')
        self.cmd_log.see(tk.END)

    def _send_silent(self, text):
        """Send data over serial without logging (for high-frequency mouse sends)"""
        if not self.ser or not self.connected:
            return
        try:
            ending = self.line_ending_var.get()
            if ending == "CR":
                text += "\r"
            elif ending == "LF":
                text += "\n"
            elif ending == "CRLF":
                text += "\r\n"
            self.ser.write(text.encode('utf-8'))
        except Exception:
            pass

    def _rshift_stop_mouse(self):
        """Stop mouse tracking via Right Shift hotkey"""
        if self.mouse_tracking:
            self.stop_mouse_tracking()

    def toggle_mouse_tracking(self):
        """Toggle mouse tracking on/off"""
        if self.mouse_tracking:
            self.stop_mouse_tracking()
        else:
            self.start_mouse_tracking()

    def start_mouse_tracking(self):
        """Start mouse position polling and BT sending"""
        self.mouse_tracking = True
        self.mouse_track_btn.config(text="Stop Tracking")
        self.mouse_trace_data = []
        self.mouse_draw_counter = 0
        self.mouse_prev_screen_x = self.root.winfo_pointerx()
        self._mouse_poll_tick()

    def stop_mouse_tracking(self):
        """Stop mouse tracking"""
        self.mouse_tracking = False
        self.mouse_track_btn.config(text="Start Tracking")
        if self.mouse_poll_timer:
            self.root.after_cancel(self.mouse_poll_timer)
            self.mouse_poll_timer = None

    def _get_mouse_poll_ms(self):
        """Return poll interval in ms based on selected sample rate"""
        rate_str = self.mouse_rate_var.get()
        if rate_str == "1 Hz":
            return 1000
        elif rate_str == "10 Hz":
            return 100
        else:
            return 10  # 100 Hz

    def _mouse_poll_tick(self):
        """Sample mouse position, send ;D, update trace at selected rate"""
        if not self.mouse_tracking:
            return

        now = time.time()

        # Update sensitivity label
        sens = self.mouse_sensitivity_var.get()
        self.mouse_sens_label_var.set(f"{sens:.2f}x")

        # Delta-based tracking: accumulate mouse movement as distance
        canvas_width = self.mouse_canvas.winfo_width()
        screen_x = self.root.winfo_pointerx()

        if canvas_width > 1 and self.mouse_prev_screen_x is not None:
            delta_px = screen_x - self.mouse_prev_screen_x
            # Convert pixels to cm: full canvas width = 672.0 cm
            cm_per_px = 672.0 / canvas_width * sens
            self.mouse_distance_cm -= delta_px * cm_per_px  # right = more negative
            self.mouse_distance_cm = round(max(-672.0, min(0.0, self.mouse_distance_cm)), 2)

        # Warp cursor back to screen centre when it nears screen edges
        screen_w = self.root.winfo_screenwidth()
        margin = 50
        if screen_x <= margin or screen_x >= screen_w - margin:
            warp_x = screen_w // 2
            warp_y = self.root.winfo_pointery()
            ctypes.windll.user32.SetCursorPos(warp_x, warp_y)
            self.mouse_prev_screen_x = warp_x
        else:
            self.mouse_prev_screen_x = screen_x

        # Update distance label
        self.mouse_dist_label_var.set(f"{self.mouse_distance_cm:.2f} cm")

        # Draw indicator on mouse canvas
        self._draw_mouse_indicator()

        # Record trace data
        self.mouse_trace_data.append((now, self.mouse_distance_cm))

        # Trim to last 3 seconds
        cutoff = now - 3.0
        self.mouse_trace_data = [(t, d) for t, d in self.mouse_trace_data if t >= cutoff]

        # Redraw trace graph at ~33Hz max
        poll_ms = self._get_mouse_poll_ms()

        # Send ;D command via BT
        if self.connected and self.ser:
            cmd = f";D{self.mouse_distance_cm:.2f}"
            self._send_silent(cmd)
            # Log to command output at ~10Hz max to avoid flooding
            if not hasattr(self, '_mouse_log_counter'):
                self._mouse_log_counter = 0
            self._mouse_log_counter += 1
            log_every = max(1, (1000 // poll_ms) // 10)
            if self._mouse_log_counter % log_every == 0:
                self.cmd_log_message(cmd)
        self.mouse_draw_counter += 1
        draw_every = max(1, 30 // (1000 // poll_ms))  # ~33Hz cap
        if self.mouse_draw_counter % draw_every == 0:
            self._draw_trace()

        # Schedule next tick at selected rate
        self.mouse_poll_timer = self.root.after(poll_ms, self._mouse_poll_tick)

    def _draw_mouse_indicator(self):
        """Draw position indicator and scale on the mouse canvas"""
        self.mouse_canvas.delete("ind")
        cw = self.mouse_canvas.winfo_width()
        ch = self.mouse_canvas.winfo_height()
        if cw <= 1:
            return

        # Indicator line
        ratio = abs(self.mouse_distance_cm) / 672.0
        ix = ratio * cw
        self.mouse_canvas.create_line(ix, 0, ix, ch, fill="#e74c3c", width=2, tags="ind")

        # Distance text centered
        self.mouse_canvas.create_text(cw // 2, ch // 2,
                                      text=f"{self.mouse_distance_cm:.2f} cm",
                                      font=("Consolas", 12, "bold"),
                                      fill="#2c3e50", tags="ind")

        # Scale ticks at bottom
        for i in range(8):
            mx = (i / 7) * cw
            cm_val = -672.0 * (i / 7)
            self.mouse_canvas.create_line(mx, ch - 10, mx, ch, fill="#999", tags="ind")
            self.mouse_canvas.create_text(mx, ch - 14, text=f"{cm_val:.0f}",
                                          font=("Arial", 7), fill="#999", tags="ind")

    def _draw_trace(self):
        """Draw the scrolling position trace graph"""
        self.trace_canvas.delete("tr")
        cw = self.trace_canvas.winfo_width()
        ch = self.trace_canvas.winfo_height()

        if cw <= 1 or ch <= 1 or len(self.mouse_trace_data) < 2:
            return

        now = time.time()
        pad = 30  # left padding for labels
        pad_r = 10
        pad_t = 10
        pad_b = 20
        plot_w = cw - pad - pad_r
        plot_h = ch - pad_t - pad_b

        # Y-axis grid: 0 to -672.0 cm (top = 0, bottom = -672.0)
        for i in range(8):
            y = pad_t + (i / 7) * plot_h
            cm_val = -672.0 * (i / 7)
            self.trace_canvas.create_line(pad, y, cw - pad_r, y,
                                          fill="#2a2a4a", tags="tr")
            self.trace_canvas.create_text(pad - 3, y, text=f"{cm_val:.0f}",
                                          anchor=tk.E, fill="#667",
                                          font=("Arial", 7), tags="tr")

        # X-axis grid: 3 seconds
        for i in range(4):
            x = pad + (i / 3) * plot_w
            self.trace_canvas.create_line(x, pad_t, x, ch - pad_b,
                                          fill="#2a2a4a", tags="tr")
            self.trace_canvas.create_text(x, ch - pad_b + 10,
                                          text=f"-{3 - i}s", fill="#667",
                                          font=("Arial", 7), tags="tr")

        # Plot trace line
        points = []
        for t, d in self.mouse_trace_data:
            time_ratio = (t - (now - 3.0)) / 3.0
            x = pad + time_ratio * plot_w
            dist_ratio = abs(d) / 672.0
            y = pad_t + dist_ratio * plot_h
            points.append(x)
            points.append(y)

        if len(points) >= 4:
            self.trace_canvas.create_line(*points, fill="#00ff88", width=2,
                                          smooth=False, tags="tr")

        # Current value dot
        if self.mouse_trace_data:
            last_d = self.mouse_trace_data[-1][1]
            dist_ratio = abs(last_d) / 672.0
            y = pad_t + dist_ratio * plot_h
            self.trace_canvas.create_oval(cw - pad_r - 4, y - 4,
                                          cw - pad_r + 4, y + 4,
                                          fill="#00ff88", outline="white", tags="tr")

    def on_tab_change(self, event):
        """Handle tab change event"""
        selected_tab = self.notebook.index(self.notebook.select())

        # If changed to Serial Testing tab, focus on data entry
        if selected_tab == 1 and self.connected:
            self.data_entry.focus()

        # If changed to Control tab, focus root to capture keys
        if selected_tab == 3 and self.connected:
            self.root.focus_set()

        # If changed to Command tab, focus command entry
        if selected_tab == 4 and self.connected:
            self.cmd_entry.focus()

    def log(self, message):
        """Add a message to the log with timestamp"""
        timestamp = time.strftime("%H:%M:%S", time.localtime())
        log_message = f"[{timestamp}] {message}\n"

        self.log_text.config(state=tk.NORMAL)
        self.log_text.insert(tk.END, log_message)
        # Trim to last 200 lines
        line_count = int(self.log_text.index('end-1c').split('.')[0])
        if line_count > 200:
            self.log_text.delete('1.0', f'{line_count - 200}.0')
        self.log_text.see(tk.END)
        self.log_text.config(state=tk.DISABLED)

    def refresh_ports(self):
        """Refresh the list of available COM ports - Bluetooth Classic only"""
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

        bt_count = 0
        # Add ports to the list - filter for Bluetooth Classic only
        for port in ports:
            port_name = port.device
            description = port.description.lower()
            hwid = port.hwid.lower()

            # Identify device type - skip BLE devices
            device_type = "Unknown"
            is_bluetooth = False

            # Check for BLE indicators (skip these)
            if any(x in description for x in ['ble', 'low energy', 'le ']):
                continue  # Skip BLE devices
            if any(x in hwid for x in ['ble', 'low energy']):
                continue  # Skip BLE devices

            # Check for Bluetooth Classic
            if any(x in description for x in ['bluetooth', 'bt', 'hc-04', 'hc04', 'serial port', 'standard serial']):
                is_bluetooth = True
                device_type = "Bluetooth"
                bt_count += 1
            elif 'bthenum' in hwid:  # Windows Bluetooth enumeration
                is_bluetooth = True
                device_type = "Bluetooth"
                bt_count += 1
            elif 'com' in port_name.lower():
                device_type = "Serial"

            # Store port info
            self.ports[port_name] = {
                'name': port_name,
                'description': port.description,
                'hwid': port.hwid,
                'is_bluetooth': is_bluetooth,
                'device_type': device_type
            }

            # Add to UI
            values = (port_name, port.description, port.hwid, device_type)

            # Use tags for Bluetooth devices
            tags = ("standard",)
            if is_bluetooth:
                tags = ("bluetooth",)

            self.port_tree.insert('', tk.END, port_name, values=values, tags=tags)

        # Configure tag for Bluetooth devices
        self.port_tree.tag_configure('bluetooth', background='#90EE90')  # Light green

        self.log(f"Found {len(self.ports)} COM port(s), {bt_count} Bluetooth Classic")

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

        try:
            self.connection_timeout = int(self.timeout_var.get())
        except:
            self.connection_timeout = 3

        # Start connection in a separate thread
        self.log(f"Connecting to {port_name} at {baud_rate} baud (timeout: {self.connection_timeout}s)...")
        self.status_var.set(f"Connecting...")

        # Disable buttons during connection
        self.connect_button.config(state=tk.DISABLED)
        self.refresh_button.config(state=tk.DISABLED)

        conn_thread = threading.Thread(target=self.connect_thread, args=(port_name, baud_rate))
        conn_thread.daemon = True
        conn_thread.start()

    def connect_thread(self, port_name, baud_rate):
        """Handle port connection in a separate thread with timeout"""
        try:
            # Try to open the serial port with timeout
            self.ser = serial.Serial(
                port=port_name,
                baudrate=baud_rate,
                timeout=self.connection_timeout,
                write_timeout=self.connection_timeout
            )

            # Give it a moment to establish
            time.sleep(0.5)

            # Check if port is actually open
            if not self.ser.is_open:
                raise serial.SerialException("Port failed to open")

            # If we get here, connection was successful
            self.connected = True

            # Start reading thread
            self.keep_reading = True
            self.reading_thread = threading.Thread(target=self.read_serial)
            self.reading_thread.daemon = True
            self.reading_thread.start()

            # Update UI from main thread
            self.root.after(0, self.connection_successful, port_name, baud_rate)

        except serial.SerialException as e:
            # Update UI from main thread
            self.root.after(0, self.connection_failed, str(e))
            self.cleanup_connection()

        except Exception as e:
            # Update UI from main thread
            self.root.after(0, self.connection_failed, str(e))
            self.cleanup_connection()

    def cleanup_connection(self):
        """Clean up connection resources"""
        if self.ser:
            try:
                self.ser.close()
            except:
                pass
            self.ser = None
        self.connected = False

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
        self.notebook.tab(3, state="normal")
        self.notebook.tab(4, state="normal")

        # Update button states
        self.connect_button.config(state=tk.DISABLED)
        self.refresh_button.config(state=tk.NORMAL)
        self.disconnect_button.config(state=tk.NORMAL)

        # Highlight connected port in the list
        self.port_tree.item(port_name, tags=("connected",))
        self.port_tree.tag_configure("connected", background="#aaffaa")

        # Clear receive areas
        self.clear_receive_text()

        # Welcome message
        self.log("Connected! You can now send/receive data in the 'Serial Testing' tab")

    def connection_failed(self, error_msg):
        """Handle failed connection (called from main thread)"""
        self.log(f"Connection failed: {error_msg}")
        self.status_var.set("Connection failed")

        # Reset connection status
        self.connected = False
        self.ser = None

        # Update UI
        self.refresh_button.config(state=tk.NORMAL)
        if self.selected_port:
            self.connect_button.config(state=tk.NORMAL)

        messagebox.showerror("Connection Error",
                           f"Could not connect to the port.\n\nError: {error_msg}\n\n" +
                           "For HC-04 modules:\n" +
                           "1. Make sure the module is powered on\n" +
                           "2. Verify it's paired with your computer\n" +
                           "3. Try a different baud rate (usually 9600 or 115200)\n" +
                           "4. Make sure the COM port is not used by another app\n" +
                           "5. Try disconnecting and re-pairing the device")

    def read_serial(self):
        """Read data from the serial port in a separate thread"""
        receive_buffer = bytearray()
        last_display_time = 0
        DISPLAY_INTERVAL = 0.1  # throttle UI updates to 10Hz max

        while self.keep_reading and self.ser:
            try:
                if not self.ser.is_open:
                    break

                # Check if there's data available
                if self.ser.in_waiting > 0:
                    # Read data
                    data = self.ser.read(self.ser.in_waiting)
                    receive_buffer.extend(data)

                now = time.time()
                if receive_buffer and (now - last_display_time) >= DISPLAY_INTERVAL:
                    data_to_display = bytes(receive_buffer)
                    receive_buffer.clear()
                    last_display_time = now
                    self.root.after(0, lambda d=data_to_display: self.display_received_data(d))

                # Small delay to prevent CPU hogging
                time.sleep(0.05)

            except serial.SerialException as e:
                if self.connected:
                    self.root.after(0, lambda err=str(e):
                                  self.log(f"Serial error: {err}"))
                    self.root.after(0, self.disconnect_port)
                break
            except Exception as e:
                if self.connected:
                    self.root.after(0, lambda err=str(e):
                                  self.log(f"Read error: {err}"))
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

        # Only update UI when Serial Testing tab is active
        if current_tab == 1:
            self.receive_text.insert(tk.END, f"[{timestamp}] {display_text}\n")
            # Trim to last 500 lines to prevent slowdown
            line_count = int(self.receive_text.index('end-1c').split('.')[0])
            if line_count > 500:
                self.receive_text.delete('1.0', f'{line_count - 500}.0')
            self.receive_text.see(tk.END)

    def disconnect_port(self):
        """Disconnect from current port"""
        # Stop reading thread
        self.keep_reading = False
        if self.reading_thread and self.reading_thread.is_alive():
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

        # Stop mouse tracking if active
        if hasattr(self, 'mouse_tracking') and self.mouse_tracking:
            self.stop_mouse_tracking()

        # Disable tabs
        self.notebook.tab(1, state="disabled")
        self.notebook.tab(2, state="disabled")
        self.notebook.tab(3, state="disabled")
        self.notebook.tab(4, state="disabled")

        # Switch to connection tab
        self.notebook.select(0)

        # Update button states
        self.disconnect_button.config(state=tk.DISABLED)
        self.refresh_button.config(state=tk.NORMAL)

        # Reset port list highlighting
        if self.selected_port and self.selected_port in self.ports:
            if self.ports[self.selected_port]['is_bluetooth']:
                self.port_tree.item(self.selected_port, tags=("bluetooth",))
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
            self.log(f"Sent: {data.strip()}")

        except Exception as e:
            self.log(f"Send error: {str(e)}")
            messagebox.showerror("Send Error", f"Could not send data: {str(e)}")

            # Connection might be lost
            if "device disconnected" in str(e).lower() or "port is closed" in str(e).lower():
                self.disconnect_port()

    def clear_receive_text(self):
        """Clear the receive text area"""
        self.receive_text.delete(1.0, tk.END)

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

    # ========== File Upload Methods ==========

    def browse_txt_file(self):
        """Open file dialog to select TXT file"""
        filepath = filedialog.askopenfilename(
            title="Select TXT File",
            filetypes=[("Text files", "*.txt *.TXT"), ("All files", "*.*")]
        )
        if filepath:
            self.selected_file_path = filepath
            self.file_path_var.set(filepath)

            # Get file info
            file_size = os.path.getsize(filepath)
            filename = os.path.basename(filepath)

            # Check 8.3 format
            name, ext = os.path.splitext(filename)
            total_chunks = (file_size + FT_CHUNK_SIZE - 1) // FT_CHUNK_SIZE

            if len(name) > 8:
                self.file_info_var.set(
                    f"WARNING: Filename '{name}' exceeds 8 characters!\n"
                    f"Will be truncated to: {name[:8]}{ext}\n"
                    f"Size: {file_size:,} bytes ({total_chunks} chunks)"
                )
            else:
                self.file_info_var.set(
                    f"Filename: {filename}\n"
                    f"Size: {file_size:,} bytes\n"
                    f"Chunks: {total_chunks}"
                )

            if file_size > 65535:
                self.upload_button.config(state=tk.DISABLED)
                self.ram_upload_button.config(state=tk.DISABLED)
                self.upload_log_message("ERROR: File too large (max 64KB)")
            elif self.connected:
                self.upload_button.config(state=tk.NORMAL)
                self.ram_upload_button.config(state=tk.NORMAL)

    def upload_log_message(self, message):
        """Add message to upload log"""
        timestamp = time.strftime("%H:%M:%S")
        self.upload_log.insert(tk.END, f"[{timestamp}] {message}\n")
        self.upload_log.see(tk.END)

    def calculate_checksum(self, data):
        """Calculate XOR checksum"""
        checksum = 0
        for byte in data:
            checksum ^= byte
        return checksum

    def _stop_reading_thread(self):
        """Stop the background serial reading thread"""
        self.keep_reading = False
        if self.reading_thread and self.reading_thread.is_alive():
            self.reading_thread.join(timeout=1.0)

    def _restart_reading_thread(self):
        """Restart the background serial reading thread"""
        if self.connected and self.ser and not self.keep_reading:
            self.keep_reading = True
            self.reading_thread = threading.Thread(target=self.read_serial)
            self.reading_thread.daemon = True
            self.reading_thread.start()

    def start_upload(self):
        """Start file upload in a separate thread"""
        if not self.selected_file_path:
            messagebox.showerror("Error", "No file selected")
            return

        self.upload_button.config(state=tk.DISABLED)
        self.abort_button.config(state=tk.NORMAL)
        self.upload_abort_flag = False
        self.progress_var.set(0)

        # Stop reading thread so it doesn't steal binary responses
        self._stop_reading_thread()

        upload_thread = threading.Thread(target=self.upload_file_thread)
        upload_thread.daemon = True
        upload_thread.start()

    def upload_file_thread(self):
        """Thread function to handle file upload"""
        try:
            filepath = self.selected_file_path
            file_size = os.path.getsize(filepath)
            filename = os.path.basename(filepath)
            name, ext = os.path.splitext(filename)

            # Truncate to 8.3 format
            name = name[:8].upper()
            ext = ext[:4].upper()

            total_chunks = (file_size + FT_CHUNK_SIZE - 1) // FT_CHUNK_SIZE

            self.root.after(0, lambda: self.upload_log_message(
                f"Starting upload: {name}{ext} ({file_size} bytes, {total_chunks} chunks)"))

            # Read file data
            with open(filepath, 'rb') as f:
                file_data = f.read()

            # Step 1: Send FILE_START
            start_packet = bytearray([FT_CMD_FILE_START])
            start_packet.extend(struct.pack('<H', file_size))
            start_packet.extend(struct.pack('<H', total_chunks))
            start_packet.extend(name.ljust(8, '\x00').encode('ascii')[:8])
            start_packet.extend(ext.ljust(4, '\x00').encode('ascii')[:4])
            start_packet.append(self.calculate_checksum(start_packet))

            self.root.after(0, lambda: self.upload_log_message("Sending FILE_START..."))
            print(f"[DBG] FILE_START packet ({len(start_packet)} bytes): "
                  f"{' '.join(f'{b:02X}' for b in start_packet)}")
            start_success = False
            for start_retry in range(10):
                self.ser.reset_input_buffer()
                self.ser.write(start_packet)
                print(f"[DBG] Sent FILE_START, attempt {start_retry+1}/10, waiting for response...")

                response = self.wait_for_response(timeout=2.0)

                if response and response[0] == FT_RSP_READY:
                    print(f"[DBG] Got READY response!")
                    start_success = True
                    break
                elif response and response[0] == FT_RSP_ERROR:
                    err = response[1] if len(response) > 1 else 0
                    err_msgs = {1: "checksum", 2: "sequence", 3: "SD write", 4: "SD full",
                               5: "file exists", 6: "bad name", 7: "timeout", 8: "overflow"}
                    print(f"[DBG] Got ERROR response: {err_msgs.get(err, err)}")
                    self.root.after(0, lambda e=err: self.upload_log_message(
                        f"Device error: {err_msgs.get(e, e)}"))
                    self.root.after(0, self.upload_complete_error)
                    return
                else:
                    rsp_hex = ' '.join(f'{b:02X}' for b in response) if response else "None"
                    print(f"[DBG] No valid response (got: {rsp_hex}), retry {start_retry+1}/10")
                    self.root.after(0, lambda rt=start_retry, rh=rsp_hex: self.upload_log_message(
                        f"Retry {rt+1}/10 got:[{rh}]"))

                time.sleep(0.1 * (start_retry + 1))

            if not start_success:
                self.root.after(0, lambda: self.upload_log_message("Failed to start transfer"))
                self.root.after(0, self.upload_complete_error)
                return

            # Drain any stale responses left in the buffer from FILE_START retries
            time.sleep(0.15)
            stale = self.ser.in_waiting
            if stale:
                self.ser.read(stale)
                print(f"[DBG] Flushed {stale} stale bytes before sending chunks")
            self.ser.reset_input_buffer()

            self.root.after(0, lambda: self.upload_log_message("Device ready, sending chunks..."))

            # Step 2: Send chunks
            start_time = time.time()
            bytes_sent = 0

            for chunk_idx in range(total_chunks):
                if self.upload_abort_flag:
                    self.send_abort()
                    return

                start = chunk_idx * FT_CHUNK_SIZE
                end = min(start + FT_CHUNK_SIZE, file_size)
                chunk_data = file_data[start:end]
                data_len = len(chunk_data)

                data_packet = bytearray([FT_CMD_FILE_DATA])
                data_packet.extend(struct.pack('<H', chunk_idx))
                data_packet.append(data_len)
                data_packet.extend(chunk_data)
                data_packet.append(self.calculate_checksum(data_packet))

                max_retries = 10
                success = False
                for retry in range(max_retries):
                    if retry > 0:
                        time.sleep(min(50 * (retry + 1), 300) / 1000.0)
                    self.ser.reset_input_buffer()

                    self.ser.write(data_packet)

                    base_timeout = 1.0 if ((chunk_idx + 1) % 8 == 0) else 0.5
                    timeout = base_timeout + (0.2 * retry)
                    response = self.wait_for_response(timeout=timeout)

                    if response and response[0] == FT_RSP_ACK:
                        success = True
                        bytes_sent += data_len
                        if (chunk_idx + 1) % 8 == 0:
                            time.sleep(0.005)
                        else:
                            time.sleep(0.001)
                        break
                    elif response and response[0] == FT_RSP_ERROR:
                        err = response[1] if len(response) > 1 else 0
                        err_msgs = {1: "checksum", 2: "sequence", 3: "SD write", 4: "SD full",
                                   5: "file exists", 6: "bad name", 7: "timeout", 8: "overflow"}
                        print(f"[DBG] DATA phase ERROR: {err_msgs.get(err, err)}")
                        self.root.after(0, lambda e=err: self.upload_log_message(
                            f"Device error: {err_msgs.get(e, e)}"))
                        self.root.after(0, self.upload_complete_error)
                        return

                if not success:
                    self.root.after(0, lambda c=chunk_idx: self.upload_log_message(
                        f"FAILED at chunk {c} - aborting"))
                    self.root.after(0, self.upload_complete_error)
                    return

                elapsed = time.time() - start_time
                speed = bytes_sent / elapsed if elapsed > 0 else 0
                progress = (chunk_idx + 1) / total_chunks * 100

                self.root.after(0, lambda p=progress, c=chunk_idx+1, t=total_chunks, s=speed:
                              self.update_progress(p, f"{c}/{t} | {s:.0f} B/s"))

            # Step 3: Send FILE_END
            self.root.after(0, lambda: self.upload_log_message("Finalizing..."))
            end_packet = bytearray([FT_CMD_FILE_END])
            end_packet.extend(struct.pack('<H', file_size & 0xFFFF))
            end_packet.append(self.calculate_checksum(end_packet))

            end_success = False
            for end_retry in range(10):
                self.ser.reset_input_buffer()
                self.ser.write(end_packet)

                response = self.wait_for_response(timeout=2.0 + (0.5 * end_retry))

                if response and response[0] == FT_RSP_SUCCESS:
                    end_success = True
                    elapsed = time.time() - start_time
                    avg_speed = file_size / elapsed if elapsed > 0 else 0
                    self.root.after(0, lambda s=avg_speed, t=elapsed: self.upload_log_message(
                        f"Upload complete! {s:.0f} B/s avg, {t:.1f}s total"))
                    self.root.after(0, self.upload_complete_success)
                    break
                else:
                    time.sleep(0.2 * (end_retry + 1))

            if not end_success:
                self.root.after(0, lambda: self.upload_log_message("Failed to finalize"))
                self.root.after(0, self.upload_complete_error)

        except Exception as e:
            self.root.after(0, lambda: self.upload_log_message(f"Error: {str(e)}"))
            self.root.after(0, self.upload_complete_error)

    def wait_for_response(self, timeout=2.0):
        """Wait for binary response from device.
        Reads bytes in BULK, scans for a valid protocol header (0xA0-0xA4),
        skips null/junk bytes (common on Windows BT SPP)."""
        old_timeout = self.ser.timeout
        try:
            deadline = time.time() + timeout
            skipped = 0
            leftover = bytearray()   # bytes read past the header

            # Phase 1: bulk-read and scan for a valid response header
            first_byte = None
            while time.time() < deadline:
                # Grab everything the OS has buffered
                waiting = self.ser.in_waiting
                if waiting > 0:
                    chunk = self.ser.read(waiting)
                    for i, b in enumerate(chunk):
                        if 0xA0 <= b <= 0xA4:
                            first_byte = b
                            leftover = bytearray(chunk[i + 1:])  # keep tail
                            skipped += i  # bytes before header = junk
                            break
                        # everything else (0x00, 0x7F, 0x80, ...) is junk
                        # no per-byte print — just count
                    else:
                        skipped += len(chunk)

                    if first_byte is not None:
                        break
                else:
                    # Nothing buffered — tiny sleep so we don't spin at 100 % CPU
                    time.sleep(0.002)

            if first_byte is None:
                print(f"[DBG] wait_for_response: no header after {timeout}s (skipped {skipped} junk bytes)")
                return None

            if skipped:
                print(f"[DBG] skipped {skipped} junk bytes before header")

            # Determine total packet length from header byte
            pkt_lens = {
                FT_RSP_ACK: 4, FT_RSP_NAK: 5, FT_RSP_SUCCESS: 4,
                FT_RSP_ERROR: 3,
            }
            expected_len = pkt_lens.get(first_byte, 2)
            remaining = expected_len - 1  # already have the header

            # Phase 2: assemble response from leftover + serial read
            response = bytearray([first_byte])
            if remaining <= len(leftover):
                response.extend(leftover[:remaining])
            else:
                response.extend(leftover)
                still_need = remaining - len(leftover)
                self.ser.timeout = 0.5
                rest = self.ser.read(still_need)
                response.extend(rest)

            print(f"[DBG] response: {' '.join(f'0x{b:02X}' for b in response)}")
            return bytes(response)

        except Exception as e:
            print(f"[DBG] wait_for_response exception: {e}")
            return None
        finally:
            self.ser.timeout = old_timeout

    def update_progress(self, progress, label):
        """Update progress bar and label"""
        self.progress_var.set(progress)
        self.progress_label_var.set(label)

    def upload_complete_success(self):
        """Handle successful upload completion"""
        self.progress_label_var.set("Upload Complete!")
        self.upload_button.config(state=tk.NORMAL)
        self.abort_button.config(state=tk.DISABLED)
        self._restart_reading_thread()
        self.refresh_device_files()

    def upload_complete_error(self):
        """Handle upload error"""
        self.progress_label_var.set("Upload Failed")
        self.upload_button.config(state=tk.NORMAL)
        self.abort_button.config(state=tk.DISABLED)
        self._restart_reading_thread()

    def abort_upload(self):
        """Abort current upload"""
        self.upload_abort_flag = True
        self.upload_log_message("Upload aborted by user")

    def send_abort(self):
        """Send abort command to device"""
        abort_packet = bytearray([FT_CMD_FILE_ABORT])
        abort_packet.append(self.calculate_checksum(abort_packet))
        self.ser.write(abort_packet)
        self.root.after(0, lambda: self.upload_log_message("Abort sent to device"))
        self.root.after(0, self.upload_complete_error)

    # ========== RAM Upload Methods ==========

    def start_ram_upload(self):
        """Start RAM upload in a separate thread"""
        if not self.selected_file_path:
            messagebox.showerror("Error", "No file selected")
            return

        self.upload_button.config(state=tk.DISABLED)
        self.ram_upload_button.config(state=tk.DISABLED)
        self.abort_button.config(state=tk.NORMAL)
        self.upload_abort_flag = False
        self.progress_var.set(0)

        # Stop reading thread so it doesn't consume our binary data
        self._stop_reading_thread()

        ram_thread = threading.Thread(target=self.ram_upload_thread)
        ram_thread.daemon = True
        ram_thread.start()

    def ram_upload_thread(self):
        """Thread: parse TXT file into floats, send with sentinels"""
        try:
            filepath = self.selected_file_path
            self.root.after(0, lambda: self.upload_log_message(
                f"RAM upload: parsing {os.path.basename(filepath)}"))

            # Parse TXT file: each line = 14 space/comma-separated floats
            float_data = []
            with open(filepath, 'r') as f:
                for line_num, line in enumerate(f, 1):
                    line = line.strip()
                    if not line or line.startswith('#'):
                        continue  # skip empty/comment lines
                    # Support both space and comma separators, strip C-style 'f' suffix
                    parts = line.replace(',', ' ').split()
                    # Remove trailing 'f'/'F' from C-style float literals (e.g. "12.0f")
                    parts = [p.rstrip('fF') for p in parts]
                    if len(parts) != 14:
                        self.root.after(0, lambda ln=line_num, n=len(parts):
                            self.upload_log_message(
                                f"WARN: line {ln} has {n} values (expected 14), skipping"))
                        continue
                    if len(float_data) // 14 >= 365:
                        self.root.after(0, lambda: self.upload_log_message(
                            "Max 365 chords reached, truncating"))
                        break
                    for val_str in parts:
                        float_data.append(float(val_str))

            num_chords = len(float_data) // 14
            if num_chords == 0:
                self.root.after(0, lambda: self.upload_log_message("ERROR: No valid chord data found"))
                self.root.after(0, self._ram_upload_done)
                return

            self.root.after(0, lambda n=num_chords: self.upload_log_message(
                f"Parsed {n} chords ({n*14} floats, {n*14*4} bytes)"))

            # Pack floats as little-endian binary
            payload = struct.pack(f'<{len(float_data)}f', *float_data)
            total_bytes = len(payload) + 8  # payload + 2 sentinels

            self.root.after(0, lambda t=total_bytes: self.upload_log_message(
                f"Sending {t} bytes (incl. sentinels)..."))

            # Flush stale data from buffers
            self.ser.reset_input_buffer()
            self.ser.reset_output_buffer()
            time.sleep(0.1)

            # Send start sentinel SEPARATELY so STM32 idle-line DMA
            # delivers it as a complete 4-byte unit (avoids BT fragmentation)
            self.ser.write(RAM_SENTINEL_START)
            self.ser.flush()
            time.sleep(0.1)  # ensure idle-line fires and STM32 detects sentinel

            self.root.after(0, lambda: self.upload_log_message("Start sentinel sent"))

            # Send payload in chunks to avoid BT buffer overflow
            bytes_sent = 0
            start_time = time.time()

            for offset in range(0, len(payload), RAM_BT_CHUNK_SIZE):
                if self.upload_abort_flag:
                    self.root.after(0, lambda: self.upload_log_message("Upload aborted"))
                    self.root.after(0, self._ram_upload_done)
                    return

                chunk = payload[offset:offset + RAM_BT_CHUNK_SIZE]
                self.ser.write(chunk)
                bytes_sent += len(chunk)

                # Small delay to let BT module + DMA process
                time.sleep(0.01)

                # Update progress
                progress = bytes_sent / len(payload) * 100
                elapsed = time.time() - start_time
                speed = bytes_sent / elapsed if elapsed > 0 else 0
                self.root.after(0, lambda p=progress, s=speed: self.update_progress(
                    p, f"{p:.0f}% | {s:.0f} B/s"))

            # Send end sentinel separately
            self.ser.flush()
            time.sleep(0.05)
            self.ser.write(RAM_SENTINEL_END)
            self.ser.flush()

            elapsed = time.time() - start_time
            avg_speed = total_bytes / elapsed if elapsed > 0 else 0
            self.root.after(0, lambda s=avg_speed, t=elapsed, n=num_chords:
                self.upload_log_message(
                    f"RAM upload complete! {n} chords, {s:.0f} B/s, {t:.1f}s"))
            self.root.after(0, lambda: self.update_progress(100, "RAM Upload Complete!"))
            self.root.after(0, self._ram_upload_done)

        except ValueError as e:
            self.root.after(0, lambda: self.upload_log_message(f"Parse error: {e}"))
            self.root.after(0, self._ram_upload_done)
        except Exception as e:
            self.root.after(0, lambda: self.upload_log_message(f"Error: {e}"))
            self.root.after(0, self._ram_upload_done)

    def _ram_upload_done(self):
        """Re-enable buttons after RAM upload"""
        self.upload_button.config(state=tk.NORMAL)
        self.ram_upload_button.config(state=tk.NORMAL)
        self.abort_button.config(state=tk.DISABLED)
        self._restart_reading_thread()

    def refresh_device_files(self):
        """Request file list from device"""
        if not self.connected:
            return

        for item in self.device_files_tree.get_children():
            self.device_files_tree.delete(item)

        was_reading = self.keep_reading
        self.keep_reading = False
        time.sleep(0.1)

        try:
            self.ser.reset_input_buffer()
            self.ser.write(b"/listFiles\n")

            lines = []
            timeout = time.time() + 3.0
            while time.time() < timeout:
                if self.ser.in_waiting > 0:
                    data = self.ser.read(self.ser.in_waiting)
                    lines.extend(data.decode('utf-8', errors='replace').split('\n'))
                    if any('END' in line for line in lines):
                        break
                time.sleep(0.05)

            for line in lines:
                line = line.strip()
                if ',' in line and not line.startswith('FILES:'):
                    parts = line.split(',')
                    if len(parts) >= 2:
                        filename = parts[0]
                        size = parts[1]
                        self.device_files_tree.insert('', tk.END, values=(filename, size))

        except Exception as e:
            self.upload_log_message(f"Error reading file list: {e}")
        finally:
            if was_reading:
                self.keep_reading = True
                self.reading_thread = threading.Thread(target=self.read_serial)
                self.reading_thread.daemon = True
                self.reading_thread.start()


if __name__ == "__main__":
    try:
        import serial
    except ImportError:
        print("ERROR: PySerial library not found. Please install it with:")
        print("pip install pyserial")
        sys.exit(1)

    root = tk.Tk()
    app = HC04SerialGUI(root)

    root.protocol("WM_DELETE_WINDOW", app.on_closing)

    root.mainloop()
