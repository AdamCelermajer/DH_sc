"""Bind standalone Lua host/Android artifacts and actual commons inventory.

Host execution is a native source-backend audit, not original whole-VM parity.
Android artifacts are inspected builds; this tool performs no APK/device actions.
"""
import argparse
import hashlib
import json
from pathlib import Path
import struct
import subprocess

ROOT = Path(__file__).resolve().parents[3]
MODULE = ROOT / 'port/script-runtime'

def sha(path):
    return hashlib.sha256(Path(path).read_bytes()).hexdigest()

def rel(path):
    return Path(path).relative_to(ROOT).as_posix()

def write(path, value):
    path.parent.mkdir(parents=True, exist_ok=True)
    path.write_text(json.dumps(value, indent=2, sort_keys=True) + '\n', encoding='utf8')

def inventory(binary):
    """Independent field-width parser for a dump from the actual native compiler."""
    assert binary[:12] == bytes.fromhex('1b4c75615100010404040400')
    offset = 12
    reads, writes = set(), set()
    prototype_count = 0
    def take(n):
        nonlocal offset
        value = binary[offset:offset+n]
        assert len(value) == n
        offset += n
        return value
    def u32():
        return struct.unpack('<I', take(4))[0]
    def text():
        n = u32()
        if not n:
            return None
        data = take(n)
        assert data[-1] == 0
        return data[:-1].decode('utf8')
    def prototype():
        nonlocal prototype_count
        prototype_count += 1
        text(); take(8); take(4)
        code = [u32() for _ in range(u32())]
        constants = []
        for _ in range(u32()):
            kind = take(1)[0]
            if kind == 0: constants.append(None)
            elif kind == 1: constants.append(bool(take(1)[0]))
            elif kind == 3: constants.append(struct.unpack('<f', take(4))[0])
            elif kind == 4: constants.append(text())
            else: raise ValueError('bad constant kind')
        for instruction in code:
            opcode, bx = instruction & 63, instruction >> 14
            if opcode == 5: reads.add(constants[bx])  # Lua 5.1 GETGLOBAL
            elif opcode == 7: writes.add(constants[bx])  # SETGLOBAL
        for _ in range(u32()): prototype()
        take(u32() * 4)
        for _ in range(u32()): text(); take(8)
        for _ in range(u32()): text()
    prototype()
    assert offset == len(binary)
    return {'prototypes': prototype_count, 'global_reads': sorted(reads),
            'global_writes': sorted(writes), 'serialized_size_t': 4,
            'serialized_number': 4, 'bytes_consumed': offset}

