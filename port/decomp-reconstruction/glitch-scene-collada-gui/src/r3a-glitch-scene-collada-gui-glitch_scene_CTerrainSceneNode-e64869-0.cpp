// Batch r3a-glitch-scene-collada-gui-glitch_scene_CTerrainSceneNode-e64869-0 (part 1 of 2).
// Bodies of glitch::scene::CTerrainSceneNode that the terrain layout in scene_simple_accessors.hpp can carry.
// The class has no canonical header yet (it is a stand-in there, without the scene::ISceneNode base), so the rows
// that touch the base, the material holder at +0x1ac, the render buffer at +0x1b0 or the patch record tables stay
// partial. Their list is at the end of this file.
#include "glitch-scene-collada-gui/scene_simple_accessors.hpp"

namespace glitch::scene {

// 0x6d0238 _ZN6glitch5scene17CTerrainSceneNode19overrideLODDistanceEid
// Sets the override flag (+0x1bd). For an index inside the table (count +0x180) the squared distance is stored at
// table[index] and the function returns 1; otherwise it returns 0.
std::int32_t CTerrainSceneNode::overrideLODDistance(std::int32_t index, double distance) {
    lodDistanceOverridden = 1;
    if (index < 0 || index >= lodDistanceCount) {
        return 0;
    }
    lodDistanceSquared[static_cast<std::size_t>(index)] = distance * distance;
    return 1;
}

// 0x6d042c _ZN6glitch5scene17CTerrainSceneNode22setCurrentLODOfPatchesERKSt6vectorIiNS_4core10SAllocatorIiLNS_6memory13E_MEMORY_HINTE0EEEE
// patchesPerSide squared patches each take one LOD value from the input, in order (stride 0x38 in the patch table).
void CTerrainSceneNode::setCurrentLODOfPatches(const std::vector<std::int32_t>& lods) {
    const std::int32_t count = patchesPerSide * patchesPerSide;
    if (count > 0) {
        for (std::int32_t i = 0; i != count; ++i) {
            patches[static_cast<std::size_t>(i)].lod = lods[static_cast<std::size_t>(i)];
        }
    }
}

// 0x6d21ac _ZNK6glitch5scene17CTerrainSceneNode22getCurrentLODOfPatchesERSt6vectorIiNS_4core10SAllocatorIiLNS_6memory13E_MEMORY_HINTE0EEEE
// Clears the output and appends the LOD word of every patch (patchesPerSide squared entries). The original inlines the
// vector growth; the append is the same sequence.
void CTerrainSceneNode::getCurrentLODOfPatches(std::vector<std::int32_t>& lods) const {
    lods.clear();
    const std::int32_t count = patchesPerSide * patchesPerSide;
    if (count > 0) {
        for (std::int32_t i = 0; i != count; ++i) {
            lods.push_back(patches[static_cast<std::size_t>(i)].lod);
        }
    }
}

// PARTIAL rows of this batch (no body written; the ledger notes give the reason per row):
//   0x6d0074 getMaterial(uint) const          forwards to the holder at +0x1ac (vtable slot 6, sret); holder type not declared.
//   0x6d0098 getMaterialCount() const         forwards to the holder at +0x1ac (vtable slot 4); holder type not declared.
//   0x6d00d8 getRenderBuffer() const          counted pointer at +0x1b0 (count at +4); element type not declared.
//   0x6d01f0 loadHeightMapRAW(...)            the Hex-Rays body is a stub returning 0 (ELF size 16); needs asm check.
//   0x6d028c getIndex(...) const              reads the patch record tables (+0x28..+0x34) and the grid words +0x130/+0x174/+0x178.
//   0x6d0470, 0x6d0660 preRenderIndicesCalculationsDirect<T>  index generation over the patch tables; not reconstructed.
//   0x6d083c serializeAttributes(...) const   the ISceneNode base serialization is not in the stand-in.
//   0x6d0cfc createPatches()                  patch table build; not reconstructed.
//   0x6d0dbc preRenderLODCalculations()       LOD selection over the patch table; not reconstructed.
//   0x6d127c ~CTerrainSceneNode (D2), 0x6d132c ~CTerrainSceneNode (D1)   need the scene::ISceneNode base destructor.
//   0x6d1650 calculateDistanceThresholds(bool)  not reconstructed.
//   0x6d176c render(void*)                    needs the render path of the base and the render buffer.
//   0x6d18bc, 0x6d1b5c constructors (C2, C1)  need the scene::ISceneNode base constructor and the STerrainData build.
//   0x6d1e34 getHeight(float, float) const    needs the patch height grid and the render buffer.

}  // namespace glitch::scene
