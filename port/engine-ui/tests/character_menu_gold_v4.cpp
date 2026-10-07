#include "character_menu_gold_v4.hpp"
#include <cassert>
#include <iostream>
using namespace dh2::ui;
int main(){
 std::string error;auto owner=std::make_shared<int>(1);int amount=17,reads=0;bool live=true,remote_seen=false,mutate=false;
 CharacterMenuGoldServicesV4 s{owner,[&](std::int32_t i,bool remote,std::uintptr_t& actor,std::string&){assert(i==0);remote_seen=remote;actor=live?41:0;return true;},[&](std::uintptr_t actor,std::int32_t& value,std::string&){assert(actor==41);++reads;value=amount;return true;},[&](const char* format,std::int32_t value,std::string& out,std::string&){assert(std::string(format)=="^d");out=std::to_string(value);if(mutate)amount=23;return true;}};
 CharacterMenuCallV1 c;CharacterMenuValueV1 index;index.kind=2;index.number=0;c.arguments={index};
 assert(character_menu_gold_call_v4(c,s,error)&&c.result.kind==4&&c.result.text=="17"&&reads==1);
 CharacterMenuValueV1 object;object.kind=5;object.object=99;c.arguments.push_back(object);mutate=true;reads=0;
 int writes=0;c.member=[&](std::uintptr_t receiver,const char* name,const CharacterMenuValueV1& value,std::string&){assert(receiver==99);if(writes++==0){assert(std::string(name)=="Gold"&&value.kind==2&&value.number==23);amount=31;}else assert(std::string(name)=="GoldString"&&value.kind==4&&value.text=="17");return true;};
 assert(character_menu_gold_call_v4(c,s,error)&&reads==2&&writes==2&&c.result.kind==5&&c.result.object==99);
 CharacterMenuValueV1 flag;flag.kind=1;flag.boolean=true;c.arguments.push_back(flag);mutate=false;reads=0;
 assert(character_menu_gold_call_v4(c,s,error)&&remote_seen&&reads==1&&c.result.kind==4&&c.result.text=="31");
 live=false;c.result.kind=2;c.result.number=101;assert(character_menu_gold_call_v4(c,s,error)&&c.result.number==101);
 live=true;c.arguments={index,object};writes=0;c.member=[](std::uintptr_t,const char*,const CharacterMenuValueV1&,std::string& e){e="actual receiver delivery failed";return false;};
 assert(!character_menu_gold_call_v4(c,s,error)&&c.result.kind==2&&c.result.number==101);
 std::cout<<"PASS whole gold typed forms, source parser/read/set ordering, remote/null and failure prefix\n";
}
