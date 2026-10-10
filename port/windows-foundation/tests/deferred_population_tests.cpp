#include "actor_population.hpp"
#include "asset_catalog.hpp"
#include <filesystem>
#include <iostream>
#include <stdexcept>
using namespace dh::foundation;
void check(bool value,const std::string& message){if(!value)throw std::runtime_error(message);}
int main(int argc,char**argv){try{
    check(argc==2,"Supply repository root");
    AssetCatalog assets(std::filesystem::path(argv[1])/".local-inputs/windows-shared-assets");
    ActorProfileLibrary profiles;std::string error;
    check(profiles.load(assets,"actor-profiles-v2.xml",error),error);
    const std::string source="original-cache/data/scene/001_swamp.mlx";
    std::vector<ActorDefinition> definitions;
    check(load_actor_definitions(assets,source,definitions,error),error);
    std::uint64_t selected=0,unknown=0;
    for(const auto&definition:definitions){
        const auto found=definition.properties.find("charpropsname");
        if(definition.gametype!="Character"||found==definition.properties.end()||!profiles.find(found->second))continue;
        if(!selected)selected=definition.stableId;
        else {unknown=definition.stableId;break;}
    }
    check(selected&&unknown,"Two explicit available source actors required");
    ActorPopulation population;
    auto policy=[&](const ActorDefinition& definition){
        if(definition.stableId==selected)return PopulationDecision::deferred;
        if(definition.stableId==unknown)return PopulationDecision::unknown;
        return PopulationDecision::exclude;
    };
    auto customize=[](const ActorDefinition&,const ActorProfile&){
        ActorCustomization result;result.allow_missing_animation_targets=true;
        result.use_authored_modular_defaults=true;return result;
    };
    check(population.load(assets,source,profiles,policy,customize,error),error);
    check(population.actors().size()==1,"Deferred actor did not load exactly once");
    const auto& actor=population.actors().front();
    check(actor.visual.loaded(),"Deferred original visual not ready");
    check(!actor.enabled&&!actor.initially_enabled&&actor.initial_decision==PopulationDecision::deferred,"Deferred activation facts missing");
    check(population.enabled_count()==0&&population.initial_deferred_count()==1,"Deferred population counts differ");
    check(population.definitions().size()==definitions.size(),"Source definitions were not all retained");
    check(population.definitions().front().sourceId==definitions.front().sourceId,"Retained source identity mutated");
    bool sawUnknown=false;for(const auto&notice:population.notices())if(notice.reason.find("unknown")!=std::string::npos)sawUnknown=true;
    check(sawUnknown,"Unknown decision silently treated as deferred");
    const auto placement=actor.transform;const auto animation=std::string(actor.visual.animation_name());
    check(population.set_enabled(selected,true,error),error);
    check(population.enabled_count()==1&&!population.actors().front().initially_enabled,"Initial activation fact overwritten");
    check(population.actors().front().transform==placement&&std::string(population.actors().front().visual.animation_name())==animation,"Activation changed authored placement/pose");
    check(population.set_enabled(selected,false,error),error);
    check(!population.set_enabled(unknown,true,error),"Unavailable unknown actor activated");
    check(population.enabled_count()==0,"Failed activation changed other actor");
    // Caller-constructed duplicate identities must fail atomically.
    PopulationActor duplicate;duplicate.definition.stableId=selected;population.actors().push_back(std::move(duplicate));
    check(!population.set_enabled(selected,true,error),"Ambiguous stable ID accepted");
    check(!population.actors().front().enabled,"Ambiguous activation changed existing actor");
    std::cout<<"PASS visual-ready deferred actor, explicit unknown gap, complete source retention, stable-ID activation and atomic rejection\n";
    return 0;
}catch(const std::exception& error){std::cerr<<error.what()<<'\n';return 1;}}
