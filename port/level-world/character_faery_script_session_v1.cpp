#include "character_faery_script_session_v1.hpp"

namespace dh2::character_faery_script_session_v1 {
namespace {

bool same_owner(const Session& left, const Session& right) noexcept {
    return left.kind == right.kind && left.character == right.character &&
        left.char_ai_owner == right.char_ai_owner &&
        left.ais == right.ais &&
        left.char_ai_script == right.char_ai_script &&
        left.lua_instance == right.lua_instance &&
        left.lua_state == right.lua_state &&
        left.constructed == right.constructed &&
        left.character_bound == right.character_bound &&
        left.functions_bound == right.functions_bound &&
        left.common_loaded == right.common_loaded &&
        left.initialized == right.initialized &&
        left.init_post_called == right.init_post_called &&
        left.init_final_called == right.init_final_called &&
        left.close_owned == right.close_owned;
}

bool same_addresses(const Session& left, const Session& right) noexcept {
    return left.kind == right.kind && left.character == right.character &&
        left.char_ai_owner == right.char_ai_owner &&
        left.ais == right.ais &&
        left.char_ai_script == right.char_ai_script &&
        left.lua_instance == right.lua_instance &&
        left.lua_state == right.lua_state &&
        left.close_owned == right.close_owned;
}

bool same_script_owner(const Session& left, const Session& right) noexcept {
    return left.kind == right.kind &&
        left.char_ai_owner == right.char_ai_owner && left.ais == right.ais &&
        left.char_ai_script == right.char_ai_script &&
        left.lua_instance == right.lua_instance &&
        left.lua_state == right.lua_state &&
        left.constructed == right.constructed &&
        left.close_owned == right.close_owned;
}

} // namespace

bool Owner::matches(Identity character, Identity char_ai_owner) const noexcept {
    if (!character || !char_ai_owner || requested_character_ != character ||
        session_.char_ai_owner != char_ai_owner) return false;
    if (stage_ == Stage::ais_constructed || stage_ == Stage::functions_bound ||
        (stage_ == Stage::faulted && !session_.character))
        return !session_.character;
    return stage_ == Stage::closed || session_.character == character;
}

bool Owner::valid_constructed_session() const noexcept {
    return session_.kind == ScriptKind::faery && session_.constructed &&
        session_.close_owned && session_.char_ai_owner && session_.ais &&
        session_.char_ai_script == session_.ais &&
        session_.ais <= UINTPTR_MAX - 0x4 && !session_.character &&
        session_.lua_instance == session_.ais + 0x4 && session_.lua_state &&
        !session_.character_bound && !session_.functions_bound &&
        !session_.common_loaded && !session_.initialized;
}

bool Owner::valid_function_bound_session() const noexcept {
    return session_.kind == ScriptKind::faery && session_.constructed &&
        session_.close_owned && session_.char_ai_owner && session_.ais &&
        session_.char_ai_script == session_.ais &&
        session_.ais <= UINTPTR_MAX - 0x4 && !session_.character &&
        session_.lua_instance == session_.ais + 0x4 && session_.lua_state &&
        session_.functions_bound && !session_.character_bound &&
        !session_.common_loaded && !session_.initialized;
}

bool Owner::valid_bound_session() const noexcept {
    return session_.character == requested_character_ && requested_character_ &&
        session_.ais && session_.char_ai_script == session_.ais &&
        session_.ais <= UINTPTR_MAX - 0x4 &&
        session_.lua_instance == session_.char_ai_script + 0x4 &&
        session_.lua_state && session_.constructed && session_.functions_bound &&
        session_.character_bound && session_.close_owned;
}

bool Owner::valid_common_session() const noexcept {
    return valid_bound_session() && session_.common_loaded &&
        !session_.initialized;
}

Status Owner::construct(Identity character, Identity char_ai_owner,
                        const Services& services, std::string& error) {
    error.clear();
    if (stage_ != Stage::empty || session_.constructed) {
        error = "AISFaery session owner is already active or retired";
        return Status::already_constructed;
    }
    if (!character || !char_ai_owner || !services.context || !services.construct ||
        !services.dispatch) {
        error = "AISFaery requires a canonical Character, its CharAI owner, and complete script services";
        return Status::invalid_argument;
    }
    services_ = services;
    requested_character_ = character;
    int result = 1;
    try {
        // The provider executes CharAI::SetScript<AISFaery>: one 0xc8-byte
        // derived owner, CharAIScript(bool=true), and AISFaery init fields.
        // A failed provider must roll back its partial source allocation.
        result = services_.construct(services_.context, character,
                                     char_ai_owner, session_, error);
    } catch (...) {
        error = "AISFaery source constructor provider threw";
    }
    if (result != 0) {
        stage_ = Stage::faulted;
        if (error.empty()) error = "AISFaery source constructor provider failed";
        return Status::provider_failed;
    }
    if (!valid_constructed_session()) {
        stage_ = Stage::faulted;
        error = "AISFaery constructor returned a mismatched or pre-bound owner";
        return Status::identity_mismatch;
    }
    stage_ = Stage::ais_constructed;
    return Status::complete;
}

Status Owner::dispatch(Operation operation, Stage expected, Stage completed,
                       std::string& error) {
    if (stage_ != expected) {
        error = "AISFaery script operation is outside source lifecycle order";
        return Status::wrong_stage;
    }
    const bool owner_ready = operation == Operation::bind_functions
        ? valid_constructed_session() : operation == Operation::bind_character
            ? valid_function_bound_session() : operation == Operation::load_common
                ? valid_bound_session() : operation == Operation::initialize
                    ? valid_common_session() : valid_bound_session();
    if (!services_.dispatch || !owner_ready) {
        error = "AISFaery CharAIScript/LuaScript owner is incomplete";
        return Status::service_unavailable;
    }

    const Session before = session_;
    Request request{};
    request.operation = operation;
    request.character = requested_character_;
    request.char_ai_owner = session_.char_ai_owner;
    request.ais = session_.ais;
    request.char_ai_script = session_.char_ai_script;
    request.lua_instance = session_.lua_instance;
    int result = 1;
    try {
        result = services_.dispatch(services_.context, request, session_, error);
    } catch (...) {
        error = "AISFaery script lifecycle provider threw";
    }
    if (result != 0) {
        stage_ = Stage::faulted;
        if (error.empty()) error = "AISFaery script lifecycle provider failed";
        return Status::provider_failed;
    }

    // These are lifecycle certificates for callbacks that successfully
    // dispatched through this exact AIS-owned CharAIScript/LuaScript. The
    // provider does not own a parallel copy of the stage flags.
    if (operation == Operation::init_post) session_.init_post_called = true;
    if (operation == Operation::init_final) session_.init_final_called = true;

    bool valid = false;
    if (operation == Operation::bind_functions) {
        valid = same_addresses(before, session_) && valid_function_bound_session();
    } else if (operation == Operation::bind_character) {
        valid = same_script_owner(before, session_) && valid_bound_session() &&
            !session_.initialized;
    } else if (operation == Operation::load_common) {
        valid = same_addresses(before, session_) && valid_common_session();
    } else if (operation == Operation::initialize) {
        valid = same_addresses(before, session_) && valid_bound_session() &&
            session_.common_loaded && session_.initialized;
    } else if (operation == Operation::init_post) {
        valid = same_addresses(before, session_) && valid_bound_session() &&
            session_.initialized && session_.init_post_called;
    } else if (operation == Operation::init_final) {
        valid = same_addresses(before, session_) && valid_bound_session() &&
            session_.initialized && session_.init_post_called &&
            session_.init_final_called;
    } else if (operation == Operation::update) {
        valid = same_owner(before, session_) && valid_bound_session() &&
            session_.common_loaded && session_.initialized;
    }
    if (!valid) {
        stage_ = Stage::faulted;
        error = "AISFaery operation changed or omitted its canonical owner state";
        return Status::identity_mismatch;
    }
    stage_ = completed;
    error.clear();
    return Status::complete;
}

Status Owner::bind_character(std::string& error) {
    error.clear();
    // Source StepSetCharacter follows StepBindFunction. It stores Character
    // at +0x98, installs that Character's ScriptManager, and assigns the path
    // string at +0x68; the Lua Instance was already constructed at +0x4.
    return dispatch(Operation::bind_character, Stage::functions_bound,
                    Stage::character_bound, error);
}

Status Owner::bind_functions(std::string& error) {
    error.clear();
    // Source StepBindFunction runs while this AIS is pending and before
    // SetCharacter; it installs LuaScript and CharAIScript Binder entries on
    // the same Instance created by CharAIScript's base constructor.
    return dispatch(Operation::bind_functions, Stage::ais_constructed,
                    Stage::functions_bound, error);
}

Status Owner::initialize(std::string& error) {
    error.clear();
    // The faery's staged built-in has no external path; source StepInitScript
    // invokes its inherited AISDefault::OnInit after common has loaded.
    return dispatch(Operation::initialize, Stage::common_loaded,
                    Stage::initialized, error);
}

Status Owner::init_post(std::string& error) {
    error.clear();
    if (session_.init_post_called) {
        error = "AISFaery OnInitPost was already dispatched";
        return Status::wrong_stage;
    }
    return dispatch(Operation::init_post, Stage::initialized,
                    Stage::initialized, error);
}

Status Owner::init_final(std::string& error) {
    error.clear();
    if (!session_.init_post_called || session_.init_final_called) {
        error = "AISFaery OnInitFinal requires one prior OnInitPost";
        return Status::wrong_stage;
    }
    return dispatch(Operation::init_final, Stage::initialized,
                    Stage::initialized, error);
}

Status Owner::load_common(std::string& error) {
    error.clear();
    // CharAI load stage 3 calls LuaScript::Load("_commons") only after
    // BindFunction and SetCharacter have established this same Instance.
    return dispatch(Operation::load_common, Stage::character_bound,
                    Stage::common_loaded, error);
}

Status Owner::update(std::string& error) {
    error.clear();
    const auto status = dispatch(Operation::update, Stage::initialized,
                                 Stage::initialized, error);
    if (status == Status::complete) ++update_count_;
    return status;
}

Status Owner::close(std::string& error) {
    error.clear();
    // A failed provider construction must have rolled back before returning
    // an error. With no published owner there is no source AIS to destroy;
    // retire the failed certificate so its factory Record can be released.
    if (!session_.constructed && stage_ == Stage::faulted) {
        session_ = {};
        services_ = {};
        requested_character_ = 0;
        stage_ = Stage::closed;
        return Status::complete;
    }
    if (!session_.constructed || !session_.close_owned ||
        !session_.ais || session_.char_ai_script != session_.ais ||
        !services_.dispatch) {
        if (stage_ == Stage::empty || stage_ == Stage::closed)
            return Status::complete;
        error = "AISFaery destructor lacks the same owned CharAIScript session";
        return Status::service_unavailable;
    }
    Request request{};
    request.operation = Operation::close;
    request.character = requested_character_;
    request.char_ai_owner = session_.char_ai_owner;
    request.ais = session_.ais;
    request.char_ai_script = session_.char_ai_script;
    request.lua_instance = session_.lua_instance;
    int result = 1;
    try {
        result = services_.dispatch(services_.context, request, session_, error);
    } catch (...) {
        error = "AISFaery destructor provider threw";
    }
    if (result != 0) {
        stage_ = Stage::faulted;
        if (error.empty()) error = "AISFaery destructor provider failed";
        return Status::provider_failed;
    }
    session_ = {};
    services_ = {};
    requested_character_ = 0;
    stage_ = Stage::closed;
    error.clear();
    return Status::complete;
}

} // namespace dh2::character_faery_script_session_v1
