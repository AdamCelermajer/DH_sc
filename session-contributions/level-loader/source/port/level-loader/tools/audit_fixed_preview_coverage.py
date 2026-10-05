"""Try the generic private renderer for each assembled fixed definition.

These are separate cold previews, not campaign transitions or save restoration.
"""
import hashlib,json,pathlib,re,subprocess,time,xml.etree.ElementTree as ET
ROOT=pathlib.Path(__file__).resolve().parents[3]
assert ROOT==pathlib.Path(r'C:\Users\adamc\.codex\worktrees\generic-level-loader\DH_sc')
REPORTS=ROOT/'port/level-loader/reports'
ADB=str(pathlib.Path.home()/'AppData/Local/Android/Sdk/platform-tools/adb.exe')
BASE=[ADB,'-s','emulator-5590']
def run(args):return subprocess.check_output(BASE+args,timeout=30)
assert run(['emu','avd','name']).decode().replace('\r','').splitlines()[0]=='DH2_Loader_API37'
assert run(['shell','getprop','sys.boot_completed']).decode().strip()=='1'
coverage=json.loads((REPORTS/'fixed-map-level-coverage.json').read_text())
results=[]
def logs():return run(['logcat','-d','-v','threadtime','DH2Loader:I','AndroidRuntime:E','*:S']).decode(errors='replace')
for row in coverage['levels']:
    result={k:row[k] for k in ('name','file','map_preparation')}
    result.update(runtime_objects_verified=False,gameplay_verified=False)
    if row['map_preparation']!='assembled':
        result.update(preview='source_blocked',reason=row['reason']);results.append(result);continue
    previous=logs();run(['shell','am','force-stop','local.dh2.loader'])
    run(['shell','am','start','-n','local.dh2.loader/com.example.dh2.LoaderPreviewActivity','--es','level',row['name'],'--es','definition',row['file']])
    deadline=time.monotonic()+30;new=''
    while time.monotonic()<deadline:
        current=logs();new=current[len(previous):] if current.startswith(previous) else current
        marker=re.search(r'MAP_FRAME_OK identity='+re.escape(row['name'])+r' modules=(\d+) meshes=(\d+) draws=(\d+) triangles=(\d+) textures=(\d+) viewport=(\d+)x(\d+)',new)
        if marker:
            result.update(preview='frame_submitted',native={k:int(v) for k,v in zip(('modules','meshes','draws','triangles','textures','width','height'),marker.groups())})
            break
        if 'Preparation failed:' in new or 'FRAME_FAILED' in new or 'FATAL EXCEPTION' in new:
            result.update(preview='failed',reason=new[-2500:]);break
        time.sleep(.25)
    else:result.update(preview='timeout',reason=new[-2500:])
    # Android window idle observation before capture avoids recording a previous
    # activity frame merely because the native draw command was submitted.
    ui='/data/local/tmp/loader-preview-coverage.xml';run(['shell','uiautomator','dump',ui])
    tree=ET.fromstring(run(['exec-out','cat',ui]));result['visible_text']=[n.get('text') for n in tree.iter('node') if n.get('text')]
    data=run(['exec-out','screencap','-p']);assert data.startswith(b'\x89PNG\r\n\x1a\n')
    screenshot='fixed-preview-'+row['name'].lower()+'.png';(REPORTS/screenshot).write_bytes(data)
    result.update(screenshot=screenshot,screenshot_sha256=hashlib.sha256(data).hexdigest())
    results.append(result);print(json.dumps({'level':row['name'],'preview':result['preview']}),flush=True)
summary={}
for row in results:summary[row['preview']]=summary.get(row['preview'],0)+1
logfile=REPORTS/'fixed-preview-coverage-logcat.txt';logfile.write_text(logs(),encoding='utf-8')
apk=ROOT/'port/android-native/app/build/outputs/apk/debug/app-debug.apk'
report={'scope':'Generic private renderer attempts for prepared fixed source sets. Frame submission and screenshots do not establish original scene parity, objects, campaign transitions or saves.',
        'serial':'emulator-5590','avd':'DH2_Loader_API37','app_id':'local.dh2.loader','apk_sha256':hashlib.sha256(apk.read_bytes()).hexdigest(),
        'assembly_coverage_sha256':hashlib.sha256((REPORTS/'fixed-map-level-coverage.json').read_bytes()).hexdigest(),
        'audit_source_sha256':hashlib.sha256(pathlib.Path(__file__).read_bytes()).hexdigest(),'summary':summary,'levels':results,'full_loader_verified':False}
(REPORTS/'fixed-map-preview-coverage.json').write_text(json.dumps(report,indent=2)+'\n')
print(json.dumps(summary),flush=True)
