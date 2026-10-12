exec(open('C:/Users/adamc/AppData/Local/Temp/dh2-lightpoint-producers-v53.py').read().split('p=LightProbe();ctor,props=')[0])
class Sync(Parser):
 def __init__(self):
  super().__init__(e,{'functions':[]});self.actor=self.data+0x60000;self.node=self.data+0x61000;self.light=self.data+0x62000;self.uc.hook_add(UC_HOOK_CODE,self.allocation)
 def run(self,radius):
  self.uc.mem_write(self.actor,bytes(0x200));self.uc.mem_write(self.node,bytes(0x200));self.uc.mem_write(self.light,bytes(0x200));self.pointer(self.actor+0x120,self.node);self.pointer(self.node+0x134,self.light);self.pointer(self.light,1)
  self.uc.mem_write(self.actor+0x130,struct.pack('<10f',radius,.25,.5,.75,1,2,3,4,5,6));self.uc.mem_write(self.actor+0x158,struct.pack('<3f',7,8,9));self.uc.mem_write(self.light+0x40,struct.pack('<f',4321))
  self.invoke(0x40a8f8,[self.actor],budget=4000000)
  return {'radius':radius,'actual_CLight_bytes4_44':bytes(self.uc.mem_read(self.light+4,64)).hex(),'refcount':self.word(self.light)}
p=Sync();print('Radius compare import',p.imports.get(0x30e2f8));rows=[p.run(x) for x in [-1.,0.,.005,.01,.0101,2.,3.5]];(r/'original-light-sync-v53.json').write_text(json.dumps({'actual_original_SyncData':True,'actual_node_CLight_storage':'explicit host source-layout fixture, not renderer owner','radius_compare':p.imports.get(0x30e2f8),'rows':rows},indent=2)+'\n');print(json.dumps(rows,indent=2))
