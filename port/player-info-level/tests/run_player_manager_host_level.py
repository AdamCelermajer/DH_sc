"""Run host cases and focused original ARM checks for host-level selection."""
import argparse
import hashlib
import json
from pathlib import Path
import struct
import subprocess

from elftools.elf.elffile import ELFFile
from unicorn import Uc, UC_ARCH_ARM, UC_MODE_ARM, UC_HOOK_CODE, UC_HOOK_MEM_READ_UNMAPPED
from unicorn.arm_const import (UC_ARM_REG_R0, UC_ARM_REG_R1, UC_ARM_REG_R2,
                               UC_ARM_REG_R4, UC_ARM_REG_R5, UC_ARM_REG_R7,
                               UC_ARM_REG_R9, UC_ARM_REG_PC,
                               UC_ARM_REG_LR, UC_ARM_REG_SP)

MODULE = Path(__file__).resolve().parents[1]
ROOT = MODULE.parents[1]
MANIFEST = MODULE / 'reference/player-manager-host-level/original-functions.json'
ORIGINAL_SHA = '36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80'
ONLINE = 0x7fd794
PROPS_GET_INT = 0x3df6e0
SET_CHARACTER_LEVEL = 0x370e48
HOSTING_PLAYER = 0x36e09c
PLAYER_BY_ID = 0x36dfb0
MANAGE_CHARACTERS = 0x37280c
EXECUTED_FUNCTIONS = {HOSTING_PLAYER, PLAYER_BY_ID, MANAGE_CHARACTERS}
PLAYER_INFO_C1 = 0x37418c
SET_CHANGED = 0x814f84
PLAYER_INFO_INIT_START = 0x3742d8
PLAYER_INFO_INIT_END = 0x37434c
RECONCILE_START = 0x372cf8
RECONCILE_STOP = 0x372d30
MANAGER = 0x10001000
NODE = 0x10002000
ONLINE_OBJECT = 0x10008000
STACK_TOP = 0x20008000
STOP = 0x30000000


def original_code(path):
    raw = path.read_bytes()
    assert hashlib.sha256(raw).hexdigest() == ORIGINAL_SHA
    code = {}
    with path.open('rb') as stream:
        elf = ELFFile(stream)
        segments = [s for s in elf.iter_segments() if s['p_type'] == 'PT_LOAD']
        symbols = {s.name: s for s in elf.get_section_by_name('.dynsym').iter_symbols()}
        for item in json.loads(MANIFEST.read_text(encoding='utf-8'))['functions']:
            address = int(item['elf_address'], 0)
            size = item['size']
            symbol = symbols[item['original_symbol']]
            assert (int(symbol['st_value']), int(symbol['st_size'])) == (address, size)
            segment = next(s for s in segments if s['p_vaddr'] <= address and
                           address + size <= s['p_vaddr'] + s['p_filesz'])
            offset = segment['p_offset'] + address - segment['p_vaddr']
            body = raw[offset:offset + size]
            assert hashlib.sha256(body).hexdigest() == item['sha256']
            code[address] = body
        for evidence in json.loads(MANIFEST.read_text(encoding='utf-8'))['supporting_evidence']:
            if evidence.get('size') is None:
                continue
            address = int(evidence['elf_address'], 0)
            symbol = symbols[evidence['original_symbol']]
            size = evidence['size']
            assert (int(symbol['st_value']), int(symbol['st_size'])) == (address, size)
            segment = next(s for s in segments if s['p_vaddr'] <= address and
                           address + size <= s['p_vaddr'] + s['p_filesz'])
            offset = segment['p_offset'] + address - segment['p_vaddr']
            body = raw[offset:offset + size]
            assert hashlib.sha256(body).hexdigest() == evidence['sha256']
            code[address] = body
        symbols = {s.name: s for s in elf.get_section_by_name('.dynsym').iter_symbols()}
        assert int(symbols['_Z9GetOnlinev']['st_value']) == ONLINE
    return code


def put32(uc, address, value):
    uc.mem_write(address, struct.pack('<I', value & 0xffffffff))


def get32(uc, address):
    return struct.unpack('<I', uc.mem_read(address, 4))[0]


def signed32(value):
    value &= 0xffffffff
    return value - 0x100000000 if value & 0x80000000 else value


