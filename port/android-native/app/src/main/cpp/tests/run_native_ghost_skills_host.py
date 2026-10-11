"""Run the native Ghost SetSkills adapter against real cached pydata rows."""
import argparse
import hashlib
import json
import os
from pathlib import Path
import shutil
import subprocess

ROOT = Path(__file__).resolve().parents[7]
CPP = Path(__file__).resolve().parents[1]
LEVEL = ROOT / "port/level-world"
GAME_DATA = ROOT / "port/game-data"


def sha(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("--compiler", type=Path)
    parser.add_argument("--c-compiler", type=Path)
    parser.add_argument("--cache", type=Path, default=ROOT.parent / "cache/files")
    parser.add_argument("--debug-seed", type=Path,
                        default=ROOT / "port/android-native/app/src/main/assets/data/DebugSwitches.savegame")
    parser.add_argument("--original-elf", type=Path, required=True)
    parser.add_argument("--output", type=Path,
                        default=CPP / "build/native-ghost-skills-host")
    args = parser.parse_args()
    compiler = str(args.compiler or os.environ.get("CXX") or shutil.which("g++") or "")
    if not compiler:
        parser.error("pass --compiler or set CXX")
    sibling_c = Path(compiler).with_name("gcc.exe" if os.name == "nt" else "gcc")
    c_compiler = str(args.c_compiler or os.environ.get("CC") or
                     (sibling_c if sibling_c.is_file() else shutil.which("gcc")) or "")
    if not c_compiler:
        parser.error("pass --c-compiler or set CC (the C constants reader must be compiled as C)")
    output = args.output.resolve()
    output.mkdir(parents=True, exist_ok=True)
    pydata = args.cache.resolve() / "data/pydata"
    table_names = (
        "skills_pyarray.bin", "skills_pyarraynames.bin", "skills_pystructnames.bin",
        "faeries_pyarray.bin", "faeries_pyarraynames.bin", "faeries_pystructnames.bin",
        "character_properties_pyarray.bin", "character_properties_pyarraynames.bin",
        "character_properties_pystructnames.bin", "faeries_pycst.bin",
        "animations_pycst.bin",
    )
    cache_paths = [pydata / name for name in table_names]
    debug_seed = args.debug_seed.resolve()
    paths = cache_paths + [debug_seed]
    if not all(path.is_file() for path in paths):
        missing = [str(path) for path in paths if not path.is_file()]
        raise SystemExit("missing required actual cache input: " + ", ".join(missing))

    sources = [
        CPP / "native_ghost_skills.cpp",
        CPP / "native_ghost_death_v1.cpp",
        CPP / "native_debug_files.cpp",
        CPP / "tests/native_ghost_skills_host.cpp",
        LEVEL / "character_ai_set_skills_and_spells.cpp",
        LEVEL / "character_ai_set_target.cpp",
        LEVEL / "character_faery_selection.cpp",
        LEVEL / "character_ai_skill_script_constructor.cpp",
        LEVEL / "debug_switches_runtime.cpp",
        LEVEL / "debug_switches_persistence.cpp",
        LEVEL / "character_ai_classification.cpp",
        LEVEL / "character_animation_ai.cpp",
        LEVEL / "character_stance.cpp",
        GAME_DATA / "skill_tables.cpp",
        GAME_DATA / "data.cpp",
        GAME_DATA / "properties.cpp",
        GAME_DATA / "class_tables.cpp",
    ]
    executable = output / "native_ghost_skills_host.exe"
    constants_object = output / "pydata_constants.o"
    c_command = [c_compiler, "-std=c11", "-O1", "-Wall", "-Wextra", "-Werror",
                 "-I", str(ROOT / "port/persistence"), "-c",
                 str(ROOT / "port/pydata-constants/constants.c"),
                 "-o", str(constants_object)]
    compiled_c = subprocess.run(c_command, cwd=ROOT, capture_output=True, text=True)
    if compiled_c.returncode:
        raise RuntimeError(compiled_c.stdout + compiled_c.stderr)
    command = [compiler, "-std=c++17", "-O1", "-Wall", "-Wextra", "-Werror", "-pedantic",
               "-I", str(ROOT),
               "-I", str(ROOT / "port/game-data"),
               "-I", str(ROOT / "port/level-world"),
               "-I", str(ROOT / "port/pydata-constants"),
               "-I", str(ROOT / "port/persistence"),
               *map(str, sources), str(constants_object), "-o", str(executable)]
    built = subprocess.run(command, cwd=ROOT, capture_output=True, text=True)
    if built.returncode:
        raise RuntimeError(built.stdout + built.stderr)

    debug_dir = output / "debug-files"
    debug_dir.mkdir(parents=True, exist_ok=True)
    host = subprocess.run([str(executable), *map(str, paths), str(debug_dir)],
                          cwd=ROOT, capture_output=True, text=True)
    if host.returncode:
        raise RuntimeError(host.stdout + host.stderr)
    lines = host.stdout.strip().splitlines()
    result = json.loads(lines[-1])
    if result.get("validation") != "PASS":
        raise AssertionError("native adapter host report did not pass")

    source_runner = LEVEL / "tests/run_character_ai_set_skills_and_spells_host.py"
    source_output = output / "source-setskills-arm"
    source_gate = subprocess.run([
        os.environ.get("PYTHON", "python"), str(source_runner),
        "--compiler", compiler,
        "--original-elf", str(args.original_elf.resolve()),
        "--output", str(source_output / "host.exe"),
        "--report", str(source_report_path := source_output / "validation.json"),
    ], cwd=ROOT, capture_output=True, text=True)
    if source_gate.returncode:
        raise RuntimeError(source_gate.stdout + source_gate.stderr)
    source_report = json.loads(source_report_path.read_text(encoding="utf-8"))
    source_arm = source_report.get("original_arm_comparison", {})
    if (source_report.get("validation") != "PASS" or
            source_arm.get("validation") != "PASS" or source_arm.get("mismatches") != 0):
        raise AssertionError("SetSkills source-kernel ARM gate did not pass")

    inputs = sources + [Path(__file__).resolve(), source_runner,
                        CPP / "native_ghost_skills.hpp",
                        CPP / "native_ghost_death_v1.hpp",
                        CPP / "native_debug_files.hpp",
                        LEVEL / "character_ai_classification.hpp",
                        LEVEL / "character_stance.hpp",
                        LEVEL / "character_ai_set_skills_and_spells.hpp",
                        LEVEL / "character_ai_set_target.hpp",
                        LEVEL / "character_faery_selection.hpp",
                        GAME_DATA / "animation_tables.hpp",
                        GAME_DATA / "skill_tables.hpp",
                        GAME_DATA / "properties.hpp",
                        ROOT / "port/pydata-constants/constants.c"]
    report = {
        "validation": "PASS",
        "host_runtime_cases": 4,
        "death_provider_source_operations_per_actor": 3,
        "actors": ["Crypt_Ghost", "Crypt_Ghost_RE"],
        "source_kernel_host_cases": source_report.get("host_report", {}).get("host_cases"),
        "source_kernel_original_arm_cases": source_arm.get("cases"),
        "source_kernel_arm_gate": str(source_report_path.relative_to(ROOT)),
        "source_kernel_arm_mismatches": source_arm.get("mismatches"),
        "scope": "Real-cache Ghost adapter and player_ai_death backend-operation checks plus source-kernel host/ARM gates; full death orchestration and Android renderer/device checks remain outside this runner.",
        "cache_inputs": {path.name: sha(path) for path in paths},
        "source_hashes": {str(path.relative_to(ROOT)): sha(path) for path in inputs},
        "host_stdout": lines,
        "host_command": command,
        "c_command": c_command,
        "host_executable": str(executable),
    }
    report_path = output / "validation.json"
    report_path.write_text(json.dumps(report, indent=2) + "\n", encoding="utf-8")
    print(json.dumps(report, indent=2))


if __name__ == "__main__":
    main()
