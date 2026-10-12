"""Actual ItemTable/Item/ItemBase readers versus native ARM64 record decoding.

Original stream-reader instructions execute; stream virtual and allocator are
explicit byte/storage services. Record vtable/string pointers and bool padding
are normalized into the native scalar projection, while every data bit and
string byte is compared. No serialized row/type result is supplied as a bool.
"""
import argparse,hashlib,json,struct,sys,random
from pathlib import Path
ROOT=Path(__file__).resolve().parents[1];REPO=ROOT.parents[1]
sys.path.insert(0,str(REPO/'port/level-world/tests'))
from navigation_differential import Cpu as Parent
from unicorn import UC_HOOK_CODE

def words(*values):return struct.pack('<'+'I'*len(values),*(v&0xffffffff for v in values))
def strings(data):
 at=0;out=[]
 while at<len(data):
  count=struct.unpack_from('<I',data,at)[0];at+=4;block=[]
  for _ in range(count):
   size=struct.unpack_from('<I',data,at)[0];at+=4;block.append(data[at:at+size]);at+=size
  out.append(block)
 return out

class Original(Parent):
 def __init__(self,path,manifest):
  super().__init__(path,False,manifest);self.blob=b'';self.cursor=0;self.heap=self.data+0x100000;self.reads=0;self.allocations=0
  self.stream=self.data+0x1000;self.vt=self.stream+0x100;self.pointer(self.stream,self.vt);self.pointer(self.vt+0x18,self.callback+16)
  self.uc.hook_add(UC_HOOK_CODE,self.storage)
 def external(self,uc,address,size,unused):
  if address==self.callback+16:
   assert self.reg(0)==self.stream;count=self.reg(2);assert self.cursor+count<=len(self.blob),(self.cursor,count)
   if count:uc.mem_write(self.reg(1),self.blob[self.cursor:self.cursor+count])
   self.cursor+=count;self.reads+=1;self.put(1,0);self.returned(count)
  elif self.imports.get(address)=='strcmp':
   def text(at):
    result=bytearray()
    while uc.mem_read(at+len(result),1)!=b'\0':result.extend(uc.mem_read(at+len(result),1))
    return bytes(result)
   a,b=text(self.reg(0)),text(self.reg(1));self.returned((a>b)-(a<b))
  else:super().external(uc,address,size,unused)
 def returned(self,value=0):self.put(0,value);self.uc.reg_write(self.pc,self.uc.reg_read(self.lr))
 def storage(self,uc,address,size,unused):
  if address==0x31056c:
   count=self.reg(0);assert count<0x1000000;ptr=self.heap;self.heap+=(count+15)&~15
   assert self.heap<self.data+0x2000000
   if count:uc.mem_write(ptr,bytes(count))
   self.allocations+=1;self.returned(ptr)
  elif address==0x310440:self.returned()
 def word(self,address):return struct.unpack('<I',self.uc.mem_read(address,4))[0]
 def projection(self,address):
  values=list(struct.unpack('<41I',self.uc.mem_read(address,164)))
  texts=[bytes(self.uc.mem_read(values[pointer],values[length])) if values[length] else b'' for length,pointer in ((1,2),(19,20))]
  values[0]=values[2]=values[20]=0;values[7]&=255
  return words(*values),texts

