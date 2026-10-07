"""Root-run, emulator5554-only bounded attribution capture; default is dry-run.

Never starts/resets ADB, an emulator or app, installs/root-enables, changes
settings, stops other sessions or applies source patches. No automatic retry.
"""
from pathlib import Path
import argparse,subprocess,socket,json,hashlib,time,uuid,os
ROOT=Path(__file__).resolve().parents[3]
CONFIG=ROOT/'port/android-native/reports/present-attribution-v42/capture.textproto'
MAX_BYTES=8*1024**2
def validate_config(text):
 for expected in ['duration_ms: 20000','size_kb: 8192','write_into_file: true','max_file_size_bytes: 8388608']:
  if expected not in text:raise ValueError('Missing exact bound: '+expected)
 if any(word in text for word in ['exclusive_prio:','deferred_start:','trigger_config','output_path:']):raise ValueError('Unsupported session behavior')
 return True
def parse_stat(raw):
 try:
  opening=raw.index('(');closing=raw.rindex(')');pid=int(raw[:opening].strip());words=raw[closing+1:].split()
  return {'available':True,'pid':pid,'comm':raw[opening+1:closing],'state':words[0],'minor_faults':int(words[7]),'major_faults':int(words[9]),'user_ticks':int(words[11]),'kernel_ticks':int(words[12]),'threads':int(words[17]),'start_ticks':int(words[19])}
 except (ValueError,IndexError):return {'available':False,'reason':'stat unreadable/format unavailable'}
def main():
 parser=argparse.ArgumentParser(description=__doc__);parser.add_argument('--capture',action='store_true');parser.add_argument('--serial',choices=['emulator-5554'],default='emulator-5554');args=parser.parse_args()
 config=CONFIG.read_text();validate_config(config)
 if not args.capture:print(json.dumps({'dry_run':True,'no_ADB_or_device_calls':True,'serial':args.serial,'duration_ms':20000,'trace_file_max_bytes':MAX_BYTES,'config':str(CONFIG)}));return 0
 # ADB normally auto-starts its server. Refuse absent daemon instead.
 with socket.create_connection(('127.0.0.1',5037),timeout=2):pass
 adb=Path(os.environ.get('ANDROID_HOME',str(Path.home()/'AppData/Local/Android/Sdk')))/'platform-tools/adb.exe'
 stamp=time.strftime('%Y%m%d-%H%M%S')+'-'+uuid.uuid4().hex[:8];out=ROOT/'.local-inputs/present-attribution-v42'/stamp;out.mkdir(parents=True)
 base=[str(adb),'-s',args.serial];calls=[]
 def run(tail,timeout=5,input=None,required=True):
  p=subprocess.run(base+tail,capture_output=True,text=True,input=input,timeout=timeout);calls.append({'argv':base+tail,'exit_code':p.returncode,'stdout':p.stdout[-65536:],'stderr':p.stderr[-65536:]})
  if required and p.returncode:raise RuntimeError(str(tail)+p.stderr)
  return p
 record={'status':'PENDING','serial':args.serial,'utc_begin':time.strftime('%Y-%m-%dT%H:%M:%SZ',time.gmtime()),'config_sha256':hashlib.sha256(config.encode()).hexdigest(),'max_bytes':MAX_BYTES,'max_duration_ms':20000,'no_app_emulator_ADB_launch':True,'calls':calls}
 try:
  if run(['get-state']).stdout.strip()!='device':raise RuntimeError('Existing emulator not ready')
  if run(['shell','getprop','ro.kernel.qemu']).stdout.strip()!='1':raise RuntimeError('Target is not an emulator; no capture')
  pid_text=run(['shell','pidof','com.example.dh2']).stdout.split()
  if len(pid_text)!=1 or not pid_text[0].isdigit():raise RuntimeError('One existing game process required; no app launch')
  pid=int(pid_text[0]);record['guest_pid']=pid
  record['before_stat']=parse_stat(run(['shell','cat',f'/proc/{pid}/stat'],required=False).stdout)
  record['atrace_categories']=run(['shell','atrace','--list_categories'],required=False).stdout
  remote='/data/misc/perfetto-traces/dh2-present-v42-'+stamp+'.pftrace';record['remote_trace']=remote
  # The service enforces both file and duration caps even if the local client
  # exits. Never kill global perfetto/traced or enable new global settings.
  p=run(['shell','perfetto','--txt','-c','-','-o',remote],timeout=35,input=config)
  record['perfetto_stdout']=p.stdout;record['perfetto_stderr']=p.stderr
  size_text=run(['shell','stat','-c','%s',remote]).stdout.strip()
  if not size_text.isdigit() or not 0<int(size_text)<=MAX_BYTES:raise RuntimeError('Trace size violates bound or no trace; no pull')
  trace=out/'capture.pftrace';run(['pull',remote,str(trace)],timeout=10)
  if trace.stat().st_size!=int(size_text) or trace.stat().st_size>MAX_BYTES:raise RuntimeError('Pulled trace size differs')
  record['trace_bytes']=trace.stat().st_size;record['trace_sha256']=hashlib.sha256(trace.read_bytes()).hexdigest()
  record['after_stat']=parse_stat(run(['shell','cat',f'/proc/{pid}/stat'],required=False).stdout)
  if run(['shell','pidof','com.example.dh2']).stdout.split()!=[str(pid)]:raise RuntimeError('Game process disappeared/changed; attribution stale')
  before,after=record['before_stat'],record['after_stat']
  if before.get('available') and after.get('available') and before['start_ticks']!=after['start_ticks']:raise RuntimeError('Game process changed; attribution stale')
  record['status']='CAPTURED_NOT_ANALYZED';record['unavailable_swap_is_not_zero']=True
 except Exception as error:record['status']='FAILED_NO_RETRY';record['error']=str(error)
 finally:
  record['utc_end']=time.strftime('%Y-%m-%dT%H:%M:%SZ',time.gmtime());(out/'receipt.json').write_text(json.dumps(record,indent=2)+'\n');print(json.dumps({'status':record['status'],'receipt':str(out/'receipt.json'),'error':record.get('error')}))
 return 0 if record['status']=='CAPTURED_NOT_ANALYZED' else 1
if __name__=='__main__':raise SystemExit(main())
