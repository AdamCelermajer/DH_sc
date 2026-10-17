#!/usr/bin/env python3
"""Build the pinned Irrlicht 1.8.5 Null-driver candidate and run its host mesh probe."""
from __future__ import annotations

import argparse
import hashlib
import json
import os
from pathlib import Path
import re
import shutil
import subprocess
import sys
from concurrent.futures import ThreadPoolExecutor, as_completed


HERE = Path(__file__).resolve().parent
REPO = HERE.parents[1]
UPSTREAM = HERE / "upstream" / "irrlicht-1.8.5"
MANIFEST = HERE / "upstream-source-manifest.json"
SOURCE = UPSTREAM / "source" / "Irrlicht"
CACHE_DEFAULT = REPO.parent / "cache" / "files"
NDK_DEFAULT = REPO.parent / "emulator-test" / "sdk" / "ndk" / "29.0.14206865"

COMMON_MAKE_VARIABLES = (
    "IRRMESHOBJ", "IRROBJ", "IRRPARTICLEOBJ", "IRRANIMOBJ", "IRRIMAGEOBJ",
    "IRRIOOBJ", "IRRGUIOBJ", "ZLIBOBJ", "JPEGLIBOBJ", "LIBPNGOBJ",
    "LIBAESGM", "BZIP2OBJ",
)
NULL_VIDEO_OBJECTS = ("CNullDriver.o", "CVideoModeList.o", "CFPSCounter.o")
NULL_PLATFORM_OBJECTS = ("CLogger.o", "COSOperator.o", "os.o")
APP_SOURCES = (
    "port/android-app/scene_buffers.cpp",
    "port/scene-payloads/scene.cpp",
    "port/scene-draw/draw.cpp",
    "port/asset-payloads/payloads.cpp",
    "port/material-bindings/bindings.cpp",
    "port/engine-resources/resources.cpp",
    "port/engine-math/math.cpp",
    "port/animation-pose/pose.cpp",
    "port/animation-values/values.cpp",
    "port/animation-timeline/timeline.cpp",
    "port/animation-mixing/mixing.cpp",
    "port/animation-layers/layers.cpp",
    "port/skin-payloads/skin.cpp",
)


