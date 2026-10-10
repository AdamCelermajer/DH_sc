"""Execute and verify the packaged source combo independently of legacy cases."""
import argparse
import hashlib
import json
from pathlib import Path
import re
import subprocess
import tempfile

parser = argparse.ArgumentParser()
parser.add_argument('--package', required=True, type=Path)
parser.add_argument('--report', required=True, type=Path)
options = parser.parse_args()
package = options.package.resolve()
executable = package / 'dh-foundation.exe'
checks = []
for name, expected in [('combo-held', [0, 1, 2]), ('combo-release', [0]),
                       ('combo-accepted-release', [0, 1]), ('combo-blocked', [0])]:
    case = name + '-verification'
    with (package / (case + '.log')).open('w', encoding='utf-8') as output:
        result = subprocess.run([str(executable), '--startup-config', case + '.args'],
                                cwd=package, stdout=output, stderr=subprocess.STDOUT, timeout=60)
    content = (package / (case + '.log')).read_text(encoding='utf-8')
    assert result.returncode == 0, content
    assert 'Rendered frames=180; clean shutdown' in content, content
    hits = [int(index) for index in re.findall(r'Source combo hit frame=\d+ actor=18446744073709551615 index=(\d+)', content)]
    assert hits == expected, (case, hits, expected)
    boundaries = re.findall(r'Source combo boundary frame=(\d+) actor=18446744073709551615 generation=(\d+) begin=(\d+) depth=(\d+) step=(\d+) count=(\d+) clip=(\S+)', content)
    assert boundaries, case
    generations = {int(row[1]) for row in boundaries}
    assert len(generations) == 1, (case, generations)
    roots = [int(row[4]) for row in boundaries if row[2] == '1' and row[3] == '0']
    assert roots == expected, (case, roots, expected)
    strikes = [row[6] for row in boundaries if row[2] == '1' and row[3] == '1' and row[4] == '1']
    assert len(strikes) == len(expected) and len(set(strikes)) == len(expected), (case, strikes)
    for clip in strikes:
        assert (package / 'assets' / clip).is_file(), (case, clip)
    damage = re.findall(r'Damage frame=(\d+) attacker=18446744073709551615 target=(\d+) marker=attack_mainhand removed=([\d.]+) dead=(\d+)', content)
    assert len(damage) == len(expected), (case, damage)
    assert (package / (case + '.ppm')).is_file(), case
    checks.append({'case': case, 'exit_code': 0, 'source_groups': roots,
                   'action_generations': sorted(generations), 'original_strike_clips': strikes,
                   'damage_receipts': damage, 'rendered_frames': 180})

with tempfile.TemporaryDirectory(prefix='dh2-default-run-') as directory:
    config = Path(directory) / 'default-run.args'
    startup = (package / 'startup.args').read_text(encoding='utf-8').replace('--assets\nassets\n', '--assets\n' + str(package / 'assets') + '\n')
    config.write_text(startup +
                      '\n--fixed-step\n.016\n--frames\n90\n--move-axis\n1,0,0\n--move-frames\n60\n--capture\n' + str(package / 'default-run-verification.ppm') + '\n', encoding='utf-8')
    with (package / 'default-run-verification.log').open('w', encoding='utf-8') as output:
        result = subprocess.run([str(executable), '--startup-config', str(config)],
                                cwd=package, stdout=output, stderr=subprocess.STDOUT, timeout=60)
    content = (package / 'default-run-verification.log').read_text(encoding='utf-8')
    assert result.returncode == 0 and 'Rendered frames=90; clean shutdown' in content, content
    assert 'Actor final position=1573.77,-203.944,255 grounded=1' in content, content
    assert 'sourceXY=888.992 worldXY=518.686' in content, content
    assert '--move-run' not in config.read_text(), 'Default run fixture overrides the preference'

receipt = {'status': 'packaged_source_combo_verified', 'goal_complete': False,
           'executable_sha256': hashlib.sha256(executable.read_bytes()).hexdigest(),
           'checks': checks, 'default_run': {'exit_code': 0, 'rendered_frames': 90,
                                           'position': [1573.77, -203.944, 255],
                                           'source_travel': 888.992, 'world_travel': 518.686},
           'limits': ['Scripted held commands use the same admitted controller path; physical keyboard play is not established by these cases.',
                      'Source preattack/look/sticky/heading/OOI integration remains incomplete.',
                      'Restore resets transient combo state rather than resuming a mid-swing cursor.']}
options.report.resolve().write_text(json.dumps(receipt, indent=2) + '\n', encoding='utf-8')
print(json.dumps({'combo_cases': len(checks), 'source_groups': [row['source_groups'] for row in checks]}))
