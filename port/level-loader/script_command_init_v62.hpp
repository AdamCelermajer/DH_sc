#pragma once
#include "script_command_receivers_v59.hpp"
#include <cstring>
#include <utility>
namespace dh2::loader {
// Types are Main's existing concrete native owners. These are scoped transport
// borrows, never new constants/menu/object/FX/animator registries or receivers.
template<class T>struct ActualInitBorrowV62 {
 std::shared_ptr<T> owner;T* actual{};
 bool valid(bool nullable=false)const noexcept{return actual?owner&&owner.get()==actual:nullable&&!owner;}
};
template<class Character,class Animator>struct ActualAnimator49cBorrowV62 {
 ActualInitBorrowV62<Animator> animator;Character* same_character{};
};
template<class Types>struct ScriptInitLeavesV62 {
 using App=typename Types::Application;using Constants=typename Types::Constants;
 using MenuManager=typename Types::MenuManager;using Menu=typename Types::Menu;
 using FX=typename Types::FXManager;using Objects=typename Types::ObjectManager;
 using Handle=typename Types::ObjectHandle;using Object=typename Types::Object;
 using Character=typename Types::Character;using Animator=typename Types::Animator;
 std::weak_ptr<App> application;
 // Original4598c8 reads current Main singleton, never a bind-time snapshot.
 std::function<bool(ActualInitBorrowV62<FX>&,std::string&)> current_fx_singleton;
 std::function<bool(App&,ActualInitBorrowV62<Constants>&,std::string&)> app_constants2c;
 std::function<bool(Constants&,const char*,const char*,std::int32_t&,std::string&)> get_constant;
 std::function<bool(App&,ActualInitBorrowV62<MenuManager>&,std::string&)> menu_get_instance;
 std::function<bool(MenuManager&,const char*,ActualInitBorrowV62<Menu>&,std::string&)> menu_by_name;
 // Actual engine body, including its functional registration mutation, REQUIRED.
 std::function<bool(FX&,std::int32_t,std::string&)> fx_register_set_to_load;
 std::function<bool(App&,ActualInitBorrowV62<Objects>&,std::string&)> app_objects38;
 std::function<bool(Objects&,const char*,std::int32_t,bool,const char*,ActualInitBorrowV62<Handle>&,std::string&)> object_by_name;
 std::function<bool(Handle&,bool,ActualInitBorrowV62<Object>&,std::string&)> handle_get_object;
 std::function<bool(Handle&,ActualInitBorrowV62<Character>&,std::string&)> handle_as_character;
 std::function<bool(Character&,ActualAnimator49cBorrowV62<Character,Animator>&,std::string&)> character_animator49c;
 // Actual engine body, including its functional dictionary registration, REQUIRED.
 std::function<bool(Animator&,std::int32_t,std::string&)> animator_add_dict_to_set;
};
namespace init_v62_detail {
inline bool stop(const char* reason,std::string& e){e=reason;return false;}
inline bool word(const CheckedCommandBorrowV59& c,unsigned offset,std::int32_t& out,std::string& e){
 auto p=c.actual_data->scalar(offset);if(!p||p->width!=4)return stop("Require SAME actual Init Data32-bit field",e);std::memcpy(&out,&p->bits,4);return true;
}
inline bool same_borrow(const CheckedCommandBorrowV59& c,std::string& e){
 if(!c.actual_receiver)return stop("Require SAME actual Init receiver",e);CheckedCommandBorrowV59 fresh;if(!c.actual_receiver->checked_data_borrow(fresh,e))return false;
 if(c.identity!=fresh.identity||c.actual_data.get()!=fresh.actual_data.get()||c.descriptor!=fresh.descriptor||c.skip4!=fresh.skip4||c.kind8!=fresh.kind8||c.data_c!=fresh.data_c)return stop("Foreign/copied actual Init field borrow",e);return true;
}
template<class Receiver>bool receiver(const CheckedCommandBorrowV59& c,std::string& e){return std::dynamic_pointer_cast<Receiver>(c.actual_receiver)?true:stop("Wrong concrete class for original Init",e);}
}
// Reconstructs source orchestration, never Main leaves or source Execute.
// Bind callback closures with weak Main captures. This adapter retains weak App
// and FX authorities; temporary typed borrows pin only the reached body.
template<class Types>ScriptCommandBehaviorV59 script_init_behavior_v62(ScriptInitLeavesV62<Types> leaves){
 using L=ScriptInitLeavesV62<Types>;ScriptCommandBehaviorV59 result;result.actual_owner=leaves.application;
 result.init=[leaves=std::move(leaves)](const CheckedCommandBorrowV59& c,std::string& error)->bool {
  using namespace init_v62_detail;if(!same_borrow(c,error))return false;auto app=leaves.application.lock();if(!app)return stop("Required actual Main Application expired",error);
  // c.actual_data is the source-cached data_c pointer: Start459048,
  // Effect4598b4, Actor45a2f8. Pin it across leaves; do not rebind to a
  // changed command header. V59 performs explicit native post-body guards.
  const auto kind=*c.kind8;
  if(kind==10){
   if(!receiver<Script_StartDialogReceiverV59>(c,error))return false;
   if(!leaves.app_constants2c)return stop("Required actual Application.constants2c",error);
   ActualInitBorrowV62<typename L::Constants> constants;if(!leaves.app_constants2c(*app,constants,error))return false;if(!constants.valid())return stop("Required SAME actual constants2c owner",error);
   // Source459060 loads scalar BEFORE getConstant459068. Retain that
   // scalar even when the actual lookup leaf mutates later live Data.
   std::int32_t authored;if(!word(c,0x0c,authored,error))return false;
   if(!leaves.get_constant)return stop("Required actual PyDataConstants.getConstant",error);
   std::int32_t target;if(!leaves.get_constant(*constants.actual,"DialogStyles","EnterLocationDialog",target,error))return false;
   *c.skip4=authored==target?1:0;error.clear();return true;
  }
  if(kind==22||kind==23){
   if(kind==22?!receiver<Script_ShowFlashReceiverV59>(c,error):!receiver<Script_HideFlashReceiverV59>(c,error))return false;
   if(!leaves.menu_get_instance)return stop("Required actual MenuManager.GetInstance",error);
   ActualInitBorrowV62<typename L::MenuManager> menus;if(!leaves.menu_get_instance(*app,menus,error))return false;if(!menus.valid())return stop("Required actual MenuManager owner",error);
   // Source4597f4/459848 loads CURRENT command.data_c AFTER GetInstance.
   // Unlike the other three bodies, pinning the incoming Data snapshot
   // across this leaf would change the source branch. Require a live lease.
   CheckedCommandBorrowV59 menu_data;if(!c.actual_receiver->checked_data_borrow(menu_data,error))return false;
   const auto name=menu_data.actual_data->cstring(0x10);if(!name)return stop("Required SAME actual Show/Hide Data CString10",error);
   if(std::strstr(name,"HUD"))return c.actual_receiver->write_operand_pointer(0x10,0,{},error);
   if(!leaves.menu_by_name)return stop("Required actual MenuManager.GetMenuByName",error);
   ActualInitBorrowV62<typename L::Menu> menu;if(!leaves.menu_by_name(*menus.actual,name,menu,error))return false;if(!menu.valid(true))return stop("Required SAME actual Menu alias result",error);
   return c.actual_receiver->write_operand_pointer(0x10,reinterpret_cast<std::uintptr_t>(menu.actual),menu.owner,error);
  }
  if(kind==20){
   if(!receiver<Script_PlayEffectReceiverV59>(c,error))return false;std::int32_t id;if(!word(c,0x08,id,error))return false;
   // Source4598bc Data+8 precedes current singleton read4598c8.
   if(!leaves.current_fx_singleton)return stop("Required current Main VisualFXManager singleton projection",error);
   ActualInitBorrowV62<typename L::FX> fx;if(!leaves.current_fx_singleton(fx,error))return false;if(!fx.valid())return stop("Required SAME current VisualFXManager singleton owner",error);
   if(!leaves.fx_register_set_to_load)return stop("Required Main RegisterFXSetToLoad body",error);
   if(!leaves.fx_register_set_to_load(*fx.actual,id,error))return false;error.clear();return true;
  }
  if(kind==45){
   if(!receiver<Script_PlayActorAnimReceiverV59>(c,error))return false;
   if(!leaves.app_objects38)return stop("Required actual Application.ObjectManager38",error);
   ActualInitBorrowV62<typename L::Objects> objects;if(!leaves.app_objects38(*app,objects,error))return false;if(!objects.valid())return stop("Required SAME actual ObjectManager38",error);
   const auto name=c.actual_data->cstring(0x18);if(!name)return stop("Required SAME actual PlayActorAnim Data CString18",error);
   if(!leaves.object_by_name)return stop("Required actual ObjectManager.GetObjectByName",error);
   ActualInitBorrowV62<typename L::Handle> handle;if(!leaves.object_by_name(*objects.actual,name,-1,false,nullptr,handle,error))return false;if(!handle.valid())return stop("Required actual temporary ObjectHandle result",error);
   if(!leaves.handle_get_object)return stop("Required actual ObjectHandle.GetObject(false)",error);
   ActualInitBorrowV62<typename L::Object> object;if(!leaves.handle_get_object(*handle.actual,false,object,error))return false;if(!object.valid(true))return stop("Foreign actual GetObject result",error);if(!object.actual){error.clear();return true;}
   if(!leaves.handle_as_character)return stop("Required actual ObjectHandle Character conversion",error);
   ActualInitBorrowV62<typename L::Character> character;if(!leaves.handle_as_character(*handle.actual,character,error))return false;if(!character.valid(true))return stop("Foreign actual Character conversion result",error);if(!character.actual){error.clear();return true;}
   if(!leaves.character_animator49c)return stop("Required SAME Character embedded Animator49c",error);
   ActualAnimator49cBorrowV62<typename L::Character,typename L::Animator> animator;if(!leaves.character_animator49c(*character.actual,animator,error))return false;
   if(!animator.animator.valid()||animator.same_character!=character.actual||animator.animator.owner.owner_before(character.owner)||character.owner.owner_before(animator.animator.owner))return stop("Require SAME Character/embedded Animator49c alias lease",error);
   std::int32_t first;if(!word(c,0x08,first,error))return false;if(!leaves.animator_add_dict_to_set)return stop("Required Main ANIM_AddAnimDictToSet body",error);
   if(!leaves.animator_add_dict_to_set(*animator.animator.actual,first,error))return false;
   // Source45a370 freshly reads cached SAME Data+c after firstAdd.
   // Character/Animator stay cached inr4; there is no owner relookup.
   std::int32_t second;if(!word(c,0x0c,second,error))return false;if(!leaves.animator_add_dict_to_set(*animator.animator.actual,second,error))return false;error.clear();return true;
  }
  return stop("V62 dispatch only owns five proven nontrivial Init orders",error);
 };
 // Execute intentionally absent: Main supplies actual functional engine bodies.
 return result;
}
}
