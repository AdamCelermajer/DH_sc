#include "../original_filename_policy_v49.hpp"
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <sstream>
using namespace dh2::loader;
static void require(bool b,const char* s){if(!b)throw std::runtime_error(s);}
static std::string decode(const std::string& s){if(s=="-")return {};std::string out;for(std::size_t i=0;i<s.size();i+=2)out.push_back(static_cast<char>(std::stoul(s.substr(i,2),nullptr,16)));return out;}
int main(int argc,char** argv){try{
 require(argc==2,"Required original ARM source case fixture");std::ifstream in(argv[1]);require(bool(in),"Fixture unavailable");std::string a,b,c,e;unsigned basename{},count{};
 while(in>>a>>b>>c>>basename){OriginalFilenameResultV49 out;require(original_filename_policy_v49(decode(a),decode(b),out,e)&&e.empty(),"Original source valid domain rejected");require(out.filename==decode(c)&&out.texture_basename_relocated==bool(basename)&&out.input==decode(b)&&out.working_directory==decode(a),"Native/source original filename projection differs");++count;}
 require(count==231,"Original fixture incomplete");OriginalFilenameResultV49 out;out.filename="preserved";
 require(!original_filename_policy_v49("data","data",out,e)&&out.filename=="preserved","Undefined source overread became a guessed route");
 require(!original_filename_policy_v49("",std::string("a\0b",3),out,e)&&out.filename=="preserved","Embedded NUL accepted as detached C-string route");
 OriginalFilesystemBorrowV49 fs;require(!borrowed_original_filename_policy_v49(fs,"data/X.tga",out,e)&&out.filename=="preserved","Missing FS owner/provider accepted");
 // Explicit working-directory transport fixture; no production mount/GPU claim.
 fs.actual_filesystem_owner=std::make_shared<int>(49);unsigned reads{};
 fs.borrow_working_directory=[&](std::string& s,std::string&){++reads;s="";return true;};
 require(borrowed_original_filename_policy_v49(fs,"data/3d/Other/F.TGA",out,e)&&reads==2&&out.filename=="data/3d/other/f.tga"&&e.empty(),"Actual synchronous borrow/source order changed");
 auto old=out.filename;reads=0;fs.borrow_working_directory=[&](std::string& s,std::string&){s=++reads==1?"":"changed";return true;};
 require(!borrowed_original_filename_policy_v49(fs,"data/X.tga",out,e)&&out.filename==old,"Changing live FS working directory accepted/changed prior output");
 std::array<std::string,2> paths;require(original_image_search_candidates_v49("data/3D/GameObjects", "q:/data/iphone/3D/Textures/full.tga",paths,e)&&paths[0]=="data/3D/GameObjects/q:/data/iphone/3D/Textures/full.tga"&&paths[1]=="q:/data/iphone/3D/Textures/full.tga"&&e.empty(),"CResFactory source path/order changed");
 require(original_image_search_candidates_v49("", "a.tga",paths,e)&&paths[0]=="/a.tga"&&paths[1]=="a.tga","Empty source directory guessed normalization");
 std::cout<<"PASS original_ARM_native_cases="<<count<<" live_working_directory_guard=1 exact_case_substring_basename_and_prefix_preserved=1 undefined_domain_rejected=1 GPU_format_mount_policy=0\n";
}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
