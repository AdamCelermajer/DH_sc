$ErrorActionPreference='Stop'
$focusRoot=$PSScriptRoot.Replace('\','/')
$repoRoot=(Resolve-Path (Join-Path $PSScriptRoot '../../../..')).Path
$buildRoot=Join-Path $repoRoot '.local-inputs/audio-v40-focus'
New-Item -ItemType Directory -Path $buildRoot -Force | Out-Null
$focusWsl=$focusRoot -replace '^C:', '/mnt/c'
$buildWsl=$buildRoot.Replace('\','/') -replace '^C:', '/mnt/c'
$results=@()
foreach($optimization in @('O1','O2')) {
    & wsl.exe -- g++ -std=c++17 "-$optimization" -Wall -Wextra -Werror '-fsanitize=address,undefined' -fno-omit-frame-pointer -pthread "$focusWsl/audio_lifecycle_gate_v40.cpp" "$focusWsl/gate-fixture.cpp" -o "$buildWsl/gate-fixture-$optimization"
    if($LASTEXITCODE -ne 0){throw 'Gate compilation failed'}
    $receipt=& wsl.exe -- "$buildWsl/gate-fixture-$optimization"
    if($LASTEXITCODE -ne 0){throw 'Gate fixture failed'}
    $receipt
    $results+=[pscustomobject]@{optimization=$optimization;sanitizers='ASan+UBSan';exit_code=0;output=$receipt}
}
$hashes=@{}
foreach($name in @('audio_lifecycle_gate_v40.cpp','audio_lifecycle_gate_v40.hpp','gate-fixture.cpp')) { $hashes[$name]=(Get-FileHash -LiteralPath (Join-Path $PSScriptRoot $name) -Algorithm SHA256).Hash.ToLower() }
@{runs=$results;source_sha256=$hashes}|ConvertTo-Json -Depth 4|Set-Content -LiteralPath (Join-Path $PSScriptRoot 'host-validation.json')
