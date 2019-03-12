#!/usr/bin/env python

from argparse import ArgumentParser

__version__ = "1.0"

def main():

    parser = ArgumentParser(description="Binary to C header converter")
    parser.add_argument("--version", action="version", version="%(prog)s "  + __version__)
    parser.add_argument("file", help="the file to be converted")
    parser.add_argument("name", help="variable name in header file")

    args = parser.parse_args()
    
    with open(args.file, "rb") as fd:
        data = bytearray(fd.read())

    print("/* %s, %d bytes */\n" % (args.file, len(data)))
    print("const uint8_t %s[] =\n{" % args.name)
    line = "" 

    for i in data:
        num = "%d" % i + ","
        if len(line) + len(num) < 100:
            line += num
        else:
            print("    " + line)
            line = num

    print("    " + line[:-1])
    print("};")

if __name__ == "__main__":
    main()
