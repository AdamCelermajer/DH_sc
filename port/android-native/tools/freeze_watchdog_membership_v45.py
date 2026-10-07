"""Freeze exact updated independent watchdog and injected safety receipts."""
from pathlib import Path
import hashlib,json,zipfile
root=Path(__file__).resolve().parents[3];out=root/'port/android-native/reports/emulator-watchdog-membership-v45'
source=root/'port/android-native/tools/emulator_watchdog_v36.py';digest=hashlib.sha256(source.read_bytes()).hexdigest()
paths=[out/'fixture-receipt.json',root/'port/android-native/reports/emulator-watchdog-child-exit-v42/race-test-receipt.json',root/'port/android-native/reports/emulator-watchdog-v36/fixture-receipt.json']
receipts=[json.loads(p.read_text()) for p in paths]
for receipt in receipts:assert receipt['status']=='PASS' and receipt['watchdog_sha256']==digest
assert sum(p['tests'] for p in receipts)==41
(out/'prior-race-receipt.json').write_text(json.dumps(receipts[1],indent=2)+'\n')
(out/'existing-protocol-receipt.json').write_text(json.dumps(receipts[2],indent=2)+'\n')
files=[source,root/'port/android-native/tools/tests/test_watchdog_membership_v45.py',
 root/'port/android-native/tools/tests/test_watchdog_child_exit_v42.py',root/'port/android-native/tools/tests/test_emulator_watchdog_v36.py',Path(__file__)]
files += [out/name for name in ['README.md','fixture-receipt.json','prior-race-receipt.json','existing-protocol-receipt.json','captured-receipt-analysis.json']]
manifest={'status':'FROZEN_LAUNCH_READY_COMPONENT','watchdog_sha256':digest,'tests':41,'memory_limits_unchanged':True,
 'job_termination_api_and_scope_unchanged':True,'metric_pass_bound':3,'membership_query_bound':5,'diagnostic_list_bound':64,
 'live_acceptance':'PENDING_NEXT_ROOT_AUTHORIZED_GUARDED_LAUNCH','files':{p.relative_to(root).as_posix():{'sha256':hashlib.sha256(p.read_bytes()).hexdigest(),'bytes':p.stat().st_size} for p in files}}
(out/'manifest.json').write_text(json.dumps(manifest,indent=2)+'\n');files.append(out/'manifest.json')
archive=out/'handoff.zip'
with zipfile.ZipFile(archive,'w',zipfile.ZIP_DEFLATED) as z:
 for path in files:z.write(path,path.relative_to(root).as_posix())
with zipfile.ZipFile(archive) as z:
 assert z.testzip() is None
 for path,row in manifest['files'].items():assert hashlib.sha256(z.read(path)).hexdigest()==row['sha256']
result={'zip_sha256':hashlib.sha256(archive.read_bytes()).hexdigest(),'watchdog_sha256':digest,'bytes':archive.stat().st_size,'files':len(files),'tests':41}
(out/'handoff-receipt.json').write_text(json.dumps(result,indent=2)+'\n');print(json.dumps(result))
