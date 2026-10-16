// RT debug: dump of characters (name, visual, scene node visibility) for rendering bisects, called from the windowed runner (DH2_CHARDUMP).
#include <cstdio>
#include <cstring>
#include <typeinfo>
#include "dh2/Application.hpp"
#include "dh2/Character.hpp"
#include "dh2/ObjectManager.hpp"
#include "dh2/VisualObject.hpp"
#include "dh2/RootSceneNode.hpp"
#include "glitch-scene-collada-gui/scene/ISceneNode.hpp"
#include "host_runner.hpp"

namespace dh2host {
static void walk(glitch::scene::ISceneNode* n, int depth) {
    const auto bb = n->getBoundingBox();
    std::fprintf(g_runnerOut, "[chardump]  %*s%s type=%x vis=%d vp=%d flags=%x cull=%d bb=(%.0f,%.0f,%.0f)-(%.0f,%.0f,%.0f)\n", depth * 2, "", n->getName(), (unsigned)n->getType(), n->visible, n->visibleInParent, n->flags, 0,
                 bb.MinEdge.X, bb.MinEdge.Y, bb.MinEdge.Z, bb.MaxEdge.X, bb.MaxEdge.Y, bb.MaxEdge.Z);
    for (auto it = n->children.begin(); it != n->children.end(); ++it) walk(*it, depth + 1);
}
void rtCharDump() {
    Application& app = ApplicationGetInstance();
    if (!app.m_objectManager) return;
    for (Character* c : app.m_objectManager->m_characters) {
        const char* nm = c->m_name.c_str();
        if (!(std::strstr(nm, "Priest") || std::strstr(nm, "Faery") || std::strstr(nm, "Moth_02") || c == app.m_objectManager->m_characters.front())) continue;
        glitch::scene::ISceneNode* n = c->m_visualObject ? static_cast<glitch::scene::ISceneNode*>(c->m_visualObject->m_node) : nullptr;
        std::fprintf(g_runnerOut, "[chardump] %s vis=%d en=%d visual=%p node=%p", nm, c->m_visible, c->m_enabled, (void*)c->m_visualObject, (void*)n);
        if (n) {
            const glitch::core::vector3d<float> p = n->getAbsolutePosition();
            std::fprintf(g_runnerOut, " nvis=%d nvisParent=%d parent=%p pos=(%.0f,%.0f,%.0f) name=%s", n->visible, n->visibleInParent, (void*)n->getParent(), p.X, p.Y, p.Z, n->getName());
        }
        std::fprintf(g_runnerOut, "\n");
        if (n && std::strstr(nm, "Priest")) walk(n, 0);
    }
}
}  // namespace dh2host
