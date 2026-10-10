[CmdletBinding()]
param([string]$Compiler, [switch]$RunTargetlessDiagnostic)
$ErrorActionPreference = 'Stop'
$repoRoot = [IO.Path]::GetFullPath((Join-Path $PSScriptRoot '..\..\..\..'))
if (-not $Compiler) {
    $Compiler = Join-Path $repoRoot '.local-inputs\windows-toolchain\llvm-mingw-20261006-ucrt-x86_64\bin\clang++.exe'
}
$outputRoot = Join-Path $repoRoot '.local-inputs\post-skill-attack-regression-v1'
New-Item -ItemType Directory -Force -Path $outputRoot | Out-Null
$sourcePath = Join-Path $PSScriptRoot 'runtime_skill_activation_session_v1_tests.cpp'
$generatedSource = Join-Path $outputRoot 'post_skill_attack_regression.generated.cpp'
$source = [IO.File]::ReadAllText($sourcePath)
$sourceHashBefore = (Get-FileHash -Algorithm SHA256 -LiteralPath $sourcePath).Hash
$source = $source.Replace("`r`n", "`n")
$heldCastLoop = @'
        InputActions held_space_during_skill;
        held_space_during_skill.attack = true;
        for (unsigned frame = 0; frame < 180 && completion_count == 0; ++frame) {
            check(session.update(1.0 / 60.0, held_space_during_skill, {0, 0, 0}, 0.0f, error), error);
            check(cast_coordinator.advance_after_session_update(session, 1.0 / 60.0, error), error);
        }
'@
$castLoopPattern = '(?m)(?<=A wrong event/state/lifecycle rejection changed HP or combat RNG"\);\n)        for \(unsigned frame = 0; frame < 180 && completion_count == 0; \+\+frame\) \{\n            check\(session\.update\(1\.0 / 60\.0, \{\}, \{0, 0, 0\}, 0\.0f, error\), error\);\n            check\(cast_coordinator\.advance_after_session_update\(session, 1\.0 / 60\.0, error\), error\);\n        \}$'
if ([regex]::Matches($source, $castLoopPattern).Count -ne 1) { throw 'The BashDown animation loop anchor is missing or ambiguous' }
$source = [regex]::Replace($source, $castLoopPattern, $heldCastLoop.TrimEnd("`r", "`n"))
$anchor = '        // Normal progression/remapping occurs after the source bank and'
if ((($source -split [regex]::Escape($anchor)).Length - 1) -ne 1) { throw 'The connected Knight coordinator test anchor is missing or ambiguous' }
$probe = @'
        // B003: exercise the real Knight BashDown coordinator/Post and the
        // same CombatSession's held Space command after a neutral frame.
        check(retained_actor->action == CharacterAction::idle &&
              retained_actor->target_id == invalid_actor_id &&
              cast_coordinator.receipt(1)->phase == RuntimeSkillCastPhaseV1::completed,
              "Held Space was admitted inside Skill6 or BashDown Post did not retire the skill and clear target");
        InputActions released_after_post;
        check(session.update(1.0 / 60.0, released_after_post, {0, 0, 0}, 0.0f, error), error);
        check(cast_coordinator.advance_after_session_update(session, 1.0 / 60.0, error), error);
        check(retained_actor->action == CharacterAction::idle &&
              retained_actor->target_id == invalid_actor_id,
              "Neutral release after BashDown Post left the player in Skill6 or restored its cleared target");
        InputActions post_skill_space;
        post_skill_space.attack = true;
        check(session.update(1.0 / 60.0, post_skill_space, {0, 0, 0}, 0.0f, error), error);
        check(cast_coordinator.advance_after_session_update(session, 1.0 / 60.0, error), error);
        const ActorId post_skill_target = retained_actor->target_id;
        check(retained_actor->action == CharacterAction::attacking &&
              (post_skill_target == 2 || post_skill_target == 3) &&
              session.world()->eligible_target(*retained_actor, *session.actor(post_skill_target)) &&
              session.world()->original_melee_in_range(1, post_skill_target),
              "PC Space after completed BashDown did not reacquire and start an in-range Knight attack");
        std::cout << "PASS B003 Knight BashDown held-through-skill, released/neutral, then Space: target="
                  << post_skill_target << " action=Attack" << std::endl;
        InputActions released_space;
        for (unsigned frame = 0; frame < 360 && retained_actor->action == CharacterAction::attacking; ++frame) {
            check(session.update(1.0 / 60.0, released_space, {0, 0, 0}, 0.0f, error), error);
            check(cast_coordinator.advance_after_session_update(session, 1.0 / 60.0, error), error);
        }
        check(retained_actor->action == CharacterAction::idle,
              "B003 Space attack did not complete after key release");
        std::cout << "PASS B003 Knight BashDown coordinator/Post, Space reacquisition, and released attack completion" << std::endl;
        session.detach_for_restore();
        return 0;

