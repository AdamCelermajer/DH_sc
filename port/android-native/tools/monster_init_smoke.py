"""Verify original monster initialization and persistent sessions on an emulator.

Caller installs the supplied APK first. This tests the eleven packaged Crypt
placements and rotation ownership, not complete AI, campaign or phone parity.
"""
import argparse
import hashlib
import json
import math
from pathlib import Path
import re
import struct
import subprocess
import time
import zipfile
import xml.etree.ElementTree as ET
from PIL import Image, ImageChops
from emulator_smoke import launch_fresh
from live_actor_smoke import BAD, PACKAGE, inspect_apk

FLOAT = r'[-+\d.eE]+'
SESSION = re.compile(
    r'Monster script session \| (\S+) \| character (\S+) \| identity ([0-9a-f]+) '
    r'\| session ([0-9a-f]+) \| retained (\d+) \| files (\d+) \| step (\d+) '
    r'\| flags ([0-9a-f]+) \| level raw (-?\d+) \| HP (-?\d+) (-?\d+) '
    r'\| MP (-?\d+) (-?\d+) \| saved ('+FLOAT+r') ('+FLOAT+r') '
    r'\| flee (\d+) \| buff ('+FLOAT+r') \| timers (-?\d+) (\d+) (-?\d+) (\d+) '
    r'\| registrations (\d+) \| unavailable (\d+)')
READY = re.compile(r'Monster scripts ready \| initialized (\d+) \| retained (\d+)')
SCENE_OBJECT = re.compile(r'Scene script object \| (\S+) \| identity ([0-9a-f]+) '
    r'\| CharAI ([0-9a-f]+) \| retained (\d+) \| target ([0-9a-f]+) '
    r'\| source GetID and HasTarget verified')
PLAYER_OBJECT = re.compile(r'Player script object \| identity ([0-9a-f]+) '
    r'\| CharAI ([0-9a-f]+) \| retained (\d+) \| properties ([0-9a-f]+) '
    r'\| life ([0-9a-f]+) \| HP (-?\d+) \| dead (\d+) \| checksum ([0-9a-f]+) '
    r'\| position ('+FLOAT+r') ('+FLOAT+r') ('+FLOAT+r') \| shared gameplay backing verified')
SETTINGS = re.compile(r'Design settings ready \| rows (\d+) \| Default index (-?\d+) '
    r'\| fields (\d+) \| EnemySpottedAggro bits ([0-9a-f]+) '
    r'\| records consumed (\d+) \| names consumed (\d+) \| retained (\d+) '
    r'\| backing ([0-9a-f]+) \| owned cache data')
POSITION_METHOD = re.compile(r'Scene position methods \| (\S+) \| identity ([0-9a-f]+) '
    r'\| position ('+FLOAT+r') ('+FLOAT+r') ('+FLOAT+r') \| player identity ([0-9a-f]+) '
    r'\| player position ('+FLOAT+r') ('+FLOAT+r') ('+FLOAT+r') \| retained (\d+) '
    r'\| original object GetPosition verified')
BACKING = re.compile(r'Monster native backing \| (\S+) \| character (\S+) '
    r'\| resources (\d+) \| occurrences (\d+) \| template (-?\d+) '
    r'\| CPU instance ([0-9a-f]+) \| retained (\d+) \| preset (-?\d+) '
    r'\| authored state (\d+) \| CPU playback prepared \| live FSM pending')
INITIALIZATION = re.compile(r'Actor initialization ready \| records (\d+) \| sources (\d+) '
    r'\| DACT SHA ([0-9a-f]+) \| sidecar SHA ([0-9a-f]+) \| independently hashed bytes')
EFFECTS = re.compile(r'Effects native backing \| sets (\d+) \| character rows (\d+) \| footsteps (\d+) '
    r'\| dictionary (\d+) \| consumed (\d+) (\d+) (\d+) \| snapshot ([0-9a-f]+) '
    r'\| modules ([0-9a-f]+) \| retained (\d+) \| source cache owned \| FX factory pending')


def effects_backing(text,retained):
    rows = [row for row in EFFECTS.findall(text) if int(row[9]) == retained]
    assert len(rows) == 1
    row = rows[0]
    assert tuple(map(int,row[:7])) == (276,3,7,284,12700,12755,12953)
    assert int(row[7],16) and int(row[8],16)
    return dict(sets=276,character_rows=3,footsteps=7,dictionary=284,
                consumed=[12700,12755,12953],snapshot=row[7],modules=row[8])


