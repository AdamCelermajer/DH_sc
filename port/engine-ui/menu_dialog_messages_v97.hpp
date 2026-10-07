#pragma once
#include <cstdint>
#include <deque>
#include <functional>
#include <memory>
#include <string>
#include <vector>
namespace gameswf {struct fn_call;}
namespace dh2::ui {
struct DialogActorRowV97 {std::int32_t frame_id4{},name10{};std::string frame_label8,name;};
class DialogActorsV97 {
 std::vector<DialogActorRowV97> rows_;bool initialized_{};
public:
 //Actual dialogs PyArray FIRST table. Its suffix is a distinct DialogList
 //table, not additional actor rows. Native row stride20 has a CString at8.
 bool decode(const std::vector<std::uint8_t>& records,const std::vector<std::uint8_t>& names,
             const std::vector<std::uint8_t>& schema,std::string&);
 const DialogActorRowV97* row(std::int32_t)const noexcept;
 bool initialized()const noexcept{return initialized_;}
};
struct DialogMsgV97 {
 std::string title0,message18,actor34;
 std::int32_t style30{-1};
};
struct MenuDialogServicesV97 {
 std::shared_ptr<void> owner;
 std::function<bool(std::int32_t,std::string&,std::string&)> localized;
 std::function<bool(std::int32_t,std::int32_t&,std::string&)> actor_text_id;
 std::function<bool(std::int32_t,std::string&,std::string&)> style_name;
 std::function<bool(const char*,std::int32_t,std::string&)> invoke;
 const char* skip_method{}; //original static BSSNULL, not a guessed callback
 const char* start_method{"StartDialog"};
};
class MenuDialogMessagesV97 {
 std::deque<DialogMsgV97> queue_;MenuDialogServicesV97 services_;
public:
 explicit MenuDialogMessagesV97(MenuDialogServicesV97);
 bool construct(std::int32_t first,std::int32_t text,std::int32_t style,std::int32_t actor,DialogMsgV97&,std::string&);
 bool construct_strings_v108(const std::string&,const std::string&,std::int32_t style,std::int32_t actor,DialogMsgV97&,std::string&);
 bool enqueue(const DialogMsgV97&,std::int32_t context,bool start,std::string&);
 bool next(DialogMsgV97&,bool&,std::string&)const;
 bool style_name(std::int32_t,std::string&,std::string&)const;
 bool skip(std::string&);
 void flush()noexcept;
 bool active()const noexcept{return !queue_.empty();}
 std::size_t count()const noexcept{return queue_.size();}
};
//Distinct source MenuMessageManager<AchievementMsg,1>/<OnlineStatusMsg,1>
//singletons. These own real queued native values, not a second dialog/status
//queue or fabricated "active" booleans. Native caller supplies all data words.
struct AchievementMsgV97 {std::string title0,message18;std::int32_t field30,field34,field38;};
struct OnlineStatusMsgV97 {std::string text0;std::uint32_t metadata18;};
//Original TutorialMsg is its own trivial eight-byte queue value. Neither
//CharacterMenuTutorial nor AchievementMsg owns this singleton's storage.
struct TutorialMsgV107 {std::int32_t word0,word4;};
class MenuAuxiliaryMessagesV97 {
 std::deque<AchievementMsgV97> achievements_;
 std::deque<OnlineStatusMsgV97> online_;
 std::deque<TutorialMsgV107> tutorials_;
public:
 void enqueue_achievement(const AchievementMsgV97& value){achievements_.push_back(value);}
 std::size_t achievement_count()const noexcept{return achievements_.size();}
 const AchievementMsgV97* next_achievement()const noexcept{return achievements_.empty()?nullptr:&achievements_.front();}
 void skip_achievement()noexcept{if(!achievements_.empty())achievements_.pop_front();}
 void enqueue_online(const OnlineStatusMsgV97& value){online_.push_back(value);}
 void enqueue_tutorial(const TutorialMsgV107& value){tutorials_.push_back(value);}
 std::size_t tutorial_count()const noexcept{return tutorials_.size();}
 const TutorialMsgV107* next_tutorial()const noexcept{return tutorials_.empty()?nullptr:&tutorials_.front();}
 void skip_tutorial()noexcept{if(!tutorials_.empty())tutorials_.pop_front();}
 void flush_tutorial()noexcept{while(!tutorials_.empty())tutorials_.pop_front();}
 void flush_achievements()noexcept{while(!achievements_.empty())achievements_.pop_front();}
 void flush_online()noexcept{while(!online_.empty())online_.pop_front();}
 bool online_empty()const noexcept{return online_.empty();}
 void skip_online()noexcept{if(!online_.empty())online_.pop_front();}
 const OnlineStatusMsgV97* next_online()const noexcept{return online_.empty()?nullptr:&online_.front();}
};
//Source44ff14: populate the actual argument OBJECT, then return foundBool.
//NativeStopMessage's player/network prelude is provided by the application.
bool menu_dialog_native_v97(MenuDialogMessagesV97&,const char*,const gameswf::fn_call&,bool& handled,std::string&);
}
