"""Recover source Sounds bindings from registered generated stream, not labels.

Bounded original ARM execution only. Constructor registration callees are
observed services; real Arrays::Sounds and SoundAutoGen readers execute with
exact serialized bytes and explicit stream/allocator boundaries.
"""
from pathlib import Path
import sys, struct, json, hashlib, zipfile
import xml.etree.ElementTree as ET

ROOT = Path(__file__).resolve().parents[3]
OUT = ROOT / 'port/engine-audio/reference/source-bindings-v38'
OUT.mkdir(parents=True, exist_ok=True)
sys.path.insert(0, r'C:/Users/adamc/AppData/Local/uv/cache/archive-v0/jc8RoeQ1US_9Gkmi/Lib/site-packages')
sys.path.insert(0, str(ROOT / 'port/engine-animation/tests'))
from particle_factory_differential import FactoryCpu
from capstone import Cs, CS_ARCH_ARM, CS_MODE_ARM
from unicorn import UC_HOOK_CODE

LIB = ROOT / '.local-inputs/libDungeonHunter2.so'
ARCHIVE = Path(r'C:/Users/adamc/Downloads/Dungeon-Hunter-2-HD-v1-0-2-cache.zip')
PREFIX = 'com.gameloft.android.GAND.GloftD2SS/files/'
cpu = FactoryCpu(LIB, False, {'functions': []})
md = Cs(CS_ARCH_ARM, CS_MODE_ARM)
md.skipdata = True
word = lambda a: struct.unpack('<I', cpu.uc.mem_read(a, 4))[0]
pack = lambda v: struct.pack('<I', v & 0xffffffff)
registration_calls = {i.address: i for i in md.disasm(bytes(cpu.uc.mem_read(0x4be550, 10380)), 0x4be550) if i.mnemonic == 'bl'}
registrations = []
phase = 'register'
stream = b''
position = 0
heap = cpu.data + 0x100000
reads = []

def ret(value=0):
    cpu.put(0, value)
    cpu.uc.reg_write(cpu.pc, cpu.uc.reg_read(cpu.lr))

def consume(count):
    global position
    assert count >= 0 and position + count <= len(stream), (position, count, len(stream))
    result = stream[position:position+count]
    reads.append([position, count])
    position += count
    return result

def hook(uc, at, size, unused):
    global heap
    if phase == 'register' and at in registration_calls:
        ins = registration_calls[at]
        if ins.op_str in ('#0x4be3e0', '#0x4bdccc'):
            registrations.append(dict(call=hex(at), operation='file' if ins.op_str == '#0x4be3e0' else 'class', name=cpu.string(cpu.reg(1)).decode(), function=hex(cpu.reg(2)), finalizer=hex(cpu.reg(3))))
        # Only callee services are excluded; original constructor instructions
        # produce all names/function arguments from its relocated GOT/literals.
        uc.reg_write(cpu.pc, at+4)
    elif phase == 'read':
        if at == 0x313a90:
            ret(struct.unpack('<I', consume(4))[0])
        elif at in (0x459090, 0x3df1a0):
            uc.mem_write(cpu.reg(1), consume(4)); ret()
        elif at == 0x317454:
            count = cpu.reg(2); uc.mem_write(cpu.reg(1), consume(count)); ret(count)
        elif at == 0x31056c:
            count = cpu.reg(0); assert count < 0x100000
            address = (heap+15)&~15; heap = address + max(16, count)
            uc.mem_write(address, bytes(count)); ret(address)

cpu.uc.hook_add(UC_HOOK_CODE, hook)
cpu.invoke(0x4be550, [cpu.data + 0x1000, 0], budget=100000)
binding_registration = next(r for r in registrations if r['operation'] == 'file' and r['function'] == '0x4b5690')
names_registration = next(r for r in registrations if r['operation'] == 'file' and r['function'] == '0x4b313c')
assert binding_registration['name'] == 'sdd_dungeon_hunter_2_iphone_pyarray.bin'
assert names_registration['name'] == 'sdd_dungeon_hunter_2_iphone_pyarraynames.bin'

with zipfile.ZipFile(ARCHIVE) as archive:
    binary = archive.read(PREFIX + 'data/pydata/' + binding_registration['name'])
    name_binary = archive.read(PREFIX + 'data/pydata/' + names_registration['name'])
    xml_binary = archive.read(PREFIX + 'data/sounds/sounds.xml')
    delivered = {Path(n).name for n in archive.namelist() if n.startswith(PREFIX+'data/sounds/')}

