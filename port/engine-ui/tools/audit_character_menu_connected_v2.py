"""Hash-bound expanded AS composition evidence; candidate owners are not frozen."""
from pathlib import Path
from datetime import datetime, timezone
import hashlib,json,sys
ROOT=Path(__file__).resolve().parents[3]
OUT=ROOT/'.local-inputs/character-menu-connected-v2-host'
def digest(path):
 checksum=hashlib.sha256()
 with path.open('rb') as stream:
  for chunk in iter(lambda:stream.read(1024*1024),b''):checksum.update(chunk)
 return checksum.hexdigest()
primary=[]
for stem in ('character_menu_stats_owner_v1','character_menu_actions_owner_v1','character_menu_queries_owner_v1',
             'character_menu_font_palette_v1','character_menu_inventory_order_v1',
             'character_menu_inventory_mutation_v1','character_menu_as_bridge_v1'):
 primary.extend(f'port/engine-ui/{stem}.{ext}' for ext in ('hpp','cpp'))
primary+=['port/engine-ui/character_menu_skill_authority_v1.hpp',
 'port/engine-ui/tests/character_menu_connected_v2.cpp',
 'port/engine-ui/tests/character_menu_connected_as_host_v2.hpp',
 'port/level-world/tests/character_player_skills_v3.cpp']
current={name:digest(ROOT/name) for name in primary}
if '--capture-sources' in sys.argv:
 (OUT/'compile-inputs.json').write_text(json.dumps(current,indent=2)+'\n')
 print(json.dumps({'primary_compile_input_files':len(current)}))
 sys.exit(0)
compiled=json.loads((OUT/'compile-inputs.json').read_text())
assert current==compiled,'Primary sources changed during build; repeat affected build before audit'
fixture=json.loads((OUT/'fixture-source.json').read_text())
for name,sha in fixture['generated'].items():assert digest(ROOT/name)==sha
runs={}
for mode in ('SAN','O2'):
 result=OUT/f'result-{mode}.json';data=json.loads(result.read_text())
 assert data['validation']=='PASS' and data['real_retained_AS_transport'] and data['actual_AS_inventory_lists']
 assert not data['authored_character_menu_flow']
 runs[mode]={'result':data,'result_sha256':digest(result),'executable_sha256':digest(OUT/f'menu-{mode}')}
assert runs['SAN']['result']==runs['O2']['result']
sources=[]
for stem in ('character_menu_stats_owner_v1','character_menu_actions_owner_v1','character_menu_queries_owner_v1',
             'character_menu_font_palette_v1','character_menu_inventory_order_v1',
             'character_menu_inventory_mutation_v1','character_menu_as_bridge_v1'):
 sources.extend(f'port/engine-ui/{stem}.{ext}' for ext in ('hpp','cpp'))
sources+=['port/engine-ui/character_menu_skill_authority_v1.hpp',
 'port/engine-ui/tests/character_menu_connected_v2.cpp',
 'port/engine-ui/tests/character_menu_connected_as_host_v2.hpp',
 'port/engine-ui/tools/prepare_character_menu_connected_v2.py',
 'port/engine-ui/tools/build_character_menu_connected_v2.sh',
 'port/engine-ui/tools/audit_character_menu_connected_v2.py']
snap=ROOT/'.local-inputs/character-menu-native-v1-host/snapshot'
record={'validation':'PASS','recorded_utc':datetime.now(timezone.utc).isoformat(),
 'scope':'Three-class same-player native stat/item/skill operations through actual retained GameSWF calls and inventory-list AS objects',
 'source_hashes':{name:digest(ROOT/name) for name in sources},
 'primary_compile_inputs_unchanged_through_verification':compiled,
 'fixture_derivation':fixture,'native_runs':runs,
 'linked_snapshot':{p.name:digest(p) for p in sorted(snap.glob('*.so'))},
 'zip_link_archive_sha256':digest(snap/'libdh2_zip_asset_pack_v1.a'),
 'original_SWF_inputs':{name:digest(ROOT/'port/android-native/app/src/main/assets/original-cache/data/menus'/name) for name in ('dqshared_droid.swf','dqhud_droid.swf')},
 'limits':{'whole_authored_character_menu_flow_exercised':False,'live_Android_graph_bound':False,
 'APK_or_emulator_modified':False,'menu_owners_still_mutable_candidates':True,
 'Drop_Transmute_Save_ChangeFaery_complete':False,
 'all_decoded_gameplay_asset_hashes_included':False,
 'GPU_startup_settings_localization_offline_World_and_HUD_refresh_boundaries_are_fixtures':True}}
report=ROOT/'port/engine-ui/reports/character-menu-connected-v2-host-audit.json'
report.write_text(json.dumps(record,indent=2)+'\n')
print(json.dumps({'validation':'PASS','report':str(report),'visible_character_menu':False}))
