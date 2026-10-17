#!/usr/bin/env python3
"""Build an isolated API 37 Irrlicht NativeActivity from local-cache assets."""
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
import time
import zipfile


HERE = Path(__file__).resolve().parent
IRRLICHT = HERE.parent
REPO = HERE.parents[2]
WORK = REPO.parent
BUILD = HERE / "build"
PACKAGE = "org.dh2.irrlicht.cache.scene.smoke"
ABIS = (
    ("arm64-v8a", "aarch64-linux-android26", "AArch64"),
    ("x86_64", "x86_64-linux-android26", "Advanced Micro Devices X86-64"),
)
PINNED_ASSETS = {
    "data/3d/modules/void_maze/void_maze.bdae": {
        "apk_path": "dh2/void_maze.bdae",
        "bytes": 199300,
        "sha256": "7b67b90b65b41de96a5bd5a9a9d9d8f40cdd4425e806057a28ee5299ed2840ba",
        "role": "BRES source scene",
    },
    "data/3d/textures/env_voidmaze.tga": {
        "apk_path": "dh2/env_voidmaze.tga",
        "bytes": 32828,
        "sha256": "aac2c1923d2b9add49d6d8c32211d0af9d1f0a9e022530d551d1dc65d2703c20",
        "role": "source diffuse sampler image",
    },
}
PORT_SOURCES = (
    REPO / "port/android-app/scene_buffers.cpp",
    REPO / "port/skin-payloads/skin.cpp",
    REPO / "port/animation-pose/pose.cpp",
    REPO / "port/animation-values/values.cpp",
    REPO / "port/animation-timeline/timeline.cpp",
    REPO / "port/animation-mixing/mixing.cpp",
    REPO / "port/animation-layers/layers.cpp",
    REPO / "port/scene-draw/draw.cpp",
    REPO / "port/scene-payloads/scene.cpp",
    REPO / "port/asset-payloads/payloads.cpp",
    REPO / "port/engine-resources/resources.cpp",
    REPO / "port/engine-math/math.cpp",
    REPO / "port/material-bindings/bindings.cpp",
    REPO / "port/texture-assets/texture.cpp",
    REPO / "port/texture-assets/decode.cpp",
)


def sha256(path: Path) -> str:
    digest = hashlib.sha256()
    with path.open("rb") as stream:
        for block in iter(lambda: stream.read(1024 * 1024), b""):
            digest.update(block)
    return digest.hexdigest()


def run(command: list[str | Path], *, log: Path | None = None,
        cwd: Path | None = None) -> str:
    args = [str(part) for part in command]
    result = subprocess.run(args, cwd=cwd, text=True, stdout=subprocess.PIPE,
                            stderr=subprocess.STDOUT, check=False)
    output = result.stdout or ""
    if log:
        log.parent.mkdir(parents=True, exist_ok=True)
        log.write_text(output, encoding="utf-8", errors="replace")
    if result.returncode:
        raise RuntimeError(f"command failed ({result.returncode}): {args!r}\n"
                           + "\n".join(output.splitlines()[-80:]))
    return output


def locate_platform(sdk: Path) -> Path:
    for name in ("android-37.2", "android-37.0.0", "android-37.0", "android-37"):
        jar = sdk / "platforms" / name / "android.jar"
        if jar.is_file():
            return jar
    raise FileNotFoundError(f"Android API 37 android.jar not found under {sdk}")


def collect_assets(cache: Path, packaged_assets: Path) -> list[dict]:
    cache = cache.resolve(strict=True)
    if not cache.is_dir():
        raise NotADirectoryError(cache)
    records = []
    for relative, expected in PINNED_ASSETS.items():
        source = (cache / Path(relative)).resolve(strict=True)
        if cache not in source.parents:
            raise ValueError(f"cache source resolves outside supplied cache root: {relative}")
        actual = {"bytes": source.stat().st_size, "sha256": sha256(source)}
        if actual != {"bytes": expected["bytes"], "sha256": expected["sha256"]}:
            raise ValueError(f"pinned local-cache asset changed: {relative}: {actual}")
        destination = packaged_assets / expected["apk_path"]
        destination.parent.mkdir(parents=True, exist_ok=True)
        shutil.copy2(source, destination)
        if sha256(destination) != expected["sha256"]:
            raise ValueError(f"asset changed while packaging: {relative}")
        records.append({"source": relative, "apk_asset": expected["apk_path"],
                        "role": expected["role"], **actual})
    return records


