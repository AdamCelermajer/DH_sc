#!/usr/bin/env python3
"""Original ARM32 float scene-track bodies versus compiled ARM64 and host.

Real relocated cache and synthetic BRES fixtures feed the original accessors.
Only imported arithmetic/libm/libc helpers use the host dependency model.
"""
import argparse
from collections import Counter
import ctypes as c
import hashlib
import importlib.util
import json
import math
from pathlib import Path
import random
import struct
import sys
from elftools.elf.elffile import ELFFile
from unicorn.arm64_const import UC_ARM64_REG_X0, UC_ARM64_REG_S0

ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(ROOT/'../engine-math/tests'))
from differential import Cpu, Dependencies, ORIGINAL_SHA256
sys.path.insert(0, str(ROOT/'../asset-payloads/tools'))
import api
spec = importlib.util.spec_from_file_location('asset_differential', ROOT/'../asset-payloads/tests/differential.py')
fixtures = importlib.util.module_from_spec(spec); spec.loader.exec_module(fixtures)

ROWS = [(1,'key',0x61225c,72),(10,'key',0x612394,72),(5,'key',0x61cf8c,76),
        (1,'lerp',0x6286cc,256),(10,'lerp',0x628850,256),(5,'lerp',0x613294,80),
        (1,'delta',0x6122a4,80),(10,'delta',0x6123dc,80),(5,'delta',0x61cfd8,276),
        (1,'delta_lerp',0x6122f4,124),(10,'delta_lerp',0x61242c,124),(5,'delta_lerp',0x61d100,424),
        (5,'blend',0x6130d4,448),
        (9,'key',0x61f594,64),(9,'lerp',0x61f94c,60),
        (9,'delta',0x61f5e4,252),(9,'delta_lerp',0x61f6f4,384),
        (2,'key',0x61f9a4,112),(2,'lerp',0x61fa24,184),
        (2,'delta',0x61faf8,108),(2,'delta_lerp',0x61fb78,212),
        (3,'key',0x61fc70,112),(3,'lerp',0x61fcf0,184),
        (3,'delta',0x61fdc4,108),(3,'delta_lerp',0x61fe44,212),
        (4,'key',0x61ff3c,112),(4,'lerp',0x61ffbc,180),
        (4,'delta',0x615e6c,108),(4,'delta_lerp',0x61db54,212)]

class EngineCpu(Cpu):
    def __init__(self, *args):
        super().__init__(*args)
        if self.arm64: self.int_regs = tuple(UC_ARM64_REG_X0+i for i in range(8))

    def call(self, address, args, fp=None):
        sp = self.stack + 0xe000
        self.uc.reg_write(self.sp_reg, sp); self.uc.reg_write(self.lr_reg, self.stop)
        n = 8 if self.arm64 else 4
        for i,value in enumerate(args[:n]): self.write_reg(i,value)
        if len(args)>n: self.uc.mem_write(sp, struct.pack('<'+'I'*(len(args)-n),*args[n:]))
        if fp is not None: self.uc.reg_write(UC_ARM64_REG_S0, struct.unpack('<I',struct.pack('<f',fp))[0])
        if isinstance(address,str): address=self.symbols[address]
        self.uc.emu_start(address,self.stop,count=1000000)
        assert self.uc.reg_read(self.pc_reg)==self.stop and self.uc.reg_read(self.sp_reg)==sp
        return self.reg(0)

