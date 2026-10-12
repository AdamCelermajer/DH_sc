"""Execute original nested-rule and room-pool declaration interpretation.

Actual RootRule/rule constructors, virtual readers, NewRule, list/block map
lookup, Hash, LoadRoomPools/RPElem and FillSizes execute ARM32. Known block
keys are caller inputs from original block-map receipts; list declarations
execute the previously verified original readers. Imported atoi/search and
allocation are explicit services. RoomPool::ComputeSizeOfRules, random
selection, layout generation and full LoadRuleFile orchestration do not run.
"""
import argparse,hashlib,json,pathlib,re,struct,sys,zipfile
from procedural_lists_original import create_original_lists_cpu

class CheckedDomain(Exception):pass

def create_original_rules_cpu(engine,dependency_root):
    base=type(create_original_lists_cpu(engine,dependency_root))
    class Rules(base):
        def external(self,uc,address,size,unused):
            name=self.imports.get(address)
            if name=='atoi':
                if not self.reg(0):raise CheckedDomain('original atoi receives null input')
                match=re.match(r'[\t\n\v\f\r ]*([+-]?[0-9]+)',self.text(self.reg(0)))
                value=int(match[1]) if match else 0
                if not -2147483648<=value<=2147483647:raise CheckedDomain('atoi outside int32 domain')
                self.returned(value)
            elif name in ('strchr','strrchr'):
                start=self.reg(0);assert start,'Null original character-search input'
                raw=self.text(start).encode()+b'\0';c=bytes([self.reg(1)&255])
                index=raw.find(c) if name=='strchr' else raw.rfind(c)
                self.returned(start+index if index>=0 else 0)
            else:return super().external(uc,address,size,unused)
        def execute_rules(self,raw,names):
            lists=self.execute_lists(raw,names)
            root=self.word(self.data+0x10000+0x18)
            while root and (self.word(root+0x14)!=1 or self.text(self.word(root+0x34))!='rules'):root=self.word(root+0x3c)
            assert root
            self.invoke(0x48dff8,[self.app+0x6c,self.app])
            self.invoke(0x485cdc,[self.app,root],budget=30000000)
            child=self.word(root+0x18);rule_root=0
            while child:
                if self.word(child+0x14)==1 and self.text(self.word(child+0x34))=='RootRule':rule_root=child;break
                child=self.word(child+0x3c)
            from unicorn import UC_HOOK_CODE
            returns=[];active=[];comma_queries=[];list_validation={};block_validation={}
            def observe(uc,address,size,unused):
                if address==0x490bdc:active.append(self.reg(0))
                elif address==0x490f48:returns.append((active.pop(),self.reg(0)))
                elif address in (0x491050,0x4911d0):list_validation[active[-1]]=bool(self.reg(0))
                elif address==0x490d58:block_validation.setdefault(active[-1],[]).append(bool(self.reg(0)))
                elif address==0x48de44 and self.word(self.reg(1)+0x14)!=1:
                    raise CheckedDomain('original unfiltered rule child is non-element')
                elif address==0x490e8c:
                    if self.reg(6)>=16:raise CheckedDomain('original sixteen-child capacity exceeded')
                elif address==0x490ea0 and not self.reg(0):raise CheckedDomain('original unknown/non-element child would dereference null')
                elif address==0x490d38:
                    p=self.reg(0);comma_queries.append(p)
                    if len(comma_queries)>=2 and comma_queries[-2]==p:
                        raise CheckedDomain('original explicit-block comma loop does not advance')
            hook=self.uc.hook_add(UC_HOOK_CODE,observe)
            try:result=self.invoke(0x4913cc,[self.app+0x6c,rule_root],budget=30000000)
            finally:self.uc.hook_del(hook)
            results=dict(returns);rule_types={0x969c38:'RootRule',0x969b78:'EndPath',0x969bb8:'ForceBlock',0x969bf8:'Path'}
            # Resolve list pointers to selected source-declaration indices.
            list_ptrs=[]
            def walk(p):
                if not p:return
                walk(self.word(p+8));list_ptrs.append(self.word(p+20));walk(self.word(p+12))
            walk(self.word(self.app+0x58))
            list_ids={p:lists['selected_sources'][i] for i,p in enumerate(list_ptrs)}
            def rule(p):
                kind=rule_types[self.word(p)];start,end=self.word(p+0x70),self.word(p+0x74)
                assert end>=start and (end-start)%24==0
                count=self.word(p+12);assert count<=16
                row={'type':kind,'read_result':results[p],'exit':self.text(self.word(p+0x64)),
                    'id_hash':self.word(p+0x7c),'length':list(struct.unpack('<2i',self.uc.mem_read(p+0x80,8))),
                    'list_source':list_ids.get(self.word(p+0x68),None),
                    'list_index':struct.unpack('<i',self.uc.mem_read(p+0x6c,4))[0],
                    'list_validation':list_validation.get(p,None),'block_validation':block_validation.get(p,[]),
                    'block_names':[self.text(self.word(s+20)) for s in range(start,end,24)],
                    'children':[rule(self.word(p+16+4*i)) for i in range(count)]}
                if kind=='Path':row['dont_go_back']=bool(self.uc.mem_read(p+0x8c,1)[0])
                if kind=='ForceBlock':row['connect_from']=self.word(p+0x88)
                return row
            pools=[];start,end=self.word(self.app+0x118),self.word(self.app+0x11c)
            assert end>=start and (end-start)%4==0
            for ptr in range(start,end,4):
                p=self.word(ptr);begin,finish=self.word(p+4),self.word(p+8)
                assert finish>=begin and (finish-begin)%24==0
                pools.append({'size':struct.unpack('<i',self.uc.mem_read(p,4))[0],
                    'elements':[{'id_hash':self.word(e+4),'recursive':bool(self.uc.mem_read(e+8,1)[0]),
                        'sizes':list(struct.unpack('<3i',self.uc.mem_read(e+12,12)))} for e in range(begin,finish,24)]})
            assert self.heap<self.blocks_storage
            return {'xml_error':lists['xml_error'],'root_present':bool(rule_root),'read_result':result,
                'rule':rule(self.app+0x6c),'pools':pools}
    return Rules()

