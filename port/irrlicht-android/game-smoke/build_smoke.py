#!/usr/bin/env python3
"""Build, package, and optionally run the isolated SceneMesh adapter smoke."""
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
NDK_DEFAULT = WORK / "emulator-test" / "sdk" / "ndk" / "29.0.14206865"
SDK_DEFAULT = WORK / "emulator-test" / "sdk"
ABIS = (("arm64-v8a", "aarch64-linux-android26", "AArch64"),
        ("x86_64", "x86_64-linux-android26", "Advanced Micro Devices X86-64"))
PACKAGE = "org.dh2.irrlicht.adapter.smoke"


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
                           + "\n".join(output.splitlines()[-70:]))
    return output


def locate_platform(sdk: Path) -> Path:
    for name in ("android-37.0.0", "android-37.0", "android-37"):
        jar = sdk / "platforms" / name / "android.jar"
        if jar.is_file():
            return jar
    raise FileNotFoundError("Android API 37 android.jar not found")


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("--ndk", type=Path, default=Path(
        os.environ.get("ANDROID_NDK_HOME") or os.environ.get("ANDROID_NDK_ROOT")
        or NDK_DEFAULT))
    parser.add_argument("--sdk", type=Path, default=Path(
        os.environ.get("ANDROID_SDK_ROOT") or os.environ.get("ANDROID_HOME")
        or SDK_DEFAULT))
    parser.add_argument("--install", action="store_true")
    parser.add_argument("--serial", default="emulator-5558")
    args = parser.parse_args()
    ndk = args.ndk.resolve()
    sdk = args.sdk.resolve()
    build_tools = sdk / "build-tools" / "37.0.0"
    host_bin = ndk / "toolchains" / "llvm" / "prebuilt" / "windows-x86_64" / "bin"
    clang = host_bin / "clang++.exe"
    clang_c = host_bin / "clang.exe"
    readelf = host_bin / "llvm-readelf.exe"
    native_glue = ndk / "sources" / "android" / "native_app_glue"
    irr_include = IRRLICHT / "upstream" / "include"
    static_root = IRRLICHT / "build" / "static"
    manifest_path = IRRLICHT / "upstream-source-manifest.json"
    if not clang.is_file() or not native_glue.is_dir():
        raise FileNotFoundError(f"NDK r29 not found at {ndk}")
    if not (static_root / "x86_64" / "libIrrlicht.a").is_file():
        raise FileNotFoundError("pinned Irrlicht static libraries missing; build the isolated r6038 engine first")
    if not locate_platform(sdk).is_file() or not build_tools.is_dir():
        raise FileNotFoundError(f"API 37 platform/build tools missing under {sdk}")

    if BUILD.exists():
        resolved = BUILD.resolve()
        if resolved.parent != HERE.resolve() or resolved.name != "build":
            raise RuntimeError(f"refusing to write to unexpected build path {resolved}")
    BUILD.mkdir(parents=True, exist_ok=True)
    logs = BUILD / "logs"
    outputs = {}
    for abi, target, machine in ABIS:
        abi_dir = BUILD / "obj" / abi
        abi_dir.mkdir(parents=True, exist_ok=True)
        sysroot = host_bin.parent / "sysroot"
        flags = [f"--target={target}", f"--sysroot={sysroot}", "-std=c++17",
                 "-fPIC", "-fno-exceptions", "-fno-rtti", "-O2",
                 "-Wall", "-Wextra",
                 "-Werror", "-Wno-unused-parameter",
                 "-Wno-inconsistent-missing-override",
                 "-Wno-deprecated-copy-with-user-provided-copy",
                 "-I", str(irr_include),
                 "-I", str(IRRLICHT / "game"),
                 "-I", str(REPO / "port" / "android-app"), "-I", str(native_glue)]
        main_obj = abi_dir / "main.o"
        adapter_obj = abi_dir / "scene_mesh_adapter.o"
        glue_obj = abi_dir / "android_native_app_glue.o"
        run([clang, *flags, "-c", HERE / "main.cpp", "-o", main_obj],
            log=logs / f"{abi}-main-compile.log")
        run([clang, *flags, "-c", IRRLICHT / "game" / "scene_mesh_adapter.cpp",
             "-o", adapter_obj], log=logs / f"{abi}-adapter-compile.log")
        run([clang_c, f"--target={target}", f"--sysroot={sysroot}", "-fPIC",
             "-Wall", "-Wextra", "-I", str(native_glue), "-c",
             native_glue / "android_native_app_glue.c", "-o", glue_obj],
            log=logs / f"{abi}-glue-compile.log")
        library = BUILD / "native" / abi / "libGameSmoke.so"
        library.parent.mkdir(parents=True, exist_ok=True)
        run([clang, f"--target={target}", f"--sysroot={sysroot}", "-shared",
             "-Wl,--no-undefined", "-Wl,-z,max-page-size=16384",
             "-Wl,-z,common-page-size=16384", main_obj, adapter_obj, glue_obj,
             static_root / abi / "libIrrlicht.a", "-landroid", "-llog", "-lEGL",
             "-lGLESv1_CM", "-lGLESv2", "-lz", "-static-libstdc++", "-o", library],
            log=logs / f"{abi}-link.log")
        header = run([readelf, "-h", library])
        if machine not in header:
            raise RuntimeError(f"unexpected ELF machine for {abi}")
        segments = run([readelf, "-lW", library])
        aligns = [int(fields[-1], 16) for line in segments.splitlines()
                  if (fields := line.split()) and fields[0] == "LOAD"]
        if not aligns or min(aligns) < 16384:
            raise RuntimeError(f"{abi} library has insufficient PT_LOAD alignment: {aligns}")
        outputs[abi] = {"path": library.relative_to(HERE).as_posix(),
                        "bytes": library.stat().st_size,
                        "sha256": sha256(library),
                        "machine": machine, "pt_load_alignments": aligns}

    upstream_manifest = json.loads(manifest_path.read_text(encoding="utf-8"))
    upstream_records = {entry["path"]: entry for entry in upstream_manifest["files"]}
    upstream_media = IRRLICHT / "upstream" / "media"
    packaged_assets = BUILD / "apk-assets"
    shaders_output = packaged_assets / "media" / "Shaders"
    shaders_output.mkdir(parents=True, exist_ok=True)
    shader_records = []
    for shader in sorted((upstream_media / "Shaders").glob("*")):
        if not shader.is_file():
            continue
        relative = shader.relative_to(IRRLICHT / "upstream").as_posix()
        source_record = upstream_records.get(relative)
        if (not source_record or sha256(shader) != source_record["sha256"] or
                shader.stat().st_size != source_record["bytes"]):
            raise RuntimeError(f"shader differs from pinned upstream manifest: {relative}")
        target_shader = shaders_output / shader.name
        shutil.copy2(shader, target_shader)
        shader_records.append({"path": relative, "bytes": shader.stat().st_size,
                               "sha256": source_record["sha256"]})
    if not any(item["path"].endswith("/COGLES2Solid.vsh") for item in shader_records):
        raise RuntimeError("pinned upstream COGLES2Solid vertex shader is missing")
    if not any(item["path"].endswith("/COGLES2Solid.fsh") for item in shader_records):
        raise RuntimeError("pinned upstream COGLES2Solid fragment shader is missing")

    base = BUILD / "base.apk"
    run([build_tools / "aapt2.exe", "link", "--manifest", HERE / "AndroidManifest.xml",
         "-I", locate_platform(sdk), "--min-sdk-version", "26", "--target-sdk-version",
         "37", "-A", packaged_assets, "-o", base], log=logs / "aapt2-link.log")
    unsigned = BUILD / "game-smoke-unsigned.apk"
    with zipfile.ZipFile(base, "r") as source, zipfile.ZipFile(unsigned, "w") as target:
        for item in source.infolist():
            target.writestr(item, source.read(item.filename))
        for abi, _, _ in ABIS:
            target.write(HERE / outputs[abi]["path"],
                         f"lib/{abi}/libGameSmoke.so", compress_type=zipfile.ZIP_STORED)
    aligned = BUILD / "game-smoke-aligned.apk"
    run([build_tools / "zipalign.exe", "-f", "-P", "16", "4", unsigned, aligned],
        log=logs / "zipalign.log")
    java_home = Path(os.environ.get("JAVA_HOME", "C:/Program Files/Java/jdk-23"))
    keytool = java_home / "bin" / "keytool.exe"
    if not keytool.is_file():
        found = shutil.which("keytool")
        if not found:
            raise FileNotFoundError("JDK keytool required to sign the smoke APK")
        keytool = Path(found)
    keystore = BUILD / "game-smoke-debug.jks"
    if not keystore.is_file():
        run([keytool, "-genkeypair", "-keystore", keystore, "-storepass", "android",
             "-keypass", "android", "-alias", "debug", "-keyalg", "RSA", "-keysize",
             "2048", "-validity", "3650", "-dname", "CN=DH2 SceneMesh adapter smoke"],
            log=logs / "keytool.log")
    apk = BUILD / "dh2-irrlicht-adapter-smoke-debug.apk"
    run([build_tools / "apksigner.bat", "sign", "--ks", keystore, "--ks-key-alias",
         "debug", "--ks-pass", "pass:android", "--key-pass", "pass:android",
         "--out", apk, aligned], log=logs / "apksigner.log")
    verify = run([build_tools / "apksigner.bat", "verify", "--verbose", apk],
                 log=logs / "apksigner-verify.log")
    run([build_tools / "zipalign.exe", "-c", "-P", "16", "4", apk],
        log=logs / "zipalign-verify.log")
    report = {"result": "build pass", "package": PACKAGE, "min_api": 26,
              "target_api": 37, "native_compile_api": 26,
              "abis": outputs,
              "assets": {"upstream_svn_revision": upstream_manifest["svn_revision"],
                         "upstream_tree_manifest_sha256": upstream_manifest["tree_manifest_sha256"],
                         "shader_file_count": len(shader_records),
                         "shader_files": shader_records,
                         "font_note": "No external font asset is required: CGUIEnvironment loads its embedded BuiltInFontData."},
              "apk": {"path": apk.relative_to(HERE).as_posix(),
                      "bytes": apk.stat().st_size, "sha256": sha256(apk),
                      "signed": "Verifies" in verify,
                      "zipalign_page_size": 16384},
              "scope": "Synthetic SceneMesh adapter render smoke; not a DH2 asset or game integration."}

    if args.install:
        adb = sdk / "platform-tools" / "adb.exe"
        devices = run([adb, "devices", "-l"])
        if not re.search(rf"(?m)^{re.escape(args.serial)}\s+device\b", devices):
            raise RuntimeError(f"emulator {args.serial} is not online:\n{devices}")
        sdk_version = run([adb, "-s", args.serial, "shell", "getprop",
                           "ro.build.version.sdk"]).strip()
        page_size = run([adb, "-s", args.serial, "shell", "getconf", "PAGESIZE"]).strip()
        if page_size != "16384":
            raise RuntimeError(f"{args.serial} reports page size {page_size!r}, expected 16384")
        if sdk_version != "37":
            raise RuntimeError(f"{args.serial} reports Android API {sdk_version!r}, expected 37")
        run([adb, "-s", args.serial, "install", "-r", apk], log=logs / "adb-install.log")
        run([adb, "-s", args.serial, "shell", "am", "force-stop", PACKAGE])
        run([adb, "-s", args.serial, "logcat", "-c"])
        run([adb, "-s", args.serial, "shell", "am", "start", "-n",
             f"{PACKAGE}/android.app.NativeActivity"], log=logs / "adb-launch.log")
        time.sleep(12)
        screenshot = BUILD / "runtime-smoke.png"
        capture = subprocess.run([str(adb), "-s", args.serial, "exec-out",
                                  "screencap", "-p"], stdout=subprocess.PIPE,
                                 stderr=subprocess.PIPE, check=False)
        if capture.returncode:
            raise RuntimeError("screencap failed: " +
                               capture.stderr.decode("utf-8", errors="replace"))
        screenshot.write_bytes(capture.stdout)
        try:
            from PIL import Image, ImageChops
        except ImportError as exc:
            raise RuntimeError("runtime screenshot QA requires Pillow (`python -m pip install Pillow`)") from exc
        image = Image.open(screenshot).convert("RGB")
        width, height = image.size
        # This box is centered tightly on the adapter pyramid and excludes
        # the diagnostic cube to its left and the small title at the top.
        region = image.crop((int(width * 0.42), int(height * 0.22),
                             int(width * 0.58), int(height * 0.60)))
        background = Image.new("RGB", region.size, (17, 26, 40))
        difference = ImageChops.difference(region, background).convert("L")
        changed_pixels = sum(1 for pixel in difference.getdata() if pixel > 12)
        pixel_evidence = {
            "screenshot_size": [width, height],
            "checked_region_fraction": [0.42, 0.22, 0.58, 0.60],
            "clear_color_rgb": [17, 26, 40],
            "pixels_different_from_clear_color": changed_pixels,
            "minimum_required_mesh_region_pixels": 500,
            "visible_mesh_region_pass": changed_pixels >= 500,
        }
        pid = run([adb, "-s", args.serial, "shell", "pidof", PACKAGE]).strip().split()
        if not pid:
            raise RuntimeError("smoke Activity has no live process after launch")
        logcat = run([adb, "-s", args.serial, "logcat", "-d", "--pid", pid[0]],
                     log=logs / "runtime-logcat.txt")
        installed = run([adb, "-s", args.serial, "shell", "pm", "path", PACKAGE]).strip()
        remote_apk = next((line.removeprefix("package:") for line in installed.splitlines()
                           if line.startswith("package:")), None)
        if not remote_apk:
            raise RuntimeError(f"could not resolve installed APK path: {installed!r}")
        installed_copy = BUILD / "installed-base.apk"
        run([adb, "-s", args.serial, "pull", remote_apk, installed_copy],
            log=logs / "adb-pull-installed-apk.log")
        installed_hash = sha256(installed_copy)
        if installed_hash != sha256(apk):
            raise RuntimeError("installed base.apk hash differs from the built APK")
        if "PASS: SceneMesh adapter produced four buffers" not in logcat:
            raise RuntimeError("adapter smoke did not emit its PASS marker; inspect runtime logcat")
        if ("FATAL EXCEPTION" in logcat or "Fatal signal" in logcat or
                "SIGSEGV" in logcat or "SIGABRT" in logcat):
            raise RuntimeError("fatal runtime error detected; inspect runtime logcat")
        report["runtime"] = {"serial": args.serial, "sdk": 37, "page_size": 16384,
                              "activity_pid": pid[0],
                              "installed_package_path": installed,
                              "installed_apk_sha256": installed_hash,
                              "installed_apk_matches_build": True,
                              "pass_marker": True,
                              "screenshot": screenshot.relative_to(HERE).as_posix(),
                              "screenshot_sha256": sha256(screenshot),
                              "pixel_evidence": pixel_evidence,
                              "logcat": "logs/runtime-logcat.txt",
                              "logcat_excerpt": logcat[-12000:]}
        if changed_pixels < 500:
            report["result"] = "build and adapter creation pass; visible rendering NOT passed"
            report_path = BUILD / "runtime-smoke-validation.json"
            report_path.write_text(json.dumps(report, indent=2) + "\n", encoding="utf-8")
            raise RuntimeError(f"adapter pyramid is not visible in screenshot region ({changed_pixels} changed pixels)")
        report["result"] = "build and visible adapter-mesh render pass on API 37 / 16 KiB"

    report_path = BUILD / "runtime-smoke-validation.json"
    report_path.write_text(json.dumps(report, indent=2) + "\n", encoding="utf-8")
    print(json.dumps(report, indent=2))
    return 0


if __name__ == "__main__":
    try:
        raise SystemExit(main())
    except (OSError, RuntimeError, ValueError, KeyError) as exc:
        print(f"ERROR: {exc}", file=sys.stderr)
        raise SystemExit(1)
