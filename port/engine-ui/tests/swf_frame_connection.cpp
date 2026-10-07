#include "swf_frame_connection.hpp"
#include "swf_input_connection.hpp"
#include "gameswf/gameswf_sprite.h"
#include "gameswf/gameswf_movie_def.h"
#include "gameswf/gameswf_stream.h"
#include "gameswf/gameswf_impl.h"
#include "gameswf/gameswf_button.h"
#include "base/tu_file.h"
#include <iostream>
#include <stdexcept>
#include <vector>
#include <memory>
using namespace dh2::ui;
namespace {
unsigned checks;void require(bool value,const std::string&e="Source frame assertion"){++checks;if(!value)throw std::runtime_error(e+" ["+std::to_string(checks)+"]");}
std::vector<int>order;gameswf::sprite_instance*root_clip;gameswf::action_buffer*requeue_action;unsigned requeues;
SwfFrameConnection*release_on_enter;
unsigned warnings;void log(bool,const char*message){if(std::string(message)=="source sprite goto action iteration limit\n")++warnings;else throw std::runtime_error(message);}
struct Shape:gameswf::character_def{gameswf::character*created{};explicit Shape(gameswf::player*p):character_def(p){}gameswf::character*create_character_instance(gameswf::character*p,int id)override{created=character_def::create_character_instance(p,id);return created;}};
void init(const gameswf::fn_call&){order.push_back(1);}
void parent0(const gameswf::fn_call&){order.push_back(2);}
void parent1(const gameswf::fn_call&){order.push_back(3);}
void child0(const gameswf::fn_call&){order.push_back(4);}
void load(const gameswf::fn_call&f){order.push_back(f.this_ptr==root_clip?5:6);}
void enter(const gameswf::fn_call&){order.push_back(7);if(release_on_enter){release_on_enter->release();release_on_enter=nullptr;}}
void requeue(const gameswf::fn_call&){order.push_back(8);if(++requeues<14)root_clip->m_goto_frame_action_list.push_back(requeue_action);}
struct Tag:gameswf::execute_tag {std::unique_ptr<gameswf::action_buffer>action;explicit Tag(const char*name){std::vector<unsigned char>payload{7,0,0,0,0,0};while(*name)payload.push_back(*name++);payload.push_back(0);std::vector<unsigned char>bytes{0x96,static_cast<unsigned char>(payload.size()),0};bytes.insert(bytes.end(),payload.begin(),payload.end());bytes.insert(bytes.end(),{0x3d,0x17,0});tu_file file(tu_file::memory_buffer);file.write_bytes(bytes.data(),bytes.size());file.set_position(0);gameswf::stream stream(&file);action=std::make_unique<gameswf::action_buffer>();action->read(&stream);}void execute(gameswf::character*c)override{c->add_action_buffer(action.get());}std::uint32_t get_depth_id_of_replace_or_add_tag()const override{return 1u<<16;}};
struct Graph {
 gameswf::gc_ptr<gameswf::player>player;gameswf::gc_ptr<gameswf::movie_def_impl>def;gameswf::gc_ptr<gameswf::root>root;gameswf::gc_ptr<gameswf::sprite_definition>child_def;std::shared_ptr<SwfInputHistory>history=std::make_shared<SwfInputHistory>();SwfFrameConnection frames;
 Graph(){std::string e;player=new gameswf::player;require(history->bind(player.get_ptr(),e),e);require(frames.bind(player.get_ptr(),history,e),e);
  for(auto pair:{std::pair<const char*,gameswf::as_c_function_ptr>("init",init),{"parent0",parent0},{"parent1",parent1},{"child0",child0},{"requeue",requeue}})player->get_global()->set_member(pair.first,gameswf::as_value(pair.second));
  def=new gameswf::movie_def_impl(player.get_ptr(),gameswf::DO_NOT_LOAD_BITMAPS,gameswf::DO_NOT_LOAD_FONT_SHAPES);def->m_version=8;def->m_frame_rate=10;def->m_frame_size.m_x_max=9600;def->m_frame_size.m_y_max=6400;def->set_frame_count(2);def->m_playlist.resize(2);def->m_init_action_list.resize(2);def->m_playlist[0].push_back(new Tag("parent0"));def->m_playlist[1].push_back(new Tag("parent1"));def->m_init_action_list[0].push_back(new Tag("init"));def->inc_loading_frame();def->inc_loading_frame();root=def->create_instance();root_clip=static_cast<gameswf::sprite_instance*>(root->get_root_movie());root_clip->set_member("onLoad",gameswf::as_value(load));
  child_def=new gameswf::sprite_definition(player.get_ptr(),def.get_ptr());child_def->set_frame_count(1);child_def->m_playlist.resize(1);child_def->m_playlist[0].push_back(new Tag("child0"));child_def->inc_loading_frame();gameswf::gc_ptr<gameswf::sprite_instance>child=new gameswf::sprite_instance(player.get_ptr(),child_def.get_ptr(),root.get_ptr(),root_clip,1);child->set_name("btn_child");child->set_member("onLoad",gameswf::as_value(load));gameswf::matrix m;gameswf::cxform cx;root_clip->m_display_list.add_display_object(child.get_ptr(),1,false,cx,m,0,0,0);
 }
 ~Graph(){root_clip->m_display_list.clear();for(auto i=player->m_heap.begin();i!=player->m_heap.end();++i){auto*c=i->first.get_ptr();c->m_members.clear();c->m_proto=nullptr;if(auto*env=c->get_environment())env->m_target=nullptr;}player->get_global()->m_members.clear();player->get_global()->m_proto=nullptr;player->clear_heap();def->m_instance=nullptr;player->m_current_root=nullptr;root=nullptr;child_def=nullptr;def=nullptr;frames.release();history->release();player=nullptr;root_clip=nullptr;}
};
}
int main(){try{gameswf::register_log_callback(log);Graph graph;std::string e;order.clear();require(graph.frames.advance(graph.root.get_ptr(),0,false,e),e);require(order==std::vector<int>({1,5,2,6,4,5}),"Deferred init/load/action/child order");float gc=0;require(graph.frames.gc_remaining(graph.root.get_ptr(),gc,e)&&gc==2.f);SwfInputHistoryFlags flags{};auto*child=static_cast<gameswf::sprite_instance*>(root_clip->m_display_list.get_character(0));require(graph.history->read(child,flags,e)&&flags.need9d&&child->m_on_event_load_called);
 root_clip->set_member("onEnterFrame",gameswf::as_value(enter));order.clear();require(graph.frames.advance(graph.root.get_ptr(),.1f,false,e),e);require(order==std::vector<int>({7,3}),"Parent actions run before static children gate");
 root_clip->set_play_state(gameswf::character::STOP);auto action=std::make_unique<Tag>("requeue");requeue_action=action->action.get();root_clip->m_goto_frame_action_list.push_back(requeue_action);requeues=0;order.clear();require(graph.frames.advance(graph.root.get_ptr(),.1f,false,e),e);require(requeues==12&&warnings==1&&root_clip->m_goto_frame_action_list.size()==1,"Reentrant goto batch survives iteration limit");root_clip->m_goto_frame_action_list.clear();
 root_clip->goto_frame(0);require(root_clip->m_current_frame==0&&root_clip->m_play_state==gameswf::character::STOP&&root_clip->m_goto_frame_action_list.size()==1);order.clear();require(graph.frames.advance(graph.root.get_ptr(),.1f,false,e),e);require(order==std::vector<int>({2,7}),"Source goto target executes once before EnterFrame");
 const auto before=root_clip->m_goto_frame_action_list.size();root_clip->goto_frame(0);require(root_clip->m_goto_frame_action_list.size()==before);root_clip->goto_frame(-1);require(root_clip->m_play_state==gameswf::character::STOP&&root_clip->m_current_frame==0);
 gameswf::matrix m;m.m_[0][2]=100;m.m_[1][2]=200;child->set_matrix(m);gameswf::character::drag_state drag;drag.SetCharacter(child);graph.root->notify_mouse_state(20,30,0);graph.root->set_drag_state(drag);require(graph.frames.advance(graph.root.get_ptr(),.1f,false,e),e);require(child->get_matrix().m_[0][2]==100&&child->get_matrix().m_[1][2]==200,"Source delayed drag offset initialization");graph.root->notify_mouse_state(22,35,0);require(graph.frames.advance(graph.root.get_ptr(),.1f,false,e),e);require(child->get_matrix().m_[0][2]==140&&child->get_matrix().m_[1][2]==300);graph.root->stop_drag();require(graph.root->m_drag_state.OffsetX()==300&&graph.root->m_drag_state.OffsetY()==400,"Source stop clears target only");
 gameswf::gc_ptr<Shape>shape=new Shape(graph.player.get_ptr());gameswf::gc_ptr<gameswf::button_character_definition>button_def=new gameswf::button_character_definition(graph.player.get_ptr());gameswf::button_record record{};record.m_character_def=shape.get_ptr();record.m_character_id=1;record.m_up=true;button_def->m_button_records.push_back(record);gameswf::gc_ptr<gameswf::character>button=button_def->create_character_instance(root_clip,90);SwfInputHistoryFlags base_flags{};require(graph.history->read(shape->created,base_flags,e)&&base_flags.need9d);swf_frame_character_advance(button.get_ptr(),.1f);require(graph.history->read(shape->created,base_flags,e)&&!base_flags.need9d,"Active DefineButton record runs source base clear-need endpoint");graph.history->write_need(shape->created,true,e);button_def->m_button_records[0].m_up=false;graph.player->set_as_garbage();swf_frame_character_advance(button.get_ptr(),.1f);require(!graph.player->is_garbage(shape->created)&&graph.history->read(shape->created,base_flags,e)&&base_flags.need9d,"Inactive DefineButton record marks alive without advancing");
 require(graph.frames.advance(graph.root.get_ptr(),2.1f,false,e),e);require(!graph.player->is_garbage(child)&&root_clip->m_display_list.get_character(0)==child,"Periodic GC retains source display subtree");
 release_on_enter=&graph.frames;require(graph.frames.advance(graph.root.get_ptr(),2.1f,false,e),e);require(!release_on_enter,"Reentry releases handle without destroying in-flight receiver");require(!graph.frames.advance(graph.root.get_ptr(),.1f,false,e));
 std::cout<<"{\"validation\":\"PASS\",\"actual_core_scheduler_checks\":"<<checks<<",\"genuine_AS_action_order\":true,\"source_reentrant_goto_limit\":12,\"source_2d_drag\":true,\"mismatches\":0}\n";
 }catch(const std::exception&e){std::cerr<<e.what()<<"\n";return 1;}}

