"""Build and run the single-owner saved-skill host replay against PGS1 gold."""
import argparse
import hashlib
import json
from pathlib import Path
import shutil
import subprocess

TESTS = Path(__file__).resolve().parent
MODULE = TESTS.parent
ROOT = MODULE.parents[1]
REFERENCE = MODULE / "reference/player-savegame-v1"


def sha(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("--cache", type=Path,
                        default=ROOT / ".local-inputs/skill-tables")
    parser.add_argument("--gold", type=Path, default=REFERENCE / "fixtures.bin")
    parser.add_argument("--output-dir", type=Path,
                        default=MODULE / "build/player-savegame-v1-host")
    parser.add_argument("--compiler", default=shutil.which("g++") or "g++")
    parser.add_argument("--report", type=Path)
    args = parser.parse_args()
    cache = args.cache.resolve()
    gold = args.gold.resolve()
    output_dir = args.output_dir.resolve()
    if not cache.is_dir():
        raise NotADirectoryError(cache)
    for name in ("skills_pyarray.bin", "skills_pyarraynames.bin",
                 "skills_pystructnames.bin"):
        if not (cache / name).is_file():
            raise FileNotFoundError(cache / name)
    if not gold.is_file():
        raise FileNotFoundError(gold)

    compiler = shutil.which(args.compiler) or args.compiler
    output_dir.mkdir(parents=True, exist_ok=True)
    executable = output_dir / "player_savegame_v1_host.exe"
    sources = [TESTS / "player_savegame_v1.cpp",
               MODULE / "player_savegame_v1.cpp",
               MODULE / "skill_tables.cpp"]
    command = [compiler, "-std=c++17", "-O1", "-Wall", "-Wextra", "-Werror",
               "-fno-fast-math", f"-I{MODULE}", *(str(p) for p in sources),
               "-o", str(executable)]
    subprocess.run(command, cwd=ROOT, check=True)
    completed = subprocess.run([str(executable), str(gold), str(cache)],
                               cwd=ROOT, check=True, capture_output=True, text=True)
    result = json.loads(completed.stdout)
    report_path = (args.report.resolve() if args.report else
                   output_dir / "validation.json")
    report_path.parent.mkdir(parents=True, exist_ok=True)
    inputs = [*sources, *[cache / name for name in
             ("skills_pyarray.bin", "skills_pyarraynames.bin",
              "skills_pystructnames.bin")], gold]
    report = {
        **result,
        "compile_command": command,
        "compiler": subprocess.run([compiler, "--version"], check=True,
                                   capture_output=True, text=True).stdout.splitlines()[0],
        "input_sha256": {str(path): sha(path) for path in inputs},
        "executable_sha256": sha(executable),
        "source_scope": "host-owned save fields and source PGS1 saved-skill operations; no profile lifecycle or starter skill grant",
    }
    report_path.write_text(json.dumps(report, indent=2) + "\n", encoding="utf-8")
    print(json.dumps(report))


if __name__ == "__main__":
    main()
