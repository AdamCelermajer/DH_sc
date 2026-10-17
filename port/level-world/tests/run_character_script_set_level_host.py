"""Character _SetLevel260B caller vs original ARM, with named real dependencies."""
from __future__ import annotations
import argparse,hashlib,json,math,os,random,shutil,struct,subprocess,sys
from pathlib import Path
MODULE=Path(__file__).resolve().parents[1];ROOT=MODULE.parents[1]
MANIFEST=MODULE/'reference/character-script-set-level/original-functions.json'
SHA='36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80'
C,ARGS,VECTOR,VALUES,A1,A2,M1,M2,M3=0x10014000,0x10018000,0x10019000,0x10020000,0x10030000,0x10031000,0x10032000,0x10033000,0x10034000

def digest(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def float_bits(value):return struct.unpack('<I',struct.pack('<f',value))[0]
def number(word):return struct.unpack('<f',struct.pack('<I',word))[0]
def fixtures():
    rows=[]
    def add(name,n1=2560,n2=None,max1=100,max2=None,mutation=0,count=1,type_value=3):
        rows.append((name,[count,type_value,float_bits(n1),float_bits(n1 if n2 is None else n2),max1&0xffffffff,(max1 if max2 is None else max2)&0xffffffff,mutation,0x12345678]))
    for value in (0,-0.0,1,-1,255.9,-256.75,2560,25599.75,25600,25600.9,25601,99999,-2147483648,2147483520):add('number_'+str(value),value)
    for maximum in (0,1,100,-1,0x800000,0x1000000,0x80000000,0x7fffffff):
        add('limit_wrap_'+str(maximum),max1=maximum,max2=maximum^0x03000001)
    add('fresh_second_number_above_limit',25600,99999)
    add('fresh_second_number_negative',25600,-257.75)
    add('fresh_second_limit_negative',25601,max2=-1)
    add('fresh_second_limit_zero',25601,max2=0)
    for mutation in range(1,7):
        for upper in (False,True):add('mutation_'+str(mutation)+'_'+str(upper),25601 if upper else 2560,mutation=mutation)
    add('empty',count=0)
    for type_value in (0,1,2,4,5,6,7,0xffffffff):add('unsupported_initial_type_'+str(type_value),type_value=type_value)
    for count in (2,3,7):add('extra_args_'+str(count),count=count)
    rng=random.Random(0x3b73a4)
    for i in range(24):
        add('random_'+str(i),rng.randrange(-100000,100000)+0.75,rng.randrange(-100000,100000)+0.5,rng.getrandbits(32),rng.getrandbits(32),rng.randrange(7))
    return rows

def oracle(original,exe):
    from elftools.elf.elffile import ELFFile
    from unicorn import UC_HOOK_CODE,UC_HOOK_MEM_WRITE
    sys.path.insert(0,str(ROOT/'port/engine-resources/tests'));from cpu import Cpu
    from elf_import_identity import verify_imports
    verified=verify_imports(original,{0x30e4cc:'__aeabi_f2iz'})
    raw=original.read_bytes();assert digest(original)==SHA
    manifest=json.loads(MANIFEST.read_text())
    with original.open('rb') as stream:
        elf=ELFFile(stream);symbols={s.name:s for s in elf.get_section_by_name('.symtab').iter_symbols()}
        loads=[s for s in elf.iter_segments() if s['p_type']=='PT_LOAD']
        def data(at,n):
            s=next(s for s in loads if s['p_vaddr']<=at and at+n<=s['p_vaddr']+s['p_filesz'])
            offset=int(s['p_offset'])+at-int(s['p_vaddr']);return raw[offset:offset+n]
        for row in manifest['functions']:
            at,n=int(row['elf_address'],0),row['size'];symbol=symbols[row['original_symbol']]
            assert (int(symbol['st_value']),int(symbol['st_size']))==(at,n)
            assert hashlib.sha256(data(at,n)).hexdigest()==row['sha256']
        for row in manifest['design_literals']:
            at=(int(row['pc_add_address'],0)+8+struct.unpack('<I',data(int(row['literal_address'],0),4))[0])&0xffffffff
            text=row['text'].encode()+b'\0';assert at==int(row['elf_address'],0) and data(at,len(text))==text
            assert hashlib.sha256(text).hexdigest()==row['sha256']
        app=manifest['application_got'];base=(int(app['base_pc_add'],0)+8+struct.unpack('<I',data(int(app['base_literal'],0),4))[0])&0xffffffff
        slot=base+struct.unpack('<I',data(int(app['symbol_literal'],0),4))[0];assert slot==int(app['slot'],0)
        assert struct.unpack('<I',data(slot,4))[0]==int(app['target'],0)
        assert int(symbols[app['symbol_names'][0]]['st_value'])==int(app['target'],0)
    old=Cpu(original,False,manifest);old.uc.mem_map(0x10000000,0x60000)
    def get(at):return struct.unpack('<I',old.uc.mem_read(at,4))[0]
    def returned(value):old.put(0,value);old.uc.reg_write(old.pc,old.uc.reg_read(old.lr))
    def string(at):
        raw=bytearray()
        while old.uc.mem_read(at,1)!=b'\0':raw+=old.uc.mem_read(at,1);at+=1
        return raw.decode('ascii')
    records=[]
    for name,x in fixtures():
        count,type_value,num1,num2,max1,max2,mutation,base_level=x
        old.uc.mem_write(old.stack,bytes(0x10000));old.uc.mem_write(C,bytes(0x1000));old.pointer(C+0x5b8,base_level)
        old.uc.mem_write(VALUES,bytes(0x400));old.pointer(VALUES+4,type_value)
        old.pointer(ARGS+4,VECTOR);old.pointer(VECTOR,VALUES);old.pointer(VECTOR+4,VALUES+112*count);old.pointer(VECTOR+8,VALUES+112*count)
        old.pointer(slot,A1);old.pointer(A1+0x2c,M1);old.pointer(A2+0x2c,M3)
        calls=[];number_calls=[0];design_calls=[0];source_store=[False];clamped=[False];actual_index_calls=[]
        def mutate(op):
            if mutation==1 and op==0 and number_calls[0]==1:old.pointer(slot,A2)
            if mutation==2 and op==1 and design_calls[0]==1:old.pointer(slot,A2)
            if mutation==3 and op==1 and design_calls[0]==1:old.pointer(A1+0x2c,M2)
            if mutation==4 and op==1 and design_calls[0]==1:nonlocal_num[0]=float_bits(-257.75)
            if mutation==5 and op==3:old.pointer(C+0x5b8,0xaabbccdd)
            if mutation==6 and op==4:old.pointer(C+0x5b8,0xdeadbeef)
        nonlocal_num=[num2]
        def call(op,subject=0,argument=0,bits=0,value=0):
            calls.append([op,subject,argument,bits,value,get(C+0x5b8)]);mutate(op);returned(value)
        def observe(_,at,__,___):
            if at==0x37baf8:
                assert old.reg(0)==ARGS and old.reg(1)==0;actual_index_calls.append([ARGS,0])
            elif at==0x31bbf0:
                assert old.reg(0)==VALUES;number_calls[0]+=1;value=num1 if number_calls[0]==1 else nonlocal_num[0]
                call(0,ARGS,value=value)
            elif at==0x4c4bdc:
                assert old.reg(0) in (M1,M2,M3)
                assert string(old.reg(1))=='CharacterDesign' and string(old.reg(2))=='MaxLevelDVeryHard'
                design_calls[0]+=1;call(1,old.reg(0),value=max1 if design_calls[0]==1 else max2)
            elif at==0x30e4cc:
                bits=old.reg(0);value=number(bits);assert math.isfinite(value) and -2147483648<=value<2147483648
                call(2,bits=bits,value=int(value)&0xffffffff)
            elif at==0x3e0810:
                assert old.reg(0)==C+0x560 and old.reg(1)==1;call(3,C+0x560,1)
            elif at==0x3bdca4:
                assert old.reg(0)==C and old.reg(1)==0xffffffff;call(4,C,0xffffffff)
            elif at==0x3bdbb8:
                assert old.reg(0)==C and old.reg(1)==0xffffffff;call(5,C,0xffffffff)
            elif at==0x3b7480:clamped[0]=True
        def store(_,kind,at,size,value,unused):
            if at==C+0x5b8 and old.uc.reg_read(old.pc)==0x3b7454:assert size==4;source_store[0]=True
        code=old.uc.hook_add(UC_HOOK_CODE,observe);write=old.uc.hook_add(UC_HOOK_MEM_WRITE,store)
        old.invoke(0x3b73a4,[ARGS,0,C]);old.uc.hook_del(code);old.uc.hook_del(write)
        expected={'status':0,'base_level':get(C+0x5b8),'applied':int(source_store[0]),'clamped':int(clamped[0]),'calls':calls}
        actual=json.loads(subprocess.check_output([str(exe),*map(str,x)],text=True));assert actual==expected,(name,actual,expected)
        assert len(actual_index_calls)==number_calls[0]
        records.append({'case':name,'input':x,'matched':True,'source_result':expected,'compiled_result':actual,'actual_argument_index_calls':actual_index_calls})
    return {'validation':'PASS','comparisons':len(records),'mismatches':0,'results':records,'verified_imports':verified,
        'executed_scope':'Complete original260B Character::_SetLevel and actual88B Arguments::operator[] instructions for valid retained vectors. Value::getNumber144B, getConstant84B, RecalcProperties68B, RegenHP236B and RegenMP236B are explicitly named borrowed provider fixtures. Original f2iz PLT identity is independently executed/verified; its numerical body is modeled only for finite representable binary32 values with truncation toward zero. Nonfinite/out-of-range dependency failures are host-only port boundaries, not original behavior claims. Caller type/count gates, fresh calls, signed branch, wrap32 shift8, baseword store and final calls execute original instructions. No full property recalculation/HP/MP/VM body or native wiring claimed.',
        'native_wired':False}

def main():
    parser=argparse.ArgumentParser(description=__doc__);parser.add_argument('--compiler');parser.add_argument('--original-elf',type=Path,required=True)
    parser.add_argument('--cache',type=Path,default=ROOT.parent/'cache/files');parser.add_argument('--output',type=Path,default=MODULE/'build/character-script-set-level/host.exe')
    parser.add_argument('--report',type=Path,default=MODULE/'build/character-script-set-level/validation.json');args=parser.parse_args()
    compiler=args.compiler or os.environ.get('CXX') or shutil.which('g++')
    if not compiler:parser.error('pass --compiler')
    exe=args.output.resolve();exe.parent.mkdir(parents=True,exist_ok=True)
    sources=[MODULE/'character_script_set_level.cpp',MODULE/'tests/character_script_set_level.cpp']
    command=[compiler,'-std=c++17','-O1','-Wall','-Wextra','-Werror','-pedantic',*map(str,sources),'-o',str(exe)]
    built=subprocess.run(command,cwd=ROOT,capture_output=True,text=True)
    if built.returncode:raise RuntimeError(built.stdout+built.stderr)
    host=json.loads(subprocess.check_output([str(exe)],text=True));assert host['validation']=='PASS' and host['host_cases']==92
    comparison=oracle(args.original_elf.resolve(),exe);assert comparison['comparisons']==74 and comparison['mismatches']==0
    manifest=json.loads(MANIFEST.read_text());constant=manifest['authored_design_constant'];input_file=args.cache.resolve()/constant['path']
    assert input_file.stat().st_size==constant['bytes'] and digest(input_file)==constant['sha256']
    reader_report=ROOT/constant['existing_original_reader_report'];assert digest(reader_report)==constant['existing_report_sha256']
    decoded=json.loads(reader_report.read_text());assert decoded['original_sha256']==SHA
    row=next(row for row in decoded['files'] if row['path']==constant['path']);assert row['fully_consumed'] and row['sha256']==digest(input_file)
    assert constant['constant'] in row['rows'] and constant['constant']['value']==100
    paths=sources+[MODULE/'character_script_set_level.hpp',Path(__file__).resolve(),MANIFEST,MANIFEST.with_name('NOTES.md')]
    report={'validation':'PASS','host_report':host,'original_arm_comparison':comparison,'original_sha256':SHA,'compiler_command':command,
        'source_sha256':{p.relative_to(ROOT).as_posix():digest(p) for p in paths},'native_wired':False,'new_full_dependency_bodies':0,
        'verified_import_helper_sha256':digest(MODULE/'tests/elf_import_identity.py'),'authored_design_constant':constant}
    args.report.parent.mkdir(parents=True,exist_ok=True);args.report.write_text(json.dumps(report,indent=2)+'\n',encoding='utf-8')
    print(json.dumps({'validation':'PASS','host_cases':host['host_cases'],'original_arm_cases':comparison['comparisons'],'mismatches':0}))
if __name__=='__main__':main()
