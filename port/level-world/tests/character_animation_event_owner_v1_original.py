"""Original AISDefault/CharAI relay with named Lua, foot and FX boundaries."""
from pathlib import Path
import sys,struct,json,hashlib,itertools
sys.path.insert(0,r'C:\Users\adamc\AppData\Local\uv\cache\archive-v0\jc8RoeQ1US_9Gkmi\Lib\site-packages')
root=Path(__file__).resolve().parents[3];sys.path.insert(0,str(root/'port/game-data/tests'))
from aggro_differential import Cpu
from unicorn import UC_HOOK_CODE
c=Cpu(root/'.local-inputs/libDungeonHunter2.so',False,{'functions':[]})
word=lambda a:struct.unpack('<I',c.uc.mem_read(a,4))[0]
base=(0x3dcaa8+8+word(0x3dcc58))&0xffffffff
def global_cell(literal):return word((base+word(literal))&0xffffffff)
size=global_cell(0x3dcc70);members=global_cell(0x3dcc74)
fs_size=global_cell(0x3dcc78);fs_members=global_cell(0x3dcc7c)
ais=c.data+0x1000;actor=c.data+0x2000;rows=c.data+0x5000;steps=c.data+0x6000;floor=c.data+0x8000;strings=c.data+0x10000
c.pointer(ais+0x98,actor);c.pointer(actor+0x4f4,37)
c.pointer(size,2);c.pointer(members,rows);c.pointer(fs_size,2);c.pointer(fs_members,steps)
for i,(effect,trigger) in enumerate([(-1,1),(253,0)]):
 c.pointer(rows+i*24+12,effect&0xffffffff);c.uc.mem_write(rows+i*24+20,bytes([trigger]))
for i,(name,effect) in enumerate([('',-1),('water',275)]):
 at=strings+100+i*32;c.uc.mem_write(at,name.encode()+b'\0');c.pointer(steps+i*32+12,at);c.pointer(steps+i*32+4,effect&0xffffffff)
def string(at):
 result=bytearray()
 while len(result)<1024:
  b=bytes(c.uc.mem_read(at+len(result),1))[0]
  if not b:return result.decode()
  result.append(b)
 raise AssertionError('invalid fixture string')
def ret(value=0):c.put(0,value&0xffffffff);c.uc.reg_write(c.pc,c.uc.reg_read(c.lr))
trace=[];direct_foot=False;found=False
node=c.data+0x12000;output=c.data+0x13000;ai=c.data+0x14000;vtable=c.data+0x15000
c.pointer(ais,vtable);c.pointer(vtable+0x94,0x3dca50);c.pointer(ai+0x1c,ais)
def hook(uc,at,n,unused):
 if at in (0x3192b4,0x319228):ret()
 elif at==0x39ec10:trace.append(['argument',string(c.reg(1))]);ret()
 elif at==0x3cdd78:trace.append(['lag',c.reg(1)]);ret()
 elif at==0x37c41c:trace.append(['lua',string(c.reg(1))]);ret()
 elif at==0x30e31c:ret(0 if string(c.reg(0))==string(c.reg(1)) else 1)
 elif at in (0x3a57f4,0x3a5874) and not direct_foot:
  foot='left' if at==0x3a57f4 else 'right';trace.append(['foot',foot]);uc.mem_write(c.reg(0),struct.pack('<3f',*(11,12,13) if foot=='left' else (22,23,24)));ret(c.reg(0))
 elif at==0x495d14:
  effect=struct.unpack('<i',struct.pack('<I',c.reg(1)))[0];trace.append(['fx',effect,list(struct.unpack('<3f',uc.mem_read(c.reg(2),12)))]);ret()
 elif at==0x3935dc:trace.append(['position']);ret(actor+0x160)
 elif at==0x470a18:trace.append(['node',string(c.reg(1))]);ret(node if found else 0)
c.uc.hook_add(UC_HOOK_CODE,hook)
records=[]
for event,row,floor_name in itertools.product(['step_left','step_right','other','ev_step_left'],[-1,0,1,99],[None,'','water','unknown']):
 c.uc.mem_write(strings,event.encode()+b'\0');c.pointer(actor+0x1014,row&0xffffffff)
 c.pointer(actor+0x1d8,0 if floor_name is None else floor)
 if floor_name is not None:
  c.uc.mem_write(strings+300,floor_name.encode()+b'\0');c.pointer(floor+0x3c,strings+300)
 trace.clear();c.invoke(0x3dca50,[ais,strings]);observed=list(trace)
 expected=[['argument',event],['lag',37],['lua','OnAnimEvent']]
 if event in ('step_left','step_right'):
  foot='left' if event=='step_left' else 'right';point=[float(x) for x in ((11,12,13) if foot=='left' else (22,23,24))]
  expected += [['foot',foot],['fx',253 if row==1 else -1,point]]
  if row!=1 and floor_name in ('','water'):expected.append(['fx',-1 if floor_name=='' else 275,point])
 assert observed==expected,(event,row,floor_name,observed,expected)
 trace.clear();c.invoke(0x3d0cc4,[ai,strings]);assert trace==expected
 records.append(dict(event=event,row=row,floor=floor_name,trace=observed))
c.pointer(ai+0x1c,0);trace.clear();c.invoke(0x3d0cc4,[ai,strings]);assert trace==[]
direct_foot=True;foot_records=[]
c.uc.mem_write(actor+0x160,struct.pack('<3f',1,2,3));c.uc.mem_write(node+0x54,struct.pack('<3f',11,12,13))
for left,visual,found in itertools.product([False,True],[False,True],[False,True]):
 c.pointer(actor+0x2d8,1 if visual else 0);trace.clear()
 c.invoke(0x3a57f4 if left else 0x3a5874,[output,actor])
 point=list(struct.unpack('<3f',c.uc.mem_read(output,12)))
 expected=[11.,12.,13.] if visual and found else [1.,2.,3.]
 assert point==expected
 assert trace==[['position']]+([['node','Bip01_L_Foot' if left else 'Bip01_R_Foot']] if visual else [])
 foot_records.append(dict(left=left,visual=visual,found=found,point=point,trace=list(trace)))
report={'validation':'PASS','original_cases':len(records)*2+1+len(foot_records),'original_sha256':hashlib.sha256((root/'.local-inputs/libDungeonHunter2.so').read_bytes()).hexdigest(),'script_sha256':hashlib.sha256(Path(__file__).read_bytes()).hexdigest(),'scope':__doc__,'lua_foot_fx_services_are_named_fixtures':True,'native_comparisons':0,'records':records,'foot_getters':foot_records}
out=root/'port/level-world/reports/character-animation-event-v1-original-audit.json';out.write_text(json.dumps(report,indent=2)+'\n')
print(json.dumps({k:v for k,v in report.items() if k not in ('records','foot_getters')}))
