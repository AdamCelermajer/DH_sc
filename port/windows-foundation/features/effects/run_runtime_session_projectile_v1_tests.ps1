$ErrorActionPreference = 'Stop'
$taskWorkspace = (Resolve-Path (Join-Path $PSScriptRoot '../../../..')).Path
$taskBuild = Join-Path $taskWorkspace '.local-inputs/runtime-session-projectile-v1'
$taskLibs = Join-Path $taskBuild 'private-libs'
$taskAssets = Join-Path $taskBuild 'assets'
$taskBin = Join-Path $taskWorkspace '.local-inputs/windows-toolchain/llvm-mingw-20261006-ucrt-x86_64/bin'
$taskCompiler = Join-Path $taskBin 'clang++.exe'
$sharedLibs = Join-Path $taskWorkspace '.local-inputs/windows-foundation-build'
New-Item -ItemType Directory -Force -Path $taskLibs | Out-Null
$libraryNames = @('libfoundation_data.a','libcontent_xml.a','libdh2_freetype237.a',
    'librecovered_trigger_contacts.a','librecovered_content.a',
    'physics-backend/libdh2_box2d_201.a')
foreach ($libraryName in $libraryNames) {
    $source = Join-Path $sharedLibs $libraryName
    $destination = Join-Path $taskLibs (Split-Path $libraryName -Leaf)
    if (-not (Test-Path -LiteralPath $destination) -or
        (Get-Item -LiteralPath $source).LastWriteTimeUtc -gt (Get-Item -LiteralPath $destination).LastWriteTimeUtc) {
        Copy-Item -LiteralPath $source -Destination $destination -Force
    }
}
if (-not (Test-Path -LiteralPath $taskAssets)) {
    Copy-Item -LiteralPath (Join-Path $taskWorkspace '.local-inputs/windows-shared-assets') `
        -Destination $taskAssets -Recurse
}
$completePyData = Join-Path $taskWorkspace '.local-inputs/runtime-source-projectile-v1/assets/pydata'
$sessionPyData = Join-Path $taskAssets 'original-cache/data/pydata'
Copy-Item -Path (Join-Path $completePyData '*') -Destination $sessionPyData -Recurse -Force
$wandAnimation = Join-Path $taskWorkspace '.local-inputs/windows-source-clock-v19-preview-9/assets/animations/skill_dh2_prince_mage_wand_attack.bdae'
$wandDestination = Join-Path $taskAssets 'data/3D/characters/prince/animations/skill_dh2_prince_mage_wand_attack.bdae'
New-Item -ItemType Directory -Force -Path (Split-Path $wandDestination) | Out-Null
Copy-Item -LiteralPath $wandAnimation -Destination $wandDestination -Force
$projectileSource = Join-Path $taskWorkspace '.local-inputs/runtime-source-projectile-v1/assets/data/3D/projectiles/elemental_bolt_fire.bdae'
$projectileDestination = Join-Path $taskAssets 'data/3D/projectiles/elemental_bolt_fire.bdae'
New-Item -ItemType Directory -Force -Path (Split-Path $projectileDestination) | Out-Null
Copy-Item -LiteralPath $projectileSource -Destination $projectileDestination -Force
$taskSources = @(
    'port/windows-foundation/features/effects/runtime_session_projectile_v1_tests.cpp',
    'port/windows-foundation/features/effects/runtime_session_projectile_v1.cpp',
    'port/windows-foundation/features/effects/runtime_session_projectile_frame_v1.cpp',
    'port/windows-foundation/features/effects/runtime_session_projectile_contacts_v1.cpp',
    'port/windows-foundation/features/effects/runtime_session_projectile_render_v1.cpp',
    'port/windows-foundation/features/effects/runtime_source_projectile_v1.cpp',
    'port/windows-foundation/original_scene.cpp',
    'port/windows-foundation/content_paths.cpp',
    'port/windows-foundation/animation_markers.cpp',
    'port/windows-foundation/combat_session.cpp',
    'port/windows-foundation/combat_system.cpp',
    'port/windows-foundation/actor_state.cpp',
    'port/windows-foundation/actor_combat_runtime.cpp',
    'port/windows-foundation/playable_actor_bodies.cpp',
    'port/windows-foundation/original_actor_body_plan.cpp',
    'port/windows-foundation/original_actor_physical.cpp',
    'port/game-data/combat_events.cpp',
    'port/android-native/app/src/main/cpp/source_process_arrays_v101.cpp',
    'port/engine-animation/event_track.cpp',
    'port/engine-animation/events.cpp',
    'port/engine-resources/resources.cpp',
    'port/level-world/physical_world.cpp',
    'port/level-world/native_body.cpp'
) | ForEach-Object { Join-Path $taskWorkspace $_ }
$taskLibraries = $libraryNames | ForEach-Object { Join-Path $taskLibs (Split-Path $_ -Leaf) }
$taskOutput = Join-Path $taskBuild 'runtime-session-projectile-v1-tests.exe'
& $taskCompiler -std=c++17 -O2 -DNDEBUG -Wall -Wextra -Werror -Wno-missing-field-initializers -Wno-unused-value -Dfinite=isfinite -pedantic '-Iport/windows-foundation' '-Iport/physics-backend/box2d-2.0.1/Include' -static @taskSources @taskLibraries -lkernel32 -luser32 -lgdi32 -lwinspool -lshell32 -lole32 -loleaut32 -luuid -lcomdlg32 -ladvapi32 -o $taskOutput
if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }
$env:PATH = $taskBin + [IO.Path]::PathSeparator + $env:PATH
& $taskOutput $taskAssets
exit $LASTEXITCODE
