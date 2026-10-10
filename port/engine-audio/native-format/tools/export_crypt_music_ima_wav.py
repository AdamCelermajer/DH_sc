#!/usr/bin/env python3
"""Remux verified Crypt VoxN IMA segments for source-decoder verification.

The source codec is unchanged: each source segment is a sequence of 1,024-byte
IMA blocks. This exporter maps the VXN state order to its same-order segment
rows and writes corresponding WAVE_FORMAT_IMA_ADPCM headers. Android's
platform does not guarantee IMA-WAVE decoding; this is a source-integrity
fixture only. Package PCM exported by export_crypt_music_pcm_wav.py instead.
"""
from __future__ import annotations

import argparse
import hashlib
import json
from pathlib import Path
import struct
import zipfile

ROOT = "com.gameloft.android.GAND.GloftD2SS/files/"
SOURCE_MEMBER = ROOT + "data/sounds/m_level_cathedral_sfx_catacomb.vxn"
SOUNDS_MEMBER = ROOT + "data/sounds/sounds.xml"
RULE_MEMBER = ROOT + "data/scene/007_crypt_01.rule.xml"
SOURCE_BYTES = 10_281_472
SOURCE_SHA256 = "6c0236d19b66011d986a7fd31a332257d8f689ceea69e049abf90376d33ed909"
RATE = 32_000
CHANNELS = 2
BLOCK_BYTES = 1_024
BLOCK_FRAMES = 1_017
MAX_OUTPUT_BYTES = 24 * 1024 * 1024


def sha256_stream(source) -> str:
    digest = hashlib.sha256()
    for block in iter(lambda: source.read(1024 * 1024), b""):
        digest.update(block)
    return digest.hexdigest()


def source_mapping(z: zipfile.ZipFile) -> dict:
    import xml.etree.ElementTree as ET

    sounds = ET.fromstring(z.read(SOUNDS_MEMBER))
    rows = [x.attrib for x in sounds.iter("sound")
            if x.attrib.get("label") == "CryptOneAmbientMusic"]
    if len(rows) != 1:
        raise ValueError(f"expected one CryptOneAmbientMusic row, found {len(rows)}")
    row = rows[0]
    if (row.get("uid"), row.get("filename"), row.get("loop")) != (
            "486", "m_level_cathedral_sfx_catacomb.vxn", "yes"):
        raise ValueError("source Crypt music table row changed")
    rule = z.read(RULE_MEMBER)[:4096].decode("utf-8", "strict")
    if 'music="CryptOneAmbientMusic"' not in rule:
        raise ValueError("Crypt rule does not request the verified VXN cue")
    return {"uid": 486, "label": "CryptOneAmbientMusic",
            "filename": row["filename"], "loop": True,
            "rule_member": RULE_MEMBER, "music_member": SOUNDS_MEMBER}


def read_chunks(metadata: bytes) -> list[tuple[bytes, bytes]]:
    chunks: list[tuple[bytes, bytes]] = []
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


