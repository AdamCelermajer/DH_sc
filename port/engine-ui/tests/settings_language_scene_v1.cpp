#include "../settings_language_scene_v1.hpp"
#include <algorithm>
#include <iostream>
#include <stdexcept>
#include <vector>
using namespace dh2::ui;
namespace {
struct Calls {std::vector<SettingsSceneOperationV1> operations;};
int invoke(void* raw,const SettingsSceneRequest16V1* request,std::uint32_t* result){
 auto& calls=*static_cast<Calls*>(raw);calls.operations.push_back(request->operation);
 *result=request->operation==SettingsSceneOperationV1::is_game_object?1u:0u;return 0;
}
void check(bool value){if(!value)throw std::runtime_error("Settings language scene source contract mismatch");}
void run(std::uint32_t type,bool expect_refresh){
 SettingsCharacterNode16V1 characters{};characters.next=&characters;
 std::uint8_t localized=1;const std::uint32_t object_type=type;
 SettingsSceneObject24V1 object{0x1234,&object_type,&localized};
 SettingsObjectNode32V1 end{},node{};node.parent=&end;node.object=&object;
 SettingsLanguageScene24V1 scene{&characters,&end,&node};Calls calls;SettingsSceneServices16V1 services{&calls,invoke};
 check(dh2_settings_v1_refresh_language_scene(&scene,&services)==0);
 check(localized==(type==14?0:1));
 const bool refreshed=std::find(calls.operations.begin(),calls.operations.end(),SettingsSceneOperationV1::refresh_item)!=calls.operations.end();
 check(refreshed==expect_refresh);
}
}
int main(){try{run(14,false);run(3,true);std::cout<<"{\"validation\":\"PASS\",\"type14_byte_cleared\":true,\"type3_refresh_dispatched\":true}\n";return 0;}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
