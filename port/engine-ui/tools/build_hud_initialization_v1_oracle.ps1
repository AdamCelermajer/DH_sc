param([string]$Ndk="$env:LOCALAPPDATA/Android/Sdk/ndk/29.0.14206865",[string]$Output='.local-inputs/hud-initialization-v1-oracle.so')
$ErrorActionPreference='Stop'
$repo=(Resolve-Path (Join-Path $PSScriptRoot '../../..')).Path
$compiler=Join-Path $Ndk 'toolchains/llvm/prebuilt/windows-x86_64/bin/clang++.exe'
$files=@('port/engine-ui/hud_initialization_v1.cpp','port/engine-ui/hud_initialization_v1.hpp')
$before=@{}
foreach($f in $files){$before[$f]=(Get-FileHash -LiteralPath (Join-Path $repo $f)).Hash.ToLowerInvariant()}
$outputPath=Join-Path $repo $Output
$arguments=@('--target=aarch64-linux-android24','-shared','-fPIC','-O2','-std=c++17','-Wall','-Wextra','-Werror',(Join-Path $repo $files[0]),'-o',$outputPath)
& $compiler @arguments
if($LASTEXITCODE -ne 0){throw 'HUD initialization oracle compilation failed'}
foreach($f in $files){if($before[$f] -ne (Get-FileHash -LiteralPath (Join-Path $repo $f)).Hash.ToLowerInvariant()){throw 'Compiler input changed'}}
$record=[ordered]@{validation='PASS';source_sha256=$before;compiler_sha256=(Get-FileHash -LiteralPath $compiler).Hash.ToLowerInvariant();compiler_arguments=$arguments;library_sha256=(Get-FileHash -LiteralPath $outputPath).Hash.ToLowerInvariant()}
$record|ConvertTo-Json -Depth 10|Set-Content -LiteralPath "$outputPath.build.json" -Encoding utf8
$record|ConvertTo-Json -Depth 10
