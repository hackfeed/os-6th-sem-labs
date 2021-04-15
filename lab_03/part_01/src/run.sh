#!/bin/bash
make
echo ">>>>> Loading md1"
insmod md1.ko
lsmod | grep md
echo ">>>>> System log after module insert:"
dmesg
echo ">>>>> Removing md1"
rmmod md1
echo ">>>>> System log after module removal:"
dmesg
