#!/usr/bin/env python3
"""Execute original software-skin palette instructions against the source port.

Fixtures supply already-resolved scene world matrices and preallocated vectors.
No bone lookup, renderer, allocation, vertex skinning or gameplay is modeled.
Imported float arithmetic/libc use the existing host C dependency oracle.
"""
import argparse
import ctypes as c
import hashlib
import importlib.util
import json
from pathlib import Path
import random
import struct
import sys

ROOT=Path(__file__).resolve().parents[1]
sys.path.insert(0,str(ROOT/'../engine-math/tests'))
from differential import Cpu,Dependencies,ORIGINAL_SHA256
spec=importlib.util.spec_from_file_location('skin_api',ROOT/'tests/audit.py')
api=importlib.util.module_from_spec(spec);spec.loader.exec_module(api)

SYMBOL='_ZN6glitch7collada6detail29CColladaSoftwareSkinTechnique12prepareCacheEv'
def main():
    p=argparse.ArgumentParser();p.add_argument('--original',required=True,type=Path)
    p.add_argument('--library',required=True,type=Path);p.add_argument('--oracle',required=True,type=Path)
    p.add_argument('--sample',required=True,type=Path);p.add_argument('--report',required=True,type=Path)
    a=p.parse_args();sha=lambda path:hashlib.sha256(path.read_bytes()).hexdigest()
    assert sha(a.original)==ORIGINAL_SHA256
    cpu=Cpu(a.original,False,Dependencies(a.oracle),{'functions':[
        {'elf_address':'0x0066fe34','size':276},{'elf_address':'0x006651a0','size':1704}]})
    dll=api.bind(a.library);raw=a.sample.read_bytes();storage=c.create_string_buffer(raw);image=api.scene.Bres()
    assert dll.dh2_bres_open(c.byref(image),storage,len(raw))==0
    rng=random.Random(20261002);records=[];comparisons=0
    # Distinct guarded buffers in the CPU's mapped fixture arena.
    obj=cpu.data; skin_ptr=obj+0x1000;cache=obj+0x2000
    matrix_ptr=obj+0x3000; world_ptr=obj+0x7000; ptrs=obj+0xb000; inverses=obj+0xc000
    original_sp=cpu.stack+0xe000
    for controller in (0,1):
        skin=api.Skin();assert dll.dh2_skin_open(c.byref(skin),c.byref(image),controller)==0
        records.append({'controller':controller,'joints':skin.joints})
        for case in range(16):
            worlds=(api.scene.Matrix*skin.joints)();out=(api.scene.Matrix*skin.joints)()
            for j in range(skin.joints):
                # General finite affine matrices include noncommuting scale,
                # rotations/shears and translations; original world hint zero.
                values=[rng.uniform(-2,2)for _ in range(16)]
                for k in (3,7,11):values[k]=0
                values[15]=1
                if case==0:values=[1,0,0,0,0,1,0,0,0,0,1,0,j,2,-3,1]
                worlds[j].m[:]=values
                cpu.uc.mem_write(world_ptr+j*68,struct.pack('<16f',*worlds[j].m)+b'\0\0\0\0')
            cpu.uc.mem_write(ptrs,struct.pack('<'+'I'*skin.joints,*[world_ptr+j*68 for j in range(skin.joints)]))
            cpu.uc.mem_write(inverses,raw[skin.inverse_matrices:skin.inverse_matrices+skin.joints*64])
            old_skin=bytearray(156)
            struct.pack_into('<I',old_skin,4,inverses)
            struct.pack_into('<16f',old_skin,0x10,*skin.bind_shape.m)
            struct.pack_into('<I',old_skin,0x74,skin.joints)
            cpu.uc.mem_write(skin_ptr,bytes(old_skin))
            cpu.uc.mem_write(obj,struct.pack('<6I',0,0,0,skin_ptr,cache,0))
            cpu.uc.mem_write(cache,struct.pack('<7I',1,matrix_ptr,matrix_ptr+skin.joints*68,
                matrix_ptr+skin.joints*68,ptrs,ptrs+skin.joints*4,ptrs+skin.joints*4))
            cpu.uc.mem_write(matrix_ptr,b'\xa5'*(skin.joints*68+16))
            cpu.uc.reg_write(cpu.sp_reg,original_sp);cpu.uc.reg_write(cpu.lr_reg,cpu.stop)
            cpu.write_reg(0,obj)
            cpu.uc.emu_start(cpu.symbols[SYMBOL],cpu.stop,count=3000000)
            assert cpu.uc.reg_read(cpu.pc_reg)==cpu.stop
            assert cpu.uc.reg_read(cpu.sp_reg)==original_sp
            assert bytes(cpu.uc.mem_read(matrix_ptr+skin.joints*68,16))==b'\xa5'*16
            assert struct.unpack('<I',cpu.uc.mem_read(cache,4))[0]==0
            assert dll.dh2_skin_palette(c.byref(skin),worlds,skin.joints,out,skin.joints)==0
            for j in range(skin.joints):
                actual=struct.pack('<16f',*out[j].m)
                expected=bytes(cpu.uc.mem_read(matrix_ptr+j*68,64))
                actual_bits=struct.unpack('<16I',actual);expected_bits=struct.unpack('<16I',expected)
                assert all(x==y or (x&0x7fffffff)==(y&0x7fffffff)==0 for x,y in zip(actual_bits,expected_bits)),(controller,case,j,struct.unpack('<16f',actual),struct.unpack('<16f',expected))
                comparisons+=1
    result={'complete_engine':False,'scope_lookup_validated':False,'vertex_skinning_differential':False,
            'comparison':'Exact 16-float palette bits except signed zero; original stack restoration and output guards',
            'original_symbol':SYMBOL,'original_elf_address':'0x0066fe34',
            'matrix_multiply_elf_address':'0x006651a0','original_sha256':sha(a.original),
            'library_sha256':sha(a.library),'oracle_sha256':sha(a.oracle),
            'sample_sha256':sha(a.sample),'seed':20261002,'poses_per_controller':16,
            'controllers':records,'matrix_comparisons':comparisons,'mismatches':0,
            'import_calls':cpu.import_calls,'oracle':'Host C float arithmetic and memcpy/memset for imported helpers'}
    a.report.write_text(json.dumps(result,indent=2)+'\n');print(json.dumps(result))
if __name__=='__main__':main()
