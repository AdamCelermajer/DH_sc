import os,pathlib,subprocess,sys,hashlib,json
root=pathlib.Path(__file__).resolve().parent.parent
temp=root/'.local-inputs/openable-graph-v4-tmp';temp.mkdir(exist_ok=True)
env=os.environ.copy();env['TEMP']=str(temp);env['TMP']=str(temp)
adb=r'C:\Users\adamc\AppData\Local\Android\Sdk\platform-tools\adb.exe'
remote='/data/local/tmp/dh2-openable-graph-v4-cache'
subprocess.run([adb,'-s','emulator-5554','shell','mkdir','-p',remote],check=True,capture_output=True)
manifest=[]
for name in ['go_chest_swamp.bdae','go_chest_swamp_big.bdae','go_chest_swamp_rotten.bdae']:
 p=root/'reference/openable-container-v1/cache'/name
 subprocess.run([adb,'-s','emulator-5554','push',str(p),remote+'/'+name],check=True,capture_output=True)
 manifest.append({'source':str(p),'remote':remote+'/'+name,'sha256':hashlib.sha256(p.read_bytes()).hexdigest()})
(temp/'fixtures.json').write_text(json.dumps(manifest,indent=2))
sys.exit(subprocess.call([sys.executable,str(root/'.local-inputs/root_android_linked_owner_test.py'),'canonical-openable-graph-v4','port/level-world/tests/canonical_openable_graph_v4.cpp','port/level-world/canonical_openable_graph_v4.cpp','port/level-world/game_object_set_position_v2.cpp','--',remote],cwd=root,env=env))
