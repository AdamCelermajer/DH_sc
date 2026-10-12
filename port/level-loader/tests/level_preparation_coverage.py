"""Compare staged source results with the frozen prior fixed/procedural milestone."""
import argparse, hashlib, json, pathlib, subprocess, zipfile
ap=argparse.ArgumentParser();ap.add_argument('--probe',required=True)
for name in ('cache','baseline','out'):ap.add_argument('--'+name,type=pathlib.Path,required=True)
a=ap.parse_args();root=pathlib.Path(__file__).resolve().parents[1]
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest()
with zipfile.ZipFile(a.baseline) as archive:
    manifest=json.loads(archive.read('HANDOFF-MANIFEST.json'))
    def receipt(name):
        key='port/level-loader/reports/'+name;raw=archive.read(key)
        assert hashlib.sha256(raw).hexdigest()==manifest['files_sha256'][key]
        return json.loads(raw)
    fixed=receipt('source-pipeline-fixed-maps-host.json')
    fixed_sources=receipt('source-pipeline-fixed-sources-host.json')
    fixed_declarations=receipt('source-pipeline-fixed-declarations-host.json')
    procedural=receipt('source-pipeline-procedural-maps-host.json')
with a.cache.open('rb') as stream:assert hashlib.file_digest(stream,'sha256').hexdigest()==fixed['cache_sha256']
decls={row['name']:row['occurrences'] for row in fixed_declarations['levels'] if row['declarations']=='source_compared'}
documents={row['name']:row['result']['documents'] for row in fixed_sources['levels'] if row['source_preparation']=='prepared'}
assert a.probe.startswith('/mnt/c/');probe=pathlib.Path('C:/'+a.probe[7:]);digest=sha(probe)
base=['wsl.exe','-d','Ubuntu','--','env','ASAN_OPTIONS=detect_leaks=1:halt_on_error=1','UBSAN_OPTIONS=halt_on_error=1',a.probe]
cache='/mnt/c/'+str(a.cache.resolve()).replace('\\','/')[3:];rows=[]
def check(identity,definition,kind,seed,status,expected,declarations=None):
    run=subprocess.run(base+[cache,identity,definition,kind,str(seed),'original'],capture_output=True,timeout=90)
    actual=None
    if status=='assembly_blocked':
        assert run.returncode!=0 and b'vm011_corner_voidmaze_ne_00_01.mgp' in run.stderr,run.stderr
    else:
        assert run.returncode==0 and not run.stderr,run.stderr
        actual=json.loads(run.stdout)
        assert actual['validation']==('NO_LAYOUT' if status=='original_no_layout' else 'PASS'),actual
        if status=='assembled':
            assert actual['module_count']==expected['module_count'],(identity,seed)
            assert actual['source_documents']==(documents[identity] if kind=='fixed' else expected['source_documents']),(identity,seed)
            assert actual['assets']==expected['asset_count' if kind=='fixed' else 'assets'],(identity,seed)
            assert actual['scene_instances']==sum(expected['geometry_kinds'].values()),(identity,seed)
            assert actual['declarations']==(declarations if declarations is not None else sum(expected['declaration_types'].values())),(identity,seed)
            assert actual['generated']==(kind=='procedural') and actual['stages']==(11 if kind=='procedural' else 4)
    rows.append({'identity':identity,'definition':definition,'kind':kind,'seed':seed,'status':status,'actual':actual})
for row in fixed['levels']:
    if row['map_preparation']=='assembled':check(row['name'],row['file'],'fixed',0,'assembled',row['result'],decls[row['name']])
for row in procedural['levels']:
    for run in row['runs']:
        check(row['identity'],row['definition'],'procedural',run['seed'],run['status'],run.get('result',{}))
    print(json.dumps({'identity':row['identity'],'runs_verified':2}),flush=True)
assert len(rows)==86 and sha(probe)==digest
paths=['level_preparation_v1.hpp','level_preparation_v1.cpp','CMakeLists.txt','tests/level_preparation_probe.cpp','tests/level_preparation_coverage.py']
report={'validation':'PASS','scope':__doc__,'source_sha256':{p:sha(root/p) for p in paths},'probe_sha256':digest,
        'baseline_archive_sha256':sha(a.baseline),'cache_sha256':fixed['cache_sha256'],'cases':rows,
        'fixed_cases':16,'procedural_cases':70,'runtime_objects_verified':False,'full_loader_verified':False}
a.out.write_text(json.dumps(report,indent=2)+'\n');print(json.dumps({'validation':'PASS','cases':len(rows)}))
