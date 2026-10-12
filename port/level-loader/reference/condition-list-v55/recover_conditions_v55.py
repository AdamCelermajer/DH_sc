from pathlib import Path
import subprocess,json,zipfile,sys
root=Path('C:/Users/adamc/Desktop/workspace/DH_sc');b=Path('C:/Users/adamc/.codex/worktrees/generic-level-loader/DH_sc/port/level-loader');r=b/'reference/condition-list-v55';r.mkdir(exist_ok=True)
sdk=Path('C:/Users/adamc/AppData/Local/Android/Sdk/ndk/29.0.14206865/toolchains/llvm/prebuilt/windows-x86_64/bin');elf=root/'.local-inputs/libDungeonHunter2.so'
nm=subprocess.run([str(sdk/'llvm-nm.exe'),'-S','--defined-only','--demangle',str(elf)],capture_output=True,text=True,check=True).stdout.splitlines();selected=[]
for line in nm:
 a=line.split(maxsplit=3)
 if len(a)!=4:continue
 if any(k in a[3] for k in ['ConditionList::','v2ConditionStub','v2ConditionList','v2Conditions','Structs::v2Condition::','Arrays::Conditions','CONDITION_TYPES','Conditions_Names','Conditions_Count','GetCondition']):
  selected.append({'address':hex(int(a[0],16)),'size':int(a[1],16),'kind':a[2],'name':a[3]})
for s in selected:
 if s['kind'] in ['T','W','t']:
  out=subprocess.run([str(sdk/'llvm-objdump.exe'),'--disassemble','--demangle','--start-address='+s['address'],'--stop-address='+hex(int(s['address'],16)+s['size']),str(elf)],capture_output=True,text=True,check=True).stdout;(r/(s['address']+'.asm')).write_text(out)
(r/'symbols-v55.json').write_text(json.dumps(selected,indent=2)+'\n');print(json.dumps(selected,indent=2))
with zipfile.ZipFile('C:/Users/adamc/Downloads/dungeonhunter2/Dungeon-Hunter-2-HD-v1-0-2-cache.zip') as z:
 files=[n for n in z.namelist() if 'condition' in n.lower()];print('cachefiles',files)
 for n in files:
  if n.endswith('.bin'):
   p=r/Path(n).name;p.write_bytes(z.read(n));print(p.name,p.stat().st_size,p.read_bytes()[:100].hex())
(r/'recover_conditions_v55.py').write_bytes(Path(__file__).read_bytes())
