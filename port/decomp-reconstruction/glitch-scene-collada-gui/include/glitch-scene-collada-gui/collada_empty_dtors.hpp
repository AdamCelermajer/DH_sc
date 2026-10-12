// Classes of the glitch::collada interfaces whose only reconstructed body is an
// empty base-object destructor (D1; the complete-object D2 is the same empty
// body when both are present). The original classes are polymorphic; these
// stand-ins carry no vtable, so the destructor is a plain defaulted member.
// The deleting destructors (D0) are compiler-generated and are not listed here.
#pragma once

namespace glitch::collada {

class IEventsManager {
public:
    // 0x60e028 _ZN6glitch7collada14IEventsManagerD1Ev (empty)
    ~IEventsManager() = default;
};

class CEventsManager {
public:
    // 0x60edd0 _ZN6glitch7collada14CEventsManagerD1Ev (empty)
    ~CEventsManager() = default;
};

class IAnimationTrackEx {
public:
    // 0x60def4 _ZN6glitch7collada17IAnimationTrackExD1Ev (empty)
    ~IAnimationTrackEx() = default;
};

class IColladaSceneNodeSynchronizedAnimatorInterface {
public:
    // 0x660dac _ZN6glitch7collada46IColladaSceneNodeSynchronizedAnimatorInterfaceD1Ev (empty)
    ~IColladaSceneNodeSynchronizedAnimatorInterface() = default;
};

class CResFileReader {
public:
    // 0x657888 _ZN6glitch7collada14CResFileReaderD1Ev (empty)
    ~CResFileReader() = default;
};

class CResFactory {
public:
    // 0x65788c _ZN6glitch7collada11CResFactoryD2Ev (empty, same as D1)
    // 0x657890 _ZN6glitch7collada11CResFactoryD1Ev (empty)
    ~CResFactory() = default;
};

class CAnimationTrackEx {
public:
    // 0x6e29b0 _ZN6glitch7collada17CAnimationTrackExD1Ev (empty)
    ~CAnimationTrackEx() = default;
};

namespace animation_track {

class CApplicatorInfo {
public:
    // 0x667c34 _ZN6glitch7collada15animation_track15CApplicatorInfoD1Ev (empty)
    ~CApplicatorInfo() = default;
};

class CMaterialApplicatorInfo {
public:
    // 0x667ccc _ZN6glitch7collada15animation_track23CMaterialApplicatorInfoD1Ev (empty)
    ~CMaterialApplicatorInfo() = default;
};

}  // namespace animation_track

}  // namespace glitch::collada
