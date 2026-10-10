"""Compile/run the focused native host regression against the current host build.

No APK, JVM/UI run, emulator, or save-file operations. Requires the existing
native-host-v128 dh2_native target to have linked successfully first.
"""
import argparse
import os
from pathlib import Path
import shlex
import subprocess

parser = argparse.ArgumentParser()
parser.add_argument("--build", type=Path, required=True)
args = parser.parse_args()
build = args.build.resolve()
root = Path(__file__).resolve().parents[3]
out = build.parent / "lifecycle-v123-focused"
out.mkdir(exist_ok=True)
command = subprocess.check_output([
    "ninja", "-C", str(build), "-t", "commands",
    "native/CMakeFiles/dh2_native.dir/native_menu_preview_lifecycle_v123.cpp.o",
], text=True).splitlines()[-1]
compiler = shlex.split(command)
compiler = compiler[:compiler.index("-MD")]
libraries = sorted(build.rglob("lib*.so"))
if not any(p.name == "libdh2_native.so" for p in libraries):
    raise SystemExit("Build dh2_native in this same host build before running the test")
search = ":".join(sorted({str(p.parent) for p in libraries}))
executable = out / "native-menu-preview-lifecycle-v123"
compile_args = compiler + [
    str(root / "port/android-native/tests/native_menu_preview_lifecycle_v123.cpp"),
    "-o", str(executable), "-Wl,--no-as-needed",
] + [str(p) for p in libraries] + [
    "-Wl,-rpath," + search, "-Wl,-rpath-link," + search,
]
result = subprocess.run(compile_args, cwd=build, text=True,
                        stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
(out / "compile.log").write_text(result.stdout)
if result.returncode:
    print(result.stdout[-8000:])
    raise SystemExit(result.returncode)
env = dict(os.environ, LD_LIBRARY_PATH=search)
run = subprocess.run([str(executable)], cwd=root, env=env, text=True,
                     stdout=subprocess.PIPE, stderr=subprocess.STDOUT, timeout=30)
(out / "run.log").write_text(run.stdout)
print(run.stdout, end="")
raise SystemExit(run.returncode)
