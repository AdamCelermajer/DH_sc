"""Launch only the loader's named AVD on its reserved, checked ports."""
import argparse,json,pathlib,socket,subprocess,time
parser=argparse.ArgumentParser()
parser.add_argument('--headless',action='store_true',help='Hide the emulator window; visible by default')
parser.add_argument('--restart',action='store_true',help='Restart only the verified loader AVD, keeping its data')
options=parser.parse_args()
root=pathlib.Path(__file__).resolve().parents[3]
assert root==pathlib.Path(r'C:\Users\adamc\.codex\worktrees\generic-level-loader\DH_sc')
avd=pathlib.Path.home()/'.android/avd/DH2_Loader_API37.avd/config.ini'
config={key.strip():value.strip() for line in avd.read_text().splitlines()
        if '=' in line for key,value in [line.split('=',1)]}
assert config.get('AvdId')=='DH2_Loader_API37'
if options.restart:
    adb=str(pathlib.Path.home()/'AppData/Local/Android/Sdk/platform-tools/adb.exe')
    base=[adb,'-s','emulator-5590']
    name=subprocess.check_output(base+['emu','avd','name'],timeout=15).decode().replace('\r','').splitlines()
    assert name and name[0]=='DH2_Loader_API37',f'Refusing to restart another AVD: {name}'
    subprocess.check_call(base+['emu','kill'],timeout=15)
    deadline=time.monotonic()+30
    while True:
        busy=False
        for port in (5590,5591):
            with socket.socket() as probe:
                try:probe.bind(('127.0.0.1',port))
                except OSError:busy=True
        if not busy:break
        if time.monotonic()>=deadline:raise TimeoutError('Loader ports still occupied; no launch attempted')
        time.sleep(.5)
guards=[]
try:
    for port in (5590,5591):
        guard=socket.socket();guard.bind(('127.0.0.1',port));guards.append(guard)
finally:
    for guard in guards:guard.close()
output=root.parent/'build/emulator';output.mkdir(parents=True,exist_ok=True)
emulator=pathlib.Path.home()/'AppData/Local/Android/Sdk/emulator/emulator.exe'
args=[str(emulator),'-avd','DH2_Loader_API37','-port','5590',
      '-no-audio','-no-boot-anim','-no-snapshot','-gpu','swiftshader_indirect']
if options.headless:args.append('-no-window')
with (output/'stdout.log').open('ab') as out,(output/'stderr.log').open('ab') as err:
    # Keep the visible emulator independent of a short-lived shell/job. The
    # Python helper itself has no console to preserve; the emulator owns its GUI.
    flags=subprocess.DETACHED_PROCESS|subprocess.CREATE_NEW_PROCESS_GROUP|subprocess.CREATE_BREAKAWAY_FROM_JOB
    try:process=subprocess.Popen(args,stdout=out,stderr=err,creationflags=flags)
    except OSError as error:
        if error.winerror!=5:raise
        # Some host jobs prohibit breakaway. Record that limitation explicitly.
        flags=subprocess.DETACHED_PROCESS|subprocess.CREATE_NEW_PROCESS_GROUP
        process=subprocess.Popen(args,stdout=out,stderr=err,creationflags=flags)
receipt={'pid':process.pid,'avd':'DH2_Loader_API37','serial':'emulator-5590','headless':options.headless,
         'command':args,'creation_flags':flags,'breakaway_from_job':bool(flags&subprocess.CREATE_BREAKAWAY_FROM_JOB)}
(output/'launch.json').write_text(json.dumps(receipt,indent=2)+'\n')
print(json.dumps(receipt))
