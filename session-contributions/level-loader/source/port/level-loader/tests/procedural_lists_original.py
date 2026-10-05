"""Execute original room-list readers and actual block/list map operations.

Known block keys are caller inputs from verified file-list/block-map receipts.
Original ValidBlock membership, list allocation, lowercase conversion, element
parsing, copying and unique map insertion execute ARM32. No membership or list
result is supplied by a hook. This does not execute layout selection/generation.
"""
import argparse,hashlib,json,pathlib,struct,sys,zipfile
from procedural_blocks_original import create_original_blocks_cpu

def create_original_lists_cpu(engine,dependency_root):
    base=type(create_original_blocks_cpu(engine,dependency_root))
    class Lists(base):
        def __init__(self):
            super().__init__();self.app=self.data+0x1a00000
            self.blocks_storage=self.data+0x1800000
        def external(self,uc,address,size,unused):
            if self.imports.get(address)=='strcpy' and uc.reg_read(self.lr)==0x484604:
                p,q=self.reg(0),self.reg(1);assert q,'Null ValidBlock strcpy input'
                raw=self.text(q).encode()+b'\0';assert len(raw)<=512,'Original ValidBlock fixed buffer limit'
                uc.mem_write(p,raw);self.returned(p)
            else:return super().external(uc,address,size,unused)
        def setup_keys(self,names):
            self.uc.mem_write(self.app,bytes(400))
            for offset in (0x3c,0x54):
                p=self.app+offset;self.uc.mem_write(p,struct.pack('<6I',0,0,p,p,0,0))
            # ValidBlock only observes map membership; values are caller-owned
            # non-null identity storage, not fabricated block interpretation.
            key=self.app+0x1000;pair=self.app+0x200;out=pair+32
            for i,name in enumerate(names):
                raw=name.encode();assert b'\0' not in raw and len(raw)<512
                self.uc.mem_write(key,raw+b'\0')
                self.uc.mem_write(pair,struct.pack('<2I',key,self.blocks_storage+i*4))
                self.invoke(0x486154,[out,self.app+0x3c,pair],budget=3000000)
                key+=len(raw)+1
            assert self.heap<self.blocks_storage
        def execute_lists(self,raw,names):
            self.setup_keys(names);parsed=self.parse(raw)
            root=self.word(self.data+0x10000+0x18)
            while root and (self.word(root+0x14)!=1 or self.text(self.word(root+0x34))!='rules'):
                root=self.word(root+0x3c)
            assert root,'Missing original rules root'
            from unicorn import UC_HOOK_CODE
            declarations=[];availability=[]
            def observe(uc,address,size,unused):
                if address==0x48ed2c:declarations.append(self.reg(0));availability.append([])
                elif address==0x48ee54:availability[-1].append(bool(self.reg(0)))
            hook=self.uc.hook_add(UC_HOOK_CODE,observe,begin=0x48ed2c,end=0x48ee54)
            try:result=self.invoke(0x4865f0,[self.app,root],budget=30000000)
            finally:self.uc.hook_del(hook)
            selected=[]
            def walk(at):
                if not at:return
                walk(self.word(at+8));selected.append(self.word(at+20));walk(self.word(at+12))
            walk(self.word(self.app+0x58));assert len(selected)==self.word(self.app+0x64)
            rows=[]
            for di,at in enumerate(declarations):
                start,end=self.word(at+28),self.word(at+32);assert end>=start and (end-start)%80==0
                elems=[]
                for e in range(start,end,80):
                    elems.append({'block_name':self.text(self.word(e+24)),
                        'gameplay':self.text(self.word(e+48)),'visual':self.text(self.word(e+72)),
                        'chances':struct.unpack('<i',self.uc.mem_read(e+76,4))[0],
                        'block_available':availability[di][len(elems)]})
                rows.append({'name':self.text(self.word(at+20)),
                    'replacement':bool(self.uc.mem_read(at+24,1)[0]),'elements':elems})
            assert self.heap<self.blocks_storage
            selected_sources=[declarations.index(p) for p in selected]
            return {'xml_error':parsed['xml_error'],'load_result':result,'declarations':rows,
                'selected_sources':selected_sources,'lists':[rows[i] for i in selected_sources]}
    return Lists()

