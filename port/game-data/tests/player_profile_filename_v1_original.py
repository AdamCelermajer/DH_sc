"""Replay original filename caller; declare libc/string assignment leaves."""
import argparse
import hashlib
import json
import struct
from pathlib import Path
from elftools.elf.elffile import ELFFile
from unicorn import Uc, UC_ARCH_ARM, UC_MODE_ARM, UC_HOOK_CODE
from unicorn.arm_const import UC_ARM_REG_R0,UC_ARM_REG_R1,UC_ARM_REG_R2,UC_ARM_REG_R3,UC_ARM_REG_SP,UC_ARM_REG_LR,UC_ARM_REG_PC

def capture(elf_path):
    with elf_path.open('rb') as f:
        elf=ELFFile(f)
        segments=[(int(s['p_vaddr']),int(s['p_memsz']),s.data()) for s in elf.iter_segments() if s['p_type']=='PT_LOAD']
        symbols={s.name:(int(s['st_value']),int(s['st_size'])) for s in elf.get_section_by_name('.dynsym').iter_symbols()}
    def raw(address,size):
        for base,_,data in segments:
            if base<=address and address+size<=base+len(data):return data[address-base:address-base+size]
        raise ValueError(hex(address))
    names=['_ZN14PlayerSavegame14SG_GetFilenameEjRSsbb','_ZN14PlayerSavegame20SG_GetFilenamePrefixEv','_ZN14PlayerSavegame23SG_GetFilenameExtensionEv','_ZN14PlayerSavegame33SG_GetCheckpointFilenameExtensionEv']
    functions=[{'symbol':n,'address':hex(symbols[n][0]),'size':symbols[n][1],'sha256':hashlib.sha256(raw(*symbols[n])).hexdigest()} for n in names]
    start=min(x[0] for x in segments)&~4095
    end=(max(base+size for base,size,_ in segments)+4095)&~4095
    slots=[0,1,2,9,10,41,99,100,255,256,998,999,1000,1001,4095,4096,32767,32768,65535,65536,99999,100000,999999,1000000,16777215,16777216,2147483646,2147483647,2147483648,4294967293,4294967294,4294967295]
    cases=[]
    for slot in slots:
        for checkpoint in (0,1):
            for multiplayer in (0,1):
                uc=Uc(UC_ARCH_ARM,UC_MODE_ARM);uc.mem_map(start,end-start)
                for base,_,data in segments:uc.mem_write(base,data)
                stop=0x70000000;stack=0x71000000;output=0x72000000
                for base in (stop,stack,output):uc.mem_map(base,0x10000)
                trace=[];result=[]
                def cstring(pointer):
                    data=bytes(uc.mem_read(pointer,256));return data.split(b'\0')[0]
                def hook(machine,address,size,context):
                    if address==stop:machine.emu_stop();return
                    if address==0x30eae4:
                        destination=machine.reg_read(UC_ARM_REG_R0);fmt=cstring(machine.reg_read(UC_ARM_REG_R1))
                        prefix=cstring(machine.reg_read(UC_ARM_REG_R2));number=machine.reg_read(UC_ARM_REG_R3)
                        sp=machine.reg_read(UC_ARM_REG_SP);extra,extension=struct.unpack('<II',machine.mem_read(sp,8))
                        assert fmt==b'%s%03u%s%s'
                        data=fmt%(prefix,number,cstring(extra),cstring(extension))
                        trace.append({'leaf':'sprintf','format':fmt.decode(),'slot':number,'checkpoint_suffix':cstring(extra).decode(),'extension':cstring(extension).decode()})
                        machine.mem_write(destination,data+b'\0');machine.reg_write(UC_ARM_REG_R0,len(data));machine.reg_write(UC_ARM_REG_PC,machine.reg_read(UC_ARM_REG_LR))
                    elif address==0x30de54:
                        machine.reg_write(UC_ARM_REG_R0,len(cstring(machine.reg_read(UC_ARM_REG_R0))));machine.reg_write(UC_ARM_REG_PC,machine.reg_read(UC_ARM_REG_LR))
                    elif address==0x3109e0:
                        begin=machine.reg_read(UC_ARM_REG_R1);finish=machine.reg_read(UC_ARM_REG_R2)
                        assert machine.reg_read(UC_ARM_REG_R0)==output and finish>=begin
                        result.append(bytes(machine.mem_read(begin,finish-begin)).decode());machine.reg_write(UC_ARM_REG_PC,machine.reg_read(UC_ARM_REG_LR))
                uc.hook_add(UC_HOOK_CODE,hook)
                uc.reg_write(UC_ARM_REG_SP,stack+0x8000);uc.reg_write(UC_ARM_REG_LR,stop)
                for register,value in [(UC_ARM_REG_R0,slot),(UC_ARM_REG_R1,output),(UC_ARM_REG_R2,checkpoint),(UC_ARM_REG_R3,multiplayer)]:uc.reg_write(register,value)
                uc.emu_start(symbols[names[0]][0],stop,count=5000)
                assert len(result)==len(trace)==1,(slot,checkpoint,multiplayer)
                cases.append({'slot':slot,'checkpoint':checkpoint,'multiplayer':multiplayer,'filename':result[0],'source_trace':trace})
    return {'validation':'PASS','original_sha256':hashlib.sha256(elf_path.read_bytes()).hexdigest(),'functions':functions,'cases':cases,'case_count':len(cases),'scope':'Whole original filename caller and literal getters executed. sprintf/strlen/std::string range assignment are declared libc/string leaves. No filesystem, backup recovery or campaign loading claimed.'}

if __name__=='__main__':
    p=argparse.ArgumentParser();p.add_argument('--elf',type=Path,required=True);p.add_argument('--output',type=Path,required=True);a=p.parse_args()
    result=capture(a.elf);a.output.parent.mkdir(parents=True,exist_ok=True);a.output.write_text(json.dumps(result,indent=2)+'\n',encoding='utf-8');print(json.dumps({'validation':result['validation'],'case_count':result['case_count']}))
