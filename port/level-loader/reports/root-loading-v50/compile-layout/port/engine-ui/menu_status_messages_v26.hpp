#pragma once
#include <array>
#include <deque>
#include <functional>
#include <memory>
#include <string>
#include <cstdint>
namespace dh2::ui {
struct MenuStatusServicesV26 {
 std::shared_ptr<void> owner;
 // Actual MenuManager.GetHUDRoot42cb8c; NULL is an original completed branch.
 std::function<bool(std::uintptr_t&,std::string&)> hud_root;
 // Same StatusMsg.s_NodeCachedChar + protected real MenuFX Invoke. Must
 // refresh/get the actual '_root' and call the supplied authored method.
 std::function<bool(std::uintptr_t,const char* method,std::int32_t context,std::string&)> invoke;
};
// ONE source singleton99fa8c per actual Application service lifetime, shared
// by LevelUp/quests/HUD. Four constructor-empty source message deques. This
// owns text rather than a second localization cache or fake visible labels.
class MenuStatusMessagesV26 {
 struct Message {std::string text;std::uint32_t metadata18{};};
 std::array<std::deque<Message>,4> queues_;
 MenuStatusServicesV26 services_;
 // Original static9a53e0 is BSS NULL; later genuine producer can bind it.
 const char* skip_method_{};
 bool invoke(const char*,std::int32_t,std::string&);
public:
 explicit MenuStatusMessagesV26(MenuStatusServicesV26 s):services_(std::move(s)){}
 // LevelUp3bed78 is direct push_back context0, preserving its failure prefix.
 bool enqueue_level_up(const std::string& actual_localized_MENU_LEVEL_UP,std::string&);
 // Proven actual pickup3ecff8/LevelUp context0 specialization. Other source
 // context enqueue branches require their original selector before binding.
 bool enqueue_status(std::int32_t actual_context,const std::string&,
                     std::uint32_t actual_metadata18,std::string&);
 // NativeGetNextStatusMessage43fec4: inspect, do not pop. 'found=false'
 // corresponds to untouched/undefined AS return, not a successful empty font.
 bool next(std::int32_t context,std::string& actual_text,bool& found,std::string&)const;
 bool next_metadata(std::int32_t context,std::uint32_t&,bool& found,std::string&)const;
 // Status branch NativeStopMessage443228: pop→optional skip→next start.
 // This method is called only AFTER the AS transport matched original status
 // category; other message families remain their genuine owners' authority.
 bool stop_status(std::string&);
 std::size_t count(std::int32_t context)const noexcept;
 void bind_actual_skip_method(const char* p)noexcept{skip_method_=p;}
};
}
