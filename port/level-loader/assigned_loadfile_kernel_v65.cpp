#include "assigned_loadfile_kernel_v65.hpp"
#include <exception>
namespace dh2::loader {
namespace {
bool same_owner(const std::shared_ptr<void>& a,const std::shared_ptr<void>& b){return !a.owner_before(b)&&!b.owner_before(a);}
std::shared_ptr<AssignedLoadFileDataV65> live(const AssignedRootBorrowV64& b,std::string& e){
 if(!assigned_root_live_v64(b,e))return {};
 auto state=std::static_pointer_cast<AssignedLoadFileDataV65>(b.assigned_alias);
 if(state->retired||b.stream_identity!=reinterpret_cast<std::uintptr_t>(state->stream.get())||b.document38!=&state->document38||b.root3c!=&state->root3c||b.node40!=&state->node40||b.child44!=&state->child44||b.flag48!=&state->flag48||b.buffer_flag34!=&state->buffer_flag34){e="Required SAME live loader-owned assigned cells";return {};}
 return state;
}
}
bool copy_stream_to_level140_v65(CanonicalLevelContextV1& level,const SourceIStreamBorrowV65& generated,const AssignedCopyLeavesV65& leaves,std::string& e){
 auto native=level.constructor_borrow_v3();auto& owning=level.assigned_source_owner_slot_v65();auto& operation=level.assigned_source_operation_slot_v65();
 if(operation){operation=2;e="Assigned stream mutation reentered";return false;}
 if(!native.owner||!native.fields||!leaves.owner||!leaves.size||!leaves.read||!generated.actual_owner||!generated.identity||native.fields->field140||owning){e="Required fresh SAME Level140 and actual generator Size/Read leaves";return false;}
 operation=1;struct Guard{std::uint8_t& flag;~Guard(){flag=0;}} guard{operation};
 try{
  // Allocate receiver before StreamBuffer C1(Size/Grow/Read), as3f02c0/e4.
  auto state=std::make_shared<AssignedLoadFileDataV65>();std::uint64_t source_size=0;
  if(!leaves.size(generated,source_size,e))return false;
  if(operation!=1){if(e.empty())e="Assigned mutation recursively failed during Size";return false;}
  if(native.fields->field140||owning){e="Actual assigned slot changed during Size";return false;}
  // StreamBuffer::expand317180 passes only low32 to Buffer::setSize317014.
  // Fresh Buffer clear resets flag; setSize owns one allocation + NULL sentinel,
  // then source3170cc writes flag1 and3170d0/d4 writes requested size/count.
  const auto copied_size=static_cast<std::uint32_t>(source_size);
  std::vector<std::uint8_t> bytes(copied_size);
  state->stream=std::make_unique<dh2::level::SavegameStreamV2>(std::move(bytes));
  state->buffer_flag34=1;
  if(!leaves.read(generated,state->stream->source_copy_destination_v65(),copied_size,e))return false;
  if(operation!=1){if(e.empty())e="Assigned mutation recursively failed during Read";return false;}
  if(native.fields->field140||owning){e="Actual assigned slot changed during Size/Grow/Read";return false;}
  // Receiver38/3c/40/44/48 were zero-initialized. Publish raw140 LAST.
  owning=state;native.fields->field140=reinterpret_cast<std::uintptr_t>(state.get());return true;
 }catch(const std::exception& ex){if(e.empty())e=ex.what();return false;}
}
bool assign_generated_stream_v65(CanonicalLevelContextV1& level,const GeneratedSourceStreamV38& source,const AssignedCopyLeavesV65& leaves,std::string& e){
 return copy_stream_to_level140_v65(level,{source.actual_owner,source.identity},leaves,e);
}
AssignedRootServicesV64 assigned_root_kernel_services_v65(AssignedReadLeavesV65 leaves){
 AssignedRootServicesV64 services;services.owner=leaves.owner;
 services.borrow=[](CanonicalLevelContextV1& level,AssignedRootBorrowV64& out,std::string& e){
  auto native=level.constructor_borrow_v3();auto& owning=level.assigned_source_owner_slot_v65();
  if(!native.owner||!native.fields||!owning||owning.get()!=reinterpret_cast<void*>(native.fields->field140)){e="Required actual loader-owned assigned140 receiver";return false;}
  auto state=std::static_pointer_cast<AssignedLoadFileDataV65>(owning);
  if(state->retired||!state->stream){e="Assigned stream receiver already retired";return false;}
  out={native.owner,level.identity(),&native.fields->field140,&owning,native.fields->field140,reinterpret_cast<std::uintptr_t>(state->stream.get()),owning,std::shared_ptr<const void>(owning,state->stream.get()),&state->buffer_flag34,&state->document38,&state->root3c,&state->node40,&state->child44,&state->flag48};return true;
 };
 services.read_document=[leaves](const AssignedRootBorrowV64& b,const std::string& name,XmlDocumentV1::Borrow& out,std::shared_ptr<const void>& alias,std::string& e){
  auto state=live(b,e);if(!state)return LifecycleStepV36::failed;
  if(state->read_failed){e=state->error;return LifecycleStepV36::failed;}
  if(state->read_started){
   if(name!=state->source_name||!state->xml.borrow()){e="Assigned native Read prefix cannot replay/change source";state->read_failed=true;state->error=e;return LifecycleStepV36::failed;}
   out=state->xml.borrow();alias=std::shared_ptr<const void>(state->backend,reinterpret_cast<const void*>(state->document38));return LifecycleStepV36::complete;
  }
  state->read_started=true;state->source_name=name;
  auto fail=[&]{state->read_failed=true;state->error=e;return LifecycleStepV36::failed;};
  try{
  // Actual TiXML C1 and38 publication precede original buffer/debug checks.
  if(!XmlDocumentV1::allocate_native_backend_v65(state->backend,e))return fail();
  state->document38=state->backend->identity();
  if(!state->buffer_flag34){
   if(!leaves.unavailable_buffer){e="Required original DebugSwitch buffer assertion leaf";return fail();}
   if(!leaves.unavailable_buffer(e)||state->read_failed||!assigned_root_live_v64(b,e))return fail();
  }
  // Original LoadFromBuffer normalization + ONE parse in actual retained doc38.
  // Snapshot projection maps that same backend; no alternate parser or reparse.
  if(!state->xml.capture_retained_level_buffer_v65(name,state->stream->bytes(),state->backend,e))return fail();
  if(!assigned_root_live_v64(b,e))return fail();
  auto document=state->xml.borrow();
  if(!document.diagnostic().code){state->root3c=state->document38;state->node40=document.native_top_level_identity_v65(0);}
  out=std::move(document);alias=std::shared_ptr<const void>(state->backend,reinterpret_cast<const void*>(state->document38));return LifecycleStepV36::complete;
  }catch(const std::exception& ex){if(e.empty())e=ex.what();return fail();}
 };
 services.parse_result=[leaves](const AssignedRootBorrowV64& b,bool ok,std::string& e){
  if(!live(b,e))return false;if(!leaves.parse_result){e="Required existing canonical XML parser-result handler";return false;}return leaves.parse_result(ok,e);
 };
 services.observe_walk=[](const AssignedRootBorrowV64& b,const LevelFileWalkV1& walk,std::string& e){
  auto state=live(b,e);if(!state)return false;
  if(!walk.document||walk.document.native_document_identity_v65()!=state->document38){e="Walk must project actual assigned native document38";return false;}
  state->node40=walk.document.native_top_level_identity_v65(walk.root_cursor);state->child44=0;
  if(walk.root_cursor<walk.document.top_level_nodes().size()&&!walk.advance_root){
   const auto& node=walk.document.top_level_nodes()[walk.root_cursor];
   if(node.kind==1&&node.element!=UINT32_MAX&&node.value==walk.requested_root){
    const auto& children=walk.document.elements()[node.element].children;
    if(walk.child_cursor<children.size())state->child44=walk.document.native_element_identity_v65(children[walk.child_cursor]);
   }
  }
  return true;
 };
 services.before_element=[](const AssignedRootBorrowV64& b,const XmlDocumentV1::Borrow& document,std::uint32_t element,std::string& e){
  auto state=live(b,e);if(!state)return false;
  if(document.native_document_identity_v65()!=state->document38||!state->child44||document.native_element_identity_v65(element)!=state->child44){e="Factory must consume SAME actual native child44";return false;}return true;
 };
 services.release=[](const AssignedRootReleaseV64& b,std::string& e){
  // Only live Level/owning metadata is inspected before any native cell access.
  if(!*b.slot140){if(*b.owning_slot&&!same_owner(*b.owning_slot,b.assigned_alias)){e="Cannot drop replacement assigned owner";return false;}b.owning_slot->reset();return true;}
  if(*b.slot140!=b.assigned_identity||!*b.owning_slot||b.owning_slot->get()!=b.assigned_alias.get()||!same_owner(*b.owning_slot,b.assigned_alias)){e="Cannot D0 replacement assigned receiver";return false;}
  auto state=std::static_pointer_cast<AssignedLoadFileDataV65>(b.assigned_alias);
  if(state->retired){e="Assigned D0 was not followed by native140 clear";return false;}
  // Original completion D0s document38 first, then reloads140 and D0s assigned
  // state (whose D0 owns only StreamBuffer/CustomFree), finally140=0.
  if(state->backend)state->backend->release_native();
  // Original leaves retired document38 untouched until assigned receiver D0;
  // retained metadata aliases never dereference that retired pointer.
  state->stream.reset();state->buffer_flag34=0;state->retired=true;
  *b.slot140=0;b.owning_slot->reset();return true;
 };
 return services;
}
std::function<bool(const LifecycleBorrowV36&,const GeneratedSourceStreamV38&,std::string&)> assigned_stream_stage7_binding_v65(std::weak_ptr<CanonicalLevelContextV1> weak,AssignedCopyLeavesV65 leaves){
 return [weak=std::move(weak),leaves=std::move(leaves)](const LifecycleBorrowV36& actual,const GeneratedSourceStreamV38& source,std::string& e){
  auto level=weak.lock();if(!level){e="Assigned Level lifetime ended";return false;}auto native=level->constructor_borrow_v3();
  if(actual.actual_level_owner.get()!=level.get()||actual.actual_level_owner.owner_before(native.owner)||native.owner.owner_before(actual.actual_level_owner)){e="Stage7 Assign requires SAME actual Level owner";return false;}
  return assign_generated_stream_v65(*level,source,leaves,e);
 };
}
}