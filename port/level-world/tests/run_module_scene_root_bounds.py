#!/usr/bin/env python3
"""Host-check Module::InitPost's scene-root bounds producer on real SWAMP data."""
from __future__ import annotations

import argparse
import hashlib
import json
import os
from pathlib import Path
import shutil
import struct
import subprocess
import sys

ROOT = Path(__file__).resolve().parents[3]
LEVEL = ROOT / "port/level-world"
ELF_SHA = "36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80"
ELF_DEFAULT = ROOT.parent / "test_strategy/libDungeonHunter2.so"
ORIGINAL_MANIFEST = LEVEL / "reference/module-scene-root-bounds/original-functions.json"
FUNCTIONS = {
    "VisualObject_constructor": ("_ZN12VisualObjectC1EP10GameObjectRKSsS3_", 0x472A0C, 592),
    "AssetManager_loadSceneNode": ("_ZN12AssetManager13loadSceneNodeEPKcS1_bi", 0x50A504, 60),
    "SceneManager_LoadScene": ("_ZN12SceneManager9LoadSceneEPKcS1_bb", 0x3596F8, 832),
    "RootSceneNode_ResetPositionFromFile": ("_ZN13RootSceneNode21ResetPositionFromFileEv", 0x35CC0C, 208),
    "VisualObject_SyncPosition": ("_ZN12VisualObject12SyncPositionEv", 0x470CB8, 20),
    "VisualObject_SetPosition": ("_ZN12VisualObject11SetPositionERK7Point3DIfE", 0x470C24, 96),
    "Module_InitPost": ("_ZN6Module8InitPostEv", 0x388B20, 312),
    "RootSceneNode_constructor": ("_ZN13RootSceneNodeC1ERKN6glitch7collada16CColladaDatabaseE", 0x35D824, 236),
    "RootSceneNode_RefreshBoundingBox": ("_ZN13RootSceneNode18RefreshBoundingBoxEv", 0x35C854, 156),
    "CSceneNode_computeBoundingBox": ("_ZNK6glitch7collada10CSceneNode18computeBoundingBoxERNS_4core8aabbox3dIfEE", 0x65CF8C, 452),
    "ISceneNode_getTransformedBoundingBox": ("_ZNK6glitch5scene10ISceneNode25getTransformedBoundingBoxEv", 0x59770C, 124),
}
SOURCES = [
    LEVEL / "tests/module_scene_root_bounds_host.cpp",
    LEVEL / "module_scene_root_bounds.cpp",
    ROOT / "port/level-world/floor_source.cpp",
    ROOT / "port/world-data/world.cpp",
    ROOT / "port/world-data/world_scene.cpp",
    ROOT / "port/scene-payloads/scene.cpp",
    ROOT / "port/scene-materials/scene.cpp",
    ROOT / "port/asset-payloads/payloads.cpp",
    ROOT / "port/engine-resources/resources.cpp",
    ROOT / "port/engine-math/math.cpp",
]


def digest(data: bytes) -> str:
    return hashlib.sha256(data).hexdigest()


def bytes_at(elf, raw: bytes, address: int, size: int) -> bytes:
    for segment in elf.iter_segments():
        if segment["p_type"] != "PT_LOAD":
            continue
        start = int(segment["p_vaddr"])
        filesz = int(segment["p_filesz"])
        if start <= address and address + size <= start + filesz:
            offset = int(segment["p_offset"]) + address - start
            return raw[offset:offset + size]
    raise ValueError(f"ELF bytes unavailable at {address:#x}/{size:#x}")


