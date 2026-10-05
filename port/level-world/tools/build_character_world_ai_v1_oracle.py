from pathlib import Path
import subprocess,hashlib,json
root=Path(__file__).resolve().parents[3];compiler=Path('C:/Users/adamc/AppData/Local/Android/Sdk/ndk/29.0.14206865/toolchains/llvm/prebuilt/windows-x86_64/bin/clang++.exe')
sources=['port/level-world/character_world_ai_relationship_v1.cpp','port/level-world/character_world_handle_v1.cpp','port/level-world/object_identity.cpp']
output=root/'.local-inputs/libcharacter_world_ai_v1_oracle.so'
subprocess.run([str(compiler),'--target=aarch64-linux-android24','-std=c++17','-O2','-fPIC','-shared',*[str(root/p) for p in sources],'-o',str(output)],check=True)
print(json.dumps({'validation':'PASS','sha256':hashlib.sha256(output.read_bytes()).hexdigest()}))
