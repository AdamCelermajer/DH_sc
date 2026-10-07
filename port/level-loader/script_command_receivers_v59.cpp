#include "script_command_receivers_v59.hpp"
#include <stdexcept>
namespace dh2::loader {
#include "command_c1_descriptors_v59.inc"
namespace {
std::int32_t signed_word(std::uint32_t v){std::int32_t s;std::memcpy(&s,&v,4);return s;}
const CommandC1DescriptorV59& canonical_descriptor(const CommandC1DescriptorV59& d,std::int32_t expected){
 const auto found=command_c1_descriptors_v59().find(expected);if(found==command_c1_descriptors_v59().end()||&d!=&found->second)throw std::invalid_argument("Require SAME stable canonical descriptor for actual concrete class");return found->second;
}
bool operand_domain(const CommandC1DescriptorV59& d,std::uint32_t o,std::uint8_t n){return o>=16&&(n==1||n==4)&&o+n<=d.original_size&&(n==1||o%4==0);}
}
CanonicalScriptCommandV59::CanonicalScriptCommandV59(const CommandC1DescriptorV59& d,ScriptCommandBehaviorV59 b,std::int32_t expected):descriptor_(canonical_descriptor(d,expected)),behaviors_(std::move(b)){
 for(const auto& s:d.stores){if(s.offset==4){if(s.width!=1)throw std::runtime_error("Original skip4 store width mismatch");skip4_=std::uint8_t(s.value);}else if(s.offset==8){kind8_=signed_word(s.value);}else if(s.offset==12){if(s.value)throw std::runtime_error("Original data_c constructor must be NULL");data_c_=0;}else if(!operand_domain(d,s.offset,std::uint8_t(s.width)))throw std::runtime_error("Unrecovered original C1 operand store");}
}
void CanonicalScriptCommandV59::apply_constructor_operands(){
 for(const auto& s:descriptor_.stores)if(s.offset>=16){if(native_pointer_cell(s.offset)){if(s.width!=4||s.value!=0)throw std::runtime_error("Original source pointer C1 must produce NULL");*native_pointer_cell(s.offset)=0;native_pointer_owner_cell(s.offset)->reset();continue;}auto cell=operand_cell(s.offset);if(!cell||cell->original_width!=s.width)throw std::runtime_error("Concrete original receiver C1 cell mismatch");cell->value=OperandScalarV59{std::uint8_t(s.width),s.value};}
}
bool CanonicalScriptCommandV59::fail(const std::string& e,std::string& error){failed_=true;failure_=e.empty()?"Command required actual body failed":e;error=failure_;return false;}
bool CanonicalScriptCommandV59::checked_data_borrow(CheckedCommandBorrowV59& out,std::string& error){
 if(released_){error="Released actual command receiver";return false;}
 if(receiver_kind()!=descriptor_.kind||kind8_!=descriptor_.kind||!data_||!data_->complete()||data_->source_kind()!=descriptor_.kind||data_c_!=data_->identity()){error="Required SAME actual command/header/generated-data assignment";return false;}
 CheckedCommandBorrowV59 next{shared_from_this(),data_,identity(),&descriptor_,&skip4_,&kind8_,&data_c_};out=std::move(next);error.clear();return true;
}
bool CanonicalScriptCommandV59::checked_menu_borrow(CheckedCommandMenuBorrowV59& out,std::string& error){
 CheckedCommandBorrowV59 actual;if(!checked_data_borrow(actual,error))return false;auto cell=menu_pointer_cell();auto owner=menu_owner_cell();if(!cell||!owner){error="Actual receiver has no source-proven menu+10 pointer cell";return false;}auto pin=owner->lock();if(*cell&&(!pin||reinterpret_cast<std::uintptr_t>(pin.get())!=*cell)){error="Required live SAME actual Menu alias owner";return false;}CheckedCommandMenuBorrowV59 next{actual.actual_receiver,std::move(pin),identity(),cell};out=std::move(next);error.clear();return true;
}
bool CanonicalScriptCommandV59::checked_pointer_borrow(std::uint32_t offset,CheckedCommandPointerBorrowV59& out,std::string& error){
 CheckedCommandBorrowV59 actual;if(!checked_data_borrow(actual,error))return false;auto cell=native_pointer_cell(offset);auto owner=native_pointer_owner_cell(offset);if(!cell||!owner){error="Actual receiver has no source-proven native pointer at this offset";return false;}auto pin=owner->lock();if(*cell&&(!pin||reinterpret_cast<std::uintptr_t>(pin.get())!=*cell)){error="Required live SAME actual source target alias owner";return false;}CheckedCommandPointerBorrowV59 next{actual.actual_receiver,std::move(pin),identity(),offset,cell};out=std::move(next);error.clear();return true;
}
ScriptCommandBorrowV52 CanonicalScriptCommandV59::constructor_borrow(){auto self=shared_from_this();return {self,identity(),descriptor_.factory,&skip4_,&kind8_,&data_c_,&data_,[self](std::string& e){return self->init(e);},[self](std::string& e){return self->release_base_storage(e);},[self](bool skip,std::int32_t module,std::string& e){return self->execute(skip,module,e);},[self](bool& value,std::string& e){return self->blocking_v96(value,e);},[self](std::string& e){return self->update_v96(e);},self};}
bool CanonicalScriptCommandV59::init(std::string& error){
 if(busy_)return fail("Actual command Init reentered; prefix retained",error);if(failed_){error=failure_;return false;}
 // LoadScriptFile and the later InitCommands both invoke this virtual Init.
 CheckedCommandBorrowV59 actual;if(!checked_data_borrow(actual,error))return fail(error,error);
 struct Busy{bool& b;explicit Busy(bool& v):b(v){b=true;}~Busy(){b=false;}} guard(busy_);
 if(descriptor_.Init==0x455628){initialized_=true;error.clear();return true;} // Original literal BX LR.
 auto pin=behaviors_.actual_owner.lock();if(!pin||!behaviors_.init)return fail("Required original "+std::string(descriptor_.class_name)+" Init "+std::to_string(descriptor_.Init),error);
 try{if(!behaviors_.init(actual,error))return fail(error,error);if(failed_){error=failure_;return false;}CheckedCommandBorrowV59 after;if(!checked_data_borrow(after,error)||after.actual_data.get()!=actual.actual_data.get())return fail("Main Init changed SAME receiver/data assignment",error);initialized_=true;error.clear();return true;}catch(const std::exception& e){return fail(e.what(),error);}catch(...){return fail("Main original Init threw",error);}
}
bool CanonicalScriptCommandV59::execute(bool skip,std::int32_t context,std::string& error){
 if(released_||!initialized_){error="Actual command Execute requires live initialized receiver";return false;}
 if(busy_){error="Actual command Execute reentry unavailable";return false;}
 CheckedCommandBorrowV59 actual;if(!checked_data_borrow(actual,error))return false;auto pin=behaviors_.actual_owner.lock();if(!pin||!behaviors_.execute){error="Required Main original "+std::string(descriptor_.class_name)+" Execute(bool,int)";return false;}
 struct Busy{bool& b;explicit Busy(bool& v):b(v){b=true;}~Busy(){b=false;}} guard(busy_);
 try{return behaviors_.execute(actual,skip,context,error);}catch(const std::exception& e){error=e.what();return false;}catch(...){error="Main original Execute threw";return false;}
}
bool CanonicalScriptCommandV59::release_base_storage(std::string& error){
 if(busy_){error="Actual command base teardown during body unavailable";return false;}if(released_){error="Actual command base storage already released";return false;}
 // Manager executes source BASE data-free/dtor/c-pointer clear FIRST. Do not
 // silently substitute destructor release while a data assignment remains.
 if(data_||data_c_){error="Require original BASE data teardown prefix before command CustomFree";return false;}
 // Base Free has no derived Finish/Execute/operand cleanup dispatch.
 // Native class members release their leases only when CustomFree ownership ends.
 released_=true;error.clear();return true;
}
bool CanonicalScriptCommandV59::read_operand_word(std::uint32_t o,std::uint8_t n,std::uint32_t& out,std::string& error)const {
 if(released_||!operand_domain(descriptor_,o,n)){error="Invalid actual receiver operand word";return false;}
 const auto base=n==1?(o&~3u):o;auto cell=operand_cell(base);
 if(cell&&o+n<=base+cell->original_width)if(auto scalar=std::get_if<OperandScalarV59>(&cell->value)){out=scalar->bits>>(8*(o-base));if(n==1)out&=255;error.clear();return true;}
 error="Original operand unproduced or currently a native pointer";return false;
}
bool CanonicalScriptCommandV59::write_operand_word(std::uint32_t o,std::uint8_t n,std::uint32_t value,std::string& error){
 if(released_||!operand_domain(descriptor_,o,n)){error="Invalid actual receiver operand word producer";return false;}
 const auto base=n==1?(o&~3u):o;auto cell=operand_cell(base);
 if(!cell||o+n>base+cell->original_width){error="No original actual cell at requested word offset/width";return false;}
 if(auto scalar=std::get_if<OperandScalarV59>(&cell->value)){const auto shift=8*(o-base);const auto mask=n==4?0xffffffffu:255u<<shift;scalar->bits=(scalar->bits&~mask)|((value<<shift)&mask);error.clear();return true;}
 if(o!=base||n!=cell->original_width){error="Cannot partially replace a native pointer cell";return false;}
 cell->value=OperandScalarV59{n,n==1?value&255:value};error.clear();return true;
}
bool CanonicalScriptCommandV59::read_operand_pointer(std::uint32_t o,std::uintptr_t& out,std::string& error)const {
 if(released_||!operand_domain(descriptor_,o,4)){error="Invalid actual native pointer operand";return false;}if(native_pointer_cell(o)){const auto value=*native_pointer_cell(o);auto pin=native_pointer_owner_cell(o)->lock();if(value&&(!pin||reinterpret_cast<std::uintptr_t>(pin.get())!=value)){error="Required live SAME actual source target alias owner";return false;}out=value;error.clear();return true;}auto cell=operand_cell(o);
 if(cell&&cell->original_width==4)if(auto v=std::get_if<OperandPointerV59>(&cell->value)){out=v->value;error.clear();return true;}
 error="Original opaque constructor word has no proven native pointer producer";return false;
}
bool CanonicalScriptCommandV59::write_operand_pointer(std::uint32_t o,std::uintptr_t value,std::shared_ptr<void> owner,std::string& error){
 if(released_||!operand_domain(descriptor_,o,4)||(value&&!owner)){error="Required actual pointer producer/lease for command operand";return false;}if(native_pointer_cell(o)){if(value&&reinterpret_cast<std::uintptr_t>(owner.get())!=value){error="Require SAME actual native pointer/alias owner";return false;}*native_pointer_cell(o)=value;if(value)*native_pointer_owner_cell(o)=owner;else native_pointer_owner_cell(o)->reset();error.clear();return true;}auto cell=operand_cell(o);
 if(!cell||cell->original_width!=4){error="No original full-width actual cell for pointer producer";return false;}cell->value=OperandPointerV59{value,std::move(owner)};error.clear();return true;
}
bool CanonicalScriptCommandV59::constructor_word(std::uint32_t o,std::uint8_t n,std::uint32_t& out,std::string& error)const{
 if(released_){error="Released actual command receiver";return false;}if(n==4&&native_pointer_cell(o)&&!*native_pointer_cell(o)){out=0;error.clear();return true;}if(o==4&&n==1){out=skip4_;error.clear();return true;}if(o==8&&n==4){out=std::uint32_t(kind8_);error.clear();return true;}if(o==12&&n==4&&data_c_==0){out=0;error.clear();return true;}return read_operand_word(o,n,out,error);
}
bool create_script_command_receiver_v59(std::int32_t kind,ScriptCommandBehaviorV59 services,std::shared_ptr<CanonicalScriptCommandV59>& out,std::string& error){
 const auto found=command_c1_descriptors_v59().find(kind);if(found==command_c1_descriptors_v59().end()){error="Required original C1 receiver for unretained command class "+std::to_string(kind);return false;}
 try{std::shared_ptr<CanonicalScriptCommandV59> candidate;switch(kind){
case 0:candidate=std::make_shared<Script_ExecScriptReceiverV59>(found->second,services);break;
case 1:candidate=std::make_shared<Script_EnterCutSceneModeReceiverV59>(found->second,services);break;
case 2:candidate=std::make_shared<Script_ExitCutSceneModeReceiverV59>(found->second,services);break;
case 3:candidate=std::make_shared<Script_CONSOLEReceiverV59>(found->second,services);break;
case 4:candidate=std::make_shared<Script_SetCameraReceiverV59>(found->second,services);break;
case 5:candidate=std::make_shared<Script_PlayCameraReceiverV59>(found->second,services);break;
case 6:candidate=std::make_shared<Script_SetCameraClipReceiverV59>(found->second,services);break;
case 7:candidate=std::make_shared<Script_WaitCameraReceiverV59>(found->second,services);break;
case 8:candidate=std::make_shared<Script_SetCameraTargetReceiverV59>(found->second,services);break;
case 10:candidate=std::make_shared<Script_StartDialogReceiverV59>(found->second,services);break;
case 12:candidate=std::make_shared<Script_WaitDialogReceiverV59>(found->second,services);break;
case 13:candidate=std::make_shared<Script_PlaySoundReceiverV59>(found->second,services);break;
case 14:candidate=std::make_shared<Script_StopSoundReceiverV59>(found->second,services);break;
case 15:candidate=std::make_shared<Script_PlayLevelMusicReceiverV59>(found->second,services);break;
case 16:candidate=std::make_shared<Script_EnterSafeZoneReceiverV59>(found->second,services);break;
case 17:candidate=std::make_shared<Script_LeaveSafeZoneReceiverV59>(found->second,services);break;
case 19:candidate=std::make_shared<Script_PlayAnimByNameReceiverV59>(found->second,services);break;
case 20:candidate=std::make_shared<Script_PlayEffectReceiverV59>(found->second,services);break;
case 21:candidate=std::make_shared<Script_StopEffectReceiverV59>(found->second,services);break;
case 22:candidate=std::make_shared<Script_ShowFlashReceiverV59>(found->second,services);break;
case 23:candidate=std::make_shared<Script_HideFlashReceiverV59>(found->second,services);break;
case 24:candidate=std::make_shared<Script_LockCharacterReceiverV59>(found->second,services);break;
case 25:candidate=std::make_shared<Script_UnlockCharacterReceiverV59>(found->second,services);break;
case 26:candidate=std::make_shared<Script_WaitReceiverV59>(found->second,services);break;
case 27:candidate=std::make_shared<Script_SetFaeryStateReceiverV59>(found->second,services);break;
case 29:candidate=std::make_shared<Script_PutCharacterInLimbusReceiverV59>(found->second,services);break;
case 30:candidate=std::make_shared<Script_SpawnCharacterReceiverV59>(found->second,services);break;
case 31:candidate=std::make_shared<Script_PutCharacterInIdleReceiverV59>(found->second,services);break;
case 32:candidate=std::make_shared<Script_MarkCharacterAsScriptedReceiverV59>(found->second,services);break;
case 39:candidate=std::make_shared<Script_StopActorReceiverV59>(found->second,services);break;
case 40:candidate=std::make_shared<Script_MoveActorReceiverV59>(found->second,services);break;
case 41:candidate=std::make_shared<Script_LookActorReceiverV59>(found->second,services);break;
case 42:candidate=std::make_shared<Script_ShowActorReceiverV59>(found->second,services);break;
case 43:candidate=std::make_shared<Script_HideActorReceiverV59>(found->second,services);break;
case 44:candidate=std::make_shared<Script_KillActorReceiverV59>(found->second,services);break;
case 45:candidate=std::make_shared<Script_PlayActorAnimReceiverV59>(found->second,services);break;
case 46:candidate=std::make_shared<Script_SetActorPositionReceiverV59>(found->second,services);break;
case 51:candidate=std::make_shared<Script_UnEquipHandsReceiverV59>(found->second,services);break;
case 52:candidate=std::make_shared<Script_ReEquipHandsReceiverV59>(found->second,services);break;
case 54:candidate=std::make_shared<Script_OpenDoorReceiverV59>(found->second,services);break;
case 55:candidate=std::make_shared<Script_CloseDoorReceiverV59>(found->second,services);break;
case 63:candidate=std::make_shared<Script_RestartLevelReceiverV59>(found->second,services);break;
case 68:candidate=std::make_shared<Script_ShowTrophiesReceiverV59>(found->second,services);break;
case 69:candidate=std::make_shared<Script_SaveGameReceiverV59>(found->second,services);break;
case 70:candidate=std::make_shared<Script_BlockSaveGameReceiverV59>(found->second,services);break;
case 77:candidate=std::make_shared<Script_LockTutorialReceiverV59>(found->second,services);break;
case 78:candidate=std::make_shared<Script_DoTutorialReceiverV59>(found->second,services);break;
case 79:candidate=std::make_shared<Script_FlushMessagesReceiverV59>(found->second,services);break;
case 9:candidate=std::make_shared<Script_EmptyImplReceiverV59>(found->second,services);break;
case 11:candidate=std::make_shared<Script_StartDialogIDReceiverV59>(found->second,services);break;
case 18:candidate=std::make_shared<Script_PlayAnimByIdReceiverV59>(found->second,services);break;
case 28:candidate=std::make_shared<Script_IncFaeryLevelReceiverV59>(found->second,services);break;
case 33:candidate=std::make_shared<Script_AIEnableTimerReceiverV59>(found->second,services);break;
case 34:candidate=std::make_shared<Script_AIResetTimerReceiverV59>(found->second,services);break;
case 35:candidate=std::make_shared<Script_AIDoSkillReceiverV59>(found->second,services);break;
case 36:candidate=std::make_shared<Script_AISetHPReceiverV59>(found->second,services);break;
case 37:candidate=std::make_shared<Script_AIChangeScriptReceiverV59>(found->second,services);break;
case 38:candidate=std::make_shared<Script_AISetIntReceiverV59>(found->second,services);break;
case 47:candidate=std::make_shared<Script_SetActorMasterReceiverV59>(found->second,services);break;
case 48:candidate=std::make_shared<Script_DropLootReceiverV59>(found->second,services);break;
case 49:candidate=std::make_shared<Script_AutoEquipReceiverV59>(found->second,services);break;
case 50:candidate=std::make_shared<Script_UnEquipItemFromSlotReceiverV59>(found->second,services);break;
case 53:candidate=std::make_shared<Script_SetStaticReceiverV59>(found->second,services);break;
case 56:candidate=std::make_shared<Script_ActivateProjectileTrapReceiverV59>(found->second,services);break;
case 57:candidate=std::make_shared<Script_DeactivateProjectileTrapReceiverV59>(found->second,services);break;
case 58:candidate=std::make_shared<Script_ActivateTriggerPlateReceiverV59>(found->second,services);break;
case 59:candidate=std::make_shared<Script_DeactivateTriggerPlateReceiverV59>(found->second,services);break;
case 60:candidate=std::make_shared<Script_SpawnContainerReceiverV59>(found->second,services);break;
case 61:candidate=std::make_shared<Script_SetLevelStateReceiverV59>(found->second,services);break;
case 62:candidate=std::make_shared<Script_SetWorldMapLocationStateReceiverV59>(found->second,services);break;
case 64:candidate=std::make_shared<Script_ChangeLevelReceiverV59>(found->second,services);break;
case 65:candidate=std::make_shared<Script_EndGameReceiverV59>(found->second,services);break;
case 66:candidate=std::make_shared<Script_AwardTrophyReceiverV59>(found->second,services);break;
case 67:candidate=std::make_shared<Script_AwardEndGameTrophiesReceiverV59>(found->second,services);break;
case 71:candidate=std::make_shared<Script_EnqueueTutorialMessageReceiverV59>(found->second,services);break;
case 72:candidate=std::make_shared<Script_SkipTutorialMessageReceiverV59>(found->second,services);break;
case 73:candidate=std::make_shared<Script_SkipAllTutorialMessagesReceiverV59>(found->second,services);break;
case 74:candidate=std::make_shared<Script_EnqueueCharMenuTutorialMessageReceiverV59>(found->second,services);break;
case 75:candidate=std::make_shared<Script_SkipCharMenuTutorialMessageReceiverV59>(found->second,services);break;
case 76:candidate=std::make_shared<Script_SkipAllCharMenuTutorialMessagesReceiverV59>(found->second,services);break;
 default:error="Unretained original class C1";return false;}out=std::move(candidate);error.clear();return true;
 }catch(const std::exception& e){error=e.what();return false;}
}
ScriptManagerServicesV52 bind_script_command_factories_v59(ScriptManagerServicesV52 input,ScriptCommandBehaviorV59 services){
 if(input.command_factory)throw std::invalid_argument("Existing real command factory must not be replaced");
 input.command_factory=[services=std::move(services)](std::int32_t kind,ScriptCommandBorrowV52& out,std::string& error){std::shared_ptr<CanonicalScriptCommandV59> actual;if(!create_script_command_receiver_v59(kind,services,actual,error))return false;auto borrow=actual->constructor_borrow();out=std::move(borrow);error.clear();return true;};return input;
}
}
