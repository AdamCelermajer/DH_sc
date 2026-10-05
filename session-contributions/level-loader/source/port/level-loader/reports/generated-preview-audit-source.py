"""Visible private map inspection of all catalog definitions and tested seeds.

Cold previews verify submitted frames and pixels, not campaign transitions,
original lighting, gameplay objects, or persistent state.
"""
import collections,hashlib,io,json,pathlib,re,subprocess,time,xml.etree.ElementTree as ET
from PIL import Image
ROOT=pathlib.Path(__file__).resolve().parents[3]
assert ROOT==pathlib.Path(r'C:\Users\adamc\.codex\worktrees\generic-level-loader\DH_sc')
REPORTS=ROOT/'port/level-loader/reports'
BASE=[str(pathlib.Path.home()/'AppData/Local/Android/Sdk/platform-tools/adb.exe'),'-s','emulator-5590']
def run(args):return subprocess.check_output(BASE+args,timeout=30)
def sha(path):
    with path.open('rb') as file:return hashlib.file_digest(file,'sha256').hexdigest()
assert run(['emu','avd','name']).decode().replace('\r','').splitlines()[0]=='DH2_Loader_API37'
assert run(['shell','getprop','sys.boot_completed']).strip()==b'1'
catalog_path=ROOT/'port/android-native/app/src/main/assets/loader-map-catalog.json'
catalog=json.loads(catalog_path.read_text());assert len(catalog['maps'])==51
apk=ROOT/'port/android-native/app/build/outputs/apk/debug/app-debug.apk'
apk_hash=sha(apk);assert json.loads((REPORTS/'preview-apk.json').read_text())['sha256']==apk_hash
def logs(pid):return run(['logcat','-d','--pid='+pid,'-v','threadtime','DH2Loader:I','AndroidRuntime:E','*:S']).decode(errors='replace')
results=[];all_logs=[]
partial=REPORTS/'generated-preview-partial.json';script_hash=sha(pathlib.Path(__file__));catalog_hash=sha(catalog_path)
if partial.is_file():
    saved=json.loads(partial.read_text())
    assert saved['apk_sha256']==apk_hash and saved['catalog_sha256']==catalog_hash and saved['script_sha256']==script_hash
    results=saved['levels'];all_logs=saved['logs']
    assert all(sha(REPORTS/row['screenshot'])==row['screenshot_sha256'] for row in results)
    print(json.dumps({'resuming_completed_cases':len(results)}),flush=True)
completed={(row['identity'],row['seed']) for row in results}
prior=REPORTS/'generated-preview-coverage.json'
if prior.is_file():
    baseline=REPORTS/('generated-preview-baseline-'+json.loads(prior.read_text())['apk_sha256'][:12])
    assert not baseline.exists(),'Archive already exists; inspect prior evidence before replacing it'
    baseline.mkdir()
    for path in sorted(REPORTS.glob('generated-preview-*.png')):path.rename(baseline/path.name)
    for name in ('generated-preview-coverage.json','generated-preview-logs.json','generated-preview-progress.json'):
        (REPORTS/name).rename(baseline/name)
