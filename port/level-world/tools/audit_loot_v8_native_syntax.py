from pathlib import Path
import subprocess,json,hashlib
root=Path(__file__).resolve().parents[3]
compiler=Path(r'C:\Users\adamc\AppData\Local\Android\Sdk\ndk\29.0.14206865\toolchains\llvm\prebuilt\windows-x86_64\bin\clang++.exe')
sources=['port/game-data/loot_entry_selection_v8.cpp','port/game-data/loot_table_selection_v8.cpp','port/game-data/loot_item_selection_v8.cpp','port/game-data/loot_creation_v8.cpp','port/game-data/loot_temporary_inventory_v8.cpp','port/game-data/loot_audiovisual_v8.cpp','port/level-world/character_loot_drop_v8.cpp','port/level-world/character_loot_item_manager_v8.cpp','port/level-world/character_loot_drop_award_v8.cpp','port/level-world/character_loot_interact_v8.cpp','port/level-world/player_manager_loot_queries_v8.cpp','port/level-world/player_equipment_render_owner_v1.cpp']
commands=[]
sources.append('port/level-world/character_loot_scatter_v8.cpp')
for target in ['aarch64-linux-android26','x86_64-linux-android26']:
 args=[str(compiler),'--target='+target,'-std=c++17','-Wall','-Wextra','-Werror','-Wno-misleading-indentation','-fsyntax-only',*sources]
 result=subprocess.run(args,cwd=root,capture_output=True,text=True);commands.append(dict(target=target,args=args,exit_code=result.returncode,stdout=result.stdout,stderr=result.stderr));assert result.returncode==0,commands[-1]
report=dict(validation='PASS',commands=commands,source_sha256={p:hashlib.sha256((root/p).read_bytes()).hexdigest()for p in sources},full_native_link=False,APK=False)
(root/'port/level-world/reports/loot-v8-native-syntax-audit.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(dict(validation='PASS',targets=[r['target']for r in commands],sources=len(sources))))
