param([string]$Ndk='C:\Users\adamc\AppData\Local\Android\Sdk\ndk\29.0.14206865')
$ErrorActionPreference='Stop'
$taskRoot=Resolve-Path (Join-Path $PSScriptRoot '..\..\..')
$taskOut=Join-Path $taskRoot '.local-inputs\character-skill-gameplay-v3'
New-Item -ItemType Directory -Force -Path $taskOut | Out-Null
$taskCompiler=Join-Path $Ndk 'toolchains\llvm\prebuilt\windows-x86_64\bin\clang++.exe'
& $taskCompiler --target=aarch64-linux-android26 -std=c++17 -O2 -shared -fPIC -static-libstdc++ -fno-fast-math -ffp-contract=off -fno-stack-protector (Join-Path $taskRoot 'port\level-world\character_skill_callbacks_v3.cpp') (Join-Path $taskRoot 'port\level-world\character_skill_buff_bindings_v3.cpp') (Join-Path $taskRoot 'port\level-world\character_skill_class_v3.cpp') (Join-Path $taskRoot 'port\level-world\character_faery_element_v3.cpp') (Join-Path $taskRoot 'port\level-world\character_skill_ai_v3.cpp') (Join-Path $taskRoot 'port\level-world\character_skill_cooldown_v3.cpp') (Join-Path $taskRoot 'port\game-data\class_tables.cpp') (Join-Path $taskRoot 'port\game-data\properties.cpp') -o (Join-Path $taskOut 'libskill_gameplay_v3_oracle.so')
if($LASTEXITCODE){throw 'Native skill gameplay oracle compilation failed'}
Get-FileHash -Algorithm SHA256 (Join-Path $taskOut 'libskill_gameplay_v3_oracle.so')
