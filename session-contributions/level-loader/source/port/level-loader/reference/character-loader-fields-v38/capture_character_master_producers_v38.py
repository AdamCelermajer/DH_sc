from pathlib import Path
import subprocess,json,re
base=Path('C:/Users/adamc/.codex/worktrees/generic-level-loader/DH_sc/port/level-loader/reference/character-loader-fields-v38');elf=Path('C:/Users/adamc/Desktop/workspace/DH_sc/.local-inputs/libDungeonHunter2.so');sdk=Path('C:/Users/adamc/AppData/Local/Android/Sdk/ndk/29.0.14206865/toolchains/llvm/prebuilt/windows-x86_64/bin');nm=subprocess.run([str(sdk/'llvm-nm.exe'),'-S','--defined-only','--demangle',str(elf)],capture_output=True,check=True).stdout.decode()
selected=[]
for l in nm.splitlines():
 c=l.split(maxsplit=3)
 if len(c)==4 and c[2] in ('T','W','t') and any(v in c[3] for v in ['CharAI::AI_SetMaster','CharAI::AI_GetMaster','CharAI::_UpdateMaster','Character::_SetMaster','PlayerManager::SpawnPlayer','PlayerManager::LoadPlayer','PlayerManager::SpawnFaery','Level::PlaceFaeryAndFollowers','Level::_LoadProcess','Character::InitSpawned']):
  a=int(c[0],16);size=int(c[1],16);asm=subprocess.run([str(sdk/'llvm-objdump.exe'),'--disassemble','--demangle','--start-address='+hex(a),'--stop-address='+hex(a+size),str(elf)],capture_output=True,check=True).stdout.decode();(base/(hex(a)+'.asm')).write_text(asm);selected.append({'address':hex(a),'size':size,'symbol':c[3]})
(base/'master-producer-symbols-v38.json').write_text(json.dumps(selected,indent=2)+'\n');print(json.dumps(selected))
