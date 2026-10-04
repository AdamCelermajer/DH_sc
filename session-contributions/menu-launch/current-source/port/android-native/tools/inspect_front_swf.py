"""Read original front-end SWF placement names and movie bounds without executing it."""
import argparse
import hashlib
import json
import struct
import zlib
from pathlib import Path


class Bits:
    def __init__(self, data, offset):
        self.data, self.bit = data, offset * 8

    def read(self, count):
        value = 0
        for _ in range(count):
            value = value * 2 + ((self.data[self.bit // 8] >> (7 - self.bit % 8)) & 1)
            self.bit += 1
        return value

    def signed(self, count):
        value = self.read(count)
        return value - (1 << count) if count and value & (1 << (count - 1)) else value

    def offset(self):
        return (self.bit + 7) // 8


def matrix_end(data, offset):
    b = Bits(data, offset)
    for _ in range(2):
        if b.read(1):
            count = b.read(5)
            b.read(count * 2)
    count = b.read(5)
    b.read(count * 2)
    return b.offset()


def color_end(data, offset):
    b = Bits(data, offset)
    add, multiply, count = b.read(1), b.read(1), b.read(4)
    b.read(count * 4 * (add + multiply))
    return b.offset()


def inspect(path):
    original = path.read_bytes()
    data = original[:8] + zlib.decompress(original[8:]) if original[:3] == b"CWS" else original
    if data[:3] not in (b"CWS", b"FWS"):
        raise ValueError("Unsupported SWF signature")
    if len(data) != struct.unpack_from("<I", data, 4)[0]:
        raise ValueError("Uncompressed SWF length mismatch")
    b = Bits(data, 8)
    count = b.read(5)
    bounds = [b.signed(count) for _ in range(4)]
    offset = b.offset()
    frame_rate, frames = struct.unpack_from("<HH", data, offset)
    placements, exports = [], []

    def tags(start, end, parent):
        cursor, frame = start, 0
        while cursor + 2 <= end:
            header = struct.unpack_from("<H", data, cursor)[0]
            cursor += 2
            kind, size = header >> 6, header & 63
            if size == 63:
                size = struct.unpack_from("<I", data, cursor)[0]
                cursor += 4
            limit = cursor + size
            if limit > end:
                raise ValueError("SWF tag exceeds parent")
            if kind == 1:
                frame += 1
            elif kind == 39:
                identity, count = struct.unpack_from("<HH", data, cursor)
                tags(cursor + 4, limit, f"sprite:{identity}")
            elif kind == 26:
                flags, depth = struct.unpack_from("<BH", data, cursor)
                pos = cursor + 3
                identity = None
                if flags & 2:
                    identity = struct.unpack_from("<H", data, pos)[0]
                    pos += 2
                if flags & 4:
                    pos = matrix_end(data, pos)
                if flags & 8:
                    pos = color_end(data, pos)
                if flags & 16:
                    pos += 2
                if flags & 32:
                    stop = data.index(0, pos, limit)
                    placements.append(dict(parent=parent, frame=frame, depth=depth,
                                           character=identity, name=data[pos:stop].decode("utf-8")))
            elif kind == 56:
                count = struct.unpack_from("<H", data, cursor)[0]
                pos = cursor + 2
                for _ in range(count):
                    identity = struct.unpack_from("<H", data, pos)[0]
                    pos += 2
                    stop = data.index(0, pos, limit)
                    exports.append(dict(character=identity, name=data[pos:stop].decode("utf-8")))
                    pos = stop + 1
            cursor = limit
            if kind == 0:
                break

    tags(offset + 4, len(data), "root")
    return dict(file=path.name, sha256=hashlib.sha256(original).hexdigest(),
                bounds_twips=bounds, frame_rate=frame_rate / 256, frames=frames,
                root_placements=[x for x in placements if x["parent"] == "root"],
                placements=placements, exports=exports)


if __name__ == "__main__":
    p = argparse.ArgumentParser()
    p.add_argument("directory", type=Path)
    p.add_argument("--output", type=Path, required=True)
    args = p.parse_args()
    results = [inspect(args.directory / n) for n in ("dqmenus_droid.swf", "loadanims_droid.swf")]
    args.output.write_text(json.dumps(results, indent=2) + "\n")
    for result in results:
        print(result["file"], result["bounds_twips"], result["root_placements"])
