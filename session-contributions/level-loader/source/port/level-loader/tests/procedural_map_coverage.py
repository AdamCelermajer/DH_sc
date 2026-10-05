"""Attempt generated geometry and retained declarations for all procedural rows."""
import argparse,collections,hashlib,json,pathlib,subprocess
def sha(path):
    with path.open('rb') as file:return hashlib.file_digest(file,'sha256').hexdigest()
def main():
    ap=argparse.ArgumentParser();ap.add_argument('--probe',required=True)
    for name in ('cache','inventory','out'):ap.add_argument('--'+name,type=pathlib.Path,required=True)
    a=ap.parse_args();inventory=json.loads(a.inventory.read_text())
    assert sha(a.cache)==inventory['cache_sha256']
    assert a.probe.startswith('/mnt/c/');probe=pathlib.Path('C:/'+a.probe[7:]);digest=sha(probe)
    command=['wsl.exe','-d','Ubuntu','--','env','ASAN_OPTIONS=detect_leaks=1:halt_on_error=1',
             'UBSAN_OPTIONS=halt_on_error=1',a.probe]
    cache='/mnt/c/'+str(a.cache).replace('\\','/')[3:];rows=[]
    for level in inventory['levels']:
        if not level['file'].endswith('.rule.xml'):continue
        row={'identity':level['name'],'definition':level['file'],'runs':[]}
        for seed in (0,1):
            proc=subprocess.run(command+[cache,level['name'],level['file'],str(seed)],capture_output=True,timeout=90)
            result={'seed':seed,'map_render_verified':False,'runtime_objects_verified':False}
            if proc.returncode:
                result.update(status='assembly_blocked',reason=proc.stderr.decode().strip())
            else:
                assert not proc.stderr,(level['name'],seed,proc.stderr.decode())
                actual=json.loads(proc.stdout)
                result.update(status='assembled' if actual['validation']=='PASS' else 'original_no_layout',result=actual)
            row['runs'].append(result)
        rows.append(row)
        print(json.dumps({'identity':row['identity'],'status':[r['status'] for r in row['runs']]}),flush=True)
    assert len(rows)==35 and sha(probe)==digest
    summary=dict(collections.Counter(run['status'] for row in rows for run in row['runs']))
    report={'scope':__doc__,'cache_sha256':sha(a.cache),'inventory_sha256':sha(a.inventory),
            'probe_sha256':digest,'probe':a.probe,'script_sha256':sha(pathlib.Path(__file__)),
            'levels':rows,'summary':summary,'all_rows_attempted':True,'android_procedural_rendering_verified':False,
            'runtime_factory_contract_agreed':False,'runtime_objects_verified':False,'full_loader_verified':False}
    a.out.write_text(json.dumps(report,indent=2)+'\n');print(json.dumps({'rows':len(rows),'summary':summary}))
    return 0
if __name__=='__main__':raise SystemExit(main())
