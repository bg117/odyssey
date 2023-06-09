import os
import sys
import subprocess
import shutil
import struct

input_file = sys.argv[1]
output_file = sys.argv[2]

if not len(sys.argv) == 3:
    print(
        f"Usage: {sys.argv[0]} <input PSF> <output PSF>",
        file=sys.stderr,
    )
    exit(1)

if shutil.which("objcopy") is not None:
    objcopy_exec = "objcopy"
elif shutil.which("llvm-objcopy") is not None:
    objcopy_exec = "llvm-objcopy"
else:
    print("Error: cannot locate objcopy/llvm-objcopy",
        file=sys.stderr)
    exit(2)

dir_name = os.path.dirname(input_file)
base_file_name = os.path.basename(input_file)
base_file_name_no_ext = os.path.splitext(base_file_name)[0]

input_file_mod = os.path.join(dir_name, base_file_name + ".tmp")

shutil.copyfile(input_file, input_file_mod)

# Load the object file content
with open(input_file_mod, "r+b") as bin_file:
    bin_data = bytearray(bin_file.read())

    # Prepend the null-terminated base file name and size to the .data section
    base_file_name_bytes = base_file_name_no_ext.encode("utf-8") + b"\x00"
    file_size_padded = struct.pack("<Q", os.stat(input_file).st_size)
    bin_data = base_file_name_bytes + file_size_padded + bin_data

    bin_file.seek(0)
    bin_file.write(bin_data)

subprocess.run(
    [
        objcopy_exec,
        "-I",
        "binary",
        "-O",
        "elf64-x86-64",
        "--rename-section",
        ".data=.fb_fonts,CONTENTS,ALLOC,DATA,LOAD,READONLY",
        input_file_mod,
        output_file,
    ]
)

os.remove(input_file_mod)
