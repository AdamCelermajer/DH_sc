"""Compile-only exact new HUD connections for both current Android ABIs."""
from pathlib import Path
import hashlib,json,subprocess
R=Path(__file__).resolve().parents[3];U=R/'port/engine-ui';C=Path.home()/'AppData/Local/Android/Sdk/ndk/29.0.14206865/toolchains/llvm/prebuilt/windows-x86_64/bin/clang++.exe'
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest()
sources=['hud_initialization_v1','hud_initialization_core_v1','hud_initialization_owned_v1','hud_manager_core_v2'];inputs={str(U/(n+e)):sha(U/(n+e)) for n in sources for e in ('.cpp','.hpp')};out=R/'.local-inputs/hud-initialization-v1/android-objects';out.mkdir(parents=True,exist_ok=True);rows=[]
for abi in ('aarch64','x86_64'):
 for n in sources:
  obj=out/(abi+'-'+n+'.o');cmd=[str(C),'--target='+abi+'-linux-android24','-O2','-fPIC','-std=c++17','-Wall','-Wextra','-Werror','-Wno-unused-parameter','-Wno-unused-private-field','-fno-fast-math','-ffp-contract=off','-isystem',str(U/'vendor/gameswf1714'),'-DTU_CONFIG_LINK_TO_JPEGLIB=0','-DTU_CONFIG_LINK_TO_LIBPNG=0','-DTU_CONFIG_LINK_TO_FREETYPE=0','-DTU_CONFIG_LINK_TO_THREAD=0','-c',str(U/(n+'.cpp')),'-o',str(obj)];q=subprocess.run(cmd,capture_output=True,text=True)
  if q.returncode:raise RuntimeError(q.stderr)
  rows.append(dict(abi=abi,source=n,command=cmd,object_sha256=sha(obj)))
assert inputs=={p:sha(Path(p)) for p in inputs},'Compiler inputs changed'
report=dict(validation='PASS',source_sha256=inputs,compiler_sha256=sha(C),objects=rows,scope=__doc__,linked_DSO_or_APK_proof=False);(U/'reports/hud-initialization-v1-android-compile.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(dict(validation='PASS',compiled_objects=len(rows))))
