"""
SD Card Hex Reader for Windows 11
Auto-detects removable drives and reads raw content in hex format.
Requires Administrator privileges to read raw disk data.
"""

import ctypes
import sys
import string


def is_admin():
    """Check if script is running with administrator privileges."""
    try:
        return ctypes.windll.shell32.IsUserAnAdmin()
    except:
        return False


def get_removable_drives():
    """Detect all removable drives (SD cards, USB drives, etc.)."""
    drives = []
    bitmask = ctypes.windll.kernel32.GetLogicalDrives()
    
    for letter in string.ascii_uppercase:
        if bitmask & 1:
            drive_path = f"{letter}:\\"
            drive_type = ctypes.windll.kernel32.GetDriveTypeW(drive_path)
            # Drive type 2 = DRIVE_REMOVABLE
            if drive_type == 2:
                # Get volume info
                volume_name = ctypes.create_unicode_buffer(1024)
                ctypes.windll.kernel32.GetVolumeInformationW(
                    drive_path,
                    volume_name,
                    1024,
                    None, None, None, None, 0
                )
                drives.append({
                    'letter': letter,
                    'path': drive_path,
                    'name': volume_name.value or "Unnamed"
                })
        bitmask >>= 1
    
    return drives


def read_drive_hex(drive_letter, offset=0, length=512):
    """
    Read raw bytes from a drive and return hex data.
    
    Args:
        drive_letter: The drive letter (e.g., 'E')
        offset: Starting byte offset (should be multiple of 512 for sector alignment)
        length: Number of bytes to read (should be multiple of 512)
    """
    # Align to sector boundaries
    sector_size = 512
    aligned_offset = (offset // sector_size) * sector_size
    aligned_length = ((length + sector_size - 1) // sector_size) * sector_size
    
    drive_path = f"\\\\.\\{drive_letter}:"
    
    GENERIC_READ = 0x80000000
    FILE_SHARE_READ = 0x00000001
    FILE_SHARE_WRITE = 0x00000002
    OPEN_EXISTING = 3
    FILE_FLAG_NO_BUFFERING = 0x20000000
    
    handle = ctypes.windll.kernel32.CreateFileW(
        drive_path,
        GENERIC_READ,
        FILE_SHARE_READ | FILE_SHARE_WRITE,
        None,
        OPEN_EXISTING,
        FILE_FLAG_NO_BUFFERING,
        None
    )
    
    if handle == -1:
        error = ctypes.windll.kernel32.GetLastError()
        raise OSError(f"Failed to open drive {drive_letter}: (Error {error}). "
                     "Make sure you're running as Administrator.")
    
    try:
        # Seek to offset
        high_offset = ctypes.c_long(aligned_offset >> 32)
        low_result = ctypes.windll.kernel32.SetFilePointer(
            handle, 
            aligned_offset & 0xFFFFFFFF,
            ctypes.byref(high_offset),
            0  # FILE_BEGIN
        )
        
        if low_result == 0xFFFFFFFF and ctypes.windll.kernel32.GetLastError() != 0:
            raise OSError("Failed to seek to offset")
        
        # Read data
        buffer = ctypes.create_string_buffer(aligned_length)
        bytes_read = ctypes.c_ulong(0)
        
        success = ctypes.windll.kernel32.ReadFile(
            handle,
            buffer,
            aligned_length,
            ctypes.byref(bytes_read),
            None
        )
        
        if not success:
            error = ctypes.windll.kernel32.GetLastError()
            raise OSError(f"Failed to read drive (Error {error})")
        
        return buffer.raw[:bytes_read.value], aligned_offset
        
    finally:
        ctypes.windll.kernel32.CloseHandle(handle)


def format_hex_dump(data, start_address=0, bytes_per_line=16):
    """Format binary data as a hex dump with addresses and ASCII."""
    lines = []
    
    for i in range(0, len(data), bytes_per_line):
        chunk = data[i:i + bytes_per_line]
        address = start_address + i
        
        # Hex part
        hex_part = ' '.join(f'{b:02X}' for b in chunk)
        hex_part = hex_part.ljust(bytes_per_line * 3 - 1)
        
        # ASCII part
        ascii_part = ''.join(
            chr(b) if 32 <= b < 127 else '.'
            for b in chunk
        )
        
        lines.append(f"{address:08X}  {hex_part}  |{ascii_part}|")
    
    return '\n'.join(lines)


def main():
    print("=" * 70)
    print("SD Card Hex Reader for Windows 11")
    print("=" * 70)
    
    # Check admin privileges
    if not is_admin():
        print("\n[!] This script requires Administrator privileges.")
        print("    Right-click and select 'Run as administrator'")
        input("\nPress Enter to exit...")
        sys.exit(1)
    
    # Detect removable drives
    print("\n[*] Scanning for removable drives...")
    drives = get_removable_drives()
    
    if not drives:
        print("\n[!] No removable drives (SD cards) detected.")
        print("    Make sure your SD card is inserted and mounted.")
        input("\nPress Enter to exit...")
        sys.exit(1)
    
    # Display detected drives
    print(f"\n[+] Found {len(drives)} removable drive(s):\n")
    for i, drive in enumerate(drives):
        print(f"    [{i + 1}] {drive['letter']}: - {drive['name']}")
    
    # Select drive
    print()
    while True:
        try:
            choice = input("Select drive number (or 'q' to quit): ").strip()
            if choice.lower() == 'q':
                sys.exit(0)
            idx = int(choice) - 1
            if 0 <= idx < len(drives):
                selected = drives[idx]
                break
            print("Invalid selection.")
        except ValueError:
            print("Please enter a number.")
    
    # Get read parameters
    print(f"\n[*] Selected: {selected['letter']}: ({selected['name']})")
    
    try:
        offset_str = input("Start offset in bytes [0]: ").strip() or "0"
        if offset_str.lower().startswith("0x"):
            offset = int(offset_str, 16)
        else:
            offset = int(offset_str)
    except ValueError:
        offset = 0
    
    try:
        length_str = input("Bytes to read [512]: ").strip() or "512"
        length = int(length_str)
        length = min(length, 65536)  # Cap at 64KB for safety
    except ValueError:
        length = 512
    
    # Read and display
    print(f"\n[*] Reading {length} bytes from offset {offset} (0x{offset:X})...")
    print("-" * 70)
    
    try:
        data, actual_offset = read_drive_hex(selected['letter'], offset, length)
        print(f"[+] Read {len(data)} bytes from offset 0x{actual_offset:X}\n")
        print(format_hex_dump(data, actual_offset))
        print("-" * 70)
        print(f"[+] Total bytes displayed: {len(data)}")
        
        # Option to save
        save = input("\nSave to file? (y/n): ").strip().lower()
        if save == 'y':
            filename = f"sd_dump_{selected['letter']}_{actual_offset:08X}.bin"
            with open(filename, 'wb') as f:
                f.write(data)
            print(f"[+] Saved raw data to: {filename}")
            
            txt_filename = filename.replace('.bin', '.txt')
            with open(txt_filename, 'w') as f:
                f.write(format_hex_dump(data, actual_offset))
            print(f"[+] Saved hex dump to: {txt_filename}")
            
    except OSError as e:
        print(f"\n[!] Error: {e}")
    
    input("\nPress Enter to exit...")


if __name__ == "__main__":
    main()