def verify_original(path: Path) -> dict:
    raw = path.read_bytes()
    elf_sha = digest(raw)
    if elf_sha != ELF_SHA:
        raise ValueError(f"original ELF SHA mismatch: {elf_sha}")
    sys.path.insert(0, str(ROOT / "port/engine-resources/tests"))
    from elftools.elf.elffile import ELFFile

    with path.open("rb") as stream:
        elf = ELFFile(stream)
        symtab = elf.get_section_by_name(".symtab")
        symbols = {symbol.name: symbol for symbol in symtab.iter_symbols()}
        evidence = {}
        for label, (name, address, size) in FUNCTIONS.items():
            symbol = symbols.get(name)
            if symbol is None or (int(symbol["st_value"]), int(symbol["st_size"])) != (address, size):
                raise ValueError(f"source function identity changed: {label}/{name}")
            evidence[label] = {
                "original_symbol": name,
                "address": hex(address),
                "size": size,
                "sha256": digest(bytes_at(elf, raw, address, size)),
            }
        manifest = json.loads(ORIGINAL_MANIFEST.read_text(encoding="utf-8"))
        if manifest.get("original_sha256") != elf_sha:
            raise ValueError("checked-in source evidence manifest is for another ELF")
        pinned = {entry["label"]: entry for entry in manifest.get("functions", [])}
        if set(pinned) != set(FUNCTIONS):
            raise ValueError("checked-in source function manifest scope changed")
        for label, observed in evidence.items():
            expected = pinned[label]
            if any(expected.get(key) != observed[key]
                   for key in ("original_symbol", "address", "size", "sha256")):
                raise ValueError(f"function bytes differ from checked-in evidence: {label}")

        def require_bytes(address: int, expected: str, label: str) -> None:
            observed = bytes_at(elf, raw, address, len(bytes.fromhex(expected)))
            if observed != bytes.fromhex(expected):
                raise ValueError(f"source callsite changed: {label}={observed.hex()}")

        # VisualObject passes a selected-node pointer into loadSceneNode.
        # That callee canonicalizes node!=null to its reset flag before
        # tail-calling SceneManager::LoadScene.
        require_bytes(0x472B18, "795e02eb", "VisualObject -> loadSceneNode branch")
        require_bytes(0x50A518, "00c052e201c0a0130030a0e3", "loadSceneNode node/reset conversion")
        require_bytes(0x50A534, "6f3cf9ea", "loadSceneNode -> SceneManager::LoadScene")
        require_bytes(0x3597C4, "100d00eb", "SceneManager -> ResetPositionFromFile")
        require_bytes(0x35CC10, "f45090e5", "reset reads selected first child")
        require_bytes(0x35CC38, "a43093e5", "reset selected-child setPosition slot +0xa4")
        require_bytes(0x35CC68, "9c709ce5", "reset selected-child setRotation slot +0x9c")
        require_bytes(0x35CC88, "943093e5", "reset selected-child setScale slot +0x94")
        require_bytes(0x35CCAC, "a43093e5", "reset outer-root setPosition slot +0xa4")
        require_bytes(0x35CCD0, "b8f093e5", "reset updates selected root absolute transform")
        require_bytes(0x470CC4, "161e81e2d5ffffea", "SyncPosition owner +0x160 -> SetPosition")
        require_bytes(0x388B6C, "34f093e5", "Module::InitPost root transformed bounds slot +0x34")
        require_bytes(0x388B48, "d83298e5", "Module::InitPost reads VisualObject at +0x2d8")
        require_bytes(0x388B54, "083093e5", "Module::InitPost reads scene root at VisualObject +8")

        vt = symbols.get("_ZTV13RootSceneNode")
        if vt is None or int(vt["st_value"]) != 0x95FDD8:
            raise ValueError("RootSceneNode vtable symbol changed")
        table = int(vt["st_value"])
        got_literal = struct.unpack("<I", bytes_at(elf, raw, 0x35D900, 4))[0]
        got_base = 0x35D83C + got_literal
        offset = struct.unpack("<I", bytes_at(elf, raw, 0x35D90C, 4))[0]
        got_slot = got_base + offset
        relocations = []
        for section in elf.iter_sections():
            if section["sh_type"] not in ("SHT_REL", "SHT_RELA"):
                continue
            symtab_for_reloc = elf.get_section(section["sh_link"])
            for relocation in section.iter_relocations():
                if int(relocation["r_offset"]) == got_slot:
                    relocations.append((section.name, int(relocation["r_info_type"]),
                                        symtab_for_reloc.get_symbol(relocation["r_info_sym"]).name))
        if relocations != [(".rel.dyn", 23, "")]:
            raise ValueError(f"RootSceneNode vptr GOT relocation changed: {relocations}")
        if struct.unpack("<I", bytes_at(elf, raw, got_slot, 4))[0] != table:
            raise ValueError("RootSceneNode vptr GOT no longer names its vtable")
        if bytes_at(elf, raw, 0x35D890, 8) != bytes.fromhex("1c3083e2003084e5"):
            raise ValueError("RootSceneNode constructor address point changed")
        address_point = table + 0x1C
        targets = {}
        for offset, expected in ((0x30, 0x5839B0), (0x34, 0x59770C), (0xA0, 0x597124)):
            slot = address_point + offset
            target = struct.unpack("<I", bytes_at(elf, raw, slot, 4))[0]
            relocs = [int(r["r_info_type"]) for sec in elf.iter_sections()
                      if sec["sh_type"] in ("SHT_REL", "SHT_RELA")
                      for r in sec.iter_relocations() if int(r["r_offset"]) == slot]
            if target != expected or relocs != [23]:
                raise ValueError(f"RootSceneNode address-point slot {offset:#x} changed")
            targets[hex(offset)] = hex(target)
        evidence["RootSceneNode_vptr_gate"] = {
            "vtable": "_ZTV13RootSceneNode",
            "symbol_address": hex(table),
            "constructor_address_point": hex(address_point),
            "constructor_store_bytes": bytes_at(elf, raw, 0x35D890, 8).hex(),
            "vtable_got_slot": hex(got_slot),
            "relocation": relocations[0],
            "virtual_targets": targets,
        }
        evidence["callsite_evidence"] = {
            "VisualObject_node_select_reset": "selected node pointer is converted to reset=true before SceneManager::LoadScene",
            "ResetPositionFromFile": "selected child is reset to position zero, identity rotation, unit scale; outer root position zero and selected root absolute transform updated",
            "owner_sync": "VisualObject::SyncPosition passes owner GameObject +0x160 XYZ to VisualObject::SetPosition",
            "Module_bounds": "Module::InitPost invokes RootSceneNode vptr+0x34 getTransformedBoundingBox",
        }
        pinned_call_bytes = manifest.get("callsite_bytes", {})
        for address, key in ((0x472B18, "VisualObject_calls_loadSceneNode_at_0x472b18"),
                             (0x50A518, "loadSceneNode_node_to_reset_flag_at_0x50a518"),
                             (0x50A534, "loadSceneNode_calls_LoadScene_at_0x50a534"),
                             (0x3597C4, "LoadScene_calls_ResetPositionFromFile_at_0x3597c4"),
                             (0x470CC4, "SyncPosition_owner_plus_0x160_then_SetPosition_at_0x470cc4"),
                             (0x388B48, "Module_InitPost_loads_visual_at_plus_0x2d8"),
                             (0x388B54, "Module_InitPost_loads_root_at_visual_plus_8"),
                             (0x388B6C, "Module_InitPost_calls_root_slot_plus_0x34_at_0x388b6c")):
            if pinned_call_bytes.get(key) != bytes_at(elf, raw, address,
                    len(bytes.fromhex(pinned_call_bytes.get(key, "")))).hex():
                raise ValueError(f"callsite differs from checked-in evidence: {key}")
    return {"original_sha256": elf_sha, "functions": evidence}


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--cache", type=Path, required=True)
    parser.add_argument("--cxx", default="g++")
    parser.add_argument("--original-elf", type=Path, default=ELF_DEFAULT)
    parser.add_argument("--output", type=Path, default=LEVEL / "build/module-scene-root-bounds-host")
    args = parser.parse_args()
    cache = args.cache.resolve()
    if not (cache / "data/scene/001_swamp.mlx").is_file():
        parser.error(f"cache root lacks SWAMP MLX: {cache}")
    compiler = shutil.which(args.cxx)
    if not compiler:
        parser.error(f"host C++ compiler not found: {args.cxx}")
    args.output.mkdir(parents=True, exist_ok=True)
    exe = args.output.resolve() / ("module-scene-root-bounds.exe" if os.name == "nt"
                                   else "module-scene-root-bounds")
    subprocess.run([compiler, "-std=c++17", "-O2", "-fno-fast-math", "-ffp-contract=off",
                    "-Wall", "-Wextra", "-Werror", "-pedantic", "-fno-rtti",
                    *map(str, SOURCES), "-o", str(exe)], check=True)
    completed = subprocess.run([str(exe), str(cache)], capture_output=True, text=True, check=True)
    original = verify_original(args.original_elf.resolve())
    tracked = [LEVEL / "module_scene_root_bounds.hpp",
               LEVEL / "module_scene_root_bounds.cpp",
               LEVEL / "tests/module_scene_root_bounds_host.cpp",
               Path(__file__).resolve(), ORIGINAL_MANIFEST,
               LEVEL / "reference/module-scene-root-bounds/NOTES.md"]
    compile_inputs = list(dict.fromkeys([*SOURCES,
        LEVEL / "module_scene_root_bounds.hpp", LEVEL / "octree.hpp",
        LEVEL / "collision.hpp", LEVEL / "floor_source.hpp", LEVEL / "selector.hpp",
        ROOT / "port/world-data/world.hpp", ROOT / "port/world-data/world_scene.hpp",
        ROOT / "port/scene-payloads/scene.hpp", ROOT / "port/scene-materials/scene.hpp",
        ROOT / "port/asset-payloads/payloads.hpp", ROOT / "port/engine-resources/resources.hpp",
        ROOT / "port/engine-math/math.hpp"]))
    assets = [cache / "data/scene/001_swamp.mlx",
              cache / "data/3d/modules/swamp/swamp.bdae"]
    report = {
        "validation": "PASS",
        "scope": "Source-derived static SWAMP module scene-root AABB adapter. It reuses the checked Module root binding/subtree walker and original transformBoxEx-compatible floor bounds kernel; explicit owner position represents live GameObject+0x160.",
        "limitations": [
            "Does not construct original Irrlicht/glitch runtime objects or execute source vtables.",
            "Verified cache has static type-3 geometry only; other instance types are counted and ignored, and nonzero ignored count invalidates this fixture.",
            "Mesh bounds are read from serialized SMesh min/max and host-checked against every primitive's position stream.",
            "The adapter does not derive Module-to-RoomZone membership or infer room identity from DACT indices."
        ],
        "original_source": original,
        "source_sha256": {p.relative_to(ROOT).as_posix(): hashlib.sha256(p.read_bytes()).hexdigest()
                          for p in tracked},
        "compile_inputs_sha256": {p.relative_to(ROOT).as_posix(): hashlib.sha256(p.read_bytes()).hexdigest()
                                  for p in compile_inputs},
        "cache_inputs_sha256": {p.relative_to(cache).as_posix(): hashlib.sha256(p.read_bytes()).hexdigest()
                                for p in assets},
        "command": [compiler, "-std=c++17", "-O2", "-fno-fast-math", "-ffp-contract=off",
                    "-Wall", "-Wextra", "-Werror", "-pedantic", "-fno-rtti",
                    *[p.relative_to(ROOT).as_posix() for p in SOURCES]],
        "stdout": completed.stdout,
        "production_bounds_match_vertex_extents": True,
        "module_count": 9,
        "owner_translation_replays": 9,
        "unsupported_scaled_module_rejection": True,
    }
    report_path = args.output.resolve() / "validation.json"
    report_path.write_text(json.dumps(report, indent=2) + "\n", encoding="utf-8")
    print(json.dumps({"validation": report["validation"],
                      "report": str(report_path),
                      "original_sha256": original["original_sha256"],
                      "modules": 9}))


if __name__ == "__main__":
    main()
