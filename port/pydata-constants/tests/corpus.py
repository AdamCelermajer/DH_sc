#!/usr/bin/env python3
"""Compare source decoding against the recorded original reader's actual writes."""
import argparse
import ctypes as c
import hashlib
import importlib.util
import json
from pathlib import Path
import struct
ROOT=Path(__file__).resolve().parents[1];REPO=ROOT.parent.parent
spec=importlib.util.spec_from_file_location('constants_cpu',ROOT/'../animation-values/tests/differential.py')
cpu=importlib.util.module_from_spec(spec);spec.loader.exec_module(cpu)
class View(c.Structure):_fields_=[('bytes',c.c_void_p),('size',c.c_uint32),('groups',c.c_uint32),('entries',c.c_uint32)]
class Entry(c.Structure):_fields_=[(name,c.c_uint32)for name in ('group_offset','group_length','name_offset','name_length')]+[('value',c.c_int32)]
class Result(c.Structure):_fields_=[('found',c.c_uint32),('value',c.c_int32)]
class Dependencies(cpu.Dependencies):
    def call(self,machine,name):
        if name not in ('memcmp','memchr'):return super().call(machine,name)
        a,b,count=[machine.reg(i)for i in range(3)];raw=bytes(machine.uc.mem_read(a,count))if count else b''
        if name=='memchr':
            offset=raw.find(bytes([b&255]));value=0 if offset<0 else a+offset
        else:
            other=bytes(machine.uc.mem_read(b,count))if count else b''
            value=0 if raw==other else -1 if raw<other else 1
        machine.write_reg(0,value&0xffffffffffffffff)
        machine.uc.reg_write(machine.pc_reg,machine.uc.reg_read(machine.lr_reg))
def main():
    p=argparse.ArgumentParser()
    for name in ('cache','host','arm64','oracle','report'):p.add_argument('--'+name,type=Path,required=True)
    a=p.parse_args();sha=lambda path:hashlib.sha256(path.read_bytes()).hexdigest()
    trace_path=REPO/'reports/pydata-constant-reader-trace.json';trace=json.loads(trace_path.read_text())
    host=c.CDLL(str(a.host.resolve()));host.dh2_pycst_open.argtypes=[c.POINTER(View),c.c_void_p,c.c_uint32]
    host.dh2_pycst_copy_entries.argtypes=[c.POINTER(View),c.POINTER(Entry),c.c_uint32]
    host.dh2_pycst_get.argtypes=[c.POINTER(View),c.c_void_p,c.c_uint32,c.c_void_p,c.c_uint32,c.POINTER(Result)]
    for name in ('open','copy_entries','get'):getattr(host,'dh2_pycst_'+name).restype=c.c_uint32
    new=cpu.EngineCpu(a.arm64,True,Dependencies(a.oracle),{'functions':[]})
    base=new.data+0x100000;new.uc.mem_map(base,0x100000)
    raw_ptr,view_ptr,out_ptr,group_ptr,key_ptr,result_ptr=[base+x for x in (0,0x40000,0x50000,0x90000,0x90400,0x90800)]
    records=[];entries_total=0;lookups=0
    for file in trace['files']:
        path=a.cache/file['path'];raw=path.read_bytes();assert sha(path)==file['sha256']
        owner=c.create_string_buffer(raw);view=View();status=host.dh2_pycst_open(c.byref(view),owner,len(raw))
        new.uc.mem_write(raw_ptr,raw);new.uc.mem_write(view_ptr,b'\xa5'*40)
        remote=new.call('dh2_pycst_open',[view_ptr,raw_ptr,len(raw)])
        assert status==remote
        if not file['fully_consumed']:
            assert status==2 and bytes(new.uc.mem_read(view_ptr,40))==b'\xa5'*40
            records.append({'path':file['path'],'sha256':sha(path),'source_status':status,
                            'original_partial_entries':file['entries'],'original_consumed_bytes':file['consumed_bytes']})
            continue
        assert status==0 and view.entries==file['entries']
        remote_view=bytes(new.uc.mem_read(view_ptr,24))
        assert struct.unpack('<QIII',remote_view[:20])==(raw_ptr,view.size,view.groups,view.entries)
        assert remote_view[24:]==b'' and bytes(new.uc.mem_read(view_ptr+24,16))==b'\xa5'*16
        values=(Entry*view.entries)();assert host.dh2_pycst_copy_entries(c.byref(view),values,view.entries)==0
        new.uc.mem_write(out_ptr,b'\xa5'*(20*view.entries+16))
        assert new.call('dh2_pycst_copy_entries',[view_ptr,out_ptr,view.entries])==0
        assert bytes(new.uc.mem_read(out_ptr,20*view.entries))==bytes(values)
        assert bytes(new.uc.mem_read(out_ptr+20*view.entries,16))==b'\xa5'*16
        decoded=[]
        for value in values:
            group=raw[value.group_offset:value.group_offset+value.group_length].decode('ascii')
            key=raw[value.name_offset:value.name_offset+value.name_length].decode('ascii')
            decoded.append({'group':group,'name':key,'value':value.value})
        assert decoded==file['rows'];entries_total+=len(decoded)
        expected={(row['group'],row['name']):row['value']for row in decoded}
        candidates=list(expected.items())[::max(1,len(expected)//20)]+[(('__missing__','__missing__'),None)]
        for (group,key),value in candidates:
            gg,kk=group.encode(),key.encode();result=Result()
            assert host.dh2_pycst_get(c.byref(view),gg,len(gg),kk,len(kk),c.byref(result))==0
            assert (result.found,result.value)==(int(value is not None),value if value is not None else 0)
            new.uc.mem_write(group_ptr,gg);new.uc.mem_write(key_ptr,kk);new.uc.mem_write(result_ptr,b'\xa5'*24)
            assert new.call('dh2_pycst_get',[view_ptr,group_ptr,len(gg),key_ptr,len(kk),result_ptr])==0
            assert bytes(new.uc.mem_read(result_ptr,8))==bytes(result)
            assert bytes(new.uc.mem_read(result_ptr+8,16))==b'\xa5'*16
            lookups+=1
        assert bytes(new.uc.mem_read(raw_ptr,len(raw)))==raw and owner.raw[:len(raw)]==raw
        records.append({'path':file['path'],'sha256':sha(path),'source_status':status,
                        'groups':view.groups,'entries_matched':view.entries})
    result={'complete_game':False,'scripts_executed':False,'reader_trace_sha256':sha(trace_path),
            'host_sha256':sha(a.host),'arm64_sha256':sha(a.arm64),'oracle_sha256':sha(a.oracle),
            'source_sha256':{name:sha(ROOT/name)for name in ('constants.c','constants.h')},
            'test_sha256':sha(Path(__file__)),'entries_matched':entries_total,'source_lookup_checks':lookups,
            'files':records,'input_preservation':True,'output_guards':True,'stack_restoration':True,
            'dependency_model':'Source ARM64/host decode all rows from 26 complete integer files and match recorded original ARM32 reader insertion writes; source lookups are checked against those records, not original map lookup. Source memchr/memcmp imports use a bounded byte model. The mixed sound file is deliberately rejected rather than importing original partial/misread writes.',
            'import_calls':new.import_calls}
    a.report.write_text(json.dumps(result,indent=2)+'\n');print(json.dumps({k:v for k,v in result.items()if k!='files'}))
if __name__=='__main__':main()
