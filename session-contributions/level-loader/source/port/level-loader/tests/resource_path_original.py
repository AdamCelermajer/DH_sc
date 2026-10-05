"""Original Level::LoadFile path attempts with missing-file environment service.

The original function performs path construction and stripping itself. The
boundary services expose compiled-data mode and report missing files. No XML,
module selection or object behavior is stubbed into a successful result.
"""
import argparse,hashlib,json,pathlib,struct,sys
ap=argparse.ArgumentParser();ap.add_argument('--engine',type=pathlib.Path,required=True)
ap.add_argument('--dependency-root',type=pathlib.Path,required=True)
ap.add_argument('--out',type=pathlib.Path,required=True);a=ap.parse_args()
engine_sha=hashlib.sha256(a.engine.read_bytes()).hexdigest()
assert engine_sha=='36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80'
sys.path.insert(0,str(a.dependency_root));sys.path.insert(0,str(pathlib.Path(__file__).parent))
from xml_original_probe import Parser
from unicorn import UC_HOOK_CODE
class PathProbe(Parser):
    def __init__(self):
        super().__init__(a.engine,{'functions':[]})
        self.attempts=[];self.mode_queries=[]
        level=self.data+0x30000;application=self.data+0x32000
        system=self.data+0x33000;files=self.data+0x34000;vtable=self.data+0x35000
        self.stub=self.data+0x50000
        self.uc.mem_write(self.stub,struct.pack('<I',0xe12fff1e))
        self.pointer(0x994a98+0x37f4,application)
        self.pointer(application+0x10,system);self.pointer(system+0x34,files)
        self.pointer(files,vtable);self.pointer(vtable+0x88,self.stub)
        self.level=level
        self.uc.hook_add(UC_HOOK_CODE,self.allocation)
        self.uc.hook_add(UC_HOOK_CODE,self.boundary)
    def boundary(self,uc,addr,size,unused):
        if addr==0x320678:
            self.mode_queries.append(self.text(self.reg(1)));self.returned(0)
        elif addr==self.stub:
            self.attempts.append(self.text(self.reg(1)));self.returned(0)
    def run(self,value):
        self.uc.mem_write(self.level,bytes(0x200));self.attempts=[];self.mode_queries=[]
        name=self.data+0x38000;kind=self.data+0x38100
        text=self.data+0x39000;tag=self.data+0x3a000
        self.uc.mem_write(text,value.encode()+b'\0');self.uc.mem_write(tag,b'Level\0')
        self.invoke(0x3140ec,[name,text,0]);self.invoke(0x3140ec,[kind,tag,0])
        result=self.invoke(0x3f3b40,[self.level,name,kind],budget=10000000)
        return {'input':value,'attempts':self.attempts,'mode_queries':self.mode_queries,'result':result}
cases=['001_swamp.mlx','data/iphone/3d/modules/swamp/mgp/obj_4of4_brdwalk_sw_00.mgp',
       'old/debug/ps3/iphone/data/a.mgp','data/iphone/iphone/a.mvp','data/IPP/iphone/a.mvp',
       'data/iphone/3d/modules/swamp/mgp/a.mgp']
rows=[]
for case in cases:
    p=PathProbe()
    try:rows.append(p.run(case))
    except Exception as e:rows.append({'input':case,'failure':str(e),'attempts':p.attempts,'queries':p.mode_queries})
report={'engine_sha256':engine_sha,
        'scope':'Original compiled-data path attempts with all resource reads missing; no successful load inferred.',
        'cases':rows}
a.out.write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(report,indent=2))
