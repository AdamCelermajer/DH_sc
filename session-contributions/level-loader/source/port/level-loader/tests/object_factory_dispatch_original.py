"""Execute original factory dispatch and XML entry gates over raw declarations.

Original table lookup, case-sensitive comparisons, factory-address selection,
type-pointer write, Add argument packing and XML null-handle branch execute.
Class construction/Add/property behavior, debug services, handles, Attribute
lookup, sprintf and floating arithmetic are explicit service boundaries. No
real gameplay object is constructed, activated, rendered or saved.
"""
import argparse,hashlib,json,pathlib,struct,sys
ap=argparse.ArgumentParser()
for name in ('engine','dependency-root','factories','out'):ap.add_argument('--'+name,type=pathlib.Path,required=True)
a=ap.parse_args();sys.path.insert(0,str(a.dependency_root))
sys.path.insert(0,str(pathlib.Path(__file__).resolve().parents[2]/'engine-math/tests'))
from differential import Cpu
from unicorn import UC_HOOK_CODE
root=pathlib.Path(__file__).resolve().parents[1]
registry=json.loads(a.factories.read_text())
assert registry['engine_sha256']==hashlib.sha256(a.engine.read_bytes()).hexdigest()=='36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80'
constructors={int(r['factory_address'],16) for r in registry['registry']}
registered={r['gametype']:r for r in registry['registry']}
events=[];attributes={};factory_failure=False;debug_result=False;current={}
def word(at):return struct.unpack('<I',cpu.uc.mem_read(at,4))[0]
def put(at,value):cpu.uc.mem_write(at,struct.pack('<I',value&0xffffffff))
def text(at):
    result=bytearray()
    if not at:return None
    while len(result)<65536:
        b=bytes(cpu.uc.mem_read(at+len(result),1))
        if b==b'\0':return result.decode('utf-8')
        result.extend(b)
    raise AssertionError('unterminated string')
def back(value=0):cpu.write_reg(0,value);cpu.uc.reg_write(cpu.pc_reg,cpu.uc.reg_read(cpu.lr_reg))
class Imports:
    def call(self,c,name):
        if name=='strcmp':
            x,y=text(c.reg(0)),text(c.reg(1));assert x is not None and y is not None
            back((x>y)-(x<y))
        elif name=='sprintf':
            dst,fmt,value,character=[c.reg(i) for i in range(4)]
            format_text=text(fmt);assert format_text is not None
            if format_text=='%s_%c':
                source=text(value);assert source is not None
                formatted=source+'_'+chr(character)
            else:
                # Original first call uses the authored name as the format.
                # Only fixtures without percent conversion are admitted.
                assert '%' not in format_text,format_text
                formatted=format_text
            cpu.uc.mem_write(dst,formatted.encode()+b'\0');back(len(formatted))
        elif name=='__aeabi_fadd':
            x,y=[struct.unpack('<f',struct.pack('<I',c.reg(i)))[0] for i in (0,1)]
            back(struct.unpack('<I',struct.pack('<f',x+y))[0])
        else:raise AssertionError('unmodeled import '+name)
cpu=Cpu(a.engine,False,Imports(),{'functions':[]})
out=cpu.data+0x1000;manager=cpu.data+0x1100;object_at=cpu.data+0x2000;vt=cpu.data+0x3000
element=cpu.data+0x4000;application=cpu.data+0x5000;level=cpu.data+0x6000;offset=cpu.data+0x6800
init_stub=cpu.stop+0x100;game_object_stub=cpu.stop+0x110
def string(value):
    global next_text
    if value is None:return 0
    raw=value.encode()+b'\0';at=next_text;next_text+=(len(raw)+3)&~3
    assert next_text<cpu.data+0xf000
    cpu.uc.mem_write(at,raw);return at
