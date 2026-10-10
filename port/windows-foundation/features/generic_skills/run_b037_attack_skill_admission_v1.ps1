[CmdletBinding()]
param([string]$Compiler)
# B037 focused same-Session check: a skill is admitted while Space is held in a
# swing; rejections (casting, hurt, dead) hold; Space release follows CSM_StoppedAttacking
# (Character+1090 = CharAI+122 = AttackState64::finisher). Runs a private generated copy of the
# canonical session test, injecting the probe at the same anchor B003 uses.
$ErrorActionPreference = 'Stop'
$repoRoot = [IO.Path]::GetFullPath((Join-Path $PSScriptRoot '..\..\..\..'))
if (-not $Compiler) {
    $Compiler = Join-Path $repoRoot '.local-inputs\windows-toolchain\llvm-mingw-20261006-ucrt-x86_64\bin\clang++.exe'
}
$outputRoot = Join-Path $repoRoot '.local-inputs\b037-attack-skill-admission-v1'
New-Item -ItemType Directory -Force -Path $outputRoot | Out-Null
$sourcePath = Join-Path $PSScriptRoot 'runtime_skill_activation_session_v1_tests.cpp'
$generatedSource = Join-Path $outputRoot 'b037_attack_skill_admission.generated.cpp'
$sourceHashBefore = (Get-FileHash -Algorithm SHA256 -LiteralPath $sourcePath).Hash
$source = [IO.File]::ReadAllText($sourcePath).Replace("`r`n", "`n")
$anchor = '        // Normal progression/remapping occurs after the source bank and'
if ((($source -split [regex]::Escape($anchor)).Length - 1) -ne 1) { throw 'The connected Knight coordinator test anchor is missing or ambiguous' }
# Production player profile enables the source combo chain (main.cpp policy.sourceCombo=true).
$profileAnchor = '        profile.retainedPhaseClock = true;'
if (($source -split [regex]::Escape($profileAnchor)).Length - 1 -ne 1) { throw 'The player profile anchor is missing or ambiguous' }
$source = $source.Replace($profileAnchor, $profileAnchor + "`n        profile.sourceCombo = true; // B037: production player profile enables source combo")
$probe = @'
        // B037: skill admission while Space is held in a swing (source
        // AI_IsSkillUsable rejects only casting/using; CSAttack 50005 is unguarded).
        {
            check(retained_actor->action == CharacterAction::idle &&
                  cast_coordinator.receipt(1)->phase == RuntimeSkillCastPhaseV1::completed,
                  "B037 precondition: BashDown must have completed and left the player idle");
            for (unsigned frame = 0; frame < 3600; ++frame) {
                check(session.update(1.0 / 60.0, {}, {0, 0, 0}, 0.0f, error), error);
                check(cast_coordinator.advance_after_session_update(session, 1.0 / 60.0, error), error);
            }
            InputActions space;
            space.attack = true;
            for (unsigned frame = 0; frame < 120 && retained_actor->action != CharacterAction::attacking; ++frame) {
                check(session.update(1.0 / 60.0, space, {0, 0, 0}, 0.0f, error), error);
                check(cast_coordinator.advance_after_session_update(session, 1.0 / 60.0, error), error);
            }
            check(retained_actor->action == CharacterAction::attacking, "B037: held Space did not start a swing");
            for (unsigned frame = 0; frame < 6; ++frame) {
                check(session.update(1.0 / 60.0, space, {0, 0, 0}, 0.0f, error), error);
                check(cast_coordinator.advance_after_session_update(session, 1.0 / 60.0, error), error);
            }
            check(retained_actor->action == CharacterAction::attacking, "B037: swing ended before the mid-swing skill press");
            RuntimeSkillCastReceiptV1 mid_swing;
            check(cast_coordinator.begin_skill_cast_v1(cast_request, session, mid_swing, error) &&
                  mid_swing.phase == RuntimeSkillCastPhaseV1::prepared_pending_use,
                  error.empty() ? "B037: skill pressed mid-swing was not admitted" : error);
            check(retained_actor->action == CharacterAction::casting,
                  "B037: admitted skill did not enter the casting/Skill6 state");
            {const auto* sa = session.source_attack_state(1);
            check(sa && sa->continued == 0 && sa->last == 0,
                  std::string("B037: swing departure left stale combo continuation; ") + (sa ? "continued=" + std::to_string(sa->continued) + " last=" + std::to_string(sa->last) : std::string("no sourceCombo state")));}
            std::cout << "PASS B037 mid-swing skill admitted: generation=" << mid_swing.generation << std::endl;

            RuntimeSkillCastReceiptV1 rejected_while_casting;
            check(!cast_coordinator.begin_skill_cast_v1(cast_request, session, rejected_while_casting, error) &&
                  error.find("casting") != std::string::npos,
                  "B037: a second skill was admitted while the first is casting");
            unsigned attack_restarts = 0;
            for (unsigned frame = 0; frame < 240 && cast_coordinator.receipt(1)->phase != RuntimeSkillCastPhaseV1::completed; ++frame) {
                check(session.update(1.0 / 60.0, space, {0, 0, 0}, 0.0f, error), error);
                check(cast_coordinator.advance_after_session_update(session, 1.0 / 60.0, error), error);
                if (retained_actor->action == CharacterAction::attacking) ++attack_restarts;
            }
            check(cast_coordinator.receipt(1)->phase == RuntimeSkillCastPhaseV1::completed,
                  "B037: mid-swing skill did not complete its Use/Post");
            check(attack_restarts == 0,
                  "B037: still-held Space started a second attack during the skill");
            std::cout << "PASS B037 held Space during skill: no second attack before Post, completed once" << std::endl;

            const float hp_saved = retained_actor->health;
            retained_actor->health = 0.0f;
            retained_actor->action = CharacterAction::dead;
            RuntimeSkillCastReceiptV1 dead_cast;
            check(!cast_coordinator.begin_skill_cast_v1(cast_request, session, dead_cast, error) &&
                  error.find("dead") != std::string::npos,
                  "B037: dead Character admitted a skill");
            retained_actor->health = hp_saved;
            retained_actor->action = CharacterAction::hurt;
            RuntimeSkillCastReceiptV1 hurt_cast;
            check(!cast_coordinator.begin_skill_cast_v1(cast_request, session, hurt_cast, error) &&
                  error.find("Hurt") != std::string::npos,
                  "B037: hurt action admitted a skill without verified source admission");
            retained_actor->action = CharacterAction::idle;
            std::cout << "PASS B037 dead and hurt rejections hold; casting rejection holds" << std::endl;

            // Space release: 50001 leaves Attack only while Character+1090 (CharAI+122) is set.
            // CharAI+122 is (step!=0 && final step) = AttackState64::finisher. AttackState64::last
            // (CharAI+121) also covers the pre step and gates input, so it is not the release gate.
            // Release on a strike/pre frame (finisher==0) must not stop the swing. The recovery cut
            // (finisher!=0) is not reached in this fixture: a live target makes the source End skip
            // the final step, so it is left unverified here.
            InputActions released;
            bool strike_release_done = false;
            for (unsigned frame = 0; frame < 360 && retained_actor->action != CharacterAction::attacking; ++frame) {
                check(session.update(1.0 / 60.0, space, {0, 0, 0}, 0.0f, error), error);
                check(cast_coordinator.advance_after_session_update(session, 1.0 / 60.0, error), error);
            }
            check(retained_actor->action == CharacterAction::attacking, "B037 release probe: Space did not start a swing");
            for (unsigned frame = 0; frame < 360 && retained_actor->action == CharacterAction::attacking && !strike_release_done; ++frame) {
                const auto* attack = session.source_attack_state(1);
                const bool release_now = attack && attack->finisher == 0;
                check(session.update(1.0 / 60.0, release_now ? released : space, {0, 0, 0}, 0.0f, error), error);
                check(cast_coordinator.advance_after_session_update(session, 1.0 / 60.0, error), error);
                if (!release_now) continue;
                check(retained_actor->action == CharacterAction::attacking,
                      "B037: release in a strike (finisher==0) cut the swing immediately");
                strike_release_done = true;
                std::cout << "PASS B037 release in strike window (finisher==0) did not stop the swing" << std::endl;
            }
            check(strike_release_done, "B037 release probe did not reach a strike frame");
            for (unsigned frame = 0; frame < 360 && retained_actor->action == CharacterAction::attacking; ++frame) {
                check(session.update(1.0 / 60.0, released, {0, 0, 0}, 0.0f, error), error);
                check(cast_coordinator.advance_after_session_update(session, 1.0 / 60.0, error), error);
            }
            check(retained_actor->action != CharacterAction::attacking,
                  "B037: released swing never ended after the strike release");
            session.detach_for_restore();
            return 0;
        }

