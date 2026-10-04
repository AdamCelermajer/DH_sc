"""Immutable private-DSO sanitizer proof for one authoritative player skill Session.

Dependencies are the coherent private main120 snapshot and frozen formatting
v1/runtime receipts. This runner does not rebuild or modify central libraries.
"""
import argparse,hashlib,json,shlex,subprocess,time
from pathlib import Path
ROOT=Path(__file__).resolve().parents[3]
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def linux(p):return '/mnt/'+p.drive[0].lower()+str(p)[2:].replace('\\','/')
commands=[]
def run(command):
 commands.append(command);p=subprocess.run(['wsl','-e','bash','-lc',command],capture_output=True,text=True,encoding='utf8',errors='replace')
 if p.returncode:raise RuntimeError(command+'\n'+p.stdout+p.stderr)
 if p.stderr.strip():raise RuntimeError('Unexpected compiler/sanitizer stderr: '+p.stderr)
 return p.stdout.strip()
def main():
 p=argparse.ArgumentParser();p.add_argument('--build',default='/home/adampalace/dh2-character-skill-gameplay-v3/player');p.add_argument('--snapshot',default='/home/adampalace/dh2-hud-formatting-v1-audit/main120');p.add_argument('--runtime',default='/home/adampalace/dh2-character-skill-gameplay-v3/runtime');p.add_argument('--formatting',default='/home/adampalace/dh2-hud-formatting-v1-audit/final');a=p.parse_args();started=time.monotonic()
 names=['character_script_owner_v2','character_script_session_v3','character_skills_session_v3','character_skill_info_session_v3','character_script_player_vcb_v2','character_current_skill_v2','character_player_skills_v3','character_skills_owner_v3','character_skill_callback_session_v3','character_skill_callbacks_v3','character_skill_buff_bindings_v3','character_skill_class_v3','character_skill_cooldown_v3','character_current_spell_v1','character_faery_element_v3','character_skill_ai_v3','character_script_assets_v1']
 sources=['port/level-world/'+n+'.cpp'for n in names]+['port/asset-payloads/zip_asset_pack_v1.cpp'];tests=['port/level-world/tests/character_player_skills_v3.cpp','port/level-world/tests/character_skill_ai_v3.cpp','port/level-world/tests/character_skill_callbacks_v3.cpp']
 headers=[str(p.relative_to(ROOT)).replace('\\','/')for p in (ROOT/'port').rglob('*')if p.is_file()and p.suffix in('.h','.hpp')]
 ref=ROOT/'port/level-world/reference/character-skill-gameplay-v3';inputs=list(ref.rglob('*'));inputs+=[ROOT/'port/level-world/reference/character-game-design/real-cache-inputs.bin',ROOT/'port/engine-ui/reference/hud-formatting-v1/skill-class-gold.bin',ROOT/'port/engine-ui/reference/hud-formatting-v1/freeze-manifest.json'];inputs+=list((ROOT/'.local-inputs/character-skill-session-v2/cache').rglob('*'));inputs+=list((ROOT/'port/android-native/app/src/main/assets/data').glob('skills_*'));inputs+=list((ROOT/'port/android-native/app/src/main/assets/original-cache/data').rglob('*'))
 tracked=sorted(set(sources+tests+headers+[str(Path(__file__).relative_to(ROOT)).replace('\\','/')]+[str(x.relative_to(ROOT)).replace('\\','/')for x in inputs if x.is_file()]))
 original_zip=Path(r'C:\Users\adamc\Downloads\dungeonhunter2\Dungeon-Hunter-2-HD-v1-0-2-cache.zip');zip_before=sha(original_zip);assert zip_before=='3fdf4e4c21d45a780a7c35fb4042abde0e88e76bf75416aad1f227481560b679','Unexpected original cache ZIP'
 before={x:sha(ROOT/x)for x in tracked};dependency=run('sha256sum '+shlex.quote(a.snapshot)+'/*.so '+shlex.quote(a.runtime+'/libdh2_script_runtime.so')+' '+shlex.quote(a.formatting+'/libdh2_hud_formatting_v1.so'))
 run('mkdir -p '+shlex.quote(a.build));flags=['g++','-std=c++17','-O1','-g','-fsanitize=address,undefined','-fno-omit-frame-pointer','-fno-fast-math','-ffp-contract=off','-Wall','-Wextra','-Werror','-Wno-misleading-indentation'];ld=a.build+':'+a.formatting+':'+a.runtime+':'+a.snapshot
 deps=['-L'+a.formatting,'-ldh2_hud_formatting_v1','-L'+a.runtime,'-ldh2_script_runtime','-L'+a.snapshot,'-ldh2_level_world','-ldh2_game_data','-ldh2_engine_ui','-lz','-Wl,-rpath,'+ld]
 run(shlex.join(flags+['-fPIC','-shared']+[linux(ROOT/s)for s in sources]+deps+['-o',a.build+'/libdh2_character_skill_gameplay_v3.so']))
 cases=[('player',tests[0],[linux(ROOT/'port/level-world/reference/character-game-design/real-cache-inputs.bin'),linux(ROOT/'port/android-native/app/src/main/assets'),linux(ROOT/'.local-inputs/character-skill-session-v2/cache'),linux(ROOT/'port/engine-ui/reference/hud-formatting-v1/skill-class-gold.bin'),'/mnt/c/Users/adamc/Downloads/dungeonhunter2/Dungeon-Hunter-2-HD-v1-0-2-cache.zip']),('ai',tests[1],[linux(ref/'ai-gold-v3.bin')]),('callbacks',tests[2],[linux(ref/'callback-gold.bin')])]
 results={}
 for name,test,args in cases:
  exe=a.build+'/'+name+'_audit';run(shlex.join(flags+[linux(ROOT/test),'-L'+a.build,'-ldh2_character_skill_gameplay_v3']+deps+['-o',exe]));results[name]=json.loads(run('LD_LIBRARY_PATH='+shlex.quote(ld)+' ASAN_OPTIONS=detect_leaks=1:halt_on_error=1 UBSAN_OPTIONS=halt_on_error=1 '+shlex.join([exe]+args)))
 assert before=={x:sha(ROOT/x)for x in tracked},'Compiler input/source/header/cache changed during proof'
 assert sha(original_zip)==zip_before,'Original cache ZIP changed during proof'
 assert dependency==run('sha256sum '+shlex.quote(a.snapshot)+'/*.so '+shlex.quote(a.runtime+'/libdh2_script_runtime.so')+' '+shlex.quote(a.formatting+'/libdh2_hud_formatting_v1.so')),'Private transitive dependencies changed'
 artifacts=run('sha256sum '+shlex.quote(a.build)+'/*.so '+shlex.quote(a.build)+'/*_audit');loaded=run('LD_LIBRARY_PATH='+shlex.quote(ld)+' ldd '+shlex.quote(a.build+'/player_audit'))
 report=dict(validation='PASS',source_and_inputs_sha256=before,original_cache_zip=dict(path=str(original_zip),sha256=zip_before,verified_before_and_after=True),source_header_capture='All project headers are a conservative superset of actual private compiler inputs; copied main120 dependencies retain historical compilation provenance.',results=results,sanitizers=dict(address=True,undefined=True,leaks=True,findings=0),private_dependency_sha256={line.split(maxsplit=1)[1].strip():line.split()[0]for line in dependency.splitlines()},artifact_sha256={line.split(maxsplit=1)[1].strip():line.split()[0]for line in artifacts.splitlines()},dynamic_loader=loaded,commands=commands,compiler=run('g++ --version').splitlines()[0],elapsed_seconds=time.monotonic()-started,scope=__doc__+' Real three-class AI commons publication/player InitVCB/configure/update and real 19 source skill scripts on one authoritative VM/owner/property/temp/timer per player. Saved rows initialize at source level0 then controlled nonzero levels exercise real Hardiness/StaffMaster/Acrobat callbacks, owned Buff/class/recalc and removal. Complete 219-script ZIP snapshot drives real faery script initialization and same-owner current spell queries. Difficulty selection and Idle FSM are named fixtures; gameplay command/FSM/search providers remain required. Missing Debug file, Idle FSM projection and text-only Debug service are explicit fixtures. Both a controlled timer return and actual shared SetSkillCooldown with explicit test duration25 exercise same-owner Timer35 creation, native instance write and selected AIS/private VM expiry. Source AI whole choreography also replays its original gold, while retained passive Begin/Focus/Event/Blur callbacks use explicitly controlled current-index inputs, not a recovered SkillFSM6. No full Character frame, packaged/GPU or gameplay parity claim.')
 target=ROOT/'port/level-world/reports/character-skill-gameplay-v3-player-host-audit.json';target.write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(dict(validation='PASS',results=results,report=str(target),report_sha256=sha(target))))
if __name__=='__main__':main()
