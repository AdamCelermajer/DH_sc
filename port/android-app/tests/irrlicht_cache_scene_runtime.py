#!/usr/bin/env python3
"""Install and visually verify the local-only Irrlicht cache-scene APK."""
from __future__ import annotations

import argparse
import xml.etree.ElementTree as ET
import hashlib
import json
import os
from pathlib import Path
import re
import subprocess
import sys
import time
import zipfile

REPO = Path(__file__).resolve().parents[3]
PACKAGE = 'local.dh2.sourceviewer'
ACTIVITY = f'{PACKAGE}/android.app.NativeActivity'


def sha(path: Path) -> str:
    digest = hashlib.sha256()
    with path.open('rb') as stream:
        for block in iter(lambda: stream.read(1024 * 1024), b''):
            digest.update(block)
    return digest.hexdigest()


def run(adb: Path, serial: str, *args: object) -> str:
    completed = subprocess.run([str(adb), '-s', serial, *map(str, args)],
                               stdout=subprocess.PIPE, stderr=subprocess.STDOUT,
                               text=True, check=False)
    if completed.returncode:
        raise RuntimeError(f'adb command failed ({completed.returncode}): {args}\n'
                           + completed.stdout[-5000:])
    return completed.stdout.strip()


def capture(adb: Path, serial: str, path: Path) -> None:
    completed = subprocess.run([str(adb), '-s', serial, 'exec-out', 'screencap', '-p'],
                               stdout=subprocess.PIPE, stderr=subprocess.PIPE,
                               check=False)
    if completed.returncode or not completed.stdout.startswith(b'\x89PNG'):
        raise RuntimeError('emulator screenshot capture failed: ' +
                           completed.stderr.decode('utf-8', errors='replace'))
    path.write_bytes(completed.stdout)


