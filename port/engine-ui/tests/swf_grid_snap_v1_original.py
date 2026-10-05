"""Actual original bitmap grid-snap block versus optimized native ARM64."""
import sys,struct,json,hashlib,random,pathlib,argparse
ROOT=pathlib.Path(__file__).resolve().parents[3]
sys.path.insert(0,str(ROOT/'port/engine-ui/tests'))
from gfnt_differential import Cpu,bits
from unicorn import UC_HOOK_CODE
def main():
 p=argparse.ArgumentParser();p.add_argument('--library',type=pathlib.Path,required=True);p.add_argument('--output',type=pathlib.Path,required=True);a=p.parse_args()
 old=Cpu(ROOT/'.local-inputs/libDungeonHunter2.so',False,{'functions':[]});new=Cpu(a.library,True,{'functions':[]})
 def stop(uc,address,size,_):
  if address==0x7d9138:uc.reg_write(old.pc,old.stop)
 old.uc.hook_add(UC_HOOK_CODE,stop,begin=0x7d9138,end=0x7d9138)
 values=[bits(v) for v in [-2147483648.,2147483648.,-40.9,-30.,-20.9,-20.,-19.,-10.,-9.,-.9,0.,9.,10.,19.,20.,30.]]
 values += [0x7fc00000,0xffc00000,0x7f800000,0xff800000,0x7f7fffff,0xff7fffff]
 values += [bits(i/10) for i in range(-10000,10001)]
 rng=random.Random(137);values += [rng.getrandbits(32) for _ in range(4096)]
 for value in values:
  old.put(5,20);old.put(9,old.data+0x1000)
  old.uc.mem_write(old.stack+0xe014,struct.pack('<I',0x66666667))
  old.invoke(0x7d9114,[value])
  expected=struct.unpack('<I',old.uc.mem_read(old.data+0x100c,4))[0]
  actual=new.invoke('dh2_swf_grid_snap_v1',[value]);assert expected==actual,(hex(value),hex(expected),hex(actual))
 report={'validation':'PASS','cases':len(values),'mismatches':0,'scope':'Original draw_bitmap7d9114..7d9134 instructions; compiler float conversion imports modeled. Negative truncation asymmetry, NaN/infinity and wrapping +10/multiply preserved.','original_sha256':hashlib.sha256((ROOT/'.local-inputs/libDungeonHunter2.so').read_bytes()).hexdigest(),'library_sha256':hashlib.sha256(a.library.read_bytes()).hexdigest()}
 a.output.write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(report))
if __name__=='__main__':main()
