#!/usr/bin/env python3
"""Exercise Gameplay -> Diagnostics -> Irrlicht SWAMP -> Java Gameplay on API 37."""
from __future__ import annotations

import argparse
import hashlib
import json
from pathlib import Path
import re
import subprocess
import time
import uuid
import xml.etree.ElementTree as ET
import zipfile


PACKAGE = 'local.dh2.sourceviewer'
GAMEPLAY_ACTIVITY = f'{PACKAGE}/local.dh2.sourceviewer.GameplayActivity'
DIAGNOSTICS_ACTIVITY = f'{PACKAGE}/local.dh2.sourceviewer.MainActivity'
IRRLICHT_ACTIVITY = f'{PACKAGE}/android.app.NativeActivity'
IRRLICHT_BUTTON_LABEL = 'SWAMP module 0 · Irrlicht source view'
HUD_PATTERN = re.compile(r'^HP\s+(\d+)/(\d+)\s+\|\s+Sentries\s+(\d+)/(\d+)')
ASSEMBLY_PATTERN = re.compile(
    r'SWAMP module 0 assembled in Irrlicht r6038: .*?source_draws=\d+ '
    r'visible_diagnostic_draws=\d+.*?path_mask=0x[0-9A-Fa-f]+.*?'
    r'AlphaMap_refs=22 AlphaMap_unresolved_refs=\d+ '
    r'AlphaMap_cutout_draws=22')
FATAL_MARKERS = (
    'FATAL EXCEPTION', 'Fatal signal', 'SIGSEGV', 'SIGABRT', 'libc++abi',
    'JNI DETECTED ERROR', 'GL_INVALID_OPERATION', 'GL_INVALID_ENUM',
    'Tried to set a texture not owned by this driver',
    'Failed to create Irrlicht device',
)
EXPECTED_SWAMP_ASSETS = {
    'dh2/swamp.bdae', 'dh2/swamp.mlx',
    'dh2/swamp-entry-mgp.mgp', 'dh2/swamp-diffuse.tga',
    'dh2/swamp-alpha.tga',
}


def sha256_bytes(data: bytes) -> str:
    return hashlib.sha256(data).hexdigest()


def sha256_file(path: Path) -> str:
    digest = hashlib.sha256()
    with path.open('rb') as source:
        for block in iter(lambda: source.read(1024 * 1024), b''):
            digest.update(block)
    return digest.hexdigest()


