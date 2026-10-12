// glitch::CIrrFactory: the singleton that creates the GUI environment and the file
// system. Its constructor publishes the object through the static s_instance
// pointer, and getInstance returns that pointer, creating a function-local instance
// on first use.
#pragma once

namespace glitch {

class CIrrFactory {
public:
    // 0x533e98 _ZN6glitch11CIrrFactoryC2Ev
    // 0x533ecc _ZN6glitch11CIrrFactoryC1Ev
    CIrrFactory();
    // 0x533f00 _ZN6glitch11CIrrFactoryD2Ev (empty)
    // 0x533f04 _ZN6glitch11CIrrFactoryD1Ev (empty)
    virtual ~CIrrFactory() = default;

    // 0x533fe8 _ZN6glitch11CIrrFactory11getInstanceEv
    static CIrrFactory* getInstance();

    // _ZN6glitch11CIrrFactory10s_instanceE: the object published by the constructor.
    static CIrrFactory* s_instance;
};

}  // namespace glitch
