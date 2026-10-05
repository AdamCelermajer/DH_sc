"""Execute original named-node reset and visual placement against native TRS.

Actual ARM ResetPositionFromFile, ISceneNode setters, GameObject transform
normalization block and VisualObject::Sync execute. Absolute-position updates
and mesh-box services are observed boundaries; imported arithmetic/libm use
host arithmetic/Python libm. A supplied wrapper and selected-child fixture is
used; Collada construction, full InitPost, shader/render equivalence, animation
and persistence are not claimed. VisualObject constructor's scene-load prefix
also executes, through AssetManager, with SceneManager as an observed boundary.
"""
import argparse, hashlib, json, math, os, pathlib, random, struct, subprocess, sys

ap = argparse.ArgumentParser()
for n in ('engine', 'dependency-root', 'probe', 'out'):
    ap.add_argument('--' + n, type=pathlib.Path, required=True)
a = ap.parse_args()
sys.path.insert(0, str(a.dependency_root))
sys.path.insert(0, str(pathlib.Path(__file__).resolve().parents[2] / 'engine-math/tests'))
from differential import Cpu, f32, scalar
from unicorn import UC_HOOK_CODE
from unicorn.arm_const import UC_ARM_REG_R4

engine_sha = hashlib.sha256(a.engine.read_bytes()).hexdigest()
assert engine_sha == '36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80'
class Arithmetic:
    def call(self, c, name):
        n = name.removeprefix('__aeabi_')
        if n in ('fadd', 'fsub', 'fmul', 'fdiv', 'dadd', 'dsub', 'dmul', 'ddiv'):
            k = n[0]; x, y = [c.get_float(k, i) for i in range(2)]
            c.put_float(x+y if n.endswith('add') else x-y if n.endswith('sub') else x*y if n.endswith('mul') else x/y, k)
        elif n in ('f2d', 'd2f'):
            c.put_float(c.get_float(n[0], 0), n[-1])
        elif n.startswith(('fcmp', 'dcmp')):
            x, y = [c.get_float(n[0], i) for i in range(2)]
            c.write_reg(0, int({'eq': x == y, 'lt': x < y, 'le': x <= y,
                              'gt': x > y, 'ge': x >= y, 'un': math.isnan(x) or math.isnan(y)}[n[4:]]))
        elif n in ('sin', 'cos', 'sinf', 'cosf', 'sqrtf'):
            k = 'f' if n.endswith('f') else 'd'; x = c.get_float(k, 0)
            c.put_float(math.sin(x) if n.startswith('sin') else math.cos(x) if n.startswith('cos') else math.sqrt(x), k)
        elif n in ('memcpy', 'memset'):
            dst, src, count = [c.reg(i) for i in range(3)]
            c.uc.mem_write(dst, bytes(c.uc.mem_read(src, count)) if n == 'memcpy' else bytes([src & 255])*count)
            c.write_reg(0, dst)
        else:
            raise RuntimeError('Unmodeled import: ' + name)
        c.uc.reg_write(c.pc_reg, c.uc.reg_read(c.lr_reg))

c = Cpu(a.engine, False, Arithmetic(), {'functions': []})
owner, visual, root, child, sibling, descendant, vt = [c.data + x for x in (0x1000, 0x2000, 0x3000, 0x4000, 0x5000, 0x6000, 0x7000)]
events = []; scene_calls = []; mode = None
def put(at, value): c.uc.mem_write(at, struct.pack('<I', value))
def back(): c.uc.reg_write(c.pc_reg, c.uc.reg_read(c.lr_reg))
def read_trs(at):
    return [*struct.unpack('<3f', c.uc.mem_read(at+0xac, 12)),
            *struct.unpack('<4f', c.uc.mem_read(at+0xb8, 16)),
            *struct.unpack('<3f', c.uc.mem_read(at+0xc8, 12))]
def write_trs(at, values):
    c.uc.mem_write(at+0xac, struct.pack('<3f', *values[:3]))
    c.uc.mem_write(at+0xb8, struct.pack('<4f', *values[3:7]))
    c.uc.mem_write(at+0xc8, struct.pack('<3f', *values[7:]))
