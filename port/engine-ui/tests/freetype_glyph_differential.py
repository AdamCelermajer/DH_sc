"""Original no-cache provider/padding vs O2 ARM64 layout, genuine FT237 metrics corpus."""
import argparse,hashlib,importlib.util,json,pathlib,struct,sys
from unicorn import UC_HOOK_CODE
from unicorn.arm64_const import UC_ARM64_REG_S0
R=pathlib.Path(__file__).resolve().parents[3]
spec=importlib.util.spec_from_file_location('gfnt_cpu',R/'port/engine-ui/tests/gfnt_differential.py');mod=importlib.util.module_from_spec(spec);spec.loader.exec_module(mod)
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest()
class Cpu(mod.Cpu):
 def external(self,uc,address,size,unused):
  name=self.imports.get(address)
  if name in ('bitmap_width','bitmap_height'):
   self.put(0,self.dimensions[0 if name=='bitmap_width' else 1]);uc.reg_write(self.pc,uc.reg_read(self.lr));return
  super().external(uc,address,size,unused)
def main():
 p=argparse.ArgumentParser();p.add_argument('--corpus',type=pathlib.Path,required=True);p.add_argument('--library',type=pathlib.Path,required=True);p.add_argument('--output',type=pathlib.Path,required=True);a=p.parse_args()
 m=json.loads((R/'port/level-world/reference/font-text/freetype-glyph/original-functions.json').read_text());old=Cpu(R/'.local-inputs/libDungeonHunter2.so',False,m);new=Cpu(a.library,True,{'functions':[]})
 d=old.data;provider=d+0x1000;entity=d+0x2000;face=d+0x3000;slot=d+0x4000;pixels=d+0x10000;image=d+0x5000;imagepixels=d+0x100000;bitmap=d+0x6000;vtable=d+0x7000;glyph=d+0x8000;bounds=d+0x9000;advance=d+0xa000
 old.pointer(entity+0x24,face);old.pointer(face+0x54,slot);old.pointer(provider+0x28,0);old.pointer(bitmap,vtable)
 for off,name in [(0x24,'bitmap_width'),(0x28,'bitmap_height')]:
  addr=old.extern+0xff10+off;old.pointer(vtable+off,addr);old.imports[addr]=name
 calls=[];image_copy=b'';pixel_height=0;flags=0
 def service(uc,address,size,unused):
  nonlocal image_copy,pixel_height,flags
  if address not in (0x7d113c,0x7c4418,0x70a7fc,0x709660,0x752ba8,0x7b5c84,0x773bac,0x77a740,0x7d0444,0x7c5878):return
  args=[old.reg(i) for i in range(4)];calls.append(hex(address))
  if address==0x7d113c:old.put(0,entity)
  elif address==0x7c4418:old.put(0,0xffffffff)
  elif address==0x70a7fc:pixel_height=args[2];assert args[1]==0;old.put(0,0)
  elif address==0x709660:flags=args[2];old.put(0,0)
  elif address==0x752ba8:assert args[0]==24;old.uc.mem_write(glyph,b'\0'*24);old.put(0,glyph)
  elif address==0x7b5c84:
   w,h=args[:2];old.dimensions=(w,h);old.uc.mem_write(image,struct.pack('<6I',0,0,imagepixels,w,h,w));old.put(0,image)
  elif address==0x773bac:
   assert args[:2]==list(old.dimensions);image_copy=bytes(old.uc.mem_read(args[2],args[0]*args[1]));old.put(0,bitmap)
  elif address==0x77a740:old.pointer(args[0],args[1]);old.put(0,args[0])
  else:old.put(0,0)
  uc.reg_write(old.pc,uc.reg_read(old.lr))
 old.uc.hook_add(UC_HOOK_CODE,service)
 raw=a.corpus.read_bytes();magic,count=struct.unpack_from('<2I',raw);assert magic==0x31474646;at=8;total_services=0
 for i in range(count):
  code,height,scale=struct.unpack_from('<3I',raw,at);at+=12;metrics=raw[at:at+32];at+=32;expected=raw[at:at+40];at+=40;n=struct.unpack_from('<I',raw,at)[0];at+=4;alpha=raw[at:at+n];at+=n
  width,rows,bearingx,bearingy,adv,bw,bh,pitch=struct.unpack('<8i',metrics)
  old.uc.mem_write(slot+0x18,struct.pack('<5i',width,rows,bearingx,bearingy,adv));old.uc.mem_write(slot+0x4c,struct.pack('<3iI',bh,bw,pitch,pixels));old.uc.mem_write(pixels,alpha or b'\0');old.pointer(provider+4,scale);old.uc.mem_write(bounds,b'\xa5'*16);old.pointer(advance,0xa5a5a5a5);calls.clear()
  rc=old.invoke(0x7d1614,[provider,code,d+0xb000,0,0,height,bounds,advance]);assert rc==bitmap and flags==4
  words=struct.pack('<2I',*old.dimensions)+bytes(old.uc.mem_read(bounds,16))+bytes(old.uc.mem_read(advance,4))+struct.pack('<iII',pixel_height,bw,bh)
  assert words==expected,('original',i,words.hex(),expected.hex())
  ew,eh=old.dimensions;padded=bytearray(ew*eh)
  for y in range(bh):padded[y*ew:y*ew+bw]=alpha[y*pitch:y*pitch+bw]
  assert image_copy==padded,('alpha padding',i)
  ni=new.data+0x1000;no=new.data+0x2000;new.uc.mem_write(ni,metrics);new.uc.reg_write(UC_ARM64_REG_S0,scale);nr=new.invoke('dh2_freetype_glyph_layout',[no,ni,height]);assert nr==1 and bytes(new.uc.mem_read(no,40))==words,('ARM64',i)
  total_services+=len(calls)
 assert at==len(raw)
 sources=[R/'port/engine-ui/freetype_glyph_kernel.hpp',R/'port/engine-ui/freetype_glyph_kernel.cpp',pathlib.Path(__file__)]
 report={'validation':'PASS','comparisons':count,'mismatches':0,'ordered_service_requests':total_services,'original_sha256':m['original_sha256'],'corpus_sha256':sha(a.corpus),'library_sha256':sha(a.library),'source_sha256':{str(p.relative_to(R)).replace('\\','/'):sha(p) for p in sources},'scope':'Actual original get_char_image no-cache and draw_bitmap instructions, including pixel-height, ordered FT/render boundaries, exact normalized float32 bounds/advance and alpha padding, vs O2 ARM64 layout. Original face lookup/cache/FT glyph producer/allocation/upload are explicit services. Corpus metrics/alpha were produced by genuine source-built FreeType2.3.7 and exact Fontin bytes; original FreeType raster leaves and full GPU parity are not claimed.'}
 a.output.parent.mkdir(parents=True,exist_ok=True);a.output.write_text(json.dumps(report,indent=2)+'\n');print(json.dumps({k:report[k] for k in ['validation','comparisons','ordered_service_requests','mismatches']}))
if __name__=='__main__':main()
