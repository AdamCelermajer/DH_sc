#pragma once

#include "character_faery_script_session_v1.hpp"
#include "character_runtime_factory_v1.hpp"

namespace dh2::character_faery_record_session_v1 {

namespace session = character_faery_script_session_v1;
namespace factory = character_runtime_factory_v1;

enum class Status : unsigned char {
    complete,
    incomplete_record,
    identity_mismatch,
    session_rejected,
};

// Binds lifecycle calls to the AISFaery owner stored inside the exact factory
// Record. This is not the missing Android factory/ScriptManager provider: the
// supplied source service must still construct the real AIS and Lua Instance.
Status construct(factory::Record&, const session::Services&, std::string&);
Status bind_functions(factory::Record&, std::string&);
Status bind_character(factory::Record&, std::string&);
Status load_common(factory::Record&, std::string&);
Status initialize(factory::Record&, std::string&);
Status init_post(factory::Record&, std::string&);
Status init_final(factory::Record&, std::string&);
Status update(factory::Record&, std::string&);
Status close(factory::Record&, std::string&);

} // namespace dh2::character_faery_record_session_v1
