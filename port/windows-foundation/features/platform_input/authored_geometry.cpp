#include "source_owner_contract.hpp"
namespace dh::foundation::platform_input {
namespace {bool fail(std::string& e,const char* text){if(e.empty())e=text;return false;}}
bool authored_hit(const SourceOwnerBorrow& owner,Point point,Hit& hit,std::string& error) {
    error.clear();hit={};
    if(!owner.receiver_lease||!owner.authored_hud)return fail(error,"Required existing authored HUD owner lease");
    using C=dh2::ui::AuthoredHudControlV1;
    const std::array<std::pair<C,Control>,9> controls{{
        {C::character,Control::profile},{C::potion,Control::potion},{C::faery,Control::spell},
        {C::skill1,Control::skill1},{C::skill2,Control::skill2},{C::skill3,Control::skill3},
        {C::attack,Control::attack},{C::joystick,Control::joystick},{C::pause,Control::none}
    }};
    for(const auto& control:controls) {
        dh2::ui::AuthoredHudGeometryV1 geometry;
        if(!owner.authored_hud->geometry(control.first,point.x,point.y,geometry,error))return false;
        if(geometry.hit) {hit={control.second,std::uint64_t(geometry.character_id)};return true;}
    }
    return true;
}

}