def base_machine(code):
    uc = Uc(UC_ARCH_ARM, UC_MODE_ARM)
    for base, size in ((0x36d000, 0x3000), (0x370000, 0x1000),
                       (0x372000, 0x3000), (0x3df000, 0x1000),
                       (0x7fd000, 0x1000), (0x10000000, 0x20000),
                       (0x20000000, 0x10000), (0x30000000, 0x1000)):
        uc.mem_map(base, size)
    for address in EXECUTED_FUNCTIONS:
        uc.mem_write(address, code[address])
    uc.reg_write(UC_ARM_REG_SP, STACK_TOP)
    return uc


def run_player_lookup(code, function, internal_id=0, tree_key=None):
    uc = base_machine(code)
    put32(uc, ONLINE_OBJECT + 5, 0)
    put32(uc, MANAGER + 0x694, NODE if tree_key is not None else 0)
    put32(uc, NODE + 8, 0)
    put32(uc, NODE + 12, 0)
    put32(uc, NODE + 16, 0 if tree_key is None else tree_key)
    put32(uc, MANAGER + 8 + 0x330, 41)
    put32(uc, NODE + 0x18 + 0x330, 73)
    online_calls = []

    def hook(machine, address, size, context):
        if address == ONLINE:
            online_calls.append(address)
            machine.reg_write(UC_ARM_REG_R0, ONLINE_OBJECT)
            machine.reg_write(UC_ARM_REG_PC, machine.reg_read(UC_ARM_REG_LR))
        elif address == STOP:
            machine.emu_stop()

    uc.hook_add(UC_HOOK_CODE, hook)
    uc.hook_add(UC_HOOK_MEM_READ_UNMAPPED,
                lambda machine, access, address, size, value, context: False)
    if function == HOSTING_PLAYER:
        uc.reg_write(UC_ARM_REG_R0, MANAGER)
        uc.reg_write(UC_ARM_REG_LR, STOP)
        uc.emu_start(HOSTING_PLAYER, STOP + 4, count=1000)
    else:
        uc.reg_write(UC_ARM_REG_R0, MANAGER)
        uc.reg_write(UC_ARM_REG_R1, internal_id & 0xffffffff)
        uc.reg_write(UC_ARM_REG_R2, 0)
        uc.reg_write(UC_ARM_REG_LR, STOP)
        uc.emu_start(PLAYER_BY_ID, STOP + 4, count=1000)
    assert uc.reg_read(UC_ARM_REG_PC) == STOP
    pointer = uc.reg_read(UC_ARM_REG_R0)
    return {'selected_identity': pointer,
            'selected_level_330': get32(uc, pointer + 0x330),
            'online_calls': len(online_calls)}


def run_original_reconcile(code, initial_member, properties):
    uc = base_machine(code)
    player = 0x10001000
    props = 0x10005000
    put32(uc, player + 0x330, initial_member)
    seen = {'gets': [], 'setters': []}
    values = iter(properties)

    def hook(machine, address, size, context):
        if address == PROPS_GET_INT:
            args = (machine.reg_read(UC_ARM_REG_R0),
                    machine.reg_read(UC_ARM_REG_R1),
                    machine.reg_read(UC_ARM_REG_R2))
            seen['gets'].append(args)
            assert args == (props, 19, 0), args
            machine.reg_write(UC_ARM_REG_R0, next(values))
            machine.reg_write(UC_ARM_REG_PC, machine.reg_read(UC_ARM_REG_LR))
        elif address == SET_CHARACTER_LEVEL:
            args = (machine.reg_read(UC_ARM_REG_R0), machine.reg_read(UC_ARM_REG_R1))
            seen['setters'].append(args)
            assert args[0] == player, args
            put32(machine, player + 0x330, args[1])
            machine.reg_write(UC_ARM_REG_R0, 0)
            machine.reg_write(UC_ARM_REG_PC, machine.reg_read(UC_ARM_REG_LR))
        elif address == RECONCILE_STOP:
            machine.emu_stop()

    uc.hook_add(UC_HOOK_CODE, hook)
    uc.hook_add(UC_HOOK_MEM_READ_UNMAPPED,
                lambda machine, access, address, size, value, context: False)
    uc.reg_write(UC_ARM_REG_R4, player)
    uc.reg_write(UC_ARM_REG_R9, props)
    uc.reg_write(UC_ARM_REG_LR, STOP)
    uc.emu_start(RECONCILE_START, RECONCILE_STOP + 4, count=256)
    assert uc.reg_read(UC_ARM_REG_PC) == RECONCILE_STOP
    return {'property_reads': len(seen['gets']),
            'setter_calls': len(seen['setters']),
            'setter_argument': signed32(seen['setters'][0][1]) if seen['setters'] else 0,
            'level_after': signed32(get32(uc, player + 0x330)),
            'get_arguments': [list(x) for x in seen['gets']],
            'setter_arguments': [list(x) for x in seen['setters']]}


