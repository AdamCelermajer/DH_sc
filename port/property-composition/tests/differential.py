#!/usr/bin/env python3
"""Original property recomposition with synthetic real tree/deque layouts."""
import argparse,ctypes as c,hashlib,importlib.util,json,random,struct
from collections import Counter
from pathlib import Path
from elftools.elf.elffile import ELFFile
from unicorn import UC_HOOK_CODE
ROOT=Path(__file__).resolve().parents[1];REPO=ROOT.parent.parent
spec=importlib.util.spec_from_file_location('property_test',ROOT/'../character-properties/tests/differential.py')
base=importlib.util.module_from_spec(spec);spec.loader.exec_module(base)
Sheet,Table=base.Sheet,base.Table
class Group(c.Structure):_fields_=[('sheets',c.POINTER(c.POINTER(Sheet))),('count',c.c_uint32)]
class Inputs(c.Structure):_fields_=[('base',c.POINTER(Sheet)),('saved',c.POINTER(Sheet)),('gears',c.POINTER(Sheet)),('groups',c.POINTER(Group)),('count',c.c_uint32)]
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest()
def main():
    p=argparse.ArgumentParser()
    for n in ('original','oracle','cache','host','arm64','report'):p.add_argument('--'+n,type=Path,required=True)
    a=p.parse_args();raw=(a.cache/'data/pydata/character_properties_pyarray.bin').read_bytes()
    old=base.original.Original(a.original,a.oracle,raw);m=old.machine
    new=base.original.cpu.EngineCpu(a.arm64,True,base.Dependencies(a.oracle),{'functions':[]})
    lib=c.CDLL(str(a.host.resolve()));lib.dh2_property_open.argtypes=[c.POINTER(Table),c.c_void_p,c.c_uint32]
    lib.dh2_property_recalc.argtypes=[c.POINTER(Table),c.POINTER(Inputs),c.c_uint32,c.POINTER(Sheet)]
    lib.dh2_property_recalc.restype=c.c_uint32
    buf=c.create_string_buffer(raw);table=Table();assert not lib.dh2_property_open(c.byref(table),buf,len(raw))
    new.uc.mem_map(new.data+0x10000,0x200000);np=new.data+0x10000;nv=new.data+0x100;ni=new.data+0x7000;nf=new.data+0x8000
    new.uc.mem_write(np,raw);assert not new.call('dh2_property_open',[nv,np,len(raw)])
    owner=m.data+0x3000;old_start=0xb000000;m.uc.mem_map(old_start,0x200000)
    defaults=old.tables[0]['rows'][0];types=old.tables[0]['rows'][1];rng=random.Random(20261002)
    calls=Counter();entries=Counter();sizes=Counter();samples=[]
    def entered(uc,address,size,unused):entries[hex(address)]+=1
    for address in (0x3dfe60,0x3de870,0x3de8b4):m.uc.hook_add(UC_HOOK_CODE,entered,begin=address,end=address)
    def run(id,type,values,groups,label):
        ob=old_start;nb=new.data+0x90000
        def oa(data):
            nonlocal ob
            p=ob;ob=(ob+len(data)+31)&~15;m.uc.mem_write(p,data);return p
        def na(data):
            nonlocal nb
            p=nb;nb=(nb+len(data)+31)&~15;new.uc.mem_write(p,data);return p
        def make(v):
            sheet=Sheet();sheet.values[:]=defaults;sheet.values[id]=v;return sheet
        plain=[make(v) for v in values[:3]];final=make(values[3]);snapshot=bytes(final)
        m.uc.mem_write(owner,b'\xa5'*0xe44)
        for off,s in zip((8,0x38c,0x710,0xa94),[*plain,final]):m.uc.mem_write(owner+off+4,bytes(s))
        np_plain=[na(bytes(s)) for s in plain]
        new.uc.mem_write(nf-16,b'\xa5'*928);new.uc.mem_write(nf,snapshot)
        header=owner+0xe18;nodes=[oa(bytes(0x60)) for g in groups]
        host_sheets=[];host_arrays=[];host_groups=(Group*len(groups))();native_groups=[]
        for k,vals in enumerate(groups):
            ss=[make(v) for v in vals];host_sheets.extend(ss)
            arr=(c.POINTER(Sheet)*len(ss))(*(c.pointer(s) for s in ss));host_arrays.append(arr)
            host_groups[k]=Group(arr,len(ss))
            native_array=na(b''.join(struct.pack('<Q',na(bytes(s))) for s in ss)) if ss else 0
            native_groups.append(struct.pack('<QI4x',native_array,len(ss)))
            # Original deque uses 32 four-byte pointers per block. Keep an extra
            # block for exact-boundary end iterators, and use real iterator bodies.
            pointers=[oa(b'\xa5'*4+bytes(s)) for s in ss]
            blocks=[]
            for start in range(0,(len(ss)//32+1)*32,32):
                pp=pointers[start:start+32];blocks.append(oa(struct.pack('<32I',*(pp+[0]*(32-len(pp))))))
            slots=oa(struct.pack('<'+'I'*len(blocks),*blocks))
            begin=struct.pack('<4I',blocks[0],blocks[0],blocks[0]+128,slots)
            last=len(ss)//32;end=struct.pack('<4I',blocks[last]+4*(len(ss)%32),blocks[last],blocks[last]+128,slots+4*last)
            m.uc.mem_write(nodes[k]+0x34,begin+end)
            sizes[len(ss)]+=1
        def tree(lo,hi,parent):
            if lo>=hi:return 0
            mid=(lo+hi)//2;node=nodes[mid]
            left=tree(lo,mid,node);right=tree(mid+1,hi,node)
            m.uc.mem_write(node,struct.pack('<4I',1,parent,left,right));return node
        root=tree(0,len(nodes),header)
        m.uc.mem_write(header,struct.pack('<4I',0,root,nodes[0] if nodes else header,nodes[-1] if nodes else header))
        ng=na(b''.join(native_groups)) if native_groups else 0
        new.uc.mem_write(ni,struct.pack('<4QI4x',*np_plain,ng,len(groups)))
        inputs=Inputs(*(c.pointer(s) for s in plain),host_groups,len(groups))
        # Actual cache types are used first; synthetic type combinations are
        # controlled replacements in both original and source fixture memory.
        type_bytes=struct.pack('<i',type)
        m.uc.mem_write(old.character_pointer+900+4+id*4,type_bytes)
        c.memmove(c.addressof(buf)+900+id*4,type_bytes,4);new.uc.mem_write(np+900+id*4,type_bytes)
        before_old=bytes(m.uc.mem_read(owner,0xe44));source_inputs=[bytes(s) for s in plain]+[bytes(s) for s in host_sheets]
        before_old_groups=bytes(m.uc.mem_read(old_start,ob-old_start));before_native_inputs=bytes(new.uc.mem_read(new.data+0x90000,nb-(new.data+0x90000)))
        old.invoke(0x3dfe60,[owner,id]);assert not lib.dh2_property_recalc(c.byref(table),c.byref(inputs),id,c.byref(final))
        assert not new.call('dh2_property_recalc',[nv,ni,id,nf])
        expected=bytes(m.uc.mem_read(owner+0xa98,896))
        assert expected==bytes(final)==bytes(new.uc.mem_read(nf,896)),(label,id,type,values,groups,struct.unpack_from('<i',expected,id*4)[0],final.values[id])
        after_old=bytes(m.uc.mem_read(owner,0xe44));at=0xa98+id*4
        assert before_old[:at]==after_old[:at] and before_old[at+4:]==after_old[at+4:]
        assert bytes(new.uc.mem_read(nf-16,16))==bytes(new.uc.mem_read(nf+896,16))==b'\xa5'*16
        assert source_inputs==[bytes(s) for s in plain]+[bytes(s) for s in host_sheets]
        assert before_old_groups==bytes(m.uc.mem_read(old_start,ob-old_start))
        assert before_native_inputs==bytes(new.uc.mem_read(new.data+0x90000,nb-(new.data+0x90000)))
        calls[label]+=1
        if len(samples)<12:samples.append({'field':id,'type':type,'group_sizes':[len(g) for g in groups],'output':final.values[id]})
    choose=lambda default:rng.choice([default,default,-2147483648,2147483647,-1,0,1,rng.randint(-100000,100000)])
    for id,type in enumerate(types):
        run(id,type,[choose(defaults[id]) for _ in range(4)],[[choose(defaults[id]) for _ in range(n)] for n in (0,2,33)],'actual cache type')
    id=36
    for type in range(-1,64):
        for lengths in ((),(0,),(1,),(2,3),(0,2,33)):
            run(id,type,[choose(defaults[id]) for _ in range(4)],[[choose(defaults[id]) for _ in range(n)] for n in lengths],'synthetic flag combinations')
    # Adversarial cancellation reaches the default between additions: preserve
    # the original sentinel replacement instead of normal mathematical summing.
    d=defaults[id]
    for vals,groups in [([5,d-5,7,123],[[9]]),([d,d,d,123],[[d],[1,2]]),([d,d,d,123],[[3,4],[d,d]]),([d,d,d,123],[[1]*64]),([d,d,d,123],[]),([2147483647,1,0,123],[[2147483647]])]:
        for type in (4,2,1,32,16,0):run(id,type,vals,groups,'sentinel and priority edges')
    with a.original.open('rb') as f:
        elf=ELFFile(f);syms=list(elf.get_section_by_name('.dynsym').iter_symbols());loads=[s for s in elf.iter_segments() if s['p_type']=='PT_LOAD']
        extra=[]
        for address in (0x3dfe60,0x3de870,0x3de8b4):
            s=next(s for s in syms if s['st_value']==address);n=s['st_size'];seg=next(s for s in loads if s['p_vaddr']<=address< s['p_vaddr']+s['p_filesz'])
            f.seek(seg['p_offset']+address-seg['p_vaddr']);extra.append({'elf_address':hex(address),'size':n,'symbol':s.name,'sha256':hashlib.sha256(f.read(n)).hexdigest()})
    r={'complete_game':False,'scripts_executed':False,'original_sha256':sha(a.original),'oracle_sha256':sha(a.oracle),'host_sha256':sha(a.host),'arm64_sha256':sha(a.arm64),
       'test_sha256':sha(Path(__file__)),'cache_sha256':hashlib.sha256(raw).hexdigest(),'seed':20261002,'cases':dict(calls),'comparisons':sum(calls.values()),'mismatches':0,
       'original_function_entries':dict(entries),'deque_sizes':dict(sizes),'sample_results':samples,'function_evidence':extra+old.evidence,
       'whole_state_comparison':True,'input_preservation':True,'output_guards':True,'stack_restoration':True,
       'dependency_model':'Actual original 1972-byte RecalcProperty, actual ordered tree traversal, both original deque iterator bodies and property helper bodies execute. Synthetic tree/deque/sheet fixtures cover original cache types and synthetic flags. Stream/allocation/disposal/memcpy/memmove/memcmp are bounded models; no tree/deque getter or recalc stub. Original map/deque construction, buff lifecycle/timers, entities, Lua and Android integration are unfinished. Source ARM64 executes in Unicorn, without hardware validation.'}
    a.report.write_text(json.dumps(r,indent=2)+'\n');print(json.dumps({k:v for k,v in r.items() if k not in ('function_evidence','sample_results')}))
if __name__=='__main__':main()
