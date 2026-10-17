#!/usr/bin/env python3
"""Exercise SWAMP movement and the bounded LizardMan_Intro trace on API 37."""
from __future__ import annotations

import argparse
import hashlib
import json
from pathlib import Path
import re
import subprocess
import time
import xml.etree.ElementTree as ET

PACKAGE = 'local.dh2.sourceviewer'
COMPONENT = PACKAGE + '/.GameplayActivity'


class Device:
    def __init__(self, adb: Path, serial: str, apk: Path, output: Path):
        self.adb = adb
        self.serial = serial
        self.apk = apk
        self.output = output
        self.output.mkdir(parents=True, exist_ok=True)

    def command(self, *args: str, binary: bool = False):
        return subprocess.check_output(
            [str(self.adb), '-s', self.serial, *args],
            timeout=60, stderr=subprocess.STDOUT,
            text=not binary, encoding=None if binary else 'utf-8',
            errors=None if binary else 'replace')

    def hierarchy(self):
        self.command('shell', 'uiautomator', 'dump', '/sdcard/swamp-intro-trace.xml')
        raw = self.command('shell', 'cat', '/sdcard/swamp-intro-trace.xml')
        return ET.fromstring(raw)

    def foreground_activity(self):
        output = self.command('shell', 'dumpsys', 'activity', 'activities')
        match = re.search(r'topResumedActivity=.*?\s((?:[A-Za-z0-9_]+\.)+[A-Za-z0-9_]+/\S+)', output)
        return match.group(1) if match else None

    @staticmethod
    def nodes(root):
        return list(root.iter('node'))

    def find_node(self, text: str, root=None):
        if root is None:
            root = self.hierarchy()
        matches = [node for node in self.nodes(root) if node.get('text') == text]
        if len(matches) != 1:
            raise RuntimeError(f'{self.serial}: expected one node {text!r}, found {len(matches)}')
        return matches[0]

    def tap(self, node):
        x1, y1, x2, y2 = map(int, re.findall(r'\d+', node.get('bounds', '')))
        time.sleep(0.2)
        self.command('shell', 'input', 'tap', str((x1 + x2) // 2), str((y1 + y2) // 2))

    def wait_for(self, text: str, attempts: int = 8):
        last = None
        for _ in range(attempts):
            last = self.hierarchy()
            matches = [node for node in self.nodes(last) if node.get('text') == text]
            if len(matches) == 1:
                return last
            time.sleep(0.3)
        raise RuntimeError(f'{self.serial}: did not find {text!r}; hierarchy={ET.tostring(last, encoding="unicode")[:2000]}')

    def text_status(self, root):
        values = [node.get('text', '') for node in self.nodes(root)]
        return next((value for value in values if value.startswith('HP ') and 'Sentries ' in value), None)

    def screenshot(self, name: str):
        data = self.command('exec-out', 'screencap', '-p', binary=True)
        path = self.output / name
        path.write_bytes(data)
        return {'file': name, 'bytes': len(data), 'sha256': hashlib.sha256(data).hexdigest()}

    def movement_node(self, root=None):
        if root is None:
            root = self.hierarchy()
        matches = [node for node in self.nodes(root)
                   if node.get('content-desc') == 'Source movement status']
        if len(matches) != 1:
            raise RuntimeError(f'{self.serial}: expected one source movement status, found {len(matches)}')
        return matches[0]

    def described_node(self, root, description):
        matches = [node for node in self.nodes(root)
                   if node.get('content-desc') == description]
        if len(matches) != 1:
            raise RuntimeError(f'{self.serial}: expected one node described {description!r}, found {len(matches)}')
        return matches[0]

    @staticmethod
    def position_from_report(report: str):
        match = re.search(r'X (-?\d+(?:\.\d+)?)\s+Y (-?\d+(?:\.\d+)?)\s+Z (-?\d+(?:\.\d+)?)', report)
        if not match:
            raise RuntimeError(f'movement report has no source XYZ: {report!r}')
        return tuple(float(value) for value in match.groups())

    def wait_movement(self, predicate, attempts=12):
        last_root = None
        last_text = ''
        for _ in range(attempts):
            last_root = self.hierarchy()
            last_text = self.movement_node(last_root).get('text', '')
            if predicate(last_text):
                return last_root, last_text
            time.sleep(0.1)
        raise RuntimeError(f'{self.serial}: movement status did not reach the expected state: {last_text!r}')

    def run_axis_swipe(self, root, sx: float, sy: float, duration_ms: int,
                       name: str):
        pad = next((node for node in self.nodes(root)
                    if node.get('content-desc') == 'Movement pad'), None)
        if pad is None:
            raise RuntimeError(f'{self.serial}: movement pad was absent')
        x1, y1, x2, y2 = map(int, re.findall(r'\d+', pad.get('bounds', '')))
        center_x, center_y = (x1+x2)//2, (y1+y2)//2
        radius = min(x2-x1, y2-y1)*0.36*0.92
        end_x = round(center_x + sx*radius)
        end_y = round(center_y - sy*radius)
        before_text = self.movement_node(root).get('text', '')
        before = self.position_from_report(before_text)
        command = [str(self.adb), '-s', self.serial, 'shell', 'input', 'swipe',
                   str(center_x), str(center_y), str(end_x), str(end_y), str(duration_ms)]
        process = subprocess.Popen(command, stdout=subprocess.PIPE,
                                   stderr=subprocess.STDOUT, text=True)
        # Capture while the shell input swipe is still holding the pad. Avoid
        # dumping the accessibility hierarchy here: uiautomator dump can take
        # longer than a short swipe, so by the time it returns the player's HUD
        # has correctly changed back to IDLE.
        time.sleep(0.20)
        walk_shot = self.screenshot(name+'-walk.png')
        process.wait(timeout=max(10, duration_ms/1000+5))
        if process.returncode:
            raise RuntimeError(f'{self.serial}: touch swipe failed: {process.stdout.read()}')
        after_root, after_text = self.wait_movement(lambda text: 'IDLE' in text, attempts=8)
        after = self.position_from_report(after_text)
        release_shot = self.screenshot(name+'-release-idle.png')
        time.sleep(0.5)
        _, settled_text = self.wait_movement(lambda text: 'IDLE' in text, attempts=4)
        settled = self.position_from_report(settled_text)
        if any(abs(settled[i]-after[i]) > 0.02 for i in range(3)):
            raise RuntimeError(f'{self.serial}: player drifted after touch release: {after} -> {settled}')
        return {
            'before_xyz': before,
            # The walk screenshot is captured while the touch command is held;
            # heading persists after release and is read from the idle report.
            # The HUD is sampled after the Android swipe returns, when input
            # has been released and IDLE is expected. The paired screenshot
            # above is the visual capture taken while touch remains held.
            'post_release_report': after_text,
            'after_release_xyz': after,
            'after_release_stable_xyz': settled,
            'walk_screenshot': walk_shot,
            'release_idle_screenshot': release_shot,
        }

    def movement_check(self, root):
        east_test = self.run_axis_swipe(root, 1.0, 0.0, 1400, 'movement-east')
        east = east_test['after_release_xyz']
        start = east_test['before_xyz']
        if east[0] <= start[0]+3.0 or abs(east[1]-start[1]) > 0.75:
            raise RuntimeError(f'{self.serial}: +X touch did not move only along source X: {start} -> {east}')
        if abs(float(re.search(r'yaw (-?\d+(?:\.\d+)?)', east_test['post_release_report']).group(1)) + 1.5708) > 0.08:
            raise RuntimeError(f'{self.serial}: +X heading was incorrect: {east_test["post_release_report"]}')

        root = self.hierarchy()
        forward = self.run_axis_swipe(root, 0.0, 1.0, 650, 'movement-north')
        after_forward = forward['after_release_xyz']
        before_forward = forward['before_xyz']
        if after_forward[1] <= before_forward[1]+2.0 or abs(after_forward[0]-before_forward[0]) > 0.75:
            raise RuntimeError(f'{self.serial}: +Y touch did not move only along source Y: {before_forward} -> {after_forward}')
        if abs(float(re.search(r'yaw (-?\d+(?:\.\d+)?)', forward['post_release_report']).group(1))) > 0.08:
            raise RuntimeError(f'{self.serial}: +Y heading was incorrect: {forward["post_release_report"]}')

        # Interrupt a held touch by opening Android Settings. This exercises
        # onPause/onResume while preserving the exact encounter Activity stack;
        # relaunching the app from HOME creates a new encounter owner and
        # invalidates the preview's return path.
        root = self.hierarchy()
        pad = next(node for node in self.nodes(root) if node.get('content-desc') == 'Movement pad')
        x1, y1, x2, y2 = map(int, re.findall(r'\d+', pad.get('bounds', '')))
        cx, cy = (x1+x2)//2, (y1+y2)//2
        radius = min(x2-x1, y2-y1)*0.36*0.92
        held = subprocess.Popen([str(self.adb), '-s', self.serial, 'shell', 'input', 'swipe',
            str(cx), str(cy), str(round(cx+radius)), str(cy), '2600'],
            stdout=subprocess.PIPE, stderr=subprocess.STDOUT, text=True)
        time.sleep(0.3)
        self.command('shell', 'am', 'start', '-a', 'android.settings.SETTINGS')
        held.wait(timeout=12)
        foreground = self.foreground_activity()
        for _ in range(16):
            if foreground and 'com.android.settings/' in foreground:
                break
            time.sleep(0.25)
            foreground = self.foreground_activity()
        if not foreground or 'com.android.settings/' not in foreground:
            raise RuntimeError(f'{self.serial}: Android Settings did not open for lifecycle pause test: {foreground!r}')
        self.command('shell', 'input', 'keyevent', '4')
        foreground = self.foreground_activity()
        for _ in range(16):
            if foreground and foreground.endswith('/.SwampPreviewActivity'):
                break
            time.sleep(0.25)
            foreground = self.foreground_activity()
        if not foreground or not foreground.endswith('/.SwampPreviewActivity'):
            raise RuntimeError(f'{self.serial}: paused SWAMP screen did not resume: {foreground!r}')
        root, resumed_text = self.wait_movement(lambda text: 'IDLE' in text or text.startswith('BLOCKED'), attempts=12)
        resumed_position = self.position_from_report(resumed_text)
        time.sleep(0.6)
        _, paused_stable_text = self.wait_movement(lambda text: 'IDLE' in text or text.startswith('BLOCKED'), attempts=6)
        paused_stable = self.position_from_report(paused_stable_text)
        if any(abs(paused_stable[i]-resumed_position[i]) > 0.02 for i in range(3)):
            raise RuntimeError(f'{self.serial}: position drifted after pause/resume: {resumed_position} -> {paused_stable}')
        movement = {'east': east_test, 'north': forward}
        movement['pause_resume'] = {
            'position_after_resume': resumed_position,
            'position_after_idle_wait': paused_stable,
            'no_drift': True,
            'screenshot': self.screenshot('movement-pause-resume.png'),
        }

        # Hold +Y long enough to reach the actual module-zero mesh boundary.
        # The visible diagnostic must say why the endpoint was rejected.
        root = self.hierarchy()
        pad = next(node for node in self.nodes(root) if node.get('content-desc') == 'Movement pad')
        x1, y1, x2, y2 = map(int, re.findall(r'\d+', pad.get('bounds', '')))
        cx, cy = (x1+x2)//2, (y1+y2)//2
        radius = min(x2-x1, y2-y1)*0.36*0.92
        held = subprocess.Popen([str(self.adb), '-s', self.serial, 'shell', 'input', 'swipe',
            str(cx), str(cy), str(cx), str(round(cy-radius)), '12000'],
            stdout=subprocess.PIPE, stderr=subprocess.STDOUT, text=True)
        _, rejected_text = self.wait_movement(lambda text: text.startswith('BLOCKED') and
                                              'no floor' in text.lower(), attempts=4)
        boundary_shot = self.screenshot('movement-no-floor.png')
        rejected_position = self.position_from_report(rejected_text)
        held.wait(timeout=12)
        _, idle_at_edge = self.wait_movement(lambda text: 'IDLE' in text, attempts=8)
        edge_position = self.position_from_report(idle_at_edge)
        if any(abs(edge_position[i]-rejected_position[i]) > 0.02 for i in range(3)):
            raise RuntimeError(f'{self.serial}: released position differed from the rejected candidate position: {rejected_position} -> {edge_position}')
        time.sleep(0.5)
        _, edge_stable = self.wait_movement(lambda text: 'IDLE' in text, attempts=4)
        if self.position_from_report(edge_stable) != edge_position:
            raise RuntimeError(f'{self.serial}: movement resumed without touch after edge release')
        movement['no_floor_rejection'] = {
            'report_while_stick_held': rejected_text,
            'position_after_rejection': rejected_position,
            'position_after_release': edge_position,
            'diagnostic_screenshot': boundary_shot,
            'blocked_candidate_preserved_position': True,
        }
        return movement, self.hierarchy()

    def check(self):
        self.command('install', '-r', str(self.apk.resolve()))
        if self.command('shell', 'getprop', 'ro.build.version.sdk').strip() != '37':
            raise RuntimeError(f'{self.serial}: expected API 37')
        page_size = int(self.command('shell', 'getconf', 'PAGE_SIZE').strip())
        if page_size not in (4096, 16384):
            raise RuntimeError(f'{self.serial}: unexpected page size {page_size}')
        if self.command('shell', 'getprop', 'ro.kernel.qemu').strip() != '1':
            raise RuntimeError(f'{self.serial}: target is not an emulator')
        self.command('logcat', '-c')
        self.command('shell', 'am', 'force-stop', PACKAGE)
        foreground = None
        for _ in range(16):
            # Updating an already-installed debug APK can briefly put Android's
            # PackageUpdateActivity above the app. In that window am start may
            # report success without bringing GameplayActivity to the front.
            self.command('shell', 'am', 'start', '-W', '-n', COMPONENT)
            foreground = self.foreground_activity()
            if foreground and foreground.endswith('/.GameplayActivity'):
                break
            time.sleep(0.5)
        if not foreground or not foreground.endswith('/.GameplayActivity'):
            raise RuntimeError(f'{self.serial}: GameplayActivity did not become foreground before opening SWAMP: {foreground!r}')

        root = self.wait_for('SWAMP preview')
        before_hud = self.text_status(root)
        # The preview button can become visible a few frames before the
        # encounter status text after a cold GameplayActivity launch. Poll the
        # same accessibility tree briefly instead of treating that transient
        # ordering as a failed device run.
        for _ in range(8):
            if before_hud:
                break
            time.sleep(0.25)
            root = self.hierarchy()
            before_hud = self.text_status(root)
        if not before_hud:
            raise RuntimeError(f'{self.serial}: encounter HUD was not present before preview')
        # Exercise the real route the player uses. The preview Activity is
        # private to the app and cannot be started directly by ADB shell.
        swamp_button = self.find_node('SWAMP preview', root)
        launcher_tap_fallback = False
        self.tap(swamp_button)
        foreground = self.foreground_activity()
        for _ in range(20):
            if foreground and foreground.endswith('/.SwampPreviewActivity'):
                break
            time.sleep(0.25)
            foreground = self.foreground_activity()
        if not foreground or not foreground.endswith('/.SwampPreviewActivity'):
            # Some API 37 emulator rotations report accessibility bounds that
            # include a stale top inset for this launcher row. `wm size` may
            # still report the portrait physical dimensions while the app is
            # rendered landscape, so use the shorter axis for the tap's Y
            # coordinate before treating navigation as an app failure.
            size_text = self.command('shell', 'wm', 'size')
            sizes = re.findall(r'(\d+)x(\d+)', size_text)
            if sizes:
                screen_width, screen_height = map(int, sizes[-1])
                x1, _, x2, _ = map(int, re.findall(r'\d+', swamp_button.get('bounds', '')))
                tap_x = (x1 + x2) // 2
                tap_y = round(min(screen_width, screen_height) * 0.18)
                self.command('shell', 'input', 'tap', str(tap_x), str(tap_y))
                launcher_tap_fallback = True
                for _ in range(20):
                    foreground = self.foreground_activity()
                    if foreground and foreground.endswith('/.SwampPreviewActivity'):
                        break
                    time.sleep(0.25)
            if not foreground or not foreground.endswith('/.SwampPreviewActivity'):
                raise RuntimeError(f'{self.serial}: SWAMP button did not open its private preview; foreground={foreground!r}; accessibility_bounds={swamp_button.get("bounds")!r}; screen={size_text!r}')
        root = self.wait_for('Run LizardMan_Intro trace')
        # Compact status stays on screen; source/test prose is available only
        # after opening the explicit details affordance.
        compact_status = self.movement_node(root).get('text', '')
        for _ in range(20):
            if compact_status.startswith(('IDLE', 'WALK', 'BLOCKED')):
                break
            time.sleep(0.1)
            root = self.hierarchy()
            compact_status = self.movement_node(root).get('text', '')
        if not compact_status.startswith(('IDLE', 'WALK', 'BLOCKED')):
            raise RuntimeError(f'{self.serial}: compact movement HUD did not leave its loading state: {compact_status!r}')
        if len(compact_status.splitlines()) > 2 or 'collision unavailable' in compact_status.lower():
            raise RuntimeError(f'{self.serial}: movement HUD is not compact: {compact_status!r}')
        if any(node.get('content-desc') == 'SWAMP source and script diagnostics'
               for node in self.nodes(root)):
            raise RuntimeError(f'{self.serial}: verbose source diagnostics are visible while Details is closed')
        compact_shot = self.screenshot('swamp-compact-initial.png')
        self.tap(self.find_node('Details', root))
        details_root = self.wait_for('Hide details')
        source_details = self.described_node(details_root, 'SWAMP source and script diagnostics').get('text', '')
        if 'SWAMP module 0' not in source_details or 'Scripts parsed' not in source_details:
            raise RuntimeError(f'{self.serial}: Details did not reveal source/test diagnostics: {source_details!r}')
        details_shot = self.screenshot('swamp-details-open.png')
        self.tap(self.find_node('Hide details', details_root))
        root = self.wait_for('Run LizardMan_Intro trace')
        movement, root = self.movement_check(root)
        self.tap(self.find_node('Run LizardMan_Intro trace', root))
        time.sleep(5.0)
        complete_shot = self.screenshot('trace-complete.png')
        root = self.hierarchy()
        text_values = [node.get('text', '') for node in self.nodes(root)]
        trace = next((value for value in text_values if value.startswith('LizardMan_Intro · explicit developer trace')), None)
        if not trace:
            raise RuntimeError(f'{self.serial}: trace status was absent: {text_values}')
        required = (
            'trigger 1/1 fired',
            'End: complete',
            'native state 1 requested; no actor allocation/render',
            'Player lock: OFF (logical)',
            'cutscene: OFF (logical)',
            'LocalPlayer unresolved; this preview has no native player Character',
            '[0500] Spawn state 1 requested',
            '[2000] Spawn state 1 requested',
            'Unsupported command (explicit no-op)',
        )
        missing = [phrase for phrase in required if phrase not in trace]
        if missing:
            raise RuntimeError(f'{self.serial}: trace report missing {missing}: {trace}')
        if not re.search(r'\[4010\] .*EndScriptedCutScene', trace):
            raise RuntimeError(f'{self.serial}: expected deferred common-script completion at 4010ms')
        scroll = self.find_node('Return to encounter', root)
        scroll_node = next((node for node in self.nodes(root)
                            if node.get('class') == 'android.widget.ScrollView'), None)
        if scroll_node is None or scroll_node.get('scrollable') != 'true':
            raise RuntimeError(f'{self.serial}: trace transcript is not scrollable')
        _, _, _, panel_bottom = map(int, re.findall(r'\d+', scroll_node.get('bounds', '')))
        return_top = int(re.findall(r'\d+', scroll.get('bounds', ''))[1])
        if panel_bottom > return_top:
            raise RuntimeError(f'{self.serial}: Return control overlaps the transcript panel')
        run_trace_node = self.find_node('Run LizardMan_Intro trace', root)
        _, run_top, _, run_bottom = map(int, re.findall(r'\d+', run_trace_node.get('bounds', '')))
        if panel_bottom > run_top or run_bottom > return_top:
            raise RuntimeError(f'{self.serial}: trace/Return controls overlap the transcript or each other')
        x1, y1, x2, y2 = map(int, re.findall(r'\d+', scroll_node.get('bounds', '')))
        center_x = (x1 + x2) // 2
        self.command('shell', 'input', 'swipe', str(center_x), str(y1 + 35),
                     str(center_x), str(y2 - 35), '450')
        time.sleep(0.2)
        top_shot = self.screenshot('trace-events-top.png')
        self.command('shell', 'input', 'swipe', str(center_x), str(y2 - 35),
                     str(center_x), str(y1 + 35), '450')
        time.sleep(0.2)
        timeline_shot = self.screenshot('trace-events-timeline.png')
        shots = [compact_shot, details_shot,
                 movement['east']['walk_screenshot'],
                 movement['east']['release_idle_screenshot'],
                 movement['north']['walk_screenshot'],
                 movement['north']['release_idle_screenshot'],
                 movement['pause_resume']['screenshot'],
                 movement['no_floor_rejection']['diagnostic_screenshot'],
                 complete_shot, top_shot, timeline_shot]
        self.tap(self.find_node('Return to encounter', root))
        root = self.wait_for('SWAMP preview')
        after_hud = self.text_status(root)
        if after_hud != before_hud:
            raise RuntimeError(f'{self.serial}: encounter HUD changed across preview return: before={before_hud!r}, after={after_hud!r}')
        return_shot = self.screenshot('encounter-return.png')
        shots.append(return_shot)

        app_pid = self.command('shell', 'pidof', PACKAGE).strip()
        if not app_pid:
            raise RuntimeError(f'{self.serial}: app process disappeared after returning to encounter')
        log = self.command('logcat', '-d', '-t', '4000', '--pid=' + app_pid)
        if 'source movement animation=walk' not in log or 'source movement animation=idle' not in log:
            raise RuntimeError(f'{self.serial}: expected source walk and idle animation transitions in app log')
        filtered = [line for line in log.splitlines() if any(token in line for token in
            ('FATAL EXCEPTION', 'Fatal signal', 'libc++abi', 'DH2Swamp: E', 'Script trace'))]
        if any(('FATAL EXCEPTION' in line or 'Fatal signal' in line or 'libc++abi' in line)
               for line in filtered):
            raise RuntimeError(f'{self.serial}: fatal/native failure log entries: {filtered}')
        (self.output / 'filtered.log').write_text('\n'.join(filtered) + ('\n' if filtered else ''), encoding='utf-8')

        installed_path = self.command('shell', 'pm', 'path', PACKAGE).strip().removeprefix('package:')
        installed_apk = self.output / 'installed.apk'
        self.command('pull', installed_path, str(installed_apk))
        digest = lambda path: hashlib.sha256(path.read_bytes()).hexdigest()
        if digest(installed_apk) != digest(self.apk):
            raise RuntimeError(f'{self.serial}: installed APK does not match tested build')

        return {
            'serial': self.serial,
            'android_release': self.command('shell', 'getprop', 'ro.build.version.release').strip(),
            'sdk': 37,
            'page_size_bytes': page_size,
            'abi': self.command('shell', 'getprop', 'ro.product.cpu.abi').strip(),
            'before_encounter_hud': before_hud,
            'trace_ui_text': trace,
            'after_return_encounter_hud': after_hud,
            'encounter_unchanged_after_return': True,
            'launcher_tap_fallback_used': launcher_tap_fallback,
            'manual_trace_started': True,
            'trace_completed': True,
            'movement_checks': movement,
            'movement_animation_transitions_logged': ['walk', 'idle'],
            'trace_panel_and_both_controls_nonoverlapping': True,
            'screenshots': shots,
            'filtered_error_log_entries': len(filtered),
            'filtered_log_sha256': digest(self.output / 'filtered.log'),
            'installed_apk_sha256': digest(installed_apk),
            'installed_apk_bytes': installed_apk.stat().st_size,
        }


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--adb', type=Path, required=True)
    parser.add_argument('--apk', type=Path, default=Path(__file__).parents[1] / 'build/dh2-source-renderer-debug.apk')
    parser.add_argument('--serial', action='append', required=True)
    parser.add_argument('--output', type=Path, required=True)
    args = parser.parse_args()
    results = []
    for serial in args.serial:
        device_dir = args.output / serial
        results.append(Device(args.adb.resolve(), serial, args.apk.resolve(), device_dir).check())
    result = {'apk_sha256': hashlib.sha256(args.apk.read_bytes()).hexdigest(),
              'apk_bytes': args.apk.stat().st_size,
              'checks': results}
    args.output.mkdir(parents=True, exist_ok=True)
    (args.output / 'result.json').write_text(json.dumps(result, indent=2) + '\n', encoding='utf-8')
    print(json.dumps(result, indent=2))


if __name__ == '__main__':
    main()
