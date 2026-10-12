"""Capture and execute the original effects cache readers with raw-stream fixtures.

Allocation and little-endian stream services are explicit host fixtures. Original
table/row instructions execute; this does not establish FX factory or playback.
"""
import hashlib,json,struct,sys,zipfile
from pathlib import Path
from capstone import Cs,CS_ARCH_ARM,CS_MODE_ARM
from elftools.elf.elffile import ELFFile
from unicorn import UC_HOOK_CODE
HERE=Path(__file__).resolve().parent;ROOT=HERE.parents[3]
sys.path.insert(0,str(ROOT/'port/engine-animation/tests'))
sys.path.insert(0,str(ROOT/'port/level-world/tools'))
from compiled_transforms_differential import Cpu,word,words
from prepare_actors import strings
ORIGINAL='36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80'
CACHE='3fdf4e4c21d45a780a7c35fb4042abde0e88e76bf75416aad1f227481560b679'
SYMBOLS=['_ZN6Arrays19AnimatedEffectTable4readEP11IStreamBase',
 '_ZN7Structs9AnimFXTpl4readEP11IStreamBase','_ZN7Structs6AnimFX4readEP11IStreamBase',
 '_ZN6Arrays15CharEffectTable4readEP11IStreamBase','_ZN7Structs10CharEffect4readEP11IStreamBase',
 '_ZN6Arrays19FootstepEffectTable4readEP11IStreamBase','_ZN7Structs14FootstepEffect4readEP11IStreamBase',
 '_ZN6Arrays10EffectDict4readEP11IStreamBase']
def sha(raw):return hashlib.sha256(raw).hexdigest()
def signed(raw,offset):return struct.unpack_from('<i',raw,offset)[0]
def canonical(result):
 out=bytearray(b'EFX1')
 def w(v):out.extend(struct.pack('<I',v&0xffffffff))
 def text(v):b=v.encode();w(len(b));out.extend(b)
 def integers(v):w(len(v));[w(x) for x in v]
 def names(v):w(len(v));[text(x) for x in v]
 w(len(result['sets']))
 for row in result['sets']:
  for key in ('force_cache','loop','type'):w(row[key])
  w(len(row['steps']))
  for step in row['steps']:
   for key in ('file','force_cancel','loop','orient_once','orient_with_anchor','play_time','pool_size','redir','scale_with_anchor','self_illum','speed_bits'):w(step[key])
   text(step['subobject'])
 w(len(result['character_rows']))
 for row in result['character_rows']:
  for key in ('blood_death','blood','footprint','swoosh','trigger_floor_fx'):w(row[key])
 w(len(result['footstep_rows']))
 for row in result['footstep_rows']:
  w(row['effect']);text(row['floor_type']);integers(row['run_sounds']);integers(row['walk_sounds'])
 for row in result['names']:names(row)
 names(result['dictionary_names']);names(result['dictionary_paths'])
 for key in ('sets','characters','footsteps','dictionary'):w(result['consumed'][key])
 return bytes(out)
