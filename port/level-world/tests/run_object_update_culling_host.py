"""Compare complete ObjectBase culling and remote callers to original ARM.

Camera/online/level providers are named fixture boundaries. Imported binary32
helpers are modeled only after relocated PLT identity verification.
"""
from __future__ import annotations
import argparse, hashlib, json, math, random, shutil, struct, subprocess, sys
from pathlib import Path

MODULE = Path(__file__).resolve().parents[1]
ROOT = MODULE.parents[1]
SHA = '36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80'
MANIFEST = MODULE / 'reference/object-update-culling/original-functions.json'
sys.path.insert(0, str(ROOT / 'port/engine-resources/tests'))
from cpu import Cpu
from unicorn import UC_HOOK_CODE, UC_HOOK_MEM_WRITE
from elf_import_identity import verify_imports

BASE = 0x02000000
OBJECT, BOX, ONLINE, LEVEL = [BASE+x for x in (0x1000,0x3000,0x4000,0x5000)]
CAMERA, ROOT_NODE, ALT_ROOT, FRUSTUM = [BASE+x for x in (0x6000,0x7000,0x8000,0x9000)]
VTABLE, CAMERA_VTABLE, REMOTE_STUB, FRUSTUM_STUB = [BASE+x for x in (0xa000,0xb000,0xc000,0xc010)]

def digest(path): return hashlib.sha256(path.read_bytes()).hexdigest()
def words(values): return struct.pack('<'+'I'*len(values),*[x&0xffffffff for x in values])
def floating(word): return struct.unpack('<f',struct.pack('<I',word))[0]
def bits(value):
    try: return struct.unpack('<I',struct.pack('<f',value))[0]
    except OverflowError: return 0xff800000 if value<0 else 0x7f800000
def same_float(a,b): return a==b or math.isnan(floating(a)) and math.isnan(floating(b))

class OriginalCpu(Cpu):
    def external(self,uc,address,size,unused):
        name=self.imports.get(address)
        if name in ('__aeabi_fcmpge','__aeabi_fcmpgt','__aeabi_fmul','__aeabi_fadd'):
            a,b=floating(self.reg(0)),floating(self.reg(1))
            result=int(a>=b) if name=='__aeabi_fcmpge' else int(a>b) if name=='__aeabi_fcmpgt' else bits(a*b if name=='__aeabi_fmul' else a+b)
            self.import_calls[name]=self.import_calls.get(name,0)+1
            self.put(0,result);uc.reg_write(self.pc,uc.reg_read(self.lr))
        else: super().external(uc,address,size,unused)

def fixtures(count):
    rows=[]
    def add(name,phase=1,online=0,remote=0,level=1,mutation=0,aabb=None,planes=None):
        rows.append((name,[phase,online,remote,level,mutation]+(aabb or [bits(-1.)]*3+[bits(1.)]*3)+(planes or [0]*24)))
    for phase in (0,2,3,128,255): add('direct_phase_'+str(phase),phase=phase)
    add('no_level',level=0)
    for online in (1,128,255):
        for remote in (0,1,0x80000000,0xffffffff):add(f'online_{online}_raw_remote_{remote}',online=online,remote=remote)
    for index in range(6):
        planes=[0]*24;planes[index*4+3]=bits(1.)
        add('reject_plane_'+str(index),planes=planes)
    for sign in range(8):
        normal=[bits(-1. if sign>>axis&1 else 1.) for axis in range(3)]
        add('corner_sign_'+str(sign),planes=(normal+[bits(2.)])*6)
    for d in (0,0x80000000,1,0x80000001,0x7f800000,0xff800000,0x7fc01234,0x7f801234):
        add('distance_boundary_'+hex(d),planes=([0,0,0,d])*6)
    for n in (0,0x80000000,1,0x80000001,0x00800000,0x80800000,0x7f7fffff,0xff7fffff,0x7f800000,0xff800000,0x7fc01234,0x7f801234):
        add('normal_boundary_'+hex(n),planes=([n,n,n,0])*6)
        add('aabb_boundary_'+hex(n),aabb=[n]*6,planes=([bits(1.),bits(-1.),bits(.5),0])*6)
    for mutation in (1,2,4,8,16,32,64,128,256,512,1024,64|128,32|1024):
        add('mutation_'+str(mutation),phase=0 if mutation==1 else 1,online=1 if mutation in (4,8) else 0,mutation=mutation)
    add('getlevel_mutation_then_reject',mutation=32,planes=([0,0,0,bits(1.)])*6)
    add('captured_aabb_ignores_level_mutation',mutation=16,planes=([bits(1.),0,0,0])*6)
    add('getlevel_null_preserves_source_store',level=0,mutation=32)
    rng=random.Random(202610041)
    edge=[0,0x80000000,1,0x80000001,0x00800000,0x7f7fffff,0xff7fffff,0x7f800000,0xff800000,0x7fc01234,0x7f801234]
    for i in range(count):
        pick=lambda: rng.choice(edge) if i%3==0 else rng.getrandbits(32) if i%3==1 else bits(rng.uniform(-300,300))
        add('generated_'+str(i),phase=rng.choice((0,1,1,1,2,255)),online=rng.choice((0,0,1,255)),
            remote=rng.getrandbits(32),level=rng.randrange(2),aabb=[pick() for _ in range(6)],planes=[pick() for _ in range(24)])
    return rows

