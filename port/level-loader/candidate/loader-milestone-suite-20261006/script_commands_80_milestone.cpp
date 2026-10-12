#include "script_command_receivers_v59.hpp"
#include <cassert>
#include <fstream>
#include <iterator>
#include <iostream>
#include <set>
#include <typeindex>
#include <type_traits>
#include <stdexcept>
using namespace dh2::loader;
static_assert(!std::is_copy_constructible_v<Script_ShowFlashReceiverV59>);
static_assert(!std::is_move_constructible_v<Script_StartDialogReceiverV59>);
using Bytes=std::vector<std::uint8_t>;
Bytes file(const std::string& p){std::ifstream s(p,std::ios::binary);assert(s);return Bytes(std::istreambuf_iterator<char>(s),{});}
struct Gold {Bytes b;std::size_t at{};std::uint32_t word(){assert(at+4<=b.size());auto p=&b[at];at+=4;return p[0]|std::uint32_t(p[1])<<8|std::uint32_t(p[2])<<16|std::uint32_t(p[3])<<24;}Bytes raw(std::uint32_t n){assert(at+n<=b.size());Bytes v(b.begin()+at,b.begin()+at+n);at+=n;return v;}std::string text(){auto b=raw(word());return {b.begin(),b.end()};}};
#include "../command-c1-v59/c1_gold_v59.inc"
int main(int argc,char** argv){assert(argc==2);std::string base=argv[1],error;std::set<std::type_index> types;
 unsigned constructors=0,cells=0,nontrivial_calls=0,exec_calls=0;
 for(const auto& row:c1_gold){std::shared_ptr<CanonicalScriptCommandV59> c;assert(create_script_command_receiver_v59(row.kind,{},c,error));assert(c->receiver_kind()==row.kind&&c->descriptor().original_size==row.size&&c->descriptor().Init==row.init);types.insert(typeid(*c));
  for(const auto& v:row.cells){unsigned value;assert(c->constructor_word(v.offset,v.width,value,error)&&value==v.value);++cells;}
  auto b=c->constructor_borrow();assert(b.actual_owner.get()==c.get()&&b.identity==c->identity()&&*b.skip4==0&&*b.kind8==-1&&!*b.data_c&&!*b.retained_data);CheckedCommandBorrowV59 checked;assert(!c->checked_data_borrow(checked,error));assert(!c->execute(false,0,error));assert(b.release_storage(error));assert(c->released());++constructors;
 }
 assert(constructors==46&&types.size()==46); // Preserved independently captured C1 gold.
 unsigned all_types=0,data_factories=0,base_init_types=0,required_init_types=0;std::set<std::type_index> complete_types;
 for(int kind=0;kind<80;++kind){
  std::shared_ptr<CanonicalScriptCommandV59> command;assert(create_script_command_receiver_v59(kind,{},command,error));
  assert(command&&command->receiver_kind()==kind&&command->descriptor().kind==kind);complete_types.insert(typeid(*command));
  auto storage=ScriptCommandDataV52::original_storage_factory(kind,error);assert(storage&&storage->source_kind()==kind&&!storage->complete());++data_factories;
  // The added34 Data readers have source schemas but no original common/SWAMP
  // gold sample here. Availability is not an independent decoding/effect proof.
  if(command->descriptor().Init==0x455628)++base_init_types;else ++required_init_types;
  assert(command->release_base_storage(error)&&command->released());++all_types;
 }
 assert(all_types==80&&complete_types.size()==80&&data_factories==80&&base_init_types==75&&required_init_types==5);
 std::shared_ptr<CanonicalScriptCommandV59> unchanged;assert(!create_script_command_receiver_v59(-1,{},unchanged,error)&&!unchanged);assert(!create_script_command_receiver_v59(80,{},unchanged,error)&&!unchanged);
 // Only these five nontrivial Init bodies are EXPLICIT Main service fixtures.
 // All C1s and the41 literal base455628 BX LR bodies use actual native code.
 auto main_owner=std::make_shared<int>(7);std::weak_ptr<int> weak_main=main_owner;
 ScriptCommandBehaviorV59 behavior;behavior.actual_owner=main_owner;
 behavior.init=[&](const CheckedCommandBorrowV59& c,std::string&){++nontrivial_calls;assert(c.actual_receiver.get()==c.actual_receiver->constructor_borrow().actual_owner.get());assert(c.actual_data.get()==c.actual_receiver->constructor_borrow().retained_data->get());assert(*c.data_c==c.actual_data->identity()&&c.actual_data->complete());return true;};
 // Execute fixture is never called by ScriptManager Load/Unload.
 behavior.execute=[&](const auto&,bool,int,std::string&){++exec_calls;return true;};
 ScriptManagerServicesV52 s;s.owner=std::make_shared<int>(1); // actual stream authority fixture, not App.
 s.open_file=[base](const std::string& name,ScriptFileV52& out,std::string&){auto b=std::make_shared<Bytes>(file(base+"/"+name));out={b,b->data(),b->size(),true};return true;};
 auto real=bind_script_command_factories_v59(s,behavior);std::vector<std::weak_ptr<void>> weak_commands;auto factory=real.command_factory;real.command_factory=[&](int k,auto& out,std::string& e){if(!factory(k,out,e))return false;weak_commands.push_back(out.actual_owner);return true;};
 auto m=std::make_shared<ScriptManagerOwnerV52>(real);assert(m->load_commands("scripts_pyscripts.bin",true,error));assert(m->load_names("scripts_pyscriptnames.bin",true,error));assert(m->load_commands("001_swamp_pyscripts.bin",false,error));assert(m->load_names("001_swamp_pyscriptnames.bin",false,error));assert(m->commands().size()==70&&m->names().size()==70&&m->diagnostics().initialized_commands==870&&exec_calls==0);
 Gold gold{file(base+"/data-gold-v52.bin")};assert(gold.word()==870);unsigned read_values=0,records=0;
 for(std::size_t i=0;i<70;++i){auto& script=m->commands()[i];assert(script.storage8&&script.storage8->size()==std::size_t(script.count0));const auto& context=m->contexts()[i];assert(context.command0==-1&&context.field4==0&&context.state8==2);for(const auto& record:*script.storage8){auto kind=gold.word();gold.word();gold.word();auto n=gold.word();auto data=*record.command.retained_data;assert(data&&data->source_kind()==int(kind)&&*record.command.kind8==int(kind)&&*record.command.data_c==data->identity()&&data->complete());auto string=data->strings().begin();std::size_t array_index=0;for(std::uint32_t j=0;j<n;++j){auto offset=gold.word(),width=gold.word();auto raw=gold.raw(width);if(offset!=0xffffffffu){auto s=data->scalar(offset);assert(s&&s->width==width);std::uint32_t bits=0;for(unsigned k=0;k<width;++k)bits|=std::uint32_t(raw[k])<<(8*k);assert(s->bits==bits);}else if(kind==0){assert(width==4&&data->array(20));std::uint32_t bits=0;for(unsigned k=0;k<4;++k)bits|=std::uint32_t(raw[k])<<(8*k);assert(data->array(20)->at(array_index++)==bits);}else {assert(string!=data->strings().end()&&string->second.size()==width+1&&string->second.back()==0);assert(std::equal(raw.begin(),raw.end(),string->second.begin(),[](auto a,auto b){return a==std::uint8_t(b);}));++string;}++read_values;}++records;}}
 assert(records==870&&gold.word()==70);for(int i=0;i<70;++i){auto name=gold.text();assert(std::string(m->name_from_id(i))==name&&m->id_from_name(name.c_str(),true)>=0);if(i>=15)assert(m->id_from_name(name.c_str(),false)>=15);}assert(gold.at==gold.b.size()&&!m->name_from_id(-1)&&!m->name_from_id(70)&&m->id_from_name("absent",true)==-1);
 std::map<int,std::shared_ptr<ScriptCommandDataV52>> actual_data;
 for(const auto& list:m->commands())for(const auto& record:*list.storage8)actual_data.emplace(*record.command.kind8,*record.command.retained_data);
 unsigned actual_init_stops=0,base_returns=0;
 for(const auto& row:c1_gold){std::shared_ptr<CanonicalScriptCommandV59> c;assert(create_script_command_receiver_v59(row.kind,{},c,error));auto b=c->constructor_borrow();*b.kind8=row.kind;*b.retained_data=actual_data.at(row.kind);*b.data_c=actual_data.at(row.kind)->identity();CheckedCommandBorrowV59 checked;assert(c->checked_data_borrow(checked,error)&&checked.actual_receiver.get()==c.get()&&checked.actual_data.get()==actual_data.at(row.kind).get());
  if(row.init==0x455628){assert(c->init(error));++base_returns;assert(!c->execute(false,0,error));}else{assert(!c->init(error)&&!c->initialized());assert(!c->init(error));++actual_init_stops;}
  assert(!c->release_base_storage(error));b.retained_data->reset();*b.data_c=0;assert(c->release_base_storage(error));assert(!c->release_base_storage(error));
 }
 assert(base_returns==41&&actual_init_stops==5);
 {std::shared_ptr<CanonicalScriptCommandV59> c;assert(create_script_command_receiver_v59(40,{},c,error));unsigned word;std::uintptr_t pointer;assert(!c->read_operand_word(17,1,word,error));assert(!c->read_operand_word(16,4,word,error));assert(!c->write_operand_word(17,1,9,error));assert(!c->write_operand_pointer(16,42,main_owner,error));assert(!c->read_operand_pointer(20,pointer,error));auto target=std::make_shared<int>(3);std::weak_ptr<int> weak=target;assert(c->write_operand_pointer(20,reinterpret_cast<std::uintptr_t>(target.get()),target,error));target.reset();assert(!weak.expired()&&c->read_operand_pointer(20,pointer,error)&&pointer);assert(!c->write_operand_word(21,1,4,error));assert(c->write_operand_word(20,4,0,error)&&weak.expired());assert(c->release_base_storage(error));}
 {std::shared_ptr<CanonicalScriptCommandV59> c;assert(create_script_command_receiver_v59(1,behavior,c,error));auto b=c->constructor_borrow();*b.kind8=1;*b.retained_data=actual_data.at(1);*b.data_c=actual_data.at(1)->identity();assert(c->init(error));main_owner.reset();assert(weak_main.expired());assert(!c->execute(false,0,error)&&exec_calls==0);b.retained_data->reset();*b.data_c=0;assert(c->release_base_storage(error));}
 // Admitted64-bit run must prove actual Menu+10 native pointer transport;
 // no constructor opaque word or scalar accessor may truncate this target.
 {static_assert(sizeof(std::uintptr_t)>4);std::shared_ptr<CanonicalScriptCommandV59> c;assert(create_script_command_receiver_v59(22,{},c,error));auto b=c->constructor_borrow();*b.kind8=22;*b.retained_data=actual_data.at(22);*b.data_c=actual_data.at(22)->identity();std::uintptr_t value=99;assert(c->read_operand_pointer(16,value,error)&&value==0);auto menu=std::make_shared<int>(8);const auto native=reinterpret_cast<std::uintptr_t>(menu.get());assert((native>>32)!=0);assert(c->write_operand_pointer(16,native,menu,error));assert(c->read_operand_pointer(16,value,error)&&value==native);unsigned word=77;assert(!c->read_operand_word(16,4,word,error)&&word==77);assert(!c->write_operand_word(16,4,1,error));assert(!c->write_operand_pointer(16,native+1,menu,error));CheckedCommandMenuBorrowV59 checked;assert(c->checked_menu_borrow(checked,error)&&checked.actual_receiver.get()==c.get()&&*checked.menu10==native&&checked.actual_menu_owner.get()==menu.get());checked={};menu.reset();value=77;assert(!c->read_operand_pointer(16,value,error)&&value==77);assert(!c->checked_menu_borrow(checked,error));assert(c->write_operand_pointer(16,0,{},error));assert(c->checked_menu_borrow(checked,error)&&!*checked.menu10&&!checked.actual_menu_owner);checked={};b.retained_data->reset();*b.data_c=0;assert(c->release_base_storage(error));}
 {std::shared_ptr<CanonicalScriptCommandV59> c;assert(create_script_command_receiver_v59(10,{},c,error));auto foreign=c->descriptor();bool rejected=false;try{auto wrong=std::make_shared<Script_StartDialogReceiverV59>(foreign,ScriptCommandBehaviorV59{});}catch(const std::invalid_argument&){rejected=true;}assert(rejected);}
 m->fields().field4=0x12345678;m->fields().byte30=0x77;assert(m->unload_all(error)&&m->diagnostics().released_commands==870&&m->commands().empty()&&m->names().empty()&&m->fields().current0==-1&&m->fields().common8==0&&m->fields().field4==0x12345678&&m->fields().byte30==0x77&&exec_calls==0);for(auto& weak:weak_commands)assert(weak.expired());
 // Real load stops honestly at first missing nontrivial Main Init, retains
 // exact published prefix, then performs actual base teardown without effects.
 ScriptManagerOwnerV52 stopped(bind_script_command_factories_v59(s,{}));assert(!stopped.load_commands("scripts_pyscripts.bin",true,error));assert(stopped.diagnostics().data_records>stopped.diagnostics().initialized_commands);assert(stopped.unload_all(error));
 std::cout<<"{\"validation\":\"PASS\",\"canonical_named_C1_classes\":80,\"independent_C1_gold_classes\":46,\"available_Data_types\":80,\"literal_base_Init_types_available\":75,\"original_C1_cells\":"<<cells<<",\"original_scripts\":70,\"original_data_records\":870,\"original_read_values\":"<<read_values<<",\"literal_base_Init_classes\":41,\"required_nontrivial_Init_stops\":5,\"nontrivial_Init_fixture_calls\":"<<nontrivial_calls<<",\"Execute_calls\":0,\"whole_command_execution_verified\":false,\"whole_level_ready\":false}\n";
}
