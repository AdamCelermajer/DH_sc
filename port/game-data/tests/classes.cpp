#include "class_tables.hpp"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <iterator>
#include <random>
#include <stdexcept>
using namespace dh2::data;
std::vector<std::uint8_t> read(const std::string& p){std::ifstream f(p,std::ios::binary);return {std::istreambuf_iterator<char>(f),{}};}
Bytes bytes(const std::vector<std::uint8_t>& b){return {b.data(),b.size()};}
void check(bool b,const char* error){if(!b)throw std::runtime_error(error);}
void word(std::vector<std::uint8_t>& b,std::int32_t v){auto bits=std::uint32_t(v);for(unsigned i=0;i<4;++i)b.push_back((bits>>(8*i))&255);}
int main(int argc,char** argv){try{
 if(argc!=3)return 2;
 std::string root=argv[1],characters=argv[2],error;auto a=read(root+"/character_classes_pyarray.bin"),b=read(root+"/character_classes_pyarraynames.bin"),c=read(root+"/character_classes_pystructnames.bin");ClassTables table;
 check(load_classes(bytes(a),bytes(b),bytes(c),table,error),error.c_str());check(table.names.size()==260&&table.rows.size()==260&&table.data_consumed==34224,"Class dimensions differ");
 std::vector<std::uint8_t> encoded;word(encoded,table.rows.size());unsigned formulas=0;for(const auto& row:table.rows){word(encoded,row.size());for(const auto& f:row){for(auto v:{f.destination,f.type,f.p1,f.p2,f.p3})word(encoded,v);++formulas;}}
 check(encoded==a&&formulas==1659,"Native class roundtrip differs");
 unsigned applications=0;for(unsigned i=0;i<table.rows.size();++i)for(int level:{256,512,2560,12800}){PropertySheet sheet{};sheet[19]=level;check(apply_class(table,i,sheet,error),error.c_str());++applications;}
 PropertySheet sheet{};sheet[19]=256;check(apply_class(table,18,sheet,error)&&sheet[38]==12160&&sheet[43]==5632,"BaseMonster level one stats differ");
 sheet[19]=2560;check(apply_class(table,18,sheet,error)&&sheet[38]==29440,"BaseMonster level ten health differs");
 ClassTables synthetic;synthetic.rows={{{0,9,256,0,0},{0,6,1,0,0},{0,8,0,0,0},{0,9,999,0,0}}};sheet={};sheet[1]=128;check(apply_class(synthetic,0,sheet,error)&&sheet[0]==129,"Buff fallthrough/type8 early return differs");
 synthetic.rows={{{0,1,-666,1,7}}};sheet={};sheet[0]=20;sheet[1]=-257;check(apply_class(synthetic,0,sheet,error)&&sheet[0]==6,"Negative ASR/current-base differs");
 synthetic.rows={{{-1,0,0,-1,-1}}};auto before=sheet;check(!apply_class(synthetic,0,sheet,error)&&sheet==before,"Recursive class failure was not atomic");
 synthetic.rows={{{224,9,0,0,0}}};check(!apply_class(synthetic,0,sheet,error)&&sheet==before,"Invalid destination changed sheet");
 auto cr=read(characters+"/character_properties_pyarray.bin"),cn=read(characters+"/character_properties_pyarraynames.bin"),cf=read(characters+"/character_properties_pystructnames.bin");CharacterTable chars;check(load_characters(bytes(cr),bytes(cn),bytes(cf),chars,error),error.c_str());
 check(chars.fields[19]=="Level"&&chars.fields[38]=="Max_HP"&&chars.fields[43]=="Max_MP","Property IDs differ");unsigned links=0;for(const auto& row:chars.rows){const auto class_id=row[std::find(chars.fields.begin(),chars.fields.end(),"ClassID")-chars.fields.begin()];auto result=row;check(apply_class(table,class_id,result,error),error.c_str());++links;}
 std::mt19937 rng(20261002);unsigned rejected=0;for(unsigned i=0;i<3000;++i){auto x=a,y=b,z=c;auto* selected=i%3==0?&x:i%3==1?&y:&z;if(i%2)selected->resize(rng()%selected->size());else (*selected)[rng()%selected->size()]^=1u<<(rng()%8);ClassTables result;bool ok=load_classes(bytes(x),bytes(y),bytes(z),result,error);if(!ok){++rejected;check(result.names.empty()&&result.rows.empty(),"Failed class read left output");}}
 std::cout<<"{\"classes\":260,\"formulas\":1659,\"serialized_bytes\":34224,\"native_roundtrip_matches_original\":true,\"class_level_applications\":"<<applications<<",\"character_row_applications\":"<<links<<",\"mutations_and_truncations\":3000,\"rejected\":"<<rejected<<",\"recursive_failure_atomic\":true,\"buff_fallthrough_and_early_return_verified\":true,\"base_monster_level_one_max_hp_raw\":12160,\"base_monster_level_ten_max_hp_raw\":29440}\n";return 0;
}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 3;}}
