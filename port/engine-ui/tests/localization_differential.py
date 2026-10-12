"""Original ordered metadata/transform execution versus O2 native helpers."""
import argparse,hashlib,json,struct,sys,time
from pathlib import Path
from localization_transform_probe import Cpu,ROOT,REPO,main as transform_probe
from unicorn import UC_HOOK_CODE
from localization_table_probe import main as table_probe
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def words(*v):return struct.pack('<'+'I'*len(v),*v)
def text(s):return words(len(s))+s
def main():
 p=argparse.ArgumentParser();p.add_argument('--library',type=Path,default=REPO/'.local-inputs/localization-discovery/localization64.so');a=p.parse_args();started=time.monotonic();transform_probe();table_probe()
 new=Cpu(a.library,True,{'functions':[]});at=new.data+0x1000;out=at+0x100000;records=[];counts={}
 def compare(op,blob,expected):
  new.uc.mem_write(at,blob);n=new.invoke('dh2_localization_test',[op,at,len(blob),out]);actual=bytes(new.uc.mem_read(out,len(expected)))if expected else b'';assert n==len(expected) and actual==expected,(op,n,blob.hex(),actual.hex(),expected.hex());records.append(words(op,len(blob))+blob+text(expected));counts[op]=counts.get(op,0)+1
 rows=json.loads((REPO/'.local-inputs/localization-discovery/transform-probe.json').read_text())
 for r in rows['cases']:
  mode=r['mode'];blob=words(0)+(words(*[i*0x010203 for i in range(10)])if mode==0 else b'')+text(bytes.fromhex(r['input']));expected=bytes.fromhex(r['output'])
  if mode==0:expected=words(r['returned'])+expected
  if mode==2:blob+=text(b'Prince')
  compare(mode,blob,expected)
 # Execute original color parser on EVERY actual cache string reaching preload's
 # caret/pipe branch, using the original FontTextColors input words.
 def cst(path):
  raw=path.read_bytes();at=0
  def word():
   nonlocal at
   v=struct.unpack_from('<I',raw,at)[0];at+=4;return v
  def string():
   nonlocal at
   n=word();s=raw[at:at+n];at+=n;return s
  result={}
  for _ in range(word()):
   group=string();row={}
   for _ in range(word()):key=string();row[key]=word()
   result[group]=row
  assert at==len(raw);return result
 fonts=REPO/'port/android-native/app/src/main/assets/data/fonts_pycst.bin';colors=cst(fonts)[b'FontTextColors'];keys=[b'zero',b'one',b'two',b'three',b'four',b'five',b'six',b'seven',b'eight',b'nine'];color_words=[colors[k]for k in keys]
 old=Cpu(REPO/'.local-inputs/libDungeonHunter2.so',False,{'functions':[]});old.pointer(0x99f698,1);oi=old.data+0x1000;oo=oi+0x20000;manager=oo+0x200;old.uc.mem_write(oi+0x10000,b'\0')
 def hook(uc,address,size,user):
  if address==0x4c4bdc:
   assert old.string(old.reg(1))==b'FontTextColors';old.put(0,colors[old.string(old.reg(2))]);uc.reg_write(old.pc,uc.reg_read(old.lr))
 old.uc.hook_add(UC_HOOK_CODE,hook);actual_color_cases=0;asset_bindings={fonts.relative_to(REPO).as_posix():sha(fonts)}
 for row in json.loads((REPO/'.local-inputs/localization-discovery/common-text-probe.json').read_text())['packs']:
  pack=row['pack'];old.pointer(manager+4,pack)
  for sheet in row['sheets']:
   path=REPO/'port/android-native/app/src/main/assets/original-cache/data'/sheet['filename'];raw=path.read_bytes();asset_bindings[path.relative_to(REPO).as_posix()]=sha(path);cursor=2
   for _ in range(struct.unpack_from('<H',raw)[0]):
    length=struct.unpack_from('<H',raw,cursor)[0];cursor+=2;value=raw[cursor:cursor+length];cursor+=length
    if b'^'not in value and b'|'not in value:continue
    old.uc.mem_write(oi,value+b'\0');old.invoke(0x3140ec,[oo,oi+0x10000,0]);changed=old.invoke(0x507ea4,[manager,oo,oi]);start=struct.unpack('<I',old.uc.mem_read(oo+20,4))[0];end=struct.unpack('<I',old.uc.mem_read(oo+16,4))[0];expected=words(changed)+bytes(old.uc.mem_read(start,end-start));compare(0,words(4<=pack<=6)+words(*color_words)+text(value),expected);old.invoke(0x3139ac,[oo]);actual_color_cases+=1
   assert cursor==len(raw)
 metadata=json.loads((REPO/'.local-inputs/localization-discovery/common-text-probe.json').read_text());assets=REPO/'port/android-native/app/src/main/assets/original-cache/data/pydata';blob=b''.join(text((assets/x).read_bytes())for x in ('common_text_pyarray.bin','common_text_pyarraynames.bin','common_text_pystructnames.bin'));expected=b''.join(text(s[k].encode())for row in metadata['packs']for s in row['sheets']for k in ('name','filename'));compare(3,blob,expected)
 gold=ROOT/'reference/localization/localization-fixtures.bin';gold.write_bytes(words(0x31434f4c,len(records))+b''.join(records));sources=[ROOT/'localization.hpp',ROOT/'localization.cpp',ROOT/'tests/localization.cpp',Path(__file__),ROOT/'tests/localization_transform_probe.py',ROOT/'tests/localization_table_probe.py'];report={'validation':'PASS','original_sha256':sha(REPO/'.local-inputs/libDungeonHunter2.so'),'original_instructions_executed':True,'optimized_arm64_instructions_executed':True,'comparisons':len(records),'operation_counts':counts,'actual_cache_color_cases':actual_color_cases,'asset_sha256':asset_bindings,'mismatches':0,'arm64_library_sha256':sha(a.library),'gold_sha256':sha(gold),'source_sha256':{x.relative_to(REPO).as_posix():sha(x)for x in sources},'original_probes':{p.name:sha(p)for p in [REPO/'.local-inputs/localization-discovery/transform-probe.json',REPO/'.local-inputs/localization-discovery/common-text-probe.json']},'scope':'Actual original common_text reader/name/filename getters and color/plain/first-player-name transforms. Explicit stream/allocator, libc formatting and constants/default services. Full owned NativeGetStringFromSymbol connection audited separately; no GameSWF conversion/complete varargs/local language producer or packaged GPU claim.','elapsed_seconds':time.monotonic()-started};dest=ROOT/'reports/localization-arm64-differential.json';dest.write_text(json.dumps(report,indent=2)+'\n');print(json.dumps({'validation':'PASS','comparisons':len(records),'operation_counts':counts,'mismatches':0}))
if __name__=='__main__':main()
