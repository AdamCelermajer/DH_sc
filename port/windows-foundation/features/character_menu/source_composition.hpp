#pragma once

#include "character_menu.hpp"
#include "../../../engine-ui/character_menu_actions_owner_v1.hpp"

#include <array>
#include <map>
#include <memory>

namespace dh::foundation::character_menu {

// Root supplies adapters around the independently owned Equipment, Skills,
// and Faery presenters. owner is the SAME canonical selected-character lease
// for all three providers; these callbacks never create or retain gameplay
// copies. ready() is a read-only source-provider check and append() projects
// only the requested page into the shared 480x320 menu frame.
struct SourcePageProviderV1 {
    std::shared_ptr<void> owner;
    std::function<bool(std::string&)> ready;
    std::function<bool(Frame&, std::string&)> append;
    std::function<bool(float authored_x, float authored_y, std::string&)> release;
};

inline bool dispatch_source_stat_training_v1(
    dh2::ui::CharacterMenuActionsOwnerV1&,
    std::uint32_t, std::string&);
inline bool source_stat_training_admissible_v1(
    dh2::ui::CharacterMenuActionsOwnerV1&, std::string&);

class SourceCompositionV1 {
    struct State {
        std::shared_ptr<void> owner;
        std::array<SourcePageProviderV1, 5> pages{}; // slots: 0 stats (unused), 1 equipment, 2 skills, 3 faery, 4 map (P16)
        // Authored NativePushMenu pages are a separate source route from the
        // four CharacterMenu tabs. Keys are exact menu symbols, e.g.
        // menu_QuestLogSheetNEW; no new Tab value is implied.
        std::map<std::string, SourcePageProviderV1> source_menu_pages;
        std::function<int(float, float)> stat_button;
        std::function<bool(std::uint32_t, std::string&)> stat_training;
        bool content_installed{};
    };
    std::shared_ptr<State> state_;

    static std::size_t index(Tab tab) noexcept {
        switch (tab) {
        case Tab::stats: return 0;
        case Tab::equipment: return 1;
        case Tab::skills: return 2;
        case Tab::faery: return 3;
        case Tab::map: return 4;
        }
        return 5;
    }
public:
    explicit SourceCompositionV1(std::shared_ptr<void> same_source_owner)
        : state_(std::make_shared<State>()) {
        state_->owner = std::move(same_source_owner);
    }

    // Registration is rejected unless all callbacks retain/borrow the exact
    // owner token published by the root source-character graph.
    bool register_page(Tab tab, SourcePageProviderV1 provider, std::string& error) {
        const auto slot = index(tab);
        if (slot == 0 || slot >= state_->pages.size()) {
            error = "Source composition only registers Equipment, Skills, and Faery providers";
            return false;
        }
        if (!state_->owner || !provider.owner || provider.owner.get() != state_->owner.get()) {
            error = "Source page provider is not retained by the same selected-character owner";
            return false;
        }
        if (!provider.ready || !provider.append || !provider.release) {
            error = "Source page provider requires readiness, content, and release callbacks";
            return false;
        }
        state_->pages[slot] = std::move(provider);
        error.clear();
        return true;
    }

    // Register a page pushed by the original menu stack. It shares the
    // canonical CharacterState owner contract with tab providers but is
    // selected by the active source menu symbol, not by CharacterMenu::Tab.
    bool register_source_menu_page(const std::string& source_menu_symbol,
                                   SourcePageProviderV1 provider,
                                   std::string& error) {
        if (source_menu_symbol.empty()) {
            error = "Source menu page requires its exact authored menu symbol";
            return false;
        }
        if (!state_->owner || !provider.owner || provider.owner.get() != state_->owner.get()) {
            error = "Source menu page provider is not retained by the same selected-character owner";
            return false;
        }
        if (!provider.ready || !provider.append || !provider.release) {
            error = "Source menu page provider requires readiness, content, and release callbacks";
            return false;
        }
        if (state_->source_menu_pages.find(source_menu_symbol) != state_->source_menu_pages.end()) {
            error = "Source menu page symbol is already registered";
            return false;
        }
        state_->source_menu_pages.emplace(source_menu_symbol, std::move(provider));
        error.clear();
        return true;
    }

