#include "openable_container_owner_v1.hpp"
#include <cstring>
#include <utility>
namespace dh2::world {
namespace {
template<class F,class... A> bool call(const F& f,const char* name,std::string& e,A&&... a) {
    if(!f){e=std::string("OpenableContainer required source service: ")+name;return false;}
    return f(std::forward<A>(a)...,e);
}
}
bool read_openable_container_record_v1(const std::uint8_t* p,std::size_t n,
 OpenableContainerRowV1& out,std::size_t& used,std::string& e){
    used=0; if(!p){e="null OpenableContainer record";return false;}
    auto word=[&](std::int32_t& v){if(n-used<4)return false;std::uint32_t u=0;
        for(unsigned i=0;i<4;++i)u|=std::uint32_t(p[used+i])<<(i*8);
        std::memcpy(&v,&u,4);used+=4;return true;};
    OpenableContainerRowV1 r;std::int32_t len=0;
    if(!word(r.field4)||!word(r.sound)||used==n){e="short OpenableContainer prefix";return false;}
    r.keep_physics=p[used++]!=0;
    if(!word(r.loot)||!word(len)||len<0||std::size_t(len)>n-used){e="invalid OpenableContainer script length";return false;}
    r.script.assign(reinterpret_cast<const char*>(p+used),std::size_t(len));used+=std::size_t(len);
    if(!word(r.field1c)||!word(r.field20)||!word(r.visual)){e="short OpenableContainer tail";return false;}
    out=std::move(r);return true;
}
OpenableContainerOwnerV1::OpenableContainerOwnerV1(OpenableContainerFieldsV1& f,OpenableContainerServicesV1 s):f_(f),s_(std::move(s)){}
bool OpenableContainerTableV1::load(const std::uint8_t* p,std::size_t n,const std::uint8_t* names,std::size_t nn,std::string& e){
    auto u=[](const std::uint8_t* b){return std::uint32_t(b[0])|(std::uint32_t(b[1])<<8)|(std::uint32_t(b[2])<<16)|(std::uint32_t(b[3])<<24);};
    if(!p||!names||n<4||nn<4){e="short OpenableContainers group";return false;}
    auto count=u(p);if(count!=u(names)||count>(n-4)/29){e="OpenableContainers count mismatch";return false;}
    std::vector<OpenableContainerRowV1> rows;std::vector<std::string> keys;std::size_t pos=4,np=4;
    for(std::uint32_t i=0;i<count;++i){OpenableContainerRowV1 r;std::size_t z=0;
        if(!read_openable_container_record_v1(p+pos,n-pos,r,z,e))return false;
        pos+=z;rows.push_back(std::move(r));
        if(nn-np<4){e="short OpenableContainers names";return false;}
        auto len=u(names+np);np+=4;if(len>nn-np){e="invalid OpenableContainers name length";return false;}
        keys.emplace_back(reinterpret_cast<const char*>(names+np),len);np+=len;
    }
    if(pos!=n||np!=nn){e="unexpected OpenableContainers group tail";return false;}
    rows_=std::move(rows);names_=std::move(keys);return true;
}
bool OpenableContainerTableV1::resolve(const std::string& name,std::int32_t& id,OpenableContainerRowV1& row,std::string&) const{
    id=-1;for(std::size_t i=0;i<names_.size();++i)if(names_[i]==name){id=static_cast<std::int32_t>(i);row=rows_[i];return true;}
    row=OpenableContainerRowV1{};return true;
}
bool OpenableContainerOwnerV1::row(OpenableContainerRowV1& r,std::string& e){
    std::int32_t id=-1;return call(s_.resolve_row,"Arrays::OpenableContainers",e,f_.data_desc,id,r);
}
bool OpenableContainerOwnerV1::set_state(std::int32_t state,std::string& e){
    // Source SetState dereferences the actual visual scene. Mutation precedes state store.
    if(!call(s_.scene_flags,"Container::SetState scene+11c",e,state==3?0x400u:0u,state==3?0u:0x400u))return false;
    f_.state394=state;return true;
}
bool OpenableContainerOwnerV1::is_locked() const {
    return std::uint32_t(f_.state394)-3u>1u && f_.key_id710!=-1;
}
bool OpenableContainerOwnerV1::is_interactive(bool disabled) const{return !disabled&&f_.state394==2;}
bool OpenableContainerOwnerV1::init_post(std::string& e){
    std::int32_t roll=0,prob=0;
    if(!call(s_.spawn_roll_and_probability,"CheckSpawnProbability",e,roll,prob))return false;
    if(roll<prob){
        OpenableContainerRowV1 r;std::int32_t id=-1;
        if(!call(s_.resolve_row,"GetDataId",e,f_.data_desc,id,r))return false;
        f_.data374=id;
        if(id!=-1&&r.visual!=-1&&!call(s_.visual_asset,"Visuals row asset",e,r.visual))return false;
        if(!call(s_.game_object_init_post,"GameObject::InitPost",e))return false;
        bool condition=false;if(!call(s_.meet_condition,"MeetCondition",e,condition))return false;
        if(!condition){if(!call(s_.disable_object,"Disable",e)||!set_state(4,e))return false;}
        else {
            bool visual=false;if(!call(s_.has_visual,"visual+2d8",e,visual))return false;
            if(visual){
                if(!call(s_.bind_timeline_callbacks,"timeline callbacks",e))return false;
                bool played=false;
                if(!call(s_.play_animation,"prespawn",e,"prespawn",played))return false;
                if(played){if(!set_state(0,e))return false;}
                else {if(!call(s_.play_animation,"spawn",e,"spawn",played))return false;
                    if(played){if(!set_state(1,e))return false;}
                    else {if(!call(s_.play_animation,"idle",e,"idle",played))return false;
                        if(played&&!set_state(2,e))return false;}}
                if(!call(s_.apply_mesh_box,"ApplyMeshBox",e))return false;
            }
            if(!call(s_.precache_complete_source_v42,"complete captured-manager LoadSound prefix",e))return false;
            if(!call(s_.load_object_script,"object script load",e,id==-1?nullptr:r.script.c_str(),"data/scripts/objects/"))return false;
        }
    }
    // Derived InitPost executes even when the base source spawn gate returned.
    if(!f_.key_name.empty()&&!call(s_.resolve_item_name,"Items name lookup",e,f_.key_name,f_.key_id710))return false;
    return true;
}
bool OpenableContainerOwnerV1::init_final(std::string& e){
    std::int32_t roll=0,prob=0;if(!call(s_.spawn_roll_and_probability,"CheckSpawnProbability",e,roll,prob))return false;
    if(roll>=prob)return true;
    if(!call(s_.game_object_init_final,"GameObject::InitFinal",e))return false;
    if(std::uint32_t(f_.state394)-3u>1u&&!call(s_.source_on_interact,"GameObject::Update",e))return false;
    bool condition=false;if(!call(s_.meet_condition,"MeetCondition",e,condition))return false;
    return !condition||call(s_.create_attach_po_decor,"PODecor + SetPhysicalObject(false)",e);
}
bool OpenableContainerOwnerV1::spawn(std::string& e){
    bool visual=false;if(!call(s_.has_visual,"visual+2d8",e,visual))return false;
    if(!visual||f_.state394!=0)return true;
    bool played=false;return set_state(1,e)&&call(s_.play_animation,"spawn",e,"spawn",played);
}
bool OpenableContainerOwnerV1::try_unlock(std::uintptr_t actor,bool& unlocked,std::string& e){
    unlocked=false;if(!is_locked()){unlocked=true;return true;}
    bool character=false;if(!call(s_.is_character,"GameObject::IsCharacter",e,actor,character))return false;
    if(!character)return true;
    bool found=false;std::int16_t qty=0;
    if(!call(s_.find_key,"ItemInventory::FindItem",e,actor,f_.key_id710,found,qty))return false;
    if(!found||f_.key_qty>qty||!f_.key_consume)return true;
    return call(s_.remove_key,"ItemInventory::RemoveItemByID",e,actor,f_.key_id710,unlocked);
}
bool OpenableContainerOwnerV1::interact_base(std::uintptr_t actor,std::string& e){
    if(std::uint32_t(f_.state394)-3u<=1u)return true;
    f_.opener398=actor;if(!set_state(4,e))return false;
    OpenableContainerRowV1 r;if(!row(r,e))return false;
    if(!r.keep_physics&&!call(s_.detach_physical,"SetPhysicalObject(null,false)",e))return false;
    bool visual=false;if(!call(s_.has_visual,"visual+2d8",e,visual))return false;
    if(visual){bool played=false;if(!set_state(3,e)||!call(s_.play_animation,"activate",e,"activate",played))return false;}
    else if(!do_open(e))return false;
    if(!call(s_.play_sound_3d,"VoxSoundManager::Play3D",e,r.sound))return false;
    return call(s_.source_on_interact,"GameObject::Update",e);
}
bool OpenableContainerOwnerV1::do_open(std::string& e){
    OpenableContainerRowV1 r;
    if(!row(r,e))return false;
    const auto powers=loot_fixed_num_powers();
    if(!call(s_.drop_loot_table,"ItemObject::DropLootTable",e,r.loot,f_.opener398,powers,false))return false;
    bool script=false;if(!call(s_.has_script,"LuaScript pointer",e,script))return false;
    return !script||call(s_.script_call,"OnOpen",e,"OnOpen",f_.opener398,nullptr);
}
bool OpenableContainerOwnerV1::animation_event(const char* name,std::string& e){
    if(!name){e="null authored container event";return false;}
    if(std::strcmp(name,"opened")==0)return do_open(e);
    bool script=false;if(!call(s_.has_script,"LuaScript pointer",e,script))return false;
    return !script||call(s_.script_call,"OnAnimEvent",e,"OnAnimEvent",std::uintptr_t(0),name);
}
bool OpenableContainerOwnerV1::animation_finished(bool active,std::string& e){
    bool played=false;
    if(f_.state394==1)return set_state(2,e)&&call(s_.play_animation,"idle",e,"idle",played);
    if(f_.state394==3)return set_state(4,e)&&call(s_.play_animation,"idleactive",e,"idleactive",played);
    return active||call(s_.scene_flags,"timeline inactive scene+11c",e,0x200u,0u);
}
}
