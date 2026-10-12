"""AI death original-gold replay plus genuine AI/debug/target failure prefix.

Isolated source coordinator invokes actual sanitizer world/data/runtime DSOs.
TimerStop is observed through a test interposer forwarding to the genuine DSO.
Required SM_SetDeadState is not faked; composed event2 stops at that boundary.
"""
import argparse,hashlib,json,struct,subprocess
from pathlib import Path
ROOT=Path(__file__).resolve().parents[1];REPO=ROOT.parents[1]
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def linux(p):return '/mnt/c/'+str(p.resolve()).replace('\\','/')[3:]
def words(*xs):return struct.pack('<'+'I'*len(xs),*(x&0xffffffff for x in xs))
def text(s):b=s.encode();return words(len(b))+b
def trace(tr):return words(len(tr))+b''.join(words(*r[:7])+text(r[7])+words(*r[8:]) for r in tr)
def main():
 p=argparse.ArgumentParser();p.add_argument('--main-linked',action='store_true');p.add_argument('--output',type=Path);a=p.parse_args();scratch=REPO/'.local-inputs/character-ai-death-discovery';gold=ROOT/'reference/character-ai-death/death-fixtures.json';original=json.loads(gold.read_text());blob=bytearray(b'AID1'+words(len(original['records'])))
 for r in original['records']:
  x=r['config'];blob+=words(r['wrapper'],x['owner'],x['group'],x['active'],x['timer0'],x['timer1'],*x['word'],*x['timers'],*x['targets'],*x['bytes'],x['debug'],x['mutate_op'],x['next_owner'],x['next_active'],x['next_timer0'],x['next_timer1'],x.get('reentry_op',-1))+trace(r['trace'])+words(*r['state'],*r['output'])+b''.join(bytes.fromhex(t) for rows in r['timers'] for t in rows)+words(len(r['nested']))
  for nested in r['nested']:blob+=trace(nested['trace'])+words(*nested['output'])
 hostgold=scratch/'host-fixtures.bin';hostgold.write_bytes(blob);missing=scratch/'actually-missing-DebugSwitches.savegame';assert not missing.exists();commands=[]
 def run(*args):
  r=subprocess.run(['wsl','--cd',linux(REPO),*args],capture_output=True,text=True,timeout=120);commands.append(dict(arguments=args,returncode=r.returncode,stdout=r.stdout,stderr=r.stderr));assert r.returncode==0 and not r.stderr.strip(),dict(args=args,returncode=r.returncode,stdout=r.stdout[-1800:],stderr=r.stderr[-8000:]);return r.stdout.strip()
 flags=['-std=c++17','-O1','-g','-fno-fast-math','-ffp-contract=off','-fsanitize=address,undefined','-fno-omit-frame-pointer','-Wall','-Wextra','-Werror','-Wno-misleading-indentation'];build='/home/adampalace/dh2-world-build';deps=[build+'/libdh2_level_world.so',build+'/game-data/libdh2_game_data.so',build+'/script-runtime/libdh2_script_runtime.so'];before={x:run('sha256sum',x).split()[0] for x in deps};links=[]
 for sub,name in [('', 'level_world'),('/game-data','game_data'),('/script-runtime','script_runtime')]:links+=['-L'+build+sub,'-ldh2_'+name,'-Wl,-rpath,'+build+sub]
 library=linux(scratch/'libcharacter_ai_death_audit.so');exe=linux(scratch/'host');files=[ROOT/x for x in ['character_ai_death.hpp','character_ai_death.cpp','tests/character_ai_death.cpp','tests/character_ai_death_host.py','tests/character_ai_death_differential.py']];sources={str(x.relative_to(REPO)):sha(x) for x in files}
 if a.main_linked:library=deps[0];modulelinks=[]
 else:run('g++',*flags,'-fPIC','-shared',linux(ROOT/'character_ai_death.cpp'),*links,'-Wl,--no-undefined','-o',library);modulelinks=['-L'+linux(scratch),'-lcharacter_ai_death_audit']
 run('g++',*flags,linux(ROOT/'tests/character_ai_death.cpp'),'-I'+linux(ROOT),*modulelinks,*links,'-Wl,-rpath,'+linux(scratch),'-ldl','-o',exe);env=['env','LD_LIBRARY_PATH='+':'.join([build,build+'/game-data',build+'/script-runtime',linux(scratch)]),'ASAN_OPTIONS=detect_leaks=1:abort_on_error=1','UBSAN_OPTIONS=halt_on_error=1'];audit=json.loads(run(*env,exe,linux(hostgold),linux(missing)));linked=run(*env,'ldd',exe);assert audit['validation']=='PASS' and audit['module_library']==library and audit['world_library']==deps[0] and audit['timer_stop_library']==deps[0];assert all(x in linked for x in [*deps,'libasan.so','libubsan.so']);binary={x:run('sha256sum',x).split()[0] for x in [*deps,library,exe]};assert all(binary[x]==v for x,v in before.items());assert all(sha(REPO/x)==v for x,v in sources.items());arm=ROOT/'reports/character-ai-death-arm64-differential.json';proof=json.loads(arm.read_text());assert proof['validation']=='PASS' and proof['gold_sha256']==sha(gold);assert all(sha(REPO/x)==v for x,v in proof['source_sha256'].items());report=dict(validation='PASS',scope=__doc__,host_audit=audit,source_sha256=sources,binary_sha256=binary,input_sha256={str(x.relative_to(REPO)):sha(x) for x in [gold,hostgold]},original_sha256=proof['original_sha256'],arm64_report_sha256=sha(arm),sanitizers=['AddressSanitizer','UndefinedBehaviorSanitizer'],sanitizer_findings=0,main_world_library_executed=True,module_in_main_world=a.main_linked,full_group_state_aggro_skill_spell_bodies=False,packaged_APK=False,commands=commands,linked_dependencies=linked);out=a.output or ROOT/('reports/character-ai-death-main-linked-host-audit.json' if a.main_linked else 'reports/character-ai-death-host-audit.json');out.write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(dict(validation='PASS',host=audit)))
if __name__=='__main__':main()
