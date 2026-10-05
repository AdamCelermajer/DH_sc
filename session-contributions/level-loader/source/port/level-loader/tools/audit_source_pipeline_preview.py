"""Observe current fixed/generated map frames after cached traversal integration.

This bounded regression does not instantiate objects, resolve conditions or
test campaign saves. Every device command targets and checks the private AVD.
"""
import hashlib, io, json, pathlib, re, subprocess, time, xml.etree.ElementTree as ET
from PIL import Image, ImageChops

root = pathlib.Path(__file__).resolve().parents[3]
assert root == pathlib.Path(r'C:\Users\adamc\.codex\worktrees\generic-level-loader\DH_sc')
reports = root/'port/level-loader/reports'
adb = str(pathlib.Path.home()/'AppData/Local/Android/Sdk/platform-tools/adb.exe')
base = [adb, '-s', 'emulator-5590']
def run(args, timeout=35):
    assert subprocess.check_output(base+['emu','avd','name'], timeout=15).decode().replace('\r','').splitlines()[0] == 'DH2_Loader_API37'
    return subprocess.check_output(base+args, timeout=timeout)
assert run(['shell','getprop','sys.boot_completed']).decode().strip() == '1'
def logs():
    return run(['logcat','-d','-v','threadtime','DH2Loader:I','AndroidRuntime:E','*:S']).decode(errors='replace')
def wait_frame(before, marker):
    deadline = time.monotonic()+35
    while time.monotonic() < deadline:
        current = logs()
        # Search only newly appended output; historical errors are not this run.
        delta = current[len(before):] if current.startswith(before) else current
        if marker in delta and 'MAP_FRAME_OK' in delta:
            assert 'FRAME_FAILED' not in delta and 'FATAL EXCEPTION' not in delta, delta
            return delta
        time.sleep(.25)
    raise RuntimeError('No new map frame: '+marker)
def hierarchy():
    path = '/data/local/tmp/source-pipeline-ui.xml'
    run(['shell','uiautomator','dump',path])
    return ET.fromstring(run(['exec-out','cat',path]))
def tap(text):
    node = next(n for n in hierarchy().iter('node') if n.get('text') == text)
    left, top, right, bottom = map(int,re.findall(r'\d+',node.get('bounds')))
    run(['shell','input','tap',str((left+right)//2),str((top+bottom)//2)])
def capture(label):
    raw = run(['exec-out','screencap','-p'])
    assert raw.startswith(b'\x89PNG\r\n\x1a\n')
    path = reports/(label+'.png'); path.write_bytes(raw)
    return path, Image.open(io.BytesIO(raw)).convert('RGB')
def start(identity, definition, seed=0):
    before = logs()
    run(['shell','am','force-stop','local.dh2.loader'])
    run(['shell','am','start','-n','local.dh2.loader/com.example.dh2.LoaderPreviewActivity',
         '--es','level',identity,'--es','definition',definition,'--ei','seed',str(seed)])
    delta = wait_frame(before, 'MAP_FRAME_OK identity='+identity+' ')
    ui = hierarchy()
    status = next(n.get('text') for n in ui.iter('node') if 'mesh instances' in n.get('text',''))
    assert status.startswith(identity+' ') and 'mobs/chests pending' in status, status
    path, image = capture('source-pipeline-'+identity.lower())
    # Native frame + actual image + explicit status; no inference about actors.
    row = {'identity':identity,'definition':definition,'seed':seed,'status':status,
           'frame_log':delta,'screenshot':path.name}
    print(json.dumps({'identity':identity,'status':status}), flush=True)
    return row, image, ui

catalog = json.loads((reports/'source-pipeline-fixed-sources-host.json').read_text())
generated = json.loads((reports/'source-pipeline-procedural-maps-host.json').read_text())
fixed = next(x for x in catalog['levels'] if x['name']=='DARKWOOD')
random = next(x for x in generated['levels'] if x['identity']=='SWAMP_02')
steps = []
for identity, definition in [('DARKWOOD',fixed['file']),('SWAMP_02',random['definition']),('SWAMP','001_swamp.mlx')]:
    row, image, ui = start(identity,definition); steps.append(row)
    assert 'gameplay=0 objects=0' in row['frame_log']
before = logs(); tap('RELOAD'); wait_frame(before,'MAP_FRAME_OK identity=SWAMP ')
previous_path, previous = capture('source-pipeline-swamp-reloaded')
status = next(n for n in ui.iter('node') if 'mesh instances' in n.get('text',''))
bottom = int(re.findall(r'\d+',status.get('bounds'))[-1])
top = min(int(re.findall(r'\d+',n.get('bounds'))[1]) for n in ui.iter('node') if n.get('text')=='WHOLE MAP')
before = logs(); tap('FAILED-LOAD CHECK')
delta = wait_frame(before,'Preparation failed; previous map retained:')
failed_path, failed = capture('source-pipeline-swamp-failed-retained')
rect = (0,bottom,previous.width,top)
assert ImageChops.difference(previous.crop(rect),failed.crop(rect)).getbbox() is None
steps.append({'case':'missing_definition','previous_map_pixels_unchanged':True,
              'compared_rect':rect,'frame_log':delta,'screenshot':failed_path.name})
before = logs(); tap('RELOAD'); wait_frame(before,'MAP_FRAME_OK identity=SWAMP ')
# Leave a useful source module view visible without changing emulator window size.
tap('NEXT MODULE'); module_ui = hierarchy()
module_status = next(n.get('text') for n in module_ui.iter('node') if n.get('text','').startswith('Module '))
assert module_status.startswith('Module 0 |'), module_status
path, _ = capture('source-pipeline-swamp-module0')
steps.append({'case':'module_focus','status':module_status,'screenshot':path.name})
sha = lambda p: hashlib.sha256(p.read_bytes()).hexdigest()
logpath = reports/'source-pipeline-preview-logcat.txt'; logpath.write_text(logs(),encoding='utf-8')
apk = root/'port/android-native/app/build/outputs/apk/debug/app-debug.apk'
installed = json.loads((reports/'preview-apk.json').read_text())
assert installed['sha256'] == sha(apk)
report = {'validation':'PASS','scope':__doc__,'serial':'emulator-5590','avd':'DH2_Loader_API37',
          'app_id':'local.dh2.loader','apk_sha256':sha(apk),'audit_source_sha256':sha(pathlib.Path(__file__)),
          'steps':steps,'screenshots':{p.name:sha(p) for p in reports.glob('source-pipeline-*.png')},
          'logcat_sha256':sha(logpath),'runtime_objects_verified':False,
          'mob_and_chest_rendering_verified':False,'full_loader_verified':False}
(reports/'source-pipeline-preview.json').write_text(json.dumps(report,indent=2)+'\n')
print(json.dumps({'validation':'PASS','steps':len(steps),'full_loader_verified':False}),flush=True)
