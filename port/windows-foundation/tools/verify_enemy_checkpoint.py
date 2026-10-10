"""Run the packaged shared enemy AI; verify behavior, not full encounter fidelity."""
import argparse
import hashlib
import json
from pathlib import Path
import re
import subprocess

parser = argparse.ArgumentParser()
parser.add_argument('--package', type=Path, required=True)
parser.add_argument('--report', type=Path, required=True)
args = parser.parse_args()
package = args.package.resolve()
manifest = json.loads((package / 'package-receipt.json').read_text(encoding='utf-8'))
for entry in manifest['files']:
    asset = package / entry['path']
    assert asset.stat().st_size == entry['bytes'], entry['path']
    assert hashlib.sha256(asset.read_bytes()).hexdigest() == entry['sha256'], entry['path']

results = []
for name, frames in [('enemy-approach', 180), ('enemy-fight', 600)]:
    with (package / (name + '-verification.log')).open('w', encoding='utf-8') as output:
        result = subprocess.run([str(package / 'dh-foundation.exe'), '--startup-config',
                                 name + '-verification.args'], cwd=package,
                                stdout=output, stderr=subprocess.STDOUT, timeout=60)
    assert result.returncode == 0, (name, result.returncode)
    log = (package / (name + '-verification.log')).read_text(encoding='utf-8')
    assert f'Rendered frames={frames}; clean shutdown' in log, name
    hits = re.findall(r'Damage frame=(\d+) attacker=(\d+) target=(\d+) marker=(\S+) removed=([\d.]+) dead=([01])', log)
    enemy_hits = [hit for hit in hits if hit[1] == '7118915781085668844' and hit[2] == '18446744073709551615' and float(hit[4]) > 0]
    assert enemy_hits, (name, 'enemy did not damage player')
    enemy_final = re.search(r'Actor final id=7118915781085668844 .* position=([^\r\n]+)', log)
    assert enemy_final, name
    position = list(map(float, enemy_final[1].split(',')))
    if name == 'enemy-approach':
        assert position[1] < 800, (name, 'enemy did not approach', position)
    player_hits = [hit for hit in hits if hit[1] == '18446744073709551615' and hit[2] == '7118915781085668844']
    if name == 'enemy-fight':
        assert any(hit[5] == '1' for hit in player_hits), 'player did not kill enemy'
        assert re.search(r'Actor final id=7118915781085668844 .* HP=0/[^ ]+ target=0 action=5', log), 'death/target cleanup absent'
    results.append({'case': name, 'frames': frames, 'enemy_damage_events': enemy_hits,
                    'player_damage_events': player_hits, 'enemy_final_position': position})

report = {'status': 'packaged_enemy_approach_attack_and_death_verified', 'goal_complete': False,
          'package': str(package), 'executable_sha256': hashlib.sha256((package / 'dh-foundation.exe').read_bytes()).hexdigest(),
          'manifest_files_verified': len(manifest['files']), 'cases': results,
          'limitations': ['Explicit diagnostic placement, not original encounter activation.',
                          'Full mob population, original AI scheduling, navigation avoidance and visual fidelity remain incomplete.']}
args.report.parent.mkdir(parents=True, exist_ok=True)
args.report.write_text(json.dumps(report, indent=2) + '\n', encoding='utf-8')
print(json.dumps(report))
