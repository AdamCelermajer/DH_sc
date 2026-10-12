// Wave 04 reconstruction. See include/glitch-scene-collada-gui/recon/v2/wave_04.hpp.
#include "glitch-scene-collada-gui/recon/v2/wave_04.hpp"

#include <algorithm>

namespace glitch::collada::recon_v2_04 {

// 0x60b918 _ZN6glitch7collada28CAnimationStreamingManager7releaseEPNS0_16CColladaDatabaseE
CAnimationStreamingManager::SegmentCacheEntry* release_0x60b918(CAnimationStreamingManager* self, CColladaDatabase* a2) {
    // The key is the record's fileName (CResFile +0x20). A null record gives a null key.
    CResFile* record = a2->record;
    const char* key = record ? record->fileName : nullptr;

    CAnimationStreamingManager::SegmentCacheEntry* pos = self->cacheBegin;
    CAnimationStreamingManager::SegmentCacheEntry* result = self->cacheEnd;
    while (pos != result) {
        if (pos->fileName == key) {
            self->cacheUsage -= static_cast<std::int32_t>(pos->size);
            // std::vector::_M_erase(pos): shift the tail down by one, shrink the end, return pos.
            CAnimationStreamingManager::SegmentCacheEntry* last = std::move(pos + 1, self->cacheEnd, pos);
            self->cacheEnd = last;
            result = self->cacheEnd;
        } else {
            pos += 1;
        }
    }
    return result;
}

// 0x60f25c _ZN6glitch7collada16CColladaDatabaseC2EPKcPNS0_15CColladaFactoryE
CColladaDatabase* ctor_0x60f25c(CColladaDatabase* self, const char* a2, CColladaFactory* a3) {
    CResFile* v5 = resFileManagerLoadName(CResFileManager::Inst, a2, 0, 0);
    self->factory = a3;
    self->record = v5;
    if (v5) {
        ++v5->refCount;
    }
    return self;
}

// 0x60f2b0 (same body as 0x60f25c)
CColladaDatabase* ctor_0x60f2b0(CColladaDatabase* self, const char* a2, CColladaFactory* a3) {
    CResFile* v5 = resFileManagerLoadName(CResFileManager::Inst, a2, 0, 0);
    self->factory = a3;
    self->record = v5;
    if (v5) {
        ++v5->refCount;
    }
    return self;
}

// 0x60f304 (IReadFile, bool, factory)
CColladaDatabase* ctor_0x60f304(CColladaDatabase* self, glitch::io::IReadFile* a2, bool a3, CColladaFactory* a4) {
    CResFile* v6 = resFileManagerLoadFile(CResFileManager::Inst, a2, 0, 0, a3);
    self->factory = a4;
    self->record = v6;
    if (v6) {
        ++v6->refCount;
    }
    return self;
}

// 0x60f364 (same body as 0x60f304)
CColladaDatabase* ctor_0x60f364(CColladaDatabase* self, glitch::io::IReadFile* a2, bool a3, CColladaFactory* a4) {
    CResFile* v6 = resFileManagerLoadFile(CResFileManager::Inst, a2, 0, 0, a3);
    self->factory = a4;
    self->record = v6;
    if (v6) {
        ++v6->refCount;
    }
    return self;
}

// 0x60f3c4 (IReadFile, factory; load flag is 0)
CColladaDatabase* ctor_0x60f3c4(CColladaDatabase* self, glitch::io::IReadFile* a2, CColladaFactory* a3) {
    CResFile* v5 = resFileManagerLoadFile(CResFileManager::Inst, a2, 0, 0, false);
    self->factory = a3;
    self->record = v5;
    if (v5) {
        ++v5->refCount;
    }
    return self;
}

// 0x60f42c (same body as 0x60f3c4)
CColladaDatabase* ctor_0x60f42c(CColladaDatabase* self, glitch::io::IReadFile* a2, CColladaFactory* a3) {
    CResFile* v5 = resFileManagerLoadFile(CResFileManager::Inst, a2, 0, 0, false);
    self->factory = a3;
    self->record = v5;
    if (v5) {
        ++v5->refCount;
    }
    return self;
}

}  // namespace glitch::collada::recon_v2_04
