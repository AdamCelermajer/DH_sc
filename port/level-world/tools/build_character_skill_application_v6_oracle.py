from pathlib import Path
import subprocess,hashlib,json
root=Path(__file__).resolve().parents[3]
compiler=Path('C:/Users/adamc/AppData/Local/Android/Sdk/ndk/29.0.14206865/toolchains/llvm/prebuilt/windows-x86_64/bin/clang++.exe')
sources=['port/level-world/character_skill_application_v6.cpp','port/level-world/character_skill_attack_v6.cpp','port/level-world/character_combat_result_v6.cpp','port/level-world/character_hit_v6.cpp','port/level-world/character_target_providers.cpp','port/game-data/properties.cpp','port/game-data/combat.cpp','port/level-world/character_buffs.cpp','port/level-world/character_timers.cpp','port/game-data/fresh_inventory_owned_v4.cpp']
sources.append('port/level-world/character_skill_target_queries_v6.cpp')
sources.extend(['port/level-world/character_skill_aggro_v6.cpp','port/game-data/aggro.cpp'])
sources.extend(['port/level-world/character_skill_save_reload_v6.cpp','port/game-data/player_savegame_v1.cpp'])
sources.append('port/level-world/tests/character_skill_save_reload_v6_oracle.cpp')
sources.append('port/level-world/character_skill_reload_v6.cpp')
output=root/'.local-inputs/libcharacter_skill_application_v6_oracle.so'
command=[str(compiler),'--target=aarch64-linux-android24','-std=c++17','-O2','-shared','-fPIC',*sources,'-o',str(output)]
r=subprocess.run(command,cwd=root,capture_output=True,text=True)
assert r.returncode==0,(r.returncode,r.stdout,r.stderr)
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest()
report={'command':command,'exit_code':r.returncode,'stdout':r.stdout,'stderr':r.stderr,'source_sha256':{p:sha(root/p)for p in sources},'library_sha256':sha(output)}
Path(str(output)+'.build.json').write_text(json.dumps(report,indent=2)+'\n')
print(json.dumps({'validation':'PASS','library_sha256':sha(output)}))
