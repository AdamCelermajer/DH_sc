from pathlib import Path
import json,subprocess,hashlib
b=Path('C:/Users/adamc/.codex/worktrees/generic-level-loader/DH_sc/port/level-loader');r=b/'reference/production-filesystem-adoption-v50'
(r/'original_filesystem_fields_v50.hpp').write_text('''#pragma once
#include <array>
#include <string>
namespace dh2::loader {
// One shared source static global per engine/Application boot, not per actor/cache.
// Initial original ELF data at99b138 is 1024 zero bytes. Never reset on GL restore.
struct OriginalWorkingDirectoryV50 {
 std::array<char,1024> source_buffer{};
 std::string value()const{return source_buffer.data();}
 // Genuine host chdir has already run on the runtime thread. Its result0 copies
 // the source path; failure retains all bytes. This method never invokes chdir.
 bool observe_chdir_result(const std::string&,int actual_host_result,
                          bool& original_return,std::string& error);
};
struct OriginalFileSystemPathsV50 {
 std::array<char,260> root2c{},copy130{},resources234{},trace338{},save43c{};
};
// Whole original C1 buffer/init projection. RES_PATH is a source platform global,
// initially the original empty literal; subsequent Android delivery is host input.
// It is not current main WorldScriptContext::files_directory/private save root.
bool construct_original_filesystem_paths_v50(const std::string& actual_RES_PATH,
 OriginalFileSystemPathsV50&,std::string& error);
} // namespace dh2::loader
''',newline='\n')
(r/'original_filesystem_fields_v50.cpp').write_text('''#include "original_filesystem_fields_v50.hpp"
#include <cstring>
namespace dh2::loader {
bool OriginalWorkingDirectoryV50::observe_chdir_result(const std::string& path,int status,
 bool& source_return,std::string& error){
 if(path.find('\\0')!=std::string::npos||path.size()>=source_buffer.size()){
  error="Original working-directory path outside proven C-string/1024-byte domain";return false;
 }
 source_return=status==0;
 if(source_return)std::memcpy(source_buffer.data(),path.c_str(),path.size()+1);
 error.clear();return true;
}
bool construct_original_filesystem_paths_v50(const std::string& root,OriginalFileSystemPathsV50& out,std::string& error){
 // _getPaths sprintf uses a 256-byte stack temporary, including data/ and NUL.
 // Reject original overflow domain; do not quietly truncate or relocate it.
 if(root.find('\\0')!=std::string::npos||root.size()>250){error="Original RES_PATH outside proven 256-byte trace temporary domain";return false;}
 OriginalFileSystemPathsV50 result;
 auto copy=[&](auto& buffer,const std::string& value){std::memcpy(buffer.data(),value.c_str(),value.size()+1);};
 copy(result.root2c,root);copy(result.copy130,root);copy(result.save43c,root);
 copy(result.trace338,root+"data/"); // resources234 remains actual C1 zeros.
 out=std::move(result);error.clear();return true;
}
} // namespace dh2::loader
''',newline='\n')
rows=json.loads((r/'original-fs-init-oracle-v50.json').read_text())
fixture=r/'original-fs-fields-v50.tsv'
lines=[]
for x in rows['constructor_cases']:lines.append('\t'.join(['C',x['RES_PATH_hex'] or '-']+[x['buffers_hex'][str(p)] for p in [0x2c,0x130,0x234,0x338,0x43c]])+'\n')
for x in rows['setter_cases']:lines.append('\t'.join(['S',str(x['host_chdir_return']),x['path_hex'] or '-',str(x['source_result']),x['working_buffer_hex']])+'\n')
fixture.write_text(''.join(lines),newline='\n')
(r/'original_fs_fields_v50_test.cpp').write_text('''#include "original_filesystem_fields_v50.hpp"
#include <fstream>
#include <sstream>
#include <iostream>
#include <stdexcept>
#include <algorithm>
using namespace dh2::loader;
static void ck(bool b,const char* m){if(!b)throw std::runtime_error(m);}
static std::string decode(const std::string& s){if(s=="-")return {};std::string out;for(std::size_t i=0;i<s.size();i+=2)out.push_back(static_cast<char>(std::stoul(s.substr(i,2),nullptr,16)));return out;}
int main(int argc,char**argv){try{ck(argc==2,"Required ARM fixture");std::ifstream f(argv[1]);ck(bool(f),"Missing ARM fixture");OriginalWorkingDirectoryV50 working;ck(std::all_of(working.source_buffer.begin(),working.source_buffer.end(),[](char c){return c==0;}),"Original boot global was not zeroed");unsigned ctors{},setters{};std::string line,e;
while(std::getline(f,line)){std::istringstream in(line);std::string kind,a,b,c;in>>kind;
 if(kind=="C"){in>>a;OriginalFileSystemPathsV50 paths;ck(construct_original_filesystem_paths_v50(decode(a),paths,e)&&e.empty(),"Source C1 rejected valid RES_PATH");for(const auto* p:{&paths.root2c,&paths.copy130,&paths.resources234,&paths.trace338,&paths.save43c}){in>>b;ck(std::string(p->data(),p->size())==decode(b),"Native/source original C1 buffers differ");}++ctors;}
 else{int status{},want{};in>>status>>a>>want>>b;bool source{};ck(working.observe_chdir_result(decode(a),status,source,e)&&source==bool(want)&&e.empty(),"Source chdir result changed");ck(std::string(working.source_buffer.data(),working.source_buffer.size())==decode(b),"Source shared directory whole buffer differs");++setters;}}
ck(ctors==5&&setters==16,"ARM case fixture incomplete");auto old=working.source_buffer;bool source=true;ck(!working.observe_chdir_result(std::string(1024,'x'),0,source,e)&&old==working.source_buffer,"Overflow source domain mutated shared state");OriginalFileSystemPathsV50 paths;paths.root2c[0]='X';ck(!construct_original_filesystem_paths_v50(std::string(251,'x'),paths,e)&&paths.root2c[0]=='X',"Source trace overflow domain got fabricated constructor state");
std::cout<<"PASS original_ARM/native_C1_cases="<<ctors<<" setter_cases="<<setters<<" initial_global1024_zero=1 exact_tail_preservation=1 constructor_resources234_zero=1 host_chdir_performed=0 production_cache_binding=0\\n";
}catch(const std::exception&e){std::cerr<<e.what()<<'\\n';return 1;}}
''',newline='\n')
def lx(p):return '/mnt/c/'+str(p).replace('\\','/')[3:]
binary=r/'source-fs-fields-v50-test';cmd=['wsl','--','/usr/bin/g++','-std=c++17','-Wall','-Wextra','-Werror','-O1','-g','-fsanitize=address,undefined',lx(r/'original_filesystem_fields_v50.cpp'),lx(r/'original_fs_fields_v50_test.cpp'),'-o',lx(binary)]
build=subprocess.run(cmd,capture_output=True,text=True);assert build.returncode==0,build.stderr
run=subprocess.run(['wsl','--',lx(binary),lx(fixture)],capture_output=True,text=True);assert run.returncode==0,run.stdout+run.stderr
(r/'source-fields-native.log').write_text(build.stdout+build.stderr+run.stdout+run.stderr)
(r/'source-fields-verified-v50.json').write_text(json.dumps({'validation':'PASS','output':run.stdout,'ASAN_UBSAN':True,'original_ARM_fixture_sha256':hashlib.sha256((r/'original-fs-init-oracle-v50.json').read_bytes()).hexdigest(),'main_binding':False,'host_chdir_executed':False},indent=2)+'\n')
print(run.stdout)
