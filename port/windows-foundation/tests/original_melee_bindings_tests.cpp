#include "original_melee_bindings.hpp"
#include <cmath>
#include <functional>
#include <iostream>
#include <stdexcept>

using namespace dh::foundation;
namespace {
void require(bool result,const char* message) { if(!result) throw std::runtime_error(message); }
bool decode(OriginalMeleeBindings& bindings,const std::string& xml,std::string& error) {
    return bindings.decode({xml.begin(),xml.end()},error);
}
const std::string prefix="<meleeBindings version='1'><factions fallbackId='1'><faction id='1' name='source'><relation target='2' value='-1000'/></faction><faction id='2' name='target'><relation target='1' value='0'/></faction></factions>";
const std::string actor="<actor id='fixture' propertyRow='33' factionId='1' aiId='4' aiName='fixture-ai' model='model.bdae' condition='quest_unknown'><ai AttackDelay='1500' MeleeRadius='200.0' Script='symbolic'/><state name='Attack' stance='explicit'><sequence id='7' name='combo' loop='0' type='2'><step index='0' animationId='8' redirect='1' speed='1' blendOut='100' moveGO='1'><step index='0' animationId='9' redirect='0' speed='1.3' blendOut='100' moveGO='1' uri='clip.bdae'/></step></sequence><sequence id='7' name='combo' loop='0' type='2'/></state><state name='Empty'/><symbolic ref='source-reference'/></actor>";
}
int main(int argc,char** argv) {
    try {
        OriginalMeleeBindings bindings;
        std::string error;
        require(decode(bindings,prefix+actor+"<clips><clip uri='clip.bdae' startMs='-20' endMs='900'><marker name='hit' timeMs='234' authoredTimeMs='233'/><marker name='hit' timeMs='867' authoredTimeMs='866'/></clip></clips></meleeBindings>",error),error.c_str());
        require(bindings.relationship(1,2)==-1000 && bindings.relationship(2,1)==0,"Directed signed relations lost");
        require(!bindings.relationship(2,2) && !bindings.relationship(99,1),"Missing relationships guessed");
        require(bindings.fallback_faction_id()==1,"Authored fallback lost");
        const auto* row=bindings.find_actor("fixture");
        require(row && row->properties.at("condition")=="quest_unknown" && row->aiProperties.at("AttackDelay")=="1500","Raw condition/AI fields lost");
        require(bindings.actors_for_row(33).size()==1 && bindings.actors_for_row(34).empty(),"Row lookup guessed");
        require(row->states.at("Empty").empty() && !bindings.sequence("fixture","Empty",0),"Empty state replaced");
        const auto* seq=bindings.sequence("fixture","Attack",0);
        require(seq && seq->steps.size()==1 && seq->steps[0].children.size()==1,"Nested redirect flattened");
        require(std::abs(seq->steps[0].children[0].speed-1.3)<1e-12,"Original step speed lost");
        require(bindings.sequence("fixture","Attack",1)!=nullptr && !bindings.sequence("fixture","Attack",2),"Explicit variants/repeated sequence IDs changed");
        const auto* clip=bindings.find_clip("CLIP.BDAE");
        require(clip && clip->startMs==-20 && clip->markers.size()==2 && clip->markers[0].timeMs==234 && clip->markers[1].authoredTimeMs==866,"Clip bounds/repeated markers lost");
        require(!decode(bindings,prefix+actor+actor+"</meleeBindings>",error),"Duplicate actor accepted");
        require(bindings.find_actor("fixture") && bindings.clips().size()==1,"Failed load replaced old bindings");
        require(!decode(bindings,"<!DOCTYPE meleeBindings>"+prefix+actor+"</meleeBindings>",error),"DTD accepted");
        auto invalid=actor;
        const auto speed=invalid.find("speed='1.3'"); invalid.replace(speed,11,"speed='nan'");
        require(!decode(bindings,prefix+invalid+"</meleeBindings>",error),"Nonfinite step speed accepted");
        require(!decode(bindings,prefix+actor+"</broken>",error),"Malformed XML accepted");
        std::string deep;
        for(int i=0;i<130;++i)deep+="<n>";
        for(int i=0;i<130;++i)deep+="</n>";
        require(!decode(bindings,deep,error),"XML recursion bound ignored");
        const auto repository=argc>1?std::filesystem::path(argv[1]):std::filesystem::current_path();
        const auto source=repository/".local-inputs/windows-melee-bindings";
        if(std::filesystem::is_regular_file(source/"original-melee-bindings.xml")) {
            AssetCatalog assets(source);
            require(bindings.load(assets,"original-melee-bindings.xml",error),error.c_str());
            require(bindings.actors().size()==25 && bindings.factions().size()==16 && bindings.clips().size()==95,"Original binding table counts changed");
            require(bindings.relationship(11,7)==-1000 && bindings.relationship(15,7)==1000 && !bindings.relationship(7,15),"Original directed relationships changed");
            const auto* priest=bindings.find_actor("WanderingPriest");
            require(priest && priest->propertyRow==419 && priest->aiProperties.at("MeleeRadius")=="200.0","Original actor metadata changed");
            const auto* attack=bindings.sequence("WanderingPriest","Attack",0);
            require(attack && attack->steps.size()==2 && attack->steps[0].children.size()==3 && attack->steps[0].redirect==1,"Original combo grouping changed");
            const auto* lizard=bindings.find_clip("data/3D/characters/lizardman/animations/lizardman_attack_01.bdae");
            require(lizard && lizard->startMs==100 && lizard->markers.size()==2 && lizard->markers[0].timeMs==234 && lizard->markers[1].timeMs==867,"Original repeated hit occurrences lost");
            const auto* spawn=bindings.sequence("Swamp_LizadMan_Type1","Spawn",0);
            require(spawn && spawn->id==380 && spawn->type==0 && spawn->loop==0 && spawn->steps.size()==1 && spawn->steps[0].animationId==828,
                    "Original lizard spawn sequence missing");
            std::size_t leaves=0;
            std::function<void(const OriginalMeleeStep&)> verify=[&](const OriginalMeleeStep& step) {
                if(!step.uri.empty()) {require(bindings.find_clip(step.uri)!=nullptr,"Original leaf clip lacks exact marker metadata");++leaves;}
                for(const auto& child:step.children)verify(child);
            };
            for(const auto& actor:bindings.actors())for(const auto& state:actor.second.states)for(const auto& sequence:state.second)for(const auto& step:sequence.steps)verify(step);
            std::cout<<"Original bindings: 25 actors, 16 factions, 95 clips, "<<leaves<<" ordered leaf references\n";
        } else std::cout<<"Original source fixture unavailable; generic parser checks passed\n";
        std::cout<<"Original melee binding tests passed\n";
        return 0;
    } catch(const std::exception& exception) {std::cerr<<exception.what()<<'\n';return 1;}
}
