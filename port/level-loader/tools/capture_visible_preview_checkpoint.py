"""Bind the visible preview checks to this APK, native frame log, and pixels."""
import hashlib,json,pathlib,re,subprocess,zipfile
from collections import Counter
from PIL import Image
root=pathlib.Path(__file__).resolve().parents[3]
assert root==pathlib.Path(r'C:\Users\adamc\.codex\worktrees\generic-level-loader\DH_sc')
reports=root/'port/level-loader/reports'
base=[str(pathlib.Path.home()/'AppData/Local/Android/Sdk/platform-tools/adb.exe'),'-s','emulator-5590']
def run(args):return subprocess.check_output(base+args,timeout=15).decode(errors='replace')
assert run(['emu','avd','name']).replace('\r','').splitlines()[0]=='DH2_Loader_API37'
pid=run(['shell','pidof','local.dh2.loader']).strip();assert pid.isdigit(),pid
log=run(['logcat','-d','--pid='+pid,'-v','threadtime','DH2Loader:I','AndroidRuntime:E','*:S'])
assert 'FATAL EXCEPTION' not in log and 'FRAME_FAILED' not in log
logpath=reports/'visible-preview-logcat.txt';logpath.write_text(log,encoding='utf-8')
frames=re.findall(r'MAP_FRAME_OK identity=(\S+) modules=(\d+) meshes=(\d+) draws=(\d+) triangles=(\d+) textures=(\d+) viewport=(\d+)x(\d+) gameplay=0 objects=0',log)
assert {'SWAMP','RED_DESERT_HUB','ICY_HUB'}<=set(row[0] for row in frames)
window=json.loads((reports/'visible-emulator-window.json').read_text())
assert any(row['visible'] for row in window['windows'])
catalogpath=root/'port/android-native/app/src/main/assets/loader-map-catalog.json'
catalog=json.loads(catalogpath.read_text());assert len(catalog['maps'])==16
apk=root/'port/android-native/app/build/outputs/apk/debug/app-debug.apk'
def digest(path):return hashlib.sha256(path.read_bytes()).hexdigest()
installed=json.loads((reports/'preview-apk.json').read_text());assert installed['sha256']==digest(apk)
pixels=[]
for label in ('swamp','red-desert','icy-hub','icy-module','failed-retains-icy','swamp-ready'):
    path=reports/('visible-loader-'+label+'.png')
    with Image.open(path) as image:
        assert image.size==(2400,1080)
        # GL surface bounds observed in this build's Android UI hierarchy.
        colors=Counter(image.convert('RGB').crop((0,269,2400,765)).getdata())
        different=sum(colors.values())-colors.most_common(1)[0][1]
        assert different>10000,(label,different)
    pixels.append({'screenshot':str(path.relative_to(root)),'sha256':digest(path),'surface_non_background_pixels':different})
sources=['port/android-native/app/src/main/java/com/example/dh2/LoaderPreviewActivity.java',
         'port/android-native/app/src/main/assets/loader-map-catalog.json',
         'port/android-native/app/build.gradle.kts','port/level-loader/android/loader_preview.cpp',
         'port/level-loader/VISIBLE-PREVIEW-HANDOFF.md']
sources += ['port/level-loader/tools/'+name+'.py' for name in
            ('build_preview','start_preview_emulator','preview_device','check_visible_preview','capture_visible_preview_checkpoint')]
with zipfile.ZipFile(apk) as archive:
    assert json.loads(archive.read('assets/loader-map-catalog.json'))==catalog
    libraries={name:hashlib.sha256(archive.read(name)).hexdigest() for name in archive.namelist()
               if name.endswith('/libdh2_loader_preview.so')}
    assert len(libraries)==2,libraries
receipt={'validation':'PASS','scope':'Visible fixed-map inspection, not full loader or gameplay',
         'serial':'emulator-5590','app_id':'local.dh2.loader','pid':int(pid),'window':window,
         'apk_sha256':digest(apk),'packaged_libraries':libraries,'catalog':catalog,
         'source_sha256':{name:digest(root/name) for name in sources},
         'frame_log_sha256':digest(logpath),'frames':frames,'pixels':pixels,
         'manual_ui_checks':['picker Swamp -> Red Desert Hub -> Icy Hub -> Swamp',
                             'module focus and zoom','failed load retains Icy Hub','reload uses selected Icy Hub'],
         'procedural_layout_rendered':False,'runtime_mobs_chests_rendered':False,'full_loader_verified':False}
(reports/'visible-preview-checkpoint.json').write_text(json.dumps(receipt,indent=2)+'\n')
print(json.dumps({'validation':'PASS','visible':True,'maps':sorted(set(row[0] for row in frames)),
                  'catalog_maps':len(catalog['maps']),'screenshots':len(pixels),'apk_sha256':digest(apk)}))
