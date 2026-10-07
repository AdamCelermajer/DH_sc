"""Stage exactly one protected report field against root's current request-id code."""
from pathlib import Path
import difflib,hashlib,json
root=Path(__file__).resolve().parents[3];out=root/'port/android-native/reports/pacing-report-v46';out.mkdir(parents=True,exist_ok=True)
path='port/android-native/app/src/main/java/com/example/dh2/MainActivity.java';before=(root/path).read_text()
anchor='                                report.put("request_id",reportRequest==null?"":reportRequest);'
assert before.count(anchor)==1
after=before.replace(anchor,anchor+'\n                                report.put("pacing",new org.json.JSONObject(FramePacingReportV46.capture(framePacer)));')
(out/'MainActivity.java').write_text(after)
(out/'integration.patch').write_bytes(''.join(difflib.unified_diff(before.splitlines(True),after.splitlines(True),fromfile='a/'+path,tofile='b/'+path)).encode())
(out/'baseline-source-sha256.json').write_text(json.dumps({path:hashlib.sha256((root/path).read_bytes()).hexdigest()},indent=2)+'\n')
assert 'registerReceiver(debugAttackReceiver,filter,"android.permission.DUMP"' in after
assert 'ApplicationInfo.FLAG_DEBUGGABLE' in after and before.count('request_id')==after.count('request_id')
print('Staged one pacing export field; request ID and protected debug gate unchanged')
