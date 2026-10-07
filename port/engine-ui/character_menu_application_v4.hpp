#pragma once
#include "authored_menu_application_fields_v3.hpp"
#include "character_menu_queries_owner_v1.hpp"
#include <deque>
#include <memory>
#include <optional>
namespace gameswf {struct fn_call;}
namespace dh2::ui {
struct CharacterMenuTutorialMessageV4 {
 std::int32_t string_id{-1};std::string frame,menu;
};
// Sole logical source Singleton<MenuMessageManager<CharMenuTutorialMsg,1>>.
// Global initializer434afc..434bc4 constructs the empty deque; the separate
// CharMenuTutorialMsg::s_SkipFuncName is BSS-null until its genuine producer
// publishes a function name; it is not a movie/receiver identity.
class CharacterMenuTutorialQueueV4 {
 std::deque<CharacterMenuTutorialMessageV4> messages_;
 std::optional<std::string> skip_function_;
public:
 bool empty()const noexcept{return messages_.empty();}
 std::size_t size()const noexcept{return messages_.size();}
 const CharacterMenuTutorialMessageV4* first()const noexcept{return empty()?nullptr:&messages_.front();}
 const char* skip_function()const noexcept{return skip_function_?skip_function_->c_str():nullptr;}
 // Whole Script_EnqueueCharMenuTutorialMessage460b1c logical copy, borrowing
 // actual command+18 stringID, +0c frame and +14 menu producers.
 bool enqueue_script_command(std::int32_t,const char* actual_frame,const char* actual_menu,std::string&);
 // Source native Skip pops first before invoking the retained receiver.
 bool skip(const std::function<bool(const char*,std::string&)>&,std::string&);
 void flush()noexcept{while(!messages_.empty())messages_.pop_front();}
 // Called only by the actual static-name producer, not inferred menu policy.
 void publish_source_skip_function(const char* actual){if(actual)skip_function_=actual;else skip_function_.reset();}
};
struct CharacterMenuApplicationServicesV4 {
 std::shared_ptr<void> owner;
 CharacterMenuTutorialQueueV4* tutorial{};
 AuthoredMenuApplicationFieldsV3* application{};
 std::function<bool(std::int32_t,std::string&,std::string&)> localized;
 std::function<bool(const char* actual_skip_function,std::string&)> invoke_tutorial;
};
// Call while the actual fn_call's owning GameSWF Scope is active. Result and
// passed receiver tags stay native; no JSON/flattened result conversion.
bool character_menu_application_native_v4(const char* name,const gameswf::fn_call&,
 const CharacterMenuApplicationServicesV4&,bool& handled,std::string&);
bool character_menu_application_call_v4(const char* name,CharacterMenuCallV1&,
 const CharacterMenuApplicationServicesV4&,bool& handled,std::string&);
}
