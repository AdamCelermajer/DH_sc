"""Actual named-job/watchdog handshake, timeout and owner-death proof.
Only benign Python sleepers in 64MiB jobs; no emulator or large allocation.
"""
from pathlib import Path
import json,os,subprocess,sys,time,uuid
from windows_job_guard_v36 import MemoryJob,creation_time,Wait
ROOT=Path(__file__).resolve().parents[3]
OUT=ROOT/'port/android-native/reports/emulator-guard-v36';OUT.mkdir(parents=True,exist_ok=True)
report=[]
for scenario in ('timeout','owner-death'):
 folder=OUT/(scenario+'-'+uuid.uuid4().hex[:8]);folder.mkdir()
 other=subprocess.Popen([sys.executable,'-c','import time;time.sleep(15)'],creationflags=0x08000000)
 fake_owner=None;watchdog=None
 try:
  owner_pid=os.getpid()
  if scenario=='owner-death':
   fake_owner=subprocess.Popen([sys.executable,'-c','import time;time.sleep(15)'],creationflags=0x08000000);owner_pid=fake_owner.pid
  with MemoryJob(64*1024**2) as job:
   target=job.create_suspended(sys.executable,['-c','import time;time.sleep(15)'])
   manifest={'job_name':job.name,'launcher_pid':owner_pid,'launcher_create_time':creation_time(pid=owner_pid),'target_pid':target['pid'],'target_create_time':target['create_time_filetime']}
   (folder/'manifest.json').write_text(json.dumps(manifest))
   args=[sys.executable,str(Path(__file__).with_name('emulator_watchdog_v36.py')),'--manifest',str(folder/'manifest.json'),'--telemetry',str(folder/'telemetry.jsonl'),'--receipt',str(folder/'receipt.json'),'--ready',str(folder/'ready.json'),'--poll-seconds','.1','--timeout-seconds',str(1 if scenario=='timeout' else 10)]
   watchdog=subprocess.Popen(args,stdout=subprocess.PIPE,stderr=subprocess.PIPE,creationflags=0x08000000)
   deadline=time.monotonic()+5
   while not (folder/'ready.json').exists():
    if watchdog.poll() is not None:raise AssertionError(watchdog.communicate())
    assert time.monotonic()<deadline,'Independent handshake exceeded5seconds';time.sleep(.02)
   assert json.loads((folder/'ready.json').read_text())['verified']
   assert Wait(job.process.hProcess,0)==258,'Suspended target exited before resume'
   job.resume()
   if fake_owner:fake_owner.terminate();fake_owner.wait(timeout=2)
   stdout,stderr=watchdog.communicate(timeout=5)
   assert not stderr,stderr
   receipt=json.loads((folder/'receipt.json').read_text())
   assert receipt['status']=='terminated_by_guard',receipt
   assert receipt['termination_succeeded'],receipt
   assert Wait(job.process.hProcess,2000)==0,'Owned child survived watchdog stop'
   assert other.poll() is None,'Independent unrelated process was stopped'
   report.append({'scenario':scenario,'receipt':str(folder/'receipt.json'),'owned_target_stopped':True,'unrelated_target_preserved':True})
 finally:
  if watchdog and watchdog.poll() is None:watchdog.terminate();watchdog.wait(timeout=2)
  for process in (fake_owner,other):
   if process and process.poll() is None:process.terminate();process.wait(timeout=2)
(OUT/'native-watchdog-proof.json').write_text(json.dumps(report,indent=2)+'\n')
print('PASS native independent watchdog: readiness, suspended assignment, timeout, ownerdeath, exactjobcleanup, unrelatedpreservation')
