"""Actual original typed contribution and slerp instructions versus O2 ARM64.

Only imported IEEE soft-float/libc/libm contracts are modeled. Both sides use
the same Windows UCRT single-precision libm model, not a historical Bionic
claim. Host replay can bind these exact libm call inputs/results as fixtures.
All finite and signed-zero outputs are bit exact; arithmetic NaNs compare
classification. Copy branches must retain every input bit, including NaNs.
"""
import argparse
from collections import Counter
import ctypes
import hashlib
import json
import math
from pathlib import Path
import random
import struct
import sys
import time

ROOT=Path(__file__).resolve().parents[1]
REPO=ROOT.parents[1]
sys.path.insert(0,str(REPO/'port/engine-resources/tests'))
from cpu import Cpu as BaseCpu
from unicorn.arm64_const import UC_ARM64_REG_S0,UC_ARM64_REG_TPIDR_EL0


def bits(value):
    return struct.unpack('<I',struct.pack('<f',ctypes.c_float(value).value))[0]


def floating(word):
    return struct.unpack('<f',struct.pack('<I',word&0xffffffff))[0]


def words(values):
    return struct.pack('<'+'I'*len(values),*values)


def same_word(left,right,copy=False):
    return left==right or (not copy and math.isnan(floating(left)) and math.isnan(floating(right)))


class Cpu(BaseCpu):
    def __init__(self,*args):
        super().__init__(*args)
        self.trig=[]
        self.crt=ctypes.CDLL('ucrtbase')
        self.libm={}
        for name in ('sinf','acosf','sqrtf'):
            function=getattr(self.crt,name)
            function.argtypes=[ctypes.c_float]
            function.restype=ctypes.c_float
            self.libm[name]=function
        if self.arm64:
            self.uc.reg_write(UC_ARM64_REG_TPIDR_EL0,self.data+0xf000)
            self.uc.mem_write(self.data+0xf028,struct.pack('<Q',0xabadb1e0))

    def external(self,uc,address,size,unused):
        name=self.imports.get(address)
        if name in ('__aeabi_fadd','__aeabi_fsub','__aeabi_fmul','__aeabi_fdiv'):
            a,b=floating(self.reg(0)),floating(self.reg(1))
            if name.endswith('fdiv') and b==0:
                value=math.nan if a==0 or math.isnan(a) else math.copysign(math.inf,math.copysign(1,a)*math.copysign(1,b))
            else:
                value=a+b if name.endswith('fadd') else a-b if name.endswith('fsub') else a*b if name.endswith('fmul') else a/b
            self.put(0,bits(value))
        elif name.startswith('__aeabi_fcmp'):
            a,b=floating(self.reg(0)),floating(self.reg(1))
            self.put(0,int({'eq':a==b,'lt':a<b,'le':a<=b,'gt':a>b,'ge':a>=b,'un':math.isnan(a) or math.isnan(b)}[name[12:]]))
        elif name in self.libm:
            word=uc.reg_read(UC_ARM64_REG_S0) if self.arm64 else self.reg(0)
            output=bits(self.libm[name](floating(word)))
            self.trig.append(({'sinf':0,'acosf':1,'sqrtf':2}[name],word,output))
            if self.arm64:uc.reg_write(UC_ARM64_REG_S0,output)
            else:self.put(0,output)
        elif name=='__memcpy_chk':
            destination,source,count,capacity=[self.reg(i) for i in range(4)]
            assert count<=capacity
            uc.mem_write(destination,bytes(uc.mem_read(source,count)));self.put(0,destination)
        else:
            return super().external(uc,address,size,unused)
        self.import_calls[name]=self.import_calls.get(name,0)+1
        uc.reg_write(self.pc,uc.reg_read(self.lr))


