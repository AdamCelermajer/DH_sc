"""Compare native generated module overrides with original ARM setter boundaries."""
import argparse,hashlib,json,pathlib,subprocess
def sha(path):
    with path.open('rb') as file:return hashlib.file_digest(file,'sha256').hexdigest()
def main():
    ap=argparse.ArgumentParser()
    ap.add_argument('--probe',required=True)
    for name in ('cache','original','out'):ap.add_argument('--'+name,type=pathlib.Path,required=True)
    a=ap.parse_args();gold=json.loads(a.original.read_text())
    assert gold['selected_execution_complete'] and sha(a.cache)==gold['cache_sha256']
    assert a.probe.startswith('/mnt/c/');probe=pathlib.Path('C:/'+a.probe[7:]);digest=sha(probe)
    cache='/mnt/c/'+str(a.cache).replace('\\','/')[3:]
    command=['wsl.exe','-d','Ubuntu','--','env','ASAN_OPTIONS=detect_leaks=1:halt_on_error=1',
             'UBSAN_OPTIONS=halt_on_error=1',a.probe]
    rows=[]
    for case in gold['cases']:
        for run in case['runs']:
            proc=subprocess.run(command+[cache,case['name'],case['name'],str(run['seed'])],capture_output=True,timeout=90)
            assert proc.returncode==0 and not proc.stderr,(case['name'],run['seed'],proc.stderr.decode())
            actual=json.loads(proc.stdout);expected={key:value for key,value in run.items() if key!='seed'}
            differences={key:{'original':value,'native':actual.get(key)} for key,value in expected.items() if actual.get(key)!=value}
            rows.append({'name':case['name'],'seed':run['seed'],'matches':not differences,
                         'modules':len(actual['modules']),'differences':differences})
    assert sha(probe)==digest,'Probe changed during comparison'
    complete=gold['original_execution_complete'] and len(rows)==70 and all(row['matches'] for row in rows)
    report={'scope':__doc__,'validation':'PASS' if all(row['matches'] for row in rows) else 'FAIL',
            'cache_sha256':gold['cache_sha256'],'original_receipt_sha256':sha(a.original),
            'probe_sha256':digest,'probe':a.probe,'script_sha256':sha(pathlib.Path(__file__)),
            'cases':rows,'runs':len(rows),'module_occurrences':sum(row['modules'] for row in rows),
            'native_module_projection_verified':complete,'full_property_serialization_verified':False,
            'android_procedural_rendering_verified':False,'full_loader_verified':False}
    a.out.write_text(json.dumps(report,indent=2)+'\n')
    print(json.dumps({'validation':report['validation'],'runs':len(rows),'modules':report['module_occurrences'],
                      'failures':[{'name':row['name'],'seed':row['seed'],'keys':list(row['differences'])} for row in rows if not row['matches']]}))
    return 0 if report['validation']=='PASS' else 1
if __name__=='__main__':raise SystemExit(main())