def run(command: list[str], *, cwd: Path | None = None) -> str:
    try:
        completed = subprocess.run(command, cwd=cwd, check=True, text=True,
                                   stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
        return completed.stdout
    except subprocess.CalledProcessError as error:
        detail = error.stdout or str(error)
        raise RuntimeError(detail[-10000:]) from error


def response_file(path: Path, arguments: list[str]) -> Path:
    path.parent.mkdir(parents=True, exist_ok=True)
    normalized = [value.replace("\\", "/") for value in arguments]
    lines = [f'"{value.replace(chr(34), chr(92) + chr(34))}"'
             if any(ch.isspace() for ch in value) else value for value in normalized]
    path.write_text("\n".join(lines) + "\n", encoding="utf-8")
    return path


def verify_upstream() -> dict:
    manifest = json.loads(MANIFEST.read_text(encoding="utf-8"))
    records = manifest["files"]
    canonical = json.dumps(records, sort_keys=True, separators=(",", ":")).encode()
    digest = hashlib.sha256(canonical).hexdigest()
    if digest != manifest["tree_manifest_sha256"]:
        raise RuntimeError("upstream manifest canonical hash mismatch")
    for record in records:
        path = UPSTREAM.joinpath(*Path(record["path"]).parts)
        if not path.is_file():
            raise RuntimeError(f"missing pinned upstream file: {record['path']}")
        data = path.read_bytes()
        if len(data) != record["bytes"] or hashlib.sha256(data).hexdigest() != record["sha256"]:
            raise RuntimeError(f"pinned upstream file differs: {record['path']}")
    return manifest


def make_variables() -> dict[str, str]:
    lines = (SOURCE / "Makefile").read_text(encoding="utf-8").splitlines()
    variables: dict[str, str] = {}
    pending = ""
    for line in lines:
        pending = (pending + " " + line.strip()) if pending else line
        if pending.rstrip().endswith("\\"):
            pending = pending.rstrip()[:-1] + " "
            continue
        match = re.match(r"^([A-Z][A-Z0-9_]*)\s*(?:\+=|:=|=)\s*(.*)$", pending)
        if match:
            variables[match.group(1)] = match.group(2)
        pending = ""
    return variables


def upstream_source_list() -> list[Path]:
    variables = make_variables()
    memo: dict[str, list[str]] = {}

    def objects(name: str, stack: tuple[str, ...] = ()) -> list[str]:
        if name in memo:
            return memo[name]
        if name in stack:
            raise RuntimeError(f"cyclic Makefile variable: {name}")
        text = variables.get(name, "")
        expanded = re.sub(
            r"\$\(([A-Za-z0-9_]+)\)",
            lambda match: " ".join(objects(match.group(1), stack + (name,))),
            text,
        )
        result = re.findall(r"(?:[A-Za-z0-9_.-]+/)*[A-Za-z0-9_.-]+\.o\b", expanded)
        memo[name] = result
        return result

    object_names: list[str] = []
    for name in COMMON_MAKE_VARIABLES:
        object_names.extend(objects(name))
    object_names.extend(NULL_VIDEO_OBJECTS)
    object_names.extend(NULL_PLATFORM_OBJECTS)
    object_names = list(dict.fromkeys(object_names))

    result: list[Path] = []
    for obj in object_names:
        relative = Path(obj).with_suffix(".cpp")
        candidate = SOURCE / relative
        if not candidate.is_file():
            matches = [p for suffix in (".cpp", ".c", ".cc", ".cxx")
                       for p in SOURCE.rglob(Path(obj).stem + suffix)]
            if len(matches) != 1:
                raise RuntimeError(f"cannot map Makefile object {obj} to one source file: {matches}")
            candidate = matches[0]
        if not candidate.is_file():
            raise RuntimeError(f"Makefile object has no upstream source file: {obj}")
        result.append(candidate)
    return result


def include_flags() -> list[str]:
    return ["-I" + str(path) for path in (
        UPSTREAM / "include", SOURCE, SOURCE / "zlib", SOURCE / "jpeglib", SOURCE / "libpng")]


def engine_cpp_flags() -> list[str]:
    return [
        "-std=gnu++11", "-O2", "-DNDEBUG", "-fPIC", "-fno-exceptions", "-fno-rtti",
        "-fstrict-aliasing", "-ffunction-sections", "-fdata-sections", "-DIRRLICHT_EXPORTS=1",
        "-DNO_IRR_COMPILE_WITH_X11_", "-DNO_IRR_COMPILE_WITH_OPENGL_",
        "-DNO_IRR_COMPILE_WITH_SOFTWARE_", "-DNO_IRR_COMPILE_WITH_BURNINGSVIDEO_",
        "-DNO_IRR_COMPILE_WITH_SDL_DEVICE_", "-DNO_IRR_COMPILE_WITH_CONSOLE_DEVICE_",
        "-DNO_IRR_COMPILE_WITH_JOYSTICK_EVENTS_",
        *include_flags(),
    ]


def engine_c_flags() -> list[str]:
    return [
        "-std=c99", "-O2", "-DNDEBUG", "-fPIC", "-fstrict-aliasing",
        "-ffunction-sections", "-fdata-sections", "-DPNG_THREAD_UNSAFE_OK",
        "-DPNG_NO_MMX_CODE", "-DPNG_NO_MNG_FEATURES", "-DPNG_ARM_NEON_OPT=0",
        "-DNO_IRR_COMPILE_WITH_X11_", "-DNO_IRR_COMPILE_WITH_OPENGL_",
        "-DNO_IRR_COMPILE_WITH_SOFTWARE_", "-DNO_IRR_COMPILE_WITH_BURNINGSVIDEO_",
        "-DNO_IRR_COMPILE_WITH_SDL_DEVICE_", "-DNO_IRR_COMPILE_WITH_CONSOLE_DEVICE_",
        "-DNO_IRR_COMPILE_WITH_JOYSTICK_EVENTS_",
        *include_flags(),
    ]


def compile_sources(cxx: str, cc: str, sources: list[Path], output_dir: Path,
                   cpp_flags: list[str], c_flags: list[str], workers: int = 4) -> list[Path]:
    output_dir.mkdir(parents=True, exist_ok=True)
    objects: dict[Path, Path] = {}
    for source in sources:
        try:
            label = source.relative_to(SOURCE).as_posix()
        except ValueError:
            try:
                label = source.relative_to(REPO).as_posix()
            except ValueError:
                label = source.name
        selected_flags = c_flags if source.suffix == ".c" else cpp_flags
        identity = label + "\0" + "\0".join(selected_flags)
        key = hashlib.sha256(identity.encode("utf-8")).hexdigest()[:12]
        target = output_dir / f"{key}-{source.stem}.o"
        objects[source] = target

    def compile_one(source: Path, target: Path) -> str:
        if target.is_file() and target.stat().st_mtime_ns >= source.stat().st_mtime_ns:
            return ""
        compiler = cc if source.suffix == ".c" else cxx
        flags = c_flags if source.suffix == ".c" else cpp_flags
        try:
            return run([compiler, *flags, "-c", str(source), "-o", str(target)])
        except Exception as error:
            raise RuntimeError(str(error)[-6000:]) from error

    with ThreadPoolExecutor(max_workers=max(1, workers)) as pool:
        jobs = {pool.submit(compile_one, source, target): source
                for source, target in objects.items()}
        for future in as_completed(jobs):
            source = jobs[future]
            try:
                future.result()
            except Exception as error:
                raise RuntimeError(f"failed compiling {source}: {error}") from error
    return list(objects.values())


def verify_elf_alignment(readelf: Path, library: Path) -> tuple[str, list[int]]:
    output = run([str(readelf), "-lW", str(library)])
    aligns: list[int] = []
    for line in output.splitlines():
        columns = line.split()
        if columns and columns[0] == "LOAD":
            aligns.append(int(columns[-1], 16))
    if not aligns:
        raise RuntimeError(f"no PT_LOAD segments in {library}")
    if any(value < 16384 for value in aligns):
        raise RuntimeError(f"PT_LOAD segment is below 16 KiB alignment: {aligns}")
    return output, aligns


def build_android(ndk: Path, sources: list[Path], output_root: Path) -> list[dict]:
    bin_dir = ndk / "toolchains" / "llvm" / "prebuilt" / "windows-x86_64" / "bin"
    clang = bin_dir / "clang++.exe"
    readelf = bin_dir / "llvm-readelf.exe"
    if not clang.is_file() or not readelf.is_file():
        raise FileNotFoundError(f"NDK r29 toolchain not found at {bin_dir}")
    c_compiler = bin_dir / "clang.exe"
    if not c_compiler.is_file():
        raise FileNotFoundError(f"NDK C compiler not found at {c_compiler}")
    version = run([str(clang), "--version"]).splitlines()[0]
    reports: list[dict] = []
    # This NDK r29 installation contains Android native sysroot stubs only through API 35.
    # The app targets SDK/runtime API 37; API 35 is the highest available native target and
    # is the same target used by port/android-app/build.py.
    for abi, target in (("arm64-v8a", "aarch64-linux-android35"),
                        ("x86_64", "x86_64-linux-android35")):
        build_dir = output_root / abi
        build_dir.mkdir(parents=True, exist_ok=True)
        library = build_dir / "libirrlicht_nullprobe.so"
        cpp_flags = [f"--target={target}", *engine_cpp_flags(),
                     "-Wno-deprecated-declarations", "-Wno-absolute-value"]
        c_flags = [f"--target={target}", *engine_c_flags(),
                   "-Wno-deprecated-declarations", "-Wno-absolute-value"]
        objects = compile_sources(str(clang), str(c_compiler), sources,
                                  build_dir / "obj", cpp_flags, c_flags)
        custom_objects = compile_sources(str(clang), str(c_compiler),
            [HERE / "core_globals.cpp", HERE / "null_probe.cpp"],
            build_dir / "obj-custom", cpp_flags, c_flags)
        link_args = [f"--target={target}", "-shared", "-Wl,--no-undefined",
                     "-Wl,-z,max-page-size=16384", "-Wl,-z,common-page-size=16384",
                     "-o", str(library), *(str(p) for p in objects),
                     *(str(p) for p in custom_objects)]
        link_rsp = response_file(build_dir / "link-arguments.rsp", link_args)
        output = run([str(clang), "@" + str(link_rsp)])
        elf, alignments = verify_elf_alignment(readelf, library)
        dynamic = run([str(readelf), "-d", str(library)])
        needed = re.findall(r"\(NEEDED\).*\[(.*?)\]", dynamic)
        (build_dir / "readelf-program-headers.txt").write_text(elf, encoding="utf-8")
        reports.append({
            "abi": abi,
            "target": target,
            "artifact": str(library.relative_to(REPO)),
            "bytes": library.stat().st_size,
            "pt_load_alignments": alignments,
            "needed_shared_libraries": needed,
            "compile_output": output[-4000:],
        })
        print(f"built {abi}: {library.stat().st_size} bytes; PT_LOAD alignments={alignments}")
    return [{"ndk_clang": version, "libraries": reports}]


def build_host(compiler: str, sources: list[Path], output_root: Path,
               cache: Path) -> dict:
    output_root.mkdir(parents=True, exist_ok=True)
    executable = output_root / "scene_mesh_null_host.exe"
    cc = shutil.which("gcc") or "gcc"
    engine_cpp = compile_sources(
        compiler, cc, sources, output_root / "obj-engine",
        [*engine_cpp_flags(), "-D_IRR_STATIC_LIB_"], engine_c_flags())
    app_cpp_sources = [REPO / source for source in APP_SOURCES]
    app_cpp_sources.extend((HERE / "host" / "scene_mesh_null_host.cpp",
                            HERE / "core_globals.cpp"))
    app_flags = ["-std=gnu++17", *engine_cpp_flags()[1:], "-D_IRR_STATIC_LIB_",
                 "-I" + str(REPO / "port" / "android-app")]
    app_objects = compile_sources(compiler, cc, app_cpp_sources, output_root / "obj-app",
                                  app_flags, engine_c_flags())
    link_args = ["-Wl,--gc-sections", *(str(p) for p in engine_cpp),
                 *(str(p) for p in app_objects), "-o", str(executable)]
    link_rsp = response_file(output_root / "link-arguments.rsp", link_args)
    output = run([compiler, "@" + str(link_rsp)])
    sample = cache / "data" / "3d" / "animateddecors" / "candle_flame.bdae"
    test_output = run([str(executable), str(sample)], cwd=REPO)
    print(test_output.rstrip())
    return {
        "compiler": compiler,
        "c_compiler": cc,
        "artifact": str(executable.relative_to(REPO)),
        "bytes": executable.stat().st_size,
        "sample": str(sample.relative_to(REPO.parent)),
        "output": test_output,
        "compile_output_tail": output[-2000:],
    }


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--ndk", type=Path, default=NDK_DEFAULT)
    parser.add_argument("--cache", type=Path, default=CACHE_DEFAULT)
    parser.add_argument("--host-cxx", default=shutil.which("g++") or "g++")
    parser.add_argument("--host-only", action="store_true", help="run the real SceneMesh/Null-driver host check only")
    parser.add_argument("--android-only", action="store_true", help="build both Android ABIs without running the host check")
    args = parser.parse_args()
    if args.host_only and args.android_only:
        parser.error("--host-only and --android-only cannot be combined")
    manifest = verify_upstream()
    sources = upstream_source_list()
    output_root = HERE / "build"
    report: dict[str, object] = {
        "candidate": "official Irrlicht 1.8.5 Null-driver/core build",
        "runtime_api_target": 37,
        "ndk_native_target_api": 35,
        "native_target_limit": "installed NDK r29 sysroot has no API 36/37 stubs; native API 35 matches port/android-app/build.py and runs on API 37",
        "upstream_tag": manifest["tag_url"],
        "source_manifest_sha256": manifest["tree_manifest_sha256"],
        "source_files_compiled": [p.relative_to(SOURCE).as_posix() for p in sources],
        "omitted_platform_factory": "Irrlicht.cpp is omitted because its upstream 1.8.5 Android build selects the Linux/X11 device factory; core_globals.cpp supplies the exact identity globals needed by the isolated Null-driver probe",
        "excluded_upstream_backends": ["OpenGL", "Direct3D", "software renderers", "X11 device", "SDL device"],
        "upstream_patches": [],
        "patch_policy": "upstream source bytes are checked against the per-file SHA-256 manifest before every build",
    }
    if not args.android_only:
        if not args.cache.is_dir():
            raise FileNotFoundError(f"local cache is not available: {args.cache}")
        report["host_test"] = build_host(args.host_cxx, sources, output_root / "host", args.cache.resolve())
    if not args.host_only:
        report["android_build"] = build_android(args.ndk.resolve(), sources, output_root)
    report_path = HERE / "build-report.json"
    report_path.write_text(json.dumps(report, indent=2) + "\n", encoding="utf-8")
    verify_upstream()
    print(f"report: {report_path.relative_to(REPO)}")
    return 0


if __name__ == "__main__":
    try:
        raise SystemExit(main())
    except Exception as error:
        print(f"build failed: {error}", file=sys.stderr)
        raise SystemExit(1)
