#include "status_message_owner_v23.hpp"
namespace dh2::ui {
bool StatusMessageOwnerV23::local_query(std::string& e){
 player::PlayerInfoFieldsV1* p{};
 // Source query result is discarded, including the genuine dummy record.
 return players_.get_local_player(0,false,p,e);
}
bool StatusMessageOwnerV23::invoke_start(std::string& e){
 if(!services_.provider_lease||!services_.current_movie){e="Required retained StatusMsg Flash current movie";return false;}
 std::uintptr_t movie{};if(!services_.current_movie(services_.context,movie,e))return false;
 if(!movie)return true; // exact original missing movie early return, queue stays
 if(!services_.invoke){e="Required SAME movie _root.onStatusMessage";return false;}
 return services_.invoke(services_.context,movie,"_root","onStatusMessage",0.0,e);
}
bool StatusMessageOwnerV23::enqueue(const char* text,std::uint32_t meta,std::string& e){
 if(!text){e="Required source StatusMsg text";return false;}
 messages_.push_back({text,meta});
 // Publish before movie callback, preserving reentry and failed prefix.
 return messages_.size()!=1||invoke_start(e);
}
bool StatusMessageOwnerV23::peek(double selector,std::string& text,std::string& e){
 if(selector!=0.0){e="NativeGetNextStatusMessage requires numeric selector zero";return false;}
 if(!local_query(e))return false;text=messages_.empty()?std::string{}:messages_.front().text;return true;
}
bool StatusMessageOwnerV23::stop_status(std::string& e){
 if(!local_query(e))return false;
 if(!messages_.empty())messages_.pop_front();
 return messages_.empty()||invoke_start(e);
}
}
