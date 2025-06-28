import os
import re
import shutil
import tkinter as tk
from tkinter import filedialog, messagebox

# Where to look for libraries
LIBRARY_DIRS = [
    os.path.expanduser("~/Documents/Arduino/libraries"),
    os.path.expanduser("~/Arduino/libraries"),
    os.path.expanduser("~/Library/Arduino15/packages/arduino/hardware/avr/1.8.6/libraries"),
]

def extract_prototypes(code):
    pattern = re.compile(r'(?:void|int|float|bool|char|long|byte|unsigned|[\w:]+)\s+(?!if|for|while|switch)(\w+)\s*\(([^)]*)\)\s*\{')

    prototypes = []
    for match in pattern.finditer(code):
        return_type = code[match.start():match.start(1)].strip()
        name = match.group(1)
        args = match.group(2).strip()
        prototypes.append(f"{return_type} {name}({args});")
    return prototypes

def find_header_files(code):
    includes = re.findall(r'#include\s+[<"]([^">]+)[">]', code)
    header_files = set()
    for inc in includes:
        if not inc.endswith(".h"):
            inc += ".h"
        header_files.add(inc)
    return header_files

def copy_libraries(header_files, dest_dir):
    lib_dest = os.path.join(dest_dir, "libraries")
    os.makedirs(lib_dest, exist_ok=True)
    copied = []

    for header in header_files:
        found = False
        for base in LIBRARY_DIRS:
            if not os.path.isdir(base):
                continue
            for item in os.listdir(base):
                lib_path = os.path.join(base, item)
                if os.path.isdir(lib_path):
                    for root, dirs, files in os.walk(lib_path):
                        if header in files:
                            shutil.copytree(lib_path, os.path.join(lib_dest, item), dirs_exist_ok=True)
                            copied.append(item)
                            found = True
                            break
                if found:
                    break
            if found:
                break
    return copied

def convert_and_export(ino_path, output_dir):
    with open(ino_path, 'r') as f:
        ino_code = f.read()

    base_name = os.path.splitext(os.path.basename(ino_path))[0]
    cpp_code = f'#include "{base_name}.h"\n#include <Arduino.h>\n\n{ino_code}'
    prototypes = extract_prototypes(ino_code)
    headers = find_header_files(ino_code)

    h_code = f"#ifndef {base_name.upper()}_H\n#define {base_name.upper()}_H\n\n"
    h_code += "#include <Arduino.h>\n\n"
    h_code += "\n".join(prototypes)
    h_code += "\n\n#endif"

    os.makedirs(output_dir, exist_ok=True)
    cpp_path = os.path.join(output_dir, base_name + ".cpp")
    h_path = os.path.join(output_dir, base_name + ".h")
    ino_copy_path = os.path.join(output_dir, os.path.basename(ino_path))

    with open(cpp_path, 'w') as f:
        f.write(cpp_code)
    with open(h_path, 'w') as f:
        f.write(h_code)
    if os.path.abspath(ino_path) != os.path.abspath(ino_copy_path):
        shutil.copy2(ino_path, ino_copy_path)

    copied_libs = copy_libraries(headers, output_dir)
    return cpp_path, h_path, ino_copy_path, copied_libs

# --- GUI ---
def browse_ino():
    file_path = filedialog.askopenfilename(filetypes=[("Arduino sketch", "*.ino")])
    if file_path:
        ino_entry.delete(0, tk.END)
        ino_entry.insert(0, file_path)

def browse_output_dir():
    dir_path = filedialog.askdirectory()
    if dir_path:
        out_entry.delete(0, tk.END)
        out_entry.insert(0, dir_path)

def run_conversion():
    ino_path = ino_entry.get()
    out_dir = out_entry.get()

    if not ino_path.endswith(".ino") or not os.path.isfile(ino_path):
        messagebox.showerror("Error", "Select a valid .ino file.")
        return
    if not os.path.isdir(out_dir):
        messagebox.showerror("Error", "Enter a valid output directory.")
        return

    cpp_path, h_path, ino_copy_path, copied_libs = convert_and_export(ino_path, out_dir)
    msg = f"Converted:\n- {os.path.basename(cpp_path)}\n- {os.path.basename(h_path)}\n- {os.path.basename(ino_copy_path)} (copy)"
    if copied_libs:
        msg += f"\n\nCopied libraries:\n" + "\n".join(copied_libs)
    else:
        msg += "\n\nNo libraries found (check #include or location)."

    messagebox.showinfo("Done", msg)

# GUI layout
root = tk.Tk()
root.title("Arduino INO → CPP/H + libraries (deep scan)")

tk.Label(root, text="Arduino .ino file:").pack()
ino_entry = tk.Entry(root, width=60)
ino_entry.pack(padx=10)
tk.Button(root, text="Browse .ino", command=browse_ino).pack(pady=3)

tk.Label(root, text="Output directory:").pack()
out_entry = tk.Entry(root, width=60)
out_entry.pack(padx=10)
tk.Button(root, text="Select folder", command=browse_output_dir).pack(pady=3)

tk.Button(root, text="Convert and export", command=run_conversion).pack(pady=15)

# Set minimum window size to current size
root.update()
root.minsize(root.winfo_width(), root.winfo_height())

root.mainloop()
