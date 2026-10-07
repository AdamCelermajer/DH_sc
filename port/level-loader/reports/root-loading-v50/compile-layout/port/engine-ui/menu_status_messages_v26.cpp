#include "menu_status_messages_v26.hpp"
namespace dh2::ui {
bool MenuStatusMessagesV26::invoke(const char* method,std::int32_t context,std::string& error){
 if(!services_.owner||!services_.hud_root){error="Required same MenuManager HUD-root/status owner";return false;}
 std::uintptr_t hud{};if(!services_.hud_root(hud,error))return false;
 if(!hud)return true; // Actual source42cb8c NULL branch, no invented display.
 if(!services_.invoke){error="Required retained status CachedChar/MenuFX method invocation";return false;}
 // Original calls may synchronously reenter message queries/completion. The
 // actual movie provider owns protected AS scope; no private busy suppression.
 return services_.invoke(hud,method,context,error);
}
bool MenuStatusMessagesV26::enqueue_level_up(const std::string& text,std::string& error){
 return enqueue_status(0,text,0,error);
}
bool MenuStatusMessagesV26::enqueue_status(std::int32_t context,const std::string& text,
 std::uint32_t metadata,std::string& error){
 error.clear();
 if(context!=0){error="Required original positive status enqueue context selector";return false;}
 queues_[0].push_back({text,metadata});
 if(queues_[0].size()==1)return invoke("onStatusMessage",0,error);
 return true;
}
bool MenuStatusMessagesV26::next(std::int32_t context,std::string& text,bool& found,std::string& error)const{
 error.clear();found=false;
 if(context<0||context>=4){error="Status context outside actual four source queues";return false;}
 const auto& queue=queues_[std::size_t(context)];
 if(queue.empty())return true;
 text=queue.front().text;found=true;return true;
}
bool MenuStatusMessagesV26::next_metadata(std::int32_t context,std::uint32_t& metadata,
 bool& found,std::string& error)const{
 error.clear();found=false;
 if(context<0||context>=4){error="Status context outside actual four source queues";return false;}
 const auto& queue=queues_[std::size_t(context)];if(queue.empty())return true;
 metadata=queue.front().metadata18;found=true;return true;
}
bool MenuStatusMessagesV26::stop_status(std::string& error){
 error.clear();
 auto& queue=queues_[0];if(queue.empty())return true;
 queue.pop_front();
 if(skip_method_&&!invoke(skip_method_,0,error))return false;
 if(!queue.empty())return invoke("onStatusMessage",0,error);
 return true;
}
std::size_t MenuStatusMessagesV26::count(std::int32_t context)const noexcept{
 return context>=0&&context<4?queues_[std::size_t(context)].size():0;
}
}
