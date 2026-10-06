from pathlib import Path
import sys,json,struct,hashlib
sys.path.insert(0,r'C:/Users/adamc/AppData/Local/uv/cache/archive-v0/jc8RoeQ1US_9Gkmi/Lib/site-packages')
root=Path(r'C:\Users\adamc\Desktop\workspace\DH_sc');sys.path.insert(0,str(root/'port/game-data/tests'))
from aggro_differential import Cpu
from unicorn import UC_HOOK_CODE
c=Cpu(root/'.local-inputs/libDungeonHunter2.so',False,{'functions':[]});obj=c.data+0x1000;heap=c.data+0x5000;mode='ctor';properties={};calls=[];queries=[]
def word(p):return struct.unpack('<I',c.uc.mem_read(p,4))[0]
def text(p):
 b=bytearray()
 while c.uc.mem_read(p,1)!=b'\0':b+=c.uc.mem_read(p,1);p+=1
 return b.decode()
def ret(v=None):
 if v is not None:c.put(0,v)
 c.uc.reg_write(c.pc,c.uc.reg_read(c.lr))
def hook(uc,address,size,user):
 global heap
 if address==0x310570:
  n=c.reg(0);assert c.reg(1)==0
  if mode=='ctor':assert n==928;out=obj
  else:assert n in (36,56);out=heap;heap+=0x100;c.uc.mem_write(out,b'\xa5'*n)
  calls.append({'allocation':n});ret(out)
 elif address==0x38c398:assert c.reg(0)==obj and c.reg(1)==20;calls.append({'base_GO_ID':20});ret()
 elif address==0x38cee8:assert c.reg(0)==obj;calls.append({'inherited_GameObject_properties':True});ret()
 elif address==0x31167c:
  target=c.reg(0);assert c.reg(1)==16;c.pointer(target+0x10,target);c.pointer(target+0x14,target);ret()
 elif address==0x3116e8:
  target,begin,end=[c.reg(i) for i in range(3)];assert begin==end;c.pointer(target+0x10,target);c.pointer(target+0x14,target);c.uc.mem_write(target,b'\0');ret()
 elif address in (0x3139ac,0x3140ec):ret()
 elif address==0x4c4bdc:
  queries.append({'group':text(c.reg(1)),'key':text(c.reg(2)),'fixture_value':123 if not queries else 456});ret(queries[-1]['fixture_value'])
 elif address==0x513ce4:
  assert c.reg(0)==obj+4;name=text(c.reg(1));p=c.reg(2);offset=word(p+4)+4
  if offset==0x374:default=c.uc.mem_read(p+0x20,1)[0]
  elif offset==0x378:default=''
  else:assert offset in (0x394,0x398);default=struct.unpack('<f',c.uc.mem_read(p+0x20,4))[0]
  properties[name]={'source_offset':offset,'default':default};ret()
c.uc.hook_add(UC_HOOK_CODE,hook);cases=[]
for poison in (0,0xa5,0x7f):
 c.uc.mem_write(obj,bytes([poison])*928);calls.clear();c.invoke(0x340ccc,[]);assert c.reg(0)==obj
 fields={'sound378_empty':c.uc.mem_read(obj+0x378,1)[0]==0,'sound_id390':word(obj+0x390),'fade394':struct.unpack('<f',c.uc.mem_read(obj+0x394,4))[0],'fade398':struct.unpack('<f',c.uc.mem_read(obj+0x398,4))[0],'byte39c':c.uc.mem_read(obj+0x39c,1)[0],'updatable85':c.uc.mem_read(obj+0x85,1)[0],'enabled374_unproduced':c.uc.mem_read(obj+0x374,1)[0]==poison}
 assert fields=={'sound378_empty':True,'sound_id390':0xffffffff,'fade394':-1.,'fade398':-1.,'byte39c':0,'updatable85':1,'enabled374_unproduced':True},fields
 declare=word(word(obj)+0x18);assert declare==0x395718
 cases.append({'poison':poison,'fields':fields,'declare_properties':hex(declare),'calls':list(calls)})
mode='properties';calls.clear();c.invoke(0x395718,[obj]);print(json.dumps({'properties':properties,'queries':queries},indent=2))
report={'validation':'PASS','original_sha256':hashlib.sha256((root/'.local-inputs/libDungeonHunter2.so').read_bytes()).hexdigest(),'factory_cases':cases,'properties':properties,'constant_queries':queries,'scope':'Whole original SoundEmitter ctor/schema; inherited GameObject, allocator/string services intercepted; dynamic constant return values explicit fixtures, not production defaults.'}
out=Path(r'C:\Users\adamc\.codex\worktrees\generic-level-loader\DH_sc\port\level-loader\reference/sound-emitter-v32');out.mkdir(exist_ok=True);(out/'sound-emitter-original-v32.json').write_text(json.dumps(report,indent=2)+'\n')
