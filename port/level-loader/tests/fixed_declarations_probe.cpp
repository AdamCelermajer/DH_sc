#include "fixed_declarations_v1.hpp"
#include <fstream>
#include <iomanip>
#include <iostream>
#include <map>
#include <stdexcept>
using namespace dh2;
static void require(bool ok,const std::string& e){if(!ok)throw std::runtime_error(e);}
static std::string quote(const std::string& s){
    std::string out="\"";constexpr char hex[]="0123456789abcdef";
    for(unsigned char c:s){if(c=='\"'||c=='\\'){out+='\\';out+=char(c);}
        else if(c<32){out+="\\u00";out+=hex[c>>4];out+=hex[c&15];}else out+=char(c);}
    return out+'\"';
}
static const char* origin(loader::DeclarationOriginV1 value){
    switch(value){case loader::DeclarationOriginV1::level:return "level";
        case loader::DeclarationOriginV1::gameplay:return "gameplay";default:return "visual";}
}
static void point(const std::optional<std::array<float,3>>& p){
    if(!p){std::cout<<"null";return;}std::cout<<'[';for(unsigned j=0;j<3;++j){if(j)std::cout<<',';std::cout<<(*p)[j];}std::cout<<']';
}
static void tree(const loader::XmlDocumentV1::Borrow& doc,std::uint32_t node){
    const auto& e=doc.elements().at(node);
    std::cout<<"{\"tag\":"<<quote(e.tag)<<",\"attributes\":{";
    bool comma=false;
    for(const auto& a:e.attributes){if(comma)std::cout<<',';comma=true;std::cout<<quote(a.first)<<':'<<quote(a.second);}
    std::cout<<"},\"children\":[";comma=false;
    for(auto child:e.children){if(comma)std::cout<<',';comma=true;tree(doc,child);}
    std::cout<<"]}";
}
int main(int argc,char** argv){
    if(argc!=4)return 2;
    try {
        loader::FixedDeclarationsV1::Borrow retained;std::string error;
        {
            auto file=std::make_shared<std::ifstream>(argv[1],std::ios::binary|std::ios::ate);
            require(bool(*file),"Cache unavailable");const auto length=file->tellg();require(length>=0,"Cache length missing");
            assets::ZipBackingV1 backing;backing.owner=file;backing.bytes=std::uint64_t(length);
            backing.read=[file](std::uint64_t at,void* dst,std::size_t n,std::string& e){
                file->clear();file->seekg(std::streamoff(at));file->read(static_cast<char*>(dst),std::streamsize(n));
                if(!*file){e="Cache read failed";return false;}return true;
            };
            assets::ZipAssetPackV1 pack;require(pack.mount(std::move(backing),"com.gameloft.android.GAND.GloftD2SS/files/",error),error);
            loader::FixedSourcesV1 sources;require(sources.prepare(pack,argv[2],argv[3],error),error);
            loader::FixedMapV1 map;require(map.prepare(pack,sources.borrow(),error),error);
            loader::FixedDeclarationsV1 declarations;require(declarations.prepare(map.borrow(),error),error);retained=declarations.borrow();
            require(!declarations.prepare({},error),"Empty map accepted");
            require(declarations.borrow().map().sources().identity()==argv[2],"Failed prepare replaced declarations");
            require(sources.prepare(pack,"separate-visit",argv[3],error),error);require(map.prepare(pack,sources.borrow(),error),error);
            require(declarations.prepare(map.borrow(),error),error);
            require(declarations.borrow().map().sources().identity()=="separate-visit","Fresh identity missing");
        }
        require(retained.map().sources().identity()==argv[2],"Retained identity changed");
        std::map<std::string,unsigned> counts;unsigned absent_type=0,absent_name=0;
        for(const auto& d:retained.declarations()){
            const auto& e=retained.element(d);const auto* type=e.attribute("gametype");
            if(type)++counts[*type];else ++absent_type;
            if(!e.attribute("name"))++absent_name;
            if(d.module!=loader::no_source_v1)require(d.module<retained.map().modules().size(),"Declaration module missing");
            require(!retained.document(d).source().empty(),"Retained raw XML missing");
        }
        std::cout<<std::setprecision(9)<<"{\"validation\":\"PASS\",\"identity\":"<<quote(retained.map().sources().identity())
            <<",\"occurrences\":"<<retained.declarations().size()<<",\"module_count\":"<<retained.map().modules().size()<<",\"types\":{";
        bool comma=false;for(const auto& row:counts){if(comma)std::cout<<',';comma=true;std::cout<<quote(row.first)<<':'<<row.second;}
        std::cout<<"},\"missing_gametype\":"<<absent_type<<",\"missing_name\":"<<absent_name<<",\"declarations\":[";comma=false;
        for(const auto& d:retained.declarations()){
            if(comma)std::cout<<',';
            comma=true;const auto& doc=retained.document(d);const auto& e=retained.element(d);
            std::cout<<"{\"document\":"<<d.document<<",\"element\":"<<d.element<<",\"uri\":"<<quote(doc.uri())<<",\"tag\":"<<quote(e.tag)
                <<",\"source_order\":"<<d.source_order<<",\"origin\":"<<quote(origin(d.origin))<<",\"module\":";
            if(d.module==loader::no_source_v1)std::cout<<"null";else std::cout<<d.module;
            std::cout<<",\"module_name\":"<<(d.module==loader::no_source_v1?"null":quote(retained.map().modules().at(d.module).authored_name))<<",\"module_offset\":[";
            for(unsigned j=0;j<3;++j){if(j)std::cout<<',';std::cout<<d.module_offset[j];}
            std::cout<<"],\"attributes\":{";bool attr_comma=false;
            for(const auto& a:e.attributes){if(attr_comma)std::cout<<',';attr_comma=true;std::cout<<quote(a.first)<<':'<<quote(a.second);}
            std::cout<<"},\"authored_position\":";point(d.authored_position);std::cout<<",\"translated_position\":";point(d.translated_position);
            std::cout<<",\"rotation_degrees\":";point(d.rotation_degrees);std::cout<<",\"scale\":";point(d.scale);
            std::cout<<",\"tree\":";tree(doc,d.element);std::cout<<'}';
        }
        std::cout<<"],\"ownership_checks\":true,\"runtime_objects_verified\":false,\"gameplay_verified\":false}\n";
    }catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}
}
