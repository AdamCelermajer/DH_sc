#!/usr/bin/env python3
"""Run original ARM32 accessors/search against the compiled ARM64 component.

Both execute real engine/port instructions. Only imported compiler arithmetic
and libc dependencies are modeled. Relocated ARM32 fixtures use real cache
bytes; the port views retain immutable offsets and pointers above 4 GiB.
"""
import argparse
from collections import Counter
import ctypes as c
import hashlib
import json
from pathlib import Path
import random
import struct
import subprocess
import sys
import time
from elftools.elf.elffile import ELFFile

ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(ROOT/'../engine-resources/tests'))
from cpu import Cpu as BaseCpu, u32, i32

def f32(v):
    return c.c_float(v).value

def fb(v):
    return struct.unpack('<I', struct.pack('<f', v))[0]

class Dependencies:
    def __init__(self, path):
        dll = c.CDLL(str(path.resolve()))
        self.functions = {}
        specs = {'i2f': (c.c_float, [c.c_int32]), 'ui2d': (c.c_double, [c.c_uint32]), 'i2d': (c.c_double, [c.c_int32]),
                 'f2iz': (c.c_int32, [c.c_float]), 'd2iz': (c.c_int32, [c.c_double]),
                 'fmul': (c.c_float, [c.c_float]*2), 'fdiv': (c.c_float, [c.c_float]*2),
                 'dmul': (c.c_double, [c.c_double]*2)}
        for name, (restype, args) in specs.items():
            function = getattr(dll, 'oracle_'+name)
            function.restype, function.argtypes = restype, args
            self.functions[name] = function

class Cpu(BaseCpu):
    def __init__(self, path, arm64, provenance, dependencies):
        self.dependencies = dependencies
        super().__init__(path, arm64, provenance)

    def external(self, uc, address, size, unused):
        full = self.imports.get(address, '')
        name = full.removeprefix('__aeabi_')
        if name not in self.dependencies.functions and not name.startswith('fcmp'):
            return super().external(uc, address, size, unused)
        assert not self.arm64, 'Unexpected ARM64 compiler helper'
        self.import_calls[full] = self.import_calls.get(full, 0)+1
        ff = lambda r: struct.unpack('<f', struct.pack('<I', u32(self.reg(r))))[0]
        dd = lambda r: struct.unpack('<d', struct.pack('<II', u32(self.reg(r)), u32(self.reg(r+1))))[0]
        if name.startswith('fcmp'):
            a,b = ff(0),ff(1)
            result = {'lt': a<b, 'le': a<=b, 'gt': a>b, 'ge': a>=b, 'eq': a==b}[name[4:]]
            self.put(0, int(result))
        else:
            args = {'i2f': lambda: [i32(self.reg(0))], 'ui2d': lambda: [u32(self.reg(0))], 'i2d': lambda: [i32(self.reg(0))],
                    'f2iz': lambda: [ff(0)], 'd2iz': lambda: [dd(0)],
                    'fmul': lambda: [ff(0),ff(1)], 'fdiv': lambda: [ff(0),ff(1)],
                    'dmul': lambda: [dd(0),dd(2)]}[name]()
            result = self.dependencies.functions[name](*args)
            if name in ('ui2d','i2d','dmul'):
                lo,hi = struct.unpack('<II', struct.pack('<d', result)); self.put(0, lo); self.put(1, hi)
            elif name in ('f2iz','d2iz'):
                self.put(0, u32(result))
            else:
                self.put(0, fb(result))
        self.uc.reg_write(self.pc, self.uc.reg_read(self.lr))

def verify_original(path, manifest):
    assert hashlib.sha256(path.read_bytes()).hexdigest() == manifest['original_sha256']
    with path.open('rb') as f:
        elf = ELFFile(f)
        loads = [s for s in elf.iter_segments() if s['p_type'] == 'PT_LOAD']
        for r in manifest['functions']:
            address = int(r['elf_address'],16)
            s = next(s for s in loads if s['p_vaddr'] <= address and address+r['size'] <= s['p_vaddr']+s['p_filesz'])
            f.seek(s['p_offset']+address-s['p_vaddr'])
            assert hashlib.sha256(f.read(r['size'])).hexdigest() == r['sha256']

