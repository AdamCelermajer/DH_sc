"""Bind frontend creation metadata to local IDA exports and original cache captures."""
import hashlib
import json
import struct
from pathlib import Path

HERE = Path(__file__).resolve().parent
ROOT = HERE.parents[4]
IDA = ROOT / '.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so'
paths = [
    IDA / 'pseudocode/0042/00428cc0.c',
    IDA / 'pseudocode/0042/00428498.c',
    IDA / 'pseudocode/0042/004220a0.c',
    ROOT / 'port/game-data/reference/player-creation-v2/original-class-producers.json',
    ROOT / 'port/game-data/reference/fresh-player-profile-v1/original-creation.json',
    ROOT / '.local-inputs/actors/character_properties_pyarraynames.bin',
]
TEXT = ROOT / 'port/android-native/app/src/main/assets/original-cache/data/text'
def text_table(path):
    data = path.read_bytes()
    offset = 2
    strings = []
    for _ in range(struct.unpack_from('<H', data)[0]):
        count = struct.unpack_from('<H', data, offset)[0]
        offset += 2
        strings.append(data[offset:offset+count].rstrip(b'\0').decode('utf8'))
        offset += count
    assert offset == len(data)
    return strings
labels = {}
for pack in ('menu', 'gameplaymenus', 'global'):
    symbols, english = TEXT / (pack+'.symbols'), TEXT / (pack+'.english')
    labels.update(zip(text_table(symbols), text_table(english)))
    paths += [symbols, english]
inventory = ROOT / 'port/script-runtime/reference/design-bindings/cache-constant-inventory.json'
constants = json.loads(inventory.read_text())
# Inventory retains cache integer constants. Validate the exact ^2 source color.
def find_color(node):
    if isinstance(node, dict):
        if isinstance(node.get('FontTextColors'), list):
            return node['FontTextColors']
        for value in node.values():
            result = find_color(value)
            if result is not None: return result
    elif isinstance(node, list):
        for value in node:
            result = find_color(value)
            if result is not None: return result
color_group = find_color(constants)
assert color_group[0]['values']['two'] == 0x9ADEFF
color_keys = ['zero', 'one', 'two', 'three', 'four', 'five', 'six', 'seven', 'eight', 'nine']
color_values = [color_group[0]['values'][key] for key in color_keys]
assert color_values == [16777215, 10289050, 10149631, 13933311, 16771226, 65535, 16711680, 6710886, 3355443, 11184810]
fonts = ROOT / 'port/android-native/app/src/main/assets/data/fonts_pycst.bin'
def decode_constants(raw):
    offset = 0
    def word():
        nonlocal offset
        value = struct.unpack_from('<I', raw, offset)[0]
        offset += 4
        return value
    def string():
        nonlocal offset
        count = word()
        value = raw[offset:offset+count]
        offset += count
        return value
    result = {}
    for _ in range(word()):
        group = string()
        values = {}
        for _ in range(word()):
            key = string()
            values[key] = word()
        result[group] = values
    assert offset == len(raw)
    return result
decoded_colors = decode_constants(fonts.read_bytes())[b'FontTextColors']
assert [decoded_colors[key.encode()] for key in color_keys] == color_values
paths.append(fonts)
schema_path = ROOT / '.local-inputs/actors/character_properties_pystructnames.bin'
schema_bytes = schema_path.read_bytes()
schema_offset = 4
schema_fields = []
for _ in range(struct.unpack_from('<I', schema_bytes)[0]):
    count = struct.unpack_from('<I', schema_bytes, schema_offset)[0]
    schema_offset += 4
    schema_fields.append(schema_bytes[schema_offset:schema_offset+count].decode())
    schema_offset += count