def hook(uc, address, size, unused):
    if address == 0x38bf30 and mode == 'normalize' or address == 0x472b1c and mode == 'constructor_prefix':
        uc.reg_write(c.pc_reg, c.stop)
    elif address in (0x35c27c, 0x47211c, 0x470a54):
        events.append({'address': hex(address), 'object': hex(c.reg(0)), 'arg1': c.reg(1)})
        back()
    elif address == 0x50a564 and mode == 'constructor_prefix':
        c.write_reg(0, c.data+0x8000); back()
    elif address == 0x3596f8 and mode == 'constructor_prefix':
        sp = uc.reg_read(c.sp_reg)
        scene_calls.append({'xref_pointer': c.reg(2), 'register': c.reg(3),
                            'reset_from_file': struct.unpack('<I', uc.mem_read(sp, 4))[0]})
        c.write_reg(0, 0); back()
c.uc.hook_add(UC_HOOK_CODE, hook)
point = c.symbols['_ZTV13RootSceneNode'] + 28
assert bytes(c.uc.mem_read(0x35d890, 4)) == struct.pack('<I', 0xe283301c)
c.uc.mem_write(vt, bytes(c.uc.mem_read(point, 0xbc)))
for slot, target in ((0x90,0x5970bc),(0x94,0x5970c4),(0x98,0x5970ec),(0x9c,0x5970f4),(0xa4,0x59712c)):
    assert struct.unpack('<I', c.uc.mem_read(vt+slot, 4))[0] == target

# Prefix fixtures model only std::string's begin/end pointers read by this code.
# The actual AssetManager argument normalization and tail-call instructions run.
mode = 'constructor_prefix'
# AssetManager's World GOT slot resolves to a global whose +0x10 is used.
# Resolve the actual instruction's PC-relative GOT sequence without guessing.
got = (0x50a510+8+struct.unpack('<I', c.uc.mem_read(0x50a538,4))[0]) & 0xffffffff
offset = struct.unpack('<I', c.uc.mem_read(0x50a53c,4))[0]
global_at = struct.unpack('<I', c.uc.mem_read(got+offset,4))[0]
put(global_at+0x10, c.data+0x8000); put(c.data+0x801c, c.data+0x8100)
prefix_rows = []
for xref in ('', 'module_a', 'module_b'):
    c.uc.mem_write(visual, bytes(0x100))
    dae_at, xref_at = c.data+0x9000, c.data+0x9100
    for at, text in ((dae_at, 'data/test.bdae'), (xref_at, xref)):
        data = text.encode(); start = at+0x40
        c.uc.mem_write(start, data+b'\0'); put(at+0x10,start); put(at+0x14,start+len(data))
    # Stop at the prefix boundary before constructor side effects continue.
    # This is deliberately not a complete call or a stack-restoration check.
    c.uc.reg_write(c.sp_reg,c.stack+0xe000); c.uc.reg_write(c.lr_reg,c.stop)
    for i, value in enumerate((visual,owner,dae_at,xref_at)): c.write_reg(i,value)
    c.uc.emu_start(0x472a0c,c.stop,count=10000)
    assert c.uc.reg_read(c.pc_reg) == c.stop
    row = scene_calls[-1]
    assert row['register'] == 0 and row['reset_from_file'] == bool(xref)
    assert bool(row['xref_pointer']) == bool(xref)
    prefix_rows.append({'xref': xref, **row})

mode = 'normalize'
reset_guard_rows = []
for first_link in (0,4):
    c.uc.mem_write(root,bytes(0x400)); put(root,vt); put(root+0xf4,first_link)
    write_trs(root,[1,2,3,0,0,0,1,2,3,4])
    before=bytes(c.uc.mem_read(root,0x400)); events.clear()
    c.invoke('_ZN13RootSceneNode21ResetPositionFromFileEv',[root])
    assert bytes(c.uc.mem_read(root,0x400)) == before and events == []
    reset_guard_rows.append({'first_link':first_link,'wrapper_unchanged':True})
rng = random.Random(20261005)
cases = [([0,0,0],[0,0,0],[1,1,1]), ([1,-2,3],[10,20,30],[.01,.02,.03]),
         ([1,2,3],[90,-90,180],[0,-.00001,-2]), ([100,200,300],[360,720,-720],[100,100,100])]
for value in (scalar(0x38d1b716),scalar(0x38d1b717),scalar(0x38d1b718)):
    cases.append(([1,2,3],[10,20,30],[value,-value,value]))
for _ in range(128):
    cases.append(tuple([f32(rng.uniform(-limit,limit)) for _ in range(3)] for limit in (20000,720,4)))
