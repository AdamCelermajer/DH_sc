#include "../item_inventory_v1.hpp"
#include "../player_savegame_v1.hpp"
#include <fstream>
#include <iostream>
#include <stdexcept>
using namespace dh2::data;using B=std::vector<std::uint8_t>;
void check(bool x,const char* why){if(!x)throw std::runtime_error(why);}
B file(const std::string& p){std::ifstream f(p,std::ios::binary);check(bool(f),"missing inventory input");return B(std::istreambuf_iterator<char>(f),{});}
void w(B& b,std::uint32_t x){for(int i=0;i<4;++i)b.push_back(std::uint8_t(x>>(8*i)));}
struct R{const B& b;std::size_t p{};std::uint32_t word(){check(p+4<=b.size(),"short gold");std::uint32_t x=0;for(int i=0;i<4;++i)x|=std::uint32_t(b[p++])<<(8*i);return x;}B bytes(std::size_t n){check(n<=b.size()-p,"short gold payload");B x(b.begin()+p,b.begin()+p+n);p+=n;return x;}};
B snapshot(ItemInventoryV1& inv){B out;w(out,inv.gold());w(out,std::uint8_t(inv.current_equipment()));w(out,inv.items().size());std::uint32_t potion=UINT32_MAX;
 for(std::uint32_t i=0;i<inv.items().size();++i){auto& slot=*inv.items()[i];auto& item=*slot.item;if(&item==inv.potion())potion=i;
  for(auto word:{std::uint32_t(item.id),std::uint32_t(item.quantity),std::uint32_t(item.value),std::uint32_t(item.identified),std::uint32_t(std::uint8_t(slot.equipment_set)),std::uint32_t(std::uint8_t(slot.equipment_slot))})w(out,word);
  w(out,item.powers.size());for(auto power:item.powers)w(out,power);
 }w(out,potion);for(auto& set:inv.equipment())for(auto slot:set){std::uint32_t found=UINT32_MAX;for(std::uint32_t i=0;i<inv.items().size();++i)if(slot&&inv.items()[i].get()==slot)found=i;w(out,found);}return out;
}
struct Context{B trace;bool fail{};std::uint32_t fail_at{},calls{};bool nested_rejected{};const InventoryServicesV1* services{};};
bool service(void* ptr,ItemInventoryV1& inv,const InventoryRequestV1& q,InventoryResponseV1& response,std::string& error){auto& c=*static_cast<Context*>(ptr);auto item=q.item;
 for(auto word:{std::uint32_t(q.operation),item?std::uint32_t(item->id):0,item?std::uint32_t(item->quantity):0,item?std::uint32_t(item->value):0,std::uint32_t(q.argument),q.index,q.set})w(c.trace,word);
 ++c.calls;if(c.fail&&c.calls==c.fail_at){error="required controlled service rejected";return false;}
 if(q.operation==InventoryOperationV1::add_power)item->powers.push_back(q.argument);
 if(q.operation==InventoryOperationV1::inventory_full)response.flag=false;
 if(q.operation==InventoryOperationV1::equip_item)check(inv.record_delivered_equipment(q.set,std::uint32_t(q.argument),q.index,error),"fixture equipment identity differs");
 if(c.services&&!c.nested_rejected){InventoryLoadReceiptV1 receipt;std::string e;std::uint8_t b[12]{};check(!inv.load_section({b,12},{},*c.services,receipt,e),"reentrant inventory load accepted");c.nested_rejected=true;}
 return true;
}
int main(int argc,char** argv){try{check(argc==3,"inventory audit args");auto gold=file(argv[1]);R r{gold};check(r.bytes(4)==B({'I','N','V','1'}),"gold magic");std::vector<std::string> powers;auto n=r.word();while(n--){auto s=r.bytes(r.word());powers.emplace_back(s.begin(),s.end());}
 std::string root=argv[2],error;auto records=file(root+"/loot_table_pyarray.bin"),names=file(root+"/loot_table_pyarraynames.bin"),schema=file(root+"/loot_table_pystructnames.bin");ItemTable table;check(load_items({records.data(),records.size()},{names.data(),names.size()},{schema.data(),schema.size()},table,error),"real ItemTable load failed");
 auto count=r.word();std::uint32_t calls=0,checks=0;B first;int firstcap=0;
 for(std::uint32_t i=0;i<count;++i){auto cap=static_cast<std::int8_t>(r.word());auto blob=r.bytes(r.word());auto expected=r.bytes(r.word());auto trace=r.bytes(std::size_t(r.word())*28);if(!i){first=blob;firstcap=cap;}
  ItemInventoryV1 inv(table);inv.set_character(UINT64_C(0x100000055));inv.project_potion_capacity(cap);check(inv.items().empty()&&inv.num_potions()==0&&inv.gold()==0&&inv.gold_limit()==INT32_MAX,"blank ctor differs");for(auto& set:inv.equipment())for(auto slot:set)check(!slot,"constructor equipment differs");
  Context c;InventoryServicesV1 s{&c,service};c.services=&s;InventoryLoadReceiptV1 receipt;check(inv.load_section({blob.data(),blob.size()},powers,s,receipt,error)&&receipt.completed&&receipt.consumed==blob.size(),"load failed");
  auto actual=snapshot(inv);if(actual!=expected)std::cerr<<"inventory snapshot case "<<i<<" sizes "<<actual.size()<<'/'<<expected.size()<<'\n';check(actual==expected,"inventory snapshot differs");
  if(c.trace!=trace){std::cerr<<"trace case "<<i<<" sizes "<<c.trace.size()<<'/'<<trace.size()<<'\n';for(std::size_t j=0;j<c.trace.size()&&j<trace.size();++j)if(c.trace[j]!=trace[j]){std::cerr<<"first trace byte "<<j<<" got "<<int(c.trace[j])<<" expected "<<int(trace[j])<<'\n';break;}}check(c.trace==trace,"source inventory order differs");calls+=c.calls;++checks;
  if(inv.potion())check(inv.num_potions()==dh2_inventory_v1_quantity(inv.potion()->quantity),"signed potion getter differs");
 }
 check(r.p==gold.size(),"gold suffix");std::uint32_t guards=0;ItemInventoryV1 a(table);Context c;InventoryServicesV1 s{&c,service};InventoryLoadReceiptV1 receipt;
 check(!a.load_section({first.data(),first.size()},powers,s,receipt,error)&&a.items().empty(),"owner gate");++guards;a.set_character(1);InventoryServicesV1 absent;
 check(!a.load_section({first.data(),first.size()},powers,absent,receipt,error)&&a.items().empty(),"service gate");++guards;
 for(unsigned j=0;j<12;++j){ItemInventoryV1 b(table);b.set_character(1);Context d;InventoryServicesV1 bound{&d,service};check(!b.load_section({first.data(),j},powers,bound,receipt,error)&&b.gold()==0&&d.calls==0,"truncated header wrote");++guards;}
 c.fail=true;c.fail_at=2;a.project_potion_capacity(static_cast<std::int8_t>(firstcap));check(!a.load_section({first.data(),first.size()},powers,s,receipt,error)&&c.calls==2&&!receipt.completed,"required failure suppressed");++guards;
 std::cout<<"{\"validation\":\"PASS\",\"original_load_cases\":"<<checks<<",\"ordered_required_service_deliveries\":"<<calls<<",\"guards\":"<<guards<<",\"source_item_rows\":"<<table.rows.size()<<",\"mismatches\":0,\"deeper_game_services\":\"controlled fixtures\"}\n";
 return 0;}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
