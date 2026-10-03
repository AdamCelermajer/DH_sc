"""Actual original timer/Trace callback kernels versus standalone optimized ARM64.

Original getBool/getUInteger/f2uiz execute. CharTimers calls and integer return
container are explicit services. Original string-getBool Lua primitives are an
explicit truthy-string service; actual source VM projection is audited on host.
"""
import argparse,hashlib,json,random,struct,sys
from pathlib import Path
from unicorn import UC_HOOK_CODE
ROOT=Path(__file__).resolve().parents[3]
sys.path.insert(0,str(ROOT/'port/level-world/tests'))
from visual_timeline_differential import TimelineCpu
def words(*v):return struct.pack('<'+'I'*len(v),*(x&0xffffffff for x in v))
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def float_word(n):return struct.unpack('<I',struct.pack('<f',float(n)))[0]
def signed(v):return v if v<0x80000000 else v-0x100000000
class Machine:
    def __init__(self,path,native,manifest):
        self.c=TimelineCpu(path,native,manifest);self.native=native;c=self.c;d=c.data
        self.args=d+0x1000;self.vector=d+0x2000;self.records=d+0x3000
        self.owner=d+0x4000;self.services=d+0x5000;self.results=d+0x6000
        self.returned=d+0x7000;self.start=d+0x8000;self.stop=d+0x8100;self.string=d+0x9000
        self.context=d+0xa000;self.error=d+0xb000
        c.uc.mem_write(self.string,b'0\0')
        if native:c.uc.mem_write(self.services,struct.pack('<4QQ',self.context,self.owner,self.start,self.stop,0))
        else:c.pointer(self.args+4,self.vector)
        c.uc.hook_add(UC_HOOK_CODE,self.hook)
    def ret(self,value=0):
        c=self.c;c.put(0,value);c.uc.reg_write(c.pc,c.uc.reg_read(c.lr))
    def hook(self,uc,address,size,_):
        c=self.c
        if self.native:
            if address==self.start:
                assert c.reg(0)==self.context and c.reg(1)==self.owner
                self.calls.append((0,c.reg(2),c.reg(3)&0xffffffff,c.reg(4),1,c.reg(5)))
                self.ret(self.id)
            elif address==self.stop:
                assert c.reg(0)==self.context and c.reg(1)==self.owner
                self.calls.append((1,c.reg(2),0,0,1,0));self.ret()
        else:
            if address==0x3dbe24:
                assert c.reg(0)==self.owner+0x3b4
                user=struct.unpack('<I',uc.mem_read(c.uc.reg_read(c.sp),4))[0]
                self.calls.append((0,c.reg(1),c.reg(2),c.reg(3),1,user));self.ret(self.id)
            elif address==0x3db2d8:
                assert c.reg(0)==self.owner+0x3b4
                self.calls.append((1,c.reg(1),0,0,1,0));self.ret()
            elif address==0x37cb24:
                self.output=(1,float_word(signed(c.reg(1))));self.ret()
            elif address==0x84c7e0:self.ret(self.context)
            elif address==0x84c04c:
                assert c.reg(0)==self.context and c.reg(1)==self.string;self.ret()
            elif address==0x84b320:
                assert c.reg(0)==self.context;self.ret(1)
            elif address==0x85797c:self.ret()
    def execute(self,op,records,ident):
        c=self.c;self.id=ident;self.calls=[];self.output=(0,0)
        if self.native:
            raw=b''
            for kind,payload,boolean,pointer in records:
                raw+=words(kind,0,payload,boolean)+struct.pack('<QQQ',self.string if kind==4 else 0,1 if kind==4 and pointer else 0,self.context if pointer and kind in (2,7) else 0)
            if raw:c.uc.mem_write(self.records,raw)
            c.uc.mem_write(self.results,bytes(16*40));c.uc.mem_write(self.returned,words(0xdeadbeef))
            result=c.invoke(('dh2_script_game_start_timer','dh2_script_game_stop_timer','dh2_script_game_trace')[op],
                [self.services,self.records,len(records),self.results,16,self.returned,self.error,256])
            assert result==0
            count=struct.unpack('<I',c.uc.mem_read(self.returned,4))[0]
            self.output=(count,struct.unpack('<I',c.uc.mem_read(self.results+8,4))[0] if count else 0)
        else:
            c.uc.mem_write(self.vector,words(self.records,self.records+112*len(records),self.records+112*len(records)))
            for index,(kind,payload,boolean,pointer) in enumerate(records):
                p=self.records+112*index;c.uc.mem_write(p,bytes(112));c.uc.mem_write(p+4,words(kind,float_word(boolean) if kind==1 else payload))
                c.pointer(p+0x20,self.string);c.pointer(p+0x6c,self.context if pointer else 0)
            c.invoke((0x3b7590,0x3b7064,0x37ee80)[op],[self.args,self.results,self.owner])
        return self.output,tuple(self.calls)
