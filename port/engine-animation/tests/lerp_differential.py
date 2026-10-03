"""Original float-3 scale interpreter versus actual compiled ARM64 arithmetic.

The original accessor executes against a minimal relocated caller fixture.
Only imported memcpy and soft-float helpers are modeled. No getter is mocked.
"""
import argparse,hashlib,json,random,struct,sys
from pathlib import Path
from unicorn.arm64_const import UC_ARM64_REG_S0
ROOT=Path(__file__).resolve().parents[1]
sys.path.insert(0,str(ROOT/'../scene-materials/tests'))
from transform_differential import Cpu,pack
def bits(x):return struct.unpack('<I',pack([x]))[0]
def main():
 p=argparse.ArgumentParser();p.add_argument('--engine',type=Path,required=True);p.add_argument('--library',type=Path,required=True)
 p.add_argument('--report',type=Path,required=True);a=p.parse_args()
 manifest=json.loads((ROOT/'original-functions.json').read_text());assert hashlib.sha256(a.engine.read_bytes()).hexdigest()==manifest['original_sha256']
 old=Cpu(a.engine,False,manifest);new=Cpu(a.library,True,{'functions':[]})
 accessor,record,sampler,data,values,out=[old.data+x for x in (0x1000,0x2000,0x3000,0x4000,0x5000,0x6000)]
 old.uc.mem_write(accessor,struct.pack('<4I',record,data,0,0));old.uc.mem_write(record,b'\0'*32)
 old.pointer(record+8,sampler);old.uc.mem_write(sampler,b'\0'*28)
 old.uc.mem_write(data,struct.pack('<III',1,2,values))
 rng=random.Random(221026);cases=[]
 for fraction in (0,.00001,.25,.5,.75,.99999,1):
  for i in range(40):cases.append(([rng.uniform(-1000,1000) for _ in range(3)],[rng.uniform(-1000,1000) for _ in range(3)],fraction))
 # Include the actual candle values and a fractional point between them.
 cases.extend([([1,1,1],[1.0081000328]*3,f) for f in (.1,.3,.6,.9)])
 for left,right,fraction in cases:
  old.uc.mem_write(values,pack(left+right));old.invoke(0x628850,[accessor,0,0,bits(fraction),out])
  dst,lp,rp=[new.data+x for x in (0x1000,0x2000,0x3000)]
  new.uc.mem_write(lp,pack(left));new.uc.mem_write(rp,pack(right));new.uc.reg_write(UC_ARM64_REG_S0,bits(fraction))
  new.invoke('dh2_animation_lerp3',[dst,lp,rp])
  expected=bytes(old.uc.mem_read(out,12));actual=bytes(new.uc.mem_read(dst,12))
  assert expected==actual,(left,right,fraction,expected.hex(),actual.hex())
 report={'original_sha256':manifest['original_sha256'],'arm64_library_sha256':hashlib.sha256(a.library.read_bytes()).hexdigest(),
  'original_address':'0x628850','bit_exact_cases':len(cases),'bit_exact_components':len(cases)*3,
  'scope':'float-3 interpolation arithmetic; original getter executed with a relocated caller fixture; no full-game equivalence'}
 a.report.parent.mkdir(parents=True,exist_ok=True);a.report.write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(report))
if __name__=='__main__':main()
