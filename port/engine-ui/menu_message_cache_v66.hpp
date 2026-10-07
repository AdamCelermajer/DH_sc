#pragma once
#include "swf_actionscript_connection.hpp"
#include <array>
#include <cstdint>
#include <memory>
#include <string>
namespace dh2::ui {
class SwfMovie;
// Source static DebugCachedCharacter receivers, not roots held by AS values.
// Weak graph/proxy ownership lets an unloaded HUD die before the next lookup.
class DebugCachedCharacterV66 {
 struct State;std::unique_ptr<State> state_;
public:
 DebugCachedCharacterV66();~DebugCachedCharacterV66();
 DebugCachedCharacterV66(const DebugCachedCharacterV66&)=delete;
 DebugCachedCharacterV66& operator=(const DebugCachedCharacterV66&)=delete;
 // RefreshCache(character=NULL,...)427c44 is an immediate return, including
 // no render/path/cache mutation. Stage26 uses this exact branch five times.
 void refresh_null_character() noexcept;
 bool refresh_character(SwfAsGraph&,const SwfAsValue&,std::uintptr_t render,
                        const SwfAsValue* parent,std::string&);
 bool refresh_name(SwfAsGraph&,const char*,std::uintptr_t render,
                   const SwfAsValue* parent,std::string&);
 bool alive_in(SwfAsGraph&,bool&,std::string&);
 bool get_character(SwfAsGraph&,SwfAsValue&,std::string&);
 const std::string& path()const noexcept;
 std::uint32_t changed_count()const noexcept;
};
enum class MessageFamilyV66:std::uint8_t {status,online,dialog,achievement,tutorial};
class MenuMessageCachesV66 {
 std::array<DebugCachedCharacterV66,5> caches_;
public:
 DebugCachedCharacterV66& cache(MessageFamilyV66 f) noexcept{return caches_[static_cast<unsigned>(f)];}
 // Source3f6fd4..3f7030 order, preserving the repeated Dialog receiver.
 void level_stage26_refresh_null() noexcept;
 // Exact source node/start symbols: all four cache '_root'; start methods
 // remain family-specific. Invocation refreshes only a dead/absent cache.
 bool invoke(SwfMovie&,MessageFamilyV66,std::uintptr_t expected_root,
             const char* actual_method,std::int32_t context,std::string&);
 static const char* start_method(MessageFamilyV66) noexcept;
};
}
