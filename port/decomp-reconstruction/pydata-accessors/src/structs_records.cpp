// Structs::X destructors, finalize and read.
// HAND-MAINTAINED SOURCE (Track B decision 7). First produced by gen_structs.py, which is retired
// (Ghidra-era) and must not be rerun; edit this file directly.
#include "pydata-accessors/structs_records.hpp"

namespace Structs {

// 0x4c56a0 _ZN7Structs9AIFactionD2Ev
// 0x4c56a4 _ZN7Structs9AIFactionD1Ev
AIFaction::~AIFaction() {
}

// 0x4ceae4 _ZN7Structs9AIFactionD0Ev
void DeletingDestructor(AIFaction* self) {
    self->AIFaction::~AIFaction();
    CustomFree(self);
}

// 0x4c56a8 _ZN7Structs9AIFaction8finalizeEv
void AIFaction::finalize() {
}

// 0x4eb408 _ZN7Structs9AIFaction4readEP11IStreamBase
void AIFaction::read(IStreamBase* stream) {
    m_w1 = StreamReader::readAs<std::int32_t>(stream);  // +0x4
    m_w2 = StreamReader::readAs<std::int32_t>(stream);  // +0x8
}

// 0x4db85c _ZN7Structs7AIPropsD2Ev
// 0x4db800 _ZN7Structs7AIPropsD1Ev
AIProps::~AIProps() {
    if (m_w11 != nullptr) CustomFree(m_w11);  // +0x2c
}

// 0x4db840 _ZN7Structs7AIPropsD0Ev
void DeletingDestructor(AIProps* self) {
    self->AIProps::~AIProps();
    CustomFree(self);
}

// 0x4db7d8 _ZN7Structs7AIProps8finalizeEv
void AIProps::finalize() {
    if (m_w11 != nullptr) {
        CustomFree(m_w11);  // +0x2c
        m_w10 = 0;  // +0x28
        m_w11 = nullptr;
    }
}

// 0x4c6990 _ZN7Structs10SimpleAnimD2Ev
// 0x4c6994 _ZN7Structs10SimpleAnimD1Ev
SimpleAnim::~SimpleAnim() {
}

// 0x4ce438 _ZN7Structs10SimpleAnimD0Ev
void DeletingDestructor(SimpleAnim* self) {
    self->SimpleAnim::~SimpleAnim();
    CustomFree(self);
}

// 0x4c6998 _ZN7Structs10SimpleAnim8finalizeEv
void SimpleAnim::finalize() {
}

// 0x4dbd6c _ZN7Structs10SimpleAnim4readEP11IStreamBase
void SimpleAnim::read(IStreamBase* stream) {
    StreamReader::readAs<std::int16_t>(stream, &m_w1);  // +0x4
    StreamReader::readAs<std::int16_t>(stream, &m_off6);  // +0x6
}

// 0x4c699c _ZN7Structs4AnimD2Ev
// 0x4c69d0 _ZN7Structs4AnimD1Ev
Anim::~Anim() {
}

// 0x4ce454 _ZN7Structs4AnimD0Ev
void DeletingDestructor(Anim* self) {
    self->Anim::~Anim();
    CustomFree(self);
}

// 0x4c6a04 _ZN7Structs4Anim8finalizeEv
// thunk: b 0x4c6998, forwards to SimpleAnim::finalize
void Anim::finalize() {
    SimpleAnim::finalize();
}

// 0x4dbe3c _ZN7Structs4Anim4readEP11IStreamBase
void Anim::read(IStreamBase* stream) {
    SimpleAnim::read(stream);
    StreamReader::readAs<std::int16_t>(stream, &m_w2);  // +0x8
    StreamReader::readAs<std::int16_t>(stream, &m_off10);  // +0xa
}

// 0x4db614 _ZN7Structs8AnimStepD2Ev
// 0x4db5b8 _ZN7Structs8AnimStepD1Ev
AnimStep::~AnimStep() {
    if (m_w9 != nullptr) CustomFree(m_w9);  // +0x24
}

// 0x4db5f8 _ZN7Structs8AnimStepD0Ev
void DeletingDestructor(AnimStep* self) {
    self->AnimStep::~AnimStep();
    CustomFree(self);
}

// 0x4db590 _ZN7Structs8AnimStep8finalizeEv
void AnimStep::finalize() {
    if (m_w9 != nullptr) {
        CustomFree(m_w9);  // +0x24
        m_w8 = 0;  // +0x20
        m_w9 = nullptr;
    }
}

// 0x4c573c _ZN7Structs19CharacterPropertiesD2Ev
// 0x4c5740 _ZN7Structs19CharacterPropertiesD1Ev
CharacterProperties::~CharacterProperties() {
}

// 0x4ce4c4 _ZN7Structs19CharacterPropertiesD0Ev
void DeletingDestructor(CharacterProperties* self) {
    self->CharacterProperties::~CharacterProperties();
    CustomFree(self);
}

// 0x4c5744 _ZN7Structs19CharacterProperties8finalizeEv
void CharacterProperties::finalize() {
}

// 0x4f2750 _ZN7Structs19CharacterProperties4readEP11IStreamBase
void CharacterProperties::read(IStreamBase* stream) {
    m_w1 = StreamReader::readAs<std::int32_t>(stream);  // +0x4
    m_w2 = StreamReader::readAs<std::int32_t>(stream);  // +0x8
    m_w3 = StreamReader::readAs<std::int32_t>(stream);  // +0xc
    m_w4 = StreamReader::readAs<std::int32_t>(stream);  // +0x10
    m_w5 = StreamReader::readAs<std::int32_t>(stream);  // +0x14
    m_w6 = StreamReader::readAs<std::int32_t>(stream);  // +0x18
    m_w7 = StreamReader::readAs<std::int32_t>(stream);  // +0x1c
    m_w8 = StreamReader::readAs<std::int32_t>(stream);  // +0x20
    m_w9 = StreamReader::readAs<std::int32_t>(stream);  // +0x24
    m_w10 = StreamReader::readAs<std::int32_t>(stream);  // +0x28
    m_w11 = StreamReader::readAs<std::int32_t>(stream);  // +0x2c
    m_w12 = StreamReader::readAs<std::int32_t>(stream);  // +0x30
    m_w13 = StreamReader::readAs<std::int32_t>(stream);  // +0x34
    m_w14 = StreamReader::readAs<std::int32_t>(stream);  // +0x38
    m_w15 = StreamReader::readAs<std::int32_t>(stream);  // +0x3c
    m_w16 = StreamReader::readAs<std::int32_t>(stream);  // +0x40
    m_w17 = StreamReader::readAs<std::int32_t>(stream);  // +0x44
    m_w18 = StreamReader::readAs<std::int32_t>(stream);  // +0x48
    m_w19 = StreamReader::readAs<std::int32_t>(stream);  // +0x4c
    m_w20 = StreamReader::readAs<std::int32_t>(stream);  // +0x50
    m_w21 = StreamReader::readAs<std::int32_t>(stream);  // +0x54
    m_w22 = StreamReader::readAs<std::int32_t>(stream);  // +0x58
    m_w23 = StreamReader::readAs<std::int32_t>(stream);  // +0x5c
    m_w24 = StreamReader::readAs<std::int32_t>(stream);  // +0x60
    m_w25 = StreamReader::readAs<std::int32_t>(stream);  // +0x64
    m_w26 = StreamReader::readAs<std::int32_t>(stream);  // +0x68
    m_w27 = StreamReader::readAs<std::int32_t>(stream);  // +0x6c
    m_w28 = StreamReader::readAs<std::int32_t>(stream);  // +0x70
    m_w29 = StreamReader::readAs<std::int32_t>(stream);  // +0x74
    m_w30 = StreamReader::readAs<std::int32_t>(stream);  // +0x78
    m_w31 = StreamReader::readAs<std::int32_t>(stream);  // +0x7c
    m_w32 = StreamReader::readAs<std::int32_t>(stream);  // +0x80
    m_w33 = StreamReader::readAs<std::int32_t>(stream);  // +0x84
    m_w34 = StreamReader::readAs<std::int32_t>(stream);  // +0x88
    m_w35 = StreamReader::readAs<std::int32_t>(stream);  // +0x8c
    m_w36 = StreamReader::readAs<std::int32_t>(stream);  // +0x90
    m_w37 = StreamReader::readAs<std::int32_t>(stream);  // +0x94
    m_w38 = StreamReader::readAs<std::int32_t>(stream);  // +0x98
    m_w39 = StreamReader::readAs<std::int32_t>(stream);  // +0x9c
    m_w40 = StreamReader::readAs<std::int32_t>(stream);  // +0xa0
    m_w41 = StreamReader::readAs<std::int32_t>(stream);  // +0xa4
    m_w42 = StreamReader::readAs<std::int32_t>(stream);  // +0xa8
    m_w43 = StreamReader::readAs<std::int32_t>(stream);  // +0xac
    m_w44 = StreamReader::readAs<std::int32_t>(stream);  // +0xb0
    m_w45 = StreamReader::readAs<std::int32_t>(stream);  // +0xb4
    m_w46 = StreamReader::readAs<std::int32_t>(stream);  // +0xb8
    m_w47 = StreamReader::readAs<std::int32_t>(stream);  // +0xbc
    m_w48 = StreamReader::readAs<std::int32_t>(stream);  // +0xc0
    m_w49 = StreamReader::readAs<std::int32_t>(stream);  // +0xc4
    m_w50 = StreamReader::readAs<std::int32_t>(stream);  // +0xc8
    m_w51 = StreamReader::readAs<std::int32_t>(stream);  // +0xcc
    m_w52 = StreamReader::readAs<std::int32_t>(stream);  // +0xd0
    m_w53 = StreamReader::readAs<std::int32_t>(stream);  // +0xd4
    m_w54 = StreamReader::readAs<std::int32_t>(stream);  // +0xd8
    m_w55 = StreamReader::readAs<std::int32_t>(stream);  // +0xdc
    m_w56 = StreamReader::readAs<std::int32_t>(stream);  // +0xe0
    m_w57 = StreamReader::readAs<std::int32_t>(stream);  // +0xe4
    m_w58 = StreamReader::readAs<std::int32_t>(stream);  // +0xe8
    m_w59 = StreamReader::readAs<std::int32_t>(stream);  // +0xec
    m_w60 = StreamReader::readAs<std::int32_t>(stream);  // +0xf0
    m_w61 = StreamReader::readAs<std::int32_t>(stream);  // +0xf4
    m_w62 = StreamReader::readAs<std::int32_t>(stream);  // +0xf8
    m_w63 = StreamReader::readAs<std::int32_t>(stream);  // +0xfc
    m_w64 = StreamReader::readAs<std::int32_t>(stream);  // +0x100
    m_w65 = StreamReader::readAs<std::int32_t>(stream);  // +0x104
    m_w66 = StreamReader::readAs<std::int32_t>(stream);  // +0x108
    m_w67 = StreamReader::readAs<std::int32_t>(stream);  // +0x10c
    m_w68 = StreamReader::readAs<std::int32_t>(stream);  // +0x110
    m_w69 = StreamReader::readAs<std::int32_t>(stream);  // +0x114
    m_w70 = StreamReader::readAs<std::int32_t>(stream);  // +0x118
    m_w71 = StreamReader::readAs<std::int32_t>(stream);  // +0x11c
    m_w72 = StreamReader::readAs<std::int32_t>(stream);  // +0x120
    m_w73 = StreamReader::readAs<std::int32_t>(stream);  // +0x124
    m_w74 = StreamReader::readAs<std::int32_t>(stream);  // +0x128
    m_w75 = StreamReader::readAs<std::int32_t>(stream);  // +0x12c
    m_w76 = StreamReader::readAs<std::int32_t>(stream);  // +0x130
    m_w77 = StreamReader::readAs<std::int32_t>(stream);  // +0x134
    m_w78 = StreamReader::readAs<std::int32_t>(stream);  // +0x138
    m_w79 = StreamReader::readAs<std::int32_t>(stream);  // +0x13c
    m_w80 = StreamReader::readAs<std::int32_t>(stream);  // +0x140
    m_w81 = StreamReader::readAs<std::int32_t>(stream);  // +0x144
    m_w82 = StreamReader::readAs<std::int32_t>(stream);  // +0x148
    m_w83 = StreamReader::readAs<std::int32_t>(stream);  // +0x14c
    m_w84 = StreamReader::readAs<std::int32_t>(stream);  // +0x150
    m_w85 = StreamReader::readAs<std::int32_t>(stream);  // +0x154
    m_w86 = StreamReader::readAs<std::int32_t>(stream);  // +0x158
    m_w87 = StreamReader::readAs<std::int32_t>(stream);  // +0x15c
    m_w88 = StreamReader::readAs<std::int32_t>(stream);  // +0x160
    m_w89 = StreamReader::readAs<std::int32_t>(stream);  // +0x164
    m_w90 = StreamReader::readAs<std::int32_t>(stream);  // +0x168
    m_w91 = StreamReader::readAs<std::int32_t>(stream);  // +0x16c
    m_w92 = StreamReader::readAs<std::int32_t>(stream);  // +0x170
    m_w93 = StreamReader::readAs<std::int32_t>(stream);  // +0x174
    m_w94 = StreamReader::readAs<std::int32_t>(stream);  // +0x178
    m_w95 = StreamReader::readAs<std::int32_t>(stream);  // +0x17c
    m_w96 = StreamReader::readAs<std::int32_t>(stream);  // +0x180
    m_w97 = StreamReader::readAs<std::int32_t>(stream);  // +0x184
    m_w98 = StreamReader::readAs<std::int32_t>(stream);  // +0x188
    m_w99 = StreamReader::readAs<std::int32_t>(stream);  // +0x18c
    m_w100 = StreamReader::readAs<std::int32_t>(stream);  // +0x190
    m_w101 = StreamReader::readAs<std::int32_t>(stream);  // +0x194
    m_w102 = StreamReader::readAs<std::int32_t>(stream);  // +0x198
    m_w103 = StreamReader::readAs<std::int32_t>(stream);  // +0x19c
    m_w104 = StreamReader::readAs<std::int32_t>(stream);  // +0x1a0
    m_w105 = StreamReader::readAs<std::int32_t>(stream);  // +0x1a4
    m_w106 = StreamReader::readAs<std::int32_t>(stream);  // +0x1a8
    m_w107 = StreamReader::readAs<std::int32_t>(stream);  // +0x1ac
    m_w108 = StreamReader::readAs<std::int32_t>(stream);  // +0x1b0
    m_w109 = StreamReader::readAs<std::int32_t>(stream);  // +0x1b4
    m_w110 = StreamReader::readAs<std::int32_t>(stream);  // +0x1b8
    m_w111 = StreamReader::readAs<std::int32_t>(stream);  // +0x1bc
    m_w112 = StreamReader::readAs<std::int32_t>(stream);  // +0x1c0
    m_w113 = StreamReader::readAs<std::int32_t>(stream);  // +0x1c4
    m_w114 = StreamReader::readAs<std::int32_t>(stream);  // +0x1c8
    m_w115 = StreamReader::readAs<std::int32_t>(stream);  // +0x1cc
    m_w116 = StreamReader::readAs<std::int32_t>(stream);  // +0x1d0
    m_w117 = StreamReader::readAs<std::int32_t>(stream);  // +0x1d4
    m_w118 = StreamReader::readAs<std::int32_t>(stream);  // +0x1d8
    m_w119 = StreamReader::readAs<std::int32_t>(stream);  // +0x1dc
    m_w120 = StreamReader::readAs<std::int32_t>(stream);  // +0x1e0
    m_w121 = StreamReader::readAs<std::int32_t>(stream);  // +0x1e4
    m_w122 = StreamReader::readAs<std::int32_t>(stream);  // +0x1e8
    m_w123 = StreamReader::readAs<std::int32_t>(stream);  // +0x1ec
    m_w124 = StreamReader::readAs<std::int32_t>(stream);  // +0x1f0
    m_w125 = StreamReader::readAs<std::int32_t>(stream);  // +0x1f4
    m_w126 = StreamReader::readAs<std::int32_t>(stream);  // +0x1f8
    m_w127 = StreamReader::readAs<std::int32_t>(stream);  // +0x1fc
    m_w128 = StreamReader::readAs<std::int32_t>(stream);  // +0x200
    m_w129 = StreamReader::readAs<std::int32_t>(stream);  // +0x204
    m_w130 = StreamReader::readAs<std::int32_t>(stream);  // +0x208
    m_w131 = StreamReader::readAs<std::int32_t>(stream);  // +0x20c
    m_w132 = StreamReader::readAs<std::int32_t>(stream);  // +0x210
    m_w133 = StreamReader::readAs<std::int32_t>(stream);  // +0x214
    m_w134 = StreamReader::readAs<std::int32_t>(stream);  // +0x218
    m_w135 = StreamReader::readAs<std::int32_t>(stream);  // +0x21c
    m_w136 = StreamReader::readAs<std::int32_t>(stream);  // +0x220
    m_w137 = StreamReader::readAs<std::int32_t>(stream);  // +0x224
    m_w138 = StreamReader::readAs<std::int32_t>(stream);  // +0x228
    m_w139 = StreamReader::readAs<std::int32_t>(stream);  // +0x22c
    m_w140 = StreamReader::readAs<std::int32_t>(stream);  // +0x230
    m_w141 = StreamReader::readAs<std::int32_t>(stream);  // +0x234
    m_w142 = StreamReader::readAs<std::int32_t>(stream);  // +0x238
    m_w143 = StreamReader::readAs<std::int32_t>(stream);  // +0x23c
    m_w144 = StreamReader::readAs<std::int32_t>(stream);  // +0x240
    m_w145 = StreamReader::readAs<std::int32_t>(stream);  // +0x244
    m_w146 = StreamReader::readAs<std::int32_t>(stream);  // +0x248
    m_w147 = StreamReader::readAs<std::int32_t>(stream);  // +0x24c
    m_w148 = StreamReader::readAs<std::int32_t>(stream);  // +0x250
    m_w149 = StreamReader::readAs<std::int32_t>(stream);  // +0x254
    m_w150 = StreamReader::readAs<std::int32_t>(stream);  // +0x258
    m_w151 = StreamReader::readAs<std::int32_t>(stream);  // +0x25c
    m_w152 = StreamReader::readAs<std::int32_t>(stream);  // +0x260
    m_w153 = StreamReader::readAs<std::int32_t>(stream);  // +0x264
    m_w154 = StreamReader::readAs<std::int32_t>(stream);  // +0x268
    m_w155 = StreamReader::readAs<std::int32_t>(stream);  // +0x26c
    m_w156 = StreamReader::readAs<std::int32_t>(stream);  // +0x270
    m_w157 = StreamReader::readAs<std::int32_t>(stream);  // +0x274
    m_w158 = StreamReader::readAs<std::int32_t>(stream);  // +0x278
    m_w159 = StreamReader::readAs<std::int32_t>(stream);  // +0x27c
    m_w160 = StreamReader::readAs<std::int32_t>(stream);  // +0x280
    m_w161 = StreamReader::readAs<std::int32_t>(stream);  // +0x284
    m_w162 = StreamReader::readAs<std::int32_t>(stream);  // +0x288
    m_w163 = StreamReader::readAs<std::int32_t>(stream);  // +0x28c
    m_w164 = StreamReader::readAs<std::int32_t>(stream);  // +0x290
    m_w165 = StreamReader::readAs<std::int32_t>(stream);  // +0x294
    m_w166 = StreamReader::readAs<std::int32_t>(stream);  // +0x298
    m_w167 = StreamReader::readAs<std::int32_t>(stream);  // +0x29c
    m_w168 = StreamReader::readAs<std::int32_t>(stream);  // +0x2a0
    m_w169 = StreamReader::readAs<std::int32_t>(stream);  // +0x2a4
    m_w170 = StreamReader::readAs<std::int32_t>(stream);  // +0x2a8
    m_w171 = StreamReader::readAs<std::int32_t>(stream);  // +0x2ac
    m_w172 = StreamReader::readAs<std::int32_t>(stream);  // +0x2b0
    m_w173 = StreamReader::readAs<std::int32_t>(stream);  // +0x2b4
    m_w174 = StreamReader::readAs<std::int32_t>(stream);  // +0x2b8
    m_w175 = StreamReader::readAs<std::int32_t>(stream);  // +0x2bc
    m_w176 = StreamReader::readAs<std::int32_t>(stream);  // +0x2c0
    m_w177 = StreamReader::readAs<std::int32_t>(stream);  // +0x2c4
    m_w178 = StreamReader::readAs<std::int32_t>(stream);  // +0x2c8
    m_w179 = StreamReader::readAs<std::int32_t>(stream);  // +0x2cc
    m_w180 = StreamReader::readAs<std::int32_t>(stream);  // +0x2d0
    m_w181 = StreamReader::readAs<std::int32_t>(stream);  // +0x2d4
    m_w182 = StreamReader::readAs<std::int32_t>(stream);  // +0x2d8
    m_w183 = StreamReader::readAs<std::int32_t>(stream);  // +0x2dc
    m_w184 = StreamReader::readAs<std::int32_t>(stream);  // +0x2e0
    m_w185 = StreamReader::readAs<std::int32_t>(stream);  // +0x2e4
    m_w186 = StreamReader::readAs<std::int32_t>(stream);  // +0x2e8
    m_w187 = StreamReader::readAs<std::int32_t>(stream);  // +0x2ec
    m_w188 = StreamReader::readAs<std::int32_t>(stream);  // +0x2f0
    m_w189 = StreamReader::readAs<std::int32_t>(stream);  // +0x2f4
    m_w190 = StreamReader::readAs<std::int32_t>(stream);  // +0x2f8
    m_w191 = StreamReader::readAs<std::int32_t>(stream);  // +0x2fc
    m_w192 = StreamReader::readAs<std::int32_t>(stream);  // +0x300
    m_w193 = StreamReader::readAs<std::int32_t>(stream);  // +0x304
    m_w194 = StreamReader::readAs<std::int32_t>(stream);  // +0x308
    m_w195 = StreamReader::readAs<std::int32_t>(stream);  // +0x30c
    m_w196 = StreamReader::readAs<std::int32_t>(stream);  // +0x310
    m_w197 = StreamReader::readAs<std::int32_t>(stream);  // +0x314
    m_w198 = StreamReader::readAs<std::int32_t>(stream);  // +0x318
    m_w199 = StreamReader::readAs<std::int32_t>(stream);  // +0x31c
    m_w200 = StreamReader::readAs<std::int32_t>(stream);  // +0x320
    m_w201 = StreamReader::readAs<std::int32_t>(stream);  // +0x324
    m_w202 = StreamReader::readAs<std::int32_t>(stream);  // +0x328
    m_w203 = StreamReader::readAs<std::int32_t>(stream);  // +0x32c
    m_w204 = StreamReader::readAs<std::int32_t>(stream);  // +0x330
    m_w205 = StreamReader::readAs<std::int32_t>(stream);  // +0x334
    m_w206 = StreamReader::readAs<std::int32_t>(stream);  // +0x338
    m_w207 = StreamReader::readAs<std::int32_t>(stream);  // +0x33c
    m_w208 = StreamReader::readAs<std::int32_t>(stream);  // +0x340
    m_w209 = StreamReader::readAs<std::int32_t>(stream);  // +0x344
    m_w210 = StreamReader::readAs<std::int32_t>(stream);  // +0x348
    m_w211 = StreamReader::readAs<std::int32_t>(stream);  // +0x34c
    m_w212 = StreamReader::readAs<std::int32_t>(stream);  // +0x350
    m_w213 = StreamReader::readAs<std::int32_t>(stream);  // +0x354
    m_w214 = StreamReader::readAs<std::int32_t>(stream);  // +0x358
    m_w215 = StreamReader::readAs<std::int32_t>(stream);  // +0x35c
    m_w216 = StreamReader::readAs<std::int32_t>(stream);  // +0x360
    m_w217 = StreamReader::readAs<std::int32_t>(stream);  // +0x364
    m_w218 = StreamReader::readAs<std::int32_t>(stream);  // +0x368
    m_w219 = StreamReader::readAs<std::int32_t>(stream);  // +0x36c
    m_w220 = StreamReader::readAs<std::int32_t>(stream);  // +0x370
    m_w221 = StreamReader::readAs<std::int32_t>(stream);  // +0x374
    m_w222 = StreamReader::readAs<std::int32_t>(stream);  // +0x378
    m_w223 = StreamReader::readAs<std::int32_t>(stream);  // +0x37c
    m_w224 = StreamReader::readAs<std::int32_t>(stream);  // +0x380
}

// 0x4c645c _ZN7Structs18AssassinPropertiesD2Ev
// 0x4c6490 _ZN7Structs18AssassinPropertiesD1Ev
AssassinProperties::~AssassinProperties() {
}

// 0x4ce614 _ZN7Structs18AssassinPropertiesD0Ev
void DeletingDestructor(AssassinProperties* self) {
    self->AssassinProperties::~AssassinProperties();
    CustomFree(self);
}

// 0x4c64c4 _ZN7Structs18AssassinProperties8finalizeEv
// thunk: b 0x4c5744, forwards to CharacterProperties::finalize
void AssassinProperties::finalize() {
    CharacterProperties::finalize();
}

// 0x4f7a98 _ZN7Structs18AssassinProperties4readEP11IStreamBase
// thunk: b 0x4f2750, forwards to CharacterProperties::read
void AssassinProperties::read(IStreamBase* stream) {
    CharacterProperties::read(stream);
}

// 0x4c6828 _ZN7Structs27AttackingFollowerPropertiesD2Ev
// 0x4c685c _ZN7Structs27AttackingFollowerPropertiesD1Ev
AttackingFollowerProperties::~AttackingFollowerProperties() {
}

// 0x4ce518 _ZN7Structs27AttackingFollowerPropertiesD0Ev
void DeletingDestructor(AttackingFollowerProperties* self) {
    self->AttackingFollowerProperties::~AttackingFollowerProperties();
    CustomFree(self);
}

// 0x4c6890 _ZN7Structs27AttackingFollowerProperties8finalizeEv
// thunk: b 0x4c5744, forwards to CharacterProperties::finalize
void AttackingFollowerProperties::finalize() {
    CharacterProperties::finalize();
}

// 0x4f7a74 _ZN7Structs27AttackingFollowerProperties4readEP11IStreamBase
// thunk: b 0x4f2750, forwards to CharacterProperties::read
void AttackingFollowerProperties::read(IStreamBase* stream) {
    CharacterProperties::read(stream);
}

// 0x4c6bf4 _ZN7Structs14AttrBonusEntryD2Ev
// 0x4c6bf8 _ZN7Structs14AttrBonusEntryD1Ev
AttrBonusEntry::~AttrBonusEntry() {
}

// 0x4ce240 _ZN7Structs14AttrBonusEntryD0Ev
void DeletingDestructor(AttrBonusEntry* self) {
    self->AttrBonusEntry::~AttrBonusEntry();
    CustomFree(self);
}

// 0x4c6bfc _ZN7Structs14AttrBonusEntry8finalizeEv
void AttrBonusEntry::finalize() {
}

// 0x4fef00 _ZN7Structs14AttrBonusEntry4readEP11IStreamBase
void AttrBonusEntry::read(IStreamBase* stream) {
    m_w1 = StreamReader::readAs<std::int32_t>(stream);  // +0x4
    m_w2 = StreamReader::readAs<std::int32_t>(stream);  // +0x8
    m_w3 = StreamReader::readAs<std::int32_t>(stream);  // +0xc
}

// 0x4c5e74 _ZN7Structs16BanditPropertiesD2Ev
// 0x4c5ea8 _ZN7Structs16BanditPropertiesD1Ev
BanditProperties::~BanditProperties() {
}

// 0x4ce79c _ZN7Structs16BanditPropertiesD0Ev
void DeletingDestructor(BanditProperties* self) {
    self->BanditProperties::~BanditProperties();
    CustomFree(self);
}

// 0x4c5edc _ZN7Structs16BanditProperties8finalizeEv
// thunk: b 0x4c5744, forwards to CharacterProperties::finalize
void BanditProperties::finalize() {
    CharacterProperties::finalize();
}

// 0x4f7ad0 _ZN7Structs16BanditProperties4readEP11IStreamBase
// thunk: b 0x4f2750, forwards to CharacterProperties::read
void BanditProperties::read(IStreamBase* stream) {
    CharacterProperties::read(stream);
}

// 0x4c58f8 _ZN7Structs22BigLizardManPropertiesD2Ev
// 0x4c592c _ZN7Structs22BigLizardManPropertiesD1Ev
BigLizardManProperties::~BigLizardManProperties() {
}

// 0x4ce908 _ZN7Structs22BigLizardManPropertiesD0Ev
void DeletingDestructor(BigLizardManProperties* self) {
    self->BigLizardManProperties::~BigLizardManProperties();
    CustomFree(self);
}

// 0x4c5960 _ZN7Structs22BigLizardManProperties8finalizeEv
// thunk: b 0x4c5744, forwards to CharacterProperties::finalize
void BigLizardManProperties::finalize() {
    CharacterProperties::finalize();
}

// 0x4f7b04 _ZN7Structs22BigLizardManProperties4readEP11IStreamBase
// thunk: b 0x4f2750, forwards to CharacterProperties::read
void BigLizardManProperties::read(IStreamBase* stream) {
    CharacterProperties::read(stream);
}

// 0x4c5f4c _ZN7Structs19BigSpiderPropertiesD2Ev
// 0x4c5f80 _ZN7Structs19BigSpiderPropertiesD1Ev
BigSpiderProperties::~BigSpiderProperties() {
}

// 0x4ce764 _ZN7Structs19BigSpiderPropertiesD0Ev
void DeletingDestructor(BigSpiderProperties* self) {
    self->BigSpiderProperties::~BigSpiderProperties();
    CustomFree(self);
}

// 0x4c5fb4 _ZN7Structs19BigSpiderProperties8finalizeEv
// thunk: b 0x4c5744, forwards to CharacterProperties::finalize
void BigSpiderProperties::finalize() {
    CharacterProperties::finalize();
}

// 0x4f7ac8 _ZN7Structs19BigSpiderProperties4readEP11IStreamBase
// thunk: b 0x4f2750, forwards to CharacterProperties::read
void BigSpiderProperties::read(IStreamBase* stream) {
    CharacterProperties::read(stream);
}

// 0x4db3cc _ZN7Structs10CamAnimSetD2Ev
// 0x4db370 _ZN7Structs10CamAnimSetD1Ev
CamAnimSet::~CamAnimSet() {
    if (m_w2 != nullptr) CustomFree(m_w2);  // +0x8
}

// 0x4db3b0 _ZN7Structs10CamAnimSetD0Ev
void DeletingDestructor(CamAnimSet* self) {
    self->CamAnimSet::~CamAnimSet();
    CustomFree(self);
}

// 0x4db348 _ZN7Structs10CamAnimSet8finalizeEv
void CamAnimSet::finalize() {
    if (m_w2 != nullptr) {
        CustomFree(m_w2);  // +0x8
        m_w1 = 0;  // +0x4
        m_w2 = nullptr;
    }
}

// 0x4db2f8 _ZN7Structs8CharAnimD2Ev
// 0x4db28c _ZN7Structs8CharAnimD1Ev
CharAnim::~CharAnim() {
    if (m_w17 != nullptr) CustomFree(m_w17);  // +0x44
    if (m_w34 != nullptr) CustomFree(m_w34);  // +0x88
}

// 0x4db2dc _ZN7Structs8CharAnimD0Ev
void DeletingDestructor(CharAnim* self) {
    self->CharAnim::~CharAnim();
    CustomFree(self);
}

// 0x4db248 _ZN7Structs8CharAnim8finalizeEv
void CharAnim::finalize() {
    if (m_w17 != nullptr) {
        CustomFree(m_w17);  // +0x44
        m_w16 = 0;  // +0x40
        m_w17 = nullptr;
    }
    if (m_w34 != nullptr) {
        CustomFree(m_w34);  // +0x88
        m_w33 = 0;  // +0x84
        m_w34 = nullptr;
    }
}

// 0x4c6b88 _ZN7Structs10CharEffectD2Ev
// 0x4c6b8c _ZN7Structs10CharEffectD1Ev
CharEffect::~CharEffect() {
}

// 0x4ce33c _ZN7Structs10CharEffectD0Ev
void DeletingDestructor(CharEffect* self) {
    self->CharEffect::~CharEffect();
    CustomFree(self);
}

// 0x4c6b90 _ZN7Structs10CharEffect8finalizeEv
void CharEffect::finalize() {
}

// 0x4ed5a8 _ZN7Structs10CharEffect4readEP11IStreamBase
void CharEffect::read(IStreamBase* stream) {
    m_w1 = StreamReader::readAs<std::int32_t>(stream);  // +0x4
    m_w2 = StreamReader::readAs<std::int32_t>(stream);  // +0x8
    m_w3 = StreamReader::readAs<std::int32_t>(stream);  // +0xc
    m_w4 = StreamReader::readAs<std::int32_t>(stream);  // +0x10
    m_w5 = StreamReader::readAs<bool>(stream);  // +0x14
}

// 0x4c6978 _ZN7Structs12CharInfoNameD2Ev
// 0x4c697c _ZN7Structs12CharInfoNameD1Ev
CharInfoName::~CharInfoName() {
}

// 0x4ce48c _ZN7Structs12CharInfoNameD0Ev
void DeletingDestructor(CharInfoName* self) {
    self->CharInfoName::~CharInfoName();
    CustomFree(self);
}

// 0x4c6980 _ZN7Structs12CharInfoName8finalizeEv
void CharInfoName::finalize() {
}

// 0x4c56ac _ZN7Structs9ClassFuncD2Ev
// 0x4c56b0 _ZN7Structs9ClassFuncD1Ev
ClassFunc::~ClassFunc() {
}

// 0x4ceac8 _ZN7Structs9ClassFuncD0Ev
void DeletingDestructor(ClassFunc* self) {
    self->ClassFunc::~ClassFunc();
    CustomFree(self);
}

// 0x4c56b4 _ZN7Structs9ClassFunc8finalizeEv
void ClassFunc::finalize() {
}

// 0x4c570c _ZN7Structs16ClassFuncAddPropD2Ev
// 0x4c5710 _ZN7Structs16ClassFuncAddPropD1Ev
ClassFuncAddProp::~ClassFuncAddProp() {
}

// 0x4ce9e8 _ZN7Structs16ClassFuncAddPropD0Ev
void DeletingDestructor(ClassFuncAddProp* self) {
    self->ClassFuncAddProp::~ClassFuncAddProp();
    CustomFree(self);
}

// 0x4c5714 _ZN7Structs16ClassFuncAddProp8finalizeEv
void ClassFuncAddProp::finalize() {
}

// 0x4f0abc _ZN7Structs16ClassFuncAddProp4readEP11IStreamBase
void ClassFuncAddProp::read(IStreamBase* stream) {
    m_w1 = StreamReader::readAs<std::int32_t>(stream);  // +0x4
    m_w2 = StreamReader::readAs<std::int32_t>(stream);  // +0x8
    m_w3 = StreamReader::readAs<std::int32_t>(stream);  // +0xc
    m_w4 = StreamReader::readAs<std::int32_t>(stream);  // +0x10
    m_w5 = StreamReader::readAs<std::int32_t>(stream);  // +0x14
}

// 0x4c56dc _ZN7Structs17ClassFuncAddValueD2Ev
// 0x4c56e0 _ZN7Structs17ClassFuncAddValueD1Ev
ClassFuncAddValue::~ClassFuncAddValue() {
}

// 0x4cea58 _ZN7Structs17ClassFuncAddValueD0Ev
void DeletingDestructor(ClassFuncAddValue* self) {
    self->ClassFuncAddValue::~ClassFuncAddValue();
    CustomFree(self);
}

// 0x4c56e4 _ZN7Structs17ClassFuncAddValue8finalizeEv
void ClassFuncAddValue::finalize() {
}

// 0x4f124c _ZN7Structs17ClassFuncAddValue4readEP11IStreamBase
void ClassFuncAddValue::read(IStreamBase* stream) {
    m_w1 = StreamReader::readAs<std::int32_t>(stream);  // +0x4
    m_w2 = StreamReader::readAs<std::int32_t>(stream);  // +0x8
    m_w3 = StreamReader::readAs<std::int32_t>(stream);  // +0xc
    m_w4 = StreamReader::readAs<std::int32_t>(stream);  // +0x10
    m_w5 = StreamReader::readAs<std::int32_t>(stream);  // +0x14
}

// 0x4c5730 _ZN7Structs25ClassFuncApplyGroupOnlyIfD2Ev
// 0x4c5734 _ZN7Structs25ClassFuncApplyGroupOnlyIfD1Ev
ClassFuncApplyGroupOnlyIf::~ClassFuncApplyGroupOnlyIf() {
}

// 0x4ce994 _ZN7Structs25ClassFuncApplyGroupOnlyIfD0Ev
void DeletingDestructor(ClassFuncApplyGroupOnlyIf* self) {
    self->ClassFuncApplyGroupOnlyIf::~ClassFuncApplyGroupOnlyIf();
    CustomFree(self);
}

// 0x4c5738 _ZN7Structs25ClassFuncApplyGroupOnlyIf8finalizeEv
void ClassFuncApplyGroupOnlyIf::finalize() {
}

// 0x4f0510 _ZN7Structs25ClassFuncApplyGroupOnlyIf4readEP11IStreamBase
void ClassFuncApplyGroupOnlyIf::read(IStreamBase* stream) {
    m_w1 = StreamReader::readAs<std::int32_t>(stream);  // +0x4
    m_w2 = StreamReader::readAs<std::int32_t>(stream);  // +0x8
    m_w3 = StreamReader::readAs<std::int32_t>(stream);  // +0xc
    m_w4 = StreamReader::readAs<std::int32_t>(stream);  // +0x10
    m_w5 = StreamReader::readAs<std::int32_t>(stream);  // +0x14
}

// 0x4c56e8 _ZN7Structs14ClassFuncGroupD2Ev
// 0x4c56ec _ZN7Structs14ClassFuncGroupD1Ev
ClassFuncGroup::~ClassFuncGroup() {
}

// 0x4cea3c _ZN7Structs14ClassFuncGroupD0Ev
void DeletingDestructor(ClassFuncGroup* self) {
    self->ClassFuncGroup::~ClassFuncGroup();
    CustomFree(self);
}

// 0x4c56f0 _ZN7Structs14ClassFuncGroup8finalizeEv
void ClassFuncGroup::finalize() {
}

// 0x4f1068 _ZN7Structs14ClassFuncGroup4readEP11IStreamBase
void ClassFuncGroup::read(IStreamBase* stream) {
    m_w1 = StreamReader::readAs<std::int32_t>(stream);  // +0x4
    m_w2 = StreamReader::readAs<std::int32_t>(stream);  // +0x8
    m_w3 = StreamReader::readAs<std::int32_t>(stream);  // +0xc
    m_w4 = StreamReader::readAs<std::int32_t>(stream);  // +0x10
    m_w5 = StreamReader::readAs<std::int32_t>(stream);  // +0x14
}

// 0x4c56b8 _ZN7Structs15ClassFuncLinearD2Ev
// 0x4c56bc _ZN7Structs15ClassFuncLinearD1Ev
ClassFuncLinear::~ClassFuncLinear() {
}

// 0x4ceaac _ZN7Structs15ClassFuncLinearD0Ev
void DeletingDestructor(ClassFuncLinear* self) {
    self->ClassFuncLinear::~ClassFuncLinear();
    CustomFree(self);
}

// 0x4c56c0 _ZN7Structs15ClassFuncLinear8finalizeEv
void ClassFuncLinear::finalize() {
}

// 0x4f17f8 _ZN7Structs15ClassFuncLinear4readEP11IStreamBase
void ClassFuncLinear::read(IStreamBase* stream) {
    m_w1 = StreamReader::readAs<std::int32_t>(stream);  // +0x4
    m_w2 = StreamReader::readAs<std::int32_t>(stream);  // +0x8
    m_w3 = StreamReader::readAs<std::int32_t>(stream);  // +0xc
    m_w4 = StreamReader::readAs<std::int32_t>(stream);  // +0x10
    m_w5 = StreamReader::readAs<std::int32_t>(stream);  // +0x14
}

// 0x4c5700 _ZN7Structs20ClassFuncLinearLevelD2Ev
// 0x4c5704 _ZN7Structs20ClassFuncLinearLevelD1Ev
ClassFuncLinearLevel::~ClassFuncLinearLevel() {
}

// 0x4cea04 _ZN7Structs20ClassFuncLinearLevelD0Ev
void DeletingDestructor(ClassFuncLinearLevel* self) {
    self->ClassFuncLinearLevel::~ClassFuncLinearLevel();
    CustomFree(self);
}

// 0x4c5708 _ZN7Structs20ClassFuncLinearLevel8finalizeEv
void ClassFuncLinearLevel::finalize() {
}

// 0x4f0ca0 _ZN7Structs20ClassFuncLinearLevel4readEP11IStreamBase
void ClassFuncLinearLevel::read(IStreamBase* stream) {
    m_w1 = StreamReader::readAs<std::int32_t>(stream);  // +0x4
    m_w2 = StreamReader::readAs<std::int32_t>(stream);  // +0x8
    m_w3 = StreamReader::readAs<std::int32_t>(stream);  // +0xc
    m_w4 = StreamReader::readAs<std::int32_t>(stream);  // +0x10
    m_w5 = StreamReader::readAs<std::int32_t>(stream);  // +0x14
}

// 0x4c56f4 _ZN7Structs15ClassFuncMinMaxD2Ev
// 0x4c56f8 _ZN7Structs15ClassFuncMinMaxD1Ev
ClassFuncMinMax::~ClassFuncMinMax() {
}

// 0x4cea20 _ZN7Structs15ClassFuncMinMaxD0Ev
void DeletingDestructor(ClassFuncMinMax* self) {
    self->ClassFuncMinMax::~ClassFuncMinMax();
    CustomFree(self);
}

// 0x4c56fc _ZN7Structs15ClassFuncMinMax8finalizeEv
void ClassFuncMinMax::finalize() {
}

// 0x4f0e84 _ZN7Structs15ClassFuncMinMax4readEP11IStreamBase
void ClassFuncMinMax::read(IStreamBase* stream) {
    m_w1 = StreamReader::readAs<std::int32_t>(stream);  // +0x4
    m_w2 = StreamReader::readAs<std::int32_t>(stream);  // +0x8
    m_w3 = StreamReader::readAs<std::int32_t>(stream);  // +0xc
    m_w4 = StreamReader::readAs<std::int32_t>(stream);  // +0x10
    m_w5 = StreamReader::readAs<std::int32_t>(stream);  // +0x14
}

// 0x4c5718 _ZN7Structs28ClassFuncScaleWithPercentageD2Ev
// 0x4c571c _ZN7Structs28ClassFuncScaleWithPercentageD1Ev
ClassFuncScaleWithPercentage::~ClassFuncScaleWithPercentage() {
}

// 0x4ce9cc _ZN7Structs28ClassFuncScaleWithPercentageD0Ev
void DeletingDestructor(ClassFuncScaleWithPercentage* self) {
    self->ClassFuncScaleWithPercentage::~ClassFuncScaleWithPercentage();
    CustomFree(self);
}

// 0x4c5720 _ZN7Structs28ClassFuncScaleWithPercentage8finalizeEv
void ClassFuncScaleWithPercentage::finalize() {
}

// 0x4f08d8 _ZN7Structs28ClassFuncScaleWithPercentage4readEP11IStreamBase
void ClassFuncScaleWithPercentage::read(IStreamBase* stream) {
    m_w1 = StreamReader::readAs<std::int32_t>(stream);  // +0x4
    m_w2 = StreamReader::readAs<std::int32_t>(stream);  // +0x8
    m_w3 = StreamReader::readAs<std::int32_t>(stream);  // +0xc
    m_w4 = StreamReader::readAs<std::int32_t>(stream);  // +0x10
    m_w5 = StreamReader::readAs<std::int32_t>(stream);  // +0x14
}

// 0x4c5724 _ZN7Structs35ClassFuncScaleWithPercentageForBuffD2Ev
// 0x4c5728 _ZN7Structs35ClassFuncScaleWithPercentageForBuffD1Ev
ClassFuncScaleWithPercentageForBuff::~ClassFuncScaleWithPercentageForBuff() {
}

// 0x4ce9b0 _ZN7Structs35ClassFuncScaleWithPercentageForBuffD0Ev
void DeletingDestructor(ClassFuncScaleWithPercentageForBuff* self) {
    self->ClassFuncScaleWithPercentageForBuff::~ClassFuncScaleWithPercentageForBuff();
    CustomFree(self);
}

// 0x4c572c _ZN7Structs35ClassFuncScaleWithPercentageForBuff8finalizeEv
void ClassFuncScaleWithPercentageForBuff::finalize() {
}

// 0x4f06f4 _ZN7Structs35ClassFuncScaleWithPercentageForBuff4readEP11IStreamBase
void ClassFuncScaleWithPercentageForBuff::read(IStreamBase* stream) {
    m_w1 = StreamReader::readAs<std::int32_t>(stream);  // +0x4
    m_w2 = StreamReader::readAs<std::int32_t>(stream);  // +0x8
    m_w3 = StreamReader::readAs<std::int32_t>(stream);  // +0xc
    m_w4 = StreamReader::readAs<std::int32_t>(stream);  // +0x10
    m_w5 = StreamReader::readAs<std::int32_t>(stream);  // +0x14
}

// 0x4c56d0 _ZN7Structs15ClassFuncSetOIDD2Ev
// 0x4c56d4 _ZN7Structs15ClassFuncSetOIDD1Ev
ClassFuncSetOID::~ClassFuncSetOID() {
}

// 0x4cea74 _ZN7Structs15ClassFuncSetOIDD0Ev
void DeletingDestructor(ClassFuncSetOID* self) {
    self->ClassFuncSetOID::~ClassFuncSetOID();
    CustomFree(self);
}

// 0x4c56d8 _ZN7Structs15ClassFuncSetOID8finalizeEv
void ClassFuncSetOID::finalize() {
}

// 0x4f1430 _ZN7Structs15ClassFuncSetOID4readEP11IStreamBase
void ClassFuncSetOID::read(IStreamBase* stream) {
    m_w1 = StreamReader::readAs<std::int32_t>(stream);  // +0x4
    m_w2 = StreamReader::readAs<std::int32_t>(stream);  // +0x8
    m_w3 = StreamReader::readAs<std::int32_t>(stream);  // +0xc
    m_w4 = StreamReader::readAs<std::int32_t>(stream);  // +0x10
    m_w5 = StreamReader::readAs<std::int32_t>(stream);  // +0x14
}

// 0x4c56c4 _ZN7Structs17ClassFuncSetValueD2Ev
// 0x4c56c8 _ZN7Structs17ClassFuncSetValueD1Ev
ClassFuncSetValue::~ClassFuncSetValue() {
}

// 0x4cea90 _ZN7Structs17ClassFuncSetValueD0Ev
void DeletingDestructor(ClassFuncSetValue* self) {
    self->ClassFuncSetValue::~ClassFuncSetValue();
    CustomFree(self);
}

// 0x4c56cc _ZN7Structs17ClassFuncSetValue8finalizeEv
void ClassFuncSetValue::finalize() {
}

// 0x4f1614 _ZN7Structs17ClassFuncSetValue4readEP11IStreamBase
void ClassFuncSetValue::read(IStreamBase* stream) {
    m_w1 = StreamReader::readAs<std::int32_t>(stream);  // +0x4
    m_w2 = StreamReader::readAs<std::int32_t>(stream);  // +0x8
    m_w3 = StreamReader::readAs<std::int32_t>(stream);  // +0xc
    m_w4 = StreamReader::readAs<std::int32_t>(stream);  // +0x10
    m_w5 = StreamReader::readAs<std::int32_t>(stream);  // +0x14
}

// 0x4c5d9c _ZN7Structs10Coward_DogD2Ev
// 0x4c5dd0 _ZN7Structs10Coward_DogD1Ev
Coward_Dog::~Coward_Dog() {
}

// 0x4ce7d4 _ZN7Structs10Coward_DogD0Ev
void DeletingDestructor(Coward_Dog* self) {
    self->Coward_Dog::~Coward_Dog();
    CustomFree(self);
}

// 0x4c5e04 _ZN7Structs10Coward_Dog8finalizeEv
// thunk: b 0x4c5744, forwards to CharacterProperties::finalize
void Coward_Dog::finalize() {
    CharacterProperties::finalize();
}

// 0x4f7ad8 _ZN7Structs10Coward_Dog4readEP11IStreamBase
// thunk: b 0x4f2750, forwards to CharacterProperties::read
void Coward_Dog::read(IStreamBase* stream) {
    CharacterProperties::read(stream);
}

// 0x4c6384 _ZN7Structs17CultistPropertiesD2Ev
// 0x4c63b8 _ZN7Structs17CultistPropertiesD1Ev
CultistProperties::~CultistProperties() {
}

// 0x4ce64c _ZN7Structs17CultistPropertiesD0Ev
void DeletingDestructor(CultistProperties* self) {
    self->CultistProperties::~CultistProperties();
    CustomFree(self);
}

// 0x4c63ec _ZN7Structs17CultistProperties8finalizeEv
// thunk: b 0x4c5744, forwards to CharacterProperties::finalize
void CultistProperties::finalize() {
    CharacterProperties::finalize();
}

// 0x4f7aa0 _ZN7Structs17CultistProperties4readEP11IStreamBase
// thunk: b 0x4f2750, forwards to CharacterProperties::read
void CultistProperties::read(IStreamBase* stream) {
    CharacterProperties::read(stream);
}

// 0x4c5fb8 _ZN7Structs12DKPropertiesD2Ev
// 0x4c5fec _ZN7Structs12DKPropertiesD1Ev
DKProperties::~DKProperties() {
}

// 0x4ce748 _ZN7Structs12DKPropertiesD0Ev
void DeletingDestructor(DKProperties* self) {
    self->DKProperties::~DKProperties();
    CustomFree(self);
}

// 0x4c6020 _ZN7Structs12DKProperties8finalizeEv
// thunk: b 0x4c5744, forwards to CharacterProperties::finalize
void DKProperties::finalize() {
    CharacterProperties::finalize();
}

// 0x4f7ac4 _ZN7Structs12DKProperties4readEP11IStreamBase
// thunk: b 0x4f2750, forwards to CharacterProperties::read
void DKProperties::read(IStreamBase* stream) {
    CharacterProperties::read(stream);
}

// 0x4c6a14 _ZN7Structs14DesignSettingsD2Ev
// 0x4c6a18 _ZN7Structs14DesignSettingsD1Ev
DesignSettings::~DesignSettings() {
}

// 0x4ce400 _ZN7Structs14DesignSettingsD0Ev
void DeletingDestructor(DesignSettings* self) {
    self->DesignSettings::~DesignSettings();
    CustomFree(self);
}

// 0x4c6a1c _ZN7Structs14DesignSettings8finalizeEv
void DesignSettings::finalize() {
}

// 0x4ee0d0 _ZN7Structs14DesignSettings4readEP11IStreamBase
void DesignSettings::read(IStreamBase* stream) {
    StreamReader::readAs<float>(stream, &m_w1);  // +0x4
    StreamReader::readAs<float>(stream, &m_w2);  // +0x8
    StreamReader::readAs<float>(stream, &m_w3);  // +0xc
    StreamReader::readAs<float>(stream, &m_w4);  // +0x10
    m_w5 = StreamReader::readAs<std::int32_t>(stream);  // +0x14
    StreamReader::readAs<float>(stream, &m_w6);  // +0x18
    StreamReader::readAs<float>(stream, &m_w7);  // +0x1c
    StreamReader::readAs<float>(stream, &m_w8);  // +0x20
    m_w9 = StreamReader::readAs<std::int32_t>(stream);  // +0x24
    m_w10 = StreamReader::readAs<std::int32_t>(stream);  // +0x28
    StreamReader::readAs<float>(stream, &m_w11);  // +0x2c
    StreamReader::readAs<float>(stream, &m_w12);  // +0x30
    StreamReader::readAs<float>(stream, &m_w13);  // +0x34
    StreamReader::readAs<float>(stream, &m_w14);  // +0x38
    StreamReader::readAs<float>(stream, &m_w15);  // +0x3c
    StreamReader::readAs<float>(stream, &m_w16);  // +0x40
    StreamReader::readAs<float>(stream, &m_w17);  // +0x44
    StreamReader::readAs<float>(stream, &m_w18);  // +0x48
    StreamReader::readAs<float>(stream, &m_w19);  // +0x4c
    StreamReader::readAs<float>(stream, &m_w20);  // +0x50
    StreamReader::readAs<float>(stream, &m_w21);  // +0x54
    StreamReader::readAs<float>(stream, &m_w22);  // +0x58
    StreamReader::readAs<float>(stream, &m_w23);  // +0x5c
    StreamReader::readAs<float>(stream, &m_w24);  // +0x60
    m_w25 = StreamReader::readAs<std::int32_t>(stream);  // +0x64
    m_w26 = StreamReader::readAs<std::int32_t>(stream);  // +0x68
    m_w27 = StreamReader::readAs<std::int32_t>(stream);  // +0x6c
    m_w28 = StreamReader::readAs<std::int32_t>(stream);  // +0x70
    m_w29 = StreamReader::readAs<std::int32_t>(stream);  // +0x74
    m_w30 = StreamReader::readAs<std::int32_t>(stream);  // +0x78
    m_w31 = StreamReader::readAs<std::int32_t>(stream);  // +0x7c
    m_w32 = StreamReader::readAs<std::int32_t>(stream);  // +0x80
    m_w33 = StreamReader::readAs<std::int32_t>(stream);  // +0x84
    m_w34 = StreamReader::readAs<std::int32_t>(stream);  // +0x88
    StreamReader::readAs<float>(stream, &m_w35);  // +0x8c
    StreamReader::readAs<float>(stream, &m_w36);  // +0x90
    StreamReader::readAs<float>(stream, &m_w37);  // +0x94
    StreamReader::readAs<float>(stream, &m_w38);  // +0x98
    StreamReader::readAs<float>(stream, &m_w39);  // +0x9c
    StreamReader::readAs<float>(stream, &m_w40);  // +0xa0
    StreamReader::readAs<float>(stream, &m_w41);  // +0xa4
    StreamReader::readAs<float>(stream, &m_w42);  // +0xa8
    StreamReader::readAs<float>(stream, &m_w43);  // +0xac
}

// 0x4c6b7c _ZN7Structs10DialogStepD2Ev
// 0x4c6b80 _ZN7Structs10DialogStepD1Ev
DialogStep::~DialogStep() {
}

// 0x4ce358 _ZN7Structs10DialogStepD0Ev
void DeletingDestructor(DialogStep* self) {
    self->DialogStep::~DialogStep();
    CustomFree(self);
}

// 0x4c6b84 _ZN7Structs10DialogStep8finalizeEv
void DialogStep::finalize() {
}

// 0x4f1fb4 _ZN7Structs10DialogStep4readEP11IStreamBase
void DialogStep::read(IStreamBase* stream) {
    m_w1 = StreamReader::readAs<std::int32_t>(stream);  // +0x4
    m_w2 = StreamReader::readAs<std::int32_t>(stream);  // +0x8
    m_w3 = StreamReader::readAs<std::int32_t>(stream);  // +0xc
    m_w4 = StreamReader::readAs<std::int32_t>(stream);  // +0x10
}

// 0x4c64c8 _ZN7Structs16DragonPropertiesD2Ev
// 0x4c64fc _ZN7Structs16DragonPropertiesD1Ev
DragonProperties::~DragonProperties() {
}

// 0x4ce5f8 _ZN7Structs16DragonPropertiesD0Ev
void DeletingDestructor(DragonProperties* self) {
    self->DragonProperties::~DragonProperties();
    CustomFree(self);
}

// 0x4c6530 _ZN7Structs16DragonProperties8finalizeEv
// thunk: b 0x4c5744, forwards to CharacterProperties::finalize
void DragonProperties::finalize() {
    CharacterProperties::finalize();
}

// 0x4f7a94 _ZN7Structs16DragonProperties4readEP11IStreamBase
// thunk: b 0x4f2750, forwards to CharacterProperties::read
void DragonProperties::read(IStreamBase* stream) {
    CharacterProperties::read(stream);
}

// 0x4c6894 _ZN7Structs23EquippedFaeryPropertiesD2Ev
// 0x4c68c8 _ZN7Structs23EquippedFaeryPropertiesD1Ev
EquippedFaeryProperties::~EquippedFaeryProperties() {
}

// 0x4ce4fc _ZN7Structs23EquippedFaeryPropertiesD0Ev
void DeletingDestructor(EquippedFaeryProperties* self) {
    self->EquippedFaeryProperties::~EquippedFaeryProperties();
    CustomFree(self);
}

// 0x4c68fc _ZN7Structs23EquippedFaeryProperties8finalizeEv
// thunk: b 0x4c5744, forwards to CharacterProperties::finalize
void EquippedFaeryProperties::finalize() {
    CharacterProperties::finalize();
}

// 0x4f7a70 _ZN7Structs23EquippedFaeryProperties4readEP11IStreamBase
// thunk: b 0x4f2750, forwards to CharacterProperties::read
void EquippedFaeryProperties::read(IStreamBase* stream) {
    CharacterProperties::read(stream);
}

// 0x4da404 _ZN7Structs5FaeryD2Ev
// 0x4da3a8 _ZN7Structs5FaeryD1Ev
Faery::~Faery() {
    if (m_w6 != nullptr) CustomFree(m_w6);  // +0x18
}

// 0x4da3e8 _ZN7Structs5FaeryD0Ev
void DeletingDestructor(Faery* self) {
    self->Faery::~Faery();
    CustomFree(self);
}

// 0x4da380 _ZN7Structs5Faery8finalizeEv
void Faery::finalize() {
    if (m_w6 != nullptr) {
        CustomFree(m_w6);  // +0x18
        m_w5 = 0;  // +0x14
        m_w6 = nullptr;
    }
}

// 0x4da340 _ZN7Structs9FaeryListD2Ev
// 0x4da2e4 _ZN7Structs9FaeryListD1Ev
FaeryList::~FaeryList() {
    if (m_w2 != nullptr) CustomFree(m_w2);  // +0x8
}

// 0x4da324 _ZN7Structs9FaeryListD0Ev
void DeletingDestructor(FaeryList* self) {
    self->FaeryList::~FaeryList();
    CustomFree(self);
}

// 0x4da2bc _ZN7Structs9FaeryList8finalizeEv
void FaeryList::finalize() {
    if (m_w2 != nullptr) {
        CustomFree(m_w2);  // +0x8
        m_w1 = 0;  // +0x4
        m_w2 = nullptr;
    }
}

// 0x4d5638 _ZN7Structs21FastTravelDestinationD2Ev
// 0x4d55dc _ZN7Structs21FastTravelDestinationD1Ev
FastTravelDestination::~FastTravelDestination() {
    if (m_w4 != nullptr) CustomFree(m_w4);  // +0x10
}

// 0x4d561c _ZN7Structs21FastTravelDestinationD0Ev
void DeletingDestructor(FastTravelDestination* self) {
    self->FastTravelDestination::~FastTravelDestination();
    CustomFree(self);
}

// 0x4d55b4 _ZN7Structs21FastTravelDestination8finalizeEv
void FastTravelDestination::finalize() {
    if (m_w4 != nullptr) {
        CustomFree(m_w4);  // +0x10
        m_w3 = 0;  // +0xc
        m_w4 = nullptr;
    }
}

// 0x4c67bc _ZN7Structs18FollowerPropertiesD2Ev
// 0x4c67f0 _ZN7Structs18FollowerPropertiesD1Ev
FollowerProperties::~FollowerProperties() {
}

// 0x4ce534 _ZN7Structs18FollowerPropertiesD0Ev
void DeletingDestructor(FollowerProperties* self) {
    self->FollowerProperties::~FollowerProperties();
    CustomFree(self);
}

// 0x4c6824 _ZN7Structs18FollowerProperties8finalizeEv
// thunk: b 0x4c5744, forwards to CharacterProperties::finalize
void FollowerProperties::finalize() {
    CharacterProperties::finalize();
}

// 0x4f7a78 _ZN7Structs18FollowerProperties4readEP11IStreamBase
// thunk: b 0x4f2750, forwards to CharacterProperties::read
void FollowerProperties::read(IStreamBase* stream) {
    CharacterProperties::read(stream);
}

// 0x4c6b94 _ZN7Structs12FontColorDefD2Ev
// 0x4c6b98 _ZN7Structs12FontColorDefD1Ev
FontColorDef::~FontColorDef() {
}

// 0x4ce320 _ZN7Structs12FontColorDefD0Ev
void DeletingDestructor(FontColorDef* self) {
    self->FontColorDef::~FontColorDef();
    CustomFree(self);
}

// 0x4c6b9c _ZN7Structs12FontColorDef8finalizeEv
void FontColorDef::finalize() {
}

// 0x4f1d00 _ZN7Structs12FontColorDef4readEP11IStreamBase
void FontColorDef::read(IStreamBase* stream) {
    m_w1 = StreamReader::readAs<std::int32_t>(stream);  // +0x4
    m_w2 = StreamReader::readAs<std::int32_t>(stream);  // +0x8
}

// 0x4c6900 _ZN7Structs21FriendlyNPCPropertiesD2Ev
// 0x4c6934 _ZN7Structs21FriendlyNPCPropertiesD1Ev
FriendlyNPCProperties::~FriendlyNPCProperties() {
}

// 0x4ce4e0 _ZN7Structs21FriendlyNPCPropertiesD0Ev
void DeletingDestructor(FriendlyNPCProperties* self) {
    self->FriendlyNPCProperties::~FriendlyNPCProperties();
    CustomFree(self);
}

// 0x4c6968 _ZN7Structs21FriendlyNPCProperties8finalizeEv
// thunk: b 0x4c5744, forwards to CharacterProperties::finalize
void FriendlyNPCProperties::finalize() {
    CharacterProperties::finalize();
}

// 0x4f7a6c _ZN7Structs21FriendlyNPCProperties4readEP11IStreamBase
// thunk: b 0x4f2750, forwards to CharacterProperties::read
void FriendlyNPCProperties::read(IStreamBase* stream) {
    CharacterProperties::read(stream);
}

// 0x4c6a20 _ZN7Structs14GameDifficultyD2Ev
// 0x4c6a24 _ZN7Structs14GameDifficultyD1Ev
GameDifficulty::~GameDifficulty() {
}

// 0x4ce3e4 _ZN7Structs14GameDifficultyD0Ev
void DeletingDestructor(GameDifficulty* self) {
    self->GameDifficulty::~GameDifficulty();
    CustomFree(self);
}

// 0x4c6a28 _ZN7Structs14GameDifficulty8finalizeEv
void GameDifficulty::finalize() {
}

// 0x4f23e4 _ZN7Structs14GameDifficulty4readEP11IStreamBase
void GameDifficulty::read(IStreamBase* stream) {
    m_w1 = StreamReader::readAs<std::int32_t>(stream);  // +0x4
    m_w2 = StreamReader::readAs<std::int32_t>(stream);  // +0x8
    m_w3 = StreamReader::readAs<std::int32_t>(stream);  // +0xc
    m_w4 = StreamReader::readAs<std::int32_t>(stream);  // +0x10
    m_w5 = StreamReader::readAs<std::int32_t>(stream);  // +0x14
}

// 0x4c6ba0 _ZN7Structs16GameObjectDamageD2Ev
// 0x4c6ba4 _ZN7Structs16GameObjectDamageD1Ev
GameObjectDamage::~GameObjectDamage() {
}

// 0x4ce304 _ZN7Structs16GameObjectDamageD0Ev
void DeletingDestructor(GameObjectDamage* self) {
    self->GameObjectDamage::~GameObjectDamage();
    CustomFree(self);
}

// 0x4c6ba8 _ZN7Structs16GameObjectDamage8finalizeEv
void GameObjectDamage::finalize() {
}

// 0x4f1dd0 _ZN7Structs16GameObjectDamage4readEP11IStreamBase
void GameObjectDamage::read(IStreamBase* stream) {
    m_w1 = StreamReader::readAs<std::int32_t>(stream);  // +0x4
    m_w2 = StreamReader::readAs<std::int32_t>(stream);  // +0x8
    m_w3 = StreamReader::readAs<std::int32_t>(stream);  // +0xc
    m_w4 = StreamReader::readAs<std::int32_t>(stream);  // +0x10
    m_w5 = StreamReader::readAs<std::int32_t>(stream);  // +0x14
}

// 0x4c6bac _ZN7Structs16GameObjectVisualD2Ev
// 0x4c6bb0 _ZN7Structs16GameObjectVisualD1Ev
GameObjectVisual::~GameObjectVisual() {
}

// 0x4ce2e8 _ZN7Structs16GameObjectVisualD0Ev
void DeletingDestructor(GameObjectVisual* self) {
    self->GameObjectVisual::~GameObjectVisual();
    CustomFree(self);
}

// 0x4c6bb4 _ZN7Structs16GameObjectVisual8finalizeEv
void GameObjectVisual::finalize() {
}

// 0x4ff23c _ZN7Structs16GameObjectVisual4readEP11IStreamBase
void GameObjectVisual::read(IStreamBase* stream) {
    m_w1 = StreamReader::readAs<std::int32_t>(stream);  // +0x4
    m_w2 = StreamReader::readAs<std::int32_t>(stream);  // +0x8
    m_w3 = StreamReader::readAs<std::int32_t>(stream);  // +0xc
    m_w4 = StreamReader::readAs<std::int32_t>(stream);  // +0x10
    m_w5 = StreamReader::readAs<std::int32_t>(stream);  // +0x14
    m_w6 = StreamReader::readAs<std::int32_t>(stream);  // +0x18
    m_w7 = StreamReader::readAs<std::int32_t>(stream);  // +0x1c
}

// 0x4c6a2c _ZN7Structs10GameOptionD2Ev
// 0x4c6a30 _ZN7Structs10GameOptionD1Ev
GameOption::~GameOption() {
}

// 0x4ce374 _ZN7Structs10GameOptionD0Ev
void DeletingDestructor(GameOption* self) {
    self->GameOption::~GameOption();
    CustomFree(self);
}

// 0x4c6a34 _ZN7Structs10GameOption8finalizeEv
void GameOption::finalize() {
}

// 0x4f213c _ZN7Structs10GameOption4readEP11IStreamBase
void GameOption::read(IStreamBase* stream) {
    m_w1 = StreamReader::readAs<std::int32_t>(stream);  // +0x4
    m_w2 = StreamReader::readAs<std::int32_t>(stream);  // +0x8
    m_w3 = StreamReader::readAs<std::int32_t>(stream);  // +0xc
    m_w4 = StreamReader::readAs<std::int32_t>(stream);  // +0x10
    m_w5 = StreamReader::readAs<std::int32_t>(stream);  // +0x14
    m_w6 = StreamReader::readAs<std::int32_t>(stream);  // +0x18
    m_w7 = StreamReader::readAs<std::int32_t>(stream);  // +0x1c
}

// 0x4c6a38 _ZN7Structs16GameOptionSelectD2Ev
// 0x4c6a6c _ZN7Structs16GameOptionSelectD1Ev
GameOptionSelect::~GameOptionSelect() {
}

// 0x4ce3c8 _ZN7Structs16GameOptionSelectD0Ev
void DeletingDestructor(GameOptionSelect* self) {
    self->GameOptionSelect::~GameOptionSelect();
    CustomFree(self);
}

// 0x4c6aa0 _ZN7Structs16GameOptionSelect8finalizeEv
// thunk: b 0x4c6a34, forwards to GameOption::finalize
void GameOptionSelect::finalize() {
    GameOption::finalize();
}

// 0x4f23e0 _ZN7Structs16GameOptionSelect4readEP11IStreamBase
// thunk: b 0x4f213c, forwards to GameOption::read
void GameOptionSelect::read(IStreamBase* stream) {
    GameOption::read(stream);
}

// 0x4c6aa4 _ZN7Structs16GameOptionSliderD2Ev
// 0x4c6ad8 _ZN7Structs16GameOptionSliderD1Ev
GameOptionSlider::~GameOptionSlider() {
}

// 0x4ce3ac _ZN7Structs16GameOptionSliderD0Ev
void DeletingDestructor(GameOptionSlider* self) {
    self->GameOptionSlider::~GameOptionSlider();
    CustomFree(self);
}

// 0x4c6b0c _ZN7Structs16GameOptionSlider8finalizeEv
// thunk: b 0x4c6a34, forwards to GameOption::finalize
void GameOptionSlider::finalize() {
    GameOption::finalize();
}

// 0x4f23dc _ZN7Structs16GameOptionSlider4readEP11IStreamBase
// thunk: b 0x4f213c, forwards to GameOption::read
void GameOptionSlider::read(IStreamBase* stream) {
    GameOption::read(stream);
}

// 0x4c6b10 _ZN7Structs16GameOptionToggleD2Ev
// 0x4c6b44 _ZN7Structs16GameOptionToggleD1Ev
GameOptionToggle::~GameOptionToggle() {
}

// 0x4ce390 _ZN7Structs16GameOptionToggleD0Ev
void DeletingDestructor(GameOptionToggle* self) {
    self->GameOptionToggle::~GameOptionToggle();
    CustomFree(self);
}

// 0x4c6b78 _ZN7Structs16GameOptionToggle8finalizeEv
// thunk: b 0x4c6a34, forwards to GameOption::finalize
void GameOptionToggle::finalize() {
    GameOption::finalize();
}

// 0x4f23d8 _ZN7Structs16GameOptionToggle4readEP11IStreamBase
// thunk: b 0x4f213c, forwards to GameOption::read
void GameOptionToggle::read(IStreamBase* stream) {
    GameOption::read(stream);
}

// 0x4c5e08 _ZN7Structs15GhostPropertiesD2Ev
// 0x4c5e3c _ZN7Structs15GhostPropertiesD1Ev
GhostProperties::~GhostProperties() {
}

// 0x4ce7b8 _ZN7Structs15GhostPropertiesD0Ev
void DeletingDestructor(GhostProperties* self) {
    self->GhostProperties::~GhostProperties();
    CustomFree(self);
}

// 0x4c5e70 _ZN7Structs15GhostProperties8finalizeEv
// thunk: b 0x4c5744, forwards to CharacterProperties::finalize
void GhostProperties::finalize() {
    CharacterProperties::finalize();
}

// 0x4f7ad4 _ZN7Structs15GhostProperties4readEP11IStreamBase
// thunk: b 0x4f2750, forwards to CharacterProperties::read
void GhostProperties::read(IStreamBase* stream) {
    CharacterProperties::read(stream);
}

// 0x4c60fc _ZN7Structs21GoblinRoguePropertiesD2Ev
// 0x4c6130 _ZN7Structs21GoblinRoguePropertiesD1Ev
GoblinRogueProperties::~GoblinRogueProperties() {
}

// 0x4ce6f4 _ZN7Structs21GoblinRoguePropertiesD0Ev
void DeletingDestructor(GoblinRogueProperties* self) {
    self->GoblinRogueProperties::~GoblinRogueProperties();
    CustomFree(self);
}

// 0x4c6164 _ZN7Structs21GoblinRogueProperties8finalizeEv
// thunk: b 0x4c5744, forwards to CharacterProperties::finalize
void GoblinRogueProperties::finalize() {
    CharacterProperties::finalize();
}

// 0x4f7ab8 _ZN7Structs21GoblinRogueProperties4readEP11IStreamBase
// thunk: b 0x4f2750, forwards to CharacterProperties::read
void GoblinRogueProperties::read(IStreamBase* stream) {
    CharacterProperties::read(stream);
}

// 0x4c61d4 _ZN7Structs22GoblinShamanPropertiesD2Ev
// 0x4c6208 _ZN7Structs22GoblinShamanPropertiesD1Ev
GoblinShamanProperties::~GoblinShamanProperties() {
}

// 0x4ce6bc _ZN7Structs22GoblinShamanPropertiesD0Ev
void DeletingDestructor(GoblinShamanProperties* self) {
    self->GoblinShamanProperties::~GoblinShamanProperties();
    CustomFree(self);
}

// 0x4c623c _ZN7Structs22GoblinShamanProperties8finalizeEv
// thunk: b 0x4c5744, forwards to CharacterProperties::finalize
void GoblinShamanProperties::finalize() {
    CharacterProperties::finalize();
}

// 0x4f7ab0 _ZN7Structs22GoblinShamanProperties4readEP11IStreamBase
// thunk: b 0x4f2750, forwards to CharacterProperties::read
void GoblinShamanProperties::read(IStreamBase* stream) {
    CharacterProperties::read(stream);
}

// 0x4c6168 _ZN7Structs23GoblinSlingerPropertiesD2Ev
// 0x4c619c _ZN7Structs23GoblinSlingerPropertiesD1Ev
GoblinSlingerProperties::~GoblinSlingerProperties() {
}

// 0x4ce6d8 _ZN7Structs23GoblinSlingerPropertiesD0Ev
void DeletingDestructor(GoblinSlingerProperties* self) {
    self->GoblinSlingerProperties::~GoblinSlingerProperties();
    CustomFree(self);
}

// 0x4c61d0 _ZN7Structs23GoblinSlingerProperties8finalizeEv
// thunk: b 0x4c5744, forwards to CharacterProperties::finalize
void GoblinSlingerProperties::finalize() {
    CharacterProperties::finalize();
}

// 0x4f7ab4 _ZN7Structs23GoblinSlingerProperties4readEP11IStreamBase
// thunk: b 0x4f2750, forwards to CharacterProperties::read
void GoblinSlingerProperties::read(IStreamBase* stream) {
    CharacterProperties::read(stream);
}

// 0x4c6318 _ZN7Structs15GuardPropertiesD2Ev
// 0x4c634c _ZN7Structs15GuardPropertiesD1Ev
GuardProperties::~GuardProperties() {
}

// 0x4ce668 _ZN7Structs15GuardPropertiesD0Ev
void DeletingDestructor(GuardProperties* self) {
    self->GuardProperties::~GuardProperties();
    CustomFree(self);
}

// 0x4c6380 _ZN7Structs15GuardProperties8finalizeEv
// thunk: b 0x4c5744, forwards to CharacterProperties::finalize
void GuardProperties::finalize() {
    CharacterProperties::finalize();
}

// 0x4f7aa4 _ZN7Structs15GuardProperties4readEP11IStreamBase
// thunk: b 0x4f2750, forwards to CharacterProperties::read
void GuardProperties::read(IStreamBase* stream) {
    CharacterProperties::read(stream);
}

// 0x4c6bd0 _ZN7Structs8HelpPageD2Ev
// 0x4c6bd4 _ZN7Structs8HelpPageD1Ev
HelpPage::~HelpPage() {
}

// 0x4ce294 _ZN7Structs8HelpPageD0Ev
void DeletingDestructor(HelpPage* self) {
    self->HelpPage::~HelpPage();
    CustomFree(self);
}

// 0x4c6bd8 _ZN7Structs8HelpPage8finalizeEv
void HelpPage::finalize() {
}

// 0x4ff0fc _ZN7Structs8HelpPage4readEP11IStreamBase
void HelpPage::read(IStreamBase* stream) {
    m_w1 = StreamReader::readAs<std::int32_t>(stream);  // +0x4
    m_w2 = StreamReader::readAs<std::int32_t>(stream);  // +0x8
}

// 0x4c6bdc _ZN7Structs8HintPageD2Ev
// 0x4c6be0 _ZN7Structs8HintPageD1Ev
HintPage::~HintPage() {
}

// 0x4ce278 _ZN7Structs8HintPageD0Ev
void DeletingDestructor(HintPage* self) {
    self->HintPage::~HintPage();
    CustomFree(self);
}

// 0x4c6be4 _ZN7Structs8HintPage8finalizeEv
void HintPage::finalize() {
}

// 0x4ff02c _ZN7Structs8HintPage4readEP11IStreamBase
void HintPage::read(IStreamBase* stream) {
    m_w1 = StreamReader::readAs<std::int32_t>(stream);  // +0x4
    m_w2 = StreamReader::readAs<std::int32_t>(stream);  // +0x8
}

// 0x4c5820 _ZN7Structs18InfectedPropertiesD2Ev
// 0x4c5854 _ZN7Structs18InfectedPropertiesD1Ev
InfectedProperties::~InfectedProperties() {
}

// 0x4ce940 _ZN7Structs18InfectedPropertiesD0Ev
void DeletingDestructor(InfectedProperties* self) {
    self->InfectedProperties::~InfectedProperties();
    CustomFree(self);
}

// 0x4c5888 _ZN7Structs18InfectedProperties8finalizeEv
// thunk: b 0x4c5744, forwards to CharacterProperties::finalize
void InfectedProperties::finalize() {
    CharacterProperties::finalize();
}

// 0x4f7b0c _ZN7Structs18InfectedProperties4readEP11IStreamBase
// thunk: b 0x4f2750, forwards to CharacterProperties::read
void InfectedProperties::read(IStreamBase* stream) {
    CharacterProperties::read(stream);
}

// 0x4c6c3c _ZN7Structs9InventoryD2Ev
// 0x4c6c40 _ZN7Structs9InventoryD1Ev
Inventory::~Inventory() {
}

// 0x4ce198 _ZN7Structs9InventoryD0Ev
void DeletingDestructor(Inventory* self) {
    self->Inventory::~Inventory();
    CustomFree(self);
}

// 0x4c6c44 _ZN7Structs9Inventory8finalizeEv
void Inventory::finalize() {
}

// 0x4d4954 _ZN7Structs8ItemBaseD2Ev
// 0x4d48f8 _ZN7Structs8ItemBaseD1Ev
ItemBase::~ItemBase() {
    if (m_w2 != nullptr) CustomFree(m_w2);  // +0x8
}

// 0x4d4938 _ZN7Structs8ItemBaseD0Ev
void DeletingDestructor(ItemBase* self) {
    self->ItemBase::~ItemBase();
    CustomFree(self);
}

// 0x4d44f4 _ZN7Structs8ItemBase8finalizeEv
void ItemBase::finalize() {
    if (m_w2 != nullptr) {
        CustomFree(m_w2);  // +0x8
        m_w1 = 0;  // +0x4
        m_w2 = nullptr;
    }
}

// 0x4c6c0c _ZN7Structs13ItemListEntryD2Ev
// 0x4c6c10 _ZN7Structs13ItemListEntryD1Ev
ItemListEntry::~ItemListEntry() {
}

// 0x4ce208 _ZN7Structs13ItemListEntryD0Ev
void DeletingDestructor(ItemListEntry* self) {
    self->ItemListEntry::~ItemListEntry();
    CustomFree(self);
}

// 0x4c6c14 _ZN7Structs13ItemListEntry8finalizeEv
void ItemListEntry::finalize() {
}

// 0x4c6be8 _ZN7Structs14ItemPowerEntryD2Ev
// 0x4c6bec _ZN7Structs14ItemPowerEntryD1Ev
ItemPowerEntry::~ItemPowerEntry() {
}

// 0x4ce25c _ZN7Structs14ItemPowerEntryD0Ev
void DeletingDestructor(ItemPowerEntry* self) {
    self->ItemPowerEntry::~ItemPowerEntry();
    CustomFree(self);
}

// 0x4c6bf0 _ZN7Structs14ItemPowerEntry8finalizeEv
void ItemPowerEntry::finalize() {
}

// 0x4ed200 _ZN7Structs14ItemPowerEntry4readEP11IStreamBase
void ItemPowerEntry::read(IStreamBase* stream) {
    m_w1 = StreamReader::readAs<std::int32_t>(stream);  // +0x4
    StreamReader::readAs<std::int8_t>(stream, &m_w2);  // +0x8
}

// 0x4c62ac _ZN7Structs20LargePlantPropertiesD2Ev
// 0x4c62e0 _ZN7Structs20LargePlantPropertiesD1Ev
LargePlantProperties::~LargePlantProperties() {
}

// 0x4ce684 _ZN7Structs20LargePlantPropertiesD0Ev
void DeletingDestructor(LargePlantProperties* self) {
    self->LargePlantProperties::~LargePlantProperties();
    CustomFree(self);
}

// 0x4c6314 _ZN7Structs20LargePlantProperties8finalizeEv
// thunk: b 0x4c5744, forwards to CharacterProperties::finalize
void LargePlantProperties::finalize() {
    CharacterProperties::finalize();
}

// 0x4f7aa8 _ZN7Structs20LargePlantProperties4readEP11IStreamBase
// thunk: b 0x4f2750, forwards to CharacterProperties::read
void LargePlantProperties::read(IStreamBase* stream) {
    CharacterProperties::read(stream);
}

// 0x4d5564 _ZN7Structs16LevelDeclarationD2Ev
// 0x4d54f8 _ZN7Structs16LevelDeclarationD1Ev
LevelDeclaration::~LevelDeclaration() {
    if (m_w3 != nullptr) CustomFree(m_w3);  // +0xc
    if (m_w8 != nullptr) CustomFree(m_w8);  // +0x20
}

// 0x4d5548 _ZN7Structs16LevelDeclarationD0Ev
void DeletingDestructor(LevelDeclaration* self) {
    self->LevelDeclaration::~LevelDeclaration();
    CustomFree(self);
}

// 0x4d54b4 _ZN7Structs16LevelDeclaration8finalizeEv
void LevelDeclaration::finalize() {
    if (m_w3 != nullptr) {
        CustomFree(m_w3);  // +0xc
        m_w2 = 0;  // +0x8
        m_w3 = nullptr;
    }
    if (m_w8 != nullptr) {
        CustomFree(m_w8);  // +0x20
        m_w7 = 0;  // +0x1c
        m_w8 = nullptr;
    }
}

// 0x4c6bb8 _ZN7Structs14LiftableObjectD2Ev
// 0x4c6bbc _ZN7Structs14LiftableObjectD1Ev
LiftableObject::~LiftableObject() {
}

// 0x4ce2cc _ZN7Structs14LiftableObjectD0Ev
void DeletingDestructor(LiftableObject* self) {
    self->LiftableObject::~LiftableObject();
    CustomFree(self);
}

// 0x4c6bc0 _ZN7Structs14LiftableObject8finalizeEv
void LiftableObject::finalize() {
}

// 0x4ff1cc _ZN7Structs14LiftableObject4readEP11IStreamBase
void LiftableObject::read(IStreamBase* stream) {
    m_w1 = StreamReader::readAs<std::int32_t>(stream);  // +0x4
}

// 0x4c7c98 _ZN7Structs8ListenerD2Ev
// 0x4c7c9c _ZN7Structs8ListenerD1Ev
Listener::~Listener() {
}

// 0x4cdcac _ZN7Structs8ListenerD0Ev
void DeletingDestructor(Listener* self) {
    self->Listener::~Listener();
    CustomFree(self);
}

// 0x4c7ca0 _ZN7Structs8Listener8finalizeEv
void Listener::finalize() {
}

// 0x4ed978 _ZN7Structs8Listener4readEP11IStreamBase
void Listener::read(IStreamBase* stream) {
    m_w1 = StreamReader::readAs<std::int32_t>(stream);  // +0x4
    m_w2 = StreamReader::readAs<std::int32_t>(stream);  // +0x8
    m_w3 = StreamReader::readAs<std::int32_t>(stream);  // +0xc
    m_w4 = StreamReader::readAs<std::int32_t>(stream);  // +0x10
    StreamReader::readAs<float>(stream, &m_w5);  // +0x14
    m_w6 = StreamReader::readAs<std::int32_t>(stream);  // +0x18
}

// 0x4c7c8c _ZN7Structs14ListenerVectorD2Ev
// 0x4c7c90 _ZN7Structs14ListenerVectorD1Ev
ListenerVector::~ListenerVector() {
}

// 0x4cdcc8 _ZN7Structs14ListenerVectorD0Ev
void DeletingDestructor(ListenerVector* self) {
    self->ListenerVector::~ListenerVector();
    CustomFree(self);
}

// 0x4c7c94 _ZN7Structs14ListenerVector8finalizeEv
void ListenerVector::finalize() {
}

// 0x4dc180 _ZN7Structs14ListenerVector4readEP11IStreamBase
void ListenerVector::read(IStreamBase* stream) {
    StreamReader::readAs<float>(stream, &m_w1);  // +0x4
    StreamReader::readAs<float>(stream, &m_w2);  // +0x8
    StreamReader::readAs<float>(stream, &m_w3);  // +0xc
}

// 0x4c588c _ZN7Structs19LizardManPropertiesD2Ev
// 0x4c58c0 _ZN7Structs19LizardManPropertiesD1Ev
LizardManProperties::~LizardManProperties() {
}

// 0x4ce924 _ZN7Structs19LizardManPropertiesD0Ev
void DeletingDestructor(LizardManProperties* self) {
    self->LizardManProperties::~LizardManProperties();
    CustomFree(self);
}

// 0x4c58f4 _ZN7Structs19LizardManProperties8finalizeEv
// thunk: b 0x4c5744, forwards to CharacterProperties::finalize
void LizardManProperties::finalize() {
    CharacterProperties::finalize();
}

// 0x4f7b08 _ZN7Structs19LizardManProperties4readEP11IStreamBase
// thunk: b 0x4f2750, forwards to CharacterProperties::read
void LizardManProperties::read(IStreamBase* stream) {
    CharacterProperties::read(stream);
}

// 0x4c6c00 _ZN7Structs9LootEntryD2Ev
// 0x4c6c04 _ZN7Structs9LootEntryD1Ev
LootEntry::~LootEntry() {
}

// 0x4ce224 _ZN7Structs9LootEntryD0Ev
void DeletingDestructor(LootEntry* self) {
    self->LootEntry::~LootEntry();
    CustomFree(self);
}

// 0x4c6c08 _ZN7Structs9LootEntry8finalizeEv
void LootEntry::finalize() {
}

// 0x4c66e4 _ZN7Structs29MadrukEnergySpawnerPropertiesD2Ev
// 0x4c6718 _ZN7Structs29MadrukEnergySpawnerPropertiesD1Ev
MadrukEnergySpawnerProperties::~MadrukEnergySpawnerProperties() {
}

// 0x4ce56c _ZN7Structs29MadrukEnergySpawnerPropertiesD0Ev
void DeletingDestructor(MadrukEnergySpawnerProperties* self) {
    self->MadrukEnergySpawnerProperties::~MadrukEnergySpawnerProperties();
    CustomFree(self);
}

// 0x4c674c _ZN7Structs29MadrukEnergySpawnerProperties8finalizeEv
// thunk: b 0x4c5744, forwards to CharacterProperties::finalize
void MadrukEnergySpawnerProperties::finalize() {
    CharacterProperties::finalize();
}

// 0x4f7a80 _ZN7Structs29MadrukEnergySpawnerProperties4readEP11IStreamBase
// thunk: b 0x4f2750, forwards to CharacterProperties::read
void MadrukEnergySpawnerProperties::read(IStreamBase* stream) {
    CharacterProperties::read(stream);
}

// 0x4c6678 _ZN7Structs22MadrukMinionPropertiesD2Ev
// 0x4c66ac _ZN7Structs22MadrukMinionPropertiesD1Ev
MadrukMinionProperties::~MadrukMinionProperties() {
}

// 0x4ce588 _ZN7Structs22MadrukMinionPropertiesD0Ev
void DeletingDestructor(MadrukMinionProperties* self) {
    self->MadrukMinionProperties::~MadrukMinionProperties();
    CustomFree(self);
}

// 0x4c66e0 _ZN7Structs22MadrukMinionProperties8finalizeEv
// thunk: b 0x4c5744, forwards to CharacterProperties::finalize
void MadrukMinionProperties::finalize() {
    CharacterProperties::finalize();
}

// 0x4f7a84 _ZN7Structs22MadrukMinionProperties4readEP11IStreamBase
// thunk: b 0x4f2750, forwards to CharacterProperties::read
void MadrukMinionProperties::read(IStreamBase* stream) {
    CharacterProperties::read(stream);
}

// 0x4c65a0 _ZN7Structs16MadrukPropertiesD2Ev
// 0x4c65d4 _ZN7Structs16MadrukPropertiesD1Ev
MadrukProperties::~MadrukProperties() {
}

// 0x4ce5c0 _ZN7Structs16MadrukPropertiesD0Ev
void DeletingDestructor(MadrukProperties* self) {
    self->MadrukProperties::~MadrukProperties();
    CustomFree(self);
}

// 0x4c6608 _ZN7Structs16MadrukProperties8finalizeEv
// thunk: b 0x4c5744, forwards to CharacterProperties::finalize
void MadrukProperties::finalize() {
    CharacterProperties::finalize();
}

// 0x4f7a8c _ZN7Structs16MadrukProperties4readEP11IStreamBase
// thunk: b 0x4f2750, forwards to CharacterProperties::read
void MadrukProperties::read(IStreamBase* stream) {
    CharacterProperties::read(stream);
}

// 0x4c660c _ZN7Structs21MadrukTorsoPropertiesD2Ev
// 0x4c6640 _ZN7Structs21MadrukTorsoPropertiesD1Ev
MadrukTorsoProperties::~MadrukTorsoProperties() {
}

// 0x4ce5a4 _ZN7Structs21MadrukTorsoPropertiesD0Ev
void DeletingDestructor(MadrukTorsoProperties* self) {
    self->MadrukTorsoProperties::~MadrukTorsoProperties();
    CustomFree(self);
}

// 0x4c6674 _ZN7Structs21MadrukTorsoProperties8finalizeEv
// thunk: b 0x4c5744, forwards to CharacterProperties::finalize
void MadrukTorsoProperties::finalize() {
    CharacterProperties::finalize();
}

// 0x4f7a88 _ZN7Structs21MadrukTorsoProperties4readEP11IStreamBase
// thunk: b 0x4f2750, forwards to CharacterProperties::read
void MadrukTorsoProperties::read(IStreamBase* stream) {
    CharacterProperties::read(stream);
}

// 0x4c6c30 _ZN7Structs20MerchandiseListEntryD2Ev
// 0x4c6c34 _ZN7Structs20MerchandiseListEntryD1Ev
MerchandiseListEntry::~MerchandiseListEntry() {
}

// 0x4ce1b4 _ZN7Structs20MerchandiseListEntryD0Ev
void DeletingDestructor(MerchandiseListEntry* self) {
    self->MerchandiseListEntry::~MerchandiseListEntry();
    CustomFree(self);
}

// 0x4c6c38 _ZN7Structs20MerchandiseListEntry8finalizeEv
void MerchandiseListEntry::finalize() {
}

// 0x4f1bc0 _ZN7Structs20MerchandiseListEntry4readEP11IStreamBase
void MerchandiseListEntry::read(IStreamBase* stream) {
    m_w1 = StreamReader::readAs<std::int32_t>(stream);  // +0x4
    m_w2 = StreamReader::readAs<std::int32_t>(stream);  // +0x8
}

// 0x4c6750 _ZN7Structs18MerchantPropertiesD2Ev
// 0x4c6784 _ZN7Structs18MerchantPropertiesD1Ev
MerchantProperties::~MerchantProperties() {
}

// 0x4ce550 _ZN7Structs18MerchantPropertiesD0Ev
void DeletingDestructor(MerchantProperties* self) {
    self->MerchantProperties::~MerchantProperties();
    CustomFree(self);
}

// 0x4c67b8 _ZN7Structs18MerchantProperties8finalizeEv
// thunk: b 0x4c5744, forwards to CharacterProperties::finalize
void MerchantProperties::finalize() {
    CharacterProperties::finalize();
}

// 0x4f7a7c _ZN7Structs18MerchantProperties4readEP11IStreamBase
// thunk: b 0x4f2750, forwards to CharacterProperties::read
void MerchantProperties::read(IStreamBase* stream) {
    CharacterProperties::read(stream);
}

// 0x4c59d0 _ZN7Structs20MothCocoonPropertiesD2Ev
// 0x4c5a04 _ZN7Structs20MothCocoonPropertiesD1Ev
MothCocoonProperties::~MothCocoonProperties() {
}

// 0x4ce8d0 _ZN7Structs20MothCocoonPropertiesD0Ev
void DeletingDestructor(MothCocoonProperties* self) {
    self->MothCocoonProperties::~MothCocoonProperties();
    CustomFree(self);
}

// 0x4c5a38 _ZN7Structs20MothCocoonProperties8finalizeEv
// thunk: b 0x4c5744, forwards to CharacterProperties::finalize
void MothCocoonProperties::finalize() {
    CharacterProperties::finalize();
}

// 0x4f7afc _ZN7Structs20MothCocoonProperties4readEP11IStreamBase
// thunk: b 0x4f2750, forwards to CharacterProperties::read
void MothCocoonProperties::read(IStreamBase* stream) {
    CharacterProperties::read(stream);
}

// 0x4c5964 _ZN7Structs14MothPropertiesD2Ev
// 0x4c5998 _ZN7Structs14MothPropertiesD1Ev
MothProperties::~MothProperties() {
}

// 0x4ce8ec _ZN7Structs14MothPropertiesD0Ev
void DeletingDestructor(MothProperties* self) {
    self->MothProperties::~MothProperties();
    CustomFree(self);
}

// 0x4c59cc _ZN7Structs14MothProperties8finalizeEv
// thunk: b 0x4c5744, forwards to CharacterProperties::finalize
void MothProperties::finalize() {
    CharacterProperties::finalize();
}

// 0x4f7b00 _ZN7Structs14MothProperties4readEP11IStreamBase
// thunk: b 0x4f2750, forwards to CharacterProperties::read
void MothProperties::read(IStreamBase* stream) {
    CharacterProperties::read(stream);
}

// 0x4c6c18 _ZN7Structs7NumProbD2Ev
// 0x4c6c1c _ZN7Structs7NumProbD1Ev
NumProb::~NumProb() {
}

// 0x4ce1ec _ZN7Structs7NumProbD0Ev
void DeletingDestructor(NumProb* self) {
    self->NumProb::~NumProb();
    CustomFree(self);
}

// 0x4c6c20 _ZN7Structs7NumProb8finalizeEv
void NumProb::finalize() {
}

// 0x4dbc9c _ZN7Structs7NumProb4readEP11IStreamBase
void NumProb::read(IStreamBase* stream) {
    StreamReader::readAs<std::int16_t>(stream, &m_w1);  // +0x4
    StreamReader::readAs<std::int16_t>(stream, &m_off6);  // +0x6
}

// 0x4c5748 _ZN7Structs16PlayerPropertiesD2Ev
// 0x4c577c _ZN7Structs16PlayerPropertiesD1Ev
PlayerProperties::~PlayerProperties() {
}

// 0x4ce978 _ZN7Structs16PlayerPropertiesD0Ev
void DeletingDestructor(PlayerProperties* self) {
    self->PlayerProperties::~PlayerProperties();
    CustomFree(self);
}

// 0x4c57b0 _ZN7Structs16PlayerProperties8finalizeEv
// thunk: b 0x4c5744, forwards to CharacterProperties::finalize
void PlayerProperties::finalize() {
    CharacterProperties::finalize();
}

// 0x4f7b14 _ZN7Structs16PlayerProperties4readEP11IStreamBase
// thunk: b 0x4f2750, forwards to CharacterProperties::read
void PlayerProperties::read(IStreamBase* stream) {
    CharacterProperties::read(stream);
}

// 0x4c6c54 _ZN7Structs5Pos3DD2Ev
// 0x4c6c58 _ZN7Structs5Pos3DD1Ev
Pos3D::~Pos3D() {
}

// 0x4ce160 _ZN7Structs5Pos3DD0Ev
void DeletingDestructor(Pos3D* self) {
    self->Pos3D::~Pos3D();
    CustomFree(self);
}

// 0x4c6c5c _ZN7Structs5Pos3D8finalizeEv
void Pos3D::finalize() {
}

// 0x4ff4d8 _ZN7Structs5Pos3D4readEP11IStreamBase
void Pos3D::read(IStreamBase* stream) {
    m_w1 = StreamReader::readAs<std::int32_t>(stream);  // +0x4
    m_w2 = StreamReader::readAs<std::int32_t>(stream);  // +0x8
    m_w3 = StreamReader::readAs<std::int32_t>(stream);  // +0xc
}

// 0x4c6c48 _ZN7Structs10ProjectileD2Ev
// 0x4c6c4c _ZN7Structs10ProjectileD1Ev
Projectile::~Projectile() {
}

// 0x4ce17c _ZN7Structs10ProjectileD0Ev
void DeletingDestructor(Projectile* self) {
    self->Projectile::~Projectile();
    CustomFree(self);
}

// 0x4c6c50 _ZN7Structs10Projectile8finalizeEv
void Projectile::finalize() {
}

// 0x4edbb8 _ZN7Structs10Projectile4readEP11IStreamBase
void Projectile::read(IStreamBase* stream) {
    m_w1 = StreamReader::readAs<bool>(stream);  // +0x4
    m_off5 = StreamReader::readAs<bool>(stream);  // +0x5
    m_w2 = StreamReader::readAs<std::int32_t>(stream);  // +0x8
    m_w3 = StreamReader::readAs<std::int32_t>(stream);  // +0xc
    m_w4 = StreamReader::readAs<bool>(stream);  // +0x10
    m_w5 = StreamReader::readAs<std::int32_t>(stream);  // +0x14
    m_w6 = StreamReader::readAs<std::int32_t>(stream);  // +0x18
    m_w7 = StreamReader::readAs<bool>(stream);  // +0x1c
    m_off29 = StreamReader::readAs<bool>(stream);  // +0x1d
    StreamReader::readAs<float>(stream, &m_w8);  // +0x20
    StreamReader::readAs<float>(stream, &m_w9);  // +0x24
    m_w10 = StreamReader::readAs<std::int32_t>(stream);  // +0x28
    m_w11 = StreamReader::readAs<std::int32_t>(stream);  // +0x2c
    m_w12 = StreamReader::readAs<std::int32_t>(stream);  // +0x30
    m_w13 = StreamReader::readAs<bool>(stream);  // +0x34
    m_off53 = StreamReader::readAs<bool>(stream);  // +0x35
    m_w14 = StreamReader::readAs<std::int32_t>(stream);  // +0x38
    m_w15 = StreamReader::readAs<std::int32_t>(stream);  // +0x3c
    StreamReader::readAs<float>(stream, &m_w16);  // +0x40
    StreamReader::readAs<float>(stream, &m_w17);  // +0x44
}

// 0x4c6a08 _ZN7Structs4RectD2Ev
// 0x4c6a0c _ZN7Structs4RectD1Ev
Rect::~Rect() {
}

// 0x4ce41c _ZN7Structs4RectD0Ev
void DeletingDestructor(Rect* self) {
    self->Rect::~Rect();
    CustomFree(self);
}

// 0x4c6a10 _ZN7Structs4Rect8finalizeEv
void Rect::finalize() {
}

// 0x4f25c8 _ZN7Structs4Rect4readEP11IStreamBase
void Rect::read(IStreamBase* stream) {
    m_w1 = StreamReader::readAs<std::int32_t>(stream);  // +0x4
    m_w2 = StreamReader::readAs<std::int32_t>(stream);  // +0x8
    m_w3 = StreamReader::readAs<std::int32_t>(stream);  // +0xc
    m_w4 = StreamReader::readAs<std::int32_t>(stream);  // +0x10
}

// 0x4c6984 _ZN7Structs16RepeatableSpriteD2Ev
// 0x4c6988 _ZN7Structs16RepeatableSpriteD1Ev
RepeatableSprite::~RepeatableSprite() {
}

// 0x4ce470 _ZN7Structs16RepeatableSpriteD0Ev
void DeletingDestructor(RepeatableSprite* self) {
    self->RepeatableSprite::~RepeatableSprite();
    CustomFree(self);
}

// 0x4c698c _ZN7Structs16RepeatableSprite8finalizeEv
void RepeatableSprite::finalize() {
}

// 0x4dbf10 _ZN7Structs16RepeatableSprite4readEP11IStreamBase
void RepeatableSprite::read(IStreamBase* stream) {
    StreamReader::readAs<std::int16_t>(stream, &m_w1);  // +0x4
    m_off6 = StreamReader::readAs<bool>(stream);  // +0x6
    StreamReader::readAs<std::int16_t>(stream, &m_w2);  // +0x8
    m_off10 = StreamReader::readAs<bool>(stream);  // +0xa
    StreamReader::readAs<std::int16_t>(stream, &m_w3);  // +0xc
    StreamReader::readAs<std::int16_t>(stream, &m_off14);  // +0xe
    m_w4 = StreamReader::readAs<bool>(stream);  // +0x10
    StreamReader::readAs<std::int16_t>(stream, &m_off18);  // +0x12
    m_w5 = StreamReader::readAs<bool>(stream);  // +0x14
    StreamReader::readAs<std::int16_t>(stream, &m_off22);  // +0x16
}

// 0x4c6534 _ZN7Structs25ResurrectingGodPropertiesD2Ev
// 0x4c6568 _ZN7Structs25ResurrectingGodPropertiesD1Ev
ResurrectingGodProperties::~ResurrectingGodProperties() {
}

// 0x4ce5dc _ZN7Structs25ResurrectingGodPropertiesD0Ev
void DeletingDestructor(ResurrectingGodProperties* self) {
    self->ResurrectingGodProperties::~ResurrectingGodProperties();
    CustomFree(self);
}

// 0x4c659c _ZN7Structs25ResurrectingGodProperties8finalizeEv
// thunk: b 0x4c5744, forwards to CharacterProperties::finalize
void ResurrectingGodProperties::finalize() {
    CharacterProperties::finalize();
}

// 0x4f7a90 _ZN7Structs25ResurrectingGodProperties4readEP11IStreamBase
// thunk: b 0x4f2750, forwards to CharacterProperties::read
void ResurrectingGodProperties::read(IStreamBase* stream) {
    CharacterProperties::read(stream);
}

// 0x4c63f0 _ZN7Structs19RootTrollPropertiesD2Ev
// 0x4c6424 _ZN7Structs19RootTrollPropertiesD1Ev
RootTrollProperties::~RootTrollProperties() {
}

// 0x4ce630 _ZN7Structs19RootTrollPropertiesD0Ev
void DeletingDestructor(RootTrollProperties* self) {
    self->RootTrollProperties::~RootTrollProperties();
    CustomFree(self);
}

// 0x4c6458 _ZN7Structs19RootTrollProperties8finalizeEv
// thunk: b 0x4c5744, forwards to CharacterProperties::finalize
void RootTrollProperties::finalize() {
    CharacterProperties::finalize();
}

// 0x4f7a9c _ZN7Structs19RootTrollProperties4readEP11IStreamBase
// thunk: b 0x4f2750, forwards to CharacterProperties::read
void RootTrollProperties::read(IStreamBase* stream) {
    CharacterProperties::read(stream);
}

// 0x4c5aa8 _ZN7Structs18SandWormPropertiesD2Ev
// 0x4c5adc _ZN7Structs18SandWormPropertiesD1Ev
SandWormProperties::~SandWormProperties() {
}

// 0x4ce898 _ZN7Structs18SandWormPropertiesD0Ev
void DeletingDestructor(SandWormProperties* self) {
    self->SandWormProperties::~SandWormProperties();
    CustomFree(self);
}

// 0x4c5b10 _ZN7Structs18SandWormProperties8finalizeEv
// thunk: b 0x4c5744, forwards to CharacterProperties::finalize
void SandWormProperties::finalize() {
    CharacterProperties::finalize();
}

// 0x4f7af4 _ZN7Structs18SandWormProperties4readEP11IStreamBase
// thunk: b 0x4f2750, forwards to CharacterProperties::read
void SandWormProperties::read(IStreamBase* stream) {
    CharacterProperties::read(stream);
}

// 0x4c57b4 _ZN7Structs18SkeletonPropertiesD2Ev
// 0x4c57e8 _ZN7Structs18SkeletonPropertiesD1Ev
SkeletonProperties::~SkeletonProperties() {
}

// 0x4ce95c _ZN7Structs18SkeletonPropertiesD0Ev
void DeletingDestructor(SkeletonProperties* self) {
    self->SkeletonProperties::~SkeletonProperties();
    CustomFree(self);
}

// 0x4c581c _ZN7Structs18SkeletonProperties8finalizeEv
// thunk: b 0x4c5744, forwards to CharacterProperties::finalize
void SkeletonProperties::finalize() {
    CharacterProperties::finalize();
}

// 0x4f7b10 _ZN7Structs18SkeletonProperties4readEP11IStreamBase
// thunk: b 0x4f2750, forwards to CharacterProperties::read
void SkeletonProperties::read(IStreamBase* stream) {
    CharacterProperties::read(stream);
}

// 0x4d1080 _ZN7Structs5SkillD2Ev
// 0x4d1004 _ZN7Structs5SkillD1Ev
Skill::~Skill() {
    if (m_w4 != nullptr) CustomFree(m_w4);  // +0x10
    if (m_w10 != nullptr) CustomFree(m_w10);  // +0x28
    if (m_w15 != nullptr) CustomFree(m_w15);  // +0x3c
}

// 0x4d1064 _ZN7Structs5SkillD0Ev
void DeletingDestructor(Skill* self) {
    self->Skill::~Skill();
    CustomFree(self);
}

// 0x4d0fa4 _ZN7Structs5Skill8finalizeEv
void Skill::finalize() {
    if (m_w4 != nullptr) {
        CustomFree(m_w4);  // +0x10
        m_w3 = 0;  // +0xc
        m_w4 = nullptr;
    }
    if (m_w10 != nullptr) {
        CustomFree(m_w10);  // +0x28
        m_w9 = 0;  // +0x24
        m_w10 = nullptr;
    }
    if (m_w15 != nullptr) {
        CustomFree(m_w15);  // +0x3c
        m_w14 = 0;  // +0x38
        m_w15 = nullptr;
    }
}

// 0x4d1164 _ZN7Structs9SkillListD2Ev
// 0x4d1108 _ZN7Structs9SkillListD1Ev
SkillList::~SkillList() {
    if (m_w2 != nullptr) CustomFree(m_w2);  // +0x8
}

// 0x4d1148 _ZN7Structs9SkillListD0Ev
void DeletingDestructor(SkillList* self) {
    self->SkillList::~SkillList();
    CustomFree(self);
}

// 0x4d10e0 _ZN7Structs9SkillList8finalizeEv
void SkillList::finalize() {
    if (m_w2 != nullptr) {
        CustomFree(m_w2);  // +0x8
        m_w1 = 0;  // +0x4
        m_w2 = nullptr;
    }
}

// 0x4c5c58 _ZN7Structs15SlimePropertiesD2Ev
// 0x4c5c8c _ZN7Structs15SlimePropertiesD1Ev
SlimeProperties::~SlimeProperties() {
}

// 0x4ce828 _ZN7Structs15SlimePropertiesD0Ev
void DeletingDestructor(SlimeProperties* self) {
    self->SlimeProperties::~SlimeProperties();
    CustomFree(self);
}

// 0x4c5cc0 _ZN7Structs15SlimeProperties8finalizeEv
// thunk: b 0x4c5744, forwards to CharacterProperties::finalize
void SlimeProperties::finalize() {
    CharacterProperties::finalize();
}

// 0x4f7ae4 _ZN7Structs15SlimeProperties4readEP11IStreamBase
// thunk: b 0x4f2750, forwards to CharacterProperties::read
void SlimeProperties::read(IStreamBase* stream) {
    CharacterProperties::read(stream);
}

// 0x4c6240 _ZN7Structs20SmallPlantPropertiesD2Ev
// 0x4c6274 _ZN7Structs20SmallPlantPropertiesD1Ev
SmallPlantProperties::~SmallPlantProperties() {
}

// 0x4ce6a0 _ZN7Structs20SmallPlantPropertiesD0Ev
void DeletingDestructor(SmallPlantProperties* self) {
    self->SmallPlantProperties::~SmallPlantProperties();
    CustomFree(self);
}

// 0x4c62a8 _ZN7Structs20SmallPlantProperties8finalizeEv
// thunk: b 0x4c5744, forwards to CharacterProperties::finalize
void SmallPlantProperties::finalize() {
    CharacterProperties::finalize();
}

// 0x4f7aac _ZN7Structs20SmallPlantProperties4readEP11IStreamBase
// thunk: b 0x4f2750, forwards to CharacterProperties::read
void SmallPlantProperties::read(IStreamBase* stream) {
    CharacterProperties::read(stream);
}

// 0x4c6090 _ZN7Structs18SnowWolfPropertiesD2Ev
// 0x4c60c4 _ZN7Structs18SnowWolfPropertiesD1Ev
SnowWolfProperties::~SnowWolfProperties() {
}

// 0x4ce710 _ZN7Structs18SnowWolfPropertiesD0Ev
void DeletingDestructor(SnowWolfProperties* self) {
    self->SnowWolfProperties::~SnowWolfProperties();
    CustomFree(self);
}

// 0x4c60f8 _ZN7Structs18SnowWolfProperties8finalizeEv
// thunk: b 0x4c5744, forwards to CharacterProperties::finalize
void SnowWolfProperties::finalize() {
    CharacterProperties::finalize();
}

// 0x4f7abc _ZN7Structs18SnowWolfProperties4readEP11IStreamBase
// thunk: b 0x4f2750, forwards to CharacterProperties::read
void SnowWolfProperties::read(IStreamBase* stream) {
    CharacterProperties::read(stream);
}

// 0x4c7c14 _ZN7Structs12SoundAutoGenD2Ev
// 0x4c7c18 _ZN7Structs12SoundAutoGenD1Ev
SoundAutoGen::~SoundAutoGen() {
}

// 0x4cdce4 _ZN7Structs12SoundAutoGenD0Ev
void DeletingDestructor(SoundAutoGen* self) {
    self->SoundAutoGen::~SoundAutoGen();
    CustomFree(self);
}

// 0x4c7c1c _ZN7Structs12SoundAutoGen8finalizeEv
void SoundAutoGen::finalize() {
}

// 0x502af4 _ZN7Structs12SoundAutoGen4readEP11IStreamBase
void SoundAutoGen::read(IStreamBase* stream) {
    m_w1 = StreamReader::readAs<std::int32_t>(stream);  // +0x4
    m_w2 = StreamReader::readAs<std::int32_t>(stream);  // +0x8
}

// 0x4c7c08 _ZN7Structs9SoundBankD2Ev
// 0x4c7c0c _ZN7Structs9SoundBankD1Ev
SoundBank::~SoundBank() {
}

// 0x4cdd1c _ZN7Structs9SoundBankD0Ev
void DeletingDestructor(SoundBank* self) {
    self->SoundBank::~SoundBank();
    CustomFree(self);
}

// 0x4c7c10 _ZN7Structs9SoundBank8finalizeEv
void SoundBank::finalize() {
}

// 0x502bc8 _ZN7Structs9SoundBank4readEP11IStreamBase
void SoundBank::read(IStreamBase* stream) {
    m_w1 = StreamReader::readAs<std::int32_t>(stream);  // +0x4
    m_w2 = StreamReader::readAs<std::int32_t>(stream);  // +0x8
}

// 0x4c7c20 _ZN7Structs17SoundEventAutoGenD2Ev
// 0x4c7c54 _ZN7Structs17SoundEventAutoGenD1Ev
SoundEventAutoGen::~SoundEventAutoGen() {
}

// 0x4cdd00 _ZN7Structs17SoundEventAutoGenD0Ev
void DeletingDestructor(SoundEventAutoGen* self) {
    self->SoundEventAutoGen::~SoundEventAutoGen();
    CustomFree(self);
}

// 0x4c7c88 _ZN7Structs17SoundEventAutoGen8finalizeEv
// thunk: b 0x4c7c1c, forwards to SoundAutoGen::finalize
void SoundEventAutoGen::finalize() {
    SoundAutoGen::finalize();
}

// 0x502bc4 _ZN7Structs17SoundEventAutoGen4readEP11IStreamBase
// thunk: b 0x502af4, forwards to SoundAutoGen::read
void SoundEventAutoGen::read(IStreamBase* stream) {
    SoundAutoGen::read(stream);
}

// 0x4c7ca4 _ZN7Structs9SpawnInfoD2Ev
// 0x4c7ca8 _ZN7Structs9SpawnInfoD1Ev
SpawnInfo::~SpawnInfo() {
}

// 0x4cdc90 _ZN7Structs9SpawnInfoD0Ev
void DeletingDestructor(SpawnInfo* self) {
    self->SpawnInfo::~SpawnInfo();
    CustomFree(self);
}

// 0x4c7cac _ZN7Structs9SpawnInfo8finalizeEv
void SpawnInfo::finalize() {
}

// 0x502c98 _ZN7Structs9SpawnInfo4readEP11IStreamBase
void SpawnInfo::read(IStreamBase* stream) {
    m_w1 = StreamReader::readAs<std::int32_t>(stream);  // +0x4
    m_w2 = StreamReader::readAs<std::int32_t>(stream);  // +0x8
    m_w3 = StreamReader::readAs<std::int32_t>(stream);  // +0xc
}

// 0x4c5ee0 _ZN7Structs16SpiderPropertiesD2Ev
// 0x4c5f14 _ZN7Structs16SpiderPropertiesD1Ev
SpiderProperties::~SpiderProperties() {
}

// 0x4ce780 _ZN7Structs16SpiderPropertiesD0Ev
void DeletingDestructor(SpiderProperties* self) {
    self->SpiderProperties::~SpiderProperties();
    CustomFree(self);
}

// 0x4c5f48 _ZN7Structs16SpiderProperties8finalizeEv
// thunk: b 0x4c5744, forwards to CharacterProperties::finalize
void SpiderProperties::finalize() {
    CharacterProperties::finalize();
}

// 0x4f7acc _ZN7Structs16SpiderProperties4readEP11IStreamBase
// thunk: b 0x4f2750, forwards to CharacterProperties::read
void SpiderProperties::read(IStreamBase* stream) {
    CharacterProperties::read(stream);
}

// 0x4c696c _ZN7Structs20StatAutoAssignTargetD2Ev
// 0x4c6970 _ZN7Structs20StatAutoAssignTargetD1Ev
StatAutoAssignTarget::~StatAutoAssignTarget() {
}

// 0x4ce4a8 _ZN7Structs20StatAutoAssignTargetD0Ev
void DeletingDestructor(StatAutoAssignTarget* self) {
    self->StatAutoAssignTarget::~StatAutoAssignTarget();
    CustomFree(self);
}

// 0x4c6974 _ZN7Structs20StatAutoAssignTarget8finalizeEv
void StatAutoAssignTarget::finalize() {
}

// 0x4ef520 _ZN7Structs20StatAutoAssignTarget4readEP11IStreamBase
void StatAutoAssignTarget::read(IStreamBase* stream) {
    m_w1 = StreamReader::readAs<std::int32_t>(stream);  // +0x4
    m_w2 = StreamReader::readAs<std::int32_t>(stream);  // +0x8
    m_w3 = StreamReader::readAs<std::int32_t>(stream);  // +0xc
}

// 0x4c5a3c _ZN7Structs19SwampKingPropertiesD2Ev
// 0x4c5a70 _ZN7Structs19SwampKingPropertiesD1Ev
SwampKingProperties::~SwampKingProperties() {
}

// 0x4ce8b4 _ZN7Structs19SwampKingPropertiesD0Ev
void DeletingDestructor(SwampKingProperties* self) {
    self->SwampKingProperties::~SwampKingProperties();
    CustomFree(self);
}

// 0x4c5aa4 _ZN7Structs19SwampKingProperties8finalizeEv
// thunk: b 0x4c5744, forwards to CharacterProperties::finalize
void SwampKingProperties::finalize() {
    CharacterProperties::finalize();
}

// 0x4f7af8 _ZN7Structs19SwampKingProperties4readEP11IStreamBase
// thunk: b 0x4f2750, forwards to CharacterProperties::read
void SwampKingProperties::read(IStreamBase* stream) {
    CharacterProperties::read(stream);
}

// 0x4c6c24 _ZN7Structs10TileOffsetD2Ev
// 0x4c6c28 _ZN7Structs10TileOffsetD1Ev
TileOffset::~TileOffset() {
}

// 0x4ce1d0 _ZN7Structs10TileOffsetD0Ev
void DeletingDestructor(TileOffset* self) {
    self->TileOffset::~TileOffset();
    CustomFree(self);
}

// 0x4c6c2c _ZN7Structs10TileOffset8finalizeEv
void TileOffset::finalize() {
}

// 0x4dbbcc _ZN7Structs10TileOffset4readEP11IStreamBase
void TileOffset::read(IStreamBase* stream) {
    StreamReader::readAs<std::int16_t>(stream, &m_w1);  // +0x4
    StreamReader::readAs<std::int16_t>(stream, &m_off6);  // +0x6
}

// 0x4c6bc4 _ZN7Structs12TriggerPlateD2Ev
// 0x4c6bc8 _ZN7Structs12TriggerPlateD1Ev
TriggerPlate::~TriggerPlate() {
}

// 0x4ce2b0 _ZN7Structs12TriggerPlateD0Ev
void DeletingDestructor(TriggerPlate* self) {
    self->TriggerPlate::~TriggerPlate();
    CustomFree(self);
}

// 0x4c6bcc _ZN7Structs12TriggerPlate8finalizeEv
void TriggerPlate::finalize() {
}

// 0x4ed35c _ZN7Structs12TriggerPlate4readEP11IStreamBase
void TriggerPlate::read(IStreamBase* stream) {
    m_w1 = StreamReader::readAs<std::int32_t>(stream);  // +0x4
    m_w2 = StreamReader::readAs<bool>(stream);  // +0x8
    m_w3 = StreamReader::readAs<std::int32_t>(stream);  // +0xc
    m_w4 = StreamReader::readAs<std::int32_t>(stream);  // +0x10
    m_w5 = StreamReader::readAs<std::int32_t>(stream);  // +0x14
    m_w6 = StreamReader::readAs<std::int32_t>(stream);  // +0x18
    m_w7 = StreamReader::readAs<std::int32_t>(stream);  // +0x1c
}

// 0x4c5b80 _ZN7Structs19TrollKingPropertiesD2Ev
// 0x4c5bb4 _ZN7Structs19TrollKingPropertiesD1Ev
TrollKingProperties::~TrollKingProperties() {
}

// 0x4ce860 _ZN7Structs19TrollKingPropertiesD0Ev
void DeletingDestructor(TrollKingProperties* self) {
    self->TrollKingProperties::~TrollKingProperties();
    CustomFree(self);
}

// 0x4c5be8 _ZN7Structs19TrollKingProperties8finalizeEv
// thunk: b 0x4c5744, forwards to CharacterProperties::finalize
void TrollKingProperties::finalize() {
    CharacterProperties::finalize();
}

// 0x4f7aec _ZN7Structs19TrollKingProperties4readEP11IStreamBase
// thunk: b 0x4f2750, forwards to CharacterProperties::read
void TrollKingProperties::read(IStreamBase* stream) {
    CharacterProperties::read(stream);
}

// 0x4c5b14 _ZN7Structs15TrollPropertiesD2Ev
// 0x4c5b48 _ZN7Structs15TrollPropertiesD1Ev
TrollProperties::~TrollProperties() {
}

// 0x4ce87c _ZN7Structs15TrollPropertiesD0Ev
void DeletingDestructor(TrollProperties* self) {
    self->TrollProperties::~TrollProperties();
    CustomFree(self);
}

// 0x4c5b7c _ZN7Structs15TrollProperties8finalizeEv
// thunk: b 0x4c5744, forwards to CharacterProperties::finalize
void TrollProperties::finalize() {
    CharacterProperties::finalize();
}

// 0x4f7af0 _ZN7Structs15TrollProperties4readEP11IStreamBase
// thunk: b 0x4f2750, forwards to CharacterProperties::read
void TrollProperties::read(IStreamBase* stream) {
    CharacterProperties::read(stream);
}

// 0x4c7cb0 _ZN7Structs6TrophyD2Ev
// 0x4c7cb4 _ZN7Structs6TrophyD1Ev
Trophy::~Trophy() {
}

// 0x4cdc74 _ZN7Structs6TrophyD0Ev
void DeletingDestructor(Trophy* self) {
    self->Trophy::~Trophy();
    CustomFree(self);
}

// 0x4c7cb8 _ZN7Structs6Trophy8finalizeEv
void Trophy::finalize() {
}

// 0x5060e0 _ZN7Structs6Trophy4readEP11IStreamBase
void Trophy::read(IStreamBase* stream) {
    m_w1 = StreamReader::readAs<std::int32_t>(stream);  // +0x4
    m_w2 = StreamReader::readAs<std::int32_t>(stream);  // +0x8
    m_w3 = StreamReader::readAs<std::int32_t>(stream);  // +0xc
    m_w4 = StreamReader::readAs<std::int32_t>(stream);  // +0x10
    m_w5 = StreamReader::readAs<std::int32_t>(stream);  // +0x14
    m_w6 = StreamReader::readAs<std::int32_t>(stream);  // +0x18
    m_w7 = StreamReader::readAs<std::int32_t>(stream);  // +0x1c
}

// 0x4c5cc4 _ZN7Structs18Ugly_DogPropertiesD2Ev
// 0x4c5cf8 _ZN7Structs18Ugly_DogPropertiesD1Ev
Ugly_DogProperties::~Ugly_DogProperties() {
}

// 0x4ce80c _ZN7Structs18Ugly_DogPropertiesD0Ev
void DeletingDestructor(Ugly_DogProperties* self) {
    self->Ugly_DogProperties::~Ugly_DogProperties();
    CustomFree(self);
}

// 0x4c5d2c _ZN7Structs18Ugly_DogProperties8finalizeEv
// thunk: b 0x4c5744, forwards to CharacterProperties::finalize
void Ugly_DogProperties::finalize() {
    CharacterProperties::finalize();
}

// 0x4f7ae0 _ZN7Structs18Ugly_DogProperties4readEP11IStreamBase
// thunk: b 0x4f2750, forwards to CharacterProperties::read
void Ugly_DogProperties::read(IStreamBase* stream) {
    CharacterProperties::read(stream);
}

// 0x4c5d30 _ZN7Structs22Ugly_Dog_BigPropertiesD2Ev
// 0x4c5d64 _ZN7Structs22Ugly_Dog_BigPropertiesD1Ev
Ugly_Dog_BigProperties::~Ugly_Dog_BigProperties() {
}

// 0x4ce7f0 _ZN7Structs22Ugly_Dog_BigPropertiesD0Ev
void DeletingDestructor(Ugly_Dog_BigProperties* self) {
    self->Ugly_Dog_BigProperties::~Ugly_Dog_BigProperties();
    CustomFree(self);
}

// 0x4c5d98 _ZN7Structs22Ugly_Dog_BigProperties8finalizeEv
// thunk: b 0x4c5744, forwards to CharacterProperties::finalize
void Ugly_Dog_BigProperties::finalize() {
    CharacterProperties::finalize();
}

// 0x4f7adc _ZN7Structs22Ugly_Dog_BigProperties4readEP11IStreamBase
// thunk: b 0x4f2750, forwards to CharacterProperties::read
void Ugly_Dog_BigProperties::read(IStreamBase* stream) {
    CharacterProperties::read(stream);
}

// 0x4c5bec _ZN7Structs15WitchPropertiesD2Ev
// 0x4c5c20 _ZN7Structs15WitchPropertiesD1Ev
WitchProperties::~WitchProperties() {
}

// 0x4ce844 _ZN7Structs15WitchPropertiesD0Ev
void DeletingDestructor(WitchProperties* self) {
    self->WitchProperties::~WitchProperties();
    CustomFree(self);
}

// 0x4c5c54 _ZN7Structs15WitchProperties8finalizeEv
// thunk: b 0x4c5744, forwards to CharacterProperties::finalize
void WitchProperties::finalize() {
    CharacterProperties::finalize();
}

// 0x4f7ae8 _ZN7Structs15WitchProperties4readEP11IStreamBase
// thunk: b 0x4f2750, forwards to CharacterProperties::read
void WitchProperties::read(IStreamBase* stream) {
    CharacterProperties::read(stream);
}

// 0x4cee08 _ZN7Structs14WldMapLocationD2Ev
// 0x4cedac _ZN7Structs14WldMapLocationD1Ev
WldMapLocation::~WldMapLocation() {
    if (m_w4 != nullptr) CustomFree(m_w4);  // +0x10
}

// 0x4cedec _ZN7Structs14WldMapLocationD0Ev
void DeletingDestructor(WldMapLocation* self) {
    self->WldMapLocation::~WldMapLocation();
    CustomFree(self);
}

// 0x4ced84 _ZN7Structs14WldMapLocation8finalizeEv
void WldMapLocation::finalize() {
    if (m_w4 != nullptr) {
        CustomFree(m_w4);  // +0x10
        m_w3 = 0;  // +0xc
        m_w4 = nullptr;
    }
}

// 0x4c8ba4 _ZN7Structs14WorldMapLockerD2Ev
// 0x4c8ba8 _ZN7Structs14WorldMapLockerD1Ev
WorldMapLocker::~WorldMapLocker() {
}

// 0x4cd830 _ZN7Structs14WorldMapLockerD0Ev
void DeletingDestructor(WorldMapLocker* self) {
    self->WorldMapLocker::~WorldMapLocker();
    CustomFree(self);
}

// 0x4c8bac _ZN7Structs14WorldMapLocker8finalizeEv
void WorldMapLocker::finalize() {
}

// 0x4c6024 _ZN7Structs14YetiPropertiesD2Ev
// 0x4c6058 _ZN7Structs14YetiPropertiesD1Ev
YetiProperties::~YetiProperties() {
}

// 0x4ce72c _ZN7Structs14YetiPropertiesD0Ev
void DeletingDestructor(YetiProperties* self) {
    self->YetiProperties::~YetiProperties();
    CustomFree(self);
}

// 0x4c608c _ZN7Structs14YetiProperties8finalizeEv
// thunk: b 0x4c5744, forwards to CharacterProperties::finalize
void YetiProperties::finalize() {
    CharacterProperties::finalize();
}

// 0x4f7ac0 _ZN7Structs14YetiProperties4readEP11IStreamBase
// thunk: b 0x4f2750, forwards to CharacterProperties::read
void YetiProperties::read(IStreamBase* stream) {
    CharacterProperties::read(stream);
}

// 0x4c7cbc _ZN7Structs11v2ConditionD2Ev
// 0x4c7cc0 _ZN7Structs11v2ConditionD1Ev
v2Condition::~v2Condition() {
}

// 0x4cd980 _ZN7Structs11v2ConditionD0Ev
void DeletingDestructor(v2Condition* self) {
    self->v2Condition::~v2Condition();
    CustomFree(self);
}

// 0x4c7cc4 _ZN7Structs11v2Condition8finalizeEv
void v2Condition::finalize() {
}

// 0x4c7cc8 _ZN7Structs15v2ConditionStubD2Ev
// 0x4c7cfc _ZN7Structs15v2ConditionStubD1Ev
v2ConditionStub::~v2ConditionStub() {
}

// 0x4cdc58 _ZN7Structs15v2ConditionStubD0Ev
void DeletingDestructor(v2ConditionStub* self) {
    self->v2ConditionStub::~v2ConditionStub();
    CustomFree(self);
}

// 0x4c7d30 _ZN7Structs15v2ConditionStub8finalizeEv
// thunk: b 0x4c7cc4, forwards to v2Condition::finalize
void v2ConditionStub::finalize() {
    v2Condition::finalize();
}

// 0x4c8838 _ZN7Structs16v2QuestObjectiveD2Ev
// 0x4c883c _ZN7Structs16v2QuestObjectiveD1Ev
v2QuestObjective::~v2QuestObjective() {
}

// 0x4cd84c _ZN7Structs16v2QuestObjectiveD0Ev
void DeletingDestructor(v2QuestObjective* self) {
    self->v2QuestObjective::~v2QuestObjective();
    CustomFree(self);
}

// 0x4c8840 _ZN7Structs16v2QuestObjective8finalizeEv
void v2QuestObjective::finalize() {
}

// 0x4d0114 _ZN7Structs20v2QuestObjectiveStubD2Ev
// 0x4d00a0 _ZN7Structs20v2QuestObjectiveStubD1Ev
v2QuestObjectiveStub::~v2QuestObjectiveStub() {
    if (m_w5 != nullptr) CustomFree(m_w5);  // +0x14
    if (m_w7 != nullptr) CustomFree(m_w7);  // +0x1c
}

// 0x4d00f8 _ZN7Structs20v2QuestObjectiveStubD0Ev
void DeletingDestructor(v2QuestObjectiveStub* self) {
    self->v2QuestObjectiveStub::~v2QuestObjectiveStub();
    CustomFree(self);
}

// 0x4d0008 _ZN7Structs20v2QuestObjectiveStub8finalizeEv
void v2QuestObjectiveStub::finalize() {
    if (m_w5 != nullptr) {
        CustomFree(m_w5);  // +0x14
        m_w4 = 0;  // +0x10
        m_w5 = nullptr;
    }
    if (m_w7 != nullptr) {
        CustomFree(m_w7);  // +0x1c
        m_w6 = 0;  // +0x18
        m_w7 = nullptr;
    }
    v2QuestObjective::finalize();
}

// 0x4c87c0 _ZN7Structs13v2QuestRewardD2Ev
// 0x4c87c4 _ZN7Structs13v2QuestRewardD1Ev
v2QuestReward::~v2QuestReward() {
}

// 0x4cd868 _ZN7Structs13v2QuestRewardD0Ev
void DeletingDestructor(v2QuestReward* self) {
    self->v2QuestReward::~v2QuestReward();
    CustomFree(self);
}

// 0x4c87c8 _ZN7Structs13v2QuestReward8finalizeEv
void v2QuestReward::finalize() {
}

// 0x4c87cc _ZN7Structs17v2QuestRewardStubD2Ev
// 0x4c8800 _ZN7Structs17v2QuestRewardStubD1Ev
v2QuestRewardStub::~v2QuestRewardStub() {
}

// 0x4cd964 _ZN7Structs17v2QuestRewardStubD0Ev
void DeletingDestructor(v2QuestRewardStub* self) {
    self->v2QuestRewardStub::~v2QuestRewardStub();
    CustomFree(self);
}

// 0x4c8834 _ZN7Structs17v2QuestRewardStub8finalizeEv
// thunk: b 0x4c87c8, forwards to v2QuestReward::finalize
void v2QuestRewardStub::finalize() {
    v2QuestReward::finalize();
}

// 0x4d08a4 _ZN7Structs14v2QuestScriptsD2Ev
// 0x4d03cc _ZN7Structs14v2QuestScriptsD1Ev
v2QuestScripts::~v2QuestScripts() {
    if (m_w2 != nullptr) CustomFree(m_w2);  // +0x8
    if (m_w4 != nullptr) CustomFree(m_w4);  // +0x10
    if (m_w6 != nullptr) CustomFree(m_w6);  // +0x18
    if (m_w8 != nullptr) CustomFree(m_w8);  // +0x20
    if (m_w10 != nullptr) CustomFree(m_w10);  // +0x28
    if (m_w12 != nullptr) CustomFree(m_w12);  // +0x30
    if (m_w14 != nullptr) CustomFree(m_w14);  // +0x38
    if (m_w16 != nullptr) CustomFree(m_w16);  // +0x40
    if (m_w18 != nullptr) CustomFree(m_w18);  // +0x48
    if (m_w20 != nullptr) CustomFree(m_w20);  // +0x50
    if (m_w22 != nullptr) CustomFree(m_w22);  // +0x58
    if (m_w24 != nullptr) CustomFree(m_w24);  // +0x60
    if (m_w26 != nullptr) CustomFree(m_w26);  // +0x68
    if (m_w28 != nullptr) CustomFree(m_w28);  // +0x70
}

// 0x4d0888 _ZN7Structs14v2QuestScriptsD0Ev
void DeletingDestructor(v2QuestScripts* self) {
    self->v2QuestScripts::~v2QuestScripts();
    CustomFree(self);
}

// 0x4d0238 _ZN7Structs14v2QuestScripts8finalizeEv
void v2QuestScripts::finalize() {
    if (m_w2 != nullptr) {
        CustomFree(m_w2);  // +0x8
        m_w1 = 0;  // +0x4
        m_w2 = nullptr;
    }
    if (m_w4 != nullptr) {
        CustomFree(m_w4);  // +0x10
        m_w3 = 0;  // +0xc
        m_w4 = nullptr;
    }
    if (m_w6 != nullptr) {
        CustomFree(m_w6);  // +0x18
        m_w5 = 0;  // +0x14
        m_w6 = nullptr;
    }
    if (m_w8 != nullptr) {
        CustomFree(m_w8);  // +0x20
        m_w7 = 0;  // +0x1c
        m_w8 = nullptr;
    }
    if (m_w10 != nullptr) {
        CustomFree(m_w10);  // +0x28
        m_w9 = 0;  // +0x24
        m_w10 = nullptr;
    }
    if (m_w12 != nullptr) {
        CustomFree(m_w12);  // +0x30
        m_w11 = 0;  // +0x2c
        m_w12 = nullptr;
    }
    if (m_w14 != nullptr) {
        CustomFree(m_w14);  // +0x38
        m_w13 = 0;  // +0x34
        m_w14 = nullptr;
    }
    if (m_w16 != nullptr) {
        CustomFree(m_w16);  // +0x40
        m_w15 = 0;  // +0x3c
        m_w16 = nullptr;
    }
    if (m_w18 != nullptr) {
        CustomFree(m_w18);  // +0x48
        m_w17 = 0;  // +0x44
        m_w18 = nullptr;
    }
    if (m_w20 != nullptr) {
        CustomFree(m_w20);  // +0x50
        m_w19 = 0;  // +0x4c
        m_w20 = nullptr;
    }
    if (m_w22 != nullptr) {
        CustomFree(m_w22);  // +0x58
        m_w21 = 0;  // +0x54
        m_w22 = nullptr;
    }
    if (m_w24 != nullptr) {
        CustomFree(m_w24);  // +0x60
        m_w23 = 0;  // +0x5c
        m_w24 = nullptr;
    }
    if (m_w26 != nullptr) {
        CustomFree(m_w26);  // +0x68
        m_w25 = 0;  // +0x64
        m_w26 = nullptr;
    }
    if (m_w28 != nullptr) {
        CustomFree(m_w28);  // +0x70
        m_w27 = 0;  // +0x6c
        m_w28 = nullptr;
    }
}

}  // namespace Structs
