"""Prepare isolated chest regressions from the already verified B117 fixture."""
import hashlib
import json
import argparse
from pathlib import Path

root = Path('C:/Users/adamc/Desktop/workspace/DH_sc')
parser = argparse.ArgumentParser()
parser.add_argument('--exe', type=Path, required=True)
parser.add_argument('--sha256', required=True)
parser.add_argument('--case-name', required=True)
options = parser.parse_args()
exe = options.exe.resolve(strict=True)
if hashlib.sha256(exe.read_bytes()).hexdigest().upper() != options.sha256.upper():
    raise ValueError('Frozen executable hash mismatch')
if not options.case_name or any(c not in 'abcdefghijklmnopqrstuvwxyz0123456789-' for c in options.case_name):
    raise ValueError('Case name must be a single lowercase path component')
output = root/'.local-inputs/v21.3/root'/options.case_name
output.mkdir(parents=True, exist_ok=False)
jobs = []
for kind in ('moving-open', 'idle-open'):
    case = output/kind
    case.mkdir()
    lines = (root/f'.local-inputs/b117/root/integrated-{kind}/run.args').read_text(encoding='utf-8-sig').splitlines()
    for flag, value in (
        ('--save', case/'character.save'),
        ('--game-save', case/'gameplay.save'),
        ('--capture', case/'frame.ppm'),
        ('--capture-frames', '260,303,332,400'),
    ):
        lines[lines.index(flag)+1] = str(value)
    args = case/'run.args'
    args.write_text('\n'.join(lines)+'\n', encoding='utf-8')
    jobs.append(dict(name=kind, exe=str(exe), args=['--startup-config', str(args)], cwd=str(case), log=str(case/'run.log'), timeoutSec=90))
(output/'jobs.json').write_text(json.dumps(jobs, indent=2), encoding='utf-8')
print(output/'jobs.json')
