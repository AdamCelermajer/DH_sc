import json
from pathlib import Path

# Historical snapshot writer. The current provider-frontier updater supersedes
# this file; do not replay older native progress over verified new receipts.
raise SystemExit('Historical updater retired. Run provider-frontier-20261009.py instead.')

ROOT = Path(__file__).resolve().parents[2]
path = ROOT / 'port/windows-foundation/reports/integration-task-board.json'
board = json.loads(path.read_text(encoding='utf-8-sig'))
board['accepted_release'] = {
    'path': '.local-inputs/windows-source-clock-checkpoint-v18',
    'sha256': '79e566435081650206a03ecb028736a97770fa28d38722438a1a90645e7819d8',
    'tests': 75, 'files': 572, 'runtime_cases': 29, 'negative_saves': 2,
    'modified': False, 'scope': 'Verified Stats and combat-text subset; full character menu incomplete',
    'receipt': 'port/windows-foundation/reports/menu-combat-text-integration-v18.json',
    'previous_release_preserved': '.local-inputs/windows-source-clock-checkpoint-v17',
}
board['native_bootstrap_frontier'] = {
    'root_verified_tests': 78,
    'proven': 'Character and Player catalog dispatch, actual manager Add/PropertyMap/ConditionData/App RNG, actual same-context Renderer Full/ReducedOptional policy',
    'first_required_leaf': 'RetainedCharacterFamilyVisualV6 SAME World/SceneManager/update_pf services',
    'exact_existing_provider': 'renderer_character_campaign_v62.inc: same record.services.world, actual roots, weak same candidate -> canonical_character_update_pf_v62(record,floors.collision_world,navigation.registry,error)',
    'world_identity': 'Production WorldScriptContext actual_world equality with record.services.world; fixture make_shared<int>(1) is not live world readiness',
    'root_owner': 'Actual navigation/PF, native fixture/build/closure and host publication',
    'worker_owner': '/root/integration_lead/source_factory_luna: NEW source_character_owner_factory_visual_binding* only',
    'device_scope': 'Only GameObject optional visual-loading behavior resolved through honest in-house backend policy; original Device vtable/global capabilities not claimed',
    'still_required': ['real source floor/nav owners', 'actual visual asset load and Sync', 'Character InitPost script/FSM', 'PlayerSave/Gear/skills initialization', 'whole InitFinal', 'host publication'],
}
updates = {
    'source_character_factory': ('source_factory_luna', 'concrete_visual_PF_provider_COFFERrorClean_actual_candidate_cache_BRES_test_PASS_root_full_LoadVisual_pending'),
    'equipment': ('equipment_luna', 'active_native_Gear_main_page_binding_and_source_sheet_transaction'),
    'inventory': ('inventory_luna', 'active_native_Gear_existing_Text_Power_inventory_index_binding'),
    'skill_ui': ('skill_menu_luna', 'active_fresh_native_closure_three_arg_train_probe_mutation_acceptance'),
    'faery_menu': ('faery_menu_luna', 'active_same_native_queries_actions_Save_FaeryTables_binding'),
    'class_skills': ('skill_session_luna', 'actual_record_loan_consumer_COFF_compile_ready_completed_graph_pending'),
}
for task in board['tasks']:
    if task['id'] in updates:
        owner, status = updates[task['id']]
        task.update(owner='/root/integration_lead/' + owner, status=status, model='gpt-6-luna', reasoning_effort='high', runtime_accepted=False)
    if task['id'] == 'combat_text':
        task['status'] = 'v18_scoped_render_accepted_full_native_SCT_boundary_incomplete'

