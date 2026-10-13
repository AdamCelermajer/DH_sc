// TestUD2: a Lua test user-data object that extends TestUD. Its createBindings calls TestUD::createBindings
// first, then binds Test2 (bound twice in the original). It adds no data members and its destructor
// frees only the object. Its vtable is 24 bytes, the same four-slot shape as TestUD.
// The base TestUD is inferred from TestUD2::createBindings calling TestUD::createBindings directly; the
// class-facts report lists no base, so this is open to confirmation.
// Original 32-bit size is that of TestUD (0x08). No operator new size was recovered.
// Reconstructed from libDungeonHunter2.so pseudocode (dtor 0x386ae0 / 0x386ae4, createBindings 0x386da4).
#pragma once

#include "dh2/TestUD.hpp"

class TestUD2 : public TestUD {
public:
    ~TestUD2() override;                                                            // 0x386ae0 (D1), 0x386ae4 (D0)
    const char* getUDTypeName() const override;                                     // slot 0x08: 0x386ac4 "TestUD2"
    void createBindings(sfc::script::lua::Binder& binder) override;                 // slot 0x0c: 0x386da4

    void Test2(const sfc::script::lua::Arguments& args,
               sfc::script::lua::ReturnValues& ret, void* user);                    // 0x386ad4, empty body
};
