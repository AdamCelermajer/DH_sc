from pathlib import Path
import sys,json,hashlib
root=Path(__file__).resolve().parents[3]
sys.path.insert(0,r'C:/Users/adamc/AppData/Local/uv/cache/archive-v0/jc8RoeQ1US_9Gkmi/Lib/site-packages')
sys.path.insert(0,str(root/'port/engine-animation/tests'))
from particle_factory_differential import FactoryCpu
from unicorn import UC_HOOK_CODE
c=FactoryCpu(root/'.local-inputs/libDungeonHunter2.so',False,{'functions':[]})
obj=c.data+4096;fs=obj+256;vt=fs+256;op=vt+256
trace=[]
def ret(v=0):c.put(0,v);c.uc.reg_write(c.pc,c.uc.reg_read(c.lr))
def hook(uc,a,n,user):
 if a==0x8945a4:trace.append('filesystem');ret(fs)
 elif a==op:trace.append('actual_file_open_NULL');ret(0)
c.uc.hook_add(UC_HOOK_CODE,hook)
c.pointer(fs,vt);c.pointer(vt+8,op)
c.pointer(obj+24,obj+64);c.pointer(obj+28,obj+80)
c.pointer(obj+4,999)
c.invoke(0x888a60,[obj])
assert trace==['filesystem','actual_file_open_NULL']
assert int.from_bytes(c.uc.mem_read(obj+4,4),'little')==0
c.invoke(0x888aec,[obj]);assert c.reg(0)==0
out={'validation':'PASS','cases':2,'original_sha256':hashlib.sha256((root/'.local-inputs/libDungeonHunter2.so').read_bytes()).hexdigest(),'trace':trace,'scope':'Whole original StreamCFile::Init with explicit filesystem OpenFile NULL boundary; whole CreateNewCursor proves NULL from constructor-backed zero size. LoadDataSource invalid-handle and Play IsReady early-return are preserved original body inspection, not a whole Vox runtime differential.'}
(root/'port/engine-ui/reference/menu-sound-v4/missing-file-original-proof.json').write_text(json.dumps(out,indent=2))
print(json.dumps(out))
