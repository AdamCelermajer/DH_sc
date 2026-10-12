from pathlib import Path
import sys,struct,json,subprocess
sys.path[:0]=['C:/Users/adamc/AppData/Local/uv/cache/archive-v0/jc8RoeQ1US_9Gkmi/Lib/site-packages','C:/Users/adamc/Desktop/workspace/DH_sc/port/game-data/tests']
from aggro_differential import Cpu
from unicorn import UC_HOOK_CODE
root=Path('C:/Users/adamc/Desktop/workspace/DH_sc');r=Path('C:/Users/adamc/.codex/worktrees/generic-level-loader/DH_sc/port/level-loader/reference/condition-list-v55');elf=root/'.local-inputs/libDungeonHunter2.so';c=Cpu(elf,False,{'functions':[]});u=c.uc;read=lambda a:struct.unpack('<I',u.mem_read(a,4))[0];heap=c.data+0x10000;poison=0;alloc=[];frees=[]
def ret(v=0):c.put(0,v);u.reg_write(c.pc,u.reg_read(c.lr))
def hook(uc,a,s,x):
 global heap
 if a==0x310570:
  n=c.reg(0);p=heap;heap+=32;u.mem_write(p,bytes([poison])*32);alloc.append({'bytes':n,'hint':c.reg(1),'receiver':p});ret(p)
 elif a==0x310440:frees.append({'receiver':c.reg(0),'vtable_before_free':hex(read(c.reg(0))),'stub_before_free':read(c.reg(0)+4)});ret()
u.hook_add(UC_HOOK_CODE,hook);slots=json.loads((r/'condition-factory-source-v55.json').read_text())['slots'];rows=[];profiles=[];asm=set()
for slot in slots:
 for poison in [0,0xa5,0x7f]:
  alloc.clear();frees.clear();c.invoke(int(slot['entry'],16),[]);obj=c.reg(0);assert len(alloc)==1 and alloc[0]['receiver']==obj and alloc[0]['hint']==0;assert read(obj+4)==0;vt=read(obj);evalentry=read(vt+8);d0=read(vt+4);c.invoke(d0,[obj]);assert len(frees)==1 and frees[0]['receiver']==obj
  if slot['op']<=2:assert alloc[0]['bytes']==12 and read(obj+8)==slot['op'];scalar=slot['op']
  else:assert alloc[0]['bytes']==8 and read(obj+8)==int.from_bytes(bytes([poison])*4,'little');scalar=None
  row={'op':slot['op'],'factory':slot['entry'],'allocation_bytes':alloc[0]['bytes'],'poison':poison,'source_vtable':hex(vt),'source_stub4':0,'comparator8':scalar,'extra_word_not_owned_for_8byte_classes':slot['op']>2,'Eval_entry':hex(evalentry),'deleting_destructor':hex(d0),'original_D0_free':frees[0]};rows.append(row);asm.add(d0)
  if poison==0:profiles.append({k:v for k,v in row.items() if k not in ['poison','original_D0_free','source_stub4']})
sdk=Path('C:/Users/adamc/AppData/Local/Android/Sdk/ndk/29.0.14206865/toolchains/llvm/prebuilt/windows-x86_64/bin');nm=subprocess.run([str(sdk/'llvm-nm.exe'),'-S','--defined-only','--demangle',str(elf)],capture_output=True,text=True,check=True).stdout.splitlines();symbols={}
for line in nm:
 a=line.split(maxsplit=3)
 if len(a)==4:symbols[int(a[0],16)]=(int(a[1],16),a[3])
for a in asm:
 n,name=symbols[a];out=subprocess.run([str(sdk/'llvm-objdump.exe'),'--disassemble','--demangle','--start-address='+hex(a),'--stop-address='+hex(a+n),str(elf)],capture_output=True,text=True,check=True).stdout;(r/(hex(a)+'.asm')).write_text(out)
for p in profiles:p['class_name']=symbols[int(p['factory'],16)][1];p['Eval_name']=symbols[int(p['Eval_entry'],16)][1];p['D0_name']=symbols[int(p['deleting_destructor'],16)][1]
(r/'condition-constructors-original-v55.json').write_text(json.dumps({'status':'PASS','source_factory_cases':rows,'profiles':profiles,'scope':'Whole actual original seven factory C1/base C1 and actual D0 bodies execute unchanged; only allocation/free leaves supplied. No Eval invoked. Types0..2 comparator8 exists; types3..6 actual allocation8 owns NO field8.'},indent=2)+'\n');(r/'prove_condition_factories_v55.py').write_bytes(Path(__file__).read_bytes());print(json.dumps(profiles,indent=2));print('PASS original actual factory7 xpoison3=21; actualD0/free21; no Eval')
