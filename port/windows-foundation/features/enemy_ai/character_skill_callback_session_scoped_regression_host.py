"""Build and run the scoped Session callback regression against real Lua."""
import hashlib
import json
import shlex
import subprocess
from pathlib import Path

ROOT = Path(__file__).resolve().parents[4]
FEATURE = ROOT / "port/windows-foundation/features/enemy_ai"


def linux(path):
    path = Path(path).resolve()
    return "/mnt/" + path.drive[0].lower() + str(path)[2:].replace("\\", "/")


def run(command):
    proc = subprocess.run(["wsl", "-e", "bash", "-lc", command], text=True,
                          capture_output=True, encoding="utf-8", errors="replace")
    if proc.returncode:
        raise RuntimeError(command + "\n" + proc.stdout + proc.stderr)
    return proc.stdout.strip(), proc.stderr.strip()


def sha(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()


def main():
    build = "/home/adampalace/dh2-sc-scoped-callback-audit"
    runtime = ROOT / "port/script-runtime"
    # Keep the runtime and gameplay links private to this test. The root
    # snapshots are inputs only; they are never rebuilt or overwritten.
    command_lines = []

    def checked(command):
        command_lines.append(command)
        out, err = run(command)
        return out

    checked("mkdir -p " + shlex.quote(build + "/runtime"))
    lua_units = "lapi lcode ldebug ldo ldump lfunc lgc llex lmem lobject lopcodes lparser lstate lstring ltable ltm lundump lvm lzio lauxlib lbaselib ltablib lstrlib lmathlib".split()
    c_units = ["script_runtime_return_v3.c", "script_game_bindings.c", "script_scalar_bindings.c",
               "script_design_bindings.c", "script_object_bridge.c"] + ["lua/" + unit + ".c" for unit in lua_units]
    cpp_units = ["script_function_alias.cpp", "script_constants.cpp", "script_int_bindings.cpp"]
    common = ["-O1", "-g", "-fPIC", "-fno-fast-math", "-ffp-contract=off", "-Wall", "-Wextra", "-Wno-misleading-indentation"]
    objects = []
    for index, unit in enumerate(c_units + cpp_units):
        obj = f"{build}/runtime/{index}.o"
        compiler = "g++" if unit in cpp_units else "gcc"
        standard = ["-std=c++17"] if compiler == "g++" else ["-std=c99"]
        checked(shlex.join([compiler] + standard + common + ["-I" + linux(runtime / "lua"), "-c", linux(runtime / unit), "-o", obj]))
        objects.append(obj)
    checked(shlex.join(["g++"] + common + ["-shared"] + objects + ["-lm", "-o", build + "/runtime/libdh2_script_runtime.so"]))

    test = FEATURE / "character_skill_callback_session_scoped_regression.cpp"
    exe = build + "/scoped_callback_regression"
    test_flags = ["-std=c++17", "-Wall", "-Wextra", "-Werror", "-Wno-misleading-indentation",
                  "-ffunction-sections", "-fdata-sections", "-Wl,--gc-sections"]
    checked(shlex.join(["g++"] + test_flags + [linux(test), linux(ROOT / "port/level-world/character_skill_callbacks_v3.cpp"),
        "-L" + build + "/runtime", "-ldh2_script_runtime", "-Wl,-rpath," + build + "/runtime", "-o", exe]))
    output = checked("LD_LIBRARY_PATH=" + shlex.quote(build + ":" + build + "/runtime") + " " + shlex.quote(exe))
    tracked = [test, FEATURE / "character_skill_callback_session_scoped_regression_host.py",
               ROOT / "port/level-world/character_skill_callback_session_v3.cpp"]
    report = {"validation": "PASS", "result": json.loads(output), "source_sha256": {str(p.relative_to(ROOT)).replace("\\", "/"): sha(p) for p in tracked},
              "private_build": build, "commands": command_lines,
              "scope": "Real Lua VM nested through the common Session Delivery path. Normal unscoped Pre/Use/Post/Check calls preserve source order and selected return conversion; same-VM scoped nested calls execute; stale and live foreign-VM capabilities fail before unscoped fallback. Test uses a narrow fake Session owner/view to drive the common Delivery template, not a full CharacterScriptSession/actor lifecycle claim."}
    output_path = FEATURE / "character_skill_callback_session_scoped_regression_report.json"
    output_path.write_text(json.dumps(report, indent=2) + "\n", encoding="utf-8")
    print(json.dumps({"validation": "PASS", "result": report["result"], "report": str(output_path), "report_sha256": sha(output_path)}))


if __name__ == "__main__":
    main()
