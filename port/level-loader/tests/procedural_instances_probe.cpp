#include "procedural_instances_v1.hpp"
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
static const char* kind(loader::ProceduralRuleKindV1 type){
    switch(type){case loader::ProceduralRuleKindV1::root:return "RootRule";
    case loader::ProceduralRuleKindV1::path:return "Path";
    case loader::ProceduralRuleKindV1::force_block:return "ForceBlock";
    case loader::ProceduralRuleKindV1::end_path:return "EndPath";}return "Unknown";
}
static const loader::ProceduralRuleV1& at(const loader::ProceduralRulePlanV1& plan,const std::vector<std::uint32_t>& path){
    auto* r=&plan.rule;for(const auto index:path)r=&r->children.at(index);return *r;
}
static std::string instance(const loader::ProceduralRuleInstanceV1& v){
    std::ostringstream out;out<<"{\"path\":[";bool comma=false;
    for(const auto i:v.source_path){if(comma)out<<',';comma=true;out<<i;}
    out<<"],\"type\":"<<quote(kind(v.kind))<<",\"parent\":"<<(v.parent_present?"true":"false")
        <<",\"length\":"<<v.length<<",\"exit_direction\":";
    if(v.exit_direction<0)out<<"null";else out<<v.exit_direction;
    out<<",\"block_names\":[";comma=false;
    for(const auto& name:v.block_names){if(comma)out<<',';comma=true;out<<quote(name);}
    out<<"],\"children\":"<<v.child_count<<",\"tile_present\":"<<(v.tile_present?"true":"false")
        <<",\"progress\":"<<v.progress;
    if(v.kind==loader::ProceduralRuleKindV1::path)out<<",\"path_direction\":"<<v.path_direction;
    out<<'}';return out.str();
}
static void paths(const loader::ProceduralRuleV1& rule,std::vector<std::uint32_t> path,
                  std::vector<std::vector<std::uint32_t>>& result){
    result.push_back(path);
    for(std::size_t i=0;i<rule.children.size();++i){auto child=path;child.push_back(std::uint32_t(i));paths(rule.children[i],std::move(child),result);}
}
static std::string projection(std::shared_ptr<const loader::ProceduralRulePlanV1> plan,const std::vector<std::uint32_t>& seeds,
                              loader::ProceduralRulesV1::Borrow owner={}){
    std::vector<std::vector<std::uint32_t>> visits;paths(plan->rule,{},visits);
    std::ostringstream out;out<<"{\"reader_result\":"<<int(plan->rule.read_result)<<",\"rule_nodes\":"<<visits.size()<<",\"runs\":[";
    bool sc=false;loader::ProceduralRuleInstanceV1 retained;
    for(const auto seed:seeds){if(sc)out<<',';sc=true;loader::ProceduralRandomV1 random(seed);
        out<<"{\"seed\":"<<seed<<",\"instances\":[";bool ic=false;
        for(const auto& path:visits){const bool root=at(*plan,path).kind==loader::ProceduralRuleKindV1::root;
            for(unsigned parent=0;parent<(root?1U:2U);++parent){
                std::string error;const auto before=random.state();
                const bool ok=owner?loader::initialize_procedural_rule_instance_v1(owner,path,bool(parent),random,retained,error):
                    loader::initialize_procedural_rule_instance_v1(plan,path,bool(parent),random,retained,error);
                require(ok,error);
                require(retained.source.get()==plan.get(),"Instance lost its owned source plan");
                const auto row=instance(retained);
                if(ic)out<<',';
                ic=true;
                out<<row.substr(0,row.size()-1)<<",\"random_before\":"<<before<<",\"random_after\":"<<random.state()<<'}';
                const auto state=random.state();const auto old_source=retained.source;
                require(!loader::initialize_procedural_rule_instance_v1(std::shared_ptr<const loader::ProceduralRulePlanV1>{},path,bool(parent),random,retained,error),"Missing source accepted");
                require(state==random.state()&&instance(retained)==row&&retained.source==old_source,"Failed source published instance/random state");
                auto bad=path;bad.push_back(UINT32_MAX);
                require(!loader::initialize_procedural_rule_instance_v1(plan,bad,bool(parent),random,retained,error),"Missing source path accepted");
                require(state==random.state()&&instance(retained)==row&&retained.source==old_source,"Failed path published instance/random state");
                if(owner) {
                    require(!loader::initialize_procedural_rule_instance_v1(loader::ProceduralRulesV1::Borrow{},path,bool(parent),random,retained,error),"Missing facade source accepted");
                    require(!loader::initialize_procedural_rule_instance_v1(owner,bad,bool(parent),random,retained,error),"Missing facade path accepted");
                    require(state==random.state()&&instance(retained)==row&&retained.source==old_source&&
                        &retained.source_owner.plan()==old_source.get(),"Failed facade preparation changed instance/source/random state");
                }
            }
        }
        out<<"],\"final_state\":"<<random.state()<<'}';
    }
    out<<"]}";
    const auto raw=plan->lists.document.source();
    const auto identity=owner?owner.lists().connections().blocks().sources().identity():std::string{};
    plan.reset();owner={};
    require(retained.source&&retained.source->lists.document.source()==raw,"Instance source lost after caller teardown");
    if(!identity.empty()) {
        require(bool(retained.source_owner),"Instance lost full source graph");
        const auto& sources=retained.source_owner.lists().connections().blocks().sources();
        require(sources.identity()==identity,"Instance changed visit identity");
        for(const auto& document:sources.documents())require(!document.source().empty(),"Instance lost MGX document");
    }
    return out.str();
}
int main(int argc,char** argv){
    try {
        std::string error;const std::vector<std::uint32_t> seeds{0,1,0x7fffffff,0x80000000,0xffffffff};
        if(argc==1){
            std::size_t count;
            while(std::cin>>count){require(count<=4096&&std::cin.get()=='\n',"Group count/separator invalid");
                std::vector<std::string> names;for(std::size_t i=0;i<count;++i)names.push_back(blob());
                const auto raw=blob();std::shared_ptr<const loader::ProceduralRulePlanV1> retained;
                {
                    loader::XmlDocumentV1 document;
                    require(document.capture("synthetic.rule.xml",std::vector<std::uint8_t>(raw.begin(),raw.end()),error),error);
                    const auto d=document.borrow();std::uint32_t root=UINT32_MAX;
                    for(const auto r:d.roots())if(d.elements()[r].tag=="rules"){root=r;break;}
                    loader::ProceduralListPlanV1 lists;require(loader::interpret_procedural_lists_v1(d,root,names,lists,error),error);
                    auto plan=std::make_shared<loader::ProceduralRulePlanV1>();
                    require(loader::interpret_procedural_rules_v1(lists,names,*plan,error),error);retained=std::move(plan);
                }
                require(retained->lists.document.source()==std::vector<std::uint8_t>(raw.begin(),raw.end()),"Raw rule source changed");
                std::cout<<projection(std::move(retained),seeds)<<'\n';
            }
        }else{
            require(argc==4,"Expected cache, identity, definition");loader::ProceduralRulesV1::Borrow retained;
            {
                auto file=std::make_shared<std::ifstream>(argv[1],std::ios::binary|std::ios::ate);
                require(bool(*file),"Cache unavailable");const auto length=file->tellg();require(length>=0,"Cache length missing");
                assets::ZipBackingV1 backing;backing.owner=file;backing.bytes=std::uint64_t(length);
                backing.read=[file](std::uint64_t pos,void* dst,std::size_t n,std::string& e){
                    file->clear();file->seekg(std::streamoff(pos));file->read(static_cast<char*>(dst),std::streamsize(n));
                    if(!*file){e="Cache read failed";return false;}return true;};
                assets::ZipAssetPackV1 pack;require(pack.mount(std::move(backing),"com.gameloft.android.GAND.GloftD2SS/files/",error),error);
                loader::ProceduralSourcesV1 sources;require(sources.prepare(pack,argv[2],argv[3],error),error);
                loader::ProceduralBlocksV1 blocks;require(blocks.prepare(sources.borrow(),error),error);
                loader::ProceduralConnectionsV1 connections;require(connections.prepare(blocks.borrow(),error),error);
                loader::ProceduralListsV1 lists;require(lists.prepare(connections.borrow(),error),error);
                loader::ProceduralRulesV1 rules;require(rules.prepare(lists.borrow(),error),error);
                retained=rules.borrow();
                require(sources.prepare(pack,"separate-visit",argv[3],error),error);
                require(blocks.prepare(sources.borrow(),error),error);require(connections.prepare(blocks.borrow(),error),error);
                require(lists.prepare(connections.borrow(),error),error);require(rules.prepare(lists.borrow(),error),error);
                require(rules.borrow().lists().connections().blocks().sources().identity()=="separate-visit","Separate visit identity lost");
            }
            require(retained.lists().connections().blocks().sources().identity()==argv[2],"Retained visit identity changed");
            auto owner=std::make_shared<loader::ProceduralRulesV1::Borrow>(retained);
            std::shared_ptr<const loader::ProceduralRulePlanV1> plan(owner,&owner->plan());
            owner.reset();
            const auto value=projection(std::move(plan),seeds,std::move(retained));
            std::cout<<"{\"identity\":"<<quote(argv[2])<<",\"ownership_checks\":true,\"plan\":"<<value<<"}\n";
        }
    }catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}
}
