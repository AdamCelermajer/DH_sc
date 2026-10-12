#include "script_command_receivers_v59.hpp"
#include <cassert>
#include <algorithm>
#include <iostream>
using namespace dh2::loader;
using Bytes=std::vector<std::uint8_t>;
int main(){std::string error;std::shared_ptr<CanonicalScriptCommandV59> command;assert(create_script_command_receiver_v59(4,{},command,error));assert(std::dynamic_pointer_cast<Script_SetCameraReceiverV59>(command));assert(command->descriptor().original_size==16&&command->descriptor().factory==0x458c20&&command->descriptor().Init==0x455628);
 auto borrow=command->constructor_borrow();assert(*borrow.skip4==0&&*borrow.kind8==-1&&!*borrow.data_c&&!*borrow.retained_data);unsigned word=7;assert(!command->read_operand_word(16,4,word,error)&&word==7);
 auto data=ScriptCommandDataV52::original_storage_factory(4,error);assert(data&&!data->complete()&&!data->scalar(4)&&!data->scalar(8)&&!data->scalar(16)&&!data->scalar(24)&&!data->scalar(28)&&!data->cstring(12)&&!data->cstring(20));
 // Explicit synthetic field bytes; original reader has NOT executed here.
 const std::string first("left\0tail",9),second="right";auto bytes=std::make_shared<Bytes>();auto add=[&](unsigned value){for(unsigned i=0;i<4;++i)bytes->push_back(std::uint8_t(value>>(8*i)));};add(4);add(first.size());for(auto c:first)bytes->push_back(std::uint8_t(c));add(second.size());for(auto c:second)bytes->push_back(std::uint8_t(c));add(0xdeadbeef);bytes->push_back(0x7f);
 ScriptReaderV52 reader({bytes,bytes->data(),bytes->size(),true});assert(data->read(reader,error)&&reader.finished()&&data->complete());assert(data->scalar(4)->bits==4&&data->scalar(8)->bits==first.size()&&data->scalar(16)->bits==second.size()&&data->scalar(24)->bits==0xdeadbeef&&data->scalar(28)->width==1&&data->scalar(28)->bits==0x7f);assert(data->strings().at(12).size()==first.size()+1&&std::equal(first.begin(),first.end(),data->strings().at(12).begin())&&data->strings().at(12).back()==0);assert(std::string(data->cstring(20))==second&&!data->array(20));
 *borrow.kind8=4;*borrow.retained_data=data;*borrow.data_c=data->identity();CheckedCommandBorrowV59 same;assert(command->checked_data_borrow(same,error)&&same.actual_data.get()==data.get());assert(borrow.init(error)&&command->initialized());assert(!command->execute(false,0,error)); // Main dispatch remains REQUIRED.
 borrow.retained_data->reset();*borrow.data_c=0;assert(borrow.release_storage(error));same={};borrow={};command.reset();
 // A tiny synthetic whole-manager command list exercises correct factory and
 // schema wiring; cache is an explicit transport fixture, not a real file.
 auto list=std::make_shared<Bytes>();auto header=[&](unsigned value){for(unsigned i=0;i<4;++i)list->push_back(std::uint8_t(value>>(8*i)));};header(1);header(1);list->insert(list->end(),bytes->begin(),bytes->end());ScriptManagerServicesV52 s;s.owner=std::make_shared<int>(1);s.open_file=[list](const std::string&,ScriptFileV52& out,std::string&){out={list,list->data(),list->size(),true};return true;};ScriptManagerOwnerV52 manager(bind_script_command_factories_v59(s,{}));assert(manager.load_commands("synthetic-kind4",false,error)&&manager.commands().size()==1&&manager.diagnostics().data_records==1&&manager.diagnostics().initialized_commands==1&&*manager.commands()[0].storage8->at(0).command.kind8==4);assert(manager.unload_all(error)&&manager.diagnostics().released_commands==1);
 std::cout<<"{\"validation\":\"PASS\",\"kind4_static_source_extension\":true,\"synthetic_record_test\":true,\"original_reader_executed\":false,\"actual_Main_Execute_verified\":false,\"map_reach_verified\":false,\"whole_level_ready\":false}\n";
}
