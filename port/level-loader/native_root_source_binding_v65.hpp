#pragma once
#include "level_source_loading_v43.hpp"
#include "retained_level_module_graph_v1.hpp"
#include <optional>
namespace dh2::loader {
// Native integration adapter, not an original engine entry point. Supplies only
// actual IStream/CFS/debug leaves; loader kernels own copy/parser/cursor/release.
struct NativeRootSourceLeavesV65 {
 FilenameSourceLeavesV65 filename;
 AssignedReadLeavesV65 read;
 // Generated stream Size/Read can have a different authentic provider from
// filesystem handles. Required for procedural Levels, absent for fixed maps.
 std::optional<AssignedCopyLeavesV65> generated_copy;
};
namespace native_root_compose_detail_v65 {
inline bool owns_level(const std::shared_ptr<void>& provider,
 const std::shared_ptr<CanonicalLevelContextV1>& level) noexcept {
 return provider&&level&&!provider.owner_before(level)&&!level.owner_before(provider);
}
inline bool assigned_provider_present(const AssignedRootServicesV64& s) {
 return s.owner||s.borrow||s.read_document||s.parse_result||s.observe_walk||s.before_element||s.release;
}
inline bool copy_complete(const AssignedCopyLeavesV65& s) {
 return s.owner&&s.size&&s.read;
}
}
// Compose once on the SAME candidate before SourceLoadingV43::create. Does not
// open/copy/read/parse a file, publish Level140, or run a generated-ready leaf.
// Failure leaves the existing input bindings untouched. Existing historical
// ZIP filename callbacks keep their explicit inspection contract; this entry
// point neither preserves them as production providers nor replaces them.
inline bool bind_native_root_source_v65(NativeRootSourceLeavesV65 leaves,
 SourceLoadingInputsV43& inputs,std::string& error) {
 using namespace native_root_compose_detail_v65;
 if(!inputs.preparation||!inputs.manager||inputs.manager.get()!=&inputs.preparation->manager()){
  error="Native root composition requires SAME retained preparation/manager";return false;
 }
 auto level=inputs.preparation->level();
 if(!level||level->constructor_fields_v3().field140||level->assigned_source_owner_slot_v65()){
  error="Native root composition requires fresh SAME Level140 ownership";return false;
 }
 if(inputs.native_filename_source||assigned_provider_present(inputs.assigned_source)||
    inputs.filename_provider||inputs.resolve_filename||inputs.procedural.assign_stream){
  error="Native root composition cannot replace existing source bindings";return false;
 }
 if(!leaves.filename.owner||!leaves.filename.is_using_uncompiled_data||
    !leaves.filename.open_resource||!leaves.filename.close_source||
    !copy_complete(leaves.filename.copy)||!leaves.read.owner||!leaves.read.parse_result){
  error="Native root composition requires genuine CFS open/close, IStream Size/Read and parser-result services";return false;
 }
 if(owns_level(leaves.filename.owner,level)||owns_level(leaves.filename.copy.owner,level)||
    owns_level(leaves.read.owner,level)){
  error="Native root provider cannot retain its SAME Level control block";return false;
 }
 if(level->constructor_fields_v3().byte_e8&&!leaves.generated_copy){
  error="Procedural native root requires actual generated-stream Size/Read leaves";return false;
 }
 if(leaves.generated_copy&&(!copy_complete(*leaves.generated_copy)||
    owns_level(leaves.generated_copy->owner,level))){
  error="Generated copy requires independent genuine stream Size/Read services";return false;
 }
 try{
  // Keep genuine independent leaf authority for each later Module MGP/MVP
  // occurrence; root and module copies/parsers are separate occurrences on
  // SAME Level140 and may never interleave.
  auto module_filename=leaves.filename;auto module_read=leaves.read;
  auto assigned=assigned_root_kernel_services_v65(std::move(leaves.read));
  auto filename=std::make_shared<FilenameRootRouteV65>(std::move(leaves.filename));
  decltype(inputs.procedural.assign_stream) assign;
  if(leaves.generated_copy)
   assign=assigned_stream_stage7_binding_v65(std::weak_ptr<CanonicalLevelContextV1>(level),std::move(*leaves.generated_copy));
  // All allocation/composition precedes mutation. These moves publish the
  // input services only; actual assigned receiver publication happens at copy.
  // Module binding is last fallible composition; remaining moves are noexcept.
  auto modules=inputs.preparation->module_files();
  if(!modules||!modules->bind_native_source(std::move(module_filename),std::move(module_read),error))return false;
  inputs.assigned_source=std::move(assigned);
  inputs.native_filename_source=std::move(filename);
  inputs.procedural.assign_stream=std::move(assign);
  error.clear();return true;
 }catch(const std::exception& ex){error=ex.what();return false;}
}
}
