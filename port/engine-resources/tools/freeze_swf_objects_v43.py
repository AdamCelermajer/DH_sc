"""Verify surgical applicability and freeze only source/receipt files."""
from pathlib import Path
import hashlib,json,subprocess,zipfile
root=Path(__file__).resolve().parents[3];out=root/'port/engine-resources/reports/swf-objects-v43'
baseline=json.loads((out/'baseline-source-sha256.json').read_text())
for path,digest in baseline.items():assert hashlib.sha256((root/path).read_bytes()).hexdigest()==digest,path
for name in ['host-receipt.json','android-compile.json']:assert json.loads((out/name).read_text())['status']=='PASS',name
p=subprocess.run(['git','apply','--check',str(out/'integration.patch')],cwd=root,capture_output=True,text=True)
(out/'patch-check.json').write_text(json.dumps({'exit_code':p.returncode,'stdout':p.stdout,'stderr':p.stderr},indent=2)+'\n');assert not p.returncode,p.stderr
files={path:out/Path(path).name for path in baseline}
new=['port/engine-resources/gpu_object_owner_v43.hpp','port/engine-resources/tests/swf_objects_consumer_v43.cpp',
 'port/engine-resources/tools/prepare_swf_objects_v43.py','port/engine-resources/tools/test_swf_objects_v43_host.py',
 'port/engine-resources/tools/compile_swf_objects_v43_android.py','port/engine-resources/tools/freeze_swf_objects_v43.py']
files.update({path:root/path for path in new})
names=['README.md','integration.patch','baseline-source-sha256.json','host-receipt.json','android-compile.json','android-overlay.json','patch-check.json']
files.update({'port/engine-resources/reports/swf-objects-v43/'+name:out/name for name in names})
inc='test-layout/engine-resources/tests/swf_objects_source_v43.inc'
files['port/engine-resources/reports/swf-objects-v43/'+inc]=out/inc
manifest={'status':'FROZEN_COMPONENT','shared_files_apply_via_patch_only':list(baseline),'new_owned_files':new,
 'files':{path:{'sha256':hashlib.sha256(src.read_bytes()).hexdigest(),'bytes':src.stat().st_size} for path,src in files.items()}}
(out/'handoff-manifest.json').write_text(json.dumps(manifest,indent=2)+'\n')
files['port/engine-resources/reports/swf-objects-v43/handoff-manifest.json']=out/'handoff-manifest.json'
with zipfile.ZipFile(out/'handoff.zip','w',zipfile.ZIP_DEFLATED) as archive:
 for path,src in files.items():archive.write(src,path)
receipt={'sha256':hashlib.sha256((out/'handoff.zip').read_bytes()).hexdigest(),'bytes':(out/'handoff.zip').stat().st_size,'files':len(files),'patch_check':'PASS'}
(out/'handoff-receipt.json').write_text(json.dumps(receipt,indent=2)+'\n');print(json.dumps(receipt))
