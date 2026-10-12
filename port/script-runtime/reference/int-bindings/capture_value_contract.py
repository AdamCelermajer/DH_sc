"""Capture source Value.getString constants and actual imported primitive names."""
import hashlib,json,sys
from pathlib import Path
ROOT=Path(__file__).resolve().parents[4]
sys.path.insert(0,str(ROOT/'port/engine-resources/tests'))
from cpu import Cpu
HERE=Path(__file__).resolve().parent
class Probe(Cpu):
    def external(self,uc,address,size,_):
        self.name=self.imports[address];uc.reg_write(self.pc,uc.reg_read(self.lr))
c=Probe(ROOT/'.local-inputs/libDungeonHunter2.so',False,{'functions':[]})
def word(a):return int.from_bytes(c.uc.mem_read(a,4),'little')
def string(a):
    data=bytes(c.uc.mem_read(a,128));return data.split(b'\0')[0].decode('ascii')
constants={name:string((word(literal)+pc)&0xffffffff) for name,literal,pc in [('nil',0x31c620,0x31c558),('false',0x31c62c,0x31c604),('true',0x31c630,0x31c610),('integer_format',0x31c624,0x31c590),('float_format',0x31c61c,0x31c520),('identity_format',0x31c628,0x31c5c4)]}
imports={}
for address in [0x30ecb8,0x30df8c,0x30e8a4,0x30eae4,0x30de54,0x30e4cc,0x30e2e0]:
    c.invoke(address,[0,0,0,0]);imports[hex(address)]=c.name
source=ROOT/'.local-inputs/libDungeonHunter2.so'
report=dict(original_sha256=hashlib.sha256(source.read_bytes()).hexdigest(),script_sha256=hashlib.sha256(Path(__file__).read_bytes()).hexdigest(),constants=constants,imports=imports)
(HERE/'value-contract.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(report))