for row in catalog['maps']:
    for seed in ((0,1) if row['kind']=='procedural' else (0,)):
        if (row['identity'],seed) in completed:continue
        run(['shell','am','force-stop','local.dh2.loader'])
        run(['shell','am','start','-n','local.dh2.loader/com.example.dh2.LoaderPreviewActivity',
             '--es','level',row['identity'],'--es','definition',row['definition'],'--ei','seed',str(seed)])
        deadline=time.monotonic()+40;pid='';text=''
        result={'identity':row['identity'],'definition':row['definition'],'kind':row['kind'],'seed':seed,
                'expected_preparation':row.get('preparation_by_seed',{}).get(str(seed),'assembled'),
                'runtime_objects_verified':False,'gameplay_verified':False}
        pattern=r'MAP_FRAME_OK identity='+re.escape(row['identity'])+r' procedural=(\d+) seed=(\d+) modules=(\d+) meshes=(\d+) draws=(\d+) triangles=(\d+) textures=(\d+) viewport=(\d+)x(\d+)'
        while time.monotonic()<deadline:
            if not pid:
                process=subprocess.run(BASE+['shell','pidof','local.dh2.loader'],capture_output=True,timeout=5)
                assert process.returncode in (0,1),process.stderr.decode(errors='replace')
                pid=process.stdout.decode().strip()
                if not pid:time.sleep(.2);continue
                assert pid.isdigit(),pid
            text=logs(pid);marker=re.search(pattern,text)
            if marker:
                result.update(preview='frame_submitted',native={key:int(value) for key,value in zip(
                    ('procedural','seed','modules','meshes','draws','triangles','textures','width','height'),marker.groups())})
                assert result['native']['seed']==seed and result['native']['procedural']==int(row['kind']=='procedural')
                break
            if 'Preparation failed:' in text or 'FRAME_FAILED' in text or 'FATAL EXCEPTION' in text:
                result.update(preview='failed',reason=text[-2500:]);break
            time.sleep(.2)
        else:result.update(preview='timeout',reason=text[-2500:])
        ui='/data/local/tmp/loader-generated-preview.xml';run(['shell','uiautomator','dump',ui])
        tree=ET.fromstring(run(['exec-out','cat',ui]))
        result['visible_text']=[node.get('text') for node in tree.iter('node') if node.get('text')]
        screenshot=run(['exec-out','screencap','-p']);assert screenshot.startswith(b'\x89PNG\r\n\x1a\n')
        path=REPORTS/('generated-preview-'+row['identity'].lower()+'-seed'+str(seed)+'.png');path.write_bytes(screenshot)
        result.update(screenshot=path.name,screenshot_sha256=sha(path))
        if result['preview']=='frame_submitted':
            # Bounds obtained from this activity's live labels, not an assumed resolution.
            load_node=next(node for node in tree.iter('node') if node.get('text')=='LOAD MAP')
            whole_node=next(node for node in tree.iter('node') if node.get('text')=='WHOLE MAP')
            load_bounds=[int(v) for v in re.findall(r'\d+',load_node.get('bounds'))]
            whole_bounds=[int(v) for v in re.findall(r'\d+',whole_node.get('bounds'))]
            with Image.open(io.BytesIO(screenshot)) as image:
                crop=(0,load_bounds[3],image.width,whole_bounds[1]);colors=collections.Counter(image.convert('RGB').crop(crop).get_flattened_data())
                nonbackground=sum(colors.values())-colors.most_common(1)[0][1]
                result.update(surface_bounds=list(crop),surface_non_background_pixels=nonbackground,
                              surface_color_count=len(colors),visible_map_pixels=nonbackground>10000)
            if not result['visible_map_pixels']:
                # Widely spaced authored rooms can be tiny in the overview.
                # Focus a real retained module without changing any map transform.
                focus_node=next(node for node in tree.iter('node') if node.get('text')=='NEXT MODULE')
                box=[int(v) for v in re.findall(r'\d+',focus_node.get('bounds'))]
                run(['shell','input','tap',str((box[0]+box[2])//2),str((box[1]+box[3])//2)])
                run(['shell','uiautomator','dump',ui]);focused_tree=ET.fromstring(run(['exec-out','cat',ui]))
                focused_text=[node.get('text') for node in focused_tree.iter('node') if node.get('text')]
                assert any(text.startswith('Module 0 |') for text in focused_text),focused_text
                focused_path=REPORTS/('generated-preview-'+row['identity'].lower()+'-seed'+str(seed)+'-module0.png')
                focused_path.write_bytes(run(['exec-out','screencap','-p']))
                with Image.open(focused_path) as focused_image:
                    colors=collections.Counter(focused_image.convert('RGB').crop(crop).get_flattened_data())
                    focused_pixels=sum(colors.values())-colors.most_common(1)[0][1]
                result['focused_module']={'screenshot':focused_path.name,'screenshot_sha256':sha(focused_path),
                                          'surface_non_background_pixels':focused_pixels,'visible_text':focused_text}
                result['visible_map_pixels']=focused_pixels>10000
        all_logs.append({'identity':row['identity'],'seed':seed,'pid':pid,'log':text})
        results.append(result)
        temporary=partial.with_suffix('.tmp')
        temporary.write_text(json.dumps({'apk_sha256':apk_hash,'catalog_sha256':catalog_hash,'script_sha256':script_hash,
                                         'levels':results,'logs':all_logs},indent=2)+'\n')
        temporary.replace(partial)
        print(json.dumps({key:result.get(key) for key in ('identity','seed','preview','visible_map_pixels')}),flush=True)
        (REPORTS/'generated-preview-progress.json').write_text(json.dumps({'completed':len(results),'latest':result},indent=2)+'\n')
assert len(results)==86 and sha(apk)==apk_hash
log_path=REPORTS/'generated-preview-logs.json';log_path.write_text(json.dumps(all_logs,indent=2)+'\n')
report={'scope':__doc__,'serial':'emulator-5590','avd':'DH2_Loader_API37','app_id':'local.dh2.loader',
        'apk_sha256':apk_hash,'catalog_sha256':sha(catalog_path),'script_sha256':sha(pathlib.Path(__file__)),
        'logs_sha256':sha(log_path),'levels':results,'summary':dict(collections.Counter(row['preview'] for row in results)),
        'visible_pixel_cases':sum(row.get('visible_map_pixels',False) for row in results),
        'runtime_objects_verified':False,'campaign_transitions_verified':False,'full_loader_verified':False}
(REPORTS/'generated-preview-coverage.json').write_text(json.dumps(report,indent=2)+'\n')
print(json.dumps({'summary':report['summary'],'visible_pixel_cases':report['visible_pixel_cases']}),flush=True)
