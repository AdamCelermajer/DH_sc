"""Capture original generation instruction bodies and its distribution table."""
import argparse,hashlib,json,pathlib,struct,subprocess,sys
ap=argparse.ArgumentParser()
for name in ('engine','dependency-root','objdump'):ap.add_argument('--'+name,type=pathlib.Path,required=True)
a=ap.parse_args();sys.path.insert(0,str(a.dependency_root))
from elftools.elf.elffile import ELFFile
raw=a.engine.read_bytes();digest=hashlib.sha256(raw).hexdigest()
assert digest=='36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80'
out=pathlib.Path(__file__).resolve().parents[1]/'reference/procedural-generation';out.mkdir(exist_ok=True)
with a.engine.open('rb') as file:
    elf=ELFFile(file);symbols={s.name:s for s in elf.get_section_by_name('.dynsym').iter_symbols()}
    def read(address,size):
        for segment in elf.iter_segments():
            if segment['p_type']=='PT_LOAD' and segment['p_vaddr']<=address and address+size<=segment['p_vaddr']+segment['p_filesz']:
                offset=segment['p_offset']+address-segment['p_vaddr'];return raw[offset:offset+size]
        raise AssertionError(hex(address))
    table_symbol=symbols['_ZN3rnd14gDistributionsE'];base=table_symbol['st_value'];size=table_symbol['st_size']
    assert base==0x8ce5c8 and size==6*6*121*6
    table=read(base,size);(out/'distributions.bin').write_bytes(table)
    rows=[]
    for exits in range(6):
        for children in range(6):
            offset=exits*4356+children*726;choices=[];sentinel=None
            if not exits or not children:
                rows.append({'exits':exits,'children':children,'offset':offset,'not_read_by_step':True})
                continue
            for i in range(121):
                record=struct.unpack('<6b',table[offset+6*i:offset+6*i+6])
                if record[0]<0:sentinel=i;break
                choices.append(list(record))
            if sentinel is None:
                rows.append({'exits':exits,'children':children,'offset':offset,
                             'unterminated_subtable':True,'raw_hex':table[offset:offset+726].hex()})
                continue
            assert len(choices)<=120
            for choice in choices:
                assert all(0<=index<children for index in choice[:exits])
                # Retain all six bytes, including engine padding/extra entries.
                # Step may grow its exit worklist when a direction mismatches.
                assert all(-1<=index<children for index in choice[exits:]),(exits,children,choice)
            rows.append({'exits':exits,'children':children,'offset':offset,'choices':choices,'sentinel_row':sentinel})
    functions=[]
    for name,symbol in symbols.items():
        address=symbol['st_value'];length=symbol['st_size']
        if not length or symbol['st_info']['type']!='STT_FUNC':continue
        if not (('Array2d' in name and 'rnd' in name) or address in (0x3109e0,0x48bebc,0x48bf14,
                0x488134,0x48e8ac,0x48f954,0x48fd64,0x490304,0x48f47c,0x48f25c,
                0x48f05c,0x491ab0,0x49196c,0x491928,0x4918a8,0x4913d8,
                0x491428,0x4917f8,0x48ad14,0x48ad54,0x48b85c,0x48b8f0,0x48ba58,
                0x48c31c,0x48e628,0x48e83c,0x48ddf8,0x48bd0c,0x48e690,
                0x4919b0,0x491440,0x48ec74)):continue
        path=out/(f'{address:08x}.asm')
        result=subprocess.run([str(a.objdump),'-d',f'--start-address={address}',f'--stop-address={address+length}',str(a.engine)],
                              capture_output=True,check=True)
        path.write_bytes(result.stdout)
        functions.append({'symbol':name,'address':address,'size':length,'file':path.name,
                          'instruction_bytes_sha256':hashlib.sha256(read(address,length)).hexdigest(),
                          'disassembly_sha256':hashlib.sha256(result.stdout).hexdigest()})
report={'scope':__doc__,'engine_sha256':digest,
        'script_sha256':hashlib.sha256(pathlib.Path(__file__).read_bytes()).hexdigest(),
        'objdump_sha256':hashlib.sha256(a.objdump.read_bytes()).hexdigest(),
        'distribution':{'symbol':table_symbol.name,'address':base,'size':size,
                        'sha256':hashlib.sha256(table).hexdigest(),'dimensions':[6,6,121,6],'rows':rows},
        'functions':functions,'generation_verified':False}
(out/'provenance.json').write_text(json.dumps(report,indent=2)+'\n')
print(json.dumps({'functions':len(functions),'distribution_bytes':size,'table_dimensions':[6,6,121,6],
                  'distribution_choices':sum(len(row.get('choices',[])) for row in rows)}))
