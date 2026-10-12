#include "game_object_external_script_prefix_v52.hpp"
#include <cassert>
#include <iostream>
using namespace dh2::world;
int main(){unsigned cases=0;std::string e;bool whole;
 for(const char* name:{static_cast<const char*>(nullptr),"","chest.lua"})for(bool prior:{false,true}){
  std::uintptr_t script=prior?0x100000123ULL:0;unsigned deleted=0,loaded=0;
  ExternalScriptServicesV52 s;s.actual_deleting_destructor=[&](auto old,auto&){assert(old==0x100000123ULL);++deleted;script=77;return true;};
  s.actual_new_script_suffix=[&](auto n,auto path,auto&){assert(std::string(n)=="chest.lua"&&std::string(path)=="data/scripts/objects/"&&script==0);++loaded;script=0x100000789ULL;return true;};
  assert(game_object_external_script_prefix_v52(&script,name,"data/scripts/objects/",s,whole,e)&&whole);
  assert(deleted==unsigned(prior)&&loaded==unsigned(name&&name[0]));assert(script==(name&&name[0]?0x100000789ULL:0));++cases;
 }
 std::uintptr_t script=0x100000123ULL;assert(!game_object_external_script_prefix_v52(&script,"",nullptr,{},whole,e)&&!whole&&script==0x100000123ULL);++cases;
 script=0;assert(!game_object_external_script_prefix_v52(&script,"chest.lua",nullptr,{},whole,e)&&!whole&&script==0);++cases;
 assert(!game_object_external_script_prefix_v52(nullptr,"",nullptr,{},whole,e)&&!whole);++cases;
 ExternalScriptServicesV52 s;s.actual_deleting_destructor=[](auto,auto& e){e="deleting destructor failed";return false;};script=11;assert(!game_object_external_script_prefix_v52(&script,"",nullptr,s,whole,e)&&script==11);++cases;
 std::cout<<"PASS native_external_script_prefix_cases="<<cases<<" high_pointer_preserved=1 delete_before_empty_gate=1 callback_changed_field_overwritten=1 empty_whole_method=1 positive_suffix_fixture=1 missing_body_failure_prefix=1 no_VM_created=1\n";
}
