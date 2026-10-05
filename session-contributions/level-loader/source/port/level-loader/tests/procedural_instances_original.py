"""Execute original procedural runtime constructors and caller control flow.

Original XML/list/rule readers prepare each source tree. Each runtime rule
constructor then executes ARM32 with an explicit seed and parent fixture.
Original GetInt/NextInt and pool lookup execute; integer division, allocation
and C services are explicit adapters. Preorder is a test invocation order,
not the unverified layout traversal order. The Level caller audit executes
its ARM instructions with loader/generator/serializer/constructor/destructor
services replaced by recorded boundary fixtures; it proves branching and
return policy only, not those substituted services' behavior.
"""
import argparse,hashlib,json,pathlib,struct,sys,zipfile
from procedural_rules_original import create_original_rules_cpu

def create_original_instances_cpu(engine,dependency_root):
    base=type(create_original_rules_cpu(engine,dependency_root))
    class Instances(base):
        def external(self,uc,address,size,unused):
            name=self.imports.get(address)
            if name=='__aeabi_uldivmod':
                q,r=divmod(self.reg(0)|(self.reg(1)<<32),self.reg(2)|(self.reg(3)<<32))
                for i,v in enumerate((q&0xffffffff,q>>32,r&0xffffffff,r>>32)):self.put(i,v)
                self.returned(q&0xffffffff)
            elif name=='__aeabi_uidivmod':
                q,r=divmod(self.reg(0),self.reg(1));self.put(1,r);self.returned(q)
            else:return super().external(uc,address,size,unused)
        def instances(self,raw,names,seeds):
            tree=self.execute_rules(raw,names)
            pointers=[]
            def visit(p,path):
                pointers.append((p,path))
                for i in range(self.word(p+12)):visit(self.word(p+16+4*i),path+[i])
            visit(self.app+0x6c,[])
            types={0x969c38:('RootRule',0x48d0b4),0x969bf8:('Path',0x48c130),
                   0x969bb8:('ForceBlock',0x48cf10),0x969b78:('EndPath',0x48c018)}
            parent=self.data+0x1b00000;instance=parent+0x1000
            from unicorn import UC_HOOK_CODE
            runs=[]
            for seed in seeds:
                self.pointer(self.app,seed);calls=[];rows=[]
                def observe(uc,address,size,unused):
                    if address==0x483a94:calls.append(self.word(self.app))
                hook=self.uc.hook_add(UC_HOOK_CODE,observe)
                try:
                    for rule,path in pointers:
                        kind,ctor=types[self.word(rule)]
                        for has_parent in ((False,) if kind=='RootRule' else (False,True)):
                            self.uc.mem_write(parent,bytes(256));self.uc.mem_write(instance,bytes(256))
                            before=self.word(self.app);call_before=len(calls)
                            self.invoke(ctor,[instance,rule,parent if has_parent else 0],budget=3000000)
                            start,end=self.word(instance+0x30),self.word(instance+0x34)
                            assert end>=start and (end-start)%4==0
                            direction=self.word(instance+0x3c)
                            assert self.word(instance+4)==rule and self.word(instance+0x44)==rule
                            assert self.word(instance+8)==(parent if has_parent else 0)
                            assert self.word(parent+12)==int(has_parent)
                            if has_parent:assert self.word(parent+16)==instance
                            rows.append({'path':path,'type':kind,'parent':has_parent,
                                'length':struct.unpack('<i',self.uc.mem_read(instance+0x2c,4))[0],
                                'exit_direction':self.word(direction) if direction else None,
                                'block_names':[self.text(self.word(p)) for p in range(start,end,4)],
                                'children':self.word(instance+12),'tile_present':bool(self.word(instance+0x28)),
                                'progress':self.word(instance+0x40),'random_before':before,
                                'random_after':self.word(self.app),'random_calls':len(calls)-call_before})
                            if kind=='Path':rows[-1]['path_direction']=self.word(instance+0x48)
                finally:self.uc.hook_del(hook)
                runs.append({'seed':seed,'instances':rows,'final_state':self.word(self.app),'random_calls':len(calls)})
            return {'xml_error':tree['xml_error'],'reader_result':tree['read_result'],
                    'rule_nodes':len(pointers),'runs':runs}
        def caller(self,load_result,generate_result,save_result,seed,old):
            from unicorn import UC_HOOK_CODE
            level=self.data+0x1c00000;stream=level+0x1000;old_pointer=level+0x2000
            self.uc.mem_write(level,bytes(512));self.uc.mem_write(stream,bytes(64))
            self.pointer(level+0x10c,level+0x3000);self.uc.mem_write(level+0x3000,b'fixture.rule.xml\0')
            self.pointer(level+0x14c,old_pointer if old else 0)
            events=[];new_pointer=0
            def boundary(uc,address,size,unused):
                nonlocal new_pointer
                if address==0x487e84:
                    new_pointer=self.reg(0);events.append('construct');self.returned(new_pointer)
                elif address==0x489508:
                    assert self.reg(0)==new_pointer and self.text(self.reg(1))=='fixture.rule.xml'
                    events.append('load');self.returned(load_result)
                elif address==0x488134:
                    assert self.reg(0)==new_pointer and self.reg(1)==seed
                    events.append('generate');self.returned(generate_result)
                elif address==0x488b84:
                    assert self.reg(0)==new_pointer and self.reg(1)==stream
                    events.append('serialize');self.returned(save_result)
                elif address==0x488504:
                    events.append('destroy_old' if self.reg(0)==old_pointer else 'destroy_new')
                    assert self.reg(0) in (old_pointer,new_pointer);self.returned()
            hook=self.uc.hook_add(UC_HOOK_CODE,boundary)
            try:result=self.invoke(0x3f07f8,[level,stream,seed],budget=3000000)
            finally:self.uc.hook_del(hook)
            expected=(['destroy_old'] if old else [])+['construct','load','generate','serialize','destroy_new']
            assert events==expected and result==generate_result and self.word(level+0x14c)==0
            return {'load_result':load_result,'generate_result':generate_result,'serialize_result':save_result,
                    'seed':seed,'old_generator':old,'events':events,'return':result,'generator_cleared':True}
    return Instances()

