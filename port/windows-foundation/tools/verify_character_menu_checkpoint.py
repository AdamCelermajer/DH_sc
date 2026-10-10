"""Exercise packaged equipment actions and Skills selection on one saved profile."""
import argparse
import hashlib
import json
from pathlib import Path
import subprocess

parser = argparse.ArgumentParser()
parser.add_argument('--package', type=Path, required=True)
parser.add_argument('--report', type=Path, required=True)
parser.add_argument('--direct', action='store_true', help='Exercise direct-Swamp equipment initialization without a frontend profile')
args = parser.parse_args()
package = args.package.resolve()
profile = package / 'verification-character.save'
if not args.direct and not profile.is_file():
    raise RuntimeError('Run frontend verification first to create the test profile')
base = (package / ('swamp.args' if args.direct else 'startup.args')).read_text(encoding='utf-8')
if not args.direct:
    base += '\n' + '\n'.join([
        '--save', profile.name, '--menu-actions',
        'menu_MainMenu.btn_MENU_SINGLE_PLAYER|menu_StartGame.StartMenuButtons.btn_MENU_SINGLE_PLAYER']) + '\n'
base += '\n--fixed-step\n.016\n'
cases = []
for name, extra, required in (
    ('equipment-lifecycle', [
        '--equipment-page-frame', '5', '--menu-release', '6:132:135',
        '--menu-release', '7:400:49', '--menu-release', '8:400:193',
        '--menu-close-frame', '10', '--frames', '120', '--save-frame', '40',
        '--reload-frame', '80', '--load-frame', '100',
        '--game-save', 'equipment-verification.save'], [
        'Equipment state count=3 attachments=0',
        'Equipment state count=4 attachments=1',
        'Saved live checkpoint frame=40',
        'Content unloaded and reloaded at frame=80',
        'Restored live checkpoint frame=100',
        'Rendered frames=120; clean shutdown']),
    ('skills-selection', [
        '--skills-page-frame', '5', '--menu-release', '6:30:85',
        '--frames', '35'], [
        'Character menu Skills selected frame=5 via same-state source provider',
        'Rendered frames=35; clean shutdown']),
):
    case_name = ('direct-' if args.direct else '') + name
    config = case_name + '-verification.args'
    capture = case_name + '-verification.ppm'
    if args.direct and '--game-save' in extra:
        extra = list(extra)
        extra[extra.index('--game-save') + 1] = 'direct-equipment-verification.save'
    (package / config).write_text(base + '\n'.join(extra + ['--capture', capture]) + '\n', encoding='utf-8')
    log_path = package / (case_name + '-verification.log')
    with log_path.open('w', encoding='utf-8') as output:
        result = subprocess.run([str(package / 'dh-foundation.exe'), '--startup-config', config],
                                cwd=package, stdout=output, stderr=subprocess.STDOUT, timeout=90)
    assert result.returncode == 0, (name, result.returncode, str(log_path))
    log = log_path.read_text(encoding='utf-8')
    for marker in required:
        assert marker in log, (name, marker)
    assert (package / capture).is_file(), name
    cases.append({'case': case_name, 'status': 'passed', 'capture': capture})
report = {'status': 'packaged_character_menu_cases_passed', 'goal_complete': False,
          'executable_sha256': hashlib.sha256((package / 'dh-foundation.exe').read_bytes()).hexdigest(),
          'cases': cases,
          'limits': ['Skills pixels require inspection; positive skill spending is not tested.',
                     'Equipment full-page fidelity, character preview, drop and transmute remain incomplete.']}
args.report.parent.mkdir(parents=True, exist_ok=True)
args.report.write_text(json.dumps(report, indent=2) + '\n', encoding='utf-8')
print(json.dumps(report))
