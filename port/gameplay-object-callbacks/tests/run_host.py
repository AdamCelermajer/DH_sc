#!/usr/bin/env python3
"""Verify the imported Lua callback evidence and run read-only host projections."""

import argparse
import hashlib
import json
import os
from pathlib import Path
import shutil
import subprocess
import tempfile


HERE = Path(__file__).resolve().parents[1]
REPO = HERE.parents[1]

EXPECTED = {
    ("GameObject::createBindings", "GetID"): (
        "0x0038ebe4", "_ZN10GameObject6_GetID", "pushPointerEPv(param_2,param_3)"),
    ("GameObject::createBindings", "GetName"): (
        "0x0038ebf0", "_ZN10GameObject8_GetName",
        "pushStringEPKc(param_2,*(undefined4 *)(param_3 + 0x44))"),
    ("GameObject::createBindings", "GetPosition"): (
        "0x0038e700", "_ZN10GameObject12_GetPosition",
        "param_3 + 0x160", "param_3 + 0x164", "param_3 + 0x168"),
    ("Character::createBindings", "GetState"): (
        "0x003b6d78", "_ZN9Character9_GetState",
        "SM_GetStateEv(param_3 + 0x4fc)"),
    ("Character::createBindings", "GetStateTime"): (
        "0x003b6d6c", "_ZN9Character13_GetStateTime",
        "*(undefined4 *)(param_3 + 0x55c)"),
    ("Character::createBindings", "GetHitCount"): (
        "0x003b6cc8", "_ZN9Character12_GetHitCount",
        "*(undefined2 *)(param_3 + 0x14d0)"),
}


def sha256(path: Path) -> str:
    return hashlib.sha256(path.read_bytes()).hexdigest()


def compiler() -> str:
    requested = os.environ.get("CXX", "c++")
    found = shutil.which(requested)
    if found:
        return found
    if Path(requested).is_file():
        return str(Path(requested).resolve())
    raise SystemExit(f"C++ compiler not found: {requested}; set CXX to override")


def verify_elf_callback_evidence() -> dict:
    registration_path = REPO / "reports/lua-registration-trace.json"
    pseudocode_root = REPO / "recovered/native/decompiled/libDungeonHunter2.so"
    index_path = pseudocode_root / "function-index.jsonl"
    registrations = json.loads(registration_path.read_text(encoding="utf-8"))
    index = [json.loads(line) for line in index_path.read_text(encoding="utf-8").splitlines()]
    index_by_address = {row["elf_address"].lower().lstrip("0"): row for row in index}

    callbacks = {}
    for (scope, callback_name), (address, mangled_prefix, *body_tokens) in EXPECTED.items():
        matches = [
            row for row in registrations["calls"]
            if row.get("scope") == scope
            and row.get("binding") == "function"
            and row.get("name") == callback_name
            and row.get("target_elf_address") == address
        ]
        if not matches:
            raise SystemExit(f"Lua registration missing {scope}.{callback_name} at {address}")
        if len({row.get("target_symbol") for row in matches}) != 1:
            raise SystemExit(f"Lua registration target is ambiguous for {scope}.{callback_name}")

        function = index_by_address.get(address[2:].lower().lstrip("0"))
        if not function or not function.get("success"):
            raise SystemExit(f"no successful Ghidra pseudocode export for {scope}.{callback_name}")
        if not function["name"].startswith(mangled_prefix):
            raise SystemExit(f"ELF symbol changed for {scope}.{callback_name}")
        shard = pseudocode_root / function["file"]
        data = shard.read_bytes()
        body = data[function["byte_offset"]:function["byte_offset"] + function["byte_length"]]
        if len(body) != function["byte_length"]:
            raise SystemExit(f"truncated pseudocode body for {scope}.{callback_name}")
        body_text = body.decode("utf-8", errors="replace")
        if any(token not in body_text for token in body_tokens):
            raise SystemExit(f"expected body evidence changed for {scope}.{callback_name}")
        callbacks[f"{scope}.{callback_name}"] = {
            "elf_address": address,
            "symbol": function["name"],
            "pseudocode_file": str(shard.relative_to(REPO)).replace("\\", "/"),
            "pseudocode_offset": function["byte_offset"],
            "pseudocode_length": function["byte_length"],
            "body_sha256": hashlib.sha256(body).hexdigest(),
            "registration_entries": len(matches),
            "decompiler_warning": function["has_warning"],
        }

    return {
        "registration_report": str(registration_path.relative_to(REPO)).replace("\\", "/"),
        "original_elf_sha256_from_report": registrations["original_sha256"],
        "registration_report_sha256": sha256(registration_path),
        "function_index_sha256": sha256(index_path),
        "callbacks": callbacks,
    }


