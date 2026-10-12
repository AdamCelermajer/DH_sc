"""Execute original Generate and room placement against original rule/MGX inputs.

Original readers, derived MgxBlock constructors, block-map insertion/linking,
Generate, rule traversal, selection, backtracking and Array2d operations execute
ARM32. File discovery is supplied from separately verified GetFiles receipts.
Block name/folder/target string assignment is caller input; FromFilename,
LoadRuleFile orchestration, pool ComputeSizeOfRules and serialization do not
execute. The fixture accepts only zero-pool declarations. Imported allocation,
byte and numeric services are explicit adapters. No selection, placement,
collision, shuffle or generation result is supplied by a hook.
"""
import argparse,hashlib,json,pathlib,re,struct,sys,time,zipfile
from procedural_instances_original import create_original_instances_cpu
from procedural_rules_original import CheckedDomain

_layout_classes={}

def create_original_layout_cpu(engine,dependency_root):
    key=(str(engine.resolve()),str(dependency_root.resolve()))
    if key in _layout_classes:return _layout_classes[key]()
    base=type(create_original_instances_cpu(engine,dependency_root))
    class Layout(base):
        def external(self,uc,address,size,unused):
            if self.imports.get(address)=='sprintf':
                # Bounded subset of the imported C formatter, not a room result.
                # Reject unimplemented conversions rather than approximating.
                from unicorn.arm_const import UC_ARM_REG_SP
                destination=self.reg(0);fmt=self.text(self.reg(1));args=0
                def replace(match):
                    nonlocal args
                    token=match[0]
                    if token=='%%':return '%'
                    assert re.fullmatch(r'%0?[0-9]*[sduxXc]',token),('unsupported sprintf conversion',fmt)
                    value=self.reg(2+args) if args<2 else self.word(uc.reg_read(UC_ARM_REG_SP)+4*(args-2))
                    args+=1
                    if token[-1]=='s':value=self.text(value)
                    elif token[-1]=='d' and value>=0x80000000:value-=0x100000000
                    if token[-1]=='u':token=token[:-1]+'d'
                    return token%value
                rendered=re.sub(r'%(?:%|[^%]*?[a-zA-Z])',replace,fmt)
                assert '%' not in re.sub(r'%%|%0?[0-9]*[sduxXc]','',fmt),('unsupported sprintf format',fmt)
                encoded=rendered.encode();assert len(encoded)<256,('formatter buffer domain',fmt)
                uc.mem_write(destination,encoded+b'\0');self.returned(len(encoded))
                self.adapters['sprintf:'+fmt]=self.adapters.get('sprintf:'+fmt,0)+1
            else:return super().external(uc,address,size,unused)
        def invoke(self,address,args,budget=3000000):
            # Blocks.run initializes a base Block. Layout needs the actual
            # derived virtual placement methods; internal ARM calls are intact.
            if address==0x48ab34:address=0x48ad14
            return super().invoke(address,args,budget=budget)
        def assign(self,address,value):
            raw=value.encode();assert b'\0' not in raw and len(raw)<4096
            scratch=self.app+0x8000;self.uc.mem_write(scratch,raw+b'\0')
            self.invoke(0x3109e0,[address,scratch,scratch+len(raw)])
        def prepare(self,raw,blocks,folder,target):
            tree=self.execute_rules(raw,[name for name,_,_ in blocks])
            if tree['pools']:raise CheckedDomain('pool allocation has not been executed by this fixture')
            self.invoke(0x487dd8,[self.app+8])
            map_address=self.app+0x3c
            self.uc.mem_write(map_address,struct.pack('<6I',0,0,map_address,map_address,0,0))
            pair=self.app+0x200;out=pair+32;key=self.app+0x1000
            assert len(blocks)<512
            pointers={};insertions=[]
            for i,(name,block_raw,uri) in enumerate(blocks):
                p=self.blocks_storage+i*4096;loaded=self.run(block_raw,p)
                assert loaded['load_result']==1 and self.word(p)==0x969b38
                for offset,value in ((4,name),(0x1c,folder),(0x34,target)):self.assign(p+offset,value)
                assert self.heap<self.blocks_storage
                encoded=name.encode();assert b'\0' not in encoded and len(encoded)<512
                self.uc.mem_write(key,encoded+b'\0');self.uc.mem_write(pair,struct.pack('<2I',key,p))
                self.invoke(0x486154,[out,map_address,pair])
                accepted=bool(self.uc.mem_read(out+4,1)[0])
                insertions.append({'source_index':i,'name':name,'inserted':accepted})
                if accepted:pointers[p]=(i,name,uri)
                key+=len(encoded)+1
                assert key<self.app+0x8000
            ordered=[]
            def walk(p):
                if p:
                    walk(self.word(p+8));ordered.append(self.word(p+20));walk(self.word(p+12))
            walk(self.word(map_address+4));assert len(ordered)==self.word(map_address+16)
            from unicorn import UC_HOOK_CODE
            def guard(uc,address,size,unused):
                if address==0x489cb0 and self.word(self.reg(9)+0x88)>=64:
                    raise CheckedDomain('original connection capacity exceeded')
            hook=self.uc.hook_add(UC_HOOK_CODE,guard,begin=0x489cb0,end=0x489cb0)
            try:
                for p in ordered:self.invoke(0x489aec,[p,map_address],budget=30000000)
            finally:self.uc.hook_del(hook)
            return tree,insertions,pointers
        def generate(self,seed,pointers,budget):
            from unicorn import UC_HOOK_CODE
            events={'random_calls':0,'root_attempts':0,'place_calls':0,'unspawn_calls':0,
                    'step_calls':0,'path_calls':0,'rule_calls':0}
            event_addresses={0x483a94:'random_calls',0x48c374:'root_attempts',0x4913d8:'place_calls',
                             0x4918a8:'unspawn_calls',0x48f954:'step_calls',
                             0x48fd64:'path_calls',0x490304:'rule_calls'}
            distribution_base=0x8ce5c8;distribution_end=distribution_base+26136
            def observe(uc,address,size,unused):
                key=event_addresses.get(address)
                if key:events[key]+=1
                # Runtime child array occupies six pointers before tile+0x28.
                if address==0x48bf14 and self.word(self.reg(2)+12)>=6:
                    raise CheckedDomain('original runtime rule child capacity exceeded')
                if address==0x491428 and self.word(self.reg(0)+12)>=8:
                    raise CheckedDomain('original tile child capacity exceeded')
                # The table is authoritative engine data, not synthesized.
                if address==0x48fa88:
                    children=self.reg(5);exits=self.reg(6)
                    if children and exits:
                        start=distribution_base+726*children+4356*exits
                        if children>=6 or exits>=6 or not distribution_base<=start<distribution_end:
                            raise CheckedDomain('original distribution table indices exceed captured domain')
            hook=self.uc.hook_add(UC_HOOK_CODE,observe,begin=0x483a94,end=0x491aaf)
            try:success=self.invoke(0x488134,[self.app,seed],budget=budget)
            finally:self.uc.hook_del(hook)
            tiles=[];seen=set()
            def visit(p,parent):
                assert p not in seen and len(seen)<4096;seen.add(p)
                index=len(tiles);block=self.word(p+0x30)
                assert block in pointers and self.word(p+8)==parent
                source,name,uri=pointers[block]
                count=self.word(p+12);assert count<=8
                row={'index':index,'name':self.text(self.word(p)),'block_source':source,'block_name':name,
                    'mgx_uri':uri,'grid':list(struct.unpack('<2i',self.uc.mem_read(p+0x84,8))),
                    'height_bits':self.word(p+0x8c),
                    'list_element':{'name':self.text(self.word(p+0x34+24)),
                        'gameplay':self.text(self.word(p+0x34+48)),
                        'visual':self.text(self.word(p+0x34+72)),
                        'chances':struct.unpack('<i',self.uc.mem_read(p+0x34+76,4))[0]},'children':[]}
                assert row['name']==name
                tiles.append(row)
                for i in range(count):row['children'].append(visit(self.word(p+16+4*i),p))
                return index
            root=self.word(self.app+0x114)
            assert success==int(bool(root))
            if root:visit(root,0)
            return {'seed':seed,'success':bool(success),'final_state':self.word(self.app),
                    'tiles':tiles,'events':events,'adapters':dict(self.adapters)}
    _layout_classes[key]=Layout
    return Layout()

