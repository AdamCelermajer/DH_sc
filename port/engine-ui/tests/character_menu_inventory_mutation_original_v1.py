"""Execute original RemoveItem, IsEquipped and selection bodies.
Only metadata lookup, destructors and allocation/free are boundary fixtures.
"""
import json,struct,sys
from pathlib import Path
sys.path.insert(0,str(Path(__file__).resolve().parent))
from character_menu_native_v1_original import Original,ROOT,MenuCpu
original_external=MenuCpu.external
def mutation_external(self,uc,a,size,u):
 if self.imports.get(a)=='free':self.put(0,0);uc.reg_write(self.pc,uc.reg_read(self.lr));return
 return original_external(self,uc,a,size,u)
MenuCpu.external=mutation_external
class Mutation(Original):
 def hook(self,uc,a,size,u):
  if self.active:
   if getattr(self,'constructing',False) and a==0x3ff178:
    vector=self.c.reg(0);begin=self.word(vector)
    if not begin:begin=self.alloc(24);self.c.pointer(vector,begin);self.c.pointer(vector+4,begin);self.c.pointer(vector+8,begin+24)
    end=self.word(vector+4);row=self.alloc(36);self.c.pointer(end,row);self.c.pointer(end+4,row);self.c.pointer(end+8,row+36);self.c.pointer(vector+4,end+12);self.ret();return
   if getattr(self,'transferring',False):
    if a==0x3fc3e0:
     self.effects.append(['split',self.c.reg(1)]);self.ret(self.split);return
    if a==0x3f9d04:
     self.effects.append(['unequip',self.c.reg(1),self.c.reg(2)]);self.ret();return
    if a==0x3ff5d4:
     self.effects.append(['add',int(self.c.reg(1)==self.item),self.c.reg(2),self.c.reg(3),int(self.word(inv+0x24)==self.item)]);self.ret(17);return
   if a==0x3f9e08:self.ret(self.metadata);return
   if a==0x3f9d00:self.destroyed.append(self.c.reg(0));self.ret();return
   if self.named.get(a,'').startswith(('_ZN13ItemInventory','_ZNK13ItemInventory')):return
  super().hook(uc,a,size,u)
