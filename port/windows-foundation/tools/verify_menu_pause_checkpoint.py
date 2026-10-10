"""Verify offline menu pause/resume on real packaged actors and audio output."""
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
assert (package / 'verification-character.save').is_file(), 'Run frontend verification first'
config = (package / 'enemy-combat.args').read_text(encoding='utf-8')
config += '\n' + '\n'.join([
    '--start-mode', 'menu', '--menu-assets', 'assets',
    '--save', 'verification-character.save', '--menu-actions',
    'menu_MainMenu.btn_MENU_SINGLE_PLAYER|menu_StartGame.StartMenuButtons.btn_MENU_SINGLE_PLAYER',
    '--equipment-page-frame', '5', '--menu-close-frame', '80',
    '--frames', '240', '--fixed-step', '.016',
    '--attack-start-frame', '5000', '--attack-frames', '1',
    '--game-save', 'menu-pause-verification.save',
    '--capture', 'menu-pause-verification.ppm']) + '\n'
(package / 'menu-pause-verification.args').write_text(config, encoding='utf-8')
log_path = package / 'menu-pause-verification.log'
with log_path.open('w', encoding='utf-8') as output:
    result = subprocess.run([str(package / 'dh-foundation.exe'), '--startup-config', 'menu-pause-verification.args'],
                            cwd=package, stdout=output, stderr=subprocess.STDOUT, timeout=90)
assert result.returncode == 0, (result.returncode, str(log_path))
log = log_path.read_text(encoding='utf-8')
pause = re.search(r'Character menu gameplay paused frame=5 updateSerial=(\d+)', log)
resume = re.search(r'Character menu gameplay resumed frame=80 updateSerial=(\d+)', log)
assert pause and resume and pause.group(1) == resume.group(1), 'Session advanced while menu was open'
snapshot = r'Character menu gameplay snapshot actor=(\d+) HP=([^\s]+) position=([^\s]+)(?: poseMs=([^\s]+))?'
before = re.findall(snapshot, log[pause.end():resume.start()])
after = re.findall(snapshot, log[resume.end():])
assert before and before == after, 'Actor HP/position/pose changed during offline menu'
clocks = [tuple(map(int, row)) for row in re.findall(
    r'Character menu audio clock frame=(\d+) generation=(\d+) deviceSamples=(\d+) qpcNs=(\d+)',
    log[pause.end():resume.start()])]
assert len(clocks) >= 2, 'No paired audio device publications during pause'
first, last = clocks[0], clocks[-1]
assert first[1] == last[1] and last[2] > first[2] and last[3] > first[3], 'Audio device clock stopped/reinitialized'
damage_frames = [int(frame) for frame in re.findall(r'Damage frame=(\d+)', log)]
assert damage_frames and all(frame >= 80 for frame in damage_frames), 'Combat failed to resume or advanced during pause'
assert 'Character menu gameplay pausedFrames=75 updateSerial=165' in log
assert 'Rendered frames=240; clean shutdown' in log
report = {'status': 'packaged_offline_menu_pause_and_resume_verified', 'goal_complete': False,
          'executable_sha256': hashlib.sha256((package / 'dh-foundation.exe').read_bytes()).hexdigest(),
          'frames': 240, 'paused_frames': 75, 'frozen_actor_count': len(before),
          'paused_session_serial': int(pause.group(1)), 'final_session_serial': 165,
          'audio_device_samples_during_pause': [first[2], last[2]],
          'resumed_damage_frames': damage_frames,
          'limits': ['Offline gameplay pause only; online state is not implemented.',
                     'Original voice stopping policy and full menu fidelity remain unverified.']}
args.report.parent.mkdir(parents=True, exist_ok=True)
args.report.write_text(json.dumps(report, indent=2) + '\n', encoding='utf-8')
print(json.dumps(report))