def run_projection_test(cache: Path) -> str:
    sources = [
        HERE / "tests/read_projection_test.cpp",
        HERE / "gameplay_object_callbacks.cpp",
        REPO / "port/actor-runtime/actor_registry.cpp",
        REPO / "port/world-data/world.cpp",
        REPO / "port/world-data/world_scene.cpp",
        REPO / "port/scene-payloads/scene.cpp",
        REPO / "port/engine-resources/resources.cpp",
        REPO / "port/engine-math/math.cpp",
    ]
    missing = [str(path) for path in sources if not path.is_file()]
    if missing:
        raise SystemExit("missing host test sources: " + ", ".join(missing))
    flags = [
        "-std=c++17", "-O2", "-fno-exceptions", "-fno-rtti",
        "-fno-fast-math", "-ffp-contract=off", "-fno-builtin",
        "-Wall", "-Wextra", "-Werror",
    ]
    with tempfile.TemporaryDirectory(prefix="dh2-gameplay-callbacks-") as temp:
        executable = Path(temp) / ("read_projection_test.exe" if os.name == "nt"
                                   else "read_projection_test")
        build = [compiler(), *flags, *(str(path) for path in sources),
                 "-lm", "-o", str(executable)]
        print("+", subprocess.list2cmdline(build), flush=True)
        subprocess.run(build, check=True)
        run = [str(executable), str(cache)]
        print("+", subprocess.list2cmdline(run), flush=True)
        result = subprocess.run(run, check=True, capture_output=True, text=True)
        print(result.stdout.strip())
        return result.stdout.strip()


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--cache", type=Path, default=REPO.parent / "cache/files",
                        help="owner-supplied cache/files; reads are not copied")
    parser.add_argument("--report", type=Path, default=HERE / "build/validation.json",
                        help="host-only report path (default: ignored build directory)")
    args = parser.parse_args()
    cache = args.cache.resolve()
    cache_paths = [
        "data/scene/005_infectedvillage.mlx",
        "data/3d/modules/infectedvillage/mgp/infected01.mgp",
        "data/3d/modules/infectedvillage/mgp/infected02.mgp",
    ]
    for relative in cache_paths:
        if not (cache / relative).is_file():
            raise SystemExit(f"missing owner-supplied cache input: {cache / relative}")

    evidence = verify_elf_callback_evidence()
    test_output = run_projection_test(cache)
    evidence.update({
        "format": "dh2.gameplay-object-callbacks.v1",
        "scope": "Port-owned, read-only value projections for six source-registered callbacks. No original C++ ABI, Lua binding, gameplay state owner, or Android integration is supplied.",
        "complete_game": False,
        "android_runtime_integration_tested": False,
        "get_id_numeric_value_invented": False,
        "cache_input_files": {
            relative: {"bytes": (cache / relative).stat().st_size,
                       "sha256": sha256(cache / relative)}
            for relative in cache_paths
        },
        "cache_ambush_characters_checked": 4,
        "host_test_output": test_output,
        "unimplemented_boundaries": [
            "original userdata/Lua ReturnValues marshaling",
            "numeric runtime object ID (GetID returns callback-self pointer in this export)",
            "IsDead virtual method owner",
            "controller, collision, pathfinding, AI and animation updates",
            "authoritative state-machine/time/hit-counter lifecycle",
        ],
    })
    report = args.report.resolve()
    report.parent.mkdir(parents=True, exist_ok=True)
    report.write_text(json.dumps(evidence, indent=2) + "\n", encoding="utf-8")
    print(f"wrote {report}")
    print(json.dumps(evidence, indent=2))
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
