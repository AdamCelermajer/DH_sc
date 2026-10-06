"""Freeze this chat's owned audio sources and evidence; no build integration."""
from pathlib import Path
import hashlib,json,re,shutil
root=Path(__file__).resolve().parents[3];dest=root/'handoff/audio-v34';dest.mkdir(parents=True,exist_ok=True)
owned=[p for p in (root/'port/engine-audio').rglob('*')if p.is_file()and '__pycache__'not in p.parts]
owned+=[root/p for p in ['port/level-world/vox_audio_bridge_v34.cpp','port/level-world/vox_audio_bridge_v34.hpp','port/android-native/app/src/main/cpp/audio_output_v34.cpp','port/android-native/app/src/main/cpp/audio_output_v34.hpp','port/android-native/app/src/main/java/com/example/dh2/AudioLifecycleV34.java','port/level-world/tools/audit_audio_source_v34.py']]
owned +=[p for p in (root/'port/level-world/reference/audio-source-v34').rglob('*')if p.is_file()]
deps={root/p for p in ['port/level-world/vox_play3d_owner_v2.cpp','port/engine-animation/tests/particle_cloud_models_v1_differential.py','port/engine-animation/tests/particle_factory_differential.py','port/engine-animation/tests/compiled_transforms_differential.py','port/engine-animation/tests/animation_blend_differential.py','port/engine-resources/tests/cpu.py']}
pending=list(set(owned)|deps);visited=set()
while pending:
 p=pending.pop()
 if p in visited or not p.is_file():continue
 visited.add(p)
 if p.suffix not in ['.cpp','.hpp','.h']:continue
 for include in re.findall(r'^\s*#include\s+"([^"]+)"',p.read_text(),re.M):
  target=(p.parent/include).resolve()
  if target.is_file()and target.is_relative_to(root):deps.add(target);pending.append(target)
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest()
production=['port/engine-audio/'+s+'.cpp'for s in ['audio_sample_v34','audio_catalog_v34','audio_bank_v34','audio_spatial_v34','audio_native_envelope_v34','audio_mixer_v34']]+['port/level-world/vox_audio_bridge_v34.cpp','port/android-native/app/src/main/cpp/audio_output_v34.cpp']
manifest=dict(scope='Independent source/CPU/AAudio implementation; no shared build integration or device/audible acceptance; explicit source binding and rapid-native-reversal boundaries',production_tus=production,owned=[],dependencies=[],assets=[],original={'path':'.local-inputs/libDungeonHunter2.so','sha256':sha(root/'.local-inputs/libDungeonHunter2.so')})
for kind,paths in [('owned',sorted(set(owned))),('dependencies',sorted(deps-set(owned)))]:
 for p in paths:
  rel=p.relative_to(root).as_posix();target=dest/('owned'if kind=='owned'else'dependency-snapshot')/rel;target.parent.mkdir(parents=True,exist_ok=True);shutil.copy2(p,target);manifest[kind].append(dict(path=rel,frozen=target.relative_to(dest).as_posix(),sha256=sha(p)))
census=json.loads((root/'port/level-world/reference/audio-source-v34/routing-census.json').read_text())
for row in census['assets']:
 p=root/'.local-inputs/audio-v34/cache'/row['name'];assert sha(p)==row['sha256'];manifest['assets'].append(dict(path=p.relative_to(root).as_posix(),uri='data/sounds/'+row['name'],bytes=row['bytes'],sha256=row['sha256']))
manifest['validation']={name:json.loads((root/'port/engine-audio/reports'/name).read_text())for name in ['audio-v34-host.json','audio-v34-original-proof.json','audio-v34-strict-compile.json']}
(dest/'manifest.json').write_text(json.dumps(manifest,indent=2)+'\n');print(json.dumps(dict(frozen=str(dest),owned=len(manifest['owned']),dependencies=len(manifest['dependencies']),assets=len(manifest['assets']))))
