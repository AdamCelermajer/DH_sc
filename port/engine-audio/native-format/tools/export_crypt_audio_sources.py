#!/usr/bin/env python3
"""Export the source Crypt music bank and catacomb loop without decoding them."""
from __future__ import annotations

import argparse
import hashlib
import json
from pathlib import Path
import re
import struct
import wave
import xml.etree.ElementTree as ET
import zipfile

ROOT = "com.gameloft.android.GAND.GloftD2SS/files/"
PINNED = {
    "m_level_cathedral_sfx_catacomb.vxn": (
        10_281_472,
        "6c0236d19b66011d986a7fd31a332257d8f689ceea69e049abf90376d33ed909",
    ),
    "sfx_catacomb_wind_loop.wav": (
        217_734,
        "4a5ef7713c61d024ee72453df7970b97ebfcb911a563bf996ad5be063e99a4ec",
    ),
}
MAX_COPY_BYTES = 20 * 1024 * 1024
MAX_WAV_FRAMES = 10 * 60 * 48_000


def sha256(path: Path) -> str:
    digest = hashlib.sha256()
    with path.open("rb") as source:
        for block in iter(lambda: source.read(1024 * 1024), b""):
            digest.update(block)
    return digest.hexdigest()


def one_member(names: list[str], suffix: str) -> str:
    matches = [name for name in names if name.endswith(suffix)]
    if len(matches) != 1:
        raise ValueError(f"expected one archive member ending {suffix!r}, got {len(matches)}")
    return matches[0]


def source_row(root: ET.Element, label: str) -> dict[str, str]:
    rows = [row.attrib for row in root.iter("sound") if row.attrib.get("label") == label]
    if len(rows) != 1:
        raise ValueError(f"expected one sound row for {label!r}, got {len(rows)}")
    return rows[0]


def copy_member(archive: zipfile.ZipFile, member: str, target: Path,
                expected_size: int, expected_sha: str) -> None:
    info = archive.getinfo(member)
    if info.file_size != expected_size or info.file_size > MAX_COPY_BYTES:
        raise ValueError(f"unexpected source size for {member}: {info.file_size}")
    digest = hashlib.sha256()
    written = 0
    with archive.open(info, "r") as source, target.open("wb") as output:
        while True:
            block = source.read(1024 * 1024)
            if not block:
                break
            written += len(block)
            if written > expected_size:
                raise ValueError(f"source exceeded expected bound: {member}")
            digest.update(block)
            output.write(block)
    if written != expected_size or digest.hexdigest() != expected_sha:
        target.unlink(missing_ok=True)
        raise ValueError(f"source identity mismatch for {member}")


def inspect_vxn(path: Path) -> dict[str, int | str]:
    physical = path.stat().st_size
    with path.open("rb") as source:
        prefix = source.read(24)
        if len(prefix) != 24 or prefix[:4] != b"VoxN":
            raise ValueError("Crypt music asset is not a VoxN container")
        count = struct.unpack_from("<I", prefix, 4)[0]
        declared, audio_base = struct.unpack_from("<II", prefix, 16)
        if count != 16 or declared != physical or audio_base != 512:
            raise ValueError("Crypt VoxN framing differs from the validated source profile")
        source.seek(24)
        metadata = source.read(audio_base - 8 - 24)
        if len(metadata) != 480:
            raise ValueError("short Crypt VoxN metadata")
    offset = 0
    chunks: list[tuple[bytes, bytes]] = []
    while offset < len(metadata):
        if len(metadata) - offset < 8:
            raise ValueError("truncated VoxN chunk header")
        tag = metadata[offset:offset + 4]
        size = struct.unpack_from("<I", metadata, offset + 4)[0]
        offset += 8
        if size > len(metadata) - offset:
            raise ValueError("VoxN chunk exceeds bounded metadata window")
        chunks.append((tag, metadata[offset:offset + size]))
        offset += size
    if [tag for tag, _ in chunks] != [b"Afmt", b"Segm", b"Rule", b"Plst", b"Stat", b"Trsn", b"Grps", b"Grpe"]:
        raise ValueError("unexpected Crypt VoxN chunk sequence")
    afmt = chunks[0][1]
    if len(afmt) != 12 or struct.unpack_from("<HHI", afmt) != (0x11, 2, 32_000):
        raise ValueError("unsupported Crypt VoxN format metadata")
    return {"physical_bytes": physical, "audio_base": audio_base,
            "audio_bytes": physical - audio_base, "channels": 2,
            "sample_rate_hz": 32_000, "codec_family": "IMA ADPCM (source decoder identified)",
            "payload_decoded": False, "metadata_chunk_count": len(chunks)}


