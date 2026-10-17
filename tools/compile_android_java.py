#!/usr/bin/env python3
"""Compile reconstructed Android Java; this does not build or validate a game APK."""
import argparse
import json
import os
from pathlib import Path
import subprocess
import tempfile

parser = argparse.ArgumentParser(description=__doc__)
parser.add_argument("--android-platform", required=True, type=Path,
                    help="Android SDK platform directory, e.g. platforms/android-35")
parser.add_argument("--javac", default="javac")
parser.add_argument("--output", required=True, type=Path,
                    help="Compiled class output directory outside the source repository")
parser.add_argument("--max-errors", type=int, default=100)
parser.add_argument("--source-dir", type=Path,
                    default=Path(__file__).resolve().parents[1] / "port/android-java/java")
parser.add_argument("--log", type=Path)
args = parser.parse_args()
platform = args.android_platform.resolve()
sources = sorted(args.source_dir.resolve().rglob("*.java"))
if not sources:
    parser.error("No Java sources found")
jars = [platform / "android.jar", platform / "optional/org.apache.http.legacy.jar"]
for jar in jars:
    if not jar.is_file():
        parser.error(f"Required SDK library missing: {jar}")
args.output.mkdir(parents=True, exist_ok=True)
with tempfile.TemporaryDirectory(prefix="dh2-javac-") as temporary:
    arg_file = Path(temporary) / "sources.args"
    # javac treats backslashes in argument files as escapes on Windows.
    arg_file.write_text("\n".join('"' + path.as_posix() + '"' for path in sources) + "\n")
    result = subprocess.run([
        args.javac, "--release", "17", "-encoding", "UTF-8", "-Xmaxerrs", str(args.max_errors),
        "-classpath", os.pathsep.join(str(path) for path in jars),
        "-d", str(args.output.resolve()), "@" + str(arg_file),
    ], capture_output=True, text=True)
log = result.stdout + result.stderr
if args.log:
    args.log.parent.mkdir(parents=True, exist_ok=True)
    args.log.write_text(log)
print(log, end="")
print(json.dumps({"source_files": len(sources), "exit_code": result.returncode,
                  "compiled_class_files": len(list(args.output.rglob("*.class")))}, indent=2))
raise SystemExit(result.returncode)
