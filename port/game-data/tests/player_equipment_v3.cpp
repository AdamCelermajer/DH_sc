#include "../player_equipment_v3.hpp"
#include <algorithm>
#include <array>
#include <cstring>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <vector>
using namespace dh2::data;
using Raw=std::vector<std::uint8_t>;
static unsigned checks;
static void ck(bool x){++checks;if(!x)throw std::runtime_error("Equipment proof failed");}
static void w(Raw& b,std::uint32_t n){for(unsigned i=0;i<4;++i)b.push_back(std::uint8_t(n>>(8*i)));}
static std::uint32_t rd(const Raw& b,std::size_t& p){ck(p<=b.size()&&b.size()-p>=4);std::uint32_t v=0;for(unsigned i=0;i<4;++i)v|=std::uint32_t(b[p++])<<(8*i);return v;}
static std::int32_t si(std::uint32_t v){std::int32_t n;std::memcpy(&n,&v,4);return n;}
struct Fixture {
 std::array<EquipmentItem16V3,5> item{};std::array<EquipmentSlot16V3,5> slot{};std::array<EquipmentRow12V3,5> row{};
 std::array<EquipmentSlot16V3*,5> order{};std::array<std::array<EquipmentSlot16V3*,9>,2> eq{};
 EquipmentState72V3 state{};Raw requests;std::array<std::int32_t,56> values{};bool mutated{};unsigned nested{};int fail{-1},calls{};
 void setup(const std::array<std::int32_t,56>& v){values=v;for(unsigned i=0;i<5;++i){item[i]={std::int32_t(i),std::int16_t(v[8+i*6+3]),0,UINT64_C(0x123400000000)+i};slot[i]={&item[i],{std::int8_t(v[8+i*6+4]),std::int8_t(v[8+i*6+5])},{}};row[i]={v[8+i*6],v[8+i*6+1],std::uint32_t(v[8+i*6+2])};order[i]=&slot[i];}for(unsigned s=0;s<2;++s)for(unsigned i=0;i<9;++i)eq[s][i]=v[38+9*s+i]<0?nullptr:&slot[unsigned(v[38+9*s+i])];state={UINT64_C(0x123400005678),order.data(),4,v[4],{eq[0].data(),eq[1].data()},9,std::uint32_t(v[5]),std::uint32_t(v[6]),0,row.data(),5,0};}
 int label(const EquipmentItem16V3* p)const{return p?int(p-item.data()):-1;}
 Raw snapshot(){Raw b;w(b,state.count);for(unsigned i=0;i<state.count;++i)w(b,std::uint32_t(order[i]-slot.data()));for(unsigned i=0;i<5;++i){w(b,std::uint32_t(item[i].id));w(b,std::uint32_t(std::int32_t(item[i].quantity)));b.push_back(std::uint8_t(slot[i].slots[0]));b.push_back(std::uint8_t(slot[i].slots[1]));b.push_back(0);b.push_back(0);}w(b,std::uint32_t(state.selected));for(const auto& e:eq)for(auto* p:e)w(b,p?std::uint32_t(p-slot.data()):UINT32_MAX);return b;}
 static int invoke(void* context,EquipmentState72V3* s,const EquipmentRequest40V3* q,EquipmentResponse16V3* out){auto& f=*static_cast<Fixture*>(context);ck(s==&f.state&&q->owner==s->owner);if(f.calls++==f.fail)return -9;w(f.requests,q->operation);w(f.requests,q->caller);w(f.requests,std::uint32_t(f.label(q->item)));for(auto v:q->arguments)w(f.requests,std::uint32_t(v));auto label=f.label(q->item);
  if(q->operation==split){q->item->quantity=1;f.item[4].quantity=std::int16_t(q->arguments[0]);f.item[4].id=label;out->item=&f.item[4];}
  else if(q->operation==force_add){ck(s->count<5);f.order[s->count++]=&f.slot[4];}
  else if(q->operation==has_like){out->value=f.values[0]==3&&f.values[7]%3==1;out->index=label!=0?0:1;}
  else if(q->operation==is_equipped)out->value=f.values[7]%4==2;
  else if(q->operation==add_quantity)q->item->quantity=std::int16_t(std::int32_t(q->item->quantity)+q->arguments[0]);
  else if(q->operation==delete_item){auto end=f.order.begin()+s->count;auto at=std::find(f.order.begin(),end,&f.slot[unsigned(label)]);ck(at!=end);std::move(at+1,end,at);--s->count;}
  else ck(q->operation==update_gear_properties||q->operation==skin||q->operation==validate_hp_mp);
  if(f.values[7]%5==3&&!f.mutated){f.mutated=true;s->selected=1-f.values[4];}
  // Read-only genuine helper reentry observes the live selected set. No
  // detached fixture state machine or invented successful game effect.
  std::int32_t has=-1;ck(dh2_equipment_has_two_hander_v3(&has,s,1)==0);++f.nested;return 0;
 }
 int run(std::int32_t& result){EquipmentServices16V3 service{this,invoke};auto k=values[0];auto i=std::uint32_t(values[1]),s=std::uint32_t(values[2]),force=std::uint32_t(values[3]);switch(k){case 0:return dh2_equipment_auto_v3(&result,&state,i,&service);case 1:return dh2_equipment_character_auto_v3(&result,&state,i,&service);case 2:return dh2_equipment_to_slot_v3(&state,s,i,force,&service);case 3:return dh2_equipment_from_slot_v3(&state,s,-1,&service);case 4:return dh2_equipment_has_two_hander_v3(&result,&state,force);case 5:return dh2_equipment_slot_taken_v3(&result,&state,s);default:return -99;}}
};
int main(int argc,char** argv){try{ck(argc==2);std::ifstream in(argv[1],std::ios::binary);ck(bool(in));Raw gold((std::istreambuf_iterator<char>(in)),{});std::size_t at=0;ck(rd(gold,at)==0x33555145);auto cases=rd(gold,at);unsigned requests=0,nested=0,guards=0;std::array<std::int32_t,56> last{};
 for(unsigned k=0;k<cases;++k){std::array<std::int32_t,56> v{};for(auto& x:v)x=si(rd(gold,at));auto expected=si(rd(gold,at));auto size=rd(gold,at);ck(at<=gold.size()&&size<=gold.size()-at);Raw state(gold.begin()+at,gold.begin()+at+size);at+=size;auto n=rd(gold,at);ck(at<=gold.size()&&std::uint64_t(n)*28<=gold.size()-at);Raw trace(gold.begin()+at,gold.begin()+at+28*n);at+=28*n;Fixture f;f.setup(v);std::int32_t result=0;ck(f.run(result)==0);ck(result==expected&&f.snapshot()==state&&f.requests==trace);requests+=n;nested+=f.nested;last=v;}
 ck(at==gold.size());
 for(unsigned kind=0;kind<8;++kind){Fixture f;f.setup(last);EquipmentServices16V3 svc{&f,Fixture::invoke};auto saved=f.snapshot();std::int32_t out=0x12345678;if(kind==0)f.state.reserved=1;if(kind==1)f.state.reserved2=1;if(kind==2)f.state.selected=-1;if(kind==3)f.state.count=65537;if(kind==4)f.state.slots=10;if(kind==5)f.state.equipment[0]=nullptr;if(kind==6)f.state.table=nullptr;if(kind==7)svc.invoke=nullptr;ck(dh2_equipment_auto_v3(&out,&f.state,0,&svc)==-1);ck(out==0x12345678&&f.requests.empty());if(kind!=2&&kind!=3)ck(f.snapshot()==saved);++guards;}
 for(int failure=0;failure<3;++failure){Fixture f;last[0]=1;last[1]=0;last[9]=-1;f.setup(last);f.fail=failure;std::int32_t out=0x12345678;ck(f.run(out)==-2&&out==0x12345678);ck(f.calls==failure+1);++guards;}
 Fixture f;f.setup(last);EquipmentServices16V3 svc{&f,Fixture::invoke};auto saved=f.snapshot();std::int32_t out=99;ck(dh2_equipment_auto_v3(&out,&f.state,4,&svc)==-3&&out==99&&f.snapshot()==saved);++guards;ck(dh2_equipment_to_slot_v3(&f.state,9,0,0,&svc)==-3&&f.snapshot()==saved);++guards;ck(dh2_equipment_from_slot_v3(&f.state,0,3,&svc)==-3&&f.snapshot()==saved);++guards;
 for(unsigned kind=0;kind<6;++kind){Fixture f;f.setup(last);EquipmentServices16V3 svc{&f,Fixture::invoke};auto saved=f.snapshot();void* pointers[]={&f.state,f.order.data(),f.eq[0].data(),f.row.data(),f.slot.data(),&svc};auto* result=static_cast<std::int32_t*>(pointers[kind]);ck(dh2_equipment_character_auto_v3(result,&f.state,0,&svc)==-1&&f.requests.empty()&&f.snapshot()==saved);++guards;}
 for(unsigned kind=0;kind<2;++kind){Fixture f;f.setup(last);EquipmentServices16V3 svc{&f,Fixture::invoke};if(kind==0)f.slot[0].reserved[5]=1;else f.item[0].reserved=1;auto saved=f.snapshot();std::int32_t result=99;ck(dh2_equipment_character_auto_v3(&result,&f.state,0,&svc)==-1&&result==99&&f.requests.empty()&&f.snapshot()==saved);++guards;}
 std::cout<<"{\"validation\":\"PASS\",\"original_cases\":"<<cases<<",\"ordered_requests\":"<<requests<<",\"nested_readonly_services\":"<<nested<<",\"guards\":"<<guards<<",\"checks\":"<<checks<<",\"mismatches\":0}\n";return 0;
 }catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
