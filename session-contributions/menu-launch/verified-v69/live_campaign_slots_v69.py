from pathlib import Path
import subprocess,json,time,hashlib,re
from PIL import Image
H=Path(__file__).parent;R=Path('R:/');out=H/'campaign-menu-tests-v69';out.mkdir(exist_ok=True)
adb=[r'C:\Users\adamc\AppData\Local\Android\Sdk\platform-tools\adb.exe','-P','5038','-s','emulator-5580']
def call(*args):return subprocess.check_output(adb+list(args),encoding='utf8',timeout=45)
def shot(name):(out/(name+'.png')).write_bytes(subprocess.check_output(adb+['exec-out','screencap','-p'],timeout=30))
def tap(x,y):
    shot('tap-surface')
    with Image.open(out/'tap-surface.png') as im:w,h=im.size
    cw=min(w,h*3/2);ch=min(h,w*2/3)
    call('shell','input','tap',str(round((w-cw)/2+x*cw/480)),str(round((h-ch)/2+y*ch/320)))
apk=R/'port/android-native/app/build/outputs/apk/debug/app-debug.apk'
wm=call('shell','wm','size');override=re.search(r'Override size: (\d+x\d+)',wm)
names=['dh2_000.savegame','dh2_002.savegame.bak','dh2_003.savegame']
existing=call('shell','run-as','com.example.dh2','ls','files').splitlines()
assert not any(name.startswith('dh2_0') for name in existing),existing
call('shell','am','force-stop','com.example.dh2')
(out/'before-files.tar').write_bytes(subprocess.check_output(adb+['exec-out','run-as','com.example.dh2','tar','-cf','-','files'],timeout=45))
gold=json.loads((H/'profile-date-original-v59.json').read_text());assert gold['status']=='PASS'
report={'status':'FAIL','apk_sha256':hashlib.sha256(apk.read_bytes()).hexdigest(),'profile_scope':'Temporary source-captured fresh profiles; no gameplay initialization/start proof','files':[],'surfaces':[]}
try:
    call('install','-r',str(apk))
    for name,index in zip(names,[1,4,7]):
        data=bytes.fromhex(gold['cases'][index]['file_bytes']);local=out/name;local.write_bytes(data)
        remote='/data/local/tmp/v69-'+name;call('push',str(local),remote)
        call('shell','run-as','com.example.dh2','cp',remote,'files/'+name)
        report['files'].append({'name':name,'class':gold['cases'][index]['class_name'],'sha256':hashlib.sha256(data).hexdigest()})
    call('shell','am','start','-W','-n','com.example.dh2/.MainActivity','--es','front_screen','main');time.sleep(6)
    pid=call('shell','pidof','com.example.dh2').strip();assert pid
    for size in ['1080x1920','1080x2400','1968x2184']:
        call('shell','wm','size',size);time.sleep(2);shot('occupied-'+size)
        call('shell','am','broadcast','-a','com.example.dh2.DEBUG_MENU_SOUND','-p','com.example.dh2','--es','probe','inspect-front');time.sleep(.4)
        assert call('shell','pidof','com.example.dh2').strip()==pid
        report['surfaces'].append({'override':size,'same_pid':pid})
    for n in range(3):tap(145,73);time.sleep(1);shot('next-slot-'+str(n+1))
    log=call('logcat','-d','--pid='+pid,'-v','brief');(out/'validation.log').write_text(log,encoding='utf8')
    assert 'Original UI display failed:' not in log and 'Original menu input failed:' not in log
    assert 'Original occupied save-slot presentation | slot 0 | backup 0' in log
    report['backup_profile_selected']='Original occupied save-slot presentation | slot 2 | backup 1' in log
    report['third_profile_selected']='Original occupied save-slot presentation | slot 3 | backup 0' in log
    report['known_missing']=['NativeGetParsedString: occupied slot act label displays undefined; assignment/start/create/delete not connected']
    report['status']='CAPTURED; VISUAL REVIEW PENDING'
finally:
    call('shell','am','force-stop','com.example.dh2')
    for name in names:call('shell','run-as','com.example.dh2','rm','-f','files/'+name)
    call('shell','wm','size',override[1] if override else 'reset')
    call('shell','am','start','-W','-n','com.example.dh2/.MainActivity','--es','front_screen','main');time.sleep(5);shot('restored-empty-main')
    report['restored_campaign_files']=not any(name.startswith('dh2_0') for name in call('shell','run-as','com.example.dh2','ls','files').splitlines())
    (out/'validation.json').write_text(json.dumps(report,indent=2)+'\n')
print(json.dumps(report))
