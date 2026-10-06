"""Final-source native probes on the isolated loader emulator; no gameplay."""
import pathlib,subprocess,zipfile,json,hashlib,time
root=pathlib.Path(r'C:\Users\adamc\.codex\worktrees\generic-level-loader\DH_sc');loader=root/'port/level-loader';reports=loader/'reports'
adb=r'C:\Users\adamc\AppData\Local\Android\Sdk\platform-tools\adb.exe';base=[adb,'-s','emulator-5590']
remote='/data/local/tmp/dh2-loader-map-recovery';draft=reports
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest()
def run(args,timeout=60):
    assert subprocess.check_output(base+['emu','avd','name'],timeout=15).decode().replace('\r','').splitlines()[0]=='DH2_Loader_API37'
    return subprocess.check_output(base+args,timeout=timeout).decode(errors='replace')
run(['shell','mkdir','-p',remote])
cache=pathlib.Path(r'C:\Users\adamc\Downloads\dungeonhunter2\Dungeon-Hunter-2-HD-v1-0-2-cache.zip')
assert sha(cache)=='3fdf4e4c21d45a780a7c35fb4042abde0e88e76bf75416aad1f227481560b679'
print(run(['push',str(cache),remote+'/cache.zip'],timeout=180),flush=True)
apk=root/'port/android-native/app/build/outputs/apk/debug/app-debug.apk'
with zipfile.ZipFile(apk) as z:
    shared=[n for n in z.namelist() if n=='lib/x86_64/libc++_shared.so']
    if shared:
        p=draft/'libc++_shared.so';p.write_bytes(z.read(shared[0]));run(['push',str(p),remote+'/libc++_shared.so'])
names=('dh2_loader_recovery_probe','dh2_loader_level_preparation_probe','dh2_loader_static_decor_inspection_probe')
binary={}
for name in names:
    p=root/'port/android-native/app/build/intermediates/cxx/Debug/5e223t2g/obj/x86_64'/name
    binary[name]=sha(p);run(['push',str(p),remote+'/'+name]);run(['shell','chmod','755',remote+'/'+name])
for name in ('repair-target-missing.zip','repair-authored-present.zip'):run(['push',str(reports/name),remote+'/'+name])
def probe(name,args):
    result=json.loads(run(['shell','env','LD_LIBRARY_PATH='+remote,remote+'/'+name,remote+'/cache.zip']+args))
    assert result['validation']=='PASS',result
    return result
checks=[{'name':'recovery','result':probe(names[0],[remote+'/repair-target-missing.zip',remote+'/repair-authored-present.zip'])},
        {'name':'lifecycle','result':probe(names[1],['017_red_desert_cave_02.rule.xml','022_swamp2.rule.xml','035_voidmaze_03.rule.xml'])}]
assert checks[0]['result']['recovery_checks']==20 and checks[1]['result']['lifecycle_checks']==14
coverage=json.loads((reports/'loader-recovery-coverage.json').read_text());rows=[r for r in coverage['cases'] if r['kind']=='fixed' or r['seed'] in (0,1)]
assert len(rows)==86;decor=[]
for i,row in enumerate(rows):
    result=probe(names[2],[row['identity'],row['definition'],str(row['seed'])])
    decor.append({'identity':row['identity'],'definition':row['definition'],'seed':row['seed'],'result':result})
    if (i+1)%10==0:print(json.dumps({'native_decor_cases':i+1,'total':86}),flush=True)
void=[r for r in decor if r['identity'].startswith('VOID_MAZE')];assert len(void)==6
assert all(r['result']['mesh_instances']>0 and r['result']['vertices']>1000 for r in void)
report={'validation':'PASS','scope':__doc__,'serial':'emulator-5590','avd':'DH2_Loader_API37','abi':'x86_64','sanitizers':False,'probe_sha256':binary,
        'source_sha256':{p.relative_to(root).as_posix():sha(p) for p in loader.glob('*') if p.suffix in ('.cpp','.hpp')},
        'cache_sha256':sha(cache),'apk_sha256':sha(apk),'checks':checks,'decor_cases':decor,
        'runtime_objects_verified':False,'animation_verified':False,'full_loader_verified':False}
(reports/'map-recovery-native-android.json').write_text(json.dumps(report,indent=2)+'\n')
print(json.dumps({'validation':'PASS','recovery_checks':20,'lifecycle_checks':14,'decor_cases':86}),flush=True)