rows = []; expected = []
identity = [0,0,0,0,0,0,1,1,1,1]
for position, rotation, scale in cases:
    for at in (owner,visual,root,child,sibling,descendant): c.uc.mem_write(at,bytes(0x400))
    put(root,vt); put(child,vt); put(sibling,vt); put(descendant,vt)
    put(root+0xf4,child+4); put(child+4,sibling+4); put(child+0xf4,descendant+4)
    put(visual+4,owner); put(visual+8,root)
    write_trs(root,identity)
    authored = [f32(rng.uniform(-1000,1000)) for _ in range(10)]
    write_trs(child,authored)
    for at in (sibling,descendant): write_trs(at,authored)
    untouched = {at: bytes(c.uc.mem_read(at,0x400)) for at in (sibling,descendant)}
    events.clear(); c.invoke('_ZN13RootSceneNode21ResetPositionFromFileEv',[root])
    assert read_trs(child) == identity and read_trs(root) == identity
    assert [e['object'] for e in events] == [hex(child)]
    for at, before in untouched.items(): assert bytes(c.uc.mem_read(at,0x400)) == before
    c.uc.mem_write(owner+0x160,struct.pack('<3f',*position))
    c.uc.mem_write(owner+0x16c,struct.pack('<3f',*rotation))
    c.uc.mem_write(owner+0x120,struct.pack('<3f',*scale))
    c.uc.reg_write(c.sp_reg,c.stack+0xe000); c.uc.reg_write(c.lr_reg,c.stop)
    c.uc.reg_write(UC_ARM_REG_R4,owner)
    c.uc.emu_start(0x38be84,c.stop,count=10000)
    assert c.uc.reg_read(c.pc_reg) == c.stop
    c.invoke('_ZN12VisualObject4SyncEv',[visual])
    assert read_trs(child) == identity
    for at, before in untouched.items(): assert bytes(c.uc.mem_read(at,0x400)) == before
    values = read_trs(root); expected.append(values)
    rows.append({'input': [position,rotation,scale], 'file_child_trs': authored,
                 'original_wrapper_trs': values, 'reset_child_trs': read_trs(child),
                 'sibling_and_descendant_unchanged': True})
command = [str(a.probe.resolve())]
if os.name == 'nt':
    absolute = a.probe.resolve()
    command = ['wsl','-d','Ubuntu','--','/mnt/'+absolute.drive[0].lower()+absolute.as_posix()[2:]]
inputs = ''.join(' '.join(str(v) for group in case for v in group)+'\n' for case in cases).encode()
run = subprocess.run(command,input=inputs,capture_output=True,check=True)
actual = [[f32(float(v)) for v in line.split()] for line in run.stdout.decode().splitlines()]
assert len(actual) == len(expected)
maximum = 0
for row, want, got in zip(rows, expected, actual):
    error = max(abs(x-y) for x,y in zip(want[3:7],got[3:7])); maximum=max(maximum,error)
    assert struct.pack('<3f3f',*want[:3],*want[7:]) == struct.pack('<3f3f',*got[:3],*got[7:]) and error <= 2e-6
    row.update(native_collapsed_root_trs=got, quaternion_max_error=error, matches=True)
root_dir = pathlib.Path(__file__).resolve().parents[1]
bound = ['fixed_map_v1.cpp','visual_transform_v1.cpp','tools/inspect_original_loader_vtables.py',
         'reports/original-loader-vtables.json']
for folder in ('loader-functions','visual-root-policy','visual-root-construction','visual-node-construction','visual-node-setters'):
    bound.extend(str(p.relative_to(root_dir)) for p in (root_dir/'reference'/folder).glob('*') if p.is_file())
report = {'validation':'PASS', 'scope':__doc__, 'engine_sha256':engine_sha,
          'script_sha256':hashlib.sha256(pathlib.Path(__file__).read_bytes()).hexdigest(),
          'cpu_helper_sha256':hashlib.sha256(pathlib.Path(sys.modules['differential'].__file__).read_bytes()).hexdigest(),
          'probe_sha256':hashlib.sha256(a.probe.read_bytes()).hexdigest(),
          'source_sha256':{p:hashlib.sha256((root_dir/p).read_bytes()).hexdigest() for p in sorted(bound)},
          'constructor_prefix_cases':prefix_rows, 'reset_guard_cases':reset_guard_rows, 'cases':rows,
          'maximum_quaternion_error':maximum, 'original_import_calls':c.import_calls,
          'full_collada_construction_verified':False,'full_loader_verified':False}
a.out.write_text(json.dumps(report,indent=2)+'\n')
print(json.dumps({'validation':'PASS','prefix_cases':len(prefix_rows),'placement_cases':len(rows),'maximum_quaternion_error':maximum}))
