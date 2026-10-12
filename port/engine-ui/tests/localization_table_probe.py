"""Execute original common_text array reader and source sheet accessors."""
import hashlib,json,struct,sys
from pathlib import Path
ROOT=Path(__file__).resolve().parents[1];REPO=ROOT.parents[1]
sys.path.insert(0,str(REPO/'port/game-data/tests'))
from items_differential import Original
from unicorn import UC_HOOK_CODE
class TableCpu(Original):
 def string(self,at):
  out=bytearray()
  while self.uc.mem_read(at+len(out),1)!=b'\0':out+=self.uc.mem_read(at+len(out),1)
  return bytes(out)
 def external(self,uc,address,size,user):
  if self.imports.get(address)=='sprintf':
   fmt=self.string(self.reg(1));assert fmt==b'text/%s';raw=fmt.replace(b'%s',self.string(self.reg(2)));uc.mem_write(self.reg(0),raw+b'\0');self.returned(len(raw));return
  return super().external(uc,address,size,user)
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def main():
 engine=REPO/'.local-inputs/libDungeonHunter2.so';old=TableCpu(engine,{'functions':[]});old.pointer(0x99f698,1)
 assets=REPO/'port/android-native/app/src/main/assets/original-cache/data/pydata';p=assets/'common_text_pyarray.bin';old.blob=p.read_bytes();calls=[]
 def observe(uc,at,size,user):
  if at==0x4b5510:
   v=old.word(old.reg(0));calls.append(hex(old.word(v+12)))
 old.uc.hook_add(UC_HOOK_CODE,observe);old.invoke(0x4b53f8,[old.stream],budget=20000000);assert old.cursor==len(old.blob)
 got=0x4b5424+old.word(0x4b5534);table=old.word(old.word(got+old.word(0x4b5540)));count=old.word(old.word(got+old.word(0x4b5538)));assert count==9
 rows=[];manager=old.data+0x4000;out=manager+0x1000;old.uc.mem_write(manager,bytes(0x7d8))
 for pack in range(count):
  # Native reader executes; only normalize host-independent source strings.
  entry=table+pack*12;size=old.word(entry+4);ptr=old.word(entry+8);assert size==37;row=[];old.pointer(manager+4,pack)
  for sheet in range(size):
   nameptr=old.invoke(0x507568,[manager,sheet]);old.invoke(0x507bbc,[manager,pack,sheet,out,100]);row.append({'sheet':sheet,'name':old.string(nameptr).decode(),'filename':old.string(out).decode()})
  rows.append({'pack':pack,'sheets':row})
 names=(assets/'common_text_pyarraynames.bin').read_bytes();schema=(assets/'common_text_pystructnames.bin').read_bytes()
 report={'validation':'PASS','original_sha256':sha(engine),'original_instructions_executed':True,'loader':'0x4b53f8','row_readers':sorted(set(calls)),'bytes_consumed':old.cursor,'input_sha256':sha(p),'name_sha256':hashlib.sha256(names).hexdigest(),'schema_sha256':hashlib.sha256(schema).hexdigest(),'packs':rows,'scope':'Original ordered common_text reader and filename/name getters; explicit byte stream/allocator services.'}
 dest=REPO/'.local-inputs/localization-discovery/common-text-probe.json';dest.write_text(json.dumps(report,indent=2)+'\n');print(json.dumps({k:v for k,v in report.items() if k!='packs'}));print(json.dumps(rows[0]))
if __name__=='__main__':main()
