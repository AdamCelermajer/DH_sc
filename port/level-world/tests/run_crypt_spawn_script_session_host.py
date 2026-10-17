#!/usr/bin/env python3
"""Execute the original Crypt GhostAmbush01 tables through native source services."""
import argparse
import hashlib
import json
from pathlib import Path
import shutil
import subprocess
import xml.etree.ElementTree as ET

HERE = Path(__file__).resolve().parent
MODULE = HERE.parent
REPO = MODULE.parents[1]


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--cache", type=Path, required=True)
    parser.add_argument("--compiler", default="g++")
    parser.add_argument("--report", type=Path, default=MODULE / "build/crypt-spawn-script-session/validation.json")
    args = parser.parse_args()
    cache = args.cache.resolve()
    relative = ["data/pydata/scripts_pyscriptnames.bin", "data/pydata/scripts_pyscripts.bin",
                "data/pydata/scripts/007_crypt_01_pyscriptnames.bin",
                "data/pydata/scripts/007_crypt_01_pyscripts.bin"]
    paths = [cache / name for name in relative]
    mgp = cache / "data/3d/modules/crypt/mgp/crypt_straight_c_ns_01.mgp"
    objects = {obj.attrib["name"]: obj.attrib for obj in ET.parse(mgp).getroot().findall("GameObject")}
    trigger = objects["_prim_TriggerZone_GhostAmbush01"]
    assert (trigger["gametype"], trigger["script"], trigger["triggercount"], trigger["triggerdelay"]) == (
        "TriggerZone", "GhostAmbush01", "1", "0")
    for name in ("_prim_Monster_SURPRISE_01", "_prim_Monster_SURPRISE_02"):
        actor = objects[name]
        assert (actor["gametype"], actor["ai_state"], actor["auto_spawn"], actor["charpropsname"]) == (
            "Character", "Limbus", "0", "Crypt_Ghost")
    compiler = shutil.which(args.compiler)
    if not compiler:
        raise RuntimeError(f"C++ compiler missing: {args.compiler}")
    report = args.report.resolve()
    report.parent.mkdir(parents=True, exist_ok=True)
    executable = report.parent / "crypt-spawn-script-session-host.exe"
    sources = [HERE / "crypt_spawn_script_session.cpp", MODULE / "crypt_spawn_script_session.cpp",
               MODULE / "character_factory.cpp", MODULE / "character_state.cpp",
               MODULE / "object_manager_runtime_owner_v1.cpp",
               MODULE / "character_skill_fsm_callbacks_v1.cpp",
               MODULE / "character_skill_state_dispatch_v1.cpp",
               MODULE / "character_cast_lifecycle_v1.cpp",
               MODULE / "crypt_spawn_trigger.cpp",
               REPO / "port/trigger-contact/trigger_contact.cpp",
               REPO / "port/zone-contact-runtime/zone_geometry.cpp",
               REPO / "port/script-runtime/script_runtime.cpp",
               REPO / "port/pydata-scripts/native/pydata_scripts.cpp"]
    command = [compiler, "-std=c++17", "-Wall", "-Wextra", "-Werror", "-pedantic",
               *map(str, sources), "-o", str(executable)]
    subprocess.run(command, check=True)
    tested = subprocess.run([str(executable), *map(str, paths)], check=True, capture_output=True, text=True)
    value = {"status": "passed", "scope": "source script execution and synchronous Character state/body-service projection; authored TriggerZone contact integration tested separately",
             "program": "GhostAmbush01", "local_id": 2, "global_id": 17,
             "zero_delta_followup_dispatch_fixture_ms": [250, 325],
             "fixed_25_ms_dispatch_fixture_ms": [275, 350],
             "wait_order": "IsBlocking checks prior elapsed; blocked command Update(delta) then returns without same-pass recheck; a newly executed Wait receives current delta",
             "inputs": {name: {"bytes": path.stat().st_size, "sha256": hashlib.sha256(path.read_bytes()).hexdigest()}
                        for name, path in zip(relative + [str(mgp.relative_to(cache)).replace('\\', '/')], paths + [mgp])},
             "trigger_record": trigger, "compiler_command": command, "test_output": tested.stdout.strip()}
    report.write_text(json.dumps(value, indent=2) + "\n", encoding="utf-8")
    print(tested.stdout.strip())
    print(f"report: {report}")


if __name__ == "__main__":
    main()
