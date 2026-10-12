// Track B batch e1-glitch-scene-collada-gui-glitch_collada_IAnimationSetTemplate-0.
// Owner: glitch::collada::IAnimationSetTemplate. The class is declared as a stand-in in
// glitch-scene-collada-gui/recon/slice_22.hpp (namespace glitch::collada::recon_22); the members
// and the two methods below were added there for this batch.
//
// Reconstructed here: setUnAdded.
// Partial here: addChannels (missing pieces are listed above the function).

#include "glitch-scene-collada-gui/recon/slice_22.hpp"

namespace glitch::collada::recon_22 {

// 0x667320 glitch::collada::IAnimationSetTemplate::setUnAdded()
// For each target: clear the added flag (+0x00) and the record pointer (+0x0c). Returns this.
IAnimationSetTemplate& IAnimationSetTemplate::setUnAdded() {
    for (Target** it = targetsBegin; it != targetsEnd; ++it) {
        (*it)->added = 0;
        (*it)->record = nullptr;
    }
    return *this;
}

// 0x667384 glitch::collada::IAnimationSetTemplate::addChannels(vector<SChannel const*>*, vector<CAnimationTrackEx const*>*)
// PARTIAL. Reconstructed: the walk over the targets, the skip of targets already added, the
// record allocation, the kind copy, the record link, and the added flag.
// Missing:
//  - TargetRecord::value04: the original calls virtual slot +0x54 of target->source and stores the
//    result. The interface of source is not mapped, so value04 stays 0.
//  - Allocation: the original calls operator new(0x10, memory hint). The hint is the flag byte, which
//    is 0 on every processed target. The glitch allocator is not mapped, so host new is used.
//  - Kind switch: for kinds 0x01 to 0x0d the original calls CVirtualEx<...>::getInstance() for the
//    matching instantiation and discards the result. Those calls, which set up function-local
//    statics, are not made here. Not every instantiation is declared yet.
//  - The two vector arguments are never read by the pseudocode. They are unnamed here.
void IAnimationSetTemplate::addChannels(void*, void*) {
    for (Target** it = targetsBegin; it != targetsEnd; ++it) {
        Target* target = *it;
        if (target->added) {
            continue;
        }
        TargetRecord* record = new TargetRecord();  // +0x00..+0x0f, see the partial note above
        record->kind08 = target->kind;               // record +0x08 = target +0x04
        target->record = record;                     // target +0x0c
        // MISSING: record->value04 from target->source virtual slot +0x54; kind switch getInstance calls.
        target->added = 1;
    }
}

}  // namespace glitch::collada::recon_22