def main():
 engine=ROOT/'.local-inputs/libDungeonHunter2.so';assert sha(engine.read_bytes())==ORIGINAL
 cache=Path('C:/Users/adamc/Downloads/dungeonhunter2/Dungeon-Hunter-2-HD-v1-0-2-cache.zip')
 with cache.open('rb') as f:assert hashlib.file_digest(f,'sha256').hexdigest()==CACHE
 HERE.mkdir(parents=True,exist_ok=True)
 manifest={'original_sha256':ORIGINAL,'functions':[]};assembly=[]
 with engine.open('rb') as f:
  elf=ELFFile(f);symbols={s.name:s for s in elf.get_section_by_name('.dynsym').iter_symbols()};cs=Cs(CS_ARCH_ARM,CS_MODE_ARM)
  for name in SYMBOLS:
   s=symbols[name];address,size=s['st_value'],s['st_size']
   section=elf.get_section(s['st_shndx']);raw=section.data()[address-section['sh_addr']:address-section['sh_addr']+size]
   manifest['functions'].append(dict(original_symbol=name,elf_address=hex(address),size=size,sha256=sha(raw)))
   assembly.append(name+'\n'+'\n'.join(f'{i.address:08x}: {i.mnemonic} {i.op_str}' for i in cs.disasm(raw,address)))
 (HERE/'original-functions.json').write_text(json.dumps(manifest,indent=2)+'\n')
 (HERE/'original-functions.asm').write_text('\n\n'.join(assembly)+'\n')
 cpu=Cpu(engine,False,manifest);stream={'raw':b'','offset':0};calls={};inputs={}
 readers={s['st_value']:name for name,s in symbols.items() if name.startswith('_ZN7Structs') and '4readE' in name}
 seen_readers={}
 def ret(value=0):cpu.put(0,value);cpu.uc.reg_write(cpu.pc,cpu.uc.reg_read(cpu.lr))
 def take(n):
  at=stream['offset'];raw=stream['raw'][at:at+n];assert len(raw)==n;stream['offset']+=n;return raw
 def service(uc,address,size,user):
  if address in readers:seen_readers[readers[address]]=seen_readers.get(readers[address],0)+1
  if address in (0x506990,0x4ed73c,0x4ed5a8,0x506660):calls[address]=calls.get(address,0)+1
  elif address in (0x313a90,0x4db89c,0x459090,0x3df1a0,0x4db94c):
   raw=take(1 if address==0x4db89c else 4)
   if address==0x313a90:ret(word(raw,0))
   else:cpu.uc.mem_write(cpu.reg(1),raw);ret()
  elif address==0x317454:cpu.uc.mem_write(cpu.reg(1),take(cpu.reg(2)));ret()
  elif address in (0x31056c,0x310570):
   n=cpu.reg(0);assert 0<n<0x200000;at=(cpu.heap+15)&~15;cpu.heap=at+n;cpu.uc.mem_write(at,bytes(n));ret(at)
  elif address==0x310440:ret()
 cpu.uc.hook_add(UC_HOOK_CODE,service)
 with zipfile.ZipFile(cache) as z:
  index={i.filename.lower():i for i in z.infolist()}
  def read(name):
   member=index['com.gameloft.android.gand.gloftd2ss/files/data/pydata/'+name]
   raw=z.read(member);inputs[name]=dict(entry=member.filename,bytes=len(raw),sha256=sha(raw))
   path=HERE/name
   if path.exists():assert path.read_bytes()==raw
   else:path.write_bytes(raw)
   return raw
  names_raw=read('effects_pyarraynames.bin');schemas_raw=read('effects_pystructnames.bin')
  names=[];at=0
  while at<len(names_raw):row,n=strings(names_raw[at:]);names.append(row);at+=n
  schemas=[];at=0
  while at<len(schemas_raw):row,n=strings(schemas_raw[at:]);schemas.append(row);at+=n
  dictionary_names=read('effects_dictionary_pyarraynames.bin');dictionary_raw=read('effects_dictionary_pyarray.bin')
  dictionary_keys,n=strings(dictionary_names);assert n==len(dictionary_names)
  paths,n=strings(dictionary_raw);assert n==len(dictionary_raw) and len(paths)==len(dictionary_keys)
  raw=read('effects_pyarray.bin');stream.update(raw=raw,offset=0)
  cpu.invoke(0x4bc504,[cpu.data+0x1000]);set_end=stream['offset']
  size=lambda tag:word(bytes(cpu.uc.mem_read(cpu.symbols['_ZN6Arrays'+tag+'4sizeE'],4)),0)
  base=lambda tag:word(bytes(cpu.uc.mem_read(cpu.symbols['_ZN6Arrays'+tag+'7membersE'],4)),0)
  sets=[]
  for index in range(size('19AnimatedEffectTable')):
   row=bytes(cpu.uc.mem_read(base('19AnimatedEffectTable')+index*24,24));steps=[]
   for j in range(word(row,12)):
    b=bytes(cpu.uc.mem_read(word(row,16)+j*48,48))
    steps.append(dict(file=signed(b,4),force_cancel=b[8],loop=signed(b,12),orient_once=b[16],
     orient_with_anchor=b[17],play_time=signed(b,20),pool_size=signed(b,24),redir=signed(b,28),
     scale_with_anchor=b[32],self_illum=b[33],speed_bits=word(b,36),subobject=cpu.string(word(b,44)).decode()))
   sets.append(dict(force_cache=row[4],loop=signed(row,8),type=signed(row,20),steps=steps))
  cpu.invoke(0x4bc3c0,[cpu.data+0x1000]);character_end=stream['offset']
  characters=[]
  for index in range(size('15CharEffectTable')):
   b=bytes(cpu.uc.mem_read(base('15CharEffectTable')+index*24,24))
   characters.append(dict(blood_death=signed(b,4),blood=signed(b,8),footprint=signed(b,12),swoosh=signed(b,16),trigger_floor_fx=b[20]))
  cpu.invoke(0x4bc278,[cpu.data+0x1000]);footstep_end=stream['offset']
  footsteps=[]
  for index in range(size('19FootstepEffectTable')):
   b=bytes(cpu.uc.mem_read(base('19FootstepEffectTable')+index*32,32))
   array=lambda count,pointer:[signed(bytes(cpu.uc.mem_read(pointer+4*j,4)),0) for j in range(count)]
   floor=cpu.string(word(b,12)).decode();assert len(floor)==word(b,8)
   footsteps.append(dict(effect=signed(b,4),floor_type=floor,
    run_sounds=array(word(b,16),word(b,20)),walk_sounds=array(word(b,24),word(b,28))))
  assert footstep_end==len(raw)
  stream.update(raw=dictionary_raw,offset=0)
  cpu.invoke(0x4b8458,[cpu.data+0x1000]);assert stream['offset']==len(dictionary_raw)
  count=word(bytes(cpu.uc.mem_read(cpu.symbols['_ZN6Arrays10EffectDict4sizeE'],4)),0)
  pointer=word(bytes(cpu.uc.mem_read(cpu.symbols['_ZN6Arrays10EffectDict7membersE'],4)),0)
  original_paths=[]
  for index in range(count):
   b=bytes(cpu.uc.mem_read(pointer+index*12,12));path=cpu.string(word(b,8)).decode();assert len(path)==word(b,4);original_paths.append(path)
  assert original_paths==paths
  with engine.open('rb') as f:
   elf=ELFFile(f)
   for name in seen_readers:
    if name in SYMBOLS:continue
    symbol=symbols[name];address,size=symbol['st_value'],symbol['st_size'];section=elf.get_section(symbol['st_shndx'])
    code=section.data()[address-section['sh_addr']:address-section['sh_addr']+size]
    manifest['functions'].append(dict(original_symbol=name,elf_address=hex(address),size=size,sha256=sha(code)))
    assembly.append(name+'\n'+'\n'.join(f'{i.address:08x}: {i.mnemonic} {i.op_str}' for i in cs.disasm(code,address)))
  (HERE/'original-functions.json').write_text(json.dumps(manifest,indent=2)+'\n')
  (HERE/'original-functions.asm').write_text('\n\n'.join(assembly)+'\n')
  result=dict(validation='PASS',original_sha256=ORIGINAL,cache_sha256=CACHE,
   inputs=inputs,names=names,schemas=schemas,dictionary_names=dictionary_keys,dictionary_paths=paths,
   sets=sets,character_rows=characters,footstep_rows=footsteps,
   consumed=dict(sets=set_end,characters=character_end,footsteps=footstep_end,dictionary=stream['offset']),
   reader_calls={hex(k):v for k,v in calls.items()},executed_struct_readers=seen_readers,
   script_sha256=sha(Path(__file__).read_bytes()),manifest_sha256=sha((HERE/'original-functions.json').read_bytes()),
   assembly_sha256=sha((HERE/'original-functions.asm').read_bytes()),scope=__doc__)
  result['canonical_projection_sha256']=sha(canonical(result))
  (HERE/'original-reader-projection.json').write_text(json.dumps(result,indent=2)+'\n')
  (HERE/'original-reader-projection.bin').write_bytes(canonical(result))
  print(json.dumps(dict(validation='PASS',sets=len(sets),characters=len(characters),footsteps=len(footsteps),consumed=result['consumed'])))
if __name__=='__main__':main()
