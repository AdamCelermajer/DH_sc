"""Original scalar callbacks versus optimized native ARM64; explicit ABI/Lua services."""
import argparse,hashlib,json,random,struct,sys
from pathlib import Path
from unicorn import UC_HOOK_CODE
ROOT=Path(__file__).resolve().parents[3]
sys.path.insert(0,str(ROOT/'port/level-world/tests'))
from visual_timeline_differential import TimelineCpu
from unicorn.arm64_const import UC_ARM64_REG_S0
NAMES=['to_fixed','from_fixed','mul_fixed','div_fixed','bit_not','bit_and','bit_or','bit_xor']
ADDRESSES=[0x37ebc4,0x37ee84,0x37e1a4,0x37e068,0x37f814,0x37e9ec,0x37e814,0x37f750]
def words(*v):return struct.pack('<'+'I'*len(v),*(x&0xffffffff for x in v))
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def fw(f):return struct.unpack('<I',struct.pack('<f',f))[0]
def signed(v):return v if v<0x80000000 else v-0x100000000
class Cpu(TimelineCpu):
    def external(self,uc,address,size,unused):
        name=self.imports.get(address)
        if name=='__aeabi_idiv':
            a,b=signed(self.reg(0)),signed(self.reg(1))
            if b==0:self.handler('zero',a);v=0x13579bdf
            else:v=(abs(a)//abs(b))*(-1 if (a<0)!=(b<0) else 1)
            self.put(0,v)
        elif name=='__aeabi_ui2f':self.put(0,fw(float(self.reg(0))))
        elif name=='memset':
            a,b,n=[self.reg(i) for i in range(3)];uc.mem_write(a,bytes([b&255])*n);self.put(0,a)
        elif name=='memchr':
            a,b,n=[self.reg(i) for i in range(3)];i=bytes(uc.mem_read(a,n)).find(bytes([b&255]));self.put(0,a+i if i>=0 else 0)
        elif name=='luaL_newstate':self.put(0,self.data+0x50000)
        elif name=='lua_cpcall':
            self.handler('string',0);uc.mem_write(self.reg(2)+16,words(fw(1024.5)));self.put(0,0)
        elif name=='lua_close':self.put(0,0)
        else:return super().external(uc,address,size,unused)
        self.import_calls[name]=self.import_calls.get(name,0)+1;uc.reg_write(self.pc,uc.reg_read(self.lr))
class Machine:
    def __init__(self,path,native,manifest):
        self.c=Cpu(path,native,manifest);self.native=native;c=self.c;d=c.data;c.handler=self.service
        self.args=d+0x1000;self.vector=d+0x2000;self.records=d+0x3000
        self.results=d+0x10000;self.returned=d+0x11000;self.string=d+0x12000
        self.services=d+0x13000;self.identity=d+0x14000;self.zero=d+0x14100;self.error=d+0x15000
        c.uc.mem_write(self.string,b'1024.5\0');c.uc.mem_write(self.services,struct.pack('<4Q',0,self.identity,self.zero,0))
        if not native:c.pointer(self.args+4,self.vector)
        c.uc.hook_add(UC_HOOK_CODE,self.hook)
    def ret(self,v=0):
        c=self.c;c.put(0,v);c.uc.reg_write(c.pc,c.uc.reg_read(c.lr))
    def service(self,kind,value):self.calls.append((kind,value))
    def hook(self,uc,address,size,_):
        c=self.c
        if self.native:
            if address==self.identity:
                assert c.reg(1)==0xfedcba9876543210
                self.calls.append(('identity',self.pointer));uc.mem_write(c.reg(2),words(self.pointer));self.ret()
            elif address==self.zero:
                self.calls.append(('zero',signed(c.reg(1)&0xffffffff)));uc.mem_write(c.reg(2),words(0x13579bdf));self.ret()
        else:
            if address==0x37cb24:self.output.append(fw(float(signed(c.reg(1)))));self.ret()
            elif address==0x37ccbc:self.output.append(c.reg(1));self.ret()
            elif address==0x31bbf0:
                record=c.reg(0);kind=struct.unpack('<I',uc.mem_read(record+4,4))[0]
                if kind in (2,7) and self.pointer:self.calls.append(('identity',self.pointer))
            elif address==0x84c7e0:self.ret(c.data+0x50000)
            elif address==0x84c04c:self.ret()
            elif address==0x84c450:self.calls.append(('string',0));self.ret(fw(1024.5))
            elif address==0x85797c:self.ret()
    def execute(self,op,records,pointer=0):
        c=self.c;self.output=[];self.calls=[];self.pointer=pointer
        if self.native:
            raw=b''.join(words(k,0,p,b)+struct.pack('<QQQ',self.string if k==4 else 0,6 if k==4 else 0,0xfedcba9876543210 if k in (2,7) and pointer else 0) for k,p,b in records)
            if raw:c.uc.mem_write(self.records,raw)
            c.uc.mem_write(self.results,bytes(80));c.uc.mem_write(self.returned,words(0xdeadbeef))
            status=c.invoke('dh2_script_scalar_'+NAMES[op],[self.services,self.records,len(records),self.results,2,self.returned,self.error,256])
            assert status==0,(op,status)
            n=struct.unpack('<I',c.uc.mem_read(self.returned,4))[0];assert n<=2
            self.output=[struct.unpack('<I',c.uc.mem_read(self.results+40*i+8,4))[0] for i in range(n)]
        else:
            c.uc.mem_write(self.vector,words(self.records,self.records+112*len(records),self.records+112*len(records)))
            for i,(k,p,b) in enumerate(records):
                a=self.records+112*i;c.uc.mem_write(a,bytes(112));c.uc.mem_write(a+4,words(k,fw(float(b)) if k==1 else p));c.pointer(a+0x20,self.string);c.pointer(a+0x6c,pointer)
            c.invoke(ADDRESSES[op],[self.args,self.results,0])
        return self.output,self.calls
def main():
    ap=argparse.ArgumentParser();ap.add_argument('--library',type=Path,default=ROOT/'.local-inputs/lua514-source/scalar/libscript_scalar_oracle.so');a=ap.parse_args()
    ref=ROOT/'port/script-runtime/reference/scalar-bindings';ref.mkdir(parents=True,exist_ok=True)
    manifest=json.loads((ref/'original-functions.json').read_text());elf=ROOT/'.local-inputs/libDungeonHunter2.so';assert sha(elf)==manifest['original_sha256']
    old=Machine(elf,False,manifest);new=Machine(a.library,True,{'functions':[]});rows=[];counts=[0]*8;services=0
    def compare(op,records,p=0):
        nonlocal services
        expected=old.execute(op,records,p);actual=new.execute(op,records,p);assert expected==actual,(len(rows),op,records,p,expected,actual)
        counts[op]+=1;services+=len(expected[1]);rows.append(dict(op=op,arguments=records,identity32=p,output=expected[0],services=expected[1]))
    edges=[0,0x80000000,1,0x007fffff,0x3f000000,0xbf000000,0x3f7fffff,0x3f800000,0xbf800000,0x437fffff,0x43800000,0x43808000,0xc3808000,0x4affffff,0x4b800001,0x4effffff,0x4f000000,0xcf000000,0xcf000001,0x7f7fffff,0x7f800000,0xff800000,0x7fc12345,0x7f812345,0xffc12345]
    variants=[(k,fw(513.75),b) for k in range(9) for b in ((0,1) if k==1 else (0,))]
    for op in range(8):
        compare(op,[])
        for first in variants:
            for n in (1,2,3):
                for p in (0,0xfedcba98):compare(op,[first]+[(3,fw(256.0),0)]*(n-1),p)
        for bits in edges:
            compare(op,[(3,bits,0)])
            for second in edges:compare(op,[(3,bits,0),(3,second,0)])
    for op in (5,6):
        for n in (0,1,2,3,16,17,33,65):
            records=[(3,edges[i%len(edges)],0) for i in range(n)];compare(op,records)
            for bad in range(n):
                r=records.copy();r[bad]=(4,0,0);compare(op,r)
    rng=random.Random(0x37ee84)
    for _ in range(1000):
        r=[(3,rng.getrandbits(32),0),(3,rng.getrandbits(32),0)]
        for op in range(8):compare(op,r)
    gold=ref/'scalar-original-gold.json';gold.write_text(json.dumps(dict(format='source-scalar-v1',cases=len(rows),rows=rows),separators=(',',':'))+'\n')
    binary=ref/'scalar-original-gold.bin';kind={'identity':0,'zero':1,'string':2}
    binary.write_bytes(words(0x314c4353,len(rows))+b''.join(words(r['op'],len(r['arguments']),r['identity32'],len(r['output']),len(r['services']))+b''.join(words(*v) for v in r['arguments'])+words(*r['output'])+b''.join(words(kind[k],v) for k,v in r['services']) for r in rows))
    sources=[ROOT/'port/script-runtime'/p for p in ('script_scalar_bindings.h','script_scalar_bindings.c','tests/script_scalar_differential.py','lua/lua.h','lua/luaconf.h','lua/lauxlib.h')]
    sources.append(ROOT/'port/level-world/tests/visual_timeline_differential.py')
    report=dict(validation='PASS',comparisons=len(rows),comparisons_by_callback=dict(zip(NAMES,counts)),ordered_imported_service_calls=services,mismatches=0,original_sha256=sha(elf),library=dict(path=a.library.relative_to(ROOT).as_posix(),sha256=sha(a.library)),gold=dict(path=gold.relative_to(ROOT).as_posix(),sha256=sha(gold)),binary_gold=dict(path=binary.relative_to(ROOT).as_posix(),sha256=sha(binary)),source_sha256={p.relative_to(ROOT).as_posix():sha(p) for p in sources},original_capture_sha256=sha(ref/'original-functions.json'),explicit_services=['Undefined signed AEABI conversion contract: trunc0,saturation,NaN0; integer-to-float nearest-even.','Undefined AEABI division: trunc0,MIN/-1 wraps; zero-divisor fixture returns0x13579bdf.','Source native64 identity mapped to original uint32 identity.','Fresh Lua VM numeric-string primitive fixture1024.5: genuine host Lua proof separately.','ReturnValues ordered numeric projection, not original STL allocation.'],actual_original_callback_instructions_executed=True,native_optimized_ARM64=True,whole_VM_differential=False,packaged_APK=False)
    (ROOT/'port/script-runtime/reports/script-scalar-arm64-differential.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps({k:report[k] for k in ('validation','comparisons','ordered_imported_service_calls','mismatches')}))
if __name__=='__main__':main()
