#pragma once
#include "script_value_boolean.hpp"
#include <cstddef>
#include <cstdint>
#include <memory>
namespace dh2::script_value_boolean_lua {
struct Statistics {
    std::uint32_t created,pushed,queried,closed,failed;
    std::size_t live;
};
// Real dependency adapter over the existing source-built float32 Lua5.1.4.
// Every type4 getBool gets its own fresh no-library luaL_newstate, actual
// lua_pushstring / lua_toboolean(-1) / lua_close. No whole-VM parity claim.
// Failed per-call resources are retained here; the source execute() does not
// add cleanup. Outer adapter destruction closes remaining owned states.
class TemporaryLua {
public:
    TemporaryLua();
    ~TemporaryLua();
    TemporaryLua(const TemporaryLua&)=delete;
    TemporaryLua& operator=(const TemporaryLua&)=delete;
    script_value_boolean::Services services() noexcept;
    Statistics statistics() const noexcept;
private:
    struct Impl;std::unique_ptr<Impl> impl_;
    static std::int32_t invoke(void*,const script_value_boolean::Value*,
        const script_value_boolean::Request*,script_value_boolean::Response*);
};
// One owning thread retains adapter/strings through operations. No same-adapter
// reentry/destruction during a callback. Unknown state handles/indices/operations
// fail before actual Lua effects. Protected push catches Lua allocation errors
// as port failures; this bounded failure mode is not original panic parity.
} // namespace dh2::script_value_boolean_lua
