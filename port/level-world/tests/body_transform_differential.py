"""Original SetXForm instructions versus ARM64; shape/broadphase callbacks are controlled fixtures."""
import argparse,hashlib,json,math,random,struct,sys,time
from pathlib import Path
from unicorn import UC_HOOK_CODE
from unicorn.arm64_const import UC_ARM64_REG_S0
ROOT=Path(__file__).resolve().parents[1]
sys.path.insert(0,str(ROOT/'../game-data/tests'))
from aggro_differential import Cpu as Base,float_bits
from combat_result_differential import floating

class Cpu(Base):
 def external(self,uc,address,size,unused):
  name=self.imports.get(address)
  if name=='body_callback':return self.body_callback(uc,address,size,unused)
  if name in ('sinf','cosf'):
   value=floating(uc.reg_read(UC_ARM64_REG_S0) if self.arm64 else self.reg(0))
   result=float_bits((math.sin if name=='sinf' else math.cos)(value)) if math.isfinite(value) else 0x7fc00000
   if self.arm64:uc.reg_write(UC_ARM64_REG_S0,result)
   else:self.put(0,result)
  elif name=='__aeabi_fmul':self.put(0,float_bits(floating(self.reg(0))*floating(self.reg(1))))
  else:return super().external(uc,address,size,unused)
  self.import_calls[name]=self.import_calls.get(name,0)+1;uc.reg_write(self.pc,uc.reg_read(self.lr))

