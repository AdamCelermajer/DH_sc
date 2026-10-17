#include "script_value_boolean_lua.hpp"
extern "C" {
#include "lua.h"
#include "lauxlib.h"
}
#include <algorithm>
#include <vector>
namespace dh2::script_value_boolean_lua {
static_assert(sizeof(lua_Number)==4,"reuse the source float32 Lua build");
struct TemporaryLua::Impl {
    std::vector<lua_State*> states;
    Statistics stats{};bool busy=false;
};
namespace {
struct Push {const char* text;};
int push_protected(lua_State* L){
    auto* input=static_cast<Push*>(lua_touserdata(L,1));
    lua_pushstring(L,input->text);
    // cpcall discards its return stack. Copy the actual resulting Value to the
    // retained registry, then restore it on the outer stack after success.
    lua_setfield(L,LUA_REGISTRYINDEX,"dh2_getbool_value");return 0;
}
struct Busy {bool& value;explicit Busy(bool& b):value(b){value=true;}~Busy(){value=false;}};
}
TemporaryLua::TemporaryLua():impl_(std::make_unique<Impl>()){}
TemporaryLua::~TemporaryLua(){for(auto* L:impl_->states)if(L)lua_close(L);}
script_value_boolean::Services TemporaryLua::services() noexcept{return {this,invoke};}
Statistics TemporaryLua::statistics() const noexcept{auto s=impl_->stats;s.live=impl_->states.size();return s;}
std::int32_t TemporaryLua::invoke(void* context,const script_value_boolean::Value*,
        const script_value_boolean::Request* q,script_value_boolean::Response* reply){
    using Op=script_value_boolean::Operation;
    auto& self=*static_cast<TemporaryLua*>(context);auto& impl=*self.impl_;
    if(impl.busy||!q||!reply)return 1;
    Busy busy(impl.busy);
    if(q->operation==Op::new_state){
        if(q->lua)return 1;
        // Reserve ownership metadata before the actual source allocation, so
        // a host allocation exception cannot lose a created Lua resource.
        impl.states.push_back(nullptr);auto* L=luaL_newstate();
        if(!L){impl.states.pop_back();++impl.stats.failed;reply->lua=0;return 0;}
        impl.states.back()=L;++impl.stats.created;reply->lua=reinterpret_cast<std::uintptr_t>(L);return 0;
    }
    auto* L=reinterpret_cast<lua_State*>(q->lua);
    const auto owned=std::find(impl.states.begin(),impl.states.end(),L);
    if(!L||owned==impl.states.end()){++impl.stats.failed;return 1;}
    if(q->operation==Op::push_string){
        Push input{q->text};
        // Native adapter catches unprotected Lua allocation/panic errors as a
        // port boundary. The resource remains owned; no per-call close added.
        if(lua_cpcall(L,push_protected,&input)){++impl.stats.failed;return 1;}
        // The key is already interned by the protected setfield operation;
        // this raw retained-state fetch restores the actual pushed Value after
        // cpcall discards its C frame, without allocating a second C closure.
        lua_getfield(L,LUA_REGISTRYINDEX,"dh2_getbool_value");
        ++impl.stats.pushed;return 0;
    }
    if(q->operation==Op::to_boolean){
        if(q->index!=-1){++impl.stats.failed;return 1;}
        reply->raw_boolean=std::uint32_t(lua_toboolean(L,q->index));++impl.stats.queried;return 0;
    }
    if(q->operation==Op::close_state){lua_close(L);impl.states.erase(owned);++impl.stats.closed;return 0;}
    ++impl.stats.failed;return 1;
}
} // namespace dh2::script_value_boolean_lua
