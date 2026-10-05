"""Execute original block-map insertion and room-exit connection construction.

Actual MGX block interpretation, lstr comparison, insert_unique, red-black
balancing/traversal and Block::LinkToOtherBlocks execute original ARM32.
File-list bytes are supplied from separately verified original GetFiles
receipts. Storage and imported C/float services are explicit adapters.
This does not execute full RandomGenerator::LoadBlocks or layout generation.
"""
import argparse,hashlib,json,pathlib,struct,sys,zipfile
from procedural_blocks_original import create_original_blocks_cpu
ap=argparse.ArgumentParser()
for name in ('engine','dependency-root','cache','original-lists','out'):ap.add_argument('--'+name,type=pathlib.Path,required=True)
ap.add_argument('--smoke',action='store_true');a=ap.parse_args()
root=pathlib.Path(__file__).resolve().parents[1];repo=root.parents[1]
lists=json.loads(a.original_lists.read_text());assert lists['original_execution_complete']
assert hashlib.sha256(a.cache.read_bytes()).hexdigest()==lists['cache_sha256']=='3fdf4e4c21d45a780a7c35fb4042abde0e88e76bf75416aad1f227481560b679'
probe=create_original_blocks_cpu(a.engine,a.dependency_root)
opposites=list(struct.unpack('<5I',probe.uc.mem_read(0x999880,20)))
def execute(blocks):
    # Separate caller buffers from both retained node pools and parser input.
    pool=probe.data+0x1800000;tree=probe.data+0x1a00000;pair=tree+0x100;out=pair+0x100;key=tree+0x1000
    assert len(blocks)<512
    probe.uc.mem_write(tree,struct.pack('<6I',0,0,tree,tree,0,0))
    pointers={};insertions=[]
    for i,(name,raw,uri) in enumerate(blocks):
        p=pool+i*4096;result=probe.run(raw,p);assert result['load_result']==1
        assert probe.heap<pool,'Caller buffers overlap original allocator storage'
        encoded=name.encode();assert b'\0' not in encoded and len(encoded)<512
        probe.uc.mem_write(key,encoded+b'\0');probe.uc.mem_write(pair,struct.pack('<2I',key,p))
        probe.invoke(0x486154,[out,tree,pair],budget=3000000)
        node=probe.word(out);accepted=bool(probe.uc.mem_read(out+4,1)[0])
        insertions.append({'source_index':i,'name':name,'inserted':accepted})
        if accepted:pointers[p]=(i,name,uri)
        key+=len(encoded)+1
    ordered=[]
    def walk(at):
        if not at:return
        walk(probe.word(at+8));ordered.append(probe.word(at+20));walk(probe.word(at+12))
    walk(probe.word(tree+4));assert len(ordered)==probe.word(tree+16)
    exit_ids={}
    for block_index,p in enumerate(ordered):
        for ei in range(probe.word(p+92)):exit_ids[p+96+ei*300]=[block_index,ei]
    # Guard the original fixed connection array before each append; overflow
    # is a checked-domain failure, not execution of memory corruption.
    from unicorn import UC_HOOK_CODE
    def guard(uc,address,size,unused):
        if address==0x489cb0:assert probe.word(probe.reg(9)+0x88)<64,'Original connection capacity exceeded'
    hook=probe.uc.hook_add(UC_HOOK_CODE,guard,begin=0x489cb0,end=0x489cb0)
    try:
        for p in ordered:probe.invoke(0x489aec,[p,tree],budget=30000000)
    finally:probe.uc.hook_del(hook)
    records=[]
    for p in ordered:
        source_index,name,uri=pointers[p];exits=[]
        for i in range(probe.word(p+92)):
            e=p+96+i*300;count=probe.word(e+40);assert count<=64
            connections=[exit_ids[probe.word(e+44+4*j)] for j in range(count)]
            exits.append({'index':i,'connections':connections})
        records.append({'source_index':source_index,'name':name,'uri':uri,'exits':exits})
    return {'insertions':insertions,'blocks':records}