class Checks:
    def __init__(self, old, new):
        self.old,self.new = old,new
        self.calls = Counter()
        self.samples = Counter()
        self.image_old,self.image_new = old.data+0x10000,new.data+0x10000
        self.old_accessor,self.new_accessor = old.data+0x2000,new.data+0x2000
        self.new_view = new.data+0x1000
        self.raw = b''

    def compare(self, name, old, new):
        assert old == new, (name,old,new,self.context)
        self.calls[name] += 1

    def word(self, offset):
        return struct.unpack_from('<I',self.raw,offset)[0]

    def load(self, raw):
        self.raw = raw
        fixed = bytearray(raw)
        fields = struct.unpack_from('<'+'I'*self.word(16),raw,self.word(24))
        for field in fields:
            struct.pack_into('<I',fixed,field,u32(self.image_old+self.word(field)))
        self.old.uc.mem_write(self.image_old,bytes(fixed))
        self.new.uc.mem_write(self.image_new,raw)
        assert self.new.invoke('dh2_bres_open',[self.new_view,self.image_new,len(raw)]) == 0

    def select(self, animation, segment):
        root = self.word(32)
        record = self.word(root+40)+32*animation
        lib = self.word(root+48); seg = self.word(lib+4)+24*segment
        state = self.word(seg+8)
        data = self.word(seg+12) if state == 0 else self.word(seg+20)
        for i in range(self.word(data)):
            slot = data+8+8*i
            relative = i32(self.word(slot))
            self.old.uc.mem_write(self.image_old+slot,struct.pack('<I',u32(self.image_old+slot+relative)))
        self.old.uc.mem_write(self.old_accessor,struct.pack('<4I',self.image_old+record,self.image_old+data,0,0))
        assert self.new.invoke('dh2_animation_open',[self.new_accessor,self.new_view,animation,segment]) == 0
        self.record,self.data = record,data
        self.samples['animation_segment_views'] += 1
        self.samples['segment_state_'+str(state)] += 1

    def scalar(self, address, symbol, *args):
        self.compare(symbol,u32(self.old.invoke(address,[self.old_accessor,*args])),
                     u32(self.new.invoke(symbol,[self.new_accessor,*args])))

    def pointer(self, address, symbol, *args):
        old = self.old.invoke(address,[self.old_accessor,*args])
        new = self.new.invoke(symbol,[self.new_accessor,*args])
        self.compare(symbol,old-self.image_old if old else None,new-self.image_new if new else None)

    def vectors(self, sampler):
        sm = self.word(self.record+8)+28*sampler
        vec = self.new.data+0x2800
        for output,address in [(False,0x669eec),(True,0x669e24)]:
            original = self.old.invoke(address,[self.old_accessor,sampler])
            assert self.new.invoke('dh2_animation_vector',[self.new_accessor,sampler,int(output),vec]) == 1
            count,pointer = struct.unpack('<II',self.old.uc.mem_read(original,8))
            native,nc,nt,components = struct.unpack('<QIII',self.new.uc.mem_read(vec,20))
            expected_type = self.word(sm+(16 if output else 4))
            expected_components = self.word(sm+(20 if output else 8))
            self.compare('output_vector' if output else 'time_vector',
                         (count,pointer-self.image_old,expected_type,expected_components),
                         (nc,native-self.image_new,nt,components))
            if not output:
                time_original = original; time_count = count
        return time_original,time_count

    def search(self, sampler, original_vector, ms, kind):
        key_old,key_new = self.old.data+0x3000,self.new.data+0x3000
        fraction_old,fraction_new = self.old.data+0x3010,self.new.data+0x3010
        for cpu,key,frac in [(self.old,key_old,fraction_old),(self.new,key_new,fraction_new)]:
            cpu.uc.mem_write(key,struct.pack('<i',-12345));cpu.uc.mem_write(frac,struct.pack('<f',-9.25))
        first_type = self.word(self.word(self.record+8)+4)
        if kind == 'raw':
            assert first_type in (1,3)
            address = 0x66a6d0 if first_type == 1 else 0x66abb8
            old = self.old.invoke(address,[self.old_accessor,original_vector,u32(ms),key_old])
            assert self.new.invoke('dh2_animation_vector',[self.new_accessor,sampler,0,self.new.data+0x2800]) == 1
            # Generic original dispatcher also selects sampler zero's type.
            self.new.uc.mem_write(self.new.data+0x2800+12,struct.pack('<I',first_type))
            new = self.new.invoke('dh2_animation_find_raw_index',[self.new.data+0x2800,u32(ms),key_new])
        elif kind == 'index':
            address = {1:0x66a788,3:0x66ac78,4:0x66a1f0}[first_type]
            old = self.old.invoke(address,[self.old_accessor,sampler,original_vector,u32(ms),key_old])
            new = self.new.invoke('dh2_animation_find_index',[self.new_accessor,sampler,u32(ms),key_new])
        else:
            address = {1:0x66a7c4,3:0x66acb4,4:0x66a2ac}[first_type]
            old = self.old.invoke(address,[self.old_accessor,sampler,original_vector,u32(ms),key_old,fraction_old])
            new = self.new.invoke('dh2_animation_find',[self.new_accessor,sampler,u32(ms),key_new,fraction_new])
        original = (u32(old),bytes(self.old.uc.mem_read(key_old,4)),bytes(self.old.uc.mem_read(fraction_old,4)))
        port = (u32(new),bytes(self.new.uc.mem_read(key_new,4)),bytes(self.new.uc.mem_read(fraction_new,4)))
        assert original == port, ('search',kind,first_type,ms,original,port,self.context)
        self.compare('search_'+kind+'_'+str(first_type),original,port)

    def check_animation(self, search=True, all_keys=False):
        for address,symbol in [(0x669e78,'dh2_animation_channels'),(0x669e54,'dh2_animation_has_default'),
                               (0x669ed4,'dh2_animation_scale_type'),(0x66a004,'dh2_animation_animator')]:
            self.scalar(address,symbol)
        self.pointer(0x669e00,'dh2_animation_target')
        if self.word(self.record+24):
            self.pointer(0x669e68,'dh2_animation_default')
        if self.word(self.record+28):
            self.pointer(0x669eb4,'dh2_animation_offsets'); self.pointer(0x669ec4,'dh2_animation_scales')
        for channel in range(self.word(self.record+12)):
            self.scalar(0x669e10,'dh2_animation_type',channel)
            self.pointer(0x669e44,'dh2_animation_channel',channel)
        for sampler in range(self.word(self.record+4)):
            self.samples['samplers'] += 1
            self.scalar(0x669e84,'dh2_animation_time_type',sampler)
            self.scalar(0x669e9c,'dh2_animation_interpolation',sampler)
            vector,count = self.vectors(sampler)
            if not count:
                continue
            keys = range(count) if all_keys else sorted({0,count//2,count-1})
            milliseconds = []
            for key in keys:
                old = self.old.invoke(0x669f0c,[self.old_accessor,sampler,key])
                new = self.new.invoke('dh2_animation_key_time',[self.new_accessor,sampler,key])
                self.compare('dh2_animation_key_time',u32(old),u32(new))
                milliseconds.append(i32(old))
            for address,symbol in [(0x669fac,'dh2_animation_start'),(0x669fb4,'dh2_animation_end'),(0x669fdc,'dh2_animation_length')]:
                self.scalar(address,symbol,sampler)
            if search:
                queries = sorted({v+d for v in milliseconds for d in [-1,0,1] if -2147483648 <= v+d < 2147483648})
                for ms in queries:
                    self.search(sampler,vector,ms,'index');self.search(sampler,vector,ms,'fraction')
                    if self.word(self.word(self.record+8)+4) in (1,3):
                        self.search(sampler,vector,ms,'raw')

def synthetic(times, time_type=4, interpolation=1, optional=True, mismatch=False):
    """Construct an original-format image; values are independent test fixtures."""
    raw = bytearray(2048+len(times)*24)
    root,record,samplers,channels,lib,seg,blob = 256,512,576,656,720,728,768
    def put(o,*values):
        struct.pack_into('<'+'I'*len(values),raw,o,*(u32(x) for x in values))
    raw[180:194] = b'fixture-name\0\0'; raw[200:215] = b'fixture-target\0'
    put(root,180);put(root+36,1,record);put(root+48,lib)
    n = 2 if mismatch else 1
    put(record,180,n,samplers,n,channels,0x12345678,1024 if optional else 0,1040 if optional else 0)
    put(samplers,interpolation,time_type,1,0,6,3,1)
    put(channels,-1,200,10,0x55667788)
    if mismatch:
        put(samplers+28,interpolation,3 if time_type == 1 else 1,1,0,6,3,1)
        put(channels+16,-1,200,5,0x11223344)
    put(lib,1,seg);put(seg,min(times),max(times),1,0,0,blob)
    times_at,values_at = 1152,1152+len(times)*4
    put(blob,2,len(times),times_at-(blob+8),len(times),values_at-(blob+16))
    fmt = {0:'b',1:'B',3:'H',4:'i'}[time_type]
    struct.pack_into('<'+fmt*len(times),raw,times_at,*times)
    struct.pack_into('<'+'f'*(len(times)*3),raw,values_at,*([1.25,-2.5,3.75]*len(times)))
    put(1024,7,3,1080);put(1040,1,1096,1112)
    struct.pack_into('<3f',raw,1080,1,2,3)
    struct.pack_into('<3f',raw,1096,2,2,2);struct.pack_into('<3f',raw,1112,-1,-1,-1)
    # Empty optional fields are omitted from the outer fixup list, as in cache.
    fields = [24,28,32,36,40,root,root+40,root+48,record,record+8,record+16,channels+4,lib+4,seg+20]
    if mismatch:
        fields.append(channels+20)
    if optional:
        fields += [record+24,record+28,1032,1044,1048]
    put(0,0x53455242,65534,60,len(raw),len(fields),0,60,60+len(fields)*4,root,root+192,len(raw),0,0,0,0)
    put(60,*fields)
    return bytes(raw)

def main():
    p = argparse.ArgumentParser()
    p.add_argument('--original', type=Path, required=True)
    p.add_argument('--cache', type=Path, required=True)
    p.add_argument('--ported', type=Path, default=ROOT/'build/libdh2_asset_payloads_arm64.so')
    p.add_argument('--report', type=Path, required=True)
    p.add_argument('--max-files', type=int, help='Smoke run; omit for the full corpus')
    a = p.parse_args()
    started = time.monotonic()
    provenance = json.loads((ROOT/'original-functions.json').read_text())
    verify_original(a.original,provenance)
    dependency_lib = ROOT/'build/libpayload_dependencies.so'
    subprocess.run(['cc','-shared','-fPIC','-O2','-fno-fast-math','-ffp-contract=off',str(ROOT/'tests/dependencies.c'),'-o',str(dependency_lib)],check=True)
    dependencies = Dependencies(dependency_lib)
    old,new = Cpu(a.original,False,provenance,dependencies),Cpu(a.ported,True,provenance,dependencies)
    check = Checks(old,new)
    rng = random.Random(0xd220026)
    fixtures = [(list(range(256)),1,1,True,False),
                (sorted({0,1,2,3,29,30,31,255,256,257,65534,65535}|{rng.randrange(65536) for _ in range(2048)}),3,1,True,False),
                ([-100,-1,0,1,33,34,100,100,250],4,1,False,False),
                ([0],4,1,True,False),([0,0,1,2,20],1,0,False,False),
                ([0,1,2,3,10,30],1,1,True,True),
                ([16777210,16777215,16777216,16777217,16777220],4,1,True,False),
                ([-10,0,10],0,1,True,False),
                ([-2147483648,0,2147483000],4,1,True,False)]
    for i,args in enumerate(fixtures):
        check.context = ('synthetic',i)
        check.load(synthetic(*args));check.select(0,0)
        check.check_animation(search=i not in (0,1,7),all_keys=True)
        if i in (0,1):
            vector,_ = check.vectors(0)
            for ms in [-100,-1,0,1,32,33,34,999,1000,8499,8500,8501,100000,2184499,2184500,2184501]:
                for kind in ['index','fraction','raw']:
                    check.search(0,vector,ms,kind)
    print(json.dumps({'synthetic_comparisons': sum(check.calls.values()), 'seconds': round(time.monotonic()-started,2)}),flush=True)
    files = sorted(a.cache.rglob('*.bdae'))
    if a.max_files:
        files = files[:a.max_files]
    for number,file in enumerate(files,1):
        raw = file.read_bytes(); root = struct.unpack_from('<I',raw,32)[0]
        animations = struct.unpack_from('<I',raw,root+36)[0]
        if not animations:
            continue
        check.load(raw)
        lib = check.word(root+48); segments = check.word(lib)
        for animation in sorted({0,animations//2,animations-1}):
            for segment in sorted({0,segments-1}):
                check.context = (str(file.relative_to(a.cache)),animation,segment)
                check.select(animation,segment);check.check_animation()
        check.samples['cache_files_with_animations_checked'] += 1
        if number % 200 == 0:
            print(json.dumps({'cache_files_scanned': number, 'comparisons': sum(check.calls.values()), 'seconds': round(time.monotonic()-started,2)}),flush=True)
    coverage = []
    for row in provenance['functions']:
        if row['scope'] != 'complete function':
            continue
        address = int(row['elf_address'],16); addresses = set(range(address,address+row['size'],4))
        coverage.append({'original_symbol': row['original_symbol'], 'elf_address': row['elf_address'],
                         'instruction_addresses_executed': len(addresses&old.seen), 'instruction_addresses': len(addresses)})
    assert all(row['instruction_addresses_executed'] == row['instruction_addresses'] for row in coverage), coverage
    report = {'original_sha256': provenance['original_sha256'], 'ported_sha256': hashlib.sha256(a.ported.read_bytes()).hexdigest(),
              'complete_engine': False, 'complete_accessor_search_functions': 26, 'partial_mesh_relocation_evidence_functions': 4,
              'comparisons': sum(check.calls.values()), 'mismatches': 0, 'calls': dict(check.calls), 'samples': dict(check.samples),
              'synthetic_fixtures': len(fixtures), 'cache_files_scanned': len(files),
              'sampling': 'first/middle/last animation and first/last segment in every cache file; first/middle/last key and boundary searches; synthetic typed/step/optional/mixed/large-time fixtures',
              'arm64_pointers_above_4gib': True,
              'dependency_model': 'Imported __aeabi scalar conversions/arithmetic/comparisons and libc only; original engine instructions and compiled ARM64 code execute without algorithm mocks.',
              'original_import_calls': old.import_calls, 'arm64_import_calls': new.import_calls,
              'coverage': coverage, 'elapsed_seconds': round(time.monotonic()-started,2)}
    a.report.parent.mkdir(parents=True,exist_ok=True);a.report.write_text(json.dumps(report,indent=2)+'\n')
    print(json.dumps({k:v for k,v in report.items() if k not in ('coverage','original_import_calls','arm64_import_calls')}))

if __name__ == '__main__':
    main()
