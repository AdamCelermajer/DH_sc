"""Verify original Spawn evidence and write the cumulative mapping extension.

This extends the pinned Adam audit without rewriting its historical ledger.
Evidence reach is not a count of completely rebuilt functions.
"""
import argparse
import csv
import hashlib
import json
import re
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
MANIFEST = 'port/level-world/reference/character-spawn/original-functions.json'
TEMPLATE_MANIFEST = 'port/level-world/reference/character-template-factory/original-functions.json'
RANDOM_MANIFEST = 'port/level-world/reference/character-template-random/original-functions.json'
TRIGGER_MANIFEST = 'port/level-world/reference/crypt-spawn-trigger/original-functions.json'
SCRIPT_MANIFEST = 'port/level-world/reference/crypt-ghost-scripts/original-functions.json'
GROUP_MANIFEST = 'port/level-world/reference/character-group-respawn/original-functions.json'
AGGRO_MANIFEST = 'port/level-world/reference/character-aggro-cleanup/original-functions.json'
RESPAWN_MANIFEST = 'port/level-world/reference/character-respawn-outer/original-functions.json'
LIMBUS_MANIFEST = 'port/level-world/reference/character-limbus-respawn/original-functions.json'
GROUP_BLUR_MANIFEST = 'port/level-world/reference/character-group-limbus-blur/original-functions.json'
VISIBILITY_MANIFEST = 'port/level-world/reference/character-spawn-visibility/original-functions.json'
MANIFESTS = [MANIFEST, TEMPLATE_MANIFEST, RANDOM_MANIFEST, TRIGGER_MANIFEST, SCRIPT_MANIFEST, GROUP_MANIFEST, AGGRO_MANIFEST, RESPAWN_MANIFEST, LIMBUS_MANIFEST, GROUP_BLUR_MANIFEST, VISIBILITY_MANIFEST]
AI_MANIFESTS = {
    'port/level-world/reference/character-aggro-delay/original-functions.json': 'character_aggro_delay',
    'port/level-world/reference/character-ai-turn/original-functions.json': 'character_ai_turn',
    'port/level-world/reference/character-aggro-target-search/original-functions.json': 'character_aggro_target_search',
    'port/level-world/reference/character-aggro-candidate-events/original-functions.json': 'character_aggro_candidate_events',
    'port/level-world/reference/character-enemy-spotted/original-functions.json': 'character_enemy_spotted',
    'port/level-world/reference/monster-external-script-session/original-functions.json': 'monster_external_script_session',
    'port/level-world/reference/character-ai-set-target/original-functions.json': 'character_ai_set_target',
    'port/level-world/reference/character-ai-relations/original-functions.json': 'character_ai_relations',
    'port/level-world/reference/character-ai-in-combat/original-functions.json': 'character_ai_in_combat',
    'port/level-world/reference/character-aggro-acquisition-prefix/original-functions.json': 'character_aggro_acquisition_prefix',
    'port/level-world/reference/character-ai-sight/original-functions.json': 'character_ai_sight',
    'port/level-world/reference/character-monster-retarget/original-functions.json': 'character_monster_retarget',
    'port/level-world/reference/ghost-ai-session/original-functions.json': 'ghost_ai_session',
    'port/level-world/reference/character-enemy-retention/original-functions.json': 'character_enemy_retention',
    'port/level-world/reference/character-ai-master-update/original-functions.json': 'character_ai_master_update',
    'port/level-world/reference/character-ai-update-target/original-functions.json': 'character_ai_update_target',
    'port/level-world/reference/character-ai-melee-range/original-functions.json': 'character_ai_melee_range',
    'port/level-world/reference/character-ai-interaction-range/original-functions.json': 'character_ai_interaction_range',
    'port/level-world/reference/ais-external-update/original-functions.json': 'ais_external_update',
    'port/level-world/reference/ais-state-callbacks/original-functions.json': 'ais_state_callbacks',
    'port/level-world/reference/character-ai-pause-update/original-functions.json': 'character_ai_pause_update',
    'port/level-world/reference/ais-default-collision-persist/original-functions.json': 'ais_default_collision_persist',
    'port/level-world/reference/character-ai-ranged-range/original-functions.json': 'character_ai_ranged_range',
    'port/level-world/reference/character-range-capability/original-functions.json': 'character_range_capability',
    'port/level-world/reference/character-ai-queue/original-functions.json': 'character_ai_queue',
    'port/level-world/reference/character-interactive/original-functions.json': 'character_interactive',
    'port/level-world/reference/character-ai-initialization/original-functions.json': 'character_ai_initialization',
    'port/level-world/reference/ais-external-init-vcb/original-functions.json': 'ais_external_init_vcb',
    'port/level-world/reference/ais-external-initialization/original-functions.json': 'ais_external_initialization',
    'port/level-world/reference/ais-native-bindings/original-functions.json': 'ais_native_bindings',
    'port/level-world/reference/character-ai-association/original-functions.json': 'character_ai_association',
    'port/level-world/reference/character-native-bindings/original-functions.json': 'character_native_bindings',
    'port/level-world/reference/lua-script-level-queries/original-functions.json': 'lua_script_level_queries',
    'port/level-world/reference/character-script-set-level/original-functions.json': 'character_script_set_level',
    'port/level-world/reference/character-regeneration/original-functions.json': 'character_regeneration',
    'port/level-world/reference/debug-switches-runtime/original-functions.json': 'debug_switches_runtime',
    'port/level-world/reference/debug-switches-persistence/original-functions.json': 'debug_switches_persistence',
    'port/level-world/reference/module-room-zone-bounds/original-functions.json': 'module_room_zone_bounds',
    'port/level-world/reference/level-construction-fields/original-functions.json': 'level_construction_fields',
    'port/level-world/reference/character-ai-update-all-skills/original-functions.json': 'character_ai_update_all_skills',
    'port/level-world/reference/character-faery-selection/original-functions.json': 'character_faery_selection',
    'port/level-world/reference/character-ai-skill-script-constructor/original-functions.json': 'character_ai_skill_script_constructor',
    'port/level-world/reference/character-aggro-character-list/original-functions.json': 'character_aggro_character_list',
    'port/level-world/reference/character-aggro-object-manager-list/original-functions.json': 'character_aggro_object_manager_list',
    'port/level-world/reference/character-dot-tick/original-functions.json': 'character_dot_tick',
    'port/level-world/reference/character-dot-attack/original-functions.json': 'character_dot_attack',
    'port/level-world/reference/ais-external-init-callbacks/original-functions.json': 'ais_external_init_callbacks',
    'port/level-world/reference/character-init-hp-mp/original-functions.json': 'character_init_hp_mp',
    'port/level-world/reference/character-apply-result/original-functions.json': 'character_apply_result',
    'port/level-world/reference/character-skill-state-queries/original-functions.json': 'character_skill_state_queries',
    'port/level-world/reference/character-ai-set-skills-and-spells/original-functions.json': 'character_ai_set_skills_and_spells',
    'port/level-world/reference/character-ai-skill-script-update/original-functions.json': 'character_ai_skill_script_update',
    'port/level-world/reference/character-ai-skill-script-check/original-functions.json': 'character_ai_skill_script_check',
    'port/level-world/reference/lua-script-load-once/original-functions.json': 'lua_script_load_once',
    'port/level-world/reference/script-value-boolean/original-functions.json': 'script_value_boolean',
    'port/level-world/reference/character-ai-classification/original-functions.json': 'character_ai_classification',
    'port/level-world/reference/character-zonability/original-functions.json': 'character_zonability',
    'port/level-world/reference/monster-external-script-updates/original-functions.json': 'monster_external_script_session',
    'port/level-world/reference/character-update-eligibility/original-functions.json': 'character_update_eligibility',
    'port/level-world/reference/game-object-stop/original-functions.json': 'game_object_stop',
    'port/level-world/reference/character-physics-position/original-functions.json': 'character_physics_position',
    'port/level-world/reference/character-update-script-scheduler/original-functions.json': 'character_update_script_scheduler',
}
MANIFESTS.extend(AI_MANIFESTS)
ENGINE_MANIFESTS = {
    'port/game-data/reference/skill-faery-tables/original-functions.json':
        ['port/game-data/skill_tables.hpp', 'port/game-data/skill_tables.cpp'],
    'port/game-data/reference/level-tables/original-functions.json':
        ['port/game-data/level_tables.hpp', 'port/game-data/level_tables.cpp'],
    'port/level-world/reference/module-scene-root-bounds/original-functions.json':
        ['port/level-world/module_scene_root_bounds.hpp', 'port/level-world/module_scene_root_bounds.cpp'],
    'port/player-info-level/reference/character-level-member/original-functions.json':
        ['port/player-info-level/character_level_member.hpp', 'port/player-info-level/character_level_member.cpp'],
    'port/player-info-level/reference/player-manager-host-level/original-functions.json':
        ['port/player-info-level/player_manager_host_level.hpp', 'port/player-info-level/player_manager_host_level.cpp'],
    'port/level-world/reference/room-zone-enrollment/original-functions.json':
        ['port/level-world/room_zone_enrollment.hpp', 'port/level-world/room_zone_enrollment.cpp'],
    'port/level-world/reference/game-object-zoning-visibility/original-functions.json':
        ['port/level-world/game_object_zoning_visibility.hpp', 'port/level-world/game_object_zoning_visibility.cpp'],
    'port/level-world/reference/object-update-culling/original-functions.json':
        ['port/level-world/object_update_culling.hpp', 'port/level-world/object_update_culling.cpp'],
    'port/level-world/reference/game-object-set-visible/original-functions.json':
        ['port/level-world/game_object_set_visible.hpp', 'port/level-world/game_object_set_visible.cpp'],
    'port/level-world/reference/object-update-dispatch/original-functions.json':
        ['port/level-world/object_update_dispatch.hpp', 'port/level-world/object_update_dispatch.cpp'],
    'port/engine-camera/reference/frustum-producer/original-functions.json':
        ['port/engine-camera/frustum.hpp', 'port/engine-camera/frustum.cpp'],
    'port/engine-camera/reference/frustum-bounds/original-functions.json':
        ['port/engine-camera/frustum_bounds.hpp', 'port/engine-camera/frustum_bounds.cpp'],
    'port/engine-camera/reference/plane-intersection/original-functions.json':
        ['port/engine-camera/plane_intersection.hpp', 'port/engine-camera/plane_intersection.cpp'],
    'port/engine-camera/reference/frustum-runtime/original-functions.json':
        ['port/engine-camera/frustum_runtime.hpp', 'port/engine-camera/frustum_runtime.cpp'],
    'port/level-world/reference/player-skills-preparation-v3/original-functions.json':
        ['port/level-world/player_skill_tables_adapter.hpp', 'port/level-world/player_skill_tables_adapter.cpp',
         'port/level-world/character_player_skills_preparation_v3.hpp', 'port/level-world/character_player_skills_preparation_v3.cpp'],
    'port/scene-materials/reference/swamp-technique-selection-audit/original-functions.json':
        ['port/scene-materials/technique_selector.hpp', 'port/scene-materials/technique_selector.cpp'],
    'port/scene-materials/reference/swamp-render-state-audit/original-functions.json':
        ['port/scene-materials/render_state_snapshot.hpp', 'port/scene-materials/render_state_snapshot.cpp'],
    'port/scene-materials/reference/swamp-effect-pass-conversion/manifest.json':
        ['port/scene-materials/source_state_conversion.hpp', 'port/scene-materials/source_state_conversion.cpp'],
}
MANIFESTS.extend(ENGINE_MANIFESTS)
OUTPUT = 'docs/generated/character-runtime-function-map.json'
STATE = ['port/level-world/character_state.cpp',
         'port/level-world/character_coordinator.cpp']
