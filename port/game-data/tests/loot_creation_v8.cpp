#define main old_loot_power_main
#include "loot_power_creation_v7.cpp"
#undef main
#include "../loot_temporary_inventory_v8.hpp"
struct CreationContext {
 Context* effects{};LootTemporaryInventoryV8* destination{};std::int32_t difficulty{};bool level_present{true};std::vector<std::uint32_t> query_order;
 CreationContext(Context* e,LootTemporaryInventoryV8* d):effects(e),destination(d){}
 static bool entry(void* p,const LootEntryRequestV8& q,int& value,std::string& e){auto& c=*static_cast<CreationContext*>(p);if(q.operation==LootEntryOperationV8::debug_load||q.operation==LootEntryOperationV8::debug_query)return debug_call(*c.effects->env,q.operation==LootEntryOperationV8::debug_load,q.key,value);if(q.operation==LootEntryOperationV8::mage_count||q.operation==LootEntryOperationV8::rogue_count||q.operation==LootEntryOperationV8::warrior_count){value=1;return true;}e="Required original assertion fixture";return false;}
 static bool query(void* p,const LootCreationQueryV8& q,LootCreationResponseV8& out,std::string&){auto& c=*static_cast<CreationContext*>(p);c.query_order.push_back(q.caller);out.present=q.operation==LootCreationOperationV8::current_level&&c.level_present;out.value=q.operation==LootCreationOperationV8::player_count?1:q.caller==0x4043dc?c.difficulty:0;return true;}
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
 unsigned hard_names{},very_hard_names{},nonmatching_neighbors{};auto actual_loot_borrow=loots.borrow();const auto& item_table=actual_loot_borrow.items();
 for(std::size_t base=0;base<item_table.rows.size();++base){
  const bool in_range=base+2<item_table.rows.size();
  const bool hard=in_range&&item_table.identifiers[base+1]==item_table.identifiers[base]+"_Hard";
  const bool very_hard=in_range&&item_table.identifiers[base+2]==item_table.identifiers[base]+"_VeryHard";
  std::int32_t selected=-1;ck(select_loot_item_variant_v8(item_table,std::int32_t(base),0,false,selected,e)&&selected==std::int32_t(base));
  ck(select_loot_item_variant_v8(item_table,std::int32_t(base),1,false,selected,e)&&selected==std::int32_t(base)+(hard?1:0));
  ck(select_loot_item_variant_v8(item_table,std::int32_t(base),2,false,selected,e)&&selected==std::int32_t(base)+(very_hard?2:0));
  ck(select_loot_item_variant_v8(item_table,std::int32_t(base),1,true,selected,e)&&selected==std::int32_t(base));
  ck(select_loot_item_variant_v8(item_table,std::int32_t(base),2,true,selected,e)&&selected==std::int32_t(base));
  hard_names+=hard;very_hard_names+=very_hard;nonmatching_neighbors+=in_range&&!hard&&!very_hard;
 }
 ck(hard_names&&very_hard_names&&nonmatching_neighbors);
 auto cb=file(std::string(argv[2])+"/character_properties_pyarray.bin"),cn=file(std::string(argv[2])+"/character_properties_pyarraynames.bin"),cs=file(std::string(argv[2])+"/character_properties_pystructnames.bin");CharacterTable characters;ck(load_characters(bytes(cb),bytes(cn),bytes(cs),characters,e),e);
 Environment env;env.assets=argv[3];env.private_files=argv[4];ck(env.constants&&env.debug);auto constants=file(env.assets+"/original-cache/data/pydata/common_text_pycst.bin");dh2_script_constants_reload receipt{};ck(!dh2_script_constants_load(env.constants,constants.data(),constants.size(),&receipt));dh2::ui::HudTextV1 text;auto tb=file(env.assets+"/original-cache/data/pydata/common_text_pyarray.bin"),tn=file(env.assets+"/original-cache/data/pydata/common_text_pyarraynames.bin"),ts=file(env.assets+"/original-cache/data/pydata/common_text_pystructnames.bin");ck(text.load(localized_bytes(tb),localized_bytes(tn),localized_bytes(ts),e)&&text.switch_pack(0,false,e),e);dh2::ui::LocalizationServices loc{&env,open,close,text_debug,constant,nullptr,nullptr};dh2::ui::ItemTextOwnerV5 adapter(loots.borrow().items(),characters,text,{loc});ItemPresentationOwnerV5 presentation(definitions.borrow());Context effects{&env,&adapter,&presentation};
 auto gold=file(argv[5]);Reader r{gold};ck(r.u()==0x3856434c);ck(r.u()==loots.borrow().loots().size());
 LootRandom8V2 random{0x13579bdf,0};unsigned stored{},powered{};
 for(unsigned id=0;id<loots.borrow().loots().size();++id){LootTemporaryInventoryV8 destination(loots.borrow());CreationContext c{&effects,&destination};LootCreationV8 creation(loots.borrow(),resources.borrow(),definitions.borrow(),random);LootCreationServicesV8 services{{&c,CreationContext::entry},{&effects,power},adapter.services(),&c,CreationContext::query,CreationContext::create,CreationContext::store};
  if(!creation.add(int(id),0,0,-1,false,false,services,e)){std::cerr<<"table "<<id<<" "<<e<<'\n';if(auto* item=creation.pending_item())ck(presentation.forget(*item,e));return 1;}
  ck(!creation.pending_item());ck(r.u()==id);ck(r.u()==destination.items().size());for(auto& slot:destination.items()){ck(slot->item&&!slot->item->name.empty());auto& item=*slot->item;ck(r.u()==unsigned(item.id));ck(r.u()==unsigned(item.signed_quantity()));ck(r.u()==unsigned(item.value));ck(r.u()==item.powers.size());for(auto power_id:item.powers)ck(r.u()==unsigned(power_id));if(!item.powers.empty())++powered;++stored;ck(presentation.forget(item,e));}
  ck(r.u()==random.seed);ck(r.u()==random.calls);
 }
 auto run_difficulty=[&](unsigned loot_id,std::int32_t difficulty,bool level_present,std::vector<std::int32_t>& ids,std::vector<std::uint32_t>& query_order){
  LootTemporaryInventoryV8 destination(loots.borrow());CreationContext c{&effects,&destination};c.difficulty=difficulty;c.level_present=level_present;
  LootRandom8V2 local_random{0x13579bdf,0};LootCreationV8 creation(loots.borrow(),resources.borrow(),definitions.borrow(),local_random);
  LootCreationServicesV8 services{{&c,CreationContext::entry},{&effects,power},adapter.services(),&c,CreationContext::query,CreationContext::create,CreationContext::store};
  if(!creation.add(std::int32_t(loot_id),0,0,-1,false,false,services,e))throw std::runtime_error("Difficulty AddLoot native run: "+e);
  for(auto& slot:destination.items()){ck(slot->item&&!slot->item->name.empty());ids.push_back(slot->item->id);ck(presentation.forget(*slot->item,e));}
  query_order=c.query_order;
 };
 bool found_hard_variant=false,found_very_hard_variant=false,checked_null_level=false;unsigned difficulty_tables{},difficulty_runs{};
 for(unsigned loot_id=0;loot_id<loots.borrow().loots().size()&&!(found_hard_variant&&found_very_hard_variant&&checked_null_level);++loot_id){
  std::vector<std::int32_t> base_ids,hard_ids,very_hard_ids;std::vector<std::uint32_t> base_queries,hard_queries,very_hard_queries;
  run_difficulty(loot_id,0,true,base_ids,base_queries);if(base_ids.empty())continue;++difficulty_tables;
  if(!found_hard_variant){run_difficulty(loot_id,1,true,hard_ids,hard_queries);ck(hard_ids.size()==base_ids.size());bool changed=false;for(std::size_t i=0;i<base_ids.size();++i){std::int32_t expected{};ck(select_loot_item_variant_v8(item_table,base_ids[i],1,false,expected,e));ck(hard_ids[i]==expected);changed|=expected!=base_ids[i];}if(!hard_queries.empty()){for(std::size_t i=0;i<hard_queries.size();++i)if(hard_queries[i]==0x4043cc){ck(i+1<hard_queries.size()&&hard_queries[i+1]==0x4043dc);++difficulty_runs;}}found_hard_variant=changed;}
  if(!found_very_hard_variant){run_difficulty(loot_id,2,true,very_hard_ids,very_hard_queries);ck(very_hard_ids.size()==base_ids.size());bool changed=false;for(std::size_t i=0;i<base_ids.size();++i){std::int32_t expected{};ck(select_loot_item_variant_v8(item_table,base_ids[i],2,false,expected,e));ck(very_hard_ids[i]==expected);changed|=expected!=base_ids[i];}if(!very_hard_queries.empty()){for(std::size_t i=0;i<very_hard_queries.size();++i)if(very_hard_queries[i]==0x4043cc){ck(i+1<very_hard_queries.size()&&very_hard_queries[i+1]==0x4043dc);++difficulty_runs;}}found_very_hard_variant=changed;}
  if(!checked_null_level){std::vector<std::int32_t> null_ids;std::vector<std::uint32_t> null_queries;run_difficulty(loot_id,2,false,null_ids,null_queries);ck(null_ids==base_ids);for(auto site:null_queries)ck(site!=0x4043dc);checked_null_level=true;}
 }
 ck(found_hard_variant&&found_very_hard_variant&&checked_null_level&&difficulty_tables&&difficulty_runs);
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
 ck(r.at==gold.size());ck(env.opens==env.closes);std::cout<<"{\"validation\":\"PASS\",\"actual_cache_tables\":"<<loots.borrow().loots().size()<<",\"genuine_localized_items\":"<<stored<<",\"powered_items\":"<<powered<<",\"difficulty_variant_tables\":"<<difficulty_tables<<",\"difficulty_query_pairs\":"<<difficulty_runs<<",\"hard_and_very_hard_variants\":"<<(found_hard_variant&&found_very_hard_variant)<<",\"null_level_kept_base_item\":"<<checked_null_level<<",\"shared_random_calls\":"<<random.calls<<",\"checks\":"<<checks<<",\"player_count_and_difficulty_fixture\":true,\"world_spawn_pickup\":false}\n";return 0;
 }catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
