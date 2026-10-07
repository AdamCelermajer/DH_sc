"""Original AISExternal target callback gates and argument identity choreography.

The script-call service and borrowed Arguments storage are explicit boundaries;
the original Value/table producer has its separate frozen instruction/VM proof.
"""
import argparse,hashlib,json,struct,sys
from pathlib import Path
from unicorn import UC_HOOK_CODE
ROOT=Path(__file__).resolve().parents[1];REPO=ROOT.parents[1]
sys.path.insert(0,str(REPO/'port/game-data/tests'))
from items_differential import Original,words
from navigation_differential import Cpu
REF=ROOT/'reference/object-identity-lifecycle/target-events'
class Probe(Original):
 def __init__(self):
  super().__init__(REPO/'.local-inputs/libDungeonHunter2.so',json.loads((REF/'original-functions.json').read_text()));self.owner=self.data+0x5000;self.trace=[];self.uc.hook_add(UC_HOOK_CODE,self.hook)
 def text(self,p):
  out=b''
  while self.uc.mem_read(p+len(out),1)!=b'\0':out+=bytes(self.uc.mem_read(p+len(out),1))
  return out.decode()
 def hook(self,uc,a,size,unused):
  if a==0x37c514:self.trace.append(['call0',self.reg(0),self.text(self.reg(1))]);self.returned()
  elif a==0x3192b4:self.arguments=self.reg(0);uc.mem_write(self.arguments,bytes(8));self.trace.append(['arguments']);self.returned(self.arguments)
  elif a==0x386f28:assert self.reg(0)==self.arguments;self.trace.append(['append_object_value',self.reg(1)]);self.returned()
  elif a==0x37c41c:assert self.reg(2)==self.arguments;self.trace.append(['call1',self.reg(0),self.text(self.reg(1))]);self.returned()
  elif a==0x319228:self.trace.append(['destroy_arguments']);self.returned()
def main():
 parser=argparse.ArgumentParser();parser.add_argument('--library',type=Path);a=parser.parse_args();p=Probe();records=[]
 n=Cpu(a.library,True,{'functions':[]}) if a.library else None;trace=[]
 if n:
  state,svc,cb=n.data+0x1000,n.data+0x1100,n.data+0x1200;n.uc.mem_write(cb,bytes.fromhex('c0035fd6'));n.uc.mem_write(svc,struct.pack('<QQ',0,cb))
  def hook(uc,address,size,unused):
   if address!=cb:return
   receiver,text,argument,count,kind=struct.unpack('<QQQII',uc.mem_read(n.reg(1),32));name=b''
   while uc.mem_read(text+len(name),1)!=b'\0':name+=bytes(uc.mem_read(text+len(name),1))
   trace.append([receiver,name.decode(),argument,count,kind]);n.put(0,0);uc.reg_write(n.pc,uc.reg_read(n.lr))
  n.uc.hook_add(UC_HOOK_CODE,hook)
 def compare(event,mask,enemy,expected):
  if not n:return
  trace.clear();n.uc.mem_write(state,struct.pack('<QII',p.owner,mask,0));assert n.invoke('dh2_object_target_event',[state,event,enemy,svc])==0;assert trace==expected,(event,mask,enemy,trace,expected)
 methods=[(0x3dce24,'OnTargetDied',0),(0x3dce14,'OnTargetOutOfSight',0),(0x3dce04,'OnTargetInSight',0),(0x3dcde8,'OnTargetOutOfRange',4),(0x3dcdcc,'OnTargetInRangedRange',8),(0x3dcdb0,'OnTargetInCloseRange',16),(0x3dcd94,'OnTargetInMeleeRange',32)]
 for event,(address,name,bit) in enumerate(methods,2):
  for mask in [*range(64),0x80000000,0xffffffff]:
   p.trace=[];p.pointer(p.owner+0xb8,mask);p.invoke(address,[p.owner]);expected=[['call0',p.owner,name]] if not bit or mask&bit else [];assert p.trace==expected;compare(event,mask,0,[[p.owner,name,0,0,0]] if expected else []);records.append({'address':hex(address),'mask':mask,'trace':p.trace.copy()})
 for enemy in (0,p.owner,p.owner+0x1000):
  p.trace=[];p.invoke(0x3dd2f4,[p.owner,enemy]);assert p.trace==[['arguments'],['append_object_value',enemy],['call1',p.owner,'OnEnemySpotted'],['destroy_arguments']];compare(1,0,enemy,[[p.owner,'OnEnemySpotted',enemy,1,7]]);records.append({'address':'0x3dd2f4','enemy':enemy,'trace':p.trace.copy()})
 result={'validation':'PASS','original_only_cases':len(records),'original_sha256':hashlib.sha256((REPO/'.local-inputs/libDungeonHunter2.so').read_bytes()).hexdigest(),'manifest_sha256':hashlib.sha256((REF/'original-functions.json').read_bytes()).hexdigest(),'script_sha256':hashlib.sha256(Path(__file__).read_bytes()).hexdigest(),'scope':__doc__,'cases':records}
 (REF/'dispatch-original-probe.json').write_text(json.dumps(result,indent=2)+'\n');print(json.dumps({k:v for k,v in result.items() if k!='cases'}))
 if n:
  gold=b'OIE1'+words(len(records))
  for record in records:
   if record['address']=='0x3dd2f4':event,mask,enemy=1,0,record['enemy'];calls=[x for x in record['trace'] if x[0]=='call1'];argument=enemy;argc,kind=1,7
   else:event=next(i for i,x in enumerate(methods,2) if hex(x[0])==record['address']);mask,enemy=record['mask'],0;calls=record['trace'];argument=argc=kind=0
   name=calls[0][2].encode() if calls else b''
   gold+=words(event,mask,enemy,len(calls),p.owner if calls else 0,argument if calls else 0,argc if calls else 0,kind if calls else 0,len(name))+name
  (REF/'dispatch-fixtures.bin').write_bytes(gold)
  result.pop('cases');result['comparisons']=result.pop('original_only_cases');result['arm64_sha256']=hashlib.sha256(a.library.read_bytes()).hexdigest();result['source_sha256']={x.name:hashlib.sha256(x.read_bytes()).hexdigest() for x in (ROOT/'object_identity.hpp',ROOT/'object_identity.cpp')};result['scope']+=' Native callback receiver/name/count/Value7 identity compared; source Arguments allocation/destruction ABI remains explicit storage boundary.';result['mismatches']=0
  result['corpus_sha256']=hashlib.sha256(gold).hexdigest()
  (ROOT/'reports/object-target-events-arm64-differential.json').write_text(json.dumps(result,indent=2)+'\n')
if __name__=='__main__':main()
