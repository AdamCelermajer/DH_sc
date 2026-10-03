"""Exercise source player states and authored combat events on an emulator.

The caller must already install --apk. This script verifies the installed hash,
fresh-launches Crypt with enemy AI disabled, uses actual touch movement and the
debug broadcast into the same playerAttack UI bridge, and records bounded
source-state observations without pausing the Activity or changing held input.
It does not establish full FSM, blended-pose, GPU or physical ARM64 parity.
Required transition log contract: Player source state | previous P | current C
| event 0xE | flags HEX | root R | clip K | position X Y Z | Step N.
"""
import argparse
import hashlib
import json
import math
from pathlib import Path
import re
import subprocess
import struct
import time
import xml.etree.ElementTree as ET
import zipfile

NUMBER = r'[-+\d.eE]+'
TRANSITION = re.compile(
    r'Player source state \| previous (-?\d+) \| current (-?\d+) '
    r'\| event (0x[0-9a-fA-F]+|\d+) \| flags (?:0x)?([0-9a-fA-F]+) '
    r'\| root (-?\d+) \| clip (-?\d+) \| position (' + NUMBER + ') (' + NUMBER +
    ') (' + NUMBER + r') \| Step (\d+)')
EVENT = re.compile(r'Player animation event \| clip (-?\d+) \| name (\S+) '
                   r'\| sequence (-?\d+) \| attack step (-?\d+) \| kind (-?\d+) '
                   r'\| lag (-?\d+) \| (scene before Step|synchronous actor replay) \| Step (\d+)')
HIT = re.compile(r'Prince combat hit \| target (\S+) \| attempt (\d+) '
                 r'\| result ((?:-?\d+ ){9}-?\d+) \| HP (-?\d+) (-?\d+) '
                 r'\| dead (\d+) \| combo (\d+) \| RNG (\d+) (\d+) \| statuses (\d+)')
POSITION = re.compile(r'Player position (' + NUMBER + ') (' + NUMBER + ') (' + NUMBER +
                      r') \| moved (\d+) \| blocked (\d+)')
ERROR = re.compile(r'FATAL EXCEPTION|Fatal signal|GL error|load failed|sample failed|'
                   r'completion failed|event dispatch failed|combat route failed|'
                   r'application failed|death animation failed|Shader failed|Link failed|'
                   r'Native frame failed', re.IGNORECASE)


def inspect(apk):
    """Read actual packaged ABIs without requiring screenshot/image libraries."""
    libraries = []
    with zipfile.ZipFile(apk) as archive:
        for name in archive.namelist():
            if not name.startswith('lib/') or not name.endswith('.so'):
                continue
            raw = archive.read(name)
            abi = name.split('/')[1]
            assert abi in ('arm64-v8a', 'x86_64') and raw[:5] == b'\x7fELF\x02', name
            assert 'DungeonHunter2' not in name, 'Original ARM32 oracle must not be bundled'
            offset = struct.unpack_from('<Q', raw, 32)[0]
            size, count = struct.unpack_from('<HH', raw, 54)
            alignments = [struct.unpack_from('<Q', raw, offset+i*size+48)[0] for i in range(count)
                          if struct.unpack_from('<I', raw, offset+i*size)[0] == 1]
            assert alignments and min(alignments) >= 16384, name
            libraries.append({'path': name, 'sha256': hashlib.sha256(raw).hexdigest(),
                              'minimum_load_alignment': min(alignments)})
    assert {row['path'].split('/')[1] for row in libraries} == {'arm64-v8a', 'x86_64'}
    return libraries


def launch_fresh(adb, *extras):
    # No package install is performed, so package-update relaunch retries are
    # unnecessary. A rejected/intercepted launch remains a visible test failure.
    adb('shell', 'am', 'force-stop', 'com.example.dh2')
    result = adb('shell', 'am', 'start', '-W', '-n', 'com.example.dh2/.MainActivity', *extras)
    assert 'Status: ok' in result and 'Activity: com.example.dh2/.MainActivity' in result, result
    assert 'Activity not started' not in result, result
    return result


def transitions(text):
    rows = []
    for match in TRANSITION.finditer(text):
        values = match.groups()
        position = [float(value) for value in values[6:9]]
        assert all(math.isfinite(value) for value in position), 'Nonfinite source-state position'
        rows.append({'previous': int(values[0]), 'current': int(values[1]),
                     'event': int(values[2], 0), 'flags': int(values[3], 16),
                     'root': int(values[4]), 'clip': int(values[5]),
                     'position': position, 'step': int(values[9]), 'offset': match.start()})
    return rows


