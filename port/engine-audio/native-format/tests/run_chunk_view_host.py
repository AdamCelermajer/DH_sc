#!/usr/bin/env python3
"""Sequential, bounded-memory VoxN framing gate; no audio decoding."""
from __future__ import annotations

import argparse
import hashlib
import json
from pathlib import Path
import struct
import subprocess
import sys

AREA = Path(__file__).resolve().parents[1]
REPO = AREA.parents[2]
INPUTS = [AREA / "chunk_view.hpp", Path(__file__).resolve(),
          AREA / "tests/chunk_view.cpp", AREA / "reference/chunk-view/NOTES.md",
          AREA / "reference/chunk-view/provenance.json",
          AREA / "reference/chunk-view/historical-framing.json"]
KNOWN_TAGS = {b"Afmt", b"Segm", b"Cuse", b"Grps", b"Grpe", b"Rule", b"Plst", b"Stat", b"Trsn"}
EXPECTED_TAGS = [b"Afmt", b"Segm", b"Rule", b"Plst", b"Stat", b"Trsn", b"Grps", b"Grpe"]


def sha(path: Path) -> str:
    digest = hashlib.sha256()
    with path.open("rb") as f:
        for block in iter(lambda: f.read(1024 * 1024), b""):
            digest.update(block)
    return digest.hexdigest()


def run(command: list[str], success: bool = True) -> subprocess.CompletedProcess[str]:
    result = subprocess.run(command, text=True, capture_output=True, check=False)
    if success and result.returncode:
        raise RuntimeError(f"command failed ({result.returncode}): {command}\n{result.stdout}\n{result.stderr}")
    return result


def verify_original(path: Path, provenance: dict) -> dict:
    actual = sha(path)
    if actual != provenance["original_elf_sha256"]:
        raise ValueError("original ELF identity mismatch")
    with path.open("rb") as f:
        head = f.read(52)
        if head[:7] != b"\x7fELF\x01\x01\x01":
            raise ValueError("expected original ELF32 little-endian image")
        phoff = struct.unpack_from("<I", head, 28)[0]
        phsize, phcount = struct.unpack_from("<HH", head, 42)
        loads = []
        for index in range(phcount):
            f.seek(phoff + index * phsize)
            values = struct.unpack("<8I", f.read(32))
            if values[0] == 1:
                loads.append((values[2], values[1], values[4]))
        row = provenance["original_framing_evidence"]
        address, length = int(row["elf_va"], 0), row["range_size"]
        segment = next(s for s in loads if s[0] <= address and address + length <= s[0] + s[2])
        offset = segment[1] + address - segment[0]
        f.seek(offset)
        measured = hashlib.sha256(f.read(length)).hexdigest()
        if measured != row["sha256"]:
            raise ValueError("original parser range hash mismatch")
    return {"elf_sha256": actual, "range_va": row["elf_va"], "range_bytes": length,
            "range_sha256": measured, "file_offset": offset, "original_execution": False}


