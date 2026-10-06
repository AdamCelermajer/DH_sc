#include "../player_manager_combat_runtime_v2.hpp"
#include "../player_network_local_owner_v4.hpp"
#include <iostream>
#include <stdexcept>
#include <cstdarg>
#include <functional>
// Declared renderer/platform shell fixtures. Actual PM/PlayerInfo/Matching
// owners and boot/transfer production include are linked and run unchanged.
constexpr int ANDROID_LOG_ERROR=6;
static int __android_log_print(int,const char*,const char*,...){return 0;}
struct RendererPlayerManagerBootV25;
struct WorldScriptContext {std::shared_ptr<RendererPlayerManagerBootV25> player_manager_boot_v25;};
struct EquipmentPlatform {std::shared_ptr<WorldScriptContext> world;bool online_mode{};};
std::unique_ptr<EquipmentPlatform> equipment_platform;
struct PlayerSkillsRuntime {
 std::shared_ptr<WorldScriptContext> world;
 std::unique_ptr<dh2::player::PlayerNetworkLocalOwnerV4> player_network_local;
 std::unique_ptr<dh2::player::PlayerManagerCombatRuntimeV2> player_manager;
 std::function<void()> player_manager_boot_return_v25;std::string error;
 ~PlayerSkillsRuntime(){if(player_manager_boot_return_v25)player_manager_boot_return_v25();}
};
#include "../../android-native/app/src/main/cpp/renderer_player_manager_boot_v25.inc"
namespace {unsigned checks{};void check(bool v,const std::string& e){++checks;if(!v)throw std::runtime_error(e);}}
int main(){try{
 auto world=std::make_shared<WorldScriptContext>();equipment_platform=std::make_unique<EquipmentPlatform>();equipment_platform->world=world;
 const dh2::player::DevelopmentPlayerLaunchV2 launch{0,0,0,true,0,nullptr};
 prepare_player_manager_boot_v25(world,launch);auto boot=world->player_manager_boot_v25;
 std::string error;std::int32_t count{};check(player_manager_bound_character_count_v25(world,count,error)&&count==0,error);
 auto* manager=boot->actual_manager;auto* network=boot->actual_network;
 std::int32_t registered{};check(boot->manager->manager().num_players(registered,error)&&registered==1,"Source AddPlayer map count differs");
 check(*manager->manager().character_count_field()==0,"No fake bound-character count before source AddCharacter");
 dh2::player::PlayerInfoFieldsV1* record{};check(boot->manager->manager().get_by_internal(0,false,record,error)&&record&&record->character660==0,error);
 auto actual_network=boot->network->borrow(*record);check(actual_network&&actual_network->owner1a0==-1,"Actual CNet Reset identity absent");
 std::weak_ptr<WorldScriptContext> weak=world;
 std::weak_ptr<RendererPlayerManagerBootV25> weak_boot=boot;
 for(unsigned surface=0;surface<3;++surface){
  PlayerSkillsRuntime runtime;runtime.world=world;const std::int16_t base_id=290;
  auto published=launch;published.character=0x100000123ull;published.same_base_id=&base_id;
  transfer_player_manager_boot_v25(runtime,published);
  check(runtime.player_manager.get()==manager&&runtime.player_network_local.get()==network,"Transfer constructed a second source owner");
  check(!boot->manager&&!boot->network&&boot->transferred,"Boot retained duplicate ownership after transfer");
  check(player_manager_bound_character_count_v25(world,count,error)&&count==0,"Transfer invented XP count");
  dh2::player::PlayerInfoFieldsV1* current{};check(runtime.player_manager->manager().get_by_character(published.character,false,current,error)&&current==record&&current->character_base_id13c8==&base_id,error);
  bool local{};check(runtime.player_network_local->is_local(*record,local,error)&&local,"Same CMatching local identity changed");
 }
 check(boot->manager.get()==manager&&boot->network.get()==network&&!boot->transferred,"Destructor failed to return SAME PM/Net owner");
 prepare_player_manager_boot_v25(world,launch);check(boot->actual_manager==manager,"Restore replayed constructor/AddPlayer");
 // Before-Gear failure/World teardown must release the provider without a
 // World→Boot→Network record→World reference cycle.
 actual_network.reset();boot.reset();equipment_platform.reset();world.reset();
 check(weak.expired()&&weak_boot.expired(),"PlayerManager boot ownership cycle");
 std::cout<<"{\"validation\":\"PASS\",\"checks\":"<<checks<<",\"surface_owner_transfers\":3,\"actual_registered_player_count\":1,\"actual_bound_character_count\":0,\"no_constructor_replay_and_no_world_cycle\":true,\"scope\":\"actual PM/Matching owners; declared offline renderer shell; AddCharacter positive continuation not claimed\"}\n";
 return 0;
}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
