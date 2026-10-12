"""Bind read-only retained-frame discoveries; this does not build or test native code."""
import hashlib
import json
from pathlib import Path
from elftools.elf.elffile import ELFFile

ROOT = Path(__file__).resolve().parents[4]
REF = Path(__file__).resolve().parent
ORIGINAL = ROOT / '.local-inputs/libDungeonHunter2.so'
EXPECTED = '36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80'


def sha(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()


def relative(path):
    return path.relative_to(ROOT).as_posix()


def main():
    assert sha(ORIGINAL) == EXPECTED
    manifests = sorted(REF.rglob('original-functions.json'))
    functions = []
    with ORIGINAL.open('rb') as stream:
        elf = ELFFile(stream)
        segments = [s for s in elf.iter_segments() if s['p_type'] == 'PT_LOAD']
        for path in manifests:
            manifest = json.loads(path.read_text(encoding='utf-8'))
            assert manifest['original_sha256'] == EXPECTED
            for function in manifest['functions']:
                address = int(function['elf_address'], 16)
                size = function['size']
                segment = next(s for s in segments if s['p_vaddr'] <= address
                               and address + size <= s['p_vaddr'] + s['p_filesz'])
                stream.seek(segment['p_offset'] + address - segment['p_vaddr'])
                assert hashlib.sha256(stream.read(size)).hexdigest() == function['sha256']
                functions.append({**function, 'manifest': relative(path)})
    assert len(functions) == 36
    observations = json.loads((REF / 'original-prerequisites.json').read_text())
    assert observations['validation'] == 'PASS'
    assert observations['original_cases'] == 41
    assert observations['native_comparisons'] == 0 and not observations['full_frame']
    for path, digest in observations['source_sha256'].items():
        assert sha(ROOT / path) == digest, path

    # These are an integration inventory, not compiler inputs or newly replayed proofs.
    names = ['character_state_owner_frame', 'character_native_fsm',
             'character_idle_update', 'character_idle_events', 'character_ai_frame',
             'character_ai_update', 'character_script_states', 'actor_blended_playback',
             'navigation_controller', 'subobjects_update', 'physical_world']
    sources = [ROOT / 'port/level-world' / (name + suffix)
               for name in names for suffix in ('.hpp', '.cpp')]
    artifacts = [p for p in sorted(REF.rglob('*')) if p.is_file()
                 and p.name not in ('handoff.json',) and p.suffix in ('.json', '.asm', '.md', '.py')]
    report = {
        'validation': 'PASS',
        'scope': 'Read-only source discovery and original-only prerequisite observations.',
        'original_sha256': EXPECTED,
        'captured_routines': len(functions),
        'original_prerequisite_cases': 41,
        'native_comparisons': 0,
        'new_native_code': False,
        'whole_character_frame_executed': False,
        'arm64_differential': False,
        'sanitizer_audit': False,
        'compiler_provenance_claim': False,
        'function_bytes_verified': functions,
        'artifacts_sha256': {relative(p): sha(p) for p in artifacts},
        'existing_kernel_inventory_only_sha256': {relative(p): sha(p) for p in sources},
        'eligible_branch': {
            'state': 3, 'ai_type': 4, 'zoned': 1, 'in_zone': 0,
            'active_ai_required': True, 'heading': 0,
            'visual_eligibility_is_borrowed_source_input': True,
            'ai_target_master_aggro_onupdate_skipped_by_original_gate': True,
            'fsm_elapsed_advancements': 1,
        },
        'remaining_required_delivery': [
            'Character debug/global prefixes and CanUpdate visual/online/player/culling producers',
            'Timer expiry effects/notifications and optional TimerUtil when installed',
            'Exact controller class dispatch and animator set-user ownership bookkeeping',
            'Ordered GameObject navigation/rotation/subobjects/network/idle-sound services',
            'Character posttail multiplayer/cutscene and conditional FX/text delivery',
            'Shared scene/physics/contact/ObjectManager eligibility owners',
        ],
        'reproduction': [
            'python port/level-world/reference/character-retained-frame/probe_original.py',
            'python port/level-world/reference/character-retained-frame/freeze_evidence.py',
        ],
    }
    out = ROOT / 'port/level-world/reports/character-retained-frame-source-discovery.json'
    out.write_text(json.dumps(report, indent=2) + '\n', encoding='utf-8')
    handoff = {'validation': 'PASS', 'report': relative(out), 'report_sha256': sha(out),
               'notes': relative(REF / 'NOTES.md'), 'notes_sha256': sha(REF / 'NOTES.md'),
               'production_changed': False, 'proof': {'original_cases': 41, 'native_cases': 0}}
    (REF / 'handoff.json').write_text(json.dumps(handoff, indent=2) + '\n', encoding='utf-8')
    print(json.dumps(handoff))


if __name__ == '__main__':
    main()
