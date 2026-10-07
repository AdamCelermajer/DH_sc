from pathlib import Path
import hashlib,json,subprocess,zipfile
root=Path(__file__).resolve().parents[3];out=root/'port/android-native/reports/pacing-request-lifetime-v47'
baseline=json.loads((out/'baseline-source-sha256.json').read_text())
for path,digest in baseline.items():assert hashlib.sha256((root/path).read_bytes()).hexdigest()==digest,path
assert json.loads((out/'receipt.json').read_text())['status']=='PASS'
p=subprocess.run(['git','apply','--check',str(out/'integration.patch')],cwd=root,capture_output=True,text=True);assert not p.returncode,p.stderr
files=[root/'port/android-native/tools'/name for name in ['prepare_pacing_queue_v47.py','test_pacing_queue_v47.py','freeze_pacing_queue_v47.py']]
files += [root/'port/android-native/tools/tests/FramePacingQueueV47Test.java']
files += [out/Path(path).name for path in baseline]
files += [out/name for name in ['README.md','integration.patch','baseline-source-sha256.json','receipt.json']]
manifest={'status':'FROZEN_OPT_IN_LIFECYCLE_COMPONENT','tests':136,'actual_FPS_acceptance':False,'default_continuous_unchanged':True,
 'shared_apply_patch_only':list(baseline),'files':{p.relative_to(root).as_posix():{'sha256':hashlib.sha256(p.read_bytes()).hexdigest(),'bytes':p.stat().st_size} for p in files}}
(out/'manifest.json').write_text(json.dumps(manifest,indent=2)+'\n');files.append(out/'manifest.json')
with zipfile.ZipFile(out/'handoff.zip','w',zipfile.ZIP_DEFLATED) as z:
 for path in files:z.write(path,path.relative_to(root).as_posix())
result={'sha256':hashlib.sha256((out/'handoff.zip').read_bytes()).hexdigest(),'files':len(files),'bytes':(out/'handoff.zip').stat().st_size}
(out/'handoff-receipt.json').write_text(json.dumps(result,indent=2)+'\n');print(json.dumps(result))
