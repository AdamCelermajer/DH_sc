#pragma once
#include "level_preparation_v1.hpp"
#include "character_kill.hpp"
#include "level_constructor_v3.hpp"

namespace dh2::loader {
// One retained game-Level receiver, distinct from LevelConfig and Module.
// This owns the source constructor fields and optional once-only complete C1
// continuation. Lua/save implementations are required services of that body;
// whole Level Init remains separate. Source preparation is not readiness.
class CanonicalLevelContextV1 final : public std::enable_shared_from_this<CanonicalLevelContextV1> {
    LevelSourceRequestV1 request_;
    std::shared_ptr<void> services_;
    LevelPreparationV1::Borrow prepared_;
    std::uintptr_t config38_{}; // Source LevelC1 store3f319c.
    std::int32_t music11c_{-1},safezone120_{-1},ambient124_{-1};
    // The ONLY writable source word150, inside this SAME Level's native view.
    character::KillLevel16 kill_{};
    // SAME Level Module::Load fields, exact LevelC1 direct stores.
    std::array<float,3> module_offset160_{};
    std::int32_t object_module_id18c_{-1};
    // Same Level's additional C1 fields and exact once-only continuation.
    // create() retains the historical field prefix; it does not run this body.
    LevelConstructorFieldsV3 constructor_fields_v3_;
    std::unique_ptr<LevelConstructorV3> constructor_v3_;
    CanonicalLevelContextV1(LevelSourceRequestV1,std::shared_ptr<void>);
public:
    CanonicalLevelContextV1(const CanonicalLevelContextV1&)=delete;
    CanonicalLevelContextV1& operator=(const CanonicalLevelContextV1&)=delete;
    static bool create(LevelSourceRequestV1,std::shared_ptr<void> services,
        std::shared_ptr<CanonicalLevelContextV1>& out,std::string& error);
    std::uintptr_t identity()const noexcept{return kill_.identity;}
    const character::KillLevel16* kill_level()const noexcept{return &kill_;}
    std::uint32_t& source_word150()noexcept{return kill_.loot_gate150;}
    const LevelSourceRequestV1& source_request()const noexcept{return request_;}
    const LevelPreparationV1::Borrow& prepared_source()const noexcept{return prepared_;}
    // Attaches only a matching completed SOURCE description, never an active
    // world. Invalid input preserves this receiver's previous source borrow.
    bool retain_prepared_source(LevelPreparationV1::Borrow,std::string&);
    struct ConfigFields {
        std::shared_ptr<void> level_owner;
        std::uintptr_t* config38{};
        std::int32_t* music11c{};std::int32_t* safezone120{};std::int32_t* ambient124{};
    };
    // Direct inputs to main's exact LevelConfigPublicationBorrowV2. The caller
    // adds its actual Arrays::Sounds lease/catalog and canonical config lookup.
    ConfigFields config_fields();
    struct ModuleLoadFields {
        std::shared_ptr<void> level_owner;
        std::int32_t* object_module_id18c{};
        float* module_offset160{};
    };
    // Direct inputs to root's ModuleLevelLoadBorrowV1. This creates no Module
    // selector, load callback, current-Level slot or publication path. Caller
    // supplies the original Level::LoadFile continuation on the SAME candidate.
    ModuleLoadFields module_load_fields();
    LevelConstructorBorrowV3 constructor_borrow_v3();
    bool construct_source_v3(LevelConstructorArgumentsV3,LevelConstructorServicesV3,std::string&);
    const LevelConstructorFieldsV3& constructor_fields_v3()const noexcept{return constructor_fields_v3_;}
    const LevelConstructorV3* constructor_owner_v3()const noexcept{return constructor_v3_.get();}
};

// Borrow the ONE authoritative retained GSLevel::s_level global. Original
// Application::GetCurrentLevel31f594 reads it via GOT9967fc/global9a2638;
// Application owns no parallel slot. globals_owner pins the actual retained
// global storage/provider. Root alone implements GSLevel Ctor/Dtor publication
// and its +34 lifetime using this SAME Level receiver. This API creates neither
// a singleton nor a publication path. Do not store returned borrows in owners
// they pin. Access stays sequential on the runtime's owning thread.
struct CanonicalGSLevelGlobalSlotV1 {
    std::shared_ptr<void> globals_owner;
    std::shared_ptr<CanonicalLevelContextV1>* s_level{};
};
class CanonicalCurrentLevelBorrowV1 {
    friend bool borrow_current_canonical_level_v1(const CanonicalGSLevelGlobalSlotV1&,
        CanonicalCurrentLevelBorrowV1&,std::string&);
    std::shared_ptr<void> globals_;
    std::shared_ptr<CanonicalLevelContextV1> level_;
public:
    explicit operator bool()const noexcept{return bool(level_);}
    const character::KillLevel16* kill_level()const noexcept{return level_?level_->kill_level():nullptr;}
    std::uintptr_t identity()const noexcept{return level_?level_->identity():0;}
    const std::shared_ptr<CanonicalLevelContextV1>& level()const noexcept{return level_;}
};
// A real empty GSLevel global succeeds with a null borrow, preserving the
// Kill source's explicit null-Level branch. Missing slot/lease is service
// failure and leaves out unchanged. Keep the returned borrow through Kill and
// synchronous callbacks; it pins the same live fields, not a scalar snapshot.
bool borrow_current_canonical_level_v1(const CanonicalGSLevelGlobalSlotV1&,
    CanonicalCurrentLevelBorrowV1& out,std::string& error);
}
