"""Compare initial runtime rule state, owned sources, and exact random effects.

Each constructor runs in a fixture preorder, with/without a parent and five
boundary seeds. This does not establish actual layout traversal order, parent
graph registration, room-pool random allocation or placement/serialization.
Original Level caller control-flow evidence is retained separately.
"""
import argparse,collections,hashlib,json,os,pathlib,subprocess,zipfile
ap=argparse.ArgumentParser()
for name in ('original','probe','cache','inventory','out'):ap.add_argument('--'+name,type=pathlib.Path,required=True)
ap.add_argument('--sanitizers',action='store_true');a=ap.parse_args()
root=pathlib.Path(__file__).resolve().parents[1];repo=root.parents[1]
original=json.loads(a.original.read_text());inventory=json.loads(a.inventory.read_text())
assert original['original_execution_complete'] and len(original['cases'])==50 and len(original['caller_cases'])==32
assert hashlib.sha256((root/'tests/procedural_instances_original.py').read_bytes()).hexdigest()==original['script_sha256']
for name,want in original['dependency_sources_sha256'].items():assert hashlib.sha256((repo/name).read_bytes()).hexdigest()==want,name
assert hashlib.sha256(a.cache.read_bytes()).hexdigest()==original['cache_sha256']==inventory['cache_sha256']
assert original['engine_sha256']=='36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80'
assert hashlib.sha256((root/'reports/procedural-rules-original.json').read_bytes()).hexdigest()==original['rules_receipt_sha256']
def wslpath(p):
    p=p.resolve();return '/mnt/'+p.drive[0].lower()+p.as_posix()[2:]
command=[str(a.probe.resolve())];cache=str(a.cache.resolve())
if os.name=='nt':command=['wsl','-d','Ubuntu','--',wslpath(a.probe)];cache=wslpath(a.cache)
if a.sanitizers:
    env=['env','ASAN_OPTIONS=detect_leaks=1:halt_on_error=1','UBSAN_OPTIONS=halt_on_error=1']
    command=command[:4]+env+command[4:] if os.name=='nt' else env+command
def run(args=(),raw=None):
    result=subprocess.run(command+list(args),input=raw,capture_output=True)
    if result.returncode:raise RuntimeError(result.stderr.decode())
    return [json.loads(line) for line in result.stdout.decode().splitlines()]
def blob(raw):return str(len(raw)).encode()+b'\n'+raw
def group(names,raw):return str(len(names)).encode()+b'\n'+b''.join(blob(name.encode()) for name in names)+blob(raw)
def compare(expected,actual):
    assert expected['reader_result']==actual['reader_result'] and expected['rule_nodes']==actual['rule_nodes'],expected['name']
    assert len(expected['runs'])==len(actual['runs'])==5
    count=calls=0;lengths=collections.Counter()
    for e_run,n_run in zip(expected['runs'],actual['runs']):
        assert e_run['seed']==n_run['seed'] and e_run['final_state']==n_run['final_state'],(expected['name'],e_run['seed'])
        assert len(e_run['instances'])==len(n_run['instances'])
        for e_row,n_row in zip(e_run['instances'],n_run['instances']):
            want={key:value for key,value in e_row.items() if key!='random_calls'}
            assert want==n_row,(expected['name'],e_run['seed'],e_row,n_row)
            calls+=e_row['random_calls'];count+=1;lengths[str(n_row['length'])]+=1
    return {'name':expected['name'],'reader_result':actual['reader_result'],'rule_nodes':actual['rule_nodes'],
            'instances_compared':count,'original_random_calls':calls,'lengths':dict(lengths),'matches':True}
inputs=bytearray();prefix='com.gameloft.android.GAND.GloftD2SS/files/'
with zipfile.ZipFile(a.cache) as pack:
    for row in original['cases']:
        raw=bytes.fromhex(row['input_hex']) if 'input_hex' in row else pack.read(prefix+row['name'])
        assert hashlib.sha256(raw).hexdigest()==row['input_sha256']
        inputs.extend(group(row['block_names'],raw))
actual=run(raw=bytes(inputs));assert len(actual)==len(original['cases'])
cases=[compare(expected,result) for expected,result in zip(original['cases'],actual)]
by_definition={r['name']:r for r in original['cases'] if r['name'].startswith('data/')}
levels=[];totals=collections.Counter()
for level in inventory['levels']:
    row={key:level[key] for key in ('name','file','random')}
    if not level['file'].endswith('.rule.xml'):
        row['instances']='fixed_definition';levels.append(row);continue
    native=run((cache,level['name'],level['file']))[0]
    assert native['identity']==level['name'] and native['ownership_checks']
    definition='data/scene/'+level['file'] if '/' not in level['file'] else level['file']
    result=compare(by_definition[definition.lower().replace('\\','/')],native['plan'])
    row.update(instances='original_compared',reader_result=result['reader_result'],rule_nodes=result['rule_nodes'],
               instances_compared=result['instances_compared'],original_random_calls=result['original_random_calls'],ownership_checks=True)
    for key in ('rule_nodes','instances_compared','original_random_calls'):totals[key]+=result[key]
    levels.append(row)
paths=[root/(name+'.'+suffix) for name in ('procedural_instances_v1','procedural_rules_v1','procedural_random_v1') for suffix in ('cpp','hpp')]
paths.extend((root/'tests/procedural_instances_probe.cpp',root/'CMakeLists.txt',pathlib.Path(__file__)))
report={'validation':'PASS','scope':__doc__,'cache_sha256':original['cache_sha256'],'engine_sha256':original['engine_sha256'],
        'original_receipt_sha256':hashlib.sha256(a.original.read_bytes()).hexdigest(),'probe_sha256':hashlib.sha256(a.probe.read_bytes()).hexdigest(),
        'sources_sha256':{str(p.relative_to(root)):hashlib.sha256(p.read_bytes()).hexdigest() for p in paths},
        'sanitizers':a.sanitizers,'cases':cases,'levels':levels,'authored_definitions_compared':len(by_definition),
        'summary':dict(collections.Counter(row['instances'] for row in levels)),'authored_totals':dict(totals),
        'caller_reader_failure_policy_verified':True,'caller_boundary_cases':len(original['caller_cases']),
        'pool_size_allocation_verified':False,'layout_generation_verified':False,'runtime_graph_registration_verified':False,
        'full_loader_verified':False}
a.out.write_text(json.dumps(report,indent=2)+'\n')
print(json.dumps({key:report[key] for key in ('validation','summary','authored_definitions_compared','authored_totals','caller_boundary_cases','sanitizers')}))
