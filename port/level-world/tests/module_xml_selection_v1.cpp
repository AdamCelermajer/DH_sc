#include "module_xml_selection_v1.hpp"
#include <cassert>
#include <iostream>
#include <vector>
using namespace dh2::world;
int main(){
 auto owner=std::make_shared<int>(1);std::string e,a,b;int calls=0,roll=0;
 ModuleXmlServicesV1 rng{owner,[&](int maximum,int& value,std::string&){assert(maximum==100);++calls;value=roll;return true;}};
 ModuleXmlFieldsV1 f{"primary.mgp","primary.mvp","","",""};
 assert(module_choose_xmls_v1(f,{},a,b,e)&&a==f.mgp378&&b==f.mvp390);
 f.alt_mgp3a8="one.mgp,two.mgp";assert(module_choose_xmls_v1(f,{},a,b,e));
 f.alt_mvp3c0="one.mvp,two.mvp";f.alt_prob3d8="20,35";
 for(roll=0;roll<100;++roll){assert(module_choose_xmls_v1(f,rng,a,b,e));const int index=roll<20?0:roll<55?1:2;assert(a==std::vector<std::string>({"one.mgp","two.mgp","primary.mgp"})[index]);assert(b==std::vector<std::string>({"one.mvp","two.mvp","primary.mvp"})[index]);}
 assert(calls==100);f.alt_prob3d8="";
 for(roll=0;roll<100;++roll){assert(module_choose_xmls_v1(f,rng,a,b,e));assert(a==(roll<33?"one.mgp":roll<66?"two.mgp":"primary.mgp"));}
 f.alt_mvp3c0="one.mvp";assert(!module_choose_xmls_v1(f,rng,a,b,e)&&a==f.mgp378&&b==f.mvp390&&calls==200);
 f.alt_mgp3a8.clear();f.alt_mvp3c0.clear();int id=-1;float offset[3]{},position[3]{4,5,6};int loads=0;
 ModuleLevelLoadBorrowV1 level{owner,&id,offset,[&](const std::string& path,const char* filter,bool& loaded,std::string&){assert(id==7&&offset[0]==4&&offset[1]==5&&offset[2]==6);assert(std::string(filter)=="Module");++loads;assert(path==(loads<=2?"primary.mgp":"primary.mvp"));loaded=loads!=1;return true;}};
 assert(module_load_v1(f,7,position,{},level,e)&&loads==3&&id==-1&&offset[0]==0&&offset[1]==0&&offset[2]==0);
 level.load_file=[&](const std::string&,const char*,bool&,std::string& error){error="actual load diagnostic";return false;};
 assert(!module_load_v1(f,9,position,{},level,e)&&e=="actual load diagnostic"&&id==9&&offset[2]==6);
 level.load_file={};f.mgp378.clear();f.mvp390.clear();assert(module_load_v1(f,10,position,{},level,e)&&id==-1&&offset[2]==0);
 std::cout<<"PASS Module XML weighted boundaries200, source retry order, same-Level failure prefix\n";
}
