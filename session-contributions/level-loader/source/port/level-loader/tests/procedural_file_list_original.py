"""Execute original GetFiles tokenization with supplied immutable file bytes.

File open/size/read/close and vector push_back are observed environment services.
Original string creation, searches, substring lengths and termination execute
ARM instructions. No filename parser result is supplied by a hook.
"""
import argparse,hashlib,json,pathlib,struct,sys,zipfile
ap=argparse.ArgumentParser()
ap.add_argument('--engine',type=pathlib.Path,required=True)
ap.add_argument('--dependency-root',type=pathlib.Path,required=True)
ap.add_argument('--cache',type=pathlib.Path,required=True)
ap.add_argument('--out',type=pathlib.Path,required=True)
a=ap.parse_args();sys.path.insert(0,str(a.dependency_root))
sys.path.insert(0,str(pathlib.Path(__file__).parent))
from xml_original_probe import Parser
from unicorn import UC_HOOK_CODE
assert hashlib.sha256(a.engine.read_bytes()).hexdigest()=='36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80'
class FileList(Parser):
    def __init__(self,raw):
        super().__init__(a.engine,{'functions':[]});self.raw=raw;self.rows=[];self.reads=[]
        app=self.data+0x30000;system=self.data+0x31000;manager=self.data+0x32000
        manager_vt=self.data+0x33000;self.stream=self.data+0x34000;stream_vt=self.data+0x35000
        self.open=self.data+0x50000;self.size=self.open+16;self.read=self.open+32;self.close=self.open+48
        for address in (self.open,self.size,self.read,self.close):self.uc.mem_write(address,struct.pack('<I',0xe12fff1e))
        self.pointer(0x994a98+0x37f4,app);self.pointer(app+0x10,system);self.pointer(system+0x34,manager)
        self.pointer(manager,manager_vt);self.pointer(manager_vt+0x88,self.open);self.pointer(manager_vt+0x78,self.close)
        self.pointer(self.stream,stream_vt);self.pointer(stream_vt+8,self.size);self.pointer(stream_vt+0x18,self.read)
        self.uc.hook_add(UC_HOOK_CODE,self.allocation)
        self.uc.hook_add(UC_HOOK_CODE,self.boundary)
    def boundary(self,uc,address,size,unused):
        if address==self.open:self.reads.append(self.text(self.reg(1)));self.returned(self.stream)
        elif address==self.size:self.put(1,0);self.returned(len(self.raw))
        elif address==self.read:
            assert self.reg(2)==len(self.raw)
            if self.raw:uc.mem_write(self.reg(1),self.raw)
            self.returned(len(self.raw))
        elif address==self.close:self.returned()
        elif address==0x32bad8:
            self.rows.append(self.text(self.word(self.reg(1)+0x14)));self.returned()
    def run(self):
        name=self.data+0x36000;self.uc.mem_write(name,b'mgxlist.txt\0')
        self.invoke(0x488904,[self.data+0x37000,name,self.data+0x38000],budget=2000000)
        return {'rows':self.rows,'reads':self.reads}
cases=[b'',b'one.mgx\r\n',b'one.mgx\n',b'one.mgx',b'one.mgx\r\ntwo.mgx\r\n',
    b'one.mgx\r\ntwo.mgx',b'\r\n',b'\n',b'a\r\r\n',b'a\0hidden\r\n',b'a\nb\n']
with zipfile.ZipFile(a.cache) as pack:
    prefix='com.gameloft.android.GAND.GloftD2SS/files/'
    authored=[(n[len(prefix):],pack.read(n)) for n in pack.namelist() if n.startswith(prefix) and n.endswith('/mgx/mgxlist.txt')]
rows=[];probe=FileList(b'')
for name,raw in [('case-'+str(i),v) for i,v in enumerate(cases)]+authored:
    probe.raw=raw;probe.rows=[];probe.reads=[]
    try:rows.append({'name':name,'input_hex':raw.hex(),'input_sha256':hashlib.sha256(raw).hexdigest(),**probe.run()})
    except Exception as e:
        rows.append({'name':name,'failure':str(e),'observed_rows':probe.rows});break
report={'engine_sha256':hashlib.sha256(a.engine.read_bytes()).hexdigest(),
    'cache_sha256':hashlib.sha256(a.cache.read_bytes()).hexdigest(),'scope':__doc__,
    'script_sha256':hashlib.sha256(pathlib.Path(__file__).read_bytes()).hexdigest(),'cases':rows,
    'original_execution_complete':not any('failure' in row for row in rows),
    'layout_generation_verified':False,'full_loader_verified':False}
a.out.write_text(json.dumps(report,indent=2)+'\n')
print(json.dumps({'cases':len(rows),'authored_lists':len(authored),'complete':report['original_execution_complete'],
    'edge_cases':[row for row in rows if row['name'].startswith('case-')],
    'failures':[row for row in rows if 'failure' in row]},indent=2))
raise SystemExit(0 if report['original_execution_complete'] else 1)
