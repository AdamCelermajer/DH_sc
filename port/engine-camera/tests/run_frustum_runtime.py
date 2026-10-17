"""Compare the full source frustum graph with unpatched original ARM instructions."""
from __future__ import annotations
import argparse, hashlib, json, math, os, random, shutil, struct, subprocess, sys
from pathlib import Path
import run_frustum_bounds as bounds
import run_frustum_host as planes

HERE=Path(__file__).resolve().parent
MODULE=HERE.parent
ROOT=MODULE.parents[1]
sys.path.insert(0,str(ROOT/'port/engine-resources/tests'))
from cpu import Cpu

class SourceCpu(Cpu):
    def external(self,uc,address,size,unused):
        name=self.imports.get(address)
        self.import_calls[name]=self.import_calls.get(name,0)+1
        binary={'__aeabi_fadd':bounds.fadd,'__aeabi_fsub':bounds.fsub,
                '__aeabi_fmul':bounds.fmul,'__aeabi_fdiv':bounds.fdiv}
        if name in binary: self.put(0,binary[name](self.reg(0),self.reg(1)))
        elif name=='sqrtf': self.put(0,planes.sqrtf(self.reg(0)))
        elif name in ('__aeabi_fcmpgt','__aeabi_fcmplt','__aeabi_fcmpeq'):
            a,b=map(bounds.from_bits,(self.reg(0),self.reg(1)))
            self.put(0,int(a>b if name=='__aeabi_fcmpgt' else a<b if name=='__aeabi_fcmplt' else a==b))
        elif name=='__aeabi_f2d':
            low,high=bounds.double_words(bounds.from_bits(self.reg(0)))
            self.put(0,low);self.put(1,high)
        elif name=='__aeabi_d2f': self.put(0,bounds.bits(bounds.as_double(self.reg(0),self.reg(1))))
        elif name in ('__aeabi_dmul','__aeabi_ddiv'):
            a=bounds.as_double(self.reg(0),self.reg(1));b=bounds.as_double(self.reg(2),self.reg(3))
            low,high=bounds.double_words(a*b if name=='__aeabi_dmul' else bounds.divide_double(a,b))
            self.put(0,low);self.put(1,high)
        elif name=='__aeabi_dcmplt':
            self.put(0,int(bounds.as_double(self.reg(0),self.reg(1))<bounds.as_double(self.reg(2),self.reg(3))))
        elif name=='sqrt':
            value=bounds.as_double(self.reg(0),self.reg(1))
            low,high=bounds.double_words(math.sqrt(value) if value>=0 else math.nan)
            self.put(0,low);self.put(1,high)
        elif name in ('memcpy','memmove','__aeabi_memcpy','__aeabi_memcpy4','memset'):
            super().external(uc,address,size,unused);return
        else: raise AssertionError(f'unmodeled import {name} from {self.uc.reg_read(self.lr)-4:#x}')
        self.uc.reg_write(self.pc,self.uc.reg_read(self.lr))

