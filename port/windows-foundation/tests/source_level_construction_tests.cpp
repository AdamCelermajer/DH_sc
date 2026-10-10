#include "source_level_construction.hpp"
#include "asset_catalog.hpp"
#include "../level-world/canonical_level_config_module_v1.hpp"
#include <iostream>
#include <stdexcept>
using namespace dh::foundation;
namespace {
void check(bool value,const std::string& message){if(!value)throw std::runtime_error(message);}
struct Fixture {
    std::shared_ptr<int> lease=std::make_shared<int>(1);
    std::int32_t room=-1;float offset[3]{};
    dh2::world::ModuleRuntimeGlobalsV1 globals;
    SourceLevelConstructionProviders providers(){
        SourceLevelConstructionProviders p;p.levelOwner=p.candidateOwner=p.globalsOwner=p.classesOwner=lease;
        p.levelModuleId18c=&room;p.levelModuleOffset160=offset;p.moduleGlobals=&globals;
        p.manager=std::make_shared<dh2::world::CanonicalObjectManagerV1>(dh2::world::CanonicalObjectManagerServicesV1{});
        // Explicit test parser/release boundary callbacks. Construction is NOT
        // mocked successful: absent actual class providers fail when reached.
        p.file.parse_result=[](bool parsed,std::string& e){if(!parsed){e="test parser rejected malformed input";return false;}return true;};
        p.file.release_load_state=[](std::string&){return true;};return p;
    }
};
std::vector<std::uint8_t> xml(const char* text){return {text,text+std::char_traits<char>::length(text)};}
SourceLevelConstructionStep finish(SourceLevelConstruction& owner){
    for(unsigned i=0;i<100000;++i){const auto status=owner.tick();if(status!=SourceLevelConstructionStep::pending)return status;}
    throw std::runtime_error("source construction test polling did not finish");
}
void run(const char* repo){
    std::string error;Fixture fixture;
    SourceLevelConstruction empty;
    check(empty.begin(fixture.providers(),"empty.mlx",xml("<Level/>"),0,error),error);
    check(finish(empty)==SourceLevelConstructionStep::complete&&empty.status().attempts==0,"empty original root has complete zero-construction traversal");
    check(!empty.begin(fixture.providers(),"again",xml("<Level/>"),1,error),"begin cannot replay reached owner");
    SourceLevelConstruction player;
    check(player.begin(fixture.providers(),"player.mlx",xml("<Level><GameObject gametype='Player' name='excluded'/></Level>"),0,error),error);
    check(finish(player)==SourceLevelConstructionStep::complete&&player.status().attempts==1&&player.status().modules==0,"source Level Player exclusion requires no fake class constructor");
    SourceLevelConstruction stop;
    check(stop.begin(fixture.providers(),"order.mlx",xml("<Level><GameObject gametype='Decor' name='first'/><GameObject gametype='Module' name='later'/></Level>"),0,error),error);
    check(finish(stop)==SourceLevelConstructionStep::failed&&stop.status().gametype=="Decor"&&stop.status().attempts==1,"unfiltered source walk stops at first reached missing class; never filters to Module");
    check(stop.status().error.find("registered constructor")!=std::string::npos,"precise reached construction callback reported");
    check(stop.tick()==SourceLevelConstructionStep::failed&&stop.status().attempts==1,"failed source prefix cannot be replayed");
    SourceLevelConstruction context;auto p=fixture.providers();fixture.room=7;
    check(!context.begin(p,"bad-context.mlx",xml("<Level/>"),0,error),"nonreset actual Level context rejected");fixture.room=-1;
    SourceLevelConstruction release;auto q=fixture.providers();q.file.release_load_state={};
    check(release.begin(q,"release.mlx",xml("<Level/>"),0,error),error);
    check(finish(release)==SourceLevelConstructionStep::failed&&release.status().error.find("release continuation")!=std::string::npos,"missing reached source release is failure, not success");
    SourceLevelConstruction reentrant;auto r=fixture.providers();r.file.before_element=[&](auto&,auto,std::string&){reentrant.tick();return true;};
    check(reentrant.begin(r,"reentrant.mlx",xml("<Level><GameObject gametype='Module' name='one'/></Level>"),0,error),error);
    check(finish(reentrant)==SourceLevelConstructionStep::failed&&reentrant.status().attempts==0,"reentrant callback latches before constructor side effects");
    AssetCatalog assets(std::string(repo)+"/port/windows-foundation/assets");
    const auto data=assets.read("original-cache/data/scene/001_swamp.mlx");
    SourceLevelConstruction actual;
    check(actual.begin(fixture.providers(),"original-cache/data/scene/001_swamp.mlx",data,0,error),error);
    check(finish(actual)==SourceLevelConstructionStep::failed&&actual.status().gametype=="LevelConfig"&&actual.status().attempts==1,
          "actual cached unfiltered Level reaches missing LevelConfig constructor first; no invented module mapping");
    std::cout<<"Actual-cache reached callback: "<<actual.status().error<<'\n';
}
}
int main(int argc,char** argv){try{run(argc>1?argv[1]:".");std::cout<<"Source Level construction traversal/failure contract PASS (boundary fixtures declared)\n";return 0;}catch(const std::exception& ex){std::cerr<<ex.what()<<'\n';return 1;}}
