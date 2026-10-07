"""Bind the completed owned-player/text batch to actual Android libraries.

No APK packaging or device execution is inferred from this library inspection.
"""
import hashlib
import argparse
import io
import json
from pathlib import Path

from elftools.elf.elffile import ELFFile

ROOT = Path(__file__).resolve().parents[3]
HOST = ROOT / 'port/level-world/reports/native-owned-player-text-layout-main-linked-host-audit-v1.json'
OUTPUT = ROOT / 'port/android-native/reports/native-owned-player-text-layout-library-build-v1.json'


def sha(raw):
    return hashlib.sha256(raw).hexdigest()


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--host', type=Path, default=HOST)
    parser.add_argument('--output', type=Path, default=OUTPUT)
    parser.add_argument('--include-hud-initialization', action='store_true')
    args = parser.parse_args()
    output = args.output.resolve()
    assert not output.exists(), 'Preserve prior build inspections'
    host_raw = args.host.read_bytes()
    host = json.loads(host_raw)
    assert host['validation'] == 'PASS' and host['sanitizer_findings'] == 0
    count = 120 if args.include_hud_initialization else 117
    assert len(host['host_audits']) == count
    assert host['host_audits']['Text_layout_v1']['whole_original_cases'] == 960
    for name in ('Player_savegame_v1', 'Player_faeries_v1', 'Player_metadata_v1',
                 'Item_inventory_v1', 'Inventory_potions_v1', 'Player_profile_index_v1'):
        assert host['host_audits'][name]['validation'] == 'PASS'
        assert host['host_audits'][name]['mismatches'] == 0
    modules = ('port/engine-ui/text_layout_v1', 'port/game-data/player_savegame_v1',
               'port/game-data/item_inventory_v1', 'port/game-data/player_profile_index_v1')
    if args.include_hud_initialization:
        modules += tuple('port/engine-ui/' + name for name in
                         ('hud_initialization_v1', 'hud_initialization_core_v1',
                          'hud_initialization_owned_v1', 'hud_manager_core_v2'))
        assert host['host_audits']['HUD_initialization_base_v1']['manager_supported_styles'] == 4
    paths = [name + suffix for name in modules for suffix in ('.cpp', '.hpp')]
    paths += ['port/engine-ui/CMakeLists.txt', 'port/game-data/CMakeLists.txt',
              'port/level-world/CMakeLists.txt']
    sources = {name: sha((ROOT / name).read_bytes()) for name in paths}
    for name, digest in sources.items():
        assert host['source_sha256'][name] == digest, ('Main source drift', name)
    freezes = {}
    freeze_paths = ['port/engine-ui/reference/text-layout-v1/freeze-manifest.json',
                    'port/game-data/reference/player-inventory-v1/freeze-manifest.json']
    if args.include_hud_initialization:
        freeze_paths.append('port/engine-ui/reference/hud-initialization-v1/freeze-manifest.json')
    for name in freeze_paths:
        raw = (ROOT / name).read_bytes()
        freeze = json.loads(raw)
        for key, digest in freeze.get('source_sha256', freeze['source_and_evidence_sha256']).items():
            assert sha((ROOT / key).read_bytes()) == digest, key
        freezes[name] = sha(raw)
    libraries = {
        'libdh2_engine_ui.so': ('text_v1', 'format_text', 'append_image', 'parse_html'),
        'libdh2_game_data.so': ('PlayerSavegameV1', 'ItemInventoryV1', 'PlayerProfileIndexV1'),
        'libdh2_level_world.so': ('dh2_ui_hud_skill_query',),
    }
    if args.include_hud_initialization:
        libraries['libdh2_engine_ui.so'] += ('dh2_ui_hud_initialization_v1', 'HudManagerCoreV2')
        libraries['libdh2_level_world.so'] += ('hud_initialization_owned_v1_query',)
    # Export names are checked against actual defined dynamic symbols, not strings
    # in debug sections or references in undefined import entries.
    abi_results = {}
    for abi, machine in (('arm64-v8a', 'EM_AARCH64'), ('x86_64', 'EM_X86_64')):
        build = ROOT / 'port/android-native/app/.cxx/Debug/5a1n3w3m' / abi
        database_raw = (build / 'compile_commands.json').read_bytes()
        database = json.loads(database_raw)
        records = [row for row in database
                   if Path(row['file']).resolve() in { (ROOT / (name + '.cpp')).resolve() for name in modules }]
        assert len(records) == len(modules) and len({row['file'] for row in records}) == len(modules)
        for row in records:
            assert '--target=' in row['command'] and '-android24' in row['command']
            if Path(row['file']).name == 'text_layout_v1.cpp':
                assert '-fno-fast-math' in row['command'] and '-ffp-contract=off' in row['command']
        inspected = {}
        for name, markers in libraries.items():
            path = ROOT / 'port/android-native/app/build/intermediates/cxx/Debug/5a1n3w3m/obj' / abi / name
            raw = path.read_bytes()
            elf = ELFFile(io.BytesIO(raw))
            assert elf.elfclass == 64 and elf['e_machine'] == machine and elf['e_type'] == 'ET_DYN'
            loads = []
            for segment in elf.iter_segments():
                if segment['p_type'] == 'PT_LOAD':
                    offset, address, alignment = (segment[key] for key in ('p_offset', 'p_vaddr', 'p_align'))
                    assert alignment >= 16384 and offset % 16384 == address % 16384
                    loads.append(dict(offset=offset, address=address, alignment=alignment))
            assert loads
            defined = [symbol.name for symbol in elf.get_section_by_name('.dynsym').iter_symbols()
                       if symbol['st_shndx'] != 'SHN_UNDEF']
            evidence = {}
            for marker in markers:
                matches = [symbol for symbol in defined if marker in symbol]
                assert matches, (name, marker)
                evidence[marker] = matches[:8]
            inspected[name] = dict(path=path.relative_to(ROOT).as_posix(), sha256=sha(raw), bytes=len(raw),
                                   ELF64=True, machine=machine, load_segments=loads, defined_exports=evidence)
        abi_results[abi] = dict(libraries=inspected, compiler_records=records,
                               compile_commands_sha256=sha(database_raw))
    assert sources == {name: sha((ROOT / name).read_bytes()) for name in sources}
    result = dict(validation='PASS', scope='Actual Android native library build; owned player data and original text layout.',
                  source_sha256=sources, freeze_sha256=freezes, main_host_report_sha256=sha(host_raw),
                  main_host_suites=count, sanitizer_findings=0, android_abis=abi_results,
                  NDK_version='29.0.14206865', linker_page_alignment=16384,
                  packaged_APK=False, live_complete_HUD=False, physical_arm64_verified=False)
    output.write_text(json.dumps(result, indent=2) + '\n', encoding='utf-8')
    print(json.dumps(dict(validation='PASS', main_host_suites=count, android_abis=list(abi_results),
                         output=output.relative_to(ROOT).as_posix())))


if __name__ == '__main__':
    main()
