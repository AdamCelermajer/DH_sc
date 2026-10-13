"""Reject accidental Android-ID joins for iPad source-only target types."""
from types import SimpleNamespace
import graph
import mobs
import entity_profiles
from common import read_json


def run(ctx):
    builder=graph.Builder.__new__(graph.Builder)
    builder.index=SimpleNamespace(tables=[],files=lambda _path:[])
    builder.table_ids={('character_templates#group0',0):'table:android-row'}
    builder.script_sites={};builder.edges={}
    builder.nodes={'table:android-row':{},'asset:ios-pack':{},'ios-name:ios-pack:0:0':{}}
    name={'type':'ios_source_name','source_asset_id':'ios-pack','group':0,'ordinal':0,
          'name':'Pack','native_id_verified':False}
    namespace={'type':'ios_table_source','source_asset_id':'ios-pack','group':0,
               'name':'Charater_Templates','native_id_verified':False}
    names=builder.targets(name);tables=builder.targets(namespace)
    tests=[{'name':'iPad source names cannot resolve to Android rows',
            'passed':names==['ios-name:ios-pack:0:0']},
           {'name':'iPad namespaces are scoped to their package source',
            'passed':tables==['ios-table:ios-pack:0'] and
                     builder.nodes[tables[0]]['platform']=='ios'},
           {'name':'iPad namespace points to indexed source bytes',
            'passed':any(e['from']==tables[0] and e['to']=='asset:ios-pack'
                         for e in builder.edges.values())}]
    field={'type':'table-field','table':'CharacterProperties','index':172,'name':'SomeField'}
    tests.append({'name':'registered struct fields never become missing gameplay table rows',
                  'passed':mobs.target_key(field) is None and
                           entity_profiles._edge_target({'resolves_to':[field]})==[]})
    data=read_json(ctx.out/'data/graph.json',{})
    refs={r['id']:r for r in read_json(ctx.out/'data/references.json',[])
          if any(t.get('type') in ('ios_source_name','ios_stored_constant','ios_table_source')
                 for t in r.get('resolves_to',[]))}
    targets={r:[] for r in refs}
    for edge in data.get('edges',[]):
        if edge.get('kind')=='resolution' and edge.get('reference_id') in targets:
            targets[edge['reference_id']].append(edge['to'])
    missing=[r for r,t in targets.items() if not t]
    leaked=[(r,t) for r,ts in targets.items() for t in ts if t.startswith('table:') or t.startswith('table-namespace:')]
    tests.append({'name':'all generated iPad source associations have only scoped graph targets',
                  'passed':bool(refs) and not missing and not leaked,
                  'detail':{'rows':len(refs),'missing':missing[:8],'Android_leaks':leaked[:8]}})
    return {'passed':all(t['passed'] for t in tests),'tests':tests}