    // Root's existing NativePushMenu stack supplies the exact current menu
    // symbol. Readiness is rechecked at each operation so a missing source
    // owner cannot leave a stale page selected or dispatch an action.
    bool source_menu_page_ready(const std::string& source_menu_symbol,
                                std::string& error) const {
        const auto it = state_->source_menu_pages.find(source_menu_symbol);
        if (it == state_->source_menu_pages.end()) {
            error = "No source menu page provider is registered for this exact menu symbol";
            return false;
        }
        const auto& page = it->second;
        if (!state_->owner || !page.owner || page.owner.get() != state_->owner.get() ||
            !page.ready || !page.append || !page.release) {
            error = "Source menu page lost its same-owner provider callbacks";
            return false;
        }
        try {
            if (!page.ready(error)) {
                if (error.empty()) error = "Source menu page is not ready";
                return false;
            }
        } catch (...) {
            error = "Source menu page readiness callback threw";
            return false;
        }
        error.clear();
        return true;
    }

    bool append_source_menu_page(const std::string& source_menu_symbol,
                                 Frame& frame, std::string& error) const {
        if (!source_menu_page_ready(source_menu_symbol, error)) return false;
        const auto& page = state_->source_menu_pages.at(source_menu_symbol);
        Frame staged = frame;
        try {
            if (!page.append(staged, error)) {
                if (error.empty()) error = "Source menu page content callback failed";
                return false;
            }
        } catch (...) {
            error = "Source menu page content callback threw";
            return false;
        }
        frame = std::move(staged);
        error.clear();
        return true;
    }

    bool release_source_menu_page(const std::string& source_menu_symbol,
                                  float authored_x, float authored_y,
                                  std::string& error) const {
        if (!source_menu_page_ready(source_menu_symbol, error)) return false;
        const auto& page = state_->source_menu_pages.at(source_menu_symbol);
        try {
            if (!page.release(authored_x, authored_y, error)) {
                if (error.empty()) error = "Source menu page release callback rejected the action";
                return false;
            }
        } catch (...) {
            error = "Source menu page release callback threw";
            return false;
        }
        error.clear();
        return true;
    }

    // Root supplies the source btn_train_* hit resolver and source AS
    // pre/post continuation adapters. In particular, prefix must preserve
    // AddedStatsThisTurn/useSkillPoint and NativeSaveGame order; suffix owns
    // the original post-call UI/sound refresh. The native call itself is
    // routed through CharacterMenuQueriesOwnerV1 to its existing action owner.
    bool register_stat_training(
        std::shared_ptr<void> owner,
        dh2::ui::CharacterMenuActionsOwnerV1& actions,
        std::function<int(float authored_x, float authored_y)> source_button_hit,
        std::function<bool(std::uint32_t, std::string&)> source_prefix,
        std::function<bool(std::uint32_t, std::string&)> source_suffix,
        std::string& error) {
        if (!state_->owner || !owner || owner.get() != state_->owner.get() ||
            !actions.bindings().owner || actions.bindings().owner.get() != state_->owner.get() ||
            !source_button_hit || !source_prefix || !source_suffix) {
            error = "Source stat buttons require the same owner, original hit resolver, and AS prefix/suffix";
            return false;
        }
        state_->stat_button = std::move(source_button_hit);
        // Keep the legacy native path behind one captured callback. Generic
        // Equipment/Skills compositions can include and release this header
        // without instantiating references to the NativeStats owner symbols.
        // Preserve the authored order: live guard, AS prefix, native action,
        // then AS suffix. The action owner and callbacks have the same
        // lifetime contract as before (the root's selected-character lease).
        auto* actions_ptr = &actions;
        state_->stat_training = [actions_ptr,
                                 prefix = std::move(source_prefix),
                                 suffix = std::move(source_suffix)](
                                    std::uint32_t stat, std::string& message) mutable {
            if (!source_stat_training_admissible_v1(*actions_ptr, message)) return false;
            if (!prefix(stat, message)) {
                if (message.empty()) message = "Original stat-training admission/prefix rejected the click";
                return false;
            }
            if (!dispatch_source_stat_training_v1(*actions_ptr, stat, message)) return false;
            if (!suffix(stat, message) && message.empty())
                message = "Original stat-training post-call continuation failed";
            // The native call has already happened if the suffix fails; keep
            // that distinct from an admission or native-action failure.
            return true;
        };
        error.clear();
        return true;
    }

