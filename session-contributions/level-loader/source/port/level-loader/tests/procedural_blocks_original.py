"""Execute original MGX block/exit interpretation on original XML trees.

Original Block constructor, MgxBlock::LoadFromXml, Exit::LoadFromXml,
StrToObj, string/vector operations and exit grid arithmetic execute ARM32.
Allocator and imported C/float services are explicit environment adapters.
Host CRT numeric scanning is not proof of historical Bionic parsing for all strings.
No block, direction, link-type or coordinate result is supplied by a hook.
"""
import argparse,ctypes,hashlib,json,math,pathlib,re,struct,sys,zipfile

def create_original_blocks_cpu(engine,dependency_root):
    sys.path.insert(0,str(dependency_root))
    sys.path.insert(0,str(pathlib.Path(__file__).parent))
    from xml_original_probe import Parser
    from unicorn import UC_HOOK_CODE
    from unicorn.arm_const import UC_ARM_REG_SP
    engine_hash=hashlib.sha256(engine.read_bytes()).hexdigest()
    assert engine_hash=='36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80'
    crt=ctypes.CDLL('msvcrt')
    crt.strtod.argtypes=[ctypes.c_char_p,ctypes.POINTER(ctypes.c_void_p)];crt.strtod.restype=ctypes.c_double
    crt.sscanf.argtypes=[ctypes.c_char_p,ctypes.c_char_p];crt.sscanf.restype=ctypes.c_int
    def fb(value):return struct.unpack('<I',struct.pack('<f',ctypes.c_float(value).value))[0]
    def fv(value):return struct.unpack('<f',struct.pack('<I',value))[0]
    class Blocks(Parser):
        def __init__(self):
            super().__init__(engine,{'functions':[]});self.token=0;self.adapters={}
            self.uc.hook_add(UC_HOOK_CODE,self.allocation)
            self.uc.hook_add(UC_HOOK_CODE,self.observe_exit)
        def observe_exit(self,uc,address,size,unused):
            if address==0x48a5d0:
                self.active_exit=self.child_indices[self.word(self.reg(1))]
            elif address==0x48aa28:
                self.declarations.append({'source_child':self.active_exit,'accepted':bool(self.reg(0))})
        def external(self,uc,address,size,unused):
            name=self.imports.get(address)
            if name=='strcasecmp':
                assert self.reg(0) and self.reg(1),'Null strcasecmp input'
                x,y=self.text(self.reg(0)).lower(),self.text(self.reg(1)).lower();self.returned((x>y)-(x<y))
            elif name=='memchr':
                p,c,n=[self.reg(i) for i in range(3)];assert n<0x100000
                i=bytes(uc.mem_read(p,n)).find(bytes([c&255]));self.returned(p+i if i>=0 else 0)
            elif name=='strcpy':
                p,q=self.reg(0),self.reg(1);assert q,'Null strcpy input'
                raw=self.text(q).encode()+b'\0';assert len(raw)<=256,'Original StrToObj fixed buffer limit'
                uc.mem_write(p,raw);self.returned(p)
            elif name=='strtok':
                p=self.reg(0) or self.token;delims=set(self.text(self.reg(1)).encode())
                while p and uc.mem_read(p,1)[0] in delims:p+=1
                if not p or uc.mem_read(p,1)==b'\0':self.token=0;self.returned(0)
                else:
                    start=p
                    while uc.mem_read(p,1)[0] and uc.mem_read(p,1)[0] not in delims:p+=1
                    if uc.mem_read(p,1)[0]:uc.mem_write(p,b'\0');self.token=p+1
                    else:self.token=0
                    self.returned(start)
            elif name=='strtod':
                raw=self.text(self.reg(0)).encode();buf=ctypes.create_string_buffer(raw);end=ctypes.c_void_p()
                value=crt.strtod(buf,ctypes.byref(end))
                if self.reg(1):self.pointer(self.reg(1),self.reg(0)+end.value-ctypes.addressof(buf))
                lo,hi=struct.unpack('<2I',struct.pack('<d',value));self.put(1,hi);self.returned(lo)
            elif name=='sscanf':
                raw=self.text(self.reg(0)).encode();fmt=self.text(self.reg(1)).encode()
                assert fmt in (b'%lf',b'%d'),fmt
                value=ctypes.c_double() if fmt==b'%lf' else ctypes.c_int()
                result=crt.sscanf(raw,fmt,ctypes.byref(value))
                if result==1:uc.mem_write(self.reg(2),bytes(value))
                self.returned(result)
            elif name=='__aeabi_d2f':
                value=struct.unpack('<d',struct.pack('<2I',self.reg(0),self.reg(1)))[0];self.returned(fb(value))
            elif name=='__aeabi_i2f':
                value=self.reg(0);self.returned(fb(value if value<0x80000000 else value-0x100000000))
            elif name=='__aeabi_f2iz':
                value=fv(self.reg(0));assert math.isfinite(value) and -2147483648<=value<2147483648,value
                self.returned(int(value))
            elif name in ('ceilf','floorf'):
                value=fv(self.reg(0));self.returned(fb(math.ceil(value) if name=='ceilf' else math.floor(value)))
            else:return super().external(uc,address,size,unused)
            self.adapters[name]=self.adapters.get(name,0)+1
        def run(self,raw,block_address=None):
            # Original node allocator retains global pools. Reuse storage without
            # rewinding its heap underneath those retained pool pointers.
            self.token=0
            parsed=self.parse(raw);assert not parsed['xml_error'],parsed
            doc=self.data+0x10000;root=self.word(doc+0x18)
            while root and self.word(root+0x14)!=1:root=self.word(root+0x3c)
            self.child_indices={};self.declarations=[];child=self.word(root+0x18)
            while child:
                self.child_indices[child]=len(self.child_indices);child=self.word(child+0x3c)
            block=block_address or self.data+0x30000;handle=self.data+0x31000
            self.uc.mem_write(block,bytes(2504));self.pointer(handle,root)
            self.invoke(0x48ab34,[block],budget=3000000)
            result=self.invoke(0x48a90c,[block,handle],budget=3000000)
            count=self.word(block+92);assert count<=8,('Original exit array capacity',count)
            exits=[]
            for i in range(count):
                p=block+96+300*i;start,end=self.word(p+28),self.word(p+32)
                assert end>=start and (end-start)%24==0
                types=[self.text(self.word(s+20)) for s in range(start,end,24)]
                direction=self.word(p+20)
                exits.append({'index':self.word(p+24),'grid':list(struct.unpack('<2i',self.uc.mem_read(p+8,8))),
                    'height_bits':self.word(p+16),'direction':self.word(direction),'link_types':types,
                    'connections':self.word(p+40)})
            return {'load_result':result,'unit_bits':[self.word(block+76),self.word(block+80)],
                'size':list(struct.unpack('<2i',self.uc.mem_read(block+84,8))),'exits':exits,
                'link_declarations_original':self.declarations}
    return Blocks()

