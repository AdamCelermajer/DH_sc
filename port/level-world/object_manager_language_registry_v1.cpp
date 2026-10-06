#include "object_manager_language_registry_v1.hpp"
namespace dh2::world {
ObjectManagerLanguageRegistryV1::ObjectManagerLanguageRegistryV1(){relink();}
void ObjectManagerLanguageRegistryV1::relink()noexcept{
 character_end_.next=&character_end_;auto* previous=&character_end_;
 for(auto& character:characters_){previous->next=&character;previous=&character;}previous->next=&character_end_;
 object_end_={};object_end_.parent=&object_end_;object_end_.left=&object_end_;object_end_.right=&object_end_;
 // Source tree shape is private; traversal projects its signed-key in-order
 // semantics. Stable right-chain nodes supply the same successor order.
 ui::SettingsObjectNode32V1* prior=&object_end_;ui::SettingsObjectNode32V1* first=&object_end_;
 for(auto& pair:objects_){auto& entry=pair.second;entry.object={entry.borrow.identity,entry.borrow.source_type_f4,entry.borrow.type14_localization_valid819};entry.node={prior,nullptr,nullptr,&entry.object};if(prior==&object_end_)first=&entry.node;else prior->right=&entry.node;prior=&entry.node;}
 if(first!=&object_end_){object_end_.parent=first;object_end_.left=first;object_end_.right=prior;}
 scene_={&character_end_,&object_end_,first};
}
bool ObjectManagerLanguageRegistryV1::source_added(ObjectManagerLanguageBorrowV1 borrow,std::string& error){
 if(!borrow.identity||!borrow.owner||!borrow.source_type_f4||(borrow.as_character&&borrow.as_character!=borrow.identity)){error="Required canonical ObjectManager source registration unavailable";return false;}
 auto found=objects_.find(borrow.source_handle_key);
 if(found!=objects_.end()){if(found->second.borrow.identity!=borrow.identity){error="Conflicting source handle key requires original removal first";return false;}error="Duplicate source Add projection";return false;}
 if(objects_.size()>=65535){error="Source ObjectManager projection budget exceeded";return false;}
 const auto character=borrow.as_character;objects_.emplace(borrow.source_handle_key,Entry{std::move(borrow),{}, {}});
 if(character)characters_.push_back({nullptr,character});relink();error.clear();return true;
}
bool ObjectManagerLanguageRegistryV1::source_removed(std::int32_t key,std::string& error){auto found=objects_.find(key);if(found==objects_.end()){error="Source ObjectManager removal handle absent";return false;}
 const auto character=found->second.borrow.as_character;if(character)characters_.remove_if([&](const auto& entry){return entry.character==character;});objects_.erase(found);relink();error.clear();return true;}
void ObjectManagerLanguageRegistryV1::source_flush()noexcept{characters_.clear();objects_.clear();relink();}
}
