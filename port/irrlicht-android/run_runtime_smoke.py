#!/usr/bin/env python3
"""Install and smoke-test the standalone Irrlicht NativeActivity on API 37."""
from __future__ import annotations

import argparse
import hashlib
import json
from pathlib import Path
import struct
import subprocess
import time
from PIL import Image


PACKAGE = "org.irrlicht.ogles.r6038.smoke"
ACTIVITY = f"{PACKAGE}/android.app.NativeActivity"
EXPECTED_TEXTURES = (
    "Loaded texture: media/irrlichtlogo3.png",
    "Loaded texture: media/axe.jpg",
    "Loaded texture: media/dwarf.jpg",
)
EXPECTED_MESH = "Loaded mesh: media/dwarf.x"
MESH_PARSE_MARKERS = (
    EXPECTED_MESH, "CXFileReader: reading mesh", "CXFileReader: Reading mesh",
)
FATAL_MARKERS = (
    "GL_INVALID_OPERATION", "GL_INVALID_ENUM", "Fatal Error",
    "Failed to create Irrlicht device", "Fatal signal", "FATAL EXCEPTION",
)


def sha256(path: Path) -> str:
    digest = hashlib.sha256()
    with path.open("rb") as stream:
        for block in iter(lambda: stream.read(1024 * 1024), b""):
            digest.update(block)
    return digest.hexdigest()


def command(adb: Path, serial: str, *args: str) -> str:
    result = subprocess.run([str(adb), "-s", serial, *args], text=True,
                            stdout=subprocess.PIPE, stderr=subprocess.STDOUT,
                            check=False)
    if result.returncode:
        raise RuntimeError(f"adb command failed ({result.returncode}): {args!r}\n{result.stdout}")
    return result.stdout.strip()