    // Preview 14 live route: the Stats-tab hit resolver and a same-owner train
    // callback that already performs its own admission, save and publication
    // (features/character_menu/stat_training_v1). Same owner token as the
    // canonical CharacterState; no action graph is required.
    bool register_stat_training_callbacks(
        std::shared_ptr<void> owner,
        std::function<int(float authored_x, float authored_y)> source_button_hit,
        std::function<bool(std::uint32_t, std::string&)> train,
        std::string& error) {
        if (!state_->owner || !owner || owner.get() != state_->owner.get() ||
            !source_button_hit || !train) {
            error = "Stat training callbacks require the same owner, hit resolver, and train callback";
            return false;
        }
        state_->stat_button = std::move(source_button_hit);
        state_->stat_training = std::move(train);
        error.clear();
        return true;
    }

    // This is the only tab-selection route root should use. The destination's
    // actual provider is probed before Presenter::select mutates current tab,
    // so missing source services leave the last valid page selected.
    bool select(Presenter& presenter, Tab tab, std::string& error) const {
        const auto slot = index(tab);
        if (slot >= state_->pages.size()) {
            error = "Unsupported source character-menu tab";
            return false;
        }
        if (tab != Tab::stats) {
            const auto& page = state_->pages[slot];
            if (!state_->owner || !page.owner || page.owner.get() != state_->owner.get() ||
                !page.ready || !page.append || !page.release) {
                error = "Requested source character-menu page is not fully bound";
                return false;
            }
            if (!page.ready(error)) {
                if (error.empty()) error = "Requested source character-menu page is not ready";
                return false;
            }
        }
        return presenter.select(tab, error);
    }

    // Chain with any already registered root content callback. Each feature
    // owns its page projection; only the current source page is appended.
    bool install_content(Bindings& bindings, std::string& error) const {
        if (!state_->owner) {
            error = "Source composition requires the canonical character owner";
            return false;
        }
        if (state_->content_installed) {
            error = "Source composition content callback is already installed";
            return false;
        }
        auto previous = std::move(bindings.content);
        auto shared = state_;
        bindings.content = [shared, previous = std::move(previous)](
            Tab tab, Frame& frame, std::string& message) mutable {
            if (previous && !previous(tab, frame, message)) return false;
            const auto slot = SourceCompositionV1::index(tab);
            if (tab == Tab::stats) { message.clear(); return true; }
            if (slot >= shared->pages.size()) {
                message = "Unsupported source character-menu tab";
                return false;
            }
            const auto& page = shared->pages[slot];
            if (!page.owner || page.owner.get() != shared->owner.get() || !page.append) {
                message = "Selected source character-menu page has no same-owner content provider";
                return false;
            }
            return page.append(frame, message);
        };
        state_->content_installed = true;
        error.clear();
        return true;
    }

