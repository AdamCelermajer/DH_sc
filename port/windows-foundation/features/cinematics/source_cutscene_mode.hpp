#pragma once
#include "../../../level-world/application_services_owner_v5.hpp"
#include "../../../level-world/application_player_manager_bootstrap_v59.hpp"
#include "../../../level-loader/script_manager_owner_v52.hpp"
#include <functional>
namespace dh::foundation {
// Root UI leaves are genuine methods/process cells, with weak App/World
// captures. This helper reads the published SAME PM and ScriptManager slots.
struct SourceCutsceneModeProviders {
    std::weak_ptr<dh2::application::ApplicationServicesOwnerV5> application;
    std::function<bool(std::shared_ptr<void>&,const std::uint8_t*&,std::string&)> currentLevel198;
    std::function<bool(bool&,std::string&)> forceUi;
    std::function<bool(bool,std::string&)> menuVirtual38;
    std::function<bool(std::uintptr_t,std::string&)> reloadSkills;
    std::function<bool(std::uintptr_t,bool&,std::string&)> isDead;
    std::function<bool(dh2::player::PlayerInfoFieldsV1&,std::string&)> resetDeadLocal;
    std::function<bool(const char*,std::string&)> hudNoArgs;
    std::function<bool(std::uint8_t,std::string&)> storeDisplayHud;
    std::function<bool(bool,int,int,std::string&)> sendScriptMessage;
    std::function<bool(dh2::player::PlayerInfoFieldsV1&,bool,std::string&)> playerInCutscene;
    std::function<bool(std::uintptr_t,bool,std::string&)> renderVisible;
};
class SourceCutsceneMode {
    SourceCutsceneModeProviders providers_;
public:
    explicit SourceCutsceneMode(SourceCutsceneModeProviders p):providers_(std::move(p)){}
    // kind1/2 ignore source skip/module. SKIP visibility is StartCutscene's
    // actual zero-arg AS effect; no independent invented presentation state.
    bool command(bool enter,bool skip,int module,std::string&);
};
}
