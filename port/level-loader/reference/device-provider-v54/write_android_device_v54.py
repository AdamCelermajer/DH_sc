from pathlib import Path
import subprocess,json,hashlib
b=Path('C:/Users/adamc/.codex/worktrees/generic-level-loader/DH_sc/port/level-loader');r=b/'reference/device-provider-v54';root=Path('C:/Users/adamc/Desktop/workspace/DH_sc');sdk=Path('C:/Users/adamc/AppData/Local/Android/Sdk/ndk/29.0.14206865/toolchains/llvm/prebuilt/windows-x86_64/bin')
(r/'actual_device_android_v54.hpp').write_text('''#pragma once
#include <string>
namespace model_renderer {
// Called on the actual GL thread immediately after begin_context_v38.
bool bind_actual_device_context_v54(std::string&);
void invalidate_actual_device_context_v54() noexcept;
bool borrow_actual_device_high_performance_v54(bool&,std::string&);
}
''')
inc=(r/'native_device_provider_v54.inc').read_text();start=inc.index('dh2::world::ActualDeviceProviderV54');end=inc.index('// REQUIRED exact insertion');body=inc[start:end]
body=body.replace('bool model_renderer::borrow_actual_device_high_performance_v54','}\nbool model_renderer::borrow_actual_device_high_performance_v54').replace('return front_ui.menu_device_borrow_v4(f,e);','return model_renderer::borrow_actual_menu_device_v1(f,e);')
prefix='''#include "actual_device_android_v54.hpp"
#include "actual_device_provider_v54.hpp"
#include "native_resource_budget_v38.hpp"
#include <EGL/egl.h>
#include <sys/syscall.h>
#include <unistd.h>
namespace model_renderer {bool borrow_actual_menu_device_v1(dh2::ui::MenuDeviceFactsV1&,std::string&);}
namespace {
'''
suffix='''bool model_renderer::bind_actual_device_context_v54(std::string& error){
 return actual_device_provider_v54.bind_current(dh2::android_resources::budget_lease_v39(),current_gles_device_v54,error);
}
void model_renderer::invalidate_actual_device_context_v54() noexcept {actual_device_provider_v54.context_lost();}
'''
(r/'actual_device_android_v54.cpp').write_text(prefix+body+suffix)
cmd=[str(sdk/'clang++.exe'),'--target=aarch64-linux-android26','-std=c++17','-Wall','-Wextra','-Werror','-fsyntax-only','-I'+str(root/'port/engine-resources'),'-I'+str(root/'port/engine-ui'),'-I'+str(root/'port/android-native/app/src/main/cpp'),str(r/'actual_device_android_v54.cpp')]
p=subprocess.run(cmd,capture_output=True,text=True);(b/'reports/device-provider-v54-android-syntax.log').write_text(p.stdout+p.stderr);print(p.stdout+p.stderr);assert p.returncode==0
print('PASS actual Android NDK26 aarch64 TU syntax; not linked/deployed/runtime')
(r/'write_android_device_v54.py').write_bytes(Path(__file__).read_bytes())