def run_player_info_level_initializer(code, initial_member):
    uc = Uc(UC_ARCH_ARM, UC_MODE_ARM)
    for base, size in ((0x374000, 0x1000), (0x814000, 0x1000),
                       (0x10000000, 0x10000), (0x20000000, 0x10000)):
        uc.mem_map(base, size)
    uc.mem_write(PLAYER_INFO_C1, code[PLAYER_INFO_C1])
    # The original function's PC-relative literal selects a global vtable
    # table. Retarget that one load to a harness-owned table; the member-init
    # instructions and conditional SetChanged edge remain original ARM code.
    put32(uc, 0x374b0c, 0x24)
    player = 0x10001000
    table = 0x10004000
    stack = 0x20008000
    put32(uc, player + 0x330, initial_member)
    put32(uc, table + 0x20, 0x12345678)
    put32(uc, table + 0x24, 0x20005000)
    changed = []

    def hook(machine, address, size, context):
        if address == SET_CHANGED:
            changed.append(machine.reg_read(UC_ARM_REG_R0))
            machine.reg_write(UC_ARM_REG_PC, machine.reg_read(UC_ARM_REG_LR))
        elif address == PLAYER_INFO_INIT_END:
            machine.emu_stop()

    uc.hook_add(UC_HOOK_CODE, hook)
    uc.reg_write(UC_ARM_REG_R4, player)
    uc.reg_write(UC_ARM_REG_R5, table)
    uc.reg_write(UC_ARM_REG_R7, 0x20)
    uc.reg_write(UC_ARM_REG_SP, stack)
    uc.emu_start(PLAYER_INFO_INIT_START, PLAYER_INFO_INIT_END + 4, count=128)
    assert uc.reg_read(UC_ARM_REG_PC) == PLAYER_INFO_INIT_END
    return {'input_value': initial_member,
            'value_after': signed32(get32(uc, player + 0x330)),
            'set_changed_calls': len(changed),
            'set_changed_member': changed[0] if changed else 0}


