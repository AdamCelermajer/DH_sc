"""The supported DH_sc emulator entry point. One bounded emulator per host.

No emulator runs before OS job limits and an independent watchdog are verified.
Saves are retained; only this fresh named job can be terminated by this launcher.
"""
from pathlib import Path
import argparse,ctypes as C,json,os,subprocess,sys,time,uuid,socket,math
from ctypes import wintypes as W
from windows_job_guard_v36 import MemoryJob,creation_time,api,checked,Close,Wait
from emulator_watchdog_v36 import system_qemu_snapshot

ROOT=Path(__file__).resolve().parents[3]
REGISTRY=ROOT/'.local-inputs/emulator-guards-v36'
GIB=1024**3
CreateMutex=api('CreateMutexW',[C.c_void_p,W.BOOL,W.LPCWSTR],W.HANDLE)
ReleaseMutex=api('ReleaseMutex',[W.HANDLE])
OpenJob=api('OpenJobObjectW',[W.DWORD,W.BOOL,W.LPCWSTR],W.HANDLE)
def atomic(path,value):
 temporary=path.with_suffix(path.suffix+'.tmp');temporary.write_text(json.dumps(value,indent=2)+'\n',encoding='utf8');temporary.replace(path)
def registry_live():
 for file in REGISTRY.glob('*.manifest.json'):
  record=json.loads(file.read_text(encoding='utf8'))
  try:live=creation_time(pid=record['launcher_pid'])==record['launcher_create_time']
  except OSError as e:
   if getattr(e,'winerror',None) not in (87,1168):raise
   live=False
  handle=OpenJob(4,False,record['job_name'])
  if handle:Close(handle);live=True
  elif C.get_last_error()!=2:raise C.WinError(C.get_last_error(),'Cannot verify prior named job')
  if live:return record
 return None
def free_port(port):
 with socket.socket() as sock:
  sock.setsockopt(socket.SOL_SOCKET,socket.SO_EXCLUSIVEADDRUSE,1)
  sock.bind(('127.0.0.1',port))
