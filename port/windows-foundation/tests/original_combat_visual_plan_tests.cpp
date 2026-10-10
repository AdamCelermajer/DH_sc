#include "original_combat_visual_plan.hpp"
#include <algorithm>
#include <chrono>
#include <cmath>
#include <fstream>
#include <iostream>
#include <stdexcept>

using namespace dh::foundation;
namespace {
void require(bool result,const char* message) { if(!result)throw std::runtime_error(message); }
struct Fixture {
    std::filesystem::path path;
    Fixture():path(std::filesystem::temp_directory_path()/("dh-combat-plan-"+std::to_string(std::chrono::steady_clock::now().time_since_epoch().count()))) {
        std::filesystem::create_directory(path);
        for(const auto* name:{"model.bdae","template.bdae","pre.bdae","strike.bdae","recover.bdae"})std::ofstream(path/name)<<"fixture";
    }
    ~Fixture(){for(const auto* name:{"model.bdae","template.bdae","pre.bdae","strike.bdae","recover.bdae"})std::filesystem::remove(path/name);std::filesystem::remove(path);}
};
}
int main(int argc,char** argv) {
    try {
        Fixture fixture;
        AssetCatalog assets(fixture.path);
        OriginalMeleeBindings bindings;
        std::string error;
        const std::string xml="<meleeBindings version='1'><factions><faction id='1' name='fixture'/></factions><actor id='fixture' propertyRow='3' factionId='1' aiId='4' aiName='fixture-ai' model='model.bdae' template='template.bdae'><state name='Empty'/><state name='Attack'><sequence id='7' name='combo' loop='0' type='2'><step index='4' animationId='8' redirect='1' speed='2' blendOut='77' moveGO='1'><step index='2' animationId='9' redirect='0' speed='1.3' blendOut='100' moveGO='1' uri='pre.bdae'/><step index='9' animationId='10' redirect='0' speed='1.4' blendOut='55' moveGO='0' uri='strike.bdae'/><step index='10' animationId='11' redirect='0' speed='1.5' blendOut='44' moveGO='1' uri='recover.bdae'/></step><step index='8' animationId='-1' redirect='0' speed='1' blendOut='0' moveGO='0' symbolic='no-animation'/></sequence><sequence id='12' name='other' loop='-1' type='1'><step index='0' animationId='10' redirect='0' speed='0.8' blendOut='50' moveGO='1' uri='strike.bdae'/></sequence></state></actor></meleeBindings>";
        require(bindings.decode({xml.begin(),xml.end()},error),error.c_str());
        ActorCustomization customization;
        customization.skin_id_contains="caller-selector";customization.expected_controller_count=3;
        customization.allow_missing_animation_targets=true;customization.include_static_instances=true;
        OriginalCombatVisualPlan plan;
        require(build_original_combat_visual_plan(assets,bindings,"fixture",customization,"caller-role",plan,error),error.c_str());
        require(plan.config.clips.size()==4 && plan.clipRates.size()==4,"Visual bank dropped repeated source references");
        require(plan.config.skin_id_contains=="caller-selector" && plan.config.expected_controller_count==3 && plan.config.allow_missing_animation_targets && plan.config.include_static_instances,"Caller customization changed");
        require(std::find(plan.stateNames.begin(),plan.stateNames.end(),"Empty")!=plan.stateNames.end() && !plan.sequence("Empty",0),"Empty source state invented or lost");
        const auto* full=plan.sequence("Attack",0);
        require(full && full->phases.size()==4 && full->id==7 && full->loop==0 && full->type==2,"Complete source phase plan lost");
        require(full->phases[0].sourceUri=="pre.bdae" && full->phases[1].sourceUri=="strike.bdae" && full->phases[2].sourceUri=="recover.bdae" && !full->phases[3].has_visual(),"Ordered phases/symbolic leaf changed");
        const auto* strike=plan.phase("Attack",0,{0,1});
        require(strike && strike->sourceIndices==std::vector<std::int64_t>({4,9}) && strike->ancestors.size()==1 && strike->ancestors[0].speed==2 && strike->ancestors[0].blendOut==77,"Redirect ancestry lost");
        require(strike->moveGO==0 && strike->blendOut==55 && std::abs(plan.clipRates.at(strike->clipName)-1.4)<1e-12,"Source leaf playback metadata changed");
        require(!plan.phase("Attack",0,{1}) || !plan.phase("Attack",0,{1})->has_visual(),"Nonvisual phase incorrectly selected as clip");
        require(!plan.phase("Attack",0,{0}) && !plan.phase("Attack",0,{99}) && !plan.sequence("Attack",9),"Missing explicit selection silently defaulted");
        require(plan.phase("Attack",1,{0})->clipName!=strike->clipName,"Distinct source variants share mutable visual alias");
        const auto oldRole=plan.roleId;
        require(!build_original_combat_visual_plan(assets,bindings,"missing",customization,"new-role",plan,error) && plan.roleId==oldRole,"Failed plan replaced existing output");
        require(!build_original_combat_visual_plan(assets,bindings,"fixture",customization,"",plan,error),"Caller role was invented");
        const auto repository=argc>1?std::filesystem::path(argv[1]):std::filesystem::current_path();
        const auto metadata=repository/".local-inputs/windows-melee-bindings";
        const auto sourceAssets=repository/".local-inputs/windows-population-assets";
        if(std::filesystem::exists(metadata/"original-melee-bindings.xml") && std::filesystem::is_directory(sourceAssets)) {
            AssetCatalog tableAssets(metadata),models(sourceAssets);
            require(bindings.load(tableAssets,"original-melee-bindings.xml",error),error.c_str());
            require(build_original_combat_visual_plan(models,bindings,"WanderingPriest",{},"source-role",plan,error),error.c_str());
            full=plan.sequence("Attack",0);
            require(full && full->phases.size()==6,"Original nested attack phases flattened incorrectly");
            require(full->phases[0].sourceUri.find("pre_combo_01")!=std::string::npos && full->phases[1].sourceUri.find("1hand_combo_01.bdae")!=std::string::npos && full->phases[2].sourceUri.find("combo_01_to_idle")!=std::string::npos,"Original pre/strike/recovery order changed");
            require(plan.phase("Attack",0,{0,1})==&full->phases[1],"Original explicit leaf selection incorrect");
            require(std::abs(plan.clipRates.at(full->phases[1].clipName)-1.2999999523162842)<1e-12,"Original animation rate altered");
            std::size_t banks=0;
            for(const auto& actor:bindings.actors()) {
                require(build_original_combat_visual_plan(models,bindings,actor.first,{},"source-role",plan,error),error.c_str());
                banks+=plan.config.clips.size();
            }
            std::cout<<"Original combat banks: "<<bindings.actors().size()<<" actors, "<<banks<<" ordered visual phases\n";
        } else std::cout<<"Source asset fixture unavailable; generic visual plan checks passed\n";
        std::cout<<"Original combat visual plan tests passed\n";
        return 0;
    } catch(const std::exception& exception) {std::cerr<<exception.what()<<'\n';return 1;}
}
