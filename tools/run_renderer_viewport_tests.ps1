$ErrorActionPreference = 'Stop'
$taskRoot = [IO.Path]::GetFullPath((Join-Path $PSScriptRoot '..'))
$taskBuild = Join-Path $taskRoot '.local-inputs/renderer-viewport-test'
New-Item -ItemType Directory -Force -Path $taskBuild | Out-Null
$taskCompiler = Join-Path $taskRoot '.local-inputs/windows-toolchain/llvm-mingw-20261006-ucrt-x86_64/bin/clang++.exe'
$taskExe = Join-Path $taskBuild 'renderer_viewport_tests.exe'
& $taskCompiler -std=c++17 -Wall -Wextra -Werror -static `
    (Join-Path $taskRoot 'port/windows-foundation/tests/renderer_viewport_tests.cpp') `
    (Join-Path $taskRoot 'port/windows-foundation/renderer.cpp') `
    -lopengl32 -luser32 -lgdi32 -o $taskExe
if ($LASTEXITCODE -ne 0) { throw 'Viewport strict build failed' }
& $taskExe
if ($LASTEXITCODE -ne 0) { throw 'Viewport hidden WGL test failed' }
