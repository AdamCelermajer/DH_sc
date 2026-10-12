// Checks the CIrrFactory singleton: the constructor publishes the object and
// getInstance returns the published object.
#include "glitch-scene-collada-gui/irr_factory.hpp"

#include <cstdio>

using glitch::CIrrFactory;

static int failures = 0;
#define CHECK(c) do { if (!(c)) { std::printf("FAIL %s:%d %s\n", __FILE__, __LINE__, #c); ++failures; } } while (0)

int main() {
    CHECK(CIrrFactory::s_instance == nullptr);
    CIrrFactory* first = CIrrFactory::getInstance();
    CHECK(first != nullptr);
    CHECK(CIrrFactory::getInstance() == first);
    CHECK(CIrrFactory::s_instance == first);

    // A directly constructed factory publishes itself while it lives.
    CIrrFactory* saved = CIrrFactory::s_instance;
    {
        CIrrFactory local;
        CHECK(CIrrFactory::s_instance == &local);
    }
    CIrrFactory::s_instance = saved;

    std::printf("%s\n", failures ? "irr_factory: FAILED" : "irr_factory: ok");
    return failures != 0;
}
