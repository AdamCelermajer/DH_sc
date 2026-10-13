# v21.8 subclass bindings generator.
#
# Regenerates original-melee-bindings.xml from the original cache (.local-inputs) exactly like
# .local-inputs/windows-melee-bindings/export.py, with one addition: the six subclass CharacterTable
# rows (KnightPlayerBase_Berserker/_Paladin, RoguePlayerBase_Archer/_Assassin, MagePlayerBase_Illusionist/
# _Necromancer) are exported as actors after the existing actors. Their ModelFile, AnimTable, AI, Sounds
# and Scale fields are identical to their base row in the original CharacterTable (see
# subclass-bindings-manifest.json), so their combat/animation bindings are the base bindings under the
# subclass row name.
#
# Usage (from the repo root, any Python 3):
#   py coordination/v21.8/export_melee_bindings_v21_8.py <local-inputs-dir> <output-xml> [--no-subclasses]
#
# The audit script is executed with its ROOT redirected to a temporary directory so that it does not
# rewrite tracked reports in the checkout. The staged original-cache files are read and verified only.
import sys, tempfile, json, struct, xml.etree.ElementTree as E
from pathlib import Path

LOCAL = Path(sys.argv[1]).resolve()
OUT_XML = Path(sys.argv[2]).resolve()
SUBCLASSES = '--no-subclasses' not in sys.argv
SUBCLASS_ROWS = ['KnightPlayerBase_Berserker', 'KnightPlayerBase_Paladin',
                 'RoguePlayerBase_Archer', 'RoguePlayerBase_Assassin',
                 'MagePlayerBase_Illusionist', 'MagePlayerBase_Necromancer']

AUDIT = LOCAL / 'windows-population-assets' / 'audit.py'
scratch_root = Path(tempfile.mkdtemp(prefix='melee-bindings-v21-8-'))
(scratch_root / 'port/windows-foundation/reports').mkdir(parents=True)
audit_source = AUDIT.read_text(encoding='utf-8').replace(
    'ROOT=Path(__file__).resolve().parents[2]', 'ROOT=Path(r"%s")' % scratch_root.as_posix(), 1)
assert audit_source != AUDIT.read_text(encoding='utf-8'), 'audit ROOT line not found'
A = {'__file__': str(AUDIT), '__name__': 'audit_v21_8'}
exec(compile(audit_source, str(AUDIT), 'exec'), A)

ROOT = A['ROOT']; OUT = A['OUT']; read = A['read']; R = A['R']; configs = A['configs']; assets = A['assets']
for uri in A['INDEX']:
    if uri.startswith('data/pydata/ai_') and uri.endswith('.bin'):
        read(uri, True)
an = R(read('data/pydata/ai_pyarraynames.bin')).texts(); ar = R(read('data/pydata/ai_pyarray.bin')); ai = []
for _ in range(ar.v()):
    ai.append(dict(AttackDelay=ar.v(), CombatBeat=ar.v(), CombatMusic=ar.v(), DelayedLoad=ar.v('B'), Flags=ar.v(), InteractRadius=ar.v('f'), LeashDistance=ar.v('f'), MeleeRadius=ar.v('f'), OnAggroSFX=ar.v(), Script=ar.text(), SelfFX=ar.v(), Trophy=ar.v(), Type=ar.v(), ViewRadius=ar.v('f'), ViewRadiusNoAggro=ar.v('f')))
fn = R(read('data/pydata/ai_factions_pyarraynames.bin')).texts(); fr = R(read('data/pydata/ai_factions_pyarray.bin')); factions = []
for _ in range(fr.v()):
    factions.append([dict(id=fr.v(), value=fr.v()) for _ in range(fr.v())])

root = E.Element('meleeBindings', version='1', source='original-cache', rangePolicy='original-strict-summed-melee-radii', factionPolicy='original-signed-relationship', targetPolicy='original-alive-hostile-non-player-pair')
policies = E.SubElement(root, 'sequencePolicies', source='data/pydata/animations_pyarray.bin', namesSource='data/pydata/animations_pyarraynames.bin', provenance='original-serialized-AnimTable')
for i, seq in enumerate(A['sequences']):
    E.SubElement(policies, 'sequence', id=str(i), name=A['seqnames'][i], type=str(seq['type']), loop=str(seq['loop']))
