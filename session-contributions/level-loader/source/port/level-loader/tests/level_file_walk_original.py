"""Execute original Level::LoadFile from an already-ready raw-buffer state.

Original LoadFromBuffer normalization/parser, top-level IterateChildren,
root-name comparisons, element-only child traversal and document destruction
execute. Initial path/open/async stream handling is outside this domain.
Per-element Level::_LoadFromXML and load-state destructor are service boundaries;
no gameplay factory, condition evaluator, activation or persistent state runs.
"""
import argparse,hashlib,json,pathlib,struct,sys,time,zipfile
ap=argparse.ArgumentParser()
for name in ('engine','dependency-root','out'):ap.add_argument('--'+name,type=pathlib.Path,required=True)
ap.add_argument('--cache',type=pathlib.Path)
a=ap.parse_args();sys.path.insert(0,str(a.dependency_root))
sys.path.insert(0,str(pathlib.Path(__file__).resolve().parent))
from xml_original_probe import Parser
from unicorn import UC_HOOK_CODE
from elftools.elf.elffile import ELFFile
root=pathlib.Path(__file__).resolve().parents[1]
engine_sha=hashlib.sha256(a.engine.read_bytes()).hexdigest()
assert engine_sha=='36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80'
p=Parser(a.engine,{'functions':[]});p.uc.hook_add(UC_HOOK_CODE,p.allocation)
level=p.data+0x1100;state=p.data+0x2100;vt=p.data+0x3100;filename=p.data+0x4100;tag=p.data+0x5100
buffer_pointer=p.data+0x6100;raw_at=p.data+0x100000;call_no=0;events=[];parse_result=None;parse_error=None
destroy_stub=p.stop+0x300
def put(at,value):p.uc.mem_write(at,struct.pack('<I',value&0xffffffff))
def returned(value=0):p.put(0,value);p.uc.reg_write(p.pc,p.uc.reg_read(p.lr))
class UnsafeRoot(Exception):pass
def hook(uc,address,size,unused):
    global parse_result,parse_error
    if address==0x3f01c8:
        assert p.reg(0)==level
        events.append({'call':call_no,'kind':'load_element','element':p.element(p.reg(1))});returned()
    elif address==destroy_stub:
        assert p.reg(0)==state;events.append({'call':call_no,'kind':'release_load_state'});returned()
    elif address==0x3f4014:
        parse_result=bool(p.reg(0));doc=p.word(state+0x38);parse_error=p.word(doc+0x44)
        events.append({'call':call_no,'kind':'parse_result','success':parse_result,'error':parse_error})
    elif address==0x514410 and p.reg(0)==0:
        events.append({'call':call_no,'kind':'unsafe_null_first_child_element'})
        raise UnsafeRoot('Top-level non-element value matched requested root')
p.uc.hook_add(UC_HOOK_CODE,hook)
put(vt+4,destroy_stub)
got=(0x3f3b50+8+p.word(0x3f4104))&0xffffffff
assert_address=p.word(got+p.word(0x3f4134));put(assert_address,0)
# A fixture gets a fresh engine allocator/global state. Merely resetting the
# arena would invalidate the STL allocator's cached free-list pointers.
# Preserve relocated writable segments after helper setup, then restore them
# before reusing the arena. This is fixture isolation, not engine unload policy.
with a.engine.open('rb') as stream:
    mutable=[(s['p_vaddr'],bytes(p.uc.mem_read(s['p_vaddr'],s['p_memsz'])))
             for s in ELFFile(stream).iter_segments() if s['p_type']=='PT_LOAD' and s['p_flags']&2]
def make_string(at,value):
    raw=value.encode();assert len(raw)<1024
    chars=at+0x100;p.uc.mem_write(at,bytes(24));p.uc.mem_write(chars,raw+b'\0')
    put(at+0x10,chars+len(raw));put(at+0x14,chars)
