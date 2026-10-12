from pathlib import Path
import hashlib,json,shutil,sys,subprocess,zipfile,xml.etree.ElementTree as ET
base=Path('C:/Users/adamc/.codex/worktrees/generic-level-loader/DH_sc/port/level-loader').resolve();temp=Path('C:/Users/adamc/AppData/Local/Temp');test=base/'tests/trigger-trap-v37';reference=base/'reference/trigger-trap-v37';test.mkdir(parents=True,exist_ok=True);reference.mkdir(parents=True,exist_ok=True)
for name,destination in [('canonical_trigger_trap_v37.hpp',base),('canonical_trigger_trap_v37.cpp',base),('canonical_trigger_trap_declarations_v37.inc',base),('canonical_trigger_trap_v37_test.cpp',test),('prove_trigger_trap_original_v37.py',reference)]:
 target=destination/name;assert target.resolve().is_relative_to(base);shutil.copyfile(temp/name,target)
overlay=base/'vendor/character-rng-integration-v4/port/level-world';original=(overlay/'canonical_property_map_v1.cpp').read_text();candidate=original
anchor='kind!="RoomZone"){e="required unrecovered registered class DeclareProperties";'
assert candidate.count(anchor)==1;candidate=candidate.replace(anchor,'kind!="TriggerTrap"&&'+anchor)
anchor='kind=="TriggerZoneExitLevel")?"GameObject":kind;'
assert candidate.count(anchor)==1;candidate=candidate.replace(anchor,'kind=="TriggerTrap"||'+anchor)
anchor=' #include "canonical_trigger_zone_declarations_v22.inc"'
assert candidate.count(anchor)==1;candidate=candidate.replace(anchor,anchor+'\n #include "canonical_trigger_trap_declarations_v37.inc"')
(test/'canonical_property_map_candidate_v37.cpp').write_text(candidate)
z=zipfile.ZipFile('C:/Users/adamc/Downloads/dungeonhunter2/Dungeon-Hunter-2-HD-v1-0-2-cache.zip');prefix='com.gameloft.android.GAND.GloftD2SS/files/';cases=[]
for member in z.infolist():
 if not member.filename.lower().endswith(('.mgp','.mvp')):continue
 b=z.read(member)
 try:node=ET.fromstring(b)
 except ET.ParseError:continue
 for index,obj in enumerate(node):
  if obj.tag=='GameObject' and obj.get('gametype')=='TriggerTrap':cases.append({'source':member.filename[len(prefix):],'source_sha256':hashlib.sha256(b).hexdigest(),'direct_child_index':index,'attributes':dict(obj.attrib)})
assert cases
(reference/'authored-trigger-trap-v37.json').write_text(json.dumps(cases,indent=2)+'\n')
(reference/'authored-trigger-trap-v37.tsv').write_text('\n\n'.join('\n'.join(k+'\t'+v for k,v in case['attributes'].items()) for case in cases)+'\n',encoding='utf8')
subprocess.run([sys.executable,str(reference/'prove_trigger_trap_original_v37.py'),'--output',str(reference/'trigger-trap-original-v37.json')],check=True)
sdk=Path('C:/Users/adamc/AppData/Local/Android/Sdk/ndk/29.0.14206865/toolchains/llvm/prebuilt/windows-x86_64/bin');elf=Path('C:/Users/adamc/Desktop/workspace/DH_sc/.local-inputs/libDungeonHunter2.so')
nm=subprocess.run([str(sdk/'llvm-nm.exe'),'-S','--defined-only','--demangle',str(elf)],capture_output=True,check=True).stdout.decode();lines=[l for l in nm.splitlines() if 'TriggerTrap' in l or 'ZoneEx::ZoneEx(' in l or 'Zone::Zone(' in l or 'Zone::DeclareProperties' in l];(reference/'symbols-v37.txt').write_text('\n'.join(lines)+'\n')
asm=[]
for line in lines:
 cols=line.split(maxsplit=3)
 if cols[2] in ('T','W','t') and any(n in line for n in ['GetNewInstance','TriggerTrap::TriggerTrap','TriggerTrap::InitPost','TriggerTrap::DeclareProperties','ZoneEx::ZoneEx','Zone::Zone','Zone::DeclareProperties','TriggerTrap::TransferVictims','TriggerTrap::~TriggerTrap']):
  a=int(cols[0],16);size=int(cols[1],16);asm.append(subprocess.run([str(sdk/'llvm-objdump.exe'),'--disassemble','--demangle','--start-address='+hex(a),'--stop-address='+hex(a+size),str(elf)],capture_output=True,check=True).stdout.decode())
(reference/'original-constructor-properties-init-v37.asm').write_text('\n'.join(asm))
receipt={'scope':'New unselected TriggerTrap source and isolated candidate property-map only; core overlay and CMake unchanged','original_property_map_sha256':hashlib.sha256((overlay/'canonical_property_map_v1.cpp').read_bytes()).hexdigest(),'candidate_property_map_sha256':hashlib.sha256((test/'canonical_property_map_candidate_v37.cpp').read_bytes()).hexdigest(),'authored_trigger_trap_declarations':len(cases),'source_snapshot_paths':[str(base/'canonical_trigger_trap_v37.hpp'),str(base/'canonical_trigger_trap_v37.cpp')],'source_constructor_proof':str(reference/'trigger-trap-original-v37.json')}
(base/'reports/trigger-trap-v37-source-preparation.json').write_text(json.dumps(receipt,indent=2)+'\n');print(json.dumps(receipt))
