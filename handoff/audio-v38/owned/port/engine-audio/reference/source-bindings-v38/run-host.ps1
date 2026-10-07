$ErrorActionPreference = 'Stop'
$bindingRoot = $PSScriptRoot.Replace('\','/')
$audioRoot = (Resolve-Path (Join-Path $PSScriptRoot '../..')).Path.Replace('\','/')
$repoRoot = (Resolve-Path (Join-Path $PSScriptRoot '../../../..')).Path
$buildRoot = Join-Path $repoRoot '.local-inputs/audio-v38'
New-Item -ItemType Directory -Path $buildRoot -Force | Out-Null
function Convert-BindingWslPath([string] $path) { return $path -replace '^C:', '/mnt/c' }
$bindingWsl = Convert-BindingWslPath $bindingRoot
$audioWsl = Convert-BindingWslPath $audioRoot
$buildWsl = Convert-BindingWslPath $buildRoot.Replace('\','/')
$results = @()
foreach ($optimization in @('O1','O2')) {
    $output = "$buildWsl/bindings-fixture-host-$optimization"
    $compile = @('--','g++','-std=c++17',"-$optimization",'-Wall','-Wextra','-Werror','-fno-fast-math','-ffp-contract=off','-fsanitize=address,undefined','-fno-omit-frame-pointer',"$audioWsl/audio_source_bindings_v38.cpp","$audioWsl/audio_listener_rows_v38.cpp","$audioWsl/vox_source_fields_v38.cpp","$audioWsl/audio_spatial_v34.cpp","$audioWsl/tests/vox_source_fields_v38_oracle.cpp","$bindingWsl/bindings-fixture.cpp",'-o',$output)
    & wsl.exe @compile
    if ($LASTEXITCODE -ne 0) { throw "Binding fixture compilation failed: $optimization" }
    $receipt = & wsl.exe -- $output $bindingWsl
    if ($LASTEXITCODE -ne 0) { throw "Binding fixture failed: $optimization" }
    $receipt
    $results += [pscustomobject]@{optimization=$optimization;sanitizers='ASan+UBSan';exit_code=0;output=$receipt}
}
$sources = @("$audioRoot/audio_source_bindings_v38.cpp","$audioRoot/audio_source_bindings_v38.hpp","$audioRoot/audio_listener_rows_v38.cpp","$audioRoot/audio_listener_rows_v38.hpp","$audioRoot/vox_source_fields_v38.cpp","$audioRoot/vox_source_fields_v38.hpp","$audioRoot/audio_spatial_v34.cpp","$audioRoot/tests/vox_source_fields_v38_oracle.cpp","$bindingRoot/bindings-fixture.cpp","$bindingRoot/original-runtime-pairs.bin","$audioRoot/reference/authorities-v38/original-properties-gold.bin",(Join-Path $repoRoot 'port/android-native/app/src/main/assets/data/sounds_pyarray.bin'))
foreach ($listenerName in @('AAA_DONT_DELETE_Listener','curListener','PlayerListener','BAPListener','CameraListener')) { $sources += "$audioRoot/reference/authorities-v38/$listenerName.bin" }
$hashes = @{}
foreach ($source in $sources) { $hashes[$source] = (Get-FileHash -LiteralPath $source -Algorithm SHA256).Hash.ToLower() }
@{runs=$results;source_sha256=$hashes} | ConvertTo-Json -Depth 5 | Set-Content -LiteralPath (Join-Path $PSScriptRoot 'host-validation.json')
