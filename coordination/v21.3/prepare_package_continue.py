"""Run the packaged menu configuration with isolated copies of user progress."""
import hashlib
import argparse
import json
from pathlib import Path
import shutil

root = Path('C:/Users/adamc/Desktop/workspace/DH_sc')
package = root/'.local-inputs/windows-source-clock-v21.3'
seed = root/'.local-inputs/windows-source-clock-v21.2'
parser = argparse.ArgumentParser()
parser.add_argument('--case-name', required=True)
options = parser.parse_args()
if not options.case_name or any(c not in 'abcdefghijklmnopqrstuvwxyz0123456789-' for c in options.case_name):
    raise ValueError('Case name must be a single lowercase path component')
case = root/'.local-inputs/v21.3/root'/options.case_name
exe = package/'dh-foundation.exe'
assert hashlib.sha256(exe.read_bytes()).hexdigest().upper() == '276C83146351F41E879F296143F8F59F74A0D3552F224D9015AEF87C3037779A'
case.mkdir(parents=True, exist_ok=False)
copied = []
for source in seed.iterdir():
    if source.is_file() and (source.name.endswith('.save') or '.gsave' in source.name or source.name == 'dh2_settings.tutorials'):
        digest = hashlib.sha256(source.read_bytes()).hexdigest()
        shutil.copy2(source, case/source.name)
        assert hashlib.sha256(source.read_bytes()).hexdigest() == digest
        assert hashlib.sha256((case/source.name).read_bytes()).hexdigest() == digest
        copied.append(dict(name=source.name, sha256=digest))
lines = (package/'startup.args').read_text(encoding='utf-8-sig').splitlines()
for flag, value in (
    ('--assets', package/'assets'), ('--audio-assets', package/'audio-assets'),
    ('--audio-table', package/'audio-assets/sounds_pyarray.bin'), ('--menu-assets', package/'assets'),
    ('--save', case/'character.save'), ('--game-save', case/'gameplay.save')
):
    indices = [i for i, line in enumerate(lines) if line == flag]
    assert len(indices) == 1, (flag, indices)
    lines[indices[0] + 1] = str(value)
lines += ['--frames', '500', '--fixed-step', '0.0166667', '--skip-boot', '--menu-frames', '10', '--save-slot', '0',
          '--capture', str(case/'frame.ppm'), '--capture-frames', '1,250,450',
          '--menu-actions', 'menu_MainMenu.btn_MENU_SINGLE_PLAYER|menu_StartGame.StartMenuButtons.btn_MENU_SINGLE_PLAYER']
(case/'run.args').write_text('\n'.join(lines)+'\n', encoding='utf-8')
(case/'seed-hashes.json').write_text(json.dumps(copied, indent=2)+'\n')
jobs = [dict(name='packaged-Continue', exe=str(exe), args=['--startup-config', str(case/'run.args')],
             cwd=str(case), log=str(case/'run.log'), timeoutSec=90)]
(case/'jobs.json').write_text(json.dumps(jobs, indent=2)+'\n')
print(case/'jobs.json')
