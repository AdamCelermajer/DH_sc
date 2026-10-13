"""Conservative decoders for registered PyData tables not covered by table_sites.

Only formats backed by recovered readers are decoded here. Names/indices are
joined by order, and source-defined groups require exact name/schema alignment.
Unexplained stream suffixes stay explicit coverage gaps and keep affected rows
unresolved.
"""
from collections import Counter
import math
from pathlib import Path
import struct

from common import stable_id, source_site


class Reader:
    def __init__(self, data):
        self.data = bytes(data)
        self.at = 0

    def take(self, size):
        if size < 0 or self.at + size > len(self.data):
            raise ValueError(f"truncated stream at {self.at} (need {size}, have {len(self.data)-self.at})")
        value = self.data[self.at:self.at + size]
        self.at += size
        return value

    def u32(self): return struct.unpack('<I', self.take(4))[0]
    def i32(self): return struct.unpack('<i', self.take(4))[0]
    def i16(self): return struct.unpack('<h', self.take(2))[0]
    def i8(self): return struct.unpack('<b', self.take(1))[0]
    def f32(self): return struct.unpack('<f', self.take(4))[0]
    def u8(self): return self.take(1)[0]

    def count(self, maximum=100000):
        n = self.u32()
        if n > maximum: raise ValueError(f"count {n} exceeds bound {maximum}")
        return n

    def text(self, limit=1 << 20, strip_nul=False):
        n = self.count(limit)
        value = self.take(n).decode('latin1')
        return value[:-1] if strip_nul and value.endswith('\0') else value

    def strings(self, maximum=100000): return [self.text() for _ in range(self.count(maximum))]

    def end(self):
        if self.at != len(self.data): raise ValueError(f"unconsumed suffix: {len(self.data)-self.at} bytes")


def _groups(path):
    r = Reader(Path(path).read_bytes())
    out = []
    while r.at < len(r.data): out.append(r.strings())
    r.end()
    return out


def _row_map(tables):
    return {(r.get('table'), r.get('index')): r for r in tables}