def original_cases(original,rows):
    manifest=json.loads(MANIFEST.read_bytes());cpu=OriginalCpu(original,False,manifest)
    application=cpu.symbols['_ZN9SingletonI11ApplicationE6s_instE']
    records=[];coverage=set();writes=[];calls=[];captured=[0]*6;last=[0];current=[None]
    def byte(at,value):cpu.uc.mem_write(at,bytes([value&255]))
    def ret(value):cpu.put(0,value);cpu.uc.reg_write(cpu.pc,cpu.uc.reg_read(cpu.lr))
    def hook(uc,at,size,unused):
        if 0x33de90<=at<0x33e038 or 0x33dd10<=at<0x33dd24: coverage.add(at)
        if current[0] is None:return
        phase,online,remote,level,mutation,*rest=current[0]
        if at==0x7fd794:
            calls.append([0,0])
            if mutation&1:byte(OBJECT+0x86,1)
            if mutation&2:cpu.pointer(BOX,bits(-9.))
            ret(ONLINE)
        elif at==REMOTE_STUB:
            assert cpu.reg(0)==OBJECT
            calls.append([1,1])
            if mutation&4:byte(OBJECT+0x86,0)
            if mutation&8:byte(OBJECT+0x86,1)
            ret(remote)
        elif at==0x31f594:
            assert cpu.reg(0)==application
            calls.append([2,2]);sp=cpu.uc.reg_read(cpu.sp)
            captured[:]=[struct.unpack('<I',cpu.uc.mem_read(sp+offset,4))[0] for offset in (0x18,0x14,0x10,0xc,8,4)]
            if mutation&16:cpu.pointer(BOX,bits(100.))
            if mutation&32:byte(OBJECT+0x86,77)
            if mutation&64:cpu.pointer(CAMERA+8,ALT_ROOT)
            ret(LEVEL if level else 0)
        elif at==FRUSTUM_STUB:
            subject=3 if cpu.reg(0)==ROOT_NODE else 4 if cpu.reg(0)==ALT_ROOT else 99
            assert subject!=99
            calls.append([3,subject])
            if mutation&128:cpu.pointer(CAMERA+8,ROOT_NODE)
            if mutation&1024:cpu.pointer(FRUSTUM+12+2*16+12,bits(1.))
            ret(FRUSTUM)
        elif at==0x33dfec:last[0]=cpu.reg(0)
    def write(uc,access,at,size,value,unused):
        if at==OBJECT+0x86:writes.append(value&255)
    code=cpu.uc.hook_add(UC_HOOK_CODE,hook);memory=cpu.uc.hook_add(UC_HOOK_MEM_WRITE,write)
    for name,row in rows:
        cpu.uc.mem_write(BASE,bytes(0xe000));phase,online,remote,level,mutation,*geometry=row
        current[0]=row;writes.clear();calls.clear();captured[:]=[0]*6;last[0]=0
        cpu.pointer(OBJECT,VTABLE);cpu.pointer(VTABLE+0x54,REMOTE_STUB);byte(OBJECT+0x86,phase)
        byte(ONLINE+5,online);cpu.uc.mem_write(BOX,words(geometry[:6]));cpu.uc.mem_write(FRUSTUM+12,words(geometry[6:]))
        cpu.pointer(LEVEL+0x128,CAMERA);cpu.pointer(CAMERA+8,ROOT_NODE)
        cpu.pointer(ROOT_NODE,CAMERA_VTABLE);cpu.pointer(ALT_ROOT,CAMERA_VTABLE);cpu.pointer(CAMERA_VTABLE+0x144,FRUSTUM_STUB)
        raw=cpu.invoke(0x33de90,[OBJECT,BOX])
        records.append({'status':0,'raw':raw,'phase':bytes(cpu.uc.mem_read(OBJECT+0x86,1))[0],
            'phase_writes':len(writes),'planes':cpu.case_planes,'last':last[0], 'aabb':captured.copy(),'calls':[x.copy() for x in calls]})
        # Each plane executes the final compare once. Hooks at that point are
        # accumulated by the per-case instruction counter below.
        cpu.case_planes=0
    current[0]=None
    rng=random.Random(202610042)
    remote_rows=[(word,byte) for word in (0,1,0x80000000,0xfffffffe,0xffffffff) for byte in range(256)]
    remote_rows += [(rng.getrandbits(32),rng.randrange(256)) for _ in range(512)]
    remote_returns=[]
    for word,value in remote_rows:
        cpu.pointer(OBJECT+0x110,word);byte(OBJECT+0x118,value)
        remote_returns.append(cpu.invoke(0x33dd10,[OBJECT]))
    cpu.uc.hook_del(code);cpu.uc.hook_del(memory)
    return records,remote_rows,remote_returns,coverage,cpu.import_calls

