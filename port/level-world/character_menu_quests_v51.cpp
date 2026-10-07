#include "character_menu_quests_v51.hpp"
#include "native_quest_runtime_v76.hpp"
namespace dh2::character {
CharacterMenuQuestsV51::CharacterMenuQuestsV51(std::shared_ptr<data::PlayerSavegameV1> save,
 std::shared_ptr<const data::QuestTablesPersistenceV51> tables,
 std::function<bool(data::QuestPersistenceStateV51&,std::int32_t,std::string&)> reinit):
 save_(std::move(save)),tables_(std::move(tables)),reinit_(std::move(reinit)){}
bool CharacterMenuQuestsV51::initialize(std::uint32_t which,std::string& e){
 if(!save_||!tables_||which>1){e="Required SAME Save/actual Quest table/InitQuests selector";return false;}
 if(owners_[which].ready())return owners_[which].reinitialize(reinit_,e);
 auto& collection=which?save_->volatile_quests_v45():save_->regular_quests_v45();
 return owners_[which].construct(tables_,save_->character(),collection,e);
}
bool CharacterMenuQuestsV51::load_callback(void* c,std::uintptr_t id,data::Bytes b,bool,std::size_t& used,std::string& e){
 auto& self=*static_cast<CharacterMenuQuestsV51*>(c);
 for(auto& owner:self.owners_)if(owner.resolve(id))return owner.load_quest(id,b,used,e);
 used=0;e="Required SAME regular/volatile Quest identity";return false;
}
bool CharacterMenuQuestsV51::load(data::Bytes b,std::size_t& used,std::string& e){
 if(!save_||!owners_[0].ready()||!owners_[1].ready()){used=0;e="Required both source initialized Quest owners";return false;}
 return data::load_player_quests_v1(b,save_->regular_quests_v45(),save_->volatile_quests_v45(),{this,&load_callback},used,e);
}
bool CharacterMenuQuestsV51::save_quest(std::uintptr_t id,level::SavegameStreamV2& out,std::string& e){
 for(auto& owner:owners_)if(owner.resolve(id)){
  std::vector<std::uint8_t> bytes;if(!owner.save_quest(id,bytes,e))return false;
  return out.write({bytes.data(),bytes.size()},e);
 }
 e="Required SAME regular/volatile Quest writer identity";return false;
}
data::QuestPersistenceStateV51* CharacterMenuQuestsV51::resolve_v70(std::uintptr_t identity)const noexcept{
 for(const auto& owner:owners_)if(owner.ready())if(auto* quest=owner.resolve(identity))return quest;
 return nullptr;
}
bool CharacterMenuQuestsV51::bind_runtime_v76(std::shared_ptr<world::NativeQuestRuntimeV76> runtime,std::string& e){
 if(!runtime||runtime_v76_||!runtime->belongs_to(*this)){e="Actual Quest runtime publication requires an unproduced SAME owner slot";return false;}
 runtime_v76_=std::move(runtime);e.clear();return true;
}
bool CharacterMenuQuestsV51::compile_quest_v76(std::uintptr_t identity,std::string& e){
 if(!runtime_failure_v76_.empty()){e=runtime_failure_v76_;return false;}
 if(!runtime_v76_||!resolve_v70(identity)){e="Required SAME initialized Quest/native runtime";return false;}
 if(!runtime_v76_->compile(identity,e)){retain_runtime_failure_v76(e);return false;}return true;
}
level::QuestSaveCollectionBorrowV45 CharacterMenuQuestsV51::writer(std::uint32_t which){
 if(!save_||which>1||!owners_[which].ready())return {};
 auto& collection=which?save_->volatile_quests_v45():save_->regular_quests_v45();
 std::weak_ptr<CharacterMenuQuestsV51> weak=shared_from_this();
 return {shared_from_this(),&collection.source_quests_v45(),&collection.progress(),
  [weak,which](std::uintptr_t id,level::SavegameStreamV2& out,std::string& e){auto self=weak.lock();
   if(!self){e="Expired SAME Quest writer owner";return false;}std::vector<std::uint8_t> bytes;
   if(!self->owners_[which].save_quest(id,bytes,e))return false;return out.write({bytes.data(),bytes.size()},e);
  }};
}
bool CharacterMenuQuestsV51::destroy_collection_v108(std::uint32_t which,std::string& e){
 if(!save_||which>1){e="Required SAME QuestSavegame118/b8 D1 selector";return false;}
 auto& collection=which?save_->volatile_quests_v45():save_->regular_quests_v45();
 return owners_[which].destroy_source_v108(collection,[this](std::uintptr_t id,std::string& e){
  if(!runtime_v76_){e.clear();return true;} //selected uncompiled child D1 BXLR
  return runtime_v76_->destroy_quest_v108(id,e);
 },e);
}

}
