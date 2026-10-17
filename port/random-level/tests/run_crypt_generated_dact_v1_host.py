#!/usr/bin/env python3
"""Host compile/audit for generated Crypt MGP to DACT source projection."""
import argparse
import os
from pathlib import Path
import subprocess
import tempfile


TESTS = Path(__file__).resolve().parent
PORT = TESTS.parents[1]
REPO = PORT.parent
DEFAULT_ASSETS = REPO / "port/android-native/app/src/main/assets"
DEFAULT_CACHE = Path(
    r"C:\Users\noamc\Documents\Codex\2026-10-02\ex\outputs\DH2_Remaster\original_assets\com.gameloft.android.GAND.GloftD2SS\files"
)

def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("--cache-root", type=Path, default=DEFAULT_CACHE,
                        help="decompressed original game cache root")
    parser.add_argument("--assets-root", type=Path, default=DEFAULT_ASSETS,
                        help="native app assets root containing Crypt layout/DACT/tables")
    parser.add_argument("--cxx", default=os.environ.get("CXX", "g++"),
                        help="C++17 host compiler")
    args = parser.parse_args()
    if not (args.cache_root / "data/3d/modules/crypt/mgp/crypt_entrance_s_01.mgp").is_file():
        raise SystemExit(f"missing original Crypt MGP cache under {args.cache_root}")
    if not (args.assets_root / "worlds/crypt01.dact").is_file():
        raise SystemExit(f"missing fixed Crypt DACT under {args.assets_root}")

    with tempfile.TemporaryDirectory(prefix="dh2-crypt-dact-") as temporary:
        executable = Path(temporary) / ("crypt_generated_dact.exe" if os.name == "nt"
                                         else "crypt_generated_dact")
        sources = [
            PORT / "world-data/world.cpp",
            PORT / "game-data/data.cpp",
        PORT / "game-data/condition_data_v1.cpp",
        PORT / "game-data/quest_condition_eval_v1.cpp",
        PORT / "game-data/quest_condition_factory_v1.cpp",
        PORT / "game-data/quest_table_bindings_v1.cpp",
        PORT / "quest-data/quests.c",
            PORT / "random-level/crypt_generated_dact_v1.cpp",
            TESTS / "crypt_generated_dact_v1.cpp",
        ]
        command = [args.cxx, "-std=c++17", "-O2", "-Wall", "-Wextra", "-Werror",
                   *(str(path) for path in sources), "-o", str(executable)]
        subprocess.run(command, check=True)
        subprocess.run([str(executable), str(args.cache_root.resolve()),
                        str(args.assets_root.resolve())], check=True)


if __name__ == "__main__":
    main()
