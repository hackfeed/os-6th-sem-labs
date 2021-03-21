#!/bin/bash
make
echo ">>>>> Loading md1"
insmod md1.ko
echo ">>>>> Loading md2"
insmod md2.ko
echo ">>>>> Modules md1 and md2 was loaded:"
lsmod | grep md
echo ">>>>> System log after modules insert:"
dmesg | tail -15
read -n 1 -s -r -p "Press any key to continue..."
echo
echo ">>>>> Removing md2"
rmmod md2
echo ">>>>> Loading md3"
insmod md3.ko
echo ">>>>> System log after modules insert and removal:"
dmesg | tail -15
read -n 1 -s -r -p "Press any key to continue..."
echo
echo ">>>>> Removing md3"
rmmod md3
echo ">>>>> Removing md1"
rmmod md1
echo ">>>>> System log after modules removal:"
dmesg | tail -5
