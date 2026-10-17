#include "character_faery_record_session_v1.hpp"

namespace dh2::character_faery_record_session_v1 {
namespace {

using Identity = session::Identity;
using Component = factory::ctor::Component;

bool canonical_record(factory::Record& record, Identity& character,
                     Identity& ai) noexcept {
    character = record.character.identity;
    ai = reinterpret_cast<Identity>(record.components.find(Component::ai));
    return character && record.constructor.identity == character && ai;
}

bool same_record(factory::Record& record, std::string& error) {
    Identity character = 0;
    Identity ai = 0;
    if (!canonical_record(record, character, ai)) {
        error = "AISFaery requires a constructed factory Record and canonical Component::ai";
        return false;
    }
    if (!record.faery_script.matches(character, ai)) {
        error = "AISFaery session is not owned by this Character Record and AI component";
        return false;
    }
    return true;
}

template<class Callback>
Status dispatch(factory::Record& record, std::string& error,
                Callback&& callback) {
    error.clear();
    if (!same_record(record, error)) return Status::identity_mismatch;
    const auto status = callback(record.faery_script, error);
    return status == session::Status::complete
        ? Status::complete : Status::session_rejected;
}

} // namespace

Status construct(factory::Record& record, const session::Services& services,
                 std::string& error) {
    error.clear();
    Identity character = 0;
    Identity ai = 0;
    if (!canonical_record(record, character, ai)) {
        error = "AISFaery requires a constructed factory Record and canonical Component::ai";
        return Status::incomplete_record;
    }
    return record.faery_script.construct(character, ai, services, error) ==
            session::Status::complete
        ? Status::complete : Status::session_rejected;
}

Status bind_functions(factory::Record& record, std::string& error) {
    return dispatch(record, error, [](auto& owner, auto& out) {
        return owner.bind_functions(out);
    });
}

Status bind_character(factory::Record& record, std::string& error) {
    return dispatch(record, error, [](auto& owner, auto& out) {
        return owner.bind_character(out);
    });
}

Status load_common(factory::Record& record, std::string& error) {
    return dispatch(record, error, [](auto& owner, auto& out) {
        return owner.load_common(out);
    });
}

Status initialize(factory::Record& record, std::string& error) {
    return dispatch(record, error, [](auto& owner, auto& out) {
        return owner.initialize(out);
    });
}

Status init_post(factory::Record& record, std::string& error) {
    return dispatch(record, error, [](auto& owner, auto& out) {
        return owner.init_post(out);
    });
}

Status init_final(factory::Record& record, std::string& error) {
    return dispatch(record, error, [](auto& owner, auto& out) {
        return owner.init_final(out);
    });
}

Status update(factory::Record& record, std::string& error) {
    return dispatch(record, error, [](auto& owner, auto& out) {
        return owner.update(out);
    });
}

Status close(factory::Record& record, std::string& error) {
    error.clear();
    const auto stage = record.faery_script.stage();
    if (stage == session::Stage::empty || stage == session::Stage::closed)
        return Status::complete;
    if (stage == session::Stage::faulted &&
        !record.faery_script.session().constructed) {
        return record.faery_script.close(error) == session::Status::complete
            ? Status::complete : Status::session_rejected;
    }
    if (!same_record(record, error)) return Status::identity_mismatch;
    return record.faery_script.close(error) == session::Status::complete
        ? Status::complete : Status::session_rejected;
}

} // namespace dh2::character_faery_record_session_v1