def check_elf(readelf: Path, library: Path, machine: str) -> dict:
    header = run([readelf, "-h", library])
    if machine not in header:
        raise RuntimeError(f"unexpected ELF machine in {library}: {machine}")
    segments = run([readelf, "-lW", library])
    alignments = [int(fields[-1], 16) for line in segments.splitlines()
                  if (fields := line.split()) and fields[0] == "LOAD"]
    if not alignments or min(alignments) < 16384:
        raise RuntimeError(f"{library}: insufficient PT_LOAD alignment {alignments}")
    return {"bytes": library.stat().st_size, "sha256": sha256(library),
            "machine": machine, "pt_load_alignments": alignments}


def package_apk(sdk: Path, build_tools: Path, host_bin: Path,
                packaged_assets: Path, outputs: dict) -> dict:
    base = BUILD / "base.apk"
    run([build_tools / "aapt2.exe", "link", "--manifest", HERE / "AndroidManifest.xml",
         "-I", locate_platform(sdk), "--min-sdk-version", "26",
         "--target-sdk-version", "37", "-A", packaged_assets,
         "-o", base], log=BUILD / "logs/aapt2-link.log")
    unsigned = BUILD / "cache-scene-unsigned.apk"
    with zipfile.ZipFile(base, "r") as source, zipfile.ZipFile(unsigned, "w") as target:
        for item in source.infolist():
            target.writestr(item, source.read(item.filename))
        for abi, _, _ in ABIS:
            target.write(HERE / outputs[abi]["library"],
                         f"lib/{abi}/libCacheScene.so", compress_type=zipfile.ZIP_STORED)
    aligned = BUILD / "cache-scene-aligned.apk"
    run([build_tools / "zipalign.exe", "-f", "-P", "16", "4", unsigned, aligned],
        log=BUILD / "logs/zipalign.log")

    java_home = Path(os.environ.get("JAVA_HOME", "C:/Program Files/Java/jdk-23"))
    keytool = java_home / "bin" / "keytool.exe"
    if not keytool.is_file():
        found = shutil.which("keytool")
        if not found:
            raise FileNotFoundError("JDK keytool is required to sign the smoke APK")
        keytool = Path(found)
    keystore = BUILD / "cache-scene-debug.jks"
    if not keystore.is_file():
        run([keytool, "-genkeypair", "-keystore", keystore, "-storepass", "android",
             "-keypass", "android", "-alias", "debug", "-keyalg", "RSA",
             "-keysize", "2048", "-validity", "3650", "-dname",
             "CN=DH2 Irrlicht cache scene smoke"], log=BUILD / "logs/keytool.log")
    apk = BUILD / "dh2-irrlicht-cache-scene-smoke-debug.apk"
    run([build_tools / "apksigner.bat", "sign", "--ks", keystore,
         "--ks-key-alias", "debug", "--ks-pass", "pass:android",
         "--key-pass", "pass:android", "--out", apk, aligned],
        log=BUILD / "logs/apksigner.log")
    verify = run([build_tools / "apksigner.bat", "verify", "--verbose", apk],
                 log=BUILD / "logs/apksigner-verify.log")
    run([build_tools / "zipalign.exe", "-c", "-P", "16", "4", apk],
        log=BUILD / "logs/zipalign-verify.log")
    if "Verifies" not in verify:
        raise RuntimeError("apksigner did not confirm the APK signature")
    return {"path": apk.relative_to(HERE).as_posix(), "bytes": apk.stat().st_size,
            "sha256": sha256(apk), "signed": True, "zipalign_page_size": 16384}


