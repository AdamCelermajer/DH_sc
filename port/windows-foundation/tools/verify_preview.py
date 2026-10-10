"""Run generic executable checks using an explicit content preset, emit a receipt."""
import argparse
import hashlib
import json
import subprocess
from pathlib import Path

p = argparse.ArgumentParser()
p.add_argument('--exe', type=Path, required=True)
p.add_argument('--assets', type=Path, required=True)
p.add_argument('--preset', type=Path, required=True)
p.add_argument('--output', type=Path, required=True)
a = p.parse_args()
exe, assets, output = a.exe.resolve(), a.assets.resolve(), a.output.resolve()
output.mkdir(parents=True, exist_ok=True)
config = json.loads(a.preset.read_text(encoding='utf-8-sig'))
receipt = {'executable': str(exe), 'sha256': hashlib.sha256(exe.read_bytes()).hexdigest(), 'checks': []}
def run(name, args):
    result = subprocess.run([str(exe), '--assets', str(assets), *args], cwd=output,
                            capture_output=True, text=True, timeout=60)
    (output / (name + '.log')).write_text(result.stdout + result.stderr, encoding='utf-8')
    receipt['checks'].append({'name': name, 'exit_code': result.returncode,
                              'log': str(output / (name + '.log'))})
    if result.returncode:
        raise RuntimeError(name + ': ' + result.stdout + result.stderr)
    return result.stdout
try:
    save = output / 'restart.save'
    run('save_write', ['--save', str(save), '--character-name', 'PersistenceProbe', '--save-now', '--probe'])
    text = run('save_restart', ['--save', str(save), '--probe'])
    if 'Character name=PersistenceProbe ' not in text:
        raise RuntimeError('Separate process did not restore character name')
    world = config.get('world_args', [])
    character = config.get('character_args', [])
    integrated = config.get('integrated_args', [])
    for name, args in [('world', world), ('character', character)]:
        if args:
            run(name, [*args, '--frames', '20', '--fixed-step', '.05', '--capture', str(output / (name + '.ppm'))])
    if integrated:
        text = run('integrated_reload', [*integrated, '--frames', '30', '--fixed-step', '.05',
                                        '--reload-frame', '15', '--capture', str(output / 'integrated.ppm')])
        if 'Content unloaded and reloaded at frame=15' not in text:
            raise RuntimeError('Integrated scene/character reload was not reached')
    if world:
        for name, frames in [('timeline_before_cut', '49'), ('timeline_after_cut', '51')]:
            run(name, [*world, '--timeline', '--fixed-step', '.1', '--frames', frames,
                       '--capture', str(output / (name + '.ppm'))])
        first = (output / 'timeline_before_cut.ppm').read_bytes()
        second = (output / 'timeline_after_cut.ppm').read_bytes()
        if first == second:
            raise RuntimeError('Timeline captures do not differ across cut')
        receipt['checks'].append({'name': 'timeline_images_differ_across_cut', 'pass': True})
    receipt['status'] = 'pass'
finally:
    (output / 'verification.json').write_text(json.dumps(receipt, indent=2) + '\n', encoding='utf-8')
print(json.dumps(receipt))
