$ErrorActionPreference = 'Stop'
$taskRoot = [IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../../../..'))
$taskCompiler = Join-Path $taskRoot '.local-inputs/windows-toolchain/llvm-mingw-20261006-ucrt-x86_64/bin/clang++.exe'
$taskBuild = Join-Path $taskRoot '.local-inputs/b004-b029-ooi-owner-v1-build'
New-Item -ItemType Directory -Force -Path $taskBuild | Out-Null
$taskExe = Join-Path $taskBuild 'object_of_interest_owner_v1_tests.exe'
& $taskCompiler -std=c++17 -O2 -DNDEBUG -Wall -Wextra -Werror -static `
    (Join-Path $PSScriptRoot 'object_of_interest_owner_v1.cpp') `
    (Join-Path $PSScriptRoot 'auto_target_marker_v1.cpp') `
    (Join-Path $PSScriptRoot 'object_of_interest_owner_v1_tests.cpp') `
    -o $taskExe
if ($LASTEXITCODE -ne 0) { throw 'OOI owner policy compile failed' }
& $taskExe | Tee-Object -FilePath (Join-Path $taskBuild 'run.log')
if ($LASTEXITCODE -ne 0) { throw 'OOI owner policy test failed' }
