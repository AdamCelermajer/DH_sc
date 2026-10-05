#include "procedural_sources_v1.hpp"
#include <fstream>
#include <iostream>
#include <stdexcept>
using namespace dh2;
static void require(bool ok,const std::string& e){if(!ok)throw std::runtime_error(e);}
static std::string quote(const std::string& s){
    std::string out="\"";constexpr char hex[]="0123456789abcdef";
    for(unsigned char c:s){if(c=='\"'||c=='\\'){out+='\\';out+=char(c);}
        else if(c<32){out+="\\u00";out+=hex[c>>4];out+=hex[c&15];}else out+=char(c);}
    return out+'\"';
}
static void tree(const loader::XmlDocumentV1::Borrow& doc,std::uint32_t node){
    const auto& e=doc.elements().at(node);std::cout<<"{\"tag\":"<<quote(e.tag)<<",\"attributes\":{";bool comma=false;
    for(const auto& a:e.attributes){if(comma)std::cout<<',';comma=true;std::cout<<quote(a.first)<<':'<<quote(a.second);}
    std::cout<<"},\"children\":[";comma=false;
    for(auto child:e.children){if(comma)std::cout<<',';comma=true;tree(doc,child);}
    std::cout<<"]}";
}
int main(int argc,char** argv){
    if(argc!=4)return 2;
    try{
        loader::ProceduralSourcesV1::Borrow retained;std::string error;
        {
            auto file=std::make_shared<std::ifstream>(argv[1],std::ios::binary|std::ios::ate);
            require(bool(*file),"Cache unavailable");const auto length=file->tellg();require(length>=0,"Cache length missing");
            assets::ZipBackingV1 backing;backing.owner=file;backing.bytes=std::uint64_t(length);
            backing.read=[file](std::uint64_t at,void* dst,std::size_t n,std::string& e){
                file->clear();file->seekg(std::streamoff(at));file->read(static_cast<char*>(dst),std::streamsize(n));
                if(!*file){e="Cache read failed";return false;}return true;
            };
            assets::ZipAssetPackV1 pack;require(pack.mount(std::move(backing),"com.gameloft.android.GAND.GloftD2SS/files/",error),error);
            loader::ProceduralSourcesV1 sources;require(sources.prepare(pack,argv[2],argv[3],error),error);retained=sources.borrow();
            require(!sources.prepare(pack,"missing","not-an-original-level.rule.xml",error),"Missing rule accepted");
            require(sources.borrow().identity()==argv[2],"Failed publication replaced prior sources");
            require(sources.prepare(pack,"separate-visit",argv[3],error),error);
            require(sources.borrow().identity()=="separate-visit","Fresh identity missing");
        }
        require(retained.identity()==argv[2],"Retained identity changed");
        std::cout<<"{\"validation\":\"PASS\",\"identity\":"<<quote(retained.identity())<<",\"folder\":"<<quote(retained.folder())
            <<",\"file_list_uri\":"<<quote(retained.file_list_uri())<<",\"file_list_bytes\":"<<retained.file_list_bytes().size()
            <<",\"document_count\":"<<retained.documents().size()<<",\"filenames\":[";bool comma=false;
        for(const auto& name:retained.filenames()){if(comma)std::cout<<',';comma=true;std::cout<<quote(name);}
        std::cout<<"],\"rule\":{\"uri\":"<<quote(retained.documents()[0].uri())<<",\"parsed\":"<<(retained.documents()[0].parsed()?"true":"false")
            <<",\"diagnostic\":"<<quote(retained.documents()[0].diagnostic().message)<<",\"tree\":";
        tree(retained.documents()[0],retained.rule_root());std::cout<<"},\"blocks\":[";comma=false;
        for(const auto& block:retained.blocks()){
            if(comma)std::cout<<',';
            comma=true;const auto& doc=retained.documents().at(block.document);require(!doc.source().empty(),"Raw source missing after owner teardown");
            std::cout<<"{\"filename\":"<<quote(block.filename)<<",\"name\":"<<quote(block.name)<<",\"uri\":"<<quote(doc.uri())
                <<",\"parsed\":"<<(doc.parsed()?"true":"false")<<",\"diagnostic\":"<<quote(doc.diagnostic().message)<<",\"tree\":";
            tree(doc,block.root);std::cout<<'}';
        }
        std::cout<<"],\"ownership_checks\":true,\"layout_generation_verified\":false,\"selected_placement_graph_verified\":false,\"full_loader_verified\":false}\n";
    }catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}
}
