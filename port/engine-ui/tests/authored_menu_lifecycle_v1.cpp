#include "../authored_menu_lifecycle_v1.hpp"
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <vector>
using namespace dh2::ui;
int main(int argc,char** argv){try{
 if(argc!=2)throw std::runtime_error("Original lifecycle gold path required");
 const char* names[]{"menu_CharacterMenu","menu_CharacterSheetNew","menu_CharacterSheetStats","menu_InventorySheetMain","menu_InventorySheetDetails","menu_SkillTreeSheetNew","menu_FaerySheet","menu_Merchant","menu_Ingame","menu_Options","menu_VerificationLoading","menu_language","menu_playlist","menu_splash","unknown"};
 std::ifstream f(argv[1]);if(!f)throw std::runtime_error("Gold unavailable");std::string line,error;unsigned cases=0;AuthoredMenuFieldsV1 m;
 while(std::getline(f,line)){
  std::istringstream in(line);unsigned ni,valid,localized,drag,op,vis,loc,counter,n;std::uint32_t language;
  in>>ni>>valid>>localized>>drag>>language>>op>>vis>>loc>>counter>>n;
  if(!op){m={};m.identity=1;m.name=names[ni];m.valid7c=std::uint8_t(valid);m.localized75=std::uint8_t(localized);m.visible74=5;m.counter78=31;m.drag5c=drag;}
  std::vector<std::pair<unsigned,int>> actual,expected;
  for(unsigned i=0;i<n;++i){unsigned code;int value;in>>code>>value;expected.emplace_back(code,value);}
  AuthoredMenuLifecycleServicesV1 s;s.owner=std::make_shared<int>(1);
  s.invoke=[&](auto& fields,const auto& q,int& out,auto&){
   int value=q.value;
   if(q.operation==AuthoredMenuOperationV1::invoke_as)value=std::string(q.text)=="onPush"?1:0;
   actual.emplace_back(unsigned(q.operation),value);
   if(q.operation==AuthoredMenuOperationV1::localize)fields.localized75=1;
   if(q.operation==AuthoredMenuOperationV1::get_saved_language)out=static_cast<int>(language);
   return true;
  };
  if(!(op?authored_menu_hide_v1(m,s,error):authored_menu_show_v1(m,s,error)))throw std::runtime_error(error);
  if(actual!=expected||m.visible74!=vis||m.localized75!=loc||m.counter78!=counter)throw std::runtime_error("Original lifecycle mismatch case "+std::to_string(cases)+" "+names[ni]+" op "+std::to_string(op));
  ++cases;
 }
 if(cases!=960)throw std::runtime_error("Incomplete gold");
 // Required failure preserves reached writes and never invokes later services.
 m={};m.valid7c=1;m.name="menu_CharacterMenu";unsigned calls=0;AuthoredMenuLifecycleServicesV1 reject;reject.owner=std::make_shared<int>(1);reject.invoke=[&](auto&,const auto& q,int&,auto& e){++calls;if(q.operation==AuthoredMenuOperationV1::localize){e="required localization";return false;}return true;};
 if(authored_menu_show_v1(m,reject,error)||calls!=4||m.visible74)throw std::runtime_error("Failed prefix was hidden");
 std::cout<<"{\"validation\":\"PASS\",\"original_show_hide_comparisons\":"<<cases<<",\"required_failure_prefix\":true}\n";return 0;
}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
