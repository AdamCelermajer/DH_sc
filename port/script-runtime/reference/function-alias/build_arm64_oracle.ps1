param([string]$Output=".local-inputs/script-function-alias/oracle.so")
$ErrorActionPreference="Stop"
$taskRepo=Resolve-Path (Join-Path $PSScriptRoot "../../../..")
$taskCompiler=Join-Path $env:LOCALAPPDATA "Android/Sdk/ndk/29.0.14206865/toolchains/llvm/prebuilt/windows-x86_64/bin/clang++.exe"
Push-Location $taskRepo
try {
 New-Item -ItemType Directory -Force (Split-Path $Output) | Out-Null
 $taskArguments=@('--target=aarch64-linux-android24','-shared','-fPIC','-O2','-fno-fast-math','-ffp-contract=off','-std=c++17','-Wall','-Wextra','-Werror','port/script-runtime/script_function_alias.cpp','-o',$Output)
 & $taskCompiler @taskArguments
 if($LASTEXITCODE -ne 0){throw "Alias oracle build failed"}
 $taskIncludes=Join-Path (Split-Path (Split-Path $taskCompiler)) 'sysroot/usr/include/c++/v1'
 $taskInputs=@{}
 foreach($taskInput in @('port/script-runtime/script_function_alias.cpp','port/script-runtime/script_function_alias.h','port/script-runtime/script_runtime.h')){
  $taskInputs[$taskInput]=(Get-FileHash -Algorithm SHA256 $taskInput).Hash.ToLowerInvariant()
 }
 $taskHeaders=@{}
 foreach($taskHeader in @('__tree','map','string')){
  $taskHeaders[$taskHeader]=(Get-FileHash -Algorithm SHA256 (Join-Path $taskIncludes $taskHeader)).Hash.ToLowerInvariant()
 }
 $taskBuild=@{compiler=$taskCompiler;compiler_sha256=(Get-FileHash -Algorithm SHA256 $taskCompiler).Hash.ToLowerInvariant();compiler_version=(& $taskCompiler --version | Out-String).Trim();arguments=$taskArguments;ndk='29.0.14206865';source_sha256=$taskInputs;libcxx_header_sha256=$taskHeaders;library_sha256=(Get-FileHash -Algorithm SHA256 $Output).Hash.ToLowerInvariant()}
 $taskBuild | ConvertTo-Json -Depth 5 | Set-Content -Encoding UTF8 ($Output+'.build.json')
 Get-FileHash -Algorithm SHA256 $Output
} finally {Pop-Location}
