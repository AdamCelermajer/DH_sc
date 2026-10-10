#pragma once
#include "source_level_construction.hpp"
#include "source_level_config.hpp"
#include "source_module_construction.hpp"
#include "actor_definitions.hpp"
#include <map>

namespace dh::foundation {
struct SourceRootScopeOptions {
    // Exact root XML element index -> existing opaque child occurrence.
    // Required when the default declaration association is ambiguous.
    std::map<std::uint32_t,std::string> hostOccurrenceByElement;
    std::uint64_t fileOccurrence=0;
    std::shared_ptr<void> debugOwner, fileOwner, managerServicesOwner;
    std::function<bool(const char*,bool&,std::string&)> debugSwitch;
    dh2::loader::CanonicalFileSourceServicesV1 file;
    dh2::world::CanonicalObjectManagerServicesV1 managerServices;
};

// In-house offline current-Level context. These are real owned host fields,
// not a claim that original LevelC1/Application constructors ran.
struct SourceRootScopeContext {
    std::int32_t moduleId=-1;
    std::array<float,3> moduleOffset{};
    std::uint8_t onlineByte5=0;
};

class SourceRootScopes {
public:
    SourceRootScopes();
    ~SourceRootScopes();
    SourceRootScopes(SourceRootScopes&&) noexcept;
    SourceRootScopes& operator=(SourceRootScopes&&) noexcept;
    // One construction run per owner, including honest retained failed prefixes.
    bool load(const AssetCatalog&,const std::string& originalLevelUri,
              const std::vector<ActorDefinition>&,std::string& error,
              const SourceRootScopeOptions& options={});
    bool build(const AssetCatalog&,const std::string& originalLevelUri,
               const std::vector<ActorDefinition>&,SourceWorldObjects&,
               std::string& error,const SourceRootScopeOptions& options={});
    // SourceWorldObjects::load replaces the table and removes its bindings.
    // Reapply to that same/new table; constructor records and IDs do not replay.
    bool bind(SourceWorldObjects&,std::string& error) const;
    const SourceModuleConstructionTrace& modules() const noexcept;
    const SourceLevelConstructionStatus& status() const noexcept;
    const std::vector<std::shared_ptr<dh2::world::CanonicalModuleRecordV2>>& module_records() const noexcept;
    std::size_t count() const noexcept;
    const SourceRootScopeContext& context() const noexcept;
    // Same retained default Debug/files load, without querying a switch.
    // A custom query-only provider has no standalone load capability.
    bool debug_load(std::string& error)const;
    bool debug_switch(const char* key,bool& value,std::string& error)const;
private:
    struct Impl;
    std::unique_ptr<Impl> impl_;
};
} // namespace dh::foundation
