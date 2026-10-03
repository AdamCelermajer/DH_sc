#include "../character_attack_geometry.hpp"
#include <array>
#include <cassert>
#include <cmath>
#include <cstring>
#include <fstream>
#include <iostream>
#include <vector>
using namespace dh2::character;
float floating(std::uint32_t word){float result;std::memcpy(&result,&word,4);return result;}
std::uint32_t bits(float value){std::uint32_t result;std::memcpy(&result,&value,4);return result;}
bool equal(std::uint32_t a,std::uint32_t b){return a==b||(std::isnan(floating(a))&&std::isnan(floating(b)));}
int main(int argc,char** argv){
 assert(argc==2);std::ifstream in(argv[1],std::ios::binary);std::array<std::uint32_t,5> header{};in.read(reinterpret_cast<char*>(header.data()),20);assert(in&&header[0]==0x31514741&&header[2]==1322&&header[3]==76&&header[4]==23);
 std::vector<CombatItemRecord164> rows(header[2]);in.read(reinterpret_cast<char*>(rows.data()),rows.size()*164);std::vector<float> radii(header[3]);in.read(reinterpret_cast<char*>(radii.data()),radii.size()*4);assert(in);const auto base_rows=rows;const auto base_radii=radii;std::array<std::uint32_t,5> totals{};
 for(std::uint32_t i=0;i<header[1];++i){
  std::array<std::uint32_t,23> w{};in.read(reinterpret_cast<char*>(w.data()),92);assert(in&&w[0]<5);const auto kind=w[0];const auto item=static_cast<std::int32_t>(w[1]);CombatProperties896 props{};props.words[1]=static_cast<std::int32_t>(w[2]);props.words[32]=static_cast<std::int32_t>(w[3]);props.words[30]=static_cast<std::int32_t>(w[4]);props.words[31]=static_cast<std::int32_t>(w[5]);CombatItemInstance4 instance{item<0?0:item};const CombatItemInstance4* reference=&instance;CombatEquipSet8 set{item<0?nullptr:&reference};CombatInventory16 inventory{&set,1,0};std::int32_t output[3];std::memcpy(output,w.data()+16,12);
  const auto ai=props.words[1]>=0&&static_cast<std::uint32_t>(props.words[1])<radii.size()?props.words[1]:8;
  if(kind==2&&w[13])radii[ai]=floating(w[12]);
  if(kind==2&&w[15]){assert(item>=0&&static_cast<std::size_t>(item)<rows.size());rows[item].words[39]=static_cast<std::int32_t>(w[14]);}
  float owner[3],target[3],reach[2];std::memcpy(owner,w.data()+6,12);std::memcpy(target,w.data()+9,12);std::memcpy(reach,w.data()+12,8);const std::int32_t limits[]={static_cast<std::int32_t>(w[14]),static_cast<std::int32_t>(w[15])};int result=0;
  if(kind==0)result=dh2_attack_equipment_melee_radius(output,&inventory,rows.data(),rows.size());
  else if(kind==1)result=dh2_attack_range_parameters(output,&props,&inventory,rows.data(),rows.size());
  else if(kind==2){float radius=floating(w[16]);result=dh2_attack_melee_radius(&radius,&props,&inventory,rows.data(),rows.size(),radii.data(),radii.size());const auto raw=bits(radius);std::memcpy(output,&raw,4);}
  else if(kind==3)result=dh2_attack_melee_distance(owner,target,reach);
  else result=dh2_attack_ranged_distance(owner,target,limits);
  assert(result==static_cast<std::int32_t>(w[19]));for(unsigned j=0;j<3;++j){std::uint32_t raw;std::memcpy(&raw,output+j,4);assert(kind==2&&j==0?equal(raw,w[20+j]):raw==w[20+j]);}
  if(kind==2&&w[13])radii[ai]=base_radii[ai];
  if(kind==2&&w[15])rows[item].words[39]=base_rows[item].words[39];
  ++totals[kind];
 }
 assert(in.peek()==std::char_traits<char>::eof());assert(std::memcmp(rows.data(),base_rows.data(),rows.size()*164)==0&&std::memcmp(radii.data(),base_radii.data(),radii.size()*4)==0);
 CombatProperties896 props{};props.words[1]=44;props.words[32]=-1;CombatEquipSet8 set{};CombatInventory16 inventory{&set,1,0};std::int32_t output[]={0x12345678,0x23456789,0x3456789a};const auto saved=std::array<std::int32_t,3>{output[0],output[1],output[2]};float radius=123.f;std::uint32_t guards=0;
 auto guard=[&](int status){assert(status==-1);++guards;};
 guard(dh2_attack_equipment_melee_radius(nullptr,&inventory,rows.data(),rows.size()));
 guard(dh2_attack_equipment_melee_radius(output,nullptr,rows.data(),rows.size()));
 guard(dh2_attack_range_parameters(nullptr,&props,&inventory,rows.data(),rows.size()));
 guard(dh2_attack_range_parameters(output,nullptr,&inventory,rows.data(),rows.size()));
 guard(dh2_attack_range_parameters(props.words,&props,&inventory,rows.data(),rows.size()));
 guard(dh2_attack_melee_radius(nullptr,&props,&inventory,rows.data(),rows.size(),radii.data(),radii.size()));
 guard(dh2_attack_melee_radius(&radius,&props,&inventory,rows.data(),rows.size(),nullptr,radii.size()));
 guard(dh2_attack_melee_radius(&radius,&props,&inventory,rows.data(),rows.size(),radii.data(),8));
 guard(dh2_attack_melee_radius(radii.data(),&props,&inventory,rows.data(),rows.size(),radii.data(),radii.size()));
 inventory.current_set=-1;guard(dh2_attack_equipment_melee_radius(output,&inventory,rows.data(),rows.size()));inventory.current_set=0;
 CombatItemInstance4 missing{-1};const CombatItemInstance4* reference=&missing;set.main_hand=&reference;guard(dh2_attack_equipment_melee_radius(output,&inventory,rows.data(),rows.size()));missing.item_id=rows.size();guard(dh2_attack_equipment_melee_radius(output,&inventory,rows.data(),rows.size()));set.main_hand=nullptr;
 float point[3]={},reach[2]={60,40};std::int32_t limits[]={50,100};guard(dh2_attack_melee_distance(nullptr,point,reach));guard(dh2_attack_melee_distance(point,nullptr,reach));guard(dh2_attack_melee_distance(point,point,nullptr));guard(dh2_attack_ranged_distance(nullptr,point,limits));guard(dh2_attack_ranged_distance(point,nullptr,limits));guard(dh2_attack_ranged_distance(point,point,nullptr));
 assert(radius==123.f&&output[0]==saved[0]&&output[1]==saved[1]&&output[2]==saved[2]);props.words[32]=0;props.words[30]=-257;props.words[31]=257;assert(dh2_attack_range_parameters(output,&props,nullptr,nullptr,0)==1&&output[0]==-2&&output[1]==1&&output[2]==0);
 std::cout<<"{\"validation\":\"PASS\",\"comparisons\":"<<header[1]<<",\"operation_counts\":["<<totals[0]<<','<<totals[1]<<','<<totals[2]<<','<<totals[3]<<','<<totals[4]<<"],\"atomic_guards\":"<<guards<<",\"unused_inventory_short_circuit\":true,\"mismatches\":0}\n";
}
