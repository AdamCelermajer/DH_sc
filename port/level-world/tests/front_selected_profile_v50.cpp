#include "../../android-native/app/src/main/cpp/front_selected_profile_v50.hpp"
#include "../../game-data/fresh_player_profile_v1.hpp"
#include <fstream>
#include <iostream>
#include <cassert>
using namespace dh2;
static std::vector<std::uint8_t> file(const char* name){std::ifstream f(std::string("port/android-native/app/src/main/assets/data/")+name,std::ios::binary);assert(f);return {std::istreambuf_iterator<char>(f),{}};}
int main(){unsigned checks{};std::string e;auto a=file("character_properties_pyarray.bin"),b=file("character_properties_pyarraynames.bin"),c=file("character_properties_pystructnames.bin");data::CharacterTable characters;assert(data::load_characters({a.data(),a.size()},{b.data(),b.size()},{c.data(),c.size()},characters,e));
 data::FreshPlayerProfileV1 fresh;assert(data::fresh_player_profile_v1(characters,"KnightPlayerBase","Actual source metadata",1234,5678,fresh,e));data::MenuProfileMetadataV1 metadata;
 int selected=-1;assert(data::load_menu_profile_metadata_v1({fresh.bytes.data(),fresh.bytes.size()},characters,2,1,{&selected,[](void* p,int x,std::string&){*static_cast<int*>(p)=x;return true;},nullptr},metadata,e));
 auto expected=fresh.bytes;std::shared_ptr<const android_ui::FrontSelectedProfileV50> retained;
 {data::CampaignProfileFileV1 read{std::move(fresh.bytes),data::CampaignProfileOriginV1::backup};assert(android_ui::retain_front_selected_profile_v50(std::move(read),metadata,"/actual/private/directory",1,retained,e));}
 assert(retained&&retained->file.bytes==expected&&retained->file.origin==data::CampaignProfileOriginV1::backup);checks+=3;
 assert(retained->metadata.slot==2&&retained->metadata.character_row==metadata.character_row&&retained->metadata.name==metadata.name&&retained->metadata.level==metadata.level&&retained->metadata.selected_difficulty==selected&&retained->requested_difficulty==1);checks+=6;
 assert(!retained->has_gear&&!retained->has_properties&&!retained->has_quests&&!retained->has_skills&&!retained->has_faeries);checks+=5;
 auto prior=retained;data::CampaignProfileFileV1 invalid{{1,0,0,0},data::CampaignProfileOriginV1::base};assert(!android_ui::retain_front_selected_profile_v50(std::move(invalid),metadata,"/actual/private/directory",0,retained,e)&&retained==prior);++checks;
 metadata.slot=-1;assert(!android_ui::retain_front_selected_profile_v50({expected,data::CampaignProfileOriginV1::base},metadata,"/actual/private/directory",0,retained,e)&&retained==prior);++checks;
 std::cout<<"Front actual source7-section selected profile bytes/backup/class/difficulty/immutable lifetime PASS "<<checks<<"; no GEAR/PROP inference, no live gameplay publication claimed\n";
}
