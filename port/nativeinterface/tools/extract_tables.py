#!/usr/bin/env python3
"""Reproduce the table header from the exact supplied original ELF.

This intentionally validates the input hash: offsets are specific to this build.
It neither obtains nor generates game licenses.
"""
import argparse
import hashlib
import json
import struct
from pathlib import Path

def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("elf", type=Path)
    parser.add_argument("output", type=Path)
    args = parser.parse_args()
    data = args.elf.read_bytes()
    expected = "180b582cbb7e7004c94fe771f8c8013dad4c74a9317ee4baff08c542da4ff0da"
    if hashlib.sha256(data).hexdigest() != expected:
        parser.error("input differs from the analyzed original libnativeinterface.so")
    addresses = struct.unpack_from("<6I", data, 0x2760)[1:]
    lines = ["/* Recovered byte-exact constant strings from the user-supplied libnativeinterface.so.",
             " * Third-party text is not relicensed by this source reconstruction. */",
             "#ifndef DH2_PASSPHRASE_TABLES_H", "#define DH2_PASSPHRASE_TABLES_H",
             "static const char dh2_passphrase_tables[5][1025] = {"]
    for address in addresses:
        value = data[address:address+1024]
        assert data[address+1024] == 0 and b"\0" not in value
        lines.append("    /* Original ELF virtual address 0x%04x. */" % address)
        for offset in range(0, len(value), 80):
            lines.append("    " + json.dumps(value[offset:offset+80].decode("ascii")) +
                         ("," if offset+80 >= len(value) else ""))
    lines += ["};", "#endif", ""]
    args.output.write_text("\n".join(lines))

if __name__ == "__main__":
    main()
