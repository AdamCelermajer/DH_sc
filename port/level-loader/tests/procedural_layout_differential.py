"""Compare native generation against executed original ARM layouts and events."""
import argparse,hashlib,json,pathlib,subprocess,zipfile

def sha(path):
    with path.open('rb') as file:return hashlib.file_digest(file,'sha256').hexdigest()

def blob(raw):return str(len(raw)).encode()+b'\n'+raw

def main():
    ap=argparse.ArgumentParser()
    ap.add_argument('--probe',required=True)
    ap.add_argument('--cache',type=pathlib.Path,required=True)
    ap.add_argument('--original',type=pathlib.Path,required=True)
    ap.add_argument('--out',type=pathlib.Path,required=True)
    ap.add_argument('--facade',action='store_true')
    a=ap.parse_args();gold=json.loads(a.original.read_text())
    assert gold['original_execution_complete'] and sha(a.cache)==gold['cache_sha256']
    assert a.probe.startswith('/mnt/c/'),a.probe
    probe_path=pathlib.Path('C:/'+a.probe[len('/mnt/c/'):])
    probe_sha=sha(probe_path)
    command=['wsl.exe','-d','Ubuntu','--','env','ASAN_OPTIONS=detect_leaks=1:halt_on_error=1',
             'UBSAN_OPTIONS=halt_on_error=1',a.probe]
    requests=[];payload=bytearray();prefix='com.gameloft.android.GAND.GloftD2SS/files/'
    with zipfile.ZipFile(a.cache) as pack:
        for case in gold['cases']:
            authored=case['name'].startswith('data/')
            if a.facade and not authored:continue
            raw=pack.read(prefix+case['name']) if authored else bytes.fromhex(case['input_hex'])
            assert hashlib.sha256(raw).hexdigest()==case['input_sha256']
            blocks=[]
            for block in case['blocks']:
                data=pack.read(prefix+block['uri']) if authored else bytes.fromhex(block['input_hex'])
                assert hashlib.sha256(data).hexdigest()==block['input_sha256']
                blocks.append(blob(block['name'].encode())+blob(block['uri'].encode())+blob(data))
            for run in case['runs']:
                requests.append((case,run))
                payload+=str(len(blocks)).encode()+b'\n'+b''.join(blocks)+blob(raw)+str(run['seed']).encode()+b'\n'
    if a.facade:
        cache='/mnt/c/'+str(a.cache).replace('\\','/')[3:]
        actual=[]
        for case,run in requests:
            proc=subprocess.run(command+[cache,case['name'],case['name'],str(run['seed'])],
                                capture_output=True,timeout=90)
            assert proc.returncode==0 and not proc.stderr,(case['name'],run['seed'],proc.stderr.decode())
            actual.append(json.loads(proc.stdout))
    else:
        proc=subprocess.run(command,input=payload,capture_output=True,timeout=180)
        assert proc.returncode==0 and not proc.stderr,proc.stderr.decode()
        actual=[json.loads(line) for line in proc.stdout.splitlines()]
    assert len(actual)==len(requests)
    comparisons=[]
    for (case,run),got in zip(requests,actual):
        want={key:value for key,value in run.items() if key!='adapters'}
        errors={key:{'original':value,'native':got.get(key)} for key,value in want.items() if got.get(key)!=value}
        if 'error' in got:errors['checked_domain']=got['error']
        comparisons.append({'name':case['name'],'seed':run['seed'],'matches':not errors,
                            'tiles':len(got.get('tiles',[])),'differences':errors})
    report={'validation':'PASS' if all(c['matches'] for c in comparisons) else 'FAIL',
            'scope':__doc__,'mode':'cache-facade' if a.facade else 'retained-reader-inputs',
            'original_receipt_sha256':sha(a.original),'cache_sha256':sha(a.cache),
            'probe':a.probe,'probe_sha256':probe_sha,'runs':len(comparisons),'cases':comparisons,
            'native_layout_generation_verified':all(c['matches'] for c in comparisons),
            'android_procedural_rendering_verified':False,'full_loader_verified':False,
            'script_sha256':sha(pathlib.Path(__file__))}
    a.out.write_text(json.dumps(report,indent=2)+'\n')
    assert sha(probe_path)==probe_sha,'Probe changed during comparison'
    print(json.dumps({'validation':report['validation'],'runs':len(comparisons),
                      'failures':[{'name':c['name'],'seed':c['seed'],'keys':list(c['differences'])}
                                  for c in comparisons if not c['matches']]}))
    return 0 if report['validation']=='PASS' else 1
if __name__=='__main__':raise SystemExit(main())
