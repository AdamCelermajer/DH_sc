#include "../player_initial_skill_grants_v1.hpp"

#include <array>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <vector>

namespace g=dh2::player_initial_skill_grants_v1;
namespace p=dh2::player_skill_progression_v1;
namespace d=dh2::data;
constexpr std::uintptr_t C=0x123456789abcdef0ull;
void require(bool value,const char* message) {if(!value)throw std::runtime_error(message);}
std::vector<std::uint8_t> read(const std::filesystem::path& path) {
    std::ifstream file(path,std::ios::binary);require(bool(file),"source cache file missing");
    return {std::istreambuf_iterator<char>(file),{}};
}
struct Fixture {
    d::PropertyRules rules{};d::PropertyState properties{};
    d::PropertyView view{};
    d::SkillTables tables;
    d::PlayerSavegameV1 saved, replacement;
    d::PlayerSavegameV1* current=&saved;
    d::FreshInventoryOwnedV4 inventory;
    g::Bindings bindings{};g::Result result{};std::string error;
    std::array<int,3> caps{{20,25,30}};
    std::vector<p::Operation> effects;
    unsigned slot_updates=0;int fail_slot=-1,fail_effect=-1;bool throws=false;
    bool replace_after_first_slot=false,break_full_recalc_view=false;std::vector<int> slot_equipment;
    Fixture(d::LootTablesV2::Borrow loot,int points=2,int level=1,int required=1,
            int saved_level=0,int difficulty=0,int has_slot=0,int equipment=0)
        :inventory(C,std::move(loot),{},12,properties) {
        rules.defaults.fill(0);rules.types.fill(16);rules.types[157]=32;
        properties.base[19]=level*256;properties.base[28]=0;properties.base[194]=12*256;
        properties.saved[157]=points*256;
        require(d::recalc_properties(rules,properties,error),"fixture property resolution failed");
        view=d::property_view(rules,properties);
        tables.skills.resize(1);tables.skills[0].level=required;tables.skills[0].table_name="SourceFixtureSkill";
        tables.skill_lists.resize(4);tables.skill_lists[0].members={0};tables.skill_lists[3].members={0};
        for(auto* save:{&saved,&replacement}) {
            save->set_character(C);require(save->initialize_skills(tables,0,error),"real saved rows failed");
            require(save->set_skill_level(0,saved_level,error),"fixture saved level failed");
            const std::array<std::uint32_t,2> bytes{{0,std::uint32_t(difficulty)}};std::size_t consumed=0;
            require(save->load_difficulty({reinterpret_cast<const std::uint8_t*>(bytes.data()),sizeof(bytes)},nullptr,
                [](void*,std::int32_t,std::string&){return true;},consumed,error),"profile difficulty reader failed");
        }
        bindings={C,&current,&rules,&properties,&view,&tables,&inventory,{this,update_slot},{this,full_recalculation},{this,nullptr,effect}};
        if(equipment)inventory.swap_equipment();
        if(has_slot)require(saved.set_skill_in_slot(0,0,{this,update_slot},error),"saved profile slot failed");
        slot_updates=0;slot_equipment.clear();
    }
    static bool update_slot(void* raw,std::uintptr_t character,std::string& error) {
        auto& self=*static_cast<Fixture*>(raw);++self.slot_updates;
        self.slot_equipment.push_back(self.inventory.current_equipment());
        require(character==C && self.current->skill_in_slot(0)==0 && self.current->skill_slots()[1].empty(),"saved slot did not mutate source map0 first");
        if(int(self.slot_updates)==self.fail_slot) {
            error="source AI_UpdateSkills failed after slot write";
            if(self.throws)throw std::runtime_error(error);
            return false;
        }
        if(self.replace_after_first_slot && self.slot_updates==1)self.current=&self.replacement;
        return true;
    }
    static int effect(void* raw,const p::Request* request,p::Response* response) {
        auto& self=*static_cast<Fixture*>(raw);require(request->character==C,"effect owner truncated");
        self.effects.push_back(request->operation);
        if(int(request->operation)==self.fail_effect) {
            if(self.throws)throw std::runtime_error("source effect failed");
            return -1;
        }
        switch(request->operation) {
        case p::Operation::skill_limit:
            require(request->arguments[0]>=0 && request->arguments[0]<3,"cap difficulty inferred");
            response->word=std::uint32_t(self.caps[request->arguments[0]]);break;
        case p::Operation::update_all_skills:
            break;
        case p::Operation::recalculate_properties:break;
        case p::Operation::debug_load:case p::Operation::debug_query:break;
        default:throw std::runtime_error("unexpected delegated source operation");
        }
        return 0;
    }
    static int full_recalculation(void* raw,d::PropertyView* view,bool full,std::string&) {
        auto& self=*static_cast<Fixture*>(raw);
        require(view==&self.view && full && view->base==self.properties.base.data() && view->resolved==self.properties.resolved.data(),"source full recalc lost the live view");
        p::Request request{p::Operation::recalculate_properties,C,self.current,{1,0,0,0}};p::Response response{};
        if(effect(raw,&request,&response))return -1;
        const d::ClassRow empty_class{nullptr,0};
        require(dh2_class_recalc_base(&empty_class,1,self.properties.base.data(),view)==0,"fixture source full class/buff property recalc failed");
        if(self.break_full_recalc_view)view->resolved=self.properties.saved.data();
        return 0;
    }
    g::Status initialize() {return g::initialize(&bindings,&result,error);}
    void print(g::Status status) {
        std::cout<<"{\"status\":"<<unsigned(status)<<",\"saved_level\":"<<saved.skill_level(0)
                 <<",\"points_raw\":"<<properties.resolved[157]<<",\"slot0\":"<<saved.skill_in_slot(0)
                 <<",\"map1_size\":"<<saved.skill_slots()[1].size()<<",\"equipment\":"<<inventory.current_equipment()
                 <<",\"slot_updates\":"<<slot_updates<<",\"increments\":"<<result.increment_attempts
                 <<",\"source_return\":"<<result.increment.source_return<<",\"effects\":[";
        for(std::size_t i=0;i<effects.size();++i){if(i)std::cout<<',';std::cout<<unsigned(effects[i]);}
        std::cout<<"]}\n";
    }
};

