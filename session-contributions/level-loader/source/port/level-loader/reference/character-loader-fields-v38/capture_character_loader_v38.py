from pathlib import Path
import subprocess,hashlib,json,re,sys
sys.path.insert(0,'C:/Users/adamc/AppData/Local/uv/cache/archive-v0/jc8RoeQ1US_9Gkmi/Lib/site-packages')
from elftools.elf.elffile import ELFFile
base=Path('C:/Users/adamc/.codex/worktrees/generic-level-loader/DH_sc/port/level-loader');out=base/'reference/character-loader-fields-v38';out.mkdir(parents=True,exist_ok=True)
elf=Path('C:/Users/adamc/Desktop/workspace/DH_sc/.local-inputs/libDungeonHunter2.so');sdk=Path('C:/Users/adamc/AppData/Local/Android/Sdk/ndk/29.0.14206865/toolchains/llvm/prebuilt/windows-x86_64/bin');assert hashlib.sha256(elf.read_bytes()).hexdigest()=='36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80'
nm=subprocess.run([str(sdk/'llvm-nm.exe'),'-S','--defined-only','--demangle',str(elf)],capture_output=True,check=True).stdout.decode();symbols=[]
for l in nm.splitlines():
 c=l.split(maxsplit=3)
 if len(c)==4 and c[2] in ('T','W','t'):symbols.append({'address':int(c[0],16),'size':int(c[1],16),'name':c[3]})
selected=[s for s in symbols if any(x in s['name'] for x in ['GetNewInstance<Character>','Character::Character(ObjectBase','Character::GetCharModelName','Character::SafeGetCharPropsTemplateId','Character::SafeGetCharPropsId','Character::SG_GetCurrentFaerieId','Character::GetCharFaery(int)','Character::GetCharModelId','Character::InitPost()','CharAI::CharAI(','Character::IsFaerie()','Level::_LoadPlayer()','Character::SetFaerie','Character::SetFaery'])]
for s in selected:
 a=s['address'];asm=subprocess.run([str(sdk/'llvm-objdump.exe'),'--disassemble','--demangle','--start-address='+hex(a),'--stop-address='+hex(a+s['size']),str(elf)],capture_output=True,check=True).stdout.decode();(out/(hex(a)+'.asm')).write_text(asm)
asm=subprocess.run([str(sdk/'llvm-objdump.exe'),'--disassemble','--demangle',str(elf)],capture_output=True,check=True).stdout.decode()
current='';context=[];matches=[]
for line in asm.splitlines():
 if re.match(r'^[0-9a-f]+ <',line):current=line;context=[]
 context.append(line);context=context[-9:]
 if re.search(r'#0x(?:13ca|13c8|418)\b',line) and ('Character::' in current or 'CharAI::' in current or 'Level::' in current or 'PlayerManager::' in current):matches.append({'symbol':current,'instruction':line,'preceding':list(context)})
(out/'field-reference-index-v38.json').write_text(json.dumps(matches,indent=2)+'\n');(out/'symbols-v38.json').write_text(json.dumps(selected,indent=2)+'\n')
print(json.dumps({'captured_functions':len(selected),'field_matches':len(matches),'functions_with_field_matches':sorted(set(m['symbol'] for m in matches))}))
