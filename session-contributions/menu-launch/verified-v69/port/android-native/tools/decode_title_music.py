"""Unwrap original MS-IMA VXN segments and decode each with installed FFmpeg.

Container reference: https://github.com/vgmstream/vgmstream/blob/master/src/meta/vxn.c
The intro and loop stay separate; default VXN subsong decoding loses the loop.
"""
import argparse
import hashlib
import json
import struct
import subprocess
import wave
from pathlib import Path


def decode(source, output, ffmpeg):
    data = source.read_bytes()
    if data[:4] != b"VoxN" or struct.unpack_from("<I", data, 16)[0] != len(data):
        raise ValueError("Invalid original VXN header")
    chunks = {}
    offset = 0
    while offset + 8 <= len(data):
        name, size = data[offset:offset + 4], struct.unpack_from("<I", data, offset + 4)[0]
        end = offset + 8 + size
        if end > len(data) or name in chunks:
            raise ValueError("Invalid VXN chunk")
        chunks[name] = data[offset + 8:end]
        offset = end
    if offset != len(data):
        raise ValueError("VXN trailing bytes")
    codec, channels, rate, block, bits = struct.unpack("<HHIHH", chunks[b"Afmt"])
    if (codec, channels, bits) != (0x11, 2, 4) or block != 1024:
        raise ValueError("Unsupported title encoding")
    seg = chunks[b"Segm"]
    count = struct.unpack_from("<I", seg)[0]
    if count != 2 or len(seg) != 4 + count * 24:
        raise ValueError("Expected original title intro and loop")
    payload = chunks[b"Data"]
    output.mkdir(parents=True, exist_ok=True)
    result = []
    for i, name in enumerate(("title_intro", "title_loop")):
        start, size, samples = struct.unpack_from("<III", seg, 4 + i * 24)
        if size % block or start + size > len(payload):
            raise ValueError("Invalid original audio segment")
        per_block = ((block - channels * 4) * 2 // channels) + 1
        fmt = struct.pack("<HHIIHHHH", codec, channels, rate, rate * block // per_block,
                          block, bits, 2, per_block)
        body = b"WAVEfmt " + struct.pack("<I", len(fmt)) + fmt
        body += b"fact" + struct.pack("<II", 4, samples)
        body += b"data" + struct.pack("<I", size) + payload[start:start + size]
        wrapped = output / (name + "_adpcm.wav")
        wrapped.write_bytes(b"RIFF" + struct.pack("<I", len(body)) + body)
        target = output / (name + ".wav")
        subprocess.run([ffmpeg, "-nostdin", "-v", "error", "-y", "-i", str(wrapped),
                        "-af", "atrim=end_sample=" + str(samples), "-c:a", "pcm_s16le",
                        str(target)], check=True)
        with wave.open(str(target)) as w:
            if (w.getnchannels(), w.getframerate(), w.getnframes(), w.getsampwidth()) != (channels, rate, samples, 2):
                raise ValueError("Decoded music differs from source metadata")
        result.append(dict(file=target.name, original_segment_offset=start,
                           original_segment_bytes=size, samples=samples, channels=channels,
                           rate=rate, duration=samples / rate,
                           sha256=hashlib.sha256(target.read_bytes()).hexdigest()))
    return dict(source_sha256=hashlib.sha256(data).hexdigest(), segments=result,
                container_reference="https://github.com/vgmstream/vgmstream/blob/master/src/meta/vxn.c")


if __name__ == "__main__":
    p = argparse.ArgumentParser()
    p.add_argument("source", type=Path)
    p.add_argument("output", type=Path)
    p.add_argument("--ffmpeg", default="ffmpeg")
    p.add_argument("--receipt", type=Path, required=True)
    a = p.parse_args()
    result = decode(a.source, a.output, a.ffmpeg)
    a.receipt.write_text(json.dumps(result, indent=2) + "\n")
    print(json.dumps(result))
