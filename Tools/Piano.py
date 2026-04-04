"""
HC-04 Bluetooth Serial Tool - Qt6 Edition
Rewritten from tkinter to PySide6 with pyqtgraph for high-performance plotting.
"""

import sys
import os
import struct
import time
import threading
import numpy as np

from PySide6.QtWidgets import (
    QApplication, QMainWindow, QWidget, QVBoxLayout, QHBoxLayout,
    QTabWidget, QLabel, QPushButton, QComboBox, QLineEdit, QSpinBox,
    QRadioButton, QButtonGroup, QGroupBox, QTreeWidget,
    QTreeWidgetItem, QPlainTextEdit, QProgressBar, QFileDialog,
    QMessageBox, QCheckBox, QSplitter, QHeaderView,
    QAbstractItemView,
)
from PySide6.QtCore import (
    Qt, QTimer, Signal, QObject, Slot,
)
from PySide6.QtGui import QColor, QKeyEvent

import serial
import serial.tools.list_ports

import pyqtgraph as pg

# ── Protocol Constants ──────────────────────────────────────────────────
FT_CMD_FILE_START = 0xF0
FT_CMD_FILE_DATA  = 0xF1
FT_CMD_FILE_END   = 0xF2
FT_CMD_FILE_ABORT = 0xF3

FT_RSP_ACK     = 0xA0
FT_RSP_NAK     = 0xA1
FT_RSP_READY   = 0xA2
FT_RSP_ERROR   = 0xA3
FT_RSP_SUCCESS = 0xA4

FT_CHUNK_SIZE = 64

FT_CMD_RAM_START = 0xE0
FT_CMD_RAM_DATA  = 0xE1
FT_CMD_RAM_END   = 0xE2
RAM_SENTINEL_START_BYTE = 0x78
RAM_SENTINEL_END_BYTE   = 0x91

# ── Dark Theme Stylesheet ──────────────────────────────────────────────
DARK_STYLE = """
QMainWindow, QWidget {
    background-color: #1e1e2e;
    color: #cdd6f4;
    font-family: 'Segoe UI', 'Inter', sans-serif;
    font-size: 10pt;
}

QTabWidget::pane {
    border: 1px solid #313244;
    border-radius: 6px;
    background-color: #1e1e2e;
    top: -1px;
}
QTabBar::tab {
    background-color: #181825;
    color: #6c7086;
    border: 1px solid #313244;
    border-bottom: none;
    padding: 8px 20px;
    margin-right: 2px;
    border-top-left-radius: 6px;
    border-top-right-radius: 6px;
    font-weight: 600;
}
QTabBar::tab:selected {
    background-color: #1e1e2e;
    color: #89b4fa;
    border-bottom: 2px solid #89b4fa;
}
QTabBar::tab:hover:!selected {
    background-color: #252536;
    color: #a6adc8;
}
QTabBar::tab:disabled {
    color: #45475a;
    background-color: #11111b;
}

QGroupBox {
    border: 1px solid #313244;
    border-radius: 8px;
    margin-top: 14px;
    padding-top: 14px;
    font-weight: bold;
    color: #89b4fa;
}
QGroupBox::title {
    subcontrol-origin: margin;
    left: 12px;
    padding: 0 6px;
}

QPushButton {
    background-color: #313244;
    color: #cdd6f4;
    border: 1px solid #45475a;
    border-radius: 6px;
    padding: 6px 16px;
    font-weight: 600;
    min-height: 28px;
}
QPushButton:hover {
    background-color: #45475a;
    border-color: #585b70;
}
QPushButton:pressed {
    background-color: #585b70;
}
QPushButton:disabled {
    background-color: #181825;
    color: #45475a;
    border-color: #313244;
}
QPushButton#primary {
    background-color: #89b4fa;
    color: #1e1e2e;
    border: none;
}
QPushButton#primary:hover {
    background-color: #74c7ec;
}
QPushButton#primary:pressed {
    background-color: #89dceb;
}
QPushButton#danger {
    background-color: #f38ba8;
    color: #1e1e2e;
    border: none;
}
QPushButton#danger:hover {
    background-color: #eba0ac;
}
QPushButton#success {
    background-color: #a6e3a1;
    color: #1e1e2e;
    border: none;
}
QPushButton#success:hover {
    background-color: #94e2d5;
}

QLineEdit, QSpinBox, QDoubleSpinBox, QComboBox {
    background-color: #313244;
    color: #cdd6f4;
    border: 1px solid #45475a;
    border-radius: 6px;
    padding: 5px 10px;
    min-height: 24px;
    selection-background-color: #89b4fa;
    selection-color: #1e1e2e;
}
QLineEdit:focus, QSpinBox:focus, QComboBox:focus {
    border-color: #89b4fa;
}
QComboBox::drop-down {
    border: none;
    width: 24px;
}
QComboBox QAbstractItemView {
    background-color: #313244;
    color: #cdd6f4;
    border: 1px solid #45475a;
    selection-background-color: #89b4fa;
    selection-color: #1e1e2e;
}

QPlainTextEdit {
    background-color: #11111b;
    color: #a6adc8;
    border: 1px solid #313244;
    border-radius: 6px;
    padding: 6px;
    font-family: 'Cascadia Code', 'Consolas', monospace;
    font-size: 9pt;
    selection-background-color: #45475a;
}

QTreeWidget {
    background-color: #11111b;
    color: #cdd6f4;
    border: 1px solid #313244;
    border-radius: 6px;
    alternate-background-color: #181825;
    outline: none;
}
QTreeWidget::item {
    padding: 4px 8px;
    border: none;
}
QTreeWidget::item:selected {
    background-color: #313244;
    color: #89b4fa;
}
QTreeWidget::item:hover {
    background-color: #252536;
}
QHeaderView::section {
    background-color: #181825;
    color: #6c7086;
    border: none;
    border-bottom: 1px solid #313244;
    padding: 6px 8px;
    font-weight: bold;
    font-size: 9pt;
}

QProgressBar {
    background-color: #313244;
    border: none;
    border-radius: 6px;
    height: 20px;
    text-align: center;
    color: #cdd6f4;
    font-weight: bold;
}
QProgressBar::chunk {
    background: qlineargradient(x1:0, y1:0, x2:1, y2:0,
        stop:0 #89b4fa, stop:1 #74c7ec);
    border-radius: 6px;
}

QRadioButton, QCheckBox {
    color: #cdd6f4;
    spacing: 6px;
}
QRadioButton::indicator, QCheckBox::indicator {
    width: 16px;
    height: 16px;
    border-radius: 4px;
    border: 2px solid #45475a;
    background-color: #313244;
}
QRadioButton::indicator:checked, QCheckBox::indicator:checked {
    background-color: #89b4fa;
    border-color: #89b4fa;
}

QSlider::groove:horizontal {
    background: #313244;
    height: 6px;
    border-radius: 3px;
}
QSlider::handle:horizontal {
    background: #89b4fa;
    width: 16px;
    height: 16px;
    margin: -5px 0;
    border-radius: 8px;
}
QSlider::sub-page:horizontal {
    background: #89b4fa;
    border-radius: 3px;
}

QSplitter::handle {
    background-color: #313244;
    width: 2px;
}

QScrollBar:vertical {
    background: #181825;
    width: 10px;
    border-radius: 5px;
}
QScrollBar::handle:vertical {
    background: #45475a;
    border-radius: 5px;
    min-height: 20px;
}
QScrollBar::handle:vertical:hover {
    background: #585b70;
}
QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical {
    height: 0;
}
QScrollBar:horizontal {
    background: #181825;
    height: 10px;
    border-radius: 5px;
}
QScrollBar::handle:horizontal {
    background: #45475a;
    border-radius: 5px;
    min-width: 20px;
}
QScrollBar::handle:horizontal:hover {
    background: #585b70;
}
QScrollBar::add-line:horizontal, QScrollBar::sub-line:horizontal {
    width: 0;
}
"""

# ── Max log lines ───────────────────────────────────────────────────────
MAX_LOG_LINES = 500


# ── Signals bridge for thread-safe UI updates ───────────────────────────
class SerialSignals(QObject):
    data_received   = Signal(bytes)
    log_message     = Signal(str)
    connection_ok   = Signal(str, int)     # port, baud
    connection_fail = Signal(str)
    upload_progress = Signal(float, str)   # percent, label
    upload_log      = Signal(str)
    upload_done     = Signal(bool)         # success?
    ram_done        = Signal()