def event(kind,**fields):events.append({'kind':kind,**fields})
def hook(uc,address,size,unused):
    if address in constructors:
        event('factory_callback',address=hex(address),returned_null=factory_failure)
        back(0 if factory_failure else object_at)
    elif address==0x34b270:
        sp=uc.reg_read(cpu.sp_reg)
        assert cpu.reg(1)==manager and cpu.reg(2)==object_at
        event('add',name=text(cpu.reg(3)),raw_type=text(word(sp)),object_id=struct.unpack('<i',uc.mem_read(sp+4,4))[0],
              flag=bool(word(sp+8)),registered_type=text(word(object_at+0x20)))
        put(cpu.reg(0),object_at);put(cpu.reg(0)+4,42);put(cpu.reg(0)+8,0);back(cpu.reg(0))
    elif address==0x337888:event('debug_load');back()
    elif address==0x337a88:event('debug_switch',returned=debug_result);back(int(debug_result))
    elif address==0x3140ec:
        event('string_construct',value=text(cpu.reg(1)))
        at=cpu.reg(0);uc.mem_write(at,bytes(24));put(at+0x14,at);back(at)
    elif address==0x33f50c:
        uc.mem_write(cpu.reg(0),bytes(12));back(cpu.reg(0))
    elif address==0x33fdc0:
        ptr=word(cpu.reg(0));event('resolve_handle',required=bool(cpu.reg(1)),null=not bool(ptr));back(ptr)
    elif address==0x514c70:
        assert cpu.reg(0)==element;back(attributes.get(text(cpu.reg(1)),0))
    elif address in (0x513d78,0x513fec,0x5136ec,0x513a00):
        assert cpu.reg(0)==object_at+4
        event({0x513d78:'init_properties',0x513fec:'set_template',0x5136ec:'load_defaults',0x513a00:'load_overrides'}[address],
              **({'value':current.get('template')} if address==0x513fec else {}));back()
    elif address==init_stub:event('early_init_post');back()
    elif address==game_object_stub:event('is_game_object',returned=current.get('game_object',False));back(int(current.get('game_object',False)))
    elif address==0x33fee4:back(word(cpu.reg(0)))
    elif address==0x393db4:
        assert cpu.reg(0)==object_at
        values=list(struct.unpack('<3f',uc.mem_read(cpu.reg(1),12)))
        event('set_position',value=values,flag=bool(cpu.reg(2)));uc.mem_write(object_at+0x160,struct.pack('<3f',*values));back()
cpu.uc.hook_add(UC_HOOK_CODE,hook)
put(vt+0x1c,init_stub);put(vt+0x20,game_object_stub)
# Explicit Application singleton storage for Level's object-manager lookup.
got=(0x3f01dc+8+word(0x3f0288))&0xffffffff
put(got+word(0x3f0294),application);put(application+0x38,manager)
def reset(attrs=None,fail=False,debug=False):
    global events,attributes,next_text,factory_failure,debug_result,current
    events=[];attributes={};next_text=cpu.data+0x7000;factory_failure=fail;debug_result=debug;current=attrs or {}
    cpu.uc.mem_write(object_at,bytes(0x400));put(object_at,vt)
    cpu.uc.mem_write(object_at+0x160,struct.pack('<3f',1.0,2.0,3.0))
    cpu.uc.mem_write(offset,struct.pack('<3f',1000.0,0.0,-500.0))
    cpu.uc.mem_write(level+0x160,struct.pack('<3f',1000.0,0.0,-500.0));put(level+0x18c,77)
    if attrs:
        for k,v in attrs.items():
            if isinstance(v,str):attributes[k]=string(v)
    # Original assert level zero for constructor-null continuation cases.
    factory_got=(0x34b544+8+word(0x34b700))&0xffffffff
    assert_pointer=word(factory_got+word(0x34b718));put(assert_pointer,0)
rows=[]
for name in list(registered)+[r.swapcase() for r in registered]+['link','unknown','']:
    for fail in ([False,True] if name in registered else [False]):
        reset(fail=fail)
        result=cpu.invoke('_ZN13ObjectManager12GetNewObjectEPKcS1_ib',[out,manager,string(name),string('authored_name')],
                          stack=struct.pack('<II',77,1))
        handle=list(struct.unpack('<3I',cpu.uc.mem_read(out,12)))
        expected=registered.get(name)
        calls=[e for e in events if e['kind']=='factory_callback']
        assert len(calls)==int(expected is not None),(name,fail,expected,events,hex(word(0x95c800)))
        if expected:assert calls[0]['address']==expected['factory_address']
        assert bool(handle[0])==bool(expected and not fail)
        assert result==out
        if handle[0]:
            added=next(e for e in events if e['kind']=='add')
            assert added=={'kind':'add','name':'authored_name','raw_type':name,'object_id':77,'flag':True,'registered_type':name}
        else:assert handle==[0,0,0]
        rows.append({'gametype':name,'factory_failure':fail,'handle_null':not bool(handle[0]),'events':events})
