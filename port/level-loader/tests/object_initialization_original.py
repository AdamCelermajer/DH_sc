"""Execute original ObjectManager::InitPost's resumable phase dispatcher.

Manager lists and int-keyed tree nodes are explicit fixtures. ObjectHandle,
Module::LoadModule, per-object InitPost/IsUpdatable/conditions, RoomZone object
list initialization, allocation and list clear are service boundaries. The
actual ARM dispatcher walks and mutates the supplied manager/list/tree state.
No constructors, factory, full object behavior, gameplay or persistence execute.
"""
import argparse, hashlib, json, pathlib, struct, sys

ap=argparse.ArgumentParser()
for n in ('engine','dependency-root','out'):ap.add_argument('--'+n,type=pathlib.Path,required=True)
a=ap.parse_args()
sys.path.insert(0,str(a.dependency_root))
sys.path.insert(0,str(pathlib.Path(__file__).resolve().parents[2]/'engine-math/tests'))
from differential import Cpu
from unicorn import UC_HOOK_CODE

engine_sha=hashlib.sha256(a.engine.read_bytes()).hexdigest()
assert engine_sha=='36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80'
class Imports:
    def call(self,c,name):
        if name not in ('strcmp','strcasecmp'):raise RuntimeError('Unmodeled import: '+name)
        x,y=string(c.reg(0)),string(c.reg(1))
        if name=='strcasecmp':x,y=x.lower(),y.lower()  # checked ASCII fixture domain
        c.write_reg(0,0 if x==y else 1)
        back()
c=Cpu(a.engine,False,Imports(),{'functions':[]})
manager=c.data+0x1000;vt=c.data+0x2000;heap=c.data+0xa000
init_stub=c.stop+0x100;updating_stub=c.stop+0x110
objects={};events=[];allocations=[];fixture={};call_no=0
def word(at):return struct.unpack('<I',c.uc.mem_read(at,4))[0]
def put(at,value):c.uc.mem_write(at,struct.pack('<I',value))
def string(at):
    out=bytearray()
    while at and (value:=bytes(c.uc.mem_read(at,1))[0]):out.append(value);at+=1
    return out.decode()
def back():c.uc.reg_write(c.pc_reg,c.uc.reg_read(c.lr_reg))
def event(kind,at=None,**fields):
    events.append({'call':call_no,'phase':word(manager+0x7c),'kind':kind,
                   **({'object':objects[at]['label']} if at is not None else {}),**fields})
def allocate():
    global heap
    at=heap;heap+=16;assert heap<c.data+0xf000
    allocations.append(at);c.uc.mem_write(at,bytes(16));return at
def append(head,at):
    node=allocate();previous=word(head+4)
    put(node,head);put(node+4,previous);put(node+8,at)
    put(previous,node);put(head+4,node)
def list_objects(head):
    result=[];at=word(head)
    while at!=head:
        assert len(result)<100
        result.append(objects[word(at+8)]['label']);at=word(at)
    return result
def insert_object(at):
    node=objects[at]['node'];header=manager+0x0c
    key=struct.unpack('<i',c.uc.mem_read(node+16,4))[0]
    parent=header;cursor=word(header+4);side=4
    while cursor:
        parent=cursor
        existing=struct.unpack('<i',c.uc.mem_read(cursor+16,4))[0]
        assert key!=existing
        side=8 if key<existing else 12;cursor=word(parent+side)
    put(node+4,parent);put(parent+side,node)
    first=word(header+8);last=word(header+12)
    if first==header or key<struct.unpack('<i',c.uc.mem_read(first+16,4))[0]:put(header+8,node)
    if last==header or key>struct.unpack('<i',c.uc.mem_read(last+16,4))[0]:put(header+12,node)
def hook(uc,address,size,unused):
    if address==0x33f50c:
        uc.mem_write(c.reg(0),bytes(12));back()
    elif address==0x33f524:
        put(c.reg(0),c.reg(1));put(c.reg(0)+4,0);put(c.reg(0)+8,0);back()
    elif address==0x33fdc0:
        at=word(c.reg(0));value=at if at in objects and not objects[at].get('deleted') else 0
        c.write_reg(0,value);back()
    elif address==0x38a88c:
        at=c.reg(0);event('load_module',at)
        if objects[at].get('append_module'):
            append(manager+0x68,objects[at]['append_module']);event('append_module',objects[at]['append_module'])
        if objects[at].get('insert_object'):
            insert_object(objects[at]['insert_object']);event('insert_object',objects[at]['insert_object'])
        back()
    elif address==init_stub:
        at=c.reg(0);event('init_post',at)
        if 'init_a8' in objects[at]:put(at+0xa8,objects[at]['init_a8'])
        back()
    elif address==updating_stub:
        at=c.reg(0);event('is_updatable',at);c.write_reg(0,int(objects[at].get('updating',False)));back()
    elif address==0x33e6d4:
        event('test_enable_condition',c.reg(0),force=bool(c.reg(1)));back()
    elif address==0x396c44:
        at=c.reg(0);event('room_init_object_list',at)
        if 'room_cc' in objects[at]:put(at+0xcc,objects[at]['room_cc'])
        back()
    elif address in (0x708ec0,0x343168):
        c.write_reg(0,allocate());back()
    elif address==0x34526c:
        head=c.reg(0);event('clear_list',list_offset=hex(head-manager),objects=list_objects(head))
        put(head,head);put(head+4,head);back()
