"""Exercise source Crypt contact -> Wait -> Spawn on API37/16KiB.

The authored trigger/scripts stay unchanged. A temporary fan spawn override
places the player near the trigger; actual touch movement must enter it.
"""
import argparse
import hashlib
import json
from pathlib import Path
import re
import struct
import subprocess
import time
import xml.etree.ElementTree as ET
import zipfile
from emulator_smoke import inspect, launch_fresh

PACKAGE = 'com.example.dh2'
NAMES = ('_prim_Monster_SURPRISE_01', '_prim_Monster_SURPRISE_02')
BAD = re.compile(r'FATAL EXCEPTION|Fatal signal|Native frame failed|World load failed|Model draw GL error')


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--adb', required=True)
    parser.add_argument('--serial', required=True)
    parser.add_argument('--apk', type=Path, required=True)
    parser.add_argument('--output', type=Path, required=True)
    parser.add_argument('--require-source-pursuit', action='store_true',
                        help='enable enemy AI and require two independent source Ghost body pursuits')
    parser.add_argument('--require-native-level-catalogue', action='store_true',
                        help='require the actual native catalogue reader on load, reload and recreation')
    parser.add_argument('--require-native-level-fields', action='store_true',
                        help='require original constructor field selection and source range callbacks on load/reload/recreation')
    parser.add_argument('--require-native-monster-initialization', action='store_true',
                        help='require unchanged monster OnInit on native owners, real Debug file and retained damaged health/VM/timers')
    parser.add_argument('--require-native-character-list', action='store_true',
                        help='require actual native Character enrollment and owned source-list nodes on load/reload/recreation')
    parser.add_argument('--require-native-ghost-skill-initialization', action='store_true',
                        help='require real Ghost null-script vectors and ordered InitScriptProcess without replay')
    parser.add_argument('--require-native-frame-foundations', action='store_true',
                        help='require live source classification/zonability across Ghost initialization/recreation; autonomous frames remain pending')
    args = parser.parse_args()
    assert args.serial.startswith('emulator-')
    out = args.output.resolve()
    out.mkdir(parents=True, exist_ok=True)
    report = {'validation': 'FAIL', 'apk_sha256': hashlib.sha256(args.apk.read_bytes()).hexdigest(),
              'serial': args.serial, 'full_game_playable': False, 'physical_arm64_phone_tested': False,
              'authored_trigger_and_script_modified': False, 'player_start_is_fan_test_override': True}
    transcript = []
    last_logs = ''
    pid = ''
    log_since = ''
    prior = []
    remote = f'/sdcard/Android/data/{PACKAGE}/files/mods/worlds/crypt01.dwld'
    backup = out / 'previous-world.dwld'
    existed = None
    deadline = time.monotonic() + 210

    def adb(*command, missing=False, cleanup=False):
        if not cleanup and time.monotonic() > deadline:
            raise RuntimeError('Crypt script smoke deadline exceeded')
        run = subprocess.run([args.adb, '-s', args.serial, *command], capture_output=True, text=True, timeout=30)
        transcript.append({'args': list(command), 'returncode': run.returncode, 'stderr': run.stderr,
                           'stdout': run.stdout if command[0] != 'logcat' else '[stored separately]',
                           **({'stdout_utf8_sha256': hashlib.sha256(run.stdout.strip().encode('utf-8')).hexdigest()}
                              if command[0] == 'logcat' else {})})
        if run.returncode and not missing:
            raise RuntimeError(str(command) + '\n' + run.stdout + run.stderr)
        return run.stdout.strip() if not run.returncode else ''

    def logs():
        nonlocal last_logs
        current = adb('shell', 'pidof', PACKAGE)
        assert current and current == pid, 'app exited or process changed'
        last_logs = adb('logcat', '-d', '-T', log_since, '--pid=' + pid, '-v', 'brief')
        assert not BAD.search(last_logs), 'native failure in logs'
        return last_logs

    def wait(predicate, label, timeout=30):
        end = min(deadline, time.monotonic() + timeout)
        while time.monotonic() < end:
            value = logs()
            if predicate(value):
                return value
            time.sleep(.15)
        raise AssertionError(label + ' not observed')

    def launch():
        nonlocal pid, log_since
        log_since = adb('shell', 'date', '+%s.%N')
        launch_fresh(adb, '--es', 'world', 'crypt01.dwld', '--ez', 'enemy_ai',
                     'true' if args.require_source_pursuit else 'false')
        pid = adb('shell', 'pidof', PACKAGE)
        report.setdefault('launch_log_boundaries', []).append({'pid': pid, 'epoch_since': log_since})
        return wait(lambda text: 'Crypt script ready | common 15 | level 25 | script 17' in text and
                    'Native actor frame |' in text, 'source script/actor ready')

    def capture(name):
        run = subprocess.run([args.adb, '-s', args.serial, 'exec-out', 'screencap', '-p'], capture_output=True, timeout=30)
        assert run.returncode == 0 and run.stdout.startswith(b'\x89PNG')
        path = out / (name + '.png')
        path.write_bytes(run.stdout)
        report.setdefault('screenshots', []).append({'path': path.name, 'sha256': hashlib.sha256(run.stdout).hexdigest()})

    def movement_bounds():
        adb('shell', 'uiautomator', 'dump', '/sdcard/dh2-crypt-script-window.xml')
        xml = adb('shell', 'cat', '/sdcard/dh2-crypt-script-window.xml')
        for node in ET.fromstring(xml).iter('node'):
            if node.get('content-desc') == 'Movement control':
                return tuple(map(int, re.findall(r'-?\d+', node.get('bounds'))))
        raise AssertionError('movement control absent')

    def debug_file(name):
        run = subprocess.run([args.adb, '-s', args.serial, 'exec-out', 'run-as', PACKAGE,
                              'cat', 'files/DebugSwitches.savegame'], capture_output=True, timeout=30)
        assert run.returncode == 0, 'app-private source Debug file absent'
        data = run.stdout
        assert len(data) >= 4 and struct.unpack_from('<I', data)[0] > 0, 'source Debug file malformed'
        (out / (name + '.savegame')).write_bytes(data)
        return {'bytes': len(data), 'sha256': hashlib.sha256(data).hexdigest()}

    def monster_rows(value):
        return re.findall(r'Native monster initialization \| (\S+) \| Level (-?\d+) \| HP (-?\d+) / (-?\d+) \| MP (-?\d+) / (-?\d+) \| callbacks (\d+) \| timers (\d+) \| flags ([0-9a-f]+) \| same VM published (\d+) \| retained (\d+) \| paused providers (\d+)', value)

    try:
        report['libraries'] = inspect(args.apk)
        assert adb('shell', 'getconf', 'PAGE_SIZE') == '16384'
        report['page_size'] = 16384
        report['api_level'] = int(adb('shell', 'getprop', 'ro.build.version.sdk'))
        assert report['api_level'] >= 37
        assert 'Success' in adb('install', '-r', str(args.apk))
        installed = adb('shell', 'pm', 'path', PACKAGE).splitlines()[0].removeprefix('package:')
        report['installed_apk_sha256'] = adb('shell', 'sha256sum', installed).split()[0]
        assert report['installed_apk_sha256'] == report['apk_sha256']
        prior = adb('shell', 'cmd', 'window', 'user-rotation').split()
        adb('shell', 'cmd', 'window', 'user-rotation', 'lock', '0')
        probe = subprocess.run([args.adb, '-s', args.serial, 'shell', 'test', '-e', remote], capture_output=True)
        assert probe.returncode in (0, 1)
        existed = probe.returncode == 0
        if existed:
            adb('pull', remote, str(backup))
        adb('shell', 'mkdir', '-p', str(Path(remote).parent).replace('\\', '/'))
        adb('shell', 'rm', '-f', remote)
        launch()
        time.sleep(.5)
        text = logs()
        assert 'Crypt trigger activated |' not in text and 'Crypt script SpawnCharacter |' not in text
        report['bundled_start_does_not_trigger_remote_ambush'] = True
        (out / 'bundled-start.log').write_text(text, encoding='utf-8')

        with zipfile.ZipFile(args.apk) as archive:
            raw = bytearray(archive.read('assets/worlds/crypt01.dwld'))
            descriptor = archive.read('assets/scripts/crypt-ghost01.dctr')
            provenance = json.loads(archive.read('assets/scripts/crypt-script-provenance.json'))
            actor_records = json.loads(archive.read('assets/actor-provenance.json'))['records']
            gates = {row['name']: index for index, row in enumerate(actor_records) if row.get('gated_spawn')}
            legacy = next(index for index, row in enumerate(actor_records) if row['kind'] == 1 and not row.get('gated_spawn'))
            assert set(gates) == set(NAMES)
            center = struct.unpack_from('<3f', descriptor, 12)
            scale = struct.unpack_from('<3f', descriptor, 24)
            assert descriptor[:4] == b'DCTR' and len(descriptor) == 44
            assert hashlib.sha256(descriptor).hexdigest() == provenance['descriptor_sha256']
            for item in provenance['inputs']:
                if item['name'].endswith('.bin'):
                    data = archive.read('assets/scripts/' + item['name'])
                    assert hashlib.sha256(data).hexdigest() == item['sha256']
            target = [center[0], center[1] + 100 * scale[1] + 250, center[2]]
            struct.pack_into('<3f', raw, 12, *target)
            report['authored_trigger_descriptor_sha256'] = provenance['descriptor_sha256']
            report['test_spawn'] = target
        fixture = out / 'near-authored-trigger.dwld'
        fixture.write_bytes(raw)
        adb('push', str(fixture), remote)
        text = launch()
        time.sleep(.3)
        text = logs()
        assert 'Crypt trigger activated |' not in text, 'test start must be outside source AABB'
        assert all('Gated character ready | ' + name + ' | source state 0' in text for name in NAMES)
        assert all('Spawn visibility | ' + name + ' | visible 0 | enabled 1' in text for name in NAMES)
        if args.require_native_monster_initialization:
            fresh = monster_rows(text)
            assert len(fresh) == 2 and {row[0] for row in fresh} == set(NAMES), 'two native monster owners missing'
            for row in fresh:
                assert int(row[1]) == 2048 and int(row[2]) == int(row[3]) > 256 and int(row[4]) == int(row[5])
                assert tuple(map(int, (row[6], row[7], row[9], row[10], row[11]))) == (1, 2, 1, 0, 2)
                assert int(row[8], 16) == 0x183c
            report['native_monster_initial_properties'] = fresh
            report['native_debug_file_after_initialization'] = debug_file('native-debug-after-initialization')
        capture('outside-authored-trigger')
        left, top, right, bottom = movement_bounds()
        x = round((left + right) / 2)
        y = round((top + bottom) / 2 + (right - left) * .44 * .9)
        adb('shell', 'input', 'touchscreen', 'motionevent', 'DOWN', str(x), str(y))
        try:
            text = wait(lambda value: 'Crypt trigger activated | GhostAmbush01 | count 1' in value,
                        'real touch movement into authored trigger', timeout=12)
        finally:
            adb('shell', 'input', 'touchscreen', 'motionevent', 'UP', str(x), str(y), cleanup=True)
        text = wait(lambda value: 'Crypt script completed | GhostAmbush01' in value and all(
            'Spawn source event | ' + name + ' | event 0x22 | current 3 | body 1' in value for name in NAMES),
                    'authored script and both finite Spawn animations complete')
        spawns = re.findall(r'Crypt script SpawnCharacter \| (\S+) \| script (\d+) \| command (\d+) \| time (\d+) \| result (\S+)', text)
        assert [(name, int(script), int(command), result) for name, script, command, _, result in spawns] == [
            (NAMES[0], 17, 1, 'requested'), (NAMES[1], 17, 3, 'requested')]
        waits = [tuple(map(int, row)) for row in re.findall(r'Crypt script Wait \| duration (\d+) \| time (\d+) \| dt (\d+)', text)]
        assert len(waits) == 2 and [row[0] for row in waits] == [250, 75]
        for spawn, (duration, started, initial_delta) in zip(spawns, waits):
            assert int(spawn[3]) > started and int(spawn[3]) - started + initial_delta >= duration
        for name in NAMES:
            assert text.count('Spawn body ready | ' + name + ' | creations 1 |') == 1
            assert text.count('Spawn visibility | ' + name + ' | visible 1 | enabled 1') == 1
        report['source_visibility_hides_then_restores_enabled_ghosts'] = True
        report['source_spawn_requests'] = [{'name': row[0], 'script_id': int(row[1]), 'pc': int(row[2]),
                                           'time_ms': int(row[3])} for row in spawns]
        report['source_wait_start_frames'] = [{'duration': row[0], 'time_ms': row[1], 'frame_dt': row[2]} for row in waits]
        report['actual_touch_contact_triggered_original_script'] = True
        if args.require_source_pursuit:
            from ghost_ai_runtime_evidence import validate_pursuit
            def pursued(value):
                try:
                    validate_pursuit(value)
                    return True
                except AssertionError:
                    return False
            text = wait(pursued, 'two source Ghost acquisition/Lua/path/body pursuits', timeout=30)
            report['source_ghost_pursuit'] = validate_pursuit(text)
            report['native_source_pursuit_verified'] = True
        capture('source-ghost-pair-idle')
        rejected_pairs = [(legacy, gates[NAMES[0]]), (gates[NAMES[0]], legacy),
                          (gates[NAMES[1]], -2)]
        for source, target_index in rejected_pairs:
            offset = len(text)
            adb('shell', 'am', 'start', '-f', '0x20000000', '-n', PACKAGE + '/.MainActivity',
                '--ei', 'object_index', str(source), '--ei', 'combat_target_index', str(target_index),
                '--ez', 'enemy_ai', 'false')
            marker = f'Combat target rejected | index {source} | target {target_index} | gated Character services pending'
            text = wait(lambda value: marker in value[offset:] and
                        'Gated actor combat services are pending' in value[offset:], 'gated combat ownership boundary')
            assert not any(token in text[offset:] for token in
                           ('Combat target selected |', 'Native combat hit |', 'Spawn source state |'))
        report['unbound_gated_combat_target_pairs_rejected'] = rejected_pairs
        # The development target probes also focus their source object. Put
        # the camera back on the Prince before checking reload/recreation;
        # otherwise a correct restored view can leave him outside the frame.
        offset = len(text)
        adb('shell', 'am', 'start', '-f', '0x20000000', '-n', PACKAGE + '/.MainActivity',
            '--ei', 'object_index', '-1', '--ez', 'enemy_ai', 'false')
        # A reused Activity reports "Actor state unchanged"; a newly created
        # Activity reports its loaded world. Both routes apply this focus.
        text = wait(lambda value: 'Actor command applied | index -1 | state null | time -1 |'
                    in value[offset:], 'return camera focus to the player after target probes')
        report['camera_focus_restored_to_player_after_target_probes'] = True
        damage_after = None
        if args.require_native_monster_initialization:
            offset = len(text)
            adb('shell', 'am', 'broadcast', '-a', PACKAGE + '.DEBUG_CHARACTER_HIT', '-p', PACKAGE,
                '--es', 'character_name', NAMES[0], '--ei', 'raw_damage', '256')
            text = wait(lambda value: 'Debug native monster damage | ' + NAMES[0] in value[offset:],
                        'nonlethal live source health fixture')
            damage = re.findall(r'Debug native monster damage \| (\S+) \| raw damage (\d+) \| HP before (-?\d+) \| after (-?\d+)', text[offset:])
            assert len(damage) == 1 and damage[0][0] == NAMES[0] and int(damage[0][1]) == 256
            assert int(damage[0][2]) - int(damage[0][3]) == 256
            damage_after = int(damage[0][3])
            report['nonlethal_source_health_fixture'] = {'name': NAMES[0], 'damage': 256,
                                                       'before': int(damage[0][2]), 'after': damage_after,
                                                       'full_combat_damage_calculation_tested': False}
        offset = len(text)
        adb('shell', 'am', 'broadcast', '-a', PACKAGE + '.DEBUG_RELOAD_WORLD', '-p', PACKAGE)
        text = wait(lambda value: 'Crypt script restored | activations 1 | requests 2 | ready 1 | running 0' in value[offset:] and
                    'World reload command applied |' in value[offset:], 'retained source script after world reload')
        assert 'Crypt script SpawnCharacter |' not in text[offset:] and 'Crypt trigger activated |' not in text[offset:]
        assert all('Gated character ready | ' + name + ' | source state 3 | presentation visible 1' in text[offset:] for name in NAMES)
        report['reload_preserves_consumed_trigger_and_script'] = True
        offset = len(text)
        adb('shell', 'cmd', 'window', 'user-rotation', 'lock', '1')
        text = wait(lambda value: 'Crypt script restored | activations 1 | requests 2 | ready 1 | running 0' in value[offset:] and
                    all('Gated character ready | ' + name + ' | source state 3' in value[offset:] for name in NAMES),
                    'retained script across Activity recreation')
        time.sleep(.5)
        text = logs()
        assert 'Crypt script SpawnCharacter |' not in text[offset:] and 'Crypt trigger activated |' not in text[offset:]
        assert all('Gated character ready | ' + name + ' | source state 3 | presentation visible 1' in text[offset:] for name in NAMES)
        report['recreation_preserves_script_and_ghost_states_without_replay'] = True
        if args.require_native_level_catalogue:
            catalogue = 'Native level catalogue | fast travel 33 | levels 51 | Crypt row 23 | ranges 8 10 / 45 47 / 74 76 | GSLevel ownership pending'
            assert catalogue in text[offset:], 'native catalogue missing after recreation'
            assert text.count(catalogue) >= 3, 'native catalogue did not survive load/reload/recreation'
            report['native_catalogue_load_reload_recreation'] = True
            report['native_catalogue_current_level_owner_bound'] = False
        if args.require_native_level_fields:
            fields = 'Native Level fields | ordinal 23 | hub 2 | random 1 | difficulty 0 | file 007_crypt_01.rule.xml | source ranges 8 10 / 45 47 / 74 76 | viewport owner; GSLevel stack pending'
            assert fields in text[offset:] and text.count(fields)>=3, 'native source Level fields/ranges missing on load/reload/recreation'
            report['native_level_constructor_fields_and_range_callbacks'] = True
            report['native_level_fields_owner'] = 'viewport-owned bounded projection; development normal difficulty argument'
            report['original_gslevel_save_stack_bound'] = False
        if args.require_native_monster_initialization:
            rows = monster_rows(text)
            # am start target-boundary probes may recreate the GL context
            # before damage is applied. Health preservation is checked only
            # at the two explicit restores after the live health fixture.
            after_damage = monster_rows(text[text.index('Debug native monster damage | ' + NAMES[0]):])
            owners = re.findall(r'Native monster VM owner \| (\S+) \| native handle ([0-9a-f]+) \| source active ([0-9a-f]+) \| source pending ([0-9a-f]+)', text)
            timers = re.findall(r'Native monster timer retained \| (\S+) \| slot (\d+) \| event ([0-9a-f]+) \| duration (\d+) \| elapsed (\d+) \| active (\d+) \| paused (\d+)', text)
            for name in NAMES:
                actor_rows = [row for row in rows if row[0] == name]
                assert len(actor_rows) >= 3 and int(actor_rows[0][10]) == 0
                assert all(int(row[10]) == 1 for row in actor_rows[1:]), 'OnInit reran during reload/recreation'
                assert all((int(row[1]), int(row[6]), int(row[7]), int(row[8], 16), int(row[9]), int(row[11])) ==
                           (2048, 1, 2, 0x183c, 1, 2) for row in actor_rows)
                if name == NAMES[0]:
                    damaged_restores = [row for row in after_damage if row[0] == name]
                    assert len(damaged_restores) >= 2
                    assert all(int(row[2]) == damage_after for row in damaged_restores), 'recreation healed damaged live monster'
                else:
                    assert all(int(row[2]) == int(row[3]) for row in actor_rows)
                handles = [row[1:] for row in owners if row[0] == name]
                assert len(handles) >= 3 and all(row == handles[0] and row[1] == row[2] for row in handles)
                actor_timers = [row[1:] for row in timers if row[0] == name]
                assert len(actor_timers) >= 6
                baseline = {row[1]: row for row in actor_timers[:2]}
                assert set(baseline) == {'33', '34'}
                assert all(row == baseline[row[1]] and tuple(map(int, row[-3:])) == (0, 1, 1) for row in actor_timers)
            persisted = debug_file('native-debug-after-recreation')
            assert persisted == report['native_debug_file_after_initialization'], 'native Debug file changed during graphics restore'
            report['native_debug_file_after_recreation'] = persisted
            counters = re.findall(r'Native Debug persistence \| loaded (\d+) \| switches (\d+) \| read opens (\d+) \| read closes (\d+) \| saves (\d+) \| write closes (\d+) \| IO errors (\d+)', text)
            # Original GetSwitch inserts a missing false Lua_LoadMemUsage entry
            # when SetSkillsAndSpells runs. It does not add a save in this path.
            expected_switches = 25 if args.require_native_ghost_skill_initialization else 24
            assert len(counters) >= 3 and all(tuple(map(int, row)) == (1, expected_switches, 1, 1, 5, 5, 0) for row in counters)
            report['native_debug_source_io_counters'] = list(map(int, counters[0]))
            report['native_monster_unchanged_oninit_live_properties'] = True
            report['native_monster_vm_health_and_timers_retained'] = True
            report['unfinished_ai_dot_timer_providers_paused'] = True
            report['native_character_function_registration_scope'] = '10 supported OnInit closures; complete 265 ordered bindings pending'
            report['autonomous_ghost_ai_and_full_init_script_process_complete'] = False
        if args.require_native_character_list:
            character_lists = re.findall(
                r'Native Character list \| characters (\d+) \| owned nodes (\d+) \| copied links (\d+) \| full ObjectManager factory (\d+) \| autonomous Ghost AI (\d+)', text)
            assert len(character_lists) >= 3, 'native Character list missing on load/reload/recreation'
            assert all(tuple(map(int, row)) == (14, 14, 0, 0, 0) for row in character_lists), 'native Character ownership/list scope changed'
            assert 'Native Character list |' in text[offset:], 'native Character list missing after Activity recreation'
            report['native_character_list_load_reload_recreation'] = True
            report['native_character_list_counts'] = [list(map(int, row)) for row in character_lists]
            report['native_character_list_scope'] = 'Prince and 13 live monster projections; native owned source-list nodes; full ObjectManager factory/name-map and autonomous Ghost AI remain pending'
        if args.require_native_ghost_skill_initialization:
            assert args.require_native_monster_initialization, 'skill gate also requires VM/health initialization gate'
            catalogue = 'Native skill catalogue | skill lists 36 | skills 127 | faery lists 4 | faeries 16 | shared immutable tables; full skill callbacks pending'
            assert text.count(catalogue) >= 3 and catalogue in text[offset:], 'skill catalogue missing after recreation'
            skill_rows = re.findall(
                r'Native Ghost skill initialization \| (\S+) \| phases (\d+) \| skills (\d+) \| faeries (\d+) \| null faeries (\d+) \| post (\d+) \| final (\d+) \| update slots (\d+) (\d+) \| updates (\d+) \| arguments (\d+) (\d+) \| Debug (\d+) (\d+) \| VCB (\d+) \| path (\S+) \| retained (\d+)', text)
            vector_rows = re.findall(r'Native Ghost skill vector owner \| (\S+) \| faery storage ([0-9a-f]+) \| catalogue ([0-9a-f]+)', text)
            for name in NAMES:
                selected = [row for row in skill_rows if row[0] == name]
                assert len(selected) >= 3 and int(selected[0][-1]) == 0, 'fresh ordered Ghost init absent'
                assert all(tuple(map(int, row[1:15])) == (12345, 0, 5, 5, 1, 1, 0, 5, 0, 2, 2, 2, 2, 1)
                           and row[15] == 'data/scripts/ai/' for row in selected), 'Ghost init phases/vectors/providers differ'
                assert all(int(row[-1]) == 1 for row in selected[1:]), 'Ghost init replayed during graphics restoration'
                owned = [row[1:] for row in vector_rows if row[0] == name]
                assert len(owned) >= 3 and all(row == owned[0] for row in owned), 'Ghost vector/catalogue backing replaced'
                assert all(int(handle, 16) for handle in owned[0]), 'missing Ghost vector/catalogue backing'
            report['native_skill_catalogue_load_reload_recreation'] = True
            report['native_ghost_ordered_init_script_process'] = True
            report['native_ghost_skill_initialization_rows'] = skill_rows
            report['native_ghost_vector_catalogue_backing_retained'] = True
            report['native_ghost_nonempty_skill_scripts_supported'] = False
            report['native_ghost_skill_scope'] = 'Exact authored Ghost empty SkillList/five zero-script faeries; HP/MP, SetSkillsAndSpells, UpdateAllSkills, Post and Final once on the same VM. Nonempty skill Lua, other AIS factories and autonomous frames remain pending.'
        capture('authored-ambush-restored')
        report['validation'] = 'PASS'
        if args.require_native_frame_foundations:
            classifications = re.findall(
                r'Native Character classification \| ([^|]+) \| AI (\d+) \| faction (\d+) \| type (\d+) \| monster (\d+) \| player (\d+) \| faerie (\d+) \| NPC (\d+) \| projected death (\d+)', text)
            zonability = re.findall(r'Native Character zonability \| ([^|]+) \| zonable (\d+)', text)
            verified = {}
            for name in NAMES:
                rows = [list(map(int, row[1:])) for row in classifications if row[0] == name]
                zones = [int(row[1]) for row in zonability if row[0] == name]
                assert len(rows) >= 7 and len(zones) == len(rows), (name, rows, zones)
                assert all(row == rows[0] and row[2:] == [4, 1, 0, 0, 0, 0] for row in rows), (name, rows)
                assert all(value == 1 for value in zones), (name, zones)
                verified[name] = {'AI_id': rows[0][0], 'faction_id': rows[0][1],
                                  'type': 4, 'monster': 1, 'player': 0, 'faerie': 0,
                                  'NPC': 0, 'projected_death': 0, 'zonable': 1,
                                  'observations': len(rows)}
            report['native_frame_foundation_character_facts'] = verified
            report['native_frame_foundation_scope'] = 'Source cached-ID/type/NPC predicates select the native Ghost monster AIS and legacy nongated enemy path; zonability is diagnostic only. Ghost type4 does not use the type0 name branch. DACT instance name and normalized port death are adapter inputs, without original name/dead-byte producer parity. Autonomous frame/culling/zone-enrollment providers remain pending.'
        report['scope'] = 'Original GhostAmbush01 contact/timed spawning on authored placement, approached with actual root-motion touch input from an explicit fan spawn override; source completion/body creation and reload/recreation; no complete-level or full AI claim.'
    except Exception as error:
        report['error'] = str(error)
        raise
    finally:
        if existed is not None:
            try:
                if existed:
                    adb('push', str(backup), remote, cleanup=True)
                else:
                    adb('shell', 'rm', '-f', remote, cleanup=True)
                report['previous_world_override_restored'] = True
            except Exception as error:
                report['cleanup_error'] = str(error)
                report['validation'] = 'FAIL'
        if prior:
            try:
                adb('shell', 'cmd', 'window', 'user-rotation', *prior, cleanup=True)
            except Exception as error:
                report['rotation_cleanup_error'] = str(error)
                report['validation'] = 'FAIL'
        (out / 'crypt-script.log').write_text(last_logs, encoding='utf-8')
        (out / 'adb-transcript.json').write_text(json.dumps(transcript, indent=2) + '\n', encoding='utf-8')
        report['logcat_sha256'] = hashlib.sha256((out / 'crypt-script.log').read_bytes()).hexdigest()
        report['adb_transcript_sha256'] = hashlib.sha256((out / 'adb-transcript.json').read_bytes()).hexdigest()
        report['source_camera_snapshot_count'] = len(re.findall(r'Source camera frame \|', last_logs))
        (out / 'crypt-script-smoke.json').write_text(json.dumps(report, indent=2) + '\n', encoding='utf-8')
    assert report['validation'] == 'PASS'
    print(json.dumps({key: value for key, value in report.items() if key != 'libraries'}, indent=2))


if __name__ == '__main__':
    main()
