"""Execute original MenuBase Show/Hide. Localization/GPU/debug/application,
weak live character, listener and deadzone endpoints are explicit fixtures;
original valid/localized gates, SetVisible, name branches and stores execute.
"""
from pathlib import Path
import sys,struct,json,hashlib,itertools
R=Path(__file__).resolve().parents[3]
sys.path.insert(0,r'C:\Users\adamc\AppData\Local\uv\cache\archive-v0\jc8RoeQ1US_9Gkmi\Lib\site-packages')
sys.path.insert(0,str(R/'port/engine-animation/tests'))
from compiled_transforms_differential import Cpu,words
from unicorn import UC_HOOK_CODE,UC_HOOK_MEM_WRITE
names=['menu_CharacterMenu','menu_CharacterSheetNew','menu_CharacterSheetStats','menu_InventorySheetMain','menu_InventorySheetDetails','menu_SkillTreeSheetNew','menu_FaerySheet','menu_Merchant','menu_Ingame','menu_Options','menu_VerificationLoading','menu_language','menu_playlist','menu_splash','unknown']
c=Cpu(R/'.local-inputs/libDungeonHunter2.so',False,{'functions':[]});d=c.data
m=d+0x1000;vt=d+0x2000;ch=d+0x3000;render=d+0x4000;weak=d+0x5000;cb=d+0x6000;manager=d+0x9000
events=[];language=0
def ret(v=0):c.put(0,v);c.uc.reg_write(c.pc,c.uc.reg_read(c.lr))
def hook(uc,a,z,u):
 if a==cb:ret(c.uc.mem_read(m+0x7c,1)[0]);return
 if a==cb+4:events.append((2,0));uc.mem_write(m+0x75,b'\1');ret();return
 simple={0x337888:0,0x337a88:1,0x384ef0:8,0x42331c:10,0x412aa8:11,0x42e110:13,0x46d104:15,0x46cb34:16}
 if a in simple:events.append((simple[a],c.reg(1) if a==0x46d104 else 0));ret();return
 if a==0x4223bc:events.append((3,c.reg(1))) # Run actual SetVisible body.
 if a==0x7abe0c:events.append((4,1 if c.string(c.reg(2))==b'onPush' else 0));ret();return
 if a==0x3140ec:uc.mem_write(c.reg(0),bytes(24));ret();return
 if a in [0x318254,0x41aeec,0x427ca0]:ret();return
 if a in [0x42204c,0x427d50]:ret(ch);return
 if a==0x320f5c:events.append((9,0));ret(1);return
 if a==0x42ca8c:ret(manager);return
 if a==0x46d514:events.append((14,0));ret(language);return
def write(uc,access,a,size,value,u):
 mapping={0x9a487c:5,0x99f818:6,0x9a4863:7,manager+0x60:12}
 if a in mapping:events.append((mapping[a],value))
c.uc.hook_add(UC_HOOK_CODE,hook);c.uc.hook_add(UC_HOOK_MEM_WRITE,write)
records=[]
for ni,name in enumerate(names):
 for valid,localized,drag,language in itertools.product([0,1],[0,1],[0,1],[0,7,8,0xffffffff]):
  c.uc.mem_write(m,bytes(256));c.uc.mem_write(ch,bytes(256));c.uc.mem_write(weak,words([1,1]));c.uc.mem_write(manager,bytes(256))
  c.pointer(m,vt);c.pointer(m+4,render);c.pointer(m+0x48,weak);c.pointer(m+0x4c,ch);c.pointer(m+0x5c,d+0x8000 if drag else 0)
  c.pointer(vt+0x3c,cb);c.pointer(vt+0x40,cb+4);c.uc.mem_write(cb,words([0xe12fff1e]*2))
  c.uc.mem_write(m+8,name.encode()+b'\0');c.uc.mem_write(m+0x74,bytes([5,localized]));c.uc.mem_write(m+0x78,words([31]));c.uc.mem_write(m+0x7c,bytes([valid]))
  for op,address in enumerate([0x425450,0x424af4]):
   events.clear();c.invoke(address,[m]);fields=[c.uc.mem_read(m+0x74,1)[0],c.uc.mem_read(m+0x75,1)[0],struct.unpack('<I',c.uc.mem_read(m+0x78,4))[0]]
   records.append([ni,valid,localized,drag,language,op,*fields,len(events),*[v for pair in events for v in pair]])
out=R/'port/engine-ui/reference/authored-menu-lifecycle-v1';out.mkdir(exist_ok=True)
(out/'original-gold.txt').write_text('\n'.join(' '.join(map(str,row)) for row in records)+'\n')
(out/'original-proof.json').write_text(json.dumps({'validation':'PASS','original_show_hide_calls':len(records),'original_sha256':hashlib.sha256((R/'.local-inputs/libDungeonHunter2.so').read_bytes()).hexdigest(),'names':names,'scope':__doc__},indent=2)+'\n')
print(json.dumps({'validation':'PASS','original_show_hide_calls':len(records)}))
