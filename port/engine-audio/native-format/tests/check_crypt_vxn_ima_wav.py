#!/usr/bin/env python3
"""Check Crypt VXN IMA WAV remux headers, sample caps, and exact payloads."""
from __future__ import annotations

import argparse
import hashlib
from pathlib import Path
import struct

SOURCE_BYTES = 10_281_472
SOURCE_SHA256 = "6c0236d19b66011d986a7fd31a332257d8f689ceea69e049abf90376d33ed909"
AUDIO_BASE = 512
RATE = 32_000
CHANNELS = 2
BLOCK_BYTES = 1_024
FRAMES_PER_BLOCK = 1_017
TRACKS = (
    ("ambient", 0, 5_140_480, 5_105_117),
    ("combat", 5_140_480, 5_140_480, 5_105_117),
)


def digest(path: Path) -> str:
    h = hashlib.sha256()
    with path.open("rb") as f:
        for block in iter(lambda: f.read(1024 * 1024), b""):
            h.update(block)
    return h.hexdigest()


def read_exact(f, count: int) -> bytes:
    data = f.read(count)
    if len(data) != count:
        raise ValueError("truncated VXN or WAV")
    return data


def verify_track(vxn: Path, wav: Path, start: int, encoded: int,
                 frames: int) -> dict[str, int | str]:
    if encoded % BLOCK_BYTES or frames > encoded // BLOCK_BYTES * FRAMES_PER_BLOCK:
        raise ValueError("source segment is not valid block-aligned IMA data")
    with wav.open("rb") as out:
        if read_exact(out, 12) != b"RIFF" + struct.pack("<I", wav.stat().st_size - 8) + b"WAVE":
            raise ValueError(f"{wav.name}: invalid RIFF framing")
        if read_exact(out, 4) != b"fmt ":
            raise ValueError(f"{wav.name}: missing fmt chunk")
        fmt_len = struct.unpack("<I", read_exact(out, 4))[0]
        if fmt_len != 20:
            raise ValueError(f"{wav.name}: unexpected fmt size {fmt_len}")
        fmt = struct.unpack("<HHIIHHHH", read_exact(out, fmt_len))
        expected_avg = RATE * BLOCK_BYTES // FRAMES_PER_BLOCK
        if fmt != (0x11, CHANNELS, RATE, expected_avg, BLOCK_BYTES, 4, 2, FRAMES_PER_BLOCK):
            raise ValueError(f"{wav.name}: IMA WAV format fields mismatch: {fmt}")
        if read_exact(out, 4) != b"fact" or struct.unpack("<I", read_exact(out, 4))[0] != 4:
            raise ValueError(f"{wav.name}: missing fact sample count")
        if struct.unpack("<I", read_exact(out, 4))[0] != frames:
            raise ValueError(f"{wav.name}: fact sample limit mismatch")
        if read_exact(out, 4) != b"data" or struct.unpack("<I", read_exact(out, 4))[0] != encoded:
            raise ValueError(f"{wav.name}: data chunk size mismatch")
        with vxn.open("rb") as source:
            source.seek(AUDIO_BASE + start)
            remaining = encoded
            while remaining:
                count = min(1024 * 1024, remaining)
                if read_exact(out, count) != read_exact(source, count):
                    raise ValueError(f"{wav.name}: encoded payload differs from VXN segment")
                remaining -= count
        if out.read(1):
            raise ValueError(f"{wav.name}: trailing bytes after RIFF data")
    return {"file": wav.name, "frames": frames,
            "duration_ms": frames * 1000 // RATE,
            "encoded_bytes": encoded, "wav_sha256": digest(wav)}


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--vxn", required=True, type=Path)
    parser.add_argument("--wav-dir", required=True, type=Path)
    args = parser.parse_args()
    if args.vxn.stat().st_size != SOURCE_BYTES or digest(args.vxn) != SOURCE_SHA256:
        raise ValueError("VXN source identity mismatch")
    tracks = [verify_track(args.vxn, args.wav_dir / f"crypt_{state}_music.wav",
                           start, encoded, frames)
              for state, start, encoded, frames in TRACKS]
    print(f"Crypt IMA WAV remux: PASS ({len(tracks)} states; exact encoded payload and sample caps)")
    for track in tracks:
        print(f"  {track['file']}: {track['frames']} frames / {track['encoded_bytes']} encoded bytes / sha256 {track['wav_sha256']}")


if __name__ == "__main__":
    main()
