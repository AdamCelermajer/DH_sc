param([string]$RepositoryRoot = (Resolve-Path (Join-Path $PSScriptRoot '../../../..')).Path)
$ErrorActionPreference = 'Stop'
$root = [IO.Path]::GetFullPath($RepositoryRoot)
$compiler = Join-Path $root '.local-inputs/windows-toolchain/llvm-mingw-20261006-ucrt-x86_64/bin/clang++.exe'
$output = Join-Path $root '.local-inputs/runtime_session_potion_use_v1_tests.exe'
$sources = @(
    'port/windows-foundation/features/inventory/runtime_session_potion_use_v1_tests.cpp',
    'port/windows-foundation/features/inventory/runtime_session_potion_use_v1.cpp',
    'port/level-world/character_skill_application_v6.cpp',
    'port/level-world/character_skill_attack_v6.cpp',
    'port/level-world/character_hit_v6.cpp',
    'port/level-world/character_buffs.cpp',
    'port/level-world/character_combat_result_v6.cpp',
    'port/level-world/character_target_providers.cpp',
    'port/level-world/character_timers.cpp'
)
$includes = @(
    'port/windows-foundation', 'port/game-data', 'port/level-world',
    'port/engine-animation', 'port/engine-skinning', 'port/scene-materials',
    'port/script-runtime', 'port/physics-backend/box2d-2.0.1/Include'
)
$arguments = @('-std=c++17','-O1','-Wall','-Wextra','-Werror','-Wno-misleading-indentation','-Wno-missing-field-initializers','-ffunction-sections','-fdata-sections')
$arguments += $includes | ForEach-Object { '-I' + (Join-Path $root $_) }
$arguments += $sources | ForEach-Object { Join-Path $root $_ }
$arguments += ('-L' + (Join-Path $root '.local-inputs/windows-foundation-build'))
$arguments += ('-L' + (Join-Path $root '.local-inputs/windows-foundation-build/physics-backend'))
$arguments += @('-Wl,--gc-sections','-static','-lfoundation_frontend','-lfoundation_data','-lcontent_xml','-ldh2_freetype237',
    '-lrecovered_trigger_contacts','-lrecovered_content','-ldh2_box2d_201',
    '-lkernel32','-luser32','-lgdi32','-lwinspool','-lshell32','-lole32',
    '-loleaut32','-luuid','-lcomdlg32','-ladvapi32','-o',$output)
