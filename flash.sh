#!/bin/bash

# mount drive
sudo mount -t drvfs ${1^}: /mnt/$1

# flash binary
cp BUILD/*/GCC_ARM/*.bin /mnt/$1

# unmount drive
sudo umount /mnt/$1
