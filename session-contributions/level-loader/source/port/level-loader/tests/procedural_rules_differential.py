"""Compare original nested-rule state and unallocated room-pool declarations.

All original rule fields, list/block validation and resolution, child order,
per-node return values and postorder FillSizes results are compared. The 35
authored rule definitions prepare after owner teardown without turning false
original reader results into clean success. Five unsafe original domains are
rejected explicitly. Pool random allocation and generated layouts do not run.
"""
import argparse,collections,hashlib,json,os,pathlib,subprocess,zipfile
ap=argparse.ArgumentParser()
for name in ('original','probe','cache','inventory','out'):ap.add_argument('--'+name,type=pathlib.Path,required=True)
ap.add_argument('--sanitizers',action='store_true');a=ap.parse_args()
root=pathlib.Path(__file__).resolve().parents[1];repo=root.parents[1]
original=json.loads(a.original.read_text());inventory=json.loads(a.inventory.read_text())
assert original['original_execution_complete'] and len(original['cases'])==54
assert original['original_static_tokens']=={'room_pool':'pool','pool_element':'elem'}
assert original['authored_rule_files_requested']==35 and original['synthetic_cases_requested']==19
assert hashlib.sha256((root/'tests/procedural_rules_original.py').read_bytes()).hexdigest()==original['script_sha256']
for name,expected in original['dependency_sources_sha256'].items():assert hashlib.sha256((repo/name).read_bytes()).hexdigest()==expected,name
assert hashlib.sha256(a.cache.read_bytes()).hexdigest()==original['cache_sha256']==inventory['cache_sha256']=='3fdf4e4c21d45a780a7c35fb4042abde0e88e76bf75416aad1f227481560b679'
assert original['engine_sha256']=='36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80'
def wslpath(p):
    p=p.resolve();return '/mnt/'+p.drive[0].lower()+p.as_posix()[2:]
cmd=[str(a.probe.resolve())];cache=str(a.cache.resolve())
if os.name=='nt':cmd=['wsl','-d','Ubuntu','--',wslpath(a.probe)];cache=wslpath(a.cache)
if a.sanitizers:
    prefix=['env','ASAN_OPTIONS=detect_leaks=1:halt_on_error=1','UBSAN_OPTIONS=halt_on_error=1']
    cmd=cmd[:4]+prefix+cmd[4:] if os.name=='nt' else prefix+cmd
def run(args=(),raw=None):
    p=subprocess.run(cmd+list(args),input=raw,capture_output=True)
    if p.returncode:raise RuntimeError(p.stderr.decode())
    return [json.loads(line) for line in p.stdout.decode().splitlines()]
def blob(raw):return str(len(raw)).encode()+b'\n'+raw
def group(names,raw):return str(len(names)).encode()+b'\n'+b''.join(blob(n.encode()) for n in names)+blob(raw)
def flatten(rule):
    return [rule]+[r for c in rule['children'] for r in flatten(c)]
def compare(expected,actual):
    if 'checked_domain_rejection' in expected:
        assert expected['checked_domain_rejection'] in actual['error'],(expected['name'],actual)
        return {'name':expected['name'],'original_guard_pc':expected['pc'],'checked_domain_rejection':actual['error'],'matches':True}
    assert 'error' not in actual,(expected['name'],actual)
    for key in ('root_present','read_result','rule','pools'):assert expected[key]==actual[key],(expected['name'],key,expected[key],actual[key])
    rules=flatten(actual['rule']) if actual['root_present'] else []
    return {'name':expected['name'],'original_xml_error':expected['xml_error'],'reader_result':actual['read_result'],
        'rules':len(rules),'rules_returning_false':sum(not r['read_result'] for r in rules),
        'unresolved_list_references':sum(r['list_validation'] is not None and r['list_source'] is None for r in rules),
        'unavailable_block_references':sum(not b for r in rules for b in r['block_validation']),
        'pools':len(actual['pools']),'pool_elements':sum(len(p['elements']) for p in actual['pools']),'matches':True}
