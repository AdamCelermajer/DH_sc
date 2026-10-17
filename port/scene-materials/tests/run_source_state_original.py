"""Independently execute the original state converter on host-produced inputs."""
import argparse,hashlib,json,sys
from pathlib import Path
ROOT=Path(__file__).resolve().parents[3]
sys.path.insert(0,str(ROOT/'port/engine-resources/tests'))
from cpu import Cpu
from elftools.elf.elffile import ELFFile
SHA='36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80'
SYMBOL='_ZN6glitch5video6detail10renderpass12SRenderStateC1ERKNS0_12SRenderStateE'
def digest(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def main():
    p=argparse.ArgumentParser(description=__doc__)
    p.add_argument('--original-elf',type=Path,required=True)
    p.add_argument('--inputs',type=Path,required=True)
    p.add_argument('--report',type=Path,required=True)
    a=p.parse_args();assert digest(a.original_elf)==SHA
    with a.original_elf.open('rb') as f:
        elf=ELFFile(f);symbol=next(s for s in elf.get_section_by_name('.symtab').iter_symbols() if s.name==SYMBOL)
        assert (int(symbol['st_value']),int(symbol['st_size']))==(0x5d7a10,564)
    manifest={'functions':[{'original_symbol':SYMBOL,'elf_address':'0x5d7a10','size':564}]}
    original=Cpu(a.original_elf,False,manifest);src,dst=original.data+0x100,original.data+0x200
    inputs=json.loads(a.inputs.read_text());assert len(inputs['cases'])>=36
    cases=[]
    for row in inputs['cases']:
        source=bytes.fromhex(row['source_hex']);expected=bytes.fromhex(row['expected_pass_hex'])
        assert len(source)==0x4c and len(expected)==32
        original.uc.mem_write(src,source);original.uc.mem_write(dst,b'\xff'*32)
        original.invoke(SYMBOL,[dst,src])
        actual=bytes(original.uc.mem_read(dst,32));assert actual==expected,(row['case'],actual.hex(),expected.hex())
        cases.append({**row,'original_pass_hex':actual.hex(),'matched':True})
    report={'validation':'PASS','original_sha256':SHA,'original_arm_cases':len(cases),'mismatches':0,
        'function':{**manifest['functions'][0],'sha256':hashlib.sha256(bytes(original.uc.mem_read(0x5d7a10,564))).hexdigest()},
        'inputs_sha256':digest(a.inputs),'runner_sha256':digest(Path(__file__).resolve()),'cases':cases,
        'scope':'Actual original564B conversion instructions execute on36 designed/seeded random76B inputs; every32B host output matches exactly. No modeled arithmetic, bit packing or caller result. BRES payload/profile/name association and Android renderer binding require separate proofs.'}
    a.report.parent.mkdir(parents=True,exist_ok=True);a.report.write_text(json.dumps(report,indent=2)+'\n')
    print(json.dumps({k:report[k] for k in ('validation','original_arm_cases','mismatches')}))
if __name__=='__main__':main()
