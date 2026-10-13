param([Parameter(Position=0)][string]$Command = 'check', [Parameter(ValueFromRemainingArguments=$true)][string[]]$AtlasArgs)
$ErrorActionPreference = 'Stop'
$taskRuntime = Join-Path $PSScriptRoot '.runtime/python.exe'
if (-not (Test-Path -LiteralPath $taskRuntime)) {
    $taskBundledPython = Join-Path $env:USERPROFILE '.cache/codex-runtimes/codex-primary-runtime/dependencies/python/python.exe'
    if (-not (Test-Path -LiteralPath $taskBundledPython)) { throw 'Portable runtime missing. Set up tools/atlas/.runtime with CPython 3.12, or install the Codex bundled workspace runtime.' }
    & $taskBundledPython (Join-Path $PSScriptRoot 'bootstrap.py')
    if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }
}
& $taskRuntime (Join-Path $PSScriptRoot 'atlas.py') $Command @AtlasArgs
exit $LASTEXITCODE
