exec(open('C:/Users/adamc/AppData/Local/Temp/dh2-lightpoint-dtor-proof-v55.py').read().split('p=Dtor();rows=')[0])
class Assign(Parser):
 def __init__(self):
  super().__init__(e,{'functions':[]});self.dst=self.data+0x70000;self.src=self.data+0x71000;self.old=self.data+0x72000;self.new=self.data+0x73000;self.uc.hook_add(UC_HOOK_CODE,self.allocation)
 def init(self,at,names,autolist):
  self.uc.mem_write(at,bytes(0x1a4))
  for i,name in enumerate(names):
   p=self.heap;self.heap+=len(name)+32;self.uc.mem_write(p,name.encode()+b'\0');self.invoke(0x3140ec,[at+4+i*24,p,0])
  p=self.heap;self.heap+=32;self.uc.mem_write(p,struct.pack('<'+str(len(autolist))+'I',*autolist) if autolist else bytes(4));self.pointer(at+0x190,p);self.pointer(at+0x194,p+len(autolist)*4);self.pointer(at+0x198,p+32)
 def run(self,source_has_light,self_assign):
  names=['PlayerLight','SceneLight','CameraLight','MonsterLight'];self.init(self.dst,['OLD'+str(i) for i in range(4)],[0x1234,0x5678]);self.init(self.src,names,[])
  self.uc.mem_write(self.old,bytes(0x60));self.uc.mem_write(self.new,bytes(0x60));self.pointer(self.old,2);self.pointer(self.new,2);self.pointer(self.dst+0x78,self.old)
  if source_has_light:self.pointer(self.src+0x78,self.new)
  self.pointer(self.dst+0x1a0,77);self.pointer(self.src+0x1a0,0)
  destination_names_at=self.dst+4;old_auto_buffer=self.word(self.dst+0x190);self.invoke(0x3541d4,[self.dst,self.dst if self_assign else self.src],budget=5000000)
  row={'source_light_present':source_has_light,'self_assignment':self_assign,'destination_names':[self.text(self.word(self.dst+4+i*24+20)) for i in range(4)],'same_destination_name_fields':destination_names_at==self.dst+4,'auto_count_after':(self.word(self.dst+0x194)-self.word(self.dst+0x190))//4,'auto_storage_reused':old_auto_buffer==self.word(self.dst+0x190),'old_light_ref_after':self.word(self.old),'new_light_ref_after':self.word(self.new),'destination_slot78':hex(self.word(self.dst+0x78)),'field1a0_after':self.word(self.dst+0x1a0)}
  if not self_assign:assert row['destination_names']==names and row['auto_count_after']==0 and row['old_light_ref_after']==1 and row['new_light_ref_after']==(3 if source_has_light else 2) and row['field1a0_after']==0
  else:assert row['old_light_ref_after']==2 and row['auto_count_after']==2 and row['field1a0_after']==77
  return row
p=Assign();rows=[p.run(False,False),p.run(True,False),p.run(False,True)];(r/'original-lightset-assignment-v55.json').write_text(json.dumps({'validation':'PASS','actual_original_assignment_and_vector_assignment_bodies':True,'source_manager':'explicit initialized source-layout storage; fresh ctor/driver not supplied','CLight_fixture':'independent refs retained so no final native/GPU free inferred','rows':rows},indent=2)+'\n');print(json.dumps(rows,indent=2))
