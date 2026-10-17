#include "character_target_list_backup_v1.hpp"

#include <algorithm>
#include <limits>
#include <new>

namespace dh2::character_target_list_backup_v1 {
namespace {
bool valid_services(const ServicesV1& services) noexcept {
    return services.context && services.resolve_object &&
           services.lock_object && services.unlock_object;
}
} // namespace

Owner::~Owner() noexcept {
    std::string ignored;
    (void)clear_all(ignored);
}

bool Owner::bind_services(const ServicesV1& services,
                          std::string& error) noexcept {
    if (!valid_services(services)) {
        error = "TargetList backup requires canonical object resolution and +0x29 lock services";
        return false;
    }
    if (!services_bound_) {
        services_ = services;
        services_bound_ = true;
        return true;
    }
    if (services_.context != services.context ||
        services_.resolve_object != services.resolve_object ||
        services_.lock_object != services.lock_object ||
        services_.unlock_object != services.unlock_object) {
        error = "TargetList backup cannot change its canonical ObjectManager lifetime provider";
        return false;
    }
    return true;
}

Status Owner::backup(std::string_view name,
                     const target_search::List48& results,
                     const ServicesV1& services,
                     std::string& error) noexcept {
    error.clear();
    if (results.count > results.capacity || (results.count && !results.heap)) {
        error = "TargetList backup source result heap is invalid";
        return Status::invalid_argument;
    }
    if (!bind_services(services, error)) return Status::provider_failed;

    try {
        auto inserted = named_.try_emplace(std::string(name));
        auto& destination = inserted.first->second;
        std::vector<Identity> next;
        next.reserve(results.count);
        for (std::uint32_t i = 0; i < results.count; ++i) {
            target_search::Object48* object = nullptr;
            if (!services.resolve_object(services.context,
                                         results.heap[i].identity,
                                         &object, error) || !object ||
                !object->identity) {
                if (error.empty()) error = "TargetList result has no canonical ObjectManager GameObject row";
                return Status::provider_failed;
            }
            next.push_back(object->identity);
        }

        // BackupObjectList::_Clear releases old +0x29 locks before _Add
        // retains each new GameObject in source heap order.
        release(destination);
        std::size_t retained = 0;
        for (const auto identity : next) {
            if (!services.lock_object(services.context, identity, error)) {
                for (std::size_t i = 0; i < retained; ++i)
                    services.unlock_object(services.context, next[i]);
                destination.objects.clear();
                destination.cursor = 0;
                if (error.empty()) error = "TargetList backup could not lock a canonical GameObject row";
                return Status::provider_failed;
            }
            ++retained;
        }
        destination.objects = std::move(next);
        destination.cursor = 0;
        return Status::complete;
    } catch (const std::bad_alloc&) {
        error = "TargetList backup allocation failed";
        return Status::allocation_failed;
    } catch (...) {
        error = "TargetList backup provider threw an exception";
        return Status::provider_failed;
    }
}

Status Owner::open(std::string_view name, const ServicesV1& services,
                   ViewV1& view,
                   target_search::ObjectListMethods& methods,
                   std::string& error) noexcept {
    error.clear();
    if (!bind_services(services, error)) return Status::provider_failed;
    try {
        (void)named_.try_emplace(std::string(name));
        view = {};
        view.owner = this;
        view.name.assign(name.data(), name.size());
        view.services = &services_;
        methods = {&view, reset_view, at_end_view, get_view, next_view};
        return Status::complete;
    } catch (const std::bad_alloc&) {
        error = "TargetList backup lookup allocation failed";
        return Status::allocation_failed;
    } catch (...) {
        error = "TargetList backup lookup failed";
        return Status::provider_failed;
    }
}

Status Owner::clear(std::string_view name, std::string& error) noexcept {
    error.clear();
    const auto found = named_.find(name);
    if (found == named_.end()) return Status::complete;
    release(found->second);
    return Status::complete;
}

Status Owner::clear_all(std::string& error) noexcept {
    error.clear();
    for (auto& entry : named_) release(entry.second);
    return Status::complete;
}

Status Owner::retire(std::string& error) noexcept {
    clear_all(error);
    services_ = {};
    services_bound_ = false;
    return Status::complete;
}

std::size_t Owner::size(std::string_view name) const noexcept {
    const auto found = named_.find(name);
    return found == named_.end() ? 0 : found->second.objects.size();
}

void Owner::release(NamedList& list) noexcept {
    if (services_bound_ && services_.unlock_object) {
        for (const auto identity : list.objects)
            services_.unlock_object(services_.context, identity);
    }
    list.objects.clear();
    list.cursor = 0;
}

int Owner::reset_view(void* raw) noexcept {
    auto* view = static_cast<ViewV1*>(raw);
    if (!view || !view->owner) return 1;
    const auto found = view->owner->named_.find(view->name);
    if (found == view->owner->named_.end()) return 1;
    found->second.cursor = 0;
    view->cursor = 0;
    view->error.clear();
    return 0;
}

int Owner::at_end_view(void* raw, std::uint32_t* at_end) noexcept {
    auto* view = static_cast<ViewV1*>(raw);
    if (!view || !view->owner || !at_end) return 1;
    const auto found = view->owner->named_.find(view->name);
    if (found == view->owner->named_.end()) return 1;
    *at_end = found->second.cursor >= found->second.objects.size();
    return 0;
}

int Owner::get_view(void* raw, target_search::Object48** object) noexcept {
    auto* view = static_cast<ViewV1*>(raw);
    if (!view || !view->owner || !view->services || !object) return 1;
    *object = nullptr;
    const auto found = view->owner->named_.find(view->name);
    if (found == view->owner->named_.end() ||
        found->second.cursor >= found->second.objects.size()) return 1;
    std::string error;
    if (!view->services->resolve_object(view->services->context,
            found->second.objects[found->second.cursor], object, error) ||
        !*object || (*object)->identity !=
            found->second.objects[found->second.cursor]) {
        view->error = error.empty()
            ? "TargetList backup identity no longer resolves to its canonical GameObject row"
            : std::move(error);
        *object = nullptr;
        return 1;
    }
    return 0;
}

int Owner::next_view(void* raw) noexcept {
    auto* view = static_cast<ViewV1*>(raw);
    if (!view || !view->owner) return 1;
    const auto found = view->owner->named_.find(view->name);
    if (found == view->owner->named_.end()) return 1;
    if (found->second.cursor < found->second.objects.size())
        ++found->second.cursor;
    view->cursor = found->second.cursor;
    return 0;
}

} // namespace dh2::character_target_list_backup_v1
