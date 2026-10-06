"""Small (64MiB maximum) real OS-cap proof; never starts an emulator."""
from pathlib import Path
import ctypes as C,json,os,subprocess,sys,time
from windows_job_guard_v36 import MemoryJob,Wait,OpenProcess,checked,Close
ROOT=Path(__file__).resolve().parents[3]
OUT=ROOT/'port/android-native/reports/emulator-guard-v36';OUT.mkdir(parents=True,exist_ok=True)
if len(sys.argv)>1 and sys.argv[1]=='child':
 target=Path(sys.argv[2]);allocate=C.WinDLL('kernel32',use_last_error=True).VirtualAlloc
 allocate.argtypes=[C.c_void_p,C.c_size_t,C.c_ulong,C.c_ulong];allocate.restype=C.c_void_p
 chunks=[];failure=None
 for _ in range(16):
  value=allocate(None,8*1024**2,0x3000,4)
  if not value:failure=C.get_last_error();break
  chunks.append(value)
 target.write_text(json.dumps({'pid':os.getpid(),'committed_test_bytes':len(chunks)*8*1024**2,'allocation_failed':failure is not None,'winerror':failure}))
 print('bounded child allocation refused',flush=True)
 sys.exit(0 if failure else 1)
if len(sys.argv)>1 and sys.argv[1] in ('leaf','branch'):
 marker=Path(sys.argv[2]);marker.write_text(str(os.getpid()))
 if sys.argv[1]=='branch':subprocess.Popen([sys.executable,__file__,'leaf',str(marker.with_suffix('.leaf'))],creationflags=0x08000000)
 time.sleep(10);sys.exit(0)
report={'hard_limit_bytes':64*1024**2,'checks':[]}
with MemoryJob(report['hard_limit_bytes']) as job:
 before=job.limits();assert before['job_memory_limit']==report['hard_limit_bytes']
 target=OUT/'bounded-allocation-child.json';target.unlink(missing_ok=True)
 stdout=OUT/'hard-cap-stdout.log';stderr=OUT/'hard-cap-stderr.log'
 stdout.unlink(missing_ok=True);stderr.unlink(missing_ok=True)
 identity=job.create_suspended(sys.executable,[__file__,'child',str(target)],stdout_path=stdout,stderr_path=stderr)
 assert job.members()==[identity['pid']]
 time.sleep(.1);assert not target.exists(),'Suspended child executed before guard'
 job.resume();assert Wait(job.process.hProcess,15000)==0,'Bounded test exceeded15seconds'
 data=json.loads(target.read_text());assert data['allocation_failed']
 assert 'bounded child allocation refused' in stdout.read_text();assert not stderr.read_bytes()
 assert data['committed_test_bytes']<report['hard_limit_bytes']
 after=job.limits();assert after['peak_job_bytes']<=report['hard_limit_bytes']
 report.update(before=before,after=after,child=data)
 report['checks']=['hard_job_limit_readback','per_process_limit_readback','suspended_before_assignment','only_owned_pid_membership','OS_rejects_over_cap_commit','peak_within64MiB','host_logs_captured']
marker=OUT/'descendant.pid';leaf=marker.with_suffix('.leaf');marker.unlink(missing_ok=True);leaf.unlink(missing_ok=True)
handles=[]
try:
 with MemoryJob(64*1024**2) as job:
  target=job.create_suspended(sys.executable,[__file__,'branch',str(marker)]);job.resume()
  deadline=time.monotonic()+5
  while not leaf.exists():
   assert time.monotonic()<deadline,'Descendant test exceeded5seconds';time.sleep(.05)
  ids=[int(marker.read_text()),int(leaf.read_text())]
  assert set(ids).issubset(set(job.members())),'Child escaped the owned memory job'
  handles=[checked(OpenProcess(0x100000|0x1000,False,pid),'Open synthetic held identity') for pid in ids]
 for handle in handles:assert Wait(handle,5000)==0,'Owned descendant survived last-job-handle close'
 report['checks']+=['automatic_descendant_membership','kill_on_close_owned_root','kill_on_close_owned_descendant']
finally:
 for handle in handles:Close(handle)
(OUT/'hard-job-cap-proof.json').write_text(json.dumps(report,indent=2)+'\n')
print('PASS real hard job cap:',len(report['checks']),'checks, maximum64MiB, noemulator')
