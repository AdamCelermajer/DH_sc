#include "procedural_lists_v1.hpp"
#include <fstream>
#include <iostream>
#include <sstream>
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
static void list(std::ostream& out,const loader::ProceduralListDeclarationV1& l){
    out<<"{\"name\":"<<quote(l.name)<<",\"replacement\":"<<(l.replacement?"true":"false")<<",\"elements\":[";
    bool comma=false;
    for(const auto& e:l.elements){if(comma)out<<',';comma=true;
        out<<"{\"block_name\":"<<quote(e.block_name)<<",\"gameplay\":"<<quote(e.gameplay)
            <<",\"visual\":"<<quote(e.visual)<<",\"chances\":"<<e.chances
            <<",\"block_available\":"<<(e.block_available?"true":"false")<<'}';}
    out<<"]}";
}
static std::string projection(const loader::ProceduralListPlanV1& p){
    std::ostringstream out;out<<"{\"declarations\":[";bool comma=false;
    for(const auto& l:p.declarations){if(comma)out<<',';comma=true;list(out,l);}
    out<<"],\"selected_sources\":[";comma=false;
    for(const auto i:p.selected_sources){if(comma)out<<',';comma=true;out<<i;}
    out<<"],\"lists\":[";comma=false;
    for(const auto i:p.selected_sources){if(comma)out<<',';comma=true;list(out,p.declarations.at(i));}
    out<<"]}";return out.str();
}
static bool same_source(const loader::ProceduralListPlanV1& a,const loader::ProceduralListPlanV1& b){
    return a.root==b.root&&bool(a.document)==bool(b.document)&&
        (!a.document||&a.document.source()==&b.document.source());
}
int main(int argc,char** argv){
    try {
        std::string error;
        if(argc==1){
            std::size_t count;loader::ProceduralListPlanV1 plan;
            while(std::cin>>count){require(count<=4096&&std::cin.get()=='\n',"Group count/separator invalid");
                std::vector<std::string> names;for(std::size_t i=0;i<count;++i)names.push_back(blob());
                const auto raw=blob();loader::XmlDocumentV1 document;
                require(document.capture("synthetic.rule.xml",std::vector<std::uint8_t>(raw.begin(),raw.end()),error),error);
                const auto d=document.borrow();std::uint32_t root=UINT32_MAX;
                for(const auto r:d.roots())if(d.elements()[r].tag=="rules"){root=r;break;}
                const auto before=plan;
                if(!loader::interpret_procedural_lists_v1(d,root,names,plan,error)){
                    require(projection(plan)==projection(before)&&same_source(plan,before),"Failed candidate replaced prior list plan/source");
                    std::cout<<"{\"error\":"<<quote(error)<<"}\n";continue;}
                require(!plan.document.source().empty(),"Retained XML source unavailable");
                const auto previous=plan;
                require(!loader::interpret_procedural_lists_v1(d,UINT32_MAX,names,plan,error),"Invalid root accepted");
                require(projection(plan)==projection(previous)&&same_source(plan,previous),"Failed root changed list plan/source");
                // Source declarations still contain authored case. Interpretation
                // must not mutate the retained tree like original ToLowerCase.
                require(plan.document.source()==std::vector<std::uint8_t>(raw.begin(),raw.end()),"Raw source changed");
                std::cout<<projection(plan)<<'\n';
            }
        }else{
            require(argc==4,"Expected cache, identity, definition");loader::ProceduralListsV1::Borrow retained;
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
                loader::ProceduralConnectionsV1 connections;require(connections.prepare(blocks.borrow(),error),error);
                loader::ProceduralListsV1 lists;require(lists.prepare(connections.borrow(),error),error);retained=lists.borrow();
                const auto old=projection(retained.plan());
                require(!lists.prepare({},error),"Unavailable connections accepted");
                require(projection(lists.borrow().plan())==old,"Failed prepare changed list plan");
                require(sources.prepare(pack,"separate-visit",argv[3],error),error);
                require(blocks.prepare(sources.borrow(),error),error);require(connections.prepare(blocks.borrow(),error),error);
                require(lists.prepare(connections.borrow(),error),error);
                require(lists.borrow().connections().blocks().sources().identity()=="separate-visit","Separate visit identity lost");
            }
            const auto& sources=retained.connections().blocks().sources();
            require(sources.identity()==argv[2],"Retained identity changed");
            require(!retained.plan().document.source().empty(),"Rule lost after ZIP/owner teardown");
            for(const auto& d:sources.documents())require(!d.source().empty(),"Block source lost after owner teardown");
            std::cout<<"{\"identity\":"<<quote(sources.identity())<<",\"folder\":"<<quote(sources.folder())
                <<",\"ownership_checks\":true,\"plan\":"<<projection(retained.plan())<<"}\n";
        }
    }catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}
}
