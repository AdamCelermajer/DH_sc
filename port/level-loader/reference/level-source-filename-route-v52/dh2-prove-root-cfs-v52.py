from pathlib import Path
import sys,json,struct,hashlib
sys.path.insert(0,'C:/Users/adamc/AppData/Local/uv/cache/archive-v0/jc8RoeQ1US_9Gkmi/Lib/site-packages');sys.path.insert(0,'C:/Users/adamc/Desktop/workspace/DH_sc/port/game-data/tests')
b=Path('C:/Users/adamc/.codex/worktrees/generic-level-loader/DH_sc/port/level-loader');sys.path.insert(0,str(b/'tests'));from xml_original_probe import Parser
r=b/'reference/level-source-filename-route-v52';elf=Path('C:/Users/adamc/Desktop/workspace/DH_sc/.local-inputs/libDungeonHunter2.so')
class Oracle(Parser):
 def external(self,uc,a,n,u):
  name=self.imports.get(a)
  if name=='strstr':v,k=self.text(self.reg(0)),self.text(self.reg(1));p=v.find(k);self.returned(self.reg(0)+p if p>=0 else 0)
  elif name=='fopen':self.opened.append(self.text(self.reg(0)));self.returned(0)
  else:super().external(uc,a,n,u)
 def store(self,p,value):
  raw=value.encode();h=self.heap;self.heap+=(len(raw)+32)&~15;self.uc.mem_write(h,raw+b'\0');self.pointer(p+20,h);self.pointer(p+16,h+len(raw));self.pointer(p+12,h+len(raw)+1)
o=Oracle(elf,{'functions':[]});from unicorn import UC_HOOK_CODE;o.uc.hook_add(UC_HOOK_CODE,o.allocation)
got=0x56dc50+8+o.word(0x56df28);mapping=o.word(got+o.word(0x56df2c));assert mapping==0x9f66e4
node=o.data+0x60000;inp=o.data+0x70000;mode=inp+0x2000;out=mode+0x100;o.uc.mem_write(mode,b'rb\0')
cases=[]
def projected(source,working,entry):
 filename=source;applied=False
 if entry:
  key=source[2:] if source.startswith(('./','.\\')) else source
  if working and working in key:key=key[len(working)+(0 if working[-1] in '/\\' else 1):]
  key=key.replace('\\','/')
  if key==entry[0]:filename=entry[1];applied=True
 if ':' not in filename and working:filename=working+('' if working[-1] in '/\\' else '/')+filename
 return filename,applied
for working in ['', 'data','data/','ROOT\\','/sdcard/']:
 for source in ['data/scene/001_swamp.mlx','./data/scene/001_swamp.mlx','q:/DATA/scene/file.mlx','Dir\\file.mlx','WORLD/file.mlx']:
  for entry in [None,('scene/001_swamp.mlx','data/scene/MAPPED.mlx'),('data/scene/001_swamp.mlx','C:/RESOURCE/mapped.mlx')]:
   o.uc.mem_write(mapping,bytes(20));o.pointer(mapping+8,mapping);o.pointer(mapping+12,mapping)
   if entry:
    o.uc.mem_write(node,bytes(64));o.pointer(node,1);o.pointer(node+4,mapping);o.store(node+16,entry[0]);o.store(node+40,entry[1]);o.pointer(mapping+4,node);o.pointer(mapping+8,node);o.pointer(mapping+12,node);o.pointer(mapping+16,1)
   o.uc.mem_write(0x99b138,working.encode()+b'\0');o.uc.mem_write(inp,source.encode()+b'\0');o.uc.mem_write(out,bytes(8));o.opened=[];o.invoke(0x56dc40,[out,inp,mode],budget=2000000)
   want,applied=projected(source,working,entry);assert o.opened==[want],(source,working,entry,o.opened,want)
   cases.append({'input':source,'working_directory':working,'map_entry':entry,'physical_fopen_filename':want,'map_applied':applied})
report={'validation':'PASS','original_sha256':hashlib.sha256(elf.read_bytes()).hexdigest(),'source_cfs_open56dc40_and_real_RB_tree_find56db48_executed':True,'comparisons':len(cases),'map_provenance':'explicit source-layout initialized one-node map fixtures; no production mappings inferred','physical_open':'original fopen argument captured; explicit fopen returns missing, no OS I/O','cases':cases}
(r/'original-root-cfs-open-v52.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps({k:v for k,v in report.items() if k!='cases'},indent=2))
