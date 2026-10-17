"""Compile and run the focused original ItemAudioVisual cache parser gate."""
import argparse
import json
import os
from pathlib import Path
import shutil
import subprocess

ROOT = Path(__file__).resolve().parents[3]


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--cache", type=Path,
                        default=ROOT.parent / "cache/files/data/pydata",
                        help="directory containing the three original loot_audiovisual files")
    parser.add_argument("--build", type=Path,
                        default=ROOT / "tmp/item-audiovisual-v1-host")
    parser.add_argument("--compiler", default=os.environ.get("CXX") or shutil.which("g++")
                        or r"C:\Users\noamc\.local\mingw\mingw64\bin\g++.exe")
    args = parser.parse_args()
    args.build.mkdir(parents=True, exist_ok=True)
    executable = args.build / "item_audiovisual_table_v1_host.exe"
    command = [args.compiler, "-std=c++17", "-O2", "-Wall", "-Wextra", "-Werror",
               "-I", str(ROOT / "port/game-data"),
               str(ROOT / "port/game-data/item_audiovisual_table_v1.cpp"),
               str(ROOT / "port/game-data/tests/item_audiovisual_table_v1.cpp"),
               "-o", str(executable)]
    subprocess.run(command, cwd=ROOT, check=True)
    result = subprocess.run([str(executable), str(args.cache.resolve())],
                            cwd=ROOT, check=True, capture_output=True, text=True)
    audit = json.loads(result.stdout)
    if audit.get("validation") != "PASS" or audit.get("rows") != 29:
        raise RuntimeError("ItemAudioVisual parser host gate did not pass")
    print(json.dumps({"validation": "PASS", "audit": audit,
                      "compiler": args.compiler, "host_compilation": True,
                      "android_compilation": False, "live_gameplay": False}))


if __name__ == "__main__":
    main()
