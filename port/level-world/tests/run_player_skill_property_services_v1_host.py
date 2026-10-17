"""Validate the borrowed Player property callbacks; selected DSO proof is separate."""
from __future__ import annotations

import argparse
import hashlib
import json
import os
import shutil
import subprocess
from pathlib import Path

MODULE = Path(__file__).resolve().parents[1]
ROOT = MODULE.parents[1]
MANIFEST = MODULE / "reference/player-skill-property-services-v1/original-functions.json"
CLASS_REPORT = ROOT / "port/game-data/reports/classes-arm64-differential.json"
ELF_SHA256 = "36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80"
CLASS_REPORT_SHA256 = "e52983dcccb39f401aac5c32e5312f9554185525cd890d0d87a66d19ec8a9293"
PYDATA_NAMES = (
    "character_classes_pyarray.bin",
    "character_classes_pyarraynames.bin",
    "character_classes_pystructnames.bin",
    "character_properties_pyarray.bin",
    "character_properties_pyarraynames.bin",
    "character_properties_pystructnames.bin",
)


def digest(path: Path) -> str:
    return hashlib.sha256(path.read_bytes()).hexdigest()


def check_original(elf_path: Path, manifest: dict) -> dict:
    from elftools.elf.elffile import ELFFile

    if digest(elf_path) != ELF_SHA256 or manifest["original_sha256"] != ELF_SHA256:
        raise RuntimeError("pinned original ELF digest mismatch")
    blob = elf_path.read_bytes()
    checks = []
    with elf_path.open("rb") as stream:
        elf = ELFFile(stream)
        symtab = elf.get_section_by_name(".symtab")
        if symtab is None:
            raise RuntimeError("pinned ELF has no symbol table")
        symbols = {symbol.name: symbol for symbol in symtab.iter_symbols()}
        loads = [segment for segment in elf.iter_segments() if segment["p_type"] == "PT_LOAD"]
        for row in manifest["functions"]:
            address = int(row["elf_address"], 0)
            size = int(row["size"])
            symbol = symbols.get(row["original_symbol"])
            if symbol is None or (int(symbol["st_value"]), int(symbol["st_size"])) != (address, size):
                raise RuntimeError(f"ELF symbol mismatch: {row['original_symbol']}")
            segment = next((seg for seg in loads if int(seg["p_vaddr"]) <= address and
                            address + size <= int(seg["p_vaddr"]) + int(seg["p_filesz"])), None)
            if segment is None:
                raise RuntimeError(f"ELF range is not backed by file bytes: {row['original_symbol']}")
            offset = int(segment["p_offset"]) + address - int(segment["p_vaddr"])
            code = blob[offset:offset + size]
            actual = hashlib.sha256(code).hexdigest()
            if actual != row["sha256"]:
                raise RuntimeError(f"ELF byte digest mismatch: {row['original_symbol']}")
            checks.append({"symbol": row["original_symbol"], "address": row["elf_address"],
                           "size": size, "sha256": actual})
    return {"sha256": ELF_SHA256, "function_ranges_verified": len(checks), "functions": checks}