# ── Capped log helper ───────────────────────────────────────────────────
def append_log(widget: QPlainTextEdit, text: str):
    """Append text and trim to MAX_LOG_LINES."""
    widget.appendPlainText(text)
    if widget.blockCount() > MAX_LOG_LINES:
        cursor = widget.textCursor()
        cursor.movePosition(cursor.MoveOperation.Start)
        cursor.movePosition(cursor.MoveOperation.Down, cursor.MoveMode.KeepAnchor,
                            widget.blockCount() - MAX_LOG_LINES)
        cursor.removeSelectedText()
        cursor.deleteChar()  # remove trailing newline


# ════════════════════════════════════════════════════════════════════════
#  Main Window
# ════════════════════════════════════════════════════════════════════════
class HC04MainWindow(QMainWindow):
    def __init__(self):
        super().__init__()
        self.setWindowTitle("HC-04 Bluetooth Serial Tool")
        self.resize(1050, 720)
        self.setMinimumSize(800, 550)

        # Serial state
        self.ser = None
        self.connected = False
        self.keep_reading = False
        self.reading_thread = None
        self.ports: dict = {}
        self.selected_port = None
        self.connection_timeout = 3

        # Line ending
        self.line_ending = "CRLF"

        # Signals
        self.sig = SerialSignals()
        self.sig.data_received.connect(self._on_data_received)
        self.sig.log_message.connect(self._on_log)
        self.sig.connection_ok.connect(self._on_connected)
        self.sig.connection_fail.connect(self._on_connect_fail)
        self.sig.upload_progress.connect(self._on_upload_progress)
        self.sig.upload_log.connect(self._on_upload_log)
        self.sig.upload_done.connect(self._on_upload_done)
        self.sig.ram_done.connect(self._on_ram_done)

        # Upload state
        self.selected_file_path = None
        self.upload_abort_flag = False

        self.control_keys_held: set = set()

        # VOFA state
        self.vofa_raw_buf = bytearray()
        self.vofa_raw_lock = threading.Lock()
        self._vofa_carry = bytearray()
        self.vofa_ch_count = 10
        self.vofa_frame_bytes = self.vofa_ch_count * 4
        self.vofa_tail = b'\x00\x00\x80\x7f'
        self.vofa_max_points = 20000
        self.vofa_t_ms = 0
        self.vofa_time = np.empty(0, dtype=np.float64)
        self.vofa_data = [np.empty(0, dtype=np.float64) for _ in range(self.vofa_ch_count)]
        self.vofa_paused = False
        self.vofa_window_ms = 500
        self.vofa_frame_count = 0
        self._vofa_fps_t = time.time()

        self._build_ui()
        self.refresh_ports()

    # ── UI Construction ─────────────────────────────────────────────────
    def _build_ui(self):
        central = QWidget()
        self.setCentralWidget(central)
        root_layout = QVBoxLayout(central)
        root_layout.setContentsMargins(8, 8, 8, 8)
        root_layout.setSpacing(4)

        # Header
        header = QHBoxLayout()
        title = QLabel("HC-04 Bluetooth Serial Tool")
        title.setStyleSheet("font-size: 15pt; font-weight: bold; color: #89b4fa;")
        header.addWidget(title)
        header.addStretch()
        self.status_label = QLabel("Ready")
        self.status_label.setStyleSheet("color: #a6e3a1; font-weight: 600;")
        header.addWidget(self.status_label)
        root_layout.addLayout(header)

        # Tabs
        self.tabs = QTabWidget()
        root_layout.addWidget(self.tabs)

        self._build_connection_tab()
        self._build_serial_tab()
        self._build_upload_tab()
        self._build_control_tab()
        self._build_command_tab()
        self._build_vofa_tab()

        # Disable tabs until connected
        for i in range(1, 5):
            self.tabs.setTabEnabled(i, False)

    # ────────────────────────────────────────────────────────────────────
    #  TAB 0 : Connection
    # ────────────────────────────────────────────────────────────────────
    def _build_connection_tab(self):
        tab = QWidget()
        layout = QVBoxLayout(tab)
        layout.setSpacing(8)

        # Port list
        grp = QGroupBox("Available COM Ports (Bluetooth Classic)")
        gl = QVBoxLayout(grp)
        self.port_tree = QTreeWidget()
        self.port_tree.setHeaderLabels(["Port", "Description", "Hardware ID", "Device"])
        self.port_tree.setAlternatingRowColors(True)
        self.port_tree.setRootIsDecorated(False)
        self.port_tree.setSelectionMode(QAbstractItemView.SelectionMode.SingleSelection)
        hdr = self.port_tree.header()
        hdr.setSectionResizeMode(1, QHeaderView.ResizeMode.Stretch)
        hdr.setMinimumSectionSize(80)
        self.port_tree.itemSelectionChanged.connect(self._on_port_select)
        gl.addWidget(self.port_tree)
        layout.addWidget(grp)

        # Buttons row
        btn_row = QHBoxLayout()
        self.connect_btn = QPushButton("Connect")
        self.connect_btn.setObjectName("primary")
        self.connect_btn.setEnabled(False)
        self.connect_btn.clicked.connect(self.connect_port)
        btn_row.addWidget(self.connect_btn)

        refresh_btn = QPushButton("Refresh Ports")
        refresh_btn.clicked.connect(self.refresh_ports)
        btn_row.addWidget(refresh_btn)

        self.disconnect_btn = QPushButton("Disconnect")
        self.disconnect_btn.setObjectName("danger")
        self.disconnect_btn.setEnabled(False)
        self.disconnect_btn.clicked.connect(self.disconnect_port)
        btn_row.addWidget(self.disconnect_btn)

        btn_row.addStretch()
        quit_btn = QPushButton("Quit")
        quit_btn.clicked.connect(self.close)
        btn_row.addWidget(quit_btn)
        layout.addLayout(btn_row)

        # Settings
        settings_grp = QGroupBox("Connection Settings")
        sl = QHBoxLayout(settings_grp)

        sl.addWidget(QLabel("Baud Rate:"))
        self.baud_combo = QComboBox()
        self.baud_combo.addItems(["9600", "38400", "57600", "115200", "460800", "921600"])
        self.baud_combo.setCurrentText("921600")
        self.baud_combo.setFixedWidth(110)
        sl.addWidget(self.baud_combo)

        sl.addSpacing(20)
        sl.addWidget(QLabel("Timeout (s):"))
        self.timeout_spin = QSpinBox()
        self.timeout_spin.setRange(1, 10)
        self.timeout_spin.setValue(3)
        self.timeout_spin.setFixedWidth(60)
        sl.addWidget(self.timeout_spin)

        sl.addSpacing(20)
        sl.addWidget(QLabel("Line Ending:"))
        self.le_group = QButtonGroup(self)
        for text, val in [("None", "NONE"), ("CR", "CR"), ("LF", "LF"), ("CR+LF", "CRLF")]:
            rb = QRadioButton(text)
            rb.setChecked(val == "CRLF")
            rb.toggled.connect(lambda checked, v=val: self._set_line_ending(v) if checked else None)
            self.le_group.addButton(rb)
            sl.addWidget(rb)
        sl.addStretch()
        layout.addWidget(settings_grp)

        # Connection info
        info_grp = QGroupBox("Connection Info")
        il = QHBoxLayout(info_grp)
        il.addWidget(QLabel("Port:"))
        self.conn_port_label = QLabel("None")
        self.conn_port_label.setStyleSheet("color: #f9e2af; font-weight: bold;")
        il.addWidget(self.conn_port_label)
        il.addSpacing(30)
        il.addWidget(QLabel("Status:"))
        self.conn_status_label = QLabel("Disconnected")
        self.conn_status_label.setStyleSheet("color: #f38ba8; font-weight: bold;")
        il.addWidget(self.conn_status_label)
        il.addStretch()
        layout.addWidget(info_grp)

        # Log
        log_grp = QGroupBox("Status Log")
        ll = QVBoxLayout(log_grp)
        self.log_text = QPlainTextEdit()
        self.log_text.setReadOnly(True)
        self.log_text.setMaximumBlockCount(MAX_LOG_LINES)
        ll.addWidget(self.log_text)
        layout.addWidget(log_grp)

        self.log("HC-04 Bluetooth Serial Tool ready.")
        self.log("1. Pair your HC-04 with Windows first before connecting")
        self.log("2. Select the Bluetooth COM port from the list")

        self.tabs.addTab(tab, "Connection")

    # ────────────────────────────────────────────────────────────────────
    #  TAB 1 : Serial Testing
    # ────────────────────────────────────────────────────────────────────
    def _build_serial_tab(self):
        tab = QWidget()
        layout = QVBoxLayout(tab)

        # Send
        send_grp = QGroupBox("Send Data")
        sl = QVBoxLayout(send_grp)

        row1 = QHBoxLayout()
        self.data_entry = QLineEdit()
        self.data_entry.setPlaceholderText("Type data to send...")
        self.data_entry.returnPressed.connect(self.send_data)
        row1.addWidget(self.data_entry)
        send_btn = QPushButton("Send")
        send_btn.setObjectName("primary")
        send_btn.clicked.connect(self.send_data)
        row1.addWidget(send_btn)
        sl.addLayout(row1)

        # Quick send
        qrow = QHBoxLayout()
        qrow.addWidget(QLabel("Quick:"))
        for msg in ["Hello", "Test", "OK", "1234", "ABCD"]:
            b = QPushButton(msg)
            b.setFixedWidth(60)
            b.clicked.connect(lambda _, m=msg: self.quick_send(m))
            qrow.addWidget(b)
        qrow.addStretch()
        sl.addLayout(qrow)

        # Hex send
        hrow = QHBoxLayout()
        hrow.addWidget(QLabel("Hex:"))
        self.hex_entry = QLineEdit("48 65 6C 6C 6F")
        self.hex_entry.setPlaceholderText("e.g. 48 65 6C 6C 6F")
        hrow.addWidget(self.hex_entry)
        hex_btn = QPushButton("Send Hex")
        hex_btn.clicked.connect(self.send_hex_data)
        hrow.addWidget(hex_btn)
        sl.addLayout(hrow)
        layout.addWidget(send_grp)

        # Receive
        recv_grp = QGroupBox("Received Data")
        rl = QVBoxLayout(recv_grp)

        drow = QHBoxLayout()
        drow.addWidget(QLabel("Display:"))
        self.display_group = QButtonGroup(self)
        self.display_mode = "TEXT"
        for text, val in [("Text", "TEXT"), ("Hex", "HEX"), ("Both", "BOTH")]:
            rb = QRadioButton(text)
            rb.setChecked(val == "TEXT")
            rb.toggled.connect(lambda checked, v=val: setattr(self, 'display_mode', v) if checked else None)
            self.display_group.addButton(rb)
            drow.addWidget(rb)
        drow.addSpacing(20)
        self.show_received_cb = QCheckBox("Show Received")
        self.show_received_cb.setChecked(True)
        self.show_received_cb.setStyleSheet("color: #a6e3a1; font-weight: bold;")
        drow.addWidget(self.show_received_cb)
        drow.addStretch()
        clear_btn = QPushButton("Clear")
        clear_btn.clicked.connect(lambda: self.receive_text.clear())
        drow.addWidget(clear_btn)
        rl.addLayout(drow)

        self.receive_text = QPlainTextEdit()
        self.receive_text.setReadOnly(True)
        self.receive_text.setMaximumBlockCount(MAX_LOG_LINES)
        rl.addWidget(self.receive_text)
        layout.addWidget(recv_grp)

        self.tabs.addTab(tab, "Serial Testing")

    # ────────────────────────────────────────────────────────────────────
    #  TAB 2 : File Upload
    # ────────────────────────────────────────────────────────────────────
    def _build_upload_tab(self):
        tab = QWidget()
        layout = QVBoxLayout(tab)

        # File selection
        file_grp = QGroupBox("Select TXT File")
        fl = QHBoxLayout(file_grp)
        self.file_path_label = QLabel("No file selected")
        self.file_path_label.setStyleSheet("color: #6c7086;")
        fl.addWidget(self.file_path_label, 1)
        browse_btn = QPushButton("Browse...")
        browse_btn.clicked.connect(self.browse_txt_file)
        fl.addWidget(browse_btn)
        layout.addWidget(file_grp)

        # File info
        info_grp = QGroupBox("File Information")
        il = QVBoxLayout(info_grp)
        self.file_info_label = QLabel("Select a TXT file to see details")
        self.file_info_label.setStyleSheet("color: #a6adc8;")
        il.addWidget(self.file_info_label)
        layout.addWidget(info_grp)

        # Upload controls
        ctrl_grp = QGroupBox("Upload Control")
        cl = QHBoxLayout(ctrl_grp)
        self.upload_btn = QPushButton("Upload to SD")
        self.upload_btn.setObjectName("primary")
        self.upload_btn.setEnabled(False)
        self.upload_btn.clicked.connect(self.start_upload)
        cl.addWidget(self.upload_btn)

        self.ram_upload_btn = QPushButton("Upload to RAM")
        self.ram_upload_btn.setObjectName("success")
        self.ram_upload_btn.setEnabled(False)
        self.ram_upload_btn.clicked.connect(self.start_ram_upload)
        cl.addWidget(self.ram_upload_btn)

        self.abort_btn = QPushButton("Abort")
        self.abort_btn.setObjectName("danger")
        self.abort_btn.setEnabled(False)
        self.abort_btn.clicked.connect(self.abort_upload)
        cl.addWidget(self.abort_btn)

        refresh_files_btn = QPushButton("Refresh File List")
        refresh_files_btn.clicked.connect(self.refresh_device_files)
        cl.addWidget(refresh_files_btn)
        cl.addStretch()
        layout.addWidget(ctrl_grp)

        # Progress
        prog_grp = QGroupBox("Upload Progress")
        pl = QVBoxLayout(prog_grp)
        self.progress_bar = QProgressBar()
        self.progress_bar.setValue(0)
        pl.addWidget(self.progress_bar)
        self.progress_label = QLabel("Ready")
        self.progress_label.setAlignment(Qt.AlignmentFlag.AlignCenter)
        pl.addWidget(self.progress_label)
        layout.addWidget(prog_grp)

        # Device files
        dev_grp = QGroupBox("Files on Device")
        dl = QVBoxLayout(dev_grp)
        self.device_files_tree = QTreeWidget()
        self.device_files_tree.setHeaderLabels(["Filename", "Size (bytes)"])
        self.device_files_tree.setRootIsDecorated(False)
        self.device_files_tree.setMaximumHeight(140)
        dl.addWidget(self.device_files_tree)
        layout.addWidget(dev_grp)

        # Upload log
        ulog_grp = QGroupBox("Upload Log")
        ul = QVBoxLayout(ulog_grp)
        self.upload_log_text = QPlainTextEdit()
        self.upload_log_text.setReadOnly(True)
        self.upload_log_text.setMaximumBlockCount(MAX_LOG_LINES)
        ul.addWidget(self.upload_log_text)
        layout.addWidget(ulog_grp)

        self.tabs.addTab(tab, "File Upload")

    # ────────────────────────────────────────────────────────────────────
    #  TAB 3 : Control
    # ────────────────────────────────────────────────────────────────────
    def _build_control_tab(self):
        tab = QWidget()
        layout = QVBoxLayout(tab)

        info_grp = QGroupBox("Menu Control")
        il = QVBoxLayout(info_grp)

        keys_label = QLabel("W = Up    S = Down    D = Enter/Select    A = Back")
        keys_label.setStyleSheet("font-size: 13pt; font-weight: bold; color: #f9e2af;")
        keys_label.setAlignment(Qt.AlignmentFlag.AlignCenter)
        il.addWidget(keys_label)

        hint = QLabel("Click this area first to capture keyboard input.")
        hint.setStyleSheet("color: #6c7086;")
        hint.setAlignment(Qt.AlignmentFlag.AlignCenter)
        il.addWidget(hint)

        self.control_status = QLabel("Waiting for input...")
        self.control_status.setStyleSheet(
            "font-size: 16pt; font-weight: bold; color: #89b4fa;")
        self.control_status.setAlignment(Qt.AlignmentFlag.AlignCenter)
        il.addWidget(self.control_status)

        # Clickable buttons
        brow = QHBoxLayout()
        brow.addStretch()
        for label, cmd, direction in [
            ("W  Up", ":w", "Up"), ("S  Down", ":s", "Down"),
            ("D  Enter", ":d", "Enter"), ("A  Back", ":a", "Back"),
        ]:
            b = QPushButton(label)
            b.setFixedWidth(130)
            b.clicked.connect(lambda _, c=cmd, d=direction: self.control_send(c, d))
            brow.addWidget(b)
        brow.addStretch()
        il.addLayout(brow)
        layout.addWidget(info_grp)

        log_grp = QGroupBox("Control Log")
        ll = QVBoxLayout(log_grp)
        self.control_log_text = QPlainTextEdit()
        self.control_log_text.setReadOnly(True)
        self.control_log_text.setMaximumBlockCount(MAX_LOG_LINES)
        ll.addWidget(self.control_log_text)
        layout.addWidget(log_grp)

        self.tabs.addTab(tab, "Control")

    # ────────────────────────────────────────────────────────────────────
    #  TAB 4 : Command
    # ────────────────────────────────────────────────────────────────────
    def _build_command_tab(self):
        tab = QWidget()
        layout = QVBoxLayout(tab)

        # Commands
        cmd_grp = QGroupBox("Commands")
        cl = QVBoxLayout(cmd_grp)

        row1 = QHBoxLayout()
        self.cmd_entry = QLineEdit()
        self.cmd_entry.setPlaceholderText("Type command...")
        self.cmd_entry.returnPressed.connect(self.cmd_send_raw)
        row1.addWidget(self.cmd_entry)
        send_btn = QPushButton("Send")
        send_btn.setObjectName("primary")
        send_btn.clicked.connect(self.cmd_send_raw)
        row1.addWidget(send_btn)
        cl.addLayout(row1)

        qrow = QHBoxLayout()
        qrow.addWidget(QLabel("Quick:"))
        for label, cmd in [(";E Halt", ";E"), (";F Toggle PID", ";F")]:
            b = QPushButton(label)
            b.clicked.connect(lambda _, c=cmd: self.cmd_quick(c))
            qrow.addWidget(b)
        qrow.addSpacing(20)
        qrow.addWidget(QLabel(";D Distance (cm):"))
        self.cmd_dist_entry = QLineEdit()
        self.cmd_dist_entry.setFixedWidth(80)
        self.cmd_dist_entry.returnPressed.connect(self.cmd_send_dist)
        qrow.addWidget(self.cmd_dist_entry)
        dist_btn = QPushButton("Send ;D")
        dist_btn.clicked.connect(self.cmd_send_dist)
        qrow.addWidget(dist_btn)
        qrow.addStretch()
        cl.addLayout(qrow)
        layout.addWidget(cmd_grp)

        # Command log
        clog_grp = QGroupBox("Command Output")
        cll = QVBoxLayout(clog_grp)
        self.cmd_log_text = QPlainTextEdit()
        self.cmd_log_text.setReadOnly(True)
        self.cmd_log_text.setMaximumBlockCount(200)
        clog_row = QHBoxLayout()
        clog_row.addStretch()
        clr_btn = QPushButton("Clear")
        clr_btn.clicked.connect(lambda: self.cmd_log_text.clear())
        clog_row.addWidget(clr_btn)
        cll.addWidget(self.cmd_log_text)
        cll.addLayout(clog_row)
        layout.addWidget(clog_grp)

        self.tabs.addTab(tab, "Command")

    # ────────────────────────────────────────────────────────────────────
    #  TAB 5 : VOFA Oscilloscope
    # ────────────────────────────────────────────────────────────────────
    def _build_vofa_tab(self):
        tab = QWidget()
        layout = QVBoxLayout(tab)
        layout.setSpacing(4)

        COLORS = [
            '#FF6B6B', '#4ECDC4', '#45B7D1', '#96CEB4', '#FFEAA7',
            '#DDA0DD', '#98D8C8', '#F7DC6F', '#BB8FCE', '#82E0AA',
        ]
        self._vofa_colors = COLORS

        # Top controls
        ctrl = QHBoxLayout()
        ctrl.addWidget(QLabel("Window:"))
        self.vofa_window_spin = QSpinBox()
        self.vofa_window_spin.setRange(50, 20000)
        self.vofa_window_spin.setValue(500)
        self.vofa_window_spin.setSingleStep(50)
        self.vofa_window_spin.setSuffix(" ms")
        self.vofa_window_spin.setFixedWidth(110)
        self.vofa_window_spin.valueChanged.connect(
            lambda v: setattr(self, 'vofa_window_ms', v))
        ctrl.addWidget(self.vofa_window_spin)

        ctrl.addSpacing(12)
        self.vofa_pause_btn = QPushButton("\u23f8 Pause")
        self.vofa_pause_btn.clicked.connect(self._vofa_toggle_pause)
        ctrl.addWidget(self.vofa_pause_btn)

        clear_btn = QPushButton("\u27f3 Clear")
        clear_btn.clicked.connect(self._vofa_clear)
        ctrl.addWidget(clear_btn)

        ctrl.addStretch()
        self.vofa_status_label = QLabel("Waiting for data...")
        self.vofa_status_label.setStyleSheet("color: #a6e3a1; font-family: 'Consolas';")
        ctrl.addWidget(self.vofa_status_label)
        layout.addLayout(ctrl)

        # Main area: plot + channel panel
        splitter = QSplitter(Qt.Orientation.Horizontal)

        # pyqtgraph plot
        pg.setConfigOptions(antialias=True, useOpenGL=False)
        self.vofa_plot = pg.PlotWidget()
        self.vofa_plot.setBackground('#11111b')
        self.vofa_plot.setLabel('bottom', 'Time (ms)', color='#6c7086')
        self.vofa_plot.setLabel('left', 'Value', color='#6c7086')
        self.vofa_plot.showGrid(x=True, y=True, alpha=0.15)
        self.vofa_plot.setDownsampling(mode='peak')
        self.vofa_plot.setClipToView(True)

        self.vofa_curves = []
        for i in range(self.vofa_ch_count):
            curve = self.vofa_plot.plot(
                pen=pg.mkPen(COLORS[i], width=1.5), name=f'CH{i}')
            self.vofa_curves.append(curve)

        splitter.addWidget(self.vofa_plot)

        # Channel panel
        ch_panel = QWidget()
        ch_panel.setFixedWidth(155)
        ch_panel.setStyleSheet("background-color: #181825;")
        ch_layout = QVBoxLayout(ch_panel)
        ch_layout.setContentsMargins(6, 10, 6, 6)

        ch_title = QLabel("CHANNELS")
        ch_title.setStyleSheet("color: #6c7086; font-size: 8pt; font-weight: bold;")
        ch_title.setAlignment(Qt.AlignmentFlag.AlignCenter)
        ch_layout.addWidget(ch_title)

        self.vofa_ch_checks = []
        self.vofa_val_labels = []
        for i in range(self.vofa_ch_count):
            row = QHBoxLayout()
            cb = QCheckBox(f"CH{i}")
            cb.setChecked(True)
            cb.setStyleSheet(f"color: {COLORS[i]}; font-weight: bold; font-family: 'Consolas';")
            self.vofa_ch_checks.append(cb)
            row.addWidget(cb)

            val = QLabel("  -.---")
            val.setStyleSheet("color: #cdd6f4; font-family: 'Consolas'; font-size: 9pt;")
            val.setAlignment(Qt.AlignmentFlag.AlignRight)
            self.vofa_val_labels.append(val)
            row.addWidget(val)
            ch_layout.addLayout(row)

        ch_layout.addStretch()
        splitter.addWidget(ch_panel)

        splitter.setStretchFactor(0, 1)
        splitter.setStretchFactor(1, 0)
        layout.addWidget(splitter)

        self.tabs.addTab(tab, "VOFA")

        # VOFA render timer ~30fps
        self.vofa_timer = QTimer()
        self.vofa_timer.timeout.connect(self._vofa_tick)
        self.vofa_timer.start(33)

    # ════════════════════════════════════════════════════════════════════
    #  Connection Logic
    # ════════════════════════════════════════════════════════════════════
    def _set_line_ending(self, val):
        self.line_ending = val

    def refresh_ports(self):
        self.port_tree.clear()
        self.ports = {}
        ports = list(serial.tools.list_ports.comports())
        if not ports:
            self.log("No COM ports found.")
            return

        bt_count = 0
        for port in ports:
            desc = port.description.lower()
            hwid = port.hwid.lower()

            if any(x in desc for x in ['ble', 'low energy', 'le ']):
                continue
            if any(x in hwid for x in ['ble', 'low energy']):
                continue

            device_type = "Serial"
            is_bt = False
            if any(x in desc for x in ['bluetooth', 'bt', 'hc-04', 'hc04', 'serial port', 'standard serial']):
                is_bt = True
                device_type = "Bluetooth"
                bt_count += 1
            elif 'bthenum' in hwid:
                is_bt = True
                device_type = "Bluetooth"
                bt_count += 1

            self.ports[port.device] = {
                'name': port.device, 'description': port.description,
                'hwid': port.hwid, 'is_bluetooth': is_bt, 'device_type': device_type,
            }

            item = QTreeWidgetItem([port.device, port.description, port.hwid, device_type])
            if is_bt:
                for col in range(4):
                    item.setForeground(col, QColor("#a6e3a1"))
            self.port_tree.addTopLevelItem(item)

        self.log(f"Found {len(self.ports)} COM port(s), {bt_count} Bluetooth Classic")

    def _on_port_select(self):
        items = self.port_tree.selectedItems()
        if items:
            self.selected_port = items[0].text(0)
            self.status_label.setText(f"Selected: {self.selected_port}")
            self.connect_btn.setEnabled(True)
        else:
            self.selected_port = None
            self.connect_btn.setEnabled(False)

    def connect_port(self):
        if not self.selected_port:
            return
        self.connection_timeout = self.timeout_spin.value()
        baud = int(self.baud_combo.currentText())
        self.log(f"Connecting to {self.selected_port} at {baud} baud...")
        self.status_label.setText("Connecting...")
        self.connect_btn.setEnabled(False)

        t = threading.Thread(target=self._connect_thread,
                             args=(self.selected_port, baud), daemon=True)
        t.start()

    def _connect_thread(self, port_name, baud_rate):
        try:
            self.ser = serial.Serial(
                port=port_name, baudrate=baud_rate,
                timeout=self.connection_timeout,
                write_timeout=self.connection_timeout)
            time.sleep(0.5)
            if not self.ser.is_open:
                raise serial.SerialException("Port failed to open")
            self.connected = True
            self.keep_reading = True
            self.reading_thread = threading.Thread(target=self._read_serial, daemon=True)
            self.reading_thread.start()
            self.sig.connection_ok.emit(port_name, baud_rate)
        except Exception as e:
            self._cleanup_connection()
            self.sig.connection_fail.emit(str(e))

    def _cleanup_connection(self):
        if self.ser:
            try:
                self.ser.close()
            except Exception:
                pass
            self.ser = None
        self.connected = False

    @Slot(str, int)
    def _on_connected(self, port_name, baud_rate):
        self.log(f"Connected to {port_name} at {baud_rate} baud")
        self.status_label.setText(f"Connected: {port_name}")
        self.conn_port_label.setText(f"{port_name} ({baud_rate})")
        self.conn_status_label.setText("Connected")
        self.conn_status_label.setStyleSheet("color: #a6e3a1; font-weight: bold;")
        for i in range(1, 5):
            self.tabs.setTabEnabled(i, True)
        self.connect_btn.setEnabled(False)
        self.disconnect_btn.setEnabled(True)
        if self.selected_file_path and self.connected:
            self.upload_btn.setEnabled(True)
            self.ram_upload_btn.setEnabled(True)

    @Slot(str)
    def _on_connect_fail(self, err):
        self.log(f"Connection failed: {err}")
        self.status_label.setText("Connection failed")
        self.connected = False
        self.ser = None
        if self.selected_port:
            self.connect_btn.setEnabled(True)
        QMessageBox.critical(self, "Connection Error",
                             f"Could not connect.\n\n{err}\n\n"
                             "1. Make sure the module is powered on\n"
                             "2. Verify it's paired\n"
                             "3. Try a different baud rate\n"
                             "4. Make sure the COM port is not in use")

    def disconnect_port(self):
        self.keep_reading = False
        if self.reading_thread and self.reading_thread.is_alive():
            self.reading_thread.join(timeout=1.0)
        if self.ser:
            try:
                self.ser.close()
            except Exception:
                pass
        self.ser = None
        self.connected = False
        self.log("Disconnected")
        self.status_label.setText("Disconnected")
        self.conn_port_label.setText("None")
        self.conn_status_label.setText("Disconnected")
        self.conn_status_label.setStyleSheet("color: #f38ba8; font-weight: bold;")
        for i in range(1, 5):
            self.tabs.setTabEnabled(i, False)
        self.tabs.setCurrentIndex(0)
        self.disconnect_btn.setEnabled(False)
        if self.selected_port:
            self.connect_btn.setEnabled(True)

    # ════════════════════════════════════════════════════════════════════
    #  Serial Reading Thread
    # ════════════════════════════════════════════════════════════════════
    def _read_serial(self):
        receive_buffer = bytearray()
        last_display = 0
        INTERVAL = 0.1

        while self.keep_reading and self.ser:
            try:
                if not self.ser.is_open:
                    break
                if self.ser.in_waiting > 0:
                    data = self.ser.read(self.ser.in_waiting)
                    receive_buffer.extend(data)
                    with self.vofa_raw_lock:
                        self.vofa_raw_buf.extend(data)

                now = time.time()
                if receive_buffer and (now - last_display) >= INTERVAL:
                    self.sig.data_received.emit(bytes(receive_buffer))
                    receive_buffer.clear()
                    last_display = now
                time.sleep(0.005)
            except serial.SerialException as e:
                if self.connected:
                    self.sig.log_message.emit(f"Serial error: {e}")
                break
            except Exception as e:
                if self.connected:
                    self.sig.log_message.emit(f"Read error: {e}")
                break

    @Slot(bytes)
    def _on_data_received(self, data):
        if not data:
            return
        try:
            text_data = data.decode('utf-8', errors='replace')
        except Exception:
            text_data = "<?>"
        hex_data = ' '.join(f'{b:02X}' for b in data)

        mode = self.display_mode
        if mode == "TEXT":
            display = text_data
        elif mode == "HEX":
            display = hex_data
        else:
            display = f"{text_data}\n[HEX: {hex_data}]"

        ts = time.strftime("%H:%M:%S")
        if self.tabs.currentIndex() == 1 and self.show_received_cb.isChecked():
            append_log(self.receive_text, f"[{ts}] {display}")

    @Slot(str)
    def _on_log(self, msg):
        self.log(msg)

    # ════════════════════════════════════════════════════════════════════
    #  Helpers
    # ════════════════════════════════════════════════════════════════════
    def log(self, msg):
        ts = time.strftime("%H:%M:%S")
        append_log(self.log_text, f"[{ts}] {msg}")

    def _get_line_ending_str(self):
        le = self.line_ending
        if le == "CR":
            return "\r"
        elif le == "LF":
            return "\n"
        elif le == "CRLF":
            return "\r\n"
        return ""

    def send_raw_data(self, data, silent=False):
        if not self.connected or not self.ser:
            return
        try:
            data += self._get_line_ending_str()
            self.ser.write(data.encode('utf-8'))
            if not silent:
                self.log(f"Sent: {data.strip()}")
        except Exception as e:
            if not silent:
                self.log(f"Send error: {e}")

    def calculate_checksum(self, data):
        cs = 0
        for b in data:
            cs ^= b
        return cs

    def _stop_reading_thread(self):
        self.keep_reading = False
        if self.reading_thread and self.reading_thread.is_alive():
            self.reading_thread.join(timeout=1.0)

    def _restart_reading_thread(self):
        if self.connected and self.ser and not self.keep_reading:
            self.keep_reading = True
            self.reading_thread = threading.Thread(target=self._read_serial, daemon=True)
            self.reading_thread.start()

    # ════════════════════════════════════════════════════════════════════
    #  Serial Testing Tab Actions
    # ════════════════════════════════════════════════════════════════════
    def send_data(self):
        if not self.connected:
            return
        data = self.data_entry.text()
        if not data:
            return
        self.send_raw_data(data)
        self.data_entry.clear()
        self.data_entry.setFocus()

    def quick_send(self, msg):
        if not self.connected:
            return
        self.send_raw_data(msg)

    def send_hex_data(self):
        if not self.connected:
            return
        hex_str = self.hex_entry.text().strip()
        if not hex_str:
            return
        try:
            hex_bytes = bytearray(int(h, 16) for h in hex_str.split())
            self.ser.write(hex_bytes)
            self.log(f"Sent (hex): {' '.join(f'{b:02X}' for b in hex_bytes)}")
        except Exception as e:
            self.log(f"Hex send error: {e}")

    # ════════════════════════════════════════════════════════════════════
    #  Control Tab
    # ════════════════════════════════════════════════════════════════════
    def control_send(self, cmd, direction):
        if not self.connected:
            return
        try:
            self.send_raw_data(cmd, silent=True)
            self.control_status.setText(f"Sent: {direction}")
            ts = time.strftime("%H:%M:%S")
            append_log(self.control_log_text, f"[{ts}] {direction} ({cmd})")
        except Exception as e:
            self.control_status.setText(f"Error: {e}")

    def keyPressEvent(self, event: QKeyEvent):
        # Control tab key handling
        if self.tabs.currentIndex() == 3 and self.connected:
            key = event.key()
            if key not in self.control_keys_held:
                self.control_keys_held.add(key)
                mapping = {
                    Qt.Key.Key_W: (":w", "Up"),
                    Qt.Key.Key_S: (":s", "Down"),
                    Qt.Key.Key_D: (":d", "Enter"),
                    Qt.Key.Key_A: (":a", "Back"),
                }
                if key in mapping:
                    self.control_send(*mapping[key])
                    return

        super().keyPressEvent(event)

    def keyReleaseEvent(self, event: QKeyEvent):
        self.control_keys_held.discard(event.key())
        super().keyReleaseEvent(event)

    # ════════════════════════════════════════════════════════════════════
    #  Command Tab
    # ════════════════════════════════════════════════════════════════════
    def cmd_send_raw(self):
        if not self.connected:
            return
        cmd = self.cmd_entry.text().strip()
        if not cmd:
            return
        self.send_raw_data(cmd, silent=True)
        self._cmd_log(cmd)
        self.cmd_entry.clear()

    def cmd_quick(self, cmd):
        if not self.connected:
            return
        self.send_raw_data(cmd, silent=True)
        self._cmd_log(cmd)

    def cmd_send_dist(self):
        if not self.connected:
            return
        try:
            dist = float(self.cmd_dist_entry.text())
            cmd = f";D{dist:.2f}"
            self.send_raw_data(cmd, silent=True)
            self._cmd_log(cmd)
        except ValueError:
            pass

    def _cmd_log(self, cmd):
        ts = time.strftime("%H:%M:%S")
        append_log(self.cmd_log_text, f"[{ts}] Sent: {cmd}")

    # ════════════════════════════════════════════════════════════════════
    #  File Upload
    # ════════════════════════════════════════════════════════════════════
    def browse_txt_file(self):
        filepath, _ = QFileDialog.getOpenFileName(
            self, "Select TXT File", "",
            "Text files (*.txt *.TXT);;All files (*.*)")
        if not filepath:
            return
        self.selected_file_path = filepath
        self.file_path_label.setText(filepath)
        self.file_path_label.setStyleSheet("color: #cdd6f4;")

        file_size = os.path.getsize(filepath)
        filename = os.path.basename(filepath)
        name, ext = os.path.splitext(filename)
        total_chunks = (file_size + FT_CHUNK_SIZE - 1) // FT_CHUNK_SIZE

        if len(name) > 8:
            self.file_info_label.setText(
                f"WARNING: Filename '{name}' exceeds 8 characters!\n"
                f"Will be truncated to: {name[:8]}{ext}\n"
                f"Size: {file_size:,} bytes ({total_chunks} chunks)")
            self.file_info_label.setStyleSheet("color: #f9e2af;")
        else:
            self.file_info_label.setText(
                f"Filename: {filename}\nSize: {file_size:,} bytes\nChunks: {total_chunks}")
            self.file_info_label.setStyleSheet("color: #a6adc8;")

        if file_size > 65535:
            self.upload_btn.setEnabled(False)
            self.ram_upload_btn.setEnabled(False)
            self._upload_log("ERROR: File too large (max 64KB)")
        elif self.connected:
            self.upload_btn.setEnabled(True)
            self.ram_upload_btn.setEnabled(True)

    def _upload_log(self, msg):
        ts = time.strftime("%H:%M:%S")
        append_log(self.upload_log_text, f"[{ts}] {msg}")

    @Slot(float, str)
    def _on_upload_progress(self, pct, label):
        self.progress_bar.setValue(int(pct))
        self.progress_label.setText(label)

    @Slot(str)
    def _on_upload_log(self, msg):
        self._upload_log(msg)

    @Slot(bool)
    def _on_upload_done(self, success):
        self.progress_label.setText("Upload Complete!" if success else "Upload Failed")
        self.upload_btn.setEnabled(True)
        self.ram_upload_btn.setEnabled(True)
        self.abort_btn.setEnabled(False)
        self._restart_reading_thread()
        if success:
            self.refresh_device_files()

    @Slot()
    def _on_ram_done(self):
        self.upload_btn.setEnabled(True)
        self.ram_upload_btn.setEnabled(True)
        self.abort_btn.setEnabled(False)
        self._restart_reading_thread()

    def abort_upload(self):
        self.upload_abort_flag = True
        self._upload_log("Upload aborted by user")

    def start_upload(self):
        if not self.selected_file_path:
            return
        self.upload_btn.setEnabled(False)
        self.ram_upload_btn.setEnabled(False)
        self.abort_btn.setEnabled(True)
        self.upload_abort_flag = False
        self.progress_bar.setValue(0)
        self._stop_reading_thread()
        threading.Thread(target=self._upload_file_thread, daemon=True).start()

    def _upload_file_thread(self):
        try:
            filepath = self.selected_file_path
            file_size = os.path.getsize(filepath)
            filename = os.path.basename(filepath)
            name, ext = os.path.splitext(filename)
            name = name[:8].upper()
            ext = ext[:4].upper()
            total_chunks = (file_size + FT_CHUNK_SIZE - 1) // FT_CHUNK_SIZE

            self.sig.upload_log.emit(
                f"Starting upload: {name}{ext} ({file_size} bytes, {total_chunks} chunks)")

            with open(filepath, 'rb') as f:
                file_data = f.read()

            # FILE_START
            start_packet = bytearray([FT_CMD_FILE_START])
            start_packet.extend(struct.pack('<H', file_size))
            start_packet.extend(struct.pack('<H', total_chunks))
            start_packet.extend(name.ljust(8, '\x00').encode('ascii')[:8])
            start_packet.extend(ext.ljust(4, '\x00').encode('ascii')[:4])
            start_packet.append(self.calculate_checksum(start_packet))

            self.sig.upload_log.emit("Sending FILE_START...")
            start_success = False
            for start_retry in range(10):
                self.ser.reset_input_buffer()
                self.ser.write(start_packet)
                response = self._wait_for_response(timeout=2.0)
                if response and response[0] == FT_RSP_READY:
                    start_success = True
                    break
                elif response and response[0] == FT_RSP_ERROR:
                    err = response[1] if len(response) > 1 else 0
                    err_msgs = {1: "checksum", 2: "sequence", 3: "SD write", 4: "SD full",
                                5: "file exists", 6: "bad name", 7: "timeout", 8: "overflow"}
                    self.sig.upload_log.emit(f"Device error: {err_msgs.get(err, err)}")
                    self.sig.upload_done.emit(False)
                    return
                else:
                    rsp_hex = ' '.join(f'{b:02X}' for b in response) if response else "None"
                    self.sig.upload_log.emit(f"Retry {start_retry+1}/10 got:[{rsp_hex}]")
                time.sleep(0.1 * (start_retry + 1))

            if not start_success:
                self.sig.upload_log.emit("Failed to start transfer")
                self.sig.upload_done.emit(False)
                return

            time.sleep(0.15)
            stale = self.ser.in_waiting
            if stale:
                self.ser.read(stale)
            self.ser.reset_input_buffer()

            self.sig.upload_log.emit("Device ready, sending chunks...")
            start_time = time.time()
            bytes_sent = 0

            for chunk_idx in range(total_chunks):
                if self.upload_abort_flag:
                    self._send_abort()
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

                success = False
                for retry in range(10):
                    if retry > 0:
                        time.sleep(min(50 * (retry + 1), 300) / 1000.0)
                    self.ser.reset_input_buffer()
                    self.ser.write(data_packet)

                    base_timeout = 1.0 if ((chunk_idx + 1) % 8 == 0) else 0.5
                    response = self._wait_for_response(timeout=base_timeout + 0.2 * retry)

                    if response and response[0] == FT_RSP_ACK:
                        success = True
                        bytes_sent += data_len
                        time.sleep(0.005 if (chunk_idx + 1) % 8 == 0 else 0.001)
                        break
                    elif response and response[0] == FT_RSP_ERROR:
                        err = response[1] if len(response) > 1 else 0
                        err_msgs = {1: "checksum", 2: "sequence", 3: "SD write", 4: "SD full",
                                    5: "file exists", 6: "bad name", 7: "timeout", 8: "overflow"}
                        self.sig.upload_log.emit(f"Device error: {err_msgs.get(err, err)}")
                        self.sig.upload_done.emit(False)
                        return

                if not success:
                    self.sig.upload_log.emit(f"FAILED at chunk {chunk_idx} - aborting")
                    self.sig.upload_done.emit(False)
                    return

                elapsed = time.time() - start_time
                speed = bytes_sent / elapsed if elapsed > 0 else 0
                progress = (chunk_idx + 1) / total_chunks * 100
                self.sig.upload_progress.emit(progress, f"{chunk_idx+1}/{total_chunks} | {speed:.0f} B/s")

            # FILE_END
            self.sig.upload_log.emit("Finalizing...")
            end_packet = bytearray([FT_CMD_FILE_END])
            end_packet.extend(struct.pack('<H', file_size & 0xFFFF))
            end_packet.append(self.calculate_checksum(end_packet))

            end_success = False
            for end_retry in range(10):
                self.ser.reset_input_buffer()
                self.ser.write(end_packet)
                response = self._wait_for_response(timeout=2.0 + 0.5 * end_retry)
                if response and response[0] == FT_RSP_SUCCESS:
                    end_success = True
                    elapsed = time.time() - start_time
                    avg_speed = file_size / elapsed if elapsed > 0 else 0
                    self.sig.upload_log.emit(
                        f"Upload complete! {avg_speed:.0f} B/s avg, {elapsed:.1f}s total")
                    break
                time.sleep(0.2 * (end_retry + 1))

            self.sig.upload_done.emit(end_success)
            if not end_success:
                self.sig.upload_log.emit("Failed to finalize")

        except Exception as e:
            self.sig.upload_log.emit(f"Error: {e}")
            self.sig.upload_done.emit(False)

    def _send_abort(self):
        abort_packet = bytearray([FT_CMD_FILE_ABORT])
        abort_packet.append(self.calculate_checksum(abort_packet))
        self.ser.write(abort_packet)
        self.sig.upload_log.emit("Abort sent to device")
        self.sig.upload_done.emit(False)

    def _wait_for_response(self, timeout=2.0):
        old_timeout = self.ser.timeout
        try:
            deadline = time.time() + timeout
            skipped = 0
            leftover = bytearray()
            first_byte = None

            while time.time() < deadline:
                waiting = self.ser.in_waiting
                if waiting > 0:
                    chunk = self.ser.read(waiting)
                    for i, b in enumerate(chunk):
                        if 0xA0 <= b <= 0xA4:
                            first_byte = b
                            leftover = bytearray(chunk[i + 1:])
                            skipped += i
                            break
                    else:
                        skipped += len(chunk)
                    if first_byte is not None:
                        break
                else:
                    time.sleep(0.002)

            if first_byte is None:
                return None

            pkt_lens = {
                FT_RSP_ACK: 4, FT_RSP_NAK: 5, FT_RSP_SUCCESS: 4, FT_RSP_ERROR: 3,
            }
            expected_len = pkt_lens.get(first_byte, 2)
            remaining = expected_len - 1

            response = bytearray([first_byte])
            if remaining <= len(leftover):
                response.extend(leftover[:remaining])
            else:
                response.extend(leftover)
                still_need = remaining - len(leftover)
                self.ser.timeout = 0.5
                rest = self.ser.read(still_need)
                response.extend(rest)

            return bytes(response)
        except Exception:
            return None
        finally:
            self.ser.timeout = old_timeout

    # ════════════════════════════════════════════════════════════════════
    #  RAM Upload
    # ════════════════════════════════════════════════════════════════════
    def start_ram_upload(self):
        if not self.selected_file_path:
            return
        self.upload_btn.setEnabled(False)
        self.ram_upload_btn.setEnabled(False)
        self.abort_btn.setEnabled(True)
        self.upload_abort_flag = False
        self.progress_bar.setValue(0)
        self._stop_reading_thread()
        threading.Thread(target=self._ram_upload_thread, daemon=True).start()

    def _ram_upload_thread(self):
        try:
            filepath = self.selected_file_path
            self.sig.upload_log.emit(f"RAM upload: parsing {os.path.basename(filepath)}")

            rows = []
            with open(filepath, 'r') as f:
                for line_num, line in enumerate(f, 1):
                    line = line.strip()
                    if not line or line.startswith('#'):
                        continue
                    parts = line.replace(',', ' ').split()
                    parts = [p.rstrip('fF') for p in parts]
                    if len(parts) != 15:
                        self.sig.upload_log.emit(
                            f"WARN: line {line_num} has {len(parts)} values (expected 15), skipping")
                        continue
                    if len(rows) >= 365:
                        self.sig.upload_log.emit("Max 365 chords reached, truncating")
                        break
                    rows.append([float(v) for v in parts])

            num_rows = len(rows)
            if num_rows == 0:
                self.sig.upload_log.emit("ERROR: No valid chord data found")
                self.sig.ram_done.emit()
                return

            self.sig.upload_log.emit(f"Parsed {num_rows} rows ({num_rows*14} floats, {num_rows*56} bytes)")

            self.ser.reset_input_buffer()
            self.ser.reset_output_buffer()
            time.sleep(0.1)

            # RAM_START
            self.sig.upload_log.emit("Sending RAM_START...")
            start_ok = False
            for attempt in range(5):
                self.ser.reset_input_buffer()
                self.ser.write(bytearray([FT_CMD_RAM_START, RAM_SENTINEL_START_BYTE]))
                response = self._wait_for_response(timeout=2.0)
                if response and response[0] == FT_RSP_READY:
                    start_ok = True
                    break
                time.sleep(0.1 * (attempt + 1))

            if not start_ok:
                self.sig.upload_log.emit("Failed to get READY from device")
                self.sig.ram_done.emit()
                return

            self.sig.upload_log.emit("Device ready, sending rows...")
            time.sleep(0.1)
            stale = self.ser.in_waiting
            if stale:
                self.ser.read(stale)
            self.ser.reset_input_buffer()

            start_time = time.time()

            for row_idx in range(num_rows):
                if self.upload_abort_flag:
                    self.sig.upload_log.emit("Upload aborted")
                    self.sig.ram_done.emit()
                    return

                row = rows[row_idx]
                pressed_bytes = bytes([1 if row[2 + j] != 0.0 else 0 for j in range(10)])
                chord_data = (
                    struct.pack('<2f', row[0], row[1])
                    + pressed_bytes
                    + struct.pack('<H', int(row[12]))
                    + bytes([1 if row[13] != 0.0 else 0, 1 if row[14] != 0.0 else 0])
                    + bytes(2)
                )
                packet = bytearray([FT_CMD_RAM_DATA])
                packet.extend(struct.pack('<H', row_idx))
                packet.extend(chord_data)
                packet.append(self.calculate_checksum(packet))

                success = False
                for retry in range(10):
                    if retry > 0:
                        time.sleep(0.05 * (retry + 1))
                    self.ser.reset_input_buffer()
                    self.ser.write(packet)
                    response = self._wait_for_response(timeout=1.0 + 0.2 * retry)
                    if response and response[0] == FT_RSP_ACK:
                        success = True
                        time.sleep(0.002)
                        break

                if not success:
                    self.sig.upload_log.emit(f"FAILED at row {row_idx} after 10 retries")
                    self.sig.ram_done.emit()
                    return

                progress = (row_idx + 1) / num_rows * 100
                elapsed = time.time() - start_time
                speed = (row_idx + 1) * 56 / elapsed if elapsed > 0 else 0
                self.sig.upload_progress.emit(progress, f"{row_idx+1}/{num_rows} rows | {speed:.0f} B/s")

            # RAM_END
            self.sig.upload_log.emit("Finalizing...")
            end_ok = False
            for attempt in range(5):
                self.ser.reset_input_buffer()
                self.ser.write(bytearray([FT_CMD_RAM_END, RAM_SENTINEL_END_BYTE]))
                response = self._wait_for_response(timeout=2.0)
                if response and response[0] == FT_RSP_SUCCESS:
                    end_ok = True
                    break
                time.sleep(0.2 * (attempt + 1))

            elapsed = time.time() - start_time
            avg_speed = num_rows * 56 / elapsed if elapsed > 0 else 0
            if end_ok:
                self.sig.upload_log.emit(
                    f"RAM upload complete! {num_rows} rows, {avg_speed:.0f} B/s, {elapsed:.1f}s")
                self.sig.upload_progress.emit(100, "RAM Upload Complete!")
            else:
                self.sig.upload_log.emit("Failed to finalize")

            self.sig.ram_done.emit()

        except Exception as e:
            self.sig.upload_log.emit(f"Error: {e}")
            self.sig.ram_done.emit()

    def refresh_device_files(self):
        if not self.connected:
            return
        self.device_files_tree.clear()
        was_reading = self.keep_reading
        self.keep_reading = False
        time.sleep(0.1)
        try:
            self.ser.reset_input_buffer()
            self.ser.write(b"/listFiles\n")
            lines = []
            timeout_t = time.time() + 3.0
            while time.time() < timeout_t:
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
                        self.device_files_tree.addTopLevelItem(
                            QTreeWidgetItem([parts[0], parts[1]]))
        except Exception as e:
            self._upload_log(f"Error reading file list: {e}")
        finally:
            if was_reading:
                self.keep_reading = True
                self.reading_thread = threading.Thread(target=self._read_serial, daemon=True)
                self.reading_thread.start()

    # ════════════════════════════════════════════════════════════════════
    #  VOFA Oscilloscope
    # ════════════════════════════════════════════════════════════════════
    def _vofa_tick(self):
        self._vofa_parse()
        if not self.vofa_paused and len(self.vofa_time) > 0:
            t_end = self.vofa_time[-1]
            t_start = t_end - self.vofa_window_ms

            mask = self.vofa_time >= t_start
            t_vis = self.vofa_time[mask]

            for i, curve in enumerate(self.vofa_curves):
                if self.vofa_ch_checks[i].isChecked():
                    d = self.vofa_data[i][mask]
                    curve.setData(t_vis, d)
                    curve.setVisible(True)
                else:
                    curve.setVisible(False)

            self.vofa_plot.setXRange(t_start, t_end, padding=0)

            # Auto Y-range based on visible window data
            vis_min, vis_max = float('inf'), float('-inf')
            for i in range(self.vofa_ch_count):
                if self.vofa_ch_checks[i].isChecked() and len(self.vofa_data[i]) > 0:
                    d = self.vofa_data[i][mask]
                    if len(d) > 0:
                        vis_min = min(vis_min, float(d.min()))
                        vis_max = max(vis_max, float(d.max()))
            if vis_min != float('inf'):
                if vis_min == vis_max:
                    margin = max(abs(vis_min) * 0.1, 1.0)
                else:
                    margin = (vis_max - vis_min) * 0.08
                self.vofa_plot.setYRange(vis_min - margin, vis_max + margin, padding=0)

            # Value labels
            for i in range(self.vofa_ch_count):
                if len(self.vofa_data[i]) > 0:
                    self.vofa_val_labels[i].setText(f"{self.vofa_data[i][-1]:>8.3f}")

        # FPS display
        self.vofa_frame_count_display = getattr(self, 'vofa_frame_count_display', 0)
        now = time.time()
        if now - self._vofa_fps_t >= 1.0:
            elapsed = now - self._vofa_fps_t
            data_fps = self.vofa_frame_count / elapsed
            total_pts = len(self.vofa_time)
            self.vofa_status_label.setText(
                f"{self.vofa_frame_count} frames  "
                f"data {data_fps:.0f} fps  "
                f"total {total_pts} pts")
            self.vofa_frame_count = 0
            self._vofa_fps_t = now

    def _vofa_parse(self):
        with self.vofa_raw_lock:
            chunk = bytes(self.vofa_raw_buf)
            self.vofa_raw_buf.clear()
        if not chunk:
            return

        self._vofa_carry.extend(chunk)
        tail = self.vofa_tail
        fbytes = self.vofa_frame_bytes
        n = self.vofa_ch_count

        new_times = []
        new_data = [[] for _ in range(n)]

        while True:
            idx = self._vofa_carry.find(tail)
            if idx < 0:
                if len(self._vofa_carry) > fbytes + 4:
                    self._vofa_carry = self._vofa_carry[-(fbytes + 3):]
                break
            if idx >= fbytes:
                frame = self._vofa_carry[idx - fbytes: idx]
                try:
                    floats = struct.unpack(f'<{n}f', frame)
                except struct.error:
                    self._vofa_carry = self._vofa_carry[idx + 4:]
                    continue

                self.vofa_t_ms += 1
                new_times.append(self.vofa_t_ms)
                for i, v in enumerate(floats):
                    new_data[i].append(v)
                self.vofa_frame_count += 1

            self._vofa_carry = self._vofa_carry[idx + 4:]

        if new_times:
            new_t = np.array(new_times, dtype=np.float64)
            self.vofa_time = np.concatenate([self.vofa_time, new_t])
            for i in range(n):
                nd = np.array(new_data[i], dtype=np.float64)
                self.vofa_data[i] = np.concatenate([self.vofa_data[i], nd])

            # Trim
            if len(self.vofa_time) > self.vofa_max_points:
                excess = len(self.vofa_time) - self.vofa_max_points
                self.vofa_time = self.vofa_time[excess:]
                for i in range(n):
                    self.vofa_data[i] = self.vofa_data[i][excess:]

    def _vofa_toggle_pause(self):
        self.vofa_paused = not self.vofa_paused
        self.vofa_pause_btn.setText(
            "\u25b6 Resume" if self.vofa_paused else "\u23f8 Pause")

    def _vofa_clear(self):
        self.vofa_time = np.empty(0, dtype=np.float64)
        self.vofa_data = [np.empty(0, dtype=np.float64) for _ in range(self.vofa_ch_count)]
        self._vofa_carry.clear()
        self.vofa_t_ms = 0
        self.vofa_frame_count = 0
        for curve in self.vofa_curves:
            curve.setData([], [])
        self.vofa_status_label.setText("Cleared")
        for lbl in self.vofa_val_labels:
            lbl.setText("  -.---")

    # ════════════════════════════════════════════════════════════════════
    #  Window close
    # ════════════════════════════════════════════════════════════════════
    def closeEvent(self, event):
        if self.connected:
            self.disconnect_port()
        event.accept()


# ════════════════════════════════════════════════════════════════════════
#  Entry point
# ════════════════════════════════════════════════════════════════════════
if __name__ == "__main__":
    try:
        import serial
    except ImportError:
        print("ERROR: pip install pyserial")
        sys.exit(1)
    try:
        import pyqtgraph
    except ImportError:
        print("ERROR: pip install pyqtgraph")
        sys.exit(1)

    app = QApplication(sys.argv)
    app.setStyleSheet(DARK_STYLE)

    # High-DPI
    app.setHighDpiScaleFactorRoundingPolicy(
        Qt.HighDpiScaleFactorRoundingPolicy.PassThrough)

    window = HC04MainWindow()
    window.show()
    sys.exit(app.exec())
