"""Build/run the actual-native-Gear projection test against the current Windows COFF closure."""
import hashlib
import json
import os
import subprocess
from pathlib import Path

ROOT = Path(__file__).resolve().parents[4]
OWN = ROOT / "port/windows-foundation/features/inventory"
STAGE = OWN / "source_inventory_projection_native_build"
TOOLCHAIN = ROOT / ".local-inputs/windows-toolchain/llvm-mingw-20261006-ucrt-x86_64/bin"
COMPILER = TOOLCHAIN / "clang++.exe"
ARCHIVER = TOOLCHAIN / "llvm-ar.exe"
NATIVE = ROOT / "port/windows-foundation/features/actor_frame/source_character_owner_factory_native_build/native.a"
FOUNDATION = ROOT / ".local-inputs/windows-foundation-build"
SHARED = [FOUNDATION / "libfoundation_data.a",
          FOUNDATION / "librecovered_content.a",
          FOUNDATION / "libcontent_xml.a"]
EXE = STAGE / "source_inventory_projection_v1_native_test.exe"
SOURCES = [
    OWN / "source_inventory_projection_v1.cpp",
    OWN / "source_instance_resolver.cpp",
    OWN / "source_inventory_projection_v1_native_tests.cpp",
]
INPUTS = [
    "port/level-world/reference/character-game-design/real-cache-inputs.bin",
    ".local-inputs/items-discovery",
    ".local-inputs/player-item-effects-v5/power-cache",
    "port/android-native/app/src/main/assets",
    ".local-inputs/player-item-effects-v5/private-save",
    ".local-inputs/visual-skin-owner-v6/weapons",
    "port/android-native/app/src/main/assets/models/prince_modular.bdae",
    "port/game-data/reference/player-item-effects-v5/starter-effects-fixtures.bin",
]
INCLUDES = [
    "port/level-world", "port/game-data", "port/engine-textures", "port/engine-ui",
    "port/engine-animation", "port/engine-skinning", "port/level-loader",
    "port/level-loader/vendor/tinyxml", "port/scene-materials", "port/script-runtime",
    "port/script-runtime/lua", "port/engine-resources", "port/engine-math",
    "port/engine-ui/vendor/gameswf1714", "port/windows-foundation",
]


def sha(path):
    return hashlib.sha256(Path(path).read_bytes()).hexdigest()


def run(command):
    return subprocess.run([str(part) for part in command], cwd=ROOT,
                          text=True, capture_output=True)


def main():
    STAGE.mkdir(parents=True, exist_ok=True)
    source_hashes = {p.relative_to(ROOT).as_posix(): sha(p) for p in SOURCES}
    report = {
        "validation": "BLOCKED",
        "scope": "Actual Gear inventory projection; Windows COFF root archive supplies one current ABI closure.",
        "source_sha256": source_hashes,
        "inputs": [p for p in INPUTS if (ROOT / p).exists()],
    }
    try:
        if not COMPILER.is_file() or not ARCHIVER.is_file():
            raise RuntimeError(f"LLVM-MinGW tools unavailable under {TOOLCHAIN}")
        if not NATIVE.is_file() or not all(p.is_file() for p in SHARED):
            raise RuntimeError("Current native.a or one Windows foundation archive is unavailable")

        flags = ["-std=c++20", "-Dfinite=_finite", "-DDH2_NATIVE_PROFILE_TRANSPORT_EMBED",
                 "-O0", "-ffunction-sections", "-fdata-sections", "-fno-fast-math",
                 "-ffp-contract=off", "-w", "-include", "exception"]
        for directory in INCLUDES:
            flags += ["-I" + str(ROOT / directory)]
        flags += ["-isystem", str(ROOT / "port/physics-backend/box2d-2.0.1/Include")]

        objects = []
        for source in SOURCES:
            obj = STAGE / (source.stem + ".o")
            command = [COMPILER, *flags, "-c", source, "-o", obj]
            result = run(command)
            if result.returncode:
                raise RuntimeError(f"Compile failed for {source.relative_to(ROOT)}:\n{result.stderr}")
            objects.append(obj)

        # Keep the archive ordering identical to the root native fixture: the
        # current source closure is present on both sides of foundation archives.
        link = [COMPILER, *objects, NATIVE, *SHARED, NATIVE,
                "-Wl,--gc-sections", "-Wl,-Map," + str(STAGE / "native.map"),
                "-lopengl32", "-luser32", "-lgdi32", "-o", EXE]
        result = run(link)
        (STAGE / "link-errors.txt").write_text(result.stderr)
        if result.returncode:
            raise RuntimeError(f"COFF link failed:\n{result.stderr}")

        env = dict(os.environ)
        env["PATH"] = str(TOOLCHAIN) + os.pathsep + env.get("PATH", "")
        execution = [EXE, *(ROOT / p for p in INPUTS)]
        result = subprocess.run([str(p) for p in execution], cwd=ROOT,
                                text=True, capture_output=True, env=env)
        (STAGE / "stdout.txt").write_text(result.stdout)
        (STAGE / "stderr.txt").write_text(result.stderr)
        report["native_exit"] = result.returncode
        report["stdout"] = result.stdout
        report["stderr"] = result.stderr
        if result.returncode or result.stderr:
            raise RuntimeError(f"Native run failed (exit {result.returncode}):\n{result.stdout}\n{result.stderr}")
        if "source inventory projection PASS" not in result.stdout:
            raise RuntimeError("Native test did not emit its PASS receipt")
        if source_hashes != {p.relative_to(ROOT).as_posix(): sha(p) for p in SOURCES}:
            raise RuntimeError("A source input changed during native verification")
        report["validation"] = "PASS"
        report["executable_sha256"] = sha(EXE)
        report["native_archive_sha256"] = sha(NATIVE)
        report["foundation_archives_sha256"] = {p.name: sha(p) for p in SHARED}
        report["commands"] = {
            "compile": "LLVM-MinGW C++20 COFF objects for projection, resolver and actual Gear fixture",
            "link": [str(p) for p in link],
            "run": [str(p) for p in execution],
        }
        print(result.stdout, end="")
    except Exception as exc:
        report["error"] = str(exc)
        print(str(exc))
        raise
    finally:
        (STAGE / "report.json").write_text(json.dumps(report, indent=2) + "\n")
        (ROOT / "port/windows-foundation/reports/source-inventory-projection-v1-host.json").write_text(
            json.dumps(report, indent=2) + "\n")


if __name__ == "__main__":
    main()
