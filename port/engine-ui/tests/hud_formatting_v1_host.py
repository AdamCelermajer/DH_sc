"""Private source/DSO-bound sanitizer replay of the whole HUD skill text batch."""
import argparse,hashlib,json,os,shlex,subprocess,time
from pathlib import Path
ROOT=Path(__file__).resolve().parents[3]
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def wsl(command):
 p=subprocess.run(['wsl','-e','bash','-lc',command],text=True,capture_output=True,encoding='utf8',errors='replace')
 if p.returncode:raise RuntimeError(p.stdout+'\n'+p.stderr)
 if p.stderr.strip():raise RuntimeError('Unexpected sanitizer/compiler stderr: '+p.stderr)
 return p.stdout
def linux(p):return '/mnt/'+p.drive[0].lower()+p.as_posix()[2:]
def main():
 p=argparse.ArgumentParser();p.add_argument('--build',default='/home/adampalace/dh2-hud-formatting-v1-audit/final');p.add_argument('--runtime',default='/home/adampalace/dh2-hud-formatting-v1-runtime');p.add_argument('--snapshot',default='/home/adampalace/dh2-hud-formatting-v1-audit/main120');a=p.parse_args();started=time.monotonic()
 sources=[
 'port/engine-ui/hud_text_format_v1.cpp','port/engine-ui/hud_text_v1.cpp',
 'port/level-world/character_skill_info_v1.cpp','port/level-world/character_skill_properties_v1.cpp',
 'port/level-world/character_skill_info_session_v1.cpp','port/level-world/character_hud_skill_text_v1.cpp']
 tests=['port/engine-ui/tests/hud_text_format_v1.cpp','port/level-world/tests/character_skill_info_v1.cpp','port/engine-ui/tests/hud_skill_text_v1.cpp']
 # All project compiler inputs from the private runtime CMake and source/public
 # headers used by this adapter; actual frozen dependency DSOs are separate.
 runtime_inputs=[str(x.relative_to(ROOT)).replace('\\','/')for x in (ROOT/'port/script-runtime').glob('*')if x.suffix in('.c','.cpp','.h','.hpp')]+[str(x.relative_to(ROOT)).replace('\\','/')for x in (ROOT/'port/script-runtime/lua').glob('*')if x.suffix in('.c','.h')]
 # Conservative project-header closure, not a claim that every listed header
 # is consumed. System/compiler headers are represented by compiler version.
 headers=[str(x.relative_to(ROOT)).replace('\\','/')for x in (ROOT/'port').rglob('*')if x.is_file() and x.suffix in('.h','.hpp')]
 assets=ROOT/'port/android-native/app/src/main/assets'
 inputs=[assets/'data'/n for n in ('character_properties_pyarray.bin','character_properties_pyarraynames.bin','character_properties_pystructnames.bin','character_classes_pyarray.bin','character_classes_pyarraynames.bin','character_classes_pystructnames.bin','skills_pyarray.bin','skills_pyarraynames.bin','skills_pystructnames.bin','fonts_pycst.bin')]
 inputs+=list((assets/'original-cache/data/pydata').glob('common_text*'))+list((assets/'original-cache/data/text').glob('*'))+list((ROOT/'.local-inputs/hud-formatting-v1/scripts').glob('*.luac'))
 asset_inputs=[str(x.relative_to(ROOT)).replace('\\','/')for x in inputs if x.is_file()]
 tracked=sources+tests+runtime_inputs+headers+asset_inputs+['port/script-runtime/tests/script_first_return_v1.cpp','port/engine-ui/tests/hud_formatting_v1_host.py','.local-inputs/hud-formatting-v1/runtime-build/CMakeLists.txt','port/engine-ui/reference/hud-formatting-v1/format-gold.bin','port/engine-ui/reference/hud-formatting-v1/skill-class-gold.bin','port/level-world/reference/character-skill-info-v1/skill-info-gold.bin']
 tracked=sorted(set(x for x in tracked if (ROOT/x).is_file()));before={x:sha(ROOT/x)for x in tracked}
 dep_before=wsl('sha256sum '+shlex.quote(a.snapshot)+'/*.so')
 commands=[]
 def run(command):commands.append(command);return wsl(command)
 run('cmake --build '+shlex.quote(a.runtime)+' --parallel 4')
 runtime_hash=run('sha256sum '+shlex.quote(a.runtime+'/libdh2_script_runtime.so')).split()[0]
 run('mkdir -p '+shlex.quote(a.build));flags=['g++','-std=c++17','-O1','-g','-fsanitize=address,undefined','-fno-omit-frame-pointer','-fno-fast-math','-ffp-contract=off','-Wall','-Wextra','-Werror','-Wno-misleading-indentation']
 libs=['-L'+a.runtime,'-L'+a.snapshot,'-ldh2_script_runtime','-ldh2_engine_ui','-ldh2_level_world','-ldh2_game_data','-Wl,-rpath,'+a.runtime+':'+a.snapshot]
 run(shlex.join(flags+['-shared','-fPIC']+[linux(ROOT/x)for x in sources]+libs+['-o',a.build+'/libdh2_hud_formatting_v1.so']))
 ld=a.build+':'+a.runtime+':'+a.snapshot
 results={}
 cases=[('formatter',tests[0],[linux(ROOT/'port/engine-ui/reference/hud-formatting-v1/format-gold.bin')]),('skill_info',tests[1],[linux(ROOT/'port/level-world/reference/character-skill-info-v1/skill-info-gold.bin')]),('real_skill_text',tests[2],[linux(ROOT/'port/engine-ui/reference/hud-formatting-v1/skill-class-gold.bin'),linux(ROOT/'port/android-native/app/src/main/assets'),linux(ROOT/'.local-inputs/hud-formatting-v1/scripts')])]
 for name,source,args in cases:
  run(shlex.join(flags+[linux(ROOT/source),'-L'+a.build,'-ldh2_hud_formatting_v1']+libs+['-Wl,-rpath,'+a.build,'-o',a.build+'/'+name+'_audit']))
  result=run('LD_LIBRARY_PATH='+shlex.quote(ld)+' ASAN_OPTIONS=detect_leaks=1:halt_on_error=1 UBSAN_OPTIONS=halt_on_error=1 '+shlex.join([a.build+'/'+name+'_audit']+args));results[name]=json.loads(result)
 results['return_protocol']=json.loads(run('ASAN_OPTIONS=detect_leaks=1:halt_on_error=1 UBSAN_OPTIONS=halt_on_error=1 '+shlex.quote(a.runtime+'/first_return_audit')))
 after={x:sha(ROOT/x)for x in tracked};assert before==after,'Compiler/source/gold/cache changed during proof';assert dep_before==wsl('sha256sum '+shlex.quote(a.snapshot)+'/*.so'),'Private frozen dependencies changed';assert runtime_hash==wsl('sha256sum '+shlex.quote(a.runtime+'/libdh2_script_runtime.so')).split()[0],'Versioned runtime changed during proof'
 artifacts={line.split(maxsplit=1)[1].strip():line.split()[0]for line in run('sha256sum '+shlex.quote(a.build)+'/*.so '+shlex.quote(a.build)+'/*_audit '+shlex.quote(a.runtime+'/first_return_audit')).splitlines()}
 report=dict(validation='PASS',source_and_gold_sha256=before,results=results,sanitizers=dict(address=True,undefined=True,leaks=True,findings=0),private_dependency_sha256={line.split(maxsplit=1)[1].strip():line.split()[0]for line in dep_before.splitlines()},versioned_runtime_sha256=runtime_hash,artifact_sha256=artifacts,commands=commands,compiler=run('g++ --version').splitlines()[0],elapsed_seconds=time.monotonic()-started,scope='Whole native parseEx and SkillInfo gold; actual cache classes+properties+skills+integer text and actual skill commons/StaffMaster/Hardiness script composition. Active Character/Script publication is an explicit retained VM fixture in this composition, not a complete live session. Normal uncached class/recalc/buffs/gameplay providers remain required. Session adapter compile-linked but not exercised by this test. No packaged/GPU/live gameplay parity claim.')
 dest=ROOT/'port/engine-ui/reports/hud-formatting-v1-host-audit.json';dest.write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(dict(validation='PASS',results=results,report_sha256=sha(dest))))
if __name__=='__main__':main()
