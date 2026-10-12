// Track B batch e1-glitch-core-io-boost_intrusive_ptr_glitch_video_CVertexStreams_-0.
//
// The port instantiates the class template destructor from glitch-core-io/boost/intrusive_ptr.hpp
// for boost::intrusive_ptr<const glitch::video::CVertexStreams>. That template releases through
// intrusive_ptr_release (deps/glitch_core_objects.hpp), which deletes the object. The original
// (0x35eb90) instead destroys the object and then calls CustomFree on it. The helper below is the
// original sequence for this one specialization, written out so it can be checked against the
// listing. It is not wired into the template, so the ledger row stays partial.

#include "glitch-core-io/boost/intrusive_ptr.hpp"
#include "glitch-core-io/deps/glitch_core_objects.hpp"

// 0x310440 (core-platform memory.hpp). Not in glitch-core-io's include set; declared here only.
void CustomFree(void* p);

// 0x35eb90 boost::intrusive_ptr<glitch::video::CVertexStreams const>::~intrusive_ptr()
boost::intrusive_ptr<const glitch::video::CVertexStreams>* release_intrusive_ptr_CVertexStreams_const(
    boost::intrusive_ptr<const glitch::video::CVertexStreams>* self) {
    // +0x00 of self: the stored pointer (LDR R4, [R0]).
    const glitch::video::CVertexStreams* v1 = self->px;
    if (v1 != nullptr) {
        // +0x00 of the object: intrusive count. The original stores count - 1 and tests the old
        // value against 1 (LDR/SUB/STR/BNE). The post-decrement below is the same test.
        if (v1->referenceCounter-- == 1) {
            // BL ~CVertexStreams() on the object, then BL CustomFree(object).
            v1->~CVertexStreams();
            CustomFree(const_cast<glitch::video::CVertexStreams*>(v1));
        }
    }
    return self;  // MOV R0, R5 (the this pointer is returned)
}
