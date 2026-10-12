#include "level_root_cfs_filename_v52.hpp"
#include <fstream>
#include <iostream>
#include <stdexcept>
using namespace dh2::loader;
static std::string dec(const std::string&s){if(s=="-")return {};std::string b;for(std::size_t i=0;i<s.size();i+=2)b.push_back(static_cast<char>(std::stoul(s.substr(i,2),nullptr,16)));return b;}
int main(int argc,char**argv){try{if(argc!=2)throw std::runtime_error("fixture");std::ifstream f(argv[1]);std::string a,b,c,d,e;unsigned lower{},count{};while(f>>a>>b>>c>>lower>>d){std::string out; if(!original_root_resource_join_v52(dec(a),dec(b),dec(c),lower!=0,out,e)||out!=dec(d)||!e.empty())throw std::runtime_error("Original/native resource FileHandle join differs");++count;}if(count!=24)throw std::runtime_error("fixture incomplete");std::string preserved="old";if(original_root_resource_join_v52("",std::string(260,'x'),"",false,preserved,e)||preserved!="old")throw std::runtime_error("source260 buffer overflow accepted/mutated output");std::cout<<"PASS original/native FileHandle joins="<<count<<" source_overflow_domain_rejected=1\n";}catch(const std::exception&e){std::cerr<<e.what()<<'\n';return 1;}}
