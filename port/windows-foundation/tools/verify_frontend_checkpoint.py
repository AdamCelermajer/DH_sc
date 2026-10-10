"""Exercise the packaged frontend through real Win32 input and profile reload."""
import argparse
import hashlib
import json
from pathlib import Path
import subprocess

parser = argparse.ArgumentParser()
parser.add_argument('--package', type=Path, required=True)
parser.add_argument('--report', type=Path, required=True)
args = parser.parse_args()
package = args.package.resolve()
profile = package / 'verification-character.save'
if profile.exists():
    raise RuntimeError('Use a fresh verification profile; never overwrite an existing save')
base = (package / 'startup.args').read_text(encoding='utf-8')
cases = []
for name, menu in (
    ('frontend-create', ['--verify-frontend-create', '--menu-frames', '3000']),
    ('frontend-load', ['--menu-actions', 'menu_MainMenu.btn_MENU_SINGLE_PLAYER|menu_StartGame.StartMenuButtons.btn_MENU_SINGLE_PLAYER']),
):
    configuration = base + '\n' + '\n'.join(menu + ['--save', profile.name, '--frames', '120', '--fixed-step', '.016', '--capture', name + '.ppm']) + '\n'
    (package / (name + '.args')).write_text(configuration, encoding='utf-8')
    log_path = package / (name + '.log')
    with log_path.open('w', encoding='utf-8') as output:
        result = subprocess.run([str(package / 'dh-foundation.exe'), '--startup-config', name + '.args'], cwd=package, stdout=output, stderr=subprocess.STDOUT, timeout=90)
    assert result.returncode == 0, (name, result.returncode, str(log_path))
    log = log_path.read_text(encoding='utf-8')
    assert 'generic_start_delivered":true' in log, name
    assert 'Frontend launched same CharacterState slot=0 class=KnightPlayerBase' in log, name
    assert 'Population visuals=33 declarations=61 skipped=28' in log, name
    assert 'Rendered frames=120; clean shutdown' in log, name
    assert profile.is_file(), name
    if name == 'frontend-create':
        assert '"generic_native_input":"PASS"' in log
        before = hashlib.sha256(profile.read_bytes()).hexdigest()
    else:
        assert hashlib.sha256(profile.read_bytes()).hexdigest() == before, 'Loading rewrote character profile'
    cases.append({'case': name, 'gameplay_frames': 120, 'shared_state_handoff': True})
report = {'status': 'packaged_frontend_input_and_profile_handoff_verified', 'goal_complete': False,
          'executable_sha256': hashlib.sha256((package / 'dh-foundation.exe').read_bytes()).hexdigest(),
          'cases': cases, 'limits': ['Mage/rogue combat and complete opening cinematic remain incomplete.']}
args.report.parent.mkdir(parents=True, exist_ok=True)
args.report.write_text(json.dumps(report, indent=2) + '\n', encoding='utf-8')
print(json.dumps(report))
