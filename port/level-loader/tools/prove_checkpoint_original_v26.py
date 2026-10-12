from pathlib import Path
import sys,json,struct,hashlib
sys.path.insert(0,r'C:/Users/adamc/AppData/Local/uv/cache/archive-v0/jc8RoeQ1US_9Gkmi/Lib/site-packages')
root=Path(r'C:\Users\adamc\Desktop\workspace\DH_sc');sys.path.insert(0,str(root/'port/game-data/tests'))
from aggro_differential import Cpu
from unicorn import UC_HOOK_CODE
from unicorn.arm_const import UC_ARM_REG_R0
c=Cpu(root/'.local-inputs/libDungeonHunter2.so',False,{'functions':[]});obj=c.data+0x1000;prop=c.data+0x4000;calls=[];mode='ctor'
def returned():c.uc.reg_write(c.pc,c.uc.reg_read(c.lr))
def text(address):
 b=bytearray()
 while c.uc.mem_read(address,1)!=b'\0':b+=c.uc.mem_read(address,1);address+=1
 return b.decode()
def word(address):return struct.unpack('<I',c.uc.mem_read(address,4))[0]
def hook(uc,address,size,user):
 if address==0x310570:
  assert c.reg(1)==0
  if mode=='ctor':assert c.reg(0)==928;out=obj
  else:assert c.reg(0)==44;out=prop;c.uc.mem_write(prop,b'\xa5'*44)
  calls.append({'allocation':c.reg(0)});c.uc.reg_write(UC_ARM_REG_R0,out);returned()
 elif address==0x38c398:
  assert c.reg(0)==obj and c.reg(1)==12;calls.append({'base_GO_ID':12});returned()
 elif address==0x38cee8:
  assert c.reg(0)==obj;calls.append({'inherited_GameObject_declarations':True});returned()
 elif address==0x3140ec:
  calls.append({'property_name':text(c.reg(1))});returned()
 elif address==0x513ce4:
  assert c.reg(0)==obj+4 and c.reg(2)==prop
  defaults=list(struct.unpack('<fff',c.uc.mem_read(prop+0x20,12)))
  calls.append({'added_property':text(c.reg(1)),'absolute_source_offset':word(prop+4)+4,'defaults':defaults});returned()
c.uc.hook_add(UC_HOOK_CODE,hook);cases=[]
for poison in [0,0xa5,0x7f]:
 calls.clear();c.uc.mem_write(obj,bytes([poison])*0x400);c.invoke(0x340f2c,[]);assert c.reg(0)==obj
 fields={'dimensions':list(struct.unpack('<fff',c.uc.mem_read(obj+0x374,12))),'physical380':c.uc.mem_read(obj+0x380,1)[0],'trigger381':c.uc.mem_read(obj+0x381,1)[0],'static84':c.uc.mem_read(obj+0x84,1)[0],'colzone384':word(obj+0x384),'empty388':c.uc.mem_read(obj+0x388,1)[0]==0,'word38c':word(obj+0x38c),'begin390':word(obj+0x390)-obj,'end394':word(obj+0x394)-obj,'word398':word(obj+0x398)}
 assert fields=={'dimensions':[0.0,0.0,0.0],'physical380':1,'trigger381':1,'static84':1,'colzone384':0,'empty388':True,'word38c':0,'begin390':0x388,'end394':0x388,'word398':0}
 assert word(word(obj)+0x18)==0x397df8,'Checkpoint property virtual differs from actual Zone'
 cases.append({'poison':poison,'fields':fields,'calls':list(calls)})
mode='properties';calls.clear();c.invoke(0x397df8,[obj]);assert calls[-1]['absolute_source_offset']==0x374 and calls[-1]['defaults']==[200.0]*3
report={'validation':'PASS','original_sha256':hashlib.sha256((root/'.local-inputs/libDungeonHunter2.so').read_bytes()).hexdigest(),'factory_cases':cases,'zone_properties':list(calls),'scope':'Whole Checkpoint factory→CheckpointC1→ZoneC2 stores; inherited GameObject C1/DeclareProperties and allocator/CString services explicit interceptions. Zone property virtual and actual dimension default proved. Activation/save/lifecycle unported.'}
(Path(__file__).parent/'checkpoint-original-v26.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(report,indent=2))