assert schema_offset == len(schema_bytes) and len(schema_fields) == 224
assert schema_fields[149:153] == ['Stat_Strength', 'Stat_Dexterity', 'Stat_Endurance', 'Stat_Energy']
assert not any('Intelligence' in field for field in schema_fields)
paths.append(schema_path)
paths.append(inventory)
cpp = (HERE / 'dynamic_text_bindings.cpp').read_text()
for symbol in ('MENU_CLASS_00', 'MENU_CLASS_01', 'MENU_CLASS_02', 'MENU_KNIGHT_DESC',
               'MENU_ROGUE_DESC', 'MENU_MAGE_DESC', 'MENU_CONFIRM', 'MENU_MENUTITLE_CHOOSE_CLASS'):
    assert labels[symbol].replace('\n', '\\n') in cpp, symbol
assert labels['GLOBAL_LEVEL'] == 'LEVEL'
producers = json.loads(paths[3].read_text())
rows = {r['character']: r for r in producers['rows']}
expected = [('KnightPlayerBase', 263, 165, 14, 7, 1),
            ('RoguePlayerBase', 325, 213, 27, 56, 3),
            ('MagePlayerBase', 290, 174, 21, 14, 2)]
for name, row, loot, skills, first, faery in expected:
    r = rows[name]
    assert r['row'] == row
    assert [r['resolved_raw'][str(k)] for k in (9, 28, 29)] == [loot, skills, faery]
    assert r['original_selected_skills'][0]['dictionary_id'] == first
    assert r['original_cached_GetInt']['19'] == 1
update = paths[1].read_text()
for case, name in enumerate(['KnightPlayerBase', 'RoguePlayerBase', 'MagePlayerBase']):
    # Read only the class-changing switch, after the idle-animation switch.
    branch = update.split('else\n  {\n    switch ( v1 )')[1].split('case ' + str(case) + ':')[1]
    assert name in branch.split('break;')[0]
profile = json.loads(paths[4].read_text())
assert profile['status'] == 'PASS'
assert producers['validation'] == 'PASS'
assert producers['original_sha256'] == profile['original_sha256']
evidence = {
    'status': 'PASS', 'original_sha256': profile['original_sha256'],
    'menu_order': [x[0] for x in expected],
    'initial_class_token': 'KnightPlayerBase', 'initial_player_level': 1,
    'initial_campaign_level_row': 41,
    'profile_section_tags': sorted({s['tag'] for c in profile['cases'] for s in c['sections']}),
    'dynamic_class_text': {k: labels[k] for k in ('MENU_CLASS_00', 'MENU_CLASS_01', 'MENU_CLASS_02', 'MENU_KNIGHT_DESC', 'MENU_ROGUE_DESC', 'MENU_MAGE_DESC', 'MENU_CONFIRM', 'MENU_MENUTITLE_CHOOSE_CLASS')},
    'specialization_color_rgb': '#9ADEFF',
    'source_font_colors': {key: '#%06X' % value for key, value in zip(color_keys, color_values)},
    'source_color_reset': '^r emits </font>, restoring enclosing authored HTML/TextField style',
    'font_color_decoder': 'Direct little-endian group/key/value decode of actual fonts_pycst.bin, cross-checked with frozen cache inventory',
    'source_stat_fields': {str(i): schema_fields[i] for i in range(149,153)},
    'full_projection_blocker': 'Shared intelligence has no proved source property. Native zero-rank skills/slot maps/faeries/quests/difficulty require retained native owner and explicit shared projection.',
    'dynamic_profile_fields': 'Name/level from SAME CharacterState; real saved-profile service supplies class StrID, act/location/difficulty/date. No illustrative level22 or David profile.',
    'style_source': 'Match original art TextField by authored receiver path; retain its font_id/source_height/rgba/layout. Native class_title absent from first frame requires actual noninitial-frame art recovery.',
    'complete_creation_service_available': False,
    'live_ida_mcp': '127.0.0.1:8746 connection refused; static export used',
    'scope': 'Class/menu/profile metadata only. No resolved balance fallback, unlock mapping, profile writes or whole creation parity claim.',
    'inputs': [{'path': str(p.relative_to(ROOT)).replace('\\', '/'),
                'sha256': hashlib.sha256(p.read_bytes()).hexdigest()} for p in paths],
}
(HERE / 'source_evidence.json').write_text(json.dumps(evidence, indent=2) + '\n')
print('PASS IDA menu order and original cache class metadata')
