#pragma once
#include "character_script_lifecycle.hpp"
#include "character_script_selection.hpp"
#include "character_script_states.hpp"
#include "../script-runtime/script_function_alias.h"
#include "../script-runtime/script_int_bindings.hpp"
#include <cstddef>
#include <memory>
#include <string>
namespace dh2::character {
// Constructor-written projection: every supported class has common b8/bc/c0
// fields. Player-only vector/skill extensions set derived_present; Default and
// External are smaller c4 source allocations. No native vtable/layout claim.
struct ScriptConstructorFields72 {
 std::uintptr_t character,current_state;
 std::uint32_t owned,deferred,alias_main_count,alias_backup_count,loaded_file_count,
  state_count,argument_count,alias_recording,flags_b8,counter_bc,counter_c0,
  skill_d0,skill_d4,derived_present;
};
struct ScriptBinding24 {
 const char* name;
 std::uint32_t original_callback,method,character_receiver,reserved;
};
static_assert(sizeof(ScriptConstructorFields72)==72&&sizeof(ScriptBinding24)==24);
class ScriptOwner;
struct ScriptSessionView {
 std::uintptr_t identity;
 dh2_script_vm* vm;
 dh2_script_aliases* aliases;
 const ScriptConstructorFields72* constructor_fields;
 const char* path;
 std::uint32_t kind;
 // Live VCB producer result, separate from the initial constructor projection.
 std::uint32_t callback_flags;
 std::size_t loaded_files;
};
static_assert(sizeof(ScriptSessionView)==56);
enum ScriptOwnerService : std::uint32_t {
 owner_register_binding,owner_cached_file,owner_ai_terminate,
 owner_is_character,owner_budget,owner_is_dead,owner_timer_stop,
 owner_design_tick,owner_timer_start,owner_init_vcb
};
struct ScriptOwnerRequest {
 std::uint32_t service,argument0,argument1,reserved;
 std::uintptr_t subject;
 const ScriptSessionView* session;
 const ScriptBinding24* binding;
 const char* filename;
};
struct ScriptOwnerResponse {
 std::uint32_t word=0,reserved=0;
 const void* bytes=nullptr;
 std::size_t size=0;
};
struct ScriptOwnerServices {
 void* context=nullptr;
 // Required services are genuine registrations/cache/Character operations.
 // Return0 for delivery (word0 can be source false/missing file); nonzero is
 // an explicit native provider failure, never an accepted source no-op.
 // Borrowed bytes survive this call through the synchronous VM load. Request
 // view/descriptor pointers are ephemeral; registered userdata/services must
 // independently outlive the corresponding VM and its finalizers.
 // Captured script/Character receivers must remain alive across reentry.
 int(*invoke)(void*,ScriptOwner&,const ScriptOwnerRequest&,ScriptOwnerResponse&)=nullptr;
};
// Additive native-integer mode. The external context/functions must outlive the
// owner and its VM finalizers; each private Session owns its own map/receiver.
// Missing identity/fraction services remain explicit unsupported coercions.
struct ScriptNativeIntegerConfiguration32 {
 void* context=nullptr;
 dh2_script_int_identity identity=nullptr;
 dh2_script_int_format_fraction format_fraction=nullptr;
 std::uint64_t reserved=0;
};
static_assert(sizeof(ScriptNativeIntegerConfiguration32)==32);
class ScriptOwner {
 public:
 explicit ScriptOwner(std::uintptr_t character,std::size_t vm_memory_limit=16u*1024u*1024u);
 // The legacy constructor above preserves provider-only registration. This
 // mode installs source SetInt/GetInt individually at Stage1 indices2/3,
 // BEFORE their ordered provider deliveries; providers may override at those
 // exact deliveries. Failure retains the installed native prefix.
 ScriptOwner(std::uintptr_t character,std::size_t vm_memory_limit,
  const ScriptNativeIntegerConfiguration32& native_integers);
 ~ScriptOwner();
 ScriptOwner(const ScriptOwner&)=delete;
 ScriptOwner& operator=(const ScriptOwner&)=delete;
 ScriptLifecycleState64& lifecycle() noexcept;
 const ScriptLifecycleState64& lifecycle() const noexcept;
 bool find(std::uintptr_t identity,ScriptSessionView& out) const noexcept;
 bool pending(ScriptSessionView& out) const noexcept;
 bool active(ScriptSessionView& out) const noexcept;
 // Fresh actual selected AIS constructor's Character overload at virtual+b4.
 // False/no callback when no active private Session; does not select by actor type.
 bool source_combat_results_callback(std::uint32_t& out) const noexcept;
 // Actual retained constructor vptr's virtual+24 OnDied, captured from ELF.
 bool source_death_callback(std::uint32_t& out) const noexcept;
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
 // VCB executes locally; unrecovered player-specific VCB remains a service.
 int advance(const ScriptCreationFacts24&,const ScriptOwnerServices&);
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
  const ScriptOwnerServices& persistent_services,const dh2_script_include_scope*);
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
 int load_file(std::uintptr_t,const char* name,const ScriptOwnerServices&,bool& loaded);
 int call_discard(std::uintptr_t,const char* name,const dh2_script_value*,
  std::uint32_t count,std::uint32_t& source_error);
 int init_vcb(std::uintptr_t,const ScriptOwnerServices&);
 //Actual retained AIS virtual8/12/16/20, with owner VM execution/required-
 //failure guards. Used by whole save/restore AI_ScriptInit/CleanUp bodies.
 int source_initial_virtual_v86(std::uintptr_t,std::uint32_t slot);
 // AIUnLoadScriptProcess owns the ordered skill/spell cleanup before this
 // actual AIS deleting destructor. No OnTerminate or Init replay occurs.
 int source_release_active_v105();
 int source_states_update_v108(std::uintptr_t,const ScriptStatesCalls24&);
 bool source_faery_skin_cache_v111(std::uintptr_t,std::shared_ptr<void>&,std::int32_t*&);
 private:
 struct Impl;
 std::unique_ptr<Impl> impl_;
};
}
extern "C" int dh2_character_script_constructor_fields(
 dh2::character::ScriptConstructorFields72*,std::uint32_t kind);
// Ordered captured registration descriptors; phase1=35, phase2=265 entries.
extern "C" std::uint32_t dh2_character_script_binding_count(std::uint32_t phase);
extern "C" int dh2_character_script_binding(
 dh2::character::ScriptBinding24*,std::uint32_t phase,std::uint32_t index);
