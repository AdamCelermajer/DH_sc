#!/usr/bin/env python3
import argparse
import ctypes as c
import hashlib
import importlib.util
import json
from pathlib import Path
import struct
ROOT=Path(__file__).resolve().parents[1];REPO=ROOT.parent.parent
spec=importlib.util.spec_from_file_location('constant_corpus',ROOT/'../pydata-constants/tests/corpus.py')
dep=importlib.util.module_from_spec(spec);spec.loader.exec_module(dep)
class View(c.Structure):_fields_=[('bytes',c.c_void_p),('size',c.c_uint32),('count',c.c_uint32)]
class Name(c.Structure):_fields_=[('offset',c.c_uint32),('length',c.c_uint32)]
def main():
    p=argparse.ArgumentParser()
    for name in ('cache','host','arm64','oracle','report'):p.add_argument('--'+name,type=Path,required=True)
    a=p.parse_args();sha=lambda path:hashlib.sha256(path.read_bytes()).hexdigest()
    trace_path=REPO/'reports/pydata-array-name-trace.json';trace=json.loads(trace_path.read_text())
    host=c.CDLL(str(a.host.resolve()));host.dh2_pynames_open.argtypes=[c.POINTER(View),c.c_void_p,c.c_uint32]
    host.dh2_pynames_copy.argtypes=[c.POINTER(View),c.POINTER(Name),c.c_uint32]
    host.dh2_pynames_get.argtypes=[c.POINTER(View),c.c_void_p,c.c_uint32,c.POINTER(c.c_int32)]
    for name in ('open','copy','get'):getattr(host,'dh2_pynames_'+name).restype=c.c_uint32
    new=dep.cpu.EngineCpu(a.arm64,True,dep.Dependencies(a.oracle),{'functions':[]})
    raw_ptr,view_ptr,out_ptr,key_ptr,result_ptr=[new.data+x for x in (0x1000,0x100,0xc000,0x800,0x200)]
    records=[];lookups=0;names_count=0;host_all=0
    for file in trace['files']:
        path=a.cache/file['path'];raw=path.read_bytes();assert sha(path)==file['sha256']
        for table in file['tables']:
            segment=raw[table['start']:table['end']];assert len(segment)<0xb000
            owner=c.create_string_buffer(segment);view=View()
            assert host.dh2_pynames_open(c.byref(view),owner,len(segment))==0 and view.count==table['count']
            new.uc.mem_write(raw_ptr,segment);new.uc.mem_write(view_ptr,b'\xa5'*32)
            assert new.call('dh2_pynames_open',[view_ptr,raw_ptr,len(segment)])==0
            assert struct.unpack('<QII',new.uc.mem_read(view_ptr,16))==(raw_ptr,view.size,view.count)
            assert bytes(new.uc.mem_read(view_ptr+16,16))==b'\xa5'*16
            values=(Name*view.count)();assert host.dh2_pynames_copy(c.byref(view),values,view.count)==0
            new.uc.mem_write(out_ptr,b'\xa5'*(8*view.count+16))
            assert new.call('dh2_pynames_copy',[view_ptr,out_ptr,view.count])==0
            assert bytes(new.uc.mem_read(out_ptr,8*view.count))==bytes(values)
            assert bytes(new.uc.mem_read(out_ptr+8*view.count,16))==b'\xa5'*16
            names=[segment[row.offset:row.offset+row.length].decode('ascii')for row in values]
            assert names==table['names'];names_count+=len(names)
            first={}
            for i,name in enumerate(names):first.setdefault(name,i)
            for name,index in first.items():
                index_out=c.c_int32();key=name.encode()
                assert host.dh2_pynames_get(c.byref(view),key,len(key),c.byref(index_out))==0 and index_out.value==index
                host_all+=1
            selected=list(dict.fromkeys(names[i]for i in (0,len(names)//2,len(names)-1)))if names else []
            selected+=['__missing__']
            for name in selected:
                index=first.get(name,-1);key=name.encode();new.uc.mem_write(key_ptr,key)
                new.uc.mem_write(result_ptr,b'\xa5'*20)
                assert new.call('dh2_pynames_get',[view_ptr,key_ptr,len(key),result_ptr])==0
                assert struct.unpack('<i',new.uc.mem_read(result_ptr,4))[0]==index
                assert bytes(new.uc.mem_read(result_ptr+4,16))==b'\xa5'*16;lookups+=1
            assert owner.raw[:len(segment)]==segment and bytes(new.uc.mem_read(raw_ptr,len(segment)))==segment
            records.append({'path':file['path'],'registered_name':table['registered_name'],'class':table['class'],
                            'source_segment_sha256':hashlib.sha256(segment).hexdigest(),'names':view.count})
    assert names_count==trace['names'] and lookups==trace['original_lookup_checks']
    result={'complete_game':False,'scripts_executed':False,'reader_trace_sha256':sha(trace_path),
            'names_matched':names_count,'tables':len(records),'host_all_unique_lookups':host_all,
            'arm64_lookup_cases_matching_original_cases':lookups,'host_sha256':sha(a.host),'arm64_sha256':sha(a.arm64),
            'oracle_sha256':sha(a.oracle),'test_sha256':sha(Path(__file__)),
            'source_sha256':{name:sha(ROOT/name)for name in ('names.c','names.h')},
            'input_preservation':True,'output_guards':True,'stack_restoration':True,'records':records,
            'dependency_model':'Source ARM64/host decoded all names matching recorded original readers. Source ARM64 queries repeat the 266 original lookup cases from the trace; all unique names are additionally checked on host. Libc byte helpers use a bounded model. Array-record size agreement, original allocation/registration ownership, Structs field names and script behavior remain outside scope.',
            'import_calls':new.import_calls}
    a.report.write_text(json.dumps(result,indent=2)+'\n');print(json.dumps({k:v for k,v in result.items()if k!='records'}))
if __name__=='__main__':main()
