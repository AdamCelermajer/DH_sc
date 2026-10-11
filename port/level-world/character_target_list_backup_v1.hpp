#pragma once

#include "character_target_search.hpp"

#include <cstdint>
#include <map>
#include <string>
#include <string_view>
#include <vector>

namespace dh2::character_target_list_backup_v1 {

using Identity = std::uintptr_t;

// These callbacks operate on the same canonical ObjectBase/GameObject rows
// used by the active ObjectManager. lock_object projects ObjectBase+0x29;
// unlock_object releases exactly that source lock. resolve_object returns the
// current search projection for a retained source identity.
struct ServicesV1 {
    void* context{};
    bool (*resolve_object)(void*, Identity, target_search::Object48**,
                           std::string&){};
    bool (*lock_object)(void*, Identity, std::string&){};
    void (*unlock_object)(void*, Identity) noexcept{};
};

enum class Status : std::uint8_t {
    complete,
    invalid_argument,
    provider_failed,
    allocation_failed
};

class Owner final {
public:
    Owner() = default;
    ~Owner() noexcept;
    Owner(const Owner&) = delete;
    Owner& operator=(const Owner&) = delete;
    Owner(Owner&&) = delete;
    Owner& operator=(Owner&&) = delete;

    // Replaces one named source BackupObjectList with the current TargetList
    // GameObject results, in heap storage order. Only canonical object
    // identities are retained; Target24 distance/angle records are not copied.
    Status backup(std::string_view name, const target_search::List48& results,
                  const ServicesV1& services, std::string& error) noexcept;

    // GetResultsBackup uses map::operator[]: opening an absent name creates an
    // empty reusable source ObjectList. `view` and `methods` must outlive the
    // synchronous call into dh2_target_search_object_list.
    struct ViewV1 {
        Owner* owner{};
        std::string name{};
        const ServicesV1* services{};
        std::size_t cursor{};
        std::string error{};
    };
    Status open(std::string_view name, const ServicesV1& services,
                ViewV1& view, target_search::ObjectListMethods& methods,
                std::string& error) noexcept;

    Status clear(std::string_view name, std::string& error) noexcept;
    Status clear_all(std::string& error) noexcept;
    // Clear the source-owned list locks before its current ObjectManager is
    // retired, then permit rebinding to the next Level generation.
    Status retire(std::string& error) noexcept;
    std::size_t size(std::string_view name) const noexcept;

private:
    struct NamedList {
        std::vector<Identity> objects{};
        std::size_t cursor{};
    };
    using NamedMap = std::map<std::string, NamedList, std::less<>>;

    static int reset_view(void*) noexcept;
    static int at_end_view(void*, std::uint32_t*) noexcept;
    static int get_view(void*, target_search::Object48**) noexcept;
    static int next_view(void*) noexcept;
    bool bind_services(const ServicesV1&, std::string&) noexcept;
    void release(NamedList&) noexcept;

    ServicesV1 services_{};
    bool services_bound_{};
    NamedMap named_{};
};

} // namespace dh2::character_target_list_backup_v1
