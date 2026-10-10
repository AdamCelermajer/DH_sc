"""Verify packaged original combat audio output across save/reload/restore."""
import argparse
import hashlib
import json
from pathlib import Path
import re
import subprocess

parser = argparse.ArgumentParser()
parser.add_argument('--package', type=Path, required=True)
parser.add_argument('--report', type=Path, required=True)
parser.add_argument('--step-sounds', action='store_true', help='Require actual authored swing entries and additional output voices')
args = parser.parse_args()
package = args.package.resolve()
receipt = json.loads((package / 'package-receipt.json').read_text(encoding='utf-8'))
for entry in receipt['files']:
    path = package / entry['path']
    assert path.stat().st_size == entry['bytes'], entry['path']
    assert hashlib.sha256(path.read_bytes()).hexdigest() == entry['sha256'], entry['path']
wav_count = sum(entry['path'].startswith('audio-assets/') and entry['path'].endswith('.wav') for entry in receipt['files'])
assert wav_count > 0, 'No original WAVs packaged'
log_path = package / 'audio-verification.log'
with log_path.open('w', encoding='utf-8') as output:
    result = subprocess.run([str(package / 'dh-foundation.exe'), '--startup-config', 'audio-verification.args'],
                            cwd=package, stdout=output, stderr=subprocess.STDOUT, timeout=60)
assert result.returncode == 0, result.returncode
log = log_path.read_text(encoding='utf-8')
assert log.count('Audio initialized listener=1') == 1, 'Audio output recreated or failed initialization'
assert 'Saved live checkpoint frame=40 HP=165.098 RNG=1234/0' in log
assert 'Content unloaded and reloaded at frame=80' in log
assert 'Restored live checkpoint frame=100 HP=165.098 RNG=1234/0' in log
assert 'Rendered frames=420; clean shutdown' in log
summary = re.search(r'Audio final dispatched=(\d+) startedVoices=(\d+) diagnostics=(\d+)', log)
assert summary, 'Missing actual output receipts'
dispatched, started, diagnostics = map(int, summary.groups())
assert dispatched >= 5 and started >= 5 and diagnostics == 0, summary.groups()
sounds = list(map(int, re.findall(r'Audio source frame=\d+ .* sound=(\d+) status=1 ', log)))
assert sounds, 'No original source sound selections'
step_sources = [dict(sequence=int(sequence), step=int(step), sound=int(sound), status=int(status), detail=detail)
                for sequence, step, sound, status, detail in re.findall(r'Audio step source .* sequence=(\d+) step=(\d+) sound=(-?\d+) status=(\d+) detail=([^\r\n]*)', log)]
step_diagnostics = [entry for entry in step_sources if entry['status'] >= 2]
if args.step_sounds:
    assert 'Audio step sound unavailable:' not in log, 'Production did not load actual gameplay animation metadata'
    assert {entry['sequence'] for entry in step_sources if entry['step'] == 1 and entry['sound'] == 289 and entry['status'] == 1} >= {470, 471, 472}, 'Missing original three-swing step entries'
    assert started >= 10, 'Expected original swoosh voices alongside the hit/hurt voices'
    assert all(entry['status'] == 4 and 'sfx_lizardman_attack_1.wav' in entry['detail'] for entry in step_diagnostics), 'Unexpected step playback diagnostic'
report = {'status': 'packaged_original_audio_and_persistence_verified', 'goal_complete': False,
          'executable_sha256': hashlib.sha256((package / 'dh-foundation.exe').read_bytes()).hexdigest(),
          'manifest_files_verified': len(receipt['files']), 'original_wav_count': wav_count,
          'frames': 420, 'source_sounds': sounds, 'dispatched': dispatched, 'started_voices': started,
          'diagnostics': diagnostics, 'output_initializations': 1,
          'limits': ['Original swing step-entry cues and music remain incomplete.',
                    'Perceptual/reference audio parity is not verified.',
                    'Existing finite voices may finish across world reload; source world-stop parity remains unverified.']}
args.report.parent.mkdir(parents=True, exist_ok=True)
if args.step_sounds:
    report['step_sources'] = step_sources
    report['step_playback_diagnostics'] = step_diagnostics
    report['limits'][0] = 'Original longsword step-entry swooshes play; complete global audio and music remain incomplete.'
    report['limits'].append('Source enemy attack sound474 references an unavailable sfx_lizardman_attack_1.wav; no substitute is played.')
args.report.write_text(json.dumps(report, indent=2) + '\n', encoding='utf-8')
print(json.dumps(report))
