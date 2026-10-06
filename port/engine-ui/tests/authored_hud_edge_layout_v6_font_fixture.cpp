#include "../swf_text_font_platform_v1.hpp"
// This fixture deliberately installs no text/font platform. HUD vector shapes,
// graph actions and pointer picking remain the actual cached GameSWF path.
namespace dh2::ui {
std::shared_ptr<SwfTextFontPlatformV1> SwfTextFontPlatformV1::for_player(gameswf::player*){return {};}
bool SwfTextFontPlatformV1::flush_buffered_text(std::string& error){error="Font platform is outside HUD edge fixture";return false;}
}
