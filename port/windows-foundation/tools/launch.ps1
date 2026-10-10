param(
    [ValidateSet('world','character','integrated')][string]$View = 'integrated',
    [string]$Config = '',
    [string]$Executable = '',
    [string]$Assets = ''
)
$ErrorActionPreference = 'Stop'
if (!$Config) { $Config = Join-Path $PSScriptRoot 'preview-launch.json' }
if (!$Executable) { $Executable = Join-Path $PSScriptRoot 'dh-foundation.exe' }
if (!$Assets) { $Assets = Join-Path $PSScriptRoot 'assets' }
$preset = Get-Content -LiteralPath $Config -Raw | ConvertFrom-Json
$viewArgs = @($preset.($View + '_args'))
if (!$viewArgs.Count) { throw "No $View configuration supplied" }
& $Executable --assets $Assets --save (Join-Path $PSScriptRoot 'character.save') @viewArgs
if ($LASTEXITCODE -ne 0) { throw "Preview exited with code $LASTEXITCODE" }
