from pathlib import Path
import sys,json,hashlib
sys.path.insert(0,r'C:/Users/adamc/AppData/Local/uv/cache/archive-v0/jc8RoeQ1US_9Gkmi/Lib/site-packages');sys.path.insert(0,r'C:/Users/adamc/Desktop/workspace/DH_sc/port/game-data/tests')
from items_differential import Original
from unicorn import UC_HOOK_CODE
root=Path(r'C:/Users/adamc/.codex/worktrees/generic-level-loader/DH_sc/port/level-loader');ref=root/'reference/level-init-v36';engine=Path(r'C:/Users/adamc/Desktop/workspace/DH_sc/.local-inputs/libDungeonHunter2.so');c=Original(engine,{'functions':[]});manager=c.data+0x4000;flush_seen=[];slice_mode=False
def hook(uc,address,size,unused):
 if slice_mode and address==0x3499d8:uc.reg_write(c.pc,c.stop);uc.emu_stop()
 elif address==0x3496b8:
  assert c.reg(0)==manager;flush_seen.append({'phase7c_before_flush':c.word(manager+0x7c),'map1c_before_flush':c.word(manager+0x1c)});c.put(0,0);uc.reg_write(c.pc,uc.reg_read(c.lr))
c.uc.hook_add(UC_HOOK_CODE,hook);rows=[]
for entry in (0x34a1e8,0x34a404):
 for poison in (0,0xa5,0xff):
  c.uc.mem_write(manager,bytes([poison])*0x200);c.invoke(entry,[manager]);assert c.word(manager+0x7c)==0 and c.word(manager+0x1c)==0;rows.append({'entry':hex(entry),'poison':poison,'phase7c':0,'map1c':0,'flush':flush_seen[-1]})
flush_asm=(ref/'stage-loader-v38-manager-3496b8.asm').read_text();matching=[line.strip() for line in flush_asm.splitlines() if '#0x7c]' in line];assert len(matching)==1 and '3499d4:' in matching[0]
slice_mode=True;flush_slices=[]
for poison in (0,0xa5a5a5a5,0xffffffff):
 c.pointer(manager+0x7c,poison);c.put(4,manager);c.put(0,manager+0x24);c.invoke(0x3499c8,[]);assert c.word(manager+0x7c)==0;flush_slices.append({'poison':poison,'phase7c':0})
report={'validation':'PASS','original_sha256':hashlib.sha256(engine.read_bytes()).hexdigest(),'actual_original_ctor_cases':len(rows),'cases':rows,'scope':'Whole original ObjectManager C1 body executes all field stores, across both aliases and3poisons; Flush is an explicit observer so map1c here is pre-Flush0, not completed constructor reserved-null1. Original Flush phase-reset slice3499c8..3499d4 separately executes actual MOV/STR across3poisons. Existing original Flush reserved-null-node proof remains separate.','phase7c_source_stores':['0x34a2dc','0x34a4f8','0x3499d4'],'phase7c_source_zero_registers':['0x34a200','0x34a41c','0x3499c8'],'flush_direct_phase7c_accesses':matching,'original_flush_phase_slices':flush_slices,'whole_loader_verified':False}
(ref/'stage-loader-v38-manager-ctor-original.json').write_bytes((json.dumps(report,indent=2)+'\n').encode());print(json.dumps({k:v for k,v in report.items() if k not in ('cases','scope')}))
