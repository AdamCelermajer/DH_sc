"""Tiny actual decoder/ZIP/sampler/mip/unbind fixture with fake GL metadata."""
from pathlib import Path
import hashlib,json,subprocess,runpy,zipfile
root=Path(__file__).resolve().parents[3]
out=root/'port/engine-resources/reports/texture-resource-v40'
# The one-shot patch preparer stages changes into the shared checkout. Reuse
# those staged generated inputs on reruns instead of replaying assertions that
# expect the pristine pre-patch source tree.
if not (out/'actual-effect-owner.inc').exists():
 runpy.run_path(str(root/'port/engine-resources/tools/prepare_texture_patch_v40.py'))
def unix(p):s=str(Path(p).resolve()).replace('\\','/');return '/mnt/'+s[0].lower()+s[2:]
scene=(out/'renderer_authored_effect_scene_v5.inc').read_text();start=scene.index('struct EffectGpuTextureV4 {');end=scene.index('struct EffectGpuResourceV4',start)
(out/'actual-effect-owner.inc').write_text(scene[start:end])
tga=bytearray(18);tga[2]=2;tga[12]=tga[14]=4;tga[16]=32;tga[17]=0x28;tga+=bytes([17,29,41,255])*16
fixture=out/'tiny-fixture.zip'
with zipfile.ZipFile(fixture,'w',zipfile.ZIP_DEFLATED) as z:
 z.writestr('root/data/3d/textures/tiny.tga',tga)
 z.writestr('root/data/3d/textures/stored.tga',tga,compress_type=zipfile.ZIP_STORED)
 z.writestr('root/data/3d/textures/pvr2_alias.tga',tga)
sources=['port/engine-resources/tests/texture_resource_consumer_v40.cpp','port/engine-resources/admitted_cpu_bytes_v40.cpp','port/engine-resources/retained_bytes_v39.cpp','port/engine-resources/resource_budget_v37.cpp','port/engine-resources/reports/texture-resource-v40/zip_asset_pack_v1.cpp','port/engine-resources/reports/texture-resource-v40/texture_owner_v1.cpp',*['port/engine-textures/'+p+'.cpp' for p in ['textures','pvrtc','general_fx_texture_image_v2','blood_texture_image_v1','texture_mipmap_v1','texture_binding_owner_v1','texture_unbind_v1']]]
binding_paths=[*sources,'port/engine-resources/admitted_cpu_bytes_v40.hpp','port/android-native/app/src/main/cpp/renderer_model_texture_budget_v40.inc','port/engine-resources/reports/texture-resource-v40/renderer_effect_texture_v5.inc','port/engine-resources/reports/texture-resource-v40/actual-effect-owner.inc','port/engine-resources/reports/texture-resource-v40/zip_asset_pack_v1.hpp','port/engine-resources/reports/texture-resource-v40/original_cache_assets_v1.hpp']
bindings={p:hashlib.sha256((root/p).read_bytes()).hexdigest() for p in binding_paths}
receipts=[]
for opt in ['O1','O2']:
 exe=out/('fixture-'+opt)
 compile=['g++','-std=c++17','-'+opt,'-g','-Wall','-Wextra','-Werror','-fsanitize=address,undefined','-fno-omit-frame-pointer','-pthread','-Iport/engine-ui/reports/swf-gpu-host-v38/headers','-Iport/engine-textures',*sources,'-lz','-o',unix(exe)]
 for command in [compile,['timeout','--signal=TERM','--kill-after=2','20s','env','ASAN_OPTIONS=detect_leaks=1:halt_on_error=1','UBSAN_OPTIONS=halt_on_error=1',unix(exe),unix(fixture)]]:
  p=subprocess.run(['wsl.exe','--cd',unix(root),'--exec',*command],capture_output=True,text=True,timeout=55)
  receipts.append({'command':command,'exit_code':p.returncode,'stdout':p.stdout,'stderr':p.stderr})
  (out/'host-receipt.json').write_text(json.dumps({'status':'PASS' if p.returncode==0 else 'FAIL','scope':__doc__,'bindings_sha256':bindings,'receipts':receipts},indent=2)+'\n')
  print(p.returncode,p.stdout,p.stderr[:9000])
  if p.returncode:raise SystemExit(p.returncode)
