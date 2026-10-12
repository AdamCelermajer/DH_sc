"""Exercise visible in-process map/seed switching and failed-candidate retention.

This uses only the private inspection activity on the loader-owned emulator.
"""
import collections,hashlib,json,pathlib,re,subprocess,time,xml.etree.ElementTree as ET
from PIL import Image
ROOT=pathlib.Path(__file__).resolve().parents[3];REPORTS=ROOT/'port/level-loader/reports'
assert ROOT==pathlib.Path(r'C:\Users\adamc\.codex\worktrees\generic-level-loader\DH_sc')
BASE=[str(pathlib.Path.home()/'AppData/Local/Android/Sdk/platform-tools/adb.exe'),'-s','emulator-5590']
def run(args):return subprocess.check_output(BASE+args,timeout=30)
def sha(path):return hashlib.sha256(path.read_bytes()).hexdigest()
assert run(['emu','avd','name']).decode().replace('\r','').splitlines()[0]=='DH2_Loader_API37'
apk=ROOT/'port/android-native/app/build/outputs/apk/debug/app-debug.apk'
assert json.loads((REPORTS/'preview-apk.json').read_text())['sha256']==sha(apk)
catalog=json.loads((ROOT/'port/android-native/app/src/main/assets/loader-map-catalog.json').read_text())['maps']
labels=[row['label'] for row in catalog];by_identity={row['identity']:row for row in catalog}
def hierarchy():
    path='/data/local/tmp/loader-generated-picker.xml';run(['shell','uiautomator','dump',path])
    return list(ET.fromstring(run(['exec-out','cat',path])).iter('node'))
def bounds(node):return [int(value) for value in re.findall(r'\d+',node.get('bounds'))]
def tap(node):
    x1,y1,x2,y2=bounds(node);run(['shell','input','tap',str((x1+x2)//2),str((y1+y2)//2)])
def button(label):tap(next(node for node in hierarchy() if node.get('text')==label))
def select(identity,seed):
    target=by_identity[identity]['label'];nodes=hierarchy()
    selected=next(node for node in nodes if node.get('text') in labels)
    if selected.get('text')!=target:
        tap(selected);wanted=labels.index(target)
        for attempt in range(20):
            nodes=hierarchy();options=[node for node in nodes if node.get('text') in labels]
            match=[node for node in options if node.get('text')==target]
            if match:tap(match[-1]);break
            assert options,'Map popup has no catalog labels'
            order=[labels.index(node.get('text')) for node in options]
            rects=[bounds(node) for node in options];x=(min(rect[0] for rect in rects)+max(rect[2] for rect in rects))//2
            top=max(150,min(rect[1] for rect in rects)+30);bottom=min(1000,max(rect[3] for rect in rects)-30)
            assert bottom>top
            # Small, slow gestures avoid a fling jumping across the desired row.
            start,end=(top,min(top+320,bottom)) if wanted<min(order) else (bottom,max(bottom-320,top))
            run(['shell','input','swipe',str(x),str(start),str(x),str(end),'550'])
        else:raise AssertionError('Unable to select '+identity)
    nodes=hierarchy();selected_seed=next(node for node in nodes if node.get('text') in ('Seed 0','Seed 1'))
    if selected_seed.get('text')!='Seed '+str(seed):
        tap(selected_seed);nodes=hierarchy();tap([node for node in nodes if node.get('text')=='Seed '+str(seed)][-1])
    button('LOAD MAP')
def status(nodes):return next(node.get('text') for node in nodes if node.get('text','').startswith(('Preparation failed','SWAMP','GOTHICUS','Module ','Whole map')))
checks=[]
def capture(label,expected,failed=False,compare=None):
    nodes=hierarchy();text=status(nodes)
    assert text.startswith('Preparation failed')==failed,(label,text)
    if failed:assert 'previous map retained' in text
    else:assert expected in text,(label,text)
    data=run(['exec-out','screencap','-p']);path=REPORTS/('generated-picker-'+label+'.png');path.write_bytes(data)
    surface=(0,bounds(next(node for node in nodes if node.get('text')=='LOAD MAP'))[3],
             2400,bounds(next(node for node in nodes if node.get('text')=='WHOLE MAP'))[1])
    with Image.open(path) as image:
        crop=image.convert('RGB').crop(surface);colors=collections.Counter(crop.get_flattened_data());nonbackground=sum(colors.values())-colors.most_common(1)[0][1]
        assert nonbackground>10000
        if compare:
            with Image.open(REPORTS/compare) as other:assert crop.tobytes()==other.convert('RGB').crop(surface).tobytes(),label
    row={'check':label,'status':text,'screenshot':path.name,'screenshot_sha256':sha(path),
         'surface_bounds':list(surface),'surface_non_background_pixels':nonbackground,
         'same_surface_as':compare,'previous_map_retained':failed}
    checks.append(row)
    (REPORTS/'generated-picker-progress.json').write_text(json.dumps({'apk_sha256':sha(apk),'checks':checks},indent=2)+'\n')
    print(json.dumps({'check':label,'status':text}),flush=True);return path.name
select('SWAMP_02',0);seed0=capture('swamp-return-seed0','SWAMP_02 seed 0')
button('RELOAD');capture('swamp-return-reload','SWAMP_02 seed 0',compare=seed0)
select('SWAMP_02',1);seed1=capture('swamp-return-seed1','SWAMP_02 seed 1');assert sha(REPORTS/seed0)!=sha(REPORTS/seed1)
button('FAILED-LOAD CHECK');capture('missing-file-retains-seed1','',failed=True,compare=seed1)
select('ICY_CAVERN_02',0);capture('no-layout-retains-seed1','',failed=True,compare=seed1)
button('RELOAD');capture('reload-keeps-active-seed1','SWAMP_02 seed 1',compare=seed1)
select('SWAMP_CAVE_TROLL_A',0);capture('troll-cave','SWAMP_CAVE_TROLL_A seed 0')
select('GOTHICUS_CRYPT_01',0);capture('crypt','GOTHICUS_CRYPT_01 seed 0')
select('SWAMP',0);swamp=capture('swamp-fixed','SWAMP |')
button('NEXT MODULE');capture('module-focus','Module 0')
button('ZOOM +');capture('module-zoom','Module 0')
button('WHOLE MAP');capture('swamp-ready','Whole map',compare=swamp)
pid=run(['shell','pidof','local.dh2.loader']).decode().strip();assert pid.isdigit()
log=run(['logcat','-d','--pid='+pid,'-v','threadtime','DH2Loader:I','AndroidRuntime:E','*:S']).decode(errors='replace')
assert 'FATAL EXCEPTION' not in log and 'FRAME_FAILED' not in log
assert 'identity=SWAMP_02 procedural=1 seed=0' in log and 'identity=SWAMP_02 procedural=1 seed=1' in log
log_path=REPORTS/'generated-picker-logcat.txt';log_path.write_text(log,encoding='utf-8')
report={'validation':'PASS','scope':__doc__,'serial':'emulator-5590','app_id':'local.dh2.loader','pid':int(pid),
        'apk_sha256':sha(apk),'script_sha256':sha(pathlib.Path(__file__)),'logs_sha256':sha(log_path),'checks':checks,
        'runtime_objects_verified':False,'campaign_transitions_verified':False,'full_loader_verified':False}
(REPORTS/'generated-picker-checks.json').write_text(json.dumps(report,indent=2)+'\n')
print(json.dumps({'validation':'PASS','checks':len(checks),'left_visible':'SWAMP'}))
