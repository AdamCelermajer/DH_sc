from pathlib import Path
import subprocess,json,hashlib
b=Path('C:/Users/adamc/.codex/worktrees/generic-level-loader/DH_sc/port/level-loader');r=b/'reference/production-filesystem-adoption-v50'
p=r/'original_filesystem_fields_v50.hpp';s=p.read_text().replace('#include <array>','#include <array>\n#include <memory>')
s=s.replace('struct OriginalFileSystemPathsV50 {','// One reconstructed source static global for this engine/process. Main borrows\n// this SAME owner across cache copies, World changes and GL recreation.\nstd::shared_ptr<OriginalWorkingDirectoryV50> source_filesystem_globals_v50();\nstruct OriginalFileSystemPathsV50 {');p.write_text(s,newline='\n')
p=r/'original_filesystem_fields_v50.cpp';s=p.read_text().replace('namespace dh2::loader {','namespace dh2::loader {\nstd::shared_ptr<OriginalWorkingDirectoryV50> source_filesystem_globals_v50(){\n static auto shared=std::make_shared<OriginalWorkingDirectoryV50>();\n return shared;\n}\n',1);p.write_text(s,newline='\n')
p=r/'original_fs_fields_v50_test.cpp';s=p.read_text().replace('unsigned ctors{},setters{};','auto globalA=source_filesystem_globals_v50();auto globalB=source_filesystem_globals_v50();ck(globalA==globalB,"Source process-global working-directory duplicated between resource owners");bool changed{};std::string ge;ck(globalA->observe_chdir_result("Actual/status/fixture",0,changed,ge)&&changed&&globalB->value()=="Actual/status/fixture","Source static global failed to retain setter state across aliases");unsigned ctors{},setters{};');p.write_text(s,newline='\n')
def lx(p):return '/mnt/c/'+str(p).replace('\\','/')[3:]
binary=r/'source-fs-fields-v50-test';cmd=['wsl','--','/usr/bin/g++','-std=c++17','-Wall','-Wextra','-Werror','-O1','-g','-fsanitize=address,undefined',lx(r/'original_filesystem_fields_v50.cpp'),lx(r/'original_fs_fields_v50_test.cpp'),'-o',lx(binary)]
build=subprocess.run(cmd,capture_output=True,text=True);assert build.returncode==0,build.stderr
run=subprocess.run(['wsl','--',lx(binary),lx(r/'original-fs-fields-v50.tsv')],capture_output=True,text=True);assert run.returncode==0,run.stdout+run.stderr
(r/'source-fields-native.log').write_text(build.stdout+build.stderr+run.stdout+run.stderr)
(r/'source-fields-verified-v50.json').write_text(json.dumps({'validation':'PASS','output':run.stdout,'ASAN_UBSAN':True,'one_shared_process_global_across_aliases_verified':True,'original_ARM_fixture_sha256':hashlib.sha256((r/'original-fs-init-oracle-v50.json').read_bytes()).hexdigest(),'main_binding':False,'host_chdir_executed':False},indent=2)+'\n')
print(run.stdout)
