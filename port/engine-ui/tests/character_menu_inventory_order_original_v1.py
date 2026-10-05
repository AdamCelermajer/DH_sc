"""Original slot and comparator bodies; raw item/table views are boundary fixtures."""
import json,struct,sys
from pathlib import Path
sys.path.insert(0,str(Path(__file__).resolve().parent))
from character_menu_native_v1_original import Original,ROOT
class Order(Original):
 def hook(self,uc,a,size,u):
  if self.active and getattr(self,'equipment_order',False):
   if a==0x3fa330:self.ret(self.available[self.c.reg(0)]);return
   if a==0x3ffe3c:self.ret(self.equipped.get(self.c.reg(1),0));return
   if a==0x4c4bdc:
    group=self.cstr(self.c.reg(1));key=self.cstr(self.c.reg(2));assert group=='EquipmentSlots'
    self.ret(dict(LeftHand=1,RightHand=2,LeftHandRingFinger=5,RightHandRingFinger=6)[key]);return
  if self.active and a==0x3f9e08:self.ret(self.metadata[self.c.reg(0)]);return
  if self.active and a==0x3f9e80:self.ret(self.power_counts[self.c.reg(0)]);return
  super().hook(uc,a,size,u)
m=Order();m.metadata={};m.power_counts={};inventory=m.alloc(0x80);items=m.alloc(4);cell=m.alloc(12);slots=m.alloc(8);equipment=m.alloc(36)
m.c.pointer(inventory+4,m.character);m.c.pointer(inventory+8,items);m.c.pointer(inventory+12,items+4);m.c.pointer(inventory+20,slots);m.c.pointer(slots,equipment);m.c.pointer(slots+4,equipment+36);m.c.pointer(items,cell);m.c.pointer(cell,m.item)
row=m.alloc(164);m.metadata[m.item]=row;m.power_counts[m.item]=0
cases=[];m.stat_entry=0x3fdcbc
for category in (0,4,5):
 for target in range(-5,10):
  for dual in (0,256):
   m.c.pointer(row+22*4,category);m.c.pointer(row+26*4,target&0xffffffff);m.c.pointer(m.character+0x1320,dual)
   for slot in range(9):
    m.active=True
    try:m.c.invoke(m.stat_entry,[inventory,0,slot]);result=m.c.reg(0)
    finally:m.active=False
    assert result in (0,1);cases.append((category,target,dual,slot,result))
out=ROOT/'port/engine-ui/reference/character-menu-native-v1/inventory-slot-gold-v1.bin'
out.write_bytes(struct.pack('<I',len(cases))+b''.join(struct.pack('<iiiii',*r) for r in cases))
print(json.dumps(dict(validation='PASS',original_slot_cases=len(cases),metadata_boundary_fixtures=True)))
other=m.alloc(0x100);other_row=m.alloc(164);entry_a=m.alloc(12);entry_b=m.alloc(12);context=m.alloc(4)
m.c.pointer(entry_a,m.item);m.c.pointer(entry_b,other);m.c.pointer(context,m.character);m.metadata[other]=other_row
strings=[]
for text in (b'Alpha',b'Zulu'):
 p=m.alloc(32);m.c.uc.mem_write(p,text+b'\0');strings.append(p)
for j in range(8,17):
 m.c.uc.mem_write(row+j*4,struct.pack('<f',float(j-6)))
 m.c.uc.mem_write(other_row+j*4,struct.pack('<f',float(17-j)))
sort_cases=[];m.stat_entry=0x3fd1e8
for actor in (0,263,264,265,290,291,292,325,326,327):
 m.c.uc.mem_write(m.character+0x13c8,struct.pack('<h',actor))
 for av,bv in ((25,30),(30,25),(0,0),(-25,30),(30,-25)):
  for ap,bp in ((0,0),(1,0),(0,1)):
   m.c.pointer(m.item+0x54,av&0xffffffff);m.c.pointer(other+0x54,bv&0xffffffff);m.power_counts[m.item]=ap;m.power_counts[other]=bp
   for an,bn in ((0,1),(1,0),(0,0)):
    m.c.pointer(m.item+0x1c,strings[an]);m.c.pointer(other+0x1c,strings[bn]);m.active=True
    try:m.c.invoke(m.stat_entry,[context,entry_a,entry_b]);result=m.c.reg(0)
    finally:m.active=False
    assert result in (0,1);sort_cases.append((actor,av,bv,ap,bp,an,bn,result))
out=ROOT/'port/engine-ui/reference/character-menu-native-v1/inventory-order-gold-v1.bin'
out.write_bytes(struct.pack('<I',len(sort_cases))+b''.join(struct.pack('<iiiiiiii',*r) for r in sort_cases))
print(json.dumps(dict(validation='PASS',original_value_comparator_cases=len(sort_cases),metadata_and_name_boundary_fixtures=True)))
m.equipment_order=True;equipment_context=m.alloc(8);m.c.pointer(equipment_context,m.character);m.stat_entry=0x3fd5d0
m.c.uc.mem_write(m.character+0x13c8,struct.pack('<h',0));m.c.pointer(m.item+0x54,0);m.c.pointer(other+0x54,0);m.power_counts[m.item]=m.power_counts[other]=0
equipment_cases=[]
for slot in (0,1,2,5,6):
 m.c.pointer(equipment_context+4,slot)
 pair=(1,2) if slot in (1,2) else (5,6) if slot in (5,6) else (slot,slot)
 for available_a in (0,1):
  for available_b in (0,1):
   m.available={m.item:available_a,other:available_b}
   for first in (0,m.item,other):
    for second in (0,m.item,other):
     m.equipped={pair[0]:first,pair[1]:second}
     for an,bn in ((0,1),(1,0)):
      m.c.pointer(m.item+0x1c,strings[an]);m.c.pointer(other+0x1c,strings[bn]);m.active=True
      try:m.c.invoke(m.stat_entry,[equipment_context,entry_a,entry_b]);result=m.c.reg(0)
      finally:m.active=False
      assert result in (0,1)
      equipment_cases.append((available_a,available_b,int(m.item in m.equipped.values()),int(other in m.equipped.values()),int(m.equipped.get(slot)==m.item),an,bn,result))
out=ROOT/'port/engine-ui/reference/character-menu-native-v1/inventory-equipment-order-gold-v1.bin'
out.write_bytes(struct.pack('<I',len(equipment_cases))+b''.join(struct.pack('<iiiiiiii',*r) for r in equipment_cases))
print(json.dumps(dict(validation='PASS',original_equipment_comparator_cases=len(equipment_cases),availability_equipped_ID_and_design_boundary_fixtures=True)))
