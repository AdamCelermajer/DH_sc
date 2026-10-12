"""Execute Module::LoadModule ordering with explicit selection/file-service results.

The actual ARM caller, SetObjectModuleId, position writes, file loops and string
lifetimes execute. _ChooseXmls selection and Level::LoadFile are service stubs;
no factory, properties, XML parsing, conditions or active-level transition runs.
"""
import argparse,hashlib,json,pathlib,struct,sys
ap=argparse.ArgumentParser()
for name in ('engine','dependency-root','out'):ap.add_argument('--'+name,type=pathlib.Path,required=True)
a=ap.parse_args();sys.path.insert(0,str(a.dependency_root));sys.path.insert(0,str(pathlib.Path(__file__).parent))
from xml_original_probe import Parser
from unicorn import UC_HOOK_CODE
from elftools.elf.elffile import ELFFile
root=pathlib.Path(__file__).resolve().parents[1]
engine_sha=hashlib.sha256(a.engine.read_bytes()).hexdigest()
assert engine_sha=='36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80'
p=Parser(a.engine,{'functions':[]});p.uc.hook_add(UC_HOOK_CODE,p.allocation)
module=p.data+0x10000;level=p.data+0x20000;events=[];current=None;polls={};strings=p.data+0x100000
def put(at,value):p.uc.mem_write(at,struct.pack('<I',value&0xffffffff))
got=(0x38a89c+8+p.word(0x38aa94))&0xffffffff
level_global=p.word(got+p.word(0x38aa98));put(level_global,level)
def signed(value):return value if value<0x80000000 else value-0x100000000
def offset():return list(struct.unpack('<3I',bytes(p.uc.mem_read(level+0x160,12))))
def write_string(at,value,raw_at):
    raw=value.encode();assert b'\0' not in raw
    p.uc.mem_write(at,bytes(24));p.uc.mem_write(raw_at,raw+b'\0')
    put(at+0x10,raw_at+len(raw));put(at+0x14,raw_at)
def text(at):return p.text(p.word(at+0x14))
def hook(uc,address,size,unused):
    if address==0x3ef278:
        assert p.reg(0)==level
        events.append({'kind':'set_module_id','value':signed(p.reg(1))}) # actual leaf store executes
    elif address in (0x38a8fc,0x38aa04):
        events.append({'kind':'set_offset','words':offset()}) # observes actual complete word writes
    elif address==0x38a38c:
        assert p.reg(0)==module
        events.append({'kind':'choose','module_id':signed(p.word(level+0x18c)),'offset':offset()})
        write_string(p.reg(1),current['gameplay'],strings)
        write_string(p.reg(2),current['visual'],strings+0x10000)
        p.returned()
    elif address==0x3f3b40:
        assert p.reg(0)==level
        slot='gameplay' if uc.reg_read(p.lr)==0x38a97c else 'visual'
        assert uc.reg_read(p.lr) in (0x38a97c,0x38a9d8)
        returned=polls[slot]>=current[slot+'_pending'];polls[slot]+=1
        events.append({'kind':'load_file','uri':text(p.reg(1)),'root':text(p.reg(2)),
                       'returned':returned,'module_id':signed(p.word(level+0x18c)),'offset':offset()})
        p.returned(int(returned))
p.uc.hook_add(UC_HOOK_CODE,hook)
with a.engine.open('rb') as stream:
    mutable=[(s['p_vaddr'],bytes(p.uc.mem_read(s['p_vaddr'],s['p_memsz'])))
             for s in ELFFile(stream).iter_segments() if s['p_type']=='PT_LOAD' and s['p_flags']&2]
cases=[]
for label,gameplay,visual,gp,vp in (
 ('empty','','',0,0),('gameplay_only','placements.mgp','',2,0),('visual_only','','placements.mvp',0,3),
 ('both','placements.mgp','placements.mvp',0,0),('both_pending','placements.mgp','placements.mvp',3,4),
 ('same_uri','same','same',2,3),('selection_spelling',' Source/UPPER.mgp ','Other\\path.mvp',1,1),
 ('negative_identity','gameplay.mgp','visual.mvp',1,2),('word_preservation','gameplay.mgp','visual.mvp',2,1)):
    cases.append({'label':label,'gametype':'Block' if label=='same_uri' else 'Module',
                  'module_id':-1 if label=='negative_identity' else 42,
                  'position_words':[0x80000000,0x7fc01234,0x7f800000] if label=='word_preservation'
                  else [0x447a0000,0xc0000000,0x43fa0000],
                  'gameplay':gameplay,'visual':visual,'gameplay_pending':gp,'visual_pending':vp})
for current in cases:
    for address,blob in mutable:p.uc.mem_write(address,blob)
    p.heap=p.data+0x200000;p.blob=b'';p.cursor=0;events=[];polls={'gameplay':0,'visual':0}
    p.uc.mem_write(module,bytes(0x500));p.uc.mem_write(level,bytes(0x200))
    put(module+0x40c,current['module_id']);p.uc.mem_write(module+0x160,struct.pack('<3I',*current['position_words']))
    p.invoke(0x38a88c,[module],budget=3000000)
    assert p.word(level+0x18c)==0xffffffff and offset()==[0,0,0]
    current['events']=events;current['file_polls']=polls;current['final_context']={'module_id':-1,'offset':[0,0,0]}
report={'validation':'PASS','scope':__doc__,'engine_sha256':engine_sha,
        'script_sha256':hashlib.sha256(pathlib.Path(__file__).read_bytes()).hexdigest(),
        'parser_helper_sha256':hashlib.sha256(pathlib.Path(sys.modules['xml_original_probe'].__file__).read_bytes()).hexdigest(),
        'capture_sha256':{path:hashlib.sha256((root/path).read_bytes()).hexdigest() for path in
                          ('reference/loader-functions/0038a88c.asm','reference/loader-functions/003ef278.asm')},
        'cases':cases,'original_selection_verified':False,'original_file_loading_verified':False,
        'runtime_objects_verified':False,'full_loader_verified':False}
a.out.write_text(json.dumps(report,indent=2)+'\n')
print(json.dumps({'validation':'PASS','cases':len(cases),'file_polls':sum(sum(c['file_polls'].values()) for c in cases),
                  'full_loader_verified':False}))