def prepared_backings(text, expected, manifest, retained):
    tables = {'Crypt_Skeleton': (23,44,1203), 'CryptSlime': (23,37,1267),
              'CryptSlime_RE': (23,37,1267), 'Crypt_Ghost': (18,28,716)}
    authored = {row['name']:row for row in manifest['records']}
    result = {}
    for row in BACKING.findall(text):
        if int(row[6]) != retained:
            continue
        name,character = row[:2]
        assert name in expected and name not in result and character == authored[name]['character']
        assert tuple(map(int,row[2:5])) == tables[character]
        assert int(row[5],16) and int(row[7]) == authored[name]['resolved']['preset_state'] == 3
        assert int(row[8]) == int(authored[name]['authored']['ai_state'] is not None)
        result[name] = dict(character=character,resources=int(row[2]),occurrences=int(row[3]),
                            template=int(row[4]),CPU_instance=row[5],preset=3,authored_state=int(row[8]))
    assert set(result) == set(expected) and len({row['CPU_instance'] for row in result.values()}) == 11
    rows = INITIALIZATION.findall(text)
    assert len(rows) == 1 and tuple(map(int,rows[0][:2])) == (11,3)
    assert rows[0][2:] == (manifest['descriptor_sha256'],manifest['binary_sha256'])
    return result


def owned_settings(text, retained):
    rows = [row for row in SETTINGS.findall(text) if int(row[6]) == retained]
    assert len(rows) == 1
    row = rows[0]
    assert tuple(map(int,(row[0],row[1],row[2],row[4],row[5]))) == (1,0,43,176,15)
    assert int(row[3],16) == 0x41200000 and int(row[7],16)
    return dict(rows=1,Default_index=0,fields=43,EnemySpottedAggro_bits=row[3],
                records_consumed=176,names_consumed=15,backing=row[7])


def object_positions(text, expected, player, retained):
    result = {}
    for row in POSITION_METHOD.findall(text):
        if int(row[9]) != retained:
            continue
        name = row[0]
        assert name in expected and name not in result
        point = list(map(float,row[2:5]))
        player_point = list(map(float,row[6:9]))
        assert int(row[1],16) == expected[name]['identity']
        assert math.dist(point,expected[name]['position']) < .01
        assert row[5] == player['identity'] and math.dist(player_point,player['position']) < .01
        result[name] = dict(identity=row[1],position=point,player_identity=row[5],player_position=player_point)
    assert set(result) == set(expected)
    return result


def player_object(text, retained):
    rows = [row for row in PLAYER_OBJECT.findall(text) if int(row[2]) == retained]
    assert len(rows) == 1, ('Missing/duplicate native player record', rows)
    row = rows[0]
    assert int(row[0],16) == 0x100000001
    assert all(int(row[index],16) for index in (1,3,4))
    assert int(row[5]) > 0 and int(row[6]) == 0
    position = list(map(float,row[8:11]))
    assert all(math.isfinite(value) for value in position)
    marker = re.search(r'Player properties \| KnightPlayerBase \| HP (-?\d+) '
        r'\| MP (-?\d+) \| checksum ([0-9a-f]+)',text)
    assert marker and marker[1] == row[5] and marker[3] == row[7]
    return dict(identity=row[0],CharAI=row[1],properties=row[3],life=row[4],
                hp=int(row[5]),dead=int(row[6]),checksum=row[7],position=position)


def scene_objects(text, expected, retained):
    result = {}
    for match in SCENE_OBJECT.finditer(text):
        name, identity, ai, restored, target = match.groups()
        if int(restored) != retained:
            continue
        assert name in expected and name not in result
        assert int(identity,16) == expected[name]['identity'] and int(ai,16)
        assert int(target,16) == 0
        result[name] = dict(identity=identity,CharAI=ai,target=target)
    assert set(result) == set(expected)
    assert len({row['CharAI'] for row in result.values()}) == 11
    return result


