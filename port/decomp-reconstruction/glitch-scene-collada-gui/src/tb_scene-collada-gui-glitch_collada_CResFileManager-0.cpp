// Batch e1-glitch-scene-collada-gui-glitch_collada_CResFileManager-0.
// Owner class: glitch::collada::CResFileManager (declared in layouts/collada_db.hpp).
//
// Partial. postLoadProcess (13,311 bytes of Hex-Rays pseudocode) runs these
// stages in original order. Only stage A is reconstructed. The others use
// holder or record fields that layouts/collada_db.hpp does not name (or names
// at the wrong width), or callees outside this category, so the body stops
// there and the ledger row is `partial`.
//
//   A. [reconstructed] Record reference: if file->refCount is non-zero, bump it.
//   B. [partial] Local CColladaDatabase view (record = file, factory =
//      &CColladaDatabase::DefaultFactory). Its destructor (stage N) calls
//      db_release, which needs the streaming manager and res_unload.
//   C. [partial] Holder word +0x10 non-zero: call
//      updateExternalResources(this, file, reader) and jump to N. The flag is
//      not named in layouts/collada_db.hpp (it sits in SDatabaseTables pad).
//   D. [partial] Version check: getVersion() must equal "0,0,0,777". The layout
//      holds the version as a 32-bit word at holder +0x00, but the original
//      reads it as a char pointer. On mismatch the original logs through
//      glitch::os::Printer (not on this category's include path). Then the
//      holder's +0x04 string is set to null when file +0x4c is clear, or to a
//      new string built from the reader's name (reader vtable +0x2c) when set.
//   E. [partial] Animation-track pass: for each animation (holder count +0x24,
//      stride 0x20) the word at entry +0x14 = getAnimationTrackEx(entry). The
//      callee (0x611ae0) is `skipped`.
//   F. [partial] Take a reference on the device file system (device +0x34), and
//      build the resource path string from file +0x0c through its vtable slot
//      +0x38. Both need the file-system interface, which is not mapped.
//   G. [partial] Image load pass: for each image (holder count +0x4c, stride
//      0x14) with entry +0x0c null, call the resource factory (this +0x24)
//      through its vtable slot +0x08 and store the result at entry +0x10 with
//      intrusive-count handling. The logger level is set to 3 around the call.
//      The factory signature is not mapped.
//   H. [partial] Material pass (holder count +0x5c, stride 0x24). Entry +0x18
//      is an effect index: above the effect count it becomes null; -1 loads the
//      effect from the external file named at +0x08 (getEffect of +0x0c + 1,
//      or null with a log line when the file is missing); otherwise it becomes
//      effect base + 116 * index. Each entry's sub-table (+0x10 count, +0x14
//      base, stride 0x18) gets an image fix-up: type word 11..14 at +0x08
//      turns an image index at +0x14 into null (-1) or images base + 20 * index
//      (index below the image count at +0x4c).
//   I. [partial] Effect pass (holder count +0x54, stride 0x74). Four sub-tables
//      (count and base at +0x10/+0x14, +0x28/+0x2c, +0x44/+0x48, +0x60/+0x64,
//      each stride 0x18) get the same image fix-up as H. The sentinel checks
//      against -8, -32, -60 and -88 are pointer-width artifacts and are not
//      reproduced.
//   J. [partial] Controller geometry pass (holder count +0x70, stride 0x0c): for
//      each controller whose type word is 1, the target list (controller +0x08
//      pointer; count +0x10, array +0x14) takes getGeometry(index) for each
//      index not above the geometry count (holder +0x68). Controller target
//      list layout is not mapped.
//   K. [partial] Emitter pass (holder count +0x78): for each emitter whose kind
//      word at +0x50 is 2, the GNPS block at +0x54 gets its second word set from
//      getNode(name) +0x44 +0x04, where name is the block's first word skipping
//      one leading character. getNode is not in this category, and the block
//      layout is not mapped.
//   L. [partial] Holder +0x10 set to 1 (the flag that stage C tests).
//   M. [partial] Drop the file-system reference taken in stage F.
//   N. [partial] Destroy the local CColladaDatabase view (stage B).
//
// Layout requests for the integrator (layouts/collada_db.hpp):
//   - SDatabaseTables: name the holder word at +0x04 (host-width string
//     pointer) and the flag at +0x10 (stages C and L).
//   - SDatabaseTables::version: host-width const char* (stage D).
//   - CResFile: name the flag at +0x4c (stage D).
//   - Sub-layouts for the material (H) and effect (I) sub-tables, and for the
//     controller target list (J) and the GNPS emitter block (K).
//   - Map the glitch::os::Printer logging interface into this category, or add
//     a deps stand-in, for the log lines in stages D and H.

#include "glitch-scene-collada-gui/layouts/collada_db.hpp"

namespace glitch {
namespace collada {

// 0x657994 _ZN6glitch7collada15CResFileManager12checkVersionEPNS_2io9IReadFileE
// The original body is a constant: it returns 0.
std::uint32_t CResFileManager::checkVersion(glitch::io::IReadFile*) {
    return 0;
}

// 0x658564 _ZN6glitch7collada15CResFileManager23updateExternalResourcesEPNS0_8CResFileEPNS_2io9IReadFileE. PARTIAL: resolves the external
// resources once (SDatabaseTables flag); the body is empty, as the recon/slice_32 free stand-in was.
void CResFileManager::updateExternalResources(CResFile*, glitch::io::IReadFile*) {}

// 0x658c90 _ZN6glitch7collada15CResFileManager15postLoadProcessEPNS0_8CResFileEPNS_2io9IReadFileE
int CResFileManager::postLoadProcess(CResFile* file, glitch::io::IReadFile* reader) {
    // Stage A. The original reads refCount at +0x04 and writes it back only when
    // it was non-zero.
    if (file->refCount != 0) {
        ++file->refCount;
    }

    // Stages B to N are not reconstructed (see the list above). `reader` is used
    // only by stages C and D, so it is unused here.
    (void)reader;

    // Partial: the original returns 0 on every path, including the early exit
    // through stage N.
    return 0;
}

}  // namespace collada
}  // namespace glitch
