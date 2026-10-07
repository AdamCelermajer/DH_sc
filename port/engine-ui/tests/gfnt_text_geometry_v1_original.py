import sys,pathlib,struct,json,hashlib,argparse
ROOT=pathlib.Path(__file__).resolve().parents[3];sys.path.insert(0,str(ROOT/'port/engine-ui/tests'))
from gfnt_differential import Cpu
from unicorn import UC_HOOK_CODE
def main():
 p=argparse.ArgumentParser();p.add_argument('--library',type=pathlib.Path,required=True);p.add_argument('--output',type=pathlib.Path,required=True);a=p.parse_args()
 old=Cpu(ROOT/'.local-inputs/libDungeonHunter2.so',False,{'functions':[]});new=Cpu(a.library,True,{'functions':[]})
 def stop(uc,address,size,_):
  if address==0x7c5be4:uc.reg_write(old.pc,old.stop)
 old.uc.hook_add(UC_HOOK_CODE,stop,begin=0x7c5be4,end=0x7c5be4)
 gold=(ROOT/'port/engine-ui/reference/gfnt/original-fixtures.bin').read_bytes();n=struct.unpack_from('<I',gold,4)[0];offset=8;cases=0
 for _ in range(n):
  code,size,status=struct.unpack_from('<IiI',gold,offset);offset+=12;words=struct.unpack_from('<5I',gold,offset);offset+=20;count=struct.unpack_from('<I',gold,offset)[0];offset+=4+count
  if not status or size<=0 or size>65535:continue
  bearing,baseline,width,height,advance=words;rw=max(16,(width+1+15)&~15);rh=max(16,(height+1+15)&~15)
  sp=old.stack+0xe000;entity=old.data+0x1000
  old.uc.mem_write(sp+0xc,struct.pack('<5I',*words));old.uc.mem_write(sp+0x24,struct.pack('<II',width,height));old.uc.mem_write(sp+0x30,struct.pack('<II',rh,rw));old.pointer(sp+0x38,entity)
  old.invoke(0x7c5af0,[])
  # source entity:advance+4;rect x0,x1,y0,y1+8
  expected=bytes(old.uc.mem_read(entity+8,16))+bytes(old.uc.mem_read(entity+4,4))
  output=new.data+0x1000;input=new.data+0x2000;new.uc.mem_write(input,struct.pack('<5I',*words))
  assert new.invoke('gfnt_text_geometry_v1_oracle',[output,input,size,rw,rh])==1
  actual=bytes(new.uc.mem_read(output,20));assert expected==actual,(code,size,expected.hex(),actual.hex());cases+=1
 report={'validation':'PASS','comparisons':cases,'mismatches':0,'scope':'Original cached GFNT wrapper7c5af0..7c5bd8 float geometry/advance against native original8532-raster gold; importsfloat32 modeled, source cache allocation/image upload not modeled.','library_sha256':hashlib.sha256(a.library.read_bytes()).hexdigest()}
 a.output.write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(report))
if __name__=='__main__':main()
