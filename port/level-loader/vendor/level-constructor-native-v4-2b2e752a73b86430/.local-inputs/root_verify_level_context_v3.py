from pathlib import Path
import json,hashlib,os,subprocess
root=Path(__file__).resolve().parent.parent
out=root/'port/level-loader/reports/level-constructor-v3/context';out.mkdir(parents=True,exist_ok=True)
temporary=out/'compiler-temp';temporary.mkdir(exist_ok=True)
environment=dict(os.environ,TMP=str(temporary),TEMP=str(temporary))
sdk=Path(r'C:\Users\adamc\AppData\Local\Android\Sdk');tool=sdk/'ndk/29.0.14206865/toolchains/llvm/prebuilt/windows-x86_64/bin'
paths=['port/level-loader/tests/canonical_level_constructor_v3.cpp','port/level-loader/canonical_level_context_v1.cpp','port/level-loader/level_constructor_v3.cpp','port/level-world/event_manager_owner_v12.cpp']
sources=[root/p for p in paths];binaries={}
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest()
for abi,cxx in [('x86_64','x86_64-linux-android24-clang++.cmd'),('arm64-v8a','aarch64-linux-android24-clang++.cmd')]:
 binary=out/('level-context-'+abi)
 args=[str(tool/cxx),'-std=c++17','-O2','-Wall','-Wextra','-Werror','-Wno-misleading-indentation','-I'+str(root/'port/level-world'),'-ffunction-sections','-fdata-sections',*map(str,sources),'-static-libstdc++','-Wl,--gc-sections','-o',str(binary)]
 run=subprocess.run(args,capture_output=True,env=environment);(out/(abi+'-build.log')).write_bytes(run.stdout+run.stderr)
 assert run.returncode==0,run.stderr.decode(errors='replace');binaries[abi]=binary
adb=[str(sdk/'platform-tools/adb.exe'),'-s','emulator-5554'];remote='/data/local/tmp/dh2-level-context-v3'
def run(args):
 p=subprocess.run(adb+args,capture_output=True,timeout=45);assert p.returncode==0,(args,p.stdout,p.stderr);return p.stdout.decode().strip()
run(['push',str(binaries['x86_64']),remote]);run(['shell','chmod','755',remote]);result=run(['shell',remote]);assert result.startswith('PASS same Level constructor lifetime ')
receipt={'status':'PASS','stdout':result,'source_sha256':{p.relative_to(root).as_posix():sha(p) for p in sources},'headers':{p:sha(root/p) for p in ['port/level-loader/canonical_level_context_v1.hpp','port/level-loader/level_constructor_v3.hpp','port/level-world/event_manager_owner_v12.hpp']},'binary_sha256':{abi:sha(p) for abi,p in binaries.items()},'same_canonical_fields_and_GSLevel_slot':True,'replay_refused':True,'Level_and_script_save_fixture_leases_destroyed':True,'explicit_deeper_fixtures':['Application service lifetime','LuaScript C1/load','Save storage/C1','immutable LevelList','GetOnline'],'live_app_unchanged':True,'gameplay_ready':False}
(out/'receipt.json').write_text(json.dumps(receipt,indent=2)+'\n');print(json.dumps(receipt))
