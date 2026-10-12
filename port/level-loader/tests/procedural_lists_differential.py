"""Compare typed list declarations and sorted selection with original ARM readers.

All declarations, including unselected duplicates, retained order/weights,
paths and original ValidBlock results are compared. Every procedural cache
level is prepared and checked after its owners and ZIP are destroyed. Rules,
weighted draws, room pools, layouts and gameplay are outside this check.
"""
import argparse,collections,hashlib,json,os,pathlib,subprocess,zipfile
ap=argparse.ArgumentParser()
for name in ('original','probe','cache','inventory','out'):ap.add_argument('--'+name,type=pathlib.Path,required=True)
ap.add_argument('--sanitizers',action='store_true');a=ap.parse_args()
root=pathlib.Path(__file__).resolve().parents[1];repo=root.parents[1]
original=json.loads(a.original.read_text());inventory=json.loads(a.inventory.read_text())
assert original['original_execution_complete'] and len(original['cases'])==44
assert hashlib.sha256((root/'tests/procedural_lists_original.py').read_bytes()).hexdigest()==original['script_sha256']
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
def compare(expected,actual):
    assert 'error' not in actual,(expected['name'],actual)
    assert expected['load_result']==1,(expected['name'],'Original LoadListRules result')
    for key in ('declarations','selected_sources','lists'):assert expected[key]==actual[key],(expected['name'],key,expected[key],actual[key])
    return {'name':expected['name'],'declarations':len(actual['declarations']),'selected_lists':len(actual['lists']),
        'elements':sum(len(d['elements']) for d in actual['declarations']),
        'unavailable_block_references':sum(not e['block_available'] for d in actual['declarations'] for e in d['elements']),
        'original_xml_error':expected['xml_error'],'matches':True}
inputs=bytearray();prefix='com.gameloft.android.GAND.GloftD2SS/files/'
with zipfile.ZipFile(a.cache) as pack:
    for case in original['cases']:
        raw=bytes.fromhex(case['input_hex']) if 'input_hex' in case else pack.read(prefix+case['name'])
        if 'input_sha256' in case:assert hashlib.sha256(raw).hexdigest()==case['input_sha256']
        inputs.extend(group(case['block_names'],raw))
unsafe=[('integer-overflow',b'<rules><list name="valid"><elem name="room"/></list><list name="bad"><elem name="room" chances="2147483648"/></list></rules>','chances outside int32'),
    ('unverified-non-ascii-lowercase','<rules><list name="valid"/><list name="\u00c9"/></rules>'.encode(),'lowercase outside verified ASCII'),
    ('block-validation-buffer-overflow',b'<rules><list name="valid"/><list name="bad"><elem name="'+b'a'*512+b'"/></list></rules>','exceeds original 512-byte buffer')]
for _,raw,_ in unsafe:inputs.extend(group(['room'],raw))
actual=run(raw=bytes(inputs));assert len(actual)==len(original['cases'])+len(unsafe)
cases=[compare(e,n) for e,n in zip(original['cases'],actual)]
rejections=[]
for (name,_,message),result in zip(unsafe,actual[len(cases):]):
    assert message in result['error'];rejections.append({'name':name,**result})
by_definition={r['name']:r for r in original['cases'] if r['name'].startswith('data/')}
levels=[];declarations=elements=unavailable=0
for level in inventory['levels']:
    row={k:level[k] for k in ('name','file','random')}
    if not level['file'].endswith('.rule.xml'):
        row['lists']='fixed_definition';levels.append(row);continue
    native=run((cache,level['name'],level['file']))[0]
    assert native['identity']==level['name'] and native['ownership_checks']
    definition='data/scene/'+level['file'] if '/' not in level['file'] else level['file']
    c=compare(by_definition[definition.lower().replace('\\','/')],native['plan'])
    row.update(lists='original_compared',declarations=c['declarations'],elements=c['elements'],unavailable_block_references=c['unavailable_block_references'])
    levels.append(row);declarations+=c['declarations'];elements+=c['elements'];unavailable+=c['unavailable_block_references']
paths=[root/(n+'.'+suffix) for n in ('procedural_lists_v1','procedural_connections_v1','procedural_blocks_v1','procedural_sources_v1','procedural_file_list_v1','xml_document_v1') for suffix in ('cpp','hpp')]
paths.extend((root/'tests/procedural_lists_probe.cpp',root/'CMakeLists.txt',pathlib.Path(__file__)))
report={'validation':'PASS','scope':__doc__,'cache_sha256':original['cache_sha256'],'engine_sha256':original['engine_sha256'],
    'original_receipt_sha256':hashlib.sha256(a.original.read_bytes()).hexdigest(),'probe_sha256':hashlib.sha256(a.probe.read_bytes()).hexdigest(),
    'sources_sha256':{str(p.relative_to(root)):hashlib.sha256(p.read_bytes()).hexdigest() for p in paths},
    'sanitizers':a.sanitizers,'cases':cases,'checked_domain_rejections':rejections,'levels':levels,
    'summary':dict(collections.Counter(r['lists'] for r in levels)),
    'authored_rule_files_compared':len(by_definition),'list_declaration_occurrences_compared':declarations,
    'list_element_occurrences_compared':elements,'unavailable_block_reference_occurrences':unavailable,
    'layout_generation_verified':False,'full_loader_verified':False}
a.out.write_text(json.dumps(report,indent=2)+'\n')
print(json.dumps({k:report[k] for k in ('validation','summary','authored_rule_files_compared','list_declaration_occurrences_compared','list_element_occurrences_compared','unavailable_block_reference_occurrences','sanitizers')}))
