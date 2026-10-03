"""Actual equipment/AI radius/ranged parameter and geometric bodies vs optimized ARM64."""
import argparse,hashlib,itertools,json,math,random,struct,sys
from pathlib import Path
from unicorn import UC_HOOK_CODE
ROOT=Path(__file__).resolve().parents[1];REPO=ROOT.parents[1]
sys.path.insert(0,str(REPO/'port/game-data/tests'));sys.path.insert(0,str(ROOT/'tests'))
from items_differential import Original
from navigation_differential import Cpu
from prince_attack_services_discovery import words,fbits,value
REF=ROOT/'reference/prince-live-attack-services'
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def same(a,b):return a==b or math.isnan(value(a)) and math.isnan(value(b))
def main():
 p=argparse.ArgumentParser();p.add_argument('--engine',type=Path,required=True);p.add_argument('--library',type=Path,required=True);p.add_argument('--report',type=Path,required=True);p.add_argument('--reference-output',type=Path,required=True);a=p.parse_args()
 manifest={'original_sha256':sha(a.engine),'functions':[f for d in ('reference','helpers','leaf','reader','producer','equipment') for f in json.loads((REF/d/'original-functions.json').read_text())['functions']]};assert manifest['original_sha256']=='36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80'
 old=Original(a.engine,manifest);new=Cpu(a.library,True,{'functions':[]});w=old.word
 goldpath=REPO/'port/game-data/reference/items/record-fixtures.bin';blob=goldpath.read_bytes();_,total,count,*_=struct.unpack_from('<7I',blob);at=28;rows=[]
 for i in range(total):
  size=struct.unpack_from('<I',blob,at)[0];at+=4+size;row=blob[at:at+164];at+=164
  for j in range(2):size=struct.unpack_from('<I',blob,at)[0];at+=4+size
  if i<count:rows.append(row)
 oo=old.data+0x10000;oa=oo+0x3c8;ot=old.data+0x14000;oi=oo+0x37c;os=old.data+0x18000;on=os+0x100;orf=os+0x200;oin=os+0x300;out=os+0x400;items=old.data+0x20000;av=old.data+0x70000
 old.uc.mem_write(oo,bytes(0x1800));old.uc.mem_write(ot,bytes(0x1800));old.pointer(oi,0x967768);old.pointer(oi+0x14,os);old.pointer(os,on);old.pointer(orf,oin);old.pointer(oa+4,oo);old.uc.mem_write(items,b''.join(rows));ptr=os+0x800;got=0x3f9e1c+w(0x3f9e2c);old.pointer(got+w(0x3f9e30),ptr);old.pointer(ptr,items)
 aipath=REPO/'port/android-native/app/src/main/assets/data/ai_pyarray.bin';old.blob=aipath.read_bytes();old.cursor=4;acount=struct.unpack_from('<I',old.blob)[0];radii=[]
 for i in range(acount):old.uc.mem_write(av+i*68,bytes(68));old.invoke(0x506f3c,[av+i*68,old.stream]);radii.append(w(av+i*68+32))
 assert old.cursor==len(old.blob);got=0x3d4c68+w(0x3d4c94);aptr=os+0x900;old.pointer(got+w(0x3d4c98),aptr);old.pointer(aptr,av);got=0x3a3000+w(0x3a301c);cp=os+0x910;old.pointer(got+w(0x3a3020),cp);old.pointer(cp,acount)
 np=new.data+0x10000;ni=np+0x1000;ns=ni+0x100;nr=ni+0x200;nn=ni+0x300;no=ni+0x400;nm=new.data+0x20000;na=new.data+0x70000;points=new.data+0x80000
 new.uc.mem_write(nm,b''.join(rows));new.uc.mem_write(ni,struct.pack('<QIi',ns,1,0));new.uc.mem_write(ns,struct.pack('<Q',nr));new.uc.mem_write(nr,struct.pack('<Q',nn));new.uc.mem_write(na,words(*radii));new.uc.mem_write(np,bytes(896))
 vt=old.data+0x8000;cb=[old.data+0x9000+j*16 for j in range(2)];old.pointer(oo,vt);old.pointer(ot,vt);old.pointer(vt+0x90,cb[0]);old.pointer(vt+0x128,cb[1]);current=[]
 for address in cb:old.uc.mem_write(address,bytes.fromhex('1eff2fe1'))
 def ret(v=0):old.put(0,v);old.uc.reg_write(old.pc,old.uc.reg_read(old.lr))
 def hook(uc,address,size,unused):
  if address==0x33ff8c:ret(ot)
  elif address==cb[0]:ret(8)
  elif address==cb[1]:uc.mem_write(old.reg(1),words(current[14]));uc.mem_write(old.reg(2),words(current[15]));uc.mem_write(old.reg(3),words(-1));ret(1)
  elif address==0x3d4c34 and current[0]==3:ret(current[12] if old.reg(0)==oa else current[13])
  elif address in (0x337888,0x3140ec,0x318254,0x708f00,0x310440):ret()
  elif address==0x337a88:ret(0)
 old.uc.hook_add(UC_HOOK_CODE,hook)
 cases=[]
 for kind in (0,1,2):
  for item in range(-1,count):
   f=[0]*19;f[0:6]=[kind,item,44,-1,0,0];f[16:19]=[0x12345678,0x23456789,0x3456789a];cases.append(f)
 ints=(-2147483648,-16777217,-257,-256,-255,-1,0,1,255,256,257,16777217,2147483647)
 for lo,hi,proj in itertools.product(ints,ints,(-2,0,1,2147483647)):
  f=[0]*19;f[0:6]=[1,-1,44,proj,lo,hi];f[16:19]=[0x12345678,0x23456789,0x3456789a];cases.append(f)
 for ai,radius,weapon in itertools.product((-1,8,44,acount),(0,0x80000000,fbits(-200),0x7f800000,0xff800000,0x7fc12345,fbits(200)),ints):
  f=[0]*19;f[0:6]=[2,0,ai,-1,0,0];f[12:16]=[radius,1,weapon,1];f[16]=0x12345678;cases.append(f)
 boundaries=(0,0x80000000,1,fbits(49.5),fbits(50),fbits(99.5),fbits(100),fbits(100.5),fbits(-100),0x7f800000,0xff800000,0x7fc12345)
 for kind,x,y in itertools.product((3,4),boundaries,boundaries):
  f=[0]*19;f[0]=kind;f[9:12]=[x,y,0];f[12:16]=[fbits(60),fbits(40),50,100];cases.append(f)
 rng=random.Random(20261006)
 for i in range(4096):
  f=[0]*19;f[0]=3+i%2;f[6:14]=[rng.getrandbits(32) if i<1024 else fbits(rng.uniform(-5000,5000)) for _ in range(8)];f[14:16]=[rng.choice(ints+(32768,65536,65537,-32768)) for _ in range(2)];cases.append(f)
 records=[];totals=[0]*5;nan_equivalent=0
 for index,current in enumerate(cases):
  f=current;kind,item,ai,proj,lo,hi=f[:6];old.pointer(on+4,0 if item<0 else orf);old.pointer(oin+4,max(item,0));new.uc.mem_write(ns,struct.pack('<Q',0 if item<0 else nr));new.uc.mem_write(nn,words(max(item,0)));old.uc.mem_write(oo+0xffc,words(ai));old.uc.mem_write(oo+0x1070,words(lo,hi,proj));new.uc.mem_write(np+4,words(ai));new.uc.mem_write(np+120,words(lo,hi,proj));old.uc.mem_write(out,words(*f[16:19]));new.uc.mem_write(no,words(*f[16:19]));old.uc.mem_write(oo+0x160,words(*f[6:9]));old.uc.mem_write(ot+0x160,words(*f[9:12]));new.uc.mem_write(points,words(*f[6:16]))
  selected=ai if 0<=ai<acount else 8
  if kind==2 and f[13]:old.uc.mem_write(av+selected*68+32,words(f[12]));new.uc.mem_write(na+selected*4,words(f[12]))
  if kind==2 and f[15]:old.uc.mem_write(items+item*164+156,words(f[14]));new.uc.mem_write(nm+item*164+156,words(f[14]))
  original=old.invoke((0x3fff30,0x3a4cd0,0x3d4c34,0x3d6188,0x3d6604)[kind],[oi,out] if kind==0 else [oo,out,out+4,out+8] if kind==1 else [oa] if kind==2 else [oa,ot]);oldout=struct.unpack('<3I',old.uc.mem_read(out,12))
  if kind==2:oldout=(original,*oldout[1:]);original=0
  entry=('dh2_attack_equipment_melee_radius','dh2_attack_range_parameters','dh2_attack_melee_radius','dh2_attack_melee_distance','dh2_attack_ranged_distance')[kind]
  args=[no,ni,nm,count] if kind==0 else [no,np,ni,nm,count] if kind==1 else [no,np,ni,nm,count,na,acount] if kind==2 else [points,points+12,points+24] if kind==3 else [points,points+12,points+32]
  result=new.invoke(entry,args);nativeout=struct.unpack('<3I',new.uc.mem_read(no,12));assert result==original,(index,f,result,original)
  assert all(same(x,y) if kind==2 and j==0 else x==y for j,(x,y) in enumerate(zip(oldout,nativeout))),(index,f,oldout,nativeout)
  nan_equivalent+=oldout[0]!=nativeout[0];records.append(words(*f,original,*oldout));totals[kind]+=1
  if kind==2 and f[13]:old.uc.mem_write(av+selected*68+32,words(radii[selected]));new.uc.mem_write(na+selected*4,words(radii[selected]))
  if kind==2 and f[15]:old.uc.mem_write(items+item*164+156,rows[item][156:160]);new.uc.mem_write(nm+item*164+156,rows[item][156:160])
 gold=b'AGQ1'+words(len(records),count,acount,23)+b''.join(rows)+words(*radii)+b''.join(records);a.reference_output.parent.mkdir(parents=True,exist_ok=True);a.reference_output.write_bytes(gold)
 report={'validation':'PASS','comparisons':len(records),'operation_counts':totals,'mismatches':0,'nan_classification_equivalence':nan_equivalent,'original_sha256':sha(a.engine),'arm64_sha256':sha(a.library),'corpus_sha256':hashlib.sha256(gold).hexdigest(),'item_corpus_sha256':sha(goldpath),'ai_asset_sha256':sha(aipath),'script_sha256':sha(Path(__file__)),'source_sha256':{p.name:sha(p) for p in (ROOT/'character_attack_geometry.hpp',ROOT/'character_attack_geometry.cpp')},'scope':__doc__,'geometry_handle_radius_parameter_services_explicit':True}
 a.report.parent.mkdir(parents=True,exist_ok=True);a.report.write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(report))
if __name__=='__main__':main()
