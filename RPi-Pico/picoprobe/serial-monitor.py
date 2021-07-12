#!/usr/bin/env python3
#
# Quick and dirty serial monitor for Picoprobe
# 
# Requires pySerial from either PyPI or OS Package
# 
# Assumes a baud-rate of 115200 and port /dev/ttyPico (see udev.rules)
#
import argparse
import sys

import serial


def get_args():
    parser = argparse.ArgumentParser()
    parser.add_argument("-p", "--port", default="/dev/ttyPico")
    parser.add_argument("-b", "--baud", type=int, default=115200)
    return parser.parse_args()


def main():
    args = get_args()

    port = serial.Serial(port=args.port, baudrate=args.baud)
    try:
        while True:
            data = port.read()
            sys.stdout.write(data.decode("ascii"))
    except KeyboardInterrupt:
        print("Exiting...", file=sys.stderr)
    finally:
        port.close()


if __name__ == '__main__':
    main()
