#include "glitch-video/material.hpp"

namespace glitch::video {

// 0x35364c _ZNK6glitch5video9CMaterial11getHashCodeEv
// Partial: updateHashCode and getTechnique are declared in material.hpp but not reconstructed.
uint32_t CMaterial::getHashCode() const {
    const int32_t technique = getTechnique();
    if (((m_dirtyHashes >> technique) & 1u) != 0) {
        updateHashCode(static_cast<uint8_t>(technique));
    }
    return m_hashCodes[technique];
}

// 0x3537b0 _ZNK6glitch5video9CMaterialeqERKS1_
// Partial: depends on equals (0x353684), which is not reconstructed yet.
bool CMaterial::operator==(const CMaterial& other) const {
    const int32_t technique = getTechnique();
    const int32_t otherTechnique = other.getTechnique();
    return equals(static_cast<uint8_t>(technique), other, static_cast<uint8_t>(otherTechnique));
}

// 0x3537e4 _ZNK6glitch5video9CMaterialltERKS1_
// Partial: depends on compare (0x5ca3fc) and the renderer technique table (see material.hpp).
bool CMaterial::operator<(const CMaterial& other) const {
    const int32_t technique = getTechnique();
    const int32_t otherTechnique = other.getTechnique();

    if (((m_dirtyHashes >> technique) & 1u) != 0) {
        updateHashCode(static_cast<uint8_t>(technique));
    }
    const uint32_t hash = m_hashCodes[technique];

    if (((other.m_dirtyHashes >> otherTechnique) & 1u) != 0) {
        other.updateHashCode(static_cast<uint8_t>(otherTechnique));
    }
    const uint32_t otherHash = other.m_hashCodes[otherTechnique];

    bool greaterOrEqual = hash >= otherHash;
    if (hash == otherHash) {
        const uint8_t count = getTechniqueParameterCount(technique);
        const uint8_t otherCount = other.getTechniqueParameterCount(otherTechnique);
        greaterOrEqual = count >= otherCount;
        if (count == otherCount) {
            return compare(static_cast<uint8_t>(technique), count, other, static_cast<uint8_t>(otherTechnique));
        }
    }
    return !greaterOrEqual;
}

// 0x5aa4a8 _ZN6glitch5video9CMaterial22unsetParametersDirtyEh
// Partial: the parameter hash refresh runs only when the technique has exactly one parameter
// block (count byte == 1). The return value is not reproduced (see material.hpp).
void CMaterial::unsetParametersDirty(uint8_t parameterIndex) {
    const uint32_t bit = 1u << parameterIndex;
    if ((m_dirtyParameters & bit) == 0) {
        return;
    }
    m_dirtyParameters &= ~bit;

    if ((m_dirtyHashes & bit) != 0) {
        const int32_t technique = getTechnique();
        if (getTechniqueParameterCount(technique) == 1) {
            updateParametersHashCode(parameterIndex);
        }
    }
}

// 0x5c5d7c _ZNK6glitch5video9CMaterial23getRenderStateBlockSizeERKNS0_17CMaterialRendererE
uint32_t CMaterial::getRenderStateBlockSize(const CMaterialRenderer&) {
    return 0;
}

// 0x5c5d84 _ZN6glitch5video9CMaterial22setRenderStateInternalEhhRKNS0_6detail8material12SRenderStateE
void CMaterial::setRenderStateInternal(uint8_t, uint8_t, const detail::material::SRenderState&) {}

// Partial stubs for the CMaterial callees named by the hash and compare code above. Their bodies
// (0x5c5fd8, 0x5c5d88 and the technique tables) are not reconstructed yet, so these only let the
// library link. Ledger rows for the callers stay partial.
// The pass count is the byte at +4 of the renderer's 12-byte technique entry (0x5c5d34 caller, 0x5aa4a8).
uint8_t CMaterial::getTechniqueParameterCount(int32_t technique) const {
    return m_renderer ? m_renderer->m_techniques[technique].passCount : 0;
}
void CMaterial::updateHashCode(uint8_t) const {}
void CMaterial::updateParametersHashCode(uint8_t) const {}
void CMaterial::updateRenderStateHashCode(uint8_t) const {}
bool CMaterial::equals(uint8_t, const CMaterial&, uint8_t) const { return false; }
bool CMaterial::compare(uint8_t, uint8_t, const CMaterial&, uint8_t) const { return false; }
bool CMaterial::areParametersEqual(uint8_t, uint8_t, const CMaterial&, uint8_t) const { return false; }

}  // namespace glitch::video
