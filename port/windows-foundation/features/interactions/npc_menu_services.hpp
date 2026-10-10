#pragma once
#include "npc_interact.hpp"
#include "../../../engine-ui/swf_movie.hpp"
namespace dh::foundation::interactions {
struct NpcMenuReceiverBorrow {
    std::shared_ptr<void> receiver;
    std::uintptr_t renderfx{};
    dh2::ui::SwfMovie* movie{};
};
struct NpcPlayerInfoBorrow {
    std::shared_ptr<void> receiver;
    std::uintptr_t identity{};
    const std::int32_t* id678{};
};
struct NpcMenuOwnerServices {
    std::shared_ptr<void> manager;
    // Must read the existing MenuManager globals fresh at each source lookup.
    std::function<bool(NpcMenuReceiverBorrow&,std::string&)> hud_root,merchant_root;
    // Resolve a captured RenderFX identity to its SAME already-loaded movie.
    // Never load another SWF or return current HUD for a different identity.
    std::function<bool(std::uintptr_t,NpcMenuReceiverBorrow&,std::string&)> renderfx;
    // Whole PlayerManager.GetPlayerByCharacter(character,false), same PlayerInfo.
    std::function<bool(std::uintptr_t,NpcPlayerInfoBorrow&,std::string&)> player_info;
};
bool bind_npc_menu_services(NpcInteractServices&,NpcMenuOwnerServices,std::string&);
bool invoke_npc_menu_as(const NpcMenuReceiverBorrow&,const char*,const char*,const std::vector<NpcAsValue>&,std::string&);
}