def xml(exits):
    return ('<Module>'+''.join(f'<GameObject gametype="link" linktype="{t}" direction="{d}" position="0,0,0"/>' for d,t in exits)+'</Module>').encode()
synthetic=[('ordering-duplicates-self',[
    ('z',xml([('north','gate,gate'),('south','gate')]),''),
    ('A',xml([('south','gate,gate')]),''),('a',xml([('south','gate')]),''),
    ('z',xml([('west','ignored')]),''),('empty',xml([]),'')]),
    ('unknown-and-case',[(n,xml(exits),'') for n,exits in (
        ('north',[('north','a,A')]),('south',[('south','a,A')]),
        ('none',[('none','a')]),('unknown',[('unknown','a')]),('caps',[('SOUTH','A')]))]),
    ('stale-spaces-and-final-zero',[(n,xml(exits),'') for n,exits in (
        ('left',[('east','a, b')]),('right',[('west','bb')]),
        ('bad',[('west','a,0'),('west','a,')]),('other',[('west','b')]))]),
    ('empty-map',[])]
rows=[]
for name,blocks in synthetic:
    try:rows.append({'name':name,'inputs':[{'name':n,'input_hex':raw.hex(),'uri':uri} for n,raw,uri in blocks],**execute(blocks)})
    except Exception as e:rows.append({'name':name,'failure':str(e),'pc':hex(probe.uc.reg_read(probe.pc))});break
if not any('failure' in row for row in rows) and not a.smoke:
    with zipfile.ZipFile(a.cache) as pack:
        prefix='com.gameloft.android.GAND.GloftD2SS/files/'
        for source in lists['cases']:
            if not source['name'].startswith('data/'):continue
            folder=source['name'][:-len('/mgx/mgxlist.txt')];blocks=[]
            raw_list=pack.read(prefix+source['name']);assert hashlib.sha256(raw_list).hexdigest()==source['input_sha256']
            for filename in source['rows']:
                at=filename.rfind('.mgx')
                if at<0:continue
                name=filename[:at]+filename[at+4:];uri=(folder+'/mgx/'+filename).lower().replace('\\','/')
                blocks.append((name,pack.read(prefix+uri),uri))
            try:rows.append({'name':folder,'inputs':[{'name':n,'uri':uri,'input_sha256':hashlib.sha256(raw).hexdigest()} for n,raw,uri in blocks],**execute(blocks)})
            except Exception as e:rows.append({'name':folder,'failure':str(e),'pc':hex(probe.uc.reg_read(probe.pc))});break
helpers={}
for module in tuple(sys.modules.values()):
    file=getattr(module,'__file__',None)
    if file:
        path=pathlib.Path(file).resolve()
        if path.suffix=='.py' and path.is_relative_to(repo/'port'):helpers[str(path.relative_to(repo))]=hashlib.sha256(path.read_bytes()).hexdigest()
report={'scope':__doc__,'engine_sha256':hashlib.sha256(a.engine.read_bytes()).hexdigest(),'cache_sha256':lists['cache_sha256'],
    'original_lists_sha256':hashlib.sha256(a.original_lists.read_bytes()).hexdigest(),'script_sha256':hashlib.sha256(pathlib.Path(__file__).read_bytes()).hexdigest(),
    'dependency_sources_sha256':helpers,'direction_opposites':opposites,'cases':rows,
    'original_execution_complete':not a.smoke and len(rows)==25 and not any('failure' in r for r in rows),
    'smoke_execution_complete':a.smoke and len(rows)==4 and not any('failure' in r for r in rows),
    'full_load_blocks_verified':False,'layout_generation_verified':False,'full_loader_verified':False}
a.out.write_text(json.dumps(report,indent=2)+'\n')
print(json.dumps({'cases':len(rows),'opposites':opposites,'complete':report['original_execution_complete'],
    'smoke_complete':report['smoke_execution_complete'],'failures':[r for r in rows if 'failure' in r]}))
raise SystemExit(0 if report['original_execution_complete'] or report['smoke_execution_complete'] else 1)
