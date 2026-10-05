"""Actual AS string transport evidence; does not claim native-menu parity."""
from pathlib import Path
from datetime import datetime, timezone
import hashlib, json, struct

ROOT = Path(__file__).resolve().parents[3]
OUT = ROOT / '.local-inputs/character-menu-as-names-v2-host'
def digest(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()
runs = {}
for mode in ('SAN', 'O2'):
    data = json.loads((OUT / f'result-{mode}.json').read_text())
    assert data['validation'] == 'PASS' and data['actual_AS_text_conversions'] == 9
    assert data['actual_AS_debug_text_conversions'] == 3
    runs[mode] = {'result': data, 'result_sha256': digest(OUT / f'result-{mode}.json'),
                  'executable_sha256': digest(OUT / f'as-names-{mode}')}
assert runs['SAN']['result'] == runs['O2']['result']
obj = OUT / 'as-bridge-arm64.o'
raw = obj.read_bytes()
assert raw[:6] == b'\x7fELF\x02\x01' and struct.unpack_from('<H', raw, 18)[0] == 183
sources = ['port/engine-ui/character_menu_as_bridge_v1.hpp',
           'port/engine-ui/character_menu_as_bridge_v1.cpp',
           'port/engine-ui/character_menu_queries_owner_v1.hpp',
           'port/engine-ui/tests/character_menu_as_names_v2.cpp',
           'port/engine-ui/tools/build_character_menu_as_names_v2.sh',
           'port/engine-ui/tools/audit_character_menu_as_names_v2.py']
snap = ROOT / '.local-inputs/character-menu-native-v1-host/snapshot'
report = {
    'validation': 'PASS', 'recorded_utc': datetime.now(timezone.utc).isoformat(),
    'scope': 'Distinct genuine upstream AS to_string and to_xstring on original argument slots, including deferred properties and unchanged results',
    'native': runs, 'source_hashes': {name: digest(ROOT / name) for name in sources},
    'linked_private_snapshot': {p.name: digest(p) for p in sorted(snap.glob('*.so'))},
    'original_SWF_inputs': {name: digest(ROOT / 'port/android-native/app/src/main/assets/original-cache/data/menus' / name)
                            for name in ('dqshared_droid.swf', 'dqhud_droid.swf')},
    'Android_compile': {'sha256': digest(obj), 'ELF64_AArch64': True,
                        'scope': 'Compile only; not linked or run on Android'},
    'limits': {'source_menu_to_xstring_equivalence_proven': False,
               'original_32bit_pointer_debug_text_not_claimed_for_native_64bit_addresses': True,
               'original_raw_string_tag3_represented_by_upstream_bridge': False,
               'authored_character_menu_navigation_exercised': False,
               'startup_localization_GPU_are_fixtures': True,
               'APK_or_emulator_modified': False},
}
path = ROOT / 'port/engine-ui/reports/character-menu-as-names-v2-host-audit.json'
path.write_text(json.dumps(report, indent=2) + '\n')
print(json.dumps({'validation': 'PASS', 'report': str(path), 'actual_AS_text_conversions': 9}))
