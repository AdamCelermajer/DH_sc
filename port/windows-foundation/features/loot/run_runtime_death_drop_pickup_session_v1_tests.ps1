param([string]$RepositoryRoot = (Resolve-Path (Join-Path $PSScriptRoot '../../../..')).Path)
$ErrorActionPreference = 'Stop'
$root = [IO.Path]::GetFullPath($RepositoryRoot)
$compiler = Join-Path $root '.local-inputs/windows-toolchain/llvm-mingw-20261006-ucrt-x86_64/bin/clang++.exe'
$output = Join-Path $root '.local-inputs/runtime_death_drop_pickup_session_v1_tests.exe'
$sources = @(
    'port/windows-foundation/features/loot/runtime_death_drop_pickup_session_v1_tests.cpp',
    'port/windows-foundation/features/loot/runtime_death_rewards_v1.cpp',
    'port/windows-foundation/features/loot/runtime_session_death_rewards_v1.cpp',
    'port/windows-foundation/features/loot/runtime_loot_source_owner_v1.cpp',
    'port/windows-foundation/features/loot/runtime_world_item_adapter_v1.cpp',
    'port/windows-foundation/features/loot/runtime_world_item_interaction_v1.cpp',
    'port/windows-foundation/features/inventory/inventory_feature.cpp',
    'port/windows-foundation/features/frontend/creation/runtime_creation_source_loader_v1.cpp',
    'port/windows-foundation/save_store.cpp',
    'port/level-world/player_progression_v1.cpp',
    'port/game-data/loot_table_selection_v8.cpp',
    'port/game-data/loot_item_selection_v8.cpp',
    'port/game-data/loot_entry_selection_v8.cpp',
    'port/game-data/loot_power_creation_v7.cpp',
    'port/game-data/loot_power_resources_v7.cpp',
    'port/game-data/loot_audiovisual_v8.cpp',
    'port/game-data/item_power_tables_v5.cpp',
    'port/game-data/item_presentation_v5.cpp'
)
$includes = @(
    'port/windows-foundation', 'port/game-data', 'port/level-world',
    'port/engine-animation', 'port/engine-skinning', 'port/scene-materials',
    'port/script-runtime', 'port/physics-backend/box2d-2.0.1/Include'
)
$arguments = @('-std=c++17','-O1','-Wall','-Wextra','-Werror','-Wno-misleading-indentation','-Wno-missing-field-initializers')
$arguments += $includes | ForEach-Object { '-I' + (Join-Path $root $_) }
$arguments += $sources | ForEach-Object { Join-Path $root $_ }
$arguments += ('-L' + (Join-Path $root '.local-inputs/windows-foundation-build'))
$arguments += ('-L' + (Join-Path $root '.local-inputs/windows-foundation-build/physics-backend'))
$arguments += @('-static','-lfoundation_frontend','-lfoundation_data','-lcontent_xml','-ldh2_freetype237',
    '-lrecovered_trigger_contacts','-lrecovered_content','-ldh2_box2d_201',
    '-lkernel32','-luser32','-lgdi32','-lwinspool','-lshell32','-lole32',
    '-loleaut32','-luuid','-lcomdlg32','-ladvapi32','-o',$output)