def equal(a,b):
 return len(a)==len(b) and all(x==y or math.isnan(floating(x)) and math.isnan(floating(y)) for x,y in zip(struct.unpack('<'+'I'*(len(a)//4),a),struct.unpack('<'+'I'*(len(b)//4),b)))

def main():
 p=argparse.ArgumentParser();p.add_argument('--engine',type=Path,required=True);p.add_argument('--library',type=Path,required=True);p.add_argument('--reference-output',type=Path,required=True);p.add_argument('--report',type=Path,required=True);p.add_argument('--cases',type=int,default=4000);a=p.parse_args();started=time.monotonic()
 manifest=json.loads((ROOT/'reference/body-transform/original-functions.json').read_text());assert hashlib.sha256(a.engine.read_bytes()).hexdigest()==manifest['original_sha256']
 old=Cpu(a.engine,False,manifest);new=Cpu(a.library,True,{'functions':[]});rng=random.Random(20261013)
 ob=old.data+0x1000;ow=old.data+0x2000;op=old.data+0x20000;os=old.data+0x21000;bp=old.data+0x22000
 nb=new.data+0x1000;ns=new.data+0x1100;nw=new.data+0x1200;nc=new.data+0x1300;np=new.data+0x1400;sh=new.data+0x2000
 context=0xabcdef0123456789;native_bp=0xb23456789abcdef0
 events=[[],[]];fail=-1;counts={'locked':0,'frozen':0,'committed':0,'failed':0,'synchronized':0,'destroyed':0};records=[]
 def word(at):return struct.unpack('<I',old.uc.mem_read(at,4))[0]
 def original_state():
  return struct.pack('<I',struct.unpack('<H',old.uc.mem_read(ob,2))[0])+bytes(old.uc.mem_read(ob+4,8))+bytes(old.uc.mem_read(ob+0x38,4))+bytes(old.uc.mem_read(ob+0x40,24))+bytes(old.uc.mem_read(ob+0x8c,4))+bytes(old.uc.mem_read(ob+0x90,4))
 def original_extra():return bytes(old.uc.mem_read(ob+0xc,16))+bytes(old.uc.mem_read(ob+0x1c,8))+bytes(old.uc.mem_read(ob+0x2c,8))+bytes(old.uc.mem_read(ob+0x24,8))+bytes(old.uc.mem_read(ob+0x34,4))+bytes(4)
 def return_old(v=0):old.put(0,v);old.uc.reg_write(old.pc,old.uc.reg_read(old.lr))
 def old_hook(uc,address,size,unused):
  if address not in (0x7e63d0,0x7e6180,0x7e2ac8):return
  if address==0x7e2ac8:
   assert old.reg(0)==bp;events[0].append((3,0,b''));return_old();return
  ident=(old.reg(0)-os)//32;assert old.reg(0)==os+ident*32 and old.reg(1)==bp
  if address==0x7e63d0:
   assert old.reg(2)==old.reg(3)==ob+4
   events[0].append((1,ident,bytes(uc.mem_read(ob+4,24))));return_old(int(ident!=fail))
  else:events[0].append((2,ident,b''));return_old()
 def new_hook(uc,address,size,unused):
  if address not in (new.callback+32,new.callback+48,new.callback+64):return
  assert new.reg(0)==context
  if address==new.callback+64:
   assert new.reg(1)==native_bp;events[1].append((3,0,b''))
  else:
   ident=new.reg(1)-0x100000000;assert new.reg(2)==native_bp
   if address==new.callback+32:
    assert new.reg(3)==new.reg(4);events[1].append((1,ident,bytes(uc.mem_read(new.reg(3),24))));new.put(0,int(ident!=fail))
   else:events[1].append((2,ident,b''))
  uc.reg_write(new.pc,uc.reg_read(new.lr))
 old.uc.hook_add(UC_HOOK_CODE,old_hook)
 for address in (new.callback+32,new.callback+48,new.callback+64):new.imports[address]='body_callback'
 new.body_callback=new_hook
 new.uc.mem_write(nc,struct.pack('<3Q',new.callback+32,new.callback+48,new.callback+64))
 def compare(flags,locked,count,failed,state,extra,request,label):
  nonlocal fail
  fail=failed;events[0].clear();events[1].clear();old.uc.mem_write(ob,bytes(0x100));old.uc.mem_write(ow,bytes(0x19200));old.uc.mem_write(ob,struct.pack('<H',flags));old.uc.mem_write(ob+4,state[4:12]);old.uc.mem_write(ob+0x38,state[12:16]);old.uc.mem_write(ob+0x40,state[16:40]);old.uc.mem_write(ob+0x8c,state[40:44]);old.uc.mem_write(ob+0x90,state[44:48]);old.uc.mem_write(ob+0xc,extra[:24]);old.uc.mem_write(ob+0x2c,extra[24:32]);old.uc.mem_write(ob+0x24,extra[32:40]);old.uc.mem_write(ob+0x34,extra[40:44]);old.pointer(ob+0x58,ow);old.pointer(ob+0x64,os if count else 0);old.pointer(ow+0x191d8,bp);old.uc.mem_write(ow+0x191d4,bytes((locked,)));old.uc.mem_write(op,request[:8])
  for i in range(count):old.pointer(os+i*32+8,os+(i+1)*32 if i+1<count else 0);new.uc.mem_write(sh+i*16,struct.pack('<QQ',0x100000000+i,sh+(i+1)*16 if i+1<count else 0))
  new.uc.mem_write(ns,state);new.uc.mem_write(nb,struct.pack('<Q',ns)+extra);new.uc.mem_write(nw,struct.pack('<QQQB7x',context,native_bp,sh if count else 0,locked));new.uc.mem_write(np,request)
  result=old.invoke(0x7e164c,[ob,op,struct.unpack_from('<I',request,8)[0]]);actual=new.invoke('dh2_body_set_transform',[nb,np,nw,nc]);assert result==actual,(label,'return',result,actual)
  expected_state=original_state();expected_extra=original_extra();assert equal(expected_state,bytes(new.uc.mem_read(ns,48))),(label,'state',expected_state.hex(),bytes(new.uc.mem_read(ns,48)).hex());assert equal(expected_extra,bytes(new.uc.mem_read(nb+8,48))),(label,'transform',expected_extra.hex(),bytes(new.uc.mem_read(nb+8,48)).hex())
  assert len(events[0])==len(events[1]) and all(e[:2]==n[:2] and equal(e[2],n[2]) for e,n in zip(*events)),(label,'events',events)
  if locked:counts['locked']+=1
  elif flags&2:counts['frozen']+=1
  elif failed>=0 and failed<count:counts['failed']+=1
  else:counts['committed']+=1
  counts['synchronized']+=sum(e[0]==1 for e in events[0]);counts['destroyed']+=sum(e[0]==2 for e in events[0])
  records.append(struct.pack('<4IiI',flags,locked,count,len(events[0]),failed,result)+state+extra+request+expected_state+expected_extra+b''.join(struct.pack('<II',kind,ident)+payload+bytes(24-len(payload)) for kind,ident,payload in events[0]))
 specials=(0,0x80000000,1,0x80000001,0x007fffff,0x00800000,0x7f7fffff,0xff7fffff,0x7f800000,0xff800000,0x7fc01234,0xffc01234,0x7f801234)
 def fixture(flags,angle=None,boundary=False):
  words=[rng.choice(specials) if boundary else float_bits(rng.uniform(-2000,2000)) for _ in range(23)]
  state=struct.pack('<I11I',flags,*words[:11]);extra=struct.pack('<11I',*words[11:22])+bytes(4);request=struct.pack('<3II',words[22],rng.choice(specials) if boundary else float_bits(rng.uniform(-2000,2000)),float_bits(rng.uniform(-10,10)) if angle is None else angle,1);return state,extra,request
 for flags in (0,2,8,0xfffd,0xffff):
  for locked in (0,1):
   for count in (0,1,4):
    for failed in range(-1,count):compare(flags,locked,count,failed,*fixture(flags),('gates',flags,locked,count,failed))
 for angle in specials+(float_bits(math.pi/2),float_bits(math.pi),float_bits(-math.pi/2)):
  for count in range(5):
   for failed in range(-1,count):compare(0,0,count,failed,*fixture(0,angle,True),('boundary',angle,count,failed))
 for i in range(a.cases):
  flags=rng.choice((0,0,0,8,2,0xfffd));locked=int(i%29==0);count=rng.randrange(9);failed=rng.randrange(-1,count) if count else -1;compare(flags,locked,count,failed,*fixture(flags,boundary=i%4==0),('random',i))
 # Native argument guards only: original trusts its allocations.
 assert new.invoke('dh2_body_set_transform',[0,np,nw,nc])&0xffffffff==0xffffffff
 reference=struct.pack('<II',0x31544642,len(records))+b''.join(records);a.reference_output.parent.mkdir(parents=True,exist_ok=True);a.reference_output.write_bytes(reference)
 coverage={row['original_symbol']:{'instructions':row['size']//4,'seen':sum(int(row['elf_address'],16)<=v<int(row['elf_address'],16)+row['size'] for v in old.seen)} for row in manifest['functions']}
 report={'original_sha256':manifest['original_sha256'],'arm64_library_sha256':hashlib.sha256(a.library.read_bytes()).hexdigest(),'reference_sha256':hashlib.sha256(reference).hexdigest(),'comparisons':len(records),'mismatches':0,'counts':counts,'coverage':coverage,'original_import_calls':old.import_calls,'native_import_calls':new.import_calls,'arm64_callback_context_and_shape_identity_above_4gib':True,'same_new_transform_pointer_for_both_sync_args':True,'scope':__doc__+' Original shape Synchronize/DestroyProxy and broadphase Commit backends are supplied callback fixtures; collision, contacts and world stepping remain unreconstructed. sinf/cosf are identically modeled imported services; finite arithmetic and signed zero compare exactly, arithmetic NaN payload/sign by class.','elapsed_seconds':round(time.monotonic()-started,2)};a.report.parent.mkdir(parents=True,exist_ok=True);a.report.write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(report))
if __name__=='__main__':main()