def framing(metadata: bytes, physical: int) -> dict:
    if len(metadata) < 24 or metadata[:4] != b"VoxN":
        raise ValueError("bad local container header")
    count, declared, audio = (struct.unpack_from("<I", metadata, n)[0] for n in (4, 16, 20))
    if count != 16 or declared != physical or audio < 32 or audio > physical:
        raise ValueError("unsupported or malformed local container layout")
    start, end = 8 + count, audio - 8
    if len(metadata) != end:
        raise ValueError("incorrect metadata window fixture")
    off = start
    rows = []
    while off < end:
        if end - off < 8:
            raise ValueError("truncated reference chunk header")
        name = metadata[off:off + 4]
        n = struct.unpack_from("<I", metadata, off + 4)[0]
        if n > end - off - 8:
            raise ValueError("truncated reference chunk payload")
        payload = metadata[off + 8:off + 8 + n]
        rows.append({"tag": int.from_bytes(name, "little"), "offset": off,
                     "size": n, "known": name in KNOWN_TAGS, "payload_hex": payload.hex()})
        off += 8 + n
    return {"status": "ok", "chunk_start": start, "chunk_bytes": end - start,
            "audio_base": audio, "audio_bytes": physical - audio, "chunks": rows,
            "final_offset": off - start, "payloads_decoded": False}


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--cache", type=Path, required=True, help="explicit root containing the local .vxn files")
    parser.add_argument("--original-elf", type=Path, required=True)
    parser.add_argument("--compiler", required=True, help="absolute C++17 compiler path")
    parser.add_argument("--build-dir", type=Path, default=REPO / "port/level-world/build/vox-n-chunk-view")
    parser.add_argument("--report", type=Path)
    parser.add_argument("--expected-files", type=int, default=17)
    parser.add_argument("--max-metadata-bytes", type=int, default=1024 * 1024)
    args = parser.parse_args()
    if not 24 <= args.max_metadata_bytes <= 1024 * 1024 or args.expected_files <= 0:
        raise ValueError("invalid bounded test limits")
    compiler = Path(args.compiler).resolve(strict=True)
    cache = args.cache.resolve(strict=True)
    original = args.original_elf.resolve(strict=True)
    build = args.build_dir.resolve()
    build.mkdir(parents=True, exist_ok=True)
    report_path = (args.report or build / "validation.json").resolve()
    before = {str(p.relative_to(REPO)).replace("\\", "/"): sha(p) for p in INPUTS}
    provenance = json.loads(INPUTS[4].read_text(encoding="utf-8"))
    historical = json.loads(INPUTS[5].read_text(encoding="utf-8"))
    evidence = verify_original(original, provenance)
    historical_rows = {row["file_name"]: row for row in historical["assets"]}
    if len(historical_rows) != 12:
        raise ValueError("expected exactly twelve historical framing records")
    executable = build / ("chunk_view_host.exe" if sys.platform == "win32" else "chunk_view_host")
    command = [str(compiler), "-std=c++17", "-O2", "-Wall", "-Wextra", "-Werror", "-pedantic",
               str(AREA / "tests/chunk_view.cpp"), "-o", str(executable)]
    run(command)
    guards = json.loads(run([str(executable), "--guards"]).stdout)
    if guards["status"] != "PASS" or guards["mismatches"] != 0 or guards["checks"] < 100:
        raise ValueError("host guard suite failed or incomplete")
    files = sorted(cache.rglob("*.vxn"))
    if len(files) != args.expected_files or len({p.name for p in files}) != len(files):
        raise ValueError("unexpected local VoxN file count or duplicate basenames")
    rows = []
    historical_matches = set()
    comparisons = rejection_cases = 0
    largest_metadata = 0
    for path in files:
        physical = path.stat().st_size
        with path.open("rb") as f:
            prefix = f.read(24)
            if len(prefix) != 24:
                raise ValueError("truncated local file prefix")
            audio = struct.unpack_from("<I", prefix, 20)[0]
            metadata_bytes = audio - 8
            if not 24 <= metadata_bytes <= args.max_metadata_bytes or metadata_bytes > physical:
                raise ValueError("metadata exceeds configured read bound")
            f.seek(0)
            metadata = f.read(metadata_bytes)
            if len(metadata) != metadata_bytes:
                raise ValueError("short metadata read")
        digest = sha(path)  # Streams audio for identity, never retains it.
        if path.stat().st_size != physical:
            raise ValueError("local asset changed during test")
        expected = framing(metadata, physical)
        tags = [row["tag"].to_bytes(4, "little") for row in expected["chunks"]]
        if tags != EXPECTED_TAGS:
            raise ValueError("unexpected local chunk sequence; extend explicit corpus scope first")
        fixture = build / (path.name + ".metadata")
        fixture.write_bytes(metadata)
        actual = json.loads(run([str(executable), "--inspect", str(fixture), str(physical)]).stdout)
        if actual != expected:
            raise ValueError("compiled helper differs from independent byte inventory")
        comparisons += len(expected["chunks"])
        old = historical_rows.get(path.name)
        historical_match = old is not None
        if old:
            if old["file_sha256"] != digest or old["file_size_bytes"] != physical or old["framing"] != expected:
                raise ValueError("historical corpus identity/framing mismatch")
            historical_matches.add(path.name)
        # Actual per-file chunk-window truncation and maximal payload-size mutation.
        truncated = build / "truncated.metadata"
        truncated.write_bytes(metadata[:-1])
        result = run([str(executable), "--inspect", str(truncated), str(physical)], success=False)
        if result.returncode != 1 or "misses chunk window" not in result.stderr:
            raise ValueError("metadata truncation was accepted")
        broken = bytearray(metadata)
        struct.pack_into("<I", broken, expected["chunks"][-1]["offset"] + 4, 0xffffffff)
        malformed = build / "malformed.metadata"
        malformed.write_bytes(broken)
        result = run([str(executable), "--inspect", str(malformed), str(physical)], success=False)
        if result.returncode != 1 or "malformed chunk window" not in result.stderr:
            raise ValueError("maximal payload length was accepted")
        rejection_cases += 2
        fmt = bytes.fromhex(expected["chunks"][0]["payload_hex"])
        selector = struct.unpack_from("<H", fmt)[0]
        rows.append({"path": str(path.relative_to(cache)).replace("\\", "/"),
                     "file_bytes": physical, "file_sha256": digest, "historical_match": historical_match,
                     "metadata_bytes": metadata_bytes, "metadata_sha256": hashlib.sha256(metadata).hexdigest(),
                     "format_selector_observed": selector, "codec_implemented": False, "framing": actual,
                     "fixture": {"path": str(fixture), "sha256": sha(fixture)}})
        largest_metadata = max(largest_metadata, metadata_bytes)
    if historical_matches != historical_rows.keys():
        raise ValueError("not all historical inputs were tested")
    after = {str(p.relative_to(REPO)).replace("\\", "/"): sha(p) for p in INPUTS}
    if before != after:
        raise ValueError("source inputs changed during compile/test")
    report = {"schema": "dh2-vox-n-framing-host-v1", "status": "PASS",
              "scope": "bounded port framing; no decoder/playback/original-body reconstruction",
              "complete_original_body_credit": 0, "original_execution": False,
              "source_sha256": before, "original_evidence": evidence,
              "cache_root": str(cache), "compiler": str(compiler), "compile_command": command,
              "compiler_version": run([str(compiler), "--version"]).stdout.splitlines()[0],
              "executable": {"path": str(executable), "sha256": sha(executable)},
              "local_file_count": len(rows), "historical_file_count": len(historical_matches),
              "additional_file_count": len(rows) - len(historical_matches),
              "chunk_comparisons": comparisons, "corpus_rejection_cases": rejection_cases,
              "guard_suite": guards, "mismatches": 0,
              "maximum_metadata_bytes_retained": largest_metadata,
              "hash_stream_buffer_bytes": 1024 * 1024,
              "metadata_limit_bytes": args.max_metadata_bytes,
              "unsupported": ["all audio decoding and playback", "payload schema/transition semantics",
                              "unknown tags (raw framing only)", "header read counts other than 16"],
              "assets": rows}
    report_path.parent.mkdir(parents=True, exist_ok=True)
    report_path.write_text(json.dumps(report, indent=2) + "\n", encoding="utf-8", newline="\n")
    print(json.dumps({k: report[k] for k in ["status", "local_file_count", "historical_file_count",
                     "chunk_comparisons", "corpus_rejection_cases", "guard_suite", "mismatches"]}))


if __name__ == "__main__":
    main()