def main():
    ap=argparse.ArgumentParser()
    for n in ('engine','dependency-root','cache','lists','out'):ap.add_argument('--'+n,type=pathlib.Path,required=True)
    ap.add_argument('--smoke',action='store_true');a=ap.parse_args();root=pathlib.Path(__file__).resolve().parents[1];repo=root.parents[1]
    receipt=json.loads(a.lists.read_text());assert receipt['original_execution_complete']
    assert hashlib.sha256(a.cache.read_bytes()).hexdigest()==receipt['cache_sha256']
    cpu=create_original_rules_cpu(a.engine,a.dependency_root)
    tokens={'room_pool':cpu.text(0x485cec+8+0x44eef4),'pool_element':cpu.text(0x48cb04+8+0x44830c)}
    print(json.dumps({'original_static_tokens':tokens}),flush=True)
    synthetic=[('explicit-default',b'<rules><RootRule name="room"/></rules>',['room']),
        ('list-index-and-path',b'<rules><list name="Road"><elem name="room"/></list><RootRule name="#road[0]" id="root"><Path name="#road" length="2,4" dontGoBack="0"><ForceBlock name="room" connectFrom="east" exit="south"><EndPath name="room" exit="north"/></ForceBlock></Path></RootRule></rules>',['room']),
        ('missing-endpath-name',b'<rules><RootRule name="room" id="root"><EndPath exit="north"/></RootRule></rules>',['room']),
        ('missing-root-name',b'<rules><RootRule id="root"><ForceBlock name="room"/></RootRule></rules>',['room']),
        ('missing-root',b'<rules/>',[]),
        ('pool-duplicate-and-failed-parent',b'<rules><pool size="10"><elem id="p" recursive="true"/><elem id="p"/><elem/><Elem id="p"/></pool><RootRule name="room" id="root"><Path name="room" id="p" length="3,1" dontGoBack="true"/><ForceBlock name="room" id="p" connectFrom="west"/><EndPath exit="south"/></RootRule></rules>',['room']),
        ('pool-negative-length',b'<rules><pool size="bad"><elem id="p" recursive="TRUE"/><elem id="other" recursive="false"/></pool><RootRule name="room"><Path name="room" id="p" length="-2,5"/></RootRule></rules>',['room']),
        ('postorder-root-pool-fill',b'<rules><pool size="5"><elem id="p"/><elem id="p"/></pool><pool size="1"><elem id="p" recursive="true"/></pool><RootRule name="room" id="p"><Path name="room" id="p" length="2,4"/></RootRule></rules>',['room']),
        ('bracket-variants',b'<rules><list name="road"><elem name="room"/></list><RootRule name="#road[12]"><path name="#road[-2]"/><Path name="#road[]"/><Path name="#road[7"/><ForceBlock name="#road[ +3tail]"/></RootRule></rules>',['room']),
        ('uppercase-list-validation-versus-resolution',b'<rules><list name="Road"><elem name="room"/></list><RootRule name="#Road[0]"><ForceBlock name="#missing"/></RootRule></rules>',['room']),
        ('missing-path-name-retains-extras',b'<rules><RootRule name="room"><Path length="4,7" dontGoBack="0" id="ignored"><ForceBlock name="room"/></Path></RootRule></rules>',['room']),
        ('numeric-fallbacks',b'<rules><RootRule name="missing"><Path name="room" length="junk, +4tail" dontGoBack="-3junk"/><Path name="room" length="8tail" dontGoBack="false"/></RootRule></rules>',['room']),
        ('empty-name-first-root',b'<rules><RootRule name=""/><RootRule name="ignored"/></rules>',['']),
        ('sixteen-children',b'<rules><RootRule name="room">'+b'<EndPath name="room"/>'*16+b'</RootRule></rules>',['room']),
        ('unsafe-comma-loop',b'<rules><RootRule name="room,room"/></rules>',['room']),
        ('unsafe-unknown-child',b'<rules><RootRule name="room"><Unexpected/></RootRule></rules>',['room']),
        ('unsafe-seventeen-children',b'<rules><RootRule name="room">'+b'<EndPath name="room"/>'*17+b'</RootRule></rules>',['room']),
        ('unsafe-non-element-child',b'<rules><RootRule name="room"><!-- comment --></RootRule></rules>',['room']),
        ('unsafe-missing-pool-size',b'<rules><pool/><RootRule name="room"/></rules>',['room'])]
    inputs=[(n,r,keys) for n,r,keys in synthetic]
    if not a.smoke:
        prefix='com.gameloft.android.GAND.GloftD2SS/files/'
        with zipfile.ZipFile(a.cache) as pack:
            inputs.extend((r['name'],pack.read(prefix+r['name']),r['block_names']) for r in receipt['cases'] if r['name'].startswith('data/'))
    cases=[]
    for name,raw,names in inputs:
        row={'name':name,'input_sha256':hashlib.sha256(raw).hexdigest(),'block_names':names}
        if not name.startswith('data/'):row['input_hex']=raw.hex()
        try:row.update(cpu.execute_rules(raw,names))
        except CheckedDomain as e:row.update(checked_domain_rejection=str(e),pc=hex(cpu.uc.reg_read(cpu.pc)))
        except Exception as e:row.update(failure=str(e),pc=hex(cpu.uc.reg_read(cpu.pc)))
        cases.append(row)
        if 'failure' in row:break
    helpers={}
    for module in tuple(sys.modules.values()):
        file=getattr(module,'__file__',None)
        if file:
            p=pathlib.Path(file).resolve()
            if p.suffix=='.py' and p.is_relative_to(repo/'port'):helpers[str(p.relative_to(repo))]=hashlib.sha256(p.read_bytes()).hexdigest()
    report={'scope':__doc__,'engine_sha256':hashlib.sha256(a.engine.read_bytes()).hexdigest(),'cache_sha256':receipt['cache_sha256'],
        'lists_receipt_sha256':hashlib.sha256(a.lists.read_bytes()).hexdigest(),'script_sha256':hashlib.sha256(pathlib.Path(__file__).read_bytes()).hexdigest(),
        'dependency_sources_sha256':helpers,'original_static_tokens':tokens,'cases':cases,
        'synthetic_cases_requested':len(synthetic),'authored_rule_files_requested':len(inputs)-len(synthetic),
        'original_execution_complete':not a.smoke and len(cases)==len(inputs) and not any('failure' in r for r in cases),
        'smoke_execution_complete':a.smoke and len(cases)==len(synthetic) and not any('failure' in r for r in cases),
        'pool_size_allocation_verified':False,'layout_generation_verified':False,'full_loader_verified':False}
    a.out.write_text(json.dumps(report,indent=2)+'\n')
    print(json.dumps({'cases':len(cases),'complete':report['original_execution_complete'],
        'smoke_complete':report['smoke_execution_complete'],'checked_rejections':[{k:r[k] for k in ('name','checked_domain_rejection','pc')} for r in cases if 'checked_domain_rejection' in r],
        'failures':[r for r in cases if 'failure' in r]}))
    return 0 if report['original_execution_complete'] or report['smoke_execution_complete'] else 1
if __name__=='__main__':raise SystemExit(main())
