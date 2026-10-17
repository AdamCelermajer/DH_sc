#!/usr/bin/env python3
"""Exercise all pinned infected actor/clip pairs through the Android 17 UI."""
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
LAUNCHER = f"{PACKAGE}/local.dh2.sourceviewer.GameplayActivity"
ACTOR_BUTTON = "Infected actor variants diagnostic"
MODELS = ["InfectedVillager", "InfectedBurned", "InfectedBlacksmith",
          "InfectedMaid", "InfectedMerchant", "InfectedNun"]
CLIPS = ["Idle A", "Idle B", "Walk"]


class Device:
    def __init__(self, adb: Path, serial: str, apk: Path, output: Path,
                 expected_page_size: int):
        self.adb, self.serial, self.apk, self.output = adb, serial, apk, output
        self.expected_page_size = expected_page_size
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
                self.command("shell", "uiautomator", "dump", "/sdcard/dh2-actor-window.xml")
                data = self.command("shell", "cat", "/sdcard/dh2-actor-window.xml")
                return ET.fromstring(data)
            except (RuntimeError, ET.ParseError) as error:
                last_error = str(error)
                time.sleep(0.6)
        raise RuntimeError(f"{self.serial}: could not read Android UI hierarchy: {last_error}")

    @staticmethod
    def text(root):
        return "\n".join(node.get("text", "") for node in root.iter("node"))

    def tap(self, label: str):
        last = ""
        for _ in range(4):
            root = self.hierarchy()
            nodes = [node for node in root.iter("node") if node.get("text") == label]
            if len(nodes) == 1:
                if nodes[0].get("clickable") != "true" or nodes[0].get("enabled") != "true":
                    raise RuntimeError(f"{self.serial}: {label!r} is visible but not an enabled click target")
                x1, y1, x2, y2 = map(int, re.findall(r"\d+", nodes[0].get("bounds", "")))
                time.sleep(0.2)
                self.command("shell", "input", "tap", str((x1 + x2) // 2), str((y1 + y2) // 2))
                return
            last = self.text(root)
            time.sleep(0.5)
        raise RuntimeError(f"{self.serial}: expected one {label!r}; UI text={last!r}")

    def wait_activity(self, activity: str, attempts: int = 60):
        last = ""
        for _ in range(attempts):
            last = self.command("shell", "dumpsys", "activity", "activities")
            foreground = next((line for line in last.splitlines()
                               if "topResumedActivity=" in line), "")
            if activity in foreground:
                return
            time.sleep(0.25)
        raise RuntimeError(f"{self.serial}: expected {activity} as top resumed; "
                           f"foreground={foreground!r}; activity dump={last[-1000:]}")

    def wait_status(self, model: int, clip: int, attempts: int = 12):
        expected = f"Loaded model {model + 1} / clip {clip + 1}"
        last = ""
        for _ in range(attempts):
            root = self.hierarchy()
            last = self.text(root)
            if expected in last:
                return root
            if "Actor preview validation failed" in last or "missing or too large" in last:
                raise RuntimeError(f"{self.serial}: actor load failed: {last}")
            time.sleep(0.2)
        raise RuntimeError(f"{self.serial}: expected {expected!r}; UI text={last!r}")

    def screenshot(self, name: str):
        payload = self.command("exec-out", "screencap", "-p", binary=True)
        path = self.output / name
        path.write_bytes(payload)
        return {"file": name, "bytes": len(payload),
                "sha256": hashlib.sha256(payload).hexdigest()}

    def run(self):
        self.command("install", "-r", str(self.apk))
        sdk = self.command("shell", "getprop", "ro.build.version.sdk").strip()
        release = self.command("shell", "getprop", "ro.build.version.release").strip()
        page_size = self.command("shell", "getconf", "PAGE_SIZE").strip()
        if sdk != "37" or release != "17" or page_size != str(self.expected_page_size):
            raise RuntimeError(f"{self.serial}: expected Android 17/API 37/{self.expected_page_size} byte pages, got {release}/{sdk}/{page_size}")
        self.command("shell", "am", "force-stop", PACKAGE)
        self.command("shell", "am", "start", "-W", "-n", LAUNCHER)
        self.wait_activity("GameplayActivity")
        time.sleep(0.8)
        self.tap(ACTOR_BUTTON)
        self.wait_activity("InfectedActorPreviewActivity")

        tested = []
        shots = []
        current_model, current_clip = 0, 0
        self.wait_status(current_model, current_clip)
        shots.append(self.screenshot("actor-01-infected-idle-a.png"))
        tested.append({"model": MODELS[current_model], "clip": CLIPS[current_clip]})
        print(f"PASS {MODELS[current_model]} / {CLIPS[current_clip]}", flush=True)

        # Continue the clip cycle twice per model, then increment the model.
        # This visits each of the 18 pairs exactly once because the clip index
        # carries across the model boundary.
        for model in range(6):
            for _ in range(2):
                self.tap("Next clip")
                current_clip = (current_clip + 1) % 3
                self.wait_status(current_model, current_clip)
                shots.append(self.screenshot(f"actor-{len(shots)+1:02d}-{MODELS[current_model]}-{CLIPS[current_clip].lower().replace(' ', '-')}.png"))
                tested.append({"model": MODELS[current_model], "clip": CLIPS[current_clip]})
                print(f"PASS {MODELS[current_model]} / {CLIPS[current_clip]}", flush=True)
            if model < 5:
                self.tap("Next model")
                current_model += 1
                self.wait_status(current_model, current_clip)
                shots.append(self.screenshot(f"actor-{len(shots)+1:02d}-{MODELS[current_model]}-{CLIPS[current_clip].lower().replace(' ', '-')}.png"))
                tested.append({"model": MODELS[current_model], "clip": CLIPS[current_clip]})
                print(f"PASS {MODELS[current_model]} / {CLIPS[current_clip]}", flush=True)

        if len({(row["model"], row["clip"]) for row in tested}) != 18:
            raise RuntimeError(f"did not visit every actor/clip pair: {tested}")
        self.command("shell", "input", "swipe", "1050", "720", "1410", "610", "450")
        time.sleep(0.4)
        shots.append(self.screenshot("actor-orbit.png"))
        after_orbit = self.text(self.hierarchy())
        if f"Loaded model 6 / clip {current_clip + 1}" not in after_orbit:
            raise RuntimeError(f"actor changed unexpectedly during orbit gesture: {after_orbit}")
        self.tap("Return to encounter")
        time.sleep(0.5)
        foreground = self.command("shell", "dumpsys", "activity", "activities")
        if "topResumedActivity=" not in foreground or "GameplayActivity" not in foreground:
            raise RuntimeError(f"actor screen did not return to gameplay: {foreground[-800:]}")

        installed_path = self.command("shell", "pm", "path", PACKAGE).strip().removeprefix("package:")
        installed_apk = self.output / "installed.apk"
        self.command("pull", installed_path, str(installed_apk))
        source_hash = hashlib.sha256(self.apk.read_bytes()).hexdigest()
        installed_hash = hashlib.sha256(installed_apk.read_bytes()).hexdigest()
        if source_hash != installed_hash:
            raise RuntimeError(f"installed APK hash mismatch: {source_hash} != {installed_hash}")
        return {
            "serial": self.serial, "android_release": release, "api_level": int(sdk),
            "page_size": int(page_size), "apk_sha256": source_hash,
            "apk_bytes": self.apk.stat().st_size, "installed_apk_sha256": installed_hash,
            "model_count": 6, "clip_count": 3, "pairs_tested": len(tested),
            "tested_pairs": tested, "screenshots": shots,
            "returned_to_gameplay": True,
        }


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--adb", type=Path, required=True)
    parser.add_argument("--apk", type=Path,
                        default=Path(__file__).parents[1] / "build/dh2-source-renderer-debug.apk")
    parser.add_argument("--serial", required=True)
    parser.add_argument("--output", type=Path, required=True)
    parser.add_argument("--page-size", type=int, choices=(4096, 16384), default=16384)
    args = parser.parse_args()
    device = Device(args.adb.resolve(), args.serial, args.apk.resolve(), args.output.resolve(), args.page_size)
    result = device.run()
    (args.output.resolve() / "result.json").write_text(json.dumps(result, indent=2) + "\n", encoding="utf-8")
    print(json.dumps(result, indent=2))


if __name__ == "__main__":
    main()
