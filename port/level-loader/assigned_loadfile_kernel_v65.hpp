#pragma once
#include "assigned_root_source_v64.hpp"
#include "stage_loader_v38_root_file.hpp"
#include <savegame_stream_v2.hpp>
namespace dh2::loader {
// Semantic host receiver for original LoadFileData, not a fabricated ARM layout.
// It owns the ONE copied stream/backend; never a Level or generator lease.
struct AssignedLoadFileDataV65 {
 std::unique_ptr<level::SavegameStreamV2> stream;
 std::shared_ptr<XmlDocumentV1::NativeBackendV65> backend;
 XmlDocumentV1 xml;
 std::uint8_t buffer_flag34{},flag48{};
 std::uintptr_t document38{},root3c{},node40{},child44{};
 bool read_started{},read_failed{},retired{};
 std::string source_name,error;
};
struct SourceIStreamBorrowV65 {std::shared_ptr<void> actual_owner;std::uintptr_t identity{};};
struct AssignedCopyLeavesV65 {
 std::shared_ptr<void> owner;
 // Source IStreamBase vtable+8 and +18 only. Bool is successful execution of
 // the complete native getter/read; never a generated-ready policy callback.
 std::function<bool(const SourceIStreamBorrowV65&,std::uint64_t&,std::string&)> size;
 std::function<bool(const SourceIStreamBorrowV65&,void*,std::uint32_t,std::string&)> read;
};
// Loader owns allocation/clear/Grow/copy/publish order. Genuine stream leaves
// come from the existing generator owner; temporary lifetime ends after Assign.
bool copy_stream_to_level140_v65(CanonicalLevelContextV1&,const SourceIStreamBorrowV65&,const AssignedCopyLeavesV65&,std::string&);
bool assign_generated_stream_v65(CanonicalLevelContextV1&,const GeneratedSourceStreamV38&,const AssignedCopyLeavesV65&,std::string&);
struct AssignedReadLeavesV65 {
 std::shared_ptr<void> owner;
 // Existing canonical parser-result handler is reused unchanged.
 std::function<bool(bool,std::string&)> parse_result;
 // Original Buffer availability assertion/debug policy at LoadFile3f3f8c.
 // Invoked only when actual flag34 is zero; no callback owns Read/parser body.
 std::function<bool(std::string&)> unavailable_buffer;
};
AssignedRootServicesV64 assigned_root_kernel_services_v65(AssignedReadLeavesV65);
// Adapter for Stage7's existing SAME LifecycleBorrow signature. Weak Level
// avoids storing a strong Level owner back in its own assigned-resource slot.
std::function<bool(const LifecycleBorrowV36&,const GeneratedSourceStreamV38&,std::string&)>
 assigned_stream_stage7_binding_v65(std::weak_ptr<CanonicalLevelContextV1>,AssignedCopyLeavesV65);
}