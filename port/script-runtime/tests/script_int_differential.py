"""Original private integer map/value coercions versus optimized ARM64 instructions."""
import argparse,hashlib,json,math,random,struct,sys
from pathlib import Path
from unicorn import UC_HOOK_CODE
from unicorn.arm64_const import UC_ARM64_REG_S0
ROOT=Path(__file__).resolve().parents[3];REF=ROOT/'port/script-runtime/reference/int-bindings'
sys.path.insert(0,str(ROOT/'port/level-world/tests'))
sys.path.insert(0,str(ROOT/'port/script-runtime/tests'))
from visual_timeline_differential import TimelineCpu,integer
from script_function_alias_differential import Native as AllocatorCpu
def w(*v):return struct.pack('<'+'I'*len(v),*(x&0xffffffff for x in v))
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def fw(f):return struct.unpack('<I',struct.pack('<f',f))[0]
def fp(b):return struct.unpack('<f',w(b))[0]
def signed(b):return b if b<0x80000000 else b-0x100000000
def fraction(b):
    f=fp(b)
    return ('-nan' if b>>31 else 'nan') if math.isnan(f) else format(f,'.6f')
def numeric(text):
    return {b'1024.5':1024.5,b'garbage':0.,b'0x100':256.,b'-256.5':-256.5,b'':0.}[text.split(b'\0')[0]]
class OriginalCpu(TimelineCpu):
    def external(self,uc,address,size,_):
        name=self.imports.get(address)
        if name=='floorf':
            bits=self.reg(0);f=fp(bits);self.put(0,fw(float(math.floor(f))) if math.isfinite(f) else bits)
        elif name=='__aeabi_f2d':
            raw=struct.pack('<d',fp(self.reg(0)));self.put(0,int.from_bytes(raw[:4],'little'));self.put(1,int.from_bytes(raw[4:],'little'))
        elif name=='__aeabi_ui2f':self.put(0,fw(float(self.reg(0))))
        else:return super().external(uc,address,size,_)
        self.import_calls[name]=self.import_calls.get(name,0)+1;uc.reg_write(self.pc,uc.reg_read(self.lr))