def main():
    ap=argparse.ArgumentParser()
    ap.add_argument('--library',type=Path,default=ROOT/'.local-inputs/lua514-source/android-arm64-v8a/libdh2_script_runtime.so')
    args=ap.parse_args();engine=ROOT/'.local-inputs/libDungeonHunter2.so'
    ref=ROOT/'port/script-runtime/reference/game-bindings'
    manifest=json.loads((ref/'core/original-functions.json').read_text())
    assert sha(engine)==manifest['original_sha256']
    old=Machine(engine,False,manifest);new=Machine(args.library,True,{'functions':[]})
    corpus=[];count_by_op=[0,0,0];request_count=0
    def compare(op,records,ident):
        nonlocal request_count
        expected=old.execute(op,records,ident);actual=new.execute(op,records,ident)
        assert expected==actual,(len(corpus),op,records,ident,expected,actual)
        output,calls=expected;count_by_op[op]+=1;request_count+=len(calls)
        corpus.append(words(op,len(records),ident)+b''.join(words(*r) for r in records)+words(*output,len(calls))+b''.join(words(*c) for c in calls))
    loops=[(0,0,0,0),(1,0,0,0),(1,0,1,0),(3,0,0,0),(3,0x80000000,0,0),
        (3,0x3f800000,0,0),(3,0xbf800000,0,0),(3,0x7fc12345,0,0),(3,0xffc12345,0,0),
        (3,0x7f800000,0,0),(2,0,0,0),(2,0,0,1),(7,0,0,0),(7,0,0,1),
        (4,0,0,0),(4,0,0,1),(5,0,0,0),(6,0,0,0),(8,0,0,0)]
    edges=[0,0x80000000,1,0x007fffff,0x3f000000,0x3f7fffff,0x3f800000,0x3fffffff,
        0x414c0000,0xbf800000,0xcf800000,0x4effffff,0x4f000000,0x4f7fffff,0x4f800000,
        0x4f800001,0x7f7fffff,0x7f800000,0xff800000,0x7fc12345,0x7f812345,0xffc12345]
    ids=[-1,0,1,16777217,2147483647,-2147483648]
    for op in range(3):
        for ident in ids:compare(op,[],ident)
        for first in loops:
            for second in loops:compare(op,[first,second],ids[(first[0]+second[0])%len(ids)])
        for edge in edges:
            first=(3,edge,0,0)
            compare(op,[first],1)
            for second in loops:compare(op,[first,second,(4,0,0,1)],ids[(edge+second[0])%len(ids)])
    rng=random.Random(0x3b7590)
    for _ in range(1024):
        first=(3,rng.getrandbits(32),0,0);second=rng.choice(loops)
        for op in range(3):compare(op,[first,second],rng.choice(ids))
    path=ref/'callback-reference.bin';path.write_bytes(words(0x31424d47,len(corpus))+b''.join(corpus))
    sources=[ROOT/'port/script-runtime'/p for p in ('script_runtime.h','script_runtime.c','script_game_bindings.h','script_game_bindings.c','CMakeLists.txt','tests/script_game_bindings_differential.py')]
    report={'validation':'PASS','original_sha256':sha(engine),'comparisons':len(corpus),
        'comparisons_by_callback':dict(zip(('StartTimer','StopTimer','Trace'),count_by_op)),
        'ordered_timer_service_calls':request_count,'mismatches':0,'library_sha256':sha(args.library),
        'library':args.library.relative_to(ROOT).as_posix(),'reference_sha256':sha(path),
        'source_sha256':{p.relative_to(ROOT).as_posix():sha(p) for p in sources},
        'original_functions_sha256':sha(ref/'core/original-functions.json'),
        'source_argument_projection':'sfc Value records; actual source Lua stack projection separately verified on host',
        'explicit_services':['CharTimers.Start/Stop','ReturnValues.pushInteger','string getBool Lua primitive truthiness'],
        'actual_original_unsigned_conversion_executed':True,'whole_VM_differential':False,'packaged_APK':False}
    (ROOT/'port/script-runtime/reports/script-game-bindings-arm64-differential.json').write_text(json.dumps(report,indent=2)+'\n')
    print(json.dumps({k:report[k] for k in ('validation','comparisons','ordered_timer_service_calls','mismatches')}))
if __name__=='__main__':main()
