import pathlib,sys,struct,json,hashlib,argparse,itertools
R=pathlib.Path(__file__).resolve().parents[3];sys.path.insert(0,str(R/'port/engine-ui/tests'))
from gfnt_differential import Cpu
from unicorn import UC_HOOK_CODE
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest()
def main():
 ap=argparse.ArgumentParser();ap.add_argument('--oracle',type=pathlib.Path,required=True);ap.add_argument('--output',type=pathlib.Path,required=True);a=ap.parse_args()
 manifestpath=R/'port/level-world/reference/font-text/glyph-backend/original-functions.json';manifest=json.loads(manifestpath.read_text());old=Cpu(R/'.local-inputs/libDungeonHunter2.so',False,manifest);new=Cpu(a.oracle,True,{'functions':[]})
 d=old.data;of=d+0x1000;og=d+0x2000;owner=d+0x3000;weak=d+0x4000;ctx=d+0x5000;table=d+0x6000;adv=d+0x7000
 nd=new.data;nf=nd+0x1000;ng=nd+0x2000;ns=nd+0x3000;nname=nd+0x4000;na=nd+0x7000;callback=nd+0x10000
 new.uc.mem_write(nname,b'Fontin SmallCaps\0');new.uc.mem_write(ns,struct.pack('<QQ',0xf123456789abcdef,callback));current=None;oc=[];nc=[]
 handles={1:0x1111,2:0x2222,3:0x3333,4:0x4444}
 def word(p,c):return struct.unpack('<I',c.uc.mem_read(p,4))[0]
 def outputwords(c,p,native):
  if native:
   b=bytes(c.uc.mem_read(p,48));v=list(struct.unpack('<6I2Q2I',b));return v[:6]+[v[6]&0xffffffff,v[7]&0xffffffff,v[8]]
  b=bytes(c.uc.mem_read(p,36));return [word(p,c),*[word(p+8+i*4,c) for i in range(4)],struct.unpack('<h',b[30:32])[0]&0xffffffff,word(p+4,c),word(p+24,c),b[34]]
 def orig(uc,address,size,unused):
  addresses={0x7c6048:1,0x7c59dc:2,0x7d1614:3,0x7d113c:4,0x77a740:0}
  if address not in addresses:return
  k=addresses[address];args=[old.reg(i) for i in range(4)]
  if not k:old.pointer(args[0],args[1]);old.put(0,args[0])
  else:
   oc.append(k)
   if k==1:old.put(0,handles[1] if current[0]&2 else 0)
   elif k in (2,3):
    if k==2:bounds=args[3];advance=word(old.uc.reg_read(old.sp),old)
    else:bounds=word(old.uc.reg_read(old.sp)+8,old);advance=word(old.uc.reg_read(old.sp)+12,old)
    old.uc.mem_write(bounds,struct.pack('<4I',0x80000000,0x3f000000,0xbf800000,0x7fc01234));old.pointer(advance,current[4]);old.put(0,handles[k] if current[0]&(4 if k==2 else 16) else 0)
   else:old.put(0,handles[4] if current[0]&32 else 0)
  old.uc.reg_write(old.pc,old.uc.reg_read(old.lr))
 old.uc.hook_add(UC_HOOK_CODE,orig)
 def native(uc,address,size,unused):
  if address!=callback:return
  assert new.reg(0)==0xf123456789abcdef;p=new.reg(1);k,res,identity,g,font,index,found,tail=struct.unpack('<IIQQQiIQ',new.uc.mem_read(p,48));assert res==tail==0;nc.append(k)
  if k==1:identity=handles[1] if current[0]&2 else 0
  elif k in (2,3):
   new.uc.mem_write(g+4,struct.pack('<4I',0x80000000,0x3f000000,0xbf800000,0x7fc01234))
   new.uc.mem_write(g,struct.pack('<I',current[4]));identity=handles[k] if current[0]&(4 if k==2 else 16) else 0
  elif k==4:identity=handles[4] if current[0]&32 else 0
  else:found=int(current[1]!=0);index=2 if found else -1
  new.uc.mem_write(p+8,struct.pack('<Q',identity));new.uc.mem_write(p+32,struct.pack('<iI',index,found));new.put(0,0);new.uc.reg_write(new.pc,new.uc.reg_read(new.lr))
 new.uc.hook_add(UC_HOOK_CODE,native)
 blob=bytearray(struct.pack('<II',0x31594c47,0));cases=0;requests=0;arithnan=0
 values=[0,0x80000000,0x3f800000,0xc1000000,0x7f7fffff,0x00800000,0x7f800000,0x7fc12345,0x7f812345]
 for flags,lookup,zones,count,w in itertools.product(range(64),range(3),(0,2),(0,4),values):
  current=(flags,lookup,zones,count,w);oc.clear();nc.clear();old.uc.mem_write(of,b'\0'*256);old.pointer(of+24,weak);old.pointer(of+28,owner);old.pointer(weak,2);old.uc.mem_write(weak+4,b'\x01');old.pointer(owner+0xac,ctx);old.pointer(ctx+0x10,0x100 if flags&1 else 0);old.pointer(ctx+0xc,0x200 if flags&8 else 0);old.pointer(of+0x50,table);old.pointer(of+0x60,adv);old.pointer(of+0x64,count);old.pointer(of+0x7c,zones);old.uc.mem_write(of+0x4c,b'\x01\x00');old.uc.mem_write(adv,struct.pack('<4I',0,0x44000000,w,0))
  old.uc.mem_write(table,struct.pack('<II',4,3)+struct.pack('<iiHH',-2,0,0,0)*4)
  if lookup==1:old.uc.mem_write(table+8+12,struct.pack('<iiHH',-1,65,65,2))
  elif lookup==2:old.uc.mem_write(table+8+12,struct.pack('<iiHH',3,69,69,0));old.uc.mem_write(table+8+36,struct.pack('<iiHH',-1,65,65,2))
  old.uc.mem_write(og,b'\xa5'*36);old.pointer(og+4,0x5555);old.pointer(og+24,0x6666)
  new.uc.mem_write(nf,struct.pack('<QIi6IQ',nname,65,16,0,1,int(bool(flags&1)),int(bool(flags&8)),zones,count,na));new.uc.mem_write(na,struct.pack('<4I',0,0x44000000,w,0));new.uc.mem_write(ng,struct.pack('<6I2Q2I',0xa5a5a5a5,*([0xa5a5a5a5]*4),0xa5a5a5a5,0x5555,0x6666,0xa5,0))
  rc=old.invoke(0x7d01bc,[of,og,65,16]);nr=new.invoke('dh2_swf_glyph_lookup',[ng,nf,ns]);ow=outputwords(old,og,False);nw=outputwords(new,ng,True)
  # Old hash lookup runs inline and is not an external service.
  expectedcalls=oc+([] if rc and (ow[5]==0xffffffff) else [5]);assert nc==expectedcalls,(current,rc,ow,oc,nc)
  assert rc==nr,(current,rc,nr)
  for j,(x,y) in enumerate(zip(ow,nw)):
   if j==0 and (x&0x7fffffff)>0x7f800000 and (y&0x7fffffff)>0x7f800000 and zones and rc and not (ow[5]!=0xffffffff and count):arithnan+=1;continue
   assert x==y,(current,j,hex(x),hex(y),oc,nc)
  blob.extend(struct.pack('<5I',*current));blob.extend(struct.pack('<I9I',rc,*ow));blob.extend(struct.pack('<I',len(nc)));blob.extend(struct.pack('<'+str(len(nc))+'I',*nc));cases+=1;requests+=len(nc)
 struct.pack_into('<I',blob,4,cases);P=R/'port/engine-ui/reference/swf-glyph-lookup';P.mkdir(parents=True,exist_ok=True);gold=P/'source-fixtures.bin';gold.write_bytes(blob)
 report={'validation':'PASS','comparisons':cases,'ordered_services':requests,'arithmetic_nan_class_cases':arithnan,'mismatches':0,'original_sha256':manifest['original_sha256'],'original_manifest_sha256':sha(manifestpath),'gold_sha256':sha(gold),'oracle_sha256':sha(a.oracle),'source_sha256':{str(p.relative_to(R)).replace('\\','/'):sha(p) for p in [R/'port/engine-ui/swf_glyph_lookup.hpp',R/'port/engine-ui/swf_glyph_lookup.cpp',pathlib.Path(__file__)]},'scope':'Actual original font.get_glyph including inline native code-hash empty/direct/collision lookup vs O2 ARM64 provider/advance projection. Provider calls and original smart-pointer assignment are explicit services, live weak-player valid fixture; source float32 mul imported helper modeled. Copied words/NaNs exact, arithmetic NaNs class-only. No weak-player destruction or original FT raster/bitmap allocation parity.'};a.output.write_text(json.dumps(report,indent=2)+'\n');print(json.dumps({k:report[k] for k in ('validation','comparisons','ordered_services','arithmetic_nan_class_cases')}))
if __name__=='__main__':main()
