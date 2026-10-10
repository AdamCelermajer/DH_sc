import json
from pathlib import Path
root = Path(__file__).resolve().parents[2]
path = root / 'port/windows-foundation/features/actor_frame/source_character_owner_factory_native_result.json'
result = json.loads(path.read_text(encoding='utf-8-sig'))
result['historical_355_unit_prefix'] = {
    'note': 'Previous build/runtime and load_prefix below are historical, superseded by root Player dispatch and Renderer policy integration receipts.',
    'device_missing_and_player_slot_absent_claims_current': False,
}
result['status'] = 'actual_357_unit_Player_Character_InitPost_renderer_policy_prefix_pass_visual_world_PF_pending'
result['latest_root_evidence'] = {
    'dispatch': 'reports/player-factory-dispatch-integration.json',
    'renderer_policy': 'reports/renderer-game-object-quality-integration.json',
    'source_units': 357,
    'native_runtime_exit': 0,
    'foundation_tests': 78,
    'first_required_leaf': 'Required SAME family Character/World/SceneManager/PF owners',
    'proven': 'Both Character and Player catalog aliases traverse actual receiver/Add/PropertyMap/ConditionData/App RNG and actual same-context WGL renderer Full/ReducedOptional visual policy into existing native LoadVisual.',
    'scope': 'GameObject optional asset-load policy is an honest in-house observable equivalent; exact original Device driver-vtable/global capability execution not claimed.',
    'still_not_claimed': ['complete visual/PF initialization', 'InitPost completion', 'InitFinal', 'completed Player profile/Save/Gear/skills', 'host publication'],
}
result['minimal_integratable_owner'] = 'Genuine V60 Character/Player constructor/Add/load/condition/RNG prefix and root renderer quality policy; next actual shared WorldScriptContext, roots, sewn floors::World and navigation::ObstacleRegistry service graph for native family visual/updatePF. No opaque make_shared<int> world readiness or successful no-op PF.'
path.write_text(json.dumps(result, indent=2) + '\n', encoding='utf-8')
print('Recorded current 357-unit frontier and superseded historical Device/Player-slot gaps')
