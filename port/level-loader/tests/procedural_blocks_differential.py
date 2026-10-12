"""Compare typed MGX projection and retained source plans with actual ARM receipts.

All authored MGX files, original rejection of each link declaration, default
and authored unit dimensions, accepted exit ordering, direction, coordinate
bits and link-type bytes are compared. Includes source ownership, failed
publication and explicit rejection of original unsafe arithmetic/traversal.
This does not execute connection graph construction or generated layouts.
"""
import argparse,collections,hashlib,json,os,pathlib,subprocess,zipfile
ap=argparse.ArgumentParser()
for name in ('original','probe','cache','inventory','out'):ap.add_argument('--'+name,type=pathlib.Path,required=True)
ap.add_argument('--sanitizers',action='store_true');a=ap.parse_args()
root=pathlib.Path(__file__).resolve().parents[1]
original=json.loads(a.original.read_text());inventory=json.loads(a.inventory.read_text())
assert original['original_execution_complete']
assert hashlib.sha256((root/'tests/procedural_blocks_original.py').read_bytes()).hexdigest()==original['script_sha256']
for name,expected in original['dependency_sources_sha256'].items():
    assert hashlib.sha256((root.parents[1]/name).read_bytes()).hexdigest()==expected,name
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
def compare(expected,actual):
    assert 'error' not in actual,(expected['name'],actual)
    for key in ('load_result','unit_bits','size','exits'):assert expected[key]==actual[key],(expected['name'],key,expected[key],actual[key])
    declarations=[{'source_child':d['source_child'],'accepted':d['result']=='accepted'} for d in actual['link_declarations']]
    assert declarations==expected['link_declarations_original'],(expected['name'],'link rejections',declarations,expected['link_declarations_original'])
    return {'name':expected['name'],'exit_count':len(actual['exits']),'link_declarations':len(declarations),
        'rejected_links':sum(not d['accepted'] for d in declarations),'matches':True}
prefix='com.gameloft.android.GAND.GloftD2SS/files/'
with zipfile.ZipFile(a.cache) as pack:
    inputs=bytearray()
    for row in original['cases']:
        raw=bytes.fromhex(row['input_hex']) if 'input_hex' in row else pack.read(prefix+row['name'])
        assert hashlib.sha256(raw).hexdigest()==row['input_sha256']
        inputs.extend(str(len(raw)).encode()+b'\n'+raw)
actual=run(raw=bytes(inputs));assert len(actual)==len(original['cases'])
cases=[compare(e,n) for e,n in zip(original['cases'],actual)]
unsafe=[b'<Module unit_width="0"><GameObject gametype="link" linktype="gate" direction="north" position="0,0,0"/></Module>',
    b'<Module block_width="2147483648"/>',b'<Module><GameObject gametype="visual"/><!-- unsafe sibling --><GameObject gametype="link"/></Module>',
    b'<Module><GameObject gametype="link" linktype="gate" direction="north"/></Module>',
    b'<Module>'+b'<GameObject gametype="link" linktype="gate" direction="north" position="0,0,0"/>'*9+b'</Module>',
    b'<Module><GameObject gametype="link" linktype="gate" direction="north" position="1e39,0,0"/></Module>']
rejections=run(raw=b''.join(str(len(v)).encode()+b'\n'+v for v in unsafe));assert all('error' in r for r in rejections)
by_uri={r['name']:r for r in original['cases'] if r['name'].startswith('data/')}
levels=[];occurrences=exits=rejected=0
for level in inventory['levels']:
    row={k:level[k] for k in ('name','file','random')}
    if not level['file'].endswith('.rule.xml'):
        row['typed_blocks']='fixed_definition';levels.append(row);continue
    native=run((cache,level['name'],level['file']))[0]
    assert native['identity']==level['name'] and native['ownership_checks']
    for block in native['blocks']:
        comparison=compare(by_uri[block['uri']],block['projection'])
        occurrences+=1;exits+=comparison['exit_count'];rejected+=comparison['rejected_links']
    row.update(typed_blocks='original_compared',blocks=len(native['blocks']))
    levels.append(row)
paths=[root/'procedural_blocks_v1.cpp',root/'procedural_blocks_v1.hpp',root/'xml_document_v1.cpp',root/'xml_document_v1.hpp',
    root/'procedural_sources_v1.cpp',root/'procedural_sources_v1.hpp',root/'procedural_file_list_v1.cpp',root/'procedural_file_list_v1.hpp',
    root/'tests/procedural_blocks_probe.cpp',root/'CMakeLists.txt',pathlib.Path(__file__)]
report={'validation':'PASS','scope':__doc__,'cache_sha256':original['cache_sha256'],'engine_sha256':original['engine_sha256'],
    'original_receipt_sha256':hashlib.sha256(a.original.read_bytes()).hexdigest(),'probe_sha256':hashlib.sha256(a.probe.read_bytes()).hexdigest(),
    'sources_sha256':{str(p.relative_to(root)):hashlib.sha256(p.read_bytes()).hexdigest() for p in paths},
    'sanitizers':a.sanitizers,'cases':cases,'explicit_domain_rejections':rejections,'levels':levels,
    'summary':dict(collections.Counter(r['typed_blocks'] for r in levels)),
    'authored_blocks_compared':len(by_uri),'block_occurrences_compared':occurrences,'exit_occurrences_compared':exits,
    'rejected_link_occurrences_compared':rejected,'connection_graph_verified':False,'layout_generation_verified':False,'full_loader_verified':False}
a.out.write_text(json.dumps(report,indent=2)+'\n')
print(json.dumps({k:report[k] for k in ('validation','summary','authored_blocks_compared','block_occurrences_compared','exit_occurrences_compared','rejected_link_occurrences_compared','sanitizers')}))
