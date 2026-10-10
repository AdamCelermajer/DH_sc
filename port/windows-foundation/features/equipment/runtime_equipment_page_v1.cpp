#include "runtime_equipment_page_v1.hpp"
#include <set>
#include <optional>
#include <utility>

namespace dh::foundation::equipment_menu {

struct RuntimeEquipmentPageV1::Impl {
    RuntimeEquipmentBindingV1* runtime{};
    CharacterState* character{};
    const dh2::data::ItemTable* table{};
    Presenter* equipment{};
    RuntimeEquipmentPageBindingsV1 bindings;
    std::unique_ptr<inventory::MenuPresenter> inventory;
    std::unique_ptr<inventory::DetailsPresenter> details;
    std::unique_ptr<MainPage> page;
    std::optional<RuntimeEquipmentRenderChangeV1> source_render_change;
    std::optional<RuntimeEquipmentPageReleaseV1::PendingCommand> source_pending_command;

    bool current(std::string& error) const {
        if (!runtime || !character || !table || !equipment || !inventory || !details || !page) {
            error = "Runtime equipment MAINPAGE is not initialized";
            return false;
        }
        if (!runtime->uses_character_state(*character, error)) return false;
        std::set<std::string> ids;
        for (const auto& owned : character->inventory) {
            if (owned.instance_id.empty() || !owned.quantity ||
                !ids.insert(owned.instance_id).second) {
                error = "Shared source inventory projection has an invalid or duplicate instance ID";
                return false;
            }
            const auto* item = runtime->item_for_instance(owned.instance_id, error);
            if (!item || dh2::data::item(*table,
                    dh2::data::item_id(*table, owned.definition_id)) != item) {
                if (error.empty()) error = "Shared source inventory projection differs from its ItemTable instance";
                return false;
            }
            if (bindings.source_instance_current &&
                !bindings.source_instance_current(owned, *item, error)) return false;
        }
        error.clear();
        return true;
    }
};

RuntimeEquipmentPageV1::RuntimeEquipmentPageV1() = default;
RuntimeEquipmentPageV1::~RuntimeEquipmentPageV1() = default;

bool RuntimeEquipmentPageV1::bind(RuntimeEquipmentBindingV1& runtime,
        CharacterState& character, RuntimeEquipmentPageBindingsV1 bindings,
        std::string& error) {
    if (impl_) { error = "Runtime equipment MAINPAGE can only be bound once"; return false; }
    if (!runtime.uses_character_state(character, error)) return false;
    const auto* table = runtime.item_table(error);
    if (!table) return false;
    auto* equipment = runtime.presenter_for_composition(error);
    if (!equipment) return false;
    auto actions = runtime.menu_actions(error);
    if (!actions.bound() || actions.partial()) {
        if (error.empty()) error = "Runtime equipment requires the paired runtime equip/unequip actions";
        return false;
    }
    try {
        auto next = std::make_unique<Impl>();
        next->runtime = &runtime;
        next->character = &character;
        next->table = table;
        next->equipment = equipment;
        next->bindings = std::move(bindings);
        next->inventory = std::make_unique<inventory::MenuPresenter>(character, *table);
        next->details = std::make_unique<inventory::DetailsPresenter>(character, *table, *equipment);
        next->page = std::make_unique<MainPage>(*equipment, *next->inventory, *next->details,
                next->bindings.inventory_text, next->bindings.details_text, std::move(actions));
        if (!next->current(error)) return false;
        impl_ = std::move(next);
        error.clear();
        return true;
    } catch (const std::exception& e) {
        error = e.what();
        return false;
    }
}

bool RuntimeEquipmentPageV1::ready(std::string& error) const {
    return impl_ && impl_->current(error);
}

bool RuntimeEquipmentPageV1::content(character_menu::Tab tab, character_menu::Frame& frame,
        std::string& error) {
    if (!ready(error)) return false;
    return impl_->page->content(tab, frame, error);
}

std::function<bool(character_menu::Tab, character_menu::Frame&, std::string&)>
RuntimeEquipmentPageV1::content_callback(
        std::function<bool(character_menu::Tab, character_menu::Frame&, std::string&)> other_tabs) {
    return [this, other_tabs = std::move(other_tabs)](character_menu::Tab tab,
            character_menu::Frame& frame, std::string& error) {
        if (tab == character_menu::Tab::equipment) return content(tab, frame, error);
        if (other_tabs) return other_tabs(tab, frame, error);
        error.clear();
        return true;
    };
}

bool RuntimeEquipmentPageV1::release(float x, float y, RuntimeEquipmentPageReleaseV1& output,
        std::string& error) {
    output = {};
    if (!ready(error)) return false;
    if (!impl_->page->release(x, y, output.command, error)) return false;
    if (output.command == MainPageCommand::request_auto_equip_all) {
        // NativeInvAutoEquipSlot(-1) acts on the whole sheet; it needs no selected item or slot.
        output.has_pending_command = true;
        output.pending_command = {output.command, std::string(), 10};
    }
    if (output.command == MainPageCommand::request_drop ||
        output.command == MainPageCommand::request_auto_equip ||
        output.command == MainPageCommand::request_transmute) {
        const auto& selected = impl_->equipment->selected_instance();
        const auto slot = impl_->equipment->selected_slot();
        if (selected.empty() || slot > 9) {
            error = "Original Details command requires its currently selected owned source instance and slot";
            output = {};
            return false;
        }
        if (!impl_->runtime->item_for_instance(selected, error)) {
            output = {};
            return false;
        }
        output.has_pending_command = true;
        output.pending_command = {output.command, selected, slot};
    }
    if (output.command == MainPageCommand::equipped ||
        output.command == MainPageCommand::unequipped) {
        if (!impl_->runtime->take_render_change(output.render_change, error)) {
            if (error.empty()) error = "Committed runtime equipment action omitted its render-change receipt";
            return false;
        }
        output.has_render_change = true;
    }
    error.clear();
    return true;
}

bool RuntimeEquipmentPageV1::source_page_provider(
        std::shared_ptr<void> same_source_owner,
        character_menu::SourcePageProviderV1& output, std::string& error) {
    output = {};
    if (!same_source_owner) {
        error = "Runtime equipment page requires the canonical same-source owner token";
        return false;
    }
    std::shared_ptr<RuntimeEquipmentPageV1> self;
    try {
        self = shared_from_this();
    } catch (const std::bad_weak_ptr&) {
        error = "Runtime equipment page must be retained by shared_ptr before source composition binding";
        return false;
    }
    if (!ready(error)) return false;

    character_menu::SourcePageProviderV1 provider;
    provider.owner = std::move(same_source_owner);
    provider.ready = [self](std::string& message) { return self->ready(message); };
    provider.append = [self](character_menu::Frame& frame, std::string& message) {
        return self->content(character_menu::Tab::equipment, frame, message);
    };
    provider.release = [self](float x, float y, std::string& message) {
        return self->release_for_source(x, y, message);
    };
    output = std::move(provider);
    error.clear();
    return true;
}

bool RuntimeEquipmentPageV1::release_for_source(float x, float y,
        std::string& error) {
    if (!impl_) {
        error = "Runtime equipment page is not initialized";
        return false;
    }
    if (impl_->source_render_change) {
        error = "Consume the previous source equipment render receipt before another release";
        return false;
    }
    if (impl_->source_pending_command) {
        error = "Consume the previous typed source inventory command before another release";
        return false;
    }
    RuntimeEquipmentPageReleaseV1 released;
    if (!release(x, y, released, error)) return false;
    if (released.has_render_change) impl_->source_render_change = released.render_change;
    if (released.has_pending_command) impl_->source_pending_command = released.pending_command;
    error.clear();
    return true;
}

bool RuntimeEquipmentPageV1::take_source_render_change(
        RuntimeEquipmentRenderChangeV1& output, std::string& error) {
    output = {};
    if (!impl_) {
        error = "Runtime equipment page is not initialized";
        return false;
    }
    if (!impl_->source_render_change) {
        error.clear();
        return false;
    }
    output = *impl_->source_render_change;
    impl_->source_render_change.reset();
    error.clear();
    return true;
}

bool RuntimeEquipmentPageV1::take_source_pending_command(
        RuntimeEquipmentPageReleaseV1::PendingCommand& output, std::string& error) {
    output = {};
    if (!impl_) {
        error = "Runtime equipment page is not initialized";
        return false;
    }
    if (!impl_->source_pending_command) {
        error.clear();
        return false;
    }
    output = std::move(*impl_->source_pending_command);
    impl_->source_pending_command.reset();
    error.clear();
    return true;
}

std::size_t RuntimeEquipmentPageV1::details_selected_index() const {
    return impl_ && impl_->details ? impl_->details->selected_index() : 0;
}

bool RuntimeEquipmentPageV1::reselect_details_near(std::size_t index, std::string& error) {
    if (!impl_ || !impl_->details) { error = "Runtime equipment page is not initialized"; return false; }
    return impl_->details->reselect_near(index, error);
}

void RuntimeEquipmentPageV1::leave_page() noexcept {
    if (impl_ && impl_->page) impl_->page->leave_page();
}

} // namespace dh::foundation::equipment_menu