def decode(ctx, tables):
    """Return proven payload updates and semantic references for selected tables.

    `tables` is the stable-ID inventory from tables.json. The function does not
    write generated data. Unsupported or failed groups are reported in `gaps`.
    """
    py = ctx.inputs / 'assets-extra/android/data/pydata'
    lookup = _row_map(tables)
    updates, references, gaps, decoded = [], [], [], Counter()

    def source(stem, suffix):
        path = py / f'{stem}_{suffix}.bin'
        if not path.is_file(): raise ValueError(f'missing source stream {path.name}')
        return path

    def get_row(stem, group, index):
        table = f'{stem}#group{group}'
        row = lookup.get((table, index))
        if row is None: raise ValueError(f'inventory row missing: {table}[{index}]')
        return row

    def update(stem, group, index, name, fields, schema, consumer):
        row = get_row(stem, group, index)
        if row.get('name') != name: raise ValueError(f'name/order mismatch: {stem} group {group} row {index}')
        merged = dict(row.get('fields') or {})
        for key, value in fields.items():
            if key in merged and merged[key] != value:
                raise ValueError(f'payload conflicts with existing field: {stem} group {group} row {index} field {key}')
            merged.setdefault(key, value)
        updates.append({'id': row['id'], 'table': row['table'], 'index': index, 'fields': merged,
                        'status': 'RESOLVED', 'work_status': 'DONE', 'reason': None,
                        'input_status': row.get('status'),
                        'schema_evidence': schema, 'consumer_evidence': consumer,
                        'source_file': source_site(ctx, py / f'{stem}_pyarray.bin')})
        decoded[stem] += 1
        return row

    def update_alias(table, index, name, fields, schema, consumer, source_stem):
        row = lookup.get((table, index))
        if row is None: raise ValueError(f'inventory row missing: {table}[{index}]')
        if row.get('name') != name: raise ValueError(f'name/order mismatch: {table} row {index}')
        merged = dict(row.get('fields') or {})
        for key, value in fields.items():
            if key in merged and merged[key] != value:
                raise ValueError(f'payload conflicts with existing field: {table} row {index} field {key}')
            merged.setdefault(key, value)
        updates.append({'id':row['id'],'table':table,'index':index,'fields':merged,
                        'status':'RESOLVED','work_status':'DONE','reason':None,
                        'input_status':row.get('status'),
                        'schema_evidence':schema,'consumer_evidence':consumer,
                        'source_file':source_site(ctx,py/f'{source_stem}_pyarray.bin')})
        decoded[table] += 1
        return row

    def edge(row, field, value, kind, target_table, rule):
        if value < 0 or (target_table, value) not in lookup: return
        table_stem = row['table'].split('#')[0]
        file_site = source_site(ctx, py / f'{table_stem}_pyarray.bin')
        site = f"{file_site}[table={row['table']},row={row['index']}].{field}"
        references.append({'id': stable_id('cell-ref', site, value, kind), 'site': site,
                           'name': str(value), 'kind': kind, 'rule': rule,
                           'status': 'TODO', 'work_status': 'TODO', 'reason': None,
                           'candidates': [], 'confidence': 'PROVEN', 'port_status': 'PARTIAL',
                           'implementation': rule, 'context': {'file': file_site,
                           'table': row['table'], 'index': row['index'], 'field': field},
                           'resolves_to': [{'type': 'table-row', 'table': target_table, 'index': value}],
                           'referenced_table': target_table, 'referenced_index': value})

    # CharacterClasses: Structs::ClassTable/FuncList/Func readers and the class
    # formula dispatcher; all thirteen serialized field-name sections are fixed.
    def character_classes():
        stem = 'character_classes'
        names = _groups(source(stem, 'pyarraynames'))
        schemas = _groups(source(stem, 'pystructnames'))
        expected = [
            ['DestProp','Formula','Param1','Param2','Param3'], ['list_entries'],
            ['DestProp','Formula','Base','ScaleProp','PerProp'],
            ['DestProp','Formula','Value','UNUSED0','UNUSED1'],
            ['DestProp','Formula','Value','NotUsed1','NotUsed2'],
            ['DestProp','Formula','Value','NotUsed1','NotUsed2'],
            ['DestProp','Formula','Stat1','Stat2','Stat3'],
            ['DestProp','Formula','Min','Max','NotUsed'],
            ['DestProp','Formula','Base','ScaleProp','PerProp'],
            ['DestProp','Formula','PropToAdd','NotUsed','NotUsed2'],
            ['DestProp','Formula','ScaleProp','NotUsed','NotUsed2'],
            ['DestProp','Formula','ScaleProp','NotUsed','NotUsed2'],
            ['DestProp','Formula','IsSpecialAttack','IsSpecialAttackState','NotUsed1']]
        if len(names) != 1 or len(schemas) != len(expected) or schemas != expected:
            raise ValueError('registered CharacterClasses name/schema layout differs')
        r = Reader(source(stem, 'pyarray').read_bytes())
        count = r.count(10000)
        if count != len(names[0]): raise ValueError('ClassTable row/name count mismatch')
        for i, name in enumerate(names[0]):
            formulas = []
            for j in range(r.count(10000)):
                formulas.append({'DestProp': r.i32(), 'Formula': r.i32(), 'Param1': r.i32(),
                                 'Param2': r.i32(), 'Param3': r.i32()})
            row = update(stem, 0, i, name, {'Formulas': formulas}, 'port/game-data/class_tables.cpp:39-103; 13 exact pystructnames sections',
                         'port/game-data/class_tables.cpp:95-103; class formula dispatcher and grouped-class admission')
            for j, f in enumerate(formulas):
                if f['Formula'] == 0 and any(v < -1 or v >= len(names[0]) for v in (f['Param1'], f['Param2'], f['Param3'])):
                    raise ValueError('Class group reference outside ClassTable rows')
                if f['Formula'] == 0:
                    for param in ('Param1', 'Param2', 'Param3'):
                        target = f[param]
                        if target >= 0:
                            if target >= len(names[0]): raise ValueError('Class group reference outside ClassTable rows')
                            edge(row, f'Formulas[{j}].{param}', target, 'class', f'{stem}#group0',
                                 'Formula 0 recursively applies referenced ClassTable rows; class_tables.cpp:29-39, 54-67.')
        r.end()

    # CharacterTemplates: Structs::CharTemplate reads Name followed by CharInfo
    # vector<int>; the population consumer requests the selected IDs as a preset.
    def character_templates():
        stem = 'character_templates'
        names = _groups(source(stem, 'pyarraynames'))
        schemas = _groups(source(stem, 'pystructnames'))
        if len(names) != 1 or schemas != [['Name'], ['CharInfo']]: raise ValueError('CharTemplate schema/name layout differs')
        r, n = Reader(source(stem, 'pyarray').read_bytes()), Reader(source(stem, 'pyarraynames').read_bytes())
        count = r.count(65536)
        if n.count(65536) != count or count != len(names[0]): raise ValueError('CharTemplate dimensions mismatch')
        for i, name in enumerate(names[0]):
            members = r.count(65536)
            if members > (len(r.data)-r.at)//4: raise ValueError('CharTemplate member count exceeds remaining bytes')
            values = [r.i32() for _ in range(members)]
            if n.text() != name: raise ValueError('CharTemplate name stream order mismatch')
            update(stem, 0, i, name, {'CharInfo': values}, 'port/game-data/character_templates_v78.cpp:19-21, 29-34',
                   'port/game-data/character_templates_v78.cpp:26-34, 42-47; CharacterTemplateTableV78::preset exposes ordered selected IDs')
        r.end(); n.end()

    # Arrays.v2Conditions: count, then rows with signed stub count, three i32
    # stub fields and i32 type. This matches the original reader (not the older
    # fixed-row projection) and consumes all registered source bytes.
    def conditions():
        stem = 'v2conditions'
        names = _groups(source(stem, 'pyarraynames'))
        schemas = _groups(source(stem, 'pystructnames'))
        expected_stub = {'Quest':['Op','Quest','State'], 'Event':['Op','Event','State'],
                         'Level':['Op','Level','NotUsed1']}
        if len(names) != 1 or len(schemas) < 3: raise ValueError('condition names/schema groups differ')
        if schemas[0] != ['Op'] or schemas[1] != ['Op','Op1','Op2'] or schemas[-1] != ['Conds','Type']:
            raise ValueError('condition operator/source schema differs')
        if any(group not in expected_stub.values() for group in schemas[2:-1]):
            raise ValueError('condition stub schema is unsupported')
        r = Reader(source(stem, 'pyarray').read_bytes())
        count = r.count(65536)
        if count != len(names[0]): raise ValueError('condition array count/name mismatch')
        for i, name in enumerate(names[0]):
            stubs = []
            for _ in range(r.count(262144)):
                op, arg, state = r.i32(), r.i32(), r.i32()
                if op < 0 or op >= 7: raise ValueError(f'condition factory {op} outside captured factory range')
                stubs.append({'Op':op,'Argument':arg,'State':state})
            kind = r.i32()
            row = update(stem, 0, i, name, {'Stubs':stubs,'Type':kind},
                         'port/level-world/native_conditions_v69.hpp:25-26; native_conditions_table_v69.cpp:34-43; registration v101 [iii]i at 0x4b8dec, names 0x4a4924',
                         'NativeConditionTableV69::decode; authored_conditions_v1.cpp:65-139 evaluates op0-6; event ID is retained without row-index assumption')
            for j, stub in enumerate(stubs):
                if stub['Op'] in (0,1,2):
                    edge(row, f'Stubs[{j}].Argument', stub['Argument'], 'quest', 'v2quests#group0',
                         'Native authored condition factories 0-2 pass stub.argument8 to quest_state; authored_conditions_v1.cpp:90-112.')
                elif stub['Op'] == 3:
                    edge(row, f'Stubs[{j}].Argument', stub['Argument'], 'level', 'levels#group1',
                         'Native authored condition factory 3 compares stub.argument8 to current level row; authored_conditions_v1.cpp:113-125.')
        r.end()

    # Quest definitions have variable-length child arrays; mirror the exact
    # order used by QuestTablesPersistenceV51::decode. No command/script meaning
    # is inferred from string values.
    def quests():
        stem = 'v2quests'
        names = _groups(source(stem, 'pyarraynames'))
        schemas = _groups(source(stem, 'pystructnames'))
        if len(names) != 1 or len(schemas) != 29: raise ValueError('quest names/schema group dimensions differ')
        r, n = Reader(source(stem, 'pyarray').read_bytes()), Reader(source(stem, 'pyarraynames').read_bytes())
        count, name_count = r.count(65536), n.count(65536)
        if count != name_count or count != len(names[0]): raise ValueError('quest row/name count mismatch')
        def obj(): return {'Type':r.i32(),'Description':r.i32(),'OnComplete':r.i32(),'Str1':r.text(strip_nul=True),
                           'Str2':r.text(strip_nul=True),'OID1':r.i32(),'OID2':r.i32(),'Value':r.i32()}
        for i, expected_name in enumerate(names[0]):
            name = n.text(strip_nul=True)
            if name != expected_name: raise ValueError('quest name order mismatch')
            text_fields = [r.i32() for _ in range(4)]
            prereq = [{'Type':r.i32(),'Parameter1':r.i32(),'Parameter2':r.i32()} for _ in range(r.count(65536))]
            objectives = [obj() for _ in range(r.count(65536))]
            rewards = [[{'Type':r.i32(),'Parameter1':r.i32(),'Parameter2':r.i32()} for _ in range(r.count(65536))] for _ in range(3)]
            accept, end = obj(), obj()
            target_level, repeatable, state = r.i32(), r.u8(), r.i32()
            scripts = [r.text(strip_nul=True) for _ in range(14)]
            priority, act = r.i32(), r.i32()
            update(stem, 0, i, name, {'TextFields':text_fields,'Prerequisites':prereq,'Objectives':objectives,
                   'Rewards':rewards,'Accept':accept,'End':end,'TargetLevel':target_level,'Repeatable':repeatable,
                   'State':state,'Scripts':scripts,'Priority':priority,'Act':act},
                   'port/game-data/quest_persistence_v51.cpp:8-27, 43-67',
                   'QuestTablesPersistenceV51::decode; original quest row reader preserves typed children and exact order')
        r.end(); n.end()

    # DialogActors source reader: row count then FrameID, length-prefixed label,
    # Name StringID. Existing actor-name consumer uses the string manager ID.
    def dialogs():
        stem = 'dialogs'
        names = _groups(source(stem, 'pyarraynames'))
        schemas = _groups(source(stem, 'pystructnames'))
        if (len(names) != 2 or len(schemas) != 5 or schemas[0] != ['FrameID','FrameLabel','Name']
                or schemas[3] != ['Actor','Style','Text','TitleText']):
            raise ValueError('DialogActors schema/name layout differs')
        r = Reader(source(stem, 'pyarray').read_bytes())
        count = r.count(65536)
        if count != len(names[0]): raise ValueError('DialogActors row/name count mismatch')
        for i, name in enumerate(names[0]):
            fields_value = {'FrameID':r.i32(),'FrameLabel':r.text(strip_nul=True),'Name':r.i32()}
            update(stem, 0, i, name, fields_value, 'port/windows-foundation/features/cinematic_runner/dialog_tables.cpp:36-49; registered DialogActors schema',
                   'IDA DialogMsg::SetActorName 0x433dd8; dialog_tables.hpp:3-5 identifies Name as StringManager::getString ID')
        count = r.count(65536)
        if count != len(names[1]): raise ValueError('Dialogs record/name count mismatch')
        for i, name in enumerate(names[1]):
            steps = []
            for _ in range(r.count(65536)):
                steps.append({'Actor':r.i32(),'Style':r.i32(),'Text':r.i32(),'TitleText':r.i32()})
            update(stem, 1, i, name, {'Steps':steps}, 'source_process_array_registration_v101.hpp:39; registered Dialogs format [iiii]; dialogs_pystructnames group 3',
                   'registered Dialogs table reader format and exact four-int step layout; row retains raw caption/style IDs')
        r.end()

    # WorldMap: Arrays.WorldMap and WorldMapLockers are distinct registered
    # groups. WldMapLocation uses {Name, State, LocationLevels}; locker rows use
    # {OnState, QuestID}. This is source data, not fast-travel's separate table.
    def worldmap():
        stem = 'worldmap'
        names = _groups(source(stem, 'pyarraynames'))
        schemas = _groups(source(stem, 'pystructnames'))
        if len(names) != 2 or schemas != [['Name','State','LocationLevels'], ['OnState','QuestID']]:
            raise ValueError('WorldMap source schema/name groups differ')
        r, n = Reader(source(stem, 'pyarray').read_bytes()), Reader(source(stem, 'pyarraynames').read_bytes())
        for group in range(len(schemas)):
            if n.strings() != names[group]: raise ValueError('WorldMap name stream order mismatch')
        n.end()
        count = r.count(65536)
        if count != len(names[0]): raise ValueError('WorldMap location count mismatch')
        for i, name in enumerate(names[0]):
            values = {'Name':r.i32(), 'State':r.i32()}
            values['LocationLevels'] = [r.i32() for _ in range(r.count(65536))]
            update(stem, 0, i, name, values, 'port/game-data/world_map_profile_table_v59.cpp:17-29; exact WorldMap pystructnames group 0',
                   'WorldMapProfileTableV59::decode; actual Arrays.WorldMap/WldMapLocation rows, source stride 20')
        count = r.count(65536)
        if count != len(names[1]): raise ValueError('WorldMapLocker count mismatch')
        for i, name in enumerate(names[1]):
            update(stem, 1, i, name, {'OnState':r.i32(), 'QuestID':r.i32()},
                   'port/game-data/world_map_profile_table_v59.cpp:29-30; exact WorldMap pystructnames group 1',
                   'WorldMapProfileTableV59::decode; process Arrays.WorldMapLockers / WorldMapLocker')
        r.end()

    # Source rows whose schemas and actual consumers are both recovered.
    # Integers that are opaque to the consumer are preserved as words rather
    # than promoted to guessed IDs or meanings.
    def item_powers_monopoly():
        stem = 'item_powers_monopoly'
        names = _groups(source(stem, 'pyarraynames'))
        schemas = _groups(source(stem, 'pystructnames'))
        if len(names) != 1 or schemas != [['list_entries']]: raise ValueError('ItemBonusAttrMonopoly schema differs')
        r = Reader(source(stem, 'pyarray').read_bytes())
        count = r.count(65536)
        if count != len(names[0]): raise ValueError('monopoly row/name count differs')
        for i, name in enumerate(names[0]):
            values = [r.i32() for _ in range(r.count(65536))]
            update(stem, 0, i, name, {'Entries':values},
                   'source_process_array_registration_v101.hpp:83; ItemBonusAttrMonopoly [i], add 0x4baa30, names 0x4b6c94; pystructnames list_entries',
                   'registered ItemBonusAttrList projection (one int per list entry); consumer semantics for individual entries are not asserted')
        r.end()

    def trophies():
        stem = 'trophies'
        names = _groups(source(stem, 'pyarraynames'))
        schemas = _groups(source(stem, 'pystructnames'))
        fields = ['Desc','GLIndex','GLLive','Grade','Label','Name','Type']
        if len(names) != 1 or schemas != [fields]: raise ValueError('TrophyTable schema differs')
        r = Reader(source(stem, 'pyarray').read_bytes())
        count = r.count(65536)
        if count != len(names[0]): raise ValueError('TrophyTable row/name count differs')
        for i, name in enumerate(names[0]):
            values = {field:r.i32() for field in fields}
            update(stem, 0, i, name, values,
                   'source_process_array_registration_v101.hpp:125; TrophyTable i*7 at 0x4b8f28, names 0x4b1130; trophies_pystructnames exact 7-field schema',
                   'port/level-world/trophy_manager_owner_v1.cpp:12-18 loads seven-word TrophyCatalog rows; process trophy manager projects named unlock and hit operations')
        r.end()

    def fonts():
        stem = 'fonts'
        names = _groups(source(stem, 'pyarraynames'))
        schemas = _groups(source(stem, 'pystructnames'))
        fields = ['glowcolor','textcolor']
        if len(names) != 1 or schemas != [fields]: raise ValueError('FontPalette schema differs')
        r = Reader(source(stem, 'pyarray').read_bytes())
        count = r.count(65536)
        if count != len(names[0]): raise ValueError('FontPalette row/name count differs')
        for i, name in enumerate(names[0]):
            values = {field:r.u32() for field in fields}
            update(stem, 0, i, name, values,
                   'source_process_array_registration_v101.hpp:51; FontPalette ii at 0x4bbe9c, names 0x4b2ad0; exact font pystructnames',
                   'port/engine-ui/character_menu_font_palette_v1.cpp:5-11 validates the two colors and uses textcolor for colored item names')
        r.end()

    def help_pages():
        stem = 'help_pages'
        names = _groups(source(stem, 'pyarraynames'))
        schemas = _groups(source(stem, 'pystructnames'))
        expected = [['HelpStringCategory','HelpStringText'],['HintAvatarID','HintStringID']]
        if len(names) != 2 or schemas != expected: raise ValueError('help/hint page schemas differ')
        r, n = Reader(source(stem, 'pyarray').read_bytes()), Reader(source(stem, 'pyarraynames').read_bytes())
        for group, (table, fmt, consumer) in enumerate((
            ('HelpPages','ii','port/engine-ui/loading_menu_v1.cpp:18-19 consumes HelpPages as source byte rows'),
            ('HintPages','ii','port/engine-ui/loading_menu_v1.cpp:20-23 selects source HintPage row and returns HintStringID'))):
            count, name_count = r.count(65536), n.count(65536)
            if count != name_count or count != len(names[group]): raise ValueError(f'{table} count/name mismatch')
            labels = [n.text() for _ in range(name_count)]
            if labels != names[group]: raise ValueError(f'{table} name order differs')
            for i, name in enumerate(labels):
                values = {field:r.i32() for field in expected[group]}
                update(stem, group, i, name, values,
                       f'source_process_array_registration_v101.hpp:{75+group*2}; {table} {fmt}; exact group-{group} pystructnames schema',
                       consumer)
        r.end(); n.end()

    def v2eventmanager():
        stem = 'v2eventmanager'
        names = _groups(source(stem, 'pyarraynames'))
        if len(names) != 1: raise ValueError('v2Events names must have one group')
        r, n = Reader(source(stem, 'pyarray').read_bytes()), Reader(source(stem, 'pyarraynames').read_bytes())
        count, name_count = r.count(65536), n.count(65536)
        if count != name_count or count != len(names[0]): raise ValueError('v2Events count/name mismatch')
        labels = [n.text() for _ in range(name_count)]
        if labels != names[0]: raise ValueError('v2Events name order differs')
        for i, name in enumerate(labels):
            scalar, script, objective_count = r.i32(), r.text(), r.count(4096)
            if objective_count > (len(r.data)-r.at)//28: raise ValueError('v2Events objective count exceeds stream')
            objectives = []
            for _ in range(objective_count):
                objectives.append({'Type':r.i32(),'Field8':r.i32(),'FieldC':r.i32(),
                                   'String14':r.text(),'String1C':r.text(),
                                   'Field20':r.i32(),'Field24':r.i32(),'Field28':r.i32()})
            update(stem, 0, i, name, {'Field4':scalar,'Script':script,'Objectives':objectives},
                   'source_process_array_registration_v101.hpp:129; v2Events iS[iiiSSiii] add 0x4b8c9c, names 0x4b1460; no pystructnames schema',
                   'port/level-loader/game_event_manager_v50.cpp:19-23 decodes exact row order and objective members; manager loads objective types 0-12 (lines 50-66); fields without recovered meanings remain ordinal/raw')
        r.end(); n.end()

    def spawn_groups():
        stem = 'spawn_groups'
        names = _groups(source(stem, 'pyarraynames'))
        schemas = _groups(source(stem, 'pystructnames'))
        if len(names) != 1 or schemas != [['Char','Prob','Qty'],['ActiveSpotOnly','Delay','Spawns']]:
            raise ValueError('SpawnGroups schema differs')
        r = Reader(source(stem, 'pyarray').read_bytes())
        count = r.count(65536)
        if count != len(names[0]): raise ValueError('SpawnGroups count/name mismatch')
        for i, name in enumerate(names[0]):
            active, delay, nspawns = r.u8(), r.i32(), r.count(65536)
            if nspawns > (len(r.data)-r.at)//12: raise ValueError('SpawnGroups choices exceed stream')
            choices = [{'Char':r.i32(),'Prob':r.i32(),'Qty':r.i32()} for _ in range(nspawns)]
            update(stem, 0, i, name, {'ActiveSpotOnly':active,'Delay':delay,'Spawns':choices},
                   'source_process_array_registration_v101.hpp:123; SpawnGroups bi[iii] at 0x4b9060, names 0x4b12c8; exact two pystructnames groups',
                   'port/android-native/app/src/main/cpp/source_campaign_spawn_groups_v108.cpp:76-78 consumes byte, word, then iii choices; semantic character resolution is outside this row decoder')
        r.end()

    def design():
        stem = 'design'
        names = _groups(source(stem, 'pyarraynames'))
        schemas = _groups(source(stem, 'pystructnames'))
        if len(names) != 3 or len(schemas) != 6: raise ValueError('DesignSettings group dimensions differ')
        if len(schemas[0]) != 43 or len(schemas[1]) != 5 or any(x != schemas[2] for x in schemas[3:]):
            raise ValueError('DesignSettings/GameOption schema repetitions differ')
        r, n = Reader(source(stem, 'pyarray').read_bytes()), Reader(source(stem, 'pyarraynames').read_bytes())
        for _ in range(3):
            if n.strings() != names[_]: raise ValueError('design names group order differs')
        n.end()
        def fixed(group, fields, consumer, native):
            count = r.count(65536)
            if count != len(names[group]): raise ValueError(f'design group {group} count/name mismatch')
            for i, name in enumerate(names[group]):
                words = [r.u32() for _ in fields]
                payload = [{'Field':field,'Bits':bits} for field,bits in zip(fields,words)]
                update(stem, group, i, name, {'RawWords':payload},
                       f'port/android-native/app/src/main/cpp/source_process_array_registration_v101.hpp:31-36; {native}; schema group {group}', consumer)
        fixed(0, schemas[0], 'port/game-data/design_settings.cpp:23-25,49-57 validates 43 names and preserves source words; design_settings_field_kind marks integer/float interpretation', 'DesignSettingsTable 43*i, add 0x4b3cd0, names 0x4a90b8')
        fixed(1, schemas[1], 'port/engine-ui/game_option_table_v1.cpp:27-30 skips the five-word difficulty prefix before reading options', 'GameDifficulties 5*i, add 0x4af5e8, names 0x4a8f38')
        # GameOptionTable reader consumes one count and 16 seven-word rows.
        count = r.count(65536)
        if count != len(names[2]): raise ValueError('GameOption count/name mismatch')
        fields = schemas[2]
        for group in schemas[3:]:
            if group != fields: raise ValueError('GameOption schema group differs')
        for i, name in enumerate(names[2]):
            values = [r.u32() for _ in fields]
            update(stem, 2, i, name, {'RawWords':[{'Field':field,'Bits':value} for field,value in zip(fields,values)]},
                   'source_process_array_registration_v101.hpp:35-36; GameOptionTable 7*i, add 0x4bc8e0, names 0x4a8dc4; schemas 2-5 repeat the seven-field row shape',
                   'port/engine-ui/game_option_table_v1.cpp:25-38 validates four repeated source schemas and decodes each named seven-word row')
        r.end()

    def loot_table_suffix():
        # Groups 0-5 are already parsed by table_sites. Walk their actual
        # source shapes here to establish the precise boundary before decoding
        # only the remaining registered MerchantTable and NumProbArray groups.
        stem = 'loot_table'
        names = _groups(source(stem, 'pyarraynames'))
        schemas = _groups(source(stem, 'pystructnames'))
        if len(names) != 8 or [len(group) for group in names] != [1,3,186,1322,9,339,12,39]:
            raise ValueError('LootTable registered name groups differ')
        if len(schemas) < 2 or len(schemas[1]) != 38: raise ValueError('LootTable item row schema unavailable')
        r = Reader(source(stem, 'pyarray').read_bytes())
        # DropTilePriority [hh]
        count = r.count(65536)
        if count != len(names[0]): raise ValueError('loot priority count differs')
        for _ in names[0]:
            for _ in range(r.count(65536)): r.i32()
        # Inventory row: two one-byte values, as consumed by LootTablesV2.
        count = r.count(65536)
        if count != len(names[1]): raise ValueError('loot inventory count differs')
        for _ in names[1]: r.u8(); r.u8()
        # ItemList [ihb]
        count = r.count(65536)
        if count != len(names[2]): raise ValueError('loot item-list count differs')
        for _ in names[2]:
            for _ in range(r.count(65536)): r.i32(); r.i16(); r.u8()
        # ItemTable: source field list from table_sites; strings/byte/float
        # tags are directly supported by the recovered reader in items.py.
        fields = schemas[1]
        count = r.count(65536)
        if count != len(names[3]): raise ValueError('loot item-table count differs')
        for _ in names[3]:
            for field in fields:
                if field in ('IconName','ModularModule'): r.text()
                elif field == 'Stackable': r.u8()
                else: r.take(4)
        # ItemTypeList [b] is consumed by recovered item table reader as text.
        count = r.count(65536)
        if count != len(names[4]): raise ValueError('loot item-type count differs')
        for _ in names[4]: r.text()
        # LootTable ii [8-int] [8-int] [int]
        count = r.count(65536)
        if count != len(names[5]): raise ValueError('loot table count differs')
        for _ in names[5]:
            r.i32(); r.i32()
            for _ in range(2):
                for _ in range(r.count(65536)): r.take(32)
            for _ in range(r.count(65536)): r.i32()
        merchant_count = r.count(65536)
        if merchant_count != len(names[6]): raise ValueError('MerchantTable count differs')
        for i, name in enumerate(names[6]):
            values = {'Field0':r.i32(),'Field4':r.i32(),
                      'Records':[{'First':r.i32(),'Second':r.i32()} for _ in range(r.count(65536))]}
            update(stem, 6, i, name, values,
                   'source_process_array_registration_v101.hpp:103; MerchantTable ii[ii], add 0x4b9d40, names 0x4b0460; reached after exact table_sites source layout for groups 0-5',
                   'MerchantTable registered row shape ii[ii]; each array element is an exact pair of signed words; field meanings remain positional')
        numprob_count = r.count(65536)
        if numprob_count != len(names[7]): raise ValueError('NumProbArray count differs')
        for i, name in enumerate(names[7]):
            pairs = [{'First':r.i16(),'Second':r.i16()} for _ in range(r.count(65536))]
            update(stem, 7, i, name, {'Pairs':pairs},
                   'source_process_array_registration_v101.hpp:105; NumProbArray [hh], add 0x4b9bf4, names 0x4b02c4; full stream continued after MerchantTable',
                   'NumProbArray source is a list of two signed halfwords; pair member names remain ordinal')
        r.end()

    def animations_cameras():
        # animation_tables.cpp is the actual full-stream consumer. Consume its
        # exact earlier/later group order but enrich only named camera sets.
        stem = 'animations'
        names = _groups(source(stem, 'pyarraynames'))
        schemas = _groups(source(stem, 'pystructnames'))
        step_fields = ['AnchorFX','Anim','BlendOut','Cam','CamDir','FX','MoveGO','RandomCam','Redir','Sound','Speed','Swoosh']
        sequence_fields = ['Loop','Steps','Type']
        camera_fields = ['CamAnims','Crit','Idle','Shake','Template']
        state_fields = ['Attack','AttackStatic','Blocking','DeadlyGreatKB','Despawn','DespawnGreatKB','Died','Dodging','GreatKnockedBack','Idle','IdleFromOOC','IdleOOC','IdleSneak','IdleToOOC','Injured','Interact','KnockedBack','LiftDrop','LiftIdle','LiftMove','Limbus','MenuIdle','MenuOnSelect','PreSpawn','Revived','Reviving','Run','Run180','RunSneak','Scared','Spawn','Spells','Stunned','Template','Walk','Walk180','WalkSneak']
        if len(names) != 3 or schemas != [step_fields,sequence_fields,camera_fields,state_fields]:
            raise ValueError('animation camera name/schema groups differ')
        # Verify the inventory's source alias rather than expecting a synthetic
        # animations#group1 key; references._table_inventory maps this group to
        # camera_animation_sets and owns the three existing stable IDs.
        if sum(1 for row in tables if row.get('table') == 'camera_animation_sets') != len(names[1]):
            raise ValueError('camera_animation_sets alias row count differs')
        r = Reader(source(stem, 'pyarray').read_bytes())
        sequences = r.count(65536)
        if sequences != len(names[0]): raise ValueError('animation sequence count differs')
        for _ in range(sequences):
            r.i32()
            for _ in range(r.count(10000)):
                if r.u8() not in (0,1): raise ValueError('animation AnchorFX is not a boolean')
                r.i32(); r.i32(); r.i32()
                if r.u8() not in (0,1): raise ValueError('animation CamDir is not a boolean')
                r.i32()
                if r.u8() not in (0,1): raise ValueError('animation MoveGO is not a boolean')
                for _ in range(r.count(10000)): r.i32()
                redirect, sound = r.i32(), r.i32()
                if redirect not in (0,1): raise ValueError('animation Redir outside native range')
                speed = struct.unpack('<f', r.take(4))[0]
                if not math.isfinite(speed) or speed <= 0 or speed > 100: raise ValueError('animation speed outside native range')
                if r.u8() not in (0,1): raise ValueError('animation Swoosh is not a boolean')
            r.i32()
        count = r.count(65536)
        if count != len(names[1]): raise ValueError('camera set count/name differs')
        for i, name in enumerate(names[1]):
            values = {'CamAnims':[r.i32() for _ in range(r.count(10000))],
                      'Crit':r.i32(),'Idle':r.i32(),'Shake':r.i32(),'Template':r.i32()}
            update_alias('camera_animation_sets', i, name, {'Camera':values},
                   'source_process_array_registration_v101.hpp:15-16; CamAnimSetTable [i]iiii at 0x4b37a4, names 0x4b4a58; exact animations_pystructnames camera group',
                   'port/game-data/animation_tables.cpp:13,65-67; load_animation_tables decodes CameraAnimationSet rows with vector<int> plus four signed words',
                   'animations')
        characters = r.count(65536)
        if characters != len(names[2]): raise ValueError('character animation count/name differs')
        for character in range(characters):
            for field in range(37):
                if field in (15,31):
                    for _ in range(r.count(10000)): r.i32()
                else: r.i32()
        r.end()

    def common_rects():
        stem = 'common'
        names = _groups(source(stem, 'pyarraynames'))
        schemas = _groups(source(stem, 'pystructnames'))
        fields = ['Height','PosX','PosY','Width']
        if len(names) != 1 or schemas[-1] != fields: raise ValueError('RectTable names/schema differ')
        r = Reader(source(stem, 'pyarray').read_bytes())
        count = r.count(65536)
        if count != len(names[0]): raise ValueError('RectTable count/name mismatch')
        for i, name in enumerate(names[0]):
            update(stem, 0, i, name, {field:r.i32() for field in fields},
                   'source_process_array_registration_v101.hpp:29-30; RectTable iiii at 0x4b3e18, names 0x4b796c; source_process_compiled_members_v121.hpp:20 names exact Rect fields',
                   'registered Structs::Rect exposes four signed words; these rectangles are retained as authored coordinates')
        r.end()

    def common_text():
        stem = 'common_text'
        names = _groups(source(stem, 'pyarraynames'))
        schemas = _groups(source(stem, 'pystructnames'))
        if len(names) != 1 or schemas != [['filename','name'],['list']]:
            raise ValueError('StrID_Languages schema/name layout differs')
        r = Reader(source(stem, 'pyarray').read_bytes())
        count = r.count(65536)
        if count != len(names[0]): raise ValueError('StrID_Languages count/name mismatch')
        for i, name in enumerate(names[0]):
            sheets = [{'filename':r.text(),'name':r.text()} for _ in range(r.count(65536))]
            update(stem, 0, i, name, {'list':sheets},
                   'source_process_array_registration_v101.hpp:147-148; StrID_Languages [SS] at 0x4b53f8, names 0x4b346c; pystructnames filename/name and list',
                   'port/engine-ui/reference/localization/NOTES.md:17-23; Arrays::StrID_Languages::read 0x4b53f8 and LangSheetList::read 0x4dc2ac produce ordered language-to-sheet metadata; no text-body rows are in this table')
        r.end()

    def character_properties_suffix():
        stem = 'character_properties'
        names = _groups(source(stem, 'pyarraynames'))
        schemas = _groups(source(stem, 'pystructnames'))
        if len(names) != 3 or [len(x) for x in names] != [448,3,4] or len(schemas) != 1 or len(schemas[0]) != 224:
            raise ValueError('CharacterProperties source dimensions differ')
        r = Reader(source(stem, 'pyarray').read_bytes())
        actors = r.count(65536)
        if actors != len(names[0]): raise ValueError('CharacterTable count/name mismatch before stat tables')
        r.take(actors * len(schemas[0]) * 4)
        suffix_start = r.at
        if len(r.data) - suffix_start != 196:
            raise ValueError(f'CharacterProperties suffix boundary differs: expected 196 bytes, have {len(r.data)-suffix_start}')
        schemes = r.count(65536)
        if schemes != len(names[1]): raise ValueError('StatAutoAssignSchemeTable count/name mismatch')
        for i, name in enumerate(names[1]):
            entries = []
            for _ in range(r.count(65536)):
                entries.append([r.i32(),r.i32(),r.i32()])
            update(stem, 1, i, name, {'list_entries':entries},
                   'source_process_array_registration_v101.hpp:23-24; StatAutoAssignSchemeTable [iii] at 0x4b41f4, names 0x4b7e3c; compiled member list_entries at source_process_compiled_members_v121.hpp:17',
                   'Native source table is a named list of 3-word entries; no recovered consumer establishes meanings or ID namespaces for the tuple positions')
        stats = r.count(65536)
        if stats != len(names[2]): raise ValueError('StatListTable count/name mismatch')
        for i, name in enumerate(names[2]):
            update(stem, 2, i, name, {'Values':[r.i32(),r.i32()]},
                   'source_process_array_registration_v101.hpp:25-26; StatListTable ii at 0x4b40a8, names 0x4b7ca0; compiled member list_entries at source_process_compiled_members_v121.hpp:18',
                   'StatList row preserves its exact two signed words; no consumer justifies naming either integer or emitting references')
        r.end()

    def sounds_legacy():
        stem = 'sounds'
        names = _groups(source(stem, 'pyarraynames'))
        schemas = _groups(source(stem, 'pystructnames'))
        if len(names) != 5 or [len(x) for x in names] != [3,5,27,8,183] or len(schemas) != 8:
            raise ValueError('legacy sounds group dimensions differ')
        expected = {
            0:['EnteringBus','Positioning3D'], 1:['MaxPlayback','MaxPlaybackBehavior'],
            2:['Bank','Channel','FileName','Format','Group','LoadType','Priority','Repeat','Volume'],
            3:['Id','Type'], 4:['Id','Type'], 5:['Die','Hurt','ImpactVsFlesh','ImpactVsMetal','IsFlesh','IsMetallic'],
            6:['X','Y','Z'], 7:['Anchor','MaxDistance','Orientation','RefDistance','RolloffFactor','UpVector']}
        if schemas != [expected[i] for i in range(8)]: raise ValueError('legacy sounds pystructnames groups differ')
        r = Reader(source(stem, 'pyarray').read_bytes())
        # The preceding CharSoundsTable belongs to the mob_sounds decoder and
        # to the distinct SoundAutoGen namespace; walk its exact native shape
        # to place Listeners without joining either sound ID space.
        char_count = r.count(65536)
        if char_count != len(names[0]): raise ValueError('CharSounds prefix count/name mismatch')
        for _ in range(char_count):
            for _ in range(4):
                for _ in range(r.count(65536)): r.i32()
            r.u8(); r.u8()
        listeners = r.count(65536)
        if listeners != len(names[1]): raise ValueError('Listeners count/name mismatch')
        for i, name in enumerate(names[1]):
            vals = {'Anchor':r.i32(),'MaxDistance':r.i32(),'Orientation':r.i32(),
                    'RefDistance':r.i32(),'RolloffFactor':r.f32(),'UpVector':r.i32()}
            update(stem, 1, i, name, vals,
                   'source_process_array_registration_v101.hpp:115-116; Listeners iiiifi at 0x4b9578, names 0x4b0c68; exact listener pystructnames group 7',
                   'port/engine-audio/reports/audio-v38-handoff.md; AudioListenerRowsV38 reads these six source members, and original Level::UpdateListener selects a source listener row')
        banks = r.count(65536)
        if banks != len(names[2]): raise ValueError('SoundBankPlayback count/name mismatch')
        for i, name in enumerate(names[2]):
            update(stem, 2, i, name, {field:r.i32() for field in expected[1]},
                   'source_process_array_registration_v101.hpp:117-118; SoundBankPlayback ii at 0x4b9434, names 0x4b0ad0; source compiled SoundBank members are MaxPlayback/MaxPlaybackBehavior',
                   'Registered two-word SoundBankPlayback source rows; no consumer-backed reference space is assigned')
        groups = r.count(65536)
        if groups != len(names[3]): raise ValueError('SoundGroupsRouting count/name mismatch')
        for i, name in enumerate(names[3]):
            update(stem, 3, i, name, {'EnteringBus':r.text(),'Positioning3D':r.i32()},
                   'source_process_array_registration_v101.hpp:119-120; SoundGroupsRouting Si at 0x4b92f8, names 0x4b0f98; compiled SoundGroup member names EnteringBus/Positioning3D',
                   'The source row keeps its string and integer exactly; no bus identifier resolution is inferred')
        sounds = r.count(65536)
        if sounds != len(names[4]): raise ValueError('Sounds_bak count/name mismatch')
        fields = expected[2]
        for i, name in enumerate(names[4]):
            values = {field:(r.text() if field == 'FileName' else r.i32()) for field in fields}
            update(stem, 4, i, name, values,
                   'source_process_array_registration_v101.hpp:121-122; Sounds_bak iiSiiiiii at 0x4b91ac, names 0x4b0e00; exact Sound pystructnames schema group 2',
                   'Legacy Sounds_bak source records preserve nine field positions; this namespace is separate from the live 638-row SoundAutoGen/sdd_dungeon_hunter_2_iphone table; no IDs or references are joined')
        r.end()

    # Grouped game_objects: retain every registered row using the exact
    # PyDataArrays.C1 table order and native format strings. Struct-name groups
    # are carried as evidence, but a field label is attached only when the
    # field-count alignment is unambiguous; otherwise typed payload positions
    # remain ordinal, avoiding a guessed name-to-field join.
    def game_objects():
        stem = 'game_objects'
        names = _groups(source(stem, 'pyarraynames'))
        schemas = _groups(source(stem, 'pystructnames'))
        groups = ('DestructibleContainers','Doors','ExplosiveTraps','GameObjectDamager','LiftableObjects',
                  'OpenableContainers','ProjectileTraps','TimerTraps','TriggerObjects','TriggerPlates','TriggerTraps')
        shapes = ('iiiiiiibiiSiiii','Siii','iiSi','iiiii','i','iibiSiii','iiibiSii','iiiSii','iSii','ibiiiii','iiSii')
        if len(names) != len(groups) or len(schemas) != 12:
            raise ValueError('game_objects registered group/schema dimensions differ')
        r, n = Reader(source(stem, 'pyarray').read_bytes()), Reader(source(stem, 'pyarraynames').read_bytes())
        for group, (group_name, shape) in enumerate(zip(groups, shapes)):
            count, name_count = r.count(4096), n.count(4096)
            if count != name_count or count != len(names[group]): raise ValueError(f'{group_name} row/name count mismatch')
            labels = [n.text() for _ in range(name_count)]
            if labels != names[group]: raise ValueError(f'{group_name} name order mismatch')
            for i, name in enumerate(labels):
                payload = []
                for tag in shape:
                    if tag == 'S': payload.append({'type':'cstring','value':r.text()})
                    elif tag == 'b': payload.append({'type':'byte','value':r.u8()})
                    else: payload.append({'type':'i32','value':r.i32()})
                fields = {'NativeGroup':group_name, 'Format':shape, 'Values':payload}
                update(stem, group, i, name, fields,
                       'port/level-loader/game_object_arrays_owner_v81.cpp:22-25; source_process_array_registration_v101.hpp entries for game_objects',
                       'PyDataArrays.C1 addFuncsForFile grouped order; each Structs::*::read shape in runtime_timer_trap_v1.cpp:28-30; generic rows stay positional')
        r.end(); n.end()

    decoders = {'character_classes': character_classes, 'character_templates': character_templates,
                'v2conditions': conditions, 'v2quests': quests, 'dialogs': dialogs,
                'worldmap': worldmap, 'game_objects': game_objects,
                'item_powers_monopoly': item_powers_monopoly, 'trophies': trophies,
                'fonts': fonts, 'help_pages': help_pages, 'v2eventmanager': v2eventmanager,
                'spawn_groups': spawn_groups, 'design': design, 'loot_table': loot_table_suffix,
                'animations_camera_sets': animations_cameras,
                'common_rects': common_rects, 'common_text': common_text,
                'character_properties_suffix': character_properties_suffix,
                'sounds_legacy': sounds_legacy}
    for stem, fn in decoders.items():
        before_updates, before_references, before_decoded = len(updates), len(references), decoded.copy()
        try: fn()
        except Exception as exc:
            # Roll back only this family's partial rows and edges: no corrupt or
            # partially consumed family is represented as successful coverage.
            updates[before_updates:] = []
            references[before_references:] = []
            decoded.clear(); decoded.update(before_decoded)
            gaps.append({'table':stem,'reason':str(exc)})

    return {'rows': updates, 'references': references,
            'coverage': {'decoded_rows':len(updates),
                         'resolved_rows':sum(row['status']=='RESOLVED' for row in updates),
                         'unresolved_rows':sum(row['status']!='RESOLVED' for row in updates),
                         'newly_resolved_rows':sum(row.get('input_status')!='RESOLVED' for row in updates),
                         'pre_resolved_enrichments':sum(row.get('input_status')=='RESOLVED' for row in updates),
                         'decoded_by_table':dict(sorted(decoded.items())),
                         'references':len(references),'families_attempted':len(decoders)},
            'gaps': gaps}
