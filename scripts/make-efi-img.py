from gpt_image.disk import Disk
from gpt_image.partition import PartitionType
from FATtools import partutils, mkfat, Volume
import platform
import subprocess
import os
import sys

if not len(sys.argv) == 4:
    print(
        f"Usage: {sys.argv[0]} <disk image output> <kernel executable> <limine config>",
        file=sys.stderr,
    )
    exit(1)

output = os.path.abspath(sys.argv[1])
kernel = os.path.abspath(sys.argv[2])
config = os.path.abspath(sys.argv[3])

if not (os.path.exists(kernel) and os.path.exists(config)):
    print("Error: either kernel or limine configuration doesn't exist", file=sys.stderr)
    exit(2)

with open(output, "wb") as img:
    img.truncate(64 * 1024 * 1024)

disk = Volume.vopen(output, "r+b", "disk")
gpt = partutils.partition(disk)
Volume.vclose(disk)

disk = Disk.open(output)
disk.table.partitions.entries[0].type_guid = PartitionType.EFI_SYSTEM_PARTITION.value
disk.commit()

part = Volume.vopen(output, "r+b", "partition0")
mkfat.fat_mkfs(part, part.size, params={"fat_bits": 32, "wanted_cluster": 512})

limine_dir = os.path.abspath("./.tmp-limine")
limine_deploy = os.path.join(
    limine_dir,
    "limine-deploy.exe" if platform.system() == "Windows" else "limine-deploy",
)

Volume.vclose(part)

# clone limine
if not os.path.exists(limine_dir):
    subprocess.check_output(
        [
            "git",
            "clone",
            "https://github.com/limine-bootloader/limine.git",
            "--branch=v4.20230530.0-binary",
            "--depth=1",
            limine_dir,
        ],
        stderr=subprocess.STDOUT,
    )

    # make limine tools
    subprocess.check_output(
        ["make", "-s", "-C", limine_dir],
        stderr=subprocess.STDOUT,
    )

# deploy limine to output disk image
subprocess.check_output(
    [limine_deploy, output],
    stderr=subprocess.STDOUT,
)

vol = Volume.vopen(output, "r+b")

Volume.copy_in([config, os.path.join(limine_dir, "limine.sys")], vol)

efi_boot = vol.mkdir("EFI").mkdir("BOOT")

Volume.copy_in([os.path.join(limine_dir, "BOOTX64.EFI")], efi_boot)

system = vol.mkdir("SYSTEM")

Volume.copy_in([kernel], system)

Volume.vclose(vol)