def execute(label,raw,requested):
    global events,call_no,parse_result,parse_error
    events=[];parse_result=None;parse_error=None;p.heap=p.data+0x200000;p.blob=b'';p.cursor=0
    for address,bytes_ in mutable:p.uc.mem_write(address,bytes_)
    assert b'\0' not in raw and len(raw)<0x100000
    p.uc.mem_write(level,bytes(0x200));p.uc.mem_write(state,bytes(0x50))
    put(level+0x140,state);put(state,vt);put(state+0x24,buffer_pointer);put(buffer_pointer,raw_at)
    put(state+0x30,len(raw));p.uc.mem_write(state+0x34,b'\x01')
    p.uc.mem_write(raw_at,raw+b'\0\0');make_string(filename,label);make_string(tag,requested)
    calls=[];unsafe=None
    for call_no in range(65536):
        start=len(events)
        try:result=p.invoke(0x3f3b40,[level,filename,tag],budget=30000000)
        except UnsafeRoot as error:unsafe=str(error);break
        except Exception as error:
            raise RuntimeError((label,call_no,hex(p.uc.reg_read(p.pc)),hex(p.uc.reg_read(p.lr)),events)) from error
        assert result in (0,1)
        calls.append({'returned':bool(result),'event_count':len(events)-start,'load_state_present':bool(p.word(level+0x140))})
        if result:break
    else:raise AssertionError('File caller did not complete: '+label)
    # At assert level zero, parse-failure true return retains the partial state;
    # success traversals release it. True is completion, not a success status.
    if unsafe is None:
        assert parse_result is not None
        assert bool(p.word(level+0x140))==(not parse_result)
    row={'label':label,'requested':requested,'raw_sha256':hashlib.sha256(raw).hexdigest(),
         'raw_length':len(raw),'parse_success':parse_result,'original_xml_error':parse_error,
         'calls':calls,'events':events,'unsafe':unsafe}
    if unsafe is None and not parse_result:
        # A later poll follows the original cleanup branch; it must not parse
        # or instantiate the captured partial tree after terminal parse failure.
        call_no=len(calls);start=len(events)
        result=p.invoke(0x3f3b40,[level,filename,tag],budget=30000000)
        row['failure_cleanup_poll']={'returned':bool(result),'events':events[start:],
                                     'load_state_present':bool(p.word(level+0x140))}
        assert result==1 and p.word(level+0x140)==0
        row['events']=events[:start]
    return row
fixtures=[
 ('single','Module',b'<Module><GameObject gametype="Decor" name="a"/></Module>'),
 ('comments','Module',b'<!--metadata--><Module><!--before--><GameObject name="a" gametype="Decor"/><!--middle--><GameObject name="b" gametype="Decor"/></Module>'),
 ('multiple_roots','Module',b'<Other/><Module><GameObject name="a" gametype="Decor"/></Module><Module><GameObject name="b" gametype="Decor"/></Module>'),
 ('no_matching_root','Level',b'<Module><GameObject name="a" gametype="Decor"/></Module>'),
 ('empty_matching_root','Module',b'<Module/>'),
 ('root_case','module',b'<Module><GameObject name="a" gametype="Decor"/></Module>'),
 ('empty_input','Module',b''),
 ('bad_close','Module',b'<Module><GameObject name="a" gametype="Decor"/></Other>'),
 ('newline_attributes','Module',b'<Module>\r\n<GameObject name="a\rb\r\nc\nd" gametype="Decor"/>\r</Module>'),
 ('declaration','Module',b'<?xml version="1.0"?><Module><GameObject name="a" gametype="Decor"/></Module>'),
 ('declaration_only','Module',b'<?xml version="1.0"?>'),
 ('comment_matches_root','Module',b'<!--Module--><Module/>'),
]
rows=[execute(name,raw,requested) for name,requested,raw in fixtures]
cache_sha=None;cache_rows=[]
if a.cache:
    with a.cache.open('rb') as stream:cache_sha=hashlib.file_digest(stream,'sha256').hexdigest()
    assert cache_sha=='3fdf4e4c21d45a780a7c35fb4042abde0e88e76bf75416aad1f227481560b679'
    prefix='com.gameloft.android.GAND.GloftD2SS/files/'
    with zipfile.ZipFile(a.cache) as pack:
        for entry in pack.infolist():
            if entry.is_dir():continue
            assert entry.filename.startswith(prefix)
            uri=entry.filename[len(prefix):];extension=pathlib.PurePosixPath(uri).suffix.lower()
            if extension not in ('.mlx','.mgp','.mvp'):continue
            cache_rows.append(execute(uri,pack.read(entry),'Level' if extension=='.mlx' else 'Module'))
            if len(cache_rows)%50==0:print(json.dumps({'cache_files_completed':len(cache_rows)}),flush=True)
report={'validation':'PASS','scope':__doc__,'engine_sha256':engine_sha,'cache_sha256':cache_sha,
        'script_sha256':hashlib.sha256(pathlib.Path(__file__).read_bytes()).hexdigest(),
        'parser_helper_sha256':hashlib.sha256(pathlib.Path(sys.modules['xml_original_probe'].__file__).read_bytes()).hexdigest(),
        'capture_sha256':{path:hashlib.sha256((root/path).read_bytes()).hexdigest() for path in
            ('reference/loader-functions/003f3b40.asm','reference/loader-functions/005164a8.asm')},
        'fixtures':rows,'cache_files':cache_rows,'fixture_source':{name:raw.decode() for name,requested,raw in fixtures},
        'initial_open_async_state_verified':False,'runtime_objects_verified':False,'full_loader_verified':False}
a.out.write_text(json.dumps(report,indent=2)+'\n')
print(json.dumps({'validation':'PASS','fixtures':len(rows),'cache_files':len(cache_rows),
                  'parse_failures':sum(not r['parse_success'] for r in rows+cache_rows),
                  'unsafe_roots':sum(bool(r['unsafe']) for r in rows+cache_rows),'full_loader_verified':False}))
