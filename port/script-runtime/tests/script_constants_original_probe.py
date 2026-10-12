"""Explore genuine PyDataConstants loader and string-map dependencies."""
import sys,struct,zipfile,json,argparse,hashlib
from pathlib import Path
from unicorn import UC_HOOK_CODE
from capstone import Cs,CS_ARCH_ARM,CS_MODE_ARM
from elftools.elf.elffile import ELFFile
ROOT=Path(__file__).resolve().parents[3]
sys.path.insert(0,str(ROOT/'port/game-data/tests'))
from items_differential import Original

class Oracle(Original):
 def __init__(self):
  super().__init__(ROOT/'.local-inputs/libDungeonHunter2.so',{'functions':[]})
  self.manager=self.data+0x10000
  self.uc.mem_write(self.manager,bytes(256))
  header=self.manager+4
  self.uc.mem_write(header,struct.pack('<5I',0,0,header,header,0))
  self.uc.hook_add(UC_HOOK_CODE,self.ignore_log)
  self.trace=[];self.groups_complete=0;self.name_stop=0
 def ignore_log(self,uc,address,size,unused):
  # These calls operate only on the temporary diagnostic string/logger. The
  # actual map/string insertion helpers execute below, without replacement.
  if address in (0x4c544c,0x4c5464,0x4c5470,0x4c547c):
   uc.reg_write(self.pc,address+4)
  elif address==0x4c562c:self.group=self.text(self.reg(1))
  elif address==0x4c5634:self.name=self.text(self.reg(1))
  elif address==0x4c5640:self.trace.append((self.group,self.name,self.reg(3)))
  elif address==0x4c5650:self.groups_complete+=1
  elif address in (0x4c5530,0x4c55c4) and self.reg(0)==0:self.name_stop=1
 def text(self,p):
  out=bytearray()
  while self.uc.mem_read(p+len(out),1)!=b'\0':out+=self.uc.mem_read(p+len(out),1)
  return bytes(out)
 def external(self,uc,address,size,unused):
  name=self.imports.get(address)
  if name=='__aeabi_uidiv':
   assert self.reg(1);self.returned(self.reg(0)//self.reg(1))
  elif name in ('_Znwj','_Znaj','malloc'):
   count=self.reg(0);assert count<0x1000000
   pointer=self.heap;self.heap+=(count+15)&~15
   assert self.heap<self.data+0x2000000
   if count:uc.mem_write(pointer,bytes(count))
   self.allocations+=1;self.returned(pointer)
  elif name in ('pthread_mutex_lock','pthread_mutex_unlock','_ZdlPv','free'):
   self.returned(0)
  elif name in ('memcmp','strlen','strcmp'):
   def text(p):
    out=bytearray()
    while uc.mem_read(p+len(out),1)!=b'\0':out+=uc.mem_read(p+len(out),1)
    return bytes(out)
   if name=='strlen':self.returned(len(text(self.reg(0))))
   else:
    a,b=(bytes(uc.mem_read(self.reg(i),self.reg(2))) for i in (0,1)) if name=='memcmp' else (text(self.reg(i)) for i in (0,1))
    self.returned((a>b)-(a<b))
  else:super().external(uc,address,size,unused)

def words(*values):return struct.pack('<'+'I'*len(values),*(v&0xffffffff for v in values))
def field(text):return words(len(text))+text
def encode(groups):
 return words(len(groups))+b''.join(field(group)+words(len(entries))+b''.join(field(key)+words(value) for key,value in entries) for group,entries in groups)
def sha(path):return hashlib.sha256(path.read_bytes()).hexdigest()
def main():
 p=argparse.ArgumentParser(description=__doc__);p.add_argument('--output',type=Path,required=True);a=p.parse_args()
 if a.output.exists():raise RuntimeError('Refusing to replace original constant proof')
 a.output.mkdir(parents=True)
 cache=Path('C:/Users/adamc/Downloads/dungeonhunter2/Dungeon-Hunter-2-HD-v1-0-2-cache.zip')
 assert sha(cache)=='3fdf4e4c21d45a780a7c35fb4042abde0e88e76bf75416aad1f227481560b679'
 o=Oracle();engine=ROOT/'.local-inputs/libDungeonHunter2.so';assert sha(engine)=='36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80'
 functions=[];assembly=[]
 with engine.open('rb') as f:
  elf=ELFFile(f);symbols=list(elf.get_section_by_name('.symtab').iter_symbols())
  for address in (0x4c540c,0x317734,0x313a90,0x3df1a0,0x459090,0x4c5274,0x4c4c30,0x4c4bdc,0x4c4998,0x414484):
   s=next(s for s in symbols if s['st_value']==address and s['st_size']);seg=next(s for s in elf.iter_segments() if s['p_type']=='PT_LOAD' and s['p_vaddr']<=address<s['p_vaddr']+s['p_filesz']);raw=seg.data()[address-seg['p_vaddr']:address-seg['p_vaddr']+s['st_size']]
   functions.append(dict(original_symbol=s.name,elf_address=hex(address),size=len(raw),sha256=hashlib.sha256(raw).hexdigest()))
   assembly.extend(['\n# '+s.name]+[f'{i.address:08x}: {i.mnemonic} {i.op_str}' for i in Cs(CS_ARCH_ARM,CS_MODE_ARM).disasm(raw,address)])
 manifest=dict(original_sha256=sha(engine),functions=functions)
 (a.output/'original-functions.json').write_text(json.dumps(manifest,indent=2)+'\n');(a.output/'original-functions.asm').write_text('\n'.join(assembly)+'\n')
 with zipfile.ZipFile(cache) as z:
  inputs=[(n,z.read(n)) for n in z.namelist() if n.endswith('_pycst.bin')]
 # Force sound last: this exercises the observed generic-loader early stop.
 # This is a chosen proof order, not an assertion about Application startup.
 inputs.sort(key=lambda item:(item[0].endswith('sounds_pycst.bin'),item[0]))
 synthetic=[encode([]),encode([(b'Proof',[(b'old',7),(b'duplicate',1),(b'duplicate',-2)])]),
  encode([(b'Proof',[(b'new',0x80000000)]),(b'Proof',[(b'last',0xffffffff)]),(b'empty',[])]),
  encode([(b'',[(b'',13),(b'with\0suffix',17)]),(b'bytes\xff',[(b'high\x80',-3)])]),
  encode([(b'Proof\0different',[(b'duplicate\0suffix',23)])]),
  encode([(b'g'*255,[(b'k'*255,99)])]),
  encode([(b'g'*256,[(b'ignored',12)])]),
  encode([(b'Partial',[(b'ok',1),(b'k'*256,2),(b'never',3)])]),
  encode([(b'Proof',[])])+b'ignored_suffix']
 inputs.extend((f'synthetic/{i}',blob) for i,blob in enumerate(synthetic))
 corpus=[];reports=[];query_count=0;unique=set();assignments=0
 for label,blob in inputs:
  o.blob=blob;o.cursor=0;o.trace=[];o.groups_complete=0;o.name_stop=0
  try:o.invoke(0x4c540c,[o.manager,o.stream,0],budget=200000000)
  except Exception:
   print(json.dumps(dict(input=label,failed_pc=hex(o.uc.reg_read(o.pc)),cursor=o.cursor,bytes=len(blob))),flush=True)
   raise
  queries=list(dict.fromkeys((g,k) for g,k,_ in o.trace))
  queries.extend([(b'absent',b'absent'),(b'Proof',b'old'),(b'Proof',b'duplicate'),(b'Proof',b'new'),(b'CharacterDesign',b'MaxLevelDVeryHard'),(b'AIStates',b'Attack'),(b'Empty',b''),(b'Partial',b'never')])
  if queries:queries.append((queries[0][0].swapcase(),queries[0][1]))
  qrecords=[]
  for group,key in queries:
   o.uc.mem_write(o.data+0x80000,group+b'\0');o.uc.mem_write(o.data+0x80110,key+b'\0')
   value=o.invoke(0x4c4bdc,[o.manager,o.data+0x80000,o.data+0x80110])
   qrecords.append(field(group)+field(key)+words(value))
  unique.update((g,k) for g,k,_ in o.trace);assignments+=len(o.trace);query_count+=len(queries)
  stats=(o.cursor,len(o.trace),o.groups_complete,o.name_stop)
  corpus.append(field(blob)+words(o.name_stop,*stats,len(queries))+b''.join(qrecords))
  reports.append(dict(input=label,sha256=hashlib.sha256(blob).hexdigest(),bytes=len(blob),consumed=o.cursor,assignments=len(o.trace),groups_complete=o.groups_complete,source_name_stop=bool(o.name_stop)))
  print(json.dumps(dict(input=label,consumed=o.cursor,bytes=len(blob),assignments=len(o.trace),stop=o.name_stop)),flush=True)
 gold=b'CST1'+words(len(corpus))+b''.join(corpus);(a.output/'constants-original-gold.bin').write_bytes(gold)
 report=dict(validation='PASS',scope='Actual original reloadData, bounded name readers, owned RB map insertion/string helpers and getConstant instructions; caller allocator/libc/arithmetic/thread imports explicit, logger call sites skipped.',original_sha256=sha(engine),cache_sha256=sha(cache),probe_sha256=sha(Path(__file__)),functions_manifest_sha256=sha(a.output/'original-functions.json'),gold_sha256=hashlib.sha256(gold).hexdigest(),original_loads=len(corpus),original_queries=query_count,assignments=assignments,unique_entries=len(unique),inputs=reports,ordinary_constant_files=26,sound_generic_loader_complete=False,application_file_order_proved=False,full_sound_format_decoded=False,allocator_calls=o.allocations,stream_reads=o.reads,skipped_logger_calls=['0x4c544c','0x4c5464','0x4c5470','0x4c547c'])
 (a.output/'original-loader-probe.json').write_text(json.dumps(report,indent=2)+'\n')
 print(json.dumps({k:v for k,v in report.items() if k!='inputs'}))
if __name__=='__main__':main()
