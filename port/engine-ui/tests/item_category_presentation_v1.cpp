#include "../item_category_presentation_v1.hpp"
#include <fstream>
#include <iterator>
#include <cstring>
#include <iostream>
#include <stdexcept>
using namespace dh2;
void check(bool b,const char* e){if(!b)throw std::runtime_error(e);}
struct Gold {std::vector<unsigned char> bytes;std::size_t at{};explicit Gold(const char* p){std::ifstream f(p,std::ios::binary);check(bool(f),"Missing original corpus");bytes.assign(std::istreambuf_iterator<char>(f),{});}int word(){check(at+4<=bytes.size(),"Short corpus");int v;std::memcpy(&v,bytes.data()+at,4);at+=4;return v;}};
bool constant(void*,const char* group,const char* key,std::int32_t& value,std::string& error){check(std::string(group)=="EquipmentSlots","Wrong source constant group");const char* names[]{"Torso","RightHand","LeftHand","Feet","HandArmor","RightHandRingFinger","LeftHandRingFinger","Waist","Head","Valuables"};for(int i=0;i<10;i++)if(std::string(key)==names[i]){value=i;return true;}error="Absent constant";return false;}
int main(int argc,char** argv){try{if(argc!=2)return 2;Gold gold(argv[1]);int count=gold.word();data::ItemInstanceV1 item{};data::ItemRecord164 row{};data::PropertySheet properties{};ui::ItemCategoryPresentationServicesV1 services{nullptr,constant};std::vector<ui::ItemCategoryPresentationEntryV1> categories;std::string error;
 for(int i=0;i<count;i++){int type=gold.word(),target=gold.word(),dual=gold.word(),slot=gold.word(),wanted=gold.word();row.words[22]=type;row.words[26]=target;properties[202]=dual;check(ui::item_category_presentation_v1(item,row,properties,services,categories,error),error.c_str());bool actual=false;for(const auto& c:categories){actual|=c.index==slot;check(c.localization_symbol=="GAMEPLAYMENUS_CATEGORY_"+std::to_string(c.index),"Authored category symbol differs");}check(actual==bool(wanted),"Original native category membership differs");if(target==-1)check(categories.size()==1&&categories[0].index==9&&categories[0].icon=="Potions","Original valuables branch omitted");}
 check(gold.at==gold.bytes.size(),"Trailing source membership corpus");auto retained=categories;services.constant=nullptr;check(!ui::item_category_presentation_v1(item,row,properties,services,categories,error)&&categories.size()==retained.size(),"Missing source constants mutated output");
 std::cout<<"{\"validation\":\"PASS\",\"original_slot_cases\":"<<count<<",\"source_valuables_branch\":true,\"category_only_no_item_icon_substitution\":true}\n";return 0;
 }catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
