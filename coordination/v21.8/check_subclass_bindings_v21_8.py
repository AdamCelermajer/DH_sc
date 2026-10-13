# v21.8 subclass bindings check and manifest.
#
# Verifies .local-inputs/windows-melee-bindings/original-melee-bindings.xml against the backup it was generated from
# and against the original CharacterTable, then optionally regenerates it and compares bytes.
#
#   py coordination/v21.8/check_subclass_bindings_v21_8.py <local-inputs-dir> [--regenerate] [--write-manifest]
#
# Checks:
#  1. every actor of the backup (the pre-subclass file, original-melee-bindings.base4-backup.xml) is present in the
#     new file with an identical subtree (the four player actors are included);
#  2. the six subclass actors exist; each shares the base row's model, template, AI and faction, and its state
#     trees (Attack/Idle/Walk/...) equal the base row's state trees;
#  3. the original CharacterTable fields ModelFile, AnimTable, AI, Sounds, Scale_X/Y/Z, ClassID agree between each
#     subclass row and its base row (the reason the subclass actors reuse the base animation set);
#  4. with --regenerate, export_melee_bindings_v21_8.py reproduces the installed file byte-for-byte.
import hashlib, json, struct, subprocess, sys, tempfile
import xml.etree.ElementTree as E
from pathlib import Path

HERE = Path(__file__).resolve().parent
LOCAL = Path(sys.argv[1]).resolve()
BINDINGS = LOCAL / 'windows-melee-bindings'
XML = BINDINGS / 'original-melee-bindings.xml'
BACKUP = BINDINGS / 'original-melee-bindings.base4-backup.xml'
PYDATA = LOCAL / 'assets-extra/android/data/pydata'
SUBCLASSES = {
    'KnightPlayerBase': ['KnightPlayerBase_Berserker', 'KnightPlayerBase_Paladin'],
    'RoguePlayerBase': ['RoguePlayerBase_Archer', 'RoguePlayerBase_Assassin'],
    'MagePlayerBase': ['MagePlayerBase_Illusionist', 'MagePlayerBase_Necromancer'],
}
failures = []


def check(ok, message):
    if not ok:
        failures.append(message)


class R:
    def __init__(s, b): s.b = b; s.o = 0
    def v(s, f='i'):
        x = struct.unpack_from('<' + f, s.b, s.o)[0]; s.o += struct.calcsize(f); return x
    def text(s):
        n = s.v('I'); t = s.b[s.o:s.o + n].decode(); s.o += n; return t
    def texts(s): return [s.text() for _ in range(s.v('I'))]


def table_rows():
    names = R((PYDATA / 'character_properties_pyarraynames.bin').read_bytes()).texts()
    fields = R((PYDATA / 'character_properties_pystructnames.bin').read_bytes()).texts()
    raw = (PYDATA / 'character_properties_pyarray.bin').read_bytes()
    rows = {n: dict(zip(fields, struct.unpack_from('<224i', raw, 4 + i * 896))) for i, n in enumerate(names)}
    return rows, names


def actors(root):
    return {a.get('id'): a for a in root.findall('actor')}


def canonical(el):
    return E.tostring(el, encoding='unicode')


new_root = E.parse(XML).getroot()
old_root = E.parse(BACKUP).getroot()
new_actors, old_actors = actors(new_root), actors(old_root)
for name, old in old_actors.items():
    check(name in new_actors and canonical(new_actors[name]) == canonical(old),
          f'pre-existing actor changed or missing: {name}')
for base, subs in SUBCLASSES.items():
    for sub in subs:
        check(sub in new_actors, f'subclass actor missing: {sub}')
        if sub not in new_actors or base not in new_actors:
            continue
        b, s = new_actors[base], new_actors[sub]
        for attr in ('factionId', 'aiId', 'aiName', 'model', 'template'):
            check(b.get(attr) == s.get(attr), f'{sub}: {attr} differs from {base}')
        check([canonical(x) for x in b.findall('ai')] == [canonical(x) for x in s.findall('ai')], f'{sub}: AI block differs')
        check([canonical(x) for x in b.findall('state')] == [canonical(x) for x in s.findall('state')],
              f'{sub}: state (animation) trees differ from {base}')

rows, table_names = table_rows()
for base, subs in SUBCLASSES.items():
    for sub in subs:
        check(sub in rows and base in rows, f'CharacterTable row missing: {sub} or {base}')
        if sub not in rows or base not in rows:
            continue
        b, s = rows[base], rows[sub]
        for field in ('ModelFile', 'AnimTable', 'AI', 'Sounds', 'Scale_X', 'Scale_Y', 'Scale_Z', 'ClassID', 'AIFaction', 'ModularVisual'):
            check(b[field] == s[field], f'{sub}: CharacterTable {field} differs from {base}')
        xml_row = new_actors[sub].get('propertyRow') if sub in new_actors else None
        check(xml_row == str(table_names.index(sub)), f'{sub}: propertyRow does not match its CharacterTable index')

ok = not failures
for f in failures:
    print('FAIL', f)

result = dict(ok=ok, xml=str(XML), backup=str(BACKUP), actors=len(new_actors), backup_actors=len(old_actors),
              subclass_rows=[s for subs in SUBCLASSES.values() for s in subs])
if '--regenerate' in sys.argv:
    with tempfile.TemporaryDirectory() as tmp:
        out = Path(tmp) / 'regen.xml'
        subprocess.run([sys.executable, str(HERE / 'export_melee_bindings_v21_8.py'), str(LOCAL), str(out)], check=True,
                       stdout=subprocess.DEVNULL)
        same = out.read_bytes() == XML.read_bytes()
        check(same, 'regenerated XML differs from the installed file')
        result['regenerated_identical'] = same
if '--write-manifest' in sys.argv:
    manifest = dict(
        generator='coordination/v21.8/export_melee_bindings_v21_8.py',
        checker='coordination/v21.8/check_subclass_bindings_v21_8.py',
        backup=dict(path='.local-inputs/windows-melee-bindings/original-melee-bindings.base4-backup.xml',
                    sha256=hashlib.sha256(BACKUP.read_bytes()).hexdigest()),
        output=dict(path='.local-inputs/windows-melee-bindings/original-melee-bindings.xml',
                    sha256=hashlib.sha256(XML.read_bytes()).hexdigest()),
        original_fields_compared=['ModelFile', 'AnimTable', 'AI', 'Sounds', 'Scale_X', 'Scale_Y', 'Scale_Z', 'ClassID', 'AIFaction', 'ModularVisual'],
        only_differing_fields_per_subclass={sub: sorted(k for k in rows[sub] if rows[sub][k] != rows[base][k])
                                            for base, subs in SUBCLASSES.items() for sub in subs if sub in rows and base in rows},
        checks_passed=ok)
    (HERE / 'subclass-bindings-manifest.json').write_text(json.dumps(manifest, indent=2) + '\n', encoding='utf-8')
    result['manifest'] = str(HERE / 'subclass-bindings-manifest.json')
print(json.dumps(result, indent=2))
sys.exit(0 if ok else 1)