'@
if ($RunTargetlessDiagnostic) {
    $diagnostic = @'
        // Separate source-parity diagnostic, not part of B003 acceptance.
        for (unsigned frame = 0; frame < 180; ++frame) {
            check(session.update(1.0 / 60.0, released_space, {0, 0, 0}, 0.0f, error), error);
            check(cast_coordinator.advance_after_session_update(session, 1.0 / 60.0, error), error);
        }
        const float target2_health = session.actor(2)->health;
        const float target3_health = session.actor(3)->health;
        session.actor(2)->health = 0.0f; session.actor(2)->action = CharacterAction::dead;
        session.actor(3)->health = 0.0f; session.actor(3)->action = CharacterAction::dead;
        retained_actor->target_id = invalid_actor_id;
        check(session.update(1.0 / 60.0, post_skill_space, {0, 0, 0}, 0.0f, error), error);
        check(cast_coordinator.advance_after_session_update(session, 1.0 / 60.0, error), error);
        check(retained_actor->action == CharacterAction::attacking &&
              retained_actor->target_id == invalid_actor_id,
              "Targetless diagnostic: source targetless Space did not enter Attack state");
        session.actor(2)->health = target2_health; session.actor(2)->action = CharacterAction::idle;
        session.actor(3)->health = target3_health; session.actor(3)->action = CharacterAction::idle;
        std::cout << "PASS targetless source-parity diagnostic: action=Attack target=none" << std::endl;
        session.detach_for_restore();
        return 0;

'@
    $probe = $probe.Replace('        session.detach_for_restore();', $diagnostic + '        session.detach_for_restore();')
}
$insertion = $probe + $anchor
$generated = $source.Replace($anchor, $insertion)
[IO.File]::WriteAllText($generatedSource, $generated, [Text.UTF8Encoding]::new($false))
if ((Get-FileHash -Algorithm SHA256 -LiteralPath $sourcePath).Hash -ne $sourceHashBefore) {
    throw 'The canonical coordinator test source changed while creating the private regression copy'
}

$buildRoot = Join-Path $outputRoot ('build-' + $PID)
New-Item -ItemType Directory -Force -Path $buildRoot | Out-Null
$buildArchives = @('libfoundation_data.a','libcontent_xml.a','libdh2_freetype237.a','librecovered_trigger_contacts.a','librecovered_content.a','physics-backend/libdh2_box2d_201.a')
$libraries = foreach ($archive in $buildArchives) {
    $sharedArchive = Join-Path $repoRoot ('.local-inputs\windows-foundation-build\' + $archive)
    if (-not (Test-Path -LiteralPath $sharedArchive)) { throw "Required source archive is missing: $sharedArchive" }
    $hashBefore = (Get-FileHash -Algorithm SHA256 -LiteralPath $sharedArchive).Hash
    $privateArchive = Join-Path $buildRoot $archive
    New-Item -ItemType Directory -Force -Path (Split-Path -Parent $privateArchive) | Out-Null
    Copy-Item -LiteralPath $sharedArchive -Destination $privateArchive -Force
    if ((Get-FileHash -Algorithm SHA256 -LiteralPath $sharedArchive).Hash -ne $hashBefore) {
        throw "Shared archive changed during private snapshot: $sharedArchive"
    }
    $privateArchive
}
$compilerDirectory = Split-Path $Compiler -Parent
$savedPath = $env:PATH
try {
    $env:PATH = $compilerDirectory + ';' + $env:PATH
    $exe = Join-Path $buildRoot 'post_skill_attack_regression.exe'
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
        -I $PSScriptRoot `
        -DDH_RUNTIME_SKILL_CAST_WARRIOR_ONLY_TEST `
        @sources @libraries -lkernel32 -luser32 -lgdi32 -lwinspool -lshell32 -lole32 -loleaut32 -luuid -lcomdlg32 -ladvapi32 `
        -static -o $exe
    if ($LASTEXITCODE -ne 0) { throw 'Post-skill attack regression compile failed' }
    $runLog = Join-Path $outputRoot 'run.log'
    & $exe $repoRoot | Tee-Object -FilePath $runLog
    if ($LASTEXITCODE -ne 0) { throw 'Post-skill attack regression failed' }
} finally {
    $env:PATH = $savedPath
}
