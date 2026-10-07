#pragma once
#include "character_menu_quests_v51.hpp"
#include "native_conditions_v69.hpp"
#include "../level-loader/game_event_runtime_v75.hpp"
namespace dh2::world {
struct NativeQuestFrameServicesV108 {
 std::shared_ptr<void> provider;
 std::function<bool(bool&,std::string&)> frame_gate;
 std::function<bool(const char*,const char*,std::int32_t&,std::string&)> constant;
 std::function<bool(std::uint32_t&,std::string&)> application_time70;
 std::function<bool(const char*,std::int32_t&,std::string&)> script_id;
 std::function<bool(std::int32_t,bool&,std::string&)> script_running;
 std::function<bool(std::int32_t,std::int32_t,bool,std::string&)> start_script;
 std::function<bool(std::string&)> transition_save;
 std::function<bool(std::int32_t,std::int32_t,std::string&)> current_quest,current_primary;
 std::function<bool(std::int32_t,std::int32_t,std::string&)> current_act;
 std::function<bool(const data::QuestPersistenceStateV51&,std::string&)> new_dialog;
 std::function<bool(const data::QuestPersistenceStateV51&,const std::vector<data::QuestRewardDefinitionV51>&,std::string&)> completed_dialog;
 std::function<bool(std::uintptr_t,const data::QuestRewardDefinitionV51&,bool&,std::string&)> give_reward;
 std::function<bool(std::string&)> online_activation,online_act;
 std::function<bool(bool&,std::string&)> online;
 std::function<bool(std::uintptr_t,bool&,std::string&)> is_local;
 std::function<bool(std::uintptr_t,std::string&)> all_quests_trophy;
};
struct NativeQuestRuntimeServicesV76 {
 std::shared_ptr<void> provider;
 std::shared_ptr<loader::GameEventRuntimeV75> objectives;
 std::shared_ptr<NativeConditionRuntimeV69> conditions;
 //Only the selected TalkToNPC subclass overrides the captured literal BX LR
 //base marker operation. Actual NPC/model provider implements 47d8e8.
 std::function<bool(data::QuestObjectivePersistenceV51&,std::int32_t priority,
                    std::int32_t state,std::string&)> install_talk_marker;
 std::function<bool(data::QuestObjectivePersistenceV51&,std::string&)> remove_talk_marker;
};
//Quest identities, state0 and Objective.saved14/20 stay on the existing
//CharacterMenuQuests/Save collections. This owner adds their real native
//runtime children and immutable row projections; it owns no saved-state copy.
class NativeQuestRuntimeV76 final {
 struct Record;
 std::weak_ptr<character::CharacterMenuQuestsV51> quests_;
 NativeQuestRuntimeServicesV76 services_;NativeQuestFrameServicesV108 frame_v108_;
 std::map<std::uintptr_t,std::shared_ptr<Record>> records_;
 bool busy_{},failed_{};std::string failure_;
 bool fail(std::string&,const char*);
 bool prepare(std::uintptr_t,std::shared_ptr<Record>&,std::string&);
 bool marker(Record&,data::QuestObjectivePersistenceV51&,std::int32_t,std::string&);
 bool set_state_v108(Record&,const std::shared_ptr<character::CharacterMenuQuestsV51>&,std::int32_t,std::string&);
 bool script_v108(Record&,std::int32_t,bool execute,bool& running,std::string&);
public:
 NativeQuestRuntimeV76(std::weak_ptr<character::CharacterMenuQuestsV51>,NativeQuestRuntimeServicesV76);
 bool compile(std::uintptr_t actual_quest,std::string&);
 bool prerequisites(std::uintptr_t actual_quest,bool&,std::string&);
 bool destroy_quest_v108(std::uintptr_t actual_quest,std::string&);
 bool bind_frame_v108(NativeQuestFrameServicesV108,std::string&);
 bool update_quest_v108(std::uintptr_t actual_quest,std::string&);
 bool belongs_to(const character::CharacterMenuQuestsV51& owner)const noexcept{return quests_.lock().get()==&owner;}
 bool failed()const noexcept{return failed_;}
 const auto& failure()const noexcept{return failure_;}
};
}
