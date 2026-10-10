#include "player_profile_properties.hpp"
#include <algorithm>
#include <cmath>
#include <cstring>
namespace dh::foundation {
namespace {
bool fixed(float value,std::int32_t& output){
    const double raw=std::round(double(value)*256.0);
    if(!std::isfinite(raw)||raw<0||raw>INT32_MAX)return false;
    output=static_cast<std::int32_t>(raw);return true;
}
std::int32_t delta(std::int32_t wanted,std::int32_t current){
    const auto bits=std::uint32_t(wanted)-std::uint32_t(current);std::int32_t value;
    std::memcpy(&value,&bits,sizeof(value));return value;
}
}
bool project_player_profile_properties(const OriginalPropertyDatabase& database,
    const CharacterState& profile,const OriginalCombatProperties& prepared,
    OriginalCombatProperties& output,std::string& error){
    error.clear();const auto validation=validate_character_state(profile);
    if(!validation.ok()){error=validation.errors.front();return false;}
    const auto row=std::find(database.characters.names.begin(),database.characters.names.end(),profile.class_id);
    if(row==database.characters.names.end()||
       std::size_t(row-database.characters.names.begin())>=database.characters.rows.size()||
       database.characters.rows[std::size_t(row-database.characters.names.begin())][26]!=prepared.sheets.base[26]){
        error="Prepared source class differs from selected profile";return false;
    }
    if(prepared.sheets.resolved[19]<0||std::uint32_t(prepared.sheets.resolved[19]>>8)!=profile.stats.level){
        error="Prepared source level differs from selected profile";return false;
    }
    if(profile.experience>std::uint64_t(INT32_MAX/256)||
       (profile.source_points_known&&(profile.source_stat_points>std::uint32_t(INT32_MAX/256)||
       profile.source_skill_points>std::uint32_t(INT32_MAX/256)))){
        error="Profile progression exceeds source signed Q8 range";return false;
    }
    dh2::data::PropertyRules rules;if(!dh2::data::load_property_rules(database.characters,rules,error))return false;
    auto candidate=prepared;
    // Class formulas include additive destinations. Rebuild the derived base
    // from authored inputs instead of applying them to an already derived base.
    // This is a startup/profile projection, not a live buff-sheet replacement.
    const auto authored=database.characters.rows[std::size_t(row-database.characters.names.begin())];
    auto recalc=[&](){
        candidate.sheets.base=authored;
        candidate.sheets.base[19]=prepared.sheets.base[19];
        return dh2::data::recalc_properties_with_class(database.classes,rules,candidate.sheets,error);
    };
    auto set=[&](unsigned property,std::int32_t wanted){
        auto view=dh2::data::property_view(rules,candidate.sheets);
        if(dh2_property_add(&view,property,delta(wanted,candidate.sheets.resolved[property]))||
           candidate.sheets.resolved[property]!=wanted){
            error="Source profile property cannot be projected: "+std::to_string(property);return false;
        }
        return true;
    };
    // These are final semantic attributes in CharacterState. Use source Add
    // with a delta, not Set(saved=final), because base/gear can contribute too.
    const std::pair<unsigned,float> attributes[]={{149,profile.stats.strength},{150,profile.stats.dexterity},
        {151,profile.stats.endurance},{152,profile.stats.energy}};
    for(const auto& attribute:attributes){
        if(attribute.first>=151&&!profile.source_endurance_energy_known)continue;
        std::int32_t wanted=0;if(!fixed(attribute.second,wanted)){error="Profile attribute exceeds source Q8 range";return false;}
        if(!set(attribute.first,wanted))return false;
    }
    if(!recalc())return false;
    const auto xp=candidate.sheets.resolved[33];
    if(xp<0||std::uint64_t(xp>>8)!=profile.experience)
        if(!set(33,static_cast<std::int32_t>(profile.experience*256)))return false;
    if(profile.source_points_known){
        if(!set(148,static_cast<std::int32_t>(profile.source_stat_points*256))||
           !set(157,static_cast<std::int32_t>(profile.source_skill_points*256)))return false;
    }
    for(const auto vital:{std::pair<unsigned,float>{36,profile.stats.health},{41,profile.stats.resource}}){
        std::int32_t wanted=0;if(!fixed(vital.second,wanted)){error="Profile vital exceeds source Q8 range";return false;}
        const auto maximum=candidate.sheets.resolved[vital.first==36?38:43];
        if(maximum<0||wanted>maximum){error="Profile vital exceeds prepared source maximum";return false;}
        if(!set(vital.first,wanted))return false;
    }
    // Ensure a normal gear/class recomputation retains the saved contributions.
    const auto expected=candidate.sheets.resolved;
    if(!recalc())return false;
    for(const auto property:{33u,36u,41u,148u,149u,150u,151u,152u,157u}){
        if((property==148||property==157)&&!profile.source_points_known)continue;
        if((property==151||property==152)&&!profile.source_endurance_energy_known)continue;
        if(candidate.sheets.resolved[property]!=expected[property]){error="Profile projection did not survive source recalculation";return false;}
    }
    output=std::move(candidate);return true;
}
}
