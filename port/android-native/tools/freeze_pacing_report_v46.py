from pathlib import Path
import hashlib,json,subprocess,zipfile
root=Path(__file__).resolve().parents[3];out=root/'port/android-native/reports/pacing-report-v46'
baseline=json.loads((out/'baseline-source-sha256.json').read_text())
for path,digest in baseline.items():assert hashlib.sha256((root/path).read_bytes()).hexdigest()==digest
assert json.loads((out/'receipt.json').read_text())['status']=='PASS'
p=subprocess.run(['git','apply','--check',str(out/'integration.patch')],cwd=root,capture_output=True,text=True)
assert not p.returncode,p.stderr
files=[root/'port/android-native/app/src/main/java/com/example/dh2/FramePacingReportV46.java',
 root/'port/android-native/tools/tests/FramePacingReportV46Test.java',root/'port/android-native/tools/prepare_pacing_report_v46.py',
 root/'port/android-native/tools/test_pacing_report_v46.py',Path(__file__)]
files += [out/name for name in ['README.md','MainActivity.java','integration.patch','baseline-source-sha256.json','receipt.json']]
manifest={'status':'FROZEN_COMPONENT','tests':12,'runtime_acceptance':False,'shared_apply_patch_only':list(baseline),
 'files':{p.relative_to(root).as_posix():{'sha256':hashlib.sha256(p.read_bytes()).hexdigest(),'bytes':p.stat().st_size} for p in files}}
(out/'manifest.json').write_text(json.dumps(manifest,indent=2)+'\n');files.append(out/'manifest.json')
with zipfile.ZipFile(out/'handoff.zip','w',zipfile.ZIP_DEFLATED) as z:
 for path in files:z.write(path,path.relative_to(root).as_posix())
result={'sha256':hashlib.sha256((out/'handoff.zip').read_bytes()).hexdigest(),'bytes':(out/'handoff.zip').stat().st_size,'files':len(files)}
(out/'handoff-receipt.json').write_text(json.dumps(result,indent=2)+'\n');print(json.dumps(result))
