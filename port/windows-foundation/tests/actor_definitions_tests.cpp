#include "../actor_definitions.hpp"
#include <chrono>
#include <cmath>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <set>
#include <stdexcept>

namespace fs=std::filesystem;
using namespace dh::foundation;
namespace {
void check(bool pass,const std::string& why){if(!pass)throw std::runtime_error(why);}
struct Fixture {
    fs::path root=fs::temp_directory_path()/("dh-actor-definitions-"+std::to_string(std::chrono::steady_clock::now().time_since_epoch().count()));
    Fixture(){fs::create_directories(root);}
    ~Fixture(){std::error_code ec;fs::remove_all(root,ec);}
};
void put(const fs::path& file,const std::string& text){fs::create_directories(file.parent_path());std::ofstream out(file,std::ios::binary);out<<text;check(out.good(),"Fixture write failed");}
bool same(const std::vector<ActorDefinition>& a,const std::vector<ActorDefinition>& b){
    if(a.size()!=b.size())return false;
    for(std::size_t i=0;i<a.size();++i)
        if(a[i].stableId!=b[i].stableId||a[i].sourceId!=b[i].sourceId||a[i].properties!=b[i].properties||a[i].placement!=b[i].placement||a[i].unresolvedReferences!=b[i].unresolvedReferences)return false;
    return true;
}
void unique_ids(const std::vector<ActorDefinition>& actors){std::set<std::uint64_t> seen;for(const auto& actor:actors)check(actor.stableId&&seen.insert(actor.stableId).second,"Duplicate or zero stable actor ID");}
const ActorDefinition& named(const std::vector<ActorDefinition>& actors,const std::string& name){for(const auto& actor:actors)if(actor.name==name)return actor;throw std::runtime_error("Expected actor not found: "+name);}
void fixtures(){
    Fixture fixture;AssetCatalog catalog(fixture.root);std::string error;std::vector<ActorDefinition> actors;
    put(fixture.root/"levels/test.mlx",R"(<Level><GameObject name="one" gametype="Module" dae="world.bdae" xrefobject="root" mgp="units.mgp" position="10 20 30" rotation="0 90 0"/><GameObject name="two" gametype="Module" dae="world.bdae" xrefobject="root" mgp="units.mgp" position="100 200 300"/></Level>)");
    put(fixture.root/"levels/units.mgp",R"(<Module><GameObject name="guard" gametype="Character" template="elite" position="1 2 3" health="99" activate_cond="quest_started"/><GameObject name="spawn" gametype="SpawnPoint" probability="37" data="authored_spawn_table"/></Module>)");
    put(fixture.root/"data/templates.xml",R"(<Templates><Template name="base" health="50" role="enemy" speed="7"><Property name="attack" value="4"/></Template><Template name="elite" template="base" health="75" armor="12"/></Templates>)");
    ActorLoadOptions options;options.referenceDocuments={"data/templates.xml"};options.requireReferencedFiles=true;
    check(load_actor_definitions(catalog,"levels/test.mlx",actors,error,options),"Valid actor fixture rejected: "+error);
    check(actors.size()==6,"Module declarations were not loaded once per placed instance");unique_ids(actors);
    const auto& guard=named(actors,"guard");
    check(guard.properties.at("health")=="99"&&guard.properties.at("speed")=="7"&&guard.properties.at("armor")=="12"&&guard.properties.at("attack")=="4","Authored override or multilevel template defaults lost");
    check(guard.properties.at("activate_cond")=="quest_started"&&guard.role=="enemy","Authored condition or inherited role lost");
    check(std::abs(guard.placement[12]-11)<0.001f&&std::abs(guard.placement[13]-22)<0.001f&&std::abs(guard.placement[14]-33)<0.001f,"Child placement was rotated or failed to receive module offset");
    check(named(actors,"spawn").properties.at("probability")=="37","Authored spawn probability not preserved");
    std::vector<ActorDefinition> again;
    check(load_actor_definitions(catalog,"levels/test.mlx",again,error,options)&&same(actors,again),"Reload changed actor data or stable IDs");
    auto preserved=actors;
    put(fixture.root/"levels/units.mgp","<Module><GameObject></Module>");
    check(!load_actor_definitions(catalog,"levels/test.mlx",actors,error,options)&&!error.empty()&&same(actors,preserved),"Malformed referenced XML changed prior output");
    put(fixture.root/"levels/units.mgp",R"(<Module><GameObject name="bad" gametype="Character" position="1 2 nope"/></Module>)");
    check(!load_actor_definitions(catalog,"levels/test.mlx",actors,error,options)&&same(actors,preserved),"Invalid transform changed prior output");
    put(fixture.root/"levels/units.mgp",R"(<Module><GameObject name="cycle" gametype="Character" template="a"/></Module>)");
    put(fixture.root/"data/templates.xml",R"(<Templates><Template name="a" template="b"/><Template name="b" template="a"/></Templates>)");
    check(!load_actor_definitions(catalog,"levels/test.mlx",actors,error,options)&&same(actors,preserved),"Template cycle was accepted or changed prior output");
    put(fixture.root/"data/templates.xml",R"(<Templates><Template name="duplicate"/><Template name="duplicate"/></Templates>)");
    check(!load_actor_definitions(catalog,"levels/test.mlx",actors,error,options)&&same(actors,preserved),"Ambiguous template was accepted or changed prior output");
    put(fixture.root/"levels/unsafe.mlx",R"(<Level><GameObject name="bad" gametype="Module" dae="world.bdae" xrefobject="root" mgp="../../units.mgp"/></Level>)");
    options.referenceDocuments.clear();
    check(!load_actor_definitions(catalog,"levels/unsafe.mlx",actors,error,options)&&same(actors,preserved),"Unsafe module reference was accepted through fallback");
    std::cout<<"Actor fixture identity, template inheritance, placement, and transaction checks passed\n";
}
void original(const fs::path& root){
    AssetCatalog catalog(root);std::vector<ActorDefinition> actors,again;std::string error;
    ActorLoadOptions options;options.requireReferencedFiles=true;
    check(load_actor_definitions(catalog,"data/scene/001_swamp.mlx",actors,error,options),"Original actor load failed: "+error);
    unique_ids(actors);std::size_t characters=0,spawns=0;
    for(const auto& actor:actors){characters+=actor.gametype=="Character";spawns+=actor.gametype=="SpawnPoint";check(!actor.sourcePath.empty()&&!actor.sourceId.empty(),"Original actor lost source provenance");}
    check(characters==50,"Original swamp expected 50 authored characters, got "+std::to_string(characters));
    check(spawns==11,"Original swamp expected 11 authored spawn points, got "+std::to_string(spawns));
    check(load_actor_definitions(catalog,"data/scene/001_swamp.mlx",again,error,options)&&same(actors,again),"Original actor reload was nondeterministic");
    std::cout<<"Original actor declarations: "<<characters<<" characters, "<<spawns<<" spawn points, "<<actors.size()<<" total objects; stable IDs verified\n";
}
}
int main(int argc,char** argv){try{fixtures();const fs::path repository=argc>1?fs::path(argv[1]):fs::current_path();original(argc>2?fs::path(argv[2]):repository/"port/windows-foundation/assets");return 0;}catch(const std::exception& error){std::cerr<<"Actor definition test failure: "<<error.what()<<'\n';return 1;}}
