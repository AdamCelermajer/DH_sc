"""Freeze a surgical, opt-in Java experiment with its exact bounded evidence."""
from pathlib import Path
import hashlib,json,subprocess,zipfile
root=Path(__file__).resolve().parents[3];out=root/'port/android-native/reports/frame-pacing-v44'
baseline=json.loads((out/'baseline-source-sha256.json').read_text())
for path,digest in baseline.items():assert hashlib.sha256((root/path).read_bytes()).hexdigest()==digest,path
assert json.loads((out/'java-receipt.json').read_text())['status']=='PASS'
p=subprocess.run(['git','apply','--check',str(out/'integration.patch')],cwd=root,capture_output=True,text=True)
(out/'patch-check.json').write_text(json.dumps({'exit_code':p.returncode,'stdout':p.stdout,'stderr':p.stderr},indent=2)+'\n');assert not p.returncode,p.stderr
new=['port/android-native/app/src/main/java/com/example/dh2/FramePacingControllerV44.java',
 'port/android-native/app/src/main/java/com/example/dh2/VsyncSurfaceViewV44.java',
 'port/android-native/tools/tests/FramePacingV44Test.java','port/android-native/tools/prepare_frame_pacing_v44.py',
 'port/android-native/tools/test_frame_pacing_v44.py','port/android-native/tools/freeze_frame_pacing_v44.py']
files={path:root/path for path in new};files.update({path:out/Path(path).name for path in baseline})
for name in ['README.md','integration.patch','baseline-source-sha256.json','native-wall-clock-contract.json','java-receipt.json','patch-check.json']:
 files['port/android-native/reports/frame-pacing-v44/'+name]=out/name
manifest={'status':'FROZEN_OPT_IN_EXPERIMENT','runtime_acceptance':False,'default_continuous_unchanged':True,'shared_apply_patch_only':list(baseline),'files':{path:{'sha256':hashlib.sha256(src.read_bytes()).hexdigest(),'bytes':src.stat().st_size} for path,src in files.items()}}
(out/'handoff-manifest.json').write_text(json.dumps(manifest,indent=2)+'\n');files['port/android-native/reports/frame-pacing-v44/handoff-manifest.json']=out/'handoff-manifest.json'
with zipfile.ZipFile(out/'handoff.zip','w',zipfile.ZIP_DEFLATED) as archive:
 for path,src in files.items():archive.write(src,path)
receipt={'sha256':hashlib.sha256((out/'handoff.zip').read_bytes()).hexdigest(),'bytes':(out/'handoff.zip').stat().st_size,'files':len(files),'patch_check':'PASS'}
(out/'handoff-receipt.json').write_text(json.dumps(receipt,indent=2)+'\n');print(json.dumps(receipt))
