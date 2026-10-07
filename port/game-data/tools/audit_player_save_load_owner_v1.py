"""Record whole Save.Load coordinator evidence without claiming file IO."""
from pathlib import Path
from datetime import datetime, timezone
import hashlib, json, struct

ROOT = Path(__file__).resolve().parents[3]
REF = ROOT / 'port/game-data/reference/player-save-load-v1'
OUT = ROOT / '.local-inputs/player-save-load-owner-v1-host'

def digest(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()

original = json.loads((REF / 'load-original-v1.json').read_text())
assert original['validation'] == 'PASS'
assert digest(REF / 'load-gold-v1.bin') == original['gold_sha256']
capture = json.loads((ROOT / 'port/engine-ui/reference/character-menu-flow-v1/native-stack-functions.json').read_text())
assert digest(ROOT / '.local-inputs/libDungeonHunter2.so') == capture['original_sha256']
functions = [row for row in capture['functions'] if row['address'] in ('0x465430', '0x464f4c', '0x468574')]
assert len(functions) == 3
runs = {}
for mode in ('SAN', 'O2'):
    result = json.loads((OUT / f'result-{mode}.json').read_text())
    assert result['validation'] == 'PASS'
    assert result['original_complete_cases'] == original['original_complete_cases']
    assert result['ordered_boundaries'] == original['ordered_boundaries']
    assert result['required_failure_prefixes'] == original['ordered_boundaries']
    runs[mode] = {'result': result,
                  'result_sha256': digest(OUT / f'result-{mode}.json'),
                  'executable_sha256': digest(OUT / f'load-{mode}')}
assert runs['SAN']['result'] == runs['O2']['result']
obj = OUT / 'load-owner-arm64.o'
raw = obj.read_bytes()
assert raw[:6] == b'\x7fELF\x02\x01' and struct.unpack_from('<H', raw, 18)[0] == 183
sources = [
    'port/game-data/player_save_load_owner_v1.hpp',
    'port/game-data/player_save_load_owner_v1.cpp',
    'port/game-data/player_savegame_v1.hpp',
    'port/game-data/tests/player_save_load_owner_v1.cpp',
    'port/game-data/tests/player_save_load_owner_original_v1.py',
    'port/game-data/tools/build_player_save_load_owner_v1.sh',
    'port/game-data/tools/audit_player_save_load_owner_v1.py',
]
report = {
    'validation': 'PASS', 'recorded_utc': datetime.now(timezone.utc).isoformat(),
    'scope': 'Whole SG_Load -> _Load -> _LoadVolatileQuestsLog coordinator with the same retained Save and sole owned profile field',
    'original_binary_sha256': capture['original_sha256'],
    'original_complete_functions': functions, 'original': original, 'native': runs,
    'gold_sha256': digest(REF / 'load-gold-v1.bin'),
    'original_section_callbacks': json.loads((REF / 'section-callbacks-v1.json').read_text()),
    'source_hashes': {name: digest(ROOT / name) for name in sources},
    'Android_compile': {'sha256': digest(obj), 'ELF64_AArch64': True,
                        'scope': 'Compile only; not linked or run on Android'},
    'limits': {
        'filename_profile_file_class_and_section_readers_are_fixtures': True,
        'initializers_global_game_and_online_quest_producers_are_fixtures': True,
        'owned_null_profile_startup_mask8_and_mask32_need_no_provider': True,
        'slot_minus1_alone_does_not_imply_null_profile': True,
        'synchronous_profile_mutation_differential_coverage': False,
        'full_save_file_serialization_proven': False,
        'live_Android_graph_bound': False, 'APK_or_emulator_modified': False,
    },
}
path = ROOT / 'port/game-data/reports/player-save-load-owner-v1-host-audit.json'
path.write_text(json.dumps(report, indent=2) + '\n')
print(json.dumps({'validation': 'PASS', 'report': str(path),
                  'original_complete_cases': original['original_complete_cases'],
                  'ordered_boundaries': original['ordered_boundaries'],
                  'live_Android': False}))
