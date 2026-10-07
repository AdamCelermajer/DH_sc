from pathlib import Path
import hashlib,json,zipfile
root=Path(__file__).resolve().parents[3];out=root/'port/android-native/reports/emulator-watchdog-exit-drain-v48'
source=root/'port/android-native/tools/emulator_watchdog_v36.py';digest=hashlib.sha256(source.read_bytes()).hexdigest()
receipt_paths=[out/'fixture-receipt.json',root/'port/android-native/reports/emulator-watchdog-membership-v45/fixture-receipt.json',
 root/'port/android-native/reports/emulator-watchdog-child-exit-v42/race-test-receipt.json',root/'port/android-native/reports/emulator-watchdog-v36/fixture-receipt.json']
receipts=[json.loads(p.read_text()) for p in receipt_paths]
for receipt in receipts:assert receipt['status']=='PASS' and receipt['watchdog_sha256']==digest
assert sum(p['tests'] for p in receipts)==49
native=json.loads((out/'native-benign-job-receipt.json').read_text());assert native['status']=='PASS' and native['watchdog_sha256']==digest
assert native['actual_extended_job_notification_lag_reproduced']
files=[source,Path(__file__),root/'port/android-native/tools/test_watchdog_exit_drain_native_v48.py']
files += [root/'port/android-native/tools/tests'/name for name in ['test_watchdog_exit_drain_v48.py','test_watchdog_membership_v45.py','test_watchdog_child_exit_v42.py','test_emulator_watchdog_v36.py']]
files += [out/'README.md',out/'native-benign-job-receipt.json',*receipt_paths]
manifest={'status':'FROZEN_LAUNCH_READY_COMPONENT','watchdog_sha256':digest,'injected_tests':49,
 'actual_benign_job_exit_proof':'PASS_WITH_REAL_NOTIFICATION_LAG','live_emulator_acceptance':False,'planned_exit_wait_budget_seconds':.15,
 'metric_pass_bound':3,'membership_query_bound':5,'memory_limits_and_termination_scope_unchanged':True,
 'files':{p.relative_to(root).as_posix():{'sha256':hashlib.sha256(p.read_bytes()).hexdigest(),'bytes':p.stat().st_size} for p in files}}
(out/'manifest.json').write_text(json.dumps(manifest,indent=2)+'\n');files.append(out/'manifest.json')
with zipfile.ZipFile(out/'handoff.zip','w',zipfile.ZIP_DEFLATED) as z:
 for path in files:z.write(path,path.relative_to(root).as_posix())
with zipfile.ZipFile(out/'handoff.zip') as z:
 assert z.testzip() is None
 for path,row in manifest['files'].items():assert hashlib.sha256(z.read(path)).hexdigest()==row['sha256']
result={'zip_sha256':hashlib.sha256((out/'handoff.zip').read_bytes()).hexdigest(),'watchdog_sha256':digest,'files':len(files),'bytes':(out/'handoff.zip').stat().st_size,'tests':49,'actual_native_job':'PASS_REAL_EXIT_NOTIFICATION_LAG'}
(out/'handoff-receipt.json').write_text(json.dumps(result,indent=2)+'\n');print(json.dumps(result))