def run(command: list[str], cwd: Path) -> str:
    completed = subprocess.run(command, cwd=cwd, capture_output=True, text=True)
    if completed.returncode:
        raise RuntimeError("command failed\n" + " ".join(command) + "\n" + completed.stdout + completed.stderr)
    return completed.stdout.strip()


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--compiler", default=os.environ.get("CXX") or shutil.which("g++"))
    parser.add_argument("--original-elf", type=Path,
                        default=ROOT.parent / "test_strategy/libDungeonHunter2.so")
    parser.add_argument("--pydata", type=Path, default=ROOT.parent / "cache/files/data/pydata")
    parser.add_argument("--output", type=Path,
                        default=MODULE / "build/player-skill-property-services-v1/player_skill_property_services_v1.exe")
    parser.add_argument("--report", type=Path,
                        default=MODULE / "build/player-skill-property-services-v1/validation.json")
    parser.add_argument("--skip-cache", action="store_true",
                        help="run only the synthetic fixture when the recovered cache is unavailable")
    args = parser.parse_args()
    if not args.compiler:
        parser.error("g++ not found; pass --compiler")

    elf_path = args.original_elf.resolve()
    if not elf_path.is_file():
        parser.error(f"original ELF missing: {elf_path}")
    manifest = json.loads(MANIFEST.read_text(encoding="utf-8"))
    original = check_original(elf_path, manifest)

    class_report = json.loads(CLASS_REPORT.read_text(encoding="utf-8"))
    if digest(CLASS_REPORT) != CLASS_REPORT_SHA256:
        raise RuntimeError("class-application oracle report digest mismatch")
    related = manifest["related_verified_class_application"]
    for name in ("bit_exact_entire_property_sheet_cases", "buff_snapshot_cases", "original_character_sheet_cases"):
        if class_report.get(name) != related[name]:
            raise RuntimeError(f"class-application oracle count mismatch: {name}")
    if class_report.get("original_sha256") != ELF_SHA256:
        raise RuntimeError("class-application oracle references a different original ELF")

    source_paths = [
        MODULE / "player_skill_property_services_v1.hpp",
        MODULE / "player_skill_property_services_v1.cpp",
        MODULE / "character_native_bindings.hpp",
        MODULE / "tests/player_skill_property_services_v1.cpp",
        Path(__file__).resolve(),
        MANIFEST,
        ROOT / "port/adam-script-runtime/script_runtime.h",
        ROOT / "port/game-data/class_tables.hpp",
        ROOT / "port/game-data/class_tables.cpp",
        ROOT / "port/game-data/data.hpp",
        ROOT / "port/game-data/data.cpp",
        ROOT / "port/game-data/properties.hpp",
        ROOT / "port/game-data/properties.cpp",
        CLASS_REPORT,
    ]
    pydata = args.pydata.resolve()
    use_cache = not args.skip_cache and pydata.is_dir() and all((pydata / name).is_file() for name in PYDATA_NAMES)
    if not args.skip_cache and pydata.exists() and not use_cache:
        raise RuntimeError(f"incomplete pydata cache at {pydata}")
    data_paths = [pydata / name for name in PYDATA_NAMES] if use_cache else []
    if use_cache:
        expected_cache = class_report.get("input_sha256", {})
        for path in data_paths:
            expected = expected_cache.get(path.name)
            if expected is None or digest(path) != expected:
                raise RuntimeError(f"recovered cache data digest mismatch: {path.name}")
    original_inputs = source_paths + data_paths + [elf_path]
    before = {path.relative_to(ROOT).as_posix() if path.is_relative_to(ROOT) else str(path): digest(path)
              for path in original_inputs}

    output = args.output.resolve()
    report_path = args.report.resolve()
    output.parent.mkdir(parents=True, exist_ok=True)
    command = [
        args.compiler, "-std=c++17", "-Wall", "-Wextra", "-Werror", "-fno-fast-math", "-ffp-contract=off",
        "-I", str(MODULE), "-I", str(ROOT / "port/game-data"), "-I", str(ROOT / "port/adam-script-runtime"),
        str(MODULE / "tests/player_skill_property_services_v1.cpp"),
        str(MODULE / "player_skill_property_services_v1.cpp"),
        str(ROOT / "port/game-data/properties.cpp"),
        str(ROOT / "port/game-data/class_tables.cpp"),
        str(ROOT / "port/game-data/data.cpp"),
        "-o", str(output),
    ]
    compile_output = run(command, ROOT)
    synthetic_output = run([str(output)], ROOT)
    if "PASS checks=28" not in synthetic_output or "actual_bashdown_data=not_requested" not in synthetic_output:
        raise RuntimeError("synthetic host assertion count/output mismatch: " + synthetic_output)
    cache_output = None
    if use_cache:
        cache_output = run([str(output), str(pydata)], ROOT)
        if "PASS checks=29" not in cache_output or "actual_bashdown_data=PASS" not in cache_output:
            raise RuntimeError("actual cache assertion count/output mismatch: " + cache_output)

    after = {path.relative_to(ROOT).as_posix() if path.is_relative_to(ROOT) else str(path): digest(path)
             for path in original_inputs}
    if before != after:
        raise RuntimeError("source, cache or original ELF changed during validation")
    report = {
        "validation": "PASS",
        "original_elf": original,
        "original_contract_scope": manifest["source_contract"],
        "source_range_verification_is_not_callback_execution": True,
        "verified_class_application_oracle": {
            "sha256": CLASS_REPORT_SHA256,
            "bit_exact_entire_property_sheet_cases": class_report["bit_exact_entire_property_sheet_cases"],
            "buff_snapshot_cases": class_report["buff_snapshot_cases"],
            "original_character_sheet_cases": class_report["original_character_sheet_cases"],
            "separate_from_full_callback_ARM_differential": True,
        },
        "compiler_command": command,
        "compiler": run([args.compiler, "--version"], ROOT).splitlines()[0],
        "compiler_output": compile_output,
        "synthetic_host_output": synthetic_output,
        "actual_cache_output": cache_output,
        "actual_cache_pydata": str(pydata) if use_cache else None,
        "source_cache_elf_sha256_before": before,
        "source_cache_elf_sha256_after": after,
        "host_executable_sha256": digest(output),
        "actual_cache_data_exercised": bool(use_cache),
        "native_wired": False,
        "full_player_skill_lifecycle": False,
        "limits": [
            "No registered buff owner/groups are supplied.",
            "The caller owns and shares one process-global temporary sheet across skill VMs.",
            "External identity-selected sheets are unsupported.",
            "SetProp remains bounded to recovered type-8 runtime fields; ApplyPropClass permits all class destinations and requires the canonical view for ordinary source resolution.",
            "Only the class-formula engine has a separate ARM differential; the four property callbacks are byte-pinned, not instruction-by-instruction compared here.",
        ],
    }
    report_path.parent.mkdir(parents=True, exist_ok=True)
    report_path.write_text(json.dumps(report, indent=2) + "\n", encoding="utf-8")
    print(json.dumps({"validation": "PASS", "original_ranges": original["function_ranges_verified"],
                      "synthetic": synthetic_output, "actual_cache": cache_output,
                      "executable_sha256": report["host_executable_sha256"],
                      "report": str(report_path)}))
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
