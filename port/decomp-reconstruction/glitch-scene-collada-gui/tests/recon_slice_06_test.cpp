// Slice 06 checks: registry, deletion queue, name/type lookups, flags and refcounts.
#include <cstdio>

#include "glitch-scene-collada-gui/recon/slice_06.hpp"

namespace rc = glitch::collada::recon_06;

namespace {

int failures = 0;

void check(bool condition, const char* what) {
    if (!condition) {
        std::printf("FAIL: %s\n", what);
        ++failures;
    }
}

}  // namespace

int main() {
    // addShadowReceiverTarget: a second add of the same target is ignored.
    {
        rc::SceneManager manager;
        auto* target = new rc::RefCounted();
        rc::addShadowReceiverTarget(manager, target);
        rc::addShadowReceiverTarget(manager, target);
        check(manager.shadowReceiverTargets.size() == 1, "shadow receiver added once");
        check(target->referenceCount == 2, "shadow receiver grabbed once");
        manager.shadowReceiverTargets.clear();
        delete target;
    }

    // addToDeletionQueue and clearDeletionList.
    {
        rc::SceneManager manager;
        auto* node = new rc::SceneNode();
        rc::addToDeletionQueue(manager, node);
        check(manager.deletionQueue.size() == 1, "deletion queue holds the node");
        check(node->referenceCount == 2, "deletion queue grabs the node");
        rc::clearDeletionList(manager);
        check(manager.deletionQueue.empty(), "clearDeletionList empties the queue");
        check(node->referenceCount == 1, "clearDeletionList drops the node");
        delete node;
    }

    // Scene tree: root (type "smgr") with a child "Door" and a grandchild "Light".
    rc::SceneNode root;
    root.type = rc::kNodeTypeSceneManagerRoot;
    root.name = "Root";
    root.uid = "uid-root";
    root.scopeId = "scope-root";

    auto* door = new rc::SceneNode();
    door->name = "Door";
    door->uid = "uid-door";
    door->scopeId = "scope-door";
    door->type = rc::fourcc("mesh");
    root.children.push_back(door);

    auto* light = new rc::SceneNode();
    light->name = "Light";
    light->type = rc::fourcc("lite");
    door->children.push_back(light);

    // getSceneNodeFromName is case-insensitive and searches the children.
    check(rc::getSceneNodeFromName(root, "door") == door, "name lookup ignores case");
    check(rc::getSceneNodeFromName(root, "LIGHT") == light, "name lookup finds grandchild");
    check(rc::getSceneNodeFromName(root, "missing") == nullptr, "name lookup misses");
    check(rc::getSceneNodeFromName(root, "Root") == &root, "name lookup returns self");

    // UID and scope lookups use their own keys.
    check(rc::getSceneNodeFromUID(root, "UID-DOOR") == door, "uid lookup");
    check(rc::getSceneNodeFromScopeID(root, "scope-door") == door, "scope lookup");

    // getSceneNodeFromType: exact type, wildcard, and missing.
    check(rc::getSceneNodeFromType(root, rc::fourcc("lite")) == light, "type lookup");
    check(rc::getSceneNodeFromType(root, rc::kNodeTypeAny) == &root, "wildcard type returns self");
    check(rc::getSceneNodeFromType(root, rc::fourcc("none")) == nullptr, "type lookup misses");

    // getSceneNodesFromType collects every match in the tree.
    {
        std::vector<rc::SceneNode*> out;
        rc::getSceneNodesFromType(root, rc::kNodeTypeAny, out);
        check(out.size() == 3, "wildcard collects all three nodes");
    }

    // collectAllNodes skips the empty, dummy, daen and scene-manager-root types.
    {
        rc::SceneManager manager;
        manager.root = &root;
        manager.collectPending = true;
        rc::collectAllNodes(manager);
        check(!manager.collectPending, "collectAllNodes clears the pending flag");
        check(manager.renderNodes.size() == 2, "root is skipped; door and light collected");
    }

    // setTriangleSelector grabs the new selector and drops the old one.
    {
        auto* first = new rc::TriangleSelector();
        auto* second = new rc::TriangleSelector();
        rc::setTriangleSelector(root, first);
        check(first->referenceCount == 2, "selector grabbed");
        rc::setTriangleSelector(root, second);
        check(first->referenceCount == 1, "old selector dropped");
        check(second->referenceCount == 2, "new selector grabbed");
        root.triangleSelector = nullptr;
        delete first;
        delete second;
    }

    // Visibility: a hidden latch keeps the visible bit clear.
    {
        rc::SceneNode node;
        rc::setVisible(node, false);
        rc::notifyVisibilityChanged(node, false);
        rc::setVisible(node, true);
        check((node.flags & 1u) == 0, "latched-off node stays out of the visible bit");
        rc::notifyVisibilityChanged(node, true);
        check((node.flags & 1u) != 0, "latch on sets the visible bit");
    }

    // addBindedAnimator dedupes and grabs once.
    {
        rc::SceneNode node;
        auto* animator = new rc::SceneNodeAnimator();
        rc::addBindedAnimator(node, animator);
        rc::addBindedAnimator(node, animator);
        check(node.bindedAnimators.size() == 1, "binded animator added once");
        check(animator->referenceCount == 2, "binded animator grabbed once");
        node.bindedAnimators.clear();
        delete animator;
    }

    // setName overloads.
    {
        rc::SceneNode node;
        rc::setName(node, "alpha");
        check(node.name == "alpha", "setName char const*");
        rc::setName(node, std::string("beta"));
        check(node.name == "beta", "setName string");
    }

    // SBatchConfig copy assignment copies the words.
    {
        rc::SBatchConfig from;
        from.words1[0] = 7;
        from.words2[3] = 9;
        rc::SBatchConfig to;
        to = from;
        check(to.words1[0] == 7 && to.words2[3] == 9, "SBatchConfig copies words");
    }

    // SAnimatedMesh::getMesh returns the element at the index, with a grab.
    {
        rc::SAnimatedMesh mesh;
        auto* first = new rc::RefCounted();
        auto* second = new rc::RefCounted();
        mesh.meshes.push_back(first);
        mesh.meshes.push_back(second);
        check(rc::getMesh(mesh, 1) == second, "getMesh index");
        check(second->referenceCount == 2, "getMesh grabs");
        check(rc::getMesh(rc::SAnimatedMesh(), 0) == nullptr, "getMesh on empty list");
        second->referenceCount = 1;
        delete first;
        delete second;
    }

    // Free the tree nodes allocated above.
    delete light;
    delete door;

    if (failures != 0) {
        std::printf("%d failure(s)\n", failures);
        return 1;
    }
    std::printf("slice_06 tests passed\n");
    return 0;
}
