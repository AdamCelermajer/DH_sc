#include "source_backend_script_host_v1.hpp"

#include <limits>
#include <stdexcept>
#include <cstring>

namespace dh::foundation::actor_frame {
namespace {
template<class A, class B>
bool same_owner(const std::shared_ptr<A>& a, const std::shared_ptr<B>& b) {
    return a && b && a.get() == b.get() &&
        !a.owner_before(b) && !b.owner_before(a);
}
struct SelectedPlayerPinV1 {
    std::shared_ptr<void> selector;
    std::shared_ptr<dh2::player::PlayerInfoNetworkIdentityV4> network_fields;
    std::shared_ptr<dh2::player::ApplicationPlayerManagerBootstrapV59> manager;
};
}

bool SourceBackendScriptHostV1::create(
    std::shared_ptr<dh2::application::ApplicationServicesOwnerV5> application,
    std::shared_ptr<SourceCurrentLevelBackendV1> current_level,
    SourceBackendScriptHostServicesV1 services,
    std::shared_ptr<SourceBackendScriptHostV1>& out, std::string& error) {
    if (!application || !current_level || !services.provider_lease ||
        !services.debug_files_lease ||
        !same_owner(application, current_level->graph().application)) {
        error = "Required same Application and current-Level backend script-host graph";
        return false;
    }
    const auto& debug = application->source_debug_services_v55();
    if (!debug) {
        error = "Required actual Application DebugSwitches for source character level services";
        return false;
    }
    try {
        auto host = std::shared_ptr<SourceBackendScriptHostV1>(new SourceBackendScriptHostV1);
        host->application_ = std::move(application);
        host->current_level_ = std::move(current_level);
        host->provider_lease_ = std::move(services.provider_lease);
        host->debug_files_lease_ = std::move(services.debug_files_lease);
        host->debug_ = debug;
        host->debug_files_ = services.debug_files;
        host->debug_binding_ = {host->debug_.get(), &host->debug_files_};
        host->level_services_ = {&host->debug_binding_, dh2_character_debug_level_service};
        host->host_bindings_ = {{host.get(), &SourceBackendScriptHostV1::invoke_host}};
        host->online_hosting_player_ = services.online_hosting_player;
        host->online_hosting_player_context_ = services.online_hosting_player_context;
        out = std::move(host);
        error.clear();
        return true;
    } catch (const std::exception& ex) {
        error = ex.what();
        return false;
    } catch (...) {
        error = "Unable to retain source backend script-host context";
        return false;
    }
}

int SourceBackendScriptHostV1::invoke_level(
    void* raw, dh2::character::LevelModel32* model,
    const dh2::character::LevelRequest24* request) {
    auto* self = static_cast<SourceBackendScriptHostV1*>(raw);
    if (!self || !model || !request || !self->debug_ ||
        self->application_->source_debug_services_v55().get() != self->debug_.get())
        return 1;
    return dh2_character_debug_level_service(&self->debug_binding_, model, request);
}

bool SourceBackendScriptHostV1::refresh_ranges(std::string& error) {
    const auto* tables = current_level_->graph().level_tables;
    if (!tables) {
        error = "Required same loaded source LevelTables for Host range rows";
        return false;
    }
    const auto& rows = tables->levels;
    if (rows.size() > std::numeric_limits<std::uint32_t>::max()) {
        error = "Source LevelTables row count exceeds HostContext response";
        return false;
    }
    try {
        range_projection_.resize(rows.size());
    } catch (...) {
        error = "Unable to project source LevelTables range rows";
        return false;
    }
    // Source rereads the table on each GetLevelRange call, including after
    // earlier pushes. Keep its order and read each row's original +0x30 data.
    for (std::size_t i = 0; i < rows.size(); ++i) {
        std::memcpy(&range_projection_[i], rows[i].scalar.words + 12,
            sizeof(dh2::character::LevelRangeRow24));
    }
    error.clear();
    return true;
}

int SourceBackendScriptHostV1::invoke_host(
    void* raw, const dh2::character::HostContextRequest16* request,
    dh2::character::HostContextResponse16* response) {
    auto* self = static_cast<SourceBackendScriptHostV1*>(raw);
    if (!self || !request || !response || !self->application_ || !self->current_level_) return 1;
    self->error_.clear();
    response->data = nullptr;
    response->count = 0;
    response->reserved = 0;
    try {
    switch (request->service) {
    case dh2::character::host_get_player: {
        const auto manager = self->application_->source_player_manager_v59();
        const auto online = self->application_->get_online_loading_v55();
        if (!manager || !manager->belongs_to_application(self->application_) || !online) {
            self->error_ = "Required same Application PlayerManager/GetOnline for GetHostingPlayer";
            return 1;
        }
        dh2::player::PlayerInfoFieldsV1* info{};
        std::shared_ptr<void> selection_pin;
        if (online->byte5()) {
            if (!self->online_hosting_player_) {
                self->error_ = "Required actual online GetHostingPlayer selector";
                return 1;
            }
            if (!self->online_hosting_player_(self->online_hosting_player_context_,
                    *manager, info, selection_pin, self->error_) || !info || !selection_pin) {
                if (self->error_.empty()) self->error_ = "Actual online GetHostingPlayer returned no pinned PlayerInfo";
                return 1;
            }
        } else {
            if (!manager->get_by_internal(0, false, info, self->error_) || !info) {
                if (self->error_.empty()) self->error_ = "Source offline GetHostingPlayer internal0,false failed";
                return 1;
            }
        }
        // Resolve the selected PlayerInfo against this very network owner and
        // read only the field produced by its managed source operation.
        auto* network = manager->network();
        if (!network) {
            self->error_ = "Required actual selected PlayerInfo network owner";
            return 1;
        }
        auto fields = network->borrow(*info);
        if (!fields || !fields->managed_fields_produced_v70) {
            self->error_ = "Required selected PlayerInfo managed_fields_produced_v70 cached-level producer";
            return 1;
        }
        self->player_request_pin_ = std::make_shared<SelectedPlayerPinV1>(
            SelectedPlayerPinV1{std::move(selection_pin), fields, manager});
        self->host_player_.cached_level = fields->character_level330_v70;
        self->host_player_.reserved = 0;
        response->data = &self->host_player_;
        return 0;
    }
    case dh2::character::host_get_current_level: {
        if (self->level_request_pin_) {
            auto previous = std::static_pointer_cast<SourceCurrentLevelBorrowV1>(self->level_request_pin_);
            if (*previous && !self->current_level_->still_current(*previous, self->error_)) return 1;
        }
        auto current = std::make_shared<SourceCurrentLevelBorrowV1>();
        if (!self->current_level_->current(*current, self->error_)) return 1;
        if (!*current) {
            self->level_request_pin_.reset();
            response->data = nullptr;
            return 0;
        }
        if (!self->current_level_->still_current(*current, self->error_)) return 1;
        const auto& fields = current->level()->constructor_fields_v3();
        self->host_level_ = {fields.row3c, fields.mode118};
        self->level_request_pin_ = std::move(current);
        response->data = &self->host_level_;
        return 0;
    }
    case dh2::character::host_get_range_rows:
        if (!self->refresh_ranges(self->error_)) return 1;
        response->data = self->range_projection_.empty() ? nullptr : self->range_projection_.data();
        response->count = static_cast<std::uint32_t>(self->range_projection_.size());
        return 0;
    default:
        self->error_ = "Host script push is owned by the actual CharacterScriptSession binder";
        return 1;
    }
    } catch (...) {
        self->error_ = "Source backend script-host provider failed while retaining a reached borrow";
        return 1;
    }
}

} // namespace dh::foundation::actor_frame
