#!/usr/bin/env python3
"""Verify the authored encounter checkpoint across Android Activity lifecycles.

This is an explicit-device smoke test for the exact installed APK. It never
clears logcat. Use --clear-data only on a disposable test emulator when a clean
encounter is required; that flag clears this package's private app data.
"""
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

PACKAGE = 'local.dh2.sourceviewer'
ACTIVITY = PACKAGE + '/.GameplayActivity'
HUD_PATTERN = re.compile(r'^HP\s+(\d+)/(\d+)\s+\|\s+Sentries\s+(\d+)/(\d+)')
FATAL_MARKERS = (
    'FATAL EXCEPTION', 'Fatal signal', 'SIGSEGV', 'SIGABRT', 'libc++abi',
    'JNI DETECTED ERROR', 'Fatal signal 6', 'Fatal signal 11',
)


def sha256_bytes(data: bytes) -> str:
    return hashlib.sha256(data).hexdigest()


def sha256_file(path: Path) -> str:
    digest = hashlib.sha256()
    with path.open('rb') as source:
        for block in iter(lambda: source.read(1024 * 1024), b''):
            digest.update(block)
    return digest.hexdigest()


class GameplayActivitySmoke:
    def __init__(self, adb: Path, apk: Path, serial: str, expected_page_size: int,
                 output: Path, clear_data: bool):
        self.adb = adb
        self.apk = apk
        self.serial = serial
        self.expected_page_size = expected_page_size
        self.output = output
        self.clear_data = clear_data
        self.screenshots: list[dict] = []
        self.pids: set[str] = set()
        self.pid_logs: list[dict] = []
        self.device: dict = {}
        self.candidate_hash = sha256_file(apk)
        self.installed_hash: str | None = None
        self.installed_copy: Path | None = None
        self.remote_base_path: str | None = None
        self.initial_hud: str | None = None
        self.checkpoint_hud: str | None = None
        self.after_home_hud: str | None = None
        self.after_cold_hud: str | None = None
        # /data/local/tmp is writable by the shell user and avoids API-level
        # quirks in emulated shared storage. Use a per-run name so a stale dump
        # can never be mistaken for the current window hierarchy.
        self.remote_hierarchy_path = (
            '/data/local/tmp/dh2-gameplay-lifecycle-' + uuid.uuid4().hex + '.xml')

    def command(self, *args: str, binary: bool = False, check: bool = True,
                timeout: int = 60):
        result = subprocess.run(
            [str(self.adb), '-s', self.serial, *args],
            stdout=subprocess.PIPE, stderr=subprocess.STDOUT,
            timeout=timeout, check=False,
            text=not binary, encoding=None if binary else 'utf-8',
            errors=None if binary else 'replace')
        if check and result.returncode != 0:
            output = result.stdout if isinstance(result.stdout, str) else repr(result.stdout[:300])
            raise RuntimeError(f'adb {" ".join(args)} failed ({result.returncode}): {output[-2000:]}')
        return result.stdout

    def shell(self, *args: str, check: bool = True):
        return self.command('shell', *args, check=check)

    def inspect_device(self):
        state = self.command('get-state').strip()
        if state != 'device':
            raise RuntimeError(f'{self.serial}: adb target is not online: {state!r}')
        getprop = lambda name: self.shell('getprop', name).strip()
        release = getprop('ro.build.version.release')
        api = getprop('ro.build.version.sdk')
        abi = getprop('ro.product.cpu.abi')
        page_size_text = self.shell('getconf', 'PAGE_SIZE').strip()
        qemu = getprop('ro.kernel.qemu')
        model = getprop('ro.product.model')
        device_name = getprop('ro.product.device')
        fold_markers = ('fold7', 'sm-f966')
        if any(marker in (model + ' ' + device_name).lower() for marker in fold_markers):
            raise RuntimeError(f'{self.serial}: Fold7 testing is explicitly excluded')
        if qemu != '1':
            raise RuntimeError(f'{self.serial}: target is not an Android emulator (ro.kernel.qemu={qemu!r})')
        if release != '17' or api != '37':
            raise RuntimeError(f'{self.serial}: expected Android 17/API 37, got {release}/API {api}')
        try:
            page_size = int(page_size_text)
        except ValueError as error:
            raise RuntimeError(f'{self.serial}: invalid page size response {page_size_text!r}') from error
        if page_size != self.expected_page_size:
            raise RuntimeError(f'{self.serial}: expected {self.expected_page_size}-byte pages, got {page_size}')
        if page_size not in (4096, 16384):
            raise RuntimeError(f'{self.serial}: supported test page sizes are 4096 or 16384, got {page_size}')
        if abi != 'x86_64':
            raise RuntimeError(f'{self.serial}: expected the configured x86_64 test emulator, got {abi!r}')
        self.device = {
            'serial': self.serial, 'android_release': release, 'api_level': int(api),
            'abi': abi, 'page_size_bytes': page_size, 'model': model,
            'device': device_name, 'emulator': True, 'fold7_tested': False,
        }

    def install_and_verify(self):
        self.command('install', '-r', str(self.apk))
        package_paths = self.shell('pm', 'path', PACKAGE).splitlines()
        base_path = next((line.split(':', 1)[1].strip() for line in package_paths
                          if line.startswith('package:') and line.endswith('/base.apk')), None)
        if not base_path:
            raise RuntimeError(f'{self.serial}: could not resolve installed base APK: {package_paths!r}')
        self.remote_base_path = base_path
        self.installed_copy = self.output / 'installed-base.apk'
        self.command('pull', base_path, str(self.installed_copy))
        self.installed_hash = sha256_file(self.installed_copy)
        if self.installed_hash != self.candidate_hash:
            raise RuntimeError(
                f'{self.serial}: installed APK SHA-256 {self.installed_hash} differs from '
                f'candidate {self.candidate_hash}')
        if self.clear_data:
            result = self.shell('pm', 'clear', PACKAGE).strip()
            if 'Success' not in result:
                raise RuntimeError(f'{self.serial}: pm clear did not confirm success: {result!r}')
        # Do not accidentally resume an activity that predates this exact APK.
        self.command('shell', 'am', 'force-stop', PACKAGE)

    def hierarchy(self) -> ET.Element:
        remote = self.remote_hierarchy_path
        last_raw = ''
        dump_output = ''
        try:
            # On Android 17 the dump command can print its success message
            # before the shared-storage file is visible to a following shell
            # command. Retry the dump/read pair and accept only parsed XML.
            for dump_attempt in range(2):
                dump_output = self.shell('uiautomator', 'dump', remote, check=False)
                for _ in range(5):
                    last_raw = self.shell('cat', remote, check=False)
                    if last_raw.lstrip().startswith('<?xml') or last_raw.lstrip().startswith('<hierarchy'):
                        try:
                            return ET.fromstring(last_raw)
                        except ET.ParseError:
                            pass  # A partially visible file can be completed on the next read.
                    time.sleep(0.2)
                if dump_attempt == 0:
                    time.sleep(0.2)
        finally:
            self.shell('rm', '-f', remote, check=False)
        raise RuntimeError(
            f'{self.serial}: uiautomator did not produce valid XML at {remote}; '
            f'dump output={dump_output[:500]!r}; last read={last_raw[:800]!r}')

    @staticmethod
    def node_texts(root: ET.Element) -> list[str]:
        return [node.get('text', '') for node in root.iter('node')]

    @staticmethod
    def encounter_hud(root: ET.Element) -> tuple[str, str, tuple[int, int, int, int]] | None:
        for text in GameplayActivitySmoke.node_texts(root):
            lines = text.splitlines()
            line = next((value.strip() for value in lines if HUD_PATTERN.match(value.strip())), None)
            if line:
                match = HUD_PATTERN.match(line)
                assert match
                return text, line, tuple(int(value) for value in match.groups())
        return None

    def wait_for_hud(self, timeout: float = 30.0):
        end = time.monotonic() + timeout
        last = ''
        while time.monotonic() < end:
            root = self.hierarchy()
            found = self.encounter_hud(root)
            if found:
                return root, found[0], found[1], found[2]
            last = repr(self.node_texts(root))[:1600]
            time.sleep(0.25)
        raise RuntimeError(f'{self.serial}: GameplayActivity encounter HUD did not appear: {last}')

    def foreground_activity(self) -> str:
        return self.shell('dumpsys', 'activity', 'activities')

    def is_gameplay_foreground(self) -> bool:
        state = self.foreground_activity()
        top = next((line for line in state.splitlines()
                    if 'topResumedActivity=' in line), '')
        return PACKAGE in top and 'GameplayActivity' in top

    def wait_foreground(self, wanted: bool, timeout: float = 12.0):
        end = time.monotonic() + timeout
        last = ''
        while time.monotonic() < end:
            last = self.foreground_activity()
            top = next((line for line in last.splitlines()
                        if 'topResumedActivity=' in line), '')
            active = PACKAGE in top and 'GameplayActivity' in top
            if active == wanted:
                return last
            time.sleep(0.25)
        raise RuntimeError(f'{self.serial}: GameplayActivity foreground={wanted} was not reached: {last[-1400:]}')

    def app_pids(self) -> list[str]:
        raw = self.shell('pidof', PACKAGE, check=False).strip()
        return raw.split()

    def remember_pid(self) -> str:
        pids = self.app_pids()
        if not pids:
            raise RuntimeError(f'{self.serial}: {PACKAGE} has no live app PID')
        self.pids.update(pids)
        return pids[0]

    def wait_cold_start(self) -> tuple[ET.Element, str, str, tuple[int, int, int, int], str]:
        self.command('shell', 'am', 'start', '-W', '-n', ACTIVITY)
        self.wait_foreground(True)
        pid = self.remember_pid()
        root, full, line, counts = self.wait_for_hud()
        return root, full, line, counts, pid

    def screenshot(self, name: str) -> dict:
        data = self.command('exec-out', 'screencap', '-p', binary=True, timeout=30)
        if not data.startswith(b'\x89PNG\r\n\x1a\n'):
            raise RuntimeError(f'{self.serial}: screencap did not return PNG data ({len(data)} bytes)')
        path = self.output / name
        path.write_bytes(data)
        row = {'file': name, 'bytes': len(data), 'sha256': sha256_bytes(data)}
        self.screenshots.append(row)
        return row

    def collect_pid_logs(self):
        self.pid_logs = []
        for pid in sorted(self.pids, key=int):
            log = self.command('logcat', '-d', '--pid=' + pid, '-v', 'brief', timeout=30)
            path = self.output / f'app-pid-{pid}-logcat.txt'
            path.write_text(log, encoding='utf-8')
            fatal_lines = [line for line in log.splitlines()
                           if any(marker.lower() in line.lower() for marker in FATAL_MARKERS)]
            self.pid_logs.append({
                'pid': pid, 'file': path.name, 'sha256': sha256_bytes(log.encode('utf-8')),
                'fatal_error_count': len(fatal_lines), 'fatal_errors': fatal_lines,
            })

    def run(self) -> dict:
        self.output.mkdir(parents=True, exist_ok=True)
        self.inspect_device()
        self.install_and_verify()
        root, full_status, line, counts, first_pid = self.wait_cold_start()
        self.initial_hud = line
        initial_shot = self.screenshot('encounter-initial.png')

        # Snapshot the active encounter without synthetic input. Home/resume
        # performs the urgent lifecycle flush; active play can keep coalescing
        # position saves, so "Progress saved" is not a stable foreground label.
        self.checkpoint_hud = line
        checkpoint_counts = counts
        checkpoint_shot = self.screenshot('encounter-checkpoint-ready.png')

        # Home pauses gameplay and queues an urgent immutable snapshot off the
        # UI thread. Resume the task and verify the Activity remains healthy.
        self.shell('input', 'keyevent', 'KEYCODE_HOME')
        self.wait_foreground(False)
        time.sleep(1.0)
        self.command('shell', 'am', 'start', '-W', '-n', ACTIVITY)
        self.wait_foreground(True)
        home_pid = self.remember_pid()
        root, home_status, home_line, home_counts = self.wait_for_hud()
        if home_counts[2:] != checkpoint_counts[2:]:
            raise RuntimeError(f'{self.serial}: Home/resume changed sentry progress: '
                               f'{checkpoint_counts[2:]} -> {home_counts[2:]}')
        home_shot = self.screenshot('encounter-home-resume.png')
        self.after_home_hud = home_line

        # Force-stop from Home, then require a cold process to report restoration
        # and retain encounter progress. HP can decrease on the live frame after
        # resume because the authored sentries attack autonomously.
        self.shell('input', 'keyevent', 'KEYCODE_HOME')
        self.wait_foreground(False)
        pre_stop_pids = self.app_pids()
        self.command('shell', 'am', 'force-stop', PACKAGE)
        deadline = time.monotonic() + 8.0
        while time.monotonic() < deadline and self.app_pids():
            time.sleep(0.25)
        if self.app_pids():
            raise RuntimeError(f'{self.serial}: force-stop left package processes alive: {self.app_pids()}')
        time.sleep(0.4)
        root, cold_status, cold_line, cold_counts, cold_pid = self.wait_cold_start()
        if 'Progress restored.' not in cold_status:
            raise RuntimeError(f'{self.serial}: cold launch HUD did not confirm checkpoint restore: {cold_status!r}')
        if cold_counts[2:] != checkpoint_counts[2:]:
            raise RuntimeError(f'{self.serial}: cold relaunch changed sentry progress: '
                               f'{checkpoint_counts[2:]} -> {cold_counts[2:]}')
        cold_shot = self.screenshot('encounter-cold-relaunch.png')
        self.after_cold_hud = cold_line

        self.collect_pid_logs()
        fatal = [entry for entry in self.pid_logs if entry['fatal_error_count']]
        if fatal:
            raise RuntimeError(f'{self.serial}: fatal error(s) found in app-PID-filtered logs: {fatal!r}')
        # Re-hash the candidate to avoid reporting a hash for a file changed
        # while the device run was in progress.
        final_candidate_hash = sha256_file(self.apk)
        if final_candidate_hash != self.candidate_hash:
            raise RuntimeError('candidate APK changed during runtime validation')
        if self.installed_hash != self.candidate_hash:
            raise RuntimeError('installed APK no longer matches the candidate APK')
        return {
            'schema': 'dh2.android.gameplay-activity-lifecycle-runtime.v1',
            'result': 'pass', 'complete_game': False,
            'scope': 'exact APK install plus authored encounter checkpoint, Home/resume, and force-stop/cold-relaunch smoke test',
            'package': PACKAGE, 'activity': ACTIVITY,
            'device': self.device,
            'apk': {
                'candidate_path': self.apk.as_posix(),
                'candidate_sha256': self.candidate_hash,
                'installed_base_path': self.remote_base_path,
                'installed_base_copy': self.installed_copy.as_posix() if self.installed_copy else None,
                'installed_sha256': self.installed_hash,
                'installed_matches_candidate': self.installed_hash == self.candidate_hash,
                'bytes': self.apk.stat().st_size,
            },
            'data_cleared_before_first_launch': self.clear_data,
            'checkpoint': {
                'initial_hud': self.initial_hud,
                'persisted_hud': self.checkpoint_hud,
                'after_home_resume_hud': self.after_home_hud,
                'after_cold_relaunch_hud': self.after_cold_hud,
                'home_resume_sentry_progress_unchanged': home_counts[2:] == checkpoint_counts[2:],
                'cold_relaunch_sentry_progress_unchanged': cold_counts[2:] == checkpoint_counts[2:],
                'cold_relaunch_confirmed_restore': 'Progress restored.' in cold_status,
                'full_status_after_cold_relaunch': cold_status,
                'hp_after_cold_relaunch': cold_counts[0],
                'sentries_killed_after_cold_relaunch': cold_counts[2],
                'sentries_required_after_cold_relaunch': cold_counts[3],
            },
            'lifecycle': {
                'initial_cold_launch_pid': first_pid,
                'home_resume_completed': True,
                'home_resume_process_pid': home_pid,
                'cold_relaunch_completed': True,
                'force_stopped_pids': pre_stop_pids,
                'cold_relaunch_pid': cold_pid,
            },
            'screenshots': [initial_shot, checkpoint_shot, home_shot, cold_shot],
            'app_pid_fatal_errors': self.pid_logs,
            'logcat_cleared': False,
            'test_sha256': sha256_file(Path(__file__)),
        }


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--adb', type=Path, required=True, help='path to adb executable')
    parser.add_argument('--apk', type=Path, required=True, help='candidate APK to install and verify')
    parser.add_argument('--serial', required=True, help='explicit online Android 17 emulator serial')
    parser.add_argument('--expected-page-size', type=int, required=True, choices=(4096, 16384))
    parser.add_argument('--output', type=Path, required=True, help='directory for screenshots, logs, and JSON report')
    parser.add_argument('--clear-data', action='store_true',
                        help='clear only this package data before first launch (use on a disposable test emulator)')
    args = parser.parse_args()
    adb = args.adb.resolve(strict=True)
    apk = args.apk.resolve(strict=True)
    output = args.output.resolve()
    smoke = GameplayActivitySmoke(adb, apk, args.serial, args.expected_page_size,
                                  output, args.clear_data)
    error = None
    try:
        report = smoke.run()
    except Exception as exception:
        error = f'{type(exception).__name__}: {exception}'
        try:
            smoke.collect_pid_logs()
        except Exception as log_error:
            smoke.pid_logs.append({'collection_error': f'{type(log_error).__name__}: {log_error}'})
        report = {
            'schema': 'dh2.android.gameplay-activity-lifecycle-runtime.v1',
            'result': 'fail', 'complete_game': False,
            'scope': 'exact APK install plus authored encounter checkpoint, Home/resume, and force-stop/cold-relaunch smoke test',
            'package': PACKAGE, 'activity': ACTIVITY,
            'device': smoke.device,
            'apk': {
                'candidate_path': apk.as_posix(),
                'candidate_sha256': smoke.candidate_hash,
                'installed_base_path': smoke.remote_base_path,
                'installed_base_copy': smoke.installed_copy.as_posix() if smoke.installed_copy else None,
                'installed_sha256': smoke.installed_hash,
                'installed_matches_candidate': smoke.installed_hash == smoke.candidate_hash
                    if smoke.installed_hash else False,
                'bytes': apk.stat().st_size,
            },
            'data_cleared_before_first_launch': smoke.clear_data,
            'checkpoint': {
                'initial_hud': smoke.initial_hud,
                'persisted_hud': smoke.checkpoint_hud,
                'after_home_resume_hud': smoke.after_home_hud,
                'after_cold_relaunch_hud': smoke.after_cold_hud,
            },
            'screenshots': smoke.screenshots,
            'app_pid_fatal_errors': smoke.pid_logs,
            'logcat_cleared': False,
            'error': error,
            'test_sha256': sha256_file(Path(__file__)),
        }
    output.mkdir(parents=True, exist_ok=True)
    report_path = output / 'gameplay-activity-lifecycle-runtime-validation.json'
    report_path.write_text(json.dumps(report, indent=2, ensure_ascii=False) + '\n', encoding='utf-8')
    print(json.dumps(report, indent=2, ensure_ascii=False))
    return 0 if report['result'] == 'pass' else 1


if __name__ == '__main__':
    raise SystemExit(main())
