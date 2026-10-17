"""Compare the source view-matrix helper/caller with original ARM instructions."""
from __future__ import annotations
import argparse, hashlib, json, math, os, random, shutil, struct, subprocess, sys
from pathlib import Path

HERE=Path(__file__).resolve().parent
MODULE=HERE.parent
ROOT=MODULE.parents[1]
sys.path.insert(0,str(ROOT/'port/engine-resources/tests'))
from cpu import Cpu
sys.path.insert(0,str(HERE))
import run_frustum_host as plane_math
import run_frustum_bounds as bounds_math

ELF_SHA='36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80'
RECALC=0x00583280
TRANSFORM_STATE=0x00582348
RECALC_VIEW_AREA=0x00582904
CAMERA_SIZE=0x230
VIEW_OFFSET=0x1ec

def sha(path):return hashlib.sha256(path.read_bytes()).hexdigest()
def bits(x):return struct.unpack('<I',struct.pack('<f',float(x)))[0]
def from_bits(x):return struct.unpack('<f',struct.pack('<I',x&0xffffffff))[0]
def float_equal(a,b):
    return (math.isnan(from_bits(a)) and math.isnan(from_bits(b))) or a==b
def random_vectors(count):
    cases=[
      ('ordinary',(10.,20.,30.),(0.,0.,0.),(0.,1.,0.)),
      ('negative',(3.,-7.,2.),(1.,2.,-4.),(0.,1.,0.)),
      ('parallel-up',(0.,4.,0.),(0.,0.,0.),(0.,1.,0.)),
      ('zero-direction',(2.,3.,4.),(2.,3.,4.),(0.,1.,0.)),
      ('diagonal-up',(1.,1.,1.),(0.,0.,0.),(1.,1.,1.)),
      ('signed-zero',(0.,-0.,0.),(-0.,0.,-0.),(0.,1.,0.)),
      ('near-parallel',(0.,1.,1.e-20),(0.,0.,0.),(0.,1.,0.)),
      ('large',(1.e20,-2.e20,3.e20),(-4.e20,5.e20,-6.e20),(0.,1.,0.)),
      ('infinity',(math.inf,2.,3.),(0.,0.,0.),(0.,1.,0.)),
      ('nan',(math.nan,2.,3.),(0.,0.,0.),(0.,1.,0.)),
    ]
    rng=random.Random(0x583280)
    for i in range(count):
        cases.append((f'random-{i}',
          tuple(rng.uniform(-50,50) for _ in range(3)),
          tuple(rng.uniform(-20,20) for _ in range(3)),
          tuple(rng.uniform(-5,5) for _ in range(3))))
    return [(name,[bits(x) for vec in (p,t,u) for x in vec]) for name,p,t,u in cases]

class CameraCpu(Cpu):
    def __init__(self,elf,rows):
        super().__init__(elf,False,{'functions':rows})
        from unicorn import UC_HOOK_CODE
        self.boundary_events=[]
        self.uc.hook_add(UC_HOOK_CODE,self.boundary,
                         begin=TRANSFORM_STATE,end=TRANSFORM_STATE)
        self.uc.hook_add(UC_HOOK_CODE,self.boundary,
                         begin=RECALC_VIEW_AREA,end=RECALC_VIEW_AREA)
    def boundary(self,uc,address,size,unused):
        offset=address-self.base
        if offset==TRANSFORM_STATE:
            self.boundary_events.append(('transform_state',self.reg(0),self.reg(1)))
        elif offset==RECALC_VIEW_AREA:
            self.boundary_events.append(('recalculate_view_area',self.reg(0),None))
        else:raise AssertionError(f'unexpected boundary {offset:#x}')
        self.uc.reg_write(self.pc,self.uc.reg_read(self.lr))
    def external(self,uc,address,size,unused):
        name=self.imports.get(address)
        self.import_calls[name]=self.import_calls.get(name,0)+1
        binary={
          '__aeabi_fadd':bounds_math.fadd,
          '__aeabi_fsub':bounds_math.fsub,
          '__aeabi_fmul':bounds_math.fmul,
          '__aeabi_fdiv':bounds_math.fdiv,
        }
        if name in binary:self.put(0,binary[name](self.reg(0),self.reg(1)))
        elif name=='__aeabi_fcmpeq':
            self.put(0,int(from_bits(self.reg(0))==from_bits(self.reg(1))))
        elif name=='__aeabi_fcmpge':
            self.put(0,int(from_bits(self.reg(0))>=from_bits(self.reg(1))))
        elif name=='__aeabi_fcmple':
            self.put(0,int(from_bits(self.reg(0))<=from_bits(self.reg(1))))
        elif name=='sqrtf':self.put(0,plane_math.sqrtf(self.reg(0)))
        elif name in ('memcpy','memmove','__aeabi_memcpy','__aeabi_memcpy4','memset'):
            super().external(uc,address,size,unused);return
        else:raise AssertionError(f'unmodeled import {name} at {address:#x}, caller {self.uc.reg_read(self.lr)-4:#x}')
        self.uc.reg_write(self.pc,self.uc.reg_read(self.lr))

