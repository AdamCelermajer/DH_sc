#!/usr/bin/env python3
"""Exercise Infected Village static preview open/render/orbit/return on API 37."""
from __future__ import annotations

import argparse
import hashlib
import json
from pathlib import Path
import re
import subprocess
import time
import xml.etree.ElementTree as ET


PACKAGE = "local.dh2.sourceviewer"
LAUNCHER = f"{PACKAGE}/.GameplayActivity"
PREVIEW_ACTIVITY = ".InfectedVillagePreviewActivity"
PREVIEW_BUTTON = "INFECTED VILLAGE static preview"


class Device:
    def __init__(self, adb: Path, serial: str, apk: Path, output: Path,
                 cycles: int, expected_page_size: int):
        self.adb, self.serial, self.apk, self.output = adb, serial, apk, output
        self.cycles, self.expected_page_size = cycles, expected_page_size
        self.output.mkdir(parents=True, exist_ok=True)

    def command(self, *args: str, binary: bool = False):
        result = subprocess.run([str(self.adb), "-s", self.serial, *args],
                                check=False, stdout=subprocess.PIPE,
                                stderr=subprocess.STDOUT)
        if result.returncode:
            output = result.stdout if binary else result.stdout.decode("utf-8", "replace")
            raise RuntimeError(f"adb command failed ({result.returncode}): {args!r}: {output!r}")
        return result.stdout if binary else result.stdout.decode("utf-8", "replace")

    def hierarchy(self):
        last_error = ""
        for _ in range(4):
            try:
                self.command("shell", "uiautomator", "dump", "/sdcard/dh2-village-window.xml")
                return ET.fromstring(self.command("shell", "cat", "/sdcard/dh2-village-window.xml"))
            except (RuntimeError, ET.ParseError) as error:
                last_error = str(error)
                time.sleep(0.5)
        raise RuntimeError(f"{self.serial}: could not read Android UI hierarchy: {last_error}")

    @staticmethod
    def texts(root):
        return [node.get("text", "") for node in root.iter("node")]

    def foreground(self):
        output = self.command("shell", "dumpsys", "activity", "activities")
        match = re.search(r"topResumedActivity=.*?\s((?:[A-Za-z0-9_]+\.)+[A-Za-z0-9_]+/\S+)", output)
        return match.group(1) if match else None

    def wait_activity(self, suffix: str):
        last = ""
        for _ in range(24):
            last = self.foreground() or ""
            if last.endswith(suffix):
                return last
            time.sleep(0.25)
        raise RuntimeError(f"{self.serial}: expected foreground {suffix}, got {last!r}")

    def tap(self, label: str):
        root = self.hierarchy()
        nodes = [node for node in root.iter("node") if node.get("text") == label]
        if len(nodes) != 1:
            raise RuntimeError(f"{self.serial}: expected one {label!r}, got {len(nodes)}; UI={self.texts(root)!r}")
        x1, y1, x2, y2 = map(int, re.findall(r"\d+", nodes[0].get("bounds", "")))
        time.sleep(0.15)
        self.command("shell", "input", "tap", str((x1+x2)//2), str((y1+y2)//2))

    def wait_loaded(self):
        last = ""
        for _ in range(30):
            root = self.hierarchy()
            last = "\n".join(self.texts(root))
            if ("Loaded 2 authored module roots" in last and
                    "3 checked sampler textures" in last and
                    "Static source geometry only" in last):
                return last
            if "Static source preview unavailable:" in last:
                raise RuntimeError(f"{self.serial}: source preview failed: {last}")
            time.sleep(0.35)
        raise RuntimeError(f"{self.serial}: source preview did not finish loading: {last!r}")

    def screenshot(self, name: str):
        payload = self.command("exec-out", "screencap", "-p", binary=True)
        path = self.output / name
        path.write_bytes(payload)
        return {"file": name, "bytes": len(payload), "sha256": hashlib.sha256(payload).hexdigest()}

    def run(self):
        self.command("install", "-r", str(self.apk))
        release = self.command("shell", "getprop", "ro.build.version.release").strip()
        sdk = self.command("shell", "getprop", "ro.build.version.sdk").strip()
        page_size = int(self.command("shell", "getconf", "PAGE_SIZE").strip())
        if (release, sdk, page_size) != ("17", "37", self.expected_page_size):
            raise RuntimeError(f"{self.serial}: expected Android 17/API 37/{self.expected_page_size} byte pages, got {release}/{sdk}/{page_size}")
        if self.command("shell", "getprop", "ro.kernel.qemu").strip() != "1":
            raise RuntimeError(f"{self.serial}: target is not an emulator")
        self.command("logcat", "-c")
        self.command("shell", "am", "force-stop", PACKAGE)
        self.command("shell", "am", "start", "-n", LAUNCHER)
        self.wait_activity("/.GameplayActivity")
        baseline_root = self.hierarchy()
        baseline = next((value for value in self.texts(baseline_root)
                         if value.startswith("HP ") and "Sentries " in value), None)
        if not baseline:
            raise RuntimeError(f"{self.serial}: authored encounter HUD absent before preview")

        cycles = []
        for number in range(1, self.cycles + 1):
            self.tap(PREVIEW_BUTTON)
            self.wait_activity(PREVIEW_ACTIVITY)
            loaded = self.wait_loaded()
            before = self.screenshot(f"cycle-{number:02d}-loaded.png")
            self.command("shell", "input", "swipe", "850", "700", "1420", "820", "520")
            time.sleep(0.5)
            orbit = self.screenshot(f"cycle-{number:02d}-orbit.png")
            if orbit["sha256"] == before["sha256"]:
                raise RuntimeError(f"{self.serial}: orbit gesture did not alter cycle {number}'s rendered capture")
            self.tap("Return to encounter")
            self.wait_activity("/.GameplayActivity")
            root = self.hierarchy()
            after = next((value for value in self.texts(root)
                          if value.startswith("HP ") and "Sentries " in value), None)
            if after != baseline:
                raise RuntimeError(f"{self.serial}: encounter HUD changed on cycle {number}: {baseline!r} -> {after!r}")
            cycles.append({"cycle": number, "loaded_status": loaded,
                           "preview": before, "orbit": orbit,
                           "returned": True, "hud_unchanged": True})

        log = self.command("logcat", "-d", "-t", "4000")
        errors = [line for line in log.splitlines() if any(token in line for token in
                  ("DH2InfectedVillage: E", "AndroidRuntime: E", "libEGL: E", "OpenGLRenderer: E"))]
        if errors:
            raise RuntimeError(f"{self.serial}: app/EGL/GLES error log entries: {errors}")
        installed_path = self.command("shell", "pm", "path", PACKAGE).strip().removeprefix("package:")
        pulled = self.output / "installed.apk"
        self.command("pull", installed_path, str(pulled))
        digest = lambda path: hashlib.sha256(path.read_bytes()).hexdigest()
        if digest(pulled) != digest(self.apk):
            raise RuntimeError(f"{self.serial}: installed APK does not match tested candidate")
        return {"serial": self.serial, "android_release": release, "sdk": int(sdk),
                "abi": self.command("shell", "getprop", "ro.product.cpu.abi").strip(),
                "page_size_bytes": page_size, "before_encounter_hud": baseline,
                "cycles": cycles, "filtered_error_log_entries": len(errors),
                "installed_apk_sha256": digest(pulled),
                "installed_apk_bytes": pulled.stat().st_size}


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--adb", type=Path, required=True)
    parser.add_argument("--apk", type=Path, default=Path(__file__).parents[1] / "build/dh2-source-renderer-debug.apk")
    parser.add_argument("--serial", required=True)
    parser.add_argument("--output", type=Path, required=True)
    parser.add_argument("--cycles", type=int, default=3)
    parser.add_argument("--page-size", type=int, choices=(4096, 16384), default=16384)
    args = parser.parse_args()
    if args.cycles < 1 or args.cycles > 10:
        parser.error("--cycles must be between 1 and 10")
    device = Device(args.adb.resolve(), args.serial, args.apk.resolve(), args.output.resolve(),
                    args.cycles, args.page_size)
    report = {"apk_sha256": hashlib.sha256(args.apk.read_bytes()).hexdigest(),
              "apk_bytes": args.apk.stat().st_size, "check": device.run()}
    args.output.mkdir(parents=True, exist_ok=True)
    (args.output / "result.json").write_text(json.dumps(report, indent=2) + "\n", encoding="utf-8")
    print(json.dumps(report, indent=2))


if __name__ == "__main__":
    main()
