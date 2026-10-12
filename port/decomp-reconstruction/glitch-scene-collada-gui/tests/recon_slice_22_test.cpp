// Slice 22 test: the material applicator clone copies the two words it owns.
#include "glitch-scene-collada-gui/recon/slice_22.hpp"

int main() {
    using glitch::collada::animation_track::CMaterialApplicatorInfo;
    CMaterialApplicatorInfo source{};
    source.word_0x04 = 7;
    source.word_0x08 = 9;

    CMaterialApplicatorInfo* copy = glitch::collada::recon_22::cloneMaterialApplicatorInfo(&source);
    int failures = 0;
    if (copy == nullptr || copy->word_0x04 != 7 || copy->word_0x08 != 9) {
        failures++;
    }
    delete copy;
    return failures == 0 ? 0 : 1;
}
