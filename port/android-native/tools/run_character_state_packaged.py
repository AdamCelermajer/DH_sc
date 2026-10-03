"""Replay original-derived state and blender gold against both actual APK ELFs.

No build, installation or Android execution occurs. The state/controller/body/
animation/AI/timer/FX services retain the original audit's controlled boundaries.
The blender check covers the isolated slot/fade/normalization exports, not
Playback blending, scene contributions or GPU poses. Original instructions
generated the existing gold; this run executes packaged ARM64 instructions.
"""
import argparse
import hashlib
import json
import math
import struct
import sys
import time
import zipfile
from collections import Counter
from pathlib import Path

REPO = Path(__file__).resolve().parents[3]
TESTS = REPO / 'port/level-world/tests'
REPORTS = REPO / 'port/level-world/reports'
LOCAL = REPO / '.local-inputs/character-state-packaged'
ENGINE = REPO / '.local-inputs/libDungeonHunter2.so'
WORLD_MEMBER = 'lib/arm64-v8a/libdh2_level_world.so'
ORIGINAL_SHA = '36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80'
SPECS = {
    'character-state': {
        'stem': 'character_state', 'gold': REPO / 'port/level-world/reference/character-state/state-reference.bin',
        'original_report': REPORTS / 'character-state-arm64-differential.json', 'comparisons': 3910,
        'exports': ('dh2_character_state_transition', 'dh2_character_state_event',
                    'dh2_character_state_update', 'dh2_character_attack_speed', 'dh2_character_state_is_idle'),
    },
    'animation-blender': {
        'stem': 'animation_blender', 'gold': REPO / '.local-inputs/animation-blender-discovery/reference.bin',
        'original_report': REPORTS / 'animation-blender-arm64-differential.json', 'comparisons': 2504,
        'exports': ('dh2_blender_begin', 'dh2_blender_update_weights', 'dh2_blender_normalize'),
    },
}


