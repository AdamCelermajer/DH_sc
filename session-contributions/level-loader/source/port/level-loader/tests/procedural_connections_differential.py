"""Compare native ordered connection graphs with original ARM map/link receipts.

Compares sorted unique keys, first-duplicate selection and every ordered exit
reference, including repeated references, across all 21 file-listed block pools
and 35 procedural level rows. Checks retained ownership and transactional
rejection after partial connection construction. No layout generation runs.
"""
import argparse,collections,hashlib,json,os,pathlib,subprocess,zipfile
ap=argparse.ArgumentParser()
for name in ('original','probe','cache','inventory','out'):ap.add_argument('--'+name,type=pathlib.Path,required=True)
ap.add_argument('--sanitizers',action='store_true');a=ap.parse_args();root=pathlib.Path(__file__).resolve().parents[1];repo=root.parents[1]
original=json.loads(a.original.read_text());inventory=json.loads(a.inventory.read_text())
assert original['original_execution_complete'] and original['direction_opposites']==[2,3,0,1,4]
assert hashlib.sha256((root/'tests/procedural_connections_original.py').read_bytes()).hexdigest()==original['script_sha256']
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
def group(blocks):return str(len(blocks)).encode()+b'\n'+b''.join(blob(n.encode())+blob(uri.encode())+blob(raw) for n,uri,raw in blocks)
def compare(expected,actual):
    assert 'error' not in actual,(expected['name'],actual)
    for key in ('insertions','blocks'):assert expected[key]==actual[key],(expected['name'],key,expected[key],actual[key])
    connections=sum(len(e['connections']) for b in actual['blocks'] for e in b['exits'])
    duplicates=sum(len(e['connections'])-len({tuple(c) for c in e['connections']}) for b in actual['blocks'] for e in b['exits'])
    return {'name':expected['name'],'source_blocks':len(actual['insertions']),'selected_blocks':len(actual['blocks']),
        'connections':connections,'repeated_connection_references':duplicates,'matches':True}
inputs=bytearray();prefix='com.gameloft.android.GAND.GloftD2SS/files/'
with zipfile.ZipFile(a.cache) as pack:
    for case in original['cases']:
        blocks=[]
        for source in case['inputs']:
            raw=bytes.fromhex(source['input_hex']) if 'input_hex' in source else pack.read(prefix+source['uri'])
            if 'input_sha256' in source:assert hashlib.sha256(raw).hexdigest()==source['input_sha256']
            blocks.append((source['name'],source['uri'],raw))
        inputs.extend(group(blocks))
    # Exceeds the original fixed array after 64 real connections have been built.
    north=b'<Module><GameObject gametype="link" linktype="gate" direction="north" position="0,0,0"/></Module>'
    south=north.replace(b'north',b'south')
    unsafe=[('north','',north)]+[(f'south-{i:02d}','',south) for i in range(65)]
    inputs.extend(group(unsafe))
actual=run(raw=bytes(inputs));assert len(actual)==len(original['cases'])+1
assert '64-connection capacity exceeded' in actual[-1]['error']
cases=[compare(e,n) for e,n in zip(original['cases'],actual)]
by_folder={r['name']:r for r in original['cases'] if r['name'].startswith('data/')}
levels=[];connection_occurrences=selected_occurrences=0
for level in inventory['levels']:
    row={k:level[k] for k in ('name','file','random')}
    if not level['file'].endswith('.rule.xml'):
        row['connections']='fixed_definition';levels.append(row);continue
    native=run((cache,level['name'],level['file']))[0]
    assert native['identity']==level['name'] and native['ownership_checks']
    c=compare(by_folder[native['folder'].lower().replace('\\','/')],native['graph'])
    row.update(connections='original_compared',selected_blocks=c['selected_blocks'],connection_references=c['connections'])
    levels.append(row);connection_occurrences+=c['connections'];selected_occurrences+=c['selected_blocks']
paths=[root/'procedural_connections_v1.cpp',root/'procedural_connections_v1.hpp',root/'procedural_blocks_v1.cpp',root/'procedural_blocks_v1.hpp',
    root/'procedural_sources_v1.cpp',root/'procedural_sources_v1.hpp',root/'procedural_file_list_v1.cpp',root/'procedural_file_list_v1.hpp',
    root/'xml_document_v1.cpp',root/'xml_document_v1.hpp',root/'tests/procedural_connections_probe.cpp',root/'CMakeLists.txt',pathlib.Path(__file__)]
report={'validation':'PASS','scope':__doc__,'cache_sha256':original['cache_sha256'],'engine_sha256':original['engine_sha256'],
    'original_receipt_sha256':hashlib.sha256(a.original.read_bytes()).hexdigest(),'probe_sha256':hashlib.sha256(a.probe.read_bytes()).hexdigest(),
    'sources_sha256':{str(p.relative_to(root)):hashlib.sha256(p.read_bytes()).hexdigest() for p in paths},
    'sanitizers':a.sanitizers,'cases':cases,'explicit_capacity_rejection':actual[-1],
    'levels':levels,'summary':dict(collections.Counter(r['connections'] for r in levels)),
    'authored_block_pools_compared':len(by_folder),'selected_block_occurrences_compared':selected_occurrences,
    'connection_reference_occurrences_compared':connection_occurrences,'layout_generation_verified':False,'full_loader_verified':False}
a.out.write_text(json.dumps(report,indent=2)+'\n')
print(json.dumps({k:report[k] for k in ('validation','summary','authored_block_pools_compared','selected_block_occurrences_compared','connection_reference_occurrences_compared','sanitizers')}))
