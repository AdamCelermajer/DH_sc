#define main old_loot_power_main
#include "loot_power_creation_v7.cpp"
#undef main
#include "../loot_temporary_inventory_v8.hpp"
struct CreationContext {
 Context* effects{};LootTemporaryInventoryV8* destination{};
 static bool entry(void* p,const LootEntryRequestV8& q,int& value,std::string& e){auto& c=*static_cast<CreationContext*>(p);if(q.operation==LootEntryOperationV8::debug_load||q.operation==LootEntryOperationV8::debug_query)return debug_call(*c.effects->env,q.operation==LootEntryOperationV8::debug_load,q.key,value);if(q.operation==LootEntryOperationV8::mage_count||q.operation==LootEntryOperationV8::rogue_count||q.operation==LootEntryOperationV8::warrior_count){value=1;return true;}e="Required original assertion fixture";return false;}
 static bool query(void*,const LootCreationQueryV8& q,LootCreationResponseV8& out,std::string&){out.present=q.operation==LootCreationOperationV8::current_level;out.value=q.operation==LootCreationOperationV8::player_count?1:0;return true;}
 static bool create(void* p,int id,std::unique_ptr<ItemInstanceV1>& out,std::string& e){auto& c=*static_cast<CreationContext*>(p);return c.destination->create(id,out,c.effects->text->services(),e);}
 static bool store(void* p,std::unique_ptr<ItemInstanceV1>& out,std::string& e){auto& c=*static_cast<CreationContext*>(p);return c.destination->store(out,{p,entry},nullptr,nullptr,e);}
};
struct TransferContextV8 {
 std::unique_ptr<ItemInstanceV1> destination;bool consume{},fail{},force{},convert{};
 static bool add(void* p,std::unique_ptr<ItemInstanceV1>& item,bool force,bool convert,int& result,std::string& e){
  auto& c=*static_cast<TransferContextV8*>(p);c.force=force;c.convert=convert;
  if(c.consume)c.destination=std::move(item);result=7;
  if(c.fail){e="reached destination failure";return false;}return true;
 }
};
int main(int argc,char** argv){try{
 ck(argc==6);std::string e;std::array<Raw,9> raw;const char* names[]={"item_powers_pyarray.bin","item_powers_pyarraynames.bin","item_powers_pystructnames.bin","item_powers_monopoly_pyarray.bin","item_powers_monopoly_pyarraynames.bin","item_powers_monopoly_pystructnames.bin","num_prob_records_v7.bin","loot_table_pyarraynames.bin","loot_table_pystructnames.bin"};for(unsigned i=0;i<9;++i)raw[i]=file(std::string(argv[1])+"/"+names[i]);LootPowerInputsV7 input{bytes(raw[0]),bytes(raw[1]),bytes(raw[2]),bytes(raw[3]),bytes(raw[4]),bytes(raw[5]),bytes(raw[6]),bytes(raw[7]),bytes(raw[8])};
 ItemPowerTablesV5 definitions;ck(definitions.load(input.powers,input.power_names,input.power_schema,e),e);LootPowerResourcesV7 resources;ck(resources.load(input,definitions.borrow(),e),e);
 auto lb=file(std::string(argv[1])+"/loot_table_pyarray.bin");LootTablesV2 loots;ck(loots.load(bytes(lb),input.loot_names,input.loot_schema,e),e);
 auto cb=file(std::string(argv[2])+"/character_properties_pyarray.bin"),cn=file(std::string(argv[2])+"/character_properties_pyarraynames.bin"),cs=file(std::string(argv[2])+"/character_properties_pystructnames.bin");CharacterTable characters;ck(load_characters(bytes(cb),bytes(cn),bytes(cs),characters,e),e);
 Environment env;env.assets=argv[3];env.private_files=argv[4];ck(env.constants&&env.debug);auto constants=file(env.assets+"/original-cache/data/pydata/common_text_pycst.bin");dh2_script_constants_reload receipt{};ck(!dh2_script_constants_load(env.constants,constants.data(),constants.size(),&receipt));dh2::ui::HudTextV1 text;auto tb=file(env.assets+"/original-cache/data/pydata/common_text_pyarray.bin"),tn=file(env.assets+"/original-cache/data/pydata/common_text_pyarraynames.bin"),ts=file(env.assets+"/original-cache/data/pydata/common_text_pystructnames.bin");ck(text.load(localized_bytes(tb),localized_bytes(tn),localized_bytes(ts),e)&&text.switch_pack(0,false,e),e);dh2::ui::LocalizationServices loc{&env,open,close,text_debug,constant,nullptr,nullptr};dh2::ui::ItemTextOwnerV5 adapter(loots.borrow().items(),characters,text,{loc});ItemPresentationOwnerV5 presentation(definitions.borrow());Context effects{&env,&adapter,&presentation};
 auto gold=file(argv[5]);Reader r{gold};ck(r.u()==0x3856434c);ck(r.u()==loots.borrow().loots().size());
 LootRandom8V2 random{0x13579bdf,0};unsigned stored{},powered{};
 for(unsigned id=0;id<loots.borrow().loots().size();++id){LootTemporaryInventoryV8 destination(loots.borrow());CreationContext c{&effects,&destination};LootCreationV8 creation(loots.borrow(),resources.borrow(),definitions.borrow(),random);LootCreationServicesV8 services{{&c,CreationContext::entry},{&effects,power},adapter.services(),&c,CreationContext::query,CreationContext::create,CreationContext::store};
  if(!creation.add(int(id),0,0,-1,false,false,services,e)){std::cerr<<"table "<<id<<" "<<e<<'\n';if(auto* item=creation.pending_item())ck(presentation.forget(*item,e));return 1;}
  ck(!creation.pending_item());ck(r.u()==id);ck(r.u()==destination.items().size());for(auto& slot:destination.items()){ck(slot->item&&!slot->item->name.empty());auto& item=*slot->item;ck(r.u()==unsigned(item.id));ck(r.u()==unsigned(item.signed_quantity()));ck(r.u()==unsigned(item.value));ck(r.u()==item.powers.size());for(auto power_id:item.powers)ck(r.u()==unsigned(power_id));if(!item.powers.empty())++powered;++stored;ck(presentation.forget(item,e));}
  ck(r.u()==random.seed);ck(r.u()==random.calls);
 }
 for(unsigned mode=0;mode<4;++mode){
  LootTemporaryInventoryV8 source(loots.borrow());CreationContext c{&effects,&source};std::unique_ptr<ItemInstanceV1> incoming;
  ck(source.create(0,incoming,adapter.services(),e),e);auto* identity=incoming.get();ck(presentation.forget(*identity,e));
  if(mode==3)incoming->quantity=0;
  ck(source.store(incoming,{&c,CreationContext::entry},nullptr,nullptr,e),e);ck(!incoming&&source.peek()==identity);
  TransferContextV8 target;target.consume=mode!=1;target.fail=mode==1||mode==2;int result=-1;
  bool ok=source.transfer(0,true,false,&target,TransferContextV8::add,result,e);
  if(mode==0){ck(ok&&result==7&&source.items().empty());ck(target.destination.get()==identity&&target.force&&!target.convert);}
  if(mode==1||mode==2){ck(!ok&&e=="reached destination failure"&&source.items().size()==1&&source.peek()==identity);ck((mode==1)==bool(source.items()[0]->item));ck(!source.transfer(0,true,false,&target,TransferContextV8::add,result,e));}
  if(mode==3){ck(ok&&result==0&&source.peek()==identity&&!target.destination&&!target.force);}
 }
 ck(r.at==gold.size());ck(env.opens==env.closes);std::cout<<"{\"validation\":\"PASS\",\"actual_cache_tables\":"<<loots.borrow().loots().size()<<",\"genuine_localized_items\":"<<stored<<",\"powered_items\":"<<powered<<",\"shared_random_calls\":"<<random.calls<<",\"checks\":"<<checks<<",\"player_count_and_difficulty_fixture\":true,\"world_spawn_pickup\":false}\n";return 0;
 }catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