int main(int argc,char** argv) {try {
    require(argc>=2,"cache required");const std::filesystem::path cache=argv[1];
    const auto raw=read(cache/"loot_table_pyarray.bin"),names=read(cache/"loot_table_pyarraynames.bin"),schema=read(cache/"loot_table_pystructnames.bin");
    d::LootTablesV2 loot;std::string error;
    require(loot.load({raw.data(),raw.size()},{names.data(),names.size()},{schema.data(),schema.size()},error),"actual Loot tables failed");
    if(argc>2 && std::string(argv[2])=="--oracle") {
        require(argc==12,"oracle fields required");
        Fixture fixture(loot.borrow(),std::stoi(argv[3]),std::stoi(argv[4]),std::stoi(argv[5]),std::stoi(argv[6]),std::stoi(argv[7]),std::stoi(argv[8]),std::stoi(argv[9]));
        fixture.caps={{std::stoi(argv[10]),std::stoi(argv[11]),std::stoi(argv[11])}};
        const auto status=fixture.initialize();fixture.print(status);return 0;
    }
    unsigned normal=0,failures=0,guards=0;
    for(int points:{0,1,4})for(int level:{0,1,10})for(int saved:{0,1,65535})for(int profile_slot:{0,1})for(int equipment:{0,1}) {
        Fixture fixture(loot.borrow(),points,level,1,saved,0,profile_slot,equipment);
        require(fixture.initialize()==g::Status::complete,"valid source initialization failed");
        const bool increment=!profile_slot && saved==0 && points>0 && level>=1;
        require(fixture.saved.skill_level(0)==saved+int(increment) && fixture.properties.resolved[157]==(points-int(increment))*256,"starter level/points were synthesized or omitted");
        require(fixture.saved.skill_in_slot(0)==0 && fixture.saved.skill_slots()[1].empty() && fixture.inventory.current_equipment()==equipment,"saved map or equipment owner duplicated");
        require(fixture.slot_updates==(profile_slot?0u:2u) && fixture.result.increment_attempts==unsigned(!profile_slot && saved==0),"initial source order/early return differs");
        if(!profile_slot)require(fixture.slot_equipment==std::vector<int>{equipment,1-equipment},"fresh equipment selection order differs");
        const auto points_after=fixture.properties.resolved[157];fixture.effects.clear();fixture.slot_updates=0;
        require(fixture.initialize()==g::Status::complete && fixture.effects.empty() && fixture.slot_updates==0 && fixture.properties.resolved[157]==points_after,"profile reentry replayed initial grant");++normal;
    }
    for(auto operation:{p::Operation::skill_limit,p::Operation::update_all_skills,p::Operation::recalculate_properties,p::Operation::debug_load,p::Operation::debug_query})for(bool throws:{false,true}) {
        Fixture fixture(loot.borrow());fixture.fail_effect=int(operation);fixture.throws=throws;
        require(fixture.initialize()==g::Status::service_failed && fixture.saved.skill_in_slot(0)==0 && fixture.result.equipment_swaps==2,"failure lost slot prefix");
        const bool mutated=operation!=p::Operation::skill_limit;
        require(fixture.saved.skill_level(0)==int(mutated) && fixture.properties.resolved[157]==(mutated?256:512),"failed grant rolled back or advanced source prefix");++failures;
    }
    for(bool throws:{false,true}) {
        Fixture fixture(loot.borrow());fixture.fail_slot=1;fixture.throws=throws;
        require(fixture.initialize()==g::Status::service_failed && fixture.saved.skill_in_slot(0)==0 && fixture.result.equipment_swaps==0 && fixture.result.increment_attempts==0,"failed saved-slot callback rolled back or continued");++failures;
    }
    {
        Fixture fixture(loot.borrow());fixture.bindings.effects.invoke=nullptr;
        require(fixture.initialize()==g::Status::missing_service && fixture.saved.skill_in_slot(0)==0 && fixture.saved.skill_level(0)==0 && fixture.properties.resolved[157]==512,"missing reached cap fabricated grant");++failures;
        Fixture early(loot.borrow(),2,1,1,3,0,1);early.bindings.effects.invoke=nullptr;early.bindings.slot_updates.update_skills=nullptr;
        require(early.initialize()==g::Status::complete && early.result.slots.service_calls==1,"existing profile required unvisited services");++normal;
        Fixture null_save(loot.borrow());null_save.current=nullptr;
        require(null_save.initialize()==g::Status::complete && null_save.result.equipment_swaps==2 && null_save.result.increment_attempts==0 && null_save.properties.resolved[157]==512,"null source save fabricated saved rows or grant");++normal;
    }
    {
        Fixture grouped(loot.borrow());d::PropertySheet buff{};
        grouped.rules.types[40]=4;grouped.properties.base[40]=256;grouped.properties.gear[40]=512;buff[40]=768;
        const std::int32_t* sheets[]={buff.data()};d::PropertyBuffGroup group{sheets,1};
        grouped.view.groups=&group;grouped.view.group_count=1;
        require(grouped.initialize()==g::Status::complete && grouped.properties.resolved[40]==1536 && grouped.view.groups==&group && grouped.view.group_count==1,"full grant recalc discarded live buff groups");++normal;
        Fixture missing(loot.borrow());missing.bindings.full_recalculation.invoke=nullptr;
        require(missing.initialize()==g::Status::missing_service && missing.saved.skill_level(0)==1 && missing.properties.resolved[157]==256 && missing.result.potion_stores==0,"missing full recalc advanced or rolled back source prefix");++failures;
        Fixture incoherent(loot.borrow());incoherent.break_full_recalc_view=true;
        require(incoherent.initialize()==g::Status::invalid_source_fact && incoherent.result.dependency_status==g::Status::invalid_source_fact && incoherent.saved.skill_level(0)==1 && incoherent.properties.resolved[157]==256 && incoherent.result.potion_stores==0 && incoherent.error=="initial skill live property view mismatch","successful incoherent provider lost diagnostic or source prefix");++failures;
        Fixture fresh(loot.borrow());fresh.replace_after_first_slot=true;
        require(fresh.initialize()==g::Status::complete && fresh.current==&fresh.replacement && fresh.saved.skill_level(0)==0 && fresh.replacement.skill_level(0)==1 && fresh.saved.skill_in_slot(0)==0 && fresh.replacement.skill_in_slot(0)==0 && fresh.slot_updates==2 && fresh.properties.resolved[157]==256,"source saved pointer slot was cached across provider callbacks");++normal;
    }
    {
        Fixture profile(loot.borrow(),3,10,1,0);
        std::vector<std::uint8_t> section;
        auto word=[&](std::uint32_t value){for(unsigned n=0;n<4;++n)section.push_back(std::uint8_t(value>>(n*8)));};
        const auto& name=profile.tables.skills[0].table_name;
        word(1);word(std::uint32_t(name.size()+1));section.insert(section.end(),name.begin(),name.end());section.push_back(0);
        section.push_back(7);section.push_back(0);word(1);word(0);word(0);word(0);
        std::size_t used=0;
        require(profile.saved.load_skills({section.data(),section.size()},profile.tables,used,profile.error)==0 && used==section.size(),"existing profile Skills reader failed");
        require(profile.initialize()==g::Status::complete && profile.saved.skill_level(0)==7 && profile.properties.resolved[157]==768 && !profile.slot_updates && profile.effects.empty(),"loaded profile was overwritten by starter grant");++normal;
    }
    {
        Fixture fixture(loot.borrow());fixture.result.increment_attempts=77;const auto before=fixture.result;
        require(g::initialize(nullptr,&fixture.result,error)==g::Status::invalid_argument && !std::memcmp(&fixture.result,&before,sizeof(before)),"invalid controls changed output");++guards;
        auto* saved_properties=fixture.bindings.properties;fixture.bindings.properties=reinterpret_cast<d::PropertyState*>(&fixture.result);
        require(fixture.initialize()==g::Status::invalid_argument && !std::memcmp(&fixture.result,&before,sizeof(before)),"output/property alias accepted");fixture.bindings.properties=saved_properties;++guards;
        fixture.saved.set_character(C+1);
        require(fixture.initialize()==g::Status::invalid_source_fact && fixture.slot_updates==0 && fixture.properties.resolved[157]==512,"mismatched save granted skill");++guards;
        fixture.saved.set_character(C);fixture.view.resolved=fixture.properties.saved.data();
        require(fixture.initialize()==g::Status::invalid_source_fact && fixture.slot_updates==0 && fixture.properties.resolved[157]==512,"different property view silently rebound");++guards;
        Fixture aliased(loot.borrow());const auto unchanged=aliased.result;
        const std::int32_t* sheets[]={reinterpret_cast<const std::int32_t*>(&aliased.result)};
        d::PropertyBuffGroup group{sheets,1};aliased.view.groups=&group;aliased.view.group_count=1;
        require(aliased.initialize()==g::Status::invalid_argument && !std::memcmp(&aliased.result,&unchanged,sizeof(unchanged)),"output/buff sheet alias accepted");++guards;
    }
    std::cout<<"{\"validation\":\"PASS\",\"normal_cases\":"<<normal<<",\"failure_cases\":"<<failures<<",\"guards\":"<<guards
             <<",\"same_save_property_inventory\":true,\"live_buff_groups_in_full_recalc\":true,\"provider_coherence_failure_preserves_status\":true,\"synthetic_free_grant\":false,\"outer_profile_load\":false,\"new_original_body_credit\":0}\n";
    return 0;
}catch(const std::exception& error){std::cerr<<error.what()<<'\n';return 1;}}