def main():
    parser=argparse.ArgumentParser(description=__doc__)
    for name in ('engine','library','report','reference-output'):
        parser.add_argument('--'+name,type=Path,required=True)
    args=parser.parse_args()
    started=time.monotonic()
    manifest=json.loads((ROOT/'reference/animation-blend/original-functions.json').read_text())
    assert hashlib.sha256(args.engine.read_bytes()).hexdigest()==manifest['original_sha256']
    old=Cpu(args.engine,False,manifest)
    new=Cpu(args.library,True,{'functions':[]})
    addresses=(0x620134,0x626b4c,0x6275fc,0x6130d4)
    names=('dh2_animation_blend_scalar','dh2_animation_blend_vector3','dh2_animation_blend_vector3','dh2_animation_blend_quaternion')
    components=(1,3,3,4)
    records=[]
    counts=Counter()
    scenarios=Counter()
    libm_calls=0
    def compare(operation,values,weights,label,ignored_weights=False,inputs_alias=False):
        nonlocal libm_calls
        count=len(weights);width=components[operation]
        assert len(values)==count*width
        exact_copy=count==1 and operation!=3
        if operation==3:
            present=[i for i,w in enumerate(weights) if floating(w)!=0]
            exact_copy=bool(present) and (len(present)==1 or floating(weights[present[0]])==1)
        results=[];traces=[]
        for cpu in (old,new):
            v,w,out=[cpu.data+x for x in (0x1000,0x3000,0x5000)]
            cpu.uc.mem_write(v,words(values) or bytes(16))
            cpu.uc.mem_write(w,words(weights) or bytes(16))
            cpu.uc.mem_write(out-4,words([0xaabbccdd]*6))
            cpu.trig=[]
            vp=v if count else 0;wp=(v if inputs_alias else w) if count and not ignored_weights else 0
            if inputs_alias:assert weights==values[:count]
            if cpu is old:
                argv=[vp,wp,count,out] if operation==3 else [0,vp,wp,count,out]
                cpu.invoke(addresses[operation],argv)
            else:
                assert cpu.invoke(names[operation],[out,vp,wp,count])==0
            result=bytes(cpu.uc.mem_read(out,width*4))
            assert bytes(cpu.uc.mem_read(v,len(values)*4))==words(values)
            assert bytes(cpu.uc.mem_read(w,len(weights)*4))==words(weights)
            assert bytes(cpu.uc.mem_read(out-4,4))==words([0xaabbccdd])
            assert bytes(cpu.uc.mem_read(out+width*4,4))==words([0xaabbccdd])
            results.append(result);traces.append(cpu.trig[:])
        expected,actual=[struct.unpack('<'+'I'*width,item) for item in results]
        assert all(same_word(a,b,exact_copy) for a,b in zip(expected,actual)),(len(records),operation,label,values,weights,expected,actual)
        # O2 may lower source sqrtf to native FSQRT. Compare complete output
        # words above; external sinf/acosf calls still need identical ordering
        # and arguments. Golden host replay supplies the original sqrtf calls.
        external_traces=[[item for item in trace if item[0]!=2] for trace in traces]
        assert len(external_traces[0])==len(external_traces[1]),('libm call count',len(records),label,values,weights,expected,actual,traces)
        for a,b in zip(*external_traces):
            assert a[0]==b[0] and same_word(a[1],b[1]) and same_word(a[2],b[2]),('libm',label,a,b)
        flags=int(exact_copy)|(int(ignored_weights)<<1)|(int(inputs_alias)<<2)
        trace=b''.join(words(list(item)) for item in traces[0])
        records.append(words([operation,count,flags,len(traces[0])])+words(values)+words(weights)+results[0]+trace)
        counts[operation]+=1;scenarios[label]+=1;libm_calls+=len(traces[0])

    special=[0,0x80000000,1,0x80000001,0x3f800000,0xbf800000,0x3f800001,
             0x7f7fffff,0xff7fffff,0x7f800000,0xff800000,0x7fc01234,0x7fa01234]
    rng=random.Random(20261004)
    for operation in (0,1,2):
        width=components[operation]
        compare(operation,[],[],'empty')
        for item in special:
            compare(operation,[item]*width,[0x7fc05678],'count1 raw copy',True)
        for weight in special:
            compare(operation,[0x7f800000]*width+[bits(-2.)]*width,[weight,bits(.5)],'zero and exceptional arithmetic')
        for i in range(1024):
            count=rng.choice((0,1,2,3,8,17))
            values=[bits(rng.uniform(-1e10,1e10)) for _ in range(count*width)]
            weights=[bits(rng.uniform(-2,2)) for _ in range(count)]
            compare(operation,values,weights,'ordered random')
        # Sum rounding and cancellation distinguish this from a fused/reduced sum.
        compare(operation,[bits(x) for x in (1e20,1.,-1e20) for _ in range(width)],
                [bits(1.)]*3,'cancellation slot order')
        values=[bits(.25),bits(.75)] if width==1 else [bits(.25),bits(.75),bits(1.),bits(-.2),0,bits(1.)]
        compare(operation,values,values[:2],'read inputs alias',inputs_alias=True)

    quaternions=[[0,0,0,bits(1.)],[bits(1.),0,0,0],[bits(-1.),0,0,0],
                 [0,0,0,bits(2.)],[0,0,0,0],[bits(.3),bits(.4),bits(.5),bits(.7)],
                 [0x7fc01234,0x80000000,bits(1.),0x7fa05678],
                 [0x7f800000,bits(1.),0,0], [1,1,1,1]]
    pairs=[(0,0),(0x80000000,0),(bits(1.),0x7fc01234),(bits(.3),bits(.7)),
           (bits(.3),bits(-.3)),(bits(-.3),bits(.3)),(bits(-.3),bits(-.7)),
           (0x7f800000,bits(.5)),(0x7fc01234,bits(.5)),(bits(.5),0x7f800000),
           (0,bits(.5)),(bits(.99999994),bits(.00000006))]
    compare(3,[],[],'empty')
    compare(3,[bits(.25),bits(.75),0,bits(1.),0,0,0,bits(1.)],[bits(.25),bits(.75)],'read inputs alias',inputs_alias=True)
    for q in quaternions:
        for weight in special:compare(3,q,[weight],'one quaternion contribution')
    for a in quaternions:
        for b in quaternions:
            for pair in pairs:compare(3,a+b,list(pair),'quaternion exceptional and branch corpus')
    for i in range(1536):
        count=rng.choice((2,3,5,8))
        values=[]
        for slot in range(count):
            q=[rng.uniform(-1,1) for _ in range(4)]
            length=math.sqrt(sum(v*v for v in q))
            values.extend(bits(v/length) for v in q)
        weights=[bits(rng.choice((0.,1.,rng.uniform(-2,2)))) for _ in range(count)]
        compare(3,values,weights,'ordered quaternion random')

    rejections=0
    for operation in (0,1,3):
        out,v,w=[new.data+x for x in (0x1000,0x3000,0x5000)]
        width=components[operation]
        argv=[[0,v,w,2],[out,0,w,2],[out,v,0,2],[out,v,w,0xffffffff],
              [out,v,w,65537],[v,v,w,2],[w,v,w,2],[v+4,v,w,2],
              [out+1,v,w,2],[out,v+1,w,2],[out,v,w+1,2],
              [out,0xfffffffffffffffc,w,2]]
        for arguments in argv:
            before=words([0x12345678]*32)
            for ptr in (out,v,w):new.uc.mem_write(ptr,before)
            assert new.invoke(names[operation],arguments)==1,(operation,arguments)
            assert all(bytes(new.uc.mem_read(ptr,len(before)))==before for ptr in (out,v,w))
            rejections+=1
    reference=words([0x314b4241,len(records)])+b''.join(records)
    args.reference_output.parent.mkdir(parents=True,exist_ok=True)
    args.reference_output.write_bytes(reference)
    report={'original_sha256':manifest['original_sha256'],'arm64_library_sha256':hashlib.sha256(args.library.read_bytes()).hexdigest(),
            'reference_sha256':hashlib.sha256(reference).hexdigest(),'comparisons':len(records),
            'operation_counts':dict(counts),'scenarios':dict(scenarios),'ordered_libm_calls':libm_calls,
            'atomic_rejection_checks':rejections,'mismatches':0,'libm_dependency_model':'Windows UCRT sinf/acosf/sqrtf shared across instruction CPUs',
            'optimized_native_sqrt':'Compiler-generated FSQRT permitted; complete output words still compared, no output tolerance.',
            'nonfinite_input_policy':'accepted; arithmetic NaNs classification only; all copy-branch bits exact',
            'native_output_alias_policy':'all read-input overlap rejected atomically; source alias behavior is not claimed',
            'source_sha256':{str(p.relative_to(REPO)).replace('\\','/'):hashlib.sha256(p.read_bytes()).hexdigest()
                             for p in (ROOT/'animation_blend.hpp',ROOT/'animation_blend.cpp',REPO/'port/engine-math/math.cpp',Path(__file__))},
            'original_import_calls':old.import_calls,'original_functions':{row['original_symbol']:{'instructions':row['size']//4,'seen':sum(int(row['elf_address'],16)<=pc<int(row['elf_address'],16)+row['size'] for pc in old.seen)} for row in manifest['functions']},
            'scope':__doc__,'elapsed_seconds':round(time.monotonic()-started,3)}
    args.report.parent.mkdir(parents=True,exist_ok=True)
    args.report.write_text(json.dumps(report,indent=2)+'\n')
    print(json.dumps({key:value for key,value in report.items() if key not in ('original_functions','original_import_calls','scenarios')}))


if __name__=='__main__':main()
