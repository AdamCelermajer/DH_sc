"""Original ordered union/default/accessor instructions versus compiled ARM64.

Resource selection and allocator/lifetime are explicit caller fixtures. Actual
getAnimationValue65f7b4, getBlendableAnimation61c1e0, getDefaultValue61c6bc,
template filter6e22cc, ordered union6601fc and typed/key interpreters execute.
No timeline/events, complete generic compiler or blended scene claim is made.
"""
import argparse,ctypes,hashlib,json,struct,sys,time,importlib.util
from pathlib import Path
from unicorn import UC_HOOK_CODE
from elftools.elf.elffile import ELFFile

ROOT=Path(__file__).resolve().parents[1];REPO=ROOT.parents[1]
sys.path.insert(0,str(ROOT/'tests'))
from animation_blend_differential import Cpu as MathCpu,bits,floating,words

def u32(value):return value&0xffffffff
def i32(value):return ctypes.c_int32(value).value
def word(raw,offset):return struct.unpack_from('<I',raw,offset)[0]
def block(raw):return words([len(raw)])+raw
def sha(path):return hashlib.sha256(path.read_bytes()).hexdigest()

class Cpu(MathCpu):
    def __init__(self,*args):
        super().__init__(*args);self.heap=self.data+0x800000;self.allocations={}
        if self.arm64:
            with Path(args[0]).open('rb') as stream:
                elf=ELFFile(stream)
                for section in elf.iter_sections():
                    if section['sh_type']!='SHT_RELA':continue
                    symbols=elf.get_section(section['sh_link'])
                    for relocation in section.iter_relocations():
                        if relocation['r_info_type'] not in (257,1025):continue
                        symbol=symbols.get_symbol(relocation['r_info_sym'])
                        if symbol['st_shndx']!='SHN_UNDEF':value=self.base+symbol['st_value']
                        else:
                            value=self.extern+len(self.imports)*16;self.imports[value]=symbol.name
                        self.pointer(self.base+relocation['r_offset'],value+relocation['r_addend'])
    def string(self,address):
        out=bytearray()
        for i in range(4096):
            b=self.uc.mem_read(address+i,1)[0]
            if not b:return bytes(out)
            out.append(b)
        raise AssertionError('Unterminated modeled libc string')
    def external(self,uc,address,size,unused):
        name=self.imports.get(address)
        if name in ('__aeabi_i2f','__aeabi_ui2f','__aeabi_f2iz'):
            value=floating(self.reg(0)) if name.endswith('f2iz') else i32(self.reg(0)) if name=='__aeabi_i2f' else u32(self.reg(0))
            self.put(0,u32(int(value)) if name.endswith('f2iz') else bits(value))
        elif name in ('__aeabi_i2d','__aeabi_ui2d','__aeabi_dmul','__aeabi_ddiv','__aeabi_d2iz'):
            number=lambda r:struct.unpack('<d',words([self.reg(r),self.reg(r+1)]))[0]
            if name=='__aeabi_d2iz':self.put(0,u32(int(number(0))))
            else:
                value=float(i32(self.reg(0))) if name=='__aeabi_i2d' else float(u32(self.reg(0))) if name=='__aeabi_ui2d' else number(0)*number(2) if name=='__aeabi_dmul' else number(0)/number(2)
                lo,hi=struct.unpack('<II',struct.pack('<d',value));self.put(0,lo);self.put(1,hi)
        elif name in ('strcmp','strlen','strncmp'):
            left=self.string(self.reg(0))
            if name=='strlen':self.put(0,len(left))
            else:
                right=self.string(self.reg(1))
                if name=='strncmp':left,right=left[:self.reg(2)],right[:self.reg(2)]
                self.put(0,u32((left>right)-(left<right)))
        elif name=='memchr':
            raw=bytes(uc.mem_read(self.reg(0),self.reg(2)));pos=raw.find(bytes([self.reg(1)&255]));self.put(0,self.reg(0)+pos if pos>=0 else 0)
        elif name=='wmemchr':
            pointer,value,count=self.reg(0),u32(self.reg(1)),self.reg(2);raw=bytes(uc.mem_read(pointer,count*4));index=next((i for i in range(count) if word(raw,i*4)==value),None);self.put(0,pointer+index*4 if index is not None else 0)
        elif name=='memcmp':
            left=bytes(uc.mem_read(self.reg(0),self.reg(2)));right=bytes(uc.mem_read(self.reg(1),self.reg(2)));self.put(0,u32((left>right)-(left<right)))
        elif name in ('malloc','calloc','realloc','_Znwm','_Znwj'):
            old=self.reg(0) if name=='realloc' else 0
            count=self.reg(0)*self.reg(1) if name=='calloc' else self.reg(1) if name=='realloc' else self.reg(0)
            assert count<0x1000000,(name,count)
            pointer=(self.heap+15)&~15;self.heap=pointer+max(16,count);assert self.heap<self.data+0x1f00000
            self.allocations[pointer]=count
            if old:uc.mem_write(pointer,bytes(uc.mem_read(old,min(count,self.allocations[old]))))
            if name=='calloc':uc.mem_write(pointer,bytes(count))
            self.put(0,pointer)
        elif name in ('free','_ZdlPv','_ZdlPvm','_ZdaPv','__cxa_atexit'):self.put(0,0)
        else:return super().external(uc,address,size,unused)
        self.import_calls[name]=self.import_calls.get(name,0)+1;uc.reg_write(self.pc,uc.reg_read(self.lr))

