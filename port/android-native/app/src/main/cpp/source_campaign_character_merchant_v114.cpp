#include "source_campaign_character_interaction_v114.hpp"
#include "renderer_character_campaign_v62.hpp"
#include "source_campaign_items_v88.hpp"
#include <canonical_character_candidate_v60.hpp>
#include <item_presentation_v5.hpp>
#include <loot_creation_v8.hpp>
#include <character_design_services.hpp>
#include <stdexcept>
namespace model_renderer {namespace {
using Record=dh2::world::CanonicalCharacterCandidateRecordV60;
class MerchantInventoryV114 {
 std::weak_ptr<Record> record_;
 dh2::data::FreshInventoryOwnedV4* inventory_{};
 std::shared_ptr<dh2::character::SourceItemResourcesV88> resources_;
 std::shared_ptr<void> source_provider_;
 dh2::character::WorldItemLootCreationServicesV10 source_;
 dh2::data::LootCreationV8 creation_;
 bool failed_{};
 static bool effect(void* raw,dh2::data::FreshInventoryOwnedV4& inventory,const dh2::data::OwnedInventoryRequestV4& q,dh2::data::OwnedInventoryResponseV4& out,std::string& e){
  auto& t=*static_cast<MerchantInventoryV114*>(raw);auto r=t.record_.lock();if(!r||r->inventory37c!=&inventory||t.inventory_!=&inventory){e="Changed SAME Merchant Inventory37c effect receiver";return false;}
  using O=dh2::data::OwnedInventoryOperationV4;
  switch(q.operation){
   case O::debug_load:if(!r->services.debug||!r->services.debug_files)return false;return dh2_character_debug_load(r->services.debug,r->services.debug_files)==1;
   case O::debug_query:{std::uint32_t value{};if(!q.name||!r->services.debug||!r->services.debug_files||dh2_character_debug_get(&value,r->services.debug,q.name,r->services.debug_files)!=1)return false;out.value=static_cast<std::int32_t>(value);return true;}
   case O::update_name:return q.item&&dh2::data::item_update_name_v5(*q.item,t.resources_->text(),e);
   case O::update_stats:return q.item&&dh2::data::item_update_stats_v5(*q.item,t.resources_->text(),e);
   case O::update_requirements:return q.item&&dh2::data::item_update_requirements_v5(*q.item,t.resources_->text(),e);
   case O::add_power:return q.item&&t.resources_->presentation()->add_power(*q.item,q.argument,static_cast<std::int32_t>(q.index),t.resources_->text(),e);
   default:e="Required actual Merchant owned Inventory continuation at "+std::to_string(q.source_caller);return false;
  }
 }
 static void observe(void* raw,dh2::data::FreshInventoryOwnedV4&,const dh2::data::OwnedInventoryRequestV4& q){
  auto& t=*static_cast<MerchantInventoryV114*>(raw);if(q.operation==dh2::data::OwnedInventoryOperationV4::destroy_item&&q.item){std::string e;if(!t.resources_->presentation()->forget(*q.item,e))throw std::runtime_error(e);}
 }
 dh2::data::OwnedInventoryServicesV4 effects(){return {this,effect,observe};}
 static bool create(void* raw,std::int32_t id,std::unique_ptr<dh2::data::ItemInstanceV1>& out,std::string& e){auto& t=*static_cast<MerchantInventoryV114*>(raw);auto r=t.record_.lock();
  if(!r||r->inventory37c!=t.inventory_){e="Retired actual Merchant Item constructor destination";return false;}return t.inventory_->create_item(id,1,out,t.effects(),e);}
 static bool store(void* raw,std::unique_ptr<dh2::data::ItemInstanceV1>& item,std::string& e){auto& t=*static_cast<MerchantInventoryV114*>(raw);auto r=t.record_.lock();std::int32_t index{};
  if(!r||r->inventory37c!=t.inventory_){e="Retired actual Merchant AddItemInstance destination";return false;}return t.inventory_->add_item(item,true,false,index,t.effects(),e);}
 static bool query(void* raw,const dh2::data::LootCreationQueryV8& q,dh2::data::LootCreationResponseV8& out,std::string& e){auto& t=*static_cast<MerchantInventoryV114*>(raw);const auto& s=t.source_.source;
  if(!s.query){e="Required SAME actual AddLoot Application/PM query";return false;}return s.query(s.context,q,out,e);}
public:
 MerchantInventoryV114(std::shared_ptr<Record> r,std::shared_ptr<dh2::character::SourceItemResourcesV88> resources,
  dh2::character::WorldItemLootCreationServicesV10 source,std::shared_ptr<void> provider):record_(r),inventory_(r->inventory37c),resources_(std::move(resources)),source_provider_(std::move(provider)),source_(std::move(source)),
  creation_(resources_->loot(),resources_->powers(),resources_->definitions(),*inventory_->random_v60()){}
 ~MerchantInventoryV114(){auto r=record_.lock();if(r&&r->inventory37c==inventory_)for(const auto& slot:inventory_->items())if(slot&&slot->item){std::string ignored;resources_->presentation()->forget(*slot->item,ignored);}
  if(auto pending=creation_.pending_item()){std::string ignored;resources_->presentation()->forget(*pending,ignored);}}
 bool add(std::int32_t id,std::string& e){if(failed_){e="Merchant AddLoot failed source prefix cannot replay";return false;}auto r=record_.lock();if(!r||r->inventory37c!=inventory_){e="Changed actual Merchant Inventory37c";return false;}
  auto calls=source_.source;calls.context=this;calls.create=create;calls.store=store;calls.query=query;
  try{if(!creation_.add(id,0,0,-1,false,inventory_->source_give_all_v114(),calls,e)){failed_=true;return false;}return true;}catch(const std::exception& x){failed_=true;e=x.what();return false;}}
};
}
bool source_campaign_character_merchant_stock_v114(const std::shared_ptr<void>& world,std::uintptr_t id,std::int32_t table,std::string& e){
 SourceCampaignCharacterBorrowV62 actor;if(!borrow_source_campaign_character_v62(world,id,actor,e)||!actor.character->inventory37c)return false;auto r=actor.character;
 if(!r->inventory37c->items().empty())return true; //source GetNumItems gate
 std::shared_ptr<dh2::character::SourceItemResourcesV88> resources;if(!borrow_source_campaign_item_resources_v88(world,resources,e)||!resources||!resources->ready())return false;
 if(table<0||std::size_t(table)>=resources->merchants_v114().size()){e="Required actual Arrays.MerchantTable row selected by GetLoot101c";return false;}
 const auto& row=resources->merchants_v114()[table];if(row.entries.empty())return true;
 if(!r->inventory37c->random_v60()){e="Required actual Merchant inventory Application RNG borrow";return false;}
 if(!r->merchant_inventory_context_v114){dh2::character::WorldItemLootCreationServicesV10 source;std::shared_ptr<void> provider;
  if(!borrow_source_campaign_inventory_creation_v114(world,source,provider,e)||!provider)return false;
  r->merchant_inventory_context_v114=std::make_shared<MerchantInventoryV114>(r,resources,std::move(source),std::move(provider));}
 auto native=std::static_pointer_cast<MerchantInventoryV114>(r->merchant_inventory_context_v114);
 //Original3a4e80..b4 reads Merchandise8 only. It does NOT test ConditionId4.
 for(const auto& entry:row.entries)if(!native->add(entry.merchandise8,e))return false;e.clear();return true;
}
}
