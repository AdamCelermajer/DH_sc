#include "data.hpp"
#include <fstream>
#include <iostream>
#include <iterator>
#include <random>
std::vector<std::uint8_t> read(const std::string& name){std::ifstream f(name,std::ios::binary);return {std::istreambuf_iterator<char>(f),{}};}
dh2::data::Bytes view(const std::vector<std::uint8_t>& b){return {b.data(),b.size()};}
int main(int argc,char** argv){
 if(argc!=2)return 2;const std::string root=argv[1];auto records=read(root+"/character_properties_pyarray.bin"),names=read(root+"/character_properties_pyarraynames.bin"),fields=read(root+"/character_properties_pystructnames.bin"),keys=read(root+"/character_models_dictionary_pyarraynames.bin"),models=read(root+"/character_models_dictionary_pyarray.bin");
 dh2::data::CharacterTable table;dh2::data::Dictionary dictionary;std::string error;
 if(!dh2::data::load_characters(view(records),view(names),view(fields),table,error)||!dh2::data::load_dictionary(view(keys),view(models),dictionary,error)){std::cerr<<error;return 3;}
 if(table.rows.size()!=448||table.fields.size()!=224||dictionary.values.size()!=116||table.data_consumed!=401412)return 4;
 const auto* skeleton=dh2::data::property(table,"Crypt_Skeleton","ModelFile");const auto* slime=dh2::data::property(table,"CryptSlime","ModelFile");
 if(!skeleton||*skeleton!=85||!slime||*slime!=89||dictionary.values[*skeleton]!="data/3D/characters/skeleton/skeleton.bdae"||dictionary.values[*slime]!="data/3D/characters/slime/slime_green_v2.bdae")return 5;
 if(dh2::data::property(table,"absent","ModelFile")||dh2::data::property(table,"Crypt_Skeleton","absent")||dh2::data::lookup(dictionary,"absent"))return 6;
 unsigned links=0;for(const auto& name:table.names){auto* id=dh2::data::property(table,name,"ModelFile");if(!id||*id< -1||*id>=int(dictionary.values.size()))return 7;links+=*id>=0;}
 // Meaningful truncations must fail atomically, including inside a row and
 // inside the variable-length model paths. Extra subclass bytes are allowed.
 unsigned rejected=0;std::mt19937 rng(20261002);
 for(unsigned i=0;i<2000;++i){auto a=records,b=names,c=fields,d=keys,e=models;auto* selected=i%5==0?&a:i%5==1?&b:i%5==2?&c:i%5==3?&d:&e;
  if(i%2)selected->resize(rng()%selected->size());else{unsigned index=rng()%selected->size();(*selected)[index]^=1u<<(rng()%8);}
  dh2::data::CharacterTable t;dh2::data::Dictionary m;const bool chars=dh2::data::load_characters(view(a),view(b),view(c),t,error),dict=dh2::data::load_dictionary(view(d),view(e),m,error);
  if((!chars&&(!t.rows.empty()||!t.names.empty()))||(!dict&&(!m.values.empty()||!m.names.empty())))return 8;rejected+=!chars||!dict;
 }
 std::cout<<"{\"characters\":448,\"fields\":224,\"models\":116,\"valid_model_links\":"<<links<<",\"character_section_bytes\":"<<table.data_consumed<<",\"unparsed_suffix_bytes\":"<<records.size()-table.data_consumed<<",\"mutations_and_truncations\":2000,\"rejected\":"<<rejected<<"}\n";return 0;
}
