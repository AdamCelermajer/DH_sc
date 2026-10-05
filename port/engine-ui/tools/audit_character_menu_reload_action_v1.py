"""Bind reload native caller + actual AS transport evidence, with fixture limits."""
from pathlib import Path
from datetime import datetime,timezone
import hashlib,json,struct
ROOT=Path(__file__).resolve().parents[3]
REF=ROOT/'port/engine-ui/reference/character-menu-flow-v1'
def digest(path):return hashlib.sha256(path.read_bytes()).hexdigest()
original=json.loads((REF/'reload-action-original-v1.json').read_text())
assert original['validation']=='PASS' and digest(REF/'reload-action-gold-v1.bin')==original['gold_sha256']
capture=json.loads((REF/'native-stack-functions.json').read_text())
assert digest(ROOT/'.local-inputs/libDungeonHunter2.so')==capture['original_sha256']
caller=next(row for row in capture['functions'] if row['address']=='0x43dbb8')
def runs(directory,binary):
 result={}
 for mode in ('SAN','O2'):
  out=ROOT/f'.local-inputs/{directory}'
  data=json.loads((out/f'result-{mode}.json').read_text());assert data['validation']=='PASS'
  result[mode]={'result':data,'result_sha256':digest(out/f'result-{mode}.json'),'executable_sha256':digest(out/f'{binary}-{mode}')}
 assert result['SAN']['result']==result['O2']['result']
 return result
native=runs('character-menu-reload-action-v1-host','reload-action')
assert native['SAN']['result']['original_complete_cases']==original['original_complete_cases']
assert native['SAN']['result']['ordered_original_boundaries']==original['ordered_boundaries']
transport=runs('character-menu-reload-as-v1-host','reload-as')
sources=[f'port/engine-ui/{stem}.{ext}' for stem in ('character_menu_reload_action_v1','character_menu_reload_v1','character_menu_as_bridge_v1') for ext in ('hpp','cpp')]
sources += ['port/engine-ui/character_menu_queries_owner_v1.hpp',
 'port/engine-ui/tests/character_menu_reload_action_v1.cpp','port/engine-ui/tests/character_menu_reload_action_original_v1.py',
 'port/engine-ui/tests/character_menu_reload_as_v1.cpp','port/engine-ui/tests/swf_movie.cpp',
 'port/engine-ui/tools/build_character_menu_reload_action_v1.sh','port/engine-ui/tools/build_character_menu_reload_as_v1.sh',
 'port/engine-ui/tools/audit_character_menu_reload_action_v1.py']
objects={}
for name in ('reload-action-arm64.o','reload-coordinator-arm64.o'):
 path=ROOT/'.local-inputs/character-menu-reload-action-v1-host'/name
 raw=path.read_bytes();assert raw[:6]==b'\x7fELF\x02\x01' and struct.unpack_from('<H',raw,18)[0]==183
 objects[name]={'sha256':digest(path),'ELF64_AArch64':True,'scope':'Compile only; not linked or run on Android'}
snap=ROOT/'.local-inputs/character-menu-native-v1-host/snapshot'
record={'validation':'PASS','recorded_utc':datetime.now(timezone.utc).isoformat(),
 'scope':'Whole NativeReloadSkills AS entry point composed with whole Character.ReloadSkills coordinator',
 'original_binary_sha256':capture['original_sha256'],'original_complete_caller':caller,
 'original':original,'native_caller':native,'actual_AS_transport':transport,'Android_compile':objects,
 'source_hashes':{name:digest(ROOT/name) for name in sources},
 'gold_sha256':digest(REF/'reload-action-gold-v1.bin'),
 'linked_private_snapshot':{p.name:digest(p) for p in sorted(snap.glob('*.so'))},
 'original_SWF_inputs':{name:digest(ROOT/'port/android-native/app/src/main/assets/original-cache/data/menus'/name) for name in ('dqshared_droid.swf','dqhud_droid.swf')},
 'limits':{'component_implementations_proven_by_this_audit':False,
 'AS_conversion_in_original_ARM_caller_test_is_a_fixture':True,
 'EABI_conversion_is_a_finite_representable_fixture_and_required_live_provider':True,
 'nonfinite_out_of_range_EABI_behavior_proven':False,
 'shared_HUD_SWFs_used_for_transport':True,'authored_character_menu_navigation_exercised':False,
 'live_Android_graph_bound':False,'APK_or_emulator_modified':False}}
report=ROOT/'port/engine-ui/reports/character-menu-reload-action-v1-host-audit.json'
report.write_text(json.dumps(record,indent=2)+'\n')
print(json.dumps({'validation':'PASS','report':str(report),'live_character_menu':False}))
