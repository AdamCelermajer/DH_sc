#include "merchant_shop_v1.hpp"
#include "../../save_store.hpp"
#include <filesystem>
#include <iostream>
#include <stdexcept>
using namespace dh::foundation;using namespace dh::foundation::merchant;
static void check(bool ok,const char* why){if(!ok)throw std::runtime_error(why);}
static dh2::data::ItemTable table(){dh2::data::ItemTable t;t.identifiers={"WornMallet","Potion0"};t.rows.resize(2);t.rows[0].record.words[27]=1;t.rows[0].record.words[28]=10;t.rows[1].record.words[7]=1;t.rows[1].record.words[22]=14;t.rows[1].record.words[27]=1;t.rows[1].record.words[28]=2;return t;}
int main(){try{auto items=table();Shop shop({40,100,{}});std::vector<InventoryItem> generated;
 InventoryItem cold{"merchant-roll-1","WornMallet",1,SourceItemMetadata{240,1,{"Power_Frost"}}};
 InventoryItem fire{"merchant-roll-2","WornMallet",1,SourceItemMetadata{310,1,{"Power_Fire"}}};
 shop.add_generated_stock(items,{cold,fire});check(shop.inventory().inventory.size()==2,"same-template powered rows collapsed");
 Shop duplicate_nonstack({40,100,{}});duplicate_nonstack.add_generated_stock(items,{cold,cold});
 check(duplicate_nonstack.inventory().inventory.size()==2&&duplicate_nonstack.inventory().inventory[0].instance_id!=duplicate_nonstack.inventory().inventory[1].instance_id,
       "identical nonstackable generated rows were deduplicated");
 CharacterState player;player.id="p";player.name="P";player.class_id="Warrior";player.gold=1000;std::int32_t price{};
 const auto cold_id=shop.inventory().inventory[0].instance_id;check(shop.buy(cold_id,player,items,{},price)==TradeResult::ok&&price==240,"buy did not use generated value");
 check(player.inventory.size()==1&&player.inventory[0].source&&player.inventory[0].source->power_names==std::vector<std::string>{"Power_Frost"},"buy transfer lost exact powers");
 const auto player_id=player.inventory[0].instance_id;check(shop.sell(player_id,player,items,price)==TradeResult::ok&&price==96,"sell did not use generated value");
 std::string buyback;for(const auto& item:shop.inventory().inventory)if(shop.is_buyback(item.instance_id))buyback=item.instance_id;
 check(!buyback.empty()&&shop.inventory().inventory.back().source->value==240,"sell transfer lost payload in buyback");
 check(shop.buy(buyback,player,items,{},price)==TradeResult::ok&&player.inventory.back().source->power_names==std::vector<std::string>{"Power_Frost"},"buyback repurchase lost powers");
 CharacterState equality; equality.id="e";equality.name="E";equality.class_id="Warrior";Shop stack({40,100,{}});
 auto potionA=InventoryItem{"r1","Potion0",2,SourceItemMetadata{2,1,{"PotionPower"}}};auto potionB=InventoryItem{"r2","Potion0",3,SourceItemMetadata{2,1,{"PotionPower"}}};auto potionC=InventoryItem{"r3","Potion0",1,SourceItemMetadata{3,1,{"OtherPower"}}};
 stack.add_generated_stock(items,{potionA,potionB,potionC});check(stack.inventory().inventory.size()==2&&stack.inventory().inventory[0].quantity==5,"source equality stack merge differs");
 CharacterState poor;poor.id="poor";poor.name="P";poor.class_id="Warrior";poor.gold=0;const auto before=stack.inventory().inventory.size();check(stack.buy(stack.inventory().inventory[0].instance_id,poor,items,{},price)==TradeResult::insufficient_gold&&poor.inventory.empty()&&stack.inventory().inventory.size()==before,"failed trade mutated item metadata/stock");
 const auto path=std::filesystem::temp_directory_path()/"dh2-b123-generated-item-test.save";std::string error;check(save_character(path,player,error),error.c_str());CharacterState loaded;check(load_character(path,loaded,error),error.c_str());std::filesystem::remove(path);check(loaded.inventory.size()==1&&loaded.inventory[0].source&&loaded.inventory[0].source->value==240&&loaded.inventory[0].source->identified==1&&loaded.inventory[0].source->power_names==std::vector<std::string>{"Power_Frost"},"v5 profile roundtrip lost source item metadata");
 std::cout<<"PASS production Shop generated transfer/value/stack/failure and v5 profile roundtrip\n";return 0;}catch(const std::exception& e){std::cerr<<e.what()<<"\n";return 1;}}
