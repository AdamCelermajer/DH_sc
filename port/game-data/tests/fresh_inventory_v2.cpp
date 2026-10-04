#ifdef NDEBUG
#undef NDEBUG
#endif
#include "../fresh_inventory_v2.hpp"
#include <fstream>
#include <iostream>
#include <cstring>
using namespace dh2::data;using Raw=std::vector<std::uint8_t>;
static unsigned checks;static void ck(bool b){if(!b)throw std::runtime_error("Fresh inventory check "+std::to_string(checks));++checks;}
static Raw file(const std::string& p){std::ifstream f(p,std::ios::binary);ck(bool(f));return {std::istreambuf_iterator<char>(f),{}};}
static void w(Raw& b,std::uint32_t n){for(int i=0;i<4;++i)b.push_back(std::uint8_t(n>>(8*i)));}
static std::int32_t signed_word(std::uint32_t n){std::int32_t v;std::memcpy(&v,&n,4);return v;}
static std::uint32_t rd(const Raw& b,std::size_t& at){ck(at<=b.size()&&b.size()-at>=4);std::uint32_t n;std::memcpy(&n,b.data()+at,4);at+=4;return n;}
static Raw snap(const FreshInventoryV2& o){Raw b;w(b,0);w(b,o.current_equipment());w(b,o.items().size());auto potion=UINT32_MAX;for(std::size_t i=0;i<o.items().size();++i){const auto& s=*o.items()[i];const auto& item=*s.item;if(o.potion()==&item)potion=std::uint32_t(i);for(auto n:{std::uint32_t(item.id),std::uint32_t(item.quantity),std::uint32_t(item.value),std::uint32_t(item.identified),std::uint32_t(std::uint8_t(s.equipment_set)),std::uint32_t(std::uint8_t(s.equipment_slot)),std::uint32_t(item.powers.size())})w(b,n);for(auto n:item.powers)w(b,n);}w(b,potion);for(const auto& a:o.equipment())for(auto* s:a){auto id=UINT32_MAX;for(std::size_t i=0;i<o.items().size();++i)if(s==o.items()[i].get())id=std::uint32_t(i);w(b,id);}return b;}
struct Context {Raw requests;int mutation{},minimal{},fail{-1};unsigned delivered{},guards{};};
static bool service(void* p,FreshInventoryV2& owner,const FreshInventoryRequestV2& q,FreshInventoryResponseV2& out,std::string& error){auto& c=*static_cast<Context*>(p);if(int(c.delivered++)==c.fail){error="Required fixture effect failed";return false;}auto op=std::uint32_t(q.operation);auto caller=op==0x337888||op==0x337a88||op==0x31f594?q.source_caller:0;auto* i=q.item;auto arg=op==0x337a88&&q.source_caller==0x40411c&&c.minimal?1:0;for(auto n:{op,caller,i?std::uint32_t(i->id):0,i?std::uint32_t(i->quantity):0,i?std::uint32_t(i->value):0,std::uint32_t(arg),q.index})w(c.requests,n);
 if(op==0x337a88){ck(q.name);out.value=arg;}else if(op==0x3fb290&&c.mutation==1)i->id=925;else if(op==0x3fb754&&c.mutation==2&&i->value)i->id=925;
 if(op==0x3fb290){std::string nested;ck(!owner.add_fixed_loot(165,{&c,service},nested)&&!nested.empty());++c.guards;}
 return true;}
int main(int argc,char** argv){try{ck(argc==3);auto records=file(std::string(argv[2])+"/loot_table_pyarray.bin"),names=file(std::string(argv[2])+"/loot_table_pyarraynames.bin"),schema=file(std::string(argv[2])+"/loot_table_pystructnames.bin");LootTablesV2 table;std::string error;ck(table.load({records.data(),records.size()},{names.data(),names.size()},{schema.data(),schema.size()},error));auto b=table.borrow();auto gold=file(argv[1]);std::size_t at=0;ck(rd(gold,at)==0x32564946);auto cases=rd(gold,at);unsigned services=0,guards=0;
 for(unsigned k=0;k<cases;++k){LootRandom8V2 r{rd(gold,at),rd(gold,at)};auto cap=std::int8_t(rd(gold,at));Context c;c.minimal=rd(gold,at);c.mutation=rd(gold,at);auto loot=signed_word(rd(gold,at));auto seed=rd(gold,at),calls=rd(gold,at),size=rd(gold,at);ck(size<=gold.size()-at);Raw expected(gold.begin()+at,gold.begin()+at+size);at+=size;auto n=rd(gold,at);ck(n*28<=gold.size()-at);Raw requests(gold.begin()+at,gold.begin()+at+n*28);at+=n*28;FreshInventoryV2 owner(UINT64_C(0x123400005678),b,r,cap);ck(owner.add_fixed_loot(loot,{&c,service},error));auto actual=snap(owner);if(actual!=expected)throw std::runtime_error("State mismatch in original fresh case "+std::to_string(k));if(c.requests!=requests){std::size_t i=0;while(i<std::min(c.requests.size(),requests.size())&&c.requests[i]==requests[i])++i;throw std::runtime_error("Service mismatch case "+std::to_string(k)+" word "+std::to_string(i/4)+" row "+std::to_string(i/28));}ck(r.seed==seed&&r.calls==calls);services+=n;guards+=c.guards;
  if(!c.minimal){ck(owner.num_potions()==(cap? (c.mutation?1:5):0));if(!owner.items().empty()){auto* first=owner.items().front().get();ck(owner.record_delivered_equipment(0,0,0,error)&&owner.equipment()[0][0]==first);}ck(!owner.record_delivered_equipment(2,0,0,error));++guards;}
 }
 ck(at==gold.size());for(int fail=0;fail<20;++fail){LootRandom8V2 r{1,0};FreshInventoryV2 o(1,b,r,-1);Context c;c.fail=fail;ck(!o.add_fixed_loot(165,{&c,service},error)&&error=="Required fixture effect failed");++guards;}LootRandom8V2 r{1,0};FreshInventoryV2 o(1,b,r,-1);ck(!o.add_fixed_loot(165,{},error)&&o.items().empty()&&r.seed==1);++guards;
 std::cout<<"{\"validation\":\"PASS\",\"original_fresh_loot_cases\":"<<cases<<",\"source_services\":"<<services<<",\"guards\":"<<guards<<",\"checks\":"<<checks<<",\"mismatches\":0}\n";
 return 0;}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
