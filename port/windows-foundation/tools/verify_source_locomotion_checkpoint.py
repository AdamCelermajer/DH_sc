"""Exercise original equipment-driven Walk/Run on the packaged floor/controller."""
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
base = (package / 'swamp.args').read_text(encoding='utf-8')
cases = []
start = [1090.75, -212.202, 255.0]
for mode, sequence in (('walk', 280), ('run', 271)):
    name = 'source-' + mode + '-verification'
    config = base + '\n' + '\n'.join([
        '--position', ','.join(map(str, start)), '--fixed-step', '.016',
        '--frames', '30', '--move-axis', '0,1,0', '--move-frames', '30',
        '--move-' + mode, '--attack-start-frame', '5000', '--attack-frames', '1',
        '--capture', name + '.ppm']) + '\n'
    (package / (name + '.args')).write_text(config, encoding='utf-8')
    log_path = package / (name + '.log')
    with log_path.open('w', encoding='utf-8') as output:
        result = subprocess.run([str(package / 'dh-foundation.exe'), '--startup-config', name + '.args'],
                                cwd=package, stdout=output, stderr=subprocess.STDOUT, timeout=90)
    assert result.returncode == 0, (mode, result.returncode, str(log_path))
    log = log_path.read_text(encoding='utf-8')
    assert 'Source player stance=0 main=664 off=-1' in log, mode
    clock = re.search(rf'/seq-{sequence}/step-0 timelineMs=(\d+)', log)
    assert clock and int(clock.group(1)) > 0, 'Original locomotion clock did not advance'
    position = re.search(r'Actor final position=([^\s]+) grounded=1', log)
    assert position, 'Player lost source floor support'
    final = list(map(float, position.group(1).split(',')))
    assert sum((a - b) ** 2 for a, b in zip(final, start)) > 1.0, 'Input failed to move player'
    assert 'Rendered frames=30; clean shutdown' in log
    cases.append({'mode': mode, 'source_sequence': sequence, 'timeline_ms': int(clock.group(1)),
                  'initial_position': start, 'final_position': final, 'grounded': True})
report = {'status': 'packaged_source_walk_run_and_floor_verified', 'goal_complete': False,
          'executable_sha256': hashlib.sha256((package / 'dh-foundation.exe').read_bytes()).hexdigest(),
          'cases': cases,
          'limits': ['Knight current/empty gear enrollment only; other class combat is incomplete.',
                     'Matched-reference movement appearance and complete navigation fidelity remain unverified.']}
args.report.parent.mkdir(parents=True, exist_ok=True)
args.report.write_text(json.dumps(report, indent=2) + '\n', encoding='utf-8')
print(json.dumps(report))
