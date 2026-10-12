"""Verify normal fixed/procedural requests, original backups and explicit repairs.

Native frames and nonblank map pixels are checked on the private loader AVD.
This does not test original lighting parity, actors, conditions or campaign saves.
"""
import pathlib,json,hashlib,subprocess,time,re,io,xml.etree.ElementTree as ET
from PIL import Image
root=pathlib.Path(__file__).resolve().parents[3]
assert root==pathlib.Path(r'C:\Users\adamc\.codex\worktrees\generic-level-loader\DH_sc')
reports=root/'port/level-loader/reports'
coverage=json.loads((reports/'loader-recovery-coverage.json').read_text());assert coverage['validation']=='PASS'
rows=[r for r in coverage['cases'] if r['kind']=='fixed' or r['seed'] in (0,1)]
assert len(rows)==86
adb=str(pathlib.Path.home()/'AppData/Local/Android/Sdk/platform-tools/adb.exe');base=[adb,'-s','emulator-5590']
def run(args,timeout=35,allow_absent=False):
    name=subprocess.check_output(base+['emu','avd','name'],timeout=15).decode().replace('\r','').splitlines()
    assert name[0]=='DH2_Loader_API37',name
    p=subprocess.run(base+args,capture_output=True,timeout=timeout)
    if not allow_absent:p.check_returncode()
    return p.stdout
def launch(row):
    run(['shell','am','force-stop','local.dh2.loader'])
    run(['shell','am','start','-n','local.dh2.loader/com.example.dh2.LoaderPreviewActivity','--es','level',row['identity'],'--es','definition',row['definition'],'--ei','seed',str(row['seed'])])
    deadline=time.monotonic()+5
    pid=''
    while time.monotonic()<deadline:
        pid=run(['shell','pidof','local.dh2.loader'],allow_absent=True).decode().strip()
        if re.fullmatch(r'\d+',pid):break
        time.sleep(.1)
    assert re.fullmatch(r'\d+',pid),'Loader process did not start'
    deadline=time.monotonic()+35
    while time.monotonic()<deadline:
        log=run(['logcat','-d','--pid='+pid,'-v','threadtime','DH2Loader:I','AndroidRuntime:E','*:S']).decode(errors='replace')
        if 'FRAME_FAILED' in log or 'FATAL EXCEPTION' in log or 'Preparation failed' in log:raise RuntimeError(log)
        if 'MAP_FRAME_OK identity='+row['identity']+' ' in log:return log
        time.sleep(.25)
    raise RuntimeError('No frame '+str(row))
def hierarchy():
    path='/data/local/tmp/loader-recovery-ui.xml';run(['shell','uiautomator','dump',path])
    return ET.fromstring(run(['exec-out','cat',path]))
apk=root/'port/android-native/app/build/outputs/apk/debug/app-debug.apk'
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest()
binding={'apk_sha256':sha(apk),'coverage_sha256':sha(reports/'loader-recovery-coverage.json'),
         'audit_source_sha256':sha(pathlib.Path(__file__))}
assert json.loads((reports/'preview-apk.json').read_text())['sha256']==binding['apk_sha256']
progress_path=reports/'map-recovery-preview-progress.json';results=[]
if progress_path.exists():
    previous=json.loads(progress_path.read_text())
    if previous.get('binding')==binding:
        for i,record in enumerate(previous['cases']):
            assert i<len(rows)
            assert tuple(record[k] for k in ('identity','definition','seed'))==tuple(rows[i][k] for k in ('identity','definition','seed'))
            assert sha(reports/record['screenshot'])==record['screenshot_sha256']
        results=previous['cases']
        print(json.dumps({'resumed_verified_cases':len(results),'total':len(rows)}),flush=True)
