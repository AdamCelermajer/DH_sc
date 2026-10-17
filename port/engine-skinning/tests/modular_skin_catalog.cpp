#include "../modular_skin_catalog.hpp"
#include <fstream>
#include <iostream>
#include <iterator>
#include <map>
#include <set>
#include <stdexcept>
#include <vector>

namespace {
void require(bool value,const char* message){if(!value)throw std::runtime_error(message);}
std::vector<std::uint8_t> read(const char* path){
 std::ifstream stream(path,std::ios::binary);
 if(!stream)throw std::runtime_error(std::string("Missing model: ")+path);
 return {std::istreambuf_iterator<char>(stream),{}};
}
void expected(const dh2::skinning::ModularSkinCatalog& catalog,
              const char* category,const char* item,unsigned category_id,
              unsigned module_id,unsigned controller_id){
 const auto* module=dh2::skinning::find_modular_skin(catalog,category,item);
 require(module,"Expected exact starter module is absent");
 require(module->category_id==category_id,"Starter category ID differs");
 require(module->module_id==module_id,"Starter module ID differs");
 require(module->controller_index==controller_id,"Starter BRES controller index differs");
}
}

int main(int argc,char** argv){try{
 require(argc==2,"Usage: modular_skin_catalog_audit prince_modular.bdae");
 auto bytes=read(argv[1]);dh2::skinning::ModularSkinCatalog catalog;std::string error;
 if(!dh2::skinning::load_modular_skin_catalog(bytes.data(),bytes.size(),catalog,error))
  throw std::runtime_error(error.empty()?"Prince modular catalog rejected":error);
 std::map<std::string,unsigned> counts;std::set<unsigned> controllers;
 for(const auto& module:catalog.modules){
  ++counts[module.category];
  require(controllers.insert(module.controller_index).second,
          "A controller is assigned to multiple modular slots");
  require(module.controller==module.item_name+"-mesh-skin",
          "Controller identity differs from the source item name");
 }
 require(catalog.modules.size()==172,"Expected 4 x 43 source modular entries");
 require(counts.size()==4&&counts["MC_Feet"]==43&&counts["MC_Hands"]==43&&
         counts["MC_Head"]==43&&counts["MC_Torso"]==43,
         "Prince source category ordering or module counts changed");
 expected(catalog,"MC_Torso","MC_Torso_default_warrior",3,18,171);
 expected(catalog,"MC_Feet","MC_Feet_default_warrior",0,18,42);
 expected(catalog,"MC_Hands","MC_Hands_default_warrior",1,18,85);
 expected(catalog,"MC_Torso","MC_Torso_default_rogue",3,17,170);
 expected(catalog,"MC_Feet","MC_Feet_default_rogue",0,17,41);
 expected(catalog,"MC_Hands","MC_Hands_default_rogue",1,17,84);
 expected(catalog,"MC_Torso","MC_Torso_default_mage",3,16,169);
 expected(catalog,"MC_Feet","MC_Feet_default_mage",0,16,40);
 expected(catalog,"MC_Hands","MC_Hands_default_mage",1,16,83);
 expected(catalog,"MC_Head","MC_Head__naked",2,0,123);
 const dh2::skinning::ModularSkinModule* selected=nullptr;bool placeholder=false;
 require(dh2::skinning::resolve_modular_skin(catalog,"MC_Torso",
         "MC_Torso_default_warrior",true,selected,placeholder,error)&&
         selected&&selected->module_id==18&&!placeholder,
         "Known equipped armor did not resolve exactly");
 require(dh2::skinning::resolve_modular_skin(catalog,"MC_Torso",
         "MC_Torso_not_in_resource",true,selected,placeholder,error)&&
         selected&&selected->item_name=="MC_Torso__placeholder"&&
         selected->module_id==1&&placeholder,
         "Unknown armor did not select the exact source placeholder");
 require(dh2::skinning::resolve_modular_skin(catalog,"MC_Head",{},false,
         selected,placeholder,error)&&selected&&selected->module_id==0&&!placeholder,
         "Unequipped category did not select exact source naked module");
 require(!dh2::skinning::resolve_modular_skin(catalog,"MC_RWeapon",{},false,
         selected,placeholder,error)&&!selected,
         "Weapon slot was incorrectly aliased to modular armor");
 std::cout<<"{\"validation\":\"PASS\",\"categories\":4,\"modules\":"
          <<catalog.modules.size()<<",\"exact_starter_mappings\":10"
          <<",\"placeholder_fallback\":true,\"naked_selection\":true"
          <<",\"weapon_category_rejected\":true}\n";
 return 0;
}catch(const std::exception& exception){std::cerr<<exception.what()<<'\n';return 1;}}
