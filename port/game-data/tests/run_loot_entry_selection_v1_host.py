"""Build the selected game-data DSO and run loot-entry host cases."""
import argparse
import hashlib
import json
import os
from pathlib import Path
import subprocess

ROOT = Path(__file__).resolve().parents[3]


def digest(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--build", type=Path, required=True)
    parser.add_argument("--compiler", type=Path, required=True,
                        help="host C++ compiler; matching gcc.exe is used for C")
    parser.add_argument("--report", type=Path, required=True)
    args = parser.parse_args()
    args.build.mkdir(parents=True, exist_ok=True)
    env = os.environ.copy()
    env["PATH"] = str(args.compiler.parent) + os.pathsep + env.get("PATH", "")
    log = []

    def run(command):
        result = subprocess.run([str(x) for x in command], cwd=ROOT, env=env,
                                capture_output=True, text=True)
        log.append({"command": [str(x) for x in command],
                    "returncode": result.returncode,
                    "stdout": result.stdout, "stderr": result.stderr})
        if result.returncode:
            raise RuntimeError(result.stderr or result.stdout)
        return result.stdout

    wrapper = ROOT / "port/game-data/tests/item-loot-backbone-v7"
    run(["cmake", "-S", wrapper, "-B", args.build, "-G", "Ninja",
         "-DCMAKE_BUILD_TYPE=RelWithDebInfo", "-DCMAKE_EXPORT_COMPILE_COMMANDS=ON",
         "-DCMAKE_CXX_COMPILER=" + str(args.compiler),
         "-DCMAKE_C_COMPILER=" + str(args.compiler.with_name("gcc.exe"))])
    database = json.loads((args.build / "compile_commands.json").read_text())
    selected = [row for row in database
                if "CMakeFiles/dh2_game_data.dir" in row["command"].replace("\\", "/")]
    matches = [row for row in selected
               if Path(row["file"]).name == "loot_entry_selection_v1.cpp"]
    if len(matches) != 1:
        raise AssertionError("loot-entry module is not selected exactly once")
    run(["cmake", "--build", args.build, "--parallel", "1", "--target",
         "loot_entry_selection_v1_selected"])

    suffix = ".exe" if os.name == "nt" else ""
    library = (args.build / "game-data" /
               ("libdh2_game_data.dll" if os.name == "nt" else "libdh2_game_data.so"))
    binary = args.build / ("loot_entry_selection_v1_selected" + suffix)
    env["PATH"] = str(library.parent) + os.pathsep + env["PATH"]
    result = json.loads(run([binary]))
    if result.get("validation") != "PASS":
        raise AssertionError("loot-entry selected host check failed")
    report = {
        "validation": "PASS",
        "selected_source": str(matches[0]["file"]),
        "selected_source_sha256": digest(Path(matches[0]["file"])),
        "selected_library": str(library),
        "selected_library_sha256": digest(library),
        "selected_host_test": result,
        "commands": log,
    }
    args.report.parent.mkdir(parents=True, exist_ok=True)
    args.report.write_text(json.dumps(report, indent=2) + "\n")
    print(json.dumps({key: value for key, value in report.items() if key != "commands"}))


if __name__ == "__main__":
    main()
