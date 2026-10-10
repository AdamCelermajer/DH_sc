param([string]$BuildRoot)
$ErrorActionPreference = 'Stop'
$taskRoot = [IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../../../..'))
$taskCompiler = Join-Path $taskRoot '.local-inputs/windows-toolchain/llvm-mingw-20261006-ucrt-x86_64/bin/clang++.exe'
if (!$BuildRoot) { $BuildRoot = Join-Path $taskRoot '.local-inputs/target-retention-regression-build' }
$taskBuild = [IO.Path]::GetFullPath($BuildRoot)
New-Item -ItemType Directory -Force -Path $taskBuild | Out-Null
$taskSources = @(
    'features/combat/target_retention_regression_tests.cpp',
    'features/combat/auto_target_marker_v1.cpp',
    'features/combat/session_source_object_interest_v1.cpp',
    'features/skills_animation/source_skill_animation.cpp',
    '../level-world/character_object_interest_v106.cpp',
    '../level-world/character_skill_ai_v3.cpp',
    '../level-world/character_skill_state_v4.cpp',
    'combat_session.cpp','actor_combat_runtime.cpp','combat_system.cpp',
    'actor_state.cpp','world.cpp','playable_actor_world.cpp','original_combat_properties.cpp'
) | ForEach-Object { Join-Path $taskRoot ('port/windows-foundation/' + $_) }
$taskLibraries = @('libfoundation_data.a','libcontent_xml.a','libdh2_freetype237.a','librecovered_trigger_contacts.a','librecovered_content.a','physics-backend/libdh2_box2d_201.a') |
    ForEach-Object {
        $source = Join-Path $taskRoot ('.local-inputs/windows-foundation-build/' + $_)
        if (!(Test-Path -LiteralPath $source)) { throw "Required archive is missing: $source" }
        $before = (Get-FileHash -Algorithm SHA256 -LiteralPath $source).Hash
        $destination = Join-Path $taskBuild $_
        New-Item -ItemType Directory -Force -Path (Split-Path -Parent $destination) | Out-Null
        Copy-Item -LiteralPath $source -Destination $destination -Force
        if ($before -ne (Get-FileHash -Algorithm SHA256 -LiteralPath $source).Hash) {
            throw "Shared archive changed during snapshot: $source"
        }
        $destination
    }
$taskExe = Join-Path $taskBuild 'target_retention_regression_tests.exe'
& $taskCompiler -std=c++17 -O2 -DNDEBUG -Wall -Wextra -Werror -Wno-missing-field-initializers -static `
    @taskSources @taskLibraries -lkernel32 -luser32 -lgdi32 -lwinspool -lshell32 -lole32 -loleaut32 -luuid -lcomdlg32 -ladvapi32 -o $taskExe
if ($LASTEXITCODE -ne 0) { throw 'Target retention regression compile failed' }
& $taskExe (Join-Path $taskRoot '.local-inputs/windows-shared-assets') | Tee-Object -FilePath (Join-Path $taskBuild 'run.log')
if ($LASTEXITCODE -ne 0) { throw 'Target retention regression failed' }
