#!/usr/bin/env python3
"""Check that a built APK contains the exact launch/menu assets."""

import argparse
import hashlib
import zipfile
from pathlib import Path


ROOT = Path(__file__).resolve().parents[3]
ASSETS = ROOT / "port/android-native/app/src/main/assets"

EXPECTED = {
    "original-media/intro.mp4": (20_248_857, "859f2dde32401f996a5faeaf8c914965b5410d0200315c0bfb90d8c71aa59b22", True),
    "original-media/intro_jp.mp4": (19_558_308, "771ad97dea5eaeae073afeebc3cdbb853a1860616bc43cb98fea4493902c8999", True),
    "original-media/intro_kr.mp4": (19_715_884, "fa99665ed6a640af915220386967da4e23b80df611961400589d06dad9cdd178", True),
    "original-cache/data/3d/textures/splash_final.tga": (8_388_652, "2a296425a7072f22b6ac0ec9344b441599ebc7b5497dc22e671c791f455df2a8", False),
    "original-cache/data/3d/textures/splash_final_jp.tga": (8_388_626, "e7d9b24f61dc82b22bb216c723479f0d5452c3d2ed5c8f8b2cce8a3d1966c3c3", False),
    "original-cache/data/3d/textures/splash_final_kor.tga": (16_777_260, "ce029bd1e221843d8c1ad4521189d0ce0870548914e9d53c01c461068d6c3b07", False),
    "original-media/title_intro.wav": (746_622, "424e61941a95020cf499dd933fd064b1357bc459702d984ced84af2be752e2f2", True),
    "original-media/title_loop.wav": (14_491_470, "2ad9ec9422c4a0f1c45c66abcaadde32d968d7bec60f02168c57767b9592dc4d", True),
    "original-cache/data/menus/dqmenus_droid.swf": (315_965, "c114c7d39b7fe0351a93a515c78f9457dda715d78aed6e3bcc3f4895120de421", False),
    "original-cache/data/menus/dqshared_droid.swf": (67_525, "f0f9f119f2fe474d21c722e361ef9907ef2b885654705073e0cb956432f46263", False),
    "original-cache/data/menus/dqcharmenu_droid.swf": (248_432, "43227075f407626b52eca4c345ac6f568023fcb39c269588201026c0fc6760e0", False),
    "front-compat/dqmenus_droid.swf": (315_955, "f01cbeec730848d57aed8923b51ed2c927403687122e870878d713a1d389d430", False),
    "worlds/crypt01.dwld": (1_048, "0041f14cef4acd5a438fa0f14a687529b4f3714526c0a4042ed7744a212e22da", False),
}


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("apk", type=Path, help="APK produced by the build being checked")
    args = parser.parse_args()
    if not args.apk.is_file():
        parser.error(f"APK not found: {args.apk}")

    with zipfile.ZipFile(args.apk) as apk:
        for relative, (size, digest, must_be_stored) in EXPECTED.items():
            name = f"assets/{relative}"
            try:
                info = apk.getinfo(name)
                payload = apk.read(info)
            except KeyError as exc:
                raise SystemExit(f"FAIL missing APK entry: {name}") from exc
            actual_hash = hashlib.sha256(payload).hexdigest()
            source = ASSETS / relative
            if len(payload) != size or actual_hash != digest:
                raise SystemExit(f"FAIL APK bytes differ: {name} size={len(payload)} sha256={actual_hash}")
            if not source.is_file() or source.stat().st_size != size or hashlib.sha256(source.read_bytes()).hexdigest() != digest:
                raise SystemExit(f"FAIL packaged source asset differs: {relative}")
            if must_be_stored and info.compress_type != zipfile.ZIP_STORED:
                raise SystemExit(f"FAIL MediaPlayer openFd asset is compressed: {name}")
            print(f"PASS {name} ({size} bytes)")
    print(f"PASS launch assets verified in {args.apk}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
