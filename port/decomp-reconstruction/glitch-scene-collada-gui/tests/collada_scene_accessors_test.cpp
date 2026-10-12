// Checks the glitch::collada scene-node, mesh, timeline and animation-set
// accessors that read one member or forward to another accessor, plus the
// identity values of the float and weight tracks.
#include "glitch-scene-collada-gui/collada_scene_accessors.hpp"
#include "glitch-scene-collada-gui/collada_misc.hpp"
#include "glitch-scene-collada-gui/collada_empty_dtors.hpp"

#include <cstdio>

using namespace glitch::collada;

static int failures = 0;
#define CHECK(c) do { if (!(c)) { std::printf("FAIL %s:%d %s\n", __FILE__, __LINE__, #c); ++failures; } } while (0)

int main() {
    // CDynamicAnimationSet: channels are 16-byte records indexed directly.
    {
        SChannelRecord channels[3] = {};
        CDynamicAnimationSet set;
        set.channels = channels;
        CHECK(set.getChannel(0) == &channels[0]);
        CHECK(set.getChannel(2) == &channels[2]);
        const CDynamicAnimationSet& cset = set;
        CHECK(cset.getChannel(1) == &channels[1]);
    }

    // Bounding boxes are returned by reference to the stored box.
    {
        CMesh mesh;
        CHECK(&mesh.getBoundingBox() == &mesh.boundingBox);
        CCoronasSceneNode corona;
        CHECK(&corona.getBoundingBox() == &corona.boundingBox);
    }

    // The morphing mesh clone returns a null handle.
    {
        CMorphingMesh morph;
        CHECK(!morph.clone());
    }

    // Modular skinned mesh scene node forwards to the mesh's category table.
    {
        CModularSkinnedMesh::SCategoryEntry entries[2] = {};
        entries[0].moduleCount = 4;
        entries[1].moduleCount = 7;
        CModularSkinnedMesh::SCategoryTable table;
        table.count = 2;
        table.entries = entries;
        CModularSkinnedMesh mesh;
        mesh.categories = &table;
        mesh.currentModules.resize(2);
        mesh.currentModules[1].moduleId = 9;
        CModularSkinnedMeshSceneNode node;
        node.mesh = &mesh;
        CHECK(node.getCategoryCount() == 2);
        CHECK(node.getCategoryModuleCount(1) == 7);
        CHECK(node.getCategoryModuleCount(0) == 4);
        CHECK(node.getCurrentModuleId(1) == 9);
    }

    // Animation set ranges read through the scene-node animator set.
    {
        std::uint32_t lengths[] = {30, 60};
        std::uint32_t starts[] = {1, 2};
        std::uint32_t ends[] = {10, 20};
        CAnimationSet anim;
        anim.lengths_begin = lengths;
        anim.lengths_end = lengths + 2;
        anim.starts_begin = starts;
        anim.starts_end = starts + 2;
        anim.ends_begin = ends;
        anim.ends_end = ends + 2;
        anim.animatedTrackCount = 5;
        CSceneNodeAnimatorSet set;
        set.animationSet = &anim;
        CHECK(set.getAnimationLength(1) == 60);
        CHECK(set.getAnimationLength(3) == 0);
        CHECK(set.getAnimationStart(1) == 2);
        CHECK(set.getAnimationEnd(0) == 10);
        CHECK(set.getTargetCount() == 5);
    }

    // User property: zero without a property block.
    {
        CSceneNode node;
        CHECK(node.getUserProperty() == 0);
        SNodeUserProperties props;
        props.value = -12;
        node.userProperties = &props;
        CHECK(node.getUserProperty() == -12);
    }

    // Scene-node uids come from the holder; the particle bounding box is a pointer.
    {
        SUidHolder holder;
        holder.uid = "emitter";
        CParticleSystemSceneNode ps;
        ps.uidHolder = &holder;
        CHECK(ps.getUID() == holder.uid);
        CHECK(ps.getTransformedBoundingBox() == nullptr);
        glitch::core::aabbox3d<float> box{};
        ps.transformedBoundingBox = &box;
        CHECK(ps.getTransformedBoundingBox() == &box);

        CGlitchNewParticleSystemSceneNode gps;
        gps.uidHolder = &holder;
        gps.transformedBoundingBox = &box;
        CHECK(gps.getUID() == holder.uid);
        CHECK(gps.getTransformedBoundingBox() == &box);

        CCameraSceneNode cam;
        cam.uidHolder = &holder;
        CHECK(cam.getUID() == holder.uid);
        CHECK(cam.getTargetNode() == nullptr);
    }

    // Mesh scene node: the holder wins when present, else the named mesh uid.
    {
        SNamedMeshData named;
        named.uid = "mesh-uid";
        CMeshSceneNode node;
        node.namedMesh = &named;
        CHECK(node.getUID() == named.uid);
        SUidHolder holder;
        holder.uid = "holder-uid";
        node.uidHolder = &holder;
        CHECK(node.getUID() == holder.uid);
    }

    // Timeline clip table: count, start, end and name per clip; no table gives 0.
    {
        STimelineClip clips[2];
        clips[0].name = "idle";
        clips[0].start = 0;
        clips[0].end = 30;
        clips[1].name = "run";
        clips[1].start = 30;
        clips[1].end = 50;
        STimelineClipTable table;
        table.count = 2;
        table.clips = clips;
        CTimelineController timeline;
        CHECK(timeline.getClipCount() == 0);
        timeline.clipTable = &table;
        CHECK(timeline.getClipCount() == 2);
        CHECK(timeline.getClipStart(1) == 30);
        CHECK(timeline.getClipEnd(1) == 50);
        CHECK(timeline.getClipName(0) == clips[0].name);
    }

    // Float and weight tracks: identity is 0.0 in the output word.
    {
        glitch::collada::animation_track::CFloatEx f;
        glitch::collada::animation_track::CWeightEx w;
        float out = 3.0f;
        f.getIdentityValue(&out);
        CHECK(out == 0.0f);
        out = 3.0f;
        w.getIdentityValue(&out);
        CHECK(out == 0.0f);
    }

    std::printf("%s\n", failures ? "collada_scene_accessors: FAILED" : "collada_scene_accessors: ok");
    return failures != 0;
}
