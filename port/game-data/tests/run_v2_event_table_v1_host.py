"""Import the original 66-row v2Event table without modifying its source cache."""
from __future__ import annotations

import argparse
import hashlib
import json
import os
import subprocess
import tempfile
import zipfile
from pathlib import Path

ROOT = Path(__file__).resolve().parents[3]
MODULE = ROOT / "port" / "game-data"
BUILD = ROOT / "tmp" / "v2-event-table-host"
PREFIX = "com.gameloft.android.GAND.GloftD2SS/files/data/pydata/"
PINS = {
    "v2eventmanager_pyarray.bin": (5082, "733812ebab464fd991370474a099f5fe29f0af0ae3fca79a4df22e543683c87e"),
    "v2eventmanager_pyarraynames.bin": (1204, "900e2e0afa4d37497deb8fe995fa883f50e4bb9dae632d9dc28dac8d49e273f2"),
    "v2eventmanager_pycst.bin": (84, "13e1ace862c6a8c0857a42fd0476e2ddf7a002389e004a055fb3363ace60e8c8"),
    "v2eventmanager_pystructnames.bin": (0, "e3b0c44298fc1c149afbf4c8996fb92427ae41e4649b934ca495991b7852b855"),
}


def main() -> None:
    parser = argparse.ArgumentParser()
    parser.add_argument("--cache-zip", required=True, type=Path,
                        help="original immutable Dungeon Hunter 2 cache ZIP")
    parser.add_argument("--build-dir", type=Path, default=BUILD)
    args = parser.parse_args()
    assets: dict[str, bytes] = {}
    with zipfile.ZipFile(args.cache_zip) as source:
        for name, (expected_size, expected_hash) in PINS.items():
            entry = PREFIX + name
            data = source.read(entry)
            digest = hashlib.sha256(data).hexdigest()
            if len(data) != expected_size or digest != expected_hash:
                raise SystemExit(f"source cache mismatch for {name}: {len(data)} bytes, {digest}")
            assets[name] = data

    subprocess.run(["cmake", "--build", str(args.build_dir), "--target",
                    "v2_event_table_v1_audit", "v2_event_update_kernel_v1_audit",
                    "--parallel", "1"],
                   cwd=ROOT, check=True)
    executables = []
    for name in ("v2_event_table_v1_audit", "v2_event_update_kernel_v1_audit"):
        candidates = list(args.build_dir.rglob(name + ".exe")) + list(args.build_dir.rglob(name))
        executable = next((item for item in candidates if item.is_file()), None)
        if executable is None:
            raise SystemExit(f"CMake built no {name} executable")
        executables.append(executable)

    with tempfile.TemporaryDirectory(prefix="dh2-v2event-import-") as temporary:
        temp = Path(temporary)
        for name in PINS:
            if name != "v2eventmanager_pystructnames.bin":
                (temp / name).write_bytes(assets[name])
        env = os.environ.copy()
        env["PATH"] = os.pathsep.join(str(item.parent) for item in executables) + os.pathsep + env.get("PATH", "")
        for executable in executables:
            subprocess.run([str(executable), str(temp)], cwd=ROOT, env=env, check=True)
    print(json.dumps({"validation": "PASS", "source_archive": args.cache_zip.name,
                      "sha256": {name: digest for name, (_, digest) in PINS.items()},
                      "empty_struct_names_confirmed": True}, separators=(",", ":")))


if __name__ == "__main__":
    main()
