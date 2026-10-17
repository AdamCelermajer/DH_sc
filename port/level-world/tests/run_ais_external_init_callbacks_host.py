"""Execute original AIS initialization leaves and compare typed native callers."""
import argparse
import hashlib
import json
from pathlib import Path
import struct
import subprocess

from elftools.elf.elffile import ELFFile
from unicorn import Uc, UC_ARCH_ARM, UC_MODE_ARM, UC_HOOK_CODE
from unicorn.arm_const import UC_ARM_REG_R0, UC_ARM_REG_R1, UC_ARM_REG_PC, UC_ARM_REG_LR, UC_ARM_REG_SP

MODULE=Path(__file__).resolve().parents[1]
ROOT=MODULE.parents[1]
MANIFEST=MODULE/'reference/ais-external-init-callbacks/original-functions.json'
ORIGINAL_SHA='36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80'
EXTERNAL=(0x3dcea4,0x3dce54,0x3dce44)
DEFAULT=(0x3dbe78,0x3dbe7c,0x3dbe80)
LUA_CALL=0x37c514
STOP=0x1003f000


def sha(raw):return hashlib.sha256(raw).hexdigest()


def original_image(path):
    raw=path.read_bytes();assert sha(raw)==ORIGINAL_SHA
    manifest=json.loads(MANIFEST.read_bytes())
    assert manifest['original_sha256']==ORIGINAL_SHA
    with path.open('rb') as stream:
        elf=ELFFile(stream)
        segments=[s for s in elf.iter_segments() if s['p_type']=='PT_LOAD']
        symbols={s.name:s for s in elf.get_section_by_name('.dynsym').iter_symbols()}
        def at(address,size):
            segment=next(s for s in segments if s['p_vaddr']<=address and address+size<=s['p_vaddr']+s['p_filesz'])
            offset=segment['p_offset']+address-segment['p_vaddr']
            return raw[offset:offset+size]
        code={}
        for row in manifest['functions']:
            address=int(row['elf_address'],0);size=row['size'];symbol=symbols[row['original_symbol']]
            assert (int(symbol['st_value']),int(symbol['st_size']))==(address,size)
            code[address]=at(address,size);assert sha(code[address])==row['sha256']
        strings={}
        for row in manifest['strings']:
            address=int(row['elf_address'],0);payload=at(address,row['size'])
            assert sha(payload)==row['sha256'] and payload==row['text'].encode()+b'\0'
            strings[address]=payload
    return manifest,code,strings


def original_case(code,strings,index,identity,mode):
    uc=Uc(UC_ARCH_ARM,UC_MODE_ARM)
    for page in (0x3db000,0x3dc000,0x37c000,0x8c5000):uc.mem_map(page,4096)
    uc.mem_map(0x10000000,0x40000)
    for address,payload in code.items():uc.mem_write(address,payload)
    for address,payload in strings.items():uc.mem_write(address,payload)
    captured={};instructions=0
    def hook(machine,address,size,context):
        nonlocal instructions
        if address==STOP:machine.emu_stop();return
        if address==LUA_CALL:
            pointer=machine.reg_read(UC_ARM_REG_R1)
            data=bytearray()
            while True:
                value=uc.mem_read(pointer+len(data),1)[0]
                if value==0:break
                data.append(value);assert len(data)<32
            captured.update(ais=machine.reg_read(UC_ARM_REG_R0),name=data.decode())
            if mode:uc.mem_write(0x10000000,struct.pack('<I',0x10002000))
            machine.reg_write(UC_ARM_REG_PC,machine.reg_read(UC_ARM_REG_LR));return
        instructions+=1
    uc.hook_add(UC_HOOK_CODE,hook)
    uc.mem_write(0x10000000,struct.pack('<I',identity))
    uc.reg_write(UC_ARM_REG_R0,identity);uc.reg_write(UC_ARM_REG_LR,STOP);uc.reg_write(UC_ARM_REG_SP,0x1003e000)
    uc.emu_start(EXTERNAL[index] if index<3 else DEFAULT[index-3],STOP+4,count=40)
    assert uc.reg_read(UC_ARM_REG_PC)==STOP
    return {'status':0,'calls':int(bool(captured)),'ais':captured.get('ais',0),
            'name':captured.get('name',''),'live_ais':struct.unpack('<I',uc.mem_read(0x10000000,4))[0],
            'default_init':int(index==0)},instructions


def main():
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--compiler',required=True)
    parser.add_argument('--original-elf',type=Path,required=True)
    parser.add_argument('--output',type=Path,default=MODULE/'build/ais-external-init-callbacks/host.exe')
    parser.add_argument('--report',type=Path,default=MODULE/'build/ais-external-init-callbacks/validation.json')
    args=parser.parse_args();manifest,code,strings=original_image(args.original_elf)
    sources=[MODULE/'ais_external_init_callbacks.cpp',MODULE/'tests/ais_external_init_callbacks.cpp']
    args.output.parent.mkdir(parents=True,exist_ok=True)
    command=[args.compiler,'-std=c++17','-O2','-Wall','-Wextra','-Werror','-pedantic',*map(str,sources),'-o',str(args.output.resolve())]
    subprocess.run(command,cwd=ROOT,check=True,capture_output=True,text=True)
    executable=str(args.output.resolve())
    host=json.loads(subprocess.check_output([executable],text=True));assert host=={'validation':'PASS','host_cases':22}
    rows=[]
    for index in range(3):
        for identity in (0x10001000,0x10001abc,0xfedc1234):
            for mode in (0,1):
                expected,instructions=original_case(code,strings,index,identity,mode)
                actual=json.loads(subprocess.check_output([executable,str(index),str(identity),str(mode)],text=True))
                assert actual==expected,(index,identity,mode,actual,expected)
                rows.append({'callback':index,'identity':identity,'mutation':mode,'matched':True,'instructions':instructions,'result':expected})
    defaults=[]
    for index in range(3,6):
        result,instructions=original_case(code,strings,index,0x10001000,0,)
        assert result['calls']==0 and result['live_ais']==0x10001000 and instructions==1
        defaults.append({'address':hex(DEFAULT[index-3]),'instructions':instructions,'source_empty_body_verified':True})
    paths=sources+[MODULE/'ais_external_init_callbacks.hpp',Path(__file__).resolve(),MANIFEST,MANIFEST.with_name('NOTES.md')]
    report={'validation':'PASS','host_cases':host['host_cases'],'original_arm_cases':len(rows)+len(defaults),
            'mismatches':0,'original_sha256':ORIGINAL_SHA,'compiler_command':command,'results':rows,'default_bodies':defaults,
            'source_sha256':{p.relative_to(ROOT).as_posix():sha(p.read_bytes()) for p in paths},
            'rebuilt_caller_bodies':6,'complete_dependency_bodies':0,'native_wired':False,
            'scope':'Original AISExternal 36/16/16-byte initialization callers and actual AISDefault three 4-byte empty leaves execute. LuaScript::Call is intercepted with exact receiver and literal names. Native caller effects, failures and identity refresh are checked; full LuaScript/Binder/skills/InitScriptProcess are not implied.'}
    args.report.parent.mkdir(parents=True,exist_ok=True);args.report.write_text(json.dumps(report,indent=2)+'\n')
    print(json.dumps({k:report[k] for k in ('validation','host_cases','original_arm_cases','mismatches')}))


if __name__=='__main__':main()
