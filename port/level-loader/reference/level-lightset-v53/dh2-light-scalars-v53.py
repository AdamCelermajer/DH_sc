exec(open('C:/Users/adamc/AppData/Local/Temp/dh2-lightpoint-producers-v53.py').read().split('p=LightProbe();ctor,props=')[0])
class Scalars(Parser):
 def __init__(self):
  super().__init__(e,{'functions':[]});self.actor=self.data+0x60000;self.uc.hook_add(UC_HOOK_CODE,self.allocation);self.uc.hook_add(UC_HOOK_CODE,self.capture)
 def capture(self,uc,a,size,u):
  if a==0x40b138:self.uc.reg_write(self.sp,self.stack+0xe000);self.uc.reg_write(self.pc,self.stop)
 def run(self,v):
  self.uc.mem_write(self.actor,bytes(0x200))
  for o,values in [(0x134,v[0]),(0x140,v[1]),(0x14c,v[2]),(0x158,v[3])]:self.uc.mem_write(self.actor+o,struct.pack('<3f',*values))
  self.invoke(0x40b0c0,[self.actor],budget=3000000)
  return [bytes(self.uc.mem_read(self.actor+o,12)).hex() for o in [0x134,0x140,0x14c,0x158]]
p=Scalars();inputs=[[[.265625,0,.132813],[255,255,255],[255,255,255],[255,255,255]],[[1,1000,1000000],[0,128,255],[-1,12,60],[512,32,100]],[[0,0,0],[0,0,0],[0,0,0],[0,0,0]],[[3,-2000,-800000],[1,3,7],[17,49,99],[100,125,250]]]
rows=[{'input':v,'normalized_float_bytes':p.run(v)} for v in inputs]
(r/'original-light-scalars-v53.json').write_text(json.dumps({'actual_body_prefix':'LightBase.InitPost40b0c0 through40b138 before scene access','rows':rows},indent=2)+'\n');print(json.dumps(rows,indent=2))
# Resolve actual LoadScene xref literal from same ELF bytes, no guessed name.
exec(open('C:/Users/adamc/AppData/Local/Temp/dh2-lightset-discover-v53.py').read().split('for row in rows:')[0])
print('Source LoadScene reference literal:',repr(txt(0x40b154+8+w(0x40b3b0))))
