#include "../loot_power_creation_v7.hpp"
#include <cstring>
using namespace dh2::data;
namespace {
void word(std::vector<std::uint8_t>& b,std::uint32_t x){auto* p=reinterpret_cast<const std::uint8_t*>(&x);b.insert(b.end(),p,p+4);}
std::uint32_t read(const std::uint8_t*& p){std::uint32_t n;std::memcpy(&n,p,4);p+=4;return n;}
Bytes block(const std::uint8_t*& p){auto n=read(p);auto* b=p;p+=n;return {b,n};}
bool text(void*,ItemInstanceV1&,const ItemTextRequestV5& q,ItemTextResponseV5& r,std::string& out,std::string& e){
 if(q.operation==ItemTextOperationV5::integer_string){r.text="OID"+std::to_string(q.value);return true;}
 if(q.operation==ItemTextOperationV5::parse_ex){out+=q.input;for(unsigned i=0;i<q.count;++i){std::uint32_t bits;std::memcpy(&bits,&q.arguments[i].number,4);out+=':'+std::to_string(bits)+'/'+std::to_string(q.arguments[i].integer);}return true;}
 e="Required isolated formatter service missing";return false;
}
struct Context{ItemPresentationOwnerV5* owner;ItemTextServicesV5 text;std::vector<std::uint8_t> trace;};
bool invoke(void* c,const LootPowerRequestV7& q,std::int32_t& r,std::string& e){
 auto& x=*static_cast<Context*>(c);word(x.trace,std::uint32_t(q.operation));word(x.trace,q.source_caller);word(x.trace,std::uint32_t(q.power));
 if(q.operation==LootPowerOperationV7::add_power)return x.owner->add_power(*q.item,q.power,q.difficulty,x.text,e);
 if(q.operation==LootPowerOperationV7::debug_query&&(!q.key||std::strcmp(q.key,"isTracingItemInventory_Loot"))){e="Incorrect source debug key";return false;}
 r=0;return true;
}
}
// Controlled formatting and Debug services isolate original coordinator branches;
// resource decoding, RNG, conflict rules and actual V5 Power append execute.
extern "C" std::uint32_t dh2_loot_power_creation_fixture_v7(const std::uint8_t* input,std::uint8_t* output){
 auto* p=input;LootPowerInputsV7 b;Bytes* fields[]={&b.powers,&b.power_names,&b.power_schema,&b.monopoly,&b.monopoly_names,&b.monopoly_schema,&b.quantities,&b.loot_names,&b.loot_schema};for(auto* f:fields)*f=block(p);
 ItemPowerTablesV5 definitions;LootPowerResourcesV7 resources;std::string e;
 if(!definitions.load(b.powers,b.power_names,b.power_schema,e)||!resources.load(b,definitions.borrow(),e))return UINT32_MAX;
 auto count=read(p);std::vector<std::uint8_t> result;
 while(count--){
  LootRandom8V2 random{read(p),read(p)};LootEntry32V2 entry{};entry.words[1]=static_cast<std::int32_t>(read(p));entry.words[3]=static_cast<std::int32_t>(read(p));
  const auto bonus=static_cast<std::int32_t>(read(p)),requested=static_cast<std::int32_t>(read(p)),difficulty=static_cast<std::int32_t>(read(p));
  ItemInstanceV1 item;ItemPresentationOwnerV5 powers(definitions.borrow());Context ctx{&powers,{nullptr,nullptr,text},{}};
  auto initial=read(p);while(initial--){auto id=read(p),mode=read(p);if(!powers.add_power(item,static_cast<std::int32_t>(id),static_cast<std::int32_t>(mode),ctx.text,e))return UINT32_MAX;}
  LootPowerCreationV7 creation(resources.borrow(),random);if(!creation.add_powers(entry,item,bonus,requested,difficulty,{&ctx,invoke},e))return UINT32_MAX;
  std::vector<std::uint8_t> row;word(row,random.seed);word(row,random.calls);word(row,ctx.trace.size()/12);row.insert(row.end(),ctx.trace.begin(),ctx.trace.end());
  auto* state=powers.powers(item);word(row,state?state->size():0);if(state)for(const auto& x:*state){word(row,std::uint32_t(x.id));word(row,std::uint32_t(x.sorting_order));word(row,x.description.size());row.insert(row.end(),x.description.begin(),x.description.end());}
  word(result,row.size());result.insert(result.end(),row.begin(),row.end());
 }
 std::memcpy(output,result.data(),result.size());return std::uint32_t(result.size());
}
