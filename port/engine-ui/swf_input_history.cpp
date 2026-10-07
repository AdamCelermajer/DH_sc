#include "swf_input_history.hpp"
#include "gameswf/gameswf_character.h"
#include "gameswf/gameswf_sprite.h"
#include "base/weak_ptr.h"
#include <cstring>
#include <map>
#include <mutex>
#include <stdexcept>
namespace dh2::ui {
struct SwfInputHistory::State {
 struct Entry {weak_ptr<gameswf::character> identity;SwfInputHistoryFlags flags{0,1,0,0};};
 gameswf::player* player{};std::map<gameswf::character*,Entry> entries;
 Entry& require(gameswf::character*c){auto i=entries.find(c);if(i==entries.end()||i->second.identity.get_ptr()!=c)throw std::runtime_error("Required observed source character constructor unavailable");return i->second;}
};
namespace {
std::mutex receivers_mutex;
std::map<gameswf::player*,std::weak_ptr<SwfInputHistory::State>> receivers;
std::shared_ptr<SwfInputHistory::State> receiver(gameswf::player*p){std::lock_guard<std::mutex>lock(receivers_mutex);auto i=receivers.find(p);return i==receivers.end()?nullptr:i->second.lock();}
}
SwfInputHistory::~SwfInputHistory(){release();}
bool SwfInputHistory::bind(gameswf::player*p,std::string&error){if(!p||state_){error="Malformed source input observer binding";return false;}auto next=std::make_shared<State>();next->player=p;{std::lock_guard<std::mutex>lock(receivers_mutex);auto i=receivers.find(p);if(i!=receivers.end()&&!i->second.expired()){error="Player already has an input history receiver";return false;}receivers[p]=next;}state_=std::move(next);error.clear();return true;}
void SwfInputHistory::release()noexcept{if(state_){std::lock_guard<std::mutex>lock(receivers_mutex);auto i=receivers.find(state_->player);if(i!=receivers.end()&&i->second.lock()==state_)receivers.erase(i);state_.reset();}}
bool SwfInputHistory::read(gameswf::character*c,SwfInputHistoryFlags&out,std::string&error)const{if(!state_||!c){error="Required source input history owner unavailable";return false;}try{out=state_->require(c).flags;error.clear();return true;}catch(const std::exception&e){error=e.what();return false;}}
bool SwfInputHistory::write_need(gameswf::character*c,bool value,std::string&error){if(!state_||!c){error="Required source input history owner unavailable";return false;}try{state_->require(c).flags.need9d=value;error.clear();return true;}catch(const std::exception&e){error=e.what();return false;}}
bool SwfInputHistory::notify(gameswf::character*c,std::string&error){if(!state_||!c){error="Required source input history owner unavailable";return false;}try{for(auto*p=c;p;p=p->get_parent())state_->require(p).flags.need9d=1;error.clear();return true;}catch(const std::exception&e){error=e.what();return false;}}
void swf_input_observe_constructor(gameswf::character*c){if(!c)return;auto s=receiver(c->get_player());if(!s)return;SwfInputHistory::State::Entry next;next.identity=c;s->entries.insert_or_assign(c,std::move(next));}
void swf_input_observe_assignment(gameswf::as_object*object,const char*name){if(!object||!name||!object->is(gameswf::sprite_instance::m_class_id))return;auto*c=static_cast<gameswf::character*>(object);auto s=receiver(c->get_player());if(!s)return;
 if(dh2_ui_swf_note_assignment(&s->require(c).flags,name)==2)for(auto*p=c;p;p=p->get_parent())s->require(p).flags.need9d=1;
}
}