def main():
    ap=argparse.ArgumentParser()
    for name in ('engine','dependency-root','cache','out'):ap.add_argument('--'+name,type=pathlib.Path,required=True)
    ap.add_argument('--limit',type=int,default=0)
    a=ap.parse_args();engine_hash=hashlib.sha256(a.engine.read_bytes()).hexdigest()
    cases=[]
    for i,t in enumerate(('a','a,b','a, b',' a','a ','a b','a,  b,c','a,,b,','0','','A,a','  a  ')):
        cases.append(('edge-type-'+str(i),f'<Module><GameObject gametype="link" linktype="{t}" direction="north" position="0,0,7"/></Module>'.encode()))
    for direction in ('east','SOUTH','west','none','unrecognized'):
        cases.append(('edge-direction-'+direction,f'<Module unit_width="6000" unit_height="4000" block_width="3" block_height="2"><GameObject gametype="LiNk" linktype="gate" direction="{direction}" position="10000,-4000,1.25"/></Module>'.encode()))
    cases.append(('empty-module',b'<Module/>'))
    cases.append(('skipped-links',b'<Module><GameObject gametype="visual"/><GameObject gametype="link" linktype="0"/><GameObject gametype="link" linktype="gate" position="0,0,0"/><GameObject gametype="link" direction="north" position="0,0,0"/><GameObject gametype="link" linktype="gate" direction="east" position="0,0,0"/></Module>'))
    for i,position in enumerate(('0,0,0','-9000,4000,7','9000,-4000,-7','-9000.1,4000.1,0',
            '-3000,0,1','-2999.99,0,1','3000,0,1','3000.01,0,1','100000,-100000,0',
            '3','1,,3',',2,3','1,2,3,4','bad, 2px,3','1e3, -2.5e3, 0.125')):
        cases.append(('edge-position-'+str(i),f'<Module unit_width="6000" unit_height="4000" block_width="3" block_height="2"><GameObject gametype="link" linktype="gate" direction="east" position="{position}"/></Module>'.encode()))
    for i,attrs in enumerate(('unit_width="oops" unit_height="junk" block_width="bad" block_height=""',
            'unit_width=" 6000xyz" unit_height="4e3tail" block_width="3tail" block_height="2.5"',
            'unit_width="6000.125" unit_height="4000.25" block_width="7" block_height="5"')):
        cases.append(('edge-attributes-'+str(i),f'<Module {attrs}><GameObject gametype="link" linktype="gate" direction="south" position="10000,-10000,0"/></Module>'.encode()))
    with zipfile.ZipFile(a.cache) as pack:
        prefix='com.gameloft.android.GAND.GloftD2SS/files/'
        authored=[(n[len(prefix):],pack.read(n)) for n in pack.namelist() if n.startswith(prefix) and n.endswith('.mgx')]
    if a.limit:authored=authored[:a.limit]
    probe=create_original_blocks_cpu(a.engine,a.dependency_root);rows=[]
    for name,raw in cases+authored:
        try:rows.append({'name':name,'input_sha256':hashlib.sha256(raw).hexdigest(),
            **({'input_hex':raw.hex()} if name.startswith(('edge-','empty-','skipped-')) else {}),**probe.run(raw)})
        except Exception as e:
            rows.append({'name':name,'failure':str(e),'pc':hex(probe.uc.reg_read(probe.pc))});break
    report={'engine_sha256':engine_hash,'cache_sha256':hashlib.sha256(a.cache.read_bytes()).hexdigest(),
        'script_sha256':hashlib.sha256(pathlib.Path(__file__).read_bytes()).hexdigest(),'scope':__doc__,
        'cases':rows,'original_execution_complete':len(rows)==len(cases)+len(authored) and not any('failure' in r for r in rows),
        'authored_blocks_requested':len(authored),'adapters':probe.adapters,
        'historical_bionic_numeric_parsing_verified':False,'generated_layout_verified':False,'full_loader_verified':False}
    repo=pathlib.Path(__file__).resolve().parents[3]
    helpers={}
    for module in tuple(sys.modules.values()):
        file=getattr(module,'__file__',None)
        if file:
            path=pathlib.Path(file).resolve()
            if path.suffix=='.py' and path.is_relative_to(repo/'port'):
                helpers[str(path.relative_to(repo))]=hashlib.sha256(path.read_bytes()).hexdigest()
    report['dependency_sources_sha256']=helpers
    a.out.write_text(json.dumps(report,indent=2)+'\n')
    print(json.dumps({'cases':len(rows),'authored':len(authored),'complete':report['original_execution_complete'],
        'synthetic_cases':len(cases),'failures':[r for r in rows if 'failure' in r]},indent=2))
    raise SystemExit(0 if report['original_execution_complete'] else 1)

if __name__=='__main__':main()
