"""Resolve original PLT identities by executing each relocated stub.

No floating-point results are supplied here. This independent gate identifies
the imported function before a component oracle models its numeric behavior.
"""
import sys
from pathlib import Path
ROOT=Path(__file__).resolve().parents[3]
sys.path.insert(0,str(ROOT/'port/engine-resources/tests'))
from cpu import Cpu

class ImportProbe(Cpu):
    def external(self,uc,address,size,unused):
        self.import_names.append(self.imports[address])
        self.put(0,0)
        uc.reg_write(self.pc,uc.reg_read(self.lr))

def verify_imports(original,expected):
    probe=ImportProbe(Path(original),False,{'functions':[]})
    result={}
    for address,name in expected.items():
        probe.import_names=[]
        probe.invoke(address,[0x3f800000,0x3f800000])
        assert probe.import_names==[name],(hex(address),probe.import_names,name)
        result[hex(address)]=name
    return result
