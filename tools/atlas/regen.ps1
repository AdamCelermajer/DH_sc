param([Parameter(ValueFromRemainingArguments=$true)][string[]]$AtlasArgs)
& (Join-Path $PSScriptRoot 'atlas.ps1') regen @AtlasArgs
exit $LASTEXITCODE