& $compiler @arguments
if ($LASTEXITCODE -ne 0) { throw "Death/drop/pickup session compile failed ($LASTEXITCODE)" }
$lootAssetRoot = Join-Path $root '.local-inputs/windows-source-clock-v19-preview-9/assets'
$sourceRngSeed = 37
$resultText = & $output $root $sourceRngSeed
if ($LASTEXITCODE -ne 0) { throw "Death/drop/pickup session test failed ($LASTEXITCODE): $resultText" }
$result = $resultText | ConvertFrom-Json
if ($result.validation -ne 'PASS') { throw 'Death/drop/pickup session audit did not pass' }
$suppressedText = & $output $root $sourceRngSeed 'suppressed'
if ($LASTEXITCODE -ne 0) { throw "Suppressed Level reward test failed ($LASTEXITCODE): $suppressedText" }
$suppressedResult = $suppressedText | ConvertFrom-Json
if ($suppressedResult.validation -ne 'PASS') { throw 'Suppressed Level reward audit did not pass' }
$hash = { param($path) (Get-FileHash -Algorithm SHA256 -LiteralPath (Join-Path $root $path)).Hash.ToLowerInvariant() }
$report = [ordered]@{
    validation = 'PASS'
    version = 3
    test = $result
    suppressed_test = $suppressedResult
    source_sha256 = [ordered]@{}
    original_binary_sha256 = & $hash '.local-inputs/libDungeonHunter2.so'
    loot_cache_sha256 = (Get-FileHash -Algorithm SHA256 -LiteralPath (Join-Path $lootAssetRoot 'original-cache/data/pydata/loot_table_pyarray.bin')).Hash.ToLowerInvariant()
    design_cache_sha256 = (Get-FileHash -Algorithm SHA256 -LiteralPath (Join-Path $lootAssetRoot 'original-cache/data/pydata/design_pyarray.bin')).Hash.ToLowerInvariant()
    loot_source_asset_root = '.local-inputs/windows-source-clock-v19-preview-9/assets'
    original_visual_asset_root = '.local-inputs/windows-shared-assets'
    scope = 'CPU/native linked end-to-end host fixture: actual initialized CombatSession emits a lethal DamageEvent from source enemy Swamp_LizadMan_Type1; unmodified victim property9 selects its authored Loot row and property35 supplies XP. RuntimeSessionDeathRewardsV1 binds the existing same-source RuntimeCreationSourceOwnerV1 and one existing RuntimeWorldItemAdapterV1, resolves caller current/unlocked difficulty and Debug/CharacterState providers, and runs RuntimeDeathRewardsV1 under the same PlayableActorWorld LootRandom8 stream. Source Potion0 transfers once to the same CharacterState; repeat death and stale restore lease are rejected. No main/CMake/native ItemObject graph or visual presentation claim.'
    source_evidence = [ordered]@{
        session_death = 'CombatSession is initialized with actual source property/AI/animation data and emits the lethal event through its live actor/combat runtime; RuntimeSessionDeathRewardsV1::after_update consumes that same session event batch.'
        loot = 'The source profile Swamp_LizadMan_Type1 is not edited: its live resolved property9 is 230, authored name Swamp_Basic_Loot, and resolved property35 is raw 1894. Host obtains the LootRandom8 loan from that same session PlayableActorWorld via with_loot_random, then passes it through original LootTableSelectionV8/LootItemSelectionV8. Current/unlocked difficulty is read from the bound caller owner, OneKillLevelUp is queried at the source XP award gate, Loot Debug requests are forwarded to the bound SourceScopes owner, and class counts are derived from live actors and CharacterTable IDs.'
        reward = 'The same real lethal DamageEvent mutates the same player CharacterState through source XP formula kernels and publishes exact selected ItemTable row/quantity into the same RuntimeWorldItemAdapterV1.'
        audio_event_rows = 'Source audiovisual expectation for Swamp_LizadMan_Type1 is Injury row377 step0 Sound477 and Death row374 step0 Sound476. The current audio feature report/test maps Sound477 to UID284 sfx_lizardman_hurt.wav; row377 is not a Big88 alias.'
        pickup = 'Potion0 item925 has source pickup word3=1, ItemTable type14 and equipment word26=-1; the existing RuntimeWorldItemInteractionV1 exact-ID admission routes it to Presenter pickup, removes one drop, and rejects duplicate delivery.'
        repeat = 'A second host after_update(session) reads the same retained event batch; ActorId/lifecycle receipt suppresses XP, item publication and LootRandom8 advancement after the first successful reward. An expired session actor-binding lease rejects dispatch during restore and after rebind; explicit host reset/rebind accepts the new session lease without replaying the cleared death batch.'
    }
    source_visual_audit = [ordered]@{
        references_inspected = @(
            '.local-inputs/front-v87-merge/source/port/android-native/app/src/main/assets/original-media/intro.mp4 (50.29 s; intro only)',
            '.local-inputs/frontend-feature-build/showcase-knight.mp4, showcase-mage.mp4, showcase-rogue.mp4 (about 4 s each; character showcases)',
            '.local-inputs/actor-states-tests-*/*Died-*.png (reconstructed state-test frames, not original gameplay)'
        )
        observation = 'No supplied original gameplay sequence or screenshot showing enemy death, loot appearing, and pickup was found. The available video does not show this behavior; the synthetic death-state frames are not used as original visual evidence.'
        visual_claim = 'The current native CPU fixture validates source gameplay state and item transfer only; world-item rendering and integrated visual pickup remain unverified.'
    }
    source_logic_audit = [ordered]@{
        recovered_handoff = 'port/level-world/reference/character-loot-world-v8/renderer-integration.md and original-source.asm / original-functions.json; port/level-world/reference/character-kill/NOTES.md and original-functions.asm / death-routing-functions.asm.'
        death_and_admission = 'Character::Kill source routing performs early DropLoot only when Level loot_gate150 is zero, before credited DistributeXP; repeated IsDead/death-animation callbacks are not independent reward admissions. Level constructors 0x3f3128 and 0x3f34c0 initialize Level member84 (byte offset 0x150) to zero; Character::Kill 0x3a5b18 gates DropLoot on that byte and Character::AddExperience near 0x138950 rejects a nonzero value. The gate therefore suppresses both loot and XP. The host reads a typed current gameplay owner bool and consumes admitted target_died events once per actor binding lifecycle.'
        level_gate_uncertainty = 'The available native Level-method scan found no additional writers, but did not exhaustively prove that external callers/scripts never write byte 0x150. The feature receives the current gameplay owner value and does not require a Native Level graph or VM.'
        loot_calls = 'Character::GetLoot 0x3a2fcc reads resolved property9 (Character+0x101c); Character::DropLoot 0x3a5ae4 calls it and ItemObject::DropLootTable 0x3ecba0. Actual fixture source row Swamp_LizadMan_Type1 has property9=230 (Swamp_Basic_Loot), property35 raw=1894.'
        pickup_calls = 'ItemObject::Interact 0x3ed144 and _DoAutoPickupHack 0x3ec474; GetPickUpType 0x3f9e34. Native path includes owner/lock/player admission, AutoTransmute, inventory/potion capacity, transfer, text/tutorial, statistics/FX/audio/tooltip and despawn; generic adapter only claims the source-supported item transfer/admission subset.'
        party_classification = 'PlayerManager class-count source at 0x36ea50 compares Character::InitPre cached CharacterTable base ID +0x13c8 against 263/290/325; port/windows-foundation/features/loot/runtime_loot_source_owner_v1.cpp implements the same row-ID classifier.'
    }
    save_reload_audit = [ordered]@{
        xp_source = 'Character::Kill 0x3a5b18 routes credited kills to Character::DistributeXP 0x3bf828 only after its attacker/credit gates; the source capture notes null attacker skips XP. player_progression_v1.cpp models GiveXP gates and queries OneKillLevelUp after cap/player/remote/current-Level checks; unlocked<current changes the raw award to 256. The generic runtime does not claim native Character/SG Save delivery.'
        character_codec = 'save_character/load_character in port/windows-foundation/save_store.cpp atomically writes a detached DHSave format v1 carrying CharacterState schema3. encode includes integer experience, gold, and inventory instance_id/definition_id/quantity. load decodes/validates a candidate and assigns the destination only after success; failure leaves destination unchanged. This is the existing generic CharacterState codec, not an original native save serializer.'
        gameplay_restore = 'capture_game_save/restore_game_save in game_save.cpp persist CharacterState plus actor roster and world RNG; reward receipts and CombatSession events are not part of GameSave. CombatSession::detach_for_restore and rebind_after_restore clear event batches and replace the binding lease; the reward host requires explicit reset/rebind.'
        expected = 'After real source XP and source item pickup, saving then loading into the same canonical CharacterState preserves integer XP, gold and exact Potion0 inventory identity/quantity; a completed death is not replayed after Session restore/rebind.'
        focused_test = 'Run the real Swamp_LizadMan_Type1 death through the bound host with same-world RNG diagnostic seed37. Its unmodified property9=230 source row selects Potion0 and GoldStack01 item418 (resolved source value3). Pick up both, roundtrip the same CharacterState through isolated save_character/load_character, then verify exact XP/gold/Potion instance and quantity equality plus no reward replay after restore/reset/rebind.'
        uncertainty = 'CharacterState.experience stores whole XP while source property33 is fixed-point. The generic save codec does not preserve that sub-integer remainder; this test asserts the exposed CharacterState fields only. No original visual save/reload footage was supplied.'
    }
    level_reward_gate_audit = [ordered]@{
        visual_evidence = 'The Level+0x150 admission flag is invisible. Existing source-backed same-CombatSession death/drop/pickup evidence validates the visible gameplay invariant; no supplied original screenshot/video directly exposes the flag.'
        logic_evidence = 'Level constructors 0x3f3128 and 0x3f34c0 initialize member84 (byte offset 0x150) to zero. Character::Kill 0x3a5b18 gates loot on the field and Character::AddExperience near 0x138950 rejects nonzero. This is one shared source gate for loot and XP.'
        expected_behavior = 'A nonzero current gameplay value consumes a new death once with no loot, XP, or Loot RNG advancement; clearing it later does not replay the death. Ordinary constructor-zero continues through existing rewards.'
        implementation = 'RuntimeSessionDeathRewardsV1 reads a typed admission value before dispatch; RuntimeDeathRewardsV1 records suppressed deaths as completed before killer/property/RNG work. The feature does not create or require a Native Level owner.'
        uncertainty = 'The Level-method scan was not exhaustive proof against external callers/scripts writing the byte.'
        verification = 'The paired host runs exercise ordinary false admission and source-gated true admission on the actual Swamp_LizadMan_Type1 CombatSession death. The true run checks one-time consumption, unchanged XP/drop/RNG, and no retroactive payout after the owner clears the bool.'
    }
    expected_behavior = 'A live source enemy death admitted by the original Level loot gate selects the authored Loot table with the gameplay world RNG, applies source XP and publishes selected items at the victim position. An eligible explicit pickup transfers once into the same canonical character inventory; duplicate delivery is rejected.'
    uncertainties = @(
        'No original visual capture of this sequence was supplied, so timing/render appearance is not directly observed.',
        'This feature helper is not yet enrolled in the production main/GUI update loop; the caller must supply actual current/unlocked difficulty and Debug queries, source creation/menu owners, canonical CharacterState leases, and the existing gameplay item store.',
        'Native ItemObject owner/lock/UI side effects remain outside this generic world-item adapter; full root integration must preserve those where exposed.'
    )
    focused_verification = [ordered]@{
        test = 'run_runtime_death_drop_pickup_session_v1_tests.ps1: real CombatSession Swamp_LizadMan_Type1 death at diagnostic seed37; authored property9/35; same world RNG; caller current/unlocked difficulty and Debug requests; source Potion0 and GoldStack01 item418 value3 pickup; save_character/load_character XP+gold+inventory roundtrip; repeat death suppression; restore lease rejection and clean reset/rebind.'
        status = 'Feature-owned host CPU/native test PASS; helper is composed and directly exercisable, but production main/GUI integration and visual playback remain unverified.'
    }
    implementation_status = [ordered]@{
        investigation = 'Existing recovered source and linked same-session fixture reviewed; applicable AGENTS.md evidence audit recorded above.'
        feature_implementation = 'RuntimeSessionDeathRewardsV1 composes RuntimeCreationSourceOwnerV1, RuntimeLootSourceOwnerV1, RuntimeDeathRewardsV1, the existing RuntimeWorldItemAdapterV1, caller difficulty/Debug/CharacterState callbacks, and CombatSession-owned RNG without creating another source owner/store.'
        isolated_verification = 'Feature-owned source-backed CPU/native host test; runner result and exact source hashes below.'
        integrated_runtime_visual_verification = 'Not integrated into production main/GUI loop and not visually verified.'
        remaining_limit = 'Main/GUI must enroll this helper with the actual current-menu difficulty, unlocked-save difficulty, SourceScopes Debug callbacks, canonical CharacterState resolver, and existing gameplay store. No main/CMake/core changes here.'
    }
    limitations = @(
        'This is a native linked CPU fixture over source-backed tables/animation data, not production root Session/main enrollment or a full native ItemObject/ObjectManager graph.',
        'The diagnostic RNG seed is a deterministic test input; the tested loot table and XP are read from the original source enemy profile and the stream is borrowed from the same CombatSession world. The test does not claim root main/session enrollment.',
        'The feature host now accepts the current gameplay owner typed Level reward-suppression bool; production main must provide the real current value. The helper does not require a Native Level graph and no external script-write audit is claimed.'
    )
}
foreach ($path in @($sources + 'port/windows-foundation/features/loot/runtime_death_rewards_v1.hpp' + 'port/windows-foundation/features/loot/runtime_session_death_rewards_v1.hpp' + 'port/windows-foundation/features/loot/runtime_session_death_rewards_v1.cpp' + 'port/windows-foundation/features/loot/runtime_loot_source_owner_v1.hpp' + 'port/windows-foundation/features/loot/runtime_world_item_adapter_v1.hpp' + 'port/windows-foundation/features/loot/runtime_world_item_interaction_v1.hpp' + 'port/windows-foundation/features/frontend/creation/runtime_creation_source_loader_v1.hpp' + 'port/windows-foundation/features/frontend/creation/runtime_creation_source_loader_v1.cpp' + 'port/windows-foundation/combat_session.hpp' + 'port/windows-foundation/combat_session.cpp' + 'port/windows-foundation/playable_actor_world.hpp' + 'port/windows-foundation/playable_actor_world.cpp' + 'port/game-data/loot_tables_v2.hpp' + 'port/game-data/loot_tables_v2.cpp' + 'port/game-data/design_settings.hpp' + 'port/game-data/design_settings.cpp' + 'port/level-world/reference/character-loot-world-v8/original-source.asm' + 'port/level-world/reference/character-loot-world-v8/original-functions.json' + 'port/level-world/reference/player-manager-owner-v1/original-source.asm')) {
    $report.source_sha256[$path] = & $hash $path
}
$report.source_sha256['port/windows-foundation/save_store.hpp'] = & $hash 'port/windows-foundation/save_store.hpp'
$report.source_sha256['port/windows-foundation/game_save.cpp'] = & $hash 'port/windows-foundation/game_save.cpp'
$report.source_sha256['port/windows-foundation/game_save.hpp'] = & $hash 'port/windows-foundation/game_save.hpp'
$report.source_sha256['port/windows-foundation/character_state.hpp'] = & $hash 'port/windows-foundation/character_state.hpp'
$report.actual_loot_asset_sha256 = [ordered]@{}
foreach ($path in @('original-cache/data/pydata/loot_table_pyarray.bin','original-cache/data/pydata/loot_table_pyarraynames.bin',
    'original-cache/data/pydata/loot_table_pystructnames.bin','original-cache/data/pydata/item_powers_pyarray.bin',
    'original-cache/data/pydata/item_powers_pyarraynames.bin','original-cache/data/pydata/item_powers_pystructnames.bin',
    'original-cache/data/pydata/item_powers_monopoly_pyarray.bin','original-cache/data/pydata/item_powers_monopoly_pyarraynames.bin',
    'original-cache/data/pydata/item_powers_monopoly_pystructnames.bin','data/loot_audiovisual_pyarray.bin',
    'data/loot_audiovisual_pyarraynames.bin','data/loot_audiovisual_pystructnames.bin',
    'original-cache/data/pydata/design_pyarray.bin','original-cache/data/pydata/design_pyarraynames.bin',
    'original-cache/data/pydata/design_pystructnames.bin','original-cache/data/pydata/design_pycst.bin')) {
    $report.actual_loot_asset_sha256[$path] = (Get-FileHash -Algorithm SHA256 -LiteralPath (Join-Path $lootAssetRoot $path)).Hash.ToLowerInvariant()
}
$reportPath = Join-Path $root 'port/windows-foundation/reports/runtime-death-drop-pickup-session-v1.json'
$report | ConvertTo-Json -Depth 8 | Set-Content -LiteralPath $reportPath -Encoding utf8
$resultText