def sha(path):return hashlib.sha256(path.read_bytes()).hexdigest()
def main():
    p=argparse.ArgumentParser(description=__doc__)
    p.add_argument('--original-elf',type=Path,required=True)
    p.add_argument('--compiler',default=os.environ.get('CXX') or shutil.which('g++'))
    p.add_argument('--random-cases',type=int,default=64)
    p.add_argument('--report',type=Path,default=MODULE/'build/frustum-runtime/validation.json')
    a=p.parse_args()
    if not a.compiler or a.random_cases<0:p.error('compiler required; nonnegative case count')
    elf=a.original_elf.resolve();assert sha(elf)==bounds.ELF_SHA
    producer=MODULE/'reference/frustum-producer/original-functions.json'
    bound_manifest=MODULE/'reference/frustum-bounds/original-functions.json'
    m=json.loads(bound_manifest.read_text());pm=json.loads(producer.read_text())
    bounds.verify_manifest(elf,m);planes.verify_manifest(elf,pm)
    rows=[pm['functions'][0],*m['functions'],*m['supporting_functions']]
    inputs=[MODULE/n for n in ('frustum_runtime.cpp','frustum_runtime.hpp','frustum.cpp','frustum.hpp',
              'frustum_bounds.cpp','frustum_bounds.hpp','plane_intersection.cpp','plane_intersection.hpp')]
    inputs += [HERE/'frustum_runtime_host.cpp',Path(__file__).resolve(),HERE/'run_frustum_bounds.py',
               HERE/'run_frustum_host.py',ROOT/'port/engine-resources/tests/cpu.py',producer,bound_manifest,
               MODULE/'reference/frustum-runtime/original-functions.json',
               MODULE/'reference/plane-intersection/original-functions.json']
    pins={str(x.relative_to(ROOT).as_posix()):sha(x) for x in inputs}
    directory=a.report.resolve().parent;directory.mkdir(parents=True,exist_ok=True)
    exe=directory/('host.exe' if os.name=='nt' else 'host')
    command=[a.compiler,'-std=c++17','-O1','-Wall','-Wextra','-Werror','-pedantic',
             '-ffp-contract=off','-fno-fast-math',*[str(MODULE/n) for n in
              ('frustum_runtime.cpp','frustum.cpp','frustum_bounds.cpp','plane_intersection.cpp')],
             str(HERE/'frustum_runtime_host.cpp'),'-o',str(exe)]
    subprocess.run(command,check=True,cwd=ROOT)
    guards=json.loads(subprocess.check_output([str(exe),'guards']))
    assert guards=={'guards':10},guards
    cpu=SourceCpu(elf,False,{'functions':rows})
    matrix_address,frustum_address=cpu.data+0x10000,cpu.data+0x20000
    rng=random.Random(0x5826c0);results=[]
    for name,matrix in planes.case_matrices(a.random_cases):
        before=[bounds.bits(rng.uniform(-50,50)) for _ in range(3)]+[0xdead0000+i for i in range(30)]
        cpu.uc.mem_write(matrix_address,struct.pack('<16I',*matrix))
        cpu.uc.mem_write(frustum_address,struct.pack('<33I',*before))
        cpu.invoke(planes.SYMBOL,[frustum_address,matrix_address])
        original=list(struct.unpack('<33I',cpu.uc.mem_read(frustum_address,132)))
        actual=json.loads(subprocess.check_output([str(exe),'full',*[f'{x:08x}' for x in matrix+before]]))
        assert actual['status']==0,(name,actual)
        equivalent_nan=0
        for i,(x,y) in enumerate(zip(original,actual['words'])):
            if math.isnan(bounds.from_bits(x)) or math.isnan(bounds.from_bits(y)):
                assert math.isnan(bounds.from_bits(x)) and math.isnan(bounds.from_bits(y)),(name,i,x,y)
                equivalent_nan += int(x!=y)
            else:assert x==y,(name,i,f'{x:08x}',f'{y:08x}')
        assert actual['words'][:3]==before[:3],name
        results.append({'case':name,'matched':True,'NaN_payload_only_differences':equivalent_nan})
    assert pins=={str(x.relative_to(ROOT).as_posix()):sha(x) for x in inputs},'source changed during test'
    coverage=[]
    for row in rows:
        start=int(row['elf_address'],0);addresses=set(range(start,start+row['size'],4))
        coverage.append({'address':row['elf_address'],'symbol':row['original_symbol'],
                         'range_words':len(addresses),'executed_words':len(addresses & cpu.seen)})
    # These two bodies contain only instructions. Helper symbol extents also
    # contain literal data; their own focused gate checks the executable ranges.
    for item in coverage[:2]:
        assert item['executed_words']==item['range_words'],item
    report={'validation':'PASS','scope':'Full setFrom, bounds and three-plane source composition. Original helpers run without patched tail branches or substituted point results; imported scalar FP operations are modeled IEEE operations. NaN classification is compared, payload propagation is not claimed. Native camera ownership/transforms/Android wiring remain open. Composition earns no additional original body beyond its component evidence.',
            'original_sha256':bounds.ELF_SHA,'original_arm_cases':len(results),'mismatches':0,
            'guards':guards,'source_sha256':pins,'compiler_command':command,'binary_sha256':sha(exe),
            'original_body_execution':coverage,'scalar_import_calls':cpu.import_calls,'results':results}
    a.report.write_bytes((json.dumps(report,indent=2)+'\n').encode())
    print(json.dumps({k:report[k] for k in ('validation','original_arm_cases','mismatches','guards')}))
if __name__=='__main__':main()
