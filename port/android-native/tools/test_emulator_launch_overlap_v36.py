"""Actual live lease admission proof in a tiny suspended job; no emulator."""
from pathlib import Path
import json,os,subprocess,sys,uuid
from windows_job_guard_v36 import MemoryJob,creation_time
ROOT=Path(__file__).resolve().parents[3]
registry=ROOT/'.local-inputs/emulator-guards-v36';registry.mkdir(parents=True,exist_ok=True)
manifest=registry/('synthetic-overlap-'+uuid.uuid4().hex+'.manifest.json')
try:
 with MemoryJob(64*1024**2) as job:
  target=job.create_suspended(sys.executable,['-c','import time;time.sleep(2)'])
  record={'job_name':job.name,'launcher_pid':os.getpid(),'launcher_create_time':creation_time(pid=os.getpid()),'target_pid':target['pid'],'target_create_time':target['create_time_filetime'],'synthetic_bounded_proof':True}
  manifest.write_text(json.dumps(record))
  result=subprocess.run([sys.executable,str(Path(__file__).with_name('launch_guarded_emulator_v36.py')),'--avd','Medium_Phone_API_37.0','--dry-run'],capture_output=True,text=True,timeout=10)
  assert result.returncode==125 and 'already owns the host lease' in result.stderr,result
  job.terminate()
  report={'actual_named_job_lease_refused':True,'exit':result.returncode,'error':result.stderr,'no_emulator_started':True,'maximum_job_bytes':64*1024**2}
 out=ROOT/'port/android-native/reports/emulator-guard-v36/actual-overlap-proof.json';out.write_text(json.dumps(report,indent=2)+'\n')
 print('PASS live named-job admission refused; no emulator launched')
finally:
 manifest.unlink(missing_ok=True)
