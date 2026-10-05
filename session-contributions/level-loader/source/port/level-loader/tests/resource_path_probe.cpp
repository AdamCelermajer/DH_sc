#include "resource_paths_v1.hpp"
#include <iostream>
#include <stdexcept>
int main(int argc,char** argv) {
    if(argc!=2)return 2;
    try {
        for(const auto& path:dh2::loader::compiled_level_paths_v1(argv[1]))std::cout<<path<<'\n';
    }catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}
}