mgp_rows=[]
for source in registry['unregistered_declarations']:
    if not source['uri'].lower().endswith('.mgp'):continue
    for debug in (False,True):
        reset(source['attributes'],debug=debug)
        cpu.invoke('_ZN5Level12_LoadFromXMLEP12TiXmlElement',[level,element])
        assert any(e['kind']=='debug_switch' and e['returned']==debug for e in events)
        assert events[-1]=={'kind':'resolve_handle','required':False,'null':True}
        assert not any(e['kind'] in ('factory_callback','add','init_properties','set_template','load_defaults','load_overrides','early_init_post','is_game_object','set_position') for e in events)
        mgp_rows.append({'uri':source['uri'],'name':source['name'],'attributes':source['attributes'],
                         'debug_return':debug,'returned_without_object':True,'events':events})
gates=[]
for attrs in ({'gametype':'Player','name':'player'}, {'gametype':'player','name':'lowercase'},
              {'gametype':'Decor','name':'decor','game_object':True},
              {'gametype':'LevelConfig','name':'config'}, {'gametype':'Decor','name':'templated','template':'authored_template'},
              {'gametype':'Decor'}, {'name':'unnamed_type'}, {'gametype':'Decor','name':''},
              {'gametype':'Decor','name':'_prim_PlayerLight'},
              {'gametype':'Decor','name':'filter_miss','type_filter':'Character'},
              {'gametype':'Decor','name':'filter_match','type_filter':'Decor'},
              {'gametype':'Decor','name':'filter_case','type_filter':'decor'},
              {'gametype':'Decor','name':'empty_template','template':''}):
    reset(attrs)
    # Missing gametype is checked directly at ObjectManager: Level calls strcmp
    # on the attribute first and its null-input libc behavior is not fabricated.
    route='manager' if 'gametype' not in attrs or 'type_filter' in attrs else 'level'
    if route=='manager':
        cpu.invoke('_ZN13ObjectManager11LoadFromXMLEP12TiXmlElementPKcRK7Point3DIfEi',
                   [manager,element,string(attrs.get('type_filter')),offset],stack=struct.pack('<i',77))
    else:cpu.invoke('_ZN5Level12_LoadFromXMLEP12TiXmlElement',[level,element])
    if attrs.get('gametype')=='Player':assert events==[]
    if attrs.get('gametype')=='Decor' and attrs.get('game_object'):
        assert events[-1]=={'kind':'set_position','value':[1001.0,2.0,-497.0],'flag':True}
    if attrs.get('gametype')=='LevelConfig':assert any(e['kind']=='early_init_post' for e in events)
    if 'template' in attrs:
        kinds=[e['kind'] for e in events]
        assert kinds.index('init_properties')<kinds.index('set_template')<kinds.index('load_defaults')<kinds.index('load_overrides')
    if attrs.get('name')=='_prim_PlayerLight':assert next(e for e in events if e['kind']=='add')['object_id']==-1
    if 'type_filter' in attrs and attrs['type_filter']!=attrs.get('gametype'):assert events==[]
    gates.append({'route':route,'type_filter':attrs.get('type_filter'),'attributes':attrs,'events':events})
report={'validation':'PASS','scope':__doc__,'engine_sha256':registry['engine_sha256'],
        'script_sha256':hashlib.sha256(pathlib.Path(__file__).read_bytes()).hexdigest(),
        'cpu_helper_sha256':hashlib.sha256(pathlib.Path(sys.modules['differential'].__file__).read_bytes()).hexdigest(),
        'factory_receipt_sha256':hashlib.sha256(a.factories.read_bytes()).hexdigest(),
        'capture_sha256':{p:hashlib.sha256((root/p).read_bytes()).hexdigest() for p in
            ('reference/object-loading-lifecycle/0034b520.asm','reference/loader-functions/0034b868.asm',
             'reference/object-loading-lifecycle/003f01c8.asm')},
        'factory_dispatch_cases':rows,'mgp_link_entry_cases':mgp_rows,'xml_gate_cases':gates,
        'imports':cpu.import_calls,'factory_constructor_execution_verified':False,
        'runtime_factory_contract_agreed':False,'runtime_objects_verified':False,'full_loader_verified':False}
a.out.write_text(json.dumps(report,indent=2)+'\n')
print(json.dumps({'validation':'PASS','dispatch_cases':len(rows),'mgp_link_entry_cases':len(mgp_rows),
                  'xml_gate_cases':len(gates),'full_loader_verified':False}))