class Original:
    def __init__(self,elf,manifest):
        self.c=OriginalCpu(elf,False,manifest);c=self.c;d=c.data
        self.owners=[d+0x1000,d+0x1100];self.active=0;self.args=d+0x2000;self.vector=d+0x2100;self.records=d+0x3000
        self.text=d+0x5000;self.value_text=d+0x9000;self.results=d+0xd000;self.L=d+0xe000
        self.nodes=d+0x20000;self.copy=d+0x100000;self.tables=[{},{}];self.source_identity=0
        self.output=[];self.services=[];self.last_number=0;self.clearing=False;self.erased=0;c.pointer(self.args+4,self.vector)
        for owner in self.owners:c.uc.mem_write(owner,bytes(0x100));self.balance(owner)
        c.uc.hook_add(UC_HOOK_CODE,self.hook)
    def word(self,p):return int.from_bytes(self.c.uc.mem_read(p,4),'little')
    def text_at(self,p):
        if not p:return None
        out=bytearray()
        while True:
            b=self.c.uc.mem_read(p+len(out),1)[0]
            if not b:return bytes(out)
            out.append(b);assert len(out)<65536
    def ret(self,v=0):c=self.c;c.put(0,v);c.uc.reg_write(c.pc,c.uc.reg_read(c.lr))
    def balance(self,owner):
        table=self.tables[self.owners.index(owner)];rows=sorted(table.items());c=self.c;head=owner+0x1c
        def link(rows,parent):
            if not rows:return 0
            n=len(rows)//2;key,node=rows[n];c.pointer(node+4,parent);c.pointer(node+8,link(rows[:n],node));c.pointer(node+12,link(rows[n+1:],node));return node
        c.pointer(head+4,link(rows,head));c.pointer(head+8,rows[0][1] if rows else head);c.pointer(head+12,rows[-1][1] if rows else head);c.pointer(head+16,len(rows))
    def hook(self,uc,address,size,_):
        c=self.c
        if address==0x30de54:self.ret(len(self.text_at(c.reg(0))))
        elif address==0x30e76c:self.ret()
        elif address==0x3116e8:c.pointer(c.reg(0)+0x10,c.reg(2));c.pointer(c.reg(0)+0x14,c.reg(1));self.ret(c.reg(0))
        elif address in (0x708f00,0x310440):
            if self.clearing and address==0x708f00:
                assert c.reg(1)==24;table=self.tables[self.active];key=self.word(c.reg(0)+0x10);assert table.pop(key)==c.reg(0);self.erased+=1
            self.ret()
        elif address==0x37c0ec and self.clearing:uc.reg_write(c.pc,c.stop)
        elif address==0x37d61c:
            out,head,_,pair=[c.reg(i) for i in range(4)];owner=head-0x1c;table=self.tables[self.owners.index(owner)];key=self.word(pair)
            assert key not in table;node=self.nodes;self.nodes+=0x40;uc.mem_write(node,bytes(0x40));c.pointer(node+0x10,key);c.pointer(node+0x14,self.word(pair+4));table[key]=node;self.balance(owner);c.pointer(out,node);self.ret(out)
        elif address==0x31c49c:
            record=c.reg(0);kind=self.word(record+4);self.last_number=self.word(record+8)
            identity=self.word(record+0x6c)
            if kind in (2,7) and identity:self.services.append(('identity',identity))
        elif address==0x31bbf0:
            kind=self.word(c.reg(0)+4);identity=self.word(c.reg(0)+0x6c)
            if kind in (2,7) and identity:self.services.append(('identity',identity))
        elif address==0x30eae4:
            dest,fmt,a,b=[c.reg(i) for i in range(4)];fmt=self.text_at(fmt)
            if fmt==b'%d':text=str(signed(a))
            elif fmt==b'0x%0*x':assert a==8;text='0x'+format(b,'08x')
            else:assert fmt==b'%f';self.services.append(('format',self.last_number));text=fraction(self.last_number)
            uc.mem_write(dest,text.encode()+b'\0');self.ret(len(text))
        elif address==0x3109e0:
            dest,start,end=[c.reg(i) for i in range(3)];text=bytes(uc.mem_read(start,end-start));p=self.copy;self.copy+=len(text)+1;uc.mem_write(p,text+b'\0');c.pointer(dest+0x14,p);self.ret(dest)
        elif address==0x84c7e0:self.ret(self.L)
        elif address==0x84c04c:self.numeric_text=self.text_at(c.reg(1));self.ret()
        elif address==0x84c450:self.services.append(('parse',self.numeric_text.hex()));self.ret(fw(numeric(self.numeric_text)))
        elif address==0x85797c:self.ret()
        elif address==0x37cb24:self.output.append(fw(float(signed(c.reg(1)))));self.ret()
    def configure(self,records,key):
        c=self.c;self.services=[];self.output=[];self.source_identity=records[0][3] if records else 0
        c.uc.mem_write(self.text,key+b'\0');c.uc.mem_write(self.value_text,b'1024.5\0')
        c.uc.mem_write(self.vector,w(self.records,self.records+112*len(records),self.records+112*len(records)))
        for i,(kind,bits,boolean,pointer) in enumerate(records):
            p=self.records+112*i;c.uc.mem_write(p,bytes(112));c.uc.mem_write(p+4,w(kind,fw(float(boolean)) if kind==1 else bits));c.pointer(p+0x20,self.text if i==0 else self.value_text);c.pointer(p+0x6c,pointer)
    def execute(self,op,records,key,bits):
        self.configure(records,key);c=self.c;owner=self.owners[self.active];self.result=None
        if op==0:self.result=c.invoke(0x37c164,[self.text])
        elif op==1:c.invoke(0x37d990,[owner,self.text,bits])
        elif op==2:self.result=c.invoke(0x37da30,[owner,self.text])
        elif op in (3,4):c.invoke(0x37de5c if op==3 else 0x37ec14,[self.args,self.results,owner]);self.result=self.output
        elif op==5:
            p=c.invoke(0x31c49c,[self.records]);self.result=[None if not p else self.text_at(p).hex(),p==self.text]
        elif op==6:
            p=c.invoke(0x31bbf0,[self.records]);self.result=c.invoke(0x30e4cc,[p])
        elif op==7:
            self.clearing=True;c.put(4,owner);c.invoke(0x37c0bc,[]);self.clearing=False
            assert not self.tables[self.active] and self.word(owner+0x20)==0 and self.word(owner+0x2c)==0 and self.word(owner+0x24)==owner+0x1c and self.word(owner+0x28)==owner+0x1c
        elif op==8:self.active=bits
        else:raise AssertionError(op)
        state=[[(key,self.word(node+0x14)) for key,node in sorted(t.items())] for t in self.tables]
        return self.result,state,self.services
