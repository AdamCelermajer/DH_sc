#include "script_manager_owner_v52.hpp"
#include <stdexcept>
#include <limits>
namespace dh2::loader {
#include "script_data_schemas_v52.inc"
#include "script_command_factories_v52.inc"
namespace {
std::int32_t signed_word(std::uint32_t n){std::int32_t v;std::memcpy(&v,&n,4);return v;}
bool valid_command(const ScriptCommandBorrowV52& c,std::int32_t kind){return c.actual_owner&&c.identity&&c.skip4&&c.kind8&&c.data_c&&c.retained_data&&c.original_factory==script_command_factories_v52[std::size_t(kind)];}
constexpr std::uint32_t max_scripts=8192,max_commands=65536;
}
ScriptReaderV52::ScriptReaderV52(ScriptFileV52 f):file_(std::move(f)){if(!file_.found||!file_.owner||(!file_.bytes&&file_.size)||file_.size>32u*1024u*1024u)throw std::runtime_error("Required actual bounded ScriptManager stream buffer");}
std::uint32_t ScriptReaderV52::word(std::uint8_t width){if((width!=1&&width!=2&&width!=4)||file_.size-cursor_<width)throw std::runtime_error("Truncated ScriptManager scalar");std::uint32_t out=0;for(unsigned i=0;i<width;++i)out|=std::uint32_t(file_.bytes[cursor_++])<<(8*i);return out;}
std::uint32_t ScriptReaderV52::peek_word()const {if(file_.size-cursor_<4)throw std::runtime_error("Truncated ScriptManager command kind");auto p=file_.bytes+cursor_;return p[0]|std::uint32_t(p[1])<<8|std::uint32_t(p[2])<<16|std::uint32_t(p[3])<<24;}
std::vector<char> ScriptReaderV52::text(std::uint32_t n){if(n>1024u*1024u||n>file_.size-cursor_)throw std::runtime_error("Truncated/oversize ScriptManager CString");std::vector<char> result(std::size_t(n)+1);if(n)std::memcpy(result.data(),file_.bytes+cursor_,n);cursor_+=n;result[n]=0;return result;}
ScriptCommandDataV52::ScriptCommandDataV52(std::int32_t k,const ScriptDataSchemaV52* s):kind_(k),schema_(s){if(s)scalar_=s->constructor_defaults;}
const ScriptScalarV52* ScriptCommandDataV52::scalar(std::uint32_t offset)const noexcept {auto p=scalar_.find(offset);return p==scalar_.end()?nullptr:&p->second;}
const char* ScriptCommandDataV52::cstring(std::uint32_t offset)const noexcept {auto p=strings_.find(offset);return p==strings_.end()?nullptr:p->second.data();}
const std::vector<std::uint32_t>* ScriptCommandDataV52::array(std::uint32_t offset)const noexcept {auto p=arrays_.find(offset);return p==arrays_.end()?nullptr:&p->second;}
std::shared_ptr<ScriptCommandDataV52> ScriptCommandDataV52::original_storage_factory(std::int32_t k,std::string& e){auto p=original_script_data_schemas_v52().find(k);if(p==original_script_data_schemas_v52().end()){e="Required original generated-data C1/read provider for command kind "+std::to_string(k);return {};}return std::make_shared<ScriptCommandDataV52>(k,&p->second);}
bool ScriptCommandDataV52::read(ScriptReaderV52& reader,std::string& error){if(complete_){error="Data reader already completed; refusing replay";return false;}if(!schema_){error="Required original generated-data reader";return false;}
 try{for(const auto& a:schema_->actions){if(a.width!=0&&a.width!=255){scalar_[a.offset]={a.width,reader.word(a.width)};}else{auto n=scalar(a.count_offset);if(!n||n->width!=4)throw std::runtime_error("Required produced actual data length field");if(a.width==0)strings_[a.offset]=reader.text(n->bits);else {if(n->bits>65536)throw std::runtime_error("ExecScript int array outside native bound");auto& values=arrays_[a.offset];values.resize(n->bits);for(auto& v:values)v=reader.word();}}}
  auto k=scalar(4);if(!k||k->bits!=std::uint32_t(kind_))throw std::runtime_error("Actual generated-data kind does not match peeked command");complete_=true;error.clear();return true;
 }catch(const std::exception& e){error=e.what();return false;}
}
bool ScriptManagerOwnerV52::fail(const std::string& message,std::string& error){diagnostics_.failed=true;diagnostics_.error=message.empty()?"ScriptManager required source body failed":message;error=diagnostics_.error;return false;}
bool ScriptManagerOwnerV52::reached_callback(std::string& e){if(diagnostics_.failed){e=diagnostics_.error;return false;}return true;}
bool ScriptManagerOwnerV52::common_skip(bool common)const noexcept{return common&&fields_.common8<std::int64_t(commands_.size());}
const char* ScriptManagerOwnerV52::name_from_id(std::int32_t id)const noexcept{return id>=0&&std::size_t(id)<names_.size()?names_[id].get():nullptr;}
std::int32_t ScriptManagerOwnerV52::id_from_name(const char* name,bool include_common)const noexcept {if(!name)return -1;auto first=include_common?0:fields_.common8;if(first<0)return -1;for(std::size_t i=std::size_t(first);i<names_.size();++i)if(names_[i]&&std::strcmp(names_[i].get(),name)==0)return std::int32_t(i);return -1;}
bool ScriptManagerOwnerV52::load_commands(const char* name,bool common,std::string& error){
 if(busy_||execution_depth_v96_)return fail("ScriptManager reentered; original prefix retained",error);if(diagnostics_.failed){error=diagnostics_.error;return false;}if(common_skip(common)){error.clear();return true;}
 struct Busy{bool& b;explicit Busy(bool& v):b(v){b=true;}~Busy(){b=false;}} guard(busy_);
 try{if(!services_.owner||!services_.open_file||!name)return fail("Required actual ScriptManager cache stream owner",error);ScriptFileV52 file;++diagnostics_.file_opens;if(!services_.open_file(name,file,error))return fail(error,error);if(!reached_callback(error))return false;if(!file.found){error.clear();return true;}
  ScriptReaderV52 reader(std::move(file));const auto count=reader.word();const auto before=commands_.size();if(count>max_scripts||before+count>max_scripts)return fail("Script count outside native storage domain",error);
  // Exact45b374 common8 addition BEFORE command/context vector resize.
  if(common)fields_.common8=signed_word(std::uint32_t(fields_.common8)+count);commands_.resize(before+count);contexts_.resize(before+count);
  std::uint64_t total=0;for(const auto& s:commands_)total+=std::uint32_t(s.count0);
  for(std::size_t i=before;i<commands_.size();++i){auto& script=commands_[i];auto n=reader.word();if(n>max_commands||total+n>max_commands)return fail("Command list outside native storage domain",error);total+=n;script.count0=std::int32_t(n);script.storage8.emplace(n);
   for(std::uint32_t j=0;j<n;++j){auto kind=signed_word(reader.peek_word());if(kind<0||kind>=80)return fail("Command kind outside original80 factory domain",error);auto& record=(*script.storage8)[j];
    if(!services_.command_factory)return fail("Required real original command C1 factory for kind "+std::to_string(kind),error);
    // Publish command slot immediately after its C1, BEFORE Data factory/read.
    if(!services_.command_factory(kind,record.command,error))return fail(error,error);if(!reached_callback(error))return false;
    if(!valid_command(record.command,kind))return fail("Foreign/incomplete actual source command factory borrow",error);
    if(*record.command.data_c||*record.command.retained_data)return fail("Source command C1 data_c must be NULL before Assign",error);
    if(services_.data_factory){if(!services_.data_factory(kind,record.pending_data,error))return fail(error,error);if(!reached_callback(error))return false;}
    else record.pending_data=ScriptCommandDataV52::original_storage_factory(kind,error);
    if(!record.pending_data||record.pending_data->source_kind()!=kind)return fail(error.empty()?"Required SAME actual generated-data owner":error,error);
    if(!record.pending_data->read(reader,error))return fail(error,error);if(!reached_callback(error))return false;if(!record.pending_data->complete())return fail("Actual generated-data Read body did not complete",error);++diagnostics_.data_records;
    auto data=record.pending_data;*record.command.kind8=kind;*record.command.retained_data=data;*record.command.data_c=data->identity();record.pending_data.reset();
    if(!record.command.init)return fail("Required real command Init for kind "+std::to_string(kind),error);
    if(!record.command.init(error))return fail(error,error);if(!reached_callback(error))return false;
    if(*record.command.data_c!=data->identity()||record.command.retained_data->get()!=data.get()||*record.command.kind8!=kind)return fail("Command Init changed actual data/kind binding",error);
    ++diagnostics_.initialized_commands;if(*record.command.skip4)script.skip4=1;
   }
   // Whole455bb8 StopScript(index,false) ignores false and preserves context4.
   contexts_[i].state8=2;contexts_[i].command0=-1;
  }
  if(!reader.finished())return fail("Unexpected compiled script stream tail",error);error.clear();return true;
 }catch(const std::exception& e){return fail(e.what(),error);}catch(...){return fail("ScriptManager source service threw",error);}
}
bool ScriptManagerOwnerV52::init_commands_v95(std::string& error){
 if(busy_||execution_depth_v96_)return fail("ScriptManager InitCommands reentered; native prefix retained",error);
 if(diagnostics_.failed){error=diagnostics_.error;return false;}
 struct Busy{bool& b;explicit Busy(bool& value):b(value){b=true;}~Busy(){b=false;}} guard(busy_);
 try{
  for(std::size_t script_index=0;script_index<commands_.size();++script_index){
   for(std::int32_t command_index=0;command_index<commands_[script_index].count0;++command_index){
    auto& script=commands_[script_index];
    if(!script.storage8||std::size_t(command_index)>=script.storage8->size())return fail("Required actual ScriptCmds count0/storage8 in InitCommands",error);
    auto& command=(*script.storage8)[std::size_t(command_index)].command;
    auto receiver=command.actual_owner;auto init=command.init;
    if(!receiver||!init)return fail("Required SAME command virtual0 for InitCommands",error);
    if(!init(error))return fail(error,error);
    if(!reached_callback(error))return false;
    ++diagnostics_.initialized_commands;
   }
  }
  // Counts/storage are reread after each callback. This original pass changes
  // no current/common context or aggregate skip and never reassigns Data/C1.
  error.clear();return true;
 }catch(const std::exception& e){return fail(e.what(),error);}catch(...){return fail("ScriptManager InitCommands threw; prefix retained",error);}
}
bool ScriptManagerOwnerV52::load_names(const char* name,bool common,std::string& error){
 if(busy_||execution_depth_v96_)return fail("ScriptManager names reentered; prefix retained",error);if(diagnostics_.failed){error=diagnostics_.error;return false;}if(common_skip(common)){error.clear();return true;}
 struct Busy{bool& b;explicit Busy(bool& v):b(v){b=true;}~Busy(){b=false;}} guard(busy_);
 try{if(!services_.owner||!services_.open_file||!name)return fail("Required actual ScriptManager name stream owner",error);ScriptFileV52 file;++diagnostics_.file_opens;if(!services_.open_file(name,file,error))return fail(error,error);if(!reached_callback(error))return false;if(!file.found){error.clear();return true;}
  ScriptReaderV52 reader(std::move(file));auto count=reader.word();const auto before=names_.size();if(count>max_scripts||before+count>max_scripts)return fail("Script names outside native bound",error);names_.resize(before+count);
  for(std::size_t i=before;i<names_.size();++i){const auto n=reader.word();auto value=reader.text(n);auto owned=std::make_unique<char[]>(value.size());std::memcpy(owned.get(),value.data(),value.size());names_[i]=std::move(owned);}
  if(!reader.finished())return fail("Unexpected script names stream tail",error);error.clear();return true;
 }catch(const std::exception& e){return fail(e.what(),error);}catch(...){return fail("ScriptManager name stream threw",error);}
}
bool ScriptManagerOwnerV52::flush_source_v88(std::function<bool(std::string&)> reset,std::string& e){
 if(busy_||execution_depth_v96_||!reset){e="Required idle SAME ScriptManager and actual controller process reset for Flush";return false;}
 fields_.current0=-1;fields_.byte30=0;fields_.field4=0;fields_.common8=0;
 if(!unload_all(e))return false;
 return reset(e);
}
bool ScriptManagerOwnerV52::unload_all(std::string& error){
 if(busy_||execution_depth_v96_)return fail("ScriptManager Unload reentered; prefix retained",error);
 // Unload is the required native recovery/teardown after a failed Load prefix.
 // It never resumes/replays failed C1/Read/Init and never calls Execute/Finish.
 struct Busy{bool& b;explicit Busy(bool& v):b(v){b=true;}~Busy(){b=false;}} guard(busy_);
 diagnostics_.failed=false;diagnostics_.error.clear();
 try{for(auto& script:commands_){if(script.storage8){for(auto& record:*script.storage8){auto& command=record.command;if(!command.actual_owner||record.storage_released)continue;
     if(!record.data_released){if(command.retained_data)command.retained_data->reset();if(command.data_c)*command.data_c=0;record.pending_data.reset();record.data_released=true;}
     if(!command.release_storage)return fail("Required actual ScriptCmdImpl base storage release/CustomFree",error);
     if(record.release_attempted)return fail("Actual command release previously failed; refusing body replay",error);record.release_attempted=true;
     if(!command.release_storage(error))return fail(error,error);if(!reached_callback(error))return false;record.storage_released=true;++diagnostics_.released_commands;command={};
    }script.storage8.reset();}script.count0=0; // SourceFree leaves skip4 intact.
   }
   // Source45a224 names free precedes vector resets, then StopSkipping/current0
   // and common8=0. Source field4 and byte30 are NOT Flush-reset here.
   names_.clear();commands_.clear();contexts_.clear();fields_.current0=-1;fields_.common8=0;error.clear();return true;
 }catch(const std::exception& e){return fail(e.what(),error);}catch(...){return fail("ScriptManager actual storage teardown threw",error);}
}
}
