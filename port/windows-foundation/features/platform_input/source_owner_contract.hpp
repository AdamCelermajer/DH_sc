#pragma once
#include "semantic_input.hpp"
#include "../../../engine-ui/authored_gameplay_hud_v1.hpp"
#include "../../../engine-ui/menu_manager_update_v58.hpp"
#include <memory>
#include <optional>
#include <string>

namespace dh::foundation::platform_input {
// Publication receipt only; all gameplay/HUD fields stay in existing owners.
struct SourceOwnerBorrow {
    std::shared_ptr<void> receiver_lease;
    std::uint64_t epoch{}, actor_id{};
    std::uintptr_t character{},controller{};
    dh2::ui::AuthoredGameplayHudV1* authored_hud{};
    std::function<bool(dh2::ui::MenuLevelBorrowV58&,std::string&)> current_level;
    // Existing OriginalUiSession whole event/update APIs, wrapped by its owner.
    // Do not dispatch both this route and live_dispatch for the same HUD update.
    std::function<bool(std::int64_t,PointerPhase,Point,std::string&)> native_pointer;
    std::function<bool(std::string&)> controls_update;
};
using BorrowSourceOwner=std::function<bool(SourceOwnerBorrow&,std::string&)>;
// Uses original GameSWF shape traversal. No viewport scaling, rectangles,
// thresholds or replacement HUD receiver are introduced by this helper.
bool authored_hit(const SourceOwnerBorrow&,Point,Hit&,std::string&);
// Genuine nullable CurrentLevel is allowed. Non-null Level requires same lease
// and byte198; rendered scenery or player presence never implies enabled input.
bool source_level_input(const SourceOwnerBorrow&,bool&,std::string&);
class InputOwnerEpoch {
    std::uint64_t epoch_{1};
    std::optional<Frame> pending_release_;
public:
    std::uint64_t value()const noexcept{return epoch_;}
    bool validate(const SourceOwnerBorrow&,std::uint64_t expected_actor,std::string&)const;
    // Deliver release/EndSkill to OLD owners before replacing their world.
    // Failed delivery prevents publication of a new epoch; caller stops reload.
    bool before_replace(SemanticInput&,const std::function<bool(const Frame&,std::string&)>& release_old,std::string&);
};
}