class Native(AllocatorCpu):
    def __init__(self,elf):
        super().__init__(elf);d=self.data;self.maps=[self.invoke('dh2_script_int_create',[]),self.invoke('dh2_script_int_create',[])];self.active=0
        self.workspace=self.invoke('dh2_script_string_projection_create',[]);self.services=d+0x20000;self.identity=d+0x21000;self.format=d+0x21100;self.identity32=0;self.trace=[]
        self.uc.hook_add(UC_HOOK_CODE,self.hook)
    def assign(self,p,value):
        assert len(value)<=22
        self.uc.mem_write(p,bytes(24));self.uc.mem_write(p,bytes((len(value)<<1,))+value+b'\0')
    def external(self,uc,address,size,_):
        name=self.imports.get(address)
        if name and name.startswith('_ZNSt6__ndk112basic_string') and ('6assignEPKc' in name or 'aSEPKc' in name):
            p=self.reg(0);value=self.text(self.reg(1));self.assign(p,value);self.put(0,p)
        elif name=='snprintf':
            p,n,fmt=[self.reg(i) for i in range(3)];fmt=self.text(fmt)
            if fmt==b'%d':value=str(signed(self.reg(3)&0xffffffff)).encode()
            else:assert fmt==b'0x%0*x' and self.reg(3)==8;value=('0x'+format(self.reg(4)&0xffffffff,'08x')).encode()
            assert len(value)<n;uc.mem_write(p,value+b'\0');self.put(0,len(value))
        elif name=='strlen':self.put(0,len(self.text(self.reg(0))))
        elif name=='luaL_newstate':self.put(0,self.data+0x50000)
        elif name=='lua_cpcall':
            p=self.reg(2);text=self.text(self.number(p));self.trace.append(('parse',text.hex()));uc.mem_write(p+8,w(fw(numeric(text))));self.put(0,0)
        elif name=='lua_close':self.put(0,0)
        else:return super().external(uc,address,size,_)
        self.import_calls[name]=self.import_calls.get(name,0)+1;uc.reg_write(self.pc,uc.reg_read(self.lr))
    def hook(self,uc,address,size,_):
        if address==self.identity:
            assert self.reg(1)==0xfedcba9876543210;self.trace.append(('identity',self.identity32));uc.mem_write(self.reg(2),w(self.identity32));self.put(0,0);uc.reg_write(self.pc,uc.reg_read(self.lr))
        elif address==self.format:
            bits=uc.reg_read(UC_ARM64_REG_S0);self.trace.append(('format',bits));text=fraction(bits).encode();assert len(text)<self.reg(2);uc.mem_write(self.reg(1),text+b'\0');uc.mem_write(self.reg(3),struct.pack('<Q',len(text)));self.put(0,0);uc.reg_write(self.pc,uc.reg_read(self.lr))
    def state(self):
        states=[]
        for owner in self.maps:
            entries=[];visited=set();head=owner+8
            def visit(node,parent):
                if not node:return 1
                assert node not in visited;visited.add(node);assert self.number(node+16)==parent;black=self.uc.mem_read(node+24,1)[0];assert black in (0,1)
                left,right=self.number(node),self.number(node+8)
                if not black:assert (not left or self.uc.mem_read(left+24,1)[0]) and (not right or self.uc.mem_read(right+24,1)[0])
                a=visit(left,node);entries.append((self.word(node+28),self.word(node+32)));b=visit(right,node);assert a==b;return a+black
            root=self.number(head)
            if root:assert self.uc.mem_read(root+24,1)[0]
            visit(root,head);assert len(entries)==self.number(owner+16);assert [k for k,v in entries]==sorted(k for k,v in entries);states.append(entries)
        return states
    def execute(self,op,records,key,bits):
        self.trace=[];self.identity32=0xfedcba98;self.uc.mem_write(self.input,key+b'\0');other=self.input+len(key)+1;self.uc.mem_write(other,b'1024.5\0')
        self.uc.mem_write(self.services,struct.pack('<5Q',self.maps[self.active],0,self.identity,self.format,0))
        raw=b''.join(w(k,0,b,bo)+struct.pack('<3Q',self.input if i==0 else other,len(key) if i==0 else 6,0xfedcba9876543210 if p else 0) for i,(k,b,bo,p) in enumerate(records))
        if raw:self.uc.mem_write(self.values,raw)
        self.uc.mem_write(self.results,bytes(80));self.uc.mem_write(self.returned,w(0xdeadbeef));result=None;m=self.maps[self.active]
        if op==0:result=self.invoke('dh2_script_int_hash',[self.input])
        elif op==1:assert self.invoke('dh2_script_int_set',[m,self.input,bits])==0
        elif op==2:assert self.invoke('dh2_script_int_get',[m,self.input,self.results])==0;result=self.word(self.results)
        elif op in (3,4):
            assert self.invoke('dh2_script_int_set_callback' if op==3 else 'dh2_script_int_get_callback',[self.services,self.values,len(records),self.results,2,self.returned,self.error,256])==0
            result=[self.word(self.results+8)] if self.word(self.returned) else []
        elif op==5:
            assert self.invoke('dh2_script_value_get_string',[self.workspace,self.services,self.values,self.results])==0;p=self.number(self.results);result=[None if not p else self.text(p).hex(),p==self.input]
        elif op==6:assert self.invoke('dh2_script_value_get_integer',[self.services,self.values,self.results])==0;result=self.word(self.results)
        elif op==7:assert self.invoke('dh2_script_int_clear_contents',[m])==0
        elif op==8:self.active=bits
        else:raise AssertionError(op)
        return result,self.state(),self.trace
