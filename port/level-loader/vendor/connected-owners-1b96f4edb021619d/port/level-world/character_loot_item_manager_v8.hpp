#pragma once
#include "../game-data/loot_audiovisual_v8.hpp"
#include "../game-data/loot_temporary_inventory_v8.hpp"
namespace dh2::character {
enum class LootItemOperationV8:std::uint32_t {
 init_once=0x3ece80,enable=0x393578,remove_all=0x3fe6bc,
 physical=0x394bf8,position=0x393db4,destination=0x393600,
 init_again=0x3ec0f0
};
struct LootItemObjectBorrowV8 {
 std::uintptr_t identity{};std::int32_t type_index{};
 std::int16_t* category3ac{};std::uint8_t* enabled85{};
 // Root retains this exact item object's World/script/scene/body graph.
};
struct LootItemRequestV8 {
 LootItemOperationV8 operation;LootItemObjectBorrowV8* object{};
 const data::LootAudioVisualRowV8* audiovisual{};
 data::LootTemporaryInventoryV8* inventory{};
 std::uint32_t index{};std::uintptr_t character{};
 const float* vector{};bool flag{};
};
struct LootItemServicesV8 {
 void* context{};
 // Original ObjectManager.Spawn("Item",name,false,true), followed by its
 // actual handle resolution and ItemObject type3 check. Must return the same
 // published World object, with stable source field borrows.
 bool(*spawn_object)(void*,const char* type,const char* name,bool,bool,
                     LootItemObjectBorrowV8&,std::string&){};
 bool(*invoke)(void*,const LootItemRequestV8&,std::string&){};
};
// Original ItemManagerC1, PreCache five objects per audiovisual category,
// Spawn round-robin reuse and DeSpawn ordered lifecycle. This owns only source
// CategoryInfo/ObjectInfo/cursor fields; actual object graphs remain borrowed.
class CharacterLootItemManagerV8 {
 struct Slot {LootItemObjectBorrowV8 object;bool active{};};
 struct Category {std::array<Slot,5> slots;std::uint32_t cursor{};};
 data::LootAudioVisualV8::Borrow audiovisual_;
 LootItemServicesV8 services_;std::vector<Category> categories_;
 bool running_{},attempted_{},ready_{};
 bool invoke(const LootItemRequestV8&,std::string&);
 bool despawn(Slot&,std::string&);
public:
 CharacterLootItemManagerV8(data::LootAudioVisualV8::Borrow a,LootItemServicesV8 s):audiovisual_(std::move(a)),services_(s){}
 bool precache(std::string&);
 bool spawn(data::LootTemporaryInventoryV8&,std::uint32_t,std::uintptr_t source,
            const float source_position[3],const float destination[3],
            std::uintptr_t local_player_character,LootItemObjectBorrowV8&,std::string&);
 bool despawn(std::uintptr_t,std::string&);
 bool flush(std::string&);
 bool ready()const noexcept{return ready_;}
};
}
