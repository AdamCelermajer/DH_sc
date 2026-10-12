from pathlib import Path
import json,subprocess
b=Path('C:/Users/adamc/.codex/worktrees/generic-level-loader/DH_sc/port/level-loader');r=b/'reference/level-source-filename-route-v52';rows=json.loads((r/'original-resource-join-v52.json').read_text())['cases'];enc=lambda s:s.encode().hex() or '-'
f=r/'resource-join-source-v52.tsv';f.write_text(''.join('\t'.join([enc(x['root']),enc(x['filename']),enc(x['RES_PATH']),str(x['application_byteA8']),enc(x['source_CFS_open_argument'])])+'\n' for x in rows),newline='\n')
t=r/'resource_join_native_v52.cpp';t.write_text('''#include "level_root_cfs_filename_v52.hpp"
#include <fstream>
#include <iostream>
#include <stdexcept>
using namespace dh2::loader;
static std::string dec(const std::string&s){if(s=="-")return {};std::string b;for(std::size_t i=0;i<s.size();i+=2)b.push_back(static_cast<char>(std::stoul(s.substr(i,2),nullptr,16)));return b;}
int main(int argc,char**argv){try{if(argc!=2)throw std::runtime_error("fixture");std::ifstream f(argv[1]);std::string a,b,c,d,e;unsigned lower{},count{};while(f>>a>>b>>c>>lower>>d){std::string out; if(!original_root_resource_join_v52(dec(a),dec(b),dec(c),lower!=0,out,e)||out!=dec(d)||!e.empty())throw std::runtime_error("Original/native resource FileHandle join differs");++count;}if(count!=24)throw std::runtime_error("fixture incomplete");std::string preserved="old";if(original_root_resource_join_v52("",std::string(260,'x'),"",false,preserved,e)||preserved!="old")throw std::runtime_error("source260 buffer overflow accepted/mutated output");std::cout<<"PASS original/native FileHandle joins="<<count<<" source_overflow_domain_rejected=1\\n";}catch(const std::exception&e){std::cerr<<e.what()<<'\\n';return 1;}}
''',newline='\n')
lx=lambda p:'/mnt/c/'+str(p).replace('\\','/')[3:];exe=r/'resource-join-native-v52';build=subprocess.run(['wsl','--','/usr/bin/g++','-std=c++17','-Wall','-Wextra','-Werror','-O1','-fsanitize=address,undefined',lx(r/'level_root_cfs_filename_v52.cpp'),lx(t),'-o',lx(exe)],capture_output=True,text=True);assert build.returncode==0,build.stderr
run=subprocess.run(['wsl','--',lx(exe),lx(f)],capture_output=True,text=True);assert run.returncode==0,run.stdout+run.stderr
(r/'resource-join-native-v52-verified.json').write_text(json.dumps({'validation':'PASS','ASAN_UBSAN':True,'output':run.stdout},indent=2)+'\n');print(run.stdout)
