"""Execute source pCall_/_addFromStack/_setFromStack projection ordering.

Lua stack primitives and native C++ return-vector allocation are explicit
services; the actual original projection/call-count loops execute unchanged.
This does not execute an original Lua VM or model a successful missing binding.
"""
import hashlib,json,struct,sys
from pathlib import Path
from unicorn import UC_HOOK_CODE
ROOT=Path(__file__).resolve().parents[4];HERE=Path(__file__).resolve().parent
sys.path.insert(0,str(ROOT/'port/level-world/tests'))
from visual_timeline_differential import TimelineCpu
def words(*v):return struct.pack('<'+'I'*len(v),*(x&0xffffffff for x in v))
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
manifest=json.loads((HERE/'return-discard/original-functions.json').read_text())
engine=ROOT/'.local-inputs/libDungeonHunter2.so';assert sha(engine)==manifest['original_sha256']
c=TimelineCpu(engine,False,manifest);d=c.data
instance=d+0x1000;L=d+0x2000;returns=d+0x3000;vector=d+0x4000
slots=d+0x10000;text=d+0x20000;identity=d+0x30000
c.uc.mem_write(text,b'a\0b\0')
def word(p):return struct.unpack('<I',c.uc.mem_read(p,4))[0]
def cstring(p):
    b=bytearray()
    while len(b)<4096:
        v=c.uc.mem_read(p+len(b),1)[0]
        if not v:return bytes(b)
        b.append(v)
    raise AssertionError('unbounded fixture string')
rows=[];state={}
def ret(value=0):c.put(0,value);c.uc.reg_write(c.pc,c.uc.reg_read(c.lr))
def hook(uc,address,size,_):
    if address==0x84b12c:ret(state['top'])
    elif address==0x84bc50:
        assert c.reg(1)==0 and c.reg(2)==0xffffffff
        state['top']=len(state['kinds']);ret(0)
    elif address==0x3194e0:
        uc.mem_write(c.reg(0),bytes(112));ret()
    elif address==0x3195c0:
        assert c.reg(0)==vector
        p=word(vector+4);uc.mem_write(p,bytes(uc.mem_read(c.reg(1),112)))
        c.pointer(vector+4,p+112);ret()
    elif address==0x3193e8:ret()
    elif address==0x84b264:
        index=c.reg(1);index=index if index<0x80000000 else index-0x100000000
        assert -len(state['kinds'])<=index<=-1
        state['current']=len(state['kinds'])+index
        state['indices'].append(index);ret(state['kinds'][state['current']])
    elif address==0x84c1ec:
        assert cstring(c.reg(2))==b'_this'
        state['fields'].append(state['current']);state['top']+=1;ret()
    elif address==0x84b390:ret(identity)
    elif address==0x84b320:ret(1)
    elif address==0x84c450:ret(0x3f800000)
    elif address==0x84c384:ret(text)
    elif address==0x30de54:ret(len(cstring(c.reg(0))))
    elif address==0x3109e0:
        copied=bytes(uc.mem_read(c.reg(1),c.reg(2)-c.reg(1)))
        state['string_copies'].append(copied.hex())
        c.pointer(c.reg(0)+0x14,c.reg(1));ret()
    elif address==0x84b140:
        value=c.reg(1);value=value if value<0x80000000 else value-0x100000000
        state['top']=value if value>=0 else state['top']+value+1;ret()
c.uc.hook_add(UC_HOOK_CODE,hook)
for kinds in ([],[5],[5,5],[5]*17,[5]*48,[5]*256,[0,1,2,3,4,5,6,7,8]):
    c.uc.mem_write(returns,bytes(64));c.pointer(returns+0x24,vector)
    c.uc.mem_write(vector,words(slots,slots,slots+112*512));c.pointer(instance+4,L)
    state={'top':1,'kinds':kinds,'fields':[],'indices':[],'string_copies':[]}
    c.invoke(0x31aa78,[instance,0,returns])
    assert state['top']==0 and state['indices']==list(range(-len(kinds),0))
    assert state['fields']==[i for i,k in enumerate(kinds) if k==5]
    count=(word(vector+4)-slots)//112;assert count==len(kinds)
    projected=[word(slots+112*i+4) for i in range(count)]
    expected=[7 if kind==5 else 0 if kind>=6 else kind for kind in kinds]
    assert projected==expected
    if 4 in kinds:assert '61' in state['string_copies']
    rows.append({'lua_return_types':kinds,'source_negative_indices':state['indices'],
        'this_field_lookups':state['fields'],'projected_value_types':projected,
        'string_copies_hex':state['string_copies'],'final_lua_stack_top':0})
report={'validation':'PASS','original_sha256':sha(engine),'probe_source_sha256':sha(Path(__file__)),
    'source_captures_sha256':{p.relative_to(HERE).as_posix():sha(p) for p in HERE.rglob('original-functions.asm')},
    'cases':len(rows),'maximum_return_arity':256,'mismatches':0,'observations':rows,
    'explicit_services':['Lua stack/type/field primitives','C++ Value/vector/string allocation'],
    'scope':'Actual original pCall_/ReturnValues/_setFromStack instruction loops; no original full VM execution or real source allocator claim.'}
(HERE/'return-projection-gold.json').write_text(json.dumps(report,indent=2)+'\n')
print(json.dumps({k:report[k] for k in ('validation','cases','maximum_return_arity','mismatches')}))
