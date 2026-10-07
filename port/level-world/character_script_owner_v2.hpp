#pragma once
#include "character_script_owner.hpp"
#include "character_player_aggro_owner_v1.hpp"
namespace dh2::character {
class ScriptOwnerV2;
struct ScriptOwnerServicesV2 {void* context=nullptr;int(*invoke)(void*,ScriptOwnerV2&,const ScriptOwnerRequest&,ScriptOwnerResponse&)=nullptr;};
class ScriptOwnerV2 {
 public:
 int source_states_update_v108(std::uintptr_t,const ScriptStatesCalls24&);
 bool source_faery_skin_cache_v111(std::uintptr_t,std::shared_ptr<void>&,std::int32_t*&);
 explicit ScriptOwnerV2(std::uintptr_t character,std::size_t vm_memory_limit=16u*1024u*1024u);
 // The legacy constructor above preserves provider-only registration. This
 // mode installs source SetInt/GetInt individually at Stage1 indices2/3,
 // BEFORE their ordered provider deliveries; providers may override at those
 // exact deliveries. Failure retains the installed native prefix.
 ScriptOwnerV2(std::uintptr_t character,std::size_t vm_memory_limit,
  const ScriptNativeIntegerConfiguration32& native_integers);
 ~ScriptOwnerV2();
 ScriptOwnerV2(const ScriptOwnerV2&)=delete;
 ScriptOwnerV2& operator=(const ScriptOwnerV2&)=delete;
 ScriptLifecycleState64& lifecycle() noexcept;
 const ScriptLifecycleState64& lifecycle() const noexcept;
 bool find(std::uintptr_t identity,ScriptSessionView& out) const noexcept;
 bool pending(ScriptSessionView& out) const noexcept;
 bool active(ScriptSessionView& out) const noexcept;
 // Actual selected constructor's retained AIS vtable+b4 producer. No active
 // session returns false; it never substitutes a GameObject overload.
 bool source_combat_results_callback(std::uint32_t& out)const noexcept;
 // Exact actual retained constructor vptr's virtual+24, matching V1 owner.
 bool source_death_callback(std::uint32_t& out)const noexcept;
 // Pins the actual selected AISPlayer/IPhone Session and lends its original
 // constructor-written b8..d4 fields. No active player AIS is a genuine miss.
 bool source_player_aggro_v84(std::shared_ptr<void>& session_pin,
  std::uintptr_t& selected_ais,PlayerAggroFieldsV1*& fields)const noexcept;
 // Borrowed stable Session member, not an ephemeral view/request. False for
 // legacy mode or unknown identity; output is set null. Alive through VM close,
 // expires when that Session dies. Bound callbacks must use the direct receiver
 // rather than look the closing/erased Session up through this owner again.
 bool integer_bindings(std::uintptr_t identity,
  const dh2_script_int_bindings*& out) const noexcept;
 // Actual Step0..6 order via original-derived selector/lifecycle. Delayed-load
 // budget and synchronous provider mutations use those kernels' reload points.
 // Return0 completed, -1 malformed caller, -2 provider/VM/allocation failure.
 // Failed provider/open prefixes remain observable. Delivered missing files or
 // Lua load errors are source AddFile false and still reach source publication.
 // External Init Lua errors likewise remain diagnostics and continue through
 // actual alias-membership InitVCB and source publication. External/default
 // and source AISPlayer/IPhone InitVCB execute on these owned callback flags.
 int advance(const ScriptCreationFacts24&,const ScriptOwnerServicesV2&);
 // Dedicated source Include only: caller owns persistent provider/context.
 // Scope must be live and select this exact privately owned VM. Loaded/missing/
 // positive Lua-error results are delivered0; malformed scope/caller -1,
 // provider or unsupported runtime failure -2. Generic VM reentry is forbidden.
 // Provider must not destroy this owner/receiver while outer Lua is executing.
 // advance with work remaining is rejected while this owner's VM is executing;
 // nested Include through fresh scopes remains allowed, including same-file
 // recursion before successful loaded-set insertion. No ephemeral services are
 // stored after this synchronous call.
 int include(std::uintptr_t identity,const char* requested_name,
  const ScriptOwnerServicesV2& persistent_services,const dh2_script_include_scope*);
 int last_vm_status() const noexcept;
 // Exact latest source parser/pcall status:0 success, positive Lua status,
 // negative native diagnostic. last_vm_status preserves legacy -2 Lua mapping.
 int last_source_load_status() const noexcept;
 const std::string& error() const noexcept;
 // Additive skill-setup delivery on a known privately owned Script identity.
 // copy/assign preserve complete string bytes, including embedded NUL. Caller
 // chooses active freshly; these never substitute a pending Script. Providers
 // and receiver remain alive through synchronous calls. Native failure returns
 // -2; malformed/busy calls -1. Lua diagnostics are delivered separately.
 int copy_path(std::uintptr_t,std::string& out) const noexcept;
 int assign_path(std::uintptr_t,const char* bytes,std::size_t size) noexcept;
 int load_file(std::uintptr_t,const char* name,const ScriptOwnerServicesV2&,bool& loaded);
 int call_discard(std::uintptr_t,const char* name,const dh2_script_value*,
  std::uint32_t count,std::uint32_t& source_error);
 int init_vcb(std::uintptr_t,const ScriptOwnerServicesV2&);
 int source_initial_virtual_v86(std::uintptr_t,std::uint32_t slot);
 int source_release_active_v105();
 private:
 struct Impl;
 std::unique_ptr<Impl> impl_;
};

}
