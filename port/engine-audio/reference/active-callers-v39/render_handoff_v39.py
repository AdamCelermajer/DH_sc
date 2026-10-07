from pathlib import Path
import json,hashlib
O=Path(__file__).resolve().parent;R=O.parents[3]
j=json.loads((O/'active-callers.json').read_text())
s=['# Active audio callers V39','',f"Source snapshot: {j['snapshot_utc']}. Read-only current CMake declarations, recursive renderer .inc inclusion and actual callback registration. This is not a new compile, APK or device validation; cached compile commands are metadata only.",'',
 'Root wired named sound into the ACTIVE player SkillState value3 and NPC authored operation during this audit. This captured snapshot includes those changes. Player attack/cast and NPC development scheduler paths are separate. The audio engine and AAudio output code are compiled declarations with no instantiated gameplay mixer/bank/bridge/output runtime owner in native_app/model_renderer; MainActivity still owns FrontAudio.',
 '', '## Build and inclusion authority','',
 'App CMake declares native_app.cpp, model_renderer.cpp and audio_output_v34.cpp in dh2_native, adds/links dh2_engine_audio, and compiles VoxAudioBridgeV34 in dh2_level_world. Engine-audio CMake contains V34 audio units and V38 bindings/listener/source fields. Generic target V38 CMake declaration='+str(j['build_target_v38']['cmake'])+', active callee='+str(j['build_target_v38']['callee'])+'. The old target source also remains a library source; the active callsite evidence determines which entry is used.',
 '',f"The renderer has {len(j['include_edges'])} recursive .inc edges. These present handoffs are absent from that include graph:",'']
for row in j['inactive_handoffs']:s.append('- `'+row['file']+'`: inactive; no include/registered owner found.')
s+=['','Header inclusion or library compilation does not instantiate an owner/register callbacks. The ACTIVE loot GPU sync is visual output; Item145/drop/pickup handoffs are not installed. Canonical container owners are library sources with no active renderer interaction/audio service found.','','## Family ledger','', '| Family | Snapshot status |','| --- | --- |']
for f in j['families']:s.append('| '+f['family']+' | '+f['status']+' |')
for f in j['families']:
 s+=['','### '+f['family'],'','**Active chain:** '+' -> '.join(f['active_chain'])+'.','','**Remaining:** '+'; '.join(f['remaining'])+'.','','**Root integration:** '+f['root_integration'],'','**Source evidence:**']
 for e in f['evidence']:
  if 'line'in e:s.append(f"- [{e['path']}:{e['line']}]({(O/'source-snapshot'/e['path']).as_posix()}:{e['line']}): `{e['text']}`")
  else:s.append('- Missing anchor: `'+e.get('path','')+' / '+e.get('missing','')+'`.')
s+=['','## Common positive sound boundary','',
 'Active animation/named/combat/target transports create VoxPlay3DOwnerV2 services handling only disabled/current_level. Once the real initialized Level reaches subsequent gates, online/platform/source UID/event/bank/trace/emit/native operations remain explicit required failures. The captured current-Level reply uses renderer &level and WorldScriptContext.source_level_load_phase, initialized0 with no active assignment found. Root must bind genuine GS/current-Level/phase authority, rather than setting that field38.',
 '', 'Install one real audio runtime with genuine generated bindings/selected XML, bank/sample lifetime, source/listener fields, event timestamps, and bridge/output control thread. Connect lifecycle/focus via MainActivity/NativeBridge without competing with existing FrontAudio. AudioLifecycleV34.java and audio_output_v34.cpp availability is not application wiring. Exact authored assets/missing clips remain a separate V38 ledger; no suffix/substitute fallback is authorized by this audit.',
 '', '## Verification boundary','',f"All {len(j['source_snapshot'])} inspected files are copied and hashed under source-snapshot. Changes detected during capture: {j['changed_after_snapshot']}. Evidence links point at those immutable review snapshots because root may edit live files afterward. Only this V39 reference directory was written. V38/frozen/shared renderer/CMake were not edited; no emulator/ADB/APK was used."]
(O/'handoff.md').write_text('\n'.join(s)+'\n',encoding='utf-8')
files=[p for p in O.rglob('*')if p.is_file()and p.name!='manifest.json']
entries=[dict(path=p.relative_to(R).as_posix(),bytes=p.stat().st_size,sha256=hashlib.sha256(p.read_bytes()).hexdigest())for p in sorted(files)]
(O/'manifest.json').write_text(json.dumps(dict(version=39,scope=j['scope'],snapshot_utc=j['snapshot_utc'],owned=entries,source_hashes=j['source_snapshot']),indent=2)+'\n')
assert all(hashlib.sha256((R/e['path']).read_bytes()).hexdigest()==e['sha256']for e in entries)
print('V39 manifest:',len(entries),'owned artifacts verified')
