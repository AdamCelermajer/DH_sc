#include "source_master.hpp"
#include "../../../script-runtime/script_object_bridge.h"

namespace dh::foundation::companions {
bool set_source_master(SourceMasterBorrow& b,std::uintptr_t identity,std::string& error) {
    error.clear();if(!b.fields){error="Required SAME original CharAI master fields";return false;}
    auto& f=*b.fields;f.master50=identity;
    if(!identity)return true; // source null retains alive54/sight55
    if(!b.tables||!b.same_character||!b.get_ai_id){error="Required original AI table/owner/ID query after master publication";return false;}
    std::int32_t ai_id{};if(!b.get_ai_id(ai_id,error))return false;
    if(!f.master50||!b.is_dead){error="Required reloaded actual master IsDead receiver";return false;}
    std::uint32_t dead{};if(!b.is_dead(f.master50,dead,error))return false;
    f.master_alive54=static_cast<std::uint8_t>(dead^1u);
    if(!f.master50||!b.target_position){error="Required actual master/owner GetTargetPosition services";return false;}
    std::array<float,3> master{},owner{};
    if(!b.target_position(f.master50,master,error)||!b.target_position(b.same_character,owner,error))return false;
    const auto* props=dh2::data::ai_props(*b.tables,ai_id);
    if(!props){error="Required selected original AI row ViewRadius";return false;}
    volatile float dx=master[0]-owner[0],dy=master[1]-owner[1],dz=master[2]-owner[2];
    volatile float xx=dx*dx,yy=dy*dy,zz=dz*dz,xy=xx+yy,distance=xy+zz;
    volatile float radius_squared=props->view_radius*props->view_radius;
    f.master_sight55=0;
    if(radius_squared>distance)f.master_sight55=1;
    return true;
}
bool source_has_master(const SourceMasterBorrow& b,bool& out,std::string& error) {
    error.clear();
    if(!b.fields){error="Required SAME source master field";return false;}
    out=b.fields->master50!=0;return true;
}
bool source_is_master_host(const SourceMasterBorrow& b,bool& out,std::string& error) {
    error.clear();
    if(!b.fields){error="Required SAME source master field";return false;}
    if(!b.fields->master50){out=false;return true;}
    if(!b.hosting_player_character){error="Required actual PlayerManager hosting Character producer";return false;}
    std::uintptr_t host{};if(!b.hosting_player_character(host,error))return false;
    out=host==b.fields->master50;return true;
}
bool source_set_master_values(SourceMasterBorrow& b,const dh2_script_value* args,std::uint32_t count,std::string& error) {
    error.clear();
    if(count&&!args){error="Invalid source SetMaster argument storage";return false;}
    if(!count||(args[0].type!=DH2_SCRIPT_IDENTITY&&args[0].type!=DH2_SCRIPT_SOURCE_OBJECT))return true;
    return set_source_master(b,args[0].identity,error);
}
}