try:
    for i,row in enumerate(rows):
        if i<len(results):continue
        log=launch(row);expected=row['result'];identity=row['identity']
        assert 'gameplay=0 objects=0' in log
        if expected['backup_used']:assert 'SOURCE_BACKUP_SELECTED identity='+identity in log
        else:assert 'SOURCE_BACKUP_SELECTED' not in log
        repairs=len(re.findall(r'SOURCE_REFERENCE_REPAIR identity='+re.escape(identity)+r' ',log))
        assert repairs==expected['reference_repairs'],(identity,repairs,expected)
        stages=['sources','map','declarations','publish_source']
        if row['kind']=='procedural':
            stages=['procedural_sources','blocks','connections','lists','rules','layout']+(['backup_sources'] if expected['backup_used'] else ['modules','sources'])+['map','declarations','publish_source']
        observed=re.findall(r'SOURCE_PREPARATION_STAGE identity='+re.escape(identity)+r' stage=(\w+)',log)
        assert observed==stages,(identity,observed,stages)
        ui=hierarchy();nodes=list(ui.iter('node'))
        status=next(n.get('text') for n in nodes if 'mesh instances' in n.get('text',''))
        assert status.startswith(identity+' ') and 'mobs/chests pending' in status,status
        if expected['backup_used']:assert '[original backup]' in status
        if repairs:assert '[source references repaired]' in status
        surface=next(n for n in nodes if n.get('class') in ('android.view.SurfaceView','android.opengl.GLSurfaceView'))
        rect=tuple(map(int,re.findall(r'\d+',surface.get('bounds'))))
        viewport=re.search(r'viewport=(\d+)x(\d+)',log);assert viewport
        assert rect[2]-rect[0]==int(viewport[1]) and rect[3]-rect[1]==int(viewport[2]),(rect,viewport.groups())
        raw=run(['exec-out','screencap','-p']);assert raw.startswith(b'\x89PNG')
        name='map-recovery-'+identity.lower()+'-seed'+str(row['seed'])+'.png';(reports/name).write_bytes(raw)
        image=Image.open(io.BytesIO(raw)).convert('RGB').crop(rect);colors=image.getcolors(image.width*image.height)
        assert colors and len(colors)>100,(identity,'insufficient map colors')
        changed=image.width*image.height-max(colors,key=lambda c:c[0])[0];assert changed>1000,(identity,'insufficient map pixels',changed)
        results.append({'identity':identity,'definition':row['definition'],'seed':row['seed'],'backup_used':expected['backup_used'],'reference_repairs':repairs,'stages':observed,'status':status,'frame_log':log,'screenshot':name,'screenshot_sha256':hashlib.sha256(raw).hexdigest(),'map_rect':rect,'unique_map_colors':len(colors),'non_background_map_pixels':changed})
        temporary=progress_path.with_suffix('.tmp')
        temporary.write_text(json.dumps({'binding':binding,'cases':results},indent=2)+'\n')
        temporary.replace(progress_path)
        if (i+1)%10==0 or expected['backup_used'] or repairs:print(json.dumps({'completed':i+1,'total':len(rows),'identity':identity,'backup':expected['backup_used'],'repairs':repairs}),flush=True)
finally:
    launch({'identity':'SWAMP','definition':'001_swamp.mlx','seed':0})
apk=root/'port/android-native/app/build/outputs/apk/debug/app-debug.apk'
installed=json.loads((reports/'preview-apk.json').read_text());digest=hashlib.sha256(apk.read_bytes()).hexdigest();assert installed['sha256']==digest
report={'validation':'PASS','scope':__doc__,'serial':'emulator-5590','avd':'DH2_Loader_API37','apk_sha256':digest,'coverage_sha256':binding['coverage_sha256'],'audit_source_sha256':binding['audit_source_sha256'],'cases':results,'native_frames_verified':86,'nonblank_map_viewports_verified':86,'restored':'SWAMP','runtime_objects_verified':False,'full_loader_verified':False}
(reports/'map-recovery-preview.json').write_text(json.dumps(report,indent=2)+'\n')
print(json.dumps({'validation':'PASS','rendered_cases':len(results),'restored':'SWAMP'}),flush=True)