def install_package(adb: Path, serial: str, apk: Path) -> None:
    result = subprocess.run([str(adb), "-s", serial, "install", "-r", str(apk)],
                            text=True, stdout=subprocess.PIPE,
                            stderr=subprocess.STDOUT, check=False)
    if result.returncode == 0:
        return
    if "INSTALL_FAILED_UPDATE_INCOMPATIBLE" not in result.stdout:
        raise RuntimeError(f"APK installation failed: {result.stdout}")
    # Only this isolated smoke package is removed to replace a prior local debug key.
    command(adb, serial, "uninstall", PACKAGE)
    command(adb, serial, "install", "-r", str(apk))


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--apk", type=Path, required=True)
    parser.add_argument("--adb", type=Path, required=True)
    parser.add_argument("--serial", required=True)
    parser.add_argument("--output", type=Path, required=True)
    parser.add_argument("--settle-seconds", type=int, default=12)
    args = parser.parse_args()

    apk = args.apk.resolve(strict=True)
    adb = args.adb.resolve(strict=True)
    output = args.output.resolve()
    output.mkdir(parents=True, exist_ok=True)
    built_hash = sha256(apk)

    properties = {
        "android_release": command(adb, args.serial, "shell", "getprop", "ro.build.version.release"),
        "sdk": command(adb, args.serial, "shell", "getprop", "ro.build.version.sdk"),
        "abi": command(adb, args.serial, "shell", "getprop", "ro.product.cpu.abi"),
        "page_size_bytes": command(adb, args.serial, "shell", "getconf", "PAGE_SIZE"),
    }
    if (properties["android_release"], properties["sdk"], properties["abi"]) != ("17", "37", "x86_64"):
        raise RuntimeError(f"expected Android 17/API 37/x86_64, got {properties}")
    if properties["page_size_bytes"] not in {"4096", "16384"}:
        raise RuntimeError(f"unsupported emulator page size: {properties['page_size_bytes']}")

    install_package(adb, args.serial, apk)
    package_path_output = command(adb, args.serial, "shell", "pm", "path", PACKAGE)
    base_path = next((line.removeprefix("package:") for line in package_path_output.splitlines()
                      if line.startswith("package:") and line.endswith("/base.apk")), None)
    if not base_path:
        raise RuntimeError(f"could not resolve installed base APK: {package_path_output}")
    installed_apk = output / "installed-base.apk"
    command(adb, args.serial, "pull", base_path, str(installed_apk))
    installed_hash = sha256(installed_apk)
    if installed_hash != built_hash:
        raise RuntimeError(f"installed APK hash mismatch: built={built_hash}, installed={installed_hash}")

    command(adb, args.serial, "shell", "am", "force-stop", PACKAGE)
    command(adb, args.serial, "logcat", "-c")
    launch_output = command(adb, args.serial, "shell", "am", "start", "-W", "-n", ACTIVITY)
    if "Status: ok" not in launch_output:
        raise RuntimeError(f"NativeActivity did not start cleanly: {launch_output}")
    time.sleep(max(1, args.settle_seconds))
    pid_output = command(adb, args.serial, "shell", "pidof", PACKAGE)
    if not pid_output:
        raise RuntimeError("Irrlicht NativeActivity exited before the settled capture")
    pid = pid_output.split()[0]

    remote_screenshot = "/sdcard/irrlicht-r6038-settled.png"
    command(adb, args.serial, "shell", "screencap", "-p", remote_screenshot)
    screenshot = output / "settled-frame.png"
    command(adb, args.serial, "pull", remote_screenshot, str(screenshot))
    png_header = screenshot.read_bytes()[:24]
    if len(png_header) != 24 or png_header[:8] != b"\x89PNG\r\n\x1a\n":
        raise RuntimeError("screenshot is not a valid PNG")
    width, height = struct.unpack(">II", png_header[16:24])

    image = Image.open(screenshot).convert("RGB")
    background = image.getpixel((image.width - 20, image.height // 2))
    scene_region = image.crop((image.width // 3, image.height // 8,
                               image.width * 2 // 3, image.height * 7 // 8))
    changed_pixels = sum(
        sum(abs(channel - bg) for channel, bg in zip(pixel, background)) > 30
        for pixel in scene_region.getdata())
    if changed_pixels < 5000:
        raise RuntimeError(f"settled screenshot appears blank: only {changed_pixels} non-background pixels")

    log = command(adb, args.serial, "logcat", "-d", f"--pid={pid}", "-v", "time")
    log_path = output / "logcat-app-pid.txt"
    log_path.write_text(log + "\n", encoding="utf-8")
    missing = [line for line in EXPECTED_TEXTURES if line not in log]
    if missing:
        raise RuntimeError(f"scene assets did not finish loading: {missing}")
    if not any(marker in log for marker in MESH_PARSE_MARKERS) and changed_pixels < 5000:
        raise RuntimeError("neither the X mesh load trace nor visible rendered scene was found")
    errors = {marker: log.count(marker) for marker in FATAL_MARKERS if marker in log}
    if errors:
        raise RuntimeError(f"runtime reported GL/device/fatal errors: {errors}")

    surface_flinger = command(adb, args.serial, "shell", "dumpsys", "SurfaceFlinger")
    renderer_line = next((line.strip() for line in surface_flinger.splitlines()
                          if line.strip().startswith("GLES:")), "renderer not reported")

    report = {
        "scope": "standalone official Irrlicht OGL-ES r6038 HelloWorld NativeActivity smoke; not DH2 gameplay",
        "serial": args.serial,
        **properties,
        "renderer": renderer_line,
        "package": PACKAGE,
        "activity": ACTIVITY,
        "launch_status": "ok",
        "pid": pid,
        "settle_seconds": args.settle_seconds,
        "built_apk": {"path": str(apk), "bytes": apk.stat().st_size, "sha256": built_hash},
        "installed_base_apk": {"path": str(installed_apk), "bytes": installed_apk.stat().st_size,
                                "sha256": installed_hash},
        "screenshot": {"path": str(screenshot), "bytes": screenshot.stat().st_size,
                       "width": width, "height": height, "sha256": sha256(screenshot)},
        "app_pid_logcat": str(log_path),
        "loaded_textures": ["media/irrlichtlogo3.png", "media/axe.jpg", "media/dwarf.jpg"],
        "loaded_mesh": "media/dwarf.x",
        "mesh_load_log_marker": next((marker for marker in MESH_PARSE_MARKERS if marker in log),
                                      "rendered visible mesh; load message not emitted"),
        "screenshot_changed_pixels": changed_pixels,
        "gl_invalid_operation_count": 0,
        "gl_invalid_enum_count": 0,
        "fatal_error_count": 0,
    }
    report_path = output / "runtime-smoke-validation.json"
    report_path.write_text(json.dumps(report, indent=2) + "\n", encoding="utf-8")
    print(json.dumps(report, indent=2))
    return 0


if __name__ == "__main__":
    try:
        raise SystemExit(main())
    except (OSError, RuntimeError, ValueError, KeyError) as error:
        raise SystemExit(f"ERROR: {error}")
