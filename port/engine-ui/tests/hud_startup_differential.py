"""Original wrapper corpus versus optimized ARM64 with required service boundaries."""
import argparse,json,pathlib,hashlib,struct,sys
R=pathlib.Path(__file__).resolve().parents[3]
sys.path.insert(0,str(R/'port/engine-resources/tests'))
from cpu import Cpu
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest()
class Native(Cpu):
 def external(self,uc,address,size,unused):
  if self.imports.get(address)!='startup_service':return super().external(uc,address,size,unused)
  assert self.reg(0)==self.context and self.reg(1)==self.state
  op,arg,subject,name,a,b,d,reserved=struct.unpack('<IIQQ4I',uc.mem_read(self.reg(2),40));assert reserved==0
  key=bytes(uc.mem_read(name,32)).split(b'\0')[0].decode() if name else ''
  self.calls.append([op,self.ids[subject],arg,key,[a,b,d]])
  value=0
  if op==1 and self.row[9]&1:self.pointer(self.state+8,self.identities[5])
  elif op==3:value=self.row[5 if key=='VolumeMusic' else 6]
  elif op==5:
   value=self.row[7]
   if self.row[9]&2:self.pointer(self.state+8,self.identities[6])
  elif op==7:
   value=self.row[8]
   if self.row[9]&1:self.pointer(self.state+24,self.identities[7])
  elif op==8:self.answer=arg
  uc.mem_write(self.reg(3),struct.pack('<IIQ',value&0xffffffff,0,0));self.put(0,0);uc.reg_write(self.pc,uc.reg_read(self.lr))
 def prepare(self,row):
  self.row=row;self.calls=[];self.answer=0
  self.state=self.data+0x1000;self.services=self.data+0x2000;self.context=self.data+0x3000
  self.identities={0:0,**{i:self.data+0x10000+i*0x1000 for i in range(1,8)}};self.ids={v:k for k,v in self.identities.items()}
  self.uc.mem_write(self.state,struct.pack('<4Q4I',self.identities[1],self.identities[2],self.identities[3] if row[4] else 0,self.identities[4],*row[1:4],0))
  self.uc.mem_write(self.services,struct.pack('<QQ',self.context,self.callback));self.imports[self.callback]='startup_service'
 def run(self,row):
  self.prepare(row)
  if row[0]==3:
   self.uc.mem_write(self.state,struct.pack('<4I',*row[10:14]));self.answer=self.invoke('dh2_hud_device_pipeline',[self.state])
  else:assert self.invoke('dh2_hud_load_settings' if row[0]==1 else 'dh2_hud_is_multiplayer_enabled',[self.state,self.services])==0
  return self.answer,self.calls
def main():
 p=argparse.ArgumentParser();p.add_argument('--library',type=pathlib.Path,required=True);p.add_argument('--original',type=pathlib.Path,required=True);p.add_argument('--gold-json',type=pathlib.Path,required=True);p.add_argument('--gold',type=pathlib.Path,required=True);p.add_argument('--report',type=pathlib.Path,required=True);a=p.parse_args()
 corpus=json.loads(a.gold_json.read_text());assert corpus['validation']=='PASS' and corpus['original_sha256']==sha(a.original)
 cpu=Native(a.library,True,{'functions':[]});records=[]
 for index,case in enumerate(corpus['cases']):
  answer,calls=cpu.run(case['input']);assert answer==case['answer'] and calls==case['calls'],(index,answer,calls,case)
  raw=struct.pack('<16I',*case['input'],case['answer'],len(calls))
  for op,subject,arg,name,values in calls:raw+=struct.pack('<7I',op,subject,arg,{'':0,'VolumeMusic':1,'VolumeFX':2}[name],*values)
  records.append(raw)
 a.gold.parent.mkdir(parents=True,exist_ok=True);a.gold.write_bytes(struct.pack('<II',0x31534348,len(records))+b''.join(records))
 report={'validation':'PASS','comparisons':len(records),'ordered_services':corpus['ordered_services'],'mismatches':0,'original_sha256':sha(a.original),'arm64_library_sha256':sha(a.library),'original_corpus_sha256':sha(a.gold_json),'gold_sha256':sha(a.gold),'source_sha256':{f'port/engine-ui/hud_startup_callbacks{ext}':sha(R/f'port/engine-ui/hud_startup_callbacks{ext}') for ext in ('.hpp','.cpp')},'script_sha256':sha(pathlib.Path(__file__)),'scope':corpus['scope']+' Native full64 callback/context/receiver identities; ARM64 optimized kernel executes source projection. No complete Savegame/profile/audio or AS callback integration claim.'}
 a.report.parent.mkdir(parents=True,exist_ok=True);a.report.write_text(json.dumps(report,indent=2)+'\n');print(json.dumps({k:report[k] for k in ('validation','comparisons','ordered_services','mismatches')}))
if __name__=='__main__':main()
