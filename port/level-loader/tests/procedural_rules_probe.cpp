#include "procedural_rules_v1.hpp"
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
static void rule(std::ostream& out,const loader::ProceduralRuleV1& r){
    const char* type=r.kind==loader::ProceduralRuleKindV1::root?"RootRule":
        r.kind==loader::ProceduralRuleKindV1::path?"Path":r.kind==loader::ProceduralRuleKindV1::force_block?"ForceBlock":"EndPath";
    out<<"{\"type\":"<<quote(type)<<",\"read_result\":"<<int(r.read_result)<<",\"exit\":"<<quote(r.exit)
        <<",\"id_hash\":"<<r.id_hash<<",\"length\":["<<r.length[0]<<','<<r.length[1]<<"],\"list_source\":";
    if(r.list_source<0)out<<"null";else out<<r.list_source;
    out<<",\"list_index\":"<<r.list_index<<",\"list_validation\":";
    if(!r.list_reference)out<<"null";else out<<(r.list_validation?"true":"false");
    out<<",\"block_validation\":[";bool comma=false;
    for(const auto b:r.block_validation){if(comma)out<<',';comma=true;out<<(b?"true":"false");}
    out<<"],\"block_names\":[";comma=false;
    for(const auto& s:r.block_names){if(comma)out<<',';comma=true;out<<quote(s);}
    out<<"],\"children\":[";comma=false;
    for(const auto& c:r.children){if(comma)out<<',';comma=true;rule(out,c);}
    out<<']';
    if(r.kind==loader::ProceduralRuleKindV1::path)out<<",\"dont_go_back\":"<<(r.dont_go_back?"true":"false");
    if(r.kind==loader::ProceduralRuleKindV1::force_block)out<<",\"connect_from\":"<<r.connect_from;
    out<<'}';
}
static std::string projection(const loader::ProceduralRulePlanV1& p){
    std::ostringstream out;out<<"{\"root_present\":"<<(p.root_present?"true":"false")
        <<",\"read_result\":"<<int(p.rule.read_result)<<",\"rule\":";rule(out,p.rule);
    out<<",\"pools\":[";bool comma=false;
    for(const auto& pool:p.pools){if(comma)out<<',';comma=true;out<<"{\"size\":"<<pool.size<<",\"elements\":[";bool ec=false;
        for(const auto& e:pool.elements){if(ec)out<<',';ec=true;out<<"{\"id_hash\":"<<e.id_hash<<",\"recursive\":"
            <<(e.recursive?"true":"false")<<",\"sizes\":["<<e.sizes[0]<<','<<e.sizes[1]<<','<<e.sizes[2]<<"]}";}
        out<<"]}";}
    out<<"]}";return out.str();
}
static bool same_source(const loader::ProceduralRulePlanV1& a,const loader::ProceduralRulePlanV1& b){
    return a.lists.root==b.lists.root&&bool(a.lists.document)==bool(b.lists.document)&&
        (!a.lists.document||&a.lists.document.source()==&b.lists.document.source());
}
int main(int argc,char** argv){
    try {
        std::string error;
        if(argc==1){
            std::size_t count;loader::ProceduralRulePlanV1 plan;
            while(std::cin>>count){require(count<=4096&&std::cin.get()=='\n',"Group count/separator invalid");
                std::vector<std::string> names;for(std::size_t i=0;i<count;++i)names.push_back(blob());
                const auto raw=blob();loader::XmlDocumentV1 document;
                require(document.capture("synthetic.rule.xml",std::vector<std::uint8_t>(raw.begin(),raw.end()),error),error);
                const auto d=document.borrow();std::uint32_t root=UINT32_MAX;
                for(const auto r:d.roots())if(d.elements()[r].tag=="rules"){root=r;break;}
                loader::ProceduralListPlanV1 lists;require(loader::interpret_procedural_lists_v1(d,root,names,lists,error),error);
                const auto before=plan;
                if(!loader::interpret_procedural_rules_v1(lists,names,plan,error)){
                    require(projection(plan)==projection(before)&&same_source(plan,before),"Failed candidate replaced prior rule plan/source");
                    std::cout<<"{\"error\":"<<quote(error)<<"}\n";continue;}
                const auto previous=plan;
                require(!loader::interpret_procedural_rules_v1({},names,plan,error),"Unavailable lists accepted");
                require(projection(plan)==projection(previous)&&same_source(plan,previous),"Failed source changed rule plan");
                require(plan.lists.document.source()==std::vector<std::uint8_t>(raw.begin(),raw.end()),"Raw rule source changed");
                std::cout<<projection(plan)<<'\n';
            }
        }else{
            require(argc==4,"Expected cache, identity, definition");loader::ProceduralRulesV1::Borrow retained;
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
                loader::ProceduralListsV1 lists;require(lists.prepare(connections.borrow(),error),error);
                loader::ProceduralRulesV1 rules;require(rules.prepare(lists.borrow(),error),error);retained=rules.borrow();
                const auto old=projection(retained.plan());
                require(!rules.prepare({},error),"Unavailable list plan accepted");
                require(projection(rules.borrow().plan())==old,"Failed prepare changed rule plan");
                require(sources.prepare(pack,"separate-visit",argv[3],error),error);
                require(blocks.prepare(sources.borrow(),error),error);require(connections.prepare(blocks.borrow(),error),error);
                require(lists.prepare(connections.borrow(),error),error);require(rules.prepare(lists.borrow(),error),error);
                require(rules.borrow().lists().connections().blocks().sources().identity()=="separate-visit","Separate visit identity lost");
            }
            const auto& sources=retained.lists().connections().blocks().sources();
            require(sources.identity()==argv[2],"Retained identity changed");
            require(!retained.plan().lists.document.source().empty(),"Rule source lost after owner teardown");
            for(const auto& d:sources.documents())require(!d.source().empty(),"MGX source lost after owner teardown");
            std::cout<<"{\"identity\":"<<quote(sources.identity())<<",\"ownership_checks\":true,\"plan\":"<<projection(retained.plan())<<"}\n";
        }
    }catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}
}
