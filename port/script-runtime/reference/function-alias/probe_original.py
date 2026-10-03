"""Original hash, alias lookup and producer kernels; tree/string allocation is an explicit service."""
import hashlib,json,random,struct,sys
from pathlib import Path
from unicorn import UC_HOOK_CODE
ROOT=Path(__file__).resolve().parents[4];HERE=Path(__file__).resolve().parent
sys.path.insert(0,str(ROOT/'port/level-world/tests'))
from visual_timeline_differential import TimelineCpu
def words(*v):return struct.pack('<'+'I'*len(v),*(x&0xffffffff for x in v))
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def expected_hash(text):
    h=0
    for v in text.split(b'\0')[0]:h=(h^((v if v<128 else v-256)+0x9e3779b9+(h<<6)+(h>>2)))&0xffffffff
    return h
manifest=json.loads((HERE/'original-functions.json').read_text());engine=ROOT/'.local-inputs/libDungeonHunter2.so'
assert sha(engine)==manifest['original_sha256']
c=TimelineCpu(engine,False,manifest);d=c.data
owner=d+0x1000;args=d+0x2000;vector=d+0x3000;values=d+0x4000;text=d+0x5000
nodes=d+0x10000;strings=d+0x60000;returns=d+0x8000;return_vector=d+0x9000;L=d+0xa000;live_calls=[]
tables={owner+0x34:{},owner+0x4c:{}};allocated={};nextnode=nodes;nexttext=strings;trace=[]
def word(p):return struct.unpack('<I',c.uc.mem_read(p,4))[0]
def string(p):
    out=bytearray()
    while True:
        v=c.uc.mem_read(p+len(out),1)[0]
        if not v:return bytes(out)
        out.append(v);assert len(out)<65536
def ret(v=0):c.put(0,v);c.uc.reg_write(c.pc,c.uc.reg_read(c.lr))
def balanced(table):
    entries=sorted(tables[table].items())
    def link(rows,parent):
        if not rows:return 0
        mid=len(rows)//2;k,n=rows[mid];c.pointer(n+4,parent)
        c.pointer(n+8,link(rows[:mid],n));c.pointer(n+12,link(rows[mid+1:],n));return n
    root=link(entries,table);c.pointer(table+4,root)
    c.pointer(table+8,entries[0][1] if entries else table);c.pointer(table+12,entries[-1][1] if entries else table)
    c.pointer(table+16,len(entries))
def assign(node,value):
    global nexttext
    p=nexttext;nexttext+=len(value)+1;c.uc.mem_write(p,value+b'\0')
    c.pointer(node+0x28,p);c.pointer(node+0x24,p+len(value));c.pointer(node+0x14,p+len(value)+1)
def hook(uc,address,size,_):
    global nextnode
    if address==0x30de54:ret(len(string(c.reg(0))))
    elif address==0x30e76c:ret(0) # lazy initializer/guard service, no hashing replacement
    elif address==0x3116e8:
        # hashString's temporary std::string, actual signed-byte loop follows.
        s,a,b=c.reg(0),c.reg(1),c.reg(2)
        c.pointer(s+0x10,b);c.pointer(s+0x14,a);ret(s)
    elif address in (0x708f00,0x310440):ret()
    elif address==0x37dac4:
        table,key=c.reg(0),word(c.reg(1));assert table in tables
        if key not in tables[table]:
            node=nextnode;nextnode+=0x40;c.uc.mem_write(node,bytes(0x40));c.pointer(node+0x10,key)
            tables[table][key]=node;allocated[node]=(table,key);assign(node,b'');balanced(table)
            trace.append(['insert',table-owner,key])
        ret(tables[table][key]+0x14)
    elif address==0x3109e0:
        node=c.reg(0)-0x14;assert node in allocated
        value=bytes(uc.mem_read(c.reg(1),c.reg(2)-c.reg(1)));assign(node,value)
        table,key=allocated[node];trace.append(['copy',table-owner,key,value.hex()]);ret(c.reg(0))
    elif address==0x37bd7c:
        table=c.reg(0);trace.append(['clear',table-owner]);tables[table].clear();balanced(table);ret()
    elif address==0x37be48:
        table=c.reg(0);node=word(c.reg(1));key=word(node+0x10)
        trace.append(['erase',table-owner,key]);del tables[table][key];balanced(table);ret()
    elif address==0x30e5e0:
        assert c.reg(2)==0;ret(0) # memcmp(empty,empty,0)
    elif address==0x31b268:ret(c.reg(0)) # explicit VM instance construction
    elif address==0x31167c:
        # LuaScript path string reserve16; no aliases are supplied by this service.
        assert c.reg(1)==16;c.pointer(c.reg(0)+0x10,text+0x1000);ret(c.reg(0))
    elif address==0x84c1ec:
        assert c.reg(0)==L and c.reg(1)==0xffffd8ee
        live_calls.append({'name_hex':string(c.reg(2)).hex(),'requested_identity':c.reg(2)==text});ret()
    elif address==0x31ab4c:
        assert c.reg(0)==owner+4 and c.reg(1)==args and c.reg(2)==returns;ret()
c.uc.hook_add(UC_HOOK_CODE,hook)
def reset():
    global nextnode,nexttext,trace
    nextnode=nodes;nexttext=strings;trace=[];c.uc.mem_write(owner,bytes(0x100));tables[owner+0x34].clear();tables[owner+0x4c].clear()
    balanced(owner+0x34);balanced(owner+0x4c);c.pointer(args+4,vector)
