#pragma once
#include "script_manager_owner_v52.hpp"
#include <variant>
namespace dh2::loader {
struct CommandC1StoreV58 {std::uint32_t offset,width,value,source;};
struct CommandC1DescriptorV58 {std::int32_t kind;const char* class_name;std::uint32_t factory,Init,original_size;std::vector<CommandC1StoreV58> stores;};
class CanonicalScriptCommandV58;
struct CheckedCommandBorrowV58 {
 std::shared_ptr<CanonicalScriptCommandV58> actual_receiver;
 std::shared_ptr<ScriptCommandDataV52> actual_data;
 std::uintptr_t identity{};const CommandC1DescriptorV58* descriptor{};
 std::uint8_t* skip4{};std::int32_t* kind8{};std::uintptr_t* data_c{};
};
struct ScriptCommandBehaviorV58 {
 std::shared_ptr<void> actual_owner;
 // Nontrivial original Init bodies ONLY. Source base455628 is actual BX LR.
 std::function<bool(const CheckedCommandBorrowV58&,std::string&)> init;
 std::function<bool(const CheckedCommandBorrowV58&,bool,std::int32_t,std::string&)> execute;
};
struct OperandScalarV58 {std::uint8_t width{};std::uint32_t bits{};};
struct OperandPointerV58 {std::uintptr_t value{};std::shared_ptr<void> actual_owner;};
// ONE actual receiver member, not parallel word/pointer fields. Source C1
// opaque words remain words until Main's proven typed producer writes a pointer.
using CommandOperandV58=std::variant<OperandScalarV58,OperandPointerV58>;
class CanonicalScriptCommandV58 : public std::enable_shared_from_this<CanonicalScriptCommandV58> {
 const CommandC1DescriptorV58& descriptor_;
 std::uint8_t skip4_{};std::int32_t kind8_{-1};std::uintptr_t data_c_{};
 std::shared_ptr<ScriptCommandDataV52> data_;
 std::map<std::uint32_t,CommandOperandV58> operands_;
 ScriptCommandBehaviorV58 behaviors_;bool initialized_{},released_{},busy_{},failed_{};std::string failure_;
 bool fail(const std::string&,std::string&);
protected:
 CanonicalScriptCommandV58(const CommandC1DescriptorV58&,ScriptCommandBehaviorV58);
public:
 virtual ~CanonicalScriptCommandV58()=default;
 virtual std::int32_t receiver_kind()const noexcept=0;
 const CommandC1DescriptorV58& descriptor()const noexcept{return descriptor_;}
 std::uintptr_t identity()const noexcept{return reinterpret_cast<std::uintptr_t>(this);}
 bool checked_data_borrow(CheckedCommandBorrowV58&,std::string&);
 ScriptCommandBorrowV52 constructor_borrow();
 bool init(std::string&);bool execute(bool,std::int32_t,std::string&);
 bool release_base_storage(std::string&);
 bool read_operand_word(std::uint32_t,std::uint8_t,std::uint32_t&,std::string&)const;
 bool write_operand_word(std::uint32_t,std::uint8_t,std::uint32_t,std::string&);
 bool read_operand_pointer(std::uint32_t,std::uintptr_t&,std::string&)const;
 bool write_operand_pointer(std::uint32_t,std::uintptr_t,std::shared_ptr<void>,std::string&);
 bool constructor_word(std::uint32_t,std::uint8_t,std::uint32_t&,std::string&)const;
 bool initialized()const noexcept{return initialized_;}bool released()const noexcept{return released_;}
};
// Distinct native dynamic receiver type for EACH verified original class,
// with real native virtual dispatch; no ARM vptr and no opcode-only DTO.
template<std::int32_t Kind>class ScriptCommandReceiverV58 final : public CanonicalScriptCommandV58 {
public:
 ScriptCommandReceiverV58(const CommandC1DescriptorV58& d,ScriptCommandBehaviorV58 b):CanonicalScriptCommandV58(d,std::move(b)){}
 std::int32_t receiver_kind()const noexcept override{return Kind;}
};
bool create_script_command_receiver_v58(std::int32_t,ScriptCommandBehaviorV58,
 std::shared_ptr<CanonicalScriptCommandV58>& out,std::string&);
ScriptManagerServicesV52 bind_script_command_factories_v58(ScriptManagerServicesV52,ScriptCommandBehaviorV58);
}
