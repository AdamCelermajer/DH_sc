#pragma once
#include "source_module_trace.hpp"
#include "../level-loader/canonical_cached_file_v1.hpp"
#include <memory>
namespace dh2::world { struct ModuleRuntimeGlobalsV1; }

namespace dh::foundation {
struct SourceLevelConstructionProviders {
    // SAME already constructed Level/candidate/global owners. This composition
    // does not fabricate LevelC1, reset a copied counter, or publish a World.
    std::shared_ptr<void> levelOwner, candidateOwner, globalsOwner, classesOwner;
    std::int32_t* levelModuleId18c = nullptr;
    float* levelModuleOffset160 = nullptr;
    dh2::world::ModuleRuntimeGlobalsV1* moduleGlobals = nullptr;
    std::shared_ptr<dh2::world::CanonicalObjectManagerV1> manager;
    dh2::world::CanonicalClassServicesV1 classes;
    dh2::loader::CanonicalFileSourceServicesV1 file;
    // Actual typed record from SAME constructor dispatcher, after factory
    // completion. Caller associates exact declaration with host child scope.
    std::function<bool(std::uintptr_t,std::shared_ptr<dh2::world::CanonicalModuleRecordV2>&,
                       std::string&)> moduleRecord;
    std::function<bool(const dh2::loader::XmlDocumentV1::Borrow&,std::uint32_t,
                       const std::shared_ptr<dh2::world::CanonicalModuleRecordV2>&,
                       std::string&,std::string&)> hostOccurrence;
};
enum class SourceLevelConstructionStep { idle, pending, complete, failed };
struct SourceLevelConstructionStatus {
    SourceLevelConstructionStep step = SourceLevelConstructionStep::idle;
    std::string uri, gametype, error;
    std::uint32_t element = UINT32_MAX;
    dh2::world::CanonicalFactoryStageV1 factoryPrefix = dh2::world::CanonicalFactoryStageV1::empty;
    std::size_t attempts = 0, modules = 0;
};
class SourceLevelConstruction {
public:
    SourceLevelConstruction();
    ~SourceLevelConstruction();
    SourceLevelConstruction(SourceLevelConstruction&&) noexcept;
    SourceLevelConstruction& operator=(SourceLevelConstruction&&) noexcept;
    bool begin(SourceLevelConstructionProviders,std::string sourceUri,
               std::vector<std::uint8_t> originalXml,std::uint64_t fileOccurrence,
               std::string& error);
    SourceLevelConstructionStep tick();
    const SourceLevelConstructionStatus& status() const noexcept;
    const SourceModuleConstructionTrace& modules() const noexcept;
    // Only complete construction traversal may publish scopes. This still does
    // not claim Module InitPost/geometry/trigger/Level.Init completion.
    bool bind(SourceWorldObjects&,std::string& error) const;
private:
    struct Impl;
    std::unique_ptr<Impl> impl_;
};
} // namespace dh::foundation