def compare_words(original,ported,label):
    assert len(original)==len(ported),(label,len(original),len(ported))
    for i,(a,b) in enumerate(zip(original,ported)):
        if not float_equal(a,b):
            raise AssertionError(f'{label}[{i}] original={a:08x} port={b:08x}')

def main():
    p=argparse.ArgumentParser(description=__doc__)
    p.add_argument('--original-elf',type=Path,required=True)
    p.add_argument('--compiler',default=os.environ.get('CXX') or shutil.which('g++'))
    p.add_argument('--random-cases',type=int,default=48)
    p.add_argument('--report',type=Path,default=MODULE/'build/camera-node-transform-final/validation.json')
    a=p.parse_args()
    if not a.compiler or a.random_cases<0:p.error('compiler required and case count nonnegative')
    elf=a.original_elf.resolve();assert sha(elf)==ELF_SHA
    manifest_path=MODULE/'reference/camera-node-transform/original-functions.json'
    manifest=json.loads(manifest_path.read_text())
    assert manifest['original_elf_sha256']==ELF_SHA
    rows=manifest['functions']
    for row in rows:
        address=int(row['elf_address'],0);size=row['size']
        with elf.open('rb') as stream:
            from elftools.elf.elffile import ELFFile
            image=ELFFile(stream)
            segment=next(s for s in image.iter_segments()
                         if s['p_type']=='PT_LOAD' and
                         s['p_vaddr']<=address and address+size<=s['p_vaddr']+s['p_filesz'])
            raw=segment.data()[address-segment['p_vaddr']:address-segment['p_vaddr']+size]
        assert sha_bytes(raw)==row['sha256'],row['original_symbol']
    inputs=[MODULE/'camera_math.hpp',MODULE/'camera_math.cpp',
            MODULE/'camera_node_transform.hpp',MODULE/'camera_node_transform.cpp',
            HERE/'camera_node_transform_host.cpp',Path(__file__).resolve(),manifest_path,
            HERE/'run_frustum_host.py',HERE/'run_frustum_bounds.py',
            ROOT/'port/engine-resources/tests/cpu.py']
    pins={str(x.relative_to(ROOT).as_posix()):sha(x) for x in inputs}
    out=a.report.resolve().parent;out.mkdir(parents=True,exist_ok=True)
    exe=out/('camera-node-transform-host.exe' if os.name=='nt' else 'camera-node-transform-host')
    command=[a.compiler,'-std=c++17','-O1','-Wall','-Wextra','-Werror','-pedantic',
             '-ffp-contract=off','-fno-fast-math',
             str(MODULE/'camera_math.cpp'),str(MODULE/'camera_node_transform.cpp'),
             str(HERE/'camera_node_transform_host.cpp'),'-o',str(exe)]
    subprocess.run(command,check=True,cwd=ROOT)
    guard=json.loads(subprocess.check_output([str(exe),'guards'],text=True))
    assert guard=={'guards':9},guard
    cpu=CameraCpu(elf,rows)
    cases=random_vectors(a.random_cases)
    results=[];event_trace=[]
    for name,words in cases:
        vecs=[words[0:3],words[3:6],words[6:9]]
        addresses=[cpu.data+0x10000+i*0x100 for i in range(3)]
        for address,v in zip(addresses,vecs):cpu.uc.mem_write(address,struct.pack('<3I',*v))
        output=cpu.data+0x20000
        cpu.uc.mem_write(output,bytes([0xA5])*68)
        cpu.invoke(int(rows[1]['elf_address'],0),[output,*addresses])
        original=list(struct.unpack('<16I',cpu.uc.mem_read(output,64)))
        original_identity=cpu.uc.mem_read(output+64,1)[0]
        host=json.loads(subprocess.check_output([str(exe),'helper',*[f'{x:08x}' for x in words]],text=True))
        assert host['status']==0
        compare_words(original,host['words'],name+'/helper')
        assert original_identity==host['identity']==0

        raw=bytearray((i*13+0x39)&0xff for i in range(CAMERA_SIZE))
        struct.pack_into('<3I',raw,0x54,*vecs[0])
        struct.pack_into('<3I',raw,0x138,*vecs[1])
        struct.pack_into('<3I',raw,0x144,*vecs[2])
        camera=cpu.data+0x30000
        cpu.uc.mem_write(camera,bytes(raw))
        cpu.boundary_events=[]
        cpu.invoke(RECALC,[camera])
        source_bytes=bytes(cpu.uc.mem_read(camera+VIEW_OFFSET,65))
        source_matrix=list(struct.unpack('<16I',source_bytes[:64]))
        source_identity=source_bytes[64]
        host=json.loads(subprocess.check_output([str(exe),'node',*[f'{x:08x}' for x in words]],text=True))
        assert host['status']==0
        compare_words(source_matrix,host['words'],name+'/node')
        assert source_identity==host['identity']==0
        assert host['events']==[1,2]
        assert [x[0] for x in cpu.boundary_events]==['transform_state','recalculate_view_area']
        assert cpu.boundary_events[0][1:]==(camera+0x168,0)
        assert cpu.boundary_events[1][1]==camera
        event_trace.append({'case':name,'transform_state_offset':cpu.boundary_events[0][1]-camera,
                            'state':cpu.boundary_events[0][2],'recalculate_view_area_offset':0})
        results.append({'case':name,'helper_match':True,'caller_match':True,
                        'identity_byte':source_identity})
    assert pins=={str(x.relative_to(ROOT).as_posix()):sha(x) for x in inputs},'input changed while runner was active'
    coverage=[]
    for row in rows:
        start=int(row['elf_address'],0)
        expected=set(range(start,start+row['size'],4))
        reached={address-cpu.base for address in cpu.seen}
        coverage.append({'symbol':row['original_symbol'],'address':row['elf_address'],
                         'instructions':len(expected),'executed':len(expected & reached)})
        assert len(expected & reached)==len(expected),row['original_symbol']
    report={'validation':'PASS','scope':manifest['scope'],'cases':results,
            'source_call_order':event_trace,'host_guards':guard['guards'],
            'original_instruction_coverage':coverage,'mismatches':0,
            'service_boundaries':manifest['call_boundaries'],
            'source_sha256':pins,'host_executable':str(exe),'host_executable_sha256':sha(exe),
            'compiler':subprocess.check_output([a.compiler,'--version'],text=True).splitlines()[0],
            'command':command,'original_elf':str(elf),'original_elf_sha256':sha(elf)}
    a.report.parent.mkdir(parents=True,exist_ok=True);a.report.write_text(json.dumps(report,indent=2)+'\n')
    print(json.dumps({'validation':'PASS','cases':len(results),'guards':guard['guards'],
                      'coverage':coverage,'report':str(a.report.resolve())},indent=2))

def sha_bytes(data):return hashlib.sha256(data).hexdigest()

if __name__=='__main__':main()