def main():
 p=argparse.ArgumentParser();p.add_argument('--engine',type=Path,required=True);p.add_argument('--library',type=Path,required=True);p.add_argument('--manifest',type=Path,required=True);p.add_argument('--input',type=Path,required=True);p.add_argument('--names',type=Path,required=True);p.add_argument('--schema',type=Path,required=True);p.add_argument('--report',type=Path,required=True);p.add_argument('--reference-output',type=Path,required=True);a=p.parse_args()
 m=json.loads(a.manifest.read_text());assert hashlib.sha256(a.engine.read_bytes()).hexdigest()==m['original_sha256'];old=Original(a.engine,m);new=Parent(a.library,True,{'functions':[]})
 old.blob=a.input.read_bytes();steps=[]
 for address in (0x4ba4fc,0x4ba3c8,0x4ba27c,0x4ba12c):
  start=old.cursor;old.invoke(address,[old.stream],budget=20000000);steps.append({'reader':hex(address),'start':start,'end':old.cursor})
 got=0x4ba158+old.word(0x4ba26c);count=old.word(old.word(got+old.word(0x4ba270)));table=old.word(old.word(got+old.word(0x4ba278)))
 expected=[old.projection(table+i*164) for i in range(count)];assert count==1322
 out=new.data+0x1000;text=new.data+0x2000;used=new.data+0x2100;input_address=new.data+0x3000
 def compare(blob,want,label):
  new.uc.mem_write(input_address,blob);new.uc.mem_write(out,bytes([0xaa])*164);new.uc.mem_write(text,bytes([0xbb])*32);new.uc.mem_write(used,words(0xcccccccc))
  assert new.invoke('dh2_item_decode_record',[out,text,used,input_address,len(blob)])==0,label
  consumed=struct.unpack('<I',new.uc.mem_read(used,4))[0];actual=bytes(new.uc.mem_read(out,164));spans=struct.unpack('<QIIQII',new.uc.mem_read(text,32));texts=[]
  for ptr,size,reserved in (spans[:3],spans[3:]):assert input_address<=ptr<=input_address+len(blob) and ptr+size<=input_address+len(blob) and reserved==0;texts.append(bytes(new.uc.mem_read(ptr,size)) if size else b'')
  assert (actual,texts)==want,(label,actual.hex(),want[0].hex(),texts,want[1]);return consumed
 records=[];at=steps[3]['start']+4
 for i,want in enumerate(expected):
  consumed=compare(old.blob[at:steps[3]['end']],want,('cache',i));record=old.blob[at:at+consumed];at+=consumed
  records.append(words(len(record))+record+want[0]+b''.join(words(len(t))+t for t in want[1]))
 assert at==steps[3]['end']
 rng=random.Random(20261003);boundary_words=[0,0x80000000,0x7fffffff,0xffffffff,0x7f800000,0xff800000,0x7fc01234,0x7f801234,1]
 for i in range(256):
  icon=rng.choice((b'',b'path/icon.tga',b'embedded\0tail',bytes(range(256))));name=rng.choice((b'',b'localized_name',b'\0head',b'x'*1024));numeric=[rng.choice(boundary_words) if i<128 else rng.getrandbits(32) for _ in range(35)];boolean=rng.choice((0,1,2,127,255))
  blob=words(len(icon))+icon+words(*numeric[:4])+bytes([boolean])+words(*numeric[4:15])+words(len(name))+name+words(*numeric[15:])
  old.blob=blob;old.cursor=0;row=old.data+0x2000;old.uc.mem_write(row,bytes(164));old.invoke(0x4fc068,[row,old.stream]);want=old.projection(row);assert old.cursor==len(blob)
  assert compare(blob,want,('boundary',i))==len(blob)
  records.append(words(len(blob))+blob+want[0]+b''.join(words(len(t))+t for t in want[1]))
 # Original serialized identifier readers populate the real lookup globals.
 old.blob=a.names.read_bytes();old.cursor=0;name_steps=[]
 for address in (0x4b53f4,0x4b5258,0x4b4a54,0x4b4724):
  start=old.cursor;old.invoke(address,[old.stream]);name_steps.append({'reader':hex(address),'start':start,'end':old.cursor})
 identifiers=strings(old.blob)[3];assert len(identifiers)==count
 lookup_inputs=identifiers+[b'',b'not_an_original_item',identifiers[1].swapcase(),identifiers[1]+b'\0ignored_suffix']
 lookups=[];lookup_address=old.data+0x3000
 for key in lookup_inputs:
  old.uc.mem_write(lookup_address,key+b'\0');result=old.invoke(0x4ad5f8,[lookup_address]);lookups.append(words(len(key))+key+words(result))
 # Range capability traverses genuine loaded ItemTable rows, not a type stub.
 owner=old.data+0x4000;inventory=owner+0x37c;equip_entries=old.data+0x6000;equip_set=equip_entries+0x100;reference=equip_entries+0x200;instance=equip_entries+0x300
 old.uc.mem_write(owner,bytes(0x1200));old.pointer(inventory+0x14,equip_entries);old.pointer(equip_entries,equip_set);old.pointer(equip_set+4,reference);old.pointer(reference,instance);old.uc.mem_write(owner+0x1078,words(-1));range_results=[]
 for i in range(count):
  old.uc.mem_write(instance+4,words(i));result=old.invoke(0x3a4d3c,[owner]);assert result==int(expected[i][0][88:92] in (words(4),words(5)));range_results.append(words(result))
 blob=b'ITM1'+words(len(records),count,steps[3]['start'],steps[3]['end'],len(lookups),len(range_results))+b''.join(records)+b''.join(lookups)+b''.join(range_results);a.reference_output.parent.mkdir(parents=True,exist_ok=True);a.reference_output.write_bytes(blob)
 report={'validation':'PASS','original_sha256':m['original_sha256'],'arm64_sha256':hashlib.sha256(a.library.read_bytes()).hexdigest(),'manifest_sha256':hashlib.sha256(a.manifest.read_bytes()).hexdigest(),'corpus_sha256':hashlib.sha256(blob).hexdigest(),'script_sha256':hashlib.sha256(Path(__file__).read_bytes()).hexdigest(),'source_sha256':{n:hashlib.sha256((REPO/n).read_bytes()).hexdigest() for n in ('port/game-data/items.hpp','port/game-data/items.cpp')},'comparisons':len(records),'actual_cache_rows':count,'synthetic_record_cases':256,'original_identifier_queries':len(lookups),'original_loaded_item_range_queries':len(range_results),'original_section_offsets':steps,'original_names_section_offsets':name_steps,'stream_read_calls':old.reads,'caller_storage_allocations':old.allocations,'inputs_sha256':{str(path):hashlib.sha256(path.read_bytes()).hexdigest() for path in (a.input,a.names,a.schema)},'mismatches':0,'scope':__doc__+'\nIdentifier and loaded-item range query instructions produce additional host gold; native ARM64 comparison here is the full record decoder.'};a.report.parent.mkdir(parents=True,exist_ok=True);a.report.write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(report))
if __name__=='__main__':main()