def main():
 parser=argparse.ArgumentParser(description=__doc__)
 parser.add_argument('--avd',required=True)
 parser.add_argument('--port',type=int,default=5554)
 parser.add_argument('--adb-port',type=int,default=5037)
 parser.add_argument('--gpu',choices=['swiftshader','host'],default='swiftshader')
 parser.add_argument('--disable-vulkan',action='store_true',help='Bounded GLES-only backend investigation; no AVD config write')
 parser.add_argument('--duration-seconds',type=int,default=1200)
 parser.add_argument('--hard-memory-gib',type=float,default=6)
 parser.add_argument('--soft-memory-gib',type=float,default=5)
 parser.add_argument('--min-physical-available-gib',type=float,default=6)
 parser.add_argument('--allow-guest-paging',action='store_true',help='Explicitly permit Windows paging of guest storage; commit reserve and physical watchdog remain enforced')
 parser.add_argument('--allow-physical-pressure',action='store_true',help='Explicit user-requested paging mode: log physical pressure while enforcing hard job/commit/time/identity limits')
 parser.add_argument('--dry-run',action='store_true')
 args=parser.parse_args()
 if not 1<=args.duration_seconds<=3600:parser.error('Duration must be1..3600seconds')
 if not all(math.isfinite(v) for v in [args.hard_memory_gib,args.soft_memory_gib,args.min_physical_available_gib]):parser.error('Memory thresholds must be finite')
 if not 2<=args.hard_memory_gib<=15 or not 1<=args.soft_memory_gib<args.hard_memory_gib:parser.error('Require1<=soft<hard<=15GiB; hard>=2GiB')
 if not 2<=args.min_physical_available_gib<=6:parser.error('Physical safety floor must be2..6GiB')
 if args.allow_physical_pressure and not args.allow_guest_paging:parser.error('Physical-pressure mode requires explicit guest paging')
 if args.port%2 or not 5554<=args.port<=5680:parser.error('Use an even emulator port5554..5680')
 sdk=Path(os.environ.get('ANDROID_HOME',str(Path.home()/'AppData/Local/Android/Sdk')))
 executable=sdk/'emulator/emulator.exe'
 if not executable.is_file():raise RuntimeError('Configured Android emulator unavailable')
 names=subprocess.run([str(executable),'-list-avds'],capture_output=True,check=True,timeout=10).stdout.decode('utf8','replace').splitlines()
 if args.avd not in names:raise RuntimeError('Requested AVD is not registered')
 REGISTRY.mkdir(parents=True,exist_ok=True)
 mutex=checked(CreateMutex(None,False,'Local\\DH2-Shared-Emulator-Launch-V36'),'Create launch mutex')
 acquired=False;job=None;watchdog=None;record=None;receipt=None
 try:
  result=Wait(mutex,0)
  if result not in (0,0x80):raise RuntimeError('Another DH_sc launch is being admitted; no launch performed')
  acquired=True
  existing=registry_live()
  if existing:raise RuntimeError('One guarded emulator already owns the host lease: '+existing['job_name'])
  snapshot=system_qemu_snapshot()
  if snapshot['qemu']:raise RuntimeError('Existing unmanaged emulator detected; no new launch. No unrelated process stopped.')
  system=snapshot['system']
  headroom=system['commit_limit_bytes']-system['commit_total_bytes']
  if headroom<16*GIB+int(args.hard_memory_gib*GIB):raise RuntimeError('Insufficient system commit headroom for reserved job cap plus16GiB safety margin')
  # API37 reached roughly9GiB working set during the observed15GiB-cap
  # startup. Paging permission cannot replace admission for that resident
  # footprint: retain10GiB startup room plus the selected physical floor.
  required_ram=(args.min_physical_available_gib+(10 if args.allow_guest_paging else 4))*GIB
  if not args.allow_physical_pressure and system['physical_available_bytes']<required_ram:raise RuntimeError('Insufficient available RAM for selected guest paging policy and physical safety floor')
  free_port(args.port);free_port(args.port+1)
  if args.dry_run:print(json.dumps({'admitted':True,'dry_run':True,'no_emulator_started':True,'snapshot':snapshot,'hard_job_gib':args.hard_memory_gib,'one_emulator_policy':True},indent=2));return 0
  stamp=time.strftime('%Y%m%d-%H%M%S')+'-'+uuid.uuid4().hex[:8]
  manifest=REGISTRY/(stamp+'.manifest.json');receipt=REGISTRY/(stamp+'.launcher.json')
  ready=REGISTRY/(stamp+'.ready.json');watch_receipt=REGISTRY/(stamp+'.watchdog.json');telemetry=REGISTRY/(stamp+'.telemetry.jsonl')
  job=MemoryJob(int(args.hard_memory_gib*GIB))
  env={'QT_AUTO_SCREEN_SCALE_FACTOR':'0','QT_ENABLE_HIGHDPI_SCALING':'0','QT_SCALE_FACTOR':'1','QT_FONT_DPI':'96','ANDROID_ADB_SERVER_PORT':str(args.adb_port)}
  os.environ.update(env)
  command=['-avd',args.avd,'-port',str(args.port),'-gpu',args.gpu,'-cores','2','-memory','4096','-crash-report-mode','disabled','-no-snapshot-load','-no-snapshot-save','-no-boot-anim']
  if args.disable_vulkan:command+=['-feature','-Vulkan']
  host_log=REGISTRY/(stamp+'.emulator.log');host_errors=REGISTRY/(stamp+'.emulator-errors.log')
  target=job.create_suspended(executable,command,executable.parent,host_log,host_errors)
  record={'schema':36,'job_name':job.name,'launcher_pid':os.getpid(),'launcher_create_time':creation_time(pid=os.getpid()),'target_pid':target['pid'],'target_create_time':target['create_time_filetime'],'hard_memory_bytes':int(args.hard_memory_gib*GIB),'soft_memory_bytes':int(args.soft_memory_gib*GIB),'physical_floor_bytes':int(args.min_physical_available_gib*GIB),'allow_guest_paging':args.allow_guest_paging,'avd':args.avd,'port':args.port,'adb_port':args.adb_port,'gpu':args.gpu,'duration_seconds':args.duration_seconds,'os_limit_readback':job.limits(),'preflight':snapshot,'command':command}
  atomic(manifest,record)
  if args.allow_physical_pressure:
   record['allow_physical_pressure']=True;atomic(manifest,record)
  watch_args=[sys.executable,str(Path(__file__).with_name('emulator_watchdog_v36.py')),'--manifest',str(manifest),'--telemetry',str(telemetry),'--receipt',str(watch_receipt),'--ready',str(ready),'--soft-job-gib',str(args.soft_memory_gib),'--aggregate-qemu-gib',str(max(12,args.hard_memory_gib+1)),'--min-physical-available-gib',str(args.min_physical_available_gib),'--timeout-seconds',str(args.duration_seconds),'--poll-seconds','1']
  if args.allow_physical_pressure:watch_args+=['--allow-physical-pressure']
  with (REGISTRY/(stamp+'.watchdog-errors.log')).open('wb') as errors:
   watchdog=subprocess.Popen(watch_args,stdout=subprocess.DEVNULL,stderr=errors,creationflags=0x08000000)
  deadline=time.monotonic()+10
  while not ready.is_file():
   if watchdog.poll() is not None:raise RuntimeError('Independent watchdog failed before launch; emulator remained suspended')
   if time.monotonic()>deadline:raise RuntimeError('Watchdog verification timed out; emulator remained suspended')
   time.sleep(.05)
  proof=json.loads(ready.read_text())
  if proof.get('verified') is not True or proof.get('job_name')!=job.name:raise RuntimeError('Independent guard handshake differs')
  job.resume()
  ReleaseMutex(mutex);acquired=False
  print(json.dumps({'started':True,'manifest':str(manifest),'watchdog':str(watch_receipt),'telemetry':str(telemetry),'job':job.name,'root_pid':target['pid'],'hard_gib':args.hard_memory_gib,'soft_gib':args.soft_memory_gib,'duration_seconds':args.duration_seconds}),flush=True)
  start=time.monotonic()
  while job.members():
   if watchdog.poll() is not None:
    job.terminate();raise RuntimeError('Watchdog ended; owned emulator stopped fail-closed. See '+str(watch_receipt))
   if time.monotonic()-start>args.duration_seconds+5:
    job.terminate();raise RuntimeError('Launcher duration backstop reached; owned emulator stopped')
   time.sleep(.25)
  if watchdog.poll() is None:watchdog.wait(timeout=5)
  finished=json.loads(watch_receipt.read_text())
  if finished.get('status')!='job_completed':raise RuntimeError('Guard stopped the test: '+json.dumps(finished.get('last_snapshot',{}).get('decision',finished.get('error',finished.get('status')))))
  atomic(receipt,{'status':'ended','manifest':str(manifest),'watchdog':str(watch_receipt),'job_limits':job.limits()})
  return 0
 except BaseException as e:
  if job:
   try:job.terminate()
   except OSError:pass
  if receipt:atomic(receipt,{'status':'failed','error':str(e),'record':record})
  raise
 finally:
  if job:job.close()
  if acquired:ReleaseMutex(mutex)
  Close(mutex)
if __name__=='__main__':
 try:sys.exit(main())
 except Exception as e:print('GUARDED LAUNCH REFUSED/STOPPED:',e,file=sys.stderr);sys.exit(125)