def nodes(raw):
    result=[];root=word(raw,32)
    def visit(offset):
        at=word(raw,offset);end=raw.index(0,at);result.append((raw[at:end].decode(),offset))
        for i in range(word(raw,offset+56)):visit(word(raw,offset+60)+i*80)
    if word(raw,root+152):
        scene=word(raw,root+156)
        for i in range(word(raw,scene+8)):visit(word(raw,scene+12)+i*80)
    return result

def relocate(cpu,raw,address):
    fixed=bytearray(raw)
    for i in range(word(raw,16)):
        field=word(raw,word(raw,24)+i*4);struct.pack_into('<I',fixed,field,address+word(raw,field))
    cpu.uc.mem_write(address,bytes(fixed));return address+word(raw,32)

def database(cpu,root,address):
    cpu.pointer(address,address+0x100);cpu.pointer(address+0x124,address+0x200);cpu.pointer(address+0x220,root)
    return address

def main():
    p=argparse.ArgumentParser();p.add_argument('--engine',type=Path,required=True);p.add_argument('--library',type=Path,required=True)
    p.add_argument('--report',type=Path,required=True);p.add_argument('--reference-output',type=Path,required=True);a=p.parse_args();started=time.monotonic()
    manifest=json.loads((ROOT/'reference/compiled-transforms/original-functions.json').read_text());assert sha(a.engine)==manifest['original_sha256']
    old=Cpu(a.engine,False,manifest);new=Cpu(a.library,True,{'functions':[]})
    assets=REPO/'port/android-native/app/src/main/assets';model=(assets/'models/prince_modular.bdae').read_bytes();authored=nodes(model);assert len(authored)==35
    names=['prince_idle_shield.bdae','prince_walk_1hand.bdae','prince_1hand_combo_01.bdae','prince_1hand_combo_01_moving.bdae','prince_dying_01.bdae']
    raws=[(assets/'animations'/n).read_bytes() for n in names];ids=list(range(100,105))
    # Genuine accessor bytes with an intentionally unregistered URI exercises
    # original template filtering and absent-template retained buffers.
    changed=bytearray(raws[1]);r=word(changed,32);rec=word(changed,r+40);channel=word(changed,rec+16);uri=word(changed,channel+4);end=changed.index(0,uri)
    changed[uri:end]=b'Z'*(end-uri);raws.append(bytes(changed));ids.append(105)
    empty=bytearray(raws[0]);struct.pack_into('<I',empty,word(empty,32)+36,0);raws.append(bytes(empty));ids.append(106)
    # Independent original-format fixture: one source scale track in two
    # contiguous segments and a clip DB node default overriding the template.
    spec=importlib.util.spec_from_file_location('payload_fixture',REPO/'port/asset-payloads/tests/differential.py');module=importlib.util.module_from_spec(spec);spec.loader.exec_module(module)
    custom=bytearray(module.synthetic([0,25,75],optional=False));put=lambda offset,*values:struct.pack_into('<'+'I'*len(values),custom,offset,*[u32(x) for x in values])
    root=word(custom,32);rec=word(custom,root+40);lib=word(custom,root+48)
    put(rec+20,0);custom[200:216]=b'NAMEroot_camera\0';put(lib,2,1400)
    put(1400,0,75,1,0,0,768);put(1424,75,175,1,0,0,1500);put(1500,2,3,1560-1508,3,1600-1516)
    put(1560,75,100,175);struct.pack_into('<9f',custom,1600,1,2,3,4,5,6,7,8,9)
    put(root+152,1,1720);put(1720,200,0,1,1800);put(1800,200,0,0)
    struct.pack_into('<3f4f3f',custom,1812,-0.,12.,13.,0.,0.,0.,1.,2.,3.,4.)
    put(1816,0x7fc01234,0x7f800000) # mode1/default copies preserve IEEE bytes
    fields=[word(custom,60+i*4) for i in range(word(custom,16))]+[1420,1444,root+156,1720,1732,1800]
    assert 60+len(fields)*4<=180;put(16,len(fields));put(28,60+len(fields)*4);put(60,*fields)
    raws.append(bytes(custom));ids.append(107)
    short=bytearray(module.synthetic([0,30,60],time_type=3,optional=False));put_short=lambda offset,*values:struct.pack_into('<'+'I'*len(values),short,offset,*[u32(x) for x in values])
    root=word(short,32);rec=word(short,root+40);lib=word(short,root+48);segment=word(short,lib+4)
    short[200:216]=b'NAMEroot_camera\0';put_short(rec+20,0);put_short(segment,0,2000)
    raws.append(bytes(short));ids.append(108)
    image=old.data+0x10000;modelroot=relocate(old,model,image);modeldb=database(old,modelroot,old.data+0x1000)
    image_addresses=[];databases=[]
    for ci,raw in enumerate(raws):
        address=old.data+0x400000+ci*0x10000;image_addresses.append(address);databases.append(database(old,relocate(old,raw,address),old.data+0x2000+ci*0x400))
    # Actual original compatibility constructor, with only initialized C++ guard
    # fixture; table entries and all union comparisons are original instructions.
    old.uc.mem_write(0x9f7110,words([1]));table=old.invoke(0x670a60,[]);old.pointer(0x9f7570,table)
    union=old.data+0x5000;union_ids=union+0x100;union_types=union+0x800
    template=old.data+0x7000;entries=template+0x100;entry_records=template+0x1000;node_records=template+0x3000;node_vtable=template+0x5000
    service=old.data+0xe000;current_database=0;current_data=0
    def ret(value=0):old.put(0,value);old.uc.reg_write(old.pc,old.uc.reg_read(old.lr))
    def services(uc,address,size,user):
        if address==0x611ae0:ret(node_records) # nonnull extended-track factory identity; no sampling replacement
        elif address==service:ret(old.reg(0)+4) # original node ID virtual getter fixture
        elif address==0x65f0b4:ret(current_database) # selected immutable resource, no event/timeline producer
        elif address==0x65f364:ret(current_data) # already selected immutable segment
    old.uc.hook_add(UC_HOOK_CODE,services)
    old.pointer(node_vtable+0x54,service);old.pointer(template+4,entries);old.pointer(template+8,entries+len(authored)*3*4)
    for i,(name,offset) in enumerate(authored):
        node=node_records+i*128;old.pointer(node,node_vtable);old.uc.mem_write(node+4,name.encode()+b'\0')
        for j,t in enumerate((1,5,10)):
            entry=entry_records+(i*3+j)*16;old.uc.mem_write(entry,words([0,t,node,0]));old.pointer(entries+(i*3+j)*4,entry)
    cpus=[];native_model=new.data+0x10000;new.uc.mem_write(native_model,model)
    ptrs=new.data+0x2000;sizes=ptrs+0x100;native_ids=sizes+0x100
    for ci,raw in enumerate(raws):
        address=new.data+0x400000+ci*0x10000;new.uc.mem_write(address,raw);new.pointer(ptrs+ci*8,address);new.uc.mem_write(sizes+ci*4,words([len(raw)]));new.uc.mem_write(native_ids+ci*4,words([ids[ci]]))
    fixture=new.invoke('dh2_transform_test_create',[native_model,len(model),len(raws),ptrs,sizes,native_ids],budget=30000000);assert fixture
    gold=words([0x31535443])+block(model)+words([len(raws)])+b''.join(words([cid])+block(raw) for cid,raw in zip(ids,raws));records=[]
    target_rows=[];binding_rows=[];union_calls=0;filter_calls=0;default_calls=0;mode_counts={1:0,2:0};retained=0;sample_calls=0
    scratch=old.data+0x6000;native_scratch=new.data+0x6000;set_object=old.data+0xc000;animator=set_object+0x100;record_table=old.data+0x500000;extended=record_table+0x8000;cursors=extended+0x1000;accessor=old.data+0x9000
    for policy in range(2):
        old.uc.mem_write(union,bytes(64));old.pointer(union+12,union_ids);old.pointer(union+16,union_ids);old.pointer(union+20,union_ids+0x400);old.pointer(union+24,union_types);old.pointer(union+28,union_types);old.pointer(union+32,union_types+0x400)
        for ci,raw in enumerate(raws):
            root=word(raw,32)
            for index in range(word(raw,root+36)):
                rec=word(raw,root+40)+32*index;ch=word(raw,rec+16);pointer=image_addresses[ci]+rec
                if policy==0:
                    filter_calls+=1
                    if not old.invoke(0x6e22cc,[template,image_addresses[ci]+ch]):continue
                old.invoke(0x6601fc,[union,pointer]);union_calls+=1
        n=(word(bytes(old.uc.mem_read(union,64)),16)-union_ids)//4;assert new.invoke('dh2_transform_test_count',[fixture,policy])==n
        targets=[];gold+=words([n]);target_rows.append([])
        for ti in range(n):
            ch=word(bytes(old.uc.mem_read(union_ids+ti*4,4)),0);uri_pointer=word(bytes(old.uc.mem_read(ch+4,4)),0);uri_name=old.string(uri_pointer).decode();t=word(bytes(old.uc.mem_read(ch+8,4)),0);width=4 if t==5 else 3
            node=next((i for i,(name,off) in enumerate(authored) if name==uri_name),0xffffffff)
            assert new.invoke('dh2_transform_test_target',[fixture,policy,ti,native_scratch,native_scratch+0x100,4096])==1
            assert bytes(new.uc.mem_read(native_scratch,12))==words([t,node,width]) and new.string(native_scratch+0x100).decode()==uri_name
            targets.append((ch,uri_pointer,uri_name,t,node,width));target_rows[-1].append({'uri':uri_name,'type':t,'node':node,'components':width});gold+=block(uri_name.encode())+words([t,node,width])
        bindings=[];binding_rows.append([])
        for ci,raw in enumerate(raws):
            clip_bindings=[]
            for ti,(ch,uri_pointer,name,t,node,width) in enumerate(targets):
                track=old.invoke(0x61c1e0,[databases[ci],ch]);mode=2 if track else 1
                old.uc.mem_write(scratch,bytes(16));has=old.invoke(0x61c6bc,[databases[ci],ch,scratch]);default_pointer=word(bytes(old.uc.mem_read(scratch,4)),0);default_calls+=1
                if mode==1 and not has and policy==0:
                    has=old.invoke(0x61c6bc,[modeldb,ch,scratch]);default_pointer=word(bytes(old.uc.mem_read(scratch,4)),0);default_calls+=1
                value=bytes(old.uc.mem_read(default_pointer,width*4)) if has else bytes(width*4);value+=bytes(16-len(value));expected=words([mode,int(bool(has))])+value
                assert new.invoke('dh2_transform_test_binding',[fixture,policy,ci,ti,native_scratch])==1
                assert bytes(new.uc.mem_read(native_scratch,24))==expected,(policy,ci,ti,expected.hex(),bytes(new.uc.mem_read(native_scratch,24)).hex())
                gold+=expected;mode_counts[mode]+=1;clip_bindings.append((mode,default_pointer if has else 0,track));binding_rows[-1].append({'clip':ci,'target':ti,'mode':mode,'has_default':bool(has),'value_hex':value.hex()})
            bindings.append(clip_bindings)
        # getAnimationValue executes default-copy/retained branches itself, then
        # invokes the actual original accessor via a real typed ELF vtable.
        old.pointer(animator+0x24,set_object);old.pointer(set_object+0x30,record_table);old.pointer(set_object+0x18,extended);old.pointer(animator+0x40,cursors)
        vtables={1:0x978ef8,5:0x978ae8,10:0x979618}
        for ti,(_,_,_,t,_,_) in enumerate(targets):
            object_=extended+0x200+ti*16;old.pointer(object_,vtables[t]);old.pointer(extended+ti*4,object_)
        for ci,raw in enumerate(raws):
            root=word(raw,32);lib=word(raw,root+48);segments=word(raw,lib) if lib else 0;segmentbase=word(raw,lib+4) if lib else 0
            segment_ranges=[(i32(word(raw,segmentbase+i*24)),i32(word(raw,segmentbase+i*24+4))) for i in range(segments)]
            current_database=databases[ci];old.uc.mem_write(animator+0x4c,words([ci*n]));old.uc.mem_write(animator+0x50,words([ci]))
            for ti,(mode,default_pointer,track) in enumerate(bindings[ci]):
                old.uc.mem_write(record_table+(ci*n+ti)*12,words([mode,default_pointer,track]))
                if track:old.pointer(track+20,extended+0x200+ti*16)
            queries=[-1,0,1,33,199,401,799,1201] if not segment_ranges else sorted(set([segment_ranges[0][0]-1,segment_ranges[0][0],segment_ranges[0][0]+1,(segment_ranges[0][0]+segment_ranges[-1][1])//2,segment_ranges[-1][1]-1,segment_ranges[-1][1],segment_ranges[-1][1]+1,33,199,401]+[x+d for start,end in segment_ranges for x in (start,end) for d in (-1,0,1)]))
            for ti,(_,_,_,t,_,width) in enumerate(targets):
                for interpolate in (0,1):
                    for ms in queries:
                        segment=0
                        while segment+1<len(segment_ranges) and ms>=segment_ranges[segment][1]:segment+=1
                        if segments:
                            seg=segmentbase+segment*24;offset=word(raw,seg+12 if word(raw,seg+8)==0 else seg+20);current_data=image_addresses[ci]+offset
                            for i in range(word(raw,offset)):
                                slot=offset+8+i*8;old.pointer(image_addresses[ci]+slot,image_addresses[ci]+slot+i32(word(raw,slot)))
                        else:current_data=0
                        initial=words([0x3f123456,0x80000000,0xbf234567,0x7fc01234]);initial_cursor=[0,1,7,-1,2147483647][(ti+ms+interpolate)%5]
                        if ci==7 and ms in (75,100,175):initial_cursor=0 # recovered inclusive-neighbor regression
                        old.uc.mem_write(scratch,initial);old.uc.mem_write(cursors+ti*4,words([u32(initial_cursor)]));old.uc.mem_write(animator+12,words([0 if interpolate else 1]));old.trig=[]
                        old.invoke(0x65f7b4,[animator,ti,u32(ms),scratch]);expected=bytes(old.uc.mem_read(scratch,16));key=word(bytes(old.uc.mem_read(cursors+ti*4,4)),0);trace=old.trig[:]
                        new.uc.mem_write(native_scratch,initial);new.uc.mem_write(native_scratch+0x20,words([u32(initial_cursor)]));new.trig=[]
                        assert new.invoke('dh2_transform_test_sample',[fixture,policy,ci,ti,u32(ms),native_scratch,4,native_scratch+0x20,interpolate])==1
                        actual=bytes(new.uc.mem_read(native_scratch,16));actual_key=word(bytes(new.uc.mem_read(native_scratch+0x20,4)),0)
                        assert expected==actual and key==actual_key,(policy,ci,ti,name,ms,interpolate,expected.hex(),actual.hex(),key,actual_key)
                        assert [x for x in trace if x[0]!=2]==[x for x in new.trig if x[0]!=2]
                        records.append(words([policy,ci,ti,u32(ms),interpolate,u32(initial_cursor)])+initial+words([key])+expected+words([len(trace)])+b''.join(words(x) for x in trace));sample_calls+=1;retained+=expected==initial
    new.invoke('dh2_transform_test_destroy',[fixture]);gold+=words([len(records)])+b''.join(records);a.reference_output.parent.mkdir(parents=True,exist_ok=True);a.reference_output.write_bytes(gold)
    sources=[ROOT/'animation.hpp',ROOT/'animation.cpp',Path(__file__),ROOT/'tests/compiled_transforms.cpp']
    sources += [REPO/p for p in ('port/engine-animation/events.cpp','port/engine-animation/event_track.cpp','port/engine-animation/events.hpp','port/scene-materials/scene.cpp','port/scene-materials/scene.hpp','port/asset-payloads/payloads.cpp','port/asset-payloads/payloads.hpp','port/engine-resources/resources.cpp','port/engine-resources/resources.hpp','port/engine-math/math.cpp','port/engine-math/math.hpp')]
    report={'validation':'PASS','original_sha256':manifest['original_sha256'],'original_manifest_sha256':sha(ROOT/'reference/compiled-transforms/original-functions.json'),'arm64_library_sha256':sha(a.library),'reference_sha256':sha(a.reference_output),'original_instructions_executed':True,'compiled_arm64_instructions_executed':True,'source_sha256':{str(p.relative_to(REPO)).replace('\\','/'):sha(p) for p in sources},'clips':len(raws),'authored_nodes':35,'ordered_target_counts':[len(x) for x in target_rows],'original_union_calls':union_calls,'original_template_filter_calls':filter_calls,'original_default_calls':default_calls,'mode_counts':mode_counts,'original_raw_samples':sample_calls,'retained_samples':retained,'mismatches':0,'inputs':[{'asset':n,'sha256':sha(assets/'animations'/n)} for n in names],'synthetic_inputs':['same-cache-key track with unregistered URI','same-cache-key image with empty animation library','original-format two-segment scale accessor with clip DB IEEE defaults','original-format ushort time accessor'],'scope':'Full node types1/5/10 only, source ordered union/default/getAnimationValue/accessor instructions. Extended-track factory identity, node ID virtual getter, selected immutable clip and segment are explicit oracle fixtures. No timeline/event/FSM/blended whole-scene oracle, generic compiler, compression or historical Bionic libm parity claim.','elapsed_seconds':round(time.monotonic()-started,2),'original_imports':old.import_calls,'native_imports':new.import_calls}
    a.report.parent.mkdir(parents=True,exist_ok=True);a.report.write_text(json.dumps(report,indent=2)+'\n');print(json.dumps({k:v for k,v in report.items() if k not in ('source_sha256','inputs','original_imports','native_imports','scope')}))
if __name__=='__main__':main()
