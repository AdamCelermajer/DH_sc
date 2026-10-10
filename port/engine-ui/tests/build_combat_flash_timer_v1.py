"""Build optimized ARM64 oracle for combat_flash_timer_v1_original.py."""
import subprocess
from pathlib import Path
root=Path(__file__).resolve().parents[3]
cc=r'C:\Users\adamc\AppData\Local\Android\Sdk\ndk\29.0.14206865\toolchains\llvm\prebuilt\windows-x86_64\bin\clang++.exe'
p=subprocess.run([cc,'--target=aarch64-linux-android24','-std=c++17','-O2','-shared','-fPIC','-nostdlib','-I.','port/engine-ui/combat_flash_queue_v1.cpp','port/engine-ui/reference/combat-flash-v1/timer_oracle.cpp','-o','.local-inputs/libcombat-flash-timer-v1-oracle.so'],cwd=root,capture_output=True,text=True)
print(p.stdout,p.stderr);raise SystemExit(p.returncode)
