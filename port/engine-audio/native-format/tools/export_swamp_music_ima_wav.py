#!/usr/bin/env python3
"""Export only the source-mapped SWAMP music bank and safe-zone track."""
from __future__ import annotations

import argparse
import hashlib
import json
from pathlib import Path
import struct
import zipfile
import xml.etree.ElementTree as ET

ROOT = "com.gameloft.android.GAND.GloftD2SS/files/"
SOUNDS = ROOT + "data/sounds/sounds.xml"
LEVEL = ROOT + "data/scene/001_swamp.mlx"
VXN = ROOT + "data/sounds/m_level_swamp_sfx_swamp.vxn"
SAFEZONE = ROOT + "data/sounds/m_safe_zone_01_sfx_swamp.wav"
EXPECTED = {
    SOUNDS: (102135, "f50ddf98", "4bc1de654d65470d49e48cee354d5d5cdd45006c18d65420ddc07acf5b400a6e"),
    VXN: (12278272, "52dba0e4", "980137ac5b032e5cee50e3f8e6940d31dbd5e0fd0d70346a5167d3c00567e0a8"),
    SAFEZONE: (3591228, "0189ca63", "9f2dc2ae2df224b4f40434a468f189b12e4cc22da146912f332a5483afdc75fb"),
}
RATE = 32000
CHANNELS = 2
BLOCK_BYTES = 1024
BLOCK_FRAMES = 1017


def sha256(data: bytes) -> str:
    return hashlib.sha256(data).hexdigest()


def source_rows(z: zipfile.ZipFile) -> dict[str, dict[str, str]]:
    sounds = ET.fromstring(z.read(SOUNDS))
    levels = ET.fromstring(z.read(LEVEL))
    level = next(x for x in levels.iter("GameObject") if x.get("type") == "level")
    expected_level = {"music": "SwampHubAmbientMusic",
                      "safezone_music": "SwampMerchantCampMusic"}
    if any(level.get(k) != v for k, v in expected_level.items()):
        raise ValueError("SWAMP LevelConfig music labels changed")
    wanted = set(expected_level.values())
    rows = {x.get("label"): x.attrib for x in sounds.iter("sound")
            if x.get("label") in wanted}
    if set(rows) != wanted or len(rows) != 2:
        raise ValueError("expected both SWAMP music rows in sounds.xml")
    if (rows["SwampHubAmbientMusic"].get("uid"),
            rows["SwampHubAmbientMusic"].get("filename"),
            rows["SwampHubAmbientMusic"].get("format"),
            rows["SwampHubAmbientMusic"].get("loop")) != (
            "467", "m_level_swamp_sfx_swamp.vxn", "vxn", "yes"):
        raise ValueError("SWAMP hub music Sounds row changed")
    if (rows["SwampMerchantCampMusic"].get("uid"),
            rows["SwampMerchantCampMusic"].get("filename"),
            rows["SwampMerchantCampMusic"].get("format"),
            rows["SwampMerchantCampMusic"].get("loop")) != (
            "505", "m_safe_zone_01_sfx_swamp.wav", "adpcm", "yes"):
        raise ValueError("SWAMP safe-zone music Sounds row changed")
    return rows


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


def inspect_vxn(z: zipfile.ZipFile) -> tuple[dict, list[tuple[int, int, int]]]:
    info = z.getinfo(VXN)
    size, crc, digest = EXPECTED[VXN]
    data = z.read(VXN)
    if (info.file_size, f"{info.CRC:08x}", sha256(data)) != (size, crc, digest):
        raise ValueError("SWAMP VXN source identity changed")
    if data[:4] != b"VoxN":
        raise ValueError("SWAMP bank is not a VoxN container")
    declared_size, audio_base = struct.unpack_from("<II", data, 16)
    if declared_size != size or audio_base != 512:
        raise ValueError("unsupported SWAMP VXN header")
    chunks = read_chunks(data[24:audio_base - 8])
    tags = [tag for tag, _ in chunks]
    if tags != [b"Afmt", b"Segm", b"Rule", b"Plst", b"Stat", b"Trsn", b"Grps", b"Grpe"]:
        raise ValueError("SWAMP VXN metadata chunk order changed")
    if data[audio_base - 8:audio_base - 4] != b"Data" or \
            struct.unpack_from("<I", data, audio_base - 4)[0] != len(data) - audio_base:
        raise ValueError("SWAMP VXN data chunk bounds changed")
    afmt = chunks[0][1]
    if struct.unpack_from("<HHIHH", afmt) != (17, CHANNELS, RATE, BLOCK_BYTES, 4):
        raise ValueError("unsupported SWAMP VXN IMA format")
    segm = chunks[1][1]
    count = struct.unpack_from("<I", segm)[0]
    segments = [struct.unpack_from("<6I", segm, 4 + 24 * i)[:3]
                for i in range(count)]
    if segments != [(0, 6138880, 6096650), (6138880, 6138880, 6096650)]:
        raise ValueError("SWAMP VXN state segment bounds changed")
    stat = chunks[4][1]
    states = [stat[8 + 32 * i:40 + 32 * i].split(b"\0", 1)[0].decode("ascii")
              for i in range(struct.unpack_from("<I", stat)[0])]
    if states != ["ambient", "combat"]:
        raise ValueError("SWAMP VXN state order changed")
    if audio_base + segments[-1][0] + segments[-1][1] != size:
        raise ValueError("SWAMP VXN segments do not cover its payload")
    return ({"file_bytes": size, "sha256": digest, "crc32": crc,
             "audio_base": audio_base, "codec": "VoxNativeSubDecoderIMAADPCM",
             "channels": CHANNELS, "sample_rate_hz": RATE,
             "block_bytes": BLOCK_BYTES, "frames_per_block": BLOCK_FRAMES,
             "states": states}, segments)


