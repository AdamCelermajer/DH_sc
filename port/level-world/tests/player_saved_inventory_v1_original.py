"""Whole original GEAR caller; typed stream/storage/Item callees are explicit fixtures."""
from __future__ import annotations
import hashlib,json,struct,sys
from pathlib import Path
from elftools.elf.elffile import ELFFile
from unicorn import UC_HOOK_CODE
sys.path.insert(0,str(Path(__file__).resolve().parents[2]/'engine-resources/tests'))
from cpu import Cpu
ELF_SHA='36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80'
CALLER='_ZN14PlayerSavegame15__LoadInventoryEP11IStreamBasePv'
ADDRESSES=[0x46a3a0,0x3fbc58,0x461da8,0x313b48,0x38b758,0x39f638,0x3fc26c,0x3fbc60,0x3ff5d4,0x400634,0x3fdfd8]
def provenance(path):
 blob=path.read_bytes();assert hashlib.sha256(blob).hexdigest()==ELF_SHA
 with path.open('rb') as f:
  elf=ELFFile(f);symbols=list(elf.get_section_by_name('.symtab').iter_symbols());rows=[]
  def raw(at,n):
   g=next(g for g in elf.iter_segments() if g['p_type']=='PT_LOAD' and g['p_vaddr']<=at and at+n<=g['p_vaddr']+g['p_filesz']);o=g['p_offset']+at-g['p_vaddr'];return blob[o:o+n]
  for at in ADDRESSES:
   s=next(s for s in symbols if int(s['st_value'])==at and int(s['st_size'])>0);n=int(s['st_size']);rows.append({'original_symbol':s.name,'elf_address':hex(at),'size':n,'sha256':hashlib.sha256(raw(at,n)).hexdigest()})
 return {'original_sha256':ELF_SHA,'functions':rows,'scope':'Whole 1024B PlayerSavegame::__LoadInventory caller and actual 8B SetValue store/tail. Signed/unsigned/byte/string stream transports, string allocation, strcmp, Item allocation/constructor/UpdateName/AddPower, SetGold, AddItemInstance and EquipItemToSlot are declared callee fixtures. Other bodies listed in the manifest are statically pinned, not dynamically proved here. Injected fixture failures stop at a reached callee, preserving its declared prefix; they do not claim original C++ exception behavior.','attribution':'Original Gameloft ELF caller; existing sole inventory V4 and power/presentation V5 algorithms retain Adam attribution at upstream 791e961b12233100b303038c961666834f4beb9d. This adapter does not import another inventory/Save/property owner.'}
def groups(path):
 b=path.read_bytes();at=0;out=[]
 while at<len(b):
  n=struct.unpack_from('<I',b,at)[0];at+=4;g=[]
  for _ in range(n):
   k=struct.unpack_from('<I',b,at)[0];at+=4;g.append(b[at:at+k]);at+=k
  out.append(g)
 return out
def metadata(cache):
 b=(cache/'loot_table_pyarray.bin').read_bytes();at=0
 def word():
  nonlocal at
  n=struct.unpack_from('<I',b,at)[0];at+=4;return n
 for width in [4,2,7]:
  n=word()
  if width==2:at+=n*2
  else:
   for _ in range(n):k=word();at+=k*width
 count=word();rows=[]
 for _ in range(count):
  w=[0]*41;n=word();at+=n
  for j in range(3,7):w[j]=word()
  w[7]=b[at];at+=1
  for j in range(8,19):w[j]=word()
  n=word();at+=n
  for j in range(21,41):w[j]=word()
  rows.append(w)
 p=(cache/'item_powers_pyarray.bin').read_bytes();at=0
 def pw():
  nonlocal at
  n=struct.unpack_from('<I',p,at)[0];at+=4;return n
 for _ in range(pw()):n=pw();at+=n*5
 descriptions=[]
 for _ in range(pw()):
  at+=5;n=pw();at+=n*12;descriptions.append(pw());at+=16
 return rows,descriptions
def string(s):return struct.pack('<I',len(s)+1)+s+b'\0'
def payload(gold,selected,rows):
 b=struct.pack('<III',gold,selected,len(rows))
 for name,a,c,q,v,identified,powers in rows:
  b+=string(name)+struct.pack('<IIII',a&0xffffffff,c&0xffffffff,q&0xffffffff,v&0xffffffff)+bytes([identified])+struct.pack('<I',len(powers))+b''.join(string(p) for p in powers)
 return b
def private_gear(profile):
 b=profile.read_bytes();at=4;found=[]
 for _ in range(struct.unpack_from('<I',b)[0]):
  n=struct.unpack_from('<I',b,at)[0];tag=b[at+4:at+8];at+=8
  if tag==b'GEAR':found.append(b[at:at+n])
  at+=n
 assert at==len(b) and len(found)==1
 return found[0],{'profile_sha256':hashlib.sha256(b).hexdigest(),'profile_bytes':len(b),'GEAR_sha256':hashlib.sha256(found[0]).hexdigest(),'GEAR_bytes':len(found[0]),'declared_items':struct.unpack_from('<III',found[0])[2]}
