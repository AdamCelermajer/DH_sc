exec(open('C:/Users/adamc/AppData/Local/Temp/dh2-lightpoint-dtor-proof-v55.py').read().split('p=Dtor();rows=')[0])
class Reset(Parser):
 def __init__(self):
  super().__init__(e,{'functions':[]});self.manager=self.data+0x60000;self.light=self.data+0x61000;self.uc.hook_add(UC_HOOK_CODE,self.allocation)
 def run(self,indices):
  self.uc.mem_write(self.manager,bytes([0xa5])*0x1a4);slots=[0x78+row*20+col*4 for row in range(4) for col in range(5)]+[0xc8+i*4 for i in range(5)]
  for off in slots:self.pointer(self.manager+off,0)
  self.uc.mem_write(self.light,bytes(0x60));self.pointer(self.light,len(indices)+1)
  for i in indices:self.pointer(self.manager+slots[i],self.light)
  before_names=bytes(self.uc.mem_read(self.manager+4,96));before_auto=bytes(self.uc.mem_read(self.manager+0x190,12));self.pointer(self.manager+0x1a0,99)
  self.invoke(0x40cbc0,[self.manager],budget=4000000)
  result={'occupied_slot_indices':indices,'all25_slots_cleared':all(self.word(self.manager+off)==0 for off in slots),'shared_light_refcount_before':len(indices)+1,'shared_light_refcount_after':self.word(self.light),'field1a0_after':self.word(self.manager+0x1a0),'names_bytes_preserved':before_names==bytes(self.uc.mem_read(self.manager+4,96)),'automatic_vector_preserved':before_auto==bytes(self.uc.mem_read(self.manager+0x190,12))}
  assert result['all25_slots_cleared'] and result['shared_light_refcount_after']==1 and result['field1a0_after']==0 and result['names_bytes_preserved'] and result['automatic_vector_preserved'];return result
p=Reset();rows=[p.run([]),p.run([0]),p.run([0,4,5,19,20,24]),p.run(list(range(25)))];(r/'original-lightset-reset-v55.json').write_text(json.dumps({'validation':'PASS','actual_original_ResetLightSets_body':True,'final_CLight_delete_and_GPU':'not reached; fixture keeps independent ref1','rows':rows},indent=2)+'\n');print(json.dumps(rows,indent=2))
# Preserve exact assignment body for clear reset/retention analysis.
exec(open('C:/Users/adamc/AppData/Local/Temp/dh2-light-lifetime-discover-v55.py').read().split('rows=')[0]);a=0x3541d4;n=0x294
(r/'source-003541d4.asm').write_text(subprocess.check_output([str(tool/'llvm-objdump.exe'),'-d','--demangle','--start-address='+hex(a),'--stop-address='+hex(a+n),str(e)],text=True))