for name, data in [(binding_registration['name'], binary), (names_registration['name'], name_binary)]:
    (OUT / name).write_bytes(data)

phase = 'read'; stream = binary; position = 0; reads = []
cpu.invoke(0x4b5690, [cpu.data+0x2000], budget=500000)
assert position == len(binary)
count = word(0x9a6758); members = word(0x9a675c)
assert count == 638 and len(binary) == 4+count*8
serialized_rows = [struct.unpack_from('<ii', binary, 4+8*i) for i in range(count)]
runtime_rows = [struct.unpack('<ii', cpu.uc.mem_read(members+12*i+4, 8)) for i in range(count)]
assert runtime_rows == serialized_rows
(OUT/'original-runtime-pairs.bin').write_bytes(b''.join(struct.pack('<ii', *row) for row in runtime_rows))
assert all(word(members+12*i) == 0x96a3e8+8 for i in range(count))
assert reads == [[4*i, 4] for i in range(1+2*count)]
row_reads = reads[:]
stream = name_binary; position = 0; reads = []
cpu.invoke(0x4b313c, [cpu.data+0x2000], budget=500000)
assert position == len(name_binary)
name_pointer = word(0x9a6760)
names = [cpu.string(word(name_pointer+4*i)).decode() for i in range(count)]
assert len(set(names)) == count
lookup_cases=[]
for expected in (0,33,62,157,270,478,637,-1):
    text=names[expected] if expected>=0 else 'RequiredMissingName'
    cpu.uc.mem_write(cpu.data+0x3000,text.encode()+b'\0')
    value=cpu.invoke(0x37ba84,[cpu.data+0x3000],budget=1000000)
    actual=struct.unpack('<i',pack(value))[0]
    assert actual==expected,(text,actual,expected)
    lookup_cases.append(dict(name=text,source_id=actual))

xml = ET.fromstring(xml_binary)
sounds = {int(n.attrib['uid']):dict(n.attrib) for n in xml.findall('sounds/sound')}
events = {int(n.attrib['uid']):dict(n.attrib) for n in xml.findall('events/event')}
event_elements = {}
for n in xml.findall('events/event'):
    event_elements[int(n.attrib['uid'])] = [int(value) for value in n.attrib['value'].split(',') if value.strip()]

ledger = []
for source_id, ((uid, event), name) in enumerate(zip(runtime_rows, names)):
    assert event in (0,1)
    target = (events if event else sounds).get(uid)
    row = dict(source_id=source_id, source_name=name, uid=uid, event=event, serialized_offset=4+8*source_id, runtime_stride=12, status='missing')
    if target:
        row['target'] = target
        if event:
            row['sound_uids'] = event_elements[uid]
            row['assets'] = [dict(uid=i, filename=sounds[i]['filename'], exists=sounds[i]['filename'] in delivered) for i in row['sound_uids']]
            row['status'] = 'done' if row['assets'] and all(a['exists'] for a in row['assets']) else 'partial' if any(a['exists'] for a in row['assets']) else 'missing'
        else:
            row['assets'] = [dict(uid=uid, filename=target['filename'], exists=target['filename'] in delivered)]
            row['status'] = 'done' if row['assets'][0]['exists'] else 'missing'
    ledger.append(row)

def assembly(address, length, filename):
    (OUT/filename).write_text('\n'.join(f'{i.address:08x} {i.mnemonic} {i.op_str}' for i in md.disasm(bytes(cpu.uc.mem_read(address, length)), address))+'\n')

assembly(0x4c05b8, 0x224, 'legacy-registration.asm')
assembly(0x4c0d64, 0x70, 'generated-registration.asm')
assembly(0x4b5690, 324, 'sounds-read.asm')
assembly(0x502af4, 208, 'sound-autogen-read.asm')
assembly(0x4b313c, 408, 'sounds-read-names.asm')
(OUT/'ledger.json').write_text(json.dumps(ledger, indent=2)+'\n')
(OUT/'registration.json').write_text(json.dumps([r for r in registrations if 'sound' in r['name'].lower() or 'sdd_' in r['name']], indent=2)+'\n')
report = dict(validation='PASS', original_instructions_executed=True, registration_callee_boundary='Observed names/functions; callee registrations excluded. Source constructor instructions execute.', reader_boundary='Exact archive stream reads and zero-filled allocation service; original whole Arrays::Sounds::read/readNames and SoundAutoGen::read execute.', original_sha256=hashlib.sha256(LIB.read_bytes()).hexdigest(), binary_sha256=hashlib.sha256(binary).hexdigest(), names_sha256=hashlib.sha256(name_binary).hexdigest(), xml_sha256=hashlib.sha256(xml_binary).hexdigest(), binding_registration=binding_registration, names_registration=names_registration, rows=count, event_rows=sum(e for u,e in runtime_rows), consumed_binary=len(binary), consumed_names=len(name_binary), row_word_reads=len(row_reads), all_runtime_pairs_match=True, all_runtime_vtables_match=True, xml_targets_missing=sum('target' not in r for r in ledger), statuses={s:sum(r['status']==s for r in ledger) for s in ('done','partial','missing')}, scope='Binding and referenced exact-asset availability only. Done does not mean event producer, decoded playback, APK integration or audible QA. No emulator/device used.')
report['source_name_lookups']=lookup_cases