def parse_bank(z: zipfile.ZipFile) -> tuple[dict, list[tuple[int, int, int]]]:
    info = z.getinfo(SOURCE_MEMBER)
    if info.file_size != SOURCE_BYTES or info.CRC != 0xA8ABDB0C:
        raise ValueError("Crypt VXN member size/CRC differs from pinned source")
    with z.open(info) as src:
        prefix = src.read(24)
        if len(prefix) != 24 or prefix[:4] != b"VoxN":
            raise ValueError("Crypt asset is not a VoxN container")
        read_count = struct.unpack_from("<I", prefix, 4)[0]
        declared_bytes, audio_base = struct.unpack_from("<II", prefix, 16)
        if read_count != 16 or declared_bytes != SOURCE_BYTES or audio_base != 512:
            raise ValueError("unsupported Crypt VXN header layout")
        metadata = src.read(audio_base - 32)
        if len(metadata) != 480:
            raise ValueError("short VXN metadata window")
        chunks = read_chunks(metadata)
        tags = [tag for tag, _ in chunks]
        if tags != [b"Afmt", b"Segm", b"Rule", b"Plst", b"Stat", b"Trsn", b"Grps", b"Grpe"]:
            raise ValueError("VXN metadata chunk order differs from pinned Crypt bank")
        afmt = chunks[0][1]
        if len(afmt) != 12 or struct.unpack_from("<HHIHH", afmt) != (17, CHANNELS, RATE, BLOCK_BYTES, 4):
            raise ValueError("unsupported VXN IMA format fields")
        segm = chunks[1][1]
        if len(segm) != 52 or struct.unpack_from("<I", segm)[0] != 2:
            raise ValueError("unsupported Crypt segment table")
        segments = [struct.unpack_from("<6I", segm, 4 + 24 * i)[:3] for i in range(2)]
        if segments != [(0, 5_140_480, 5_105_117),
                        (5_140_480, 5_140_480, 5_105_117)]:
            raise ValueError("Crypt state segment bounds changed")
        stat = chunks[4][1]
        if len(stat) != 68 or struct.unpack_from("<I", stat)[0] != 2:
            raise ValueError("unsupported Crypt state table")
        states = []
        state_ids = []
        for i in range(2):
            row = stat[4 + i * 32:4 + (i + 1) * 32]
            state_ids.append(struct.unpack_from("<I", row)[0])
            name = row[4:].split(b"\0", 1)[0].decode("ascii", "strict")
            if name not in ("ambient", "combat"):
                raise ValueError(f"unexpected Crypt music state: {name!r}")
            states.append(name)
        if states != ["ambient", "combat"]:
            raise ValueError("Crypt VXN state order changed")
        if state_ids != [0, 1]:
            raise ValueError("Crypt VXN state IDs changed")

        def fixed_rows(chunk_index: int, expected_count: int,
                       expected_stride: int) -> list[tuple[int, ...]]:
            data = chunks[chunk_index][1]
            if len(data) != 4 + expected_count * expected_stride:
                raise ValueError(f"unexpected {chunks[chunk_index][0]!r} table size")
            if struct.unpack_from("<I", data)[0] != expected_count:
                raise ValueError(f"unexpected {chunks[chunk_index][0]!r} row count")
            return [struct.unpack_from("<" + "I" * (expected_stride // 4), data,
                                       4 + i * expected_stride)
                    for i in range(expected_count)]

        playlist_rows = fixed_rows(3, 2, 8)
        transition_rows = fixed_rows(5, 4, 16)
        group_rows = fixed_rows(6, 2, 24)
        playlist_element_rows = fixed_rows(7, 2, 32)
        rule_rows = fixed_rows(2, 2, 36)
        expected_transition_rows = [
            (0, 0, 0xFFFFFFFF, 0), (0, 1, 0, 0),
            (1, 0, 1, 0), (1, 1, 0xFFFFFFFF, 0),
        ]
        if transition_rows != expected_transition_rows:
            raise ValueError("Crypt VXN source/target transition matrix changed")
        if rule_rows[0] != rule_rows[1] or rule_rows[0][0] != 1:
            raise ValueError("Crypt VXN state transition rules changed")
        if audio_base + segments[-1][0] + segments[-1][1] != SOURCE_BYTES:
            raise ValueError("Crypt segments do not exactly cover encoded payload")
        return ({"source_bytes": SOURCE_BYTES, "source_sha256": SOURCE_SHA256,
                 "source_crc32": f"{info.CRC:08x}", "audio_base": audio_base,
                 "codec": "IMA ADPCM", "channels": CHANNELS,
                 "sample_rate_hz": RATE, "block_bytes": BLOCK_BYTES,
                 "frames_per_block": BLOCK_FRAMES,
                 "state_order": states, "state_ids": state_ids,
                 "playlists_raw_u32": [list(row) for row in playlist_rows],
                 "transition_rows_raw_u32": [list(row) for row in transition_rows],
                 "transition_row_interpretation": {
                     "source_state_index": "word0",
                     "target_state_index": "word1 and row position within source-state list",
                     "transition_rule_index": "word2; 0xffffffff means no rule",
                     "flags": "word3 low byte; only zero observed",
                     "basis": "IDA ParseFile builds a per-source-state ordered TransitionParams vector; SetState indexes it by requested state"
                 },
                 "transition_rules_raw_u32": [list(row) for row in rule_rows],
                 "groups_raw_u32": [list(row) for row in group_rows],
                 "playlist_elements_raw_u32": [list(row) for row in playlist_element_rows]}, segments)


def write_ima_wav(z: zipfile.ZipFile, output: Path, segment: tuple[int, int, int]) -> dict:
    start, encoded_bytes, sample_limit = segment
    if encoded_bytes % BLOCK_BYTES:
        raise ValueError("IMA segment is not block aligned")
    full_blocks = encoded_bytes // BLOCK_BYTES
    if sample_limit <= 0 or sample_limit > full_blocks * BLOCK_FRAMES:
        raise ValueError("IMA sample cap exceeds decoded block capacity")
    # WAVE_FORMAT_IMA_ADPCM, fmt extension cbSize=2, samples/block=1017.
    avg_bytes_per_second = RATE * BLOCK_BYTES // BLOCK_FRAMES
    fmt = struct.pack("<HHIIHHHH", 0x11, CHANNELS, RATE,
                      avg_bytes_per_second, BLOCK_BYTES, 4, 2, BLOCK_FRAMES)
    fact = struct.pack("<I", sample_limit)
    data_bytes = encoded_bytes
    riff_bytes = 4 + 8 + len(fmt) + 8 + len(fact) + 8 + data_bytes
    if riff_bytes + 8 > MAX_OUTPUT_BYTES:
        raise ValueError("generated WAV exceeds output bound")
    output.parent.mkdir(parents=True, exist_ok=True)
    with z.open(SOURCE_MEMBER) as src, output.open("wb") as dst:
        dst.write(b"RIFF" + struct.pack("<I", riff_bytes) + b"WAVE")
        dst.write(b"fmt " + struct.pack("<I", len(fmt)) + fmt)
        dst.write(b"fact" + struct.pack("<I", len(fact)) + fact)
        dst.write(b"data" + struct.pack("<I", data_bytes))
        src.seek(512 + start)
        remaining = data_bytes
        while remaining:
            block = src.read(min(1024 * 1024, remaining))
            if not block:
                raise ValueError("short VXN segment payload")
            dst.write(block)
            remaining -= len(block)
    digest = hashlib.sha256(output.read_bytes()).hexdigest()
    return {"file": output.name, "segment_start": start,
            "encoded_bytes": encoded_bytes, "frames": sample_limit,
            "duration_ms": sample_limit * 1000 // RATE,
            "wav_bytes": output.stat().st_size, "wav_sha256": digest}


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--cache-zip", type=Path, required=True)
    parser.add_argument("--output-dir", type=Path, required=True)
    parser.add_argument("--manifest", type=Path)
    args = parser.parse_args()
    archive_path = args.cache_zip.resolve(strict=True)
    output_dir = args.output_dir.resolve()
    if archive_path == output_dir or output_dir in archive_path.parents:
        raise ValueError("output directory cannot contain/replace the source archive")
    with zipfile.ZipFile(archive_path) as z:
        mapping = source_mapping(z)
        info = z.getinfo(SOURCE_MEMBER)
        with z.open(info) as src:
            if sha256_stream(src) != SOURCE_SHA256:
                raise ValueError("Crypt VXN SHA-256 differs from pinned original")
        bank, segments = parse_bank(z)
        output_dir.mkdir(parents=True, exist_ok=True)
        rendered = []
        for state, segment in zip(bank["state_order"], segments, strict=True):
            name = f"crypt_{state}_music.wav"
            rendered.append(write_ima_wav(z, output_dir / name, segment))
    report = {"schema": "dh2-crypt-vxn-ima-wav-v1", "status": "PASS",
              "mapping": mapping, "bank": bank,
              "segment_state_order_basis": "VXN Stat state row order aligns with Segm row order; state 0 is selected by source SetMusicState(ambient), state 1 by combat",
              "tracks": rendered,
              "playback": "VXN IMA payload remuxed for the app's source-backed AudioTrack decoder; full VoxN playlist transitions remain disconnected"}
    if args.manifest:
        args.manifest.parent.mkdir(parents=True, exist_ok=True)
        args.manifest.write_text(json.dumps(report, indent=2) + "\n", encoding="utf-8")
    print(json.dumps({"status": "PASS", "states": bank["state_order"],
                      "tracks": rendered}, separators=(",", ":")))


if __name__ == "__main__":
    main()