def main():
    ap=argparse.ArgumentParser()
    for name in ('engine','dependency-root','cache','rules','out'):ap.add_argument('--'+name,type=pathlib.Path,required=True)
    ap.add_argument('--smoke',action='store_true');a=ap.parse_args()
    previous=json.loads(a.rules.read_text());assert previous['original_execution_complete']
    root=pathlib.Path(__file__).resolve().parents[1];repo=root.parents[1]
    for name,want in previous['dependency_sources_sha256'].items():assert hashlib.sha256((repo/name).read_bytes()).hexdigest()==want,name
    assert hashlib.sha256(a.cache.read_bytes()).hexdigest()==previous['cache_sha256']
    assert hashlib.sha256((root/'tests/procedural_rules_original.py').read_bytes()).hexdigest()==previous['script_sha256']
    cpu=create_original_instances_cpu(a.engine,a.dependency_root)
    callers=[cpu.caller(load,gen,save,seed,old) for load in (0,1) for gen in (0,1)
             for save in (0,1) for seed in (0,0xffffffff) for old in (False,True)]
    cases=[];seeds=[0,1,0x7fffffff,0x80000000,0xffffffff]
    selected=[r for r in previous['cases'] if 'rule' in r]
    boundary=(b'<rules><RootRule name="room" exit="west">'
              b'<Path name="room" exit="north" length="-2147483648,2147483647"/>'
              b'<Path name="room" exit="east" length="5,2"/>'
              b'<Path name="room" exit="south" length="3"/>'
              b'<Path name="room" exit="west" length="-1,1"/>'
              b'<ForceBlock name="room" exit="North"/>'
              b'<EndPath name="room" exit="none"/>'
              b'<EndPath name="room" exit="north"/>'
              b'<EndPath name="room" exit="east"/>'
              b'<EndPath name="room" exit="south"/>'
              b'<EndPath name="room" exit="west"/>'
              b'</RootRule></rules>')
    selected.append({'name':'runtime-directions-and-integer-boundaries','input_hex':boundary.hex(),
                     'input_sha256':hashlib.sha256(boundary).hexdigest(),'block_names':['room']})
    if a.smoke:selected=selected[:3]
    prefix='com.gameloft.android.GAND.GloftD2SS/files/'
    with zipfile.ZipFile(a.cache) as pack:
        for row in selected:
            raw=bytes.fromhex(row['input_hex']) if 'input_hex' in row else pack.read(prefix+row['name'])
            assert hashlib.sha256(raw).hexdigest()==row['input_sha256']
            case={k:row[k] for k in ('name','input_sha256','block_names')}
            if 'input_hex' in row:case['input_hex']=row['input_hex']
            try:case.update(cpu.instances(raw,row['block_names'],seeds))
            except Exception as e:case.update(failure=str(e),pc=hex(cpu.uc.reg_read(cpu.pc)))
            cases.append(case)
            if 'failure' in case:break
    helpers={}
    for module in tuple(sys.modules.values()):
        file=getattr(module,'__file__',None)
        if file:
            path=pathlib.Path(file).resolve()
            if path.suffix=='.py' and path.is_relative_to(repo/'port'):helpers[str(path.relative_to(repo))]=hashlib.sha256(path.read_bytes()).hexdigest()
    complete=len(cases)==len(selected) and not any('failure' in row for row in cases)
    receipt={'scope':__doc__,'engine_sha256':previous['engine_sha256'],'cache_sha256':previous['cache_sha256'],
             'rules_receipt_sha256':hashlib.sha256(a.rules.read_bytes()).hexdigest(),
             'script_sha256':hashlib.sha256(pathlib.Path(__file__).read_bytes()).hexdigest(),
             'dependency_sources_sha256':helpers,'caller_cases':callers,'cases':cases,
             'original_execution_complete':complete and not a.smoke,'smoke_execution_complete':complete and a.smoke,
             'layout_generation_verified':False,'pool_size_allocation_verified':False,'full_loader_verified':False}
    a.out.write_text(json.dumps(receipt,indent=2)+'\n')
    print(json.dumps({'complete':receipt['original_execution_complete'],'smoke_complete':receipt['smoke_execution_complete'],
                     'caller_cases':len(callers),'cases':len(cases),'failures':[row for row in cases if 'failure' in row]}))
    return 0 if complete else 1
if __name__=='__main__':raise SystemExit(main())