def module(exits):
    return ('<Module>'+''.join(f'<GameObject gametype="link" linktype="gate" direction="{d}" position="0,0,0"/>'
                             for d in exits)+'</Module>').encode()

def main():
    ap=argparse.ArgumentParser()
    for name in ('engine','dependency-root','cache','file-lists','rules','out'):
        ap.add_argument('--'+name,type=pathlib.Path,required=True)
    ap.add_argument('--smoke',action='store_true');ap.add_argument('--match',default='')
    ap.add_argument('--seeds',default='0,1');ap.add_argument('--budget',type=int,default=100000000)
    a=ap.parse_args();started=time.monotonic()
    files=json.loads(a.file_lists.read_text());rules=json.loads(a.rules.read_text())
    assert files['original_execution_complete'] and rules['original_execution_complete']
    assert hashlib.sha256(a.cache.read_bytes()).hexdigest()==files['cache_sha256']==rules['cache_sha256']
    seeds=[int(x,0) for x in a.seeds.split(',')]
    inputs=[('single-root',b'<rules><RootRule name="room"/></rules>',
             [('room',module([]),'fixture/room.mgx')],'fixture','fixture'),
        ('three-room-force-chain',b'<rules><RootRule name="a"><ForceBlock name="b"><ForceBlock name="c"/>'
             b'</ForceBlock></RootRule></rules>',
             [('a',module(['north']),'fixture/a.mgx'),('b',module(['north','south']),'fixture/b.mgx'),
              ('c',module(['south']),'fixture/c.mgx')],'fixture','fixture'),
        ('same-block-path-rejected',b'<rules><RootRule name="room"><Path name="room" length="2"/></RootRule></rules>',
             [('room',module(['north','south']),'fixture/room.mgx')],'fixture','fixture'),
        ('list-root-selection',b'<rules><list name="roots"><elem name="a" gameplay="ag" visual="av" chances="0"/>'
             b'<elem name="b" gameplay="bg" visual="bv" chances="-1"/></list><RootRule name="#roots"/></rules>',
             [('a',module([]),'fixture/a.mgx'),('b',module([]),'fixture/b.mgx')],'fixture','fixture')]
    prefix='com.gameloft.android.GAND.GloftD2SS/files/'
    cpu=create_original_layout_cpu(a.engine,a.dependency_root)
    file_rows={row['name']:row for row in files['cases'] if row['name'].startswith('data/')}
    if not a.smoke:
        with zipfile.ZipFile(a.cache) as pack:
            for row in rules['cases']:
                if not row['name'].startswith('data/') or (a.match and a.match not in row['name']):continue
                raw=pack.read(prefix+row['name']);assert hashlib.sha256(raw).hexdigest()==row['input_sha256']
                parsed=cpu.parse(raw);attrs=parsed['root']['attributes'];folder=attrs.get('folder','');target=attrs.get('target','')
                file_row=file_rows[folder.lower().replace('\\','/')+'/mgx/mgxlist.txt']
                blocks=[]
                for filename in file_row['rows']:
                    at=filename.rfind('.mgx')
                    if at<0:continue
                    name=filename[:at]+filename[at+4:];uri=(folder+'/mgx/'+filename).lower().replace('\\','/')
                    blocks.append((name,pack.read(prefix+uri),uri))
                inputs.append((row['name'],raw,blocks,folder,target))
    cases=[]
    for name,raw,blocks,folder,target in inputs:
        row={'name':name,'input_sha256':hashlib.sha256(raw).hexdigest(),'folder':folder,'target':target,
             'blocks':[{'name':n,'uri':uri,'input_sha256':hashlib.sha256(b).hexdigest(),
                        **({'input_hex':b.hex()} if not name.startswith('data/') else {})} for n,b,uri in blocks],
             'runs':[]}
        if not name.startswith('data/'):row['input_hex']=raw.hex()
        try:
            # Fresh ARM execution for each seed avoids fixture allocator growth
            # and does not assume generated tile teardown between runs.
            for seed in seeds:
                cpu=create_original_layout_cpu(a.engine,a.dependency_root)
                tree,insertions,pointers=cpu.prepare(raw,blocks,folder,target)
                row['reader_result']=tree['read_result'];row['xml_error']=tree['xml_error'];row['insertions']=insertions
                row['runs'].append(cpu.generate(seed,pointers,a.budget))
        except CheckedDomain as e:row.update(checked_domain_rejection=str(e),pc=hex(cpu.uc.reg_read(cpu.pc)))
        except Exception as e:row.update(failure=str(e),pc=hex(cpu.uc.reg_read(cpu.pc)))
        cases.append(row)
        print(json.dumps({'case':name,'runs':len(row['runs']),
            'tiles':[len(r['tiles']) for r in row['runs']],
            'failure':row.get('failure'),'checked':row.get('checked_domain_rejection')}),flush=True)
        if 'failure' in row:break
    root=pathlib.Path(__file__).resolve().parents[3];helpers={}
    for m in tuple(sys.modules.values()):
        path=getattr(m,'__file__',None)
        if path:
            path=pathlib.Path(path).resolve()
            if path.suffix=='.py' and path.is_relative_to(root/'port'):
                helpers[str(path.relative_to(root))]=hashlib.sha256(path.read_bytes()).hexdigest()
    complete=len(cases)==len(inputs) and not any('failure' in r or 'checked_domain_rejection' in r for r in cases)
    report={'scope':__doc__,'engine_sha256':hashlib.sha256(a.engine.read_bytes()).hexdigest(),
        'cache_sha256':files['cache_sha256'],'script_sha256':hashlib.sha256(pathlib.Path(__file__).read_bytes()).hexdigest(),
        'file_lists_receipt_sha256':hashlib.sha256(a.file_lists.read_bytes()).hexdigest(),
        'rules_receipt_sha256':hashlib.sha256(a.rules.read_bytes()).hexdigest(),
        'dependency_sources_sha256':helpers,'cases':cases,'requested_cases':len(inputs),
        'original_execution_complete':complete and not a.smoke and not a.match,
        'selected_execution_complete':complete,'native_layout_generation_verified':False,
        'full_loader_verified':False,'elapsed_seconds':round(time.monotonic()-started,3)}
    a.out.write_text(json.dumps(report,indent=2)+'\n')
    return 0 if complete else 1
if __name__=='__main__':raise SystemExit(main())
