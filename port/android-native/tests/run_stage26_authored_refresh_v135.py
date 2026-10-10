"""Host regression of the production stage26 refresh on unmodified packaged SWFs.

Compile the actual OriginalUiSession leaf and LifecycleV36, borrowing a copied
host UI library for the real SWF tree. No Android build or emulator is used.
"""
import argparse
import hashlib
import json
from pathlib import Path
import shutil
import subprocess
import zlib

ROOT = Path(__file__).resolve().parents[3]
CPP = ROOT / "port/android-native/app/src/main/cpp"
UI = ROOT / "port/engine-ui"


def sha(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()


def linux(path):
    absolute = path.resolve().as_posix()
    if len(absolute) < 3 or absolute[1:3] != ":/":
        raise ValueError("Expected Windows-host path for WSL")
    return "/mnt/" + absolute[0].lower() + absolute[2:]


def extract_function(source, signature):
    start = source.index(signature)
    opening = source.index("{", start)
    depth, cursor = 1, opening + 1
    while depth:
        depth += (source[cursor] == "{") - (source[cursor] == "}")
        cursor += 1
    return source[start:cursor]


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--output", type=Path, required=True)
    parser.add_argument("--ui-library", type=Path, default=ROOT /
        ".local-inputs/native-host-v128/build/native/level-world/engine-ui/libdh2_engine_ui.so")
    args = parser.parse_args()
    output = args.output.resolve()
    if not output.is_relative_to(ROOT / ".local-inputs") or output.exists():
        raise ValueError("Use a new private .local-inputs output directory")
    output.mkdir(parents=True)
    signature = "bool OriginalUiSession::complete_refresh_stage26_v66(std::string& error)"
    leaf_source = CPP / "original_ui_session.cpp"
    function = extract_function(leaf_source.read_text(encoding="utf-8"), signature)
    (output / "production_refresh.inc").write_text(function + "\n", encoding="utf-8")
    assets = ROOT / "port/android-native/app/src/main/assets/original-cache/data/menus"
    expected = {"dqhud_droid.swf": "a4ffacd1abdf7c9b2ba19c46ebb81c60c100458731a4cdba5880391b9c11b238",
                "dqshared_droid.swf": "f0f9f119f2fe474d21c722e361ef9907ef2b885654705073e0cb956432f46263"}
    for name, digest in expected.items():
        path = assets / name
        if sha(path) != digest:
            raise ValueError("Authentic packaged SWF digest changed: " + name)
        raw = path.read_bytes()
        decoded = raw[:8] + zlib.decompress(raw[8:]) if raw[:3] == b"CWS" else raw
        if b"completeRefresh" in decoded:
            raise ValueError("Authored fixture unexpectedly includes completeRefresh")
    snapshot = output / "ui"
    snapshot.mkdir()
    ui_library = args.ui_library.resolve()
    library_hash = sha(ui_library)
    shutil.copyfile(ui_library, snapshot / ui_library.name)
    test = Path(__file__).with_name("stage26_authored_refresh_v135.cpp")
    lifecycle = ROOT / "port/level-loader/lifecycle_v36.cpp"
    frozen = {str(path): sha(path) for path in (test, lifecycle, leaf_source)}
    commands = []

    def run(parts):
        command = ["wsl", "-e", *parts]
        commands.append(command)
        result = subprocess.run(command, capture_output=True, text=True, encoding="utf-8")
        if result.returncode:
            failure = {"command": command, "returncode": result.returncode,
                       "stdout": result.stdout, "stderr": result.stderr}
            (output / "failure.json").write_text(json.dumps(failure, indent=2) + "\n", encoding="utf-8")
            raise RuntimeError(json.dumps(failure))
        return result.stdout, result.stderr

    binary = output / "stage26_authored_refresh"
    command = ["c++", "-std=c++17", "-O1", "-g", "-fno-fast-math", "-ffp-contract=off",
               "-fpermissive", "-w", "-DTU_CONFIG_LINK_TO_JPEGLIB=0", "-DTU_CONFIG_LINK_TO_LIBPNG=0",
               "-DTU_CONFIG_LINK_TO_FREETYPE=0", "-DTU_CONFIG_LINK_TO_THREAD=0",
               "-I" + linux(UI), "-I" + linux(UI / "vendor/gameswf1714"), "-I" + linux(output),
               linux(test), linux(lifecycle), "-L" + linux(snapshot), "-ldh2_engine_ui",
               "-Wl,-rpath," + linux(snapshot), "-o", linux(binary)]
    stdout, stderr = run(command)
    (output / "build.log").write_text(stdout + stderr, encoding="utf-8")
    stdout, stderr = run([linux(binary), linux(assets)])
    (output / "run.log").write_text(stdout + stderr, encoding="utf-8")
    audit = json.loads(stdout)
    if audit["validation"] != "PASS" or audit["states"] != [26, 27, 28] or audit["progress"] != [68, 71, 73]:
        raise ValueError("Stage26/27 authored execution differed")
    if library_hash != sha(ui_library) or any(sha(Path(path)) != digest for path, digest in frozen.items()):
        raise ValueError("Borrowed production inputs changed during the host regression")
    receipt = {"validation": "PASS", "authored_swf_sha256": expected,
               "production_function_sha256": sha(output / "production_refresh.inc"),
               "source_sha256": {str(Path(path).relative_to(ROOT)): digest for path, digest in frozen.items()},
               "borrowed_ui_sha256": library_hash, "host_audit": audit, "commands": commands,
               "binary_sha256": sha(binary), "packaged_apk": False, "emulator_launched": False}
    (output / "receipt.json").write_text(json.dumps(receipt, indent=2) + "\n", encoding="utf-8")
    print(json.dumps({"validation": "PASS", "states": audit["states"], "progress": audit["progress"],
                      "receipt": str(output / "receipt.json")}))


if __name__ == "__main__":
    main()
