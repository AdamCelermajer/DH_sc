"""Bind the typed menu boundary's host evidence; never imply an APK milestone."""
import hashlib, json
from pathlib import Path
from datetime import datetime, timezone

ROOT=Path(__file__).resolve().parents[3]
def digest(path):
    checksum=hashlib.sha256()
    with path.open('rb') as stream:
        for block in iter(lambda:stream.read(1024*1024),b''): checksum.update(block)
    return checksum.hexdigest()

def runs(directory, executable):
    results={}
    for mode in ('SAN','O2'):
        output=ROOT/f'.local-inputs/{directory}/result-{mode}.json'
        result=json.loads(output.read_text())
        assert result['validation']=='PASS', (mode,result)
        binary=ROOT/f'.local-inputs/{directory}/{executable}-{mode}'
        results[mode]={'result':result,'result_sha256':digest(output),'executable_sha256':digest(binary)}
    assert results['SAN']['result']==results['O2']['result']
    return results

bridge=runs('character-menu-as-bridge-v1-host','bridge')
connected=runs('character-menu-connected-v1-host','menu')
sources=[
 'port/engine-ui/character_menu_as_bridge_v1.hpp',
 'port/engine-ui/character_menu_as_bridge_v1.cpp',
 'port/engine-ui/tests/character_menu_as_bridge_v1.cpp',
 'port/engine-ui/tests/character_menu_connected_as_host_v1.hpp',
 'port/engine-ui/tests/character_menu_connected_v1.cpp',
 'port/engine-ui/tools/build_character_menu_as_bridge_v1.sh',
 'port/engine-ui/tools/build_character_menu_connected_v1.sh',
 'port/engine-ui/tools/prepare_character_menu_connected_v1.py',
 'port/engine-ui/tools/audit_character_menu_as_bridge_v1.py',
]
libraries=ROOT/'.local-inputs/character-menu-native-v1-host/snapshot'
record={
 'validation':'PASS', 'recorded_utc':datetime.now(timezone.utc).isoformat(),
 'scope':'Typed original ActionScript callback transport and three-class native menu-operation composition',
 'source_hashes':{name:digest(ROOT/name) for name in sources},
 'linked_private_snapshot':{path.name:digest(path) for path in sorted(libraries.glob('*.so'))},
 'linked_zip_archive_sha256':digest(libraries/'libdh2_zip_asset_pack_v1.a'),
 'original_SWF_inputs':{name:digest(ROOT/'port/android-native/app/src/main/assets/original-cache/data/menus'/name) for name in ('dqshared_droid.swf','dqhud_droid.swf')},
 'transport':bridge, 'connected_operations':connected,
 'fixture_derivation':json.loads((ROOT/'.local-inputs/character-menu-connected-v1-host/fixture-source.json').read_text()),
 'limits':{
  'source_gameplay_menu_owners_still_under_development':True,
  'whole_authored_character_menu_flow_exercised':False,
  'main_menu_or_character_selection_in_scope':False,
  'current_APK_modified':False,
  'live_Android_character_menu_exercised':False,
  'GPU_texture_settings_startup_localization_and_offline_World_boundaries_are_fixtures':True,
  'all_decoded_fixture_asset_hashes_included':False,
 },
}
report=ROOT/'port/engine-ui/reports/character-menu-as-bridge-v1-host-audit.json'
report.write_text(json.dumps(record,indent=2)+'\n')
print(json.dumps({'validation':'PASS','report':str(report),'whole_character_menu_live':False}))
