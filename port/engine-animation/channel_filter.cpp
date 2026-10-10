#include "channel_filter.hpp"
#include <iostream>
#include <mutex>
#include <set>
#include <string>

namespace dh2::animation {

bool skip_channel_without_node(std::int32_t type,const char* target,bool target_is_node,bool dynamic_channels) {
    if(!target)return false;
    if(type==1||type==5||type==10)return false;
    const bool position_axis=type>=2&&type<=4;
    const bool angle=type==9;
    if(dynamic_channels&&(position_axis||angle))return false;
    if(target_is_node)return false;
    static std::mutex mutex;
    static std::set<std::string> logged;
    const std::string key=std::to_string(type)+" "+target;
    std::lock_guard<std::mutex> lock(mutex);
    if(logged.insert(key).second)
        std::cerr<<"Animation channel skipped (target is not a scene node) type="<<type<<" target="<<target<<" (logged once)\n";
    return true;
}

}
