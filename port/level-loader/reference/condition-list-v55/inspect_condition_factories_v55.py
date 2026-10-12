from pathlib import Path
import json,subprocess,sys,struct
sys.path[:0]=['C:/Users/adamc/AppData/Local/uv/cache/archive-v0/jc8RoeQ1US_9Gkmi/Lib/site-packages','C:/Users/adamc/Desktop/workspace/DH_sc/port/game-data/tests']
from aggro_differential import Cpu
root=Path('C:/Users/adamc/Desktop/workspace/DH_sc');r=Path('C:/Users/adamc/.codex/worktrees/generic-level-loader/DH_sc/port/level-loader/reference/condition-list-v55');elf=root/'.local-inputs/libDungeonHunter2.so';c=Cpu(elf,False,{'functions':[]});read=lambda a:struct.unpack('<I',c.uc.mem_read(a,4))[0];table=0x478960+read(0x478998);print('factorytable',hex(table),[hex(read(table+4*i)) for i in range(7)])
nm=subprocess.run(['C:/Users/adamc/AppData/Local/Android/Sdk/ndk/29.0.14206865/toolchains/llvm/prebuilt/windows-x86_64/bin/llvm-nm.exe','-S','--defined-only','--demangle',str(elf)],capture_output=True,text=True,check=True).stdout.splitlines();chosen=[]
for line in nm:
 a=line.split(maxsplit=3)
 if len(a)!=4:continue
 if a[3].startswith('Condition::') or 'Condition_Is' in a[3] or a[3].startswith('vtable for Condition'):
  chosen.append({'address':hex(int(a[0],16)),'size':int(a[1],16),'kind':a[2],'name':a[3]})
mapping={x['address']:x['name'] for x in chosen};data=json.loads((r/'original-conditions-v55.json').read_text());ops={}
for row in data['original_table_rows']:
 for s in row['original_constructed_instances']:
  ops[s['op']]={'factory':hex(read(table+4*s['op'])),'name':mapping.get(hex(read(table+4*s['op']))),'vtable':s['receiver_vtable'],'cache8':s['source_cache8']}
print(json.dumps(ops,indent=2));(r/'condition-factory-source-v55.json').write_text(json.dumps({'table':hex(table),'slots':[{'op':i,'entry':hex(read(table+4*i)),'name':mapping.get(hex(read(table+4*i)))} for i in range(7)],'original_constructed_by_type':ops,'symbols':chosen},indent=2)+'\n')
for x in chosen:
 if x['name'] in ['Condition::Condition()','Condition::~Condition()']:
  out=subprocess.run(['C:/Users/adamc/AppData/Local/Android/Sdk/ndk/29.0.14206865/toolchains/llvm/prebuilt/windows-x86_64/bin/llvm-objdump.exe','--disassemble','--demangle','--start-address='+x['address'],'--stop-address='+hex(int(x['address'],16)+x['size']),str(elf)],capture_output=True,text=True,check=True).stdout;(r/(x['address']+'.asm')).write_text(out)
