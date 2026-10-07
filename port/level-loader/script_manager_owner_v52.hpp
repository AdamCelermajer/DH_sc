#pragma once
#include <array>
#include <cstdint>
#include <cstring>
#include <functional>
#include <map>
#include <memory>
#include <optional>
#include <string>
#include <vector>
namespace dh2::loader {
class CanonicalScriptCommandV59;
struct ScriptFileV52 {std::shared_ptr<const void> owner;const std::uint8_t* bytes{};std::size_t size{};bool found{};};
class ScriptReaderV52 {
 ScriptFileV52 file_;std::size_t cursor_{};
public:
 explicit ScriptReaderV52(ScriptFileV52);
 std::uint32_t word(std::uint8_t width=4);std::uint32_t peek_word()const;
 std::vector<char> text(std::uint32_t);
 std::size_t position()const noexcept{return cursor_;}bool finished()const noexcept{return cursor_==file_.size;}
};
struct ScriptScalarV52 {std::uint8_t width{};std::uint32_t bits{};};
// width255 = source ExecScript int-array field20/count16; width0 = CString.
struct ScriptDataActionV52 {std::uint32_t offset{};std::uint8_t width{};std::uint32_t count_offset{};};
struct ScriptDataSchemaV52 {std::uint32_t original_size{},factory{},reader{};std::vector<ScriptDataActionV52> actions;std::map<std::uint32_t,ScriptScalarV52> constructor_defaults;};
// Actual owned parsed data fields, NOT an initialized/executable command.
// Main commands consume SAME data object and its strongly owned strings/array.
// No ARM pointer/vtable is embedded or interpreted as a native C++ object.
class ScriptCommandDataV52 {
 std::int32_t kind_;const ScriptDataSchemaV52* schema_{};
 std::map<std::uint32_t,ScriptScalarV52> scalar_;
 std::map<std::uint32_t,std::vector<char>> strings_;
 std::map<std::uint32_t,std::vector<std::uint32_t>> arrays_;
 bool complete_{};
public:
 explicit ScriptCommandDataV52(std::int32_t,const ScriptDataSchemaV52*);
 virtual ~ScriptCommandDataV52()=default;
 virtual bool read(ScriptReaderV52&,std::string&);
 virtual std::uintptr_t identity()const noexcept{return reinterpret_cast<std::uintptr_t>(this);}
 std::int32_t source_kind()const noexcept{return kind_;}
 virtual const ScriptScalarV52* scalar(std::uint32_t)const noexcept;
 virtual const char* cstring(std::uint32_t pointer_offset)const noexcept;
 virtual const std::vector<std::uint32_t>* array(std::uint32_t pointer_offset)const noexcept;
 const auto& scalar_fields()const noexcept{return scalar_;}const auto& strings()const noexcept{return strings_;}const auto& arrays()const noexcept{return arrays_;}
 virtual bool complete()const noexcept{return complete_;}
 static std::shared_ptr<ScriptCommandDataV52> original_storage_factory(std::int32_t,std::string&);
};
struct ScriptCommandBorrowV52 {
 std::shared_ptr<void> actual_owner;std::uintptr_t identity{};
 std::uint32_t original_factory{};
 std::uint8_t* skip4{};std::int32_t* kind8{};std::uintptr_t* data_c{};
 // Pointer into SAME actual command's retained modern lease. No side registry.
 std::shared_ptr<ScriptCommandDataV52>* retained_data{};
 std::function<bool(std::string&)> init;
 // Original Free calls BASE ScriptCmdImpl destructor then CustomFree, not
 // derived execution/Finish. Main must implement that storage release body.
 std::function<bool(std::string&)> release_storage;
 // SAME receiver's virtual Execute+8 / IsBlocking+c / Update+4.
 std::function<bool(bool,std::int32_t,std::string&)> execute;
 std::function<bool(bool&,std::string&)> blocking;
 std::function<bool(std::string&)> update;
 std::shared_ptr<CanonicalScriptCommandV59> canonical_receiver_v96;
};
struct ScriptManagerServicesV52 {
 std::shared_ptr<void> owner;
 std::function<bool(const std::string&,ScriptFileV52&,std::string&)> open_file;
 // REQUIRED real C1 class factory. Never substitute an opcode/count record.
 std::function<bool(std::int32_t,ScriptCommandBorrowV52&,std::string&)> command_factory;
 // Optional real generated-data factory extension for other map command kinds.
 // Known original-derived46 Data schemas are supplied by loader itself.
 std::function<bool(std::int32_t,std::shared_ptr<ScriptCommandDataV52>&,std::string&)> data_factory;
};
struct ScriptContextV52 {std::int32_t command0{};std::uint32_t field4{};std::int32_t state8{};};
struct ScriptCommandRecordV52 {ScriptCommandBorrowV52 command;std::shared_ptr<ScriptCommandDataV52> pending_data;bool data_released{},storage_released{},release_attempted{};};
struct ScriptCommandsV52 {std::int32_t count0{};std::uint8_t skip4{};std::optional<std::vector<ScriptCommandRecordV52>> storage8;};
struct ScriptManagerFieldsV52 {std::int32_t current0{-1},field4{},common8{};std::uint8_t byte30{};};
struct ScriptManagerDiagnosticsV52 {std::uint64_t data_records{},initialized_commands{},released_commands{},file_opens{};bool failed{};std::string error;};
struct ScriptSchedulerServicesV96 {
 std::shared_ptr<void> owner;
 std::function<bool(bool&,std::string&)> all_players_dead,online;
 std::function<bool(std::int32_t&,std::string&)> player_count714;
 std::function<bool(std::string&)> debug_load,flush_dialogs;
 std::function<bool(const char*,bool&,std::string&)> debug_switch;
 std::function<bool(const char*,const char*,std::int32_t&,std::string&)> constant;
 std::function<bool(bool,std::int32_t,std::int32_t,std::string&)> send_script_message;
 std::function<bool(const char*,std::string&)> publish_current_name;
};
// ONE original-derived ScriptManager owner; Main installs it in its existing
// application/global authority, never as Level.script44 or another Lua VM.
class ScriptManagerOwnerV52 final {
 ScriptManagerFieldsV52 fields_;std::vector<ScriptContextV52> contexts_;
 std::vector<ScriptCommandsV52> commands_;std::vector<std::unique_ptr<char[]>> names_;
 ScriptManagerServicesV52 services_;ScriptManagerDiagnosticsV52 diagnostics_;bool busy_{};
 ScriptSchedulerServicesV96 scheduler_;std::uint32_t execution_depth_v96_{};
 bool fail(const std::string&,std::string&);bool reached_callback(std::string&);
 bool common_skip(bool)const noexcept;
public:
 explicit ScriptManagerOwnerV52(ScriptManagerServicesV52 s):services_(std::move(s)){}
 ScriptManagerOwnerV52(const ScriptManagerOwnerV52&)=delete;
 std::uintptr_t identity()const noexcept{return reinterpret_cast<std::uintptr_t>(this);}
 bool load_commands(const char*,bool common,std::string&);
 bool load_names(const char*,bool common,std::string&);
 // Original455ad8 repeats virtual Init on the SAME loaded command records.
 bool init_commands_v95(std::string&);
 bool unload_all(std::string&);
 //Whole45a2ac: reset0/30/4/8, actual UnLoadAll, then controller blocked=0.
 bool flush_source_v88(std::function<bool(std::string&)> actual_controller_reset,std::string&);
 const auto& diagnostics()const noexcept{return diagnostics_;}
 auto& fields()noexcept{return fields_;}const auto& fields()const noexcept{return fields_;}
 const auto& contexts()const noexcept{return contexts_;}
 ScriptContextV52* context_at(std::int32_t id)noexcept{return id>=0&&std::size_t(id)<contexts_.size()?&contexts_[id]:nullptr;}
 const auto& commands()const noexcept{return commands_;}
 const auto& names()const noexcept{return names_;}
 const char* name_from_id(std::int32_t)const noexcept;
 std::int32_t id_from_name(const char*,bool include_common)const noexcept;
 bool bind_scheduler_v96(ScriptSchedulerServicesV96,std::string&);
 bool start_script_v96(std::int32_t id,std::int32_t module,bool received_network,std::string&);
 bool stop_script_v96(std::int32_t id,bool ignored,std::string&);
 bool is_script_running_v96(std::int32_t id,bool&,std::string&)const;
 bool skip_script_v96(std::int32_t id,bool received_network,std::string&);
 void stop_skipping_v96()noexcept{fields_.current0=-1;}
 bool execute_script_v96(std::int32_t id,bool& running,std::string&);
 bool execute_all_scripts_v96(bool& any_running,std::string&);
 // Destructor releases native leases only; source teardown is unload_all.
};
}
