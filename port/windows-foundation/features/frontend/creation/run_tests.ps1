[CmdletBinding()]
param([string]$Compiler)
$ErrorActionPreference = 'Stop'
$repoRoot = [IO.Path]::GetFullPath((Join-Path $PSScriptRoot '..\..\..\..\..'))
if (-not $Compiler) {
    $Compiler = Join-Path $repoRoot '.local-inputs\windows-toolchain\llvm-mingw-20261006-ucrt-x86_64\bin\clang++.exe'
}
$testExecutable = Join-Path $PSScriptRoot 'creation_adapter_tests.exe'
$savedPath = $env:PATH
try {
    $env:PATH = (Split-Path $Compiler -Parent) + ';' + $env:PATH
    & $Compiler -std=c++17 -Wall -Wextra -Werror (Join-Path $PSScriptRoot 'creation_adapter.cpp') (Join-Path $PSScriptRoot 'dynamic_text_bindings.cpp') (Join-Path $repoRoot 'port\windows-foundation\character_state.cpp') (Join-Path $PSScriptRoot 'creation_adapter_tests.cpp') -o $testExecutable
    if ($LASTEXITCODE -ne 0) { throw 'Creation adapter compilation failed' }
    $testOutput = & $testExecutable
    if ($LASTEXITCODE -ne 0) { throw 'Creation adapter tests failed' }
    $testOutput
    @{
        status = 'PASS'
        compiler = $Compiler
        flags = '-std=c++17 -Wall -Wextra -Werror'
        output = $testOutput
        scope = 'Adapter contract tests with arbitrary provider fixtures; not full campaign creation parity'
    } | ConvertTo-Json | Set-Content -LiteralPath (Join-Path $PSScriptRoot 'test_evidence.json')
} finally {
    $env:PATH = $savedPath
    if (Test-Path -LiteralPath $testExecutable) { Remove-Item -LiteralPath $testExecutable }
}
