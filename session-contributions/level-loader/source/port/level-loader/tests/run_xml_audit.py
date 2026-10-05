"""Native candidate inventory and actual original-parser comparison.

Native phase runs in WSL. Original phase runs on Windows with private Unicorn
dependencies. Both use unchanged canonical bytes and persist failures/results.
"""
import argparse, hashlib, json, pathlib, subprocess, sys, zipfile
ROOT=pathlib.Path(__file__).resolve().parents[3]
PREFIX='com.gameloft.android.GAND.GloftD2SS/files/'
EXPECTED='3fdf4e4c21d45a780a7c35fb4042abde0e88e76bf75416aad1f227481560b679'

def main():
    ap=argparse.ArgumentParser();ap.add_argument('--phase',choices=['native','original'],required=True)
    ap.add_argument('--cache',type=pathlib.Path,required=True);ap.add_argument('--probe',type=pathlib.Path)
    ap.add_argument('--engine',type=pathlib.Path);ap.add_argument('--all-original',action='store_true')
    ap.add_argument('--dependency-root',type=pathlib.Path);a=ap.parse_args()
    if a.dependency_root:sys.path.insert(0,str(a.dependency_root))
    reports=ROOT/'port/level-loader/reports';out=ROOT.parent/'build/parser-corpus';out.mkdir(parents=True,exist_ok=True)
    with a.cache.open('rb') as f:assert hashlib.file_digest(f,'sha256').hexdigest()==EXPECTED
    if a.phase=='native':
        rows=[]
        with zipfile.ZipFile(a.cache) as z:
            entries=sorted([e for e in z.infolist() if not e.is_dir() and
                            e.filename.lower().endswith(('.mlx','.mgp','.mvp','.rule.xml'))],key=lambda e:e.filename.lower())
            for i,e in enumerate(entries):
                raw=z.read(e);inp=out/f'{i:04}.xml';inp.write_bytes(raw)
                proc=subprocess.run([str(a.probe),str(inp)],capture_output=True,text=True)
                row={'uri':e.filename[len(PREFIX):],'input_sha256':hashlib.sha256(raw).hexdigest(),'input_file':inp.name}
                if proc.returncode:row['probe_failure']=proc.stderr
                else:row.update(json.loads(proc.stdout))
                rows.append(row)
        report={'cache_sha256':EXPECTED,'native_probe_sha256':hashlib.sha256(a.probe.read_bytes()).hexdigest(),
                'scope':'All cached MLX/MGP/MVP/rule.xml files; candidate parser only, original behavior not inferred.',
                'documents':rows,'original_behavior_verified':False}
        (reports/'native-xml-corpus.json').write_text(json.dumps(report,indent=2)+'\n')
        print(json.dumps({'documents':len(rows),'parse_errors':sum(bool(r.get('xml_error')) for r in rows),
                          'probe_failures':sum('probe_failure' in r for r in rows)}),flush=True)
    else:
        sys.path.insert(0,str(ROOT/'port/level-loader/tests'));from xml_original_probe import Parser
        from unicorn import UC_HOOK_CODE
        assert a.engine and hashlib.sha256(a.engine.read_bytes()).hexdigest()=='36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80'
        inventory=json.loads((reports/'canonical-level-inventory.json').read_text())
        levels={r['definition_uri'] for r in inventory['levels']}
        native=json.loads((reports/'native-xml-corpus.json').read_text());results=[]
        selected=[r for r in native['documents'] if a.all_original or r['uri'].lower() in levels or r.get('xml_error')]
        for i,row in enumerate(selected):
            # The modeled heap deliberately does not recycle frees. Recreate the
            # complete process at batch boundaries; resetting only its heap would
            # overwrite original static pool objects retained by the parser.
            if i%16==0:
                parser=Parser(a.engine,{'functions':[]});parser.uc.hook_add(UC_HOOK_CODE,parser.allocation)
            raw=(out/row['input_file']).read_bytes();assert hashlib.sha256(raw).hexdigest()==row['input_sha256']
            result={'uri':row['uri'],'input_sha256':row['input_sha256']}
            try:
                old=parser.parse(raw);result['original']=old
                result['tree_equal']=old['root']==row.get('root')
                result['error_equal']=old['xml_error']==row.get('xml_error')
            except Exception as e:result['original_execution_failure']=str(e)
            results.append(result)
            if (i+1)%10==0:print(json.dumps({'completed':i+1,'total':len(selected)}),flush=True)
        mismatches=[r['uri'] for r in results if not r.get('tree_equal') or not r.get('error_equal')]
        report={'validation':'PASS' if not mismatches else 'INCOMPLETE','original_sha256':hashlib.sha256(a.engine.read_bytes()).hexdigest(),
                'scope':'Original ARM32 parser versus native host parser: element tree, complete attributes and error ID. Text-node contents, level callers and gameplay are not compared.',
                'services':'Byte/string libc, heap, C-locale Bionic classification/case tables, integer division, uncontended mutex lock/unlock.',
                'cases':results,'mismatches':mismatches,'all_cached_xml_compared':a.all_original,
                'native_probe_sha256':native.get('native_probe_sha256'),'full_loader_verified':False}
        (reports/'xml-original-comparison.json').write_text(json.dumps(report,indent=2)+'\n')
        print(json.dumps({'validation':report['validation'],'cases':len(results),'mismatches':mismatches}),flush=True)

if __name__=='__main__':main()
