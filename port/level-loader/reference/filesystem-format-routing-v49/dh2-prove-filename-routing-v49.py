from pathlib import Path
import sys,struct,json,hashlib,random
sys.path.insert(0,'C:/Users/adamc/AppData/Local/uv/cache/archive-v0/jc8RoeQ1US_9Gkmi/Lib/site-packages')
sys.path.insert(0,'C:/Users/adamc/Desktop/workspace/DH_sc/port/game-data/tests')
from items_differential import Original
outdir=Path('C:/Users/adamc/.codex/worktrees/generic-level-loader/DH_sc/port/level-loader/reference/filesystem-format-routing-v49')
elf=Path('C:/Users/adamc/Desktop/workspace/DH_sc/.local-inputs/libDungeonHunter2.so')
class Oracle(Original):
 def txt(self,p):
  b=bytearray()
  while self.uc.mem_read(p+len(b),1)!=b'\0':b.extend(self.uc.mem_read(p+len(b),1))
  return bytes(b)
 def ss(self,p):return bytes(self.uc.mem_read(self.word(p+20),self.word(p+16)-self.word(p+20)))
 def store(self,p,b):
  h=self.heap;self.heap+=(len(b)+32)&~15;self.uc.mem_write(h,b+b'\0');self.pointer(p+20,h);self.pointer(p+16,h+len(b));self.pointer(p+12,h+len(b)+1)
 def storage(self,uc,a,size,unused):
  if a==self.callback+112:self.returned(self.working)
  elif a==0x3209a8:self.store(self.reg(0),b'');self.returned()
  elif a in (0x320b88,0x325ff4):self.store(self.reg(0),bytes(uc.mem_read(self.reg(1),self.reg(2)-self.reg(1))));self.returned()
  elif a==0x320a4c:self.store(self.reg(0),self.ss(self.reg(0))+bytes(uc.mem_read(self.reg(1),self.reg(2)-self.reg(1))));self.returned()
  elif a==0x34e324:self.store(self.reg(0),self.ss(self.reg(1))+self.ss(self.reg(2)));self.returned()
  elif a==0x56c4c4:
   src=self.ss(self.reg(1));start=self.reg(2);count=self.reg(3);assert start<=len(src)
   self.store(self.reg(0),src[start:start+count]);self.returned()
  elif a==0x56c740:self.basename_calls+=1 # execute original basename body
  elif a==0x34e090:self.lower_calls+=1 # execute original ASCII lowercase body
  elif a==0x310450:self.returned()
  else:super().storage(uc,a,size,unused)
 def external(self,uc,a,size,u):
  if a==self.callback+112:self.returned(self.working)
  elif self.imports.get(a)=='strlen':self.returned(len(self.txt(self.reg(0))))
  elif self.imports.get(a)=='strstr':
   x,y=self.txt(self.reg(0)),self.txt(self.reg(1));p=x.find(y);self.returned(self.reg(0)+p if p>=0 else 0)
  else:super().external(uc,a,size,u)
def projected(work,value):
 pos=value.find(work);head=b''
 if pos>=0:
  cut=pos+len(work)+1;assert cut<=len(value);head=value[:cut];value=value[cut:]
 relocated=b'.tga' in value
 if relocated:value=b'ata/3d/textures/'+value[max(value.rfind(b'/'),value.rfind(b'\\'))+1:]
 value=bytes(c+32 if 65<=c<=90 else c for c in value)
 return head+value,int(relocated)
o=Oracle(elf,{'functions':[]});owner=o.data+0x7000;vt=owner+0x100;working=owner+0x200;inp=owner+0x600;out=owner+0x1800;o.working=working;o.pointer(owner,vt);o.pointer(vt+44,o.callback+112)
cases=[]
values=[b'data/3d/textures/env_crypt_alpha.tga',b'q:/data/iphone/3d/textures/env_crypt_alpha.tga',b'Q:/data/iphone/3D/Textures/UPPER.TGA',b'data/3D/Models/a.bdae',b'other/alpha.tga.bak',b'dir.tga/SomeName.bin',b'dir.tga/',b'path\\TEXTURE.tga',b'path/KEEP.TGA',b'pvr2_env_castle_alpha.tga',b'data/3d/textures/pvr2_env_castle_alpha.tga',b'NoSlash.BIN',b'letter\xe9\x80.TGA',b'data/config.xml',b'data/shader.glsl',b'data/audio/TEST.mp3']
for work in [b'',b'.',b'data',b'q:/data/iphone',b'/sdcard/gameloft/games/GloftD2SS',b'/',b'UNMATCHED']:
 for value in values:
  pos=value.find(work)
  if pos>=0 and pos+len(work)+1>len(value):continue
  cases.append((work,value))
rng=random.Random(49)
for _ in range(120):
 work=rng.choice([b'',b'.',b'data',b'Q:/data/iphone',b'UNMATCHED'])
 value=rng.choice([b'data/',b'q:/data/iphone/',b'prefixDATA/',b'dir\\',b'/'])+bytes(rng.choice(b'AbCZ_012-') for _ in range(rng.randint(1,40)))+rng.choice([b'.tga',b'.TGA',b'.bdae',b'.tga.tmp',b'.png'])
 cases.append((work,value))
rows=[]
for work,value in cases:
 o.uc.mem_write(working,work+b'\0');o.uc.mem_write(inp,value+b'\0');o.uc.mem_write(out,bytes(32));o.basename_calls=o.lower_calls=0;o.invoke(0x34e96c,[out,owner,inp]);actual=o.ss(out);want,basename=projected(work,value)
 assert actual==want and o.basename_calls==basename and o.lower_calls==1,(work,value,actual,want,o.basename_calls)
 rows.append({'working_directory_hex':work.hex(),'input_hex':value.hex(),'output_hex':actual.hex(),'basename_calls':o.basename_calls,'source_lower_body_calls':o.lower_calls})
report={'validation':'PASS','comparisons':len(rows),'original_sha256':hashlib.sha256(elf.read_bytes()).hexdigest(),'exact_relocation_literal_hex':b'ata/3d/textures/'.hex(),'executed_original':['ApplyFilenameHacks34e96c','getFileBasename56c740','rfind56c640','ToLowerCase34e090'],'explicit_endpoints':['std string allocation/range/append/substring/concatenation','strlen/strstr','filesystem working-directory virtual'],'format_prefix_or_GPU_policy_claimed':False,'cases':rows}
(outdir/'filename-source-oracle-v49.json').write_text(json.dumps(report,indent=2)+'\n')
print(json.dumps({k:v for k,v in report.items() if k!='cases'},indent=2))
