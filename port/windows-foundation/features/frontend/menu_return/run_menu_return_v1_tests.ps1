param([string]$RepoRoot = (Resolve-Path (Join-Path $PSScriptRoot '../../../../..')).Path)
$ErrorActionPreference = 'Stop'
$repo = (Resolve-Path $RepoRoot).Path
$build = Join-Path $repo '.local-inputs/windows-foundation-build'
$tool = Join-Path $repo '.local-inputs/windows-toolchain/llvm-mingw-20261006-ucrt-x86_64/bin/clang++.exe'
$out = Join-Path $repo '.local-inputs/menu-return-v1'
New-Item -ItemType Directory -Force -Path $out | Out-Null
if (-not (Test-Path -LiteralPath $tool)) { throw "Pinned LLVM-MinGW compiler not found: $tool" }
$libraries = @('libfoundation_runtime_skill_cast.a','libfoundation_data.a','librecovered_content.a',
    'libcontent_xml.a','libdh2_freetype237.a','librecovered_trigger_contacts.a',
    'physics-backend/libdh2_box2d_201.a')
foreach ($library in $libraries) {
    if (-not (Test-Path -LiteralPath (Join-Path $build $library))) { throw "Terminal foundation archive missing: $library" }
}
$exe = Join-Path $out 'menu_return_v1_tests.exe'
$includes = @('-Iport/windows-foundation','-Iport/level-world','-Iport/game-data',
    '-Iport/engine-animation','-Iport/physics-backend/box2d-2.0.1/Include',
    '-Iport/engine-resources','-Iport/engine-skinning','-Iport/engine-math',
    '-Iport/engine-ui','-Iport/engine-textures','-Iport/scene-materials',
    '-Iport/level-loader','-Iport/script-runtime','-Iport/engine-audio',
    '-Iport/level-loader/vendor/tinyxml')
$args = @('-std=c++17','-O0','-static','-Dfinite=_finite','-Wall','-Wextra','-Werror','-Wpedantic',
    '-Wno-unused-value','-Wno-missing-field-initializers') + $includes + @(
    'port/windows-foundation/features/frontend/menu_return/menu_return_v1.cpp',
    'port/windows-foundation/features/frontend/menu_return/menu_return_v1_tests.cpp',
    'port/windows-foundation/features/generic_skills/runtime_skill_progression_v1.cpp',
    'port/windows-foundation/features/generic_skills/runtime_skill_mana_v1.cpp',
    "-L$build",'-Wl,--start-group','-lfoundation_runtime_skill_cast','-lfoundation_data',
    '-lrecovered_content','-lcontent_xml','-ldh2_freetype237','-lrecovered_trigger_contacts',
    (Join-Path $build 'physics-backend/libdh2_box2d_201.a'),'-Wl,--end-group',
    '-lkernel32','-luser32','-lgdi32','-lwinspool','-lshell32','-lole32','-loleaut32',
    '-luuid','-lcomdlg32','-ladvapi32','-o',$exe)
Push-Location $repo
try {
    & $tool @args
    if ($LASTEXITCODE -ne 0) { throw 'Menu return feature test compile/link failed' }
    & $exe (Join-Path $repo '.local-inputs/windows-shared-assets') $out
    if ($LASTEXITCODE -ne 0) { throw 'Menu return feature test failed' }
} finally {
    Pop-Location
}