def verify_case(text, predecessor, root, target):
    states = transitions(text)
    starts = [row for row in states if row['previous'] == predecessor and row['current'] == 5]
    assert len(starts) == 1, ('Missing/duplicate source attack entry', predecessor, states)
    start = starts[0]
    assert start['flags'] == 0x2341 and start['root'] == root, start
    ends = [row for row in states if row['offset'] > start['offset'] and row['previous'] == 5 and row['current'] == 3]
    assert len(ends) == 1 and ends[0]['event'] == 0x22 and ends[0]['flags'] == 0x2380, ('Finite attack did not close into source Idle', ends)
    end = ends[0]
    events = [(match, match.groups()) for match in EVENT.finditer(text)
              if start['offset'] <= match.start() < end['offset']]
    scene_events = [values for _, values in events if values[6] == 'scene before Step']
    assert scene_events, 'No authored events observed during the scene phase'
    named_melee = [values for values in scene_events if values[1] in ('attack_mainhand', 'attack_offhand')]
    assert named_melee, 'No authored melee trigger reached the player pipeline'
    assert all(int(values[7]) >= start['step'] and int(values[7]) < end['step'] for values in named_melee), ('Scene event did not precede a later completed physics Step', named_melee, end)
    hit_matches = [match for match in HIT.finditer(text)
                   if start['offset'] <= match.start() < end['offset']]
    assert hit_matches, 'Source event did not reach the native damage helper'
    hits = []
    for match in hit_matches:
        values = match.groups()
        assert values[0] == target, (values[0], target)
        preceding = [values for event, values in events if event.start() < match.start()
                     and values[1] in ('attack_mainhand', 'attack_offhand')]
        assert preceding and preceding[-1][6] == 'scene before Step', 'Hit lacks a preceding scene-phase authored melee event'
        hits.append({'attempt': int(values[1]), 'result': list(map(int, values[2].split())),
                     'hp_before': int(values[3]), 'hp_after': int(values[4]),
                     'dead': int(values[5]), 'combo': int(values[6]),
                     'random_after': [int(values[7]), int(values[8])], 'status_requests': int(values[9])})
    assert 'Player clip completed |' not in text and 'Player death clip completed |' not in text, 'Legacy second-cursor completion path is still executing'
    assert len(re.findall(r'Player attack selected \|', text)) == 1, 'Busy input restarted the attack'
    displacement = math.dist(start['position'], end['position'])
    if predecessor == 4:
        assert displacement > .1, ('Moving attack lost authored displacement', start, end)
    return {'source_start': start, 'source_end': end, 'authored_scene_events': len(scene_events),
            'authored_melee_events': len(named_melee), 'native_hits': hits,
            'displacement_units': displacement, 'source_finite_event': '0x22',
            'legacy_cursor_completion_absent': True}


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--adb', required=True)
    parser.add_argument('--serial', required=True)
    parser.add_argument('--apk', type=Path, required=True)
    parser.add_argument('--output', type=Path, required=True)
    args = parser.parse_args()
    assert args.serial.startswith('emulator-'), 'This smoke operates on a development emulator only'
    args.output.mkdir(parents=True, exist_ok=True)
    apk_hash = hashlib.sha256(args.apk.read_bytes()).hexdigest()
    libraries = inspect(args.apk)
    movement = []
    cases = {}
    last_log = ''
    operated = False
    report = {'validation': 'FAIL', 'apk_sha256': apk_hash, 'installed_apk_sha256': None,
              'libraries': libraries, 'scope': __doc__, 'physical_arm64_tested': False,
              'full_original_fsm_or_gpu_parity_claimed': False}

    def adb(*arguments, missing=False):
        result = subprocess.run([args.adb, '-s', args.serial, *arguments], text=True,
                                capture_output=True, timeout=45)
        if missing and result.returncode == 1 and not (result.stdout + result.stderr).strip():
            return ''
        if result.returncode:
            raise RuntimeError('adb ' + repr(arguments) + ': ' + result.stdout + result.stderr)
        return result.stdout.strip()

    def logs():
        nonlocal last_log
        pid = adb('shell', 'pidof', 'com.example.dh2', missing=True)
        assert pid, 'App exited'
        last_log = adb('logcat', '-d', '--pid=' + pid, '-v', 'brief')
        assert not ERROR.search(last_log), last_log[-8000:]
        return last_log

    def wait(predicate, seconds=30):
        deadline = time.monotonic() + seconds
        while True:
            text = logs()
            if predicate(text):
                return text
            assert time.monotonic() < deadline, text[-8000:]
            time.sleep(.1)

    def views():
        adb('shell', 'uiautomator', 'dump', '/sdcard/dh2-character-combat-window.xml')
        tree = ET.fromstring(adb('shell', 'cat', '/sdcard/dh2-character-combat-window.xml'))
        return {node.get('content-desc'): tuple(map(int, re.findall(r'\d+', node.get('bounds'))))
                for node in tree.iter('node') if node.get('content-desc')}

    def axis(axes, bounds, action='DOWN'):
        x0, y0, x1, y1 = bounds
        x = round((x0+x1)/2 + axes[0]*(x1-x0)*.44)
        y = round((y0+y1)/2 - axes[1]*(x1-x0)*.44)
        adb('shell', 'input', 'touchscreen', 'motionevent', action, str(x), str(y))

    def position(text):
        matches = list(POSITION.finditer(text))
        if matches:
            values = matches[-1].groups()
            result = [float(value) for value in values[:3]]
        else:
            values = re.findall(r'World ready .*?position ('+NUMBER+') ('+NUMBER+') ('+NUMBER+')', text)
            assert values, 'Missing live player position'
            result = list(map(float, values[-1]))
        assert all(math.isfinite(value) for value in result)
        return result

    def travel(index, destination, bounds):
        velocities = {}
        for _ in range(50):
            wait(lambda text: bool(transitions(text)) and transitions(text)[-1]['current']==3)
            before = logs()
            start = position(before)
            distance = destination-start[index]
            if abs(distance) < 35:
                return
            # Use Walk for the final approach. Run has enough authored motion
            # during even a short pulse to overshoot a nearby waypoint.
            mode = 'Walk' if abs(distance) < 200 else 'Run'
            velocity = velocities.get(mode)
            axes = [0., 0.]
            magnitude = .35 if mode == 'Walk' else .9
            axes[index] = magnitude if distance > 0 else -magnitude
            duration = .2 if velocity is None else min(.6, max(.08, abs(distance)/velocity*.65))
            duration_ms = round(duration*1000)
            count = len(POSITION.findall(before))
            offset = len(before)
            x0, y0, x1, y1 = bounds
            x = round((x0+x1)/2 + axes[0]*(x1-x0)*.44)
            y = round((y0+y1)/2 - axes[1]*(x1-x0)*.44)
            command_start = time.monotonic()
            # Android emits real DOWN/MOVE/UP at this fixed joystick point.
            # Keep log polling and ADB round trips outside the held interval:
            # they previously added ~250ms and caused route overshoot.
            adb('shell', 'input', 'touchscreen', 'swipe', str(x), str(y),
                str(x), str(y), str(duration_ms))
            command_seconds = time.monotonic()-command_start
            wait(lambda text: any(row['current']==4 for row in transitions(text[offset:])))
            wait(lambda text: len(POSITION.findall(text)) > count)
            wait(lambda text: bool(transitions(text)) and transitions(text)[-1]['current']==3)
            count = len(POSITION.findall(logs()))
            axis((0, 0), bounds);axis((0, 0), bounds, 'UP')
            after = position(wait(lambda text: len(POSITION.findall(text)) > count))
            change = abs(after[index]-start[index])
            movement.append({'axis': index, 'start': start, 'end': after,
                             'requested_hold_seconds': duration_ms/1000,
                             'input_duration_ms': duration_ms,
                             'input_method': 'stationary Android touchscreen swipe',
                             'locomotion': mode, 'command_wall_seconds': command_seconds,
                             'native_walk_entry_observed': True, 'settled_idle_measured': True})
            assert change > .1, 'Touch waypoint blocked; needs an explicit reviewed setup adapter, not a fabricated position'
            velocities[mode] = change/(duration_ms/1000)
        raise AssertionError('Crypt waypoint not reached by actual movement controls')

    def command_attack():
        result = adb('shell', 'am', 'broadcast', '-a', 'com.example.dh2.DEBUG_PLAYER_ATTACK',
                     '-p', 'com.example.dh2', '--ei', 'player_target_index', '4')
        assert 'Broadcast completed: result=0' in result, result
        return result

    def capture(name):
        remote = '/sdcard/dh2-character-combat.png'
        adb('shell', 'screencap', '-p', remote)
        adb('pull', remote, str(args.output/(name+'.png')))

    try:
        remote = adb('shell', 'pm', 'path', 'com.example.dh2').removeprefix('package:')
        assert '\n' not in remote and remote.endswith('.apk'), 'Expected one installed base APK'
        installed = adb('shell', 'sha256sum', remote).split()[0]
        report['installed_apk_sha256'] = installed
        assert installed == apk_hash, 'Install the supplied current APK before running this smoke'
        operated = True
        launch_fresh(adb, '--ez', 'enemy_ai', 'false', '--es', 'world', 'crypt01.dwld')
        initial = wait(lambda text: 'World ready |' in text and 'Native actor ready |' in text
                       and 'Model frame submitted at' in text)
        assert re.search(r'AI tables ready .*automatic melee 0', initial), 'Enemy AI was not disabled'
        assert transitions(initial), 'Missing reviewed Player source state observation contract'
        controls = views()
        bounds = controls['Movement control']
        # Retain the existing real UI rejection route without depending on its
        # former secondary animation cursor or long damage-sequence fixture.
        x0, y0, x1, y1 = controls['Attack nearby enemy']
        out_of_reach_offset = len(logs())
        adb('shell', 'input', 'tap', str((x0+x1)//2), str((y0+y1)//2))
        rejected = wait(lambda text: 'Player input | Walk closer to an enemy' in text[out_of_reach_offset:])
        assert not HIT.search(rejected[out_of_reach_offset:]), 'Out-of-reach input applied damage'
        assert not any(row['current'] == 5 for row in transitions(rejected[out_of_reach_offset:])), 'Out-of-reach input entered Attack'
        report['out_of_reach_ui_rejected'] = True
        for index, destination in ((1, -1000), (0, -1390), (1, -370)):
            travel(index, destination, bounds)
        wait(lambda text: bool(transitions(text)) and transitions(text)[-1]['current'] == 3)
        adb('shell', 'am', 'start', '-W', '--activity-single-top', '-n', 'com.example.dh2/.MainActivity',
            '--ei', 'object_index', '4', '--ei', 'time_ms', '-1')
        inspected = wait(lambda text: 'Inspect object 4 |' in text)
        targets = re.findall(r'Inspect object 4 \| \S+ \| (\S+) \| room \d+ \| position '+NUMBER+' '+NUMBER+' '+NUMBER, inspected)
        assert targets, 'Required source skeleton object is absent'
        target = targets[-1]
        report['target'] = {'index': 4, 'name': target}
        for label, predecessor, root in (('stationary', 3, 243), ('moving', 4, 248)):
            offset = len(logs())
            if predecessor == 4:
                axis((.35, 0), bounds)
                wait(lambda text: any(row['current'] == 4 for row in transitions(text[offset:])))
            command_attack()
            started = wait(lambda text: 'Player attack selected |' in text[offset:])
            assert any(row['previous'] == predecessor and row['current'] == 5 for row in transitions(started[offset:])), 'Attack predecessor/root selection was not source-observed'
            # Busy request must return without selecting/resetting another clip.
            command_attack()
            wait(lambda text: 'Player command applied | Attack is already in progress' in text[offset:])
            if predecessor == 4:
                wait(lambda text: any(values[1] in ('attack_mainhand', 'attack_offhand')
                                      and values[6] == 'scene before Step'
                                      for values in EVENT.findall(text[offset:])))
                axis((0, 0), bounds, 'UP')
            complete = wait(lambda text: any(row['previous'] == 5 and row['current'] == 3
                                            and row['event'] == 0x22 for row in transitions(text[offset:])), 40)
            time.sleep(.25)
            complete = logs()
            fragment = complete[offset:]
            cases[label] = verify_case(fragment, predecessor, root, target)
            # Retained logs after Idle must not contain delayed duplicate hits.
            closure = cases[label]['source_end']['offset']
            assert not HIT.search(fragment[closure:]), 'Hit emitted after finite source Idle closure'
            (args.output/(label+'.log')).write_text(fragment)
            capture(label+'-idle')
        assert any(hit['hp_after'] < hit['hp_before'] for case in cases.values() for hit in case['native_hits']), 'No native damage reached target HP'
        report.update(validation='PASS', cases=cases, movement=movement,
                      enemy_ai_disabled=True, finite_source_idle_closure=True,
                      moving_attack_displacement_preserved=True,
                      legacy_second_cursor_completion_absent=True,
                      death_case='Not run; existing defender harness is separate and uses repeated actual enemy events.')
    except Exception as error:
        report.update(error=str(error), cases=cases, movement=movement)
        raise
    finally:
        (args.output/'character-combat.log').write_text(last_log)
        (args.output/'character-combat-smoke.json').write_text(json.dumps(report, indent=2)+'\n')
        if operated:
            try:
                adb('shell', 'input', 'touchscreen', 'motionevent', 'CANCEL', '0', '0')
            except Exception:
                pass
    print(json.dumps({'validation': report['validation'], 'apk_sha256': apk_hash,
                      'cases': list(cases), 'moving_displacement': cases['moving']['displacement_units']}))


if __name__ == '__main__':
    main()