def click_text(adb: Path, serial: str, text: str) -> None:
    """Click a visible text node, scrolling the diagnostics list if needed."""
    remote = '/sdcard/irrlicht-cache-scene-window.xml'
    for attempt in range(6):
        run(adb, serial, 'shell', 'uiautomator', 'dump', remote)
        xml = run(adb, serial, 'exec-out', 'cat', remote)
        try:
            root = ET.fromstring(xml)
        except ET.ParseError as exc:
            raise RuntimeError('uiautomator did not return valid window XML') from exc
        for node in root.iter('node'):
            label = node.attrib.get('text', '')
            if text not in label or node.attrib.get('clickable') != 'true':
                continue
            numbers = [int(value) for value in re.findall(r'\d+', node.attrib.get('bounds', ''))]
            if len(numbers) != 4:
                continue
            left, top, right, bottom = numbers
            if right <= left or bottom <= top or right > 10000 or bottom > 10000:
                continue
            run(adb, serial, 'shell', 'input', 'tap',
                (left + right) // 2, (top + bottom) // 2)
            return
        if attempt < 5:
            scroll = next((node for node in root.iter('node')
                           if node.attrib.get('scrollable') == 'true'), None)
            bounds = ([int(value) for value in re.findall(
                r'\d+', scroll.attrib.get('bounds', ''))] if scroll is not None else [])
            if len(bounds) == 4:
                left, top, right, bottom = bounds
                x = (left + right) // 2
                run(adb, serial, 'shell', 'input', 'swipe',
                    x, bottom - 100, x, top + 100, 400)
            else:
                run(adb, serial, 'shell', 'input', 'swipe', 900, 1200, 900, 400, 400)
            time.sleep(.4)
    raise RuntimeError(f'could not find clickable UI text: {text}')


def pixel_evidence(path: Path) -> dict:
    try:
        from PIL import Image, ImageChops
    except ImportError as exc:
        raise RuntimeError('Pillow is required for visible-texture verification') from exc
    image = Image.open(path).convert('RGB')
    width, height = image.size
    region = image.crop((int(width * .2), int(height * .16),
                         int(width * .8), int(height * .88)))
    clear = Image.new('RGB', region.size, (17, 26, 40))
    changed = ImageChops.difference(region, clear).convert('L')
    changed_pixels = sum(1 for px in changed.getdata() if px > 12)
    unique = len(region.getcolors(maxcolors=region.width * region.height) or [])
    swatch = image.crop((20, 80, 276, 336))
    swatch_unique = len(swatch.getcolors(maxcolors=swatch.width * swatch.height) or [])
    result = {'screenshot_size': [width, height],
              'checked_region_fraction': [.2, .16, .8, .88],
              'clear_color_rgb': [17, 26, 40],
              'visible_mesh_pixels': changed_pixels,
              'mesh_unique_rgb_colors': unique,
              'texture_swatch_unique_rgb_colors': swatch_unique,
              'visible_mesh_pass': changed_pixels > 500,
              'visible_mesh_texture_pass': unique > 10,
              'texture_swatch_pass': swatch_unique > 10}
    if not result['visible_mesh_pass'] or not result['visible_mesh_texture_pass']:
        raise RuntimeError(f'cache-textured geometry is not visibly verified: {result}')
    return result


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--sdk', type=Path, default=Path(
        os.environ.get('ANDROID_SDK_ROOT') or os.environ.get('ANDROID_HOME')
        or REPO.parent / 'emulator-test' / 'sdk'))
    parser.add_argument('--apk', type=Path, default=REPO / 'port/android-app/build/'
                        / 'dh2-source-renderer-irrlicht-cache-scene-local-debug.apk')
    parser.add_argument('--serial', default='emulator-5556',
                        help='online Android emulator serial (default: emulator-5556)')
    parser.add_argument('--report', type=Path, default=None,
                        help='standalone runtime report path; defaults beside the APK')
    args = parser.parse_args()
    sdk, apk = args.sdk.resolve(), args.apk.resolve(strict=True)
    adb = sdk / 'platform-tools/adb.exe'
    devices = subprocess.run([str(adb), 'devices', '-l'], text=True,
                             stdout=subprocess.PIPE, stderr=subprocess.STDOUT,
                             check=True).stdout
    serial = args.serial
    if not re.search(rf'(?m)^{re.escape(serial)}\s+device\b', devices):
        raise RuntimeError(f'{serial} must already be online and available:\n{devices}')
    release = run(adb, serial, 'shell', 'getprop', 'ro.build.version.release')
    sdk_api = run(adb, serial, 'shell', 'getprop', 'ro.build.version.sdk')
    page_size = run(adb, serial, 'shell', 'getconf', 'PAGESIZE')
    abi = run(adb, serial, 'shell', 'getprop', 'ro.product.cpu.abi')
    if (release, sdk_api, page_size, abi) not in (
            ('17', '37', '4096', 'x86_64'), ('17', '37', '16384', 'x86_64')):
        raise RuntimeError(f'expected Android 17/API 37/x86_64/4 KiB or 16 KiB, got '
                           f'{release}/{sdk_api}/{abi}/{page_size}')
    if serial == 'emulator-5556' and page_size != '4096':
        raise RuntimeError(f'{serial} is reserved for the existing 4 KiB report; got {page_size}')
    if serial == 'emulator-5558' and page_size != '16384':
        raise RuntimeError(f'{serial} is reserved for the separate 16 KiB report; got {page_size}')

    apk_hash = sha(apk)
    with zipfile.ZipFile(apk) as archive:
        names = set(archive.namelist())
        for name, expected in {
            'assets/dh2/void_maze.bdae':
                '7b67b90b65b41de96a5bd5a9a9d9d8f40cdd4425e806057a28ee5299ed2840ba',
            'assets/dh2/env_voidmaze.tga':
                'aac2c1923d2b9add49d6d8c32211d0af9d1f0a9e022530d551d1dc65d2703c20',
        }.items():
            if name not in names or hashlib.sha256(archive.read(name)).hexdigest() != expected:
                raise RuntimeError(f'APK cache asset is missing or has the wrong hash: {name}')
        for name in ('assets/media/Shaders/COGLES2Solid.vsh',
                     'assets/media/Shaders/COGLES2Solid.fsh',
                     'assets/third-party-notices/NOTICE.txt'):
            if name not in names:
                raise RuntimeError(f'APK is missing required Irrlicht resource/notice: {name}')

    run(adb, serial, 'install', '-r', apk)
    installed_spec = run(adb, serial, 'shell', 'pm', 'path', PACKAGE)
    installed_path = next((line.split(':', 1)[1] for line in installed_spec.splitlines()
                           if line.startswith('package:') and line.endswith('/base.apk')), '')
    if not installed_path:
        raise RuntimeError(f'no installed base APK path for {PACKAGE}: {installed_spec}')
    output_root = apk.parent / ('irrlicht-cache-scene' if page_size == '4096'
                                else 'irrlicht-cache-scene-16k')
    output_root.mkdir(parents=True, exist_ok=True)
    installed_copy = output_root / f'installed-base-{serial}.apk'
    run(adb, serial, 'pull', installed_path, installed_copy)
    installed_hash = sha(installed_copy)
    if installed_hash != apk_hash:
        raise RuntimeError(f'installed APK hash {installed_hash} differs from build {apk_hash}')

    run(adb, serial, 'shell', 'am', 'force-stop', PACKAGE)
    run(adb, serial, 'logcat', '-c')
    run(adb, serial, 'shell', 'am', 'start', '-W', '-n',
        f'{PACKAGE}/local.dh2.sourceviewer.MainActivity')
    time.sleep(2)
    click_text(adb, serial, 'Irrlicht adapter diagnostic')
    time.sleep(12)
    pid = run(adb, serial, 'shell', 'pidof', PACKAGE).split()
    if not pid:
        raise RuntimeError('NativeActivity process is not alive after launch')
    log = run(adb, serial, 'logcat', '-d', '--pid', pid[0])
    marker = 'PASS: actual cache BRES assembled into Irrlicht:'
    if marker not in log:
        raise RuntimeError('cache BRES success marker missing from app logcat')
    if any(word in log for word in ('FATAL EXCEPTION', 'Fatal signal', 'SIGSEGV', 'SIGABRT')):
        raise RuntimeError('fatal process error found in app logcat')
    initial_png = output_root / f'runtime-{serial}.png'
    capture(adb, serial, initial_png)
    initial_pixels = pixel_evidence(initial_png)

    run(adb, serial, 'shell', 'input', 'keyevent', '3')
    time.sleep(3)
    run(adb, serial, 'shell', 'monkey', '-p', PACKAGE, '1')
    time.sleep(5)
    resumed_pid = run(adb, serial, 'shell', 'pidof', PACKAGE).split()
    if not resumed_pid:
        raise RuntimeError('NativeActivity process did not survive Home and relaunch')
    resumed_log = run(adb, serial, 'logcat', '-d', '--pid', resumed_pid[0])
    if marker not in resumed_log:
        raise RuntimeError('cache BRES pass marker missing after Home/resume')
    if 'GL_INVALID_OPERATION' in resumed_log or 'GL_INVALID_ENUM' in resumed_log:
        raise RuntimeError('Irrlicht GLES error found after Home/resume')
    resumed_png = output_root / f'runtime-{serial}-resumed.png'
    capture(adb, serial, resumed_png)
    resumed_pixels = pixel_evidence(resumed_png)
    log_path = output_root / f'runtime-{serial}-logcat.txt'
    log_path.write_text(resumed_log, encoding='utf-8')

    result = {
        'schema': 'dh2.android.irrlicht-cache-scene-runtime.v1',
        'result': 'pass',
        'scope': 'LOCAL-ONLY cache-bearing static textured-source-draw diagnostic; not a playable level or release APK',
        'local_only': True,
        'release_eligible': False,
        'cache_assets': {
            'assets/dh2/void_maze.bdae':
                '7b67b90b65b41de96a5bd5a9a9d9d8f40cdd4425e806057a28ee5299ed2840ba',
            'assets/dh2/env_voidmaze.tga':
                'aac2c1923d2b9add49d6d8c32211d0af9d1f0a9e022530d551d1dc65d2703c20',
            'hashes_verified_in_apk': True,
        },
        'apk': {'path': apk.as_posix(), 'sha256': apk_hash,
                'installed_sha256': installed_hash,
                'installed_hash_matches': installed_hash == apk_hash,
                'bytes': apk.stat().st_size},
        'device': {'serial': serial, 'android_release': release,
                   'android_api': int(sdk_api), 'abi': abi,
                   'page_size_bytes': int(page_size), 'fold_device_tested': False},
        'activity': ACTIVITY,
        'scene_log_line': next(line for line in log.splitlines() if marker in line),
        'pixel_evidence': {'initial': initial_pixels, 'after_resume': resumed_pixels},
        'lifecycle': {'home_pause_resume_completed': True,
                      'process_alive_after_resume': True,
                      'gl_invalid_operation': resumed_log.count('GL_INVALID_OPERATION'),
                      'gl_invalid_enum': resumed_log.count('GL_INVALID_ENUM'),
                      'fatal_signal': sum(resumed_log.count(term) for term in
                                          ('FATAL EXCEPTION', 'Fatal signal', 'SIGSEGV', 'SIGABRT')),
                      'app_pid': resumed_pid[0],
                      'app_pid_filtered_log_sha256': hashlib.sha256(
                          resumed_log.encode('utf-8')).hexdigest()},
        'screenshot': resumed_png.relative_to(apk.parent).as_posix(),
        'app_pid_filtered_log': log_path.relative_to(apk.parent).as_posix(),
    }
    report_path = (args.report.resolve() if args.report else
                   output_root / 'irrlicht-cache-scene-runtime-validation.json')
    build_report = apk.parent / 'irrlicht-cache-scene-build-validation.json'
    four_k_report = apk.parent / 'irrlicht-cache-scene' / 'irrlicht-cache-scene-runtime-validation.json'
    if page_size == '16384' and report_path in (build_report.resolve(), four_k_report.resolve()):
        raise RuntimeError('16 KiB report path must remain separate from the existing 4 KiB report')
    report_path.parent.mkdir(parents=True, exist_ok=True)
    report_path.write_text(json.dumps(result, indent=2) + '\n', encoding='utf-8')
    if (page_size == '4096' and serial == 'emulator-5556' and
            build_report.is_file() and build_report.resolve() != report_path):
        report = json.loads(build_report.read_text(encoding='utf-8'))
        report['runtime'] = result
        build_report.write_text(json.dumps(report, indent=2) + '\n', encoding='utf-8')
    print(json.dumps(result, indent=2))
    return 0


if __name__ == '__main__':
    try:
        raise SystemExit(main())
    except (OSError, RuntimeError, ValueError, KeyError, zipfile.BadZipFile) as exc:
        print(f'ERROR: {exc}', file=sys.stderr)
        raise SystemExit(1)