c.uc.hook_add(UC_HOOK_CODE,hook)
put(vt+0x1c,init_stub);put(vt+0x38,updating_stub)
assert word(c.symbols['_ZTV10ObjectBase']+8+0x38)==c.symbols['_ZNK10ObjectBase11IsUpdatableEv']
# Both guarded function-static iterators are already initialized in fixtures.
# They are overwritten by original phase-zero setup on each complete run.
statics=(0x34553c+8+word(0x3458ac))&0xffffffff
assert statics==(0x345550+8+word(0x3458b0))&0xffffffff
put(statics+0x1c,1);put(statics+0x24,1)

fixtures=[
    {'name':'empty','objects':[],'modules':[]},
    {'name':'single','objects':[{'label':'decor','type':'Decor'}],'modules':[]},
    {'name':'updating','objects':[{'label':'npc','type':'Character','updating':True}],'modules':[]},
    {'name':'module_then_objects','objects':[{'label':'module','type':'Module'},{'label':'npc','type':'Character','updating':True},{'label':'chest','type':'OpenableContainer'}],'modules':['module']},
    {'name':'dead_handle','objects':[{'label':'deleted','type':'Decor','deleted':True},{'label':'live','type':'Decor'}],'modules':[]},
    {'name':'room_flags','objects':[{'label':'room','type':'RoomZone','a8':1},{'label':'sound','type':'Sound','updating':True,'cc':1},{'label':'suppressed_ac','type':'Decor','a8':1,'ac':1},{'label':'suppressed_d0','type':'Decor','cc':1,'d0':1},{'label':'updating_a8_cc','type':'Character','updating':True,'a8':1,'cc':1}],'modules':[]},
    {'name':'module_append','objects':[{'label':'first','type':'Module','append':'second'},{'label':'second','type':'Module'},{'label':'decor','type':'Decor'}],'modules':['first']},
    {'name':'five_modules','objects':[{'label':f'm{i}','type':'Module'} for i in range(5)],'modules':[f'm{i}' for i in range(5)]},
    {'name':'room_casefold','objects':[{'label':'room','type':'rOoMzOnE','a8':1}],'modules':[]},
    {'name':'balanced_tree','tree':'balanced','objects':[{'label':f'actor{i}','type':'Decor','updating':bool(i%2)} for i in range(7)],'modules':[]},
    {'name':'all_flags','objects':[{'label':f'flags{i}','type':'Decor','updating':bool(i&1),'a8':bool(i&2),'ac':bool(i&4),'cc':bool(i&8),'d0':bool(i&16)} for i in range(32)],'modules':[]},
    {'name':'clear_preexisting_lists','preseed':True,'objects':[{'label':'actor','type':'Decor','updating':True,'a8':1}],'modules':[]},
    {'name':'registry_order','tree':'balanced','objects':[{'label':'last','type':'Decor','key':200},{'label':'first','type':'Decor','key':-42},{'label':'middle','type':'Decor','key':7}],'modules':[]},
    {'name':'init_changes_membership','objects':[{'label':'actor','type':'Decor','init_a8':1}],'modules':[]},
    {'name':'room_changes_membership','objects':[{'label':'room','type':'RoomZone','room_cc':1}],'modules':[]},
    {'name':'module_inserts_registry','objects':[{'label':'module1','type':'Module','key':10,'insert':'before'},
        {'label':'module2','type':'Module','key':15,'insert':'after'},
        {'label':'before','type':'Character','key':-5,'deferred':True,'updating':True},
        {'label':'after','type':'OpenableContainer','key':20,'deferred':True}], 'modules':['module1','module2']},
]
rows=[]
for fixture in fixtures:
    c.uc.mem_write(manager,bytes(0x100));heap=c.data+0xa000;objects={};allocations=[];events=[]
    for offset in (0x24,0x2c,0x34,0x44,0x60,0x68):put(manager+offset,manager+offset);put(manager+offset+4,manager+offset)
    header=manager+0x0c;put(header+4,0);put(header+8,header);put(header+12,header)
    by_label={};nodes=[]
    for i,source in enumerate(fixture['objects']):
        # Keep object, tree, text and allocation regions disjoint even for
        # the exhaustive 32-object branch fixture.
        at=c.data+0x3000+i*0x100;node=c.data+0x7000+i*0x50
        c.uc.mem_write(at,bytes(0x100));c.uc.mem_write(node,bytes(0x50))
        put(at,vt);text=source['type'].encode();text_at=c.data+0x9000+i*0x40
        c.uc.mem_write(text_at,text+b'\0');put(at+0x5c,text_at)
        for field in ('a8','ac','cc','d0'):put(at+int(field,16),source.get(field,0))
        objects[at]={**source,'node':node};by_label[source['label']]=at;nodes.append(node);put(node+0x2c,at)
        c.uc.mem_write(node+16,struct.pack('<i',source.get('key',i)))
    order=sorted((i for i in range(len(nodes)) if not fixture['objects'][i].get('deferred')),
                 key=lambda i:fixture['objects'][i].get('key',i))
    assert len({fixture['objects'][i].get('key',i) for i in order})==len(order)
    for rank,index in enumerate(order):
        node=nodes[index];put(node+4,header if rank==0 else nodes[order[rank-1]])
        if rank:put(nodes[order[rank-1]]+12,node)
    if order:put(header+4,nodes[order[0]]);put(header+8,nodes[order[0]]);put(header+12,nodes[order[-1]])
    if fixture.get('tree')=='balanced':
        for node in nodes:put(node+8,0);put(node+12,0)
        def balanced(indices,parent):
            if not indices:return 0
            middle=len(indices)//2;node=nodes[indices[middle]];put(node+4,parent)
            put(node+8,balanced(indices[:middle],node));put(node+12,balanced(indices[middle+1:],node))
            return node
        put(header+4,balanced(order,header))
    for obj in objects.values():
        if obj.get('append'):obj['append_module']=by_label[obj['append']]
        if obj.get('insert'):obj['insert_object']=by_label[obj['insert']]
    for label in fixture['modules']:append(manager+0x68,by_label[label])
    if fixture.get('preseed'):
        for offset in (0x2c,0x34,0x44):append(manager+offset,by_label['actor'])
    calls=[]
    for call_no in range(100):
        before=word(manager+0x7c);start=len(events)
        result=c.invoke('_ZN13ObjectManager8InitPostEv',[manager])
        assert result in (0,1)
        calls.append({'phase_before':before,'phase_after':word(manager+0x7c),'returned':bool(result),'event_count':len(events)-start})
        if result:break
    else:raise AssertionError('Fixture did not complete: '+fixture['name'])
    # Source transition 3 -> 4 clears three lists, then phase 4 repopulates
    # reached memberships. Completion advances to phase 5 and returns true.
    assert word(manager+0x7c)==5
    rows.append({'fixture':fixture,'calls':calls,'events':events,
                 'room_list':list_objects(manager+0x24),
                 'transient_lists':{hex(o):list_objects(manager+o) for o in (0x2c,0x34,0x44)},
                 'module_list':list_objects(manager+0x68), 'allocation_count':len(allocations)})
root=pathlib.Path(__file__).resolve().parents[1]
paths=[str(p.relative_to(root)) for folder in ('object-loading-lifecycle','object-manager-add') for p in (root/'reference'/folder).glob('*') if p.is_file()]
report={'validation':'PASS','scope':__doc__,'engine_sha256':engine_sha,
        'script_sha256':hashlib.sha256(pathlib.Path(__file__).read_bytes()).hexdigest(),
        'cpu_helper_sha256':hashlib.sha256(pathlib.Path(sys.modules['differential'].__file__).read_bytes()).hexdigest(),
        'capture_sha256':{p:hashlib.sha256((root/p).read_bytes()).hexdigest() for p in paths},
        'cases':rows,'original_import_calls':c.import_calls,'runtime_factory_verified':False,'full_loader_verified':False}
a.out.write_text(json.dumps(report,indent=2)+'\n')
print(json.dumps({'validation':'PASS','fixtures':len(rows),'calls':sum(len(r['calls']) for r in rows),
                  'events':sum(len(r['events']) for r in rows),'summaries':[{'name':r['fixture']['name'],
                  'calls':len(r['calls']),'room_list':r['room_list'],'transient_lists':r['transient_lists']} for r in rows]}))
