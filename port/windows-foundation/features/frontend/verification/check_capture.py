"""Check native frontend P6 captures and emit lossless PNGs plus evidence JSON.

Usage: python check_capture.py capture.ppm [capture2.ppm ...] --output results.json
This checks capture integrity and nonblank rendering; it does not assert visual parity.
"""
import argparse
import hashlib
import json
from pathlib import Path
import struct
import zlib


def read_ppm(path):
    raw = path.read_bytes()
    index = 0
    tokens = []
    while len(tokens) < 4:
        while index < len(raw) and raw[index] in b" \t\r\n":
            index += 1
        if index < len(raw) and raw[index] == 35:
            index = raw.index(b"\n", index) + 1
            continue
        start = index
        while index < len(raw) and raw[index] not in b" \t\r\n":
            index += 1
        if start == index:
            raise ValueError("Incomplete PPM header")
        tokens.append(raw[start:index])
    if tokens[0] != b"P6" or tokens[3] != b"255":
        raise ValueError("Expected binary P6 with max value 255")
    if index >= len(raw) or raw[index] not in b" \t\r\n":
        raise ValueError("Missing raster delimiter")
    index += 2 if raw[index:index + 2] == b"\r\n" else 1
    width, height = map(int, tokens[1:3])
    if width <= 0 or height <= 0:
        raise ValueError("Invalid dimensions")
    pixels = raw[index:]
    if len(pixels) != width * height * 3:
        raise ValueError(f"Raster size {len(pixels)} differs from {width * height * 3}")
    return width, height, pixels


def png_bytes(width, height, pixels):
    def chunk(kind, payload):
        return (struct.pack(">I", len(payload)) + kind + payload
                + struct.pack(">I", zlib.crc32(kind + payload) & 0xffffffff))
    rows = b"".join(b"\0" + pixels[y * width * 3:(y + 1) * width * 3]
                    for y in range(height))
    return (b"\x89PNG\r\n\x1a\n"
            + chunk(b"IHDR", struct.pack(">IIBBBBB", width, height, 8, 2, 0, 0, 0))
            + chunk(b"IDAT", zlib.compress(rows)) + chunk(b"IEND", b""))


def check(path):
    width, height, pixels = read_ppm(path)
    colors = set(zip(pixels[0::3], pixels[1::3], pixels[2::3]))
    nonblack = sum(1 for r, g, b in zip(pixels[0::3], pixels[1::3], pixels[2::3])
                   if r or g or b)
    png = path.with_suffix(".png")
    png.write_bytes(png_bytes(width, height, pixels))
    return {"capture": str(path.resolve()), "png": str(png.resolve()),
            "width": width, "height": height,
            "sha256": hashlib.sha256(path.read_bytes()).hexdigest(),
            "unique_rgb_colors": len(colors),
            "nonblack_fraction": nonblack / (width * height),
            "nonblank": len(colors) > 1,
            "result": "pass" if len(colors) > 1 else "fail"}


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("captures", nargs="+", type=Path)
    parser.add_argument("--output", required=True, type=Path)
    args = parser.parse_args()
    results = []
    for path in args.captures:
        try:
            results.append(check(path))
        except (OSError, ValueError) as error:
            results.append({"capture": str(path), "result": "fail", "error": str(error)})
    report = {"check_scope": "P6 integrity and nonblank rendering; visual review required",
              "captures": results,
              "result": "pass" if all(r["result"] == "pass" for r in results) else "fail"}
    args.output.parent.mkdir(parents=True, exist_ok=True)
    args.output.write_text(json.dumps(report, indent=2) + "\n", encoding="utf-8")
    print(json.dumps(report, indent=2))
    return 0 if report["result"] == "pass" else 1


if __name__ == "__main__":
    raise SystemExit(main())
