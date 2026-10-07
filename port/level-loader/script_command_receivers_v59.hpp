#pragma once
#include "script_manager_owner_v52.hpp"
#include <variant>
#include <utility>
namespace dh2::loader {
struct CommandC1StoreV59 {std::uint32_t offset,width,value,source;};
struct CommandC1DescriptorV59 {std::int32_t kind;const char* class_name;std::uint32_t factory,Init,original_size;std::vector<CommandC1StoreV59> stores;};
class CanonicalScriptCommandV59;
struct CheckedCommandBorrowV59 {
 std::shared_ptr<CanonicalScriptCommandV59> actual_receiver;
 std::shared_ptr<ScriptCommandDataV52> actual_data;
 std::uintptr_t identity{};const CommandC1DescriptorV59* descriptor{};
 std::uint8_t* skip4{};std::int32_t* kind8{};std::uintptr_t* data_c{};
};
struct CheckedCommandMenuBorrowV59 {
 std::shared_ptr<CanonicalScriptCommandV59> actual_receiver;
 std::shared_ptr<void> actual_menu_owner; // scoped pin; .get() equals SAME menu10
 std::uintptr_t identity{};std::uintptr_t* menu10{};
};
struct CheckedCommandPointerBorrowV59 {
 std::shared_ptr<CanonicalScriptCommandV59> actual_receiver;
 std::shared_ptr<void> actual_target_owner;
 std::uintptr_t identity{};std::uint32_t original_offset{};std::uintptr_t* actual_pointer{};
};
struct ScriptCommandBehaviorV59 {
 std::weak_ptr<void> actual_owner;
 // Nontrivial original Init bodies ONLY. Source base455628 is actual BX LR.
 std::function<bool(const CheckedCommandBorrowV59&,std::string&)> init;
 std::function<bool(const CheckedCommandBorrowV59&,bool,std::int32_t,std::string&)> execute;
 std::function<bool(const CheckedCommandBorrowV59&,bool&,std::string&)> blocking;
 std::function<bool(const CheckedCommandBorrowV59&,std::string&)> update;
};
struct OperandScalarV59 {std::uint8_t width{};std::uint32_t bits{};};
struct OperandPointerV59 {std::uintptr_t value{};std::shared_ptr<void> actual_owner;};
// ONE actual receiver member, not parallel word/pointer fields. Source C1
// opaque words remain words until Main's proven typed producer writes a pointer.
using CommandOperandV59=std::variant<std::monostate,OperandScalarV59,OperandPointerV59>;
struct CommandOperandCellV59 { std::uint8_t original_width; CommandOperandV59 value; };
class CanonicalScriptCommandV59 : public std::enable_shared_from_this<CanonicalScriptCommandV59> {
 const CommandC1DescriptorV59& descriptor_;
 std::uint8_t skip4_{};std::int32_t kind8_{-1};std::uintptr_t data_c_{};
 std::shared_ptr<ScriptCommandDataV52> data_;
 ScriptCommandBehaviorV59 behaviors_;bool initialized_{},released_{},busy_{},failed_{};std::string failure_;
 bool fail(const std::string&,std::string&);
protected:
 CanonicalScriptCommandV59(const CommandC1DescriptorV59&,ScriptCommandBehaviorV59,std::int32_t expected_kind);
 virtual std::uintptr_t* menu_pointer_cell() noexcept{return nullptr;}
 virtual const std::uintptr_t* menu_pointer_cell() const noexcept{return nullptr;}
 virtual std::weak_ptr<void>* menu_owner_cell() noexcept{return nullptr;}
 virtual const std::weak_ptr<void>* menu_owner_cell() const noexcept{return nullptr;}
 virtual std::uintptr_t* native_pointer_cell(std::uint32_t o) noexcept{return o==16?menu_pointer_cell():nullptr;}
 virtual const std::uintptr_t* native_pointer_cell(std::uint32_t o) const noexcept{return o==16?menu_pointer_cell():nullptr;}
 virtual std::weak_ptr<void>* native_pointer_owner_cell(std::uint32_t o) noexcept{return o==16?menu_owner_cell():nullptr;}
 virtual const std::weak_ptr<void>* native_pointer_owner_cell(std::uint32_t o) const noexcept{return o==16?menu_owner_cell():nullptr;}
 virtual CommandOperandCellV59* operand_cell(std::uint32_t) noexcept=0;
 virtual const CommandOperandCellV59* operand_cell(std::uint32_t) const noexcept=0;
 void apply_constructor_operands();
public:
 CanonicalScriptCommandV59(const CanonicalScriptCommandV59&)=delete;
 CanonicalScriptCommandV59& operator=(const CanonicalScriptCommandV59&)=delete;
 CanonicalScriptCommandV59(CanonicalScriptCommandV59&&)=delete;
 CanonicalScriptCommandV59& operator=(CanonicalScriptCommandV59&&)=delete;
 virtual ~CanonicalScriptCommandV59()=default;
 virtual std::int32_t receiver_kind()const noexcept=0;
 const CommandC1DescriptorV59& descriptor()const noexcept{return descriptor_;}
 std::uintptr_t identity()const noexcept{return reinterpret_cast<std::uintptr_t>(this);}
 bool checked_data_borrow(CheckedCommandBorrowV59&,std::string&);
 bool checked_menu_borrow(CheckedCommandMenuBorrowV59&,std::string&);
 bool checked_pointer_borrow(std::uint32_t,CheckedCommandPointerBorrowV59&,std::string&);
 ScriptCommandBorrowV52 constructor_borrow();
 bool init(std::string&);bool execute(bool,std::int32_t,std::string&);
 bool blocking_v96(bool&,std::string&);bool update_v96(std::string&);
 // Late source binding over already parsed/initialized SAME receiver. This
 // does not rerun C1/Assign/Init or replace existing data/menu/actor pointers.
 bool bind_execution_v96(ScriptCommandBehaviorV59,std::string&);
 bool release_base_storage(std::string&);
 bool read_operand_word(std::uint32_t,std::uint8_t,std::uint32_t&,std::string&)const;
 bool write_operand_word(std::uint32_t,std::uint8_t,std::uint32_t,std::string&);
 bool read_operand_pointer(std::uint32_t,std::uintptr_t&,std::string&)const;
 bool write_operand_pointer(std::uint32_t,std::uintptr_t,std::shared_ptr<void>,std::string&);
 bool constructor_word(std::uint32_t,std::uint8_t,std::uint32_t&,std::string&)const;
 bool initialized()const noexcept{return initialized_;}bool released()const noexcept{return released_;}
};
// Canonical concrete class for each retained original C1. Only source-produced
// extra cells exist as members of that actual receiver. No ARM object memcpy.
class Script_ExecScriptReceiverV59 final : public CanonicalScriptCommandV59 {
 CommandOperandCellV59 field10_{4,{}};
 CommandOperandCellV59* operand_cell(std::uint32_t o) noexcept override {if(o==16)return &field10_;(void)o;return nullptr;}
 const CommandOperandCellV59* operand_cell(std::uint32_t o) const noexcept override {if(o==16)return &field10_;(void)o;return nullptr;}
public:
 Script_ExecScriptReceiverV59(const CommandC1DescriptorV59& d,ScriptCommandBehaviorV59 b):CanonicalScriptCommandV59(d,std::move(b),0){apply_constructor_operands();}
 std::int32_t receiver_kind() const noexcept override {return 0;}
};
class Script_EnterCutSceneModeReceiverV59 final : public CanonicalScriptCommandV59 {
 CommandOperandCellV59* operand_cell(std::uint32_t o) noexcept override {(void)o;return nullptr;}
 const CommandOperandCellV59* operand_cell(std::uint32_t o) const noexcept override {(void)o;return nullptr;}
public:
 Script_EnterCutSceneModeReceiverV59(const CommandC1DescriptorV59& d,ScriptCommandBehaviorV59 b):CanonicalScriptCommandV59(d,std::move(b),1){apply_constructor_operands();}
 std::int32_t receiver_kind() const noexcept override {return 1;}
};
class Script_ExitCutSceneModeReceiverV59 final : public CanonicalScriptCommandV59 {
 CommandOperandCellV59* operand_cell(std::uint32_t o) noexcept override {(void)o;return nullptr;}
 const CommandOperandCellV59* operand_cell(std::uint32_t o) const noexcept override {(void)o;return nullptr;}
public:
 Script_ExitCutSceneModeReceiverV59(const CommandC1DescriptorV59& d,ScriptCommandBehaviorV59 b):CanonicalScriptCommandV59(d,std::move(b),2){apply_constructor_operands();}
 std::int32_t receiver_kind() const noexcept override {return 2;}
};
class Script_CONSOLEReceiverV59 final : public CanonicalScriptCommandV59 {
 CommandOperandCellV59* operand_cell(std::uint32_t o) noexcept override {(void)o;return nullptr;}
 const CommandOperandCellV59* operand_cell(std::uint32_t o) const noexcept override {(void)o;return nullptr;}
public:
 Script_CONSOLEReceiverV59(const CommandC1DescriptorV59& d,ScriptCommandBehaviorV59 b):CanonicalScriptCommandV59(d,std::move(b),3){apply_constructor_operands();}
 std::int32_t receiver_kind() const noexcept override {return 3;}
};
class Script_SetCameraReceiverV59 final : public CanonicalScriptCommandV59 {
 CommandOperandCellV59* operand_cell(std::uint32_t o) noexcept override {(void)o;return nullptr;}
 const CommandOperandCellV59* operand_cell(std::uint32_t o) const noexcept override {(void)o;return nullptr;}
public:
 Script_SetCameraReceiverV59(const CommandC1DescriptorV59& d,ScriptCommandBehaviorV59 b):CanonicalScriptCommandV59(d,std::move(b),4){apply_constructor_operands();}
 std::int32_t receiver_kind() const noexcept override {return 4;}
};
class Script_PlayCameraReceiverV59 final : public CanonicalScriptCommandV59 {
 CommandOperandCellV59* operand_cell(std::uint32_t o) noexcept override {(void)o;return nullptr;}
 const CommandOperandCellV59* operand_cell(std::uint32_t o) const noexcept override {(void)o;return nullptr;}
public:
 Script_PlayCameraReceiverV59(const CommandC1DescriptorV59& d,ScriptCommandBehaviorV59 b):CanonicalScriptCommandV59(d,std::move(b),5){apply_constructor_operands();}
 std::int32_t receiver_kind() const noexcept override {return 5;}
};
class Script_SetCameraClipReceiverV59 final : public CanonicalScriptCommandV59 {
 CommandOperandCellV59* operand_cell(std::uint32_t o) noexcept override {(void)o;return nullptr;}
 const CommandOperandCellV59* operand_cell(std::uint32_t o) const noexcept override {(void)o;return nullptr;}
public:
 Script_SetCameraClipReceiverV59(const CommandC1DescriptorV59& d,ScriptCommandBehaviorV59 b):CanonicalScriptCommandV59(d,std::move(b),6){apply_constructor_operands();}
 std::int32_t receiver_kind() const noexcept override {return 6;}
};
class Script_WaitCameraReceiverV59 final : public CanonicalScriptCommandV59 {
 CommandOperandCellV59* operand_cell(std::uint32_t o) noexcept override {(void)o;return nullptr;}
 const CommandOperandCellV59* operand_cell(std::uint32_t o) const noexcept override {(void)o;return nullptr;}
public:
 Script_WaitCameraReceiverV59(const CommandC1DescriptorV59& d,ScriptCommandBehaviorV59 b):CanonicalScriptCommandV59(d,std::move(b),7){apply_constructor_operands();}
 std::int32_t receiver_kind() const noexcept override {return 7;}
};
class Script_SetCameraTargetReceiverV59 final : public CanonicalScriptCommandV59 {
 CommandOperandCellV59* operand_cell(std::uint32_t o) noexcept override {(void)o;return nullptr;}
 const CommandOperandCellV59* operand_cell(std::uint32_t o) const noexcept override {(void)o;return nullptr;}
public:
 Script_SetCameraTargetReceiverV59(const CommandC1DescriptorV59& d,ScriptCommandBehaviorV59 b):CanonicalScriptCommandV59(d,std::move(b),8){apply_constructor_operands();}
 std::int32_t receiver_kind() const noexcept override {return 8;}
};
class Script_StartDialogReceiverV59 final : public CanonicalScriptCommandV59 {
 CommandOperandCellV59* operand_cell(std::uint32_t o) noexcept override {(void)o;return nullptr;}
 const CommandOperandCellV59* operand_cell(std::uint32_t o) const noexcept override {(void)o;return nullptr;}
public:
 Script_StartDialogReceiverV59(const CommandC1DescriptorV59& d,ScriptCommandBehaviorV59 b):CanonicalScriptCommandV59(d,std::move(b),10){apply_constructor_operands();}
 std::int32_t receiver_kind() const noexcept override {return 10;}
};
class Script_WaitDialogReceiverV59 final : public CanonicalScriptCommandV59 {
 CommandOperandCellV59* operand_cell(std::uint32_t o) noexcept override {(void)o;return nullptr;}
 const CommandOperandCellV59* operand_cell(std::uint32_t o) const noexcept override {(void)o;return nullptr;}
public:
 Script_WaitDialogReceiverV59(const CommandC1DescriptorV59& d,ScriptCommandBehaviorV59 b):CanonicalScriptCommandV59(d,std::move(b),12){apply_constructor_operands();}
 std::int32_t receiver_kind() const noexcept override {return 12;}
};
class Script_PlaySoundReceiverV59 final : public CanonicalScriptCommandV59 {
 CommandOperandCellV59* operand_cell(std::uint32_t o) noexcept override {(void)o;return nullptr;}
 const CommandOperandCellV59* operand_cell(std::uint32_t o) const noexcept override {(void)o;return nullptr;}
public:
 Script_PlaySoundReceiverV59(const CommandC1DescriptorV59& d,ScriptCommandBehaviorV59 b):CanonicalScriptCommandV59(d,std::move(b),13){apply_constructor_operands();}
 std::int32_t receiver_kind() const noexcept override {return 13;}
};
class Script_StopSoundReceiverV59 final : public CanonicalScriptCommandV59 {
 CommandOperandCellV59* operand_cell(std::uint32_t o) noexcept override {(void)o;return nullptr;}
 const CommandOperandCellV59* operand_cell(std::uint32_t o) const noexcept override {(void)o;return nullptr;}
public:
 Script_StopSoundReceiverV59(const CommandC1DescriptorV59& d,ScriptCommandBehaviorV59 b):CanonicalScriptCommandV59(d,std::move(b),14){apply_constructor_operands();}
 std::int32_t receiver_kind() const noexcept override {return 14;}
};
class Script_PlayLevelMusicReceiverV59 final : public CanonicalScriptCommandV59 {
 CommandOperandCellV59* operand_cell(std::uint32_t o) noexcept override {(void)o;return nullptr;}
 const CommandOperandCellV59* operand_cell(std::uint32_t o) const noexcept override {(void)o;return nullptr;}
public:
 Script_PlayLevelMusicReceiverV59(const CommandC1DescriptorV59& d,ScriptCommandBehaviorV59 b):CanonicalScriptCommandV59(d,std::move(b),15){apply_constructor_operands();}
 std::int32_t receiver_kind() const noexcept override {return 15;}
};
class Script_EnterSafeZoneReceiverV59 final : public CanonicalScriptCommandV59 {
 CommandOperandCellV59* operand_cell(std::uint32_t o) noexcept override {(void)o;return nullptr;}
 const CommandOperandCellV59* operand_cell(std::uint32_t o) const noexcept override {(void)o;return nullptr;}
public:
 Script_EnterSafeZoneReceiverV59(const CommandC1DescriptorV59& d,ScriptCommandBehaviorV59 b):CanonicalScriptCommandV59(d,std::move(b),16){apply_constructor_operands();}
 std::int32_t receiver_kind() const noexcept override {return 16;}
};
class Script_LeaveSafeZoneReceiverV59 final : public CanonicalScriptCommandV59 {
 CommandOperandCellV59* operand_cell(std::uint32_t o) noexcept override {(void)o;return nullptr;}
 const CommandOperandCellV59* operand_cell(std::uint32_t o) const noexcept override {(void)o;return nullptr;}
public:
 Script_LeaveSafeZoneReceiverV59(const CommandC1DescriptorV59& d,ScriptCommandBehaviorV59 b):CanonicalScriptCommandV59(d,std::move(b),17){apply_constructor_operands();}
 std::int32_t receiver_kind() const noexcept override {return 17;}
};
class Script_PlayAnimByNameReceiverV59 final : public CanonicalScriptCommandV59 {
 CommandOperandCellV59* operand_cell(std::uint32_t o) noexcept override {(void)o;return nullptr;}
 const CommandOperandCellV59* operand_cell(std::uint32_t o) const noexcept override {(void)o;return nullptr;}
public:
 Script_PlayAnimByNameReceiverV59(const CommandC1DescriptorV59& d,ScriptCommandBehaviorV59 b):CanonicalScriptCommandV59(d,std::move(b),19){apply_constructor_operands();}
 std::int32_t receiver_kind() const noexcept override {return 19;}
};
class Script_PlayEffectReceiverV59 final : public CanonicalScriptCommandV59 {
 CommandOperandCellV59* operand_cell(std::uint32_t o) noexcept override {(void)o;return nullptr;}
 const CommandOperandCellV59* operand_cell(std::uint32_t o) const noexcept override {(void)o;return nullptr;}
public:
 Script_PlayEffectReceiverV59(const CommandC1DescriptorV59& d,ScriptCommandBehaviorV59 b):CanonicalScriptCommandV59(d,std::move(b),20){apply_constructor_operands();}
 std::int32_t receiver_kind() const noexcept override {return 20;}
};
class Script_StopEffectReceiverV59 final : public CanonicalScriptCommandV59 {
 CommandOperandCellV59* operand_cell(std::uint32_t o) noexcept override {(void)o;return nullptr;}
 const CommandOperandCellV59* operand_cell(std::uint32_t o) const noexcept override {(void)o;return nullptr;}
public:
 Script_StopEffectReceiverV59(const CommandC1DescriptorV59& d,ScriptCommandBehaviorV59 b):CanonicalScriptCommandV59(d,std::move(b),21){apply_constructor_operands();}
 std::int32_t receiver_kind() const noexcept override {return 21;}
};
class Script_ShowFlashReceiverV59 final : public CanonicalScriptCommandV59 {
 // Original+10 store is32-bit NULL; actual menu pointer cell is native-width.
 std::uintptr_t menu10_{};std::weak_ptr<void> actual_menu_owner_;
 std::uintptr_t* menu_pointer_cell() noexcept override{return &menu10_;}
 const std::uintptr_t* menu_pointer_cell() const noexcept override{return &menu10_;}
 std::weak_ptr<void>* menu_owner_cell() noexcept override{return &actual_menu_owner_;}
 const std::weak_ptr<void>* menu_owner_cell() const noexcept override{return &actual_menu_owner_;}
 CommandOperandCellV59* operand_cell(std::uint32_t o) noexcept override {(void)o;return nullptr;}
 const CommandOperandCellV59* operand_cell(std::uint32_t o) const noexcept override {(void)o;return nullptr;}
public:
 Script_ShowFlashReceiverV59(const CommandC1DescriptorV59& d,ScriptCommandBehaviorV59 b):CanonicalScriptCommandV59(d,std::move(b),22){apply_constructor_operands();}
 std::int32_t receiver_kind() const noexcept override {return 22;}
};
class Script_HideFlashReceiverV59 final : public CanonicalScriptCommandV59 {
 // Original+10 store is32-bit NULL; actual menu pointer cell is native-width.
 std::uintptr_t menu10_{};std::weak_ptr<void> actual_menu_owner_;
 std::uintptr_t* menu_pointer_cell() noexcept override{return &menu10_;}
 const std::uintptr_t* menu_pointer_cell() const noexcept override{return &menu10_;}
 std::weak_ptr<void>* menu_owner_cell() noexcept override{return &actual_menu_owner_;}
 const std::weak_ptr<void>* menu_owner_cell() const noexcept override{return &actual_menu_owner_;}
 CommandOperandCellV59* operand_cell(std::uint32_t o) noexcept override {(void)o;return nullptr;}
 const CommandOperandCellV59* operand_cell(std::uint32_t o) const noexcept override {(void)o;return nullptr;}
public:
 Script_HideFlashReceiverV59(const CommandC1DescriptorV59& d,ScriptCommandBehaviorV59 b):CanonicalScriptCommandV59(d,std::move(b),23){apply_constructor_operands();}
 std::int32_t receiver_kind() const noexcept override {return 23;}
};
class Script_LockCharacterReceiverV59 final : public CanonicalScriptCommandV59 {
 CommandOperandCellV59* operand_cell(std::uint32_t o) noexcept override {(void)o;return nullptr;}
 const CommandOperandCellV59* operand_cell(std::uint32_t o) const noexcept override {(void)o;return nullptr;}
public:
 Script_LockCharacterReceiverV59(const CommandC1DescriptorV59& d,ScriptCommandBehaviorV59 b):CanonicalScriptCommandV59(d,std::move(b),24){apply_constructor_operands();}
 std::int32_t receiver_kind() const noexcept override {return 24;}
};
class Script_UnlockCharacterReceiverV59 final : public CanonicalScriptCommandV59 {
 CommandOperandCellV59* operand_cell(std::uint32_t o) noexcept override {(void)o;return nullptr;}
 const CommandOperandCellV59* operand_cell(std::uint32_t o) const noexcept override {(void)o;return nullptr;}
public:
 Script_UnlockCharacterReceiverV59(const CommandC1DescriptorV59& d,ScriptCommandBehaviorV59 b):CanonicalScriptCommandV59(d,std::move(b),25){apply_constructor_operands();}
 std::int32_t receiver_kind() const noexcept override {return 25;}
};
class Script_WaitReceiverV59 final : public CanonicalScriptCommandV59 {
 CommandOperandCellV59 field14_{4,{}};
 CommandOperandCellV59 field10_{4,{}};
 CommandOperandCellV59* operand_cell(std::uint32_t o) noexcept override {if(o==20)return &field14_;if(o==16)return &field10_;(void)o;return nullptr;}
 const CommandOperandCellV59* operand_cell(std::uint32_t o) const noexcept override {if(o==20)return &field14_;if(o==16)return &field10_;(void)o;return nullptr;}
public:
 Script_WaitReceiverV59(const CommandC1DescriptorV59& d,ScriptCommandBehaviorV59 b):CanonicalScriptCommandV59(d,std::move(b),26){apply_constructor_operands();}
 std::int32_t receiver_kind() const noexcept override {return 26;}
};
class Script_SetFaeryStateReceiverV59 final : public CanonicalScriptCommandV59 {
 CommandOperandCellV59* operand_cell(std::uint32_t o) noexcept override {(void)o;return nullptr;}
 const CommandOperandCellV59* operand_cell(std::uint32_t o) const noexcept override {(void)o;return nullptr;}
public:
 Script_SetFaeryStateReceiverV59(const CommandC1DescriptorV59& d,ScriptCommandBehaviorV59 b):CanonicalScriptCommandV59(d,std::move(b),27){apply_constructor_operands();}
 std::int32_t receiver_kind() const noexcept override {return 27;}
};
class Script_PutCharacterInLimbusReceiverV59 final : public CanonicalScriptCommandV59 {
 CommandOperandCellV59* operand_cell(std::uint32_t o) noexcept override {(void)o;return nullptr;}
 const CommandOperandCellV59* operand_cell(std::uint32_t o) const noexcept override {(void)o;return nullptr;}
public:
 Script_PutCharacterInLimbusReceiverV59(const CommandC1DescriptorV59& d,ScriptCommandBehaviorV59 b):CanonicalScriptCommandV59(d,std::move(b),29){apply_constructor_operands();}
 std::int32_t receiver_kind() const noexcept override {return 29;}
};
class Script_SpawnCharacterReceiverV59 final : public CanonicalScriptCommandV59 {
 CommandOperandCellV59* operand_cell(std::uint32_t o) noexcept override {(void)o;return nullptr;}
 const CommandOperandCellV59* operand_cell(std::uint32_t o) const noexcept override {(void)o;return nullptr;}
public:
 Script_SpawnCharacterReceiverV59(const CommandC1DescriptorV59& d,ScriptCommandBehaviorV59 b):CanonicalScriptCommandV59(d,std::move(b),30){apply_constructor_operands();}
 std::int32_t receiver_kind() const noexcept override {return 30;}
};
class Script_PutCharacterInIdleReceiverV59 final : public CanonicalScriptCommandV59 {
 CommandOperandCellV59* operand_cell(std::uint32_t o) noexcept override {(void)o;return nullptr;}
 const CommandOperandCellV59* operand_cell(std::uint32_t o) const noexcept override {(void)o;return nullptr;}
public:
 Script_PutCharacterInIdleReceiverV59(const CommandC1DescriptorV59& d,ScriptCommandBehaviorV59 b):CanonicalScriptCommandV59(d,std::move(b),31){apply_constructor_operands();}
 std::int32_t receiver_kind() const noexcept override {return 31;}
};
class Script_MarkCharacterAsScriptedReceiverV59 final : public CanonicalScriptCommandV59 {
 CommandOperandCellV59* operand_cell(std::uint32_t o) noexcept override {(void)o;return nullptr;}
 const CommandOperandCellV59* operand_cell(std::uint32_t o) const noexcept override {(void)o;return nullptr;}
public:
 Script_MarkCharacterAsScriptedReceiverV59(const CommandC1DescriptorV59& d,ScriptCommandBehaviorV59 b):CanonicalScriptCommandV59(d,std::move(b),32){apply_constructor_operands();}
 std::int32_t receiver_kind() const noexcept override {return 32;}
};
class Script_StopActorReceiverV59 final : public CanonicalScriptCommandV59 {
 CommandOperandCellV59* operand_cell(std::uint32_t o) noexcept override {(void)o;return nullptr;}
 const CommandOperandCellV59* operand_cell(std::uint32_t o) const noexcept override {(void)o;return nullptr;}
public:
 Script_StopActorReceiverV59(const CommandC1DescriptorV59& d,ScriptCommandBehaviorV59 b):CanonicalScriptCommandV59(d,std::move(b),39){apply_constructor_operands();}
 std::int32_t receiver_kind() const noexcept override {return 39;}
};
class Script_MoveActorReceiverV59 final : public CanonicalScriptCommandV59 {
 CommandOperandCellV59 field18_{1,{}};
 CommandOperandCellV59 field10_{1,{}};
 CommandOperandCellV59 field14_{4,{}};
 CommandOperandCellV59* operand_cell(std::uint32_t o) noexcept override {if(o==24)return &field18_;if(o==16)return &field10_;if(o==20)return &field14_;(void)o;return nullptr;}
 const CommandOperandCellV59* operand_cell(std::uint32_t o) const noexcept override {if(o==24)return &field18_;if(o==16)return &field10_;if(o==20)return &field14_;(void)o;return nullptr;}
public:
 Script_MoveActorReceiverV59(const CommandC1DescriptorV59& d,ScriptCommandBehaviorV59 b):CanonicalScriptCommandV59(d,std::move(b),40){apply_constructor_operands();}
 std::int32_t receiver_kind() const noexcept override {return 40;}
};
class Script_LookActorReceiverV59 final : public CanonicalScriptCommandV59 {
 CommandOperandCellV59* operand_cell(std::uint32_t o) noexcept override {(void)o;return nullptr;}
 const CommandOperandCellV59* operand_cell(std::uint32_t o) const noexcept override {(void)o;return nullptr;}
public:
 Script_LookActorReceiverV59(const CommandC1DescriptorV59& d,ScriptCommandBehaviorV59 b):CanonicalScriptCommandV59(d,std::move(b),41){apply_constructor_operands();}
 std::int32_t receiver_kind() const noexcept override {return 41;}
};
class Script_ShowActorReceiverV59 final : public CanonicalScriptCommandV59 {
 CommandOperandCellV59* operand_cell(std::uint32_t o) noexcept override {(void)o;return nullptr;}
 const CommandOperandCellV59* operand_cell(std::uint32_t o) const noexcept override {(void)o;return nullptr;}
public:
 Script_ShowActorReceiverV59(const CommandC1DescriptorV59& d,ScriptCommandBehaviorV59 b):CanonicalScriptCommandV59(d,std::move(b),42){apply_constructor_operands();}
 std::int32_t receiver_kind() const noexcept override {return 42;}
};
class Script_HideActorReceiverV59 final : public CanonicalScriptCommandV59 {
 CommandOperandCellV59* operand_cell(std::uint32_t o) noexcept override {(void)o;return nullptr;}
 const CommandOperandCellV59* operand_cell(std::uint32_t o) const noexcept override {(void)o;return nullptr;}
public:
 Script_HideActorReceiverV59(const CommandC1DescriptorV59& d,ScriptCommandBehaviorV59 b):CanonicalScriptCommandV59(d,std::move(b),43){apply_constructor_operands();}
 std::int32_t receiver_kind() const noexcept override {return 43;}
};
class Script_KillActorReceiverV59 final : public CanonicalScriptCommandV59 {
 CommandOperandCellV59* operand_cell(std::uint32_t o) noexcept override {(void)o;return nullptr;}
 const CommandOperandCellV59* operand_cell(std::uint32_t o) const noexcept override {(void)o;return nullptr;}
public:
 Script_KillActorReceiverV59(const CommandC1DescriptorV59& d,ScriptCommandBehaviorV59 b):CanonicalScriptCommandV59(d,std::move(b),44){apply_constructor_operands();}
 std::int32_t receiver_kind() const noexcept override {return 44;}
};
class Script_PlayActorAnimReceiverV59 final : public CanonicalScriptCommandV59 {
 CommandOperandCellV59 field18_{4,{}};
 CommandOperandCellV59 field10_{4,{}};
 CommandOperandCellV59 field14_{4,{}};
 CommandOperandCellV59* operand_cell(std::uint32_t o) noexcept override {if(o==24)return &field18_;if(o==16)return &field10_;if(o==20)return &field14_;(void)o;return nullptr;}
 const CommandOperandCellV59* operand_cell(std::uint32_t o) const noexcept override {if(o==24)return &field18_;if(o==16)return &field10_;if(o==20)return &field14_;(void)o;return nullptr;}
public:
 Script_PlayActorAnimReceiverV59(const CommandC1DescriptorV59& d,ScriptCommandBehaviorV59 b):CanonicalScriptCommandV59(d,std::move(b),45){apply_constructor_operands();}
 std::int32_t receiver_kind() const noexcept override {return 45;}
};
class Script_SetActorPositionReceiverV59 final : public CanonicalScriptCommandV59 {
 CommandOperandCellV59* operand_cell(std::uint32_t o) noexcept override {(void)o;return nullptr;}
 const CommandOperandCellV59* operand_cell(std::uint32_t o) const noexcept override {(void)o;return nullptr;}
public:
 Script_SetActorPositionReceiverV59(const CommandC1DescriptorV59& d,ScriptCommandBehaviorV59 b):CanonicalScriptCommandV59(d,std::move(b),46){apply_constructor_operands();}
 std::int32_t receiver_kind() const noexcept override {return 46;}
};
class Script_UnEquipHandsReceiverV59 final : public CanonicalScriptCommandV59 {
 CommandOperandCellV59* operand_cell(std::uint32_t o) noexcept override {(void)o;return nullptr;}
 const CommandOperandCellV59* operand_cell(std::uint32_t o) const noexcept override {(void)o;return nullptr;}
public:
 Script_UnEquipHandsReceiverV59(const CommandC1DescriptorV59& d,ScriptCommandBehaviorV59 b):CanonicalScriptCommandV59(d,std::move(b),51){apply_constructor_operands();}
 std::int32_t receiver_kind() const noexcept override {return 51;}
};
class Script_ReEquipHandsReceiverV59 final : public CanonicalScriptCommandV59 {
 CommandOperandCellV59* operand_cell(std::uint32_t o) noexcept override {(void)o;return nullptr;}
 const CommandOperandCellV59* operand_cell(std::uint32_t o) const noexcept override {(void)o;return nullptr;}
public:
 Script_ReEquipHandsReceiverV59(const CommandC1DescriptorV59& d,ScriptCommandBehaviorV59 b):CanonicalScriptCommandV59(d,std::move(b),52){apply_constructor_operands();}
 std::int32_t receiver_kind() const noexcept override {return 52;}
};
class Script_OpenDoorReceiverV59 final : public CanonicalScriptCommandV59 {
 CommandOperandCellV59 field14_{1,{}};
 CommandOperandCellV59 field10_{4,{}};
 CommandOperandCellV59* operand_cell(std::uint32_t o) noexcept override {if(o==20)return &field14_;if(o==16)return &field10_;(void)o;return nullptr;}
 const CommandOperandCellV59* operand_cell(std::uint32_t o) const noexcept override {if(o==20)return &field14_;if(o==16)return &field10_;(void)o;return nullptr;}
public:
 Script_OpenDoorReceiverV59(const CommandC1DescriptorV59& d,ScriptCommandBehaviorV59 b):CanonicalScriptCommandV59(d,std::move(b),54){apply_constructor_operands();}
 std::int32_t receiver_kind() const noexcept override {return 54;}
};
class Script_CloseDoorReceiverV59 final : public CanonicalScriptCommandV59 {
 CommandOperandCellV59 field14_{1,{}};
 CommandOperandCellV59 field10_{4,{}};
 CommandOperandCellV59* operand_cell(std::uint32_t o) noexcept override {if(o==20)return &field14_;if(o==16)return &field10_;(void)o;return nullptr;}
 const CommandOperandCellV59* operand_cell(std::uint32_t o) const noexcept override {if(o==20)return &field14_;if(o==16)return &field10_;(void)o;return nullptr;}
public:
 Script_CloseDoorReceiverV59(const CommandC1DescriptorV59& d,ScriptCommandBehaviorV59 b):CanonicalScriptCommandV59(d,std::move(b),55){apply_constructor_operands();}
 std::int32_t receiver_kind() const noexcept override {return 55;}
};
class Script_RestartLevelReceiverV59 final : public CanonicalScriptCommandV59 {
 CommandOperandCellV59* operand_cell(std::uint32_t o) noexcept override {(void)o;return nullptr;}
 const CommandOperandCellV59* operand_cell(std::uint32_t o) const noexcept override {(void)o;return nullptr;}
public:
 Script_RestartLevelReceiverV59(const CommandC1DescriptorV59& d,ScriptCommandBehaviorV59 b):CanonicalScriptCommandV59(d,std::move(b),63){apply_constructor_operands();}
 std::int32_t receiver_kind() const noexcept override {return 63;}
};
class Script_ShowTrophiesReceiverV59 final : public CanonicalScriptCommandV59 {
 CommandOperandCellV59* operand_cell(std::uint32_t o) noexcept override {(void)o;return nullptr;}
 const CommandOperandCellV59* operand_cell(std::uint32_t o) const noexcept override {(void)o;return nullptr;}
public:
 Script_ShowTrophiesReceiverV59(const CommandC1DescriptorV59& d,ScriptCommandBehaviorV59 b):CanonicalScriptCommandV59(d,std::move(b),68){apply_constructor_operands();}
 std::int32_t receiver_kind() const noexcept override {return 68;}
};
class Script_SaveGameReceiverV59 final : public CanonicalScriptCommandV59 {
 CommandOperandCellV59* operand_cell(std::uint32_t o) noexcept override {(void)o;return nullptr;}
 const CommandOperandCellV59* operand_cell(std::uint32_t o) const noexcept override {(void)o;return nullptr;}
public:
 Script_SaveGameReceiverV59(const CommandC1DescriptorV59& d,ScriptCommandBehaviorV59 b):CanonicalScriptCommandV59(d,std::move(b),69){apply_constructor_operands();}
 std::int32_t receiver_kind() const noexcept override {return 69;}
};
class Script_BlockSaveGameReceiverV59 final : public CanonicalScriptCommandV59 {
 CommandOperandCellV59* operand_cell(std::uint32_t o) noexcept override {(void)o;return nullptr;}
 const CommandOperandCellV59* operand_cell(std::uint32_t o) const noexcept override {(void)o;return nullptr;}
public:
 Script_BlockSaveGameReceiverV59(const CommandC1DescriptorV59& d,ScriptCommandBehaviorV59 b):CanonicalScriptCommandV59(d,std::move(b),70){apply_constructor_operands();}
 std::int32_t receiver_kind() const noexcept override {return 70;}
};
class Script_LockTutorialReceiverV59 final : public CanonicalScriptCommandV59 {
 CommandOperandCellV59* operand_cell(std::uint32_t o) noexcept override {(void)o;return nullptr;}
 const CommandOperandCellV59* operand_cell(std::uint32_t o) const noexcept override {(void)o;return nullptr;}
public:
 Script_LockTutorialReceiverV59(const CommandC1DescriptorV59& d,ScriptCommandBehaviorV59 b):CanonicalScriptCommandV59(d,std::move(b),77){apply_constructor_operands();}
 std::int32_t receiver_kind() const noexcept override {return 77;}
};
class Script_DoTutorialReceiverV59 final : public CanonicalScriptCommandV59 {
 CommandOperandCellV59* operand_cell(std::uint32_t o) noexcept override {(void)o;return nullptr;}
 const CommandOperandCellV59* operand_cell(std::uint32_t o) const noexcept override {(void)o;return nullptr;}
public:
 Script_DoTutorialReceiverV59(const CommandC1DescriptorV59& d,ScriptCommandBehaviorV59 b):CanonicalScriptCommandV59(d,std::move(b),78){apply_constructor_operands();}
 std::int32_t receiver_kind() const noexcept override {return 78;}
};
class Script_FlushMessagesReceiverV59 final : public CanonicalScriptCommandV59 {
 CommandOperandCellV59* operand_cell(std::uint32_t o) noexcept override {(void)o;return nullptr;}
 const CommandOperandCellV59* operand_cell(std::uint32_t o) const noexcept override {(void)o;return nullptr;}
public:
 Script_FlushMessagesReceiverV59(const CommandC1DescriptorV59& d,ScriptCommandBehaviorV59 b):CanonicalScriptCommandV59(d,std::move(b),79){apply_constructor_operands();}
 std::int32_t receiver_kind() const noexcept override {return 79;}
};
class Script_EmptyImplReceiverV59 final : public CanonicalScriptCommandV59 {
 CommandOperandCellV59* operand_cell(std::uint32_t o) noexcept override {(void)o;return nullptr;}
 const CommandOperandCellV59* operand_cell(std::uint32_t o) const noexcept override {(void)o;return nullptr;}
public:
 Script_EmptyImplReceiverV59(const CommandC1DescriptorV59& desc,ScriptCommandBehaviorV59 behavior):CanonicalScriptCommandV59(desc,std::move(behavior),9){apply_constructor_operands();}
 std::int32_t receiver_kind() const noexcept override {return 9;}
};
class Script_StartDialogIDReceiverV59 final : public CanonicalScriptCommandV59 {
 CommandOperandCellV59* operand_cell(std::uint32_t o) noexcept override {(void)o;return nullptr;}
 const CommandOperandCellV59* operand_cell(std::uint32_t o) const noexcept override {(void)o;return nullptr;}
public:
 Script_StartDialogIDReceiverV59(const CommandC1DescriptorV59& desc,ScriptCommandBehaviorV59 behavior):CanonicalScriptCommandV59(desc,std::move(behavior),11){apply_constructor_operands();}
 std::int32_t receiver_kind() const noexcept override {return 11;}
};
class Script_PlayAnimByIdReceiverV59 final : public CanonicalScriptCommandV59 {
 CommandOperandCellV59* operand_cell(std::uint32_t o) noexcept override {(void)o;return nullptr;}
 const CommandOperandCellV59* operand_cell(std::uint32_t o) const noexcept override {(void)o;return nullptr;}
public:
 Script_PlayAnimByIdReceiverV59(const CommandC1DescriptorV59& desc,ScriptCommandBehaviorV59 behavior):CanonicalScriptCommandV59(desc,std::move(behavior),18){apply_constructor_operands();}
 std::int32_t receiver_kind() const noexcept override {return 18;}
};
class Script_IncFaeryLevelReceiverV59 final : public CanonicalScriptCommandV59 {
 CommandOperandCellV59* operand_cell(std::uint32_t o) noexcept override {(void)o;return nullptr;}
 const CommandOperandCellV59* operand_cell(std::uint32_t o) const noexcept override {(void)o;return nullptr;}
public:
 Script_IncFaeryLevelReceiverV59(const CommandC1DescriptorV59& desc,ScriptCommandBehaviorV59 behavior):CanonicalScriptCommandV59(desc,std::move(behavior),28){apply_constructor_operands();}
 std::int32_t receiver_kind() const noexcept override {return 28;}
};
class Script_AIEnableTimerReceiverV59 final : public CanonicalScriptCommandV59 {
 CommandOperandCellV59* operand_cell(std::uint32_t o) noexcept override {(void)o;return nullptr;}
 const CommandOperandCellV59* operand_cell(std::uint32_t o) const noexcept override {(void)o;return nullptr;}
public:
 Script_AIEnableTimerReceiverV59(const CommandC1DescriptorV59& desc,ScriptCommandBehaviorV59 behavior):CanonicalScriptCommandV59(desc,std::move(behavior),33){apply_constructor_operands();}
 std::int32_t receiver_kind() const noexcept override {return 33;}
};
class Script_AIResetTimerReceiverV59 final : public CanonicalScriptCommandV59 {
 CommandOperandCellV59* operand_cell(std::uint32_t o) noexcept override {(void)o;return nullptr;}
 const CommandOperandCellV59* operand_cell(std::uint32_t o) const noexcept override {(void)o;return nullptr;}
public:
 Script_AIResetTimerReceiverV59(const CommandC1DescriptorV59& desc,ScriptCommandBehaviorV59 behavior):CanonicalScriptCommandV59(desc,std::move(behavior),34){apply_constructor_operands();}
 std::int32_t receiver_kind() const noexcept override {return 34;}
};
class Script_AIDoSkillReceiverV59 final : public CanonicalScriptCommandV59 {
 CommandOperandCellV59* operand_cell(std::uint32_t o) noexcept override {(void)o;return nullptr;}
 const CommandOperandCellV59* operand_cell(std::uint32_t o) const noexcept override {(void)o;return nullptr;}
public:
 Script_AIDoSkillReceiverV59(const CommandC1DescriptorV59& desc,ScriptCommandBehaviorV59 behavior):CanonicalScriptCommandV59(desc,std::move(behavior),35){apply_constructor_operands();}
 std::int32_t receiver_kind() const noexcept override {return 35;}
};
class Script_AISetHPReceiverV59 final : public CanonicalScriptCommandV59 {
 CommandOperandCellV59* operand_cell(std::uint32_t o) noexcept override {(void)o;return nullptr;}
 const CommandOperandCellV59* operand_cell(std::uint32_t o) const noexcept override {(void)o;return nullptr;}
public:
 Script_AISetHPReceiverV59(const CommandC1DescriptorV59& desc,ScriptCommandBehaviorV59 behavior):CanonicalScriptCommandV59(desc,std::move(behavior),36){apply_constructor_operands();}
 std::int32_t receiver_kind() const noexcept override {return 36;}
};
class Script_AIChangeScriptReceiverV59 final : public CanonicalScriptCommandV59 {
 CommandOperandCellV59* operand_cell(std::uint32_t o) noexcept override {(void)o;return nullptr;}
 const CommandOperandCellV59* operand_cell(std::uint32_t o) const noexcept override {(void)o;return nullptr;}
public:
 Script_AIChangeScriptReceiverV59(const CommandC1DescriptorV59& desc,ScriptCommandBehaviorV59 behavior):CanonicalScriptCommandV59(desc,std::move(behavior),37){apply_constructor_operands();}
 std::int32_t receiver_kind() const noexcept override {return 37;}
};
class Script_AISetIntReceiverV59 final : public CanonicalScriptCommandV59 {
 CommandOperandCellV59* operand_cell(std::uint32_t o) noexcept override {(void)o;return nullptr;}
 const CommandOperandCellV59* operand_cell(std::uint32_t o) const noexcept override {(void)o;return nullptr;}
public:
 Script_AISetIntReceiverV59(const CommandC1DescriptorV59& desc,ScriptCommandBehaviorV59 behavior):CanonicalScriptCommandV59(desc,std::move(behavior),38){apply_constructor_operands();}
 std::int32_t receiver_kind() const noexcept override {return 38;}
};
class Script_SetActorMasterReceiverV59 final : public CanonicalScriptCommandV59 {
 CommandOperandCellV59* operand_cell(std::uint32_t o) noexcept override {(void)o;return nullptr;}
 const CommandOperandCellV59* operand_cell(std::uint32_t o) const noexcept override {(void)o;return nullptr;}
public:
 Script_SetActorMasterReceiverV59(const CommandC1DescriptorV59& desc,ScriptCommandBehaviorV59 behavior):CanonicalScriptCommandV59(desc,std::move(behavior),47){apply_constructor_operands();}
 std::int32_t receiver_kind() const noexcept override {return 47;}
};
class Script_DropLootReceiverV59 final : public CanonicalScriptCommandV59 {
 CommandOperandCellV59* operand_cell(std::uint32_t o) noexcept override {(void)o;return nullptr;}
 const CommandOperandCellV59* operand_cell(std::uint32_t o) const noexcept override {(void)o;return nullptr;}
public:
 Script_DropLootReceiverV59(const CommandC1DescriptorV59& desc,ScriptCommandBehaviorV59 behavior):CanonicalScriptCommandV59(desc,std::move(behavior),48){apply_constructor_operands();}
 std::int32_t receiver_kind() const noexcept override {return 48;}
};
class Script_AutoEquipReceiverV59 final : public CanonicalScriptCommandV59 {
 CommandOperandCellV59* operand_cell(std::uint32_t o) noexcept override {(void)o;return nullptr;}
 const CommandOperandCellV59* operand_cell(std::uint32_t o) const noexcept override {(void)o;return nullptr;}
public:
 Script_AutoEquipReceiverV59(const CommandC1DescriptorV59& desc,ScriptCommandBehaviorV59 behavior):CanonicalScriptCommandV59(desc,std::move(behavior),49){apply_constructor_operands();}
 std::int32_t receiver_kind() const noexcept override {return 49;}
};
class Script_UnEquipItemFromSlotReceiverV59 final : public CanonicalScriptCommandV59 {
 CommandOperandCellV59* operand_cell(std::uint32_t o) noexcept override {(void)o;return nullptr;}
 const CommandOperandCellV59* operand_cell(std::uint32_t o) const noexcept override {(void)o;return nullptr;}
public:
 Script_UnEquipItemFromSlotReceiverV59(const CommandC1DescriptorV59& desc,ScriptCommandBehaviorV59 behavior):CanonicalScriptCommandV59(desc,std::move(behavior),50){apply_constructor_operands();}
 std::int32_t receiver_kind() const noexcept override {return 50;}
};
class Script_SetStaticReceiverV59 final : public CanonicalScriptCommandV59 {
 CommandOperandCellV59* operand_cell(std::uint32_t o) noexcept override {(void)o;return nullptr;}
 const CommandOperandCellV59* operand_cell(std::uint32_t o) const noexcept override {(void)o;return nullptr;}
public:
 Script_SetStaticReceiverV59(const CommandC1DescriptorV59& desc,ScriptCommandBehaviorV59 behavior):CanonicalScriptCommandV59(desc,std::move(behavior),53){apply_constructor_operands();}
 std::int32_t receiver_kind() const noexcept override {return 53;}
};
class Script_ActivateProjectileTrapReceiverV59 final : public CanonicalScriptCommandV59 {
 // Execute source stores an actual target object pointer here. Preserve
 // original32-bit NULL provenance with one native-width nonowning cell.
 std::uintptr_t target10_{};std::weak_ptr<void> actual_target_owner_;
 std::uintptr_t* native_pointer_cell(std::uint32_t o) noexcept override{return o==16?&target10_:nullptr;}
 const std::uintptr_t* native_pointer_cell(std::uint32_t o) const noexcept override{return o==16?&target10_:nullptr;}
 std::weak_ptr<void>* native_pointer_owner_cell(std::uint32_t o) noexcept override{return o==16?&actual_target_owner_:nullptr;}
 const std::weak_ptr<void>* native_pointer_owner_cell(std::uint32_t o) const noexcept override{return o==16?&actual_target_owner_:nullptr;}
 CommandOperandCellV59* operand_cell(std::uint32_t o) noexcept override {(void)o;return nullptr;}
 const CommandOperandCellV59* operand_cell(std::uint32_t o) const noexcept override {(void)o;return nullptr;}
public:
 Script_ActivateProjectileTrapReceiverV59(const CommandC1DescriptorV59& desc,ScriptCommandBehaviorV59 behavior):CanonicalScriptCommandV59(desc,std::move(behavior),56){apply_constructor_operands();}
 std::int32_t receiver_kind() const noexcept override {return 56;}
};
class Script_DeactivateProjectileTrapReceiverV59 final : public CanonicalScriptCommandV59 {
 // Execute source stores an actual target object pointer here. Preserve
 // original32-bit NULL provenance with one native-width nonowning cell.
 std::uintptr_t target10_{};std::weak_ptr<void> actual_target_owner_;
 std::uintptr_t* native_pointer_cell(std::uint32_t o) noexcept override{return o==16?&target10_:nullptr;}
 const std::uintptr_t* native_pointer_cell(std::uint32_t o) const noexcept override{return o==16?&target10_:nullptr;}
 std::weak_ptr<void>* native_pointer_owner_cell(std::uint32_t o) noexcept override{return o==16?&actual_target_owner_:nullptr;}
 const std::weak_ptr<void>* native_pointer_owner_cell(std::uint32_t o) const noexcept override{return o==16?&actual_target_owner_:nullptr;}
 CommandOperandCellV59* operand_cell(std::uint32_t o) noexcept override {(void)o;return nullptr;}
 const CommandOperandCellV59* operand_cell(std::uint32_t o) const noexcept override {(void)o;return nullptr;}
public:
 Script_DeactivateProjectileTrapReceiverV59(const CommandC1DescriptorV59& desc,ScriptCommandBehaviorV59 behavior):CanonicalScriptCommandV59(desc,std::move(behavior),57){apply_constructor_operands();}
 std::int32_t receiver_kind() const noexcept override {return 57;}
};
class Script_ActivateTriggerPlateReceiverV59 final : public CanonicalScriptCommandV59 {
 // Execute source stores an actual target object pointer here. Preserve
 // original32-bit NULL provenance with one native-width nonowning cell.
 std::uintptr_t target10_{};std::weak_ptr<void> actual_target_owner_;
 std::uintptr_t* native_pointer_cell(std::uint32_t o) noexcept override{return o==16?&target10_:nullptr;}
 const std::uintptr_t* native_pointer_cell(std::uint32_t o) const noexcept override{return o==16?&target10_:nullptr;}
 std::weak_ptr<void>* native_pointer_owner_cell(std::uint32_t o) noexcept override{return o==16?&actual_target_owner_:nullptr;}
 const std::weak_ptr<void>* native_pointer_owner_cell(std::uint32_t o) const noexcept override{return o==16?&actual_target_owner_:nullptr;}
 CommandOperandCellV59* operand_cell(std::uint32_t o) noexcept override {(void)o;return nullptr;}
 const CommandOperandCellV59* operand_cell(std::uint32_t o) const noexcept override {(void)o;return nullptr;}
public:
 Script_ActivateTriggerPlateReceiverV59(const CommandC1DescriptorV59& desc,ScriptCommandBehaviorV59 behavior):CanonicalScriptCommandV59(desc,std::move(behavior),58){apply_constructor_operands();}
 std::int32_t receiver_kind() const noexcept override {return 58;}
};
class Script_DeactivateTriggerPlateReceiverV59 final : public CanonicalScriptCommandV59 {
 // Execute source stores an actual target object pointer here. Preserve
 // original32-bit NULL provenance with one native-width nonowning cell.
 std::uintptr_t target10_{};std::weak_ptr<void> actual_target_owner_;
 std::uintptr_t* native_pointer_cell(std::uint32_t o) noexcept override{return o==16?&target10_:nullptr;}
 const std::uintptr_t* native_pointer_cell(std::uint32_t o) const noexcept override{return o==16?&target10_:nullptr;}
 std::weak_ptr<void>* native_pointer_owner_cell(std::uint32_t o) noexcept override{return o==16?&actual_target_owner_:nullptr;}
 const std::weak_ptr<void>* native_pointer_owner_cell(std::uint32_t o) const noexcept override{return o==16?&actual_target_owner_:nullptr;}
 CommandOperandCellV59* operand_cell(std::uint32_t o) noexcept override {(void)o;return nullptr;}
 const CommandOperandCellV59* operand_cell(std::uint32_t o) const noexcept override {(void)o;return nullptr;}
public:
 Script_DeactivateTriggerPlateReceiverV59(const CommandC1DescriptorV59& desc,ScriptCommandBehaviorV59 behavior):CanonicalScriptCommandV59(desc,std::move(behavior),59){apply_constructor_operands();}
 std::int32_t receiver_kind() const noexcept override {return 59;}
};
class Script_SpawnContainerReceiverV59 final : public CanonicalScriptCommandV59 {
 CommandOperandCellV59* operand_cell(std::uint32_t o) noexcept override {(void)o;return nullptr;}
 const CommandOperandCellV59* operand_cell(std::uint32_t o) const noexcept override {(void)o;return nullptr;}
public:
 Script_SpawnContainerReceiverV59(const CommandC1DescriptorV59& desc,ScriptCommandBehaviorV59 behavior):CanonicalScriptCommandV59(desc,std::move(behavior),60){apply_constructor_operands();}
 std::int32_t receiver_kind() const noexcept override {return 60;}
};
class Script_SetLevelStateReceiverV59 final : public CanonicalScriptCommandV59 {
 CommandOperandCellV59* operand_cell(std::uint32_t o) noexcept override {(void)o;return nullptr;}
 const CommandOperandCellV59* operand_cell(std::uint32_t o) const noexcept override {(void)o;return nullptr;}
public:
 Script_SetLevelStateReceiverV59(const CommandC1DescriptorV59& desc,ScriptCommandBehaviorV59 behavior):CanonicalScriptCommandV59(desc,std::move(behavior),61){apply_constructor_operands();}
 std::int32_t receiver_kind() const noexcept override {return 61;}
};
class Script_SetWorldMapLocationStateReceiverV59 final : public CanonicalScriptCommandV59 {
 CommandOperandCellV59* operand_cell(std::uint32_t o) noexcept override {(void)o;return nullptr;}
 const CommandOperandCellV59* operand_cell(std::uint32_t o) const noexcept override {(void)o;return nullptr;}
public:
 Script_SetWorldMapLocationStateReceiverV59(const CommandC1DescriptorV59& desc,ScriptCommandBehaviorV59 behavior):CanonicalScriptCommandV59(desc,std::move(behavior),62){apply_constructor_operands();}
 std::int32_t receiver_kind() const noexcept override {return 62;}
};
class Script_ChangeLevelReceiverV59 final : public CanonicalScriptCommandV59 {
 CommandOperandCellV59* operand_cell(std::uint32_t o) noexcept override {(void)o;return nullptr;}
 const CommandOperandCellV59* operand_cell(std::uint32_t o) const noexcept override {(void)o;return nullptr;}
public:
 Script_ChangeLevelReceiverV59(const CommandC1DescriptorV59& desc,ScriptCommandBehaviorV59 behavior):CanonicalScriptCommandV59(desc,std::move(behavior),64){apply_constructor_operands();}
 std::int32_t receiver_kind() const noexcept override {return 64;}
};
class Script_EndGameReceiverV59 final : public CanonicalScriptCommandV59 {
 CommandOperandCellV59* operand_cell(std::uint32_t o) noexcept override {(void)o;return nullptr;}
 const CommandOperandCellV59* operand_cell(std::uint32_t o) const noexcept override {(void)o;return nullptr;}
public:
 Script_EndGameReceiverV59(const CommandC1DescriptorV59& desc,ScriptCommandBehaviorV59 behavior):CanonicalScriptCommandV59(desc,std::move(behavior),65){apply_constructor_operands();}
 std::int32_t receiver_kind() const noexcept override {return 65;}
};
class Script_AwardTrophyReceiverV59 final : public CanonicalScriptCommandV59 {
 CommandOperandCellV59* operand_cell(std::uint32_t o) noexcept override {(void)o;return nullptr;}
 const CommandOperandCellV59* operand_cell(std::uint32_t o) const noexcept override {(void)o;return nullptr;}
public:
 Script_AwardTrophyReceiverV59(const CommandC1DescriptorV59& desc,ScriptCommandBehaviorV59 behavior):CanonicalScriptCommandV59(desc,std::move(behavior),66){apply_constructor_operands();}
 std::int32_t receiver_kind() const noexcept override {return 66;}
};
class Script_AwardEndGameTrophiesReceiverV59 final : public CanonicalScriptCommandV59 {
 CommandOperandCellV59* operand_cell(std::uint32_t o) noexcept override {(void)o;return nullptr;}
 const CommandOperandCellV59* operand_cell(std::uint32_t o) const noexcept override {(void)o;return nullptr;}
public:
 Script_AwardEndGameTrophiesReceiverV59(const CommandC1DescriptorV59& desc,ScriptCommandBehaviorV59 behavior):CanonicalScriptCommandV59(desc,std::move(behavior),67){apply_constructor_operands();}
 std::int32_t receiver_kind() const noexcept override {return 67;}
};
class Script_EnqueueTutorialMessageReceiverV59 final : public CanonicalScriptCommandV59 {
 CommandOperandCellV59* operand_cell(std::uint32_t o) noexcept override {(void)o;return nullptr;}
 const CommandOperandCellV59* operand_cell(std::uint32_t o) const noexcept override {(void)o;return nullptr;}
public:
 Script_EnqueueTutorialMessageReceiverV59(const CommandC1DescriptorV59& desc,ScriptCommandBehaviorV59 behavior):CanonicalScriptCommandV59(desc,std::move(behavior),71){apply_constructor_operands();}
 std::int32_t receiver_kind() const noexcept override {return 71;}
};
class Script_SkipTutorialMessageReceiverV59 final : public CanonicalScriptCommandV59 {
 CommandOperandCellV59* operand_cell(std::uint32_t o) noexcept override {(void)o;return nullptr;}
 const CommandOperandCellV59* operand_cell(std::uint32_t o) const noexcept override {(void)o;return nullptr;}
public:
 Script_SkipTutorialMessageReceiverV59(const CommandC1DescriptorV59& desc,ScriptCommandBehaviorV59 behavior):CanonicalScriptCommandV59(desc,std::move(behavior),72){apply_constructor_operands();}
 std::int32_t receiver_kind() const noexcept override {return 72;}
};
class Script_SkipAllTutorialMessagesReceiverV59 final : public CanonicalScriptCommandV59 {
 CommandOperandCellV59* operand_cell(std::uint32_t o) noexcept override {(void)o;return nullptr;}
 const CommandOperandCellV59* operand_cell(std::uint32_t o) const noexcept override {(void)o;return nullptr;}
public:
 Script_SkipAllTutorialMessagesReceiverV59(const CommandC1DescriptorV59& desc,ScriptCommandBehaviorV59 behavior):CanonicalScriptCommandV59(desc,std::move(behavior),73){apply_constructor_operands();}
 std::int32_t receiver_kind() const noexcept override {return 73;}
};
class Script_EnqueueCharMenuTutorialMessageReceiverV59 final : public CanonicalScriptCommandV59 {
 CommandOperandCellV59* operand_cell(std::uint32_t o) noexcept override {(void)o;return nullptr;}
 const CommandOperandCellV59* operand_cell(std::uint32_t o) const noexcept override {(void)o;return nullptr;}
public:
 Script_EnqueueCharMenuTutorialMessageReceiverV59(const CommandC1DescriptorV59& desc,ScriptCommandBehaviorV59 behavior):CanonicalScriptCommandV59(desc,std::move(behavior),74){apply_constructor_operands();}
 std::int32_t receiver_kind() const noexcept override {return 74;}
};
class Script_SkipCharMenuTutorialMessageReceiverV59 final : public CanonicalScriptCommandV59 {
 CommandOperandCellV59* operand_cell(std::uint32_t o) noexcept override {(void)o;return nullptr;}
 const CommandOperandCellV59* operand_cell(std::uint32_t o) const noexcept override {(void)o;return nullptr;}
public:
 Script_SkipCharMenuTutorialMessageReceiverV59(const CommandC1DescriptorV59& desc,ScriptCommandBehaviorV59 behavior):CanonicalScriptCommandV59(desc,std::move(behavior),75){apply_constructor_operands();}
 std::int32_t receiver_kind() const noexcept override {return 75;}
};
class Script_SkipAllCharMenuTutorialMessagesReceiverV59 final : public CanonicalScriptCommandV59 {
 CommandOperandCellV59* operand_cell(std::uint32_t o) noexcept override {(void)o;return nullptr;}
 const CommandOperandCellV59* operand_cell(std::uint32_t o) const noexcept override {(void)o;return nullptr;}
public:
 Script_SkipAllCharMenuTutorialMessagesReceiverV59(const CommandC1DescriptorV59& desc,ScriptCommandBehaviorV59 behavior):CanonicalScriptCommandV59(desc,std::move(behavior),76){apply_constructor_operands();}
 std::int32_t receiver_kind() const noexcept override {return 76;}
};
bool create_script_command_receiver_v59(std::int32_t,ScriptCommandBehaviorV59,
 std::shared_ptr<CanonicalScriptCommandV59>& out,std::string&);
ScriptManagerServicesV52 bind_script_command_factories_v59(ScriptManagerServicesV52,ScriptCommandBehaviorV59);
}
