#include "character_model_name_v38.hpp"
#include <cassert>
#include <fstream>
#include <iostream>
#include <sstream>
using namespace dh2;
using namespace dh2::character;
int main(int argc,char** argv){assert(argc==3);auto lifetime=std::make_shared<int>(1);std::vector<std::string> files;std::string line;
 std::ifstream dictionary(argv[1]);assert(dictionary);while(std::getline(dictionary,line)){if(!line.empty()&&line.back()=='\r')line.pop_back();files.push_back(line);}assert(files.size()==116);
 std::ifstream gold(argv[2]);assert(gold);unsigned cases=0;std::string error;
 while(std::getline(gold,line)){
  if(!line.empty()&&line.back()=='\r')line.pop_back();if(line.empty())continue;std::stringstream ss(line);std::vector<std::string> col;std::string part;while(std::getline(ss,part,'\t'))col.push_back(part);assert(col.size()==10);
  data::PropertySheet resolved{};resolved[3]=std::stoi(col[0]);data::PropertyView properties;properties.resolved=resolved.data();std::int16_t klass=std::stoi(col[7]);std::uintptr_t master=std::stoi(col[5])?99:0;
  CharacterModelFieldsV38 f{lifetime,1,&properties,&klass,&master};CharacterModelNamesV38 names{lifetime,&files};CharacterModelServicesV38 s;
  s.character_type=[&](auto receiver,auto& v,std::string&){assert(receiver==1);v=std::stoi(col[1])?3:(std::stoi(col[2])?1:0);return true;};
  s.is_player=[&](auto receiver,bool& v,std::string&){assert(receiver==1);v=std::stoi(col[2]);return true;};
  s.high_performance=[&](bool& v,std::string&){v=std::stoi(col[3]);return true;};s.is_local_player=[&](auto receiver,bool& v,std::string&){assert(receiver==1);v=std::stoi(col[4]);return true;};
  s.saved_current_faery=[&](auto receiver,auto difficulty,auto& v,std::string&){assert(receiver==99&&difficulty==-1);v=std::stoi(col[8]);return true;};
  s.faery_model=[&](auto receiver,auto id,auto& v,std::string&){assert(receiver==99&&id==std::stoi(col[8]));v=std::stoi(col[6]);return true;};
  CharacterModelResultV38 result;assert(character_model_name_v38(f,names,s,result,error));auto expected=col[9];assert((result.file?*result.file:"<NULL>")==expected);assert(result.dictionary_lease);
  if(std::stoi(col[1])&&resolved[3]>=0&&resolved[3]<116){auto saved=s;s.saved_current_faery={};s.faery_model={};if(master)assert(!character_model_name_v38(f,names,s,result,error));else assert(character_model_name_v38(f,names,s,result,error));s=saved;f.master418=nullptr;assert(!character_model_name_v38(f,names,s,result,error));f.master418=&master;
   master=0;assert(character_model_name_v38(f,names,s,result,error)&&result.file==&files[resolved[3]]);master=99;assert(character_model_name_v38(f,names,s,result,error));auto override_id=std::stoi(col[6]);assert((result.file?*result.file:"<NULL>")==((override_id>=0&&override_id<116)?files[override_id]:"<NULL>"));master=0;assert(character_model_name_v38(f,names,s,result,error)&&result.file==&files[resolved[3]]);
  }
  if(!std::stoi(col[1])&&resolved[3]>=0&&resolved[3]<116){s.is_player={};assert(!character_model_name_v38(f,names,s,result,error));}
  ++cases;
 }
 assert(cases==13);std::cout<<"PASS original_model_cases="<<cases<<" borrowed_fields_only=1 live_master_change_observed=1 missing_source_services_explicit=1\n";
}