class InAppSwampRuntime:
    def __init__(self, adb: Path, apk: Path, serial: str,
                 expected_page_size: int, output: Path,
                 startup_timeout: float, ui_timeout: float):
        self.adb = adb
        self.apk = apk
        self.serial = serial
        self.expected_page_size = expected_page_size
        self.output = output
        self.startup_timeout = startup_timeout
        self.ui_timeout = ui_timeout
        self.built_hash = sha256_file(apk)
        self.pids: set[str] = set()
        self.device: dict = {}
        self.start_epoch = 0.0
        self.installed_hash: str | None = None
        self.remote_base_path: str | None = None
        self.remote_hierarchy = (
            '/data/local/tmp/dh2-irrlicht-in-app-' + uuid.uuid4().hex + '.xml')
        self.transitions: list[dict] = []

    def command(self, *args: str, binary: bool = False,
                check: bool = True, timeout: int = 60):
        result = subprocess.run(
            [str(self.adb), '-s', self.serial, *args],
            stdout=subprocess.PIPE, stderr=subprocess.STDOUT,
            timeout=timeout, check=False, text=not binary,
            encoding=None if binary else 'utf-8',
            errors=None if binary else 'replace')
        if check and result.returncode:
            output = (result.stdout if isinstance(result.stdout, str)
                      else repr(result.stdout[:300]))
            raise RuntimeError(
                f'{self.serial}: adb {" ".join(args)} failed '
                f'({result.returncode}): {output[-2500:]}')
        return result.stdout

    def shell(self, *args: str, check: bool = True, timeout: int = 60) -> str:
        return self.command('shell', *args, check=check, timeout=timeout).strip()

    def inspect_apk(self) -> dict:
        with zipfile.ZipFile(self.apk) as archive:
            names = set(archive.namelist())
            required = {
                'classes.dex',
                'assets/dh2/local-irrlicht-swamp-manifest.json',
                'assets/dh2/LOCAL-ASSET-NOTICE.txt',
                'assets/media/Shaders/COGLES2Solid.vsh',
                'assets/media/Shaders/COGLES2Solid.fsh',
                'assets/third-party-notices/NOTICE.txt',
                'assets/third-party-notices/native_app_glue-NOTICE.txt',
                'lib/x86_64/libdh2source.so', 'lib/x86_64/libdh2lua.so',
                'lib/x86_64/libdh2irrlicht.so',
            }
            missing = required - names
            if missing:
                raise RuntimeError(f'integrated APK lacks required entries: {sorted(missing)}')
            manifest = json.loads(archive.read(
                'assets/dh2/local-irrlicht-swamp-manifest.json'))
            manifest_scope = str(manifest.get('scope', ''))
            if 'local-only' not in manifest_scope.lower():
                raise RuntimeError('APK SWAMP asset manifest is not marked local-only')
            rows = manifest.get('assets', [])
            rows_by_asset = {row.get('apk_asset'): row for row in rows}
            if len(rows_by_asset) != len(rows) or set(rows_by_asset) != EXPECTED_SWAMP_ASSETS:
                raise RuntimeError(
                    'integrated APK must contain the exact four declared SWAMP source assets')
            assets = {}
            for asset_name, row in rows_by_asset.items():
                payload = archive.read('assets/' + asset_name)
                actual = {'bytes': len(payload), 'sha256': sha256_bytes(payload)}
                expected = {'bytes': row.get('bytes'), 'sha256': row.get('sha256')}
                if actual != expected:
                    raise RuntimeError(
                        f'integrated APK SWAMP asset hash/size mismatch: {asset_name}')
                assets[asset_name] = actual
        return {'entry_count': len(names), 'swamp_assets': assets,
                'x86_64_native_libraries_present': True,
                'local_asset_manifest_scope': manifest_scope}

    def inspect_device(self) -> None:
        state = self.command('get-state').strip()
        if state != 'device':
            raise RuntimeError(f'{self.serial}: adb target is not online: {state!r}')
        getprop = lambda name: self.shell('getprop', name)
        release = getprop('ro.build.version.release')
        api = getprop('ro.build.version.sdk')
        abi = getprop('ro.product.cpu.abi')
        model = getprop('ro.product.model')
        device_name = getprop('ro.product.device')
        emulator = getprop('ro.kernel.qemu')
        page_size_text = self.shell('getconf', 'PAGE_SIZE')
        if any(marker in (model + ' ' + device_name).lower()
               for marker in ('fold7', 'sm-f966')):
            raise RuntimeError(f'{self.serial}: Fold7 testing is excluded')
        if emulator != '1':
            raise RuntimeError(
                f'{self.serial}: target is not an Android emulator (ro.kernel.qemu={emulator!r})')
        if release != '17' or api != '37' or abi != 'x86_64':
            raise RuntimeError(
                f'{self.serial}: expected Android 17/API 37/x86_64, got '
                f'{release}/API {api}/{abi}')
        try:
            page_size = int(page_size_text)
        except ValueError as error:
            raise RuntimeError(f'invalid page-size response: {page_size_text!r}') from error
        if page_size not in (4096, 16384) or page_size != self.expected_page_size:
            raise RuntimeError(
                f'{self.serial}: expected {self.expected_page_size}-byte pages, got {page_size}')
        self.device = {
            'serial': self.serial, 'android_release': release,
            'api_level': int(api), 'abi': abi, 'page_size_bytes': page_size,
            'model': model, 'device': device_name, 'emulator': True,
            'fold7_tested': False,
        }

    def install_and_verify(self) -> None:
        # Upgrade in place only. Never clear package data or uninstall an APK
        # when a signature mismatch occurs.
        self.command('install', '-r', str(self.apk))
        package_paths = self.shell('pm', 'path', PACKAGE).splitlines()
        base_path = next((line.split(':', 1)[1].strip()
                          for line in package_paths
                          if line.startswith('package:') and line.endswith('/base.apk')),
                         None)
        if not base_path:
            raise RuntimeError(f'could not resolve installed base APK: {package_paths!r}')
        self.remote_base_path = base_path
        installed = self.output / 'installed-base.apk'
        self.command('pull', base_path, str(installed))
        self.installed_hash = sha256_file(installed)
        if self.installed_hash != self.built_hash:
            raise RuntimeError(
                f'installed APK SHA-256 {self.installed_hash} differs from candidate '
                f'{self.built_hash}')
        self.command('shell', 'am', 'force-stop', PACKAGE)
        # A short pause distinguishes this run's app-process log records from
        # any earlier instance without clearing shared logcat.
        time.sleep(1.05)
        self.start_epoch = float(self.shell('date', '+%s'))

    def hierarchy(self) -> ET.Element:
        last_raw = ''
        dump_output = ''
        try:
            for dump_attempt in range(2):
                dump_output = self.shell(
                    'uiautomator', 'dump', self.remote_hierarchy, check=False)
                for _ in range(6):
                    last_raw = self.shell('cat', self.remote_hierarchy, check=False)
                    candidate = last_raw.lstrip()
                    if candidate.startswith('<?xml') or candidate.startswith('<hierarchy'):
                        try:
                            return ET.fromstring(last_raw)
                        except ET.ParseError:
                            pass
                    time.sleep(0.2)
                if dump_attempt == 0:
                    time.sleep(0.2)
        finally:
            self.shell('rm', '-f', self.remote_hierarchy, check=False)
        raise RuntimeError(
            f'{self.serial}: uiautomator did not produce valid window XML; '
            f'dump={dump_output[:500]!r}; last read={last_raw[:800]!r}')

    @staticmethod
    def top_activity(state: str) -> str:
        return next((line.strip() for line in state.splitlines()
                     if 'topResumedActivity=' in line), '')

    def foreground_activity(self) -> str:
        return self.shell('dumpsys', 'activity', 'activities')

    def wait_activity(self, activity_name: str, timeout: float | None = None) -> str:
        deadline = time.monotonic() + (timeout or self.ui_timeout)
        last = ''
        while time.monotonic() < deadline:
            state = self.foreground_activity()
            top = self.top_activity(state)
            if PACKAGE in top and activity_name in top:
                self.remember_pid()
                self.transitions.append({'activity': activity_name, 'top_resumed': top})
                return state
            last = top or state[-1000:]
            time.sleep(0.2)
        raise RuntimeError(
            f'{self.serial}: expected {activity_name} foreground; last top activity: {last!r}')

    def app_pids(self) -> list[str]:
        raw = self.shell('pidof', PACKAGE, check=False)
        return raw.split()

    def remember_pid(self) -> str:
        pids = self.app_pids()
        if not pids:
            raise RuntimeError(f'{self.serial}: {PACKAGE} has no live app process')
        self.pids.update(pids)
        return pids[0]

    def screenshot(self, name: str) -> dict:
        data = self.command('exec-out', 'screencap', '-p', binary=True, timeout=30)
        if not data.startswith(b'\x89PNG\r\n\x1a\n'):
            raise RuntimeError(f'{self.serial}: screencap did not return PNG data')
        path = self.output / name
        path.write_bytes(data)
        return {'file': name, 'bytes': len(data), 'sha256': sha256_bytes(data)}

    def save_hierarchy(self, name: str) -> dict:
        root = self.hierarchy()
        path = self.output / name
        path.write_bytes(ET.tostring(root, encoding='utf-8', xml_declaration=True))
        return {'file': name, 'sha256': sha256_file(path)}

    def click_text(self, text: str, timeout: float = 15.0,
                   exact: bool = False) -> dict:
        deadline = time.monotonic() + timeout
        last_labels: list[str] = []
        while time.monotonic() < deadline:
            root = self.hierarchy()
            last_labels = []
            for node in root.iter('node'):
                node_text = node.get('text', '')
                description = node.get('content-desc', '')
                label = (node_text + ' ' + description).strip()
                if label:
                    last_labels.append(label)
                matches = node_text == text if exact else text in label
                if not matches or node.get('clickable') != 'true':
                    continue
                bounds = [int(value) for value in re.findall(
                    r'\d+', node.get('bounds', ''))]
                if len(bounds) != 4:
                    continue
                left, top, right, bottom = bounds
                if right <= left or bottom <= top or right > 10000 or bottom > 10000:
                    continue
                self.shell('input', 'tap', str((left + right) // 2),
                           str((top + bottom) // 2))
                return {'text': node_text, 'content_description': description,
                        'bounds': bounds}

            scrollable = next((node for node in root.iter('node')
                               if node.get('scrollable') == 'true'), None)
            bounds = ([int(value) for value in re.findall(
                r'\d+', scrollable.get('bounds', ''))]
                      if scrollable is not None else [])
            if len(bounds) == 4:
                left, top, right, bottom = bounds
                x = (left + right) // 2
                self.shell('input', 'swipe', str(x), str(bottom - 80),
                           str(x), str(top + 80), '400')
            else:
                time.sleep(0.25)
        raise RuntimeError(
            f'{self.serial}: could not tap clickable UI label containing {text!r}; '
            f'last labels={last_labels[:50]!r}')

    def wait_for_gameplay_hud(self, timeout: float = 20.0) -> str:
        deadline = time.monotonic() + timeout
        last = []
        while time.monotonic() < deadline:
            root = self.hierarchy()
            last = [node.get('text', '') for node in root.iter('node')]
            if any(any(HUD_PATTERN.match(line.strip()) for line in text.splitlines())
                   for text in last):
                return next(line.strip() for text in last for line in text.splitlines()
                            if HUD_PATTERN.match(line.strip()))
            time.sleep(0.25)
        raise RuntimeError(
            f'{self.serial}: Java encounter HUD did not return after Back: {last[:30]!r}')

    def app_log(self, pid: str) -> str:
        return self.command('logcat', '-d', '-v', 'epoch', '--pid=' + pid,
                            timeout=30)

    def fresh_log_lines(self, log: str) -> list[str]:
        fresh = []
        for line in log.splitlines():
            match = re.match(r'^\s*(\d{9,}(?:\.\d+)?)\b', line)
            if match and float(match.group(1)) < self.start_epoch:
                continue
            fresh.append(line)
        return fresh

    def collect_and_check_logs(self) -> dict:
        reports = []
        errors = []
        assembly_seen = False
        for pid in sorted(self.pids, key=int):
            raw = self.app_log(pid)
            lines = self.fresh_log_lines(raw)
            filtered = '\n'.join(lines) + ('\n' if lines else '')
            path = self.output / f'app-pid-{pid}-logcat.txt'
            path.write_text(filtered, encoding='utf-8')
            assembly_seen |= bool(ASSEMBLY_PATTERN.search(filtered))
            for line in lines:
                lowered = line.lower()
                for marker in FATAL_MARKERS:
                    if marker.lower() in lowered:
                        errors.append({'pid': pid, 'marker': marker, 'line': line})
            reports.append({'pid': pid, 'file': path.name,
                            'lines': len(lines), 'sha256': sha256_file(path)})
        if not assembly_seen:
            raise RuntimeError(
                f'{self.serial}: no SWAMP module-zero Irrlicht assembly marker in app-process logs')
        if errors:
            raise RuntimeError(f'{self.serial}: app-process error markers found: {errors[:8]!r}')
        return {'app_processes': reports, 'module_zero_assembly_seen': assembly_seen,
                'fatal_marker_count': 0, 'logcat_cleared': False}

    def run(self) -> dict:
        self.output.mkdir(parents=True, exist_ok=True)
        (self.output / 'irrlicht-swamp-in-app-runtime-validation.json').unlink(
            missing_ok=True)
        apk_contents = self.inspect_apk()
        self.inspect_device()
        self.install_and_verify()

        launch = self.command('shell', 'am', 'start', '-W', '-n', GAMEPLAY_ACTIVITY)
        if 'Status: ok' not in launch:
            raise RuntimeError(f'GameplayActivity launch failed: {launch[-2000:]}')
        self.wait_activity('GameplayActivity')
        initial_hud = self.wait_for_gameplay_hud()
        gameplay_before = self.screenshot('gameplay-before.png')

        diagnostics_click = self.click_text('Diagnostics')
        self.wait_activity('MainActivity')
        diagnostics_hierarchy = self.save_hierarchy('diagnostics-mainactivity.xml')
        diagnostics_screen = self.screenshot('diagnostics-mainactivity.png')

        native_click = self.click_text(IRRLICHT_BUTTON_LABEL, exact=True)
        self.wait_activity('NativeActivity', timeout=self.startup_timeout)
        native_hierarchy = self.save_hierarchy('irrlicht-nativeactivity.xml')
        native_screen = self.screenshot('irrlicht-nativeactivity.png')
        native_pid = self.remember_pid()
        deadline = time.monotonic() + self.startup_timeout
        latest_log = ''
        while time.monotonic() < deadline:
            latest_log = self.app_log(native_pid)
            if ASSEMBLY_PATTERN.search('\n'.join(self.fresh_log_lines(latest_log))):
                break
            time.sleep(0.25)
        else:
            raise RuntimeError(
                f'{self.serial}: NativeActivity foregrounded but SWAMP assembly did not appear; '
                f'last app log={latest_log[-3000:]}')

        # Back first returns to the diagnostics Activity; a second Back pops
        # that Activity and resumes the Java encounter below it.
        self.shell('input', 'keyevent', '4')
        self.wait_activity('MainActivity')
        main_after_native = self.screenshot('diagnostics-after-native-back.png')
        self.shell('input', 'keyevent', '4')
        self.wait_activity('GameplayActivity')
        returned_hud = self.wait_for_gameplay_hud()
        gameplay_after = self.screenshot('gameplay-after-back.png')
        log_check = self.collect_and_check_logs()

        return {
            'schema': 'dh2.android.irrlicht-swamp-in-app-runtime.v1',
            'result': 'pass',
            'scope': 'local-only same-app NativeActivity navigation and lifecycle smoke; not a playable game port',
            'local_only': True,
            'release_eligible': False,
            'device': self.device,
            'apk': {'path': self.apk.as_posix(), 'sha256': self.built_hash,
                    'installed_sha256': self.installed_hash,
                    'installed_hash_matches': self.installed_hash == self.built_hash,
                    'bytes': self.apk.stat().st_size,
                    'installed_base_path': self.remote_base_path,
                    'package': PACKAGE, 'contents': apk_contents},
            'navigation': {
                'gameplay_activity': GAMEPLAY_ACTIVITY,
                'diagnostics_activity': DIAGNOSTICS_ACTIVITY,
                'irrlicht_activity': IRRLICHT_ACTIVITY,
                'gameplay_to_diagnostics_click': diagnostics_click,
                'diagnostics_to_irrlicht_click': native_click,
                'native_activity_foregrounded': True,
                'first_back_returned_to_diagnostics': True,
                'second_back_returned_to_gameplay': True,
                'foreground_transitions': self.transitions,
                'gameplay_hud_before': initial_hud,
                'gameplay_hud_after': returned_hud,
            },
            'runtime_checks': log_check,
            'screenshots': [gameplay_before, diagnostics_screen, native_screen,
                            main_after_native, gameplay_after],
            'hierarchies': [diagnostics_hierarchy, native_hierarchy],
            'fold7_tested': False,
        }


def main() -> int:
    repo = Path(__file__).resolve().parents[3]
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--apk', type=Path, required=True,
                        help='Built --irrlicht-swamp-in-app APK')
    parser.add_argument('--adb', type=Path, required=True,
                        help='Path to Android platform-tools adb')
    parser.add_argument('--serial', required=True,
                        help='Online Android 17/API 37 x86_64 emulator serial')
    parser.add_argument('--expected-page-size', type=int, required=True,
                        choices=(4096, 16384), help='Expected emulator page size in bytes')
    parser.add_argument('--output', type=Path, required=True,
                        help='Directory for screenshots, UI dumps, logs, and JSON report')
    parser.add_argument('--startup-timeout-seconds', type=float, default=35.0)
    parser.add_argument('--ui-timeout-seconds', type=float, default=15.0)
    args = parser.parse_args()

    apk = args.apk.resolve(strict=True)
    adb_path = args.adb.resolve(strict=True)
    output = args.output.resolve()
    if not output.is_relative_to(repo):
        raise RuntimeError('runtime evidence output must stay inside the repository workspace')
    smoke = InAppSwampRuntime(adb_path, apk, args.serial,
                              args.expected_page_size, output,
                              args.startup_timeout_seconds,
                              args.ui_timeout_seconds)
    result = smoke.run()
    report = output / 'irrlicht-swamp-in-app-runtime-validation.json'
    report.write_text(json.dumps(result, indent=2) + '\n', encoding='utf-8')
    print(report)
    print('APK SHA-256:', result['apk']['sha256'])
    print('Installed SHA-256:', result['apk']['installed_sha256'])
    print('API/page size:', result['device']['api_level'],
          result['device']['page_size_bytes'])
    print('Navigation: Gameplay -> Diagnostics -> Irrlicht SWAMP -> Back -> Diagnostics -> Back -> Gameplay')
    print('Result: PASS')
    return 0


if __name__ == '__main__':
    raise SystemExit(main())