def verify_evidence(original):
    from elftools.elf.elffile import ELFFile
    assert digest(original)==SHA
    manifest=json.loads(MANIFEST.read_bytes())
    with original.open('rb') as stream:
        elf=ELFFile(stream);syms={s.name:s for s in elf.get_section_by_name('.symtab').iter_symbols()}
        loads=[x for x in elf.iter_segments() if x['p_type']=='PT_LOAD']
        def data(at,size):
            segment=next(x for x in loads if x['p_vaddr']<=at and at+size<=x['p_vaddr']+x['p_filesz'])
            return segment.data()[at-segment['p_vaddr']:at-segment['p_vaddr']+size]
        for row in manifest['functions']+manifest['external_boundaries']:
            symbol=syms[row['original_symbol']];at=int(row['elf_address'],0);size=row['size']
            assert (symbol['st_value'],symbol['st_size'])==(at,size)
            assert hashlib.sha256(data(at,size)).hexdigest()==row['sha256']
        for row in manifest['camera_virtual_evidence']:
            table=syms[row['vtable_symbol']];assert(table['st_value'],table['st_size'])==(int(row['vtable_address'],0),row['vtable_size'])
            at=int(row['entry_address'],0);assert at==table['st_value']+0x1c+0x144
            assert struct.unpack('<I',data(at,4))[0]==0x582138
            assert hashlib.sha256(data(at,4)).hexdigest()==row['entry_sha256']
        assert struct.unpack('<I',data(0x5837dc,4))[0]==0xe283101c
        assert struct.unpack('<I',data(0x5837e4,4))[0]==0xe5841000
        vptr=manifest['camera_constructor_vptr']
        got_base=(0x58374c+struct.unpack('<I',data(0x583868,4))[0])&0xffffffff
        got_slot=got_base+struct.unpack('<I',data(0x583874,4))[0]
        assert got_slot==int(vptr['vtable_got_entry'],0)
        assert struct.unpack('<I',data(got_slot,4))[0]==int(vptr['vtable_got_value'],0)
        for producer in manifest['field_producers']:
            for instruction in producer['instructions']:
                at=int(instruction['address'],0);body=data(at,4)
                assert struct.unpack('<I',body)[0]==int(instruction['opcode'],0)
                assert hashlib.sha256(body).hexdigest()==instruction['sha256']
    return verify_imports(original,{0x30e4b4:'__aeabi_fcmpge',0x30ed6c:'__aeabi_fmul',0x30eba4:'__aeabi_fadd',0x30e2f8:'__aeabi_fcmpgt'})