def main():
    ap=argparse.ArgumentParser()
    for n in ('engine','dependency-root','cache','connections','out'):ap.add_argument('--'+n,type=pathlib.Path,required=True)
    ap.add_argument('--smoke',action='store_true');a=ap.parse_args()
    root=pathlib.Path(__file__).resolve().parents[1];repo=root.parents[1]
    conn=json.loads(a.connections.read_text());assert conn['original_execution_complete']
    assert hashlib.sha256(a.cache.read_bytes()).hexdigest()==conn['cache_sha256']
    cpu=create_original_lists_cpu(a.engine,a.dependency_root)
    synthetic=[('defaults',b'<rules><list name="MiXeD"><elem name="room"/></list></rules>', ['room']),
        ('replacement-and-random',b'<rules><list name="z" replacement="false" random="true"><elem name="room" chances="-2tail" gameplay="Mixed\\X" visual="Y"/></list><list name="A" replacement="TRUE"><elem name="room" chances="0"/></list><list name="b" random="false"><elem name="room" chances="bad"/></list></rules>',['room']),
        ('duplicate-and-filtered-tags',b'<rules><list name="Same"><elem name="room"/><ignored/><elem name="room" chances="25"/></list><List name="upper-tag"/><list name="same"><elem name="room" chances="1"/></list><list><elem name="room"/></list></rules>',['room']),
        ('empty-map',b'<rules/>',[]),
        ('missing-and-case-blocks',b'<rules><list name="keys"><elem name="room"/><elem name="Room"/><elem/><elem name="missing"/></list></rules>',['room']),
        ('integer-prefix-boundaries',b'<rules><list name="ints" replacement="true"><elem name="room" chances="2147483647"/><elem name="room" chances="-2147483648"/><elem name="room" chances=" +8junk"/><elem name="room" chances="2.5"/><elem name="room" chances="0x10"/></list></rules>',['room']),
        ('filtered-comments-and-empty-strings',b'<rules><!-- ignored --><list name=" A "><!-- ignored --><elem name="room" gameplay="" visual="" chances=""/><Elem name="missing"/></list><list name="" replacement=""><elem name="room"/></list></rules>',['room']),
        ('uppercase-map-key',b'<rules><list name="keys"><elem name="Room"/><elem name="room"/></list></rules>',['Room']),
        ('maximum-validation-buffer',b'<rules><list name="long"><elem name="'+b'a'*511+b'"/></list></rules>',['a'*511])]
    cases=[]
    for name,raw,names in synthetic:
        try:cases.append({'name':name,'input_hex':raw.hex(),'block_names':names,**cpu.execute_lists(raw,names)})
        except Exception as e:cases.append({'name':name,'failure':str(e),'pc':hex(cpu.uc.reg_read(cpu.pc))});break
    if not a.smoke and not any('failure' in r for r in cases):
        folders={r['name']:[b['name'] for b in r['blocks']] for r in conn['cases'] if r['name'].startswith('data/')}
        prefix='com.gameloft.android.GAND.GloftD2SS/files/'
        with zipfile.ZipFile(a.cache) as pack:
            for name in sorted(n for n in pack.namelist() if n.startswith(prefix) and n.endswith('.rule.xml')):
                raw=pack.read(name)
                parsed=cpu.parse(raw);tree=parsed['root']
                if not tree or tree['tag']!='rules':continue
                folder=tree['attributes'].get('folder','').lower().replace('\\','/')
                if folder not in folders:continue
                names=folders[folder]
                try:cases.append({'name':name[len(prefix):],'input_sha256':hashlib.sha256(raw).hexdigest(),
                    'block_names':names,**cpu.execute_lists(raw,names)})
                except Exception as e:cases.append({'name':name[len(prefix):],'failure':str(e),'pc':hex(cpu.uc.reg_read(cpu.pc))});break
    helpers={}
    for module in tuple(sys.modules.values()):
        file=getattr(module,'__file__',None)
        if file:
            p=pathlib.Path(file).resolve()
            if p.suffix=='.py' and p.is_relative_to(repo/'port'):helpers[str(p.relative_to(repo))]=hashlib.sha256(p.read_bytes()).hexdigest()
    report={'scope':__doc__,'engine_sha256':hashlib.sha256(a.engine.read_bytes()).hexdigest(),
        'cache_sha256':conn['cache_sha256'],'connections_receipt_sha256':hashlib.sha256(a.connections.read_bytes()).hexdigest(),
        'script_sha256':hashlib.sha256(pathlib.Path(__file__).read_bytes()).hexdigest(),'dependency_sources_sha256':helpers,
        'cases':cases,'original_execution_complete':not a.smoke and len(cases)==44 and not any('failure' in r for r in cases),
        'smoke_execution_complete':a.smoke and len(cases)==len(synthetic) and not any('failure' in r for r in cases),
        'layout_generation_verified':False,'full_loader_verified':False}
    a.out.write_text(json.dumps(report,indent=2)+'\n')
    print(json.dumps({'cases':len(cases),'complete':report['original_execution_complete'],
        'smoke_complete':report['smoke_execution_complete'],'failures':[r for r in cases if 'failure' in r]}))
    return 0 if report['original_execution_complete'] or report['smoke_execution_complete'] else 1
if __name__=='__main__':raise SystemExit(main())
