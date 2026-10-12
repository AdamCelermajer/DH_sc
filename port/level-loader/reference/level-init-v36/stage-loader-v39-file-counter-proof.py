"""Actual Level13c stores/reset/ADD32 and separate field140 pointer.
Opcode slices execute original instructions with actual receiver/caller zero
register fixtures justified by prior MOVs. No file/generator/save effects run.
"""
from pathlib import Path
import sys,struct,json,hashlib
sys.path.insert(0,r'C:/Users/adamc/AppData/Local/uv/cache/archive-v0/jc8RoeQ1US_9Gkmi/Lib/site-packages');sys.path.insert(0,r'C:/Users/adamc/Desktop/workspace/DH_sc/port/game-data/tests')
from items_differential import Original
from unicorn import UC_HOOK_CODE
engine=Path(r'C:/Users/adamc/Desktop/workspace/DH_sc/.local-inputs/libDungeonHunter2.so');c=Original(engine,{'functions':[]});level=c.data+0x4000;stop=0
def hook(uc,address,size,unused):
 if address==stop:uc.reg_write(c.pc,c.stop);uc.emu_stop()
c.uc.hook_add(UC_HOOK_CODE,hook);rows=[]
for entry,end,zero in ((0x3f3258,0x3f325c,6),(0x3f35f0,0x3f35f4,6),(0x3f7524,0x3f7528,7)):
 for poison in (0,0xa5a5a5a5,0xffffffff):
  c.pointer(level+0x13c,poison);c.pointer(level+0x140,0xfacebeef);c.put(4,level);c.put(zero,0);stop=end;c.invoke(entry,[]);assert c.word(level+0x13c)==0 and c.word(level+0x140)==0xfacebeef;rows.append({'entry':hex(entry),'initial':poison,'result':0})
increments=[]
for initial in (0,1,5,0x7fffffff,0x80000000,0xfffffffe,0xffffffff):
 c.pointer(level+0x13c,initial);c.pointer(level+0x140,0xfacebeef);c.put(4,level);stop=0x3f72cc;c.invoke(0x3f72c0,[]);result=c.word(level+0x13c);assert result==(initial+1)&0xffffffff and c.word(level+0x140)==0xfacebeef;increments.append({'initial':initial,'result':result})
report={'validation':'PASS','original_sha256':hashlib.sha256(engine.read_bytes()).hexdigest(),'original_reset_cases':len(rows),'original_add32_cases':len(increments),'resets':rows,'increments':increments,'actual_scalar13c_width_bits':32,'field140_pointer_untouched':True,'scope':__doc__,'whole_level_init_verified':False}
(Path(__file__).parent/'stage-loader-v39-file-counter-original.json').write_bytes((json.dumps(report,indent=2)+'\n').encode());print(json.dumps({k:v for k,v in report.items() if k not in ('scope','resets','increments')}))
