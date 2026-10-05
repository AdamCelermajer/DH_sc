#include "fixed_sources_v1.hpp"
#include <fstream>
#include <iostream>
#include <map>
#include <stdexcept>
using namespace dh2;
static void require(bool ok,const std::string& error) {
    if(!ok)throw std::runtime_error(error);
}
static std::string quote(const std::string& s) {
    std::string out="\"";
    for(unsigned char c:s) {
        if(c=='"'||c=='\\')out+='\\';
        if(c>=32)out+=static_cast<char>(c);
        else throw std::runtime_error("JSON text outside probe domain");
    }
    return out+'"';
}
int main(int argc,char** argv) {
    if(argc!=4)return 2;
    try {
        loader::FixedSourcesV1::Borrow retained;
        std::string error;
        {
            auto file=std::make_shared<std::ifstream>(argv[1],std::ios::binary|std::ios::ate);
            require(bool(*file),"Cache unavailable");
            const auto length=file->tellg();require(length>=0,"Cache length unavailable");
            assets::ZipBackingV1 backing;backing.owner=file;backing.bytes=static_cast<std::uint64_t>(length);
            backing.read=[file](std::uint64_t at,void* dst,std::size_t count,std::string& err) {
                file->clear();file->seekg(static_cast<std::streamoff>(at));
                file->read(static_cast<char*>(dst),static_cast<std::streamsize>(count));
                if(!*file){err="Cache read failed";return false;}return true;
            };
            assets::ZipAssetPackV1 pack;
            require(pack.mount(std::move(backing),"com.gameloft.android.GAND.GloftD2SS/files/",error),error);
            loader::FixedSourcesV1 sources;
            require(sources.prepare(pack,argv[2],argv[3],error),error);
            retained=sources.borrow();
            require(!sources.prepare(pack,"missing-level","__missing_loader_level__.mlx",error),"Missing dependency succeeded");
            require(sources.borrow().identity()==argv[2],"Failure replaced prepared sources");
            require(sources.prepare(pack,"another-visit-identity",argv[3],error),error);
            require(retained.identity()==argv[2],"Older identity borrow changed");
        }
        require(bool(retained),"Preparation ownership lost");
        std::map<std::string,unsigned> types;
        for(const auto& doc:retained.documents()) {
            require(doc.parsed(),"Retained source invalid");
            require(doc.used_level_buffer_route(),"Level source bypassed buffer traversal");
            for(const auto& element:doc.elements())
                if(const auto* type=element.attribute("gametype"))++types[*type];
        }
        std::cout<<"{\"validation\":\"PASS\",\"identity\":"<<quote(retained.identity())
            <<",\"documents\":"<<retained.documents().size()<<",\"module_links\":"<<retained.module_links().size()
            <<",\"declaration_counts_in_unique_sources\":{";
        bool comma=false;
        for(const auto& row:types){if(comma)std::cout<<',';comma=true;std::cout<<quote(row.first)<<':'<<row.second;}
        std::cout<<"},\"uris\":[";comma=false;
        for(const auto& doc:retained.documents()){if(comma)std::cout<<',';comma=true;std::cout<<quote(doc.uri());}
        std::cout<<"],\"ownership_checks\":true,\"level_buffer_route_verified\":true,\"map_render_verified\":false,\"gameplay_verified\":false}\n";
    }catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}
}
