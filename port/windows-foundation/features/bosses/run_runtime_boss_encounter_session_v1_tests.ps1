param([string]$RepoRoot = (Resolve-Path (Join-Path $PSScriptRoot '../../../..')).Path)
$ErrorActionPreference = 'Stop'
$repo = (Resolve-Path $RepoRoot).Path
$build = Join-Path $repo '.local-inputs/boss-session-v1/native-test'
$shared = Join-Path $repo '.local-inputs/windows-foundation-build'
$compiler = Join-Path $repo '.local-inputs/windows-toolchain/llvm-mingw-20261006-ucrt-x86_64/bin/clang++.exe'
New-Item -ItemType Directory -Force -Path $build | Out-Null
if (!(Test-Path -LiteralPath $compiler)) { throw "Pinned LLVM-MinGW compiler is missing: $compiler" }
foreach ($library in @('libfoundation_data.a','librecovered_content.a','libcontent_xml.a','libdh2_freetype237.a')) {
    if (!(Test-Path -LiteralPath (Join-Path $shared $library))) { throw "Read-only native closure input is missing: $library" }
}
$includes = @(
    '-Iport/windows-foundation','-Iport/level-world','-Iport/engine-animation',
    '-Iport/engine-resources','-Iport/engine-skinning','-Iport/game-data',
    '-Iport/level-loader','-Iport/scene-materials','-Iport/engine-math',
    '-Iport/engine-ui','-Iport/engine-textures','-Iport/script-runtime',
    '-Iport/engine-audio','-Iport/level-loader/vendor/tinyxml'
)
$sources = @(
    'port/windows-foundation/features/bosses/runtime_boss_encounter_session_v1_tests.cpp',
    'port/windows-foundation/features/bosses/runtime_boss_encounter_session_v1.cpp',
    'port/windows-foundation/features/bosses/runtime_boss_encounter_plan_v1.cpp',
    'port/windows-foundation/features/quests/character_quest_progress_v1.cpp'
)
$output = Join-Path $build 'runtime_boss_encounter_session_v1_tests.exe'
$args = @('-std=c++17','-O0','-static','-Wall','-Wextra','-Werror',
    '-Wno-missing-field-initializers','-Wno-unused-value','-Dfinite=_finite') + $includes + $sources + @(
    "-L$shared",'-lfoundation_data','-lrecovered_content','-lcontent_xml','-ldh2_freetype237',
    '-lkernel32','-luser32','-lgdi32','-lwinspool','-lshell32','-lole32','-loleaut32',
    '-luuid','-lcomdlg32','-ladvapi32','-o',$output
)
& $compiler @args
if ($LASTEXITCODE -ne 0) { throw "Boss same-Session native feature test compile/link failed ($LASTEXITCODE)" }
$assets = Join-Path $repo '.local-inputs/windows-source-clock-checkpoint-v17/assets'
& $output $assets 2>&1 | Tee-Object -FilePath (Join-Path $build 'run.log')
if ($LASTEXITCODE -ne 0) { throw "Boss same-Session native feature test failed; see $build/run.log" }