board['full_menu_acceptance'] = {
    'complete': False,
    'required_pages': ['Stats', 'Equipment', 'Skills', 'Faery', 'Quest', 'Map'],
    'each_page_requires': ['authored original art', 'same native authoritative values', 'source actions', 'same native Save behavior', 'reference proof', 'actual root runtime binding'],
    'unbound_page_policy': 'Preserve currently valid page and report required provider before selecting unbound content',
    'frozen_v18_art': True,
    'scope_receipt': 'port/windows-foundation/reports/act1-feature-coverage.json',
}
board['act1_scope'] = {
    'source': 'port/windows-foundation/reports/act1-feature-coverage.json',
    'complete': False,
    'priority': 'Canonical shared native owners -> mob combat/death/reward loop, alongside full menu',
    'tracks': {
        'enemy_ai': 'act1_enemy_luna: source lifecycle and existing V1 DoSkill binding; InitPost Stage6 pending',
        'loot_money_xp': 'act1_loot_luna: reuse CharacterLootLiveV22/KillRewardsV31/ProgressionWorldV23; live Level150/itempool145/Gear/save pending',
        'pots_chests_NPC': 'act1_interactions_luna: canonical container + actual source animation/scripts/conditional loot; some asset/script bytes absent',
        'quests_Quest_page': 'act1_quests_luna: actual QuestLog and SAME Level objective/event projection; live bootstrap/rewards pending',
        'minimap_Map_page': 'act1_map_luna: source655 geometry; actual Show/RenderMap projection and camera/markers still required',
        'companions': 'act1_companions_luna: real V1 source master/FSM/Lua ownership; Castor identity/attack unverified',
        'cinematics_tutorials': 'act1_cinematic_luna: authored campaign camera/trigger/HUD owners required; no frame schedules',
        'event_audio': 'audio_luna: one real V42 + hardware output, source ignored playback-return boundaries; live enrollment pending',
        'skills_trails_projectiles': 'skill_session_luna: actual Session clips proved; native VM/FSM/mana/damage binding pending, effects enrollment required',
        'progression_barriers': 'Source verification and native animation/condition owner assignment still required; witch/waterfall recollection unverified',
    },
    'rules': 'Reusable whole-game systems, original probabilities/conditions; no guessed timings, all-mob activation or fake readiness',
}
board['model_transition_complete'] = True
board['current_worker_policy'] = {'model': 'gpt-6-luna', 'reasoning_effort': 'high', 'active_escalations': 0, 'root_and_lead_exempt': True}
board['integration_assignment_snapshot'] = [
    {'owner': '/root/integration_lead/equipment_luna', 'task': 'Native Gear action binding and actual source sheets', 'path': 'features/equipment'},
    {'owner': '/root/integration_lead/inventory_luna', 'task': 'Existing Gear Text and SourceItemResources presentation binding', 'path': 'features/inventory'},
    {'owner': '/root/integration_lead/skill_menu_luna', 'task': 'Fresh current-ABI native connected train probe/mutation fixture', 'path': 'features/skill_ui and exclusive named WSL snapshot'},
    {'owner': '/root/integration_lead/faery_menu_luna', 'task': 'Same native query/action/Save/FaeryTables page binding', 'path': 'features/faery_menu'},
    {'owner': '/root/integration_lead/act1_enemy_luna', 'task': 'Real scoped DoSkill0x3b8bd8 admission and genuine VM regression', 'path': 'character_script_session.cpp Impl::binding exact branch + features/enemy_ai'},
    {'owner': '/root/integration_lead/act1_interactions_luna', 'task': 'Actual container native link/run; NPC quest event connection', 'path': 'features/interactions'},
    {'owner': '/root/integration_lead/act1_quests_luna', 'task': 'SAME EventManager/Objectives TalkToNPC source integration fixture', 'path': 'features/quests'},
    {'owner': '/root/integration_lead/act1_map_luna', 'task': 'Recover real Show/RenderMap/camera source kernels', 'path': 'features/map_ui'},
    {'owner': '/root/integration_lead/act1_barriers_luna', 'task': 'Original progression Door identities/conditions/animation owner binding', 'path': 'features/progression_barriers'},
]
for item in board['integration_assignment_snapshot']:
    item.update(model='gpt-6-luna', reasoning_effort='high')
