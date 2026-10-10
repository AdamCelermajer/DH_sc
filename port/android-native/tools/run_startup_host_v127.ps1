param(
 [string]$Cache = 'C:\Users\adamc\Downloads\Dungeon-Hunter-2-HD-v1-0-2-cache.zip',
 [string]$OutputDirectory = '',
 [string]$Distribution = 'Ubuntu'
)
$ErrorActionPreference = 'Stop'
$taskRoot = [IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../../..'))
if (!$OutputDirectory) { $OutputDirectory = Join-Path $taskRoot '.local-inputs/startup-host-v127' }
$taskCache = (Resolve-Path -LiteralPath $Cache).Path
$taskOutput = [IO.Path]::GetFullPath($OutputDirectory)
function Convert-TaskWslPath([string]$Path) {
 $converted = & wsl.exe -d $Distribution --exec wslpath -a $Path
 if ($LASTEXITCODE -ne 0) { throw 'WSL path conversion failed' }
 return ($converted | Out-String).Trim()
}
$taskRootLinux = Convert-TaskWslPath $taskRoot
$taskCacheLinux = Convert-TaskWslPath $taskCache
$taskOutputLinux = Convert-TaskWslPath $taskOutput
& wsl.exe -d $Distribution --exec bash "$taskRootLinux/port/android-native/tools/run_startup_host_v127.sh" $taskRootLinux $taskCacheLinux $taskOutputLinux
if ($LASTEXITCODE -ne 0) { throw "Startup host batch failed; inspect $taskOutput/build.log and results.jsonl" }
