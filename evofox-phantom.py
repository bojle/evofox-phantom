#!/usr/bin/python

##
## Copyright (c) 2020 https://github.com/bojle
##
## Permission is hereby granted, free of charge, to any person obtaining a copy
## of this software and associated documentation files (the "Software"), to
## deal in the Software without restriction, including without limitation the
## rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
## sell copies of the Software, and to permit persons to whom the Software is
## furnished to do so, subject to the following conditions:
##
## The above copyright notice and this permission notice shall be included in
## all copies or substantial portions of the Software.
##
## THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
## IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
## FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
## AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
## LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
## FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
## IN THE SOFTWARE.
##

import hid
import argparse

program_name = "evofox-phantom"
program_desc = "Toggle LED modes in Amkette Evofox Gaming Mouse"
phantom_vendor_id = '0x18f8'
phantom_product_id = '0x1286'

# Argparse

parser = argparse.ArgumentParser(prog=program_name, description=program_desc)
parser.add_argument("--vid", default=phantom_vendor_id, help="Vendor ID; lsusb(8)")
parser.add_argument("--pid", default=phantom_product_id, help="Product ID; lsusb(8)")
parser.add_argument("-l", "--led", nargs="?", \
        choices=['off', 'static', 'slow', 'fast'], default="off")
args = parser.parse_args()

# Main 
    
choices = {"vid": int(args.vid, base=16),\
        "pid": int(args.pid, base=16),\
        "led": args.led\
        }

mode_bits_slow =    [0x07, 0x13, 0x7f, 0x10, 0x0f, 0x00, 0x00, 0x00]
mode_bits_fast =    [0x07, 0x13, 0x7f, 0x13, 0x0f, 0x00, 0x00, 0x00]
mode_bits_static =  [0x07, 0x13, 0x7f, 0x16, 0x0f, 0x00, 0x00, 0x00]
mode_bits_off =     [0x07, 0x13, 0x7f, 0x17, 0x0f, 0x00, 0x00, 0x00]

def main():
    try:
        h = hid.device()
        h.open(choices["vid"], choices["pid"])
        h.set_nonblocking(True)
        write_buf = []
        c = choices["led"]
        if c == "off":
            write_buf = mode_bits_off
        elif c == "static":
            write_buf = mode_bits_static
        elif c == "slow":
            write_buf = mode_bits_slow
        elif c == "fast":
            write_buf = mode_bits_fast
        h.write(write_buf)
        h.close()
    except OSError as oserr:
        print("OSError: Permission Denied...Probably")

main()
