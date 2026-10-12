param(
 [string]$Ndk = "$env:LOCALAPPDATA/Android/Sdk/ndk/29.0.14206865",
 [string]$Output = '.local-inputs/hud-manager-oracle.so'
)
$ErrorActionPreference = 'Stop'
$repo = (Resolve-Path (Join-Path $PSScriptRoot '../../..')).Path
$sources = @('port/engine-ui/hud_manager.cpp','port/engine-ui/hud_manager_backends.cpp','port/engine-ui/hud_player_values.cpp','port/level-world/character_timers.cpp')
$compiler = Join-Path $Ndk 'toolchains/llvm/prebuilt/windows-x86_64/bin/clang++.exe'
$outputPath = Join-Path $repo $Output
$before = @{}
foreach ($source in $sources) { $before[$source] = (Get-FileHash -LiteralPath (Join-Path $repo $source) -Algorithm SHA256).Hash.ToLowerInvariant() }
$arguments = @('--target=aarch64-linux-android24','-shared','-fPIC','-O2','-std=c++17','-Wall','-Wextra','-Werror')
foreach ($source in $sources) { $arguments += Join-Path $repo $source }
$arguments += @('-o',$outputPath)
& $compiler @arguments
if ($LASTEXITCODE -ne 0) { throw 'HUD manager oracle compile failed' }
foreach ($source in $sources) {
 if ($before[$source] -ne (Get-FileHash -LiteralPath (Join-Path $repo $source) -Algorithm SHA256).Hash.ToLowerInvariant()) { throw "Source changed during compile: $source" }
}
$record = [ordered]@{validation='PASS';library_sha256=(Get-FileHash -LiteralPath $outputPath -Algorithm SHA256).Hash.ToLowerInvariant();compiler_sha256=(Get-FileHash -LiteralPath $compiler -Algorithm SHA256).Hash.ToLowerInvariant();compiler_arguments=$arguments;source_sha256=$before}
$record | ConvertTo-Json -Depth 10 | Set-Content -LiteralPath "$outputPath.build.json" -Encoding utf8
$record | ConvertTo-Json -Depth 10
