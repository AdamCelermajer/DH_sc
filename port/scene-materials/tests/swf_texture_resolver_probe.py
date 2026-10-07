"""Actual FileSystemBase filename hacks, with original global cwd/getter."""
import hashlib,json,sys,struct
from pathlib import Path
ROOT=Path(__file__).resolve().parents[1];REPO=ROOT.parents[1]
sys.path.insert(0,str(ROOT/'tests'))
from shader_sources_differential import Cpu as BaseCpu,sha
class Cpu(BaseCpu):
 def external(self,uc,address,size,user):
  if self.imports.get(address)=='strstr':
   at=self.string(self.reg(0)).find(self.string(self.reg(1)));self.put(0,self.reg(0)+at if at>=0 else 0);uc.reg_write(self.pc,uc.reg_read(self.lr));return
  return super().external(uc,address,size,user)
def main():
 mp=REPO/'.local-inputs/swf-render-connection-discovery/hacks/original-functions.json';m=json.loads(mp.read_text());engine=REPO/'.local-inputs/libDungeonHunter2.so';assert sha(engine)==m['original_sha256'];cpu=Cpu(engine,False,m);cpu.pointer(0x99f698,1);base=cpu.data+0x1000;obj=base;vt=base+0x100;input=base+0x300;out=base+0x1000;cpu.pointer(obj,vt);cpu.pointer(vt+0x2c,0x56c214);rows=[]
 cases=[('',n)for n in ['data/menus/MenusGraphics_droid.tga','data/menus/map_top.tga','data/menus/map_bottom.tga','data/3d/textures/atlas001.tga','data/menus/Foo.TGA','menus/MenusGraphics_droid.tga','data/menus/x.tga.bdae','data/menus/x.bdae']]+[('/cache','/cache/data/menus/MenusGraphics_droid.tga'),('/cache','data/menus/MenusGraphics_droid.tga')]
 for cwd,name in cases:
  cpu.uc.mem_write(0x99b138,cwd.encode()+b'\0');cpu.uc.mem_write(input,name.encode()+b'\0');cpu.invoke(0x34e96c,[out,obj,input]);ptr=struct.unpack('<I',cpu.uc.mem_read(out+20,4))[0];rows.append({'working_directory':cwd,'request':name,'resolved':cpu.string(ptr).decode()})
 report={'validation':'PASS','original_sha256':sha(engine),'manifest_sha256':sha(mp),'original_instructions_executed':True,'global_working_directory':'CFileSystem.WorkingDirectory99b138 initiallyBSSempty; actual getter56c214','literal_prefix':'ata/3d/textures/','resolved_requests':rows,'source_services':['initialized original allocator singleton','explicit source global working directory bytes','original working-directory getter','original basename and ASCII lower routines'],'scope':'Source filename rewrite only; true archive/disk open/cache/GPU texture/working-directory mutation callsite ownership separate.'};p=ROOT/'reference/swf-render-connection';p.mkdir(parents=True,exist_ok=True);(p/'texture-resolver-probe.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(report))
if __name__=='__main__':main()
