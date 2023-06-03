#!/bin/sh
dd if=/dev/zero bs=1M count=0 seek=64 of=odyssey.img

parted -s odyssey.img mklabel gpt

parted -s odyssey.img mkpart ESP fat32 2048s 100%
parted -s odyssey.img set 1 esp on

# only clone if limine does not exist
if [ ! -d "limine" ]; then
    git clone https://github.com/limine-bootloader/limine.git --branch=v4.20230530.0-binary --depth=1
fi

make -C limine

./limine/limine-deploy odyssey.img

LOOPBACK=$(sudo losetup -Pf --show odyssey.img)

sudo mkfs.vfat -F32 "${LOOPBACK}p1"

mkdir -p root
sudo mount "${LOOPBACK}p1" root

sudo mkdir -p root/system root/EFI/BOOT

sudo cp limine.cfg limine/limine.sys root/
sudo cp odyssey root/system/
sudo cp limine/BOOTX64.EFI root/EFI/BOOT/

sync
sudo umount root
sudo losetup -d "${LOOPBACK}"