def verify_safezone(z: zipfile.ZipFile) -> tuple[bytes, dict]:
    info = z.getinfo(SAFEZONE)
    expected = EXPECTED[SAFEZONE]
    data = z.read(SAFEZONE)
    if (info.file_size, f"{info.CRC:08x}", sha256(data)) != expected:
        raise ValueError("SWAMP safe-zone WAV source identity changed")
    if data[:4] != b"RIFF" or data[8:12] != b"WAVE":
        raise ValueError("SWAMP safe-zone asset is not WAV")
    offset = 12
    fmt = fact = payload = None
    while offset + 8 <= len(data):
        tag = data[offset:offset + 4]
        size = struct.unpack_from("<I", data, offset + 4)[0]
        body = data[offset + 8:offset + 8 + size]
        if len(body) != size:
            raise ValueError("truncated SWAMP safe-zone WAV chunk")
        if tag == b"fmt ": fmt = struct.unpack_from("<HHIIHHHH", body)
        elif tag == b"fact": fact = struct.unpack_from("<I", body)[0]
        elif tag == b"data": payload = size; break
        offset += 8 + size + (size & 1)
    if fmt != (17, 2, RATE, RATE, BLOCK_BYTES, 4, 2, BLOCK_FRAMES):
        raise ValueError("unsupported SWAMP safe-zone ADPCM WAV format")
    if fact != 3565714 or payload != 3591168 or len(data) != 60 + payload:
        raise ValueError("SWAMP safe-zone WAV frame/payload bounds changed")
    return data, {"file_bytes": len(data), "sha256": expected[2],
                  "crc32": expected[1], "codec": "WAVE_FORMAT_IMA_ADPCM",
                  "channels": CHANNELS, "sample_rate_hz": RATE,
                  "block_bytes": BLOCK_BYTES, "frames_per_block": BLOCK_FRAMES,
                  "sample_frames": fact, "payload_bytes": payload}


def write_segment(z: zipfile.ZipFile, output: Path,
                  segment: tuple[int, int, int], audio_base: int) -> dict:
    start, encoded_bytes, frames = segment
    if encoded_bytes % BLOCK_BYTES or frames > encoded_bytes // BLOCK_BYTES * BLOCK_FRAMES:
        raise ValueError("SWAMP state segment is not block-aligned")
    avg_bytes_per_second = RATE * BLOCK_BYTES // BLOCK_FRAMES
    fmt = struct.pack("<HHIIHHHH", 17, CHANNELS, RATE, avg_bytes_per_second,
                      BLOCK_BYTES, 4, 2, BLOCK_FRAMES)
    fact = struct.pack("<I", frames)
    riff_bytes = 4 + 8 + len(fmt) + 8 + len(fact) + 8 + encoded_bytes
    output.parent.mkdir(parents=True, exist_ok=True)
    with z.open(VXN) as src, output.open("wb") as dst:
        dst.write(b"RIFF" + struct.pack("<I", riff_bytes) + b"WAVE")
        dst.write(b"fmt " + struct.pack("<I", len(fmt)) + fmt)
        dst.write(b"fact" + struct.pack("<I", len(fact)) + fact)
        dst.write(b"data" + struct.pack("<I", encoded_bytes))
        src.seek(audio_base + start)
        remaining = encoded_bytes
        while remaining:
            block = src.read(min(1024 * 1024, remaining))
            if not block:
                raise ValueError("short SWAMP VXN payload")
            dst.write(block)
            remaining -= len(block)
    raw = output.read_bytes()
    return {"asset": output.name, "bytes": len(raw), "sha256": sha256(raw),
            "source_segment_offset": start, "source_segment_bytes": encoded_bytes,
            "sample_frames": frames}


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("--cache", type=Path, required=True,
                        help="original DH2 cache ZIP; read-only")
    parser.add_argument("--output-dir", type=Path, required=True,
                        help="new output directory for only the SWAMP music assets")
    args = parser.parse_args()
    if args.output_dir.exists() and any(args.output_dir.iterdir()):
        raise SystemExit("output directory must be new or empty")
    args.output_dir.mkdir(parents=True, exist_ok=True)
    with zipfile.ZipFile(args.cache) as z:
        for member, (size, crc, digest) in EXPECTED.items():
            info = z.getinfo(member)
            if (info.file_size, f"{info.CRC:08x}", sha256(z.read(member))) != (size, crc, digest):
                raise SystemExit(f"source member identity changed: {member}")
        rows = source_rows(z)
        bank, segments = inspect_vxn(z)
        safezone_data, safezone_info = verify_safezone(z)
        tracks = []
        for state, segment in zip(bank["states"], segments):
            tracks.append(write_segment(z,args.output_dir/f"swamp_{state}_music.wav",
                                        segment,bank["audio_base"]))
        safezone_path = args.output_dir / "swamp_merchant_camp_music.wav"
        safezone_path.write_bytes(safezone_data)
        safezone_info["asset"] = safezone_path.name
        manifest = {
            "schema": "dh2-swamp-music-source-export-v1",
            "level": {"member": LEVEL, "music": "SwampHubAmbientMusic",
                      "safezone_music": "SwampMerchantCampMusic"},
            "sounds_member": SOUNDS,
            "music_rows": rows,
            "hub_vxn": bank,
            "state_assets": tracks,
            "safezone_asset": safezone_info,
        }
        (args.output_dir / "swamp-music-provenance.json").write_text(
            json.dumps(manifest,indent=2)+"\n",encoding="utf-8")
    print(f"SWAMP music export PASS: {len(tracks)} VXN states plus safe-zone track")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
