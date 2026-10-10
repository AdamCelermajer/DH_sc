param(
 [string]$Cache = 'C:\Users\adamc\Downloads\Dungeon-Hunter-2-HD-v1-0-2-cache.zip',
 [string]$NdkHeaders = 'C:\Users\adamc\AppData\Local\Android\Sdk\ndk\29.0.14206865\toolchains\llvm\prebuilt\windows-x86_64\sysroot\usr\include',
 [string]$OutputDirectory = '',
 [string]$Distribution = 'Ubuntu'
)
$ErrorActionPreference = 'Stop'
$taskRoot = [IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../../..'))
if (!$OutputDirectory) { $OutputDirectory = Join-Path $taskRoot '.local-inputs/native-host-v128' }
if (Get-Process -Name 'qemu-system-x86_64','qemu-system-i386','emulator' -ErrorAction SilentlyContinue) {
 throw 'Stop the owned emulator before building/running the native host batch.'
}
$taskCache = (Resolve-Path -LiteralPath $Cache).Path
$taskNdk = (Resolve-Path -LiteralPath $NdkHeaders).Path
$taskOutput = [IO.Path]::GetFullPath($OutputDirectory)
function Convert-TaskHostPath([string]$Path) {
 $converted = & wsl.exe -d $Distribution --exec wslpath -a $Path
 if ($LASTEXITCODE -ne 0) { throw 'WSL path conversion failed' }
 return ($converted | Out-String).Trim()
}
$taskRootLinux = Convert-TaskHostPath $taskRoot
$taskCacheLinux = Convert-TaskHostPath $taskCache
$taskNdkLinux = Convert-TaskHostPath $taskNdk
$taskOutputLinux = Convert-TaskHostPath $taskOutput
& wsl.exe -d $Distribution --exec bash "$taskRootLinux/port/android-native/tools/run_native_host_v128.sh" $taskRootLinux $taskCacheLinux $taskNdkLinux $taskOutputLinux
if ($LASTEXITCODE -ne 0) { throw "Native host run failed; inspect $taskOutput/build.log and run.log" }
