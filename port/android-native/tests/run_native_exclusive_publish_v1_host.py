"""Build and run the production exclusive-publish helper on the host."""
import argparse
import subprocess
import tempfile
from pathlib import Path

ROOT = Path(__file__).resolve().parents[3]


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--compiler", type=Path, required=True)
    args = parser.parse_args()
    with tempfile.TemporaryDirectory(prefix="dh2-exclusive-publish-") as temp:
        temp = Path(temp)
        executable = temp / "exclusive-publish-host.exe"
        subprocess.run(
            [str(args.compiler.resolve()), "-std=c++17", "-Wall", "-Wextra",
             "-Werror", "-O2",
             str(ROOT / "port/android-native/tests/native_exclusive_publish_v1_host.cpp"),
             "-o", str(executable)],
            cwd=ROOT, check=True)
        subprocess.run([str(executable), str(temp / "case")], cwd=ROOT, check=True)


if __name__ == "__main__":
    main()