    // Use Presenter::hit_test here, not Presenter::release: the latter changes
    // tabs directly and would bypass the preselection readiness gate.
    Action release(Presenter& presenter, float screen_x, float screen_y,
                   int width, int height, std::string& error) const {
        if (width <= 0 || height <= 0) {
            error = "Source menu release requires positive viewport dimensions";
            return Action::none;
        }
        const auto action = presenter.hit_test(screen_x, screen_y, width, height);
        switch (action) {
        case Action::close:
            presenter.close(); error.clear(); return action;
        case Action::stats:
            if (!select(presenter, Tab::stats, error)) return Action::none;
            return action;
        case Action::equipment:
            if (!select(presenter, Tab::equipment, error)) return Action::none;
            return action;
        case Action::skills:
            if (!select(presenter, Tab::skills, error)) return Action::none;
            return action;
        case Action::faery:
            if (!select(presenter, Tab::faery, error)) return Action::none;
            return action;
        case Action::map:
            if (!select(presenter, Tab::map, error)) return Action::none;
            return action;
        case Action::map_legend:
        case Action::map_reset_zoom:
            // Map controls toggle presenter state only on the Map page (P16).
            presenter.map_control(action);
            error.clear();
            return action;
        case Action::none: break;
        }
        const float authored_x = screen_x / (width / 480.f);
        const float authored_y = screen_y / (height / 320.f);
        if (presenter.tab() == Tab::stats) {
            if (!state_->stat_button) {
                error = "Stats training buttons have no original source hit resolver";
                return Action::none;
            }
            const int stat = state_->stat_button(authored_x, authored_y);
            if (stat < 0) { error.clear(); return Action::none; }
            if (stat > 3) {
                error = "Source training hit resolver returned a stat outside authored indices 0..3";
                return Action::none;
            }
            if (!state_->stat_training) {
                error = "Hit source stat button has no complete same-owner action/prefix/suffix route";
                return Action::none;
            }
            const auto index = static_cast<std::uint32_t>(stat);
            state_->stat_training(index, error);
            return Action::none;
        }
        const auto slot = index(presenter.tab());
        if (slot == 0 || slot >= state_->pages.size()) {
            error = "No source action provider is bound for the selected menu page";
            return Action::none;
        }
        const auto& page = state_->pages[slot];
        if (!page.owner || page.owner.get() != state_->owner.get() || !page.release) {
            error = "Selected source page has no same-owner action provider";
            return Action::none;
        }
        if (!page.release(authored_x, authored_y, error)) return Action::none;
        return Action::none;
    }
};

// Original SWF's four btn_train_* onRelease handlers dispatch native stat
// indices 0..3 as (stat, player=0). Root invokes this only after the genuine
// source button hit and the original menu admission/prefix. The guard here is
// the live resolved CharacterProperties Stat_Points cell 148, and the native
// action reaches the existing same-player CharacterMenuActionsOwnerV1.
inline bool dispatch_source_stat_training_v1(
    dh2::ui::CharacterMenuActionsOwnerV1& actions,
    std::uint32_t stat, std::string& error) {
    if (stat > 3) {
        error = "Source training button stat index must be Strength/Dexterity/Endurance/Energy (0..3)";
        return false;
    }
    if (!source_stat_training_admissible_v1(actions, error)) return false;
    return actions.assign_stat(stat, error);
}

inline bool source_stat_training_admissible_v1(
    dh2::ui::CharacterMenuActionsOwnerV1& actions, std::string& error) {
    if (!actions.validate_graph(false, error)) return false;
    const auto& graph = actions.bindings().stats;
    auto* equipment = actions.bindings().equipment;
    if (!equipment || !graph.state || !graph.view ||
        graph.state != equipment->properties().get() ||
        graph.view != equipment->property_view() ||
        graph.view->base != graph.state->base.data() ||
        graph.view->saved != graph.state->saved.data() ||
        graph.view->gear != graph.state->gear.data() ||
        graph.view->resolved != graph.state->resolved.data() ||
        ::dh2_property_validate(graph.view)) {
        error = "Source stat training requires the genuine complete same-owner property groups";
        return false;
    }
    if (source_stat_integer(graph.state->resolved[148]) <= 0) {
        error = "Source stat training button is disabled by live Stat_Points property 148";
        return false;
    }
    error.clear();
    return true;
}

} // namespace dh::foundation::character_menu