def action(op,name=b'',replacement=b'',kind0=4,kind1=4,count=2):
    c.uc.mem_write(text,name+b'\0'+replacement+b'\0');other=text+len(name)+1
    if op==0:return c.invoke(0x37c164,[text])
    if op==6:
        reset();c.invoke(0x37c584,[owner,1]);assert word(owner+0x38)==0 and word(owner+0x44)==0 and word(owner+0x50)==0 and c.uc.mem_read(owner+0x64,1)==b'\0';return
    if op in (1,2):
        p=c.invoke(0x37c314 if op==1 else 0x37c2a0,[owner,text])
        return (p==text,string(p)) if op==1 else p
    if op==3:
        c.uc.mem_write(vector,words(values,values+112*count,values+112*count))
        c.uc.mem_write(values,bytes(112*max(count,2)));c.pointer(values+4,kind0);c.pointer(values+0x20,text)
        c.pointer(values+112+4,kind1);c.pointer(values+112+0x20,other)
        c.invoke(0x37ec70,[args,0,owner]);return
    c.invoke(0x37be00 if op==4 else 0x37dc44,[args,0,owner])
records=[];rows=[]
def record(op,name=b'',replacement=b'',kind0=4,kind1=4,count=2):
    before=len(trace);result=action(op,name,replacement,kind0,kind1,count)
    output=words(result) if op in (0,2) else words(result[0],len(result[1]))+result[1] if op==1 else b''
    inp=words(op,len(name),len(replacement),kind0,kind1,count)+name+replacement
    records.append(words(len(inp),len(output))+inp+output)
    rows.append({'operation':op,'name_hex':name.hex(),'replacement_hex':replacement.hex(),'kind':[kind0,kind1],'count':count,
                 'result':result if op!=1 else {'caller_identity':result[0],'text_hex':result[1].hex()},'services':trace[before:]})
    if op==0:assert result==expected_hash(name)
reset();record(6)
for s in (b'',b'OnTimer',b'a',b'\xff',b'\x80\xfea',b'Before\0After',b'x'*1024):record(0,s)
rng=random.Random(0x37c164)
for _ in range(256):record(0,bytes(rng.randrange(1,256) for _ in range(rng.randrange(0,128))))
# Find a real signed-char hash collision, then execute both names in the actual lookup.
seen={};collision=None
for i in range(300000):
    s=('Alias'+str(i)).encode();h=expected_hash(s)
    if h in seen:collision=(seen[h],s,h);break
    seen[h]=s
assert collision
names=[b'OnTimer',b'OnInit',b'Unmapped',b'',b'\xff',collision[0],collision[1]]
def inspect():
    for name in names:record(1,name);record(2,name)
inspect()
for name,value in ((b'OnTimer',b'first'),(b'OnInit',b'OnTimer'),(b'',b'empty-key'),(b'\xff',b'high-byte'),(collision[0],b'collision')):
    record(3,name,value);inspect()
record(4);record(3,b'OnTimer',b'second');record(3,b'OnTimer',b'third');record(3,b'Unmapped',b'new');record(3,b'\xff',b'');inspect();record(5);inspect()
# Empty prior value is removed, even when the old entry existed.
record(3,b'OnTimer',b'');record(4);record(3,b'OnTimer',b'nonempty');record(5);inspect()
# Push is not a nested stack: it discards the earlier backup.
record(4);record(3,b'OnTimer',b'outer');record(4);record(3,b'OnTimer',b'inner');record(5);inspect();record(5);inspect()
for k0 in range(9):
 for k1 in range(9):record(3,b'OnInit',b'guard',k0,k1);record(1,b'OnInit')
for count in (0,1,2,3):record(3,b'OnTimer',b'count',count=count);record(1,b'OnTimer')
for _ in range(240):
    op=rng.choice((1,2,3,3,3,4,5));name=rng.choice(names);value=rng.choice((b'',b'OnTimer',b'one',b'\xff\x80',b'first\0ignored'))
    record(op,name,value);inspect()
gold=HERE/'alias-reference.bin';gold.write_bytes(b'FAL1'+words(len(records))+b''.join(records))
# Original Call executes _GetFuncName then the actual Instance::pCall(name)
# which performs a fresh globals lookup. VM pCall is the explicit boundary here.
action(6);action(3,b'OnTimer',b'Renamed');action(3,b'Renamed',b'Other')
c.pointer(owner+8,L);c.pointer(returns+0x24,return_vector);c.uc.mem_write(return_vector,words(0,0,0))
for name in (b'OnTimer',b'Unmapped',b'Renamed'):
    c.uc.mem_write(text,name+b'\0');c.invoke(0x37c390,[owner,text,args,returns])
assert live_calls==[{'name_hex':b'Renamed'.hex(),'requested_identity':False},
                   {'name_hex':b'Unmapped'.hex(),'requested_identity':True},
                   {'name_hex':b'Other'.hex(),'requested_identity':False}]
report={'validation':'PASS','original_sha256':sha(engine),'script_sha256':sha(Path(__file__)),
        'manifest_sha256':sha(HERE/'original-functions.json'),'assembly_sha256':sha(HERE/'reference/original-functions.asm'),
        'gold_sha256':sha(gold),'cases':len(rows),'mismatches':0,'collision':{'first':collision[0].decode(),'second':collision[1].decode(),'hash':collision[2]},
        'explicit_services':['C++ std::string temporary construction/allocation','map::operator[]/erase/clear allocation and tree shape','strlen and zero-length memcmp','lazy hash initializer guard'],
        'scope':'Actual original hash byte loop, unsigned tree lookup, VFTable producer guards/backup/copy/pop traversal; no original allocator or full LuaManager execution.',
        'original_call_globals_lookups':live_calls,'observations':rows}
(HERE/'original-probe.json').write_text(json.dumps(report,indent=2)+'\n')
print(json.dumps({k:report[k] for k in ('validation','cases','mismatches','collision','gold_sha256')}))
