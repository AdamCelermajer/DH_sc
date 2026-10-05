"""Bind native equipment compilation and ELF inspection to the main host receipt.

This records library integration, not APK packaging or live gameplay.
"""
import hashlib
import io
import json
from pathlib import Path
from elftools.elf.elffile import ELFFile

ROOT = Path(__file__).resolve().parents[3]
APP = ROOT / 'port/android-native/app'
OUTPUT = ROOT / 'port/android-native/reports/native-equipped-player-library-build-v1.json'


def sha(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()


def main():
    assert not OUTPUT.exists(), 'Preserve existing build receipts'
    skill_dir = ROOT / 'port/level-world/reference/character-skill-gameplay-v3'
    skill_freeze_path = skill_dir / 'freeze-manifest.json'
    loot_freeze_path = ROOT / 'port/game-data/reference/loot-power-creation-v7/freeze-manifest.json'
    skills = json.loads(skill_freeze_path.read_text())
    loot = json.loads(loot_freeze_path.read_text())
    assert skills['validation'] == loot['validation'] == 'PASS'
    bindings = json.loads((skill_dir / 'integration.json').read_text())
    modules = {
        'dh2_level_world': bindings['production_world_sources'],
        'dh2_script_runtime': [bindings['runtime_replace']['replacement']],
        'dh2_game_data': [name for name in loot['production_source_sha256']
                          if name.endswith('.cpp')],
        'dh2_native': ['port/android-native/app/src/main/cpp/model_renderer.cpp',
                       'port/android-native/app/src/main/cpp/native_app.cpp'],
    }
    frozen = dict(skills['source_and_evidence_sha256'])
    for group in ('production_source_sha256', 'source_sha256', 'proof_sha256',
                  'reference_sha256', 'borrowed_dependency_sha256'):
        for name, digest in loot[group].items():
            assert name not in frozen or frozen[name] == digest
            frozen[name] = digest
    for name, digest in frozen.items():
        assert sha(ROOT / name) == digest, name
    extra_freezes = [ROOT / 'port/engine-skinning/reference/visual-skin-owner-v6/freeze-manifest.json',
                    ROOT / 'port/level-world/reference/player-equipment-render-owner-v1/freeze-manifest.json']
    for freeze_path in extra_freezes:
        freeze = json.loads(freeze_path.read_text())
        assert freeze['validation'] == 'PASS'
        owner = 'dh2_engine_skinning' if 'engine-skinning' in str(freeze_path) else 'dh2_level_world'
        modules.setdefault(owner, []).extend(name for name in freeze['production_source_sha256'] if name.endswith('.cpp'))
        for group in ('production_source_sha256','source_sha256','proof_sha256','reference_sha256'):
            for name,digest in freeze[group].items():
                assert name not in frozen or frozen[name] == digest
                frozen[name] = digest
                assert sha(ROOT/name) == digest, name
    receipt = ROOT / 'port/level-world/reports/native-equipped-player-main-linked-host-audit-v1.json'
    host = json.loads(receipt.read_text())
    assert host['validation'] == 'PASS' and host['sanitizer_findings'] == 0
    assert len(host['host_audits']) == 155
    tracked = [ROOT / name for name in frozen]
    tracked += [skill_freeze_path, loot_freeze_path, *extra_freezes, Path(__file__), receipt,
                ROOT / 'port/engine-skinning/CMakeLists.txt',
                APP / 'src/main/cpp/model_renderer.cpp', APP / 'src/main/cpp/model_renderer.hpp',
                APP / 'src/main/cpp/native_app.cpp',
                APP / 'src/main/java/com/example/dh2/NativeBridge.java',
                APP / 'src/main/java/com/example/dh2/MainActivity.java',
                ROOT / 'port/level-world/CMakeLists.txt',
                ROOT / 'port/script-runtime/CMakeLists.txt',
                ROOT / 'port/game-data/CMakeLists.txt',
                APP / 'src/main/cpp/CMakeLists.txt']
    before = {path.relative_to(ROOT).as_posix(): sha(path) for path in tracked}
    abis = {}
    for abi, machine in (('arm64-v8a', 'EM_AARCH64'), ('x86_64', 'EM_X86_64')):
        compiler_path = APP / '.cxx/Debug/5a1n3w3m' / abi / 'compile_commands.json'
        compiler = json.loads(compiler_path.read_text())
        normalized = [record['file'].replace('\\', '/') for record in compiler]
        for excluded in bindings['runtime_replace']['never_compile_together']:
            assert not any(name.endswith('/' + excluded) for name in normalized), (abi, excluded)
        records = {}
        for owner, sources in modules.items():
            for source in sources:
                found = [record for record in compiler
                         if record['file'].replace('\\', '/').endswith('/' + source)]
                assert len(found) == 1, (abi, source, len(found))
                record = found[0]
                command = record['command'].replace('\\', '/')
                assert 'CMakeFiles/' + owner + '.dir' in command
                assert '-fno-fast-math' in command and '-ffp-contract=off' in command
                records[source] = record
        libraries = {}
        required = {
            'dh2_game_data': ('dh2_loot_power_select_v7', 'dh2_loot_quantity_v7',
                             'dh2_loot_item_value_v7', 'LootPowerCreationV7', 'LootPowerResourcesV7'),
            'dh2_level_world': ('CharacterPlayerSkillsV3', 'CharacterScriptSessionV3', 'PlayerEquipmentRenderOwnerV1'),
            'dh2_engine_skinning': ('VisualSkinOwnerV6', 'VisualSkinResourcesV6', 'dh2_visual'),
            'dh2_native': ('Java_com_example_dh2_NativeBridge_playerEquipmentAction',),
            'dh2_script_runtime': ('indexed',),
        }
        for owner in dict.fromkeys((*modules, 'dh2_engine_ui', 'dh2_native')):
            path = APP / 'build/intermediates/cxx/Debug/5a1n3w3m/obj' / abi / ('lib' + owner + '.so')
            elf = ELFFile(io.BytesIO(path.read_bytes()))
            assert elf.elfclass == 64 and elf['e_machine'] == machine
            alignments = [segment['p_align'] for segment in elf.iter_segments()
                          if segment['p_type'] == 'PT_LOAD']
            assert alignments and min(alignments) >= 16384
            symbols = [symbol.name for symbol in elf.get_section_by_name('.dynsym').iter_symbols()
                       if symbol['st_shndx'] != 'SHN_UNDEF']
            needed = [tag.needed for tag in elf.get_section_by_name('.dynamic').iter_tags()
                      if tag.entry.d_tag == 'DT_NEEDED']
            selected = {}
            for marker in required.get(owner, ()):
                selected[marker] = [symbol for symbol in symbols if marker in symbol]
                assert selected[marker], (abi, owner, marker)
            if owner == 'dh2_level_world':
                assert 'libdh2_script_runtime.so' in needed and 'libdh2_game_data.so' in needed
            libraries[owner] = dict(path=str(path), sha256=sha(path), elf_class=64,
                machine=machine, load_alignments=alignments, needed=needed,
                selected_defined_symbols=selected)
        abis[abi] = dict(libraries=libraries, compiler_records=records,
                        compiler_database_sha256=sha(compiler_path))
    assert before == {path.relative_to(ROOT).as_posix(): sha(path) for path in tracked}
    OUTPUT.write_text(json.dumps(dict(validation='PASS', source_sha256=before,
        abis=abis, main_host_receipt_sha256=sha(receipt),
        selected_production_sources=sum(map(len, modules.values())),
        APK_packaged=False, live_player_skills_connected=False,
        live_player_loot_connected=False, physical_device_tested=False), indent=2) + '\n')
    print(json.dumps(dict(validation='PASS', ABIs=list(abis), APK_packaged=False)))


if __name__ == '__main__':
    main()
