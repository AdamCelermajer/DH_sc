#pragma once
#include "swf_actionscript_connection.hpp"
namespace dh2::ui {
// Original RenderFX.SetText eligibility and text/variable writes, connected to
// the native GameSWF plain-text formatter. This supplies the manager's
// parse=false domain; source HTML reader/append_text layout parity is separate.
// Called only inside SwfMovie.action_script. A null or non-edit-text receiver
// is the original delivered no-op, not a fabricated fallback text field.
bool renderfx_set_plain_text(SwfAsGraph&,const SwfAsValue& receiver,
                            const char* text,bool parse,std::string&);
}