def main():
    ap=argparse.ArgumentParser();ap.add_argument('--library',type=Path,default=ROOT/'.local-inputs/lua514-source/ints/oracle.so');a=ap.parse_args()
    source=ROOT/'.local-inputs/libDungeonHunter2.so';manifest=json.loads((REF/'original-functions.json').read_text());assert sha(source)==manifest['original_sha256']
    old=Original(source,manifest);new=Native(a.library);rows=[];counts=[0]*9;calls=0
    def compare(op,records=[],key=b'',bits=0):
        nonlocal calls
        expected=old.execute(op,records,key,bits);actual=new.execute(op,records,key,bits);assert expected==actual,(len(rows),op,records,key,bits,expected,actual)
        counts[op]+=1;calls+=len(expected[2]);rows.append(dict(op=op,arguments=records,key_hex=key.hex(),bits=bits,result=expected[0],maps=expected[1],services=expected[2]))
    for op in (3,4):compare(op,[])
    for key in (b'',b'nil',b'true',b'false',b'\xff\x80',b'Before\0After',b'Alias16038',b'Alias16275',b'x'*1024):
        compare(0,key=key);compare(2,key=key);compare(1,key=key,bits=0x80000001);compare(2,key=key)
    edges=[0,0x80000000,1,0x3f800000,0xbf800000,0x3f800001,0x3eaaaaab,0xc3804000,0x4b000000,0x4b800001,0x4effffff,0x4f000000,0xcf000000,0x7f800000,0xff800000,0x7fc12345,0xffc12345]
    values=[(k,fw(513.75),b,p) for k in range(9) for b in ((0,1) if k==1 else (0,)) for p in ((0,0xfedcba98) if k in (2,7) else (0,))]
    for v in values:
        compare(5,[v],b'1024.5');compare(6,[v],b'1024.5')
        if v[0] not in (5,6,8):
            compare(4,[v],b'1024.5')
            for value in values:compare(3,[v,value],b'1024.5');compare(4,[v],b'1024.5')
    for bits in edges:
        v=(3,bits,0,0);compare(5,[v]);compare(6,[v]);compare(3,[v,(3,bits,0,0)]);compare(4,[v])
    rng=random.Random(0x37da30)
    for i in range(400):
        key=bytes(rng.randrange(1,256) for _ in range(rng.randrange(0,32)))
        compare(0,key=key);compare(2,key=key);compare(1,key=key,bits=rng.getrandbits(32));compare(2,key=key)
        bits=rng.getrandbits(32);compare(5,[(3,bits,0,0)]);compare(6,[(3,bits,0,0)])
        compare(8,bits=i%2)
        if i%50==0:compare(7)
    new.invoke('dh2_script_string_projection_destroy',[new.workspace])
    for m in new.maps:new.invoke('dh2_script_int_destroy',[m])
    assert not new.allocations
    gold=REF/'int-original-gold.json';gold.write_text(json.dumps(dict(format='source-int-v1',cases=len(rows),rows=rows),separators=(',',':'))+'\n')
    report=dict(validation='PASS',comparisons=len(rows),comparisons_by_operation=counts,complete_private_map_state_comparisons=len(rows)*2,ordered_services=calls,mismatches=0,original_sha256=sha(source),original_manifest_sha256=sha(REF/'original-functions.json'),original_teardown_manifest_sha256=sha(REF/'teardown/original-functions.json'),actual_original_clear_entry='0x37c0bc',actual_original_clear_stop='0x37c0ec',actual_original_erased_nodes=old.erased,gold=dict(path=gold.relative_to(ROOT).as_posix(),sha256=sha(gold)),library=dict(path=a.library.relative_to(ROOT).as_posix(),sha256=sha(a.library)),source_sha256={p.relative_to(ROOT).as_posix():sha(p) for p in [ROOT/'port/script-runtime/script_int_bindings.hpp',ROOT/'port/script-runtime/script_int_bindings.cpp',Path(__file__)]},explicit_services=['Original native STL tree insertion/allocation/balance, original std::string storage, lazy hash guard; original recursive erase instructions execute with sized deallocation service.','Undefined AEABI/floorf/printf dependencies; printf fractional fixture uses GNU-style %f including signed nan.','Fresh Lua numeric-string parsing primitive; genuine host parser audited separately.','Source32 identity mapped explicitly from native64 identity.','Native libc++ string assignment, allocation/deallocation primitives; actual native map RB instructions execute.'],actual_original_instructions=True,optimized_native_ARM64=True,native_RB_tree_instructions=True,whole_VM_differential=False,shipping_Android_printf_parity=False,packaged_APK=False)
    (ROOT/'port/script-runtime/reports/script-int-arm64-differential.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps({k:report[k] for k in ['validation','comparisons','complete_private_map_state_comparisons','ordered_services','mismatches']}))
if __name__=='__main__':main()