# Whole VoxSoundManager C1 constructor argument production. Resource-root,
# filesystem, XML parsing, Vox allocation and engine endpoint are explicit
# services. Its selected XML path is observed before those services consume it.
soundpack_paths=[]
for zip_flag in (0,1):
    constructor=FactoryCpu(LIB,False,{'functions':[]})
    ctor_calls={i.address:i for i in md.disasm(bytes(constructor.uc.mem_read(0x36c7b0,728)),0x36c7b0) if i.mnemonic=='bl'}
    owner=constructor.data+0x1000; resource_root=owner+0x1000; service=resource_root+0x1000
    constructor.uc.mem_write(resource_root,b'/verified-root/\0')
    constructor.pointer(service,service+0x100)
    constructor.pointer(service+0x108,service+0x200)
    constructor.pointer(service+0x110,service+0x204)
    captured=[]; zip_captured=[]; ctor_heap=service+0x1000
    cw=lambda a:struct.unpack('<I',constructor.uc.mem_read(a,4))[0]
    def ctor_hook(uc,at,size,unused):
        global ctor_heap
        if at==0x36c8bc:
            environment=cw(constructor.reg(5)+cw(0x36ca5c))
            uc.mem_write(environment+0xa8,bytes([zip_flag]))
        if at==0x36c8d0:
            root_global=cw(constructor.reg(5)+cw(0x36ca60))
            constructor.pointer(root_global,resource_root)
        if at in (service+0x200,service+0x204):
            if at==service+0x204:zip_captured.append(constructor.string(constructor.reg(1)).decode())
            constructor.put(0,0);uc.reg_write(constructor.pc,uc.reg_read(constructor.lr));return
        if at not in ctor_calls:return
        target=ctor_calls[at].op_str
        if target=='#0x30e520':
            uc.mem_write(constructor.reg(0),constructor.string(constructor.reg(1))+b'\0')
        elif target=='#0x30de54':constructor.put(0,len(constructor.string(constructor.reg(0))))
        elif target=='#0x30e868':
            length=constructor.reg(2);data=constructor.string(constructor.reg(1))+b'\0';uc.mem_write(constructor.reg(0),data[:length].ljust(length,b'\0'))
        elif target=='#0x88d344':captured.append(constructor.string(constructor.reg(1)).decode());constructor.put(0,1)
        elif target=='#0x31056c':
            length=constructor.reg(0);assert length<0x100000;address=(ctor_heap+15)&~15;ctor_heap=address+max(length,16);uc.mem_write(address,bytes(max(length,16)));constructor.put(0,address)
        elif target=='#0x30e460':uc.mem_write(constructor.reg(0),bytes([constructor.reg(1)&255])*constructor.reg(2))
        elif target in ('#0x862b30','#0x8945a4'):constructor.put(0,service)
        # 56e064 resource resolution returns the same path fixture.
        uc.reg_write(constructor.pc,at+4)
    constructor.uc.hook_add(UC_HOOK_CODE,ctor_hook)
    constructor.invoke(0x36c7b0,[owner],budget=100000)
    assert captured==['/verified-root/data/sounds/sounds.xml'],captured
    assert bool(zip_captured)==bool(zip_flag)
    soundpack_paths.append(dict(zip_flag=zip_flag,xml=captured[0],zip_service_paths=zip_captured))
report['source_soundpack_constructor']=dict(address='0x36c7b0',cases=soundpack_paths,scope='Original XML path argument instructions execute; XML parser, root resource producer, optional zip filesystem and engine endpoints are explicit services. No sounds_he selection branch in this constructor.')
assembly(0x36c7b0,728,'sound-manager-constructor.asm')
(OUT/'proof.json').write_text(json.dumps(report, indent=2)+'\n')
print(json.dumps(report))
