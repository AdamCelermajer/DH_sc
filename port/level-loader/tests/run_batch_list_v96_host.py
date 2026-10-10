"""Build/run the production Stage22 list regression on the host; no device use."""
from pathlib import Path
import os
import subprocess

root = Path(__file__).resolve().parents[3]
output = root / ".local-inputs" / "stage22-batch-list-v96-host"
output.mkdir(exist_ok=True)

def host_path(path):
    path = path.resolve()
    if os.name == "nt":
        return "/mnt/" + path.drive[0].lower() + path.as_posix()[2:]
    return str(path)

sources = [
    root / "port/level-loader/tests/batch_list_v96.cpp",
    root / "port/level-loader/level_batching_source_v96.cpp",
    root / "port/level-world/canonical_object_manager_v1.cpp",
    root / "port/level-world/object_manager_language_registry_v1.cpp",
]
executable = output / "batch-list-v96"
command = ["g++", "-std=c++17", "-O1", "-g", "-Wall", "-Wextra",
           "-Werror", "-Wno-misleading-indentation", "-Wno-missing-field-initializers",
           "-ffunction-sections", "-fdata-sections", "-fsanitize=address,undefined"]
command += ["-I" + host_path(p) for p in sorted((root / "port").iterdir()) if p.is_dir()]
command += [host_path(p) for p in sources]
command += ["-Wl,--gc-sections", "-o", host_path(executable)]
prefix = ["wsl.exe", "--exec"] if os.name == "nt" else []
build = subprocess.run(prefix + command, text=True, stdout=subprocess.PIPE,
                       stderr=subprocess.STDOUT, timeout=90)
(output / "compile.log").write_text(build.stdout)
if build.returncode:
    print(build.stdout)
    raise SystemExit(build.returncode)
run = subprocess.run(prefix + [host_path(executable)], text=True,
                     stdout=subprocess.PIPE, stderr=subprocess.STDOUT, timeout=30)
(output / "run.log").write_text(run.stdout)
print(run.stdout, end="")
raise SystemExit(run.returncode)
