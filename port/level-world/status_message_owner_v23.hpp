#pragma once
#include "player_manager_owner_v1.hpp"
#include <deque>
namespace dh2::ui {
struct StatusMessageMovieServicesV23 {
 std::shared_ptr<void> provider_lease;
 void* context{};
 // Flash.GetInstance/GetMovie: NULL is a genuine source early return.
 bool(*current_movie)(void*,std::uintptr_t&,std::string&){};
 // SAME scoped movie, source root receiver and one numeric argument 0.
 bool(*invoke)(void*,std::uintptr_t,const char*,const char*,double,std::string&){};
};
struct StatusMessageV23 {std::string text;std::uint32_t metadata18{};};
// Source MenuMessageManager<StatusMsg,4>: 4 is the message KIND, not
// a queue capacity. Text is owned exactly as the original deque copy.
class StatusMessageOwnerV23 {
 player::PlayerManagerOwnerV1& players_;
 StatusMessageMovieServicesV23 services_;
 std::deque<StatusMessageV23> messages_;
 bool invoke_start(std::string&);
 bool local_query(std::string&);
public:
 StatusMessageOwnerV23(player::PlayerManagerOwnerV1& p,StatusMessageMovieServicesV23 s)
  :players_(p),services_(std::move(s)){}
 bool enqueue(const char*,std::uint32_t,std::string&);
 // NativeGetNextStatusMessage43fec4 validates arguments in the AS bridge;
 // source numeric selector must be 0. Query does NOT pop the front.
 bool peek(double actual_selector,std::string& text,std::string& error);
 // NativeStopMessage442ea4 branch for actual kind "status" only. Source
 // s_SkipFuncName is NULL BSS, so this domain has no skip call to invent.
 bool stop_status(std::string&);
 void flush()noexcept{messages_.clear();} // original FlushEnqueuedMessages
 std::size_t pending()const noexcept{return messages_.size();}
 const StatusMessageV23* front()const noexcept{return messages_.empty()?nullptr:&messages_.front();}
};
}
