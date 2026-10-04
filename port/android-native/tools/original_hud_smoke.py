"""Verify the authored initial health panel on one unchanged emulator APK.

This inspection does not claim original viewport publication, live game values,
HUD input, full menus or physical ARM64 behavior. Preserve screenshots and logs
on failure; do not rebuild, clear logcat or retry rendering failures.
"""
import argparse
import hashlib
import json
from pathlib import Path
import re
import subprocess
import time
import xml.etree.ElementTree as ET
from PIL import Image, ImageChops
from emulator_smoke import inspect, launch_fresh

FRAME = re.compile(r'Original health panel submitted \| viewport (\d+) (\d+) '
                   r'\| strips (\d+) \| lines (\d+) \| masks (\d+) '
                   r'\| font uploads (\d+) \| bitmaps (\d+) \| strings (\d+) '
                   r'\| core diagnostics (\d+)')
OWNER = re.compile(r'Original UI retained owner \| session (0x[0-9a-f]+) '
                   r'\| movie (0x[0-9a-f]+) \| fonts (0x[0-9a-f]+) \| retained ([01])')


def main():
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument('--adb', required=True)
    p.add_argument('--serial', required=True)
    p.add_argument('--apk', type=Path, required=True)
    p.add_argument('--output', type=Path, required=True)
    a = p.parse_args()
    assert a.serial.startswith('emulator-')
    assert not a.output.exists(), 'Preserve existing HUD observations'
    a.output.mkdir(parents=True)
    digest = hashlib.sha256(a.apk.read_bytes()).hexdigest()

    def adb(*args):
        r = subprocess.run([a.adb, '-s', a.serial, *args], capture_output=True,
                           text=True, timeout=45)
        assert r.returncode == 0, (args, r.stdout, r.stderr)
        return r.stdout.strip()

    libraries = inspect(a.apk)
    assert len(libraries) == 18
    remote = adb('shell', 'pm', 'path', 'com.example.dh2').removeprefix('package:')
    assert remote.startswith('/') and '\n' not in remote
    installed = adb('shell', 'sha256sum', remote).split()[0]
    assert installed == digest, 'Install the exact candidate before this test'
    prior = adb('shell', 'cmd', 'window', 'user-rotation').split()
    observations = []
    owner = None
    pid = None

    def capture(label, landscape=False):
        nonlocal owner
        deadline = time.monotonic() + 30
        while True:
            logs = adb('logcat', '-d', '--pid='+pid, '-v', 'brief')
            (a.output/(label+'.log')).write_text(logs+'\n', encoding='utf-8')
            assert not re.search(r'FATAL EXCEPTION|Fatal signal|GL error|Original HUD (?:load|draw) failed|Font provider failed', logs), logs
            adb('shell', 'uiautomator', 'dump', '/sdcard/dh2-hud-window.xml')
            xml = adb('shell', 'cat', '/sdcard/dh2-hud-window.xml')
            root = ET.fromstring(xml)
            view = next(n for n in root.iter('node') if n.get('content-desc') == 'DH2 native texture viewport')
            bounds = tuple(map(int, re.findall(r'\d+', view.get('bounds'))))
            width, height = bounds[2]-bounds[0], bounds[3]-bounds[1]
            frames = FRAME.findall(logs)
            if frames and tuple(map(int, frames[-1][:2])) == (width, height) and (width > height) == landscape:
                break
            assert time.monotonic() < deadline, 'No current HUD submission: '+logs
            time.sleep(.2)
        owners = OWNER.findall(logs)
        assert owners
        if owner is None:
            owner = owners[-1][:3]
            assert owners[-1][3] == '0'
        else:
            assert owners[-1][:3] == owner and owners[-1][3] == '1', owners
        frame = list(map(int, frames[-1]))
        assert frame[2] > 0 and frame[5] > 0 and frame[6] > 0 and frame[7] > 0
        assert 'Original UI packed glyph decoded' in logs
        adb('shell', 'screencap', '-p', '/sdcard/dh2-hud.png')
        screenshot = a.output/(label+'.png')
        adb('pull', '/sdcard/dh2-hud.png', str(screenshot))
        picture = Image.open(screenshot).convert('RGB').crop(bounds)
        colors = picture.getcolors(picture.width*picture.height)
        # Authored frame zero has empty bars. Live status frames are deliberately
        # unconnected, so do not require red/blue fill in this inspection.
        red = sum(n for n,(r,g,b) in colors if r > 60 and r > 1.5*g and r > 1.3*b)
        blue = sum(n for n,(r,g,b) in colors if b > 60 and b > 1.3*r and b > 1.2*g)
        coverage = ImageChops.difference(picture,Image.new('RGB',picture.size,(255,255,255)))
        coverage = coverage.convert('L').point(lambda v:255 if v>8 else 0)
        panel_bounds = coverage.getbbox()
        panel_pixels = sum(coverage.histogram()[1:])
        assert len(colors) > 128 and panel_bounds and panel_pixels > 1000, (len(colors),panel_pixels)
        (a.output/(label+'.xml')).write_text(xml, encoding='utf-8')
        observations.append(dict(label=label, viewport=[width,height], owner=owner,
                                 submissions=frame, unique_colors=len(colors),
                                 red_pixels=red, blue_pixels=blue,
                                 panel_bounds=panel_bounds, panel_pixels=panel_pixels,
                                 screenshot=screenshot.name,
                                 screenshot_sha256=hashlib.sha256(screenshot.read_bytes()).hexdigest()))
        (a.output/'observations.json').write_text(json.dumps(observations, indent=2)+'\n')

    try:
        adb('shell', 'cmd', 'window', 'user-rotation', 'lock', '0')
        launch = launch_fresh(adb, '--ez', 'original_hud', 'true')
        (a.output/'launch.txt').write_text(launch+'\n')
        deadline = time.monotonic()+10
        while not pid:
            r = subprocess.run([a.adb,'-s',a.serial,'shell','pidof','com.example.dh2'],capture_output=True,text=True)
            pid = r.stdout.strip()
            assert time.monotonic() < deadline
            if not pid: time.sleep(.1)
        capture('portrait')
        adb('shell', 'cmd', 'window', 'user-rotation', 'lock', '1')
        capture('landscape', True)
        adb('shell', 'input', 'keyevent', 'KEYCODE_HOME')
        time.sleep(.5)
        adb('shell', 'am', 'start', '-W', '-n', 'com.example.dh2/.MainActivity')
        capture('resumed', True)
        assert adb('shell', 'pidof', 'com.example.dh2') == pid
        assert hashlib.sha256(a.apk.read_bytes()).hexdigest() == digest
        report = dict(validation='PASS', apk_sha256=digest, installed_apk_sha256=installed,
                      serial=a.serial, android_api=adb('shell','getprop','ro.build.version.sdk'),
                      abi=adb('shell','getprop','ro.product.cpu.abi'), cases=observations,
                      libraries=libraries, authored_initial_health_panel=True,
                      live_game_values=False, original_viewport_publication=False,
                      HUD_input=False, physical_arm64_verified=False,
                      error_recovery_fault_injection=False)
        (a.output/'original-hud-smoke.json').write_text(json.dumps(report,indent=2)+'\n')
        print(json.dumps(dict(validation='PASS', apk_sha256=digest, cases=len(observations))))
    finally:
        if prior and prior[0] in ('free','lock'):
            adb('shell','cmd','window','user-rotation',*prior)


if __name__ == '__main__':
    main()
