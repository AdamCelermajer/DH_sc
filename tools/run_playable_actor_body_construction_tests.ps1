param([string]$AssetRoot,[string]$BuildRoot)
$ErrorActionPreference='Stop'
$taskRoot=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '..'))
if (!$AssetRoot) { $AssetRoot=Join-Path $taskRoot '.local-inputs/windows-shared-assets' }
if (!$BuildRoot) { $BuildRoot=Join-Path $taskRoot '.local-inputs/actor-body-construction-test' }
$taskBuild=[IO.Path]::GetFullPath($BuildRoot)
$null=New-Item -ItemType Directory -Force -Path $taskBuild
$taskCompiler=Join-Path $taskRoot '.local-inputs/windows-toolchain/llvm-mingw-20261006-ucrt-x86_64/bin/clang++.exe'
$taskArchives=@('libfoundation_data.a','libcontent_xml.a','libdh2_freetype237.a',
    'librecovered_trigger_contacts.a','librecovered_content.a','physics-backend/libdh2_box2d_201.a') | ForEach-Object {
    $source=Join-Path $taskRoot ('.local-inputs/windows-foundation-build/'+$_)
    $before=(Get-FileHash -Algorithm SHA256 -LiteralPath $source).Hash
    $destination=Join-Path $taskBuild $_
    $null=New-Item -ItemType Directory -Force -Path (Split-Path -Parent $destination)
    Copy-Item -LiteralPath $source -Destination $destination -Force
    if ($before -ne (Get-FileHash -Algorithm SHA256 -LiteralPath $source).Hash) {
        throw "Shared archive changed during snapshot: $source"
    }
    $destination
}
$taskSources=@('port/windows-foundation/tests/playable_actor_body_construction_tests.cpp',
    'port/windows-foundation/playable_actor_bodies.cpp','port/windows-foundation/original_actor_physical.cpp',
    'port/windows-foundation/original_actor_navigation.cpp','port/level-world/physical_world.cpp',
    'port/level-world/character_script_collision.cpp','port/level-world/native_physical_filter_v1.cpp') |
    ForEach-Object { Join-Path $taskRoot $_ }
$taskIncludes=@('port/windows-foundation','port/game-data','port/level-world') |
    ForEach-Object { '-I'+(Join-Path $taskRoot $_) }
$taskExe=Join-Path $taskBuild 'playable_actor_body_construction_tests.exe'
& $taskCompiler '-std=c++17' '-O2' '-DNDEBUG' '-Wall' '-Wextra' '-Werror' '-Wno-missing-field-initializers' `
    '-Dfinite=_finite' '-static' '-isystem' (Join-Path $taskRoot 'port/physics-backend/box2d-2.0.1/Include') `
    @taskIncludes @taskSources @taskArchives '-lkernel32' '-luser32' '-lgdi32' '-lwinspool' '-lshell32' `
    '-lole32' '-loleaut32' '-luuid' '-lcomdlg32' '-ladvapi32' '-o' $taskExe
if ($LASTEXITCODE -ne 0) { throw 'Body construction test compile failed' }
& $taskExe $AssetRoot 2>&1 | Tee-Object -FilePath (Join-Path $taskBuild 'run.log')
if ($LASTEXITCODE -ne 0) { throw 'Body construction test failed' }
