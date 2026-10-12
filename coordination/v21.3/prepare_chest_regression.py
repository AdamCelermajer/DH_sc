"""Prepare isolated chest regressions from the already verified B117 fixture."""
import hashlib
import json
from pathlib import Path

root = Path('C:/Users/adamc/Desktop/workspace/DH_sc')
exe = root/'.local-inputs/v21.3/integration/dh-foundation-integration-4b42c44b.exe'
assert hashlib.sha256(exe.read_bytes()).hexdigest().upper() == '4B42C44BB64707276153B101B2927DAA9758E5DAF2E4870BBF373E0B5D0B7AFD'
output = root/'.local-inputs/v21.3/root/chest-4b42'
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
