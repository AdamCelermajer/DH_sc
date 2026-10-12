exec(open('C:/Users/adamc/AppData/Local/Temp/dh2-prove-lightset-v53.py').read().split('p=Probe();rows=')[0])
class LightProbe(Parser):
 def __init__(self):
  super().__init__(e,{'functions':[]});self.actor=self.data+0x60000;self.properties=[];self.uc.hook_add(UC_HOOK_CODE,self.allocation);self.uc.hook_add(UC_HOOK_CODE,self.capture)
 def capture(self,uc,a,size,u):
  if a==0x513ce4:
   p=self.reg(2);name=self.text(self.reg(1));offset=self.word(p+4)+4;vt=self.word(p);raw=bytes(self.uc.mem_read(p+32,24)).hex();self.properties.append({'name':name,'offset':hex(offset),'property_vtable':hex(vt),'default_bytes':raw});self.returned()
 def run(self):
  self.uc.mem_write(self.actor,bytes([0xA5])*0x200);self.invoke(0x40bdd8,[self.actor],budget=4000000)
  ctor={hex(o):self.word(self.actor+o) for o in [0xf4,0x64,0x120,0x124,0x128,0x12c,0x134,0x138,0x13c,0x140,0x144,0x148,0x14c,0x150,0x154,0x158,0x15c,0x160,0x1a4,0x1a8,0x1ac]};ctor['85']=bytes(self.uc.mem_read(self.actor+0x85,1)).hex()
  self.invoke(0x40bfc4,[self.actor],budget=7000000);return ctor,self.properties
p=LightProbe();ctor,props=p.run();report={'original_sha256':hashlib.sha256(e.read_bytes()).hexdigest(),'actual_C1_and_DeclareProperties_executed':True,'property_registration_endpoint':'capture only, original declaration bodies/string/default producers execute','constructor':ctor,'properties':props};(r/'original-lightpoint-producers-v53.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(report,indent=2))
