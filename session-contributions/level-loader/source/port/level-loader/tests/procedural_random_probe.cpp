#include "procedural_random_v1.hpp"
#include <iostream>
#include <limits>
#include <stdexcept>
#include <string>
using dh2::loader::ProceduralRandomV1;
int main(){
    try {
        std::string op;
        while(std::cin>>op){
            if(op=="hash"){
                std::string hex,bytes;std::cin>>hex;
                if(hex!="-"){
                    if(hex.size()%2)throw std::runtime_error("Odd hex length");
                    for(std::size_t i=0;i<hex.size();i+=2){
                        const auto digit=[](char c)->unsigned{if(c>='0'&&c<='9')return unsigned(c-'0');if(c>='a'&&c<='f')return unsigned(c-'a'+10);throw std::runtime_error("Invalid hex");};
                        bytes+=char(digit(hex[i])*16+digit(hex[i+1]));
                    }
                }
                std::cout<<ProceduralRandomV1::hash(bytes)<<'\n';
            }else{
                std::uint64_t seed;std::cin>>seed;
                if(seed>0xffffffffULL)throw std::runtime_error("Seed overflow");
                ProceduralRandomV1 random(static_cast<std::uint32_t>(seed));
                if(op=="next"){
                    unsigned count;std::cin>>count;
                    for(unsigned i=0;i<count;++i){if(i)std::cout<<',';std::cout<<random.next();}
                    std::cout<<'\n';
                }else if(op=="between"){
                    std::int32_t minimum,maximum;std::cin>>minimum>>maximum;
                    std::cout<<random.between(minimum,maximum)<<','<<random.state()<<'\n';
                }else if(op=="bounded"){
                    std::uint32_t bound;std::cin>>bound;
                    std::cout<<random.bounded(bound)<<','<<random.state()<<'\n';
                }else throw std::runtime_error("Unknown operation");
            }
            if(!std::cin)throw std::runtime_error("Incomplete input");
        }
    }catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}
}
