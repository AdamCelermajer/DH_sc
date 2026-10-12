"""Inspect centrally compiled item subsystem; no APK/live-player claim."""
import hashlib
import io
import json
from pathlib import Path
from elftools.elf.elffile import ELFFile

ROOT = Path(__file__).resolve().parents[3]
APP = ROOT/'port/android-native/app'
OUTPUT = ROOT/'port/android-native/reports/native-item-effects-library-build-v5.json'
MODULES = {
    'dh2_game_data': ('item_gear_properties_v5','item_power_tables_v5',
                     'item_presentation_v5','player_gear_effects_v5'),
    'dh2_engine_ui': ('item_text_varargs_v5','item_text_owner_v5'),
}


def sha(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()


def main():
    assert not OUTPUT.exists(), 'Preserve earlier build inspections'
    freeze_path = ROOT/'port/game-data/reference/player-item-effects-v5/freeze-manifest.json'
    freeze = json.loads(freeze_path.read_text())
    assert freeze['validation'] == 'PASS'
    for name, digest in freeze['production_source_sha256'].items():
        assert sha(ROOT/name) == digest
    source_files = [ROOT/name for name in freeze['production_source_sha256']]
    source_files += [freeze_path, Path(__file__),
        ROOT/'port/game-data/CMakeLists.txt', ROOT/'port/engine-ui/CMakeLists.txt',
        ROOT/'port/android-native/app/src/main/cpp/CMakeLists.txt']
    before = {path.relative_to(ROOT).as_posix(): sha(path) for path in source_files}
    host_receipt = ROOT/'port/level-world/reports/native-item-effects-main-linked-host-audit-v5.json'
    host = json.loads(host_receipt.read_text())
    assert host['validation'] == 'PASS'
    abis = {}
    for abi, machine in (('arm64-v8a','EM_AARCH64'), ('x86_64','EM_X86_64')):
        compiler_path = APP/'.cxx/Debug/5a1n3w3m'/abi/'compile_commands.json'
        compiler = json.loads(compiler_path.read_text())
        records = {}
        for owner, modules in MODULES.items():
            for module in modules:
                found = [record for record in compiler
                    if record['file'].replace('\\','/').endswith('/'+module+'.cpp')]
                assert len(found) == 1, (abi, module, len(found))
                record = found[0]
                assert 'CMakeFiles/'+owner+'.dir' in record['command'].replace('\\','/')
                assert '-fno-fast-math' in record['command'] and '-ffp-contract=off' in record['command']
                records[module] = record
        libraries = {}
        for owner in (*MODULES, 'dh2_level_world', 'dh2_native'):
            path = APP/'build/intermediates/cxx/Debug/5a1n3w3m/obj'/abi/('lib'+owner+'.so')
            elf = ELFFile(io.BytesIO(path.read_bytes()))
            assert elf.elfclass == 64 and elf['e_machine'] == machine
            alignments = [segment['p_align'] for segment in elf.iter_segments()
                          if segment['p_type'] == 'PT_LOAD']
            assert alignments and min(alignments) >= 16384
            symbols = [symbol.name for symbol in elf.get_section_by_name('.dynsym').iter_symbols()
                       if symbol['st_shndx'] != 'SHN_UNDEF']
            needed = [tag.needed for tag in elf.get_section_by_name('.dynamic').iter_tags()
                      if tag.entry.d_tag == 'DT_NEEDED']
            selected = []
            if owner == 'dh2_game_data':
                for exact in ('dh2_gear_reset_v5', 'dh2_gear_stats_v5',
                              'dh2_gear_power_v5', 'dh2_gear_validate_vitals_v5',
                              'dh2_item_power_decode_v5'):
                    assert exact in symbols
                    selected.append(exact)
                for marker in ('ItemPowerTablesV5','PlayerGearEffectsV5','ItemPresentationOwnerV5',
                               'item_update_name_v5','item_update_stats_v5','item_update_requirements_v5'):
                    matches = [symbol for symbol in symbols if marker in symbol]
                    assert matches, (abi, marker)
                    selected.extend(matches)
            elif owner == 'dh2_engine_ui':
                assert 'libdh2_game_data.so' in needed
                for marker in ('ItemTextOwnerV5','item_text_varargs_v5'):
                    matches = [symbol for symbol in symbols if marker in symbol]
                    assert matches, (abi, marker)
                    selected.extend(matches)
            libraries[owner] = dict(path=str(path), sha256=sha(path), elf_class=64,
                machine=machine, load_alignments=alignments, needed=needed,
                selected_defined_symbols=selected)
        abis[abi] = dict(libraries=libraries, compiler_records=records,
                         compiler_database_sha256=sha(compiler_path))
    assert before == {path.relative_to(ROOT).as_posix(): sha(path) for path in source_files}
    OUTPUT.write_text(json.dumps(dict(validation='PASS', source_sha256=before, abis=abis,
        main_host_receipt_sha256=sha(host_receipt), selected_production_sources=6,
        APK_packaged=False, live_player_inventory_connected=False,
        full_Skin_resource_construction=False, physical_device_tested=False), indent=2)+'\n')
    print(json.dumps(dict(validation='PASS', ABIs=list(abis), selected_native_sources=6,
                          APK_packaged=False)))


if __name__ == '__main__':
    main()
