from pathlib import Path
import subprocess,shlex,json,hashlib
b=Path('C:/Users/adamc/.codex/worktrees/generic-level-loader/DH_sc/port/level-loader');r=b/'reference/main-stage9-adoption-v54';main=Path('C:/Users/adamc/Desktop/workspace/DH_sc');build=Path('C:/Users/adamc/.codex/worktrees/generic-level-loader/build/receiver-transport-v5-host')
p=r/'main_stage9_lightset_binding_v54.hpp';s=p.read_text().replace('class MainStage9LightsetBindingV54 final {','class MainStage9LightsetBindingV54 final : public std::enable_shared_from_this<MainStage9LightsetBindingV54> {');p.write_text(s)
p=r/'main_stage9_lightset_binding_v54.cpp.proposed';s=p.read_text().replace('#include "renderer_native_gslevel_v27.hpp"','#include "renderer_native_gslevel_v27.hpp"\n#include "native_resource_budget_v38.hpp"');s=s.replace('if(!input_.validate_scope||!input_.validate_scope(e))return false;','''if(!input_.validate_scope||!input_.validate_scope(e))return false;
 auto device=dh2::android_resources::budget_lease_v39();
 if(!device||device.get()!=input_.actual_device_owner.get()||device.owner_before(input_.actual_device_owner)||input_.actual_device_owner.owner_before(device)){e="Require SAME authentic GL Device budget owner";return false;}''')
s=s.replace('auto file_input=input_.files;file_input.validate_current=[this](std::string& error){return validate(error);};','''auto weak=weak_from_this();auto validate_weak=[weak](std::string& error){auto owner=weak.lock();if(!owner){error="Stage9 application binding released";return false;}return owner->validate(error);};
  auto file_input=input_.files;file_input.validate_current=validate_weak;''').replace('services.validate_current=[this](std::string& error){return validate(error);};','services.validate_current=validate_weak;');p.write_text(s)
ninja=(build/'build.ninja').read_text();line=next(x for x in ninja.splitlines() if x.strip().startswith('INCLUDES =') and '/vendor/character-rng-integration-v5-loading/port/level-loader' in x);flags=shlex.split(line.split('=',1)[1]);
def wsl(p):return '/mnt/'+str(p)[0].lower()+str(p)[2:].replace('\\','/') if ':' in str(p) else str(p)
flags+=['-I'+wsl(b),'-I'+wsl(r),'-I'+wsl(main/'port/android-native/app/src/main/cpp'),'-I'+wsl(main/'port/engine-resources')]
cmd=['wsl.exe','--exec','g++','-std=c++17','-Wall','-Wextra','-Werror','-Wno-misleading-indentation',*flags,'-x','c++','-fsyntax-only',wsl(p)];result=subprocess.run(cmd,text=True,capture_output=True);(r/'syntax-only-v54.log').write_text(result.stdout+result.stderr);print(result.stdout+result.stderr);assert result.returncode==0
receipt=json.loads((r/'current-root-source-receipt-v54.json').read_text());changed=[]
for item in receipt['source_files']:
 current=hashlib.sha256((main/item['path']).read_bytes()).hexdigest()
 if current!=item['sha256']:changed.append({'path':item['path'],'before':item['sha256'],'after':current})
(r/'current-root-final-readback-v54.json').write_text(json.dumps({'selected_build':'READY762 no archive linked','typed_Main_binding_syntax':'PASS -Wall -Wextra -Werror','main_source_changes_during_readonly_audit':changed,'packet_V53_sha256':hashlib.sha256((b/'reports/level-lightset-stage9-v53-6f40d806a11189d0.zip').read_bytes()).hexdigest()},indent=2)+'\n');print('Syntax PASS; moving Main files:',changed)
