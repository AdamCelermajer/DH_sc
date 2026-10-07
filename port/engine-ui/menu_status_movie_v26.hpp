#pragma once
#include "menu_status_messages_v26.hpp"
#include "swf_movie.hpp"
namespace gameswf {struct fn_call;}
namespace dh2::ui {
// Concrete protected invocation on the SAME authored HUD graph. Source root
// '_root'/method/context are retained AS values, not JSON or replacement UI.
bool menu_status_invoke_movie_v26(SwfMovie&,const char*,std::int32_t,std::string&);
// Exact affected native transport. Other NativeStopMessage categories are
// reported unhandled for their own source family owners, never accepted here.
bool menu_status_native_v26(MenuStatusMessagesV26&,const char*,const gameswf::fn_call&,
                           bool& handled,std::string&);
}