class Prefix(Exception):pass
def capture(path,cache,profile,out):
 manifest=provenance(path);cpu=Cpu(path,False,manifest);items=groups(cache/'loot_table_pyarraynames.bin')[3];powers=groups(cache/'item_powers_pyarraynames.bin')[1];rows,descriptions=metadata(cache)
 weapon=next(i for i,r in enumerate(rows) if r[26]==1 and not r[7]);armor=next(i for i,r in enumerate(rows) if r[26]==0 and not r[7]);power=next(i for i,v in enumerate(descriptions) if v!=0xffffffff)
 row=lambda i,a=-1,c=-1,q=1,v=137,k=1,ps=(): (items[i],a,c,q,v,k,[powers[p] for p in ps])
 cases=[(payload(0,0,[]),0),(payload(777,257,[]),0),(payload(7,255,[]),0),
  (payload(8,1,[row(weapon,1)]),0),(payload(8,1,[row(weapon,1,1)]),0),
  (payload(9,0,[row(armor,0),row(weapon,1,1)]),0),
  (payload(10,0,[row(weapon,q=65537,k=0)]),0),
  (payload(11,0,[row(weapon,k=255,ps=[power])]),0),
  (payload(12,1,[row(weapon,1,1,ps=[power,power,power])]),0),
  (payload(13,0,[row(weapon),row(weapon)]),0),
  (payload(14,0,[row(armor,k=2)])+b'trailing',0),
  (payload(15,1,[row(weapon)]),1),(payload(16,1,[row(weapon,k=0)]),2),
  (payload(17,1,[row(weapon,k=0,ps=[power])]),3),
  (payload(18,1,[row(weapon)]),4),(payload(19,1,[row(weapon,9)]),5),
  (payload(20,0,[(b'__unknown_item__',-1,-1,1,1,1,[])]),6),
  (payload(21,0,[(items[weapon],-1,-1,1,1,1,[b'__unknown_power__'])]),7)]
 actual,private=private_gear(profile);cases.append((actual,0))
 seen=set();records=[]
 for case_number,(blob,failure) in enumerate(cases):
  cpu.seen.clear();char=cpu.data+0x1000;save=cpu.data+0x5000;stream=cpu.data+0x6000;text=cpu.data+0x8000;heap=cpu.data+0x10000
  cpu.uc.mem_write(char,bytes(0x7000));cpu.pointer(save+0x10,char);cpu.uc.mem_write(text,bytes(0x1000));stored=[];pending=None;at=0;calls={'ctor':0,'value':0,'identified':0,'power':0,'add':0,'equip':0,'completed':0};trace=[];allocated=0
  # Original GOT accesses are used. Populate the source count and name arrays.
  base=0x46a3b8+struct.unpack('<I',cpu.uc.mem_read(0x46a774,4))[0];name_at=cpu.data+0x100000
  for index,names in enumerate([items,powers]):
   key_at=0x46a77c+index*8;count_key,name_key=struct.unpack('<II',cpu.uc.mem_read(key_at,8));count_global=struct.unpack('<I',cpu.uc.mem_read(base+count_key,4))[0];name_global=struct.unpack('<I',cpu.uc.mem_read(base+name_key,4))[0]
   pointers=name_at;name_at+=4*len(names)
   for i,name in enumerate(names):cpu.pointer(pointers+i*4,name_at);cpu.uc.mem_write(name_at,name+b'\0');name_at+=len(name)+1
   cpu.pointer(count_global,len(names));cpu.pointer(name_global,pointers)
  def done(v=0):cpu.put(0,v&0xffffffff);cpu.uc.reg_write(cpu.pc,cpu.uc.reg_read(cpu.lr))
  def take(n):
   nonlocal at
   if at+n>len(blob):raise Prefix('truncated')
   b=blob[at:at+n];at+=n;return b
  def cstr(p):
   b=bytearray()
   while True:
    c=bytes(cpu.uc.mem_read(p,1));p+=1
    if c==b'\0':return bytes(b)
    b+=c
  code_seen=set()
  def hook(uc,address,size,unused):
   nonlocal pending,allocated
   if 0x46a3a0<=address<0x46a774 or 0x3fbc58<=address<0x3fbc60:code_seen.add(address)
   if address==0x31167c:
    cpu.pointer(cpu.reg(0)+0x10,text);cpu.pointer(cpu.reg(0)+0x14,text);done()
   elif address==0x3139ac:done()
   elif address in [0x313b48,0x38b758,0x39f638]:
    n=1 if address==0x39f638 else 4;uc.mem_write(cpu.reg(1),take(n));done()
   elif address==0x461da8:
    n=struct.unpack('<I',take(4))[0]
    if not n or n>1048576:raise Prefix('source string assertion')
    value=take(n);assert value[-1]==0;uc.mem_write(text,value);cpu.pointer(cpu.reg(1)+0x14,text);done()
   elif address==0x30e31c:done(0 if cstr(cpu.reg(0))==cstr(cpu.reg(1)) else 1)
   elif address==0x3fdfd8:
    assert cpu.reg(0)==char+0x37c;cpu.pointer(char+0x39c,cpu.reg(1));trace.append([1,cpu.reg(1),0])
    if failure==1:raise Prefix('gold notification failure')
    done()
   elif address==0x310570:
    assert cpu.reg(0)==0x6c;allocated+=1;done(heap+allocated*0x100)
   elif address==0x3fc26c:
    calls['ctor']+=1;pending={'address':cpu.reg(0),'id':cpu.reg(1),'quantity':cpu.reg(2)&65535,'value':0,'identified':1,'slots':[0xffffffff,0xffffffff],'powers':[]};cpu.pointer(pending['address']+0x54,0);uc.mem_write(pending['address']+0x68,b'\1');trace.append([2,pending['id'],pending['quantity']])
    if failure==6:pending=None;raise Prefix('invalid item constructor boundary')
    done()
   elif address==0x3fb754:
    calls['value']+=1;pending['value']=struct.unpack('<I',uc.mem_read(pending['address']+0x54,4))[0];trace.append([3,pending['value'],0])
    if failure==2:raise Prefix('SetValue text failure')
    done()
   elif address==0x46a57c:calls['identified']+=1
   elif address==0x3fbc60:
    calls['power']+=1;assert cpu.reg(2)==0xffffffff;pending['identified']=bytes(uc.mem_read(pending['address']+0x68,1))[0];trace.append([4,cpu.reg(1),cpu.reg(2)])
    if failure==7:raise Prefix('invalid power boundary')
    pending['powers'].append(cpu.reg(1))
    if failure==3:raise Prefix('power text failure after append')
    done()
   elif address==0x3ff5d4:
    calls['add']+=1;assert [cpu.reg(i) for i in [0,2,3]]==[char+0x37c,1,1];pending['identified']=bytes(uc.mem_read(pending['address']+0x68,1))[0];stored.append(pending);pending=None;trace.append([5,len(stored)-1,0])
    if failure==4:raise Prefix('post-storage fullness failure')
    done(len(stored)-1)
   elif address==0x400634:
    calls['equip']+=1;selection=bytes(uc.mem_read(char+0x3aa,1))[0];slot,index=cpu.reg(1),cpu.reg(2);assert cpu.reg(0)==char+0x37c and cpu.reg(3)==1;trace.append([6,selection,slot])
    if failure==5:raise Prefix('invalid equip boundary')
    active=selection if slot in [1,2] else 0;stored[index]['slots'][active]=slot;done()
   elif address==0x46a6b8:calls['completed']+=1
  handle=cpu.uc.hook_add(UC_HOOK_CODE,hook);failed=False
  try:cpu.invoke(CALLER,[stream,save])
  except Prefix:failed=True
  finally:cpu.uc.hook_del(handle)
  seen|=code_seen
  if pending:pending['identified']=bytes(cpu.uc.mem_read(pending['address']+0x68,1))[0]
  summary=[int(failed),at,struct.unpack('<I',cpu.uc.mem_read(char+0x39c,4))[0],bytes(cpu.uc.mem_read(char+0x3aa,1))[0],calls['ctor'],calls['value'],calls['identified'],calls['power'],calls['add'],calls['equip'],calls['completed'],int(pending is not None),len(stored)]
  records.append({'payload':blob,'failure':failure,'summary':summary,'items':stored+([pending] if pending else []),'private':case_number==len(cases)-1})
 assert not cpu.import_calls
 out.mkdir(parents=True,exist_ok=True);binary=struct.pack('<II',0x31495653,len(records))
 for record in records:
  binary+=struct.pack('<II',len(record['payload']),record['failure'])+record['payload']+struct.pack('<13I',*record['summary'])
  for item in record['items']:
   binary+=struct.pack('<7I',item['id'],item['quantity'],item['value'],item['identified'],*item['slots'],len(item['powers']))+b''.join(struct.pack('<I',p) for p in item['powers'])
 # Payloads stay in the private host-output directory, never in repository reports.
 (out/'original-cases.bin').write_bytes(binary);(out/'original-functions.json').write_text(json.dumps(manifest,indent=2)+'\n')
 report={'validation':'PASS','original_sha256':ELF_SHA,'cases':len(records),'synthetic_cases':len(records)-1,'injected_prefix_cases':sum(bool(r['failure']) for r in records),'distinct_pinned_words':len(seen),'dynamic_ranges':['0x46a3a0 caller','0x3fbc58 SetValue'], 'private_payload':private,'private_payload_exported_to_repository':False,'synthetic_case_summaries':[r['summary'] for r in records if not r['private']],'scope':manifest['scope']}
 (out/'original-capture.json').write_text(json.dumps(report,indent=2)+'\n');return report
if __name__=='__main__':
 import argparse
 p=argparse.ArgumentParser()
 for n in ['original-elf','cache','profile','output']:p.add_argument('--'+n,type=Path,required=True)
 a=p.parse_args();r=capture(a.original_elf.resolve(),a.cache.resolve(),a.profile.resolve(),a.output.resolve());print(json.dumps(r))
