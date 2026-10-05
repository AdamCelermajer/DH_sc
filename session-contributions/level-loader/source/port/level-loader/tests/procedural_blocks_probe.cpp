#include "procedural_blocks_v1.hpp"
#include <algorithm>
#include <cstring>
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
static std::uint32_t bits(float f){std::uint32_t value;std::memcpy(&value,&f,4);return value;}
static void strings(const std::vector<std::string>& values){
    std::cout<<'[';bool comma=false;
    for(const auto& v:values){if(comma)std::cout<<',';comma=true;std::cout<<quote(v);}
    std::cout<<']';
}
static void projection(const loader::ProceduralBlockV1& b){
    std::cout<<"{\"load_result\":1,\"unit_bits\":["<<bits(b.unit_width)<<','<<bits(b.unit_height)
        <<"],\"size\":["<<b.width<<','<<b.height<<"],\"exits\":[";bool comma=false;
    for(const auto& e:b.exits){if(comma)std::cout<<',';comma=true;
        std::cout<<"{\"index\":"<<e.index<<",\"grid\":["<<e.grid[0]<<','<<e.grid[1]
            <<"],\"height_bits\":"<<bits(e.height)<<",\"direction\":"<<e.direction<<",\"link_types\":";
        strings(e.link_types);std::cout<<",\"connections\":0}";}
    std::cout<<"],\"link_declarations\":[";comma=false;
    for(const auto& d:b.link_declarations){if(comma)std::cout<<',';comma=true;
        static const char* names[]={"accepted","missing_direction","zero_or_empty_tail"};
        const auto& children=b.document.elements()[b.root].children;
        const auto child=std::find(children.begin(),children.end(),d.element)-children.begin();
        std::cout<<"{\"element\":"<<d.element<<",\"result\":"<<quote(names[static_cast<unsigned>(d.result)])
            <<",\"source_child\":"<<child<<",\"exit_index\":"<<d.exit_index<<",\"link_types\":";strings(d.link_types);std::cout<<'}';}
    std::cout<<"]}";
}
int main(int argc,char** argv){
    try {
        std::string error;
        if(argc==1) {
            std::size_t n{};
            while(std::cin>>n){require(n<=1048576,"Input too large");require(std::cin.get()=='\n',"Missing separator");
                std::vector<std::uint8_t> raw(n);std::cin.read(reinterpret_cast<char*>(raw.data()),n);require(bool(std::cin),"Input truncated");
                loader::ProceduralBlockV1 block;
                {loader::XmlDocumentV1 document;require(document.capture("input.mgx",std::move(raw),error),error);
                    auto d=document.borrow();require(!d.roots().empty(),"Input lacks root");
                    if(!loader::interpret_procedural_block_v1(d,d.roots()[0],block,error)) {
                        std::cout<<"{\"error\":"<<quote(error)<<"}\n";continue;}}
                require(!block.document.source().empty(),"Source lost after parser owner teardown");
                const auto count=block.exits.size();const auto source=block.document.source();
                require(!loader::interpret_procedural_block_v1({},0,block,error),"Missing source accepted");
                require(block.exits.size()==count&&block.document.source()==source,"Failed candidate replaced retained block");
                projection(block);std::cout<<'\n';
            }
        } else {
            require(argc==4,"Expected cache, identity, definition");loader::ProceduralBlocksV1::Borrow retained;
            {
                auto file=std::make_shared<std::ifstream>(argv[1],std::ios::binary|std::ios::ate);
                require(bool(*file),"Cache unavailable");const auto length=file->tellg();require(length>=0,"Cache length missing");
                assets::ZipBackingV1 backing;backing.owner=file;backing.bytes=std::uint64_t(length);
                backing.read=[file](std::uint64_t at,void* dst,std::size_t n,std::string& e){
                    file->clear();file->seekg(std::streamoff(at));file->read(static_cast<char*>(dst),std::streamsize(n));
                    if(!*file){e="Cache read failed";return false;}return true;
                };
                assets::ZipAssetPackV1 pack;require(pack.mount(std::move(backing),"com.gameloft.android.GAND.GloftD2SS/files/",error),error);
                loader::ProceduralSourcesV1 sources;require(sources.prepare(pack,argv[2],argv[3],error),error);
                loader::ProceduralBlocksV1 blocks;require(blocks.prepare(sources.borrow(),error),error);retained=blocks.borrow();
                require(!blocks.prepare({},error),"Missing source plan accepted");
                require(blocks.borrow().sources().identity()==argv[2],"Failed plan replaced prior state");
                require(sources.prepare(pack,"separate-visit",argv[3],error),error);
                require(blocks.prepare(sources.borrow(),error),error);
                require(blocks.borrow().sources().identity()=="separate-visit","Separate visit lost");
            }
            require(retained.sources().identity()==argv[2],"Retained visit changed");
            std::cout<<"{\"identity\":"<<quote(retained.sources().identity())<<",\"ownership_checks\":true,\"blocks\":[";bool comma=false;
            for(std::size_t i=0;i<retained.blocks().size();++i){if(comma)std::cout<<',';comma=true;
                const auto& b=retained.blocks()[i];require(!b.document.source().empty(),"Lost source bytes");
                std::cout<<"{\"uri\":"<<quote(b.document.uri())<<",\"name\":"<<quote(retained.sources().blocks()[i].name)<<",\"projection\":";
                projection(b);std::cout<<'}';}
            std::cout<<"]}\n";
        }
    }catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}
}
