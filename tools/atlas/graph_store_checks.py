"""Focused checks and measurements for the read-only Atlas graph API."""
from http.server import ThreadingHTTPServer
from pathlib import Path
from threading import Thread
from types import SimpleNamespace
import json
import tempfile
import time
from urllib.error import HTTPError
from urllib.request import Request, urlopen

from graph_store import GraphStore
from explorer_server import Handler
import server_checks


def _request(base, path, method='GET'):
    started = time.perf_counter()
    try:
        with urlopen(Request(base + path, method=method), timeout=45) as response:
            body = response.read()
            return response.status, response.headers, body, (time.perf_counter() - started) * 1000
    except HTTPError as error:
        body = error.read()
        return error.code, error.headers, body, (time.perf_counter() - started) * 1000


def _fixture_checks():
    tests = []
    with tempfile.TemporaryDirectory(prefix='atlas-graph-check-') as td:
        root = Path(td); out = root / 'out'; data_dir = out / 'data'; data_dir.mkdir(parents=True)
        graph_path = data_dir / 'graph.json'
        first = {'counts': {}, 'nodes': [
            {'id':'table:actor:swamp', 'name':'SwampKing', 'category':'Actors', 'kind':'actor', 'aliases':['Mud Lord','Swamp King'], 'index':17, 'site':'Data/Actors/SwampKing.lua'},
            {'id':'table:skill:bash', 'name':'BashDown', 'category':'Skills', 'kind':'skill','table':'skills#group1','index':7,'source':'data/scripts/BashDown.lua'},
            {'id':'table:item:potion', 'name':'Potion0', 'category':'Items', 'kind':'item','table':'loot_table#group3','index':925},
            {'id':'table:sound:517', 'name':'Sound517', 'category':'Sounds', 'kind':'sound','index':517},
            {'id':'table:effect:synthetic', 'name':'AAA_DONT_DELETE', 'category':'Effects', 'kind':'effect_set','table':'effect_sets','index':0},
            {'id':'localization:mudlord','name':'Mud Lord','category':'Text','kind':'localization-row'},
            {'id':'reference:abc', 'name':'Mud Lord authored reference', 'category':'References', 'kind':'reference'},
            {'id':'asset:abc', 'name':'SwampKing.mdl', 'category':'Assets', 'kind':'source_asset',
             'aliases':['3d/SwampKing.mdl'], 'path':'Data/3D/SwampKing.mdl',
             'logical_paths':['q:/data/iphone/data/3d/SwampKing.mdl']},
            {'id':'script:script1', 'name':'SwampKing.lua', 'category':'Scripts', 'kind':'script'}],
            'edges':[{'id':'e1','from':'table:actor:swamp','to':'reference:abc','kind':'reference_site','status':'RESOLVED'},
                     {'id':'e2','from':'reference:abc','to':'table:skill:bash','kind':'skill_ref','status':'INFERENCE'},
                     {'id':'e3','from':'asset:abc','to':'table:actor:swamp','kind':'reverse_fixture'},
                     {'id':'e5','from':'script:script1','to':'asset:abc','kind':'source_file'}],
            'profiles':{'table:actor:swamp':{'coverage':'partial','assets':['asset:abc']}}}
        graph_path.write_text(json.dumps(first, ensure_ascii=False, indent=2), encoding='utf-8')
        inputs = root / 'inputs'
        ctx = SimpleNamespace(out=out, inputs=inputs)
        cache = root / 'cache'
        store = GraphStore.open(ctx, cache_dir=cache)
        search = store.search('Mud Lord', offset=0, limit=1)
        tests.append({'name':'gameplay entity ranks ahead of exact incidental text and page bounds hold','passed':search['total']==2 and
                      search['results'][0]['id']=='table:actor:swamp' and search['next_offset']==1})
        text_only=store.search('Mud Lord',category='Text',limit=5)['results']
        tests.append({'name':'explicit category query keeps its category-specific result ordering','passed':
                      bool(text_only) and text_only[0]['id']=='localization:mudlord'})
        typed = [('actor:SwampKing','table:actor:swamp'),('actor:Mud Lord','table:actor:swamp'),
                 ('skill:BashDown','table:skill:bash'),('item:Potion0','table:item:potion'),
                 ('sound:517','table:sound:517')]
        typed_ok = all((lambda rows, expected: bool(rows) and rows[0]['id']==expected)(
            store.search(query,limit=5)['results'], expected) for query,expected in typed)
        tests.append({'name':'known typed prefixes search names, aliases, and exact numeric indices','passed':typed_ok})
        paths_ok = all(store.search(query,limit=5)['total'] >= 1 and
                       any(r['id']=='asset:abc' for r in store.search(query,limit=5)['results'])
                       for query in ('3d/SwampKing.mdl','data/3d/swampking.mdl',
                                     'q:/data/iphone/data/3d/swampking.mdl'))
        tests.append({'name':'Android aliases, logical URI, case, and normalized path variants search','passed':paths_ok})
        field_search = store.search('Data/Actors/SwampKing.lua',limit=5)['results']
        tests.append({'name':'physical site/source paths and graph row IDs are indexed','passed':
                      any(r['id']=='table:actor:swamp' for r in field_search) and
                      any(r['id']=='table:skill:bash' for r in store.search('data/scripts/BashDown.lua',limit=5)['results']) and
                      any(r['id']=='table:actor:swamp' for r in store.search('table:actor:swamp',limit=5)['results'])})
        tests.append({'name':'unknown q prefix remains a URI path, not a kind filter','passed':
                      any(r['id']=='asset:abc' for r in store.search('q:/data/iphone/data/3d/swampking.mdl',limit=5)['results'])})
        default = store.search('', offset=0, limit=100)
        refs = store.search('', category='References', offset=0, limit=100)
        tests.append({'name':'raw references stay opt-in and remain searchable','passed':
                      all(r['category']!='References' for r in default['results']) and refs['total']==1})
        entity = store.entity('table:actor:swamp')
        ids = {n['id'] for n in entity['nodes']}
        tests.append({'name':'one reference hop retains both edge directions and evidence','passed':
                      {'table:actor:swamp','reference:abc','table:skill:bash','asset:abc'} <= ids and
                      {e['id'] for e in entity['edges']} >= {'e1','e2','e3'} and
                      entity['incoming'][0]['from']=='asset:abc' and entity['outgoing'][0]['to']=='reference:abc'})
        closed = store.closure('table:actor:swamp','in')
        tests.append({'name':'reverse closure preserves original edge orientation','passed':
                      any(e['from']=='asset:abc' and e['to']=='table:actor:swamp' for e in closed['edges'])})
        invalid = False
        try: store.entity('../../AGENTS.md')
        except ValueError: invalid = True
        tests.append({'name':'entity IDs reject path-shaped input','passed':invalid})
        old_db = store.db_path
        store.db.close()
        first['nodes'].append({'id':'table:item:potion2', 'name':'Potion1', 'category':'Items', 'kind':'item'})
        first['edges'].append({'id':'e4','from':'table:item:potion','to':'table:actor:swamp','kind':'owner'})
        graph_path.write_text(json.dumps(first, ensure_ascii=False, indent=2), encoding='utf-8')
        rebuilt = GraphStore.open(ctx, cache_dir=cache)
        tests.append({'name':'changed source generation rebuilds a new cache','passed':
                      rebuilt.db_path != old_db and rebuilt.counts['nodes']==10 and
                      rebuilt.entity('table:item:potion2') is not None})
        before_native = rebuilt.db_path
        rebuilt.db.close()
        native_path = data_dir / 'entity-native.json'
        resource_path = data_dir / 'resource-attributions.json'
        native = {'entities_by_id': {'actor:swamp': {
            'id':'actor:swamp','kind':'actor','source':{'table':'character_properties#group0'},
            'script_ids':['script1'],'call_ids':['call1'],'native_function_ids':['fn1','fn2'],
            'script_links':[{'script_id':'script1','status':'RESOLVED'}],
            'association_note':'fixture source association','unresolved_script_links':[],'notes':[]},
            'entity:skills#group1:7': {'id':'entity:skills#group1:7','kind':'skill',
                'source':{'table':'skills#group1','index':7,'row_id':'skill-row-hash'},
                'script_ids':[],'native_function_ids':[],'script_links':[]},
            'entity:loot_table#group3:925': {'id':'entity:loot_table#group3:925','kind':'item',
                'source':{'table':'loot_table#group3','index':925,'row_id':'item-row-hash'},
                'script_ids':[],'native_function_ids':[],'script_links':[]},
            'entity:effect_sets:0': {'id':'entity:effect_sets:0','kind':'effect_set',
                'source':{'table':'effect_sets','index':0,'row_id':'effect-row-hash'},
                'script_ids':[],'native_function_ids':[],'script_links':[]}},
            'scripts_by_id': {'script1': {'id':'script1','name':'SwampKing.lua','site':'Data/Scripts/SwampKing.lua',
                                          'status':'SOURCE_DECODED','call_ids':['call1'],'function_ids':['fn1']}},
            'functions_by_id': {'fn1': {'id':'fn1','name':'_DoSkill','native_contract_status':'PROVEN',
            'port_status':'UNIMPLEMENTED','native_addresses':[{'address':'0x1234','pseudocode_path':'.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/../../outside.c',
                                                                     'source_line':9,'snippet':'void _DoSkill();'}]},
                'fn2':{'id':'fn2','name':'_PlaySound','native_addresses':[{'address':'0x5678','pseudocode_path':'.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/5678/00005678.c',
                                                                          'source_line':12,'snippet':'void _PlaySound();'}]}}}
        pseudocode = inputs / 'ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/5678/00005678.c'
        pseudocode.parent.mkdir(parents=True,exist_ok=True)
        pseudocode.write_text('void _PlaySound() { return; }\n',encoding='utf-8')
        native_path.write_text(json.dumps(native,ensure_ascii=False,indent=2),encoding='utf-8')
        resource={'resource_profiles_by_id':{'actor:swamp':{'id':'actor:swamp',
                    'association_status':'SOURCE_LINKED','owners':[{'id':'table:actor:swamp'}]},
                    'entity:skills#group1:7':{'id':'entity:skills#group1:7','association_status':'SKILL_SOURCE_TABLE'},
                    'entity:loot_table#group3:925':{'id':'entity:loot_table#group3:925','association_status':'ITEM_SOURCE_TABLE'},
                    'entity:effect_sets:0':{'id':'entity:effect_sets:0','association_status':'EFFECT_SOURCE_TABLE'}},
                  'resource_files_by_id':{}}
        resource_path.write_text(json.dumps(resource,ensure_ascii=False,indent=2),encoding='utf-8')
        native_store=GraphStore.open(ctx,cache_dir=cache)
        packet=native_store.entity('table:actor:swamp')
        native_function=(packet.get('entity_native') or {}).get('functions',[])
        tests.append({'name':'native evidence is joined by typed entity and preserves address provenance','passed':
                      native_store.db_path!=before_native and packet.get('entity_native',{}).get('script_ids')==['script1'] and
                      bool(native_function) and native_function[0]['native_addresses'][0]['address']=='0x1234' and
                      native_function[0]['native_addresses'][0]['pseudocode_path'].endswith('pseudocode/../../outside.c') and
                      native_store.entity('table:actor:swamp').get('resource_attribution',{}).get('record',{}).get('association_status')=='SOURCE_LINKED' and
                      packet['entity_native']['scripts'][0]['source_url']=='/api/source/abc'})
        canonical_packets=[('table:skill:bash','entity:skills#group1:7'),
                           ('table:item:potion','entity:loot_table#group3:925'),
                           ('table:effect:synthetic','entity:effect_sets:0')]
        canonical_ok=True
        for graph_id,canonical_id in canonical_packets:
            joined=native_store.entity(graph_id) or {}
            canonical_ok=canonical_ok and joined.get('entity_native',{}).get('id')==canonical_id and \
                joined.get('resource_attribution',{}).get('record',{}).get('id')==canonical_id
        tests.append({'name':'table hash nodes join canonical table-index native and resource records','passed':canonical_ok})
        native_store.db.close()
        native['functions_by_id']['fn1']['native_addresses'][0]['address']='0x5678'
        native_path.write_text(json.dumps(native,ensure_ascii=False,indent=2),encoding='utf-8')
        changed_native=GraphStore.open(ctx,cache_dir=cache)
        tests.append({'name':'changed native evidence invalidates the combined cache signature','passed':
                      changed_native.db_path!=before_native and
                      changed_native.entity('table:actor:swamp')['entity_native']['functions'][0]['native_addresses'][0]['address']=='0x5678'})
        native_source=changed_native.native_source('fn1',0)
        tests.append({'name':'native source preview only returns indexed address and excerpt','passed':
                      native_source['address']=='0x5678' and native_source['source_line']==9 and 'void _DoSkill' in native_source['snippet']})
        invalid_native=False
        try:changed_native.native_source('../../outside',0)
        except ValueError:invalid_native=True
        tests.append({'name':'native source IDs reject path-shaped input','passed':invalid_native})
        paged=changed_native.native_page('table:actor:swamp',function_offset=1,limit=1)
        tests.append({'name':'native function paging exposes later functions','passed':
                      paged['function_total']==2 and paged['functions'][0]['id']=='fn2' and paged['next_function_offset'] is None})
        before_resource=changed_native.db_path
        resource['resource_profiles_by_id']['actor:swamp']['association_status']='UPDATED_SOURCE_LINK'
        resource_path.write_text(json.dumps(resource,ensure_ascii=False,indent=2),encoding='utf-8')
        changed_native.db.close()
        changed_resource=GraphStore.open(ctx,cache_dir=cache)
        tests.append({'name':'changed resource attribution invalidates combined cache signature','passed':
                      changed_resource.db_path!=before_resource and
                      changed_resource.entity('table:actor:swamp')['resource_attribution']['record']['association_status']=='UPDATED_SOURCE_LINK'})
        server=ThreadingHTTPServer(('127.0.0.1',0),lambda *a,**kw: Handler(*a,ctx=ctx,assets={},graph=changed_resource,**kw))
        thread=Thread(target=server.serve_forever,daemon=True);thread.start()
        base=f'http://127.0.0.1:{server.server_port}'
        try:
            status,_,body,_=_request(base,'/api/native?id=table%3Aactor%3Aswamp&function_offset=1&limit=1')
            native_packet=json.loads(body) if status==200 else {}
            tests.append({'name':'HTTP native endpoint returns bounded continuation page','passed':
                          status==200 and native_packet['function_total']==2 and native_packet['functions'][0]['id']=='fn2'})
            status,_,body,_=_request(base,'/api/native-source?function_id=fn2&address_index=0')
            source_packet=json.loads(body) if status==200 else {}
            tests.append({'name':'HTTP native source endpoint serves its indexed excerpt','passed':
                          status==200 and source_packet['address']=='0x5678' and source_packet['source_line']==12})
            status,headers,body,_=_request(base,'/api/native-source?function_id=fn2&address_index=0&full=1')
            tests.append({'name':'HTTP full native source streams the indexed individual file','passed':
                          status==200 and headers.get_content_type()=='text/plain' and
                          body.replace(b'\r\n',b'\n')==b'void _PlaySound() { return; }\n'})
            status,_,body,_=_request(base,'/api/native-source?function_id=fn2&address_index=0&path=../../AGENTS.md&full=1')
            tests.append({'name':'native source ignores caller-supplied paths and uses only indexed catalog paths','passed':
                          status==200 and b'void _PlaySound()' in body and b'AGENTS.md' not in body})
            status,headers,body,_=_request(base,'/api/native-source?function_id=fn1&address_index=0&full=1')
            tests.append({'name':'full native source rejects indexed path traversal with excerpt fallback','passed':
                          status==404 and headers.get_content_type()=='text/plain' and
                          b'outside the approved' in body and b'void _DoSkill' in body})
            status,_,_,_=_request(base,'/api/native-source?function_id=fn2&address_index=0&full=1','POST')
            tests.append({'name':'native source route remains read-only','passed':status==405})
        finally:
            server.shutdown();server.server_close();changed_resource.db.close()
    return tests


