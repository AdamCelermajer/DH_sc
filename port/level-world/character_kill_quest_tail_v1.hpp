#pragma once

#include <cstdint>
#include <string>

namespace dh2::character_kill_quest_tail_v1 {

enum class Kind : std::uint32_t {
    kill_enemies, clear_enemies, kill_enemy_template, clear_enemy_template
};

// Semantic projection of the four local IEvent records built by
// Character::Kill. The runtime translates this to the corresponding source
// QE_* class; this is not an ABI/wire-layout claim.
struct Event {
    Kind kind{};
    std::int32_t objective_type=0;
    std::uintptr_t killer=0;
    std::uintptr_t character_word_25=0;
    std::int16_t character_halfword_2532=0;
    std::int16_t character_halfword_2533=0;
    std::int32_t source_subject=-1;
    std::int16_t source_word_24=0;
    std::uint8_t flag0=0,flag1=0;
};

struct Services {
    void* context=nullptr;
    bool (*current_level)(void*,std::uintptr_t& level,std::string& error)=nullptr;
    bool (*character_word_25)(void*,std::uintptr_t character,std::uintptr_t& value,
                              std::string& error)=nullptr;
    bool (*character_halfword)(void*,std::uintptr_t character,std::uint32_t index,
                               std::int16_t& value,std::string& error)=nullptr;
    bool (*get_constant)(void*,const char* group,const char* key,
                         std::int32_t& value,std::string& error)=nullptr;
    bool (*raise_async)(void*,std::uintptr_t current_level,
                        Event&,std::string& error)=nullptr;
};

struct Bindings {
    std::uintptr_t character=0,killer=0;
    // Fresh source facts from Character::Kill: virtual+84 and byte+5348.
    std::uint32_t virtual_84_true=0,suppress_byte_5348=0;
};

struct Result {
    std::uint32_t skipped=0,events_attempted=0,events_raised=0;
    Kind last_kind=Kind::kill_enemies;
    std::int32_t last_objective_type=0;
};

enum class Status : std::uint32_t {
    complete,invalid_argument,busy,consumed,provider_failed
};

// Called only after Character::Kill has reached its quest tail (post-loot and
// post-killer-credit). It preserves the source gates, event order, constants,
// shared mutable event semantics, and reached-prefix failures. It does not
// own a Level, EventManager, quest records, killer credit, loot, or Character.
class Runtime {
    Bindings bindings_;
    Services services_;
    bool busy_=false,consumed_=false;
public:
    Runtime(Bindings,Services);
    Runtime(const Runtime&)=delete;
    Runtime& operator=(const Runtime&)=delete;
    Status run(Result*,std::string& error);
};

} // namespace dh2::character_kill_quest_tail_v1
