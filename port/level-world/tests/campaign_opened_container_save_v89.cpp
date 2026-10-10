#include "../../level-loader/noncharacter_save_connection_v89.hpp"
#include <cassert>
#include <iostream>

using namespace dh2;

namespace {
struct RestoreTrace {
 std::int32_t state{2},archetype{41};
 std::uint8_t visible{1},enabled{1},tested_a{},tested_d{};
 std::uintptr_t visual{0x1234};
 bool visible_produced{true};std::uint8_t level_f3{},level_f4{};
 unsigned syncs{},state_sets{},detaches{},animations{};
};

level::SavegameStreamV2 save_container(RestoreTrace& trace,std::string& error){
 loader::NonCharacterSaveFieldsV89 fields;fields.kind=loader::NonCharacterSaveKindV89::container;
 fields.base={77,&trace.visible,&trace.enabled,&trace.tested_a,&trace.tested_d,&trace.archetype,&trace.visual,&trace.visible_produced};
 fields.container_state394=[&](std::int32_t& out,std::string&){out=trace.state;return true;};
 fields.death_reset390=[](bool& out,std::string&){out=false;return true;};
 level::SavegameStreamV2 stream;assert(loader::noncharacter_serialize_v89(stream,fields,error));return stream;
}
}

int main(){
 std::string error;RestoreTrace saved;saved.state=4;
 auto stream=save_container(saved,error);
 assert(stream.size()==7&&stream.bytes().back()==4);

 RestoreTrace restored;loader::NonCharacterSaveFieldsV89 fields;fields.kind=loader::NonCharacterSaveKindV89::container;
 fields.base={77,&restored.visible,&restored.enabled,&restored.tested_a,&restored.tested_d,&restored.archetype,&restored.visual,&restored.visible_produced};
 fields.container_state394=[&](std::int32_t& out,std::string&){out=restored.state;return true;};
 fields.death_reset390=[](bool& out,std::string&){out=false;return true;};
 loader::NonCharacterRestoreServicesV89 services;services.owner=std::make_shared<int>(1);
 services.gameobject.invoke=[](void* raw,const auto& request,std::string&){auto& t=*static_cast<RestoreTrace*>(raw);assert(request.entry==0x4713d0&&request.subject==t.visual&&request.payload==77);++t.syncs;return true;};
 services.gameobject.context=&restored;
 services.level_flags=[&](auto& out,std::string&){out.owner=services.owner;out.bytef3=&restored.level_f3;out.bytef4=&restored.level_f4;return true;};
 services.container_set_state=[&](std::uint8_t value,std::string&){restored.state=value;++restored.state_sets;return true;};
 services.container_keep_physics=[](bool& value,std::string&){value=false;return true;};
 services.set_physical_null_false=[&](std::string&){++restored.detaches;return true;};
 services.play_animation=[&](std::uintptr_t visual,const char* name,unsigned a,unsigned b,unsigned c,bool& result,std::string&){assert(visual==restored.visual&&std::string(name)=="idleactive"&&a==0&&b==0&&c==0);result=false;++restored.animations;return true;};
 assert(loader::noncharacter_deserialize_v89(stream,fields,services,error));
 assert(restored.state==4&&restored.syncs==1&&restored.state_sets==1&&restored.detaches==1&&restored.animations==1);
 std::cout<<"Campaign OBJS opened-container byte state4 save/reload and source SetState/physics/idleactive route PASS 6 checks; native receiver leaves explicit fixtures\n";
}
