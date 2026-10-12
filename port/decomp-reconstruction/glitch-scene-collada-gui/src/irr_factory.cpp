#include "glitch-scene-collada-gui/irr_factory.hpp"

namespace glitch {

CIrrFactory* CIrrFactory::s_instance = nullptr;

// 0x533e98 _ZN6glitch11CIrrFactoryC2Ev
// 0x533ecc _ZN6glitch11CIrrFactoryC1Ev
CIrrFactory::CIrrFactory() {
    s_instance = this;
}

// 0x533fe8 _ZN6glitch11CIrrFactory11getInstanceEv
CIrrFactory* CIrrFactory::getInstance() {
    if (s_instance != nullptr) {
        return s_instance;
    }
    // The function-local object is constructed once, and its constructor sets s_instance.
    static CIrrFactory instance;
    return &instance;
}

}  // namespace glitch
