#include "vitals.hpp"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <iterator>
#include <stdexcept>
using namespace dh2::data;
std::vector<std::uint8_t> read(const std::string& p){std::ifstream f(p,std::ios::binary);return {std::istreambuf_iterator<char>(f),{}};}
Bytes bytes(const std::vector<std::uint8_t>& b){return {b.data(),b.size()};}
void check(bool b,const char* e){if(!b)throw std::runtime_error(e);}
bool equal(const PropertyState& a,const PropertyState& b){return a.base==b.base&&a.saved==b.saved&&a.gear==b.gear&&a.resolved==b.resolved;}
void compare(const PropertyState& state,const std::vector<std::uint8_t>& reference,unsigned row){unsigned field=0;for(const auto* sheet:{&state.base,&state.saved,&state.gear,&state.resolved})for(auto value:*sheet){auto bits=std::uint32_t(value);for(unsigned j=0;j<4;++j)check(((bits>>(j*8))&255)==reference[row*3584+field*4+j],"Owner state differs from original instructions");++field;}}
int main(int argc,char** argv){try{
 if(argc!=5)return 2;std::string chars=argv[1],classes=argv[2],error;auto a=read(chars+"/character_properties_pyarray.bin"),b=read(chars+"/character_properties_pyarraynames.bin"),c=read(chars+"/character_properties_pystructnames.bin");CharacterTable table;PropertyRules rules;
 check(load_characters(bytes(a),bytes(b),bytes(c),table,error)&&load_property_rules(table,rules,error),error.c_str());auto ca=read(classes+"/character_classes_pyarray.bin"),cb=read(classes+"/character_classes_pyarraynames.bin"),cc=read(classes+"/character_classes_pystructnames.bin");ClassTables cls;check(load_classes(bytes(ca),bytes(cb),bytes(cc),cls,error),error.c_str());auto before=read(argv[3]),after=read(argv[4]);check(before.size()==448*3584&&after.size()==before.size(),"Original owner reference dimensions differ");unsigned rows=0;
 for(const auto& source:table.rows){PropertyState s;reset_properties(rules,s,&source);check(recalc_properties_with_class(cls,rules,s,error),error.c_str());compare(s,before,rows);SpawnVitals spawn;check(initialize_spawn_vitals(rules,s,spawn,error),error.c_str());compare(s,after,rows);++rows;}
 PropertyState s;auto index=std::find(table.names.begin(),table.names.end(),"Crypt_Skeleton")-table.names.begin();reset_properties(rules,s,&table.rows.at(index));check(recalc_properties_with_class(cls,rules,s,error),error.c_str());SpawnVitals spawn;check(initialize_spawn_vitals(rules,s,spawn,error),error.c_str());check(s.resolved[36]==12160&&s.resolved[38]==12160&&s.resolved[41]==5632&&spawn.first_hp.raw_add==12160&&spawn.first_hp.after==12159&&spawn.second_hp.raw_add==1&&spawn.second_hp.after==12160,"Crypt two-pass initialization differs");
 auto v=property_view(rules,s);VitalsChange change;check(dh2_vitals_regen(&v,0,256,&change)==0&&change.raw_add==0&&change.after==12160,"Full health regen changed HP");check(dh2_property_add(&v,36,-512)==0&&dh2_vitals_regen(&v,0,256,&change)==0&&change.raw_add==256&&change.after==11904,"Partial health regen differs");check(dh2_vitals_regen(&v,0,-1,&change)==0&&change.raw_add==256&&change.after==12160,"Fill-to-maximum differs");
 auto snapshot=s;change={1,2,3};check(dh2_vitals_regen(&v,2,10,&change)==1&&equal(s,snapshot)&&change.raw_add==1,"Invalid mana selector changed output/state");auto invalid=v;invalid.types=nullptr;check(dh2_vitals_regen(&invalid,0,10,&change)==1&&equal(s,snapshot),"Invalid view changed state");
 ClassTables synthetic;synthetic.rows={{{38,9,999,0,0},{-1,0,0,-1,-1}}};check(!apply_class_uncached(synthetic,0,rules,s,error)&&equal(s,snapshot),"Uncached recursive failure was not atomic");
 // A saved Level must influence an uncached linear class read.
 reset_properties(rules,s,&table.rows.at(index));s.saved[19]=9*256;check(recalc_properties_with_class(cls,rules,s,error)&&s.resolved[19]==10*256&&s.base[38]==29440,"Saved level was ignored by class source resolution");
 std::cout<<"{\"uncached_original_character_owner_states\":"<<rows<<",\"two_pass_spawn_original_owner_states\":"<<rows<<",\"properties_compared_per_owner\":896,\"crypt_hp_raw\":12160,\"crypt_mp_raw\":5632,\"crypt_first_hp_add_raw\":12160,\"crypt_second_hp_add_raw\":1,\"positive_and_fill_regen_verified\":true,\"saved_level_influences_uncached_class\":true,\"recursive_failure_atomic\":true,\"invalid_view_atomic\":true}\n";return 0;
}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 3;}}
