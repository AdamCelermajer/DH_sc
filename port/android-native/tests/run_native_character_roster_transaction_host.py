"""Build and run the Character roster transaction host checks."""
from __future__ import annotations

import argparse
import os
from pathlib import Path
import shutil
import subprocess
import sys

TESTS = Path(__file__).resolve().parent
CPP = TESTS.parent / "app" / "src" / "main" / "cpp"
REPOSITORY = TESTS.parents[2]


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--compiler", help="C++ compiler executable")
    args = parser.parse_args()
    compiler = args.compiler or os.environ.get("CXX") or shutil.which("g++") or shutil.which("clang++")
    if not compiler:
        raise SystemExit("no host C++ compiler found; pass --compiler")
    output = TESTS / "build" / "native_character_roster_transaction_host"
    if os.name == "nt":
        output = output.with_suffix(".exe")
    output.parent.mkdir(parents=True, exist_ok=True)
    level = REPOSITORY / "port" / "level-world"
    command = [
        compiler, "-std=c++17", "-O1", "-Wall", "-Wextra", "-Werror",
        str(TESTS / "native_character_roster_transaction_host.cpp"),
        str(CPP / "native_character_roster_transaction.cpp"),
        str(CPP / "native_character_list.cpp"),
        str(level / "character_ai_initialization.cpp"),
        str(level / "character_ai_association.cpp"),
        str(level / "object_manager_runtime_owner_v1.cpp"),
        str(level / "character_aggro_object_manager_list.cpp"),
        str(level / "game_object_zoning_visibility.cpp"),
        str(level / "room_zone_enrollment.cpp"),
        "-o", str(output),
    ]
    compiler_dir = str(Path(shutil.which(compiler) or compiler).resolve().parent)
    child_env = os.environ.copy()
    child_env["PATH"] = compiler_dir + os.pathsep + child_env.get("PATH", "")
    built = subprocess.run(command, cwd=REPOSITORY, text=True,
                           capture_output=True, env=child_env)
    if built.stdout:
        sys.stdout.write(built.stdout)
    if built.stderr:
        sys.stderr.write(built.stderr)
    if built.returncode:
        return built.returncode
    result = subprocess.run([str(output)], cwd=REPOSITORY, text=True,
                            capture_output=True, env=child_env)
    if result.stdout:
        sys.stdout.write(result.stdout)
    if result.stderr:
        sys.stderr.write(result.stderr)
    if result.returncode:
        return result.returncode
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
