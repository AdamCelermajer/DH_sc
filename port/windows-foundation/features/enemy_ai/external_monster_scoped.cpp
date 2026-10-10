#include "external_monster_natives.hpp"
namespace dh::foundation::enemy_ai {namespace {
int scoped(void* p,const dh2_script_callback_scope* scope,const dh2_script_value* a,std::uint32_t n,
 dh2_script_value* out,std::uint32_t cap,std::uint32_t* returned,char* error,std::size_t size){
 auto* b=static_cast<ExternalMonsterDoSkill*>(p);
 if(!b||!dh2_script_callback_scope_valid(scope))return DH2_SCRIPT_REQUIRED_SERVICE_FAILURE;
 struct Restore {ExternalMonsterDoSkill& b;const dh2_script_callback_scope* old;~Restore(){b.current_scope=old;}} restore{*b,b->current_scope};
 b->current_scope=scope;
 return external_monster_do_skill(b,a,n,out,cap,returned,error,size);
}
}
int bind_external_monster_do_skill_scoped(dh2_script_vm* vm,ExternalMonsterDoSkill* b){
 if(!vm||!b||!b->receiver_lease||!b->character)return -1;
 return dh2_script_vm_bind_source_scoped_values(vm,"DoSkill",scoped,b);
}
}
