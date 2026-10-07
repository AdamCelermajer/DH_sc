"""Full original attack step consumers versus native source; backend boundaries
are explicit fixtures, including target/last mutation during virtual calls.
This proves the two methods and call ordering, not end-to-end game combat.
"""
import argparse,ctypes,hashlib,itertools,json,random,struct,sys
from pathlib import Path
ROOT=Path(__file__).resolve().parents[1]
def digest(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def original(out):
 sys.path.insert(0,r'C:\Users\adamc\AppData\Local\uv\cache\archive-v0\jc8RoeQ1US_9Gkmi\Lib\site-packages')
 from unicorn import UC_HOOK_CODE
 sys.path.insert(0,str(ROOT/'tests'))
 from navigation_differential import Cpu
 elf=ROOT/'../../.local-inputs/libDungeonHunter2.so'
 c=Cpu(elf,False,{'functions':[]});ai=c.data+0x1000;owner=c.data+0x2000
 vt=c.data+0x5000;target=c.data+0x6000;tv=c.data+0x7000
 def word(p):return struct.unpack('<I',c.uc.mem_read(p,4))[0]
 def write(p,n):c.uc.mem_write(p,struct.pack('<I',n&0xffffffff))
 write(ai,vt);write(ai+4,owner);write(owner,vt);write(target,tv)
 for offset,address in ((0xa4,0x3c0000),(0x124,0x3c0004)):write(vt+offset,address)
 write(tv+0x34,0x3c0008)
 trace=[];case=[]
 def snapshot():return [c.uc.mem_read(ai+0x78,1)[0],c.uc.mem_read(ai+0x79,1)[0],c.uc.mem_read(ai+0x7a,1)[0],word(ai+0x74),int(bool(word(ai+0x40)))]
 def hook(uc,address,size,unused):
  mapping={0x3c932c:0,0x3c934c:1,0x3a4d5c:2,0x4052bc:3,0x3c0000:4,
   0x3a346c:5,0x3d8d70:6,0x3c9484:7,0x3c0004:8,0x3c0008:9,0x3c9464:10}
  if address not in mapping:return
  k=mapping[address];value=subject=0;ret=0
  if k==0:ret=case[2]
  elif k==1:ret=case[3]
  elif k==2:value=c.reg(1);subject=c.reg(2)
  elif k==3:subject=2;c.uc.mem_write(ai+0x79,bytes([case[14]])) if case[13] else None
  elif k==4:value=c.reg(1)
  elif k==5:ret=case[10]
  elif k==6:write(ai+0x40,0) # fixture for genuine reached clear-nonsticky body
  elif k==7:value=c.reg(1)
  elif k==8:ret=case[11]
  elif k==9:
   subject=1;ret=case[12]
   if case[15]:write(ai+0x40,0)
  trace.append([k,value,subject,snapshot()]);c.put(0,ret);uc.reg_write(c.pc,uc.reg_read(c.lr))
 c.uc.hook_add(UC_HOOK_CODE,hook)
 cases=[]
 for op,phase,step,count,continued,target_on,combo in itertools.product(range(2),range(3),range(3),(0,1,2,3),range(2),range(2),range(2)):
  cases.append([op,phase,step,count,continued,173,219,0x87654321,target_on,8,combo,0,0,0,0,0])
 rng=random.Random(20261005)
 for _ in range(800):cases.append([rng.randrange(2),rng.choice([0,1,2,0xffffffff]),rng.choice([0,1,2,3,0xffffffff]),rng.choice([0,1,2,3,4,0xffffffff]),rng.choice([0,1,2,255]),rng.randrange(256),rng.randrange(256),rng.getrandbits(32),rng.randrange(2),rng.choice([-1,0,8]),rng.randrange(2),rng.randrange(2),rng.randrange(2),rng.randrange(2),rng.randrange(256),rng.randrange(2)])
 rows=[]
 for case in cases:
  c.uc.mem_write(ai+0x74,struct.pack('<I3B',case[7],case[4],case[5],case[6]))
  write(ai+0x40,target if case[8] else 0);write(owner+0x4c8,case[1]);write(owner+0x408,2);write(owner+0x378,owner+0x800)
  c.uc.mem_write(owner+0x14a8,bytes([case[9]&255]));trace.clear()
  symbol='_ZN6CharAI23_OnAnimStepBegin_AttackEv' if case[0]==0 else '_ZN6CharAI21_OnAnimStepEnd_AttackEv'
  result=c.invoke(symbol,[ai]);assert result==1
  rows.append({'in':case,'out':snapshot(),'calls':trace.copy()})
 out.write_text(json.dumps({'original_sha256':digest(elf),'rows':rows},separators=(',',':')))
 binary=bytearray(struct.pack('<II',0x31414143,len(rows)))
 for row in rows:
  binary+=struct.pack('<16I',*[v&0xffffffff for v in row['in']])
  binary+=struct.pack('<6I',*row['out'],len(row['calls']))
  for k,value,subject,fields in row['calls']:binary+=struct.pack('<8I',k,value,subject,*fields)
 out.with_suffix('.bin').write_bytes(binary)
 print(f'Original attack animator: {len(rows)} cases captured')
def native(library,reference,report):
 class AI(ctypes.Structure):
  _fields_=[(n,ctypes.c_uint64) for n in ('owner','target','last_target','object_of_interest')]+[(n,ctypes.c_uint32) for n in ('flags','heading','continued','last','index','ooi','finisher','seeking')]
 class Request(ctypes.Structure):_fields_=[('service',ctypes.c_uint32),('value',ctypes.c_uint32),('subject',ctypes.c_uint64)]
 class Borrow(ctypes.Structure):_fields_=[('ai',ctypes.POINTER(AI)),('phase',ctypes.POINTER(ctypes.c_uint32)),('look',ctypes.POINTER(ctypes.c_uint64))]
 CB=ctypes.CFUNCTYPE(ctypes.c_int,ctypes.c_void_p,ctypes.POINTER(AI),ctypes.POINTER(Request),ctypes.POINTER(ctypes.c_uint32))
 class Services(ctypes.Structure):_fields_=[('context',ctypes.c_void_p),('invoke',CB)]
 lib=ctypes.CDLL(str(library.resolve()));data=json.loads(reference.read_text());trace=[];case=[]
 def snapshot(a):return [a.continued,a.last,a.finisher,a.index,int(bool(a.target))]
 def call(context,ap,qp,result):
  a=ap.contents;q=qp.contents;k=q.service;ret=0
  if k==0:ret=case[2]
  elif k==1:ret=case[3]
  elif k==3:
   if case[13]:a.last=case[14]
  elif k==5:ret=case[10]
  elif k==6:a.target=0
  elif k==8:ret=case[11]
  elif k==9:
   ret=case[12]
   if case[15]:a.target=0
  trace.append([k,q.value,q.subject,snapshot(a)]);result[0]=ret;return 0
 cb=CB(call);s=Services(None,cb)
 for name in ('begin','end'):
  f=getattr(lib,'dh2_character_attack_animation_'+name+'_v1');f.argtypes=[ctypes.POINTER(Borrow),ctypes.POINTER(Services)];f.restype=ctypes.c_int
 callbacks=0
 for i,row in enumerate(data['rows']):
  case=row['in'];a=AI(0x100000001,case[8],0,0,0,0,case[4],case[5],case[7],case[9]&0xffffffff,case[6],0)
  phase=ctypes.c_uint32(case[1]);look=ctypes.c_uint64(2);b=Borrow(ctypes.pointer(a),ctypes.pointer(phase),ctypes.pointer(look));trace.clear()
  f=lib.dh2_character_attack_animation_begin_v1 if case[0]==0 else lib.dh2_character_attack_animation_end_v1
  assert f(ctypes.byref(b),ctypes.byref(s))==1
  assert snapshot(a)==row['out'],(i,case,snapshot(a),row['out'])
  assert trace==row['calls'],(i,case,trace,row['calls']);callbacks+=len(trace)
 result={'validation':'PASS','original_cases':len(data['rows']),'ordered_callbacks':callbacks,'mismatches':0,'original_sha256':data['original_sha256'],'reference_sha256':digest(reference),'library_sha256':digest(library),'sources':{p.name:digest(p) for p in (ROOT/'character_attack_animation_v1.cpp',ROOT/'character_attack_animation_v1.hpp',Path(__file__))},'scope':__doc__}
 report.write_text(json.dumps(result,indent=2)+'\n');print(json.dumps(result))
if __name__=='__main__':
 p=argparse.ArgumentParser();p.add_argument('--reference',type=Path,required=True);p.add_argument('--library',type=Path);p.add_argument('--report',type=Path);a=p.parse_args()
 if a.library:native(a.library,a.reference,a.report)
 else:original(a.reference)