board['workforce_authority'] = {
    'owner': '/root/workforce_dispatcher',
    'path': 'coordination/workforce-dispatcher/',
    'responsibility': 'Only live utilization, ready queue and completion-to-next-task dispatch; integration lead reviews providers and coordinates grants',
    'capacity': {'total_slots': 25, 'root_and_integration_lead': 2, 'dispatcher': 1, 'feature_workers_max': 22, 'workers_including_dispatcher': 23},
    'duplicate_dispatcher': '/root/integration_lead/workforce_dispatcher_luna interrupted immediately after root direct dispatcher creation; no active duplicate authority',
    'roster_note': 'This integration board is not a live active-worker count. Use dispatcher actual list_agents reconciliation.',
}
board['ready_native_skill_binding'] = {
    'receipt': 'port/windows-foundation/features/skills_animation/canonical_session_skill_binding.json',
    'verification': 'Real Windows COFF object + actual typed canonical record loan contract compile; exact link closure recorded',
    'state': 'frozen_ready_for_completed_native_graph_publication',
    'runtime_accepted': False,
}
board['shared_navigation_storage'] = {
    'source': 'port/windows-foundation/source_navigation_world_storage.hpp',
    'receipt': 'reports/navigation-world-storage-integration.json',
    'root_verification': 'Actual main default90 position exact v18 parity; SAME sewn floors and one obstacle registry',
    'scope': 'Typed PF lifetime capsule, not complete source Level; native graph must borrow SAME capsule',
}
board['next_visual_asset'] = {
    'report': 'port/windows-foundation/reports/source-character-owner-factory-visual-binding.json',
    'source': 'CharacterProperties419 ModelFile field3=74 -> model dictionary74 priest_good',
    'uri': 'data/3D/characters/npcs/Priest_good.bdae',
    'actual_file': '.local-inputs/windows-shared-assets/original-cache/data/3d/characters/npcs/priest_good.bdae',
    'bytes': 42136,
    'sha256': '880ed0be28e158dc62779a1fd5f9874cbd39fb68edc31af69972aceb8979e8bf',
    'staged': False,
    'fixture_pydata_root_contains_asset': False,
}
board['visual_binding_ready'] = {
    'header': 'port/windows-foundation/features/actor_frame/source_character_owner_factory_visual_binding.hpp',
    'implementation': 'port/windows-foundation/features/actor_frame/source_character_owner_factory_visual_binding.cpp',
    'verification': 'Strict C++20 COFF provider compile; actual CandidateCache layered source reader and Priest BRES runtime test PASS',
    'ownership': 'Strong SAME nav/roots/cache; weak Application-owned canonical manager freshly locked; weak same record PF callback',
    'scope': 'Concrete source service composition; no full LoadVisual or PF registration claim before root native fixture',
}
board['canonical_skill_loan_portability'] = {
    'report': 'reports/canonical-skill-context-portability.json',
    'verification': 'Existing source FSM TU Windows COFF compile with temporary platform type declarations and finite alias',
    'unresolved_android_JNI_symbols': 0,
    'unresolved_native_symbols': 259,
    'renderer_callbacks': 18,
    'scope': 'Record overload avoids Android world lookup; actual initialized context and native provider link closure still required',
}
board['npc_quest_objective_connection'] = {
    'test': 'features/quests/npc_talk_objective_integration_test.cpp',
    'verification': 'Actual64Quest rows Abbey_Rescue SAME Save/Objective/Level EventManager synchronous scoped QE_TalkToNPC mutation and listener order PASS; independent interactions rerun PASS',
    'not_proven': ['full Player profile', 'full quest state transition', 'reward ordering'],
}
board['unassigned_dependencies'] = [d for d in board.get('unassigned_dependencies', []) if d.get('id') != 'loot']
path.write_text(json.dumps(board, indent=2) + '\n', encoding='utf-8')
print('Updated accepted v18, native frontier, active menu owners and Act1 scope')
