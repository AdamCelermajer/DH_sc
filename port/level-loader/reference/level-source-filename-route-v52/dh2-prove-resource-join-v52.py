from pathlib import Path
import sys,json
sys.path.insert(0,'C:/Users/adamc/AppData/Local/uv/cache/archive-v0/jc8RoeQ1US_9Gkmi/Lib/site-packages');sys.path.insert(0,'C:/Users/adamc/Desktop/workspace/DH_sc/port/game-data/tests')
b=Path('C:/Users/adamc/.codex/worktrees/generic-level-loader/DH_sc/port/level-loader');sys.path.insert(0,str(b/'tests'));from xml_original_probe import Parser;from unicorn import UC_HOOK_CODE
r=b/'reference/level-source-filename-route-v52';elf=Path('C:/Users/adamc/Desktop/workspace/DH_sc/.local-inputs/libDungeonHunter2.so')
class Oracle(Parser):
 def storage(self,uc,a,n,u):
  if a==0x56dc40:self.joined.append(self.text(self.reg(1)));p=self.reg(0);self.pointer(p,0);self.returned(p)
  else:super().storage(uc,a,n,u)
 def external(self,uc,a,n,u):
  name=self.imports.get(a)
  if name=='strstr':s,k=self.text(self.reg(0)),self.text(self.reg(1));pos=s.find(k);self.returned(self.reg(0)+pos if pos>=0 else 0)
  elif name in ['strcpy','strcat']:
   p=self.reg(0);s=(self.text(p) if name=='strcat' else '')+self.text(self.reg(1));uc.mem_write(p,s.encode()+b'\0');self.returned(p)
  elif name=='sprintf':
   p=self.reg(0);assert self.text(self.reg(1))=='%s%s';s=self.text(self.reg(2))+self.text(self.reg(3));uc.mem_write(p,s.encode()+b'\0');self.returned(len(s))
  else:super().external(uc,a,n,u)
o=Oracle(elf,{'functions':[]});o.uc.hook_add(UC_HOOK_CODE,o.allocation);receiver=o.data+0x30000;root=o.data+0x31000;name=root+0x1000;resource=name+0x1000;app=resource+0x1000;o.pointer(0x994a98+0x37f4,app);rows=[]
for prefix in ['', 'data/','UPPER\\Root\\']:
 for fname in ['001_swamp.mlx','data/scene/UPPER.mlx','name.savegame','prefix.savegame.bak']:
  for lower in [False,True]:
   o.uc.mem_write(root,prefix.encode()+b'\0');o.uc.mem_write(name,fname.encode()+b'\0');o.uc.mem_write(resource,b'ACTUAL/RES/\0');o.pointer(0x99b108,resource);o.uc.mem_write(app+0xa8,bytes([int(lower)]));o.uc.mem_write(receiver,bytes(64));o.joined=[];o.invoke(0x34fc18,[receiver,root,name,0,0],budget=1000000)
   want=('ACTUAL/RES/' if '.savegame' in fname else prefix)+fname
   if lower:want=want.lower()
   assert o.joined==[want],(prefix,fname,lower,o.joined,want)
   rows.append({'root':prefix,'filename':fname,'RES_PATH':'ACTUAL/RES/','application_byteA8':int(lower),'source_CFS_open_argument':want})
(r/'original-resource-join-v52.json').write_text(json.dumps({'validation':'PASS','comparisons':len(rows),'original_FileHandleC1_34fc18_and_ToLowerCase34e414_executed':True,'endpoints':'strcpy/strcat/sprintf; CFS.open returns actual missing pointer; no file read/handle success','cases':rows},indent=2)+'\n');print('PASS original FileHandle C1 source joins',len(rows))
