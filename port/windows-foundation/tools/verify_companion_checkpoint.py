"""Verify packaged noncombat source Idle playback across gameplay persistence."""
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
log_path = package / 'companion-verification.log'
with log_path.open('w', encoding='utf-8') as output:
    result = subprocess.run([str(package / 'dh-foundation.exe'), '--startup-config', 'companion-verification.args'], cwd=package, stdout=output, stderr=subprocess.STDOUT, timeout=60)
assert result.returncode == 0, result.returncode
log = log_path.read_text(encoding='utf-8')
assert 'Saved live checkpoint frame=40' in log
assert 'Content unloaded and reloaded at frame=80' in log
assert 'Restored live checkpoint frame=100' in log
assert 'Rendered frames=120; clean shutdown' in log
rows = re.findall(r'Animation-only final actor=(\d+) profile=(\S+) enabled=1 retained=1 slot=\d+ clip=(\S+) timelineMs=(\d+) sampleMs=(\d+)', log)
assert {row[1] for row in rows} == {'WanderingPriest', 'DefaultFairy'}, rows
assert all('/Idle/' in row[2] and row[3] == row[4] == '320' for row in rows), rows
report = {'status': 'packaged_companion_idle_and_persistence_verified', 'goal_complete': False,
          'executable_sha256': hashlib.sha256((package / 'dh-foundation.exe').read_bytes()).hexdigest(),
          'frames': 120, 'companions': [{'actor': row[0], 'profile': row[1], 'clip': row[2], 'post_restore_ms': int(row[3])} for row in rows],
          'limits': ['Following, companion combat, faery abilities and intro cinematic placement remain incomplete.', 'Matched-reference fidelity remains unverified.']}
args.report.parent.mkdir(parents=True, exist_ok=True)
args.report.write_text(json.dumps(report, indent=2) + '\n', encoding='utf-8')
print(json.dumps(report))
