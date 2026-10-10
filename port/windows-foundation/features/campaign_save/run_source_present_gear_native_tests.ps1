$ErrorActionPreference = 'Stop'
$repo = (Resolve-Path (Join-Path $PSScriptRoot '..\..\..\..')).Path
$output = Join-Path $repo '.local-inputs\campaign-save-present-gear-native'
New-Item -ItemType Directory -Force -Path $output | Out-Null
$bin = Join-Path $repo '.local-inputs\windows-toolchain\llvm-mingw-20261006-ucrt-x86_64\bin'
$compiler = Join-Path $bin 'clang++.exe'
$native = Join-Path $repo 'port\windows-foundation\features\actor_frame\source_character_owner_factory_native_build\native.a'
$foundation = Join-Path $repo '.local-inputs\windows-foundation-build'
$source = Join-Path $repo 'port\windows-foundation\features\campaign_save\source_present_gear_native_tests.cpp'
$mutationSource = Join-Path $repo 'port\level-world\character_menu_mutations_v4.cpp'
$object = Join-Path $output 'source_present_gear_native_tests.o'
$mutationObject = Join-Path $output 'character_menu_mutations_v4.o'
$exe = Join-Path $output 'source_present_gear_native_tests.exe'
$includeDirs = @(
    'level-world', 'game-data', 'engine-textures', 'engine-ui', 'engine-animation',
    'engine-skinning', 'level-loader', 'level-loader/vendor/tinyxml', 'scene-materials',
    'script-runtime', 'script-runtime/lua', 'engine-resources', 'engine-math',
    'engine-ui/vendor/gameswf1714'
)
$compile = @('-std=c++20', '-Dfinite=_finite', '-DDH2_NATIVE_PROFILE_TRANSPORT_EMBED',
    '-O0', '-ffunction-sections', '-fdata-sections', '-fno-fast-math', '-ffp-contract=off',
    '-w', '-include', 'exception')
foreach ($dir in $includeDirs) { $compile += '-I' + (Join-Path $repo "port\$dir") }
$compile += @('-isystem', (Join-Path $repo 'port\physics-backend\box2d-2.0.1\Include'),
    '-c', $source, '-o', $object)
& $compiler @compile
if ($LASTEXITCODE -ne 0) { throw "Native test compile failed: $LASTEXITCODE" }
$mutationCompile = @($compile[0..($compile.Length - 4)]) + @('-c', $mutationSource, '-o', $mutationObject)
& $compiler @mutationCompile
if ($LASTEXITCODE -ne 0) { throw "Gold source owner compile failed: $LASTEXITCODE" }

$archives = @($native,
    (Join-Path $foundation 'libfoundation_data.a'),
    (Join-Path $foundation 'librecovered_content.a'),
    (Join-Path $foundation 'libcontent_xml.a'), $native)
$link = @($object, $mutationObject) + $archives + @('-Wl,--gc-sections', '-Wl,--error-limit=0',
    '-lopengl32', '-luser32', '-lgdi32', '-o', $exe)
& $compiler @link
if ($LASTEXITCODE -ne 0) { throw "Native test link failed: $LASTEXITCODE" }

$arguments = @(
    (Join-Path $repo 'port\level-world\reference\character-game-design\real-cache-inputs.bin'),
    (Join-Path $repo '.local-inputs\items-discovery'),
    (Join-Path $repo '.local-inputs\player-item-effects-v5\power-cache'),
    (Join-Path $repo '.local-inputs\windows-shared-assets'),
    (Join-Path $repo '.local-inputs\player-item-effects-v5\private-save'),
    (Join-Path $repo '.local-inputs\visual-skin-owner-v6\weapons'),
    (Join-Path $repo 'port\android-native\app\src\main\assets\models\prince_modular.bdae'),
    (Join-Path $repo 'port\game-data\reference\player-item-effects-v5\starter-effects-fixtures.bin')
)
$oldPath = $env:PATH
$env:PATH = $bin + [IO.Path]::PathSeparator + $oldPath
try { $stdout = & $exe @arguments }
finally { $env:PATH = $oldPath }
if ($LASTEXITCODE -ne 0) { throw "Native test failed: $LASTEXITCODE`n$stdout" }
$reportPath = Join-Path $output 'report.json'
$stdout | Set-Content -LiteralPath $reportPath -Encoding utf8
$report = Get-Content -LiteralPath $reportPath -Raw | ConvertFrom-Json
if ($report.validation -ne 'PASS' -or $report.full_initpost_or_player_readiness -ne $false) {
    throw "Native test report did not meet its bounded claim: $reportPath"
}
Write-Output $stdout