def sha(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()


def placements(apk):
    with zipfile.ZipFile(apk) as archive:
        raw = archive.read('assets/worlds/crypt01.dact')
        assert raw[:4] == b'DACT' and len(raw) == 16 + struct.unpack_from('<I', raw, 8)[0]*256
        result = {}
        for index in range(struct.unpack_from('<I', raw, 8)[0]):
            offset = 16 + index*256
            if struct.unpack_from('<I', raw, offset)[0] != 1:
                continue
            name = raw[offset+8:offset+72].split(b'\0')[0].decode('ascii')
            result[name] = dict(character=raw[offset+72:offset+136].split(b'\0')[0].decode('ascii'),
                identity=0x100000002+index, position=list(struct.unpack_from('<3f', raw, offset+200)))
        assert len(result) == 11
        return result, hashlib.sha256(raw).hexdigest()


def sessions(text, expected, retained):
    result = {}
    for match in SESSION.finditer(text):
        values = match.groups()
        if int(values[4]) != retained:
            continue
        name = values[0]
        assert name in expected and name not in result, ('Unknown/duplicate monster', name)
        source = expected[name]
        assert values[1] == source['character'] and int(values[2], 16) == source['identity']
        assert int(values[3], 16) and tuple(map(int, values[5:7])) == (2, 7)
        # Current scene explicitly selects the original Crypt01 normal range
        # [8,10] with host level1. These constants are independently source-
        # verified in character-host-context/level-table gold, not guessed AI.
        assert int(values[8]) == 8*256
        hp, max_hp, mp, max_mp = map(int, values[9:13])
        assert hp == max_hp and hp > 0 and mp == max_mp and mp >= 0
        saved = list(map(float, values[13:15]))
        assert all(math.isfinite(value) for value in saved)
        assert all(abs(a-b) < .002 for a, b in zip(saved, source['position'][:2]))
        assert int(values[15]) == 1 and float(values[16]) == 48
        # Exact authored CharacterDesign AI_Tick/DoT_Tick cache values.
        assert tuple(map(int, values[17:21])) == (0x33, 3000, 0x34, 1000)
        assert int(values[21]) == 170 and int(values[22]) > 0
        result[name] = dict(character=values[1], identity=values[2], session=values[3],
            flags=values[7], level_raw=int(values[8]), vitals=[hp,max_hp,mp,max_mp],
            saved=saved, timers=list(map(int,values[17:21])), unavailable=int(values[22]))
    assert set(result) == set(expected), ('Missing original monster sessions', set(expected)-set(result))
    assert len({row['session'] for row in result.values()}) == 11
    return result


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--adb', required=True)
    parser.add_argument('--player-object',action='store_true',help='Verify retained real player scene backing')
    parser.add_argument('--owned-target-stage',action='store_true',help='Verify authored settings and original object GetPosition')
    parser.add_argument('--prepared-monster-backings',type=Path,
        help='Verify CPU animation resources and authored initialization using this source manifest; no live FSM claim')
    parser.add_argument('--effects-tables-stage',type=Path,help='Verify exact bundled effects streams and retained native owners')
    parser.add_argument('--shader-assets-stage',type=Path,help='Verify additional unmodified authored shader pack and members')
    parser.add_argument('--ui-assets-stage',type=Path,nargs=3,metavar=('MENU','TEXT','TEXTURE'))
    parser.add_argument('--serial', required=True)
    parser.add_argument('--apk', type=Path, required=True)
    parser.add_argument('--output', type=Path, required=True)
    parser.add_argument('--scene-objects', action='store_true',
        help='Require stable live GetID/HasTarget/CharAI records across rotation')
    args = parser.parse_args()
    assert args.serial.startswith('emulator-')
    args.output.mkdir(parents=True, exist_ok=True)
    expected, descriptor_hash = placements(args.apk)
    report = dict(validation='FAIL', scope=__doc__, apk_sha256=sha(args.apk),
        descriptor_sha256=descriptor_hash, libraries=inspect_apk(args.apk),
        physical_arm64_verified=False, full_enemy_AI=False, full_game_verified=False)
    initialization = None
    if args.ui_assets_stage:
        from inspect_character_init_build import ui_assets
        assert args.shader_assets_stage
        with zipfile.ZipFile(args.apk) as apk:
            assets = {name:hashlib.sha256(apk.read(name)).hexdigest()
                      for name in apk.namelist() if name.startswith('assets/')}
        report['ui_assets_stages'] = ui_assets(args.ui_assets_stage,assets)
    if args.prepared_monster_backings:
        initialization = json.loads(args.prepared_monster_backings.read_text())
        assert initialization['descriptor_sha256'] == descriptor_hash
        with zipfile.ZipFile(args.apk) as apk:
            assert hashlib.sha256(apk.read('assets/worlds/crypt01-actor-initialization.bin')).hexdigest() == initialization['binary_sha256']
            assert len([name for name in apk.namelist() if name.startswith('assets/')]) == (770 if args.ui_assets_stage else (347 if args.effects_tables_stage else 342) + (35 if args.shader_assets_stage else 0))
        report['initialization_manifest_sha256'] = sha(args.prepared_monster_backings)
    if args.effects_tables_stage:
        stage = json.loads(args.effects_tables_stage.read_text())
        assert initialization and stage['validation'] == 'PASS' and stage['assets_per_project'] == 347
        with zipfile.ZipFile(args.apk) as apk:
            for name,digest in stage['asset_sha256'].items():
                assert hashlib.sha256(apk.read('assets/'+name)).hexdigest() == digest
        report['effects_tables_stage_sha256'] = sha(args.effects_tables_stage)
    if args.shader_assets_stage:
        shader_stage = json.loads(args.shader_assets_stage.read_text())
        assert args.effects_tables_stage and shader_stage['validation'] == 'PASS'
        assert shader_stage['shader_member_count'] == 34 and len(shader_stage['asset_sha256']) == 35
        with zipfile.ZipFile(args.apk) as apk:
            for name,digest in shader_stage['asset_sha256'].items():
                assert hashlib.sha256(apk.read('assets/'+name)).hexdigest() == digest
        report['shader_assets_stage_sha256'] = sha(args.shader_assets_stage)
    commands, last_log, prior = [], '', None

    def adb(*command):
        process = subprocess.run([args.adb, '-s', args.serial, *command],
                                 capture_output=True, text=True, timeout=45)
        commands.append(dict(arguments=list(command), returncode=process.returncode,
                             stdout=process.stdout, stderr=process.stderr))
        assert process.returncode == 0, commands[-1]
        return process.stdout.strip()

    def wait(expected_ready):
        nonlocal last_log
        deadline = time.monotonic()+40
        while True:
            last_log = adb('logcat', '-d', '--pid='+report['pid'], '-v', 'brief')
            assert not BAD.search(last_log), last_log[-10000:]
            markers = list(READY.finditer(last_log))
            if (markers and tuple(map(int, markers[-1].groups())) == expected_ready
                    and 'Model frame submitted at ' in last_log[markers[-1].end():]):
                return last_log
            assert time.monotonic() < deadline, last_log[-10000:]
            time.sleep(.15)

    def capture_rendered(stem):
        # A GL submit marker precedes compositor presentation. Require actual
        # geometry in the captured viewport, preserving premature captures.
        deadline = time.monotonic()+20
        attempt = 0
        while True:
            adb('shell','uiautomator','dump','/sdcard/dh2-monster-init.xml')
            xml = adb('shell','cat','/sdcard/dh2-monster-init.xml')
            root = ET.fromstring(xml)
            nodes = [node for node in root.iter('node') if node.get('content-desc') == 'DH2 native texture viewport']
            assert len(nodes) == 1
            viewport = tuple(map(int,re.findall(r'-?\d+',nodes[0].get('bounds'))))
            path = args.output/(stem+'-attempt'+str(attempt)+'.png')
            adb('shell','screencap','-p','/sdcard/dh2-monster-init.png')
            adb('pull','/sdcard/dh2-monster-init.png',str(path))
            with Image.open(path) as picture:
                crop = picture.convert('RGB').crop(viewport)
                difference = ImageChops.difference(crop,Image.new('RGB',crop.size,(24,28,32)))
                mask = difference.convert('L').point(lambda value:255 if value>8 else 0)
                pixels = mask.histogram()[255]
                area = crop.width*crop.height
            if pixels > area*.2:
                (args.output/(stem+'.png')).write_bytes(path.read_bytes())
                (args.output/(stem+'.xml')).write_text(xml,encoding='utf-8')
                report.setdefault('rendered_viewports',{})[stem] = dict(viewport=list(viewport),
                    pixels=pixels,area=area,attempts=attempt+1)
                return
            assert time.monotonic()<deadline, ('Viewport remained blank',stem,pixels,area)
            attempt += 1
            time.sleep(.15)

    try:
        installed = adb('shell', 'pm', 'path', PACKAGE).splitlines()
        assert len(installed) == 1 and installed[0].startswith('package:')
        report['installed_apk_sha256'] = adb('shell', 'sha256sum', installed[0][8:]).split()[0]
        assert report['installed_apk_sha256'] == report['apk_sha256']
        prior = adb('shell', 'cmd', 'window', 'user-rotation').split()
        assert prior and prior[0] in ('lock', 'free')
        adb('shell', 'cmd', 'window', 'user-rotation', 'lock', '0')
        launch_fresh(adb, '--es', 'world', 'crypt01.dwld', '--ez', 'enemy_ai', 'false')
        report['pid'] = adb('shell', 'pidof', PACKAGE)
        assert report['pid']
        initial = wait((11, 0))
        assert 'Debug switches file | opened 0 | errno 2 | app-private directory' in initial
        assert 'host cached level 1 |' in initial and '| difficulty 0 | authored levels 51 |' in initial
        report['initial'] = sessions(initial, expected, 0)
        if args.scene_objects:
            report['scene_objects_initial'] = scene_objects(initial, expected, 0)
        if args.player_object:
            assert args.scene_objects
            report['player_object_initial'] = player_object(initial,0)
            assert report['player_object_initial']['CharAI'] not in {
                row['CharAI'] for row in report['scene_objects_initial'].values()}
        if args.owned_target_stage:
            assert args.player_object
            report['design_settings_initial'] = owned_settings(initial,0)
            report['object_positions_initial'] = object_positions(initial,expected,report['player_object_initial'],0)
        if initialization:
            report['prepared_backings_initial'] = prepared_backings(initial,expected,initialization,0)
        if args.effects_tables_stage:
            report['effects_backing_initial'] = effects_backing(initial,0)
        capture_rendered('portrait')
        offset = len(initial)
        adb('shell', 'cmd', 'window', 'user-rotation', 'lock', '1')
        restored = wait((0, 11))
        assert adb('shell', 'pidof', PACKAGE) == report['pid']
        report['restored'] = sessions(restored[offset:], expected, 1)
        assert report['restored'] == report['initial'], 'Rotation replaced VM/state or reran Init'
        if args.scene_objects:
            report['scene_objects_restored'] = scene_objects(restored[offset:],expected,1)
            assert report['scene_objects_restored'] == report['scene_objects_initial']
            assert all(row['unavailable'] == 138 for row in report['initial'].values())
            report['live_scene_object_bindings'] = True
        if args.player_object:
            report['player_object_restored'] = player_object(restored[offset:],1)
            first,last = report['player_object_initial'],report['player_object_restored']
            assert {k:v for k,v in first.items() if k!='position'} == {
                k:v for k,v in last.items() if k!='position'}
            assert all(abs(a-b)<.01 for a,b in zip(first['position'],last['position']))
            report['live_player_scene_backing'] = True
        if args.owned_target_stage:
            report['design_settings_restored'] = owned_settings(restored[offset:],1)
            report['object_positions_restored'] = object_positions(restored[offset:],expected,report['player_object_restored'],1)
            assert report['design_settings_restored'] == report['design_settings_initial']
            assert report['object_positions_restored'] == report['object_positions_initial']
            report['live_owned_design_settings'] = True
            report['live_original_object_GetPosition'] = True
        if initialization:
            report['prepared_backings_restored'] = prepared_backings(restored[offset:],expected,initialization,1)
            assert report['prepared_backings_restored'] == report['prepared_backings_initial']
            report['retained_monster_CPU_resources'] = True
            report['independently_hashed_initialization'] = True
            report['live_monster_native_FSM'] = False
        if args.effects_tables_stage:
            report['effects_backing_restored'] = effects_backing(restored[offset:],1)
            assert report['effects_backing_restored'] == report['effects_backing_initial']
            report['retained_effects_native_backing'] = True
            report['FX_factory_ready'] = False
        assert len(list(SESSION.finditer(restored))) == 22
        capture_rendered('landscape')
        report['screenshots'] = {name: sha(args.output/name) for name in ('portrait.png','landscape.png')}
        report['validation'] = 'PASS'
    finally:
        if prior:
            adb('shell', 'cmd', 'window', 'user-rotation', *prior)
        (args.output/'monster-init.log').write_text(last_log+'\n', encoding='utf-8')
        (args.output/'adb-transcript.json').write_text(json.dumps(commands, indent=2)+'\n')
        (args.output/'monster-init-smoke.json').write_text(json.dumps(report, indent=2)+'\n')
    print(json.dumps(dict(validation=report['validation'], monsters=11,
        retained_sessions=11, apk_sha256=report['apk_sha256'])))


if __name__ == '__main__':
    main()