fs = E.SubElement(root, 'factions', fallbackId='10')
for i, entries in enumerate(factions):
    f = E.SubElement(fs, 'faction', id=str(i), name=fn[i])
    for relation in entries:
        E.SubElement(f, 'relation', target=str(relation['id']), value=str(relation['value']))

# Subclass rows are appended after every existing config so the existing actor elements keep their order and content.
if SUBCLASSES:
    for n in SUBCLASS_ROWS:
        A['config'](A['names'].index(n))

needed = []; bound = []; clip_models = {}
for n, c in configs.items():
    p = c['properties']; idx = p[A['fields'][1]]; faction = p[A['fields'][0]]; a = ai[idx if 0 <= idx < len(ai) else 8]
    actor = E.SubElement(root, 'actor', id=n, propertyRow=str(c['property_row']), factionId=str(faction), aiId=str(idx), aiName=an[idx], model=c['model'], template=c['states']['Template'][0] if c['states']['Template'] else '')
    E.SubElement(actor, 'ai', **{k: str(v) for k, v in a.items()})
    states = A['charanim'][c['animation_table']]
    for state in ('Attack', 'AttackStatic', 'Injured', 'Died', 'Idle', 'Walk', 'Spawn', 'PreSpawn'):
        sn = E.SubElement(actor, 'state', name=state)
        for sid in states[state]:
            if sid < 0:
                continue
            seq = A['sequences'][sid]; seqnode = E.SubElement(sn, 'sequence', id=str(sid), name=A['seqnames'][sid], loop=str(seq['loop']), type=str(seq['type']))

            def steps(sid, parent, seen=()):
                if sid in seen:
                    return
                for order, s in enumerate(A['sequences'][sid]['steps']):
                    snode = E.SubElement(parent, 'step', index=str(order), animationId=str(s['Anim']), redirect=str(s['Redir']), speed=str(s['Speed']), blendOut=str(s['BlendOut']), moveGO=str(s['MoveGO']))
                    if s['Redir']:
                        steps(s['Anim'], snode, seen + (sid,))
                    elif s['Anim'] >= 0:
                        uri = A['clips'][s['Anim']]; snode.set('uri', uri); read(uri, True)
                        if uri not in needed:
                            needed.append(uri); clip_models[uri] = c['model']
            steps(sid, seqnode)
    bound.append(dict(character=n, faction_id=faction, faction_name=fn[faction] if 0 <= faction < len(fn) else fn[10], ai_id=idx, ai_name=an[idx], ai=a))

clip_metadata = []
MARKERS = LOCAL / 'windows-melee-bindings' / 'markers.txt'
if MARKERS.exists():
    cs = E.SubElement(root, 'clips')
    for line in MARKERS.read_text().splitlines():
        vals = line.split('|'); uri = Path(vals[0]).relative_to(A['OUT'] / 'original-cache').as_posix(); markers = []
        node = E.SubElement(cs, 'clip', uri=uri, startMs=vals[1], endMs=vals[2])
        for item in vals[3:]:
            if item == 'NO_EVENTS':
                continue
            name, ms, authored = item.rsplit(',', 2); E.SubElement(node, 'marker', name=name, timeMs=ms, authoredTimeMs=authored); markers.append(dict(name=name, time_ms=int(ms), authored_time_ms=int(authored)))
        clip_metadata.append(dict(uri=uri, start_ms=int(vals[1]), end_ms=int(vals[2]), markers=markers))
E.indent(root)
OUT_XML.parent.mkdir(parents=True, exist_ok=True)
OUT_XML.write_bytes(E.tostring(root, encoding='utf-8', xml_declaration=True))
print(json.dumps(dict(xml=str(OUT_XML), actors=[b['character'] for b in bound], clips=len(needed), clip_metadata=len(clip_metadata), subclasses=SUBCLASSES)))
