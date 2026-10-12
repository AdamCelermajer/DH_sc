#!/usr/bin/env python3
"""Export one sound UID's music from the original DH2 cache as packaged WAV assets.

Generalizes export_swamp_music_ima_wav.py. The sound is resolved from
sounds/sounds.xml by UID:

- format="vxn" (VoxN bank with states ["ambient", "combat"]): each state is cut
  from the bank's Segm table exactly as the SWAMP exporter does. The unchanged
  IMA-ADPCM blocks are wrapped in WAVE_FORMAT_IMA_ADPCM headers. Output names
  are <level>_<state>_music.wav, where <level> is taken from the bank filename
  (m_level_<level>_sfx_*.vxn) unless --name is given.
- format="adpcm" with a .wav filename (safe-zone track): the source WAV is
  validated and copied byte-for-byte to <label in snake_case>.wav.

Any other layout (different state names or count, non-contiguous segments,
unexpected chunk order or header) is reported and refused rather than guessed.
Existing output files are never overwritten.
"""
from __future__ import annotations

import argparse
import hashlib
import json
import re
import struct
import sys
import zipfile
import xml.etree.ElementTree as ET
from pathlib import Path

ROOT = "com.gameloft.android.GAND.GloftD2SS/files/"
SOUNDS = ROOT + "data/sounds/sounds.xml"
SOUND_DIR = ROOT + "data/sounds/"
SCHEMA = "dh2-level-music-source-export-v1"
RATE = 32000
CHANNELS = 2
BLOCK_BYTES = 1024
BLOCK_FRAMES = 1017
VXN_AFMT = (17, CHANNELS, RATE, BLOCK_BYTES, 4)
VXN_CHUNK_ORDER = [b"Afmt", b"Segm", b"Rule", b"Plst", b"Stat", b"Trsn", b"Grps", b"Grpe"]
VXN_STATES = ["ambient", "combat"]
SAFEZONE_FMT = (17, CHANNELS, RATE, RATE, BLOCK_BYTES, 4, 2, BLOCK_FRAMES)


def sha256(data: bytes) -> str:
    return hashlib.sha256(data).hexdigest()


def read_chunks(metadata: bytes) -> list[tuple[bytes, bytes]]:
    chunks = []
    offset = 0
    while offset < len(metadata):
        if len(metadata) - offset < 8:
            raise ValueError("truncated VXN chunk header")
        tag = metadata[offset:offset + 4]
        size = struct.unpack_from("<I", metadata, offset + 4)[0]
        offset += 8
        if size > len(metadata) - offset:
            raise ValueError(f"VXN {tag!r} chunk exceeds metadata window")
        chunks.append((tag, metadata[offset:offset + size]))
        offset += size
    return chunks


def find_sound_row(sounds_bytes: bytes, uid: int) -> dict[str, str]:
    sounds = ET.fromstring(sounds_bytes)
    rows = [dict(x.attrib) for x in sounds.iter("sound") if x.get("uid") == str(uid)]
    if len(rows) != 1:
        raise ValueError(f"expected exactly one sounds.xml row with uid={uid}, found {len(rows)}")
    return rows[0]


