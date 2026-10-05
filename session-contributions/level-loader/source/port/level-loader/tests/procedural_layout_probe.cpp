#include "procedural_layout_v1.hpp"
#include <cstring>
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
static std::string blob(){
    std::size_t n;require(bool(std::cin>>n)&&n<=1048576,"Blob size missing/exceeded");
    require(std::cin.get()=='\n',"Missing blob separator");std::string value(n,'\0');
    std::cin.read(value.data(),n);require(bool(std::cin),"Blob truncated");return value;
}
static std::string projection(const loader::ProceduralLayoutResultV1& result,const std::vector<std::string>& uris){
    std::ostringstream out;out<<"{\"seed\":"<<result.seed<<",\"success\":"<<(result.generated?"true":"false")
        <<",\"final_state\":"<<result.final_state<<",\"tiles\":[";bool comma=false;
    for(std::size_t i=0;i<result.tiles.size();++i){
        const auto& tile=result.tiles[i];std::uint32_t bits;std::memcpy(&bits,&tile.height,4);
        if(comma)out<<',';
        comma=true;
        out<<"{\"index\":"<<i<<",\"name\":"<<quote(tile.name)<<",\"block_source\":"<<tile.block_source
            <<",\"block_name\":"<<quote(tile.name)<<",\"mgx_uri\":"<<quote(uris.at(tile.block_source))
            <<",\"grid\":["<<tile.grid[0]<<','<<tile.grid[1]<<"],\"height_bits\":"<<bits
            <<",\"list_element\":{\"name\":"<<quote(tile.list_element.block_name)
            <<",\"gameplay\":"<<quote(tile.list_element.gameplay)<<",\"visual\":"<<quote(tile.list_element.visual)
            <<",\"chances\":"<<tile.list_element.chances<<"},\"children\":[";bool cc=false;
        for(const auto child:tile.children){if(cc)out<<',';cc=true;out<<child;}
        out<<"]}";
    }
    const auto& e=result.events;
    out<<"],\"events\":{\"random_calls\":"<<e.random_calls<<",\"root_attempts\":"<<e.root_attempts
        <<",\"place_calls\":"<<e.place_calls<<",\"unspawn_calls\":"<<e.unspawn_calls
        <<",\"step_calls\":"<<e.step_calls<<",\"path_calls\":"<<e.path_calls<<",\"rule_calls\":"<<e.rule_calls<<"}}";
    return out.str();
}
static std::string checked_run(const loader::ProceduralRulePlanV1& plan,const std::vector<std::string>& names,
    const std::vector<std::string>& uris,const std::vector<loader::ProceduralBlockV1>& blocks,
    const loader::ProceduralConnectionGraphV1& graph,std::uint32_t seed,loader::ProceduralRulesV1::Borrow owner={}){
    loader::ProceduralLayoutResultV1 result;std::string error;
    const bool ok=owner?loader::generate_procedural_layout_v1(owner,seed,result,error):
        loader::generate_procedural_layout_v1(plan,names,blocks,graph,seed,result,error);
    if(!ok)return "{\"seed\":"+std::to_string(seed)+",\"error\":"+quote(error)+"}";
    const auto row=projection(result,uris);const auto retained=result.source_owner;
    require(!loader::generate_procedural_layout_v1(loader::ProceduralRulesV1::Borrow{},seed,result,error),"Missing owner accepted");
    require(projection(result,uris)==row,"Failed owner changed layout");
    require(!loader::generate_procedural_layout_v1(plan,{},{},graph,seed,result,error),"Mismatched source accepted");
    require(projection(result,uris)==row,"Failed source changed layout");
    auto pooled=plan;pooled.pools.resize(1);
    require(!loader::generate_procedural_layout_v1(pooled,names,blocks,graph,seed,result,error),"Unimplemented pool accepted");
    require(projection(result,uris)==row,"Failed pool changed layout");
    loader::ProceduralLayoutResultV1 repeated;
    require(loader::generate_procedural_layout_v1(plan,names,blocks,graph,seed,repeated,error),error);
    require(projection(repeated,uris)==row,"Repeated generation changed result or source lists");
    if(owner){
        const auto identity=owner.lists().connections().blocks().sources().identity();owner={};
        require(retained&&retained.lists().connections().blocks().sources().identity()==identity,"Layout lost visit ownership");
        for(const auto& document:retained.lists().connections().blocks().sources().documents())
            require(!document.source().empty(),"Layout lost raw source");
    }
    return row;
}
int main(int argc,char** argv){
    try{
        std::string error;
        if(argc==1){
            std::size_t count;
            while(std::cin>>count){
                require(count<=512&&std::cin.get()=='\n',"Invalid block count/separator");
                std::vector<std::string> names,uris;std::vector<loader::ProceduralBlockV1> blocks;
                for(std::size_t i=0;i<count;++i){
                    names.push_back(blob());uris.push_back(blob());const auto raw=blob();loader::XmlDocumentV1 document;
                    require(document.capture(uris.back(),std::vector<std::uint8_t>(raw.begin(),raw.end()),error),error);
                    const auto source=document.borrow();require(!source.roots().empty(),"MGX root unavailable");
                    loader::ProceduralBlockV1 block;
                    require(loader::interpret_procedural_block_v1(source,source.roots()[0],block,error),error);blocks.push_back(std::move(block));
                }
                const auto raw=blob();std::uint32_t seed;require(bool(std::cin>>seed)&&std::cin.get()=='\n',"Seed missing");
                loader::XmlDocumentV1 document;
                require(document.capture("synthetic.rule.xml",std::vector<std::uint8_t>(raw.begin(),raw.end()),error),error);
                const auto source=document.borrow();std::uint32_t root=UINT32_MAX;
                for(const auto index:source.roots())if(source.elements()[index].tag=="rules"){root=index;break;}
                loader::ProceduralConnectionGraphV1 graph;
                require(loader::connect_procedural_blocks_v1(names,blocks,graph,error),error);
                loader::ProceduralListPlanV1 lists;
                require(loader::interpret_procedural_lists_v1(source,root,names,lists,error),error);
                loader::ProceduralRulePlanV1 plan;
                require(loader::interpret_procedural_rules_v1(lists,names,plan,error),error);
                std::cout<<checked_run(plan,names,uris,blocks,graph,seed)<<'\n';
            }
        }else{
            require(argc==5,"Expected cache, identity, definition, seed");loader::ProceduralRulesV1::Borrow retained;
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
                loader::ProceduralRulesV1 rules;require(rules.prepare(lists.borrow(),error),error);retained=rules.borrow();
            }
            const auto& connections=retained.lists().connections();const auto& blocks=connections.blocks();
            std::vector<std::string> names,uris;
            for(const auto& block:blocks.sources().blocks()){
                names.push_back(block.name);uris.push_back(blocks.sources().documents().at(block.document).uri());
            }
            const auto seed=std::stoul(argv[4]);require(seed<=UINT32_MAX,"Seed exceeds uint32 domain");
            std::cout<<checked_run(retained.plan(),names,uris,blocks.blocks(),connections.graph(),std::uint32_t(seed),retained)<<'\n';
        }
    }catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}
}
