"""Verify authored template population, persistence and shared moth combat."""
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
receipt = json.loads((package / 'package-receipt.json').read_text(encoding='utf-8'))
for entry in receipt['files']:
    asset = package / entry['path']
    assert asset.stat().st_size == entry['bytes'], entry['path']
    assert hashlib.sha256(asset.read_bytes()).hexdigest() == entry['sha256'], entry['path']

cases = []
for name, frames in [('population', 120), ('populated-combat', 240)]:
    log_path = package / (name + '-verification.log')
    with log_path.open('w', encoding='utf-8') as output:
        result = subprocess.run([str(package / 'dh-foundation.exe'), '--startup-config',
                                 name + '-verification.args'], cwd=package,
                                stdout=output, stderr=subprocess.STDOUT, timeout=60)
    assert result.returncode == 0, (name, result.returncode)
    log = log_path.read_text(encoding='utf-8')
    assert f'Rendered frames={frames}; clean shutdown' in log, name
    assert 'Population visuals=33 declarations=61 skipped=28' in log, name
    assert 'Population source template RNG seed=8504954 calls=20' in log, name
    actors = re.findall(r'^Actor final id=(\d+) definition=(.+?) HP=([^ ]+)', log, re.M)
    assert len(actors) == 23, (name, 'shared combat population', len(actors))
    assert len({actor[0] for actor in actors}) == len(actors), 'duplicate stable actor ID'
    assert sum('_prim_LizTemplate_' in actor[1] or '_prim_MothTemplate_' in actor[1] for actor in actors) == 20
    assert not any('_prim_MothTemplate_13' in actor[1] for actor in actors), 'unknown condition enabled'
    if name == 'population':
        assert log.count('Population visuals=33 declarations=61 skipped=28') == 2, 'reload population changed'
        assert 'Saved live checkpoint frame=40 HP=165.098 RNG=8504954/20' in log
        assert 'Content unloaded and reloaded at frame=80' in log
        assert 'Restored live checkpoint frame=100 HP=165.098 RNG=8504954/20' in log
    else:
        moth_ids = {'13982471039583375744', '17937632967283937838'}
        damaging = re.findall(r'Damage frame=(\d+) attacker=(\d+) target=18446744073709551615 marker=attack_mainhand removed=([\d.]+)', log)
        assert {hit[1] for hit in damaging if float(hit[2]) > 0} == moth_ids, 'both original moths must damage player'
        for actor in moth_ids:
            assert re.search(r'Actor final id=' + actor + r' .* target=18446744073709551615 ', log), 'moth target lost'
    cases.append({'case': name, 'frames': frames, 'population_visuals': 33, 'combat_actors': len(actors)})

report = {'status': 'packaged_population_persistence_and_moth_combat_verified', 'goal_complete': False,
          'executable_sha256': hashlib.sha256((package / 'dh-foundation.exe').read_bytes()).hexdigest(),
          'manifest_files_verified': len(receipt['files']), 'cases': cases,
          'limits': ['28 authored declarations still gated or unsupported.',
                    'Explicit first authored NPC attack groups; complete randomized attack choice/cadence remains incomplete.',
                    'Moth test uses diagnostic placement; original campaign activation and matched-reference fidelity are not established.']}
args.report.parent.mkdir(parents=True, exist_ok=True)
args.report.write_text(json.dumps(report, indent=2) + '\n', encoding='utf-8')
print(json.dumps(report))
