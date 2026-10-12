#include "user_properties_v1.hpp"
#include <iostream>
#include <iterator>
static std::string quote(const std::string& s) {
    std::string out="\"";
    constexpr char hex[]="0123456789abcdef";
    for(unsigned char c:s) {
        if(c=='"'||c=='\\'){out+='\\';out+=static_cast<char>(c);}
        else if(c<32){out+="\\u00";out+=hex[c>>4];out+=hex[c&15];}
        else out+=static_cast<char>(c);
    }
    return out+'"';
}
static bool decode(const std::string& raw) {
    std::map<std::string,std::string> values;std::string error;
    if(!dh2::loader::decode_user_properties_v1(raw,values,error)){std::cerr<<error<<'\n';return false;}
    bool comma=false;std::cout<<'{';
    for(const auto& row:values){if(comma)std::cout<<',';comma=true;std::cout<<quote(row.first)<<':'<<quote(row.second);}
    std::cout<<"}\n";
    return true;
}
int main(int argc,char**) {
    if(argc==1)return decode(std::string((std::istreambuf_iterator<char>(std::cin)),{}))?0:1;
    unsigned length;
    while(std::cin>>length) {
        if(length>4096||std::cin.get()!='\n')return 2;
        std::string raw(length,'\0');std::cin.read(raw.data(),length);
        if(!std::cin||!decode(raw))return 1;
    }
}
