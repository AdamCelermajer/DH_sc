#pragma once
#include "live_enemy_binding.hpp"
#include "../../../script-runtime/script_runtime.h"
namespace dh::foundation::enemy_ai {
// Character::_DoSkill3b8bd8's two separate original conversions are supplied
// by the Value producer. Count is Character.GetSkills3bc5fc, use is whole
// CharAI.AI_UseSkill3d8868 (SkillAI use op), on the SAME native NPC SkillOwner.
struct ExternalMonsterDoSkill {
    std::shared_ptr<void> receiver_lease;
    ActorId character{};
    std::function<bool(const dh2_script_value&,std::uint32_t&,std::string&)> get_unsigned;
    std::function<bool(std::uint32_t&,std::string&)> skill_count;
    std::function<bool(const dh2_script_value&,std::int32_t&,std::string&)> get_number_integer;
    std::function<bool(std::int32_t,std::string&)> use_skill;
    const dh2_script_callback_scope* current_scope{}; // ephemeral SAME-VM loan
};
int external_monster_do_skill(void*,const dh2_script_value*,std::uint32_t,
    dh2_script_value*,std::uint32_t,std::uint32_t*,char*,std::size_t);
// CharacterScriptSessionInput.gameplay_binding adapter for this exact source
// registration only. Unrelated callbacks retain their existing source owners.
int select_external_monster_do_skill(void*,std::uint32_t original_callback,
    dh2_script_function*,void**);
int bind_external_monster_do_skill_scoped(dh2_script_vm*,ExternalMonsterDoSkill*);
}