'@
$generated = $source.Replace($anchor, $probe + $anchor)
[IO.File]::WriteAllText($generatedSource, $generated, [Text.UTF8Encoding]::new($false))
if ((Get-FileHash -Algorithm SHA256 -LiteralPath $sourcePath).Hash -ne $sourceHashBefore) {
    throw 'The canonical session test source changed while creating the private B037 copy'
}
$buildRoot = Join-Path $outputRoot ('build-' + $PID)
New-Item -ItemType Directory -Force -Path $buildRoot | Out-Null
$buildArchives = @('libfoundation_data.a','libcontent_xml.a','libdh2_freetype237.a','librecovered_trigger_contacts.a','librecovered_content.a','physics-backend/libdh2_box2d_201.a')
$libraries = foreach ($archive in $buildArchives) {
    $sharedArchive = Join-Path $repoRoot ('.local-inputs\windows-foundation-build\' + $archive)
    if (-not (Test-Path -LiteralPath $sharedArchive)) { throw "Required source archive is missing: $sharedArchive" }
    $privateArchive = Join-Path $buildRoot $archive
    New-Item -ItemType Directory -Force -Path (Split-Path -Parent $privateArchive) | Out-Null
    Copy-Item -LiteralPath $sharedArchive -Destination $privateArchive -Force
    $privateArchive
}
$compilerDirectory = Split-Path $Compiler -Parent
$savedPath = $env:PATH
try {
    $env:PATH = $compilerDirectory + ';' + $env:PATH
    $exe = Join-Path $buildRoot 'b037_attack_skill_admission.exe'
    $sources = @(
        $generatedSource,
        (Join-Path $PSScriptRoot 'runtime_skill_activation_v1.cpp'),
        (Join-Path $PSScriptRoot 'runtime_skill_mana_v1.cpp'),
        (Join-Path $PSScriptRoot 'runtime_skill_cast_prepare_v1.cpp'),
        (Join-Path $PSScriptRoot 'runtime_skill_cast_coordinator_v1.cpp'),
        (Join-Path $PSScriptRoot 'runtime_skill_target_query_v1.cpp'),
        (Join-Path $PSScriptRoot 'runtime_skill_activation_session_v1.cpp'),
        (Join-Path $PSScriptRoot 'runtime_skill_animation_bank_v1.cpp'),
        (Join-Path $PSScriptRoot 'runtime_skill_progression_v1.cpp'),
        (Join-Path $PSScriptRoot 'generic_skills_page_v1.cpp'),
        (Join-Path $PSScriptRoot '..\skills_animation\skill_animation_program.cpp'),
        (Join-Path $repoRoot 'port\level-world\character_path_commands.cpp'),
        (Join-Path $repoRoot 'port\level-world\navigation_heading.cpp'),
        (Join-Path $repoRoot 'port\level-world\character_animation_ai.cpp'),
        (Join-Path $repoRoot 'port\windows-foundation\combat_session.cpp'),
        (Join-Path $repoRoot 'port\windows-foundation\playable_actor_world.cpp'),
        (Join-Path $repoRoot 'port\windows-foundation\save_store.cpp'),
        (Join-Path $repoRoot 'port\windows-foundation\game_save.cpp'),
        (Join-Path $repoRoot 'port\game-data\data.cpp'),
        (Join-Path $repoRoot 'port\game-data\skill_tables.cpp'),
        (Join-Path $repoRoot 'port\game-data\animation_tables.cpp')
    )
    & $Compiler -std=c++17 -Wall -Wextra -Werror -Wno-missing-field-initializers -O2 `
        -I $PSScriptRoot -DDH_RUNTIME_SKILL_CAST_WARRIOR_ONLY_TEST `
        @sources @libraries -lkernel32 -luser32 -lgdi32 -lwinspool -lshell32 -lole32 -loleaut32 -luuid -lcomdlg32 -ladvapi32 `
        -static -o $exe
    if ($LASTEXITCODE -ne 0) { throw 'B037 focused compile failed' }
    $runLog = Join-Path $outputRoot 'run.log'
    & $exe $repoRoot | Tee-Object -FilePath $runLog
    if ($LASTEXITCODE -ne 0) { throw 'B037 focused test failed' }
} finally {
    $env:PATH = $savedPath
}
