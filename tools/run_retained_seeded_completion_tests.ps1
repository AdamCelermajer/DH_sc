param([string]$AssetRoot, [string]$BuildRoot)
$ErrorActionPreference = 'Stop'
$repo = [IO.Path]::GetFullPath((Join-Path $PSScriptRoot '..'))
if (!$AssetRoot) { $AssetRoot = Join-Path $repo '.local-inputs/windows-source-clock-v19-preview-9/assets' }
if (!$BuildRoot) { $BuildRoot = Join-Path $repo '.local-inputs/session-seeded-completion-test' }
$build = [IO.Path]::GetFullPath($BuildRoot)
$compiler = Join-Path $repo '.local-inputs/windows-toolchain/llvm-mingw-20261006-ucrt-x86_64/bin/clang++.exe'
$shared = Join-Path $repo '.local-inputs/windows-foundation-build'
$null = New-Item -ItemType Directory -Path $build -Force

# Snapshot coherent shared archives into this test's private folder. Never
# compile or write into the lead's shared build directory.
$libraries = @('libfoundation_data.a', 'libcontent_xml.a', 'libdh2_freetype237.a',
               'librecovered_trigger_contacts.a', 'librecovered_content.a',
               'physics-backend/libdh2_box2d_201.a') | ForEach-Object {
    $source = Join-Path $shared $_
    if (!(Test-Path -LiteralPath $source)) { throw "Required coherent archive is missing: $source" }
    $before = (Get-FileHash -Algorithm SHA256 -LiteralPath $source).Hash
    $destination = Join-Path $build $_
    $null = New-Item -ItemType Directory -Path (Split-Path -Parent $destination) -Force
    Copy-Item -LiteralPath $source -Destination $destination -Force
    if ($before -ne (Get-FileHash -Algorithm SHA256 -LiteralPath $source).Hash) {
        throw "Shared archive changed during snapshot: $source"
    }
    $destination
}
$sources = @(
    'port/windows-foundation/tests/retained_seeded_completion_tests.cpp',
    'port/windows-foundation/retained_sequence_playback.cpp'
) | ForEach-Object { Join-Path $repo $_ }
$exe = Join-Path $build 'retained_seeded_completion_tests.exe'
& $compiler '-std=c++17' '-O2' '-DNDEBUG' '-Wall' '-Wextra' '-Werror' `
    '-Wno-missing-field-initializers' '-static' '-I' (Join-Path $repo 'port/windows-foundation') `
    @sources @libraries '-lkernel32' '-luser32' '-lgdi32' '-lwinspool' '-lshell32' `
    '-lole32' '-loleaut32' '-luuid' '-lcomdlg32' '-ladvapi32' '-o' $exe
if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }
Push-Location $repo
try {
    & $exe $AssetRoot 2>&1 | Tee-Object -FilePath (Join-Path $build 'run.log')
    if ($LASTEXITCODE -ne 0) { throw "Seeded completion test failed; see $build/run.log" }
} finally {
    Pop-Location
}
