#include "properties.hpp"
#include <fstream>
#include <iostream>
#include <iterator>
#include <stdexcept>
using namespace dh2::data;
std::vector<std::uint8_t> read(const std::string& p){std::ifstream f(p,std::ios::binary);return {std::istreambuf_iterator<char>(f),{}};}
Bytes bytes(const std::vector<std::uint8_t>& b){return {b.data(),b.size()};}
void check(bool b,const char* e){if(!b)throw std::runtime_error(e);}
int main(int argc,char** argv){try{
 if(argc!=3)return 2;std::string root=argv[1],error;auto a=read(root+"/character_properties_pyarray.bin"),b=read(root+"/character_properties_pyarraynames.bin"),c=read(root+"/character_properties_pystructnames.bin"),reference=read(argv[2]);CharacterTable table;PropertyRules rules;
 check(load_characters(bytes(a),bytes(b),bytes(c),table,error)&&load_property_rules(table,rules,error),error.c_str());check(rules.defaults[0]==10&&rules.types[0]==1&&rules.defaults[36]==-1&&rules.types[36]==32,"Original rules differ");
 check(reference.size()==table.rows.size()*896,"Original-instruction row reference dimensions differ");unsigned sheets=0;for(const auto& row:table.rows){PropertyState state;reset_properties(rules,state,&row);check(recalc_properties(rules,state,error),error.c_str());for(unsigned p=0;p<224;++p){auto bits=std::uint32_t(state.resolved[p]);for(unsigned j=0;j<4;++j)check(((bits>>(j*8))&255)==reference[sheets*896+p*4+j],"Native row differs from original-instruction reference");}++sheets;}
 PropertyState s;reset_properties(rules,s);auto v=property_view(rules,s);std::int32_t result=0;
 check(dh2_property_set_int(&v,36,47)==0&&s.saved[36]==12032&&s.resolved[36]==12032,"Fixed point HP mutation differs");check(dh2_property_add(&v,36,-256)==0&&s.saved[36]==11776&&s.resolved[36]==11776,"HP add differs");
 PropertyRules synthetic;synthetic.defaults.fill(-1);synthetic.types.fill(4);reset_properties(synthetic,s);v=property_view(synthetic,s);
 // AddProperty treats an intermediate result equal to the property's default
 // as unset again. This is observable after wrapping/cancellation.
 s.base[0]=0;s.saved[0]=-2;s.gear[0]=1;check(dh2_property_resolve(&v,0,&result)==0&&result==-1,"Default collision differs");
 PropertySheet low1,low2,high1,high2;low1.fill(-1);low2=high1=high2=low1;low1[0]=10;low2[0]=20;high1[0]=30;high2[0]=40;const std::int32_t* low[]={low1.data(),low2.data()};const std::int32_t* high[]={high1.data(),high2.data()};PropertyBuffGroup groups[]={{low,2},{nullptr,0},{high,2}};v.groups=groups;v.group_count=3;
 check(dh2_property_resolve(&v,0,&result)==0&&result==100,"Intermediate default collision did not replace on next add");synthetic.types[0]=1;check(dh2_property_resolve(&v,0,&result)==0&&result==40,"Descending group/insertion priority differs");synthetic.types[0]=2;check(dh2_property_resolve(&v,0,&result)==0&&result==0,"Base override differs");s.base[0]=s.saved[0]=s.gear[0]=-1;check(dh2_property_resolve(&v,0,&result)==0&&result==20,"Ascending group/insertion priority differs");
 synthetic.types[0]=4;s.base[0]=2147483647;s.saved[0]=1;check(dh2_property_resolve(&v,0,&result)==0&&result==-2147483548,"Additive wrapping differs");
 synthetic.types[1]=8;s.resolved[1]=17;check(dh2_property_set(&v,1,123)==0&&dh2_property_add(&v,1,5)==0&&s.resolved[1]==128,"Runtime-only setter differs");check(dh2_property_resolve(&v,1,&result)==0&&result==128,"Runtime-only recalc overwrote value");
 auto before=s;result=99;check(dh2_property_resolve(&v,224,&result)==1&&s.resolved==before.resolved&&result==99,"Invalid index changed state");groups[0].count=10001;check(dh2_property_set(&v,36,10)==1&&s.saved==before.saved&&s.resolved==before.resolved,"Invalid buff group changed state");groups[0].count=2;
 const std::int32_t* missing[]={nullptr};groups[0]={missing,1};check(dh2_property_add(&v,36,10)==1&&s.saved==before.saved,"Missing buff sheet changed state");
 auto malformed=table;malformed.names[0]="unexpected";check(!load_property_rules(malformed,synthetic,error)&&synthetic.defaults==PropertySheet{}&&synthetic.types==PropertySheet{},"Failed rules load retained output");
 std::cout<<"{\"original_character_rows_resolved\":"<<sheets<<",\"properties_per_sheet\":224,\"original_default_type_rows_verified\":true,\"default_collision_and_wrapping_verified\":true,\"buff_group_and_insertion_precedence_verified\":true,\"fixed_point_health_mutations_verified\":true,\"runtime_only_recalc_preserved\":true,\"invalid_input_atomic\":true}\n";return 0;
}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 3;}}
