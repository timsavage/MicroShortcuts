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
from typing import Generator

import serial


def get_args():
    parser = argparse.ArgumentParser()
    parser.add_argument("-p", "--port", default="/dev/ttyPico")
    parser.add_argument("-b", "--baud", type=int, default=115200)
    parser.add_argument("-o", "--output", default="ascii", choices=("hex", "ascii"))
    return parser.parse_args()


def format_ascii() -> Generator[str, bytes, None]:
    """
    Formatter that exports raw ascii
    """
    data = yield None
    while True:
        data = yield data.decode("ascii")


def format_hex() -> Generator[str, bytes, None]:
    """
    Formatter that exports hex
    """
    data = yield None
    line_count = 0
    while True:
        out = data.hex()

        line_count += 1
        if (line_count % 16) == 0:
            out += "\n"
        elif (line_count % 8) == 0:
            out += "  "
        else:
            out += " "

        data = yield out


def main():
    args = get_args()

    # Instantiate and initialise formatting generator
    formatter = {"hex": format_hex, "ascii": format_ascii}.get(args.output)()
    formatter.send(None)

    port = serial.Serial(port=args.port, baudrate=args.baud)
    try:
        while True:
            data = port.read()
            sys.stdout.write(formatter.send(data))
    except KeyboardInterrupt:
        print("Exiting...", file=sys.stderr)
    finally:
        port.close()


if __name__ == '__main__':
    main()