def sha(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()


def read_json(path):
    return json.loads(path.read_text(encoding='utf-8-sig'))


def write_json(path, value):
    path.parent.mkdir(parents=True, exist_ok=True)
    path.write_text(json.dumps(value, indent=2) + '\n', encoding='utf-8')


def relative(path):
    return path.resolve().relative_to(REPO).as_posix()


def require(condition, message):
    if not condition:
        raise AssertionError(message)


def native_cpu(library):
    """Use the existing caller-service ABI with actual ELF data relocations."""
    sys.path.insert(0, str(TESTS))
    from character_state_differential import Cpu as SourceCpu
    from elftools.elf.elffile import ELFFile
    from unicorn.arm64_const import UC_ARM64_REG_S0

    class PackagedCpu(SourceCpu):
        def __init__(self, path):
            super().__init__(path, True, {'functions': []})
            self.data_relocations = 0
            with path.open('rb') as stream:
                elf = ELFFile(stream)
                require(elf['e_machine'] == 'EM_AARCH64' and elf.elfclass == 64,
                        'APK member must be ELF64 AArch64')
                for section in elf.iter_sections():
                    if section['sh_type'] not in ('SHT_REL', 'SHT_RELA'):
                        continue
                    symbols = elf.get_section(section['sh_link'])
                    for relocation in section.iter_relocations():
                        if relocation['r_info_type'] not in (1025, 257):
                            continue
                        symbol = symbols.get_symbol(relocation['r_info_sym'])
                        addend = relocation['r_addend'] if section['sh_type'] == 'SHT_RELA' else 0
                        if symbol['st_shndx'] != 'SHN_UNDEF':
                            target = self.base + symbol['st_value'] + addend
                        else:
                            target = next((address for address, name in self.imports.items()
                                           if name == symbol.name), None)
                            if target is None:
                                target = self.extern + len(self.imports) * 16
                                require(target < self.callback, 'external relocation capacity')
                                self.imports[target] = symbol.name
                            target += addend
                        self.pointer(self.base + relocation['r_offset'], target)
                        self.data_relocations += 1

        def external(self, uc, address, size, unused):
            name = self.imports.get(address)
            if name in ('__memmove_chk', '__memset_chk'):
                destination, source, count, capacity = [self.reg(i) for i in range(4)]
                require(count <= capacity and count <= 0x2000000, 'checked libc memory capacity')
                if count:
                    data = bytes((source & 255,)) * count if name == '__memset_chk' else bytes(uc.mem_read(source, count))
                    uc.mem_write(destination, data)
                self.put(0, destination)
            elif name == 'fabsf':
                uc.reg_write(UC_ARM64_REG_S0, uc.reg_read(UC_ARM64_REG_S0) & 0x7fffffff)
            else:
                return super().external(uc, address, size, unused)
            self.import_calls[name] = self.import_calls.get(name, 0) + 1
            uc.reg_write(self.pc, uc.reg_read(self.lr))

    return PackagedCpu(library)


def replay_state(cpu, gold):
    from character_state_differential import equal
    blob = gold.read_bytes()
    magic, count = struct.unpack_from('<2I', blob)
    require(magic == 0x31545343 and count == 3910, 'state gold header/count')
    state, facts, services, properties, output = [cpu.data + value for value in (0x1000, 0x2000, 0x3000, 0x4000, 0x5000)]
    context = 0xabcdef0123456789
    callbacks, mode = [], 0

    def callback(uc, address, size, unused):
        require(cpu.reg(0) == context and cpu.reg(1) == state, '64-bit callback context/state')
        request = bytes(uc.mem_read(cpu.reg(2), 32))
        kind, a, b, c, value, reserved, identity = struct.unpack('<6IQ', request)
        require(reserved == 0, 'Request reserved word')
        callbacks.append(request)
        if kind == 1:
            uc.mem_write(state + 32, bytes(4))
            if mode == 4:
                uc.mem_write(facts + 16, bytes(12))
        elif kind == 4:
            uc.mem_write(state + 52, struct.pack('<I', a))
            if mode == 2:
                uc.mem_write(state + 44, bytes(4))
        elif kind == 6 and mode == 1:
            uc.mem_write(state + 32, bytes(4))
        elif kind == 18:
            uc.mem_write(state + 44, bytes(4))
        uc.reg_write(cpu.pc, uc.reg_read(cpu.lr))

    cpu.imports[cpu.callback + 32] = 'state_callback'
    cpu.state_callback = callback
    cpu.uc.mem_write(services, struct.pack('<QQ', context, cpu.callback + 32))
    cursor, requests = 8, 0
    counts = Counter()
    for index in range(count):
        require(cursor + 240 <= len(blob), f'truncated state record {index}')
        operation, a, b, request_count, expected_result, mode, payload = struct.unpack_from('<6IQ', blob, cursor)
        cursor += 32
        before, inputs, expected = blob[cursor:cursor + 56], blob[cursor + 56:cursor + 152], blob[cursor + 152:cursor + 208]
        cursor += 208
        require(cursor + request_count * 32 <= len(blob), f'truncated state requests {index}')
        expected_requests = [blob[cursor + i * 32:cursor + (i + 1) * 32] for i in range(request_count)]
        cursor += request_count * 32
        callbacks.clear()
        cpu.uc.mem_write(state, before)
        cpu.uc.mem_write(facts, inputs)
        if operation == 0:
            result = cpu.invoke('dh2_character_state_transition', [state, facts, a, b, payload, services])
        elif operation == 1:
            result = cpu.invoke('dh2_character_state_event', [state, facts, a, payload, services])
        elif operation == 2:
            result = cpu.invoke('dh2_character_state_update', [state, facts, a, services])
        elif operation == 3:
            cpu.uc.mem_write(properties, bytes(224 * 4))
            cpu.uc.mem_write(properties + 48 * 4, before[:4])
            result = cpu.invoke('dh2_character_attack_speed', [output, properties])
            cpu.uc.mem_write(state + 20, bytes(cpu.uc.mem_read(output, 4)))
        elif operation == 4:
            result = cpu.invoke('dh2_character_state_is_idle', [struct.unpack_from('<I', before)[0], a])
        else:
            raise AssertionError(f'unknown state gold operation {operation}')
        require(result == expected_result, f'state return mismatch {index}: {result} != {expected_result}')
        require(equal(expected, bytes(cpu.uc.mem_read(state, 56))), f'state projection mismatch {index}')
        require(len(callbacks) == request_count and all(equal(e, v, 4) for e, v in zip(expected_requests, callbacks)),
                f'ordered state service mismatch {index}')
        counts[operation] += 1
        requests += request_count
    require(cursor == len(blob) and requests == 8773, 'state corpus size/request total')
    # These checks protect the native projection ABI; original trusted objects
    # do not expose this validation contract. They do not change gold counts.
    rejects = 0
    base = struct.pack('<i13I', 3, *([0] * 13))
    valid_facts = bytes(96)
    missing = services + 32
    cpu.uc.mem_write(missing, struct.pack('<QQ', context, 0))

    def reject(before=base, inputs=valid_facts, service_pointer=services):
        nonlocal rejects
        cpu.uc.mem_write(state, before)
        cpu.uc.mem_write(facts, inputs)
        callbacks.clear()
        require(cpu.invoke('dh2_character_state_event', [state, facts, 0xc351, 0, service_pointer]) & 0xffffffff == 0xffffffff,
                'malformed state ABI input accepted')
        require(bytes(cpu.uc.mem_read(state, 56)) == before and not callbacks, 'malformed state ABI mutation')
        rejects += 1

    mode = 0
    reject(service_pointer=0)
    reject(service_pointer=missing)
    for offset, value in ((80, 1), (0, 2), (84, 2), (88, 2), (92, 2)):
        inputs = bytearray(valid_facts)
        struct.pack_into('<I', inputs, offset, value)
        reject(inputs=bytes(inputs))
    for offset, value in ((0, 7), (44, 2), (12, 3), (24, 256), (28, 256), (32, 256), (36, 256), (40, 256)):
        malformed = bytearray(base)
        struct.pack_into('<I', malformed, offset, value)
        reject(before=bytes(malformed))
    for export, arguments in (('dh2_character_state_event', [0, facts, 0x22, 0, services]),
                              ('dh2_character_state_update', [state, 0, 16, services])):
        require(cpu.invoke(export, arguments) & 0xffffffff == 0xffffffff, 'null state ABI argument')
        rejects += 1
    cpu.uc.mem_write(state, base)
    cpu.uc.mem_write(facts, valid_facts)
    callbacks.clear()
    require(cpu.invoke('dh2_character_state_transition', [state, facts, 18, 0, 0, services]) & 0xffffffff == 0xffffffff,
            'unsupported direct transition')
    require(bytes(cpu.uc.mem_read(state, 56)) == base and not callbacks, 'unsupported transition mutation')
    rejects += 1
    sentinel = struct.pack('<f', 17.)
    cpu.uc.mem_write(output, sentinel)
    require(cpu.invoke('dh2_character_attack_speed', [output, 0]) & 0xffffffff == 0xffffffff and bytes(cpu.uc.mem_read(output, 4)) == sentinel,
            'null properties mutation')
    rejects += 1
    require(cpu.invoke('dh2_character_attack_speed', [0, properties]) & 0xffffffff == 0xffffffff, 'null speed output')
    rejects += 1
    require(cpu.invoke('dh2_character_state_is_idle', [3, 2]) & 0xffffffff == 0xffffffff, 'malformed idle mode')
    rejects += 1
    require(rejects == 21, 'state rejection count')
    return {'comparisons': count, 'ordered_service_requests': requests, 'operation_counts': dict(counts),
            'malformed_no_mutation_cases': rejects,
            'callback_context_and_target_identity_above_4gib': True,
            'finite_scalar_words_and_all_nonfloat_words_exact': True,
            'arithmetic_nan_comparison': 'cached speed/scalar NaN class only; all other words exact',
            'mismatches': 0, 'scope': 'Packaged coordinator exports replay original-derived bounded state/service gold. Controller/body/animation/AI/timer/FX backends are controlled fixtures; no full FSM or frame parity claim.'}


def replay_blender(cpu, gold):
    blob = gold.read_bytes()
    magic, count = struct.unpack_from('<2I', blob)
    require(magic == 0x31414c42 and count == 2504 and len(blob) == 8 + count * 72, 'blender gold header/count/size')
    state, counts = cpu.data + 0x1000, Counter()
    for index in range(count):
        offset = 8 + index * 72
        operation, argument = struct.unpack_from('<2I', blob, offset)
        before, expected = blob[offset + 8:offset + 40], blob[offset + 40:offset + 72]
        require(operation in (0, 1, 2, 3), 'blender operation')
        export = 'dh2_blender_begin' if operation == 0 else 'dh2_blender_normalize' if operation == 2 else 'dh2_blender_update_weights'
        cpu.uc.mem_write(state, before)
        require(cpu.invoke(export, [state, argument]) == 0, f'blender return mismatch {index}')
        actual = bytes(cpu.uc.mem_read(state, 32))
        for field, (left, right) in enumerate(zip(struct.unpack('<8I', expected), struct.unpack('<8I', actual))):
            if left == right:
                continue
            acceptable_nan = field in (4, 6, 7) and all(math.isnan(struct.unpack('<f', struct.pack('<I', word))[0]) for word in (left, right))
            require(acceptable_nan, f'blender word mismatch {index} field {field}: {left:x} != {right:x}')
        counts[operation] += 1
    rejects = 0
    valid = struct.pack('<2I2ifI2f', 0, 1, 100, 100, .01, 0, 1., 0.)
    for export in SPECS['animation-blender']['exports']:
        require(cpu.invoke(export, [0, 16]) == 1, 'blender null rejection')
        rejects += 1
        for offset, value in ((0, 2), (4, 2), (16, 0x7fc01234), (24, 0x7f800000), (28, 0xff800000)):
            malformed = bytearray(valid)
            struct.pack_into('<I', malformed, offset, value)
            cpu.uc.mem_write(state, bytes(malformed))
            require(cpu.invoke(export, [state, 16]) == 1 and bytes(cpu.uc.mem_read(state, 32)) == bytes(malformed), 'blender atomic rejection')
            rejects += 1
    return {'comparisons': count, 'operation_counts': dict(counts), 'atomic_rejection_checks': rejects,
            'mismatches': 0, 'scope': 'Isolated actual packaged blender begin, fade-prefix weights and normalize exports replay original-derived gold. No two-slot Playback, scene contribution, clone ownership or GPU parity claim.'}


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--studio', type=Path, required=True)
    parser.add_argument('--output', type=Path, default=REPORTS / 'character-combat-kernel-validation.json')
    args = parser.parse_args()
    started = time.monotonic()
    script_hash = sha(Path(__file__))
    require(sha(ENGINE) == ORIGINAL_SHA, 'original ELF identity')
    LOCAL.mkdir(parents=True, exist_ok=True)
    apks = {'packaged': REPO / 'port/android-native/app/build/outputs/apk/debug/app-debug.apk',
            'studio': args.studio / 'app/build/outputs/apk/debug/app-debug.apk'}
    apk_hashes = {tag: sha(path) for tag, path in apks.items()}
    sources = {}
    for spec in SPECS.values():
        for suffix in ('.hpp', '.cpp', '_differential.py'):
            path = REPO / 'port/level-world' / (('tests/' if suffix == '_differential.py' else '') + spec['stem'] + suffix)
            sources[relative(path)] = sha(path)
        path = TESTS / (spec['stem'] + '.cpp')
        sources[relative(path)] = sha(path)
    studio_source = args.studio / 'reconstruction-source'
    if not studio_source.exists():
        studio_source = args.studio / '../..'
    for spec in SPECS.values():
        for suffix in ('.hpp', '.cpp'):
            key = 'port/level-world/' + spec['stem'] + suffix
            require(sha(studio_source / key) == sources[key], 'Studio source mismatch: ' + key)
    gold_bindings = {}
    for module, spec in SPECS.items():
        original = read_json(spec['original_report'])
        require(original['original_sha256'] == ORIGINAL_SHA and original['comparisons'] == spec['comparisons'] and original['mismatches'] == 0, module + ' original audit identity/count')
        require(sha(spec['gold']) == original['reference_sha256'], module + ' original gold identity')
        if module == 'character-state':
            require(original['kernel_source_sha256'] == sources['port/level-world/character_state.cpp'], 'state source changed since original audit')
        gold_bindings[module] = {'reference': relative(spec['gold']), 'reference_sha256': sha(spec['gold']),
                                 'original_report': relative(spec['original_report']), 'original_report_sha256': sha(spec['original_report'])}
    libraries, verified = {}, {}
    for tag, apk in apks.items():
        path = LOCAL / (tag + '-world-arm64.so')
        with zipfile.ZipFile(apk) as archive:
            raw = archive.read(WORLD_MEMBER)
        library_hash = hashlib.sha256(raw).hexdigest()
        path.write_bytes(raw)
        libraries[tag] = {'apk_member': WORLD_MEMBER, 'sha256': sha(path), 'path': relative(path)}
        verified[tag] = {}
        for module, spec in SPECS.items():
            cpu = native_cpu(path)
            require(all(name in cpu.symbols for name in spec['exports']), tag + ' missing ' + module + ' export')
            leaf = replay_state(cpu, spec['gold']) if module == 'character-state' else replay_blender(cpu, spec['gold'])
            require(sha(path) == library_hash, tag + ' extracted library changed during replay')
            leaf.update(apk_sha256=apk_hashes[tag], apk_member=WORLD_MEMBER, arm64_library_sha256=sha(path),
                        library_sha256=sha(path), original_sha256=ORIGINAL_SHA,
                        reference_sha256=gold_bindings[module]['reference_sha256'],
                        original_instruction_evidence=gold_bindings[module],
                        source_sha256={key: value for key, value in sources.items() if spec['stem'] in key},
                        native_import_calls=cpu.import_calls, defined_exports=list(spec['exports']),
                        global_data_relocations_resolved=cpu.data_relocations, actual_packaged_library_executed=True,
                        original_instructions_executed_this_run=False, original_gold_replayed=True,
                        production_rebuilt=False, emulator_tested=False, physical_arm64_tested=False)
            report_path = REPORTS / f'character-combat-{tag}-{module}-arm64-differential.json'
            write_json(report_path, leaf)
            verified[tag][module] = {'report': relative(report_path), 'report_sha256': sha(report_path),
                                     'library_sha256': sha(path), 'reference_sha256': gold_bindings[module]['reference_sha256'],
                                     'comparisons': leaf['comparisons'], 'mismatches': 0, 'differential': leaf}
            print(f'{tag} {module}: {leaf["comparisons"]} original-derived cases pass', flush=True)
    require(apk_hashes == {tag: sha(path) for tag, path in apks.items()}, 'APK changed during audit')
    require(sources == {key: sha(REPO / key) for key in sources}, 'kernel/test source changed during audit')
    for spec in SPECS.values():
        for suffix in ('.hpp', '.cpp'):
            key = 'port/level-world/' + spec['stem'] + suffix
            require(sha(studio_source / key) == sources[key], 'Studio source changed during audit: ' + key)
    require(sha(Path(__file__)) == script_hash, 'harness source changed during audit')
    for module, spec in SPECS.items():
        require(sha(spec['gold']) == gold_bindings[module]['reference_sha256'] and sha(spec['original_report']) == gold_bindings[module]['original_report_sha256'], module + ' evidence changed during audit')
    aggregate = {'validation': 'PASS', 'apk_sha256': apk_hashes, 'libraries': libraries,
                 'original_sha256': ORIGINAL_SHA, 'packaged_differentials': verified,
                 'source_sha256': sources, 'studio_kernel_sources_match': True,
                 'script_sha256': script_hash, 'gold_bindings': gold_bindings,
                 'cases_per_apk': 6414, 'state_ordered_service_requests_per_apk': 8773,
                 'each_actual_packaged_library_executed': True, 'production_rebuilt': False,
                 'emulator_tested': False, 'physical_arm64_tested': False,
                 'full_original_frame_parity_claimed': False, 'scope': __doc__,
                 'elapsed_seconds': round(time.monotonic() - started, 3)}
    write_json(args.output, aggregate)
    print(json.dumps({'validation': 'PASS', 'cases_per_apk': 6414, 'state_ordered_service_requests_per_apk': 8773, 'report': str(args.output)}))


if __name__ == '__main__':
    main()
