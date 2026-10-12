#include "glitch-scene-collada-gui/animation_set.hpp"

namespace glitch::collada {

namespace {
template <typename T>
std::uint32_t rangeAt(const T* begin, const T* end, unsigned index) {
    // Original: index < (end - begin) / 4 ? begin[index] : 0
    return index < static_cast<unsigned>(end - begin) ? static_cast<std::uint32_t>(begin[index]) : 0u;
}
}  // namespace

// 0x65f004 _ZN6glitch7collada13CAnimationSet12remAnimationEPKNS0_10SAnimationE
int remAnimation(CAnimationSet* /*self*/, const void* /*animation*/) {
    return 0;
}

// 0x65f00c _ZN6glitch7collada13CAnimationSet10getChannelEj
std::uint32_t getChannel(CAnimationSet* self, unsigned index) {
    return self->channels[index];
}

// 0x65f018 _ZNK6glitch7collada13CAnimationSet10getChannelEj
std::uint32_t getChannel(const CAnimationSet* self, unsigned index) {
    return self->channels[index];
}

// 0x65f038 _ZN6glitch7collada13CAnimationSet19getAnimationTrackExEi
std::uint32_t getAnimationTrackEx(const CAnimationSet* self, int index) {
    return self->tracks[index];
}

// 0x65f058 _ZNK6glitch7collada13CAnimationSet21getAnimatedTrackCountEv
std::uint32_t getAnimatedTrackCount(const CAnimationSet* self) {
    return self->animatedTrackCount;
}

// 0x65f060 _ZNK6glitch7collada13CAnimationSet18getAnimationLengthEj
std::uint32_t getAnimationLength(const CAnimationSet* self, unsigned index) {
    return rangeAt(self->lengths_begin, self->lengths_end, index);
}

// 0x65f07c _ZNK6glitch7collada13CAnimationSet17getAnimationStartEj
std::uint32_t getAnimationStart(const CAnimationSet* self, unsigned index) {
    return rangeAt(self->starts_begin, self->starts_end, index);
}

// 0x65f098 _ZNK6glitch7collada13CAnimationSet15getAnimationEndEj
std::uint32_t getAnimationEnd(const CAnimationSet* self, unsigned index) {
    return rangeAt(self->ends_begin, self->ends_end, index);
}

// 0x65f0b4 _ZN6glitch7collada13CAnimationSet11getDatabaseEi
SAnimationDatabaseRef* getDatabase(CAnimationSet* self, int index) {
    return self->databases + index;
}

// 0x65f0c0 _ZNK6glitch7collada13CAnimationSet10getBindingEi
const SAnimationBinding* getBinding(const CAnimationSet* self, int index) {
    return self->bindings + index;
}

// 0x65f0d0 _ZN6glitch7collada13CAnimationSet19remAnimationLibraryERNS0_16CColladaDatabaseE
void remAnimationLibrary(CAnimationSet* /*self*/, CColladaDatabase& /*database*/) {}

// 0x65f0d4 _ZN6glitch7collada13CAnimationSet19remAnimationLibraryEi
void remAnimationLibrary(CAnimationSet* /*self*/, int /*index*/) {}

}  // namespace glitch::collada