NATIVE = 'port/android-native/app/src/main/cpp/model_renderer.cpp'


def classify(row):
    symbol = row['demangled']
    if row.get('evidence_manifest') in ENGINE_MANIFESTS:
        return ('bounded_engine_kernel_or_dependency_evidence',
                ENGINE_MANIFESTS[row['evidence_manifest']],
                row['manifest_implementation_scope'] + '; ' + row.get('scope', row.get('port_coverage', 'Supporting range evidence.')) +
                '; host tests, original instruction checks and artifact-specific renderer wiring are established separately. This is not a complete reconstruction of every supporting original function.')
    if row.get('evidence_manifest') in AI_MANIFESTS:
        unit = AI_MANIFESTS[row['evidence_manifest']]
        return ('bounded_ai_kernel_or_dependency_evidence',
                [f'port/level-world/{unit}.hpp', f'port/level-world/{unit}.cpp'],
                row['manifest_implementation_scope'] + '; ' + row.get('scope', row.get('port_coverage', 'Supporting range evidence.')) +
                '; build and live wiring state are established separately by checkpoint reports. Each manifest covers a bounded kernel or adapter, not a complete reconstruction of every supporting original function.')
    if row.get('evidence_manifest') == VISIBILITY_MANIFEST:
        return ('visibility_callsite_and_vtable_evidence', [VISIBILITY_MANIFEST],
                row['scope'] + '; read-only audit adds no implementation body.')
    if row.get('evidence_manifest') == GROUP_BLUR_MANIFEST:
        return ('host_limbus_blur_group_producer', ['port/level-world/character_group_limbus_blur.cpp'],
                row['scope'] + '; post-Revive role3/any-other-Limbus traversal only; native group/member ownership remains external.')
    if row.get('evidence_manifest') == LIMBUS_MANIFEST:
        return ('host_limbus_respawn_focus_producer', ['port/level-world/character_limbus_respawn.cpp',
                'port/level-world/character_respawn_outer.cpp',
                'port/level-world/character_coordinator.cpp'], row['scope'] +
                '; source prefix/two delay reads/hosting gate/timer request/normal cleanup are host-tested; live Android byte, manager and linked-owner producers remain external.')
    if row.get('evidence_manifest') == RESPAWN_MANIFEST:
        return ('host_respawn_outer_gates', ['port/level-world/character_respawn_outer.cpp',
                'port/level-world/character_group_respawn.cpp'], row['scope'] +
                '; live property/group ownership and hosting/Limbus timer producer remain unbound.')
    if row.get('evidence_manifest') == AGGRO_MANIFEST:
        return ('host_aggro_snapshot_dispatch', ['port/level-world/character_aggro_cleanup.cpp'],
                row['scope'] + '; host-only ordering/lifetime adapter; native map mutation, OnDeAggro AI consumers and live actor ownership remain external.')
    if row.get('evidence_manifest') == GROUP_MANIFEST:
        return ('host_group_respawn_predicate', ['port/level-world/character_group_respawn.cpp'],
                row['scope'] + '; outer respawnability/delay, group membership ownership and live SpawnFacts wiring remain external.')
    if row.get('evidence_manifest') == RANDOM_MANIFEST:
        return ('host_random_stream_adapter', ['port/level-world/character_template_random.cpp',
                'port/random/random.c'], row['port_coverage'] +
                '; caller owns ordinary seed/draw order; no live template factory claim.')
    if row.get('evidence_manifest') == TRIGGER_MANIFEST:
        return ('bounded_crypt_trigger_contact', ['port/level-world/crypt_spawn_trigger.cpp',
                'port/trigger-contact/trigger_contact.cpp',
                'port/zone-contact-runtime/zone_geometry.cpp', NATIVE], row['port_coverage'] +
                '; GhostAmbush01 offline one-player contact is live; other trigger policies remain external.')
    if row.get('evidence_manifest') == SCRIPT_MANIFEST:
        return ('bounded_crypt_script_session', ['port/level-world/crypt_spawn_script_session.cpp',
                'port/script-runtime/script_runtime.cpp', NATIVE], row['port_coverage'] +
                '; live Wait/Spawn only; full Level and ScriptManager bodies are not reconstructed.')
    if row.get('evidence_manifest') == TEMPLATE_MANIFEST:
        category = ('host_template_projection' if row['port_coverage'].startswith('bounded')
                    else 'template_dependency_evidence')
        return (category, ['port/level-world/character_template_factory.cpp'],
                row['port_coverage'] + '; host-only, not live actor factory integration.')
    if symbol.startswith(('CSLimbus::', 'CSSpawn::')):
        return ('bounded_state_kernel', STATE,
                'Limbus/Spawn service ordering, flags and registered events. '
                'Respawn/group/AI facts remain external; first-spawn Ghost path is live.')
    if symbol.startswith('Script_SpawnCharacter::'):
        return ('bounded_factory_request', ['port/level-world/character_factory.cpp'],
                'Exact-name request on already-loaded actors. No actor allocation '
                'is implemented here; separate live GhostAmbush01 session supplies authored requests.')
    if symbol.startswith('CharStateMachine::SM_SetSpawnState'):
        return ('bounded_factory_request', ['port/level-world/character_factory.cpp'],
                'The observed false,false request selects Spawn ID 1; other '
                'PreSpawn/cinematic paths remain outside this adapter.')
    if symbol.startswith(('CharStateMachine::_SetState', 'CharStateMachine::RaiseStateEvent')):
        return ('bounded_state_kernel', STATE,
                'Supported state projections, synchronous blur/focus/event '
                'dispatch. Not all registered Character states or AI consumers.')
    if symbol.startswith('VisualObject::'):
        return ('original_return_stub', [NATIVE],
                'Preserves source no-op fade boundary and raw argument. No alpha ramp.')
    if symbol.startswith('GameObject::IsUpdatable'):
        return ('obsolete_virtual_candidate_evidence', [MANIFEST],
                'Constant-true getter is not the state visibility call. '
                'Original evidence retained; no live implementation claim.')
    if symbol.startswith('GameObject::SetVisible'):
        return ('bounded_source_visibility_service', STATE + [NATIVE],
                'Limbus hides and Limbus/PreSpawn blur restores enabled-byte '
                'visibility. Selected fresh Ghosts are bound; full visual '
                'synchronization and enable/serialized producers remain external.')
    if symbol.startswith(('Character::GetCharAnimTableId', 'Character::GetCharAI',
                          'Character::GetCharType')):
        return ('resolved_data_service', [NATIVE, 'port/game-data/properties.cpp'],
                'Current cached property/table projections provide selected '
                'animation and AI type inputs. Full property ownership is incomplete.')
    if symbol.startswith('Character::InitPhysicalObject'):
        return ('bounded_native_service', [NATIVE, 'port/level-world/character_body_config.cpp'],
                'Fresh Ghost configuration and real NativeWorld body creation; '
                'body replacement and lifecycle tested. Whole Character factory is pending.')
    return ('caller_or_service_evidence', [MANIFEST],
            'Original caller/service contract recorded. Complete constructor, '
            'state registration, PreSpawn, revival, linked aggro and AI ownership '
            'are not reconstructed by this checkpoint; see NOTES.md.')


