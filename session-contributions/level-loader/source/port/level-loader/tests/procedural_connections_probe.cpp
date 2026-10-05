#include "procedural_connections_v1.hpp"
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
static std::string blob(){
    std::size_t n;require(bool(std::cin>>n)&&n<=1048576,"Blob size missing/exceeded");
    require(std::cin.get()=='\n',"Missing blob separator");std::string value(n,'\0');
    std::cin.read(value.data(),n);require(bool(std::cin),"Blob truncated");return value;
}
static bool same(const loader::ProceduralConnectionGraphV1& a,const loader::ProceduralConnectionGraphV1& b){
    if(a.inserted!=b.inserted||a.selected_sources!=b.selected_sources||a.connections.size()!=b.connections.size())return false;
    for(std::size_t i=0;i<a.connections.size();++i){
        if(a.connections[i].size()!=b.connections[i].size())return false;
        for(std::size_t j=0;j<a.connections[i].size();++j){
            if(a.connections[i][j].size()!=b.connections[i][j].size())return false;
            for(std::size_t k=0;k<a.connections[i][j].size();++k)
                if(a.connections[i][j][k].block!=b.connections[i][j][k].block||a.connections[i][j][k].exit!=b.connections[i][j][k].exit)return false;
        }
    }
    return true;
}
static void projection(const std::vector<std::string>& names,const std::vector<std::string>& uris,
                       const loader::ProceduralConnectionGraphV1& graph){
    std::cout<<"{\"insertions\":[";bool comma=false;
    for(std::size_t i=0;i<names.size();++i){if(comma)std::cout<<',';comma=true;
        std::cout<<"{\"source_index\":"<<i<<",\"name\":"<<quote(names[i])
            <<",\"inserted\":"<<(graph.inserted[i]?"true":"false")<<'}';}
    std::cout<<"],\"blocks\":[";comma=false;
    for(std::size_t i=0;i<graph.selected_sources.size();++i){if(comma)std::cout<<',';comma=true;
        const auto source=graph.selected_sources[i];
        std::cout<<"{\"source_index\":"<<source<<",\"name\":"<<quote(names[source])
            <<",\"uri\":"<<quote(uris[source])<<",\"exits\":[";bool exit_comma=false;
        for(std::size_t e=0;e<graph.connections[i].size();++e){if(exit_comma)std::cout<<',';exit_comma=true;
            std::cout<<"{\"index\":"<<e<<",\"connections\":[";bool connection_comma=false;
            for(const auto& c:graph.connections[i][e]){if(connection_comma)std::cout<<',';connection_comma=true;
                std::cout<<'['<<c.block<<','<<c.exit<<']';}
            std::cout<<"]}";}
        std::cout<<"]}";}
    std::cout<<"]}";
}
int main(int argc,char** argv){
    try {
        std::string error;
        if(argc==1) {
            std::size_t count;loader::ProceduralConnectionGraphV1 graph;
            while(std::cin>>count){require(count<=512&&std::cin.get()=='\n',"Group count/separator invalid");
                std::vector<std::string> names,uris;std::vector<loader::ProceduralBlockV1> blocks;
                for(std::size_t i=0;i<count;++i){names.push_back(blob());uris.push_back(blob());const auto raw=blob();
                    loader::XmlDocumentV1 document;require(document.capture(uris.back().empty()?"synthetic.mgx":uris.back(),
                        std::vector<std::uint8_t>(raw.begin(),raw.end()),error),error);
                    const auto source=document.borrow();require(!source.roots().empty(),"MGX root unavailable");
                    loader::ProceduralBlockV1 block;require(loader::interpret_procedural_block_v1(source,source.roots()[0],block,error),error);
                    blocks.push_back(std::move(block));}
                const auto before=graph;
                if(!loader::connect_procedural_blocks_v1(names,blocks,graph,error)){
                    require(same(graph,before),"Failed candidate changed prior connection graph");
                    std::cout<<"{\"error\":"<<quote(error)<<"}\n";continue;}
                for(const auto& b:blocks)require(!b.document.source().empty(),"Raw source lost after parser teardown");
                const auto previous=graph;
                require(!loader::connect_procedural_blocks_v1({"mismatched"},{},graph,error),"Invalid candidate accepted");
                require(same(graph,previous),"Failed connection candidate replaced prior graph");
                projection(names,uris,graph);std::cout<<'\n';
            }
        } else {
            require(argc==4,"Expected cache, identity, definition");loader::ProceduralConnectionsV1::Borrow retained;
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
                loader::ProceduralBlocksV1 blocks;require(blocks.prepare(sources.borrow(),error),error);
                loader::ProceduralConnectionsV1 connections;require(connections.prepare(blocks.borrow(),error),error);retained=connections.borrow();
                require(!connections.prepare({},error),"Missing block plan accepted");
                require(connections.borrow().blocks().sources().identity()==argv[2],"Failed plan replaced prior identity");
                require(sources.prepare(pack,"separate-visit",argv[3],error),error);
                require(blocks.prepare(sources.borrow(),error),error);
                require(connections.prepare(blocks.borrow(),error),error);
                require(connections.borrow().blocks().sources().identity()=="separate-visit","Separate visit identity lost");
            }
            require(retained.blocks().sources().identity()==argv[2],"Retained visit changed");
            std::vector<std::string> names,uris;
            for(const auto& b:retained.blocks().sources().blocks()){
                const auto& doc=retained.blocks().sources().documents()[b.document];
                require(!doc.source().empty(),"Source lost after ZIP/owner teardown");names.push_back(b.name);uris.push_back(doc.uri());}
            std::cout<<"{\"identity\":"<<quote(retained.blocks().sources().identity())
                <<",\"folder\":"<<quote(retained.blocks().sources().folder())<<",\"ownership_checks\":true,\"graph\":";
            projection(names,uris,retained.graph());std::cout<<"}\n";
        }
    }catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}
}
