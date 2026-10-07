"""Verify the live original skill animation cycle and genuine Crypt NPC startup.

This proves a no-target cast only. Enemy skill damage, FX, complete AI, faery
casting, campaign progress and physical ARM64 devices remain outside this test.
"""
from pathlib import Path
import argparse, hashlib, json, re, subprocess, time, xml.etree.ElementTree as ET

def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--adb', required=True)
    parser.add_argument('--serial', required=True)
    parser.add_argument('--apk', type=Path, required=True)
    parser.add_argument('--output', type=Path, required=True)
    args = parser.parse_args()
    assert args.serial == 'emulator-5554'
    args.output.mkdir(parents=True, exist_ok=True)
    def raw(*commands):
        return subprocess.check_output([args.adb, '-s', args.serial, *commands], timeout=45)
    def adb(*commands):
        return raw(*commands).decode('utf8', 'replace').strip()
    digest = hashlib.sha256(args.apk.read_bytes()).hexdigest()
    installed = adb('shell', 'pm', 'path', 'com.example.dh2').removeprefix('package:')
    assert adb('shell', 'sha256sum', installed).split()[0] == digest
    pid = adb('shell', 'pidof', 'com.example.dh2')
    assert pid and ' ' not in pid
    def log():
        return adb('logcat', '-d', '--pid=' + pid, '-v', 'brief')
    startup = log()
    npcs = re.findall(r'Original NPC level state \| ([^|]+) \| current (\d+) \| flags ([0-9a-f]+) \| clip (\d+)', startup)
    assert len(npcs) == 11 and len(set(row[0] for row in npcs)) == 11
    assert all(row[1] == '3' and row[2] == '2380' for row in npcs)
    assert 'Original trophy manager retained | catalog 69' in startup
    adb('shell', 'uiautomator', 'dump', '/sdcard/dh2-skill-world.xml')
    xml = adb('shell', 'cat', '/sdcard/dh2-skill-world.xml')
    (args.output / 'skill-world-ui.xml').write_text(xml, encoding='utf8')
    nodes = ET.fromstring(xml)
    hud = next(node for node in nodes.iter('node') if node.get('content-desc') == 'Three equipped skills, faery spell, and health potion')
    x0, y0, x1, y1 = map(int, re.findall(r'\d+', hud.get('bounds')))
    before = len(startup)
    adb('shell', 'input', 'tap', str(round(x0 + (x1-x0)/10)), str((y0+y1)//2))
    screenshots = []
    for index in range(6):
        pixels = raw('exec-out', 'screencap', '-p')
        assert pixels.startswith(b'\x89PNG')
        path = args.output / f'skill-frame-{index}.png'
        path.write_bytes(pixels)
        screenshots.append(str(path.resolve()))
        time.sleep(.12)
    current = log()
    recent = current[before:]
    assert 'Skill activated' in recent
    assert re.search(r'Original skill activation \| row 0 \| source status 0 \| accepted 1 \| native targets 0 \| state 6 \| clip 1234', recent)
    assert re.search(r'Player source state \| previous 3 \| current 6 \| event 0xc355', recent)
    assert re.search(r'Player source state \| previous 6 \| current 3 \| event 0x22', recent)
    assert not re.search(r'Fatal signal|FATAL EXCEPTION|Native frame failed|GL error|Required source SkillState animation event', current)
    (args.output / 'skill-world-runtime.log').write_text(current, encoding='utf8')
    report = {'validation': 'PASS', 'scope': __doc__, 'apk_sha256': digest,
              'installed_apk_sha256': digest, 'pid': pid, 'original_npcs': npcs,
              'source_targetability_flags': '0x2380', 'source_trophy_rows': 69,
              'skill_row': 0, 'skill_clip': 1234, 'source_state_cycle': [3,6,3],
              'native_targets': 0, 'skill_damage_verified': False,
              'physical_arm64_tested': False, 'full_game_playable': False,
              'screenshots': screenshots}
    (args.output / 'skill-world-activation-smoke.json').write_text(json.dumps(report, indent=2)+'\n', encoding='utf8')
    print(json.dumps({key: value for key, value in report.items() if key not in ('scope','screenshots','original_npcs')}))

if __name__ == '__main__':
    main()
