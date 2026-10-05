"""Compare native path attempts to captured actual original execution."""
import argparse,hashlib,json,pathlib,subprocess
ap=argparse.ArgumentParser();ap.add_argument('--original',type=pathlib.Path,required=True)
ap.add_argument('--probe',type=pathlib.Path,required=True);ap.add_argument('--out',type=pathlib.Path,required=True)
a=ap.parse_args();original=json.loads(a.original.read_text());rows=[]
for case in original['cases']:
    assert 'failure' not in case,case
    p=subprocess.run([str(a.probe),case['input']],capture_output=True,text=True)
    native=p.stdout.splitlines();rows.append({'input':case['input'],'original':case['attempts'],
        'native':native,'equal':p.returncode==0 and native==case['attempts']})
report={'validation':'PASS' if all(r['equal'] for r in rows) else 'FAIL','scope':original['scope'],
    'original_sha256':original['engine_sha256'],'original_receipt_sha256':hashlib.sha256(a.original.read_bytes()).hexdigest(),
    'native_probe_sha256':hashlib.sha256(a.probe.read_bytes()).hexdigest(),'cases':rows,'full_loader_verified':False}
a.out.write_text(json.dumps(report,indent=2)+'\n');print(json.dumps({'validation':report['validation'],'cases':len(rows)}))
raise SystemExit(0 if report['validation']=='PASS' else 1)