def run(ctx):
    tests = _fixture_checks()
    cold_started = time.perf_counter()
    store = GraphStore.open(ctx)
    cold_ms = (time.perf_counter() - cold_started) * 1000
    warm_started = time.perf_counter()
    warm = GraphStore.open(ctx)
    warm_ms = (time.perf_counter() - warm_started) * 1000
    graph = {'counts':store.counts, 'data_version':store.generation['sha256']}
    assets = []
    assets_path = Path(ctx.out) / 'data' / 'assets.json'
    if assets_path.is_file():
        try: assets = json.loads(assets_path.read_text(encoding='utf-8'))
        except (OSError, ValueError): assets = []
    server = ThreadingHTTPServer(('127.0.0.1',0), lambda *a,**kw: Handler(*a,ctx=ctx,
                                assets={x['id']:x for x in assets},graph=warm,**kw))
    thread = Thread(target=server.serve_forever,daemon=True); thread.start()
    base = f'http://127.0.0.1:{server.server_port}'
    api = {}
    try:
        status, headers, body, elapsed = _request(base,'/api/meta')
        api['meta']={'status':status,'ms':round(elapsed,2),'bytes':len(body)}
        tests.append({'name':'meta endpoint returns bounded counts','passed':status==200 and
                      len(body)<32_000 and json.loads(body)['counts']['total']==store.counts['nodes']})
        status, headers, body, elapsed = _request(base,'/')
        api['fast_page']={'status':status,'ms':round(elapsed,2),'bytes':len(body)}
        tests.append({'name':'HTTP root serves compact template without embedded graph','passed':
                      status==200 and len(body)<100_000 and b'__ATLAS_GZIP_BASE64__' not in body})
        status, _, body2, _ = _request(base,'/index.html')
        tests.append({'name':'HTTP index.html also serves compact API template','passed':
                      status==200 and len(body2)<100_000})
        found = warm.search('Mud Lord',limit=5)['results']
        mud = next((n for n in found if n['name'].casefold()=='swampking'),None)
        if mud is None:
            found = warm.search('SwampKing',category='Actors',limit=5)['results']
            mud = next((n for n in found if n['name'].casefold()=='swampking'),found[0] if found else None)
        if mud:
            qid = __import__('urllib.parse',fromlist=['quote']).quote(mud['id'],safe='')
            status, headers, body, elapsed = _request(base,'/api/entity?id='+qid)
            api['entity_mud_lord']={'status':status,'ms':round(elapsed,2),'bytes':len(body)}
            obj = json.loads(body) if status==200 else {}
            tests.append({'name':'Mud Lord actor packet has indexed endpoints and evidence','passed':
                          status==200 and obj.get('node',{}).get('id')==mud['id'] and
                          isinstance(obj.get('nodes'),list) and isinstance(obj.get('edges'),list) and
                          len(body)<12_000_000})
        else:
            tests.append({'name':'Mud Lord actor packet has indexed endpoints and evidence','passed':False,
                          'detail':'Actor search result missing'})
        other = None
        for query,category in [('BashDown','Skills'),('Potion0','Items')]:
            rows = warm.search(query,category=category,limit=5)['results']
            row = next((n for n in rows if n['name'].casefold()==query.casefold()),rows[0] if rows else None)
            if row:
                other = row
                status, _, body, elapsed = _request(base,'/api/entity?id='+__import__('urllib.parse',fromlist=['quote']).quote(row['id'],safe=''))
                api['entity_'+query.lower()]={'status':status,'ms':round(elapsed,2),'bytes':len(body)}
                tests.append({'name':query+' entity is queryable through the production handler','passed':
                              status==200 and json.loads(body).get('node',{}).get('id')==row['id']})
                break
        if other is None:
            tests.append({'name':'a second typed entity is queryable','passed':False})
        join_cases=[('BashDown','Skills','native','entity:skills#group1:7'),
                    ('Bow12','Items','native','entity:loot_table#group3:127'),
                    ('AAA_DONT_DELETE','Animations','resource','entity:animation_sequences:0'),
                    ('AAA_DONT_DELETE','Effects','resource','entity:effect_sets:0')]
        live_join_results=[]
        for name,category,kind,expected_id in join_cases:
            results=warm.search(name,category=category,limit=10)['results']
            match=next((r for r in results if r['name']==name),None)
            packet=warm.entity(match['id']) if match else None
            actual=((packet or {}).get('entity_native') or {}).get('id') if kind=='native' else \
                (((packet or {}).get('resource_attribution') or {}).get('record') or {}).get('id')
            live_join_results.append((name,kind,actual==expected_id))
        tests.extend({'name':f'live {category} {name} table-index {kind} join appears in entity packet','passed':passed}
                     for (name,category,kind,_),(_,_,passed) in zip(join_cases,live_join_results))
        bash_rows=warm.search('BashDown',category='Skills',limit=5)['results']
        bash_packet=warm.entity(bash_rows[0]['id']) if bash_rows else None
        false_gap=any(gap.get('table')=='CharacterProperties' and gap.get('index')==172
                      for gap in ((bash_packet or {}).get('profile') or {}).get('gaps',[]))
        tests.append({'name':'final BashDown profile omits the struct-field false missing-row gap','passed':not false_gap})
        bad_status,_,_,_ = _request(base,'/api/entity?id=..%2F..%2FAGENTS.md')
        tests.append({'name':'HTTP entity route rejects path-shaped IDs','passed':bad_status==400})
        status, _, body, elapsed = _request(base,'/api/search?q=Mud%20Lord&limit=5')
        api['search_alias']={'status':status,'ms':round(elapsed,2),'bytes':len(body)}
        tests.append({'name':'HTTP search ranks alias result','passed':status==200 and
                      any(r['name'].casefold()=='swampking' for r in json.loads(body)['results'])})
        if mud:
            native_path='/api/native?id='+__import__('urllib.parse',fromlist=['quote']).quote(mud['id'],safe='')+'&function_offset=0&limit=8'
            status,_,body,elapsed=_request(base,native_path)
            api['native_mud_lord']={'status':status,'ms':round(elapsed,2),'bytes':len(body)}
            native_packet=json.loads(body) if status==200 else {}
            native_present=Path(ctx.out,'data','entity-native.json').is_file()
            tests.append({'name':'native endpoint returns bounded entity function page','passed':
                          status==200 and len(native_packet.get('functions',[]))<=8 and
                          (not native_present or native_packet.get('function_total',0)>0)})
        bad_native_status,_,_,_=_request(base,'/api/native-source?function_id=..%2F..%2FAGENTS.md')
        tests.append({'name':'native source HTTP route rejects path-shaped function IDs','passed':bad_native_status==400})
        status,_,body,_ = _request(base,'/api/closure?id='+__import__('urllib.parse',fromlist=['quote']).quote(mud['id'],safe='')+'&direction=in') if mud else (0,None,b'',0)
        tests.append({'name':'HTTP reverse closure stays bounded and keeps edge orientation','passed':
                      status==200 and json.loads(body)['direction']=='in' and len(json.loads(body)['nodes'])<=5000})
        post_status,_,_,_ = _request(base,'/api/meta','POST')
        tests.append({'name':'viewer rejects API writes','passed':post_status==405})
        protocol = server_checks.run(ctx,base)
        tests.extend({'name':'existing source/document protocol: '+t['name'],'passed':t['passed']}
                     for t in protocol['tests'])
    finally:
        server.shutdown();server.server_close();warm.db.close();store.db.close()
    return {'passed':all(t['passed'] for t in tests), 'tests':tests,
            'measurements':{'graph_nodes':graph['counts'].get('nodes'),'graph_edges':graph['counts'].get('edges'),
                'cold_cache_open_ms':round(cold_ms,2),'warm_cache_open_ms':round(warm_ms,2),
                'warm_api':api,'sqlite_cache_bytes':store.db_path.stat().st_size},
            'scope':'Graph API, cache generation, bounded packets, local read-only protocol. Does not verify browser rendering.'}
