"""Read source-render evidence and exercise only the verified private loader AVD."""
import hashlib,io,json,pathlib,re,subprocess,time,xml.etree.ElementTree as ET
from PIL import Image,ImageChops
ROOT=pathlib.Path(__file__).resolve().parents[3]
assert ROOT==pathlib.Path(r'C:\Users\adamc\.codex\worktrees\generic-level-loader\DH_sc')
REPORTS=ROOT/'port/level-loader/reports'
ADB=str(pathlib.Path.home()/'AppData/Local/Android/Sdk/platform-tools/adb.exe')
BASE=[ADB,'-s','emulator-5590']
def run(args,timeout=30):return subprocess.check_output(BASE+args,timeout=timeout)
assert run(['emu','avd','name']).decode().replace('\r','').splitlines()[0]=='DH2_Loader_API37'
assert run(['shell','getprop','sys.boot_completed']).decode().strip()=='1'
def logs():return run(['logcat','-d','-v','threadtime','DH2Loader:I','AndroidRuntime:E','*:S']).decode(errors='replace')
def count_frames():return logs().count('MAP_FRAME_OK')
def settle(previous,expected):
    end=time.monotonic()+30
    while time.monotonic()<end:
        current=logs()
        if current.count('MAP_FRAME_OK')>previous and expected in current:
            assert 'FRAME_FAILED' not in current and 'FATAL EXCEPTION' not in current
            return current
        time.sleep(.2)
    raise RuntimeError('No new native frame: '+expected)
def hierarchy():
    target='/data/local/tmp/loader-preview-audit.xml'
    run(['shell','uiautomator','dump',target]);return ET.fromstring(run(['exec-out','cat',target]))
def tap(label):
    node=next(n for n in hierarchy().iter('node') if n.get('text')==label)
    x1,y1,x2,y2=map(int,re.findall(r'\d+',node.get('bounds')))
    run(['shell','input','tap',str((x1+x2)//2),str((y1+y2)//2)])
def capture(name):
    data=run(['exec-out','screencap','-p']);assert data.startswith(b'\x89PNG\r\n\x1a\n')
    path=REPORTS/(name+'.png');path.write_bytes(data);return Image.open(io.BytesIO(data)).convert('RGB')
steps=[]
before=count_frames()
run(['shell','am','force-stop','local.dh2.loader'])
run(['shell','am','start','-n','local.dh2.loader/com.example.dh2.LoaderPreviewActivity'])
settle(before,'identity=SWAMP modules=9 meshes=369')
ui=hierarchy();status=next(n for n in ui.iter('node') if '369 mesh instances' in n.get('text',''))
status_bottom=int(re.findall(r'\d+',status.get('bounds'))[-1])
buttons_top=min(int(re.findall(r'\d+',n.get('bounds'))[1]) for n in ui.iter('node') if n.get('text')=='WHOLE MAP')
initial=capture('swamp-map-overview')
steps.append({'case':'initial_map','new_native_frame':True,'screenshot':'swamp-map-overview.png'})
print('Initial Swamp frame captured',flush=True)
for index in range(9):
    previous=count_frames();tap('NEXT MODULE')
    # Focus does not republish the whole map; the UI text proves the requested
    # authored module and the following screenshot observes the focused scene.
    ui=hierarchy();text=next(n.get('text') for n in ui.iter('node') if n.get('text','').startswith('Module '))
    assert text.startswith(f'Module {index} |'),text
    capture(f'swamp-module-{index}')
    steps.append({'case':'module_focus','index':index,'status':text,'screenshot':f'swamp-module-{index}.png'})
    print('Captured module',index,flush=True)
for index in range(3):
    previous=count_frames();tap('RELOAD');settle(previous,'identity=SWAMP modules=9 meshes=369')
    steps.append({'case':'reload','iteration':index,'new_native_frame':True})
previous_image=capture('swamp-map-reloaded')
previous=count_frames();tap('FAILED-LOAD CHECK');settle(previous,'Preparation failed; previous map retained:')
failed_image=capture('swamp-map-failed-load-retained')
rect=(0,status_bottom,initial.width,buttons_top)
difference=ImageChops.difference(previous_image.crop(rect),failed_image.crop(rect))
assert difference.getbbox() is None,'Rendered map changed after failed load'
steps.append({'case':'missing_definition','new_native_frame':True,'previous_map_pixels_unchanged':True,'compared_rect':rect})
print('Reload and failed-load retention passed',flush=True)
previous=count_frames();run(['shell','input','keyevent','3'])
run(['shell','am','start','-n','local.dh2.loader/com.example.dh2.LoaderPreviewActivity'])
settle(previous,'identity=SWAMP modules=9 meshes=369');resumed=capture('swamp-map-resumed')
assert ImageChops.difference(previous_image.crop(rect),resumed.crop(rect)).getbbox() is None,'Map changed after context recreation'
steps.append({'case':'home_resume_context_recreation','new_native_frame':True,'map_pixels_unchanged':True})
current=logs();(REPORTS/'preview-logcat.txt').write_text(current,encoding='utf-8')
apk=ROOT/'port/android-native/app/build/outputs/apk/debug/app-debug.apk'
source_paths=[ROOT/'port/level-loader/android/loader_preview.cpp',ROOT/'port/android-native/app/src/main/java/com/example/dh2/LoaderPreviewActivity.java',ROOT/'port/android-native/app/src/main/cpp/CMakeLists.txt',ROOT/'port/android-native/app/build.gradle.kts',ROOT/'port/android-native/app/src/main/AndroidManifest.xml',pathlib.Path(__file__)]
digest=lambda p:hashlib.sha256(p.read_bytes()).hexdigest()
report={'validation':'PASS','scope':'Isolated Swamp map mesh/material preview and Android lifecycle checks. Original lights/effects/helper visibility, mobs/chests, gameplay and saves remain unverified.',
        'serial':'emulator-5590','avd':'DH2_Loader_API37','app_id':'local.dh2.loader','apk_sha256':digest(apk),
        'source_sha256':{p.relative_to(ROOT).as_posix():digest(p) for p in source_paths},
        'map_mesh_render_verified':True,'complete_authored_level_verified':False,'runtime_objects_verified':False,'gameplay_verified':False,'full_loader_verified':False,
        'native_frame_markers':current.count('MAP_FRAME_OK'),'steps':steps,
        'screenshots':{p.name:digest(p) for p in REPORTS.glob('swamp-*.png')},'logcat_sha256':digest(REPORTS/'preview-logcat.txt')}
(REPORTS/'swamp-map-preview.json').write_text(json.dumps(report,indent=2)+'\n')
print(json.dumps({'validation':'PASS','steps':len(steps),'full_loader_verified':False}),flush=True)
