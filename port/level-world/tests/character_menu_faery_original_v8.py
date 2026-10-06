"""Execute original ChangeFaery ARM body with correctly identified endpoints.
Save current/count accesses are actual instructions. Difficulty, UpdateAllSkills,
model selection/visual/animator are declared boundary fixtures.
"""
import sys,json
from pathlib import Path
ROOT=Path(__file__).resolve().parents[3]
sys.path.insert(0,r'C:\Users\adamc\AppData\Local\uv\cache\archive-v0\jc8RoeQ1US_9Gkmi\Lib\site-packages')
sys.path.insert(0,str(ROOT/'port/engine-ui/tests'))
from character_menu_native_v1_original import Original
class Faery(Original):
 def hook(self,uc,a,size,u):
  if self.active:
   if a in (0x3bb9d8,0x3bba20):return
   if a==0x3bb8e4:self.trace.append(['difficulty']);self.ret(self.difficulty);return
   if a==0x3d8894:self.trace.append(['UpdateAllSkills',int(self.c.reg(0)==self.character+0x3c8)]);self.ret();return
   if a==0x3a54d4:self.trace.append(['GetCharModelName',int(self.c.reg(0)==self.faery)]);self.ret(self.model);return
   if a==0x394d34:self.trace.append(['SetVisualObject',int(self.c.reg(0)==self.faery),int(self.c.reg(1)==self.model),self.c.reg(2),self.c.reg(3)]);self.ret();return
   if a==0x3c99a0:self.trace.append(['ANIM_AddSetToRenderObject',int(self.c.reg(0)==self.faery+0x49c)]);self.ret();return
  super().hook(uc,a,size,u)
m=Faery();saved=m.alloc(512);m.faery=m.alloc(0x600);m.model=m.alloc(64)
m.c.uc.mem_write(m.model,b'DECLARED_MODEL_ENDPOINT_FIXTURE\0');m.c.pointer(m.character+0x14e8,saved);m.stat_entry=0x3ae99c;cases=[]
for tier in range(3):
 for index in range(5):
  for present in (False,True):
   for j in range(3):m.c.pointer(saved+0xa0+j*4,5);m.c.pointer(saved+0xac+j*4,0)
   m.difficulty=tier;m.c.pointer(m.character+0x420,m.faery if present else 0);m.trace=[];m.active=True
   try:m.c.invoke(m.stat_entry,[m.character,index],budget=100000)
   finally:m.active=False
   current=[m.word(saved+0xac+j*4) for j in range(3)];expected=[0]*3;expected[tier]=index;assert current==expected
   expected_trace=[['difficulty'],['UpdateAllSkills',1]]+([['GetCharModelName',1],['SetVisualObject',1,1,0,1],['ANIM_AddSetToRenderObject',1]] if present else [])
   assert m.trace==expected_trace,(m.trace,expected_trace)
   cases.append(dict(tier=tier,index=index,faery_present=present,selected=current,ordered_calls=m.trace))
out=ROOT/'port/level-world/reference/character-menu-faery-connection-v8'
(out/'original-coordinator-gold-v8.json').write_text(json.dumps(dict(validation='PASS',cases=cases,boundaries=['fresh difficulty','UpdateAllSkills','GetCharModelName','SetVisualObject','ANIM_AddSetToRenderObject'],same_original_save_stores=True),indent=2))
print(json.dumps(dict(validation='PASS',original_ARM_cases=len(cases),model_endpoint_NULL_xref_forceTrue=True,faery_NULL_source_early_exit=True)))
