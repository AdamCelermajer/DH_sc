"""Freeze source restoration and warning-clean V34 units, preserving V34 history."""
from pathlib import Path
import hashlib,json,re,shutil
root=Path(__file__).resolve().parents[3];dest=root/'handoff/audio-v38';dest.mkdir(parents=True,exist_ok=True)
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest()
previous=root/'handoff/audio-v34/owned/port/engine-audio/audio_mixer_v34.cpp';current=root/'port/engine-audio/audio_mixer_v34.cpp'
assert re.sub(r'\s','',previous.read_text())==re.sub(r'\s','',current.read_text()),'V34 mixer formatting-only delta required'
format_receipt=dict(validation='PASS',file='port/engine-audio/audio_mixer_v34.cpp',old_frozen_sha256=sha(previous),current_sha256=sha(current),non_whitespace_tokens_identical=True,reason='Two closing-for-brace line breaks; no warning suppression or semantic change')
(root/'port/engine-audio/reports/audio-v38-format-delta.json').write_text(json.dumps(format_receipt,indent=2)+'\n')
suffixes={'.cpp','.hpp','.h','.py','.ps1','.json','.md','.bin','.asm','.txt','.xml'}
owned=[p for p in (root/'port/engine-audio').rglob('*')if p.is_file()and p.suffix in suffixes and '__pycache__'not in p.parts]
owned +=[root/p for p in ['port/level-world/vox_audio_bridge_v34.cpp','port/level-world/vox_audio_bridge_v34.hpp','port/android-native/app/src/main/cpp/audio_output_v34.cpp','port/android-native/app/src/main/cpp/audio_output_v34.hpp','port/android-native/app/src/main/java/com/example/dh2/AudioLifecycleV34.java','port/level-world/tools/audit_audio_source_v34.py']]
owned +=[p for p in (root/'port/level-world/reference/audio-source-v34').rglob('*')if p.is_file()]
deps={root/p for p in ['port/level-world/vox_play3d_owner_v2.cpp','port/level-world/vox_music_state_owner_v1.cpp','port/engine-animation/tests/particle_cloud_models_v1_differential.py','port/engine-animation/tests/particle_factory_differential.py','port/engine-animation/tests/compiled_transforms_differential.py','port/engine-animation/tests/animation_blend_differential.py','port/engine-resources/tests/cpu.py','port/android-native/app/src/main/assets/data/sounds_pyarray.bin']}
for receipt in ['port/engine-audio/reference/source-bindings-v38/host-validation.json','port/engine-audio/reference/source-bindings-v38/native-validation.json','port/engine-audio/reference/event-families-v38/named-animation-host-receipt.json','port/engine-audio/reference/target-events-v38/host-receipt.json']:
 data=json.loads((root/receipt).read_text())
 for name,h in data.get('source_sha256',{}).items():
  p=Path(name);p=p if p.is_absolute()else root/p
  assert sha(p)==h,('Receipt source changed',str(p))
  if p.is_relative_to(root)and p.suffix in suffixes:deps.add(p)
pending=list(set(owned)|deps);visited=set()
while pending:
 p=pending.pop()
 if p in visited or not p.is_file():continue
 visited.add(p)
 if p.suffix not in ['.cpp','.hpp','.h']:continue
 for include in re.findall(r'^\s*#include\s+"([^"]+)"',p.read_text(),re.M):
  target=(p.parent/include).resolve()
  if target.is_file()and target.is_relative_to(root):deps.add(target);pending.append(target)
manifest=dict(scope='Source638 bindings, actual listener/property authorities, per-family producer corrections, exact missing-asset ledger; no root APK/runtime/audible acceptance inferred',owned=[],dependencies=[],assets=[],formatting_delta=format_receipt,original={'path':'.local-inputs/libDungeonHunter2.so','sha256':sha(root/'.local-inputs/libDungeonHunter2.so')},new_production_tus=['port/engine-audio/'+s+'.cpp'for s in ['audio_source_bindings_v38','audio_listener_rows_v38','vox_source_fields_v38','audio_target_events_v38']],new_production_headers=['port/engine-audio/'+s+'.hpp'for s in ['audio_world_producer_v38','audio_animation_swoosh_v38','audio_named_animation_sound_v38','audio_constructor_fields_v38']])
for kind,paths in [('owned',sorted(set(owned))),('dependencies',sorted(deps-set(owned)))]:
 for p in paths:
  rel=p.relative_to(root).as_posix();target=dest/('owned'if kind=='owned'else'dependency-snapshot')/rel;target.parent.mkdir(parents=True,exist_ok=True);shutil.copy2(p,target);manifest[kind].append(dict(path=rel,frozen=target.relative_to(dest).as_posix(),sha256=sha(p),bytes=p.stat().st_size))
for row in json.loads((root/'port/level-world/reference/audio-source-v34/routing-census.json').read_text())['assets']:
 p=root/'.local-inputs/audio-v34/cache'/row['name'];assert sha(p)==row['sha256'];manifest['assets'].append(dict(path=p.relative_to(root).as_posix(),uri='data/sounds/'+row['name'],sha256=row['sha256'],bytes=row['bytes']))
for receipt in ['port/engine-audio/reference/source-bindings-v38/host-validation.json','port/engine-audio/reference/source-bindings-v38/native-validation.json','port/engine-audio/reference/source-bindings-v38/proof.json','port/engine-audio/reference/authorities-v38/original-properties.json','port/engine-audio/reference/authorities-v38/strict-compile.json','port/engine-audio/reference/event-families-v38/producer-original-proof.json','port/engine-audio/reference/target-events-v38/target-events-differential.json','port/engine-audio/reference/target-events-v38/host-receipt.json','port/engine-audio/reports/audio-v34-strict-compile.json']:
 assert (root/receipt).is_file(),receipt
for header in manifest['new_production_headers']:assert (root/header).is_file(),header
(dest/'manifest.json').write_text(json.dumps(manifest,indent=2)+'\n');print(json.dumps(dict(frozen=str(dest),owned=len(manifest['owned']),dependencies=len(manifest['dependencies']),assets=len(manifest['assets']))))
