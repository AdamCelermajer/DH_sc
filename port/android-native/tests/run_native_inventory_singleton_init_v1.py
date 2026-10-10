"""Run exact production singleton bodies against the real host UI and SWF.

Use an existing native-host-v128 build (Linux/WSL). No emulator, APK or saves.
Only platform delivery and unconsumed non-inventory leaves are fixtures.
"""
import argparse
import hashlib
import os
from pathlib import Path
import shlex
import subprocess

parser = argparse.ArgumentParser()
parser.add_argument("--build", type=Path, required=True)
parser.add_argument("--mutation", action="store_true", help="Also reject the original cold-only details omission")
args = parser.parse_args()
root = Path(__file__).resolve().parents[3]
build = args.build.resolve()
out = build.parent / "inventory-singleton-init-v1"
out.mkdir(exist_ok=True)
source_path = root / "port/android-native/app/src/main/cpp/native_menu_singletons_v67.inc"
source = source_path.read_text()
owners = source[source.index("enum class MenuSingletonKindV67"):source.index("bool source_worldmap_initialize_v68(")]
begin = source.index("bool source_singleton_get_v67(NativeMenuPostMovieV62&,")
end = source.index("\n}\ndh2::ui::AuthoredMenuFieldsV1* model_renderer::native_derived_menu_fields_v67")
bodies = source[begin:end]
libraries = sorted(build.rglob("lib*.so"))
if not any(p.name == "libdh2_engine_ui.so" for p in libraries):
    raise SystemExit("Build the current dh2_engine_ui target in the specified host build first")
search = ":".join(sorted({str(p.parent) for p in libraries}))
env = dict(os.environ, LD_LIBRARY_PATH=search)
compiler_command = subprocess.check_output([
    "ninja", "-C", str(build), "-t", "commands",
    "native/CMakeFiles/dh2_native.dir/native_menu_preview_lifecycle_v123.cpp.o",
], text=True).splitlines()[-1]
compiler = shlex.split(compiler_command)
compiler = compiler[:compiler.index("-MD")]
# Keep the host build's actual include/ABI settings, without its cached PCH.
if "-include" in compiler:
    position = compiler.index("-include")
    del compiler[position:position + 2]
headers = [out, root / "port/engine-ui", root / "port/engine-ui/vendor/gameswf1714", root / "port/android-native/app/src/main/cpp", root / "port/level-loader"]
swf = root / "port/android-native/app/src/main/assets/original-cache/data/menus/dqcharmenu_droid.swf"
if hashlib.sha256(swf.read_bytes()).hexdigest() != "43227075f407626b52eca4c345ac6f568023fcb39c269588201026c0fc6760e0":
    raise SystemExit("The integration fixture must match the audited shipped Droid SWF")


def run(label, extracted):
    (out / "native_inventory_singletons_generated_v1.inc").write_text(owners + extracted)
    executable = out / label
    command = compiler + ["-std=c++17", "-O0", "-g"]
    command += ["-I" + str(p) for p in headers]
    # Compile the current registry/registration/unload leaves into this focused
    # executable. Existing host UI libraries supply the pinned GameSWF backend;
    # their older PostLoad signature must not replace the current source body.
    command += [str(root / "port/android-native/tests/native_inventory_singleton_init_v1.cpp"), str(root / "port/android-native/app/src/main/cpp/source_process_arrays_v101.cpp")]
    command += [str(root / "port/engine-ui" / name) for name in (
        "authored_shared_menu_roster_v27.cpp", "authored_character_menu_session_v1.cpp",
        "authored_menu_weak_character_v59.cpp", "menu_stack_owner_v1.cpp", "menu_manager_unload_v58.cpp",
    )]
    command += ["-o", str(executable), "-Wl,--no-as-needed"] + [str(p) for p in libraries] + ["-Wl,-rpath," + search, "-Wl,-rpath-link," + search]
    compiled = subprocess.run(command, cwd=root, text=True, stdout=subprocess.PIPE, stderr=subprocess.STDOUT, timeout=120)
    (out / (label + "-compile.log")).write_text(compiled.stdout)
    if compiled.returncode:
        print(compiled.stdout[-8000:])
        raise SystemExit(compiled.returncode)
    result = subprocess.run([str(executable), str(swf.parent)], cwd=root, env=env, text=True, stdout=subprocess.PIPE, stderr=subprocess.STDOUT, timeout=30)
    (out / (label + "-run.log")).write_text(result.stdout)
    print(result.stdout, end="")
    return result


result = run("production", bodies)
if result.returncode:
    raise SystemExit(result.returncode)
if args.mutation:
    start = bodies.index(" }else if(owner.kind==MenuSingletonKindV67::inventory){")
    stop = bodies.index(" }else if(owner.kind==MenuSingletonKindV67::world_map", start)
    mutant = bodies[:start] + bodies[stop:]
    cold = """ if(kind==MenuSingletonKindV67::inventory){
  NativeMenuSingletonV67* details{};if(!source_singleton_get_v67(provider,MenuSingletonKindV67::inventory_details,details,e)||
     !source_singleton_init_v67(provider,*details,e)){out->failure=e;return false;}
 }
"""
    mutant = mutant.replace(" out->constructed=true;e.clear();return true;", cold + " out->constructed=true;e.clear();return true;", 1)
    rejected = run("cold-only-omission-mutant", mutant)
    if rejected.returncode == 0 or "Every inventory Init must register its retained details receiver after main" not in rejected.stdout:
        raise SystemExit("The first-campaign integration case failed to detect the original omission")
    print("PASS: first-campaign integration rejects the original cold-only details Init omission")
    (out / "native_inventory_singletons_generated_v1.inc").write_text(owners + bodies)
print("SOURCE_SHA256 " + hashlib.sha256(source_path.read_bytes()).hexdigest())