& $compiler @arguments
if ($LASTEXITCODE -ne 0) { throw "Potion-use session compile failed ($LASTEXITCODE)" }
$resultText = & $output $root
if ($LASTEXITCODE -ne 0) { throw "Potion-use session test failed ($LASTEXITCODE): $resultText" }
$result = $resultText | ConvertFrom-Json
if ($result.validation -ne 'PASS') { throw 'Potion-use session audit did not pass' }
$hash = { param($path) (Get-FileHash -Algorithm SHA256 -LiteralPath (Join-Path $root $path)).Hash.ToLowerInvariant() }
$report = [ordered]@{
    validation = 'PASS'
    version = 1
    test = $result
    source_sha256 = [ordered]@{}
    source_evidence = [ordered]@{
        input = 'platform_input::SemanticInput maps PC key5 to ButtonEdges::potion; semantic_input reports rising pressed/held/released edges. live_dispatch.cpp invokes the potion callback only on `.pressed` after validating the bound controller. The inspected caller gates blocked/locked control; this API is called after that admission and does not add an invented timer/cooldown.'
        source_action = 'port/android-native/app/src/main/cpp/gameplay_hud.cpp:73-85 (recovered source snapshot) uses same-player active/not-dead admission, signed source potion count >0, and requires at least HP or MP below its source maximum. It calls RemoveOnePotion, adds 256 to source property219, then calls RegenHP(-1) before RegenMP(-1). Count >99 reaches a separate trophy continuation.'
        inventory = 'port/game-data/reference/player-inventory-v1/NOTES.md and captures/producers/original-functions.json: RemoveOnePotion at 0x3fe878 reads signed16 quantity and decrements if greater than one; otherwise SetPotionQty(0) clears/destroys the instance. The actual ItemTable row is Potion0 item925/type14. The feature stages the same CharacterState inventory erase/decrement before publishing source effects.'
        formulas = 'port/level-world/character_skill_application_v6.cpp: dh2_character_skill_regen_v6 reads HP (36/38) or MP (41/43), amount -1 selects maximum, caps current+amount, and only then performs the required DebugSwitches Load/String/Get/Destroy followed by PropertyAdd. The feature first synchronizes the current live ActorState HP/MP into source cells using loaded PropertyRules, then runs the actual formula in HP-then-MP order.'
        callers = 'Input capture has no timer cooldown; only a new key5 pressed edge is dispatched. Existing source controller blocked/locked gates remain the production caller responsibility. No original visual footage showing the potion input/result sequence was present in the supplied media; verification is a source-backed linked CPU session, not an original visual claim.'
    }
    source_visual_audit = [ordered]@{
        references_inspected = @('.local-inputs/front-v87-merge/source/port/android-native/app/src/main/assets/original-media/intro.mp4 (intro-only, no potion use visible)', '.local-inputs/frontend-feature-build/showcase-knight.mp4, showcase-mage.mp4, showcase-rogue.mp4 (character showcases; no potion-use sequence)')
        observation = 'No supplied original gameplay sequence demonstrates potion consumption/regeneration. Visual timing and presentation are unverified.'
        visual_claim = 'The linked test verifies the source rule/result state only; it does not claim HUD or visual feedback integration.'
    }
    behavior = [ordered]@{
        consumed_prefix = 'A required source Debug failure after RemoveOnePotion and property219 retains those reached source effects plus live/source vital synchronization; the output marks consumed_prefix_failure. No rollback erases a reached source prefix.'
        cooldown = 'No potion timer was found in the inspected caller; input uses only the key5 pressed edge, so held frames do not repeat.'
        unavailable = 'For property219>>8 above 99, the source HUD snapshot requires a trophy producer after consuming the potion. This feature commits reached consumption/count prefix and returns consumed_prefix_failure rather than claiming the trophy path.'
    }
    integrated_runtime_visual_verification = 'Not enrolled in main/GUI; linked isolated test verifies actual CombatSession player, same CharacterState, actual ItemTable Potion0/type14, PropertyRules and source RegenHP/MP Debug API.'
    main_binding_api = 'Set the existing `platform_input::LiveBorrow.potion` callback to verify its controller token is the source controller for this same actor, then call `RuntimeSessionPotionUseV1::dispatch(session, session.player_id(), *canonical_character_state, ButtonEdges{true,false,false}, {&actual_item_table, &same_property_rules, &source_skill_attack_debug_services}, receipt, error)`. `live_dispatch` calls this callback only for `frame.potion.pressed` after its leased actor/controller checks. The potion action owner still performs its recovered blocked/locked gate. CharacterState id must match the selected profile already bound in that same session; do not construct another inventory/session or call for held-only frames.'
    limitations = @('This helper is feature-owned and not enrolled in main/CMake/GUI.', 'Uses generic CharacterState inventory as same canonical owner; controller blocked/locked checks, HUD animation/audio/trophy continuation, and integrated visuals remain external.', 'No native owner graph is required; actual source Debug and ItemTable/PropertyRules providers are required when the source formula reaches them.')
}
foreach ($path in @($sources + 'port/windows-foundation/features/inventory/runtime_session_potion_use_v1.hpp' + 'port/windows-foundation/features/inventory/run_runtime_session_potion_use_v1_tests.ps1' + 'port/level-world/character_skill_combat_v6.hpp' + 'port/level-world/character_skill_application_v6.cpp' + 'port/windows-foundation/combat_session.hpp' + 'port/windows-foundation/features/platform_input/semantic_input.hpp' + 'port/windows-foundation/features/platform_input/live_dispatch.cpp' + 'port/game-data/reference/player-inventory-v1/NOTES.md' + 'port/android-native/app/src/main/cpp/gameplay_hud.cpp')) {
    if (Test-Path (Join-Path $root $path)) { $report.source_sha256[$path] = & $hash $path }
}
$reportPath = Join-Path $root 'port/windows-foundation/reports/runtime-session-potion-use-v1.json'
$report | ConvertTo-Json -Depth 8 | Set-Content -LiteralPath $reportPath -Encoding utf8
$resultText