m=Mutation();inv=m.alloc(128);entries=m.alloc(12);cell=m.alloc(12);other=m.alloc(12);sets=m.alloc(24);equipment=[m.alloc(36),m.alloc(36)];m.metadata=m.alloc(164);vt=m.alloc(16)
m.c.pointer(m.item,vt);m.c.pointer(vt+4,0x3f9d00)
cases=[]
for target in (-5,-4,-3,-2,-1,0,1,2,5,6,8):
 for selected in (0,1):
  for mask in range(4):
   for potion in (0,1):
    m.c.uc.mem_write(inv,bytes(128));m.c.uc.mem_write(cell,bytes(12));m.c.uc.mem_write(sets,bytes(24))
    for p in equipment:m.c.uc.mem_write(p,bytes(36))
    m.c.pointer(inv+8,entries);m.c.pointer(inv+12,entries+8);m.c.pointer(entries,cell);m.c.pointer(entries+4,other);m.c.pointer(cell,m.item)
    m.c.pointer(inv+20,sets);m.c.pointer(sets,equipment[0]);m.c.pointer(sets+12,equipment[1]);m.c.uc.mem_write(inv+0x2e,bytes([selected]));m.c.pointer(inv+0x24,m.item if potion else 0)
    m.c.pointer(m.metadata+104,target&0xffffffff)
    for j in range(2):
     slot=(1 if target<0 else target) if mask&(1<<j) else -1
     m.c.uc.mem_write(cell+4+j,struct.pack('b',slot))
     if slot!=-1:m.c.pointer(equipment[j]+4*slot,cell)
    m.destroyed=[];m.active=True
    try:m.c.invoke(0x3fe448,[inv,0],budget=100000)
    finally:m.active=False
    pointers=[[m.word(p+4*k) for k in range(9)] for p in equipment]
    cases.append(dict(target=target,selected=selected,mask=mask,potion=potion,
      remaining=[int(v==cell) for group in pointers for v in group],
      selected_after=m.c.uc.mem_read(inv+0x2e,1)[0],potion_after=m.word(inv+0x24),
      count=(m.word(inv+12)-m.word(inv+8))//4,shifted=m.word(entries)==other,destroyed=m.destroyed==[m.item]))
out=ROOT/'port/engine-ui/reference/character-menu-native-v1/inventory-remove-gold-v1.json'
out.write_text(json.dumps(dict(boundary_fixtures=['metadata lookup','item destructor','allocator/free'],cases=cases),indent=2)+'\n')
(out.parent/'inventory-remove-gold-v1.bin').write_bytes(struct.pack('<I',len(cases))+b''.join(struct.pack('<iiiiiiii',c['target'],c['selected'],c['mask'],c['potion'],int(any(c['remaining'][:9])),int(any(c['remaining'][9:])),c['selected_after'],c['count']) for c in cases))
assert all(c['count']==1 and c['shifted'] and c['destroyed'] and c['selected']==c['selected_after'] and c['potion_after']==0 for c in cases)
print(json.dumps(dict(validation='PASS',original_remove_cases=len(cases))))
m.transferring=True;m.split=m.alloc(128);inventory_vt=m.alloc(64);m.c.pointer(inventory_vt+32,0x3f9d04);recipient=m.alloc(128);transfers=[]
for available in (0,1,2,7):
 for requested in (-1,0,1,2,8):
  for mask in range(4):
   for potion in (0,1):
    m.c.uc.mem_write(inv,bytes(128));m.c.pointer(inv,inventory_vt);m.c.pointer(inv+8,entries);m.c.pointer(inv+12,entries+8);m.c.pointer(entries,cell);m.c.pointer(entries+4,other);m.c.pointer(cell,m.item)
    m.c.uc.mem_write(cell+4,bytes([1 if mask&1 else 255,2 if mask&2 else 255]));m.c.uc.mem_write(m.item+0x50,struct.pack('<h',available));m.c.pointer(inv+0x24,m.item if potion else 0)
    m.effects=[];m.active=True
    try:m.c.invoke(0x3ff858,[inv,0,recipient,requested,0,0],budget=100000)
    finally:m.active=False
    transfers.append(dict(available=available,requested=requested,mask=mask,potion=potion,result=m.c.reg(0),effects=m.effects,count=(m.word(inv+12)-m.word(inv+8))//4,potion_after=int(m.word(inv+0x24)==m.item)))
out=ROOT/'port/engine-ui/reference/character-menu-native-v1/inventory-transfer-coordinator-gold-v1.json'
out.write_text(json.dumps(dict(boundary_fixtures=['metadata','SplitItem','virtual UnEquipSlot','recipient AddItemInstance','allocator/free'],cases=transfers),indent=2)+'\n')
assert len(transfers)==160
for c in transfers:
 if c['requested']<=0:assert c['result']==0xffffffff and not c['effects'] and c['count']==2
 elif c['available']==0:assert c['result']==0 and not c['effects'] and c['count']==2
 elif c['available']>c['requested']:assert c['effects'][0][0]=='split' and c['count']==2 and c['potion_after']==c['potion']
 else:assert c['count']==1 and c['potion_after']==0 and c['effects'][-1][0]=='add'
print(json.dumps(dict(validation='PASS',original_transfer_coordinator_cases=len(transfers),native_split_unequip_recipient_add_unproved_here=True)))
m.transferring=False;m.constructing=True;m.c.uc.mem_write(inv,b'\xaa'*128);m.active=True
try:m.c.invoke(0x3ff200,[inv],budget=100000)
finally:m.active=False
ctor=dict(character=m.word(inv+4),items=[m.word(inv+k) for k in (8,12,16)],potion=m.word(inv+36),gold=m.word(inv+32),limit=m.word(inv+40),capacity=struct.unpack('b',m.c.uc.mem_read(inv+44,1))[0],flags=[m.c.uc.mem_read(inv+k,1)[0] for k in (45,46,47)],equipment=[])
for j in range(2):
 vector=m.word(inv+20)+12*j;begin=m.word(vector);end=m.word(vector+4);ctor['equipment'].append([m.word(begin+k*4) for k in range((end-begin)//4)])
assert ctor==dict(character=0,items=[0,0,0],potion=0,gold=0,limit=2147483647,capacity=-1,flags=[0,0,0],equipment=[[0]*9,[0]*9]),ctor
(out.parent/'inventory-default-constructor-gold-v1.json').write_text(json.dumps(dict(boundary_fixtures=['equipment vector allocation'],fields=ctor),indent=2)+'\n')
print(json.dumps(dict(validation='PASS',original_default_constructor=True,fields=ctor)))
