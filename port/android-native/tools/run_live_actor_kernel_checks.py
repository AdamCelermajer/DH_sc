"""Audit current packaged actor kernels without rebuilding or running Android.

Original ARM32 instructions are a desktop oracle only. Existing fixture/service
boundaries remain those documented by each differential. Packaged scene math
executes as ARM64 instructions across an explicit ABI/memory bridge.
"""
import argparse
import hashlib
import importlib.util
import json
import os
from pathlib import Path
import subprocess
import sys
import time
import zipfile

REPO = Path(__file__).resolve().parents[3]
TESTS = REPO / 'port/level-world/tests'
REPORTS = REPO / 'port/level-world/reports'
LOCAL = REPO / '.local-inputs/live-actor-kernel-checks'
ENGINE = REPO / '.local-inputs/libDungeonHunter2.so'
SAMPLES = REPO / '.local-inputs/visual-motion-asset-samples.bin'
MODULES = ('actor_rotation', 'decor_scene', 'character_scene', 'visual_motion')


def sha(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()


def worker(module_name, math_library, arguments):
    """Preserve source fixtures; resolve the real packaged scene/math imports."""
    sys.path.insert(0, str(TESTS))
    original_write = Path.write_text

    def write(path, content, *args, **kwargs):
        if path.name in ('crypt-placement-fixture.json', 'prince-rest-fixture.json'):
            path = LOCAL / (module_name + '-' + path.name)
        return original_write(path, content, *args, **kwargs)

    Path.write_text = write
    # Standalone oracles linked math into their own ELF; the APK instead has a
    # shared scene/math dependency. Execute that ELF, copying ABI data only.
    if module_name in ('decor_scene', 'character_scene'):
        import decor_scene_differential as decor
        from unicorn.arm64_const import (UC_ARM64_REG_X3, UC_ARM64_REG_S0,
                                         UC_ARM64_REG_S1, UC_ARM64_REG_S2, UC_ARM64_REG_TPIDR_EL0)
        source_cpu = decor.Cpu
        source_call = decor.Dependencies.call
        from elftools.elf.elffile import ELFFile
        from unicorn import UC_HOOK_MEM_INVALID
        import struct

        class PackagedCpu(source_cpu):
            def __init__(self, path, arm64, dependencies, provenance):
                super().__init__(path, arm64, dependencies, provenance)
                if arm64:
                    # Android's stack protector reads the guard from TLS+0x28.
                    # It is a stable sentinel, not an engine algorithm service.
                    self.uc.reg_write(UC_ARM64_REG_TPIDR_EL0, self.data + 0xf800)
                    def invalid(uc, access, address, size, value, unused):
                        print('Invalid packaged memory:', hex(uc.reg_read(self.pc_reg)),
                              hex(address), size, self.import_calls, file=sys.stderr)
                        return False
                    self.uc.hook_add(UC_HOOK_MEM_INVALID, invalid)
                    self.int_regs = self.int_regs + (UC_ARM64_REG_X3,)
                    # Shared APK ELFs also reference exported constants through
                    # GLOB_DAT. The historical standalone loader handled PLT
                    # and relative relocations only.
                    with path.open('rb') as stream:
                        elf = ELFFile(stream)
                        for section in elf.iter_sections():
                            if section['sh_type'] not in ('SHT_REL', 'SHT_RELA'):
                                continue
                            symbols = elf.get_section(section['sh_link'])
                            for relocation in section.iter_relocations():
                                if relocation['r_info_type'] not in (1025, 257):
                                    continue
                                symbol = symbols.get_symbol(relocation['r_info_sym'])
                                if symbol['st_shndx'] != 'SHN_UNDEF':
                                    target = self.base + symbol['st_value'] + relocation['r_addend']
                                else:
                                    target = next((address for address, name in self.imports.items()
                                                   if name == symbol.name), None)
                                    if target is None:
                                        target = self.extern + len(self.imports) * 16
                                        self.imports[target] = symbol.name
                                self.uc.mem_write(self.base + relocation['r_offset'], struct.pack('<Q', target))
                    for address, name in self.imports.items():
                        if name in ('dh2_node_matrix', 'dh2_quat_from_euler'):
                            self.symbols.setdefault(name, address)

        def call(dependencies, cpu, name):
            if name in ('__memcpy_chk', '__memmove_chk', '__memset_chk'):
                destination, source, count, capacity = [cpu.reg(i) for i in range(4)]
                assert count <= capacity and count <= 0x10000
                if count:
                    payload = bytes((source & 255,)) * count if name == '__memset_chk' else bytes(cpu.uc.mem_read(source, count))
                    cpu.uc.mem_write(destination, payload)
                cpu.write_reg(0, destination)
                cpu.uc.reg_write(cpu.pc_reg, cpu.uc.reg_read(cpu.lr_reg))
                return
            if name not in ('dh2_node_matrix', 'dh2_quat_from_euler'):
                return source_call(dependencies, cpu, name)
            assert cpu.arm64, 'Only packaged ARM64 imports use this bridge'
            if not hasattr(dependencies, 'packaged_math'):
                dependencies.packaged_math = PackagedCpu(math_library, True, dependencies, {'functions': []})
                dependencies.packaged_math_calls = {}
            other = dependencies.packaged_math
            dependencies.packaged_math_calls[name] = dependencies.packaged_math_calls.get(name, 0) + 1
            output = cpu.reg(0)
            if name == 'dh2_quat_from_euler':
                for register in (UC_ARM64_REG_S0, UC_ARM64_REG_S1, UC_ARM64_REG_S2):
                    other.uc.reg_write(register, cpu.uc.reg_read(register))
                other.invoke(name, [other.data])
                size = 16
            else:
                pointers = [other.data + 128, other.data + 256, other.data + 384]
                for index, (pointer, size) in enumerate(zip(pointers, (12, 16, 12)), 1):
                    other.uc.mem_write(pointer, bytes(cpu.uc.mem_read(cpu.reg(index), size)))
                other.invoke(name, [other.data, *pointers])
                size = 64
            cpu.uc.mem_write(output, bytes(other.uc.mem_read(other.data, size)))
            cpu.write_reg(0, output)
            cpu.uc.reg_write(cpu.pc_reg, cpu.uc.reg_read(cpu.lr_reg))

        decor.Cpu = PackagedCpu
        decor.Dependencies.call = call
    path = TESTS / (module_name + '_differential.py')
    if module_name == 'decor_scene':
        module = decor
    else:
        spec = importlib.util.spec_from_file_location('live_packaged_' + module_name, path)
        module = importlib.util.module_from_spec(spec)
        spec.loader.exec_module(module)
    sys.argv = [str(path), *arguments]
    module.main()


def main():
    if len(sys.argv) > 1 and sys.argv[1] == '_worker':
        worker(sys.argv[2], Path(sys.argv[3]), sys.argv[4:])
        return
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--studio', type=Path, required=True)
    args = parser.parse_args()
    started = time.monotonic()
    LOCAL.mkdir(parents=True, exist_ok=True)
    REPORTS.mkdir(parents=True, exist_ok=True)
    apks = {'packaged': REPO / 'port/android-native/app/build/outputs/apk/debug/app-debug.apk',
            'studio': args.studio / 'app/build/outputs/apk/debug/app-debug.apk'}
    apk_hashes = {tag: sha(path) for tag, path in apks.items()}
    libraries = {}
    dependencies = {}
    for tag, apk in apks.items():
        with zipfile.ZipFile(apk) as archive:
            libraries[tag] = {}
            for stem in ('level_world', 'scene_materials'):
                name = 'lib/arm64-v8a/libdh2_' + stem + '.so'
                output = LOCAL / (tag + '-' + stem + '.so')
                output.write_bytes(archive.read(name))
                libraries[tag][stem] = {'apk_member': name, 'sha256': sha(output), 'path': str(output)}
            # Character bounds differential consumes this actual authored asset.
            model = REPO / 'port/android-native/app/src/main/assets/models/prince_modular.bdae'
            assert archive.read('assets/models/prince_modular.bdae') == model.read_bytes()
            dependencies[tag] = {'prince_model_sha256': sha(model)}
    groups = {}
    for tag, records in libraries.items():
        key = tuple(records[stem]['sha256'] for stem in ('level_world', 'scene_materials'))
        groups.setdefault(key, []).append(tag)
    results = {}
    for tags in groups.values():
        tag = tags[0]
        library = Path(libraries[tag]['level_world']['path'])
        math_library = Path(libraries[tag]['scene_materials']['path'])
        for module in MODULES:
            label = module.replace('_', '-')
            report_path = REPORTS / ('live-actor-' + tag + '-' + label + '-arm64-differential.json')
            reference = LOCAL / (tag + '-' + label + '-reference.bin')
            command = [sys.executable, str(Path(__file__).resolve()), '_worker', module,
                       str(math_library), '--engine', str(ENGINE), '--library', str(library),
                       '--report', str(report_path), '--reference-output', str(reference)]
            if module == 'visual_motion':
                command += ['--asset-samples', str(SAMPLES), '--math-library', str(math_library)]
            environment = os.environ.copy()
            environment['PYTHONDONTWRITEBYTECODE'] = '1'
            result = subprocess.run(command, cwd=REPO, env=environment, capture_output=True, text=True)
            if result.returncode:
                (LOCAL / (tag + '-' + label + '-failure.txt')).write_text(result.stdout + result.stderr)
                raise RuntimeError(label + ' differential failed:\n' + result.stderr)
            report = json.loads(report_path.read_text())
            assert report['mismatches'] == 0
            assert report['original_sha256'] == sha(ENGINE)
            assert report.get('arm64_library_sha256', report.get('native_sha256')) == sha(library)
            assert report.get('reference_sha256', report.get('corpus_sha256')) == sha(reference)
            count = report.get('comparisons', report.get('cases'))
            if count is None:
                count = sum(report[key] for key in ('skin_comparisons', 'mesh_comparisons',
                                                   'visual_scale_comparisons', 'owner_bounds_comparisons'))
            expected = {'actor_rotation': 5590, 'decor_scene': 503, 'character_scene': 514,
                        'visual_motion': 1796}[module]
            assert count == expected, (module, count, expected)
            if module == 'visual_motion':
                assert report['counts']['asset_samples'] == 3567
                assert report['math_dependency_sha256'] == sha(math_library)
            binding = {'report': str(report_path.relative_to(REPO)), 'report_sha256': sha(report_path),
                       'library_sha256': sha(library), 'scene_math_library_sha256': sha(math_library),
                       'reference_sha256': sha(reference), 'comparisons': count, 'mismatches': 0,
                       'executed_for_tags': tags, 'differential': report}
            for item in tags:
                results.setdefault(item, {})[label] = binding
            print(','.join(tags) + ' ' + label + ': ' + str(count) + ' pass', flush=True)
    # Prevent a concurrently rebuilt APK from being bound to the earlier run.
    assert apk_hashes == {tag: sha(path) for tag, path in apks.items()}, 'APK changed during checks'
    report = {'validation': 'PASS', 'apk_sha256': apk_hashes, 'libraries': libraries,
              'original_sha256': sha(ENGINE), 'asset_samples_sha256': sha(SAMPLES),
              'dependencies': dependencies, 'packaged_differentials': results,
              'unique_library_dependency_pairs_executed': len(groups),
              'cases_per_apk': 8403, 'real_asset_samples_per_apk': 3567,
              'script_sha256': sha(Path(__file__)),
              'test_source_sha256': {module: sha(TESTS / (module + '_differential.py')) for module in MODULES},
              'scope': __doc__, 'emulator_tested': False, 'production_rebuilt': False,
              'full_original_frame_parity_claimed': False,
              'elapsed_seconds': round(time.monotonic() - started, 3)}
    path = REPORTS / 'live-actor-kernel-validation.json'
    path.write_text(json.dumps(report, indent=2) + '\n')
    print(json.dumps({'validation': 'PASS', 'apk_sha256': apk_hashes,
                      'library_sha256': {tag: rows['level_world']['sha256'] for tag, rows in libraries.items()},
                      'cases_per_apk': 8403, 'real_asset_samples_per_apk': 3567}))


if __name__ == '__main__':
    main()
