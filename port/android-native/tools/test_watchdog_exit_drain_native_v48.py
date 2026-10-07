"""Actual exit sampling in one 64 MiB Job with one benign Python child. No emulator."""
from pathlib import Path
import hashlib,json,os,sys,tempfile,time
from emulator_watchdog_v36 import WindowsAPI,ProcessGone,collect_snapshot
from windows_job_guard_v36 import MemoryJob,creation_time,Wait
ROOT=Path(__file__).resolve().parents[3];OUT=ROOT/'port/android-native/reports/emulator-watchdog-exit-drain-v48';OUT.mkdir(parents=True,exist_ok=True)
class ExitOnSample(WindowsAPI):
 def __init__(self,marker,job):super().__init__();self.marker=marker;self.job=job;self.fire=False;self.triggered=False;self.exit_observation=None
 def process_snapshot(self,pid,metadata=None):
  if self.fire and not self.triggered and pid==self.job.process.dwProcessId:
   self.triggered=True;self.marker.write_text('exit');assert Wait(self.job.process.hProcess,2000)==0,'Owned benign child failed to exit'
  try:return super().process_snapshot(pid,metadata)
  except ProcessGone as gone:
   if pid==self.job.process.dwProcessId:self.exit_observation=gone.diagnostic()
   raise
assert OUT.resolve().is_relative_to(ROOT.resolve())
with tempfile.TemporaryDirectory(dir=OUT) as folder:
 folder=Path(folder);marker=folder/'exit.marker';manifest_path=folder/'manifest.json'
 assert folder.resolve().is_relative_to(OUT.resolve())
 with MemoryJob(64*1024**2,active_process_limit=4) as job:
  code="import pathlib,time;end=time.monotonic()+5;p=pathlib.Path("+repr(str(marker))+");\nwhile not p.exists() and time.monotonic()<end: time.sleep(.005)"
  target=job.create_suspended(sys.executable,['-c',code])
  try:
   manifest={'job_name':job.name,'launcher_pid':os.getpid(),'launcher_create_time':creation_time(pid=os.getpid()),'target_pid':target['pid'],'target_create_time':target['create_time_filetime']}
   manifest_path.write_text(json.dumps(manifest));digest=hashlib.sha256(manifest_path.read_bytes()).hexdigest()
   api=ExitOnSample(marker,job);start=time.monotonic()
   first=collect_snapshot(api,manifest,job.handle,start,manifest_path,digest)
   assert first['complete'] and first['job_process_ids']==[target['pid']],first
   assert not api.target_in_job(os.getpid(),job.handle,manifest['launcher_create_time'])
   job.resume();api.fire=True
   final=collect_snapshot(api,manifest,job.handle,start,manifest_path,digest)
   assert api.triggered and final['complete'],final
   assert Wait(job.process.hProcess,0)==0
   assert final['job_process_ids']==[] and final['job_members']==[],final
   assert api.exit_observation['reason']=='GetExitCodeProcess_exited',api.exit_observation
   assert api.exit_observation['create_time_filetime']==target['create_time_filetime']
   assert api.exit_observation['exit_time_filetime']>=target['create_time_filetime']
   result={'status':'PASS','one_benign_child_only':True,'hard_job_bytes':64*1024**2,'no_emulator_or_device_operations':True,
      'actual_same_handle_exit_diagnostic_observed':True,'actual_extended_job_notification_lag_reproduced':bool(final.get('job_exit_drain_wait_seconds')),
      'exit_observation':api.exit_observation,
      'snapshot_before':first,'snapshot_after_exit':final,'watchdog_sha256':hashlib.sha256(Path(__file__).with_name('emulator_watchdog_v36.py').read_bytes()).hexdigest()}
   (OUT/'native-benign-job-receipt.json').write_text(json.dumps(result,indent=2)+'\n')
   print('PASS actual suspended/live/exit Job member sampling; lag_reproduced='+str(result['actual_extended_job_notification_lag_reproduced']))
  finally:
   if Wait(job.process.hProcess,0)!=0:job.terminate()
