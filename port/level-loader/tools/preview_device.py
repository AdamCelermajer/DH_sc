"""Every device operation is explicitly targeted and verifies the loader AVD."""
import argparse,hashlib,json,pathlib,subprocess,sys,xml.etree.ElementTree as ET
root=pathlib.Path(__file__).resolve().parents[3]
assert root==pathlib.Path(r'C:\Users\adamc\.codex\worktrees\generic-level-loader\DH_sc')
adb=str(pathlib.Path.home()/'AppData/Local/Android/Sdk/platform-tools/adb.exe')
serial='emulator-5590';base=[adb,'-s',serial]
parser=argparse.ArgumentParser();parser.add_argument('mode',choices=['state','install','launch','restore_swamp','capture','tap','key','logs','hierarchy'])
parser.add_argument('args',nargs='*');options=parser.parse_args()
def run(args,timeout=45):
    return subprocess.check_output(base+args,timeout=timeout)
name=run(['emu','avd','name']).decode().replace('\r','').splitlines()
assert name and name[0]=='DH2_Loader_API37', f'Refusing wrong AVD: {name}'
boot=run(['shell','getprop','sys.boot_completed']).decode().strip()
assert boot=='1',f'Loader AVD not booted: {boot}'
reports=root/'port/level-loader/reports';reports.mkdir(exist_ok=True)
if options.mode=='state':print(json.dumps({'avd':name[0],'serial':serial,'booted':True}))
elif options.mode=='install':
    apk=root/'port/android-native/app/build/outputs/apk/debug/app-debug.apk'
    print(run(['install','-r',str(apk)],timeout=120).decode())
    (reports/'preview-apk.json').write_text(json.dumps({'apk':str(apk),'sha256':hashlib.sha256(apk.read_bytes()).hexdigest(),'serial':serial,'avd':name[0],'app_id':'local.dh2.loader'},indent=2)+'\n')
elif options.mode=='launch':
    print(run(['shell','am','start','-n','local.dh2.loader/com.example.dh2.LoaderPreviewActivity']+options.args).decode())
elif options.mode=='restore_swamp':
    run(['shell','am','force-stop','local.dh2.loader'])
    print(run(['shell','am','start','-n','local.dh2.loader/com.example.dh2.LoaderPreviewActivity','--es','level','SWAMP','--es','definition','001_swamp.mlx']).decode())
elif options.mode=='capture':
    label=options.args[0] if options.args else 'swamp-map-overview'
    assert label.replace('-','').replace('_','').isalnum()
    data=run(['exec-out','screencap','-p']);assert data.startswith(b'\x89PNG\r\n\x1a\n')
    path=reports/(label+'.png');path.write_bytes(data);print(path)
elif options.mode=='tap':
    assert len(options.args)==2 and all(x.isdigit() for x in options.args)
    print(run(['shell','input','tap']+options.args).decode())
elif options.mode=='key':
    assert len(options.args)==1 and options.args[0] in ('3','4','82')
    print(run(['shell','input','keyevent']+options.args).decode())
elif options.mode=='logs':
    data=run(['logcat','-d','-v','threadtime','DH2Loader:I','AndroidRuntime:E','*:S']).decode(errors='replace')
    (reports/'preview-logcat.txt').write_text(data,encoding='utf-8');print(data[-10000:])
elif options.mode=='hierarchy':
    path='/data/local/tmp/loader-preview-ui.xml'
    run(['shell','uiautomator','dump',path]);data=run(['exec-out','cat',path])
    (reports/'preview-ui.xml').write_bytes(data)
    print(json.dumps([{'text':n.get('text'),'bounds':n.get('bounds')} for n in ET.fromstring(data).iter('node') if n.get('text')]))