def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('--host-build', default='/home/adampalace/dh2-lua514-build')
    args = ap.parse_args()
    scratch = ROOT / '.local-inputs/lua514-source'
    source = ROOT / 'port/level-world/reference/character-script-update/lua-inputs/ai-commons.luac'
    discovery = ROOT / 'port/level-world/reference/character-script-update/lua-inputs/discovery.json'
    original = ROOT / '.local-inputs/libDungeonHunter2.so'
    dump = scratch / 'ai-commons-native.luac'
    wsl_root = '/mnt/c/' + ROOT.as_posix()[3:]
    result = subprocess.run(['wsl', '-d', 'Ubuntu', '--', 'env',
        'ASAN_OPTIONS=detect_leaks=1:halt_on_error=1', 'UBSAN_OPTIONS=halt_on_error=1',
        args.host_build + '/lua514_audit', wsl_root + '/' + rel(source),
        wsl_root + '/' + rel(dump)], text=True, capture_output=True, check=True)
    assert not result.stderr.strip(), result.stderr
    audit = json.loads(result.stdout)
    assert audit['validation'] == 'PASS' and audit['mismatches'] == 0
    for filename in ('lua514_audit', 'libdh2_script_runtime.so'):
        subprocess.run(['wsl', '-d', 'Ubuntu', '--', 'cp',
            args.host_build + '/' + filename,
            wsl_root + '/' + rel(scratch / filename)], check=True)
    archive = scratch / 'lua-5.1.4.tar.gz'
    assert sha(archive) == 'b038e225eaf2a5b57c9bcc35cd13aa8c6c8288ef493d52970c9545074098af3a'
    assert sha(source) == '20d34968e9983e14c24223085ab40d55d47f9387dfdbbf3ff7e6b39aea91252c'
    source_files = sorted(p for p in MODULE.rglob('*') if p.is_file()
        and 'reports' not in p.parts and 'reference' not in p.parts and 'vendor' not in p.parts)
    bindings = {rel(p): sha(p) for p in source_files}
    commons = inventory(dump.read_bytes())
    assert commons['global_reads'] == ['StartTimer','StopTimer','Trace','evnet_name','type']
    assert commons['prototypes'] == 36 and len(commons['global_writes']) == 35
    commons.update({'validation':'PASS', 'actual_source_sha256':sha(source),
        'native_compiler_dump_sha256':sha(dump), 'native_compiler_dump':rel(dump),
        'source_is_plaintext': True, 'base_library_dependency':['type'],
        'required_game_bindings':['StartTimer','StopTimer','Trace'],
        'source_typo_dependency':['evnet_name'], 'game_bindings_installed':[],
        'load_requires_game_bindings':False, 'timer_callbacks_executed':False,
        'scope':'Native compiler bytecode inventory plus actual source execution; no timer/game service stubs.'})
    write(MODULE / 'reports/commons-inventory.json', commons)
    write(MODULE / 'reports/lua514-host-audit.json', {
        'validation':'PASS','host_audit':audit,'sanitizers':{'address':True,'undefined':True,'diagnostics':0},
        'source_sha256':bindings,'official_archive_sha256':sha(archive),
        'original_sha256':sha(original),'original_input_discovery_sha256':sha(discovery),
        'executable':rel(scratch/'lua514_audit'),'executable_sha256':sha(scratch/'lua514_audit'),
        'library':rel(scratch/'libdh2_script_runtime.so'),'library_sha256':sha(scratch/'libdh2_script_runtime.so'),
        'commons_inventory_sha256':sha(MODULE/'reports/commons-inventory.json'),
        'whole_original_vm_instruction_parity':False,'game_bindings_complete':False,
        'scope':'Source-built float32 Lua5.1.4 host audit and original-field-width reader; original header/arithmetic are instruction-audited, not a full VM differential.'})
    vendor = MODULE/'vendor/lua-5.1.4'
    changed = []
    for p in sorted((MODULE/'lua').glob('*')):
        if p.is_file() and p.read_bytes() != (vendor/'src'/p.name).read_bytes():
            changed.append({'path':rel(p),'sha256':sha(p),'official_sha256':sha(vendor/'src'/p.name)})
    assert {Path(p['path']).name for p in changed} == {'luaconf.h','lundump.c','ldump.c'}
    write(MODULE/'reference/source-provenance.json', {
        'official_url':'https://www.lua.org/ftp/lua-5.1.4.tar.gz',
        'official_index_url':'https://www.lua.org/ftp/','archive_sha256':sha(archive),
        'archive_bytes':archive.stat().st_size,'license_path':rel(vendor/'COPYRIGHT'),
        'license_sha256':sha(vendor/'COPYRIGHT'),
        'vendor_files_sha256':{p.relative_to(vendor).as_posix():sha(p) for p in sorted(vendor.rglob('*')) if p.is_file()},
        'adapted_files':changed,'original_sha256':sha(original),
        'original_capture_sha256':sha(MODULE/'reference/original-lua/reference/original-functions.asm'),
        'original_header_hex':'1b4c75615100010404040400',
        'deviations':['luaconf: lua_Number float; scanf %f; double strtod/floor/pow unchanged',
            'lundump: serialized size_t/string length uint32_t; header size_t width4',
            'ldump: serialized string length uint32_t with overflow rejection'],
        'native_pointer_bits':64,'native_lua_Integer_bits':64,
        'integer_api_original_parity_claimed':False})
    android = {}
    for abi,machine in [('arm64-v8a',183),('x86_64',62)]:
        library = scratch/f'android-{abi}/libdh2_script_runtime.so'
        data = library.read_bytes()
        assert data[:6] == b'\x7fELF\x02\x01' and struct.unpack_from('<H',data,18)[0] == machine
        phoff = struct.unpack_from('<Q',data,32)[0]
        entsize, count = struct.unpack_from('<HH',data,54)
        loads=[]
        for index in range(count):
            entry = phoff + index * entsize
            if struct.unpack_from('<I',data,entry)[0] == 1:
                alignment=struct.unpack_from('<Q',data,entry+48)[0]
                assert alignment>=16384
                loads.append(alignment)
        assert loads
        android[abi]={'library':rel(library),'library_sha256':sha(library),
            'elf_class':64,'machine':machine,'load_alignment':loads,'build':'PASS','executed':False}
    write(MODULE/'reports/lua514-android-build.json', {
        'validation':'PASS','source_sha256':bindings,'official_archive_sha256':sha(archive),
        'ndk':'29.0.14206865','android_api':24,'artifacts':android,
        'apk_modified':False,'device_executed':False,'instruction_differential':False})
    print(json.dumps({'host':audit,'android_builds':list(android),'required_game_bindings':commons['required_game_bindings']}))

if __name__ == '__main__':
    main()
