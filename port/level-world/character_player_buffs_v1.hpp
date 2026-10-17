#pragma once
#include "../game-data/properties.hpp"
#include "character_timers.hpp"
#include <cstddef>
#include <memory>

// Pointer-only callbacks use the canonical Lua ABI; its definition belongs
// in the implementation rather than under a caller's type-renaming macro.
typedef struct dh2_script_value dh2_script_value;

namespace dh2::character_player_buffs_v1 {
enum class Operation : std::uint32_t {timer_start,timer_stop,timer_time_left,fx_load,fx_release,fx_object,fx_enable,apply_class,recalculate};
struct Request {
    Operation operation;std::uintptr_t character,subject;
    std::int32_t id,index,repeat,event;std::uint32_t duration,enabled;
    std::int32_t* sheet;
};
struct Response {std::uintptr_t identity;std::int32_t word;std::uint32_t elapsed,duration;};
struct Services {
    void* context;
    // Zero delivers the real action. Nonzero/throw stops after completed
    // effects; no rollback or extra cleanup. Timer actions borrow Coordinator.
    // apply_class calls dh2_class_apply into Request::sheet, with this view's
    // resolved sheet as its buff read source. recalculate calls
    // dh2_class_recalc_base on this view's base sheet and resolves all 224
    // properties. Both act on live sheets/groups, never atomic snapshots.
    int (*invoke)(void*,data::PropertyView*,const Request*,Response*);
};
// fx_count==UINT32_MAX means the actual catalog has not been supplied. Only a
// reached nonnumber FX guard needs it; numeric FX IDs reach the real provider.
struct Bindings {std::uintptr_t character;data::PropertyView* properties;Services services;std::uint32_t class_count,fx_count;};
enum class Status : std::int32_t {complete=1,invalid_argument=-1,provider_failed=-2,unsupported_domain=-3};
struct Result {std::uintptr_t instance;std::uint32_t calls;Operation last_operation;};
struct Snapshot {std::uintptr_t instance;std::int32_t id,timer;std::uint32_t strength;const std::int32_t* sheet;const char* name;std::uintptr_t fx;};
class Owner {
    struct Impl;std::unique_ptr<Impl> impl_;
    explicit Owner(std::unique_ptr<Impl>);
public:
    ~Owner();
    Owner(const Owner&)=delete;Owner& operator=(const Owner&)=delete;
    static std::unique_ptr<Owner> create(Bindings);
    Status add(std::int32_t id,std::uint32_t duration,std::int32_t capacity,std::uint32_t strength,std::int32_t fx,const char* name,Result*);
    Status remove(std::int32_t id,std::uintptr_t instance,Result*);
    Status remove_all(Result*);
    Status expired(const character::Timer32*,Result*);
    Status apply(std::int32_t class_id,std::uintptr_t instance,Result*);
    // Source destructor portion: free sheets and release nonnull FX; no timer
    // stop or recalc. End timer delivery, close VM, then retire before destruction.
    Status retire(Result*);
    // Reattach the same groups when a native adapter refreshes PropertyView.
    Status attach(data::PropertyView*);
    std::uintptr_t character_identity() const;
    std::uint32_t class_count() const;std::uint32_t fx_count() const;
    std::uint32_t count() const;std::uint32_t declarations() const;
    bool snapshot(std::uint32_t,Snapshot*) const;
    bool owned_sheet(std::uintptr_t,std::int32_t**) const;
};
struct CallbackBindings {Owner* owner;};
int create_buff(void*,const dh2_script_value*,std::uint32_t,dh2_script_value*,std::uint32_t,std::uint32_t*,char*,std::size_t) noexcept;
int remove_buff(void*,const dh2_script_value*,std::uint32_t,dh2_script_value*,std::uint32_t,std::uint32_t*,char*,std::size_t) noexcept;
int apply_buff(void*,const dh2_script_value*,std::uint32_t,dh2_script_value*,std::uint32_t,std::uint32_t*,char*,std::size_t) noexcept;
// One thread. Stable actor/view/provider backing through callbacks and VM close;
// no same-owner reentry/destruction/rebinding or retained sheet use after removal.
// Source instance sheets are the one buff store, not copied player properties.
// Finite direct numeric/boolean/nil/string-name callback domain only. Native
// ARM signed/unsigned float conversion saturates; negative unsigned is zero.
// Capacity Boolean true means 128, false means 1. Unprovided
// Value coercions/assertion paths fail explicitly. Allocator/STL bodies external.
}
