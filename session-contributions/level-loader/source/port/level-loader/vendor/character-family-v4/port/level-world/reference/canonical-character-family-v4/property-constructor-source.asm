_ZN14CharPropertiesC2Ev
003df084 push     {r4, r5, r6, lr}
003df088 ldr      r5, [pc, #0x74]
003df08c ldr      r3, [pc, #0x74]
003df090 ldr      r1, [pc, #0x74]
003df094 add      r5, pc, r5
003df098 ldr      r3, [r5, r3]
003df09c ldr      r1, [r5, r1]
003df0a0 add      r2, r0, #0xe10
003df0a4 add      r2, r2, #8
003df0a8 add      r1, r1, #8
003df0ac add      ip, r3, #8
003df0b0 mov      r3, #0
003df0b4 str      ip, [r0]
003df0b8 str      r2, [r0, #0xe24]
003df0bc str      r2, [r0, #0xe20]
003df0c0 str      r1, [r0, #0xa94]
003df0c4 str      r3, [r0, #0xe30]
003df0c8 str      r3, [r0, #4]
003df0cc str      r1, [r0, #8]
003df0d0 str      r1, [r0, #0x38c]
003df0d4 str      r1, [r0, #0x710]
003df0d8 str      r3, [r0, #0xe1c]
003df0dc strb     r3, [r0, #0xe18]
003df0e0 str      r3, [r0, #0xe28]
003df0e4 mov      r4, r0
003df0e8 bl       #0x3defc4 ; _ZN14CharProperties18ResetAllPropertiesEv
003df0ec ldr      r3, [pc, #0x1c]
003df0f0 mov      r0, r4
003df0f4 ldr      r1, [r5, r3]
003df0f8 bl       #0x3def34 ; _ZN14CharProperties16_ResetPropertiesERN7Structs19CharacterPropertiesE
003df0fc mov      r0, r4
003df100 pop      {r4, r5, r6, pc}
003df104 ldrsheq  r5, [fp], #-0x9c
003df108 ldrdeq   r3, r4, [r0], -r0
003df10c andeq    r2, r0, ip, lsl #19
003df110 andeq    r1, r0, ip, asr #32
_ZN14CharPropertiesC1Ev
003deff4 push     {r4, r5, r6, lr}
003deff8 ldr      r5, [pc, #0x74]
003deffc ldr      r3, [pc, #0x74]
003df000 ldr      r1, [pc, #0x74]
003df004 add      r5, pc, r5
003df008 ldr      r3, [r5, r3]
003df00c ldr      r1, [r5, r1]
003df010 add      r2, r0, #0xe10
003df014 add      r2, r2, #8
003df018 add      r1, r1, #8
003df01c add      ip, r3, #8
003df020 mov      r3, #0
003df024 str      ip, [r0]
003df028 str      r2, [r0, #0xe24]
003df02c str      r2, [r0, #0xe20]
003df030 str      r1, [r0, #0xa94]
003df034 str      r3, [r0, #0xe30]
003df038 str      r3, [r0, #4]
003df03c str      r1, [r0, #8]
003df040 str      r1, [r0, #0x38c]
003df044 str      r1, [r0, #0x710]
003df048 str      r3, [r0, #0xe1c]
003df04c strb     r3, [r0, #0xe18]
003df050 str      r3, [r0, #0xe28]
003df054 mov      r4, r0
003df058 bl       #0x3defc4 ; _ZN14CharProperties18ResetAllPropertiesEv
003df05c ldr      r3, [pc, #0x1c]
003df060 mov      r0, r4
003df064 ldr      r1, [r5, r3]
003df068 bl       #0x3def34 ; _ZN14CharProperties16_ResetPropertiesERN7Structs19CharacterPropertiesE
003df06c mov      r0, r4
003df070 pop      {r4, r5, r6, pc}
003df074 subseq   r5, fp, ip, lsl #21
003df078 ldrdeq   r3, r4, [r0], -r0
003df07c andeq    r2, r0, ip, lsl #19
003df080 andeq    r1, r0, ip, asr #32
_ZN9Character18SafeGetCharPropsIdEv
003b3d38 push     {r4, r5, r6, r7, r8, sl, lr}
003b3d3c movw     r6, #0x13c8
003b3d40 ldrsh    r3, [r0, r6]
003b3d44 ldr      r7, [pc, #0x294]
003b3d48 mov      r4, r0
003b3d4c cmn      r3, #1
003b3d50 add      r7, pc, r7
003b3d54 sub      sp, sp, #0xc
003b3d58 movne    r0, r3
003b3d5c beq      #0x3b3d68
003b3d60 add      sp, sp, #0xc
003b3d64 pop      {r4, r5, r6, r7, r8, sl, pc}
003b3d68 ldr      r3, [r4]
003b3d6c mov      lr, pc
003b3d70 ldr      pc, [r3, #0x28]
003b3d74 subs     sl, r0, #0
003b3d78 bne      #0x3b3dcc
003b3d7c movw     r3, #0x13a8
003b3d80 ldr      r2, [r4, r3]
003b3d84 movw     r3, #0x13ac
003b3d88 ldr      r3, [r4, r3]
003b3d8c cmp      r2, r3
003b3d90 beq      #0x3b3f20
003b3d94 mov      r0, r4
003b3d98 bl       #0x3b36ec ; _ZN9Character26SafeGetCharPropsTemplateIdEv
003b3d9c cmp      r0, #0
003b3da0 blt      #0x3b3dc4
003b3da4 ldr      r3, [pc, #0x238]
003b3da8 mov      r5, #0xc
003b3dac ldr      r3, [r7, r3]
003b3db0 ldr      r3, [r3]
003b3db4 mla      r5, r5, r0, r3
003b3db8 ldr      r1, [r5, #4]
003b3dbc cmp      r1, #0
003b3dc0 bne      #0x3b3e08
003b3dc4 ldrsh    r0, [r4, r6]
003b3dc8 b        #0x3b3d60
003b3dcc mov      r1, #1
003b3dd0 mov      r0, r4
003b3dd4 bl       #0x3bc4d0 ; _ZN9Character7SG_LoadEi
003b3dd8 mov      r0, r4
003b3ddc bl       #0x3bb7fc ; _ZNK9Character17SG_GetPlayerClassEv
003b3de0 uxth     r0, r0
003b3de4 sxth     r1, r0
003b3de8 cmn      r1, #1
003b3dec strh     r0, [r4, r6]
003b3df0 beq      #0x3b3ebc
003b3df4 mov      r0, r4
003b3df8 bl       #0x3bb814 ; _ZN9Character17SG_SetPlayerClassEi
003b3dfc movw     r3, #0x13c8
003b3e00 ldrsh    r0, [r4, r3]
003b3e04 b        #0x3b3d60
003b3e08 ldr      r2, [pc, #0x1d8]
003b3e0c movw     lr, #0xe6ab
003b3e10 movw     r3, #0xdb17
003b3e14 ldr      r2, [r7, r2]
003b3e18 movt     r3, #0x2b52
003b3e1c movw     ip, #0xf26b
003b3e20 ldr      r0, [r2]
003b3e24 movt     ip, #0xda
003b3e28 mul      r0, lr, r0
003b3e2c add      r0, r0, #0x2b000
003b3e30 add      r0, r0, #0x3fc
003b3e34 add      r0, r0, #1
003b3e38 umull    lr, r3, r3, r0
003b3e3c rsb      lr, r3, r0
003b3e40 add      r3, r3, lr, lsr #1
003b3e44 lsr      r3, r3, #0x17
003b3e48 mls      r3, ip, r3, r0
003b3e4c str      r3, [r2]
003b3e50 mov      r0, r3
003b3e54 bl       #0x30eb2c
003b3e58 ldr      r3, [pc, #0x18c]
003b3e5c eor      r6, r1, r1, asr #31
003b3e60 sub      r6, r6, r1, asr #31
003b3e64 ldr      r3, [r7, r3]
003b3e68 ldr      r2, [r3]
003b3e6c add      r2, r2, #1
003b3e70 str      r2, [r3]
003b3e74 ldr      r3, [r5, #4]
003b3e78 cmp      r3, r6
003b3e7c bgt      #0x3b3ea0
003b3e80 ldr      r3, [pc, #0x168]
003b3e84 ldr      r3, [r7, r3]
003b3e88 ldr      r3, [r3]
003b3e8c cmp      r3, #2
003b3e90 streq    sl, [sl]
003b3e94 beq      #0x3b3ea0
003b3e98 cmp      r3, #1
003b3e9c beq      #0x3b3fac
003b3ea0 ldr      r3, [r5, #8]
003b3ea4 add      r6, r3, r6, lsl #3
003b3ea8 ldrh     r0, [r6, #4]
003b3eac movw     r3, #0x13c8
003b3eb0 strh     r0, [r4, r3]
003b3eb4 sxth     r0, r0
003b3eb8 b        #0x3b3d60
003b3ebc ldr      r3, [pc, #0x130]
003b3ec0 ldr      r3, [r7, r3]
003b3ec4 ldr      r6, [r3]
003b3ec8 cmp      r6, #0
003b3ecc beq      #0x3b3fa0
003b3ed0 ldr      r3, [pc, #0x120]
003b3ed4 ldr      r8, [pc, #0x120]
003b3ed8 mov      r5, #0
003b3edc ldr      r3, [r7, r3]
003b3ee0 add      r8, pc, r8
003b3ee4 ldr      r7, [r3]
003b3ee8 b        #0x3b3ef8
003b3eec add      r5, r5, #1
003b3ef0 cmp      r5, r6
003b3ef4 beq      #0x3b3fa0
003b3ef8 ldr      r1, [r7, r5, lsl #2]
003b3efc mov      r0, r8
003b3f00 bl       #0x30e31c
003b3f04 cmp      r0, #0
003b3f08 bne      #0x3b3eec
003b3f0c uxth     r5, r5
003b3f10 sxth     r1, r5
003b3f14 movw     r3, #0x13c8
003b3f18 strh     r5, [r4, r3]
003b3f1c b        #0x3b3df4
003b3f20 movw     r3, #0x13c4
003b3f24 ldr      r8, [r4, r3]
003b3f28 mov      r3, #0x13c0
003b3f2c ldr      r3, [r4, r3]
003b3f30 cmp      r3, r8
003b3f34 beq      #0x3b3dc4
003b3f38 ldr      r3, [pc, #0xb4]
003b3f3c ldr      r3, [r7, r3]
003b3f40 ldr      r6, [r3]
003b3f44 cmp      r6, #0
003b3f48 beq      #0x3b3f94
003b3f4c ldr      r3, [pc, #0xa4]
003b3f50 mov      r5, sl
003b3f54 ldr      r3, [r7, r3]
003b3f58 ldr      r7, [r3]
003b3f5c b        #0x3b3f6c
003b3f60 add      r5, r5, #1
003b3f64 cmp      r5, r6
003b3f68 beq      #0x3b3f94
003b3f6c ldr      r1, [r7, r5, lsl #2]
003b3f70 mov      r0, r8
003b3f74 bl       #0x30e31c
003b3f78 cmp      r0, #0
003b3f7c bne      #0x3b3f60
003b3f80 uxth     r5, r5
003b3f84 sxth     r0, r5
003b3f88 movw     r3, #0x13c8
003b3f8c strh     r5, [r4, r3]
003b3f90 b        #0x3b3d60
003b3f94 mvn      r0, #0
003b3f98 movw     r5, #0xffff
003b3f9c b        #0x3b3f88
003b3fa0 mvn      r1, #0
003b3fa4 movw     r5, #0xffff
003b3fa8 b        #0x3b3f14
003b3fac ldr      r0, [pc, #0x4c]
003b3fb0 ldr      r1, [pc, #0x4c]
003b3fb4 ldr      r2, [pc, #0x4c]
003b3fb8 ldr      r0, [r7, r0]
003b3fbc ldr      r3, [pc, #0x48]
003b3fc0 mov      ip, #0x2f4
003b3fc4 add      r1, pc, r1
003b3fc8 add      r2, pc, r2
003b3fcc add      r3, pc, r3
003b3fd0 add      r0, r0, #0xa8
003b3fd4 str      ip, [sp]
003b3fd8 bl       #0x30e004
003b3fdc b        #0x3b3ea0
003b3fe0 subseq   r0, lr, r0, asr #26
003b3fe4 strheq   r4, [r0], -r8
003b3fe8 muleq    r0, r4, ip
003b3fec andeq    r1, r0, r8, lsl #1
003b3ff0 andeq    r3, r0, r0, asr #19
003b3ff4 andeq    r4, r0, r4, lsl #4
003b3ff8 andeq    r3, r0, r8, lsl #24
003b3ffc subseq   pc, r0, r0, lsl #29
003b4000 andeq    r1, r0, r0, asr #19
003b4004 subseq   sl, r0, r4, lsl r4
003b4008 ldrheq   pc, [r0], #-0xd0
003b400c subseq   pc, r0, r4, ror #27
_ZN14CharProperties18LoadBasePropertiesEi
003df2a4 mov      r2, r1
003df2a8 add      r1, r0, #8
003df2ac b        #0x3df250
