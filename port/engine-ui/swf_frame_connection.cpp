#include "swf_frame_connection.hpp"
#include "swf_frame_schedule.hpp"
#include "swf_drag_values.hpp"
#include "gameswf/gameswf_sprite.h"
#include "gameswf/gameswf_movie_def.h"
#include "gameswf/gameswf_impl.h"
#include "gameswf/gameswf_button.h"
#include "gameswf/gameswf_text.h"
#include "base/tu_random.h"
#include <map>
#include <mutex>
#include <stdexcept>
#include <vector>
namespace gameswf {void execute_actions(as_environment*,const array<action_buffer*>&);}
namespace dh2::ui {
struct SwfFrameConnection::State {
 ~State();
 struct Sprite {weak_ptr<gameswf::sprite_instance> identity;bool constructed{},has_init{};array<gameswf::action_buffer*> init_pending;};
 struct Root {weak_ptr<gameswf::root> identity;SwfRootFrame32 fields{};weak_ptr<gameswf::character>drag_identity;bool drag_initialized{};};
 gameswf::player*player{};std::shared_ptr<SwfInputHistory>history;
 std::map<gameswf::sprite_instance*,Sprite>sprites;std::map<gameswf::root*,Root>roots;
 Sprite&sprite(gameswf::sprite_instance*c){auto i=sprites.find(c);if(i==sprites.end()||i->second.identity.get_ptr()!=c)throw std::runtime_error("Required source sprite constructor observation unavailable");return i->second;}
 SwfInputHistoryFlags flags(gameswf::character*c){SwfInputHistoryFlags f{};std::string e;if(!history->read(c,f,e))throw std::runtime_error(e);return f;}
 void need(gameswf::character*c,bool v){std::string e;if(!history->write_need(c,v,e))throw std::runtime_error(e);}
 void notify(gameswf::character*c){std::string e;if(!history->notify(c,e))throw std::runtime_error(e);}
};
namespace {
SwfFrameConnection::State::Root&root_entry(SwfFrameConnection::State&s,gameswf::root*r){auto&i=s.roots[r];if(i.identity.get_ptr()!=r){i=SwfFrameConnection::State::Root{};i.identity=r;}return i;}
void matrix_words(const gameswf::matrix&m,float*out){for(unsigned y=0;y<2;++y)for(unsigned x=0;x<3;++x)out[y*3+x]=m.m_[y][x];}
std::mutex mutex;std::map<gameswf::player*,std::weak_ptr<SwfFrameConnection::State>> receivers;
std::shared_ptr<SwfFrameConnection::State> receiver(gameswf::player*p){std::lock_guard<std::mutex>lock(mutex);auto i=receivers.find(p);auto s=i==receivers.end()?nullptr:i->second.lock();if(!s)throw std::runtime_error("Required source frame receiver unavailable");return s;}
void children_construct(gameswf::sprite_instance*c){
 // Original player stack pushes in reverse, processes forward, and drops each
 // strong pin immediately after its callback. Snapshot identities survive edits.
 std::vector<std::unique_ptr<gameswf::gc_ptr<gameswf::character>>> pins;
 const int count=c->m_display_list.size();pins.reserve(count);
 for(int i=count-1;i>=0;--i)pins.push_back(std::make_unique<gameswf::gc_ptr<gameswf::character>>(c->m_display_list.get_character(i)));
 while(!pins.empty()){if(auto*child=pins.back()->get_ptr())swf_frame_construct(child);pins.pop_back();}
}
bool children_advance(gameswf::sprite_instance*c,float dt,SwfFrameConnection::State&s){
 std::vector<std::unique_ptr<gameswf::gc_ptr<gameswf::character>>> pins;
 const int count=c->m_display_list.size();pins.reserve(count);
 for(int i=count-1;i>=0;--i)pins.push_back(std::make_unique<gameswf::gc_ptr<gameswf::character>>(c->m_display_list.get_character(i)));
 bool needs=false;
 while(!pins.empty()){auto*child=pins.back()->get_ptr();if(child&&s.flags(child).need9d){swf_frame_character_advance(child,dt);needs|=s.flags(child).need9d!=0;}pins.pop_back();}
 return needs;
}
std::int32_t frame16(std::int32_t x){auto b=std::uint16_t(x);return b<32768?b:std::int32_t(b)-65536;}
void write_root(gameswf::root*r,const SwfRootFrame32&s){r->m_time_remainder=s.remainder;r->m_frame_time=s.frame_time;r->m_on_event_load_called=s.loaded!=0;}
void read_root(gameswf::root*r,SwfRootFrame32&s){s.remainder=r->m_time_remainder;s.frame_time=r->m_frame_time;s.loaded=r->m_on_event_load_called;s.movie=reinterpret_cast<std::uintptr_t>(r->m_movie.get_ptr());s.player=reinterpret_cast<std::uintptr_t>(r->m_player.get_ptr());}
struct RootCall {gameswf::root*root;SwfFrameConnection::State*owner;std::string error;};
int root_service(void*p,SwfRootFrame32*s,const SwfFrameRequest24*q){auto&call=*static_cast<RootCall*>(p);auto*r=call.root;write_root(r,*s);
 try {
  auto*movie=reinterpret_cast<gameswf::character*>(q->receiver);auto*player=reinterpret_cast<gameswf::player*>(q->receiver);
  switch(q->operation){
  case SwfFrameOp::engine_mutex:(void)gameswf::gameswf_engine_mutex();break;
  case SwfFrameOp::listeners_advance:r->m_listener.advance(q->delta);break;
  case SwfFrameOp::random:(void)tu_random::next_random();break;
  case SwfFrameOp::flash_vars:if(!player)throw std::runtime_error("Required live source player for flash variables unavailable");r->set_flash_vars(player->m_flash_vars);break;
  case SwfFrameOp::init_actions:if(!movie||!movie->is(gameswf::sprite_instance::m_class_id))throw std::runtime_error("Required source root sprite init-actions receiver unavailable");swf_frame_init_actions(static_cast<gameswf::sprite_instance*>(movie));break;
  case SwfFrameOp::construct:if(!movie)throw std::runtime_error("Required source construct receiver unavailable");swf_frame_construct(movie);break;
  case SwfFrameOp::movie_advance:if(!movie)throw std::runtime_error("Required source movie advance receiver unavailable");movie->advance(q->delta);break;
  case SwfFrameOp::event_load:if(!movie)throw std::runtime_error("Required source load-event receiver unavailable");movie->on_event(gameswf::event_id::LOAD);break;
  case SwfFrameOp::mark_garbage:if(!player)throw std::runtime_error("Required live source GC player unavailable");player->set_as_garbage();break;
  case SwfFrameOp::listeners_alive:for(int i=0;i<r->m_listener.size();++i)if(auto*object=r->m_listener[i])swf_frame_this_alive(object);break;
  case SwfFrameOp::movie_alive:if(!movie)throw std::runtime_error("Required source movie alive receiver unavailable");swf_frame_this_alive(movie);break;
  case SwfFrameOp::clear_garbage:if(!player)throw std::runtime_error("Required live source GC player unavailable");player->clear_garbage();break;
  default:throw std::runtime_error("Unknown source frame operation");
  }
  read_root(r,*s);return 1;
 }catch(const std::exception&e){call.error=e.what();read_root(r,*s);return 0;}
}
}
SwfFrameConnection::State::~State(){std::lock_guard<std::mutex>lock(mutex);auto i=receivers.find(player);if(i!=receivers.end()&&i->second.expired())receivers.erase(i);}
SwfFrameConnection::~SwfFrameConnection(){release();}
bool SwfFrameConnection::bind(gameswf::player*p,std::shared_ptr<SwfInputHistory>history,std::string&e){if(!p||!history||state_){e="Malformed source frame receiver binding";return false;}auto next=std::make_shared<State>();next->player=p;next->history=std::move(history);{std::lock_guard<std::mutex>lock(mutex);auto i=receivers.find(p);if(i!=receivers.end()&&!i->second.expired()){e="Player already has source frame receiver";return false;}receivers[p]=next;}state_=std::move(next);e.clear();return true;}
void SwfFrameConnection::release()noexcept{state_.reset();}
bool SwfFrameConnection::advance(gameswf::root*r,float dt,bool catch_up,std::string&e){auto s=state_;if(!s||!r||r->m_player.get_ptr()!=s->player){e="Malformed or foreign source frame graph";return false;}try{swf_frame_root_advance(r,dt,catch_up);e.clear();return true;}catch(const std::exception&x){e=x.what();return false;}}
bool SwfFrameConnection::gc_remaining(gameswf::root*r,float&value,std::string&e)const{auto s=state_;if(!s||!r){e="Missing source frame owner";return false;}auto i=s->roots.find(r);if(i==s->roots.end()||i->second.identity.get_ptr()!=r){e="Unobserved source root frame state";return false;}value=i->second.fields.gc_remaining;e.clear();return true;}
void swf_frame_observe_sprite(gameswf::sprite_instance*c){auto s=receiver(c->get_player());SwfFrameConnection::State::Sprite entry;entry.identity=c;
 // Source movie_def_impl.has_init_actions is a loader-produced presence byte;
 // upstream keeps the actual tag arrays. Every present init tag is represented
 // by a nonempty array, including an empty action buffer tag.
 for(int f=0;f<c->m_def->get_frame_count();++f){auto*a=c->m_def->get_init_actions(f);if(a&&a->size()>0){entry.has_init=true;break;}}
 s->sprites.insert_or_assign(c,std::move(entry));
}
void swf_frame_construct(gameswf::character*c){if(!c)return;if(!c->is(gameswf::sprite_instance::m_class_id))return;auto*sprite=static_cast<gameswf::sprite_instance*>(c);auto s=receiver(c->get_player());auto&entry=s->sprite(sprite);if(!entry.constructed){sprite->m_def->instanciate_registered_class(sprite);children_construct(sprite);entry.constructed=true;}}
void swf_frame_init_actions(gameswf::sprite_instance*c){auto s=receiver(c->get_player());auto&entry=s->sprite(c);if(!entry.has_init)return;gameswf::gc_ptr<gameswf::character>pin(c);gameswf::execute_actions(&c->m_as_environment,entry.init_pending);entry.init_pending.clear();}
void swf_frame_tags(gameswf::sprite_instance*c,int frame,bool only){gameswf::gc_ptr<gameswf::character>pin(c);auto s=receiver(c->get_player());auto&entry=s->sprite(c);auto*def=c->m_def.get_ptr();if(def->is_multithread()&&frame>=def->get_loading_frame())return;if(frame<0||frame>=def->get_frame_count())throw std::runtime_error("Invalid source frame tag index");
 if(entry.has_init&&!c->m_init_actions_executed[frame]){auto*init=def->get_init_actions(frame);if(init&&init->size()>0){for(int i=0;i<init->size();++i)(*init)[i]->execute(c);c->m_init_actions_executed[frame]=true;}for(int i=0;i<c->m_action_list.size();++i)entry.init_pending.push_back(c->m_action_list[i]);c->m_action_list.clear();}
 const auto&playlist=c->m_def->get_playlist(frame);for(int i=0;i<playlist.size();++i){if(only)playlist[i]->execute_state(c);else playlist[i]->execute(c);}
 if(!only){auto*sound=gameswf::get_sound_handler();if(sound&&c->m_def->m_ss_start==frame&&c->m_def->m_ss_id>=0){sound->stop_sound(c->m_def->m_ss_id);sound->play_sound(nullptr,c->m_def->m_ss_id,0);}}
 c->set_frame_script(frame);
}
void swf_frame_actions(gameswf::sprite_instance*c){auto s=receiver(c->get_player());if(c->m_action_list.size()>0){s->need(c,true);gameswf::gc_ptr<gameswf::character>pin(c);auto batch=c->m_action_list;c->m_action_list.clear();gameswf::execute_actions(&c->m_as_environment,batch);}
 if(c->m_frame_script){gameswf::gc_ptr<gameswf::character>pin(c);gameswf::as_value fn(c->m_frame_script.get_ptr());gameswf::call_method(fn,&c->m_as_environment,c,0,0);c->m_frame_script=nullptr;}
}
void swf_frame_goto(gameswf::sprite_instance*c,int target){auto s=receiver(c->get_player());const int count=c->m_def->get_frame_count();if(target<0||target>=count||target==c->m_current_frame){c->m_play_state=gameswf::character::STOP;return;}c->m_goto_frame_action_list=c->m_action_list;c->m_action_list.clear();const int current=c->m_current_frame;
 if(target<current)for(int f=current;f>target;--f)c->execute_frame_tags_reverse(f);else for(int f=current+1;f<target;++f)c->execute_frame_tags(f,true);
 c->m_action_list.clear();c->execute_frame_tags(target,false);c->m_current_frame=frame16(target);c->m_play_state=gameswf::character::STOP;
 for(int i=0;i<c->m_action_list.size();++i)c->m_goto_frame_action_list.push_back(c->m_action_list[i]);c->m_action_list.clear();s->notify(c);
}
void swf_frame_play(gameswf::sprite_instance*c,int play){auto s=receiver(c->get_player());auto*sound=gameswf::get_sound_handler();if(sound&&c->m_def->m_ss_id>=0)sound->pause(c->m_def->m_ss_id,c->m_play_state==gameswf::character::PLAY);auto b=std::uint8_t(play);c->m_play_state=static_cast<gameswf::character::play_state>(b<128?b:int(b)-256);s->notify(c);}
namespace {
struct SpriteCall {gameswf::sprite_instance*clip;SwfFrameConnection::State*owner;std::uintptr_t goto_storage[4096],scratch[4096];std::string error;};
void read_sprite(SpriteCall&call,SwfSpriteFrame64&s){auto*c=call.clip;auto f=call.owner->flags(c);s.sprite=reinterpret_cast<std::uintptr_t>(c);s.definition=reinterpret_cast<std::uintptr_t>(c->m_def.get_ptr());s.current_frame=c->m_current_frame;s.play_state=c->m_play_state;s.loaded=c->m_on_event_load_called;s.visible=c->get_visible();s.need=f.need9d;s.enter=f.enter_e9;
 if(c->m_goto_frame_action_list.size()>4096)throw std::runtime_error("Source goto batch exceeds native bounded storage");s.goto_actions.values=call.goto_storage;s.goto_actions.capacity=4096;s.goto_actions.count=c->m_goto_frame_action_list.size();for(unsigned i=0;i<s.goto_actions.count;++i)call.goto_storage[i]=reinterpret_cast<std::uintptr_t>(c->m_goto_frame_action_list[i]);
}
void write_sprite(SpriteCall&call,const SwfSpriteFrame64&s){auto*c=call.clip;c->m_current_frame=s.current_frame;c->m_play_state=static_cast<gameswf::character::play_state>(s.play_state);c->m_on_event_load_called=s.loaded!=0;call.owner->need(c,s.need!=0);c->m_goto_frame_action_list.resize(s.goto_actions.count);for(unsigned i=0;i<s.goto_actions.count;++i)c->m_goto_frame_action_list[i]=reinterpret_cast<gameswf::action_buffer*>(s.goto_actions.values[i]);}
int sprite_service(void*p,SwfSpriteFrame64*s,const SwfSpriteRequest32*q,int*out){auto&call=*static_cast<SpriteCall*>(p);auto*c=call.clip;write_sprite(call,*s);
 try {switch(q->operation){
 case SwfSpriteOp::construct:swf_frame_construct(c);break;
 case SwfSpriteOp::event:c->on_event(gameswf::event_id(static_cast<gameswf::event_id::id_code>(q->value)));break;
 case SwfSpriteOp::drag:swf_frame_drag(c);break;
 case SwfSpriteOp::execute_actions:{array<gameswf::action_buffer*> batch;batch.resize(q->count);for(unsigned i=0;i<q->count;++i)batch[i]=reinterpret_cast<gameswf::action_buffer*>(q->actions[i]);gameswf::execute_actions(&c->m_as_environment,batch);break;}
 case SwfSpriteOp::frame_count:*out=c->m_def->get_frame_count();break;
 case SwfSpriteOp::wrap_display_list:{const auto&playlist=c->m_def->get_playlist(0);array<int>depths;for(int i=0;i<playlist.size();++i)depths.push_back(std::uint32_t(playlist[i]->get_depth_id_of_replace_or_add_tag())>>16);if(depths.size())c->m_display_list.clear_unaffected(depths);else c->m_display_list.clear();break;}
 case SwfSpriteOp::frame_tags:c->execute_frame_tags(q->frame,false);break;
 case SwfSpriteOp::do_actions:swf_frame_actions(c);break;
 case SwfSpriteOp::children_advance:*out=children_advance(c,q->delta,*call.owner);break;
 case SwfSpriteOp::warning:gameswf::log_error("source sprite goto action iteration limit\n");break;
 default:throw std::runtime_error("Unknown source sprite frame operation");
 }read_sprite(call,*s);return 1;
 }catch(const std::exception&e){call.error=e.what();read_sprite(call,*s);return 0;}
}
}
void swf_frame_sprite_advance(gameswf::sprite_instance*c,float dt){auto owner=receiver(c->get_player());owner->sprite(c);SpriteCall call{};call.clip=c;call.owner=owner.get();SwfSpriteFrame64 state{};read_sprite(call,state);state.scratch={call.scratch,0,4096};SwfSpriteServices16 services{&call,sprite_service};int rc=dh2_ui_swf_sprite_frame(&state,dt,&services);write_sprite(call,state);if(rc)throw std::runtime_error(call.error.empty()?"Malformed source sprite frame storage":call.error);}

void swf_frame_drag(gameswf::character*c){auto owner=receiver(c->get_player());if(c->get_root()->m_drag_state.GetCharacter()==c){auto*r=c->get_root();auto&entry=root_entry(*owner.get(),r);if(entry.drag_identity.get_ptr()!=c)throw std::runtime_error("Unobserved or expired source drag target");owner.get()->need(c,true);SwfDragValues108 input{};int buttons=0;r->get_mouse_state(&input.mouse[0],&input.mouse[1],&buttons);matrix_words(c->get_world_matrix(),input.world);gameswf::matrix parent;if(auto*p=c->get_parent())parent=p->get_world_matrix();matrix_words(parent,input.parent_world);matrix_words(c->get_matrix(),input.local);input.offset[0]=r->m_drag_state.OffsetX();input.offset[1]=r->m_drag_state.OffsetY();input.initialized=entry.drag_initialized;input.lock_center=r->m_drag_state.IsLockCentered();gameswf::rect rect;input.bounded=r->m_drag_state.GetBounds(&rect);if(input.bounded){input.bounds[0]=rect.m_x_min;input.bounds[1]=rect.m_x_max;input.bounds[2]=rect.m_y_min;input.bounds[3]=rect.m_y_max;}SwfDragResult44 result{};if(dh2_ui_swf_drag_values(&result,&input))throw std::runtime_error("Invalid source drag fields");if(!input.lock_center&&!input.initialized){r->m_drag_state.SetOffset(result.offset[0],result.offset[1]);entry.drag_initialized=true;owner.get()->notify(c);}gameswf::matrix local;for(unsigned y=0;y<2;++y)for(unsigned x=0;x<3;++x)local.m_[y][x]=result.matrix[y*3+x];c->set_matrix(local);}}
void swf_frame_character_advance(gameswf::character*c,float dt){auto owner=receiver(c->get_player());if(c->is(gameswf::sprite_instance::m_class_id)||c->is(gameswf::edit_text_character::m_class_id)||dynamic_cast<gameswf::button_character_definition*>(c->get_character_def()))c->advance(dt);else owner->need(c,false);}
void swf_frame_start_drag(gameswf::root*r){auto s=receiver(r->m_player.get_ptr());auto&entry=root_entry(*s,r);auto*target=r->m_drag_state.GetCharacter();entry.drag_identity=target;entry.drag_initialized=false;if(target)s->notify(target);}
void swf_frame_this_alive(gameswf::as_object*object){
 if(!object)return;
 if(!object->is(gameswf::sprite_instance::m_class_id)){object->this_alive();return;}
 auto*c=static_cast<gameswf::sprite_instance*>(object);auto*player=c->get_player();
 if(!player)throw std::runtime_error("Required source sprite GC player unavailable");
 if(!player->is_garbage(c))return;
 // 0x780594: base member traversal first, then the captured display count.
 // Each element and its garbage epoch are read freshly in source order.
 c->gameswf::as_object::this_alive();
 const int count=c->m_display_list.size();
 for(int i=0;i<count;++i)if(auto*child=c->m_display_list.get_character(i))
  if(player->is_garbage(child))swf_frame_this_alive(child);
}
void swf_frame_root_advance(gameswf::root*r,float dt,bool catch_up){auto*p=r->m_player.get_ptr();auto s=receiver(p);if(r->m_def->m_is_avm2)throw std::runtime_error("Unsupported source AVM2 frame interpreter");auto&i=root_entry(*s,r);
 read_root(r,i.fields);RootCall call{r,s.get(),{}};SwfFrameServices16 services{&call,root_service};const int rc=dh2_ui_swf_root_frame(&i.fields,dt,catch_up,&services);write_root(r,i.fields);if(rc)throw std::runtime_error(call.error.empty()?"Invalid source root frame state":call.error);
}
}


