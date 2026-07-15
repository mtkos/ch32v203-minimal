#!/usr/bin/python3

from serial import Serial

with Serial('/dev/ttyACM0', 921600) as s:
    l = s.read(48)
    for i in range(0, 48, 8):
        print(l[i:i + 8].hex())
