"""Actual source Value.getString for string/nil/Boolean ChangeAIState keys; numeric/identity formatting remains outside the native binding domain."""
import hashlib,json,math,struct,sys
from pathlib import Path
HERE=Path(__file__).resolve().parent;ROOT=HERE.parents[1];REPO=ROOT.parents[1];sys.path.insert(0,str(ROOT/'tests'))
from character_script_selection_differential import Cpu as Base,string
class Cpu(Base):
 def external(self,uc,address,size,unused):
  if self.imports.get(address)=='__aeabi_fcmpeq':
   a,b=[struct.unpack('<f',struct.pack('<I',self.reg(i)))[0] for i in range(2)];self.put(0,int(a==b));uc.reg_write(self.pc,uc.reg_read(self.lr));return
  return super().external(uc,address,size,unused)
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
c=Cpu(REPO/'.local-inputs/libDungeonHunter2.so',False,{'functions':[]});a=c.data+4096;t=c.data+8192;c.uc.mem_write(t,b'probe\0tail\0');rows=[]
for kind,value in [(0,0),(1,0),(1,0x3f800000),(1,0x80000000),(4,0)]:
 c.uc.mem_write(a,bytes(112));c.pointer(a+4,kind);c.pointer(a+8,value);c.pointer(a+0x20,t);result=c.invoke(0x31c49c,[a]);text=string(c,result).decode();assert text==('nil' if kind==0 else 'probe' if kind==4 else 'false' if value in [0,0x80000000] else 'true');rows.append(dict(type=kind,value_bits=value,key=text))
report=dict(validation='PASS',scope=__doc__,original_sha256=sha(REPO/'.local-inputs/libDungeonHunter2.so'),original_instructions_executed=True,comparisons=len(rows),rows=rows,probe_sha256=sha(Path(__file__)),manifest_sha256=sha(HERE/'dependencies/original-functions.json'),explicit_services=['Imported AEABI float equality; original getBool/getString instructions and literal strings execute.'])
(HERE/'string-key-probe.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(dict(validation='PASS',comparisons=len(rows))))
