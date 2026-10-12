from pathlib import Path
import sys,json,struct,hashlib
sys.path.insert(0,'C:/Users/adamc/AppData/Local/uv/cache/archive-v0/jc8RoeQ1US_9Gkmi/Lib/site-packages')
sys.path.insert(0,'C:/Users/adamc/Desktop/workspace/DH_sc/port/game-data/tests')
from items_differential import Original
r=Path('C:/Users/adamc/.codex/worktrees/generic-level-loader/DH_sc/port/level-loader/reference/production-filesystem-adoption-v50');elf=Path('C:/Users/adamc/Desktop/workspace/DH_sc/.local-inputs/libDungeonHunter2.so')
class Oracle(Original):
 def txt(self,p):
  b=bytearray()
  while self.uc.mem_read(p+len(b),1)!=b'\0':b.extend(self.uc.mem_read(p+len(b),1))
  return bytes(b)
 def external(self,uc,a,size,u):
  name=self.imports.get(a)
  if name=='chdir':self.chdir_calls.append(self.txt(self.reg(0)));self.returned(self.chdir_result)
  elif name=='strcpy':p=self.reg(0);uc.mem_write(p,self.txt(self.reg(1))+b'\0');self.returned(p)
  elif name=='sprintf':
   p=self.reg(0);fmt=self.txt(self.reg(1));assert fmt==b'%sdata/';s=self.txt(self.reg(2))+b'data/';uc.mem_write(p,s+b'\0');self.returned(len(s))
  elif name=='memset':p=self.reg(0);uc.mem_write(p,bytes([self.reg(1)&255])*self.reg(2));self.returned(p)
  else:super().external(uc,a,size,u)
o=Oracle(elf,{'functions':[]});working=0x99b138;res_path=0x99b108;receiver=o.data+0x1800;inp=o.data+0x3000
initial=bytes(o.uc.mem_read(working,1024));assert initial==bytes(1024)
ctors=[]
for root in [b'',b'data/',b'/sdcard/gameloft/games/GloftD2SS/',b'UPPER\\Path\\',b'x'*250]:
 o.uc.mem_write(inp,root+b'\0');o.pointer(res_path,inp);o.uc.mem_write(receiver,b'\x55'*1344);before=bytes(o.uc.mem_read(working,1024));o.invoke(0x34f6f4,[receiver]);assert bytes(o.uc.mem_read(working,1024))==before
 buffers={str(off):bytes(o.uc.mem_read(receiver+off,260)) for off in [0x2c,0x130,0x234,0x338,0x43c]}
 for off,value in [(0x2c,root),(0x130,root),(0x234,b''),(0x338,root+b'data/'),(0x43c,root)]:assert buffers[str(off)]==value+b'\0'*(260-len(value))
 assert bytes(o.uc.mem_read(receiver+4,40))==bytes(40)
 ctors.append({'RES_PATH_hex':root.hex(),'buffers_hex':{k:v.hex() for k,v in buffers.items()},'shared_working_directory_unchanged':True})
setters=[];wanted=bytearray(initial)
for address in [0x56c528,0x34e1d4]:
 for status,path in [(-1,b'/missing/source'),(0,b'FIRST/Dir'),(0,b''),(1,b'not-success'),(0,b'/actual/resource/path/'),(-2,b'/notzero'),(0,b'x'*1023),(0,b'short')]:
  o.chdir_result=status;o.chdir_calls=[];o.uc.mem_write(inp,path+b'\0');got=o.invoke(address,[receiver,inp]);changed=status==0
  if changed:wanted[:len(path)+1]=path+b'\0'
  actual=bytes(o.uc.mem_read(working,1024));assert got==int(changed) and actual==wanted and o.chdir_calls==[path]
  assert o.invoke(0x56c214,[receiver])==working
  setters.append({'address':hex(address),'host_chdir_return':status,'path_hex':path.hex(),'source_result':got,'working_buffer_hex':actual.hex()})
report={'validation':'PASS','original_sha256':hashlib.sha256(elf.read_bytes()).hexdigest(),'global_working_directory_source_initial_bytes':1024,'global_working_directory_all_zero':True,'constructor_cases':ctors,'setter_cases':setters,'source_executed':['FileSystemWin32C1_34f6f4','FileSystemBaseC1_34e538','CFileSystemC1_56c058','_getPaths34f068','changeWorkingDirectory56c528','changeWorkingDirectory34e1d4','getWorkingDirectory56c214'],'explicit_host_endpoints':['chdir result fixture (no host operation performed)','strcpy/sprintf/memset'],'production_mount_or_current_directory_claimed':False}
(r/'original-fs-init-oracle-v50.json').write_text(json.dumps(report,indent=2)+'\n')
print(json.dumps({k:v for k,v in report.items() if k not in ['constructor_cases','setter_cases']},indent=2));print('ctor_cases',len(ctors),'setter_cases',len(setters))
