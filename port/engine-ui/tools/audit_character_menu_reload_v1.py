"""Bind complete ReloadSkills caller evidence without claiming live providers."""
import hashlib
import json
from datetime import datetime, timezone
from pathlib import Path

ROOT = Path(__file__).resolve().parents[3]
REF = ROOT / 'port/engine-ui/reference/character-menu-flow-v1'
OUT = ROOT / '.local-inputs/character-menu-reload-v1-host'

def digest(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()

original = json.loads((REF / 'reload-original-v1.json').read_text())
assert original['validation'] == 'PASS'
assert digest(REF / 'reload-gold-v1.bin') == original['gold_sha256']
capture = json.loads((REF / 'native-stack-functions.json').read_text())
binary = ROOT / '.local-inputs/libDungeonHunter2.so'
assert digest(binary) == capture['original_sha256']
caller = next(row for row in capture['functions'] if row['address'] == '0x3a9db4')
assert 'ReloadSkills' in caller['symbol']
runs = {}
for mode in ('SAN', 'O2'):
    result_path = OUT / f'result-{mode}.json'
    result = json.loads(result_path.read_text())
    assert result['validation'] == 'PASS'
    for key in ('original_complete_cases', 'ordered_component_boundaries'):
        assert result[key] == original[key]
    runs[mode] = {
        'result': result,
        'result_sha256': digest(result_path),
        'executable_sha256': digest(OUT / f'reload-{mode}'),
    }
assert runs['SAN']['result'] == runs['O2']['result']
sources = [
    'port/engine-ui/character_menu_reload_v1.hpp',
    'port/engine-ui/character_menu_reload_v1.cpp',
    'port/engine-ui/tests/character_menu_reload_v1.cpp',
    'port/engine-ui/tests/character_menu_reload_original_v1.py',
    'port/engine-ui/tests/character_menu_native_v1_original.py',
    'port/engine-ui/tools/build_character_menu_reload_v1.sh',
    'port/engine-ui/tools/capture_character_menu_stack_v1.py',
    'port/engine-ui/tools/audit_character_menu_reload_v1.py',
]
record = {
    'validation': 'PASS',
    'recorded_utc': datetime.now(timezone.utc).isoformat(),
    'scope': 'Complete Character::ReloadSkills caller, required component delivery and fresh saved-class branches',
    'original_binary_sha256': capture['original_sha256'],
    'original_complete_caller': caller,
    'source_hashes': {name: digest(ROOT / name) for name in sources},
    'reference_hashes': {name: digest(REF / name) for name in (
        'reload-gold-v1.bin', 'reload-original-v1.json',
        'native-stack-functions.json', 'native-stack-functions.asm')},
    'original': original,
    'native_runs': runs,
    'limits': {
        'component_implementations_proven_by_this_audit': False,
        'component_services_are_explicit_fixtures': True,
        'actual_authored_character_menu_navigation_exercised': False,
        'native_AS_player_selection_included': False,
        'live_Android_graph_bound': False,
        'APK_or_emulator_modified': False,
    },
}
report = ROOT / 'port/engine-ui/reports/character-menu-reload-v1-host-audit.json'
report.write_text(json.dumps(record, indent=2) + '\n')
print(json.dumps({'validation': 'PASS', 'report': str(report), 'live_character_menu': False}))
