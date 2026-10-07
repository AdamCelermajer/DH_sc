import pathlib,sys,argparse,json,struct,hashlib
R=pathlib.Path(__file__).resolve().parents[3];sys.path.insert(0,str(R/'port/engine-resources/tests'));from cpu import Cpu
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest()
def main():
 p=argparse.ArgumentParser();p.add_argument('--oracle',type=pathlib.Path,required=True);p.add_argument('--output',type=pathlib.Path,required=True);a=p.parse_args();c=Cpu(a.oracle,True,{'functions':[]});d=c.data;inp=d+0x100;out=d+0x200;src=d+0x1000;dst=d+0x3000;cases=0
 for mode,bits in ((1,1),(2,8),(3,2),(4,4)):
  for value in range(256):
   for width in (1,3,7,8,9,17):
    for rows,negative in ((1,False),(3,False),(3,True)):
     rowbytes=(width*bits+7)//8;pitch=rowbytes+1;raw=bytes((value+i*19)&255 for i in range(pitch*rows));row0=(rows-1)*pitch if negative else 0;physical_pitch=-pitch if negative else pitch
     c.uc.mem_write(src,raw);c.uc.mem_write(dst,b'\xa5'*4096);c.uc.mem_write(inp,struct.pack('<QQiiiIII',src,len(raw),width,rows,physical_pitch,mode,1<<bits,row0));c.uc.mem_write(out,struct.pack('<QQ4I',dst,4096,123,123,123,0));rc=c.invoke('dh2_freetype_bitmap_alpha',[out,inp]);_,_,w,h,stride,_=struct.unpack('<QQ4I',c.uc.mem_read(out,32));assert rc==1
     expectedw=4
     while expectedw<(pitch if bits==8 else width):expectedw*=2
     expectedh=1
     while expectedh<rows:expectedh*=2
     expected=bytearray(expectedw*expectedh)
     for y in range(rows):
      for x in range(width):
       bit=x*bits;b=raw[row0+y*physical_pitch+bit//8];mask=(1<<bits)-1;expected[y*expectedw+x]=((b>>(8-bits-bit%8))&mask)*(255//mask)
     assert (w,h,stride)==(expectedw,expectedh,expectedw) and bytes(c.uc.mem_read(dst,len(expected)))==expected;cases+=1
 report={'validation':'PASS','comparisons':cases,'mismatches':0,'oracle_sha256':sha(a.oracle),'source_sha256':{str(p.relative_to(R)).replace('\\','/'):sha(p) for p in [R/'port/engine-ui/freetype_bitmap_alpha.hpp',R/'port/engine-ui/freetype_bitmap_alpha.cpp',pathlib.Path(__file__)]},'scope':'Optimized native ARM64 safe bitmap conversion vs independent MSB-first FT pixel semantics,all256bytevalues,MONO/GRAY/GRAY2/GRAY4,partialbytewidths,stride padding and bottom-up spans. Modern safe correction to original unsafe packed row copy; no original packed-font instruction parity.'};a.output.write_text(json.dumps(report,indent=2)+'\n');print(json.dumps({'validation':'PASS','comparisons':cases}))
if __name__=='__main__':main()
