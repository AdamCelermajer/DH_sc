#include "monster_decisions.hpp"
namespace dh::foundation::enemy_ai {
bool update_aggro(const Borrow& b,dh2::character::CharacterAiPointerFieldsV105& fields,
    dh2::character::TargetState48& target,dh2::character::AggroFrameServicesV108 services,std::string& error) {
    error.clear();if(!b.actor||!b.properties||!b.tables||!row(b)){error="Required same live enemy actor/properties/AI tables";return false;}
    if(!target.owner||target.owner->identity!=b.actor->id||target.target!=b.actor->target_id) {
        error="Aggro update requires same live actor and authoritative target projection";return false;
    }
    services.view_radius=[&b](float& out,std::string& e){const auto* r=row(b);if(!r){e="Required current original enemy AI row";return false;}out=r->view_radius;return true;};
    services.no_aggro_radius=[&b](float& out,std::string& e){const auto* r=row(b);if(!r){e="Required current original enemy AI row";return false;}out=r->view_radius_no_aggro;return true;};
    if(!dh2::character::character_update_aggro_v108(fields,target,services,error)){
        if(error.empty())error="Required original enemy aggro service failed";
        return false;
    }
    if(target.target!=b.actor->target_id){error="Source aggro callback failed to publish same actor target";return false;}
    return true;
}
} // namespace dh::foundation::enemy_ai

