#pragma once
#include <array>
#include <cstdint>
#include <functional>
#include <memory>
#include <string>
#include <vector>

namespace dh2::world { class CanonicalModuleV1; struct CanonicalModuleRecordV2; }
namespace dh::foundation {
class SourceWorldObjects;
struct SourceModuleOccurrence {
    // Explicit host child-occurrence identity, e.g. ActorDefinition.moduleName.
    // Spelling is opaque. No ordinal is parsed or used to calculate runtime ID.
    std::string hostOccurrence;
    std::string sourceUri;
    std::uint32_t element = 0;
    std::uint64_t fileOccurrence = 0; // Distinguishes retained repeated XML loads.
};
struct SourceModuleReceiverBorrow {
    std::shared_ptr<void> receiver;
    std::uintptr_t identity = 0;
    // Read actual Module40c and position160 from the same constructed receiver.
    // Success must not synthesize an ID from source XML or any copied counter.
    std::function<bool(std::int32_t&,std::array<float,3>&,std::string&)> read;
};
SourceModuleReceiverBorrow borrow_source_module_receiver(
    const std::shared_ptr<dh2::world::CanonicalModuleV1>& module);
// Existing retained loader stores a unique Module receiver inside this shared
// record. Borrow it through an aliasing shared_ptr; never make a second owner.
SourceModuleReceiverBorrow borrow_source_module_record(
    const std::shared_ptr<dh2::world::CanonicalModuleRecordV2>& record);
struct SourceModuleTraceEntry {
    SourceModuleOccurrence occurrence;
    SourceModuleReceiverBorrow receiver;
    std::int32_t runtimeId = -1;
    std::array<float,3> position{};
};
class SourceModuleConstructionTrace {
public:
    // Call from the actual source factory's constructor/property/load callback.
    // This trace does not construct modules, choose alternate XML, or run gates.
    bool observe(SourceModuleOccurrence,SourceModuleReceiverBorrow,std::string& error);
    bool observe(SourceModuleOccurrence,const std::shared_ptr<dh2::world::CanonicalModuleV1>&,
                 std::string& error);
    // Connect RetainedLevelModuleGraphV1.modules() with root_file().attempts().
    // declaration must be the SAME actual request.source_lease retained on the
    // record, not a matching string/ordinal. URI/element/file occurrence come
    // from that matched source request; host occurrence association is explicit.
    bool observe_record(SourceModuleOccurrence,
                        const std::shared_ptr<dh2::world::CanonicalModuleRecordV2>&,
                        const std::shared_ptr<const void>& declaration,std::string& error);
    // Refresh immutable ID and mutable position from retained real receivers.
    bool refresh(std::string& error);
    // Validate against a copy before committing so a missing/conflicting host
    // occurrence never leaves a partially bound SourceWorldObjects instance.
    bool bind(SourceWorldObjects&,std::string& error) const;
    const std::vector<SourceModuleTraceEntry>& entries() const noexcept { return entries_; }
private:
    std::vector<SourceModuleTraceEntry> entries_;
};
} // namespace dh::foundation