def run_emulator(sdk: Path, apk: Path, serial: str, report: dict) -> None:
    adb = sdk / "platform-tools" / "adb.exe"
    devices = run([adb, "devices", "-l"])
    if not re.search(rf"(?m)^{re.escape(serial)}\s+device\b", devices):
        raise RuntimeError(f"emulator {serial} is not online:\n{devices}")
    sdk_version = run([adb, "-s", serial, "shell", "getprop",
                       "ro.build.version.sdk"]).strip()
    os_release = run([adb, "-s", serial, "shell", "getprop",
                      "ro.build.version.release"]).strip()
    page_size = run([adb, "-s", serial, "shell", "getconf", "PAGESIZE"]).strip()
    abi = run([adb, "-s", serial, "shell", "getprop",
               "ro.product.cpu.abi"]).strip()
    if sdk_version != "37" or os_release != "17":
        raise RuntimeError(f"expected Android 17/API 37, got release={os_release} sdk={sdk_version}")
    if page_size != "16384":
        raise RuntimeError(f"expected a 16 KiB-page test emulator, got {page_size!r}")
    run([adb, "-s", serial, "install", "-r", apk], log=BUILD / "logs/adb-install.log")
    installed_spec = run([adb, "-s", serial, "shell", "pm", "path", PACKAGE]).strip()
    installed_path = next((line.split(":", 1)[1] for line in installed_spec.splitlines()
                           if line.startswith("package:")), "")
    if not installed_path.endswith("/base.apk"):
        raise RuntimeError(f"unexpected installed base APK path: {installed_spec}")
    installed_copy = BUILD / f"installed-base-{serial}.apk"
    run([adb, "-s", serial, "pull", installed_path, installed_copy],
        log=BUILD / "logs/adb-pull-installed.log")
    installed_sha = sha256(installed_copy)
    if installed_sha != sha256(apk):
        raise RuntimeError(f"installed base APK differs: {installed_sha} != {sha256(apk)}")
    run([adb, "-s", serial, "shell", "am", "force-stop", PACKAGE])
    run([adb, "-s", serial, "logcat", "-c"])
    run([adb, "-s", serial, "shell", "am", "start", "-n",
         f"{PACKAGE}/android.app.NativeActivity"], log=BUILD / "logs/adb-launch.log")
    time.sleep(12)
    capture = subprocess.run([str(adb), "-s", serial, "exec-out", "screencap", "-p"],
                             stdout=subprocess.PIPE, stderr=subprocess.PIPE, check=False)
    if capture.returncode:
        raise RuntimeError("screencap failed: " +
                           capture.stderr.decode("utf-8", errors="replace"))
    screenshot = BUILD / f"runtime-{serial}.png"
    screenshot.write_bytes(capture.stdout)
    pixel_evidence = {"image_analysis": "Pillow unavailable"}
    try:
        from PIL import Image, ImageChops
        image = Image.open(screenshot).convert("RGB")
        width, height = image.size
        # Stay clear of the top label and navigation bars. The middle portion
        # is where the centered normalized source module should appear.
        region = image.crop((int(width * 0.2), int(height * 0.16),
                             int(width * 0.8), int(height * 0.88)))
        background = Image.new("RGB", region.size, (17, 26, 40))
        changed = ImageChops.difference(region, background).convert("L")
        changed_pixels = sum(1 for value in changed.getdata() if value > 12)
        unique_colors = len(region.getcolors(maxcolors=region.width * region.height) or [])
        texture_region = image.crop((20, 80, 276, 336))
        texture_color_count = len(texture_region.getcolors(
            maxcolors=texture_region.width * texture_region.height) or [])
        pixel_evidence = {"screenshot_size": [width, height],
                          "checked_region_fraction": [0.2, 0.16, 0.8, 0.88],
                          "clear_color_rgb": [17, 26, 40],
                          "pixels_different_from_clear_color": changed_pixels,
                          "unique_rgb_colors_in_region": unique_colors,
                          "visible_mesh_pixels_pass": changed_pixels > 500,
                          "texture_color_variation_pass": unique_colors > 10,
                          "debug_texture_swatch_rect": [20, 80, 276, 336],
                          "debug_texture_swatch_unique_colors": texture_color_count,
                          "debug_texture_swatch_pass": texture_color_count > 10}
    except ImportError:
        pass
    pid = run([adb, "-s", serial, "shell", "pidof", PACKAGE]).strip().split()
    if not pid:
        raise RuntimeError("NativeActivity has no live process after launch")
    first_logcat = run([adb, "-s", serial, "logcat", "-d", "--pid", pid[0]],
                       log=BUILD / f"logs/runtime-{serial}-initial-logcat.txt")
    marker = "PASS: actual cache BRES assembled into Irrlicht:"
    if marker not in first_logcat:
        raise RuntimeError("cache scene marker missing from Activity logcat")
    scene_line = next((line for line in first_logcat.splitlines() if marker in line), "")
    texture_assignments = re.search(r"texture_assigned_buffers=(\d+)", scene_line)
    texture_size = re.search(r"texture_size=(\d+)x(\d+)", scene_line)
    textured_draws = re.search(r"textured_draws=(\d+)", scene_line)
    texture_only_visible = re.search(r"visible_texture_only=(\d+)", scene_line)
    texture_only_assignments = re.search(r"texture_only_assignments=(\d+)", scene_line)
    if any(term in first_logcat for term in ("FATAL EXCEPTION", "Fatal signal", "SIGSEGV", "SIGABRT")):
        raise RuntimeError("fatal runtime error detected")
    run([adb, "-s", serial, "shell", "input", "keyevent", "3"])
    time.sleep(3)
    run([adb, "-s", serial, "shell", "am", "start", "-W", "-n",
         f"{PACKAGE}/android.app.NativeActivity"], log=BUILD / "logs/adb-resume.log")
    time.sleep(5)
    resumed_pid = run([adb, "-s", serial, "shell", "pidof", PACKAGE]).strip().split()
    if not resumed_pid:
        raise RuntimeError("NativeActivity process did not survive/recover after Home resume")
    resume_capture = subprocess.run(
        [str(adb), "-s", serial, "exec-out", "screencap", "-p"],
        stdout=subprocess.PIPE, stderr=subprocess.PIPE, check=False)
    if resume_capture.returncode:
        raise RuntimeError("resume screencap failed: " +
                           resume_capture.stderr.decode("utf-8", errors="replace"))
    resumed_screenshot = BUILD / f"runtime-{serial}-resumed.png"
    resumed_screenshot.write_bytes(resume_capture.stdout)
    logcat = run([adb, "-s", serial, "logcat", "-d", "--pid", resumed_pid[0]],
                 log=BUILD / f"logs/runtime-{serial}-logcat.txt")
    if any(term in logcat for term in ("FATAL EXCEPTION", "Fatal signal", "SIGSEGV", "SIGABRT")):
        raise RuntimeError("fatal runtime error detected after Home resume")
    runtime = {"serial": serial, "android_release": os_release,
               "sdk": int(sdk_version), "page_size": int(page_size), "abi": abi,
               "activity_pid": pid[0], "installed_package_path": installed_path,
               "installed_base_apk_sha256": installed_sha,
               "installed_base_apk_matches_build": installed_sha == sha256(apk),
               "pass_marker": marker in first_logcat,
               "texture_assigned_buffers": int(texture_assignments.group(1))
                   if texture_assignments else None,
               "texture_only_visible_draws": int(texture_only_visible.group(1))
                   if texture_only_visible else None,
               "texture_only_draw_assignments": int(texture_only_assignments.group(1))
                   if texture_only_assignments else None,
               "source_textured_draw_count": int(textured_draws.group(1))
                   if textured_draws else None,
               "decoded_texture_dimensions": [int(texture_size.group(1)),
                                               int(texture_size.group(2))]
                   if texture_size else None,
               "screenshot": screenshot.relative_to(HERE).as_posix(),
               "screenshot_sha256": sha256(screenshot),
               "pixel_evidence": pixel_evidence,
               "home_pause_resume": {"attempted": True, "process_alive_after_resume": True,
                                     "resumed_screenshot": resumed_screenshot.relative_to(HERE).as_posix(),
                                     "resumed_screenshot_sha256": sha256(resumed_screenshot)},
               "logcat": f"logs/runtime-{serial}-logcat.txt",
               "gl_invalid_operation_count": logcat.count("GL_INVALID_OPERATION"),
               "logcat_excerpt": logcat[-16000:],
               "compatibility_claim": "installed/run on 16 KiB-page Android 17 API 37 emulator"}
    report["runtime"] = runtime


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--sdk", type=Path, default=Path(
        os.environ.get("ANDROID_SDK_ROOT") or os.environ.get("ANDROID_HOME")
        or WORK / "emulator-test" / "sdk"))
    parser.add_argument("--ndk", type=Path, default=Path(
        os.environ.get("ANDROID_NDK_HOME") or os.environ.get("ANDROID_NDK_ROOT")
        or WORK / "emulator-test" / "sdk" / "ndk" / "29.0.14206865"))
    parser.add_argument("--cache", type=Path, default=WORK / "cache" / "files",
                        help="Unpacked local original cache; only the two pinned test assets are copied")
    parser.add_argument("--install", action="store_true")
    parser.add_argument("--serial", help="Explicit API 37 emulator serial; never selected implicitly")
    args = parser.parse_args()
    if args.install and not args.serial:
        parser.error("--install requires an explicit --serial after emulator coordination")
    ndk, sdk, cache = args.ndk.resolve(), args.sdk.resolve(), args.cache.resolve()
    build_tools = sdk / "build-tools" / "37.0.0"
    host_bin = ndk / "toolchains/llvm/prebuilt/windows-x86_64/bin"
    clang, clang_c = host_bin / "clang++.exe", host_bin / "clang.exe"
    readelf = host_bin / "llvm-readelf.exe"
    native_glue = ndk / "sources/android/native_app_glue"
    static_root = IRRLICHT / "build/static"
    if not clang.is_file() or not native_glue.is_dir():
        raise FileNotFoundError(f"NDK r29 not found at {ndk}")
    if not (static_root / "x86_64/libIrrlicht.a").is_file():
        raise FileNotFoundError("pinned Irrlicht archives missing; build the separate shared engine first")
    if not build_tools.is_dir() or not locate_platform(sdk).is_file():
        raise FileNotFoundError(f"API 37 SDK/build tools are missing under {sdk}")
    if BUILD.exists() and (BUILD.resolve().parent != HERE.resolve() or BUILD.name != "build"):
        raise RuntimeError(f"refusing to write outside this smoke folder: {BUILD.resolve()}")
    BUILD.mkdir(parents=True, exist_ok=True)
    logs = BUILD / "logs"
    packaged_assets = BUILD / "apk-assets"
    if packaged_assets.exists():
        shutil.rmtree(packaged_assets)
    packaged_assets.mkdir(parents=True)
    asset_records = collect_assets(cache, packaged_assets)

    upstream_manifest_path = IRRLICHT / "upstream-source-manifest.json"
    upstream_manifest = json.loads(upstream_manifest_path.read_text(encoding="utf-8"))
    upstream_files = {row["path"]: row for row in upstream_manifest["files"]}
    shader_dir = IRRLICHT / "upstream/media/Shaders"
    shader_output = packaged_assets / "media/Shaders"
    shader_output.mkdir(parents=True)
    shader_records = []
    for shader in sorted(shader_dir.glob("*")):
        if not shader.is_file():
            continue
        relative = shader.relative_to(IRRLICHT / "upstream").as_posix()
        pin = upstream_files.get(relative)
        if not pin or shader.stat().st_size != pin["bytes"] or sha256(shader) != pin["sha256"]:
            raise RuntimeError(f"pinned shader changed: {relative}")
        shutil.copy2(shader, shader_output / shader.name)
        shader_records.append({"path": relative, "bytes": pin["bytes"], "sha256": pin["sha256"]})
    if not any(row["path"].endswith("/COGLES2Solid.vsh") for row in shader_records) or \
       not any(row["path"].endswith("/COGLES2Solid.fsh") for row in shader_records):
        raise RuntimeError("pinned OGLES2 solid material shader pair is incomplete")

    source_paths = (HERE / "main.cpp", IRRLICHT / "game/scene_mesh_adapter.cpp", *PORT_SOURCES)
    outputs = {}
    irr_include = IRRLICHT / "upstream/include"
    for abi, target, machine in ABIS:
        object_dir = BUILD / "obj" / abi
        object_dir.mkdir(parents=True, exist_ok=True)
        sysroot = host_bin.parent / "sysroot"
        common_flags = [f"--target={target}", f"--sysroot={sysroot}", "-std=c++17",
                        "-fPIC", "-fno-exceptions", "-fno-rtti", "-O2",
                        "-Wall", "-Wextra", "-Wno-unused-parameter",
                        "-Wno-inconsistent-missing-override",
                        "-Wno-deprecated-copy-with-user-provided-copy",
                        "-I", str(irr_include), "-I", str(IRRLICHT / "game"),
                        "-I", str(REPO / "port/android-app"),
                        "-I", str(native_glue)]
        objects = []
        for source in source_paths:
            obj = object_dir / f"{source.stem}.o"
            run([clang, *common_flags, "-c", source, "-o", obj],
                log=logs / f"{abi}-{source.stem}-compile.log")
            objects.append(obj)
        glue_obj = object_dir / "android_native_app_glue.o"
        run([clang_c, f"--target={target}", f"--sysroot={sysroot}", "-fPIC",
             "-Wall", "-Wextra", "-I", str(native_glue), "-c",
             native_glue / "android_native_app_glue.c", "-o", glue_obj],
            log=logs / f"{abi}-android-native-app-glue-compile.log")
        objects.append(glue_obj)
        library = BUILD / "native" / abi / "libCacheScene.so"
        library.parent.mkdir(parents=True, exist_ok=True)
        run([clang, f"--target={target}", f"--sysroot={sysroot}", "-shared",
             "-Wl,--no-undefined", "-Wl,-z,max-page-size=16384",
             "-Wl,-z,common-page-size=16384", *objects,
             static_root / abi / "libIrrlicht.a", "-landroid", "-llog", "-lEGL",
             "-lGLESv1_CM", "-lGLESv2", "-lz", "-static-libstdc++", "-o", library],
            log=logs / f"{abi}-link.log")
        outputs[abi] = {"library": library.relative_to(HERE).as_posix(),
                        **check_elf(readelf, library, machine)}

    apk = package_apk(sdk, build_tools, host_bin, packaged_assets, outputs)
    report = {
        "result": "Android build pass; emulator rendering not yet run",
        "scope": "Isolated cache-backed static-scene smoke using the pinned Irrlicht adapter; not game integration.",
        "package": PACKAGE,
        "min_api": 26,
        "target_api": 37,
        "native_compile_api": 26,
        "local_cache_root": "user-supplied local cache; absolute path intentionally omitted",
        "source_assets": asset_records,
        "pinned_irrlicht": {"svn_revision": upstream_manifest["svn_revision"],
                            "tree_manifest_sha256": upstream_manifest["tree_manifest_sha256"],
                            "shader_count": len(shader_records), "shaders": shader_records},
        "abis": outputs,
        "apk": apk,
        "runtime": None,
        "rendering_status": "not claimed until the on-screen imported cache mesh is visually checked",
    }
    if args.install:
        run_emulator(sdk, HERE / apk["path"], args.serial, report)
        runtime = report.get("runtime") or {}
        pixels = runtime.get("pixel_evidence") or {}
        mesh_visible = pixels.get("visible_mesh_pixels_pass") is True
        texture_visible_on_mesh = pixels.get("texture_color_variation_pass") is True
        swatch_visible = pixels.get("debug_texture_swatch_pass") is True
        if mesh_visible and texture_visible_on_mesh:
            report["result"] = (
                "Android build and visible texture mapping pass on source cache draws"
            )
            report["rendering_status"] = (
                "Source cache texture is visibly mapped on the isolated texture-bearing source draws "
                "in the API 37 screenshot; full-scene materials outside this diagnostic subset remain unverified"
            )
        elif mesh_visible and swatch_visible:
            report["result"] = "Android build and cache-scene render pass; mesh texture mapping unresolved"
            report["rendering_status"] = (
                "Imported cache mesh is visibly rendered; source texture is decoded and visibly displayed "
                "in the diagnostic swatch, but texture color variation is not visible on the mesh"
            )
        elif mesh_visible:
            report["result"] = "Android build and cache-scene render pass; texture appearance unresolved"
            report["rendering_status"] = (
                "Imported cache mesh is visibly rendered; source texture appearance was not confirmed in pixels"
            )
        else:
            report["result"] = "Android build and Activity smoke pass; visible scene render unconfirmed"
            report["rendering_status"] = (
                "The APK launched, but captured pixels did not confirm a visible imported cache mesh"
            )
    report_path = BUILD / "cache-scene-build-report.json"
    report_path.write_text(json.dumps(report, indent=2) + "\n", encoding="utf-8")
    print(json.dumps(report, indent=2))
    return 0


if __name__ == "__main__":
    try:
        raise SystemExit(main())
    except (OSError, RuntimeError, ValueError, KeyError) as exc:
        print(f"ERROR: {exc}", file=sys.stderr)
        raise SystemExit(1)
