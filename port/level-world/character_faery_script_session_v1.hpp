#pragma once

#include <cstdint>
#include <string>

namespace dh2::character_faery_script_session_v1 {

using Identity = std::uintptr_t;

enum class ScriptKind : std::uint8_t { unknown, faery };

// Source-required session lifecycle for one CharAI::SetScript<AISFaery>.
// CharAIScript's LuaScript base constructs its Instance at AIS+0x4; +0x68 is
// the source script-path string and is not a VM/session pointer.
enum class Operation : std::uint32_t {
    construct_faery_ais,
    bind_character,
    bind_functions,
    load_common,
    initialize,
    init_post,
    init_final,
    update,
    close,
};

struct Request {
    Operation operation{};
    Identity character{};
    // Identity of this Character's canonical factory Component::ai owner.
    // AISFaery is constructed by that CharAI; this token is distinct from the
    // allocated AIS/CharAIScript address.
    Identity char_ai_owner{};
    Identity ais{};
    Identity char_ai_script{};
    Identity lua_instance{};
};

struct Session {
    ScriptKind kind{ScriptKind::unknown};
    Identity character{};
    Identity char_ai_owner{};
    Identity ais{};
    Identity char_ai_script{};
    Identity lua_instance{};
    Identity lua_state{};
    bool constructed{};
    bool character_bound{};
    bool functions_bound{};
    bool common_loaded{};
    bool initialized{};
    bool init_post_called{};
    bool init_final_called{};
    bool close_owned{};
};

struct Services {
    void* context{};
    // The constructor must be the canonical CharAI::SetScript<AISFaery>
    // source call, including exactly one CharAIScript/LuaScript constructor.
    // Dispatch supplies the source callbacks and destroys that same owner on
    // close; no borrowed Player/Ghost VM may be returned.
    int (*construct)(void*, Identity character, Identity char_ai_owner,
                     Session&, std::string&){};
    // Performs CharAIScript::BindFunction, SetCharacter, LuaScript::Load,
    // CallStateInit/Update and matching source destructor through this same
    // AIS-owned LuaScript.
    // The Instance subobject begins at char_ai_script + 4 (while +0x68 is its
    // script-path string); the provider reports its retained Lua state.
    int (*dispatch)(void*, const Request&, Session&, std::string&){};
};

enum class Status : std::uint8_t {
    complete,
    invalid_argument,
    service_unavailable,
    already_constructed,
    wrong_stage,
    identity_mismatch,
    provider_failed,
};

enum class Stage : std::uint8_t {
    empty,
    ais_constructed,
    functions_bound,
    character_bound,
    common_loaded,
    initialized,
    faulted,
    closed,
};

// Owns one CharAI::SetScript<AISFaery> lifecycle certificate and dispatches
// every later operation through the same AIS-owned LuaScript Instance at +4.
// It owns no second VM and does not manufacture AIS/Character pointers. The
// constructor/provider is responsible for the real canonical source objects.
class Owner final {
public:
    Owner() = default;
    Owner(const Owner&) = delete;
    Owner& operator=(const Owner&) = delete;

    Status construct(Identity character, Identity char_ai_owner,
                     const Services&, std::string& error);
    Status bind_character(std::string& error);
    Status bind_functions(std::string& error);
    Status load_common(std::string& error);
    Status initialize(std::string& error);
    Status init_post(std::string& error);
    Status init_final(std::string& error);
    Status update(std::string& error);
    Status close(std::string& error);

    const Session& session() const noexcept { return session_; }
    Stage stage() const noexcept { return stage_; }
    std::uint32_t update_count() const noexcept { return update_count_; }
    bool matches(Identity character, Identity char_ai_owner) const noexcept;

private:
    Status dispatch(Operation, Stage expected, Stage completed,
                    std::string& error);
    bool valid_constructed_session() const noexcept;
    bool valid_function_bound_session() const noexcept;
    bool valid_bound_session() const noexcept;
    bool valid_common_session() const noexcept;

    Services services_{};
    Session session_{};
    Identity requested_character_{};
    Stage stage_{Stage::empty};
    std::uint32_t update_count_{};
};

// This certifies an already-constructed source session. It creates nothing.
// Expected AIS identity is the CharAI active AIS for this same Character.
inline bool ready(const Session& session, Identity character,
                  Identity expected_char_ai_owner, Identity expected_ais,
                  const Services& services) noexcept {
    if (session.kind != ScriptKind::faery || !character ||
        !expected_char_ai_owner || !expected_ais ||
        !services.context || !services.construct ||
        !services.dispatch || session.character != character ||
        session.char_ai_owner != expected_char_ai_owner ||
        session.ais != expected_ais || session.char_ai_script != expected_ais ||
        expected_ais > UINTPTR_MAX - 0x4 ||
        session.lua_instance != expected_ais + 0x4 || !session.lua_state ||
        !session.constructed || !session.character_bound ||
        !session.functions_bound || !session.common_loaded ||
        !session.close_owned) {
        return false;
    }
    return session.initialized;
}

} // namespace dh2::character_faery_script_session_v1