inputs=bytearray();prefix='com.gameloft.android.GAND.GloftD2SS/files/'
with zipfile.ZipFile(a.cache) as pack:
    for case in original['cases']:
        raw=bytes.fromhex(case['input_hex']) if 'input_hex' in case else pack.read(prefix+case['name'])
        assert hashlib.sha256(raw).hexdigest()==case['input_sha256']
        inputs.extend(group(case['block_names'],raw))
unsafe=[('integer-overflow',b'<rules><RootRule name="room"><Path name="room" length="2147483648"/></RootRule></rules>','integer outside int32'),
    ('lookup-buffer-overflow',b'<rules><RootRule name="'+b'a'*512+b'"/></rules>','exceeds original 512-byte buffer'),
    ('non-element-later-child',b'<rules><RootRule name="room"><EndPath name="room"/><!-- comment --></RootRule></rules>','unfiltered rule child is non-element'),
    ('text-first-child',b'<rules><RootRule name="room">text<EndPath name="room"/></RootRule></rules>','unfiltered rule child is non-element')]
for _,raw,_ in unsafe:inputs.extend(group(['room'],raw))
actual=run(raw=bytes(inputs));assert len(actual)==len(original['cases'])+len(unsafe)
cases=[compare(e,n) for e,n in zip(original['cases'],actual)]
rejections=[]
for (name,_,message),result in zip(unsafe,actual[len(cases):]):
    assert message in result['error'];rejections.append({'name':name,**result})
by_definition={r['name']:r for r in original['cases'] if r['name'].startswith('data/')}
levels=[];reader_results=collections.Counter();totals=collections.Counter()
for level in inventory['levels']:
    row={k:level[k] for k in ('name','file','random')}
    if not level['file'].endswith('.rule.xml'):
        row['rules']='fixed_definition';levels.append(row);continue
    native=run((cache,level['name'],level['file']))[0]
    assert native['identity']==level['name'] and native['ownership_checks']
    definition='data/scene/'+level['file'] if '/' not in level['file'] else level['file']
    c=compare(by_definition[definition.lower().replace('\\','/')],native['plan'])
    row.update(rules='original_compared',reader_result=c['reader_result'],rule_nodes=c['rules'],pools=c['pools'],pool_elements=c['pool_elements'])
    levels.append(row);reader_results[str(c['reader_result'])]+=1
    for key in ('rules','rules_returning_false','unresolved_list_references','unavailable_block_references','pools','pool_elements'):totals[key]+=c[key]
paths=[root/(n+'.'+suffix) for n in ('procedural_rules_v1','procedural_lists_v1','procedural_connections_v1','procedural_blocks_v1',
    'procedural_sources_v1','procedural_file_list_v1','procedural_random_v1','xml_document_v1') for suffix in ('cpp','hpp')]
paths.extend((root/'tests/procedural_rules_probe.cpp',root/'CMakeLists.txt',pathlib.Path(__file__)))
report={'validation':'PASS','scope':__doc__,'cache_sha256':original['cache_sha256'],'engine_sha256':original['engine_sha256'],
    'original_receipt_sha256':hashlib.sha256(a.original.read_bytes()).hexdigest(),'probe_sha256':hashlib.sha256(a.probe.read_bytes()).hexdigest(),
    'sources_sha256':{str(p.relative_to(root)):hashlib.sha256(p.read_bytes()).hexdigest() for p in paths},
    'sanitizers':a.sanitizers,'cases':cases,'additional_domain_rejections':rejections,'levels':levels,
    'summary':dict(collections.Counter(r['rules'] for r in levels)),'authored_rule_files_compared':len(by_definition),
    'original_reader_results':dict(reader_results),'authored_totals':dict(totals),'caller_reader_failure_policy_verified':False,
    'pool_size_allocation_verified':False,'layout_generation_verified':False,'full_loader_verified':False}
a.out.write_text(json.dumps(report,indent=2)+'\n')
print(json.dumps({k:report[k] for k in ('validation','summary','authored_rule_files_compared','original_reader_results','authored_totals','sanitizers')}))