def inspect_wav(path: Path) -> dict[str, int | str]:
    with wave.open(str(path), "rb") as wav:
        channels = wav.getnchannels()
        rate = wav.getframerate()
        frames = wav.getnframes()
        sample_width = wav.getsampwidth()
        if wav.getcomptype() != "NONE" or channels not in (1, 2) or rate <= 0 or frames > MAX_WAV_FRAMES:
            raise ValueError("unsupported or out-of-bound ambient WAV")
        return {"channels": channels, "sample_rate_hz": rate,
                "sample_width_bytes": sample_width, "frames": frames,
                "duration_ms": frames * 1000 // rate, "compression": "PCM"}


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--cache-zip", type=Path, required=True)
    parser.add_argument("--output-dir", type=Path, required=True)
    args = parser.parse_args()
    archive_path = args.cache_zip.resolve(strict=True)
    output = args.output_dir.resolve()
    output.mkdir(parents=True, exist_ok=True)
    if archive_path.is_relative_to(output) or output.is_relative_to(archive_path.parent):
        raise ValueError("output must be a separate directory from the source cache")

    with zipfile.ZipFile(archive_path) as archive:
        names = archive.namelist()
        music_row = one_member(names, ROOT + "data/sounds/sounds.xml")
        rule_path = one_member(names, ROOT + "data/scene/007_crypt_01.rule.xml")
        with archive.open(music_row) as source:
            sounds = ET.parse(source).getroot()
        with archive.open(rule_path) as source:
            opening_rule = source.read(4096).decode("utf-8", "strict")
        root_tag = re.search(r"<rules\b([^>]*)>", opening_rule)
        if not root_tag:
            raise ValueError("Crypt source rule has no bounded rules root")
        rule_attrs = dict(re.findall(r'([A-Za-z_][A-Za-z0-9_-]*)="([^"]*)"', root_tag.group(1)))
        if rule_attrs.get("music") != "CryptOneAmbientMusic" or rule_attrs.get("ambiant_music") != "sfx_catacomb_ambiance":
            raise ValueError("Crypt source rule music fields differ from expected values")
        music = source_row(sounds, "CryptOneAmbientMusic")
        wind = source_row(sounds, "CatacombWindLoop")
        if music.get("uid") != "486" or music.get("filename") != "m_level_cathedral_sfx_catacomb.vxn" or music.get("loop") != "yes":
            raise ValueError("Crypt music table mapping changed")
        if wind.get("uid") != "179" or wind.get("filename") != "sfx_catacomb_wind_loop.wav" or wind.get("loop") != "yes":
            raise ValueError("Catacomb wind sound mapping changed")
        exported: dict[str, dict] = {}
        for filename, (size, expected_sha) in PINNED.items():
            member = one_member(names, ROOT + "data/sounds/" + filename)
            target = output / filename
            copy_member(archive, member, target, size, expected_sha)
            metadata = inspect_vxn(target) if filename.endswith(".vxn") else inspect_wav(target)
            exported[filename] = {"source_member": member, "bytes": size,
                                  "sha256": expected_sha, "format": metadata}

    report = {
        "schema": "dh2-crypt-audio-source-export-v1",
        "status": "PASS",
        "source_rule": {"member": rule_path, "music": rule_attrs["music"],
                         "ambient_alias": rule_attrs["ambiant_music"]},
        "music_sound_row": music,
        "ambient_candidate_row": wind,
        "ambient_alias_resolution": "unresolved: rule alias is absent from sounds.xml labels",
        "assets": exported,
        "decoding": "VoxN payload retained encoded; WAV was validated as PCM; no codec decode or playback",
    }
    (output / "manifest.json").write_text(json.dumps(report, indent=2) + "\n", encoding="utf-8")
    print(json.dumps({"status": report["status"], "music_uid": music["uid"],
                      "ambient_candidate_uid": wind["uid"], "assets": list(exported),
                      "vxn_decoded": False}))


if __name__ == "__main__":
    main()
