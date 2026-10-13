// TestUD: a Lua test user-data object. createBindings publishes TestFunc and TestStaticMethod as Lua
// functions and TestMethod as a method. The name TestStaticMethod is kept from the original; its body
// reads this, so it is a member too. The vtable has four slots: two destructors, getUDTypeName and
// createBindings. The vtable symbol is 24 bytes: offset-to-top, typeinfo, then four slots.
// Original 32-bit size is at least 0x08 (8): vptr plus one pointer. No operator new size was recovered.
// Reconstructed from libDungeonHunter2.so pseudocode (dtor 0x386ad8 / 0x386b18, createBindings 0x386d00).
#pragma once

#include <cstdint>

namespace sfc {
namespace script {
namespace lua {
class Arguments;
class ReturnValues;
class Binder;
}  // namespace lua
}  // namespace script
}  // namespace sfc

class TestUD {
public:
    virtual ~TestUD();                                                              // 0x386ad8 (D1), 0x386b18 (D0)
    virtual const char* getUDTypeName() const;                                      // slot 0x08: 0x386ab4 "TestUD"
    virtual void createBindings(sfc::script::lua::Binder& binder);                  // slot 0x0c: 0x386d00

    void TestMethod(const sfc::script::lua::Arguments& args,
                    sfc::script::lua::ReturnValues& ret);                           // 0x386e64
    void TestStaticMethod(const sfc::script::lua::Arguments& args,
                          sfc::script::lua::ReturnValues& ret, void* user);         // 0x386e08
    void TestFunc(const sfc::script::lua::Arguments& args,
                  sfc::script::lua::ReturnValues& ret, void* user);                 // 0x386ec0

    // Original +0x00 vptr (implicit). +0x04 is the first member: a pointer to a {begin, end} pair of
    // 16-byte items. TestFunc and TestStaticMethod print (end - begin) / 16 as the argument count.
    void* unk_04;                                                                   // +0x04
};
