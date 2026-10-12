param([string]$Ndk='C:\Users\adamc\AppData\Local\Android\Sdk\ndk\29.0.14206865')
$ErrorActionPreference='Stop'
$taskRoot=Resolve-Path (Join-Path $PSScriptRoot '..\..\..')
$taskOut=Join-Path $taskRoot '.local-inputs\character-skill-state-v4'
New-Item -ItemType Directory -Force -Path $taskOut | Out-Null
$taskCompiler=Join-Path $Ndk 'toolchains\llvm\prebuilt\windows-x86_64\bin\clang++.exe'
& $taskCompiler --target=aarch64-linux-android26 -std=c++17 -O2 -shared -fPIC -static-libstdc++ -fno-stack-protector (Join-Path $taskRoot 'port\level-world\character_skill_state_v4.cpp') -o (Join-Path $taskOut 'libskill_state_v4_oracle.so')
if($LASTEXITCODE){throw 'Skill state compilation failed'}
Get-FileHash -Algorithm SHA256 (Join-Path $taskOut 'libskill_state_v4_oracle.so')
