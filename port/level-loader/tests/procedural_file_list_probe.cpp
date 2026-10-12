#include "procedural_file_list_v1.hpp"
#include <iostream>
#include <stdexcept>
#include <string>
static std::string quote(const std::string& s){
    std::string out="\"";constexpr char hex[]="0123456789abcdef";
    for(unsigned char c:s){if(c=='\"'||c=='\\'){out+='\\';out+=char(c);}
        else if(c<32){out+="\\u00";out+=hex[c>>4];out+=hex[c&15];}else out+=char(c);}
    return out+'\"';
}
int main(){
    try{
        std::size_t count;
        while(std::cin>>count){
            if(count>1048576)throw std::runtime_error("Input exceeds checked domain");
            if(std::cin.get()!='\n')throw std::runtime_error("Input delimiter missing");
            std::string bytes(count,'\0');std::cin.read(bytes.data(),static_cast<std::streamsize>(count));
            if(!std::cin)throw std::runtime_error("Incomplete input");
            const auto rows=dh2::loader::procedural_file_list_v1(bytes);std::cout<<'[';bool comma=false;
            for(const auto& row:rows){if(comma)std::cout<<',';comma=true;std::cout<<quote(row);}
            std::cout<<"]\n";
        }
    }catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}
}