def build():
    manifests = [(path, json.loads((ROOT / path).read_text(encoding='utf-8-sig')))
                 for path in MANIFESTS]
    manifest = manifests[0][1]
    assert all(source.get('original_sha256', source.get('original_elf_sha256', source.get('original_elf', {}).get('sha256'))) == manifest['original_sha256']
               for _, source in manifests)
    pinned = json.loads((ROOT / 'docs/generated/combined-function-audit.json').read_text())
    old_addresses = {int(row['address'], 0) for row in pinned['mapped_function_starts']}
    index_path = ROOT / 'recovered/native/symbols/libDungeonHunter2.so/function-index.csv'
    with index_path.open(encoding='utf-8-sig', newline='') as stream:
        index = {int(row['address'], 0): row for row in csv.DictReader(stream)}
    by_address = {}
    records = [{**record, 'evidence_manifest': path,
                **({'manifest_implementation_scope': source.get('source_reconstruction_scope', source.get('scope', 'Bounded source adapter.'))}
                   if path in AI_MANIFESTS or path in ENGINE_MANIFESTS else {})}
               for path, source in manifests
               for record in (source['functions'].values() if isinstance(source['functions'], dict) else source['functions'])]
    for original in records:
        # Effect-state manifests retain their serialized payload provenance.
        # Normalize their function pins without inventing missing byte evidence.
        if 'elf_address' not in original:
            byte_hash = original.get('raw_function_bytes_sha256', original.get('sha256'))
            assert byte_hash, original
            original.update(elf_address=original['address'], original_symbol=original.get('original_symbol', original.get('symbol')),
                            sha256=byte_hash,
                            scope=original.get('scope', original['manifest_implementation_scope']))
        address = int(original['elf_address'], 0)
        entry = index[address]
        aliases = json.loads(entry['aliases'])
        alias = next(a for a in aliases if a['name'] == original['original_symbol'])
        # Normalize missing display names only from the verified original
        # symbol index; never infer a class/function name from a guessed label.
        if 'demangled' not in original:
            original['demangled'] = alias['demangled']
        assert int(entry['range_size']) == original['size'], original['demangled']
        relative = 'recovered/native/assembly/libDungeonHunter2.so/' + entry['assembly_file']
        listing = (ROOT / relative).read_text(encoding='utf-8-sig')
        memory = {}
        for line in listing.splitlines():
            match = re.match(r'^([0-9a-fA-F]{8})\s+((?:[0-9a-fA-F]{2}\s+)+)', line)
            if match:
                start = int(match[1], 16)
                raw = bytes.fromhex(match[2])
                for offset, value in enumerate(raw):
                    memory[start + offset] = value
        data = bytes(memory[p] for p in range(address, address + original['size']))
        assert hashlib.sha256(data).hexdigest() == original['sha256'], original['demangled']
        category, paths, scope = classify(original)
        assert all((ROOT / path).is_file() for path in paths)
        mapping = {'evidence_manifest': original['evidence_manifest'],
                   'mapping_category': category, 'source_paths': paths,
                   'implementation_limit': scope}
        if address in by_address:
            row = by_address[address]
            assert (row['size'], row['sha256']) == (original['size'], original['sha256'])
            row['evidence_manifests'].append(original['evidence_manifest'])
            row['additional_mappings'].append(mapping)
            continue
        by_address[address] = {**original, 'assembly_file': relative,
                     'in_pinned_adam_ledger': address in old_addresses,
                     'mapping_category': category, 'source_paths': paths,
                     'implementation_limit': scope,
                     'evidence_manifests': [original['evidence_manifest']],
                     'additional_mappings': []}
    rows = list(by_address.values())
    addresses = {int(row['elf_address'], 0) for row in rows}
    assert len(addresses) == len(rows)
    return {
        'schema': 'dh2-character-runtime-function-map/v3',
        'scope': 'Original evidence and bounded implementation extension; '
                 'counts do not measure game completion or fully reconstructed functions.',
        'original_library_sha256': manifest['original_sha256'],
        'adam_commit': pinned['provenance']['adam_commit'],
        'pinned_ledger': 'docs/generated/combined-function-audit.json',
        'extension_manifests': MANIFESTS,
        'counts': {'pinned_addresses': len(old_addresses),
                   'extension_records': len(rows),
                   'evidence_records_before_deduplication': len(records),
                   'additional_addresses': len(addresses - old_addresses),
                   'combined_unique_addresses': len(old_addresses | addresses)},
        'verification': f'All ranges, aliases and {len(rows)} complete original byte hashes '
                        'verified against the original symbol index and assembly exports.',
        'functions': rows,
        'unsupported': list(dict.fromkeys(value for _, source in manifests
                                         for value in source.get('unsupported', []))),
    }


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--check', action='store_true', help='Verify the saved map matches current evidence')
    args = parser.parse_args()
    result = build()
    target = ROOT / OUTPUT
    if args.check:
        assert json.loads(target.read_text()) == result, 'saved map differs; regenerate and review'
    else:
        target.write_text(json.dumps(result, indent=2) + '\n', encoding='utf-8')
    print(json.dumps({'validation': 'PASS', **result['counts'], 'report': OUTPUT}))


if __name__ == '__main__':
    main()
