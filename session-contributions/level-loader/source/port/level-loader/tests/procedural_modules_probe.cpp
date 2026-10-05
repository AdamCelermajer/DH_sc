#include "procedural_modules_v1.hpp"
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
using namespace dh2;
static void require(bool ok,const std::string& error){if(!ok)throw std::runtime_error(error);}
static std::string quote(const std::string& s){
    std::string out="\"";constexpr char hex[]="0123456789abcdef";
    for(unsigned char c:s){if(c=='\"'||c=='\\'){out+='\\';out+=char(c);}
        else if(c<32){out+="\\u00";out+=hex[c>>4];out+=hex[c&15];}else out+=char(c);}
    return out+'\"';
}
static std::string projection(const loader::ProceduralModulePlanV1& plan){
    std::ostringstream out;out<<"{\"modules\":[";bool comma=false;
    for(const auto& module:plan.modules){if(comma)out<<',';comma=true;out<<"{\"properties\":{";bool pc=false;
        for(const auto& property:module.overrides){if(pc)out<<',';pc=true;out<<quote(property.first)<<':'<<quote(property.second);}
        out<<"},\"setter_attempts\":[";bool sc=false;
        for(const auto& setter:module.setter_attempts){if(sc)out<<',';sc=true;
            out<<"{\"name\":"<<quote(setter.name)<<",\"value\":"<<(setter.value?quote(*setter.value):"null")<<'}';}
        out<<"]}";
    }
    out<<"],\"returned_index\":"<<plan.returned_index<<",\"file_reads\":[";comma=false;
    for(const auto& module:plan.modules){if(comma)out<<',';comma=true;
        out<<"{\"uri\":"<<quote(module.mvx_uri)<<",\"found\":"<<(module.mvx_found?"true":"false")<<'}';}
    out<<"]}";return out.str();
}
int main(int argc,char** argv){
    try{
        require(argc==5,"Expected cache, identity, definition, seed");std::string error;
        const auto seed=std::stoul(argv[4]);require(seed<=UINT32_MAX,"Seed exceeds uint32 domain");
        loader::ProceduralModulePlanV1 retained;
        {
            auto file=std::make_shared<std::ifstream>(argv[1],std::ios::binary|std::ios::ate);
            require(bool(*file),"Cache unavailable");const auto length=file->tellg();require(length>=0,"Cache length unavailable");
            assets::ZipBackingV1 backing;backing.owner=file;backing.bytes=std::uint64_t(length);
            backing.read=[file](std::uint64_t pos,void* dst,std::size_t n,std::string& e){
                file->clear();file->seekg(std::streamoff(pos));file->read(static_cast<char*>(dst),std::streamsize(n));
                if(!*file){e="Cache read failed";return false;}return true;};
            assets::ZipAssetPackV1 pack;
            require(pack.mount(std::move(backing),"com.gameloft.android.GAND.GloftD2SS/files/",error),error);
            loader::ProceduralSourcesV1 sources;require(sources.prepare(pack,argv[2],argv[3],error),error);
            loader::ProceduralBlocksV1 blocks;require(blocks.prepare(sources.borrow(),error),error);
            loader::ProceduralConnectionsV1 connections;require(connections.prepare(blocks.borrow(),error),error);
            loader::ProceduralListsV1 lists;require(lists.prepare(connections.borrow(),error),error);
            loader::ProceduralRulesV1 rules;require(rules.prepare(lists.borrow(),error),error);
            loader::ProceduralLayoutResultV1 layout;
            require(loader::generate_procedural_layout_v1(rules.borrow(),std::uint32_t(seed),layout,error),error);
            require(loader::prepare_procedural_modules_v1(pack,layout,retained,error),error);
            const auto before=projection(retained);loader::ProceduralModulePlanV1 repeated;
            require(loader::prepare_procedural_modules_v1(pack,layout,repeated,error),error);
            require(projection(repeated)==before,"Repeated module preparation changed overrides or source");
            require(!loader::prepare_procedural_modules_v1(pack,{},retained,error),"Missing source accepted");
            require(projection(retained)==before,"Failed preparation replaced modules");
            auto bad=layout;bad.generated=!bad.generated;
            require(!loader::prepare_procedural_modules_v1(pack,bad,retained,error),"Inconsistent layout accepted");
            require(projection(retained)==before,"Failed layout replaced modules");
        }
        require(retained.layout.source_owner.lists().connections().blocks().sources().identity()==argv[2],"Retained visit changed");
        for(const auto& document:retained.mvx_documents)require(!document.source().empty(),"MVX lost after caller teardown");
        for(const auto& document:retained.layout.source_owner.lists().connections().blocks().sources().documents())
            require(!document.source().empty(),"Rule/MGX lost after caller teardown");
        std::cout<<projection(retained)<<'\n';
    }catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}
}