def run_host(executable, *arguments):
    return json.loads(subprocess.check_output([str(executable), *map(str, arguments)], text=True))


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--compiler', default='g++')
    parser.add_argument('--original-elf', type=Path,
                        default=ROOT.parent / 'test_strategy/libDungeonHunter2.so')
    parser.add_argument('--output', type=Path,
                        default=MODULE / 'build/player-manager-host-level/host.exe')
    parser.add_argument('--report', type=Path,
                        default=MODULE / 'build/player-manager-host-level/validation.json')
    args = parser.parse_args()
    elf_path = args.original_elf.resolve()
    code = original_code(elf_path)
    args.output.parent.mkdir(parents=True, exist_ok=True)
    sources = [MODULE / 'player_manager_host_level.cpp',
               MODULE / 'tests/player_manager_host_level.cpp']
    command = [args.compiler, '-std=c++17', '-O2', '-Wall', '-Wextra', '-Werror', '-pedantic',
               '-I', str(MODULE), *map(str, sources), '-o', str(args.output.resolve())]
    subprocess.run(command, cwd=ROOT, check=True, capture_output=True, text=True)
    executable = args.output.resolve()
    host = json.loads(subprocess.check_output([str(executable)], text=True))
    assert host == {'validation': 'PASS', 'host_cases': 12}

    lookup_scenarios = [
        {'function': 'hosting-id-zero-tree-hit', 'function_address': HOSTING_PLAYER,
         'id': 0, 'tree_key': 0, 'expected_pointer': NODE + 0x18, 'online_calls': 2},
        {'function': 'lookup-negative-id-manager-plus-8', 'function_address': PLAYER_BY_ID,
         'id': -1, 'tree_key': 7, 'expected_pointer': MANAGER + 8, 'online_calls': 0},
        {'function': 'lookup-offline-tree-hit', 'function_address': PLAYER_BY_ID,
         'id': 7, 'tree_key': 7, 'expected_pointer': NODE + 0x18, 'online_calls': 1},
        {'function': 'lookup-offline-tree-miss', 'function_address': PLAYER_BY_ID,
         'id': 8, 'tree_key': 7, 'expected_pointer': MANAGER + 8, 'online_calls': 1},
    ]
    lookup_results = []
    for case in lookup_scenarios:
        actual = run_player_lookup(code, case['function_address'], case['id'], case['tree_key'])
        expected = {'selected_identity': case['expected_pointer'],
                    'selected_level_330': 73 if case['expected_pointer'] == NODE + 0x18 else 41,
                    'online_calls': case['online_calls']}
        assert actual == expected, (case, actual, expected)
        lookup_results.append({'name': case['function'], 'result': actual, 'matched': True})

    reconcile_scenarios = [
        (5, [5]),       # same value: one getter, no setter
        (5, [7, 9]),    # mismatch: fresh second value is the setter argument
        (5, [1, 5]),    # second value equals old member; still dispatches setter
        (-4, [-1, -8]),
    ]
    reconcile_results = []
    for initial, properties in reconcile_scenarios:
        original = run_original_reconcile(code, initial, properties)
        host_args = [initial, properties[0], properties[1] if len(properties) > 1 else 0]
        host_result = run_host(executable, '--reconcile', *host_args)
        expected = {
            'status': 0,
            'property_reads': original['property_reads'],
            'setter_calls': original['setter_calls'],
            'level_before': initial,
            'first_property': properties[0],
            'setter_argument': original['setter_argument'],
            'level_after': original['level_after'],
        }
        assert host_result == expected, (initial, properties, original, host_result, expected)
        reconcile_results.append({'initial_member': initial, 'property_values': properties,
                                  'original_arm': original, 'host_kernel': host_result,
                                  'matched': True})

    initializer_results = []
    for initial in (0, 0x12345678):
        actual = run_player_info_level_initializer(code, initial)
        expected = {'input_value': initial, 'value_after': 0,
                    'set_changed_calls': 0 if initial == 0 else 1,
                    'set_changed_member': 0 if initial == 0 else 0x10001310}
        assert actual == expected, (initial, actual, expected)
        initializer_results.append({'original_arm': actual, 'matched': True})

    tracked = sources + [MODULE / 'player_manager_host_level.hpp', Path(__file__).resolve(),
                         MANIFEST, MANIFEST.with_name('NOTES.md')]
    report = {
        'validation': 'PASS',
        'host_cases': host['host_cases'],
        'original_arm_lookup_scenarios': len(lookup_results),
        'original_arm_reconcile_scenarios': len(reconcile_results),
        'original_arm_constructor_scenarios': len(initializer_results),
        'mismatches': 0,
        'original_sha256': ORIGINAL_SHA,
        'scope': ('Executes original offline GetHostingPlayer/GetPlayerByInternalID paths and '
                  'the original _ManageCharacters instruction slice 0x372cf8..0x372d30. '
                  'GetOnline, PROPS_GetInt, and SetCharacterLevel are explicit source-boundary '
                  'hooks; the actual GetInt result is passed as a plain signed integer.'),
        'lookup_results': lookup_results,
        'reconcile_results': reconcile_results,
        'constructor_initializer_results': initializer_results,
        'compiler_command': command,
        'source_sha256': {f.relative_to(ROOT).as_posix(): hashlib.sha256(f.read_bytes()).hexdigest()
                          for f in tracked},
        'native_wired': False,
    }
    args.report.parent.mkdir(parents=True, exist_ok=True)
    args.report.write_text(json.dumps(report, indent=2) + '\n', encoding='utf-8')
    print(json.dumps({key: report[key] for key in (
        'validation', 'host_cases', 'original_arm_lookup_scenarios',
        'original_arm_reconcile_scenarios', 'original_arm_constructor_scenarios',
        'mismatches')}))


if __name__ == '__main__':
    main()
