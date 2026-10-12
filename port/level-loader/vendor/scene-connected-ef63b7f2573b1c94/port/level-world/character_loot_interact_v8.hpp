#pragma once
#include "character_loot_item_manager_v8.hpp"
#include "../game-data/properties.hpp"
namespace dh2::character {
// Source ItemObjectC1 3ec324 stores. These are the actual retained fields;
// they must be borrowed by pool/scene/interaction, never mirrored per callback.
struct LootItemFieldsV8 {
 std::int16_t category3ac{-1},audio_drop3b4{-1},audio_pickup3b6{-1},lock3b8{-1},player_id3c0{-1};
 float speed3b0{2.5f};std::uint8_t enabled85{1},base2ee{};
 std::uintptr_t owner3bc{},tooltip_character3c4{},tooltip3c8{},glow3cc{};
};
struct LootInteractorBorrowV8 {
 std::uintptr_t identity{};data::PropertyView* properties{};
 std::uintptr_t* current_target14a4{};
};
enum class LootInteractOperationV8:std::uint32_t {
 character_cast=0x33ff54,player_id=0x36eea8,is_player=0x28,
 saved_option=0x320e44,inventory_full=0x3fe330,num_potions=0x3fc690,
 potion_capacity=0x3a8,font_color=0x3fa6cc,local_player_character=0x36e478,
 online=0x7fd794,game_difficulty=0x3bb8e4,tutorial_enabled=0x2a,
 tutorial_id=0x4591f0,start_tutorial=0x4605c0,show_text=0x3ecff8,
 localized_text=0x508edc,transfer_all=0x3ffa68,
 transmute_preview=0x3a4a3c,transmute_format=0x508ef4,
 transfer_one=0x3ffa44,transmute_index=0x3a4bec,
 is_local_player=0x36effc,unlock_trophy=0x3813b8,
 controller_networked=0x378,message=0x80e2a4,
 pickup_sound=0x36b5d8,tooltip_destroy=0x498dec,hide_glow=0x3ebccc,
 loot_fx=0x495d14,despawn=0x3eac34,increment_stat=0x3790ec
};
struct LootInteractRequestV8 {
 LootInteractOperationV8 operation;std::uintptr_t object{},character{};
 data::LootTemporaryInventoryV8* inventory{};data::ItemInstanceV1* item{};
 const char* key{};const char* text{};std::int32_t argument{};bool flag{};
 std::int32_t secondary{};
};
struct LootInteractResponseV8 {std::int32_t value{};std::uintptr_t identity{};std::string text;LootInteractorBorrowV8 character;};
struct LootInteractServicesV8 {
 void* context{};
 // Each reached original receiver is mandatory. Text uses actual constants,
 // font rows/localization and platform presentation. Transfer/Transmute must
 // use the SAME live Gear inventory and actual source mutation/quest effects.
 bool(*invoke)(void*,const LootInteractRequestV8&,LootInteractResponseV8&,std::string&){};
 data::LootEntryServicesV8 debug;
};
class CharacterLootInteractV8 {
 std::uintptr_t object_;LootItemFieldsV8& fields_;
 data::LootTemporaryInventoryV8& inventory_;LootInteractServicesV8 services_;
 bool running_{};
 bool call(LootInteractOperationV8,std::uintptr_t,data::ItemInstanceV1*,const char*,const char*,std::int32_t,bool,LootInteractResponseV8&,std::string&,std::int32_t secondary=0);
public:
 CharacterLootInteractV8(std::uintptr_t object,LootItemFieldsV8& f,data::LootTemporaryInventoryV8& i,LootInteractServicesV8 s):object_(object),fields_(f),inventory_(i),services_(s){}
 // Source Interact3ed144 complete branch/order coordinator. Production
 // continuation providers remain explicit; failed storage/presentation/quest/
 // trophy/network/FX prefixes remain in the actual borrowed graph.
 bool interact(std::uintptr_t user,std::string&);
};
}
