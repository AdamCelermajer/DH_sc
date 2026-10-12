"""Genuine glyph alpha image/conversion and exact canonical-cache identity evidence."""
import hashlib,json,struct,sys,zipfile
from pathlib import Path
from unicorn import UC_HOOK_CODE
ROOT=Path(__file__).resolve().parents[1];REPO=ROOT.parents[1]
sys.path.insert(0,str(ROOT/'tests'))
from swf_texture_resolver_probe import Cpu
from shader_sources_differential import sha,words
def main():
 engine=REPO/'.local-inputs/libDungeonHunter2.so';cpu=Cpu(engine,False,{'functions':[]});cpu.pointer(0x99f698,1);b=cpu.data+0x1000;bitmap=b;driver=b+0x100;image=b+0x300;pixels=b+0x1000;input=b+0x2000;out=b+0x3000;format_requests=[]
 def hook(uc,at,size,user):
  if at==0x5e8788:
   dimensions=struct.unpack('<2I',uc.mem_read(cpu.reg(3),8));format_requests.append({'format':cpu.reg(2),'dimensions':list(dimensions)});cpu.pointer(cpu.reg(0),image);cpu.pointer(image+4,1);cpu.pointer(image+8,pixels);uc.reg_write(cpu.pc,uc.reg_read(cpu.lr))
  elif at==0x31d584:uc.reg_write(cpu.pc,uc.reg_read(cpu.lr))
 cpu.uc.hook_add(UC_HOOK_CODE,hook);alpha_rows=[]
 for first in range(0,256,8):
  alpha=bytes((first+i)&255 for i in range(8));cpu.uc.mem_write(input,alpha);cpu.uc.mem_write(bitmap,bytes(0x34));cpu.invoke(0x7d5404,[bitmap,driver,4,2,input]);assert format_requests[-1]=={'format':12,'dimensions':[4,2]};argb=bytes(cpu.uc.mem_read(pixels,32));assert argb==b''.join(bytes([a,255,255,255])for a in alpha);cpu.invoke(0x5f95ac,[12,pixels,16,14,out,16,4,2,0]);rgba=bytes(cpu.uc.mem_read(out,32));assert rgba==b''.join(bytes([255,255,255,a])for a in alpha);alpha_rows.append({'alpha_hex':alpha.hex(),'original_image12_hex':argb.hex(),'original_converted14_rgba_hex':rgba.hex()})
 formats={str(i):list(struct.unpack('<10I',cpu.uc.mem_read(0x8e36c0+i*40,40)))for i in [2,12,14]};cache=Path('C:/Users/adamc/Downloads/dungeonhunter2/Dungeon-Hunter-2-HD-v1-0-2-cache.zip');archive_sha=sha(cache);assert archive_sha=='3fdf4e4c21d45a780a7c35fb4042abde0e88e76bf75416aad1f227481560b679';fs=b+0x500;vt=fs+0x100;cpu.pointer(fs,vt);cpu.pointer(vt+0x2c,0x56c214);cpu.uc.mem_write(0x99b138,b'\0');bindings=[]
 with zipfile.ZipFile(cache)as z:
  for export in ['menus/MenusGraphics_droid.tga','menus/MenusGraphics.tga','menus/map_top.tga','menus/map_bottom.tga','menus/iPhone_Table_DH2.tga','menus/splash_final_droid.tga']:
   request=('data/'+export).encode();cpu.uc.mem_write(input,request+b'\0');cpu.invoke(0x34e96c,[out,fs,input]);resolved=cpu.string(struct.unpack('<I',cpu.uc.mem_read(out+20,4))[0]).decode();matches=[n for n in z.namelist()if n.lower().endswith('/'+resolved)];assert len(matches)==1,(resolved,matches);data=z.read(matches[0]);bindings.append({'export_name':export,'source_loader_generic_request':request.decode(),'original_resolved_request':resolved,'canonical_cache_file':matches[0],'size':len(data),'sha256':hashlib.sha256(data).hexdigest(),'logical_casefold_index':resolved,'physical_case_preserved':True})
 report={'validation':'PASS','original_sha256':sha(engine),'original_instructions_executed':True,'alpha_constructor_cases':len(alpha_rows),'alpha_pixels':len(alpha_rows)*8,'format_table_address':'0x8e36c0','format_descriptor40_words':formats,'alpha_image_format12_constructor':'0x7d5404','actual_format12_to14_convert':'0x5f95ac','alpha_cases':alpha_rows,'canonical_cache_sha256':archive_sha,'canonical_cache_bindings':bindings,'source_services':['actual original filename/cwd/basename/lower instructions','image allocation/identity observer, real original alpha byte writes','required reference release observer','genuine original pixel_format conversion'],'scope':'Original logical filename and pixel-layout/conversion evidence. Canonical-cache casefold index is an explicit modern native resource service, not proof that the original platform registered this archive with ignorecase. No source bitmap/GL upload ownership or complete font/display-list parity.'};outpath=ROOT/'reference/swf-render-connection/connection-probe.json';outpath.write_text(json.dumps(report,indent=2)+'\n');print(json.dumps({'validation':'PASS','alpha_constructor_cases':len(alpha_rows),'alpha_pixels':len(alpha_rows)*8,'canonical_bindings':len(bindings),'report_sha256':sha(outpath)}))
if __name__=='__main__':main()
