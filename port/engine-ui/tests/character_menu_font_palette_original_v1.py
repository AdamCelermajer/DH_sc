"""Original GetFontDef/GetColor over actual fonts cache, explicit design IDs."""
import json,struct,sys
from pathlib import Path
sys.path.insert(0,str(Path(__file__).resolve().parent))
from character_menu_native_v1_original import Original,ROOT
class Palette(Original):
 def hook(self,uc,a,size,u):
  if self.active and a==0x4c4bdc:
   group=self.cstr(self.c.reg(1));key=self.cstr(self.c.reg(2))
   assert group=='ItemPowerColor' and key in ('zero','one','two','three','four')
   self.keys.append(key);self.ret(self.design_id);return
  super().hook(uc,a,size,u)
m=Palette();raw=(ROOT/'.local-inputs/character-menu-native-v1-fonts/fonts_pyarray.bin').read_bytes()
n=struct.unpack_from('<I',raw)[0];assert len(raw)==4+n*8
rows=m.alloc(n*12)
for j in range(n):
 glow,text=struct.unpack_from('<II',raw,4+j*8)
 m.c.uc.mem_write(rows+j*12,struct.pack('<III',0,glow,text))
m.c.pointer(m.c.symbols['_ZN6Arrays11FontPalette7membersE'],rows)
cases=[]
for powers in range(8):
 for design in range(n):
  m.design_id=design;m.keys=[];m.active=True
  try:
   m.c.invoke(m.entries['_ZN12ItemInstance8GetColorEi'],[powers]);color=m.c.reg(0)
  finally:m.active=False
  source_row=design if powers<=4 else 2
  assert color==struct.unpack_from('<I',raw,8+source_row*8)[0]
  assert m.keys==[('zero','one','two','three','four')[powers]] if powers<=4 else not m.keys
  cases.append(dict(powers=powers,design_id=design,font_row=source_row,color=color))
(ROOT/'port/engine-ui/reference/character-menu-native-v1/font-palette-gold-v1.json').write_text(json.dumps(cases,indent=2)+'\n')
print(json.dumps(dict(validation='PASS',original_cases=len(cases),actual_font_rows=n,constant_ID_boundary_fixture=True)))