def inspect_vxn(data: bytes) -> tuple[dict, list[tuple[int, int, int]]]:
    if data[:4] != b"VoxN":
        raise ValueError("bank is not a VoxN container")
    declared_size, audio_base = struct.unpack_from("<II", data, 16)
    if declared_size != len(data) or audio_base != 512:
        raise ValueError("unsupported VXN header (size or audio base)")
    chunks = read_chunks(data[24:audio_base - 8])
    if [tag for tag, _ in chunks] != VXN_CHUNK_ORDER:
        raise ValueError("VXN metadata chunk order differs from the verified VoxN layout")
    if data[audio_base - 8:audio_base - 4] != b"Data" or \
            struct.unpack_from("<I", data, audio_base - 4)[0] != len(data) - audio_base:
        raise ValueError("VXN data chunk bounds are not as expected")
    if struct.unpack_from("<HHIHH", chunks[0][1]) != VXN_AFMT:
        raise ValueError("unsupported VXN IMA format (expected 2ch 32 kHz IMA, 1024-byte blocks)")
    segm = chunks[1][1]
    count = struct.unpack_from("<I", segm)[0]
    segments = [struct.unpack_from("<6I", segm, 4 + 24 * i)[:3] for i in range(count)]
    stat = chunks[4][1]
    states = [stat[8 + 32 * i:40 + 32 * i].split(b"\0", 1)[0].decode("ascii")
              for i in range(struct.unpack_from("<I", stat)[0])]
    if states != VXN_STATES:
        raise ValueError(f"VXN state layout {states} is not {VXN_STATES}; not guessing")
    if len(segments) != len(states):
        raise ValueError("VXN segment count does not match state count")
    cursor = 0
    for start, encoded, frames in segments:
        if start != cursor:
            raise ValueError("VXN state segments are not contiguous from offset 0")
        if encoded == 0 or encoded % BLOCK_BYTES or \
                frames == 0 or frames > encoded // BLOCK_BYTES * BLOCK_FRAMES:
            raise ValueError("VXN state segment is not block-aligned or has invalid frame count")
        cursor += encoded
    if audio_base + cursor != len(data):
        raise ValueError("VXN segments do not cover its payload")
    return ({"file_bytes": len(data), "audio_base": audio_base,
             "codec": "VoxNativeSubDecoderIMAADPCM", "channels": CHANNELS,
             "sample_rate_hz": RATE, "block_bytes": BLOCK_BYTES,
             "frames_per_block": BLOCK_FRAMES, "states": states,
             "segments": [{"state": s, "source_offset": a, "source_bytes": b,
                           "sample_frames": f}
                          for s, (a, b, f) in zip(states, segments)]},
            segments)


def wrap_vxn_segment(data: bytes, audio_base: int, state: str,
                     segment: tuple[int, int, int]) -> tuple[str, bytes, dict]:
    start, encoded_bytes, frames = segment
    avg_bytes_per_second = RATE * BLOCK_BYTES // BLOCK_FRAMES
    fmt = struct.pack("<HHIIHHHH", 17, CHANNELS, RATE, avg_bytes_per_second,
                      BLOCK_BYTES, 4, 2, BLOCK_FRAMES)
    fact = struct.pack("<I", frames)
    riff_bytes = 4 + 8 + len(fmt) + 8 + len(fact) + 8 + encoded_bytes
    payload = data[audio_base + start:audio_base + start + encoded_bytes]
    if len(payload) != encoded_bytes:
        raise ValueError("short VXN payload")
    out = (b"RIFF" + struct.pack("<I", riff_bytes) + b"WAVE"
           + b"fmt " + struct.pack("<I", len(fmt)) + fmt
           + b"fact" + struct.pack("<I", len(fact)) + fact
           + b"data" + struct.pack("<I", encoded_bytes) + payload)
    meta = {"source_segment_offset": start, "source_segment_bytes": encoded_bytes,
            "sample_frames": frames, "state": state}
    return f"{state}", out, meta


def inspect_safezone_wav(data: bytes) -> dict:
    if data[:4] != b"RIFF" or data[8:12] != b"WAVE":
        raise ValueError("safe-zone asset is not a RIFF/WAVE file")
    offset = 12
    fmt = fact = payload = None
    while offset + 8 <= len(data):
        tag = data[offset:offset + 4]
        size = struct.unpack_from("<I", data, offset + 4)[0]
        body = data[offset + 8:offset + 8 + size]
        if len(body) != size:
            raise ValueError("truncated safe-zone WAV chunk")
        if tag == b"fmt ":
            fmt = struct.unpack_from("<HHIIHHHH", body)
        elif tag == b"fact":
            fact = struct.unpack_from("<I", body)[0]
        elif tag == b"data":
            payload = size
            break
        offset += 8 + size + (size & 1)
    if fmt != SAFEZONE_FMT:
        raise ValueError("unsupported safe-zone ADPCM WAV format")
    if fact is None or payload is None or payload % BLOCK_BYTES or \
            fact > payload // BLOCK_BYTES * BLOCK_FRAMES or len(data) != 60 + payload:
        raise ValueError("safe-zone WAV frame/payload bounds are not as expected")
    return {"codec": "WAVE_FORMAT_IMA_ADPCM", "channels": CHANNELS,
            "sample_rate_hz": RATE, "block_bytes": BLOCK_BYTES,
            "frames_per_block": BLOCK_FRAMES, "sample_frames": fact,
            "payload_bytes": payload}