def main():
    p=argparse.ArgumentParser();p.add_argument('--original',type=Path,required=True)
    p.add_argument('--arm64',type=Path,required=True);p.add_argument('--host',type=Path,required=True)
    p.add_argument('--oracle',type=Path,required=True);p.add_argument('--sample',type=Path,required=True)
    p.add_argument('--angle-sample',type=Path,required=True)
    p.add_argument('--position-sample',type=Path,required=True)
    p.add_argument('--report',type=Path,required=True);a=p.parse_args()
    sha=lambda path:hashlib.sha256(path.read_bytes()).hexdigest()
    assert sha(a.original)==ORIGINAL_SHA256
    provenance={'functions':[]}
    with a.original.open('rb') as f:
        elf=ELFFile(f);loads=[s for s in elf.iter_segments()if s['p_type']=='PT_LOAD']
        syms={s['st_value']:s.name for s in elf.get_section_by_name('.dynsym').iter_symbols()if s['st_shndx']!='SHN_UNDEF'}
        for typ,kind,address,size in ROWS:
            segment=next(s for s in loads if s['p_vaddr']<=address and address+size<=s['p_vaddr']+s['p_filesz'])
            f.seek(segment['p_offset']+address-segment['p_vaddr'])
            provenance['functions'].append({'type':typ,'kind':kind,'elf_address':f'0x{address:08x}',
                'size':size,'original_symbol':syms[address],'sha256':hashlib.sha256(f.read(size)).hexdigest()})
    deps=Dependencies(a.oracle);old=EngineCpu(a.original,False,deps,provenance)
    new=EngineCpu(a.arm64,True,deps,provenance)
    host=api.bind(a.host)
    anim_ptr=c.POINTER(api.Animation);out_ptr=c.POINTER(c.c_float)
    for name,args in {'key':[anim_ptr,c.c_uint32,out_ptr],
                      'interpolate':[anim_ptr,c.c_uint32,c.c_uint32,c.c_float,out_ptr],
                      'delta':[anim_ptr,c.c_uint32,c.c_uint32,c.c_uint32,c.c_float,c.c_bool,out_ptr]}.items():
        fun=getattr(host,'dh2_animation_float_'+name);fun.restype=c.c_uint32;fun.argtypes=args
    host.dh2_animation_quaternion_blend.restype=c.c_uint32
    host.dh2_animation_quaternion_blend.argtypes=[out_ptr,out_ptr,c.c_uint32,out_ptr]
    old_image=old.data+0x4000;new_image=new.data+0x4000
    old_acc=old.data+0x400;new_acc=new.data+0x400;new_view=new.data+0x200
    old_out=old.data+0x100;new_out=new.data+0x100
    calls=Counter();samples=[];rng=random.Random(20261002)
    def equal(expected,actual,label):
        eb=struct.unpack('<'+'I'*(len(expected)//4),expected)
        ab=struct.unpack('<'+'I'*(len(actual)//4),actual)
        assert all(x==y or (x&0x7fffffff)==(y&0x7fffffff)==0 for x,y in zip(eb,ab)),(label,eb,ab)
    def check_image(raw,label):
        assert len(raw)<0xc000
        w=lambda o:struct.unpack_from('<I',raw,o)[0]
        root=w(32);fixed=bytearray(raw)
        for field in struct.unpack_from('<'+'I'*w(16),raw,w(24)):
            struct.pack_into('<I',fixed,field,old_image+w(field))
        seg=w(w(root+48)+4);blob=w(seg+12)if w(seg+8)==0 else w(seg+20)
        for i in range(w(blob)):
            slot=blob+8+8*i;relative=struct.unpack_from('<i',raw,slot)[0]
            struct.pack_into('<I',fixed,slot,old_image+slot+relative)
        old.uc.mem_write(old_image,bytes(fixed));new.uc.mem_write(new_image,raw)
        assert new.call('dh2_bres_open',[new_view,new_image,len(raw)])==0
        owner=api.Input(host,raw)
        for i in range(w(root+36)):
            animation=owner.animation(i,0);typ=host.dh2_animation_type(c.byref(animation),0)
            if typ not in (1,2,3,4,5,9,10):continue
            if host.dh2_animation_scales(c.byref(animation)) or host.dh2_animation_offsets(c.byref(animation)):continue
            vector=api.Vector();assert host.dh2_animation_vector(c.byref(animation),0,True,c.byref(vector))
            if vector.type!=6 or vector.count<2:continue
            rec=w(root+40)+32*i
            old.uc.mem_write(old_acc,struct.pack('<4I',old_image+rec,old_image+blob,0,0))
            assert new.call('dh2_animation_open',[new_acc,new_view,i,0])==0
            cases=sorted({0,min(vector.count//2,vector.count-2),vector.count-2})
            for first in cases:
                second=first+1;reference=(first+3)%vector.count
                for t in (0.0,1.0,0.1,0.5,0.9):
                    tb=struct.unpack('<I',struct.pack('<f',t))[0]
                    for kind in ('key','lerp','delta','delta_lerp'):
                        address=next(address for ty,k,address,size in ROWS if ty==typ and k==kind)
                        old.uc.mem_write(old_out,b'\xa5'*32);new.uc.mem_write(new_out,b'\xa5'*32)
                        out=(c.c_float*4)(99,99,99,99)
                        virtual=(kind=='key' and typ in (1,5,10)) or (kind=='delta' and typ in (1,10))
                        if kind=='key':
                            original=[0,old_acc,first,old_out] if virtual else [old_acc,first,old_out]
                            native=[new_acc,first,new_out]
                            result=host.dh2_animation_float_key(c.byref(animation),first,out);symbol='dh2_animation_float_key';fp=None
                        elif kind=='lerp':
                            original=[old_acc,first,second,tb,old_out]
                            native=[new_acc,first,second,new_out]
                            result=host.dh2_animation_float_interpolate(c.byref(animation),first,second,t,out);symbol='dh2_animation_float_interpolate';fp=t
                        elif kind=='delta':
                            original=[0,old_acc,reference,first,old_out]if virtual else[old_acc,reference,first,old_out]
                            native=[new_acc,reference,first,second,0,new_out]
                            result=host.dh2_animation_float_delta(c.byref(animation),reference,first,second,t,False,out);symbol='dh2_animation_float_delta';fp=t
                        else:
                            original=[old_acc,reference,first,second,tb,old_out]
                            native=[new_acc,reference,first,second,1,new_out]
                            result=host.dh2_animation_float_delta(c.byref(animation),reference,first,second,t,True,out);symbol='dh2_animation_float_delta';fp=t
                        assert result==0,(label,i,kind,result)
                        old.call(address,original);assert new.call(symbol,native,fp)==0
                        size=16 if typ in (5,9) else 12
                        expected=bytes(old.uc.mem_read(old_out,size));actual=bytes(new.uc.mem_read(new_out,size))
                        context=(label,i,typ,kind,first,t)
                        equal(expected,actual,context);equal(expected,bytes(out)[:size],context)
                        assert bytes(old.uc.mem_read(old_out+size,32-size))==b'\xa5'*(32-size)
                        assert bytes(new.uc.mem_read(new_out+16,16))==b'\xa5'*16
                        calls[f'{typ}_{kind}']+=1
        assert bytes(owner.bytes.raw[:len(raw)])==raw
        assert bytes(new.uc.mem_read(new_image,len(raw)))==raw
        samples.append({'name':label,'sha256':hashlib.sha256(raw).hexdigest()})
    for typ in (1,2,3,4,5,9,10):
        scalar = typ in (2,3,4,9)
        raw=bytearray(fixtures.synthetic(list(range(20)),time_type=4,optional=scalar))
        w=lambda o:struct.unpack_from('<I',raw,o)[0]
        root=w(32);record=w(root+40);sampler=w(record+8);channel=w(record+16);blob=w(w(w(root+48)+4)+20)
        if scalar:
            fields=list(struct.unpack_from('<'+'I'*w(16),raw,w(24)))
            fields=[f for f in fields if f not in (record+28,1044,1048)]
            struct.pack_into('<I',raw,16,len(fields));struct.pack_into('<'+'I'*len(fields),raw,w(24),*fields)
            struct.pack_into('<I',raw,28,60+len(fields)*4)
            struct.pack_into('<I',raw,record+28,0)
            if typ==9:struct.pack_into('<3f',raw,1080,0.6,0,0.8)
        struct.pack_into('<I',raw,channel+8,typ);components=4 if typ==5 else 1 if scalar else 3
        struct.pack_into('<I',raw,sampler+20,components)
        slot=blob+16;values=slot+struct.unpack_from('<i',raw,slot)[0]
        vv=[]
        for i in range(20):
            vals=[rng.uniform(-4,4)for _ in range(components)]
            if typ==5:
                norm=math.sqrt(sum(x*x for x in vals));vals=[x/norm for x in vals]
            vv.extend(vals)
        struct.pack_into('<'+'f'*len(vv),raw,values,*vv)
        check_image(bytes(raw),f'synthetic type {typ}')
    check_image(a.sample.read_bytes(),'owner warrior walk')
    check_image(a.angle_sample.read_bytes(),'owner dual walk angle tracks')
    check_image(a.position_sample.read_bytes(),'owner cutscene component position')
    for count in (0,1,2,3,8):
        for case in range(25):
            values=[]
            for i in range(count):
                q=[rng.uniform(-1,1)for _ in range(4)];norm=math.sqrt(sum(x*x for x in q))
                values.extend(x/norm for x in q)
            weights=[rng.choice([0,0.25,0.75,1,1.5])for _ in range(count)]
            if case==0:weights=[0]*count
            cv=(c.c_float*(4*count))(*values);cw=(c.c_float*count)(*weights);co=(c.c_float*4)()
            for cpu in (old,new):
                cpu.uc.mem_write(cpu.data+0x1000,bytes(cv));cpu.uc.mem_write(cpu.data+0x2000,bytes(cw))
                cpu.uc.mem_write(cpu.data+0x100,b'\xa5'*32)
            assert host.dh2_animation_quaternion_blend(cv,cw,count,co)==0
            old.call(0x6130d4,[old.data+0x1000,old.data+0x2000,count,old_out])
            assert new.call('dh2_animation_quaternion_blend',[new.data+0x1000,new.data+0x2000,count,new_out])==0
            expected=bytes(old.uc.mem_read(old_out,16));equal(expected,bytes(new.uc.mem_read(new_out,16)),('blend',count,case))
            equal(expected,bytes(co),('host blend',count,case));calls['quaternion_blend']+=1
            assert bytes(old.uc.mem_read(old_out+16,16))==bytes(new.uc.mem_read(new_out+16,16))==b'\xa5'*16
    result={'complete_engine':False,'gameplay_verified':False,'animator_state_machine':False,
        'comparison':'Original ARM32 vs compiled ARM64 and host: exact float bits except signed zero',
        'original_sha256':sha(a.original),'arm64_sha256':sha(a.arm64),'host_sha256':sha(a.host),'oracle_sha256':sha(a.oracle),
        'seed':20261002,'function_evidence':provenance['functions'],'calls':dict(calls),'comparisons':sum(calls.values()),
        'mismatches':0,'samples':samples,'main_instruction_addresses_seen':len(old.seen),
        'input_preservation':True,'output_guards':True,'stack_restoration':True,'import_calls':old.import_calls,
        'dependency_model':'Host C arithmetic/libm and memcpy/memset; actual engine key/blend routines execute'}
    a.report.write_text(json.dumps(result,indent=2)+'\n');print(json.dumps(result))
if __name__=='__main__':main()
