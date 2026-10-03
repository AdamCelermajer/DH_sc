param([string]$Output='.local-inputs/physics-backend-discovery/backend64.so',[switch]$Layout32)
$ErrorActionPreference='Stop'
$repository=Resolve-Path (Join-Path $PSScriptRoot '../../..')
$compiler=Join-Path $env:LOCALAPPDATA 'Android/Sdk/ndk/29.0.14206865/toolchains/llvm/prebuilt/windows-x86_64/bin/clang++.exe'
Push-Location $repository
try {
 if($Layout32){
  & $compiler --target=armv7a-linux-androideabi24 -shared -fPIC -O0 -std=c++17 -include cstring -w '-Iport/physics-backend/box2d-2.0.1/Include' port/physics-backend/tests/layout.cpp -o $Output
 }else{
  $backendSources=(Get-ChildItem port/physics-backend/box2d-2.0.1/Source -Recurse -Filter *.cpp).FullName
  & $compiler --target=aarch64-linux-android24 -shared -fPIC -O0 -fno-fast-math -ffp-contract=off -std=c++17 -include cstring -w '-Iport/physics-backend/box2d-2.0.1/Include' @backendSources port/physics-backend/tests/scene.cpp -o $Output
 }
 if($LASTEXITCODE -ne 0){throw 'Physics backend oracle build failed'}
}finally{Pop-Location}