def snake(label: str) -> str:
    return re.sub(r"(?<!^)(?=[A-Z])", "_", label).lower()


def main(argv: list[str] | None = None) -> int:
    parser = argparse.ArgumentParser(description=__doc__.split("\n")[0])
    parser.add_argument("--cache-zip", type=Path, required=True,
                        help="original DH2 cache ZIP; read-only")
    parser.add_argument("--uid", type=int, required=True,
                        help="sounds.xml uid (e.g. 467 SWAMP, 471 DARKWOOD)")
    parser.add_argument("--out-dir", type=Path, required=True,
                        help="output directory (created if missing; existing outputs are never overwritten)")
    parser.add_argument("--name", help="override the <level> prefix for VXN state files")
    args = parser.parse_args(argv)

    with zipfile.ZipFile(args.cache_zip) as z:
        sounds_bytes = z.read(SOUNDS)
        sounds_info = z.getinfo(SOUNDS)
        row = find_sound_row(sounds_bytes, args.uid)
        member = SOUND_DIR + row.get("filename", "")
        if member not in z.namelist():
            raise SystemExit(f"sound file not in cache: {member}")
        info = z.getinfo(member)
        data = z.read(member)
        fmt = row.get("format")

        outputs: list[tuple[str, bytes, dict]] = []
        if fmt == "vxn":
            bank, segments = inspect_vxn(data)
            level = args.name
            if level is None:
                m = re.match(r"^m_level_(.+?)_sfx_", row["filename"])
                if not m:
                    raise SystemExit("cannot derive level prefix from bank filename; pass --name")
                level = m.group(1)
            for state, segment in zip(bank["states"], segments):
                outputs.append((f"{level}_{state}_music.wav",
                                *wrap_vxn_segment(data, bank["audio_base"], state, segment)[1:]))
            prefix = level
            source_kind = "vxn"
        elif fmt == "adpcm" and row["filename"].lower().endswith(".wav"):
            wav = inspect_safezone_wav(data)
            name = snake(row["label"])
            outputs.append((f"{name}.wav", data, {"state": None, **wav}))
            prefix = name[:-len("_music")] if name.endswith("_music") else name
            bank = None
            source_kind = "adpcm_wav"
        else:
            raise SystemExit(f"unsupported sounds.xml format {fmt!r} for uid {args.uid}; not exported")

        targets = [args.out_dir / name for name, _, _ in outputs]
        existing = [str(t) for t in targets if t.exists()]
        if existing:
            raise SystemExit(f"refusing to overwrite existing outputs: {existing}")
        args.out_dir.mkdir(parents=True, exist_ok=True)

        assets = []
        for (name, out, meta), target in zip(outputs, targets):
            target.write_bytes(out)
            raw = target.read_bytes()
            assets.append({"asset": name, "bytes": len(raw), "sha256": sha256(raw), **meta})

        manifest = {
            "schema": SCHEMA,
            "uid": args.uid,
            "sounds_member": SOUNDS,
            "sounds_sha256": sha256(sounds_bytes),
            "sounds_row": row,
            "source_member": member,
            "source_kind": source_kind,
            "source_bytes": info.file_size,
            "source_crc32": f"{info.CRC:08x}",
            "source_sha256": sha256(data),
            "vxn": bank,
            "outputs": assets,
        }
        provenance = args.out_dir / f"{prefix}-music-provenance.json"
        if provenance.exists():
            raise SystemExit(f"refusing to overwrite existing provenance: {provenance}")
        provenance.write_text(json.dumps(manifest, indent=2) + "\n", encoding="utf-8")

    for a in assets:
        print(f"{a['asset']}\t{a['bytes']}\t{a['sha256']}")
    print(f"level music export PASS: uid {args.uid} ({row['label']}), {len(assets)} asset(s)")
    return 0


if __name__ == "__main__":
    sys.exit(main())
