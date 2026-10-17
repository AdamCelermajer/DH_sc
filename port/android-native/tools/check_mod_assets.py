"""Compile and exercise the native mod-file boundary on the host."""
import argparse, json, os, pathlib, shutil, subprocess, tempfile

SOURCE = pathlib.Path(__file__).resolve().parents[1] / 'app/src/main/cpp'
TEST = r'''
#include "mod_assets.hpp"
#include <filesystem>
#include <fstream>
#include <iostream>
#include <stdexcept>
namespace fs=std::filesystem;
void check(bool ok){if(!ok)throw std::runtime_error("Mod boundary assertion failed");}
int main(int argc,char** argv){
 check(argc==2);const fs::path root=fs::path(argv[1])/"mods";
 fs::create_directories(root/"textures");
 std::vector<std::uint8_t> bytes{99};std::string error;
 using dh2::mods::Lookup;
 for(const auto& path:std::vector<std::string>{"", "/foo", "../secret", "textures/../secret", "textures//a", "./a", "a/", "C:/a", "a\\b", std::string("a\0b",3)}){
  check(dh2::mods::read(root.string(),path,bytes,error)==Lookup::rejected);check(bytes==std::vector<std::uint8_t>{99});
 }
 check(dh2::mods::read(root.string(),"textures/absent",bytes,error)==Lookup::absent);
 std::ofstream(root/"textures/good",std::ios::binary)<<"abc";
 check(dh2::mods::read(root.string(),"textures/good",bytes,error)==Lookup::loaded);
 check(bytes==std::vector<std::uint8_t>({'a','b','c'}));
 std::ofstream(root/"textures/empty");
 check(dh2::mods::read(root.string(),"textures/empty",bytes,error)==Lookup::rejected);
 check(dh2::mods::read(root.string(),"textures",bytes,error)==Lookup::rejected);
 std::ofstream big(root/"textures/big",std::ios::binary);big.seekp(32*1024*1024);big.put('x');big.close();
 check(dh2::mods::read(root.string(),"textures/big",bytes,error)==Lookup::rejected);
 check(bytes==std::vector<std::uint8_t>({'a','b','c'}));
 std::ofstream(root.parent_path()/"outside")<<"private";
 std::error_code ec;fs::create_symlink(root.parent_path()/"outside",root/"textures/link",ec);
 if(!ec)check(dh2::mods::read(root.string(),"textures/link",bytes,error)==Lookup::rejected);
 std::cout<<"PASS: path, override, fallback, size, directory, retained-output; symlink "<<(ec?"unavailable":"checked")<<"\n";
}
'''

def main():
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--report',type=pathlib.Path)
    args=parser.parse_args()
    compiler=shutil.which(os.environ.get('CXX','g++'))
    if not compiler: raise SystemExit('Set CXX to a C++17 host compiler')
    with tempfile.TemporaryDirectory(prefix='dh2-mod-assets-') as directory:
        temp=pathlib.Path(directory);test=temp/'test.cpp';test.write_text(TEST)
        executable=temp/('test.exe' if os.name=='nt' else 'test')
        subprocess.run([compiler,'-std=c++17','-Wall','-Wextra','-Werror','-I',str(SOURCE),str(test),str(SOURCE/'mod_assets.cpp'),'-o',str(executable)],check=True)
        result=subprocess.run([str(executable),str(temp)],check=True,capture_output=True,text=True)
    print(result.stdout.strip())
    if args.report:
        args.report.parent.mkdir(parents=True,exist_ok=True)
        args.report.write_text(json.dumps({'validation':'PASS','scope':'Native mod-file boundary; no game behavior or device proof','result':result.stdout.strip()},indent=2)+'\n')

if __name__=='__main__':main()
