#include "source_backend_npc_script_bootstrap_v1.hpp"

#include "source_campaign_character_fsm_v101.hpp"

namespace dh::foundation::actor_frame {
namespace {
using Record = dh2::world::CanonicalCharacterCandidateRecordV60;

template<class A, class B>
bool same_owner(const std::shared_ptr<A>& a, const std::shared_ptr<B>& b) {
    return a && b && a.get() == b.get() &&
        !a.owner_before(b) && !b.owner_before(a);
}
}

bool SourceBackendNpcScriptBootstrapV1::create(
    std::shared_ptr<void> world_lease,
    std::shared_ptr<void> objects_lease,
    dh2::character::CharacterScriptObjects& objects,
    std::shared_ptr<dh2::windows_foundation::SourceCharacterOwnerFactoryNpcScriptBindingV1> assets,
    std::shared_ptr<SourceBackendScriptHostV1> host,
    std::shared_ptr<SourceBackendNpcScriptBootstrapV1>& out,
    std::string& error) {
    out.reset();
    if (!world_lease || !objects_lease || !assets || !assets->borrow() || !host ||
        !host->context_pin() || !host->bindings() || !host->bindings()->services.invoke ||
        !host->level_services() || !host->level_services()->invoke ||
        !same_owner(world_lease, host->world_lease()) ||
        objects.services().context != &objects || !objects.services().invoke) {
        error = "Required same-world loaded NPC assets, CharacterScriptObjects, and backend host/current-Level owner";
        return false;
    }
    try {
        auto bootstrap = std::shared_ptr<SourceBackendNpcScriptBootstrapV1>(
            new SourceBackendNpcScriptBootstrapV1);
        bootstrap->world_lease_ = std::move(world_lease);
        bootstrap->objects_lease_ = std::move(objects_lease);
        bootstrap->objects_ = &objects;
        bootstrap->object_services_ = &objects.services();
        bootstrap->assets_ = std::move(assets);
        bootstrap->host_ = std::move(host);
        out = std::move(bootstrap);
        error.clear();
        return true;
    } catch (...) {
        error = "Unable to retain connected source NPC script bootstrap";
        return false;
    }
}

bool SourceBackendNpcScriptBootstrapV1::bind_events(
    Record& record, dh2::character::CharacterScriptSessionInput& input,
    std::string& error) {
    return model_renderer::bind_campaign_character_npc_events_v101(record, input, error);
}

bool SourceBackendNpcScriptBootstrapV1::bind_record(
    Record& record, dh2::character::CharacterScriptSessionInput& input,
    dh::foundation::features::SourceCharacterNpcContextBorrowV1& context,
    std::string& error) {
    const auto world_lease = world_lease_.lock();
    const auto objects_lease = objects_lease_.lock();
    const auto host = host_.lock();
    if (!world_lease || !objects_lease || !host) {
        error = "Required live same-world NPC assets/host/object service owners";
        return false;
    }
    if (record.failed || !record.actor || !record.actor->object || !record.actor->machine ||
        !record.fsm_context_v101 || !same_owner(record.services.world, world_lease)) {
        error = "Required live canonical NPC record and same retained source world";
        return false;
    }
    auto* script_object = objects_->find(record.actor->object->identity).get();
    if (script_object != record.actor->object.get() ||
        object_services_ != &objects_->services() ||
        object_services_->context != objects_) {
        error = "Required same CharacterScriptObjects owner and canonical NPC object";
        return false;
    }

    // The factory owns the record while its npc_script callback is running.
    // This no-op alias is deliberately stack-scoped: neither the bootstrap nor
    // its context lease retains the record, and the returned loan is discarded
    // before the callback returns to record.load_script/construct_script.
    auto same_record = std::shared_ptr<Record>(&record, [](Record*) {});
    model_renderer::SourceCharacterPathBorrowV105 actual_fsm;
    if (!model_renderer::borrow_source_campaign_character_path_v105(
            same_record, actual_fsm, error) ||
        actual_fsm.character != record.actor->object->identity ||
        actual_fsm.machine != &record.actor->machine->native_fsm() ||
        actual_fsm.context_lease.get() != record.fsm_context_v101.get() ||
        actual_fsm.record_lease.get() != &record) {
        if (error.empty()) error = "Required actual same-record CampaignFsmV101 path loan";
        return false;
    }

    if (!assets_->bind_resources(record, input, object_services_, error)) return false;
    dh::foundation::features::SourceCharacterNpcContextProvidersV1 providers;
    providers.world_lease = world_lease;
    providers.context_lease = host->context_pin();
    providers.host = host->bindings();
    providers.level = host->level_services();
    providers.objects = object_services_;
    providers.bind_events = &SourceBackendNpcScriptBootstrapV1::bind_events;
    return bind_source_character_owner_factory_npc_context_v1(
        same_record, input, providers, context, error);
}

bool SourceBackendNpcScriptBootstrapV1::npc_script(
    void* raw, Record& record, dh2::character::CharacterScriptSessionInput& input,
    std::string& error) {
    auto* self = static_cast<SourceBackendNpcScriptBootstrapV1*>(raw);
    if (!self) {
        error = "Required retained source NPC script bootstrap";
        return false;
    }
    dh::foundation::features::SourceCharacterNpcContextBorrowV1 context;
    return self->bind_record(record, input, context, error);
}

bool SourceBackendNpcScriptBootstrapV1::install(
    dh2::world::CanonicalCharacterCandidateServicesV60& services,
    std::string& error) {
    if (weak_from_this().expired() || world_lease_.expired() || objects_lease_.expired() ||
        !assets_ || host_.expired()) {
        error = "Required retained source NPC script bootstrap before factory installation";
        return false;
    }
    auto self = shared_from_this();
    services.npc_script = [self = std::move(self)](
        Record& record, dh2::character::CharacterScriptSessionInput& input,
        std::string& callback_error) {
        return npc_script(self.get(), record, input, callback_error);
    };
    error.clear();
    return true;
}

} // namespace dh::foundation::actor_frame