def main():
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--original-elf',type=Path,required=True);parser.add_argument('--compiler',default=shutil.which('g++'))
    parser.add_argument('--cases',type=int,default=384)
    parser.add_argument('--build-dir',type=Path,default=MODULE/'build/object-update-culling')
    parser.add_argument('--report',type=Path,default=MODULE/'build/object-update-culling/validation.json')
    args=parser.parse_args();assert args.compiler and args.cases>=0
    imports=verify_evidence(args.original_elf.resolve());args.build_dir.mkdir(parents=True,exist_ok=True)
    exe=(args.build_dir/'host').with_suffix('.exe' if sys.platform=='win32' else '')
    command=[args.compiler,'-std=c++17','-O1','-fno-fast-math','-ffp-contract=off','-Wall','-Wextra','-Werror','-pedantic',str(MODULE/'object_update_culling.cpp'),str(MODULE/'tests/object_update_culling.cpp'),'-o',str(exe)]
    subprocess.run(command,cwd=ROOT,check=True)
    rows=fixtures(args.cases)
    # Count instruction entries independently of Cpu's block coverage.
    OriginalCpu.case_planes=0
    original_external=OriginalCpu.external
    def counted(self,uc,at,size,unused):
        if self.imports.get(at)=='__aeabi_fcmpgt':self.case_planes+=1
        original_external(self,uc,at,size,unused)
    OriginalCpu.external=counted
    expected,remote_rows,remote_expected,coverage,numeric=original_cases(args.original_elf.resolve(),rows)
    run=subprocess.run([str(exe)],input='\n'.join(' '.join(map(str,row)) for _,row in rows)+'\n',text=True,capture_output=True,check=True)
    actual=[json.loads(line) for line in run.stdout.splitlines()];assert len(actual)==len(expected)
    for (name,row),wanted,got in zip(rows,expected,actual):
        assert same_float(wanted['last'],got['last']),(name,wanted,got)
        wanted['last']=got['last']
        assert wanted==got,(name,row,wanted,got)
    remote_run=subprocess.run([str(exe),'--remote'],input='\n'.join(f'{w} {b}' for w,b in remote_rows)+'\n',text=True,capture_output=True,check=True)
    assert [int(x) for x in remote_run.stdout.splitlines()]==remote_expected
    guards=json.loads(subprocess.run([str(exe),'--guards'],capture_output=True,text=True,check=True).stdout)
    assert guards=={'guard_and_failure_cases':42,'mismatches':0}
    required=set(range(0x33de90,0x33e038,4))|set(range(0x33dd10,0x33dd24,4))
    assert coverage==required,(len(coverage),len(required),sorted(required-coverage))
    inputs=[MODULE/'object_update_culling.hpp',MODULE/'object_update_culling.cpp',MODULE/'tests/object_update_culling.cpp',Path(__file__).resolve(),MANIFEST,MODULE/'reference/object-update-culling/NOTES.md',MODULE/'tests/elf_import_identity.py',ROOT/'port/engine-resources/tests/cpu.py']
    report={'validation':'PASS','original_elf_sha256':SHA,'original_culling_cases':len(rows),'original_remote_cases':len(remote_rows),'guard_report':guards,
        'active_instructions':{'culling':106,'remote':5,'covered':len(coverage),'required':len(required),'addresses':[hex(x) for x in sorted(coverage)]},
        'imports_verified_from_relocated_plt':imports,'numeric_import_calls':numeric,'mismatches':0,'compiler_commands':[command],
        'source_sha256':{p.relative_to(ROOT).as_posix():digest(p) for p in inputs},'executable_sha256':digest(exe),
        'cases':[{'name':name,'input':row,'output':out} for (name,row),out in zip(rows,actual)],
        'remote_cases':[{'input':[w,b],'output':r} for (w,b),r in zip(remote_rows,remote_expected)],
        'scope':'Complete original432B culling caller (424 instruction bytes +8 literals) and20B remote leaf. Named online/remote virtual/level/camera-frustum providers are fixture boundaries. IEEE binary32 helpers modeled after import identity verification; finite bits exact, arithmetic NaNs compared by class. Camera creation/frustum production/render ownership and native eligibility binding remain external. Port guards are not original ARM behavior.'}
    args.report.parent.mkdir(parents=True,exist_ok=True);args.report.write_text(json.dumps(report,indent=2)+'\n',encoding='utf8')
    print(json.dumps({k:report[k] for k in ('validation','original_culling_cases','original_remote_cases','guard_report','active_instructions','mismatches')},indent=2));print('report:',args.report)

if __name__=='__main__':main()
