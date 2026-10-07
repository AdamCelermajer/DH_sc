_ZN6glitch7collada14createMaterialERKNS0_16CColladaDatabaseEPNS_5video12IVideoDriverERKN5boost13intrusive_ptrINS4_17CMaterialRendererEEERNS0_9SMaterialEPNS0_14CRootSceneNodeE 0x631ce8
00631ce8 push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
00631cec mov r2, #0
00631cf0 sub sp, sp, #0xa4
00631cf4 str r0, [sp, #0x1c]
00631cf8 str r3, [sp, #0x18]
00631cfc str r2, [r0]
00631d00 ldr r0, [sp, #0x18]
00631d04 ldr r1, [pc, #0x69c]
00631d08 ldr r8, [sp, #0xc8]
00631d0c ldr r3, [r0]
00631d10 add r1, pc, r1
00631d14 str r1, [sp, #0x28]
00631d18 cmp r3, r2
00631d1c beq #0x631ea8
00631d20 add r4, sp, #0x9c
00631d24 mov r3, r2
00631d28 ldr r1, [sp, #0x18]
00631d2c ldr r2, [r8]
00631d30 mov r0, r4
00631d34 bl #0x5cc0a0
00631d38 ldr r3, [sp, #0x9c]
00631d3c add r0, sp, #0xa0
00631d40 str r3, [sp, #0x98]
00631d44 cmp r3, #0
00631d48 ldrne r2, [r3]
00631d4c addne r2, r2, #1
00631d50 strne r2, [r3]
00631d54 ldr sb, [sp, #0x1c]
00631d58 ldrne r3, [sp, #0x98]
00631d5c ldr r2, [sb]
00631d60 str r3, [sb]
00631d64 str r2, [r0, #-8]!
00631d68 bl #0x310be8
00631d6c mov r0, r4
00631d70 bl #0x310be8
00631d74 ldr sl, [r8, #0x10]
00631d78 cmp sl, #0
00631d7c str sl, [sp, #0x20]
00631d80 ble #0x631ea8
00631d84 ldr r3, [pc, #0x620]
00631d88 ldr r2, [pc, #0x620]
00631d8c mov r4, #0
00631d90 add r3, pc, r3
00631d94 add r3, r3, #0x4c
00631d98 str r3, [sp, #0x3c]
00631d9c ldr r3, [pc, #0x610]
00631da0 add r2, pc, r2
00631da4 str r2, [sp, #0x2c]
00631da8 add r3, pc, r3
00631dac str r3, [sp, #0x34]
00631db0 ldr r3, [pc, #0x600]
00631db4 mov r6, r4
00631db8 add r3, pc, r3
00631dbc str r3, [sp, #0x38]
00631dc0 b #0x631e38
00631dc4 ldr r1, [sp, #0x1c]
00631dc8 ldr r0, [r1]
00631dcc ldr r1, [r7, #0x10]
00631dd0 ldr r3, [r0, #4]
00631dd4 ldrh r2, [r3, #0xe]
00631dd8 cmp r5, r2
00631ddc ldrlo sb, [r3, #0x20]
00631de0 movhs sb, #0
00631de4 addlo sb, sb, r5, lsl #4
00631de8 ldr sl, [sb, #8]
00631dec str sl, [sp, #0xc]
00631df0 ldr r1, [r1]
00631df4 cmp sl, r1
00631df8 bls #0x631eb4
00631dfc ldr r2, [r0, #0x1c]
00631e00 ldr r3, [sb]
00631e04 ldr r1, [pc, #0x5b0]
00631e08 cmp r2, #0
00631e0c addne r2, r2, #4
00631e10 cmp r3, #0
00631e14 addne r3, r3, #4
00631e18 add r1, pc, r1
00631e1c mov r0, #3
00631e20 bl #0x60b034
00631e24 ldr r3, [sp, #0x20]
00631e28 add r6, r6, #1
00631e2c add r4, r4, #0x18
00631e30 cmp r6, r3
00631e34 beq #0x631ea8
00631e38 ldr r7, [r8, #0x14]
00631e3c ldr ip, [sp, #0x18]
00631e40 mov r2, #0
00631e44 ldr r1, [r7, r4]
00631e48 ldr r0, [ip]
00631e4c bl #0x5d308c
00631e50 movw r3, #0xffff
00631e54 cmp r0, r3
00631e58 add r7, r7, r4
00631e5c mov r5, r0
00631e60 bne #0x631dc4
00631e64 ldr r3, [r7, #8]
00631e68 cmp r3, #0x14
00631e6c bne #0x631e24
00631e70 ldr r3, [r7, #0x14]
00631e74 ldr r1, [sp, #0x18]
00631e78 add r6, r6, #1
00631e7c add r4, r4, #0x18
00631e80 ldr r0, [r1]
00631e84 ldr r1, [r3, #4]
00631e88 bl #0x5d4714
00631e8c cmp r0, #0xff
00631e90 ldrne r2, [sp, #0x1c]
00631e94 ldrne r3, [r2]
00631e98 strbne r0, [r3, #8]
00631e9c ldr r3, [sp, #0x20]
00631ea0 cmp r6, r3
00631ea4 bne #0x631e38
00631ea8 ldr r0, [sp, #0x1c]
00631eac add sp, sp, #0xa4
00631eb0 pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
00631eb4 ldrb fp, [sb, #6]
00631eb8 ldr r1, [sp, #0x2c]
00631ebc ldr ip, [r7, #8]
00631ec0 mov sl, #1
00631ec4 ldr r1, [r1, fp, lsl #2]
00631ec8 str ip, [sp, #0x30]
00631ecc str ip, [sp, #0x24]
00631ed0 ands r1, r1, sl, lsl ip
00631ed4 bne #0x631f4c
00631ed8 ldr r3, [r0, #0x1c]
00631edc cmp r3, #0
00631ee0 addne r3, r3, #4
00631ee4 str r3, [sp, #0x30]
00631ee8 ldr r5, [sb]
00631eec cmp r5, #0
00631ef0 addne r5, r5, #4
00631ef4 cmp fp, #0xff
00631ef8 beq #0x631f84
00631efc mov r0, #0
00631f00 bl #0x5e80b4
00631f04 ldr r7, [r7, #8]
00631f08 ldr sl, [r0, fp, lsl #2]
00631f0c str r7, [sp, #0x24]
00631f10 ldr r1, [sp, #0x34]
00631f14 add r0, sp, #0x40
00631f18 mov r2, #0x58
00631f1c bl #0x30e868
00631f20 ldr r0, [sp, #0x24]
00631f24 add ip, sp, #0xa0
00631f28 ldr r2, [sp, #0x30]
00631f2c add r3, ip, r0, lsl #2
00631f30 ldr ip, [r3, #-0x60]
00631f34 mov r0, #3
00631f38 mov r3, r5
00631f3c ldr r1, [sp, #0x38]
00631f40 stm sp, {sl, ip}
00631f44 bl #0x60b034
00631f48 b #0x631e24
00631f4c sub fp, fp, #9
00631f50 cmp fp, #9
00631f54 addls pc, pc, fp, lsl #2
00631f58 b #0x632024
00631f5c b #0x631e24
00631f60 b #0x631e24
00631f64 b #0x632080
00631f68 b #0x63215c
00631f6c b #0x6321ec
00631f70 b #0x63227c
00631f74 b #0x63230c
00631f78 b #0x632024
00631f7c b #0x632024
00631f80 b #0x631f90
00631f84 ldr sl, [pc, #0x434]
00631f88 add sl, pc, sl
00631f8c b #0x631f10
00631f90 ldr r1, [sp, #0xc]
00631f94 cmp r1, #0
00631f98 beq #0x631e24
00631f9c add sl, sp, #0x40
00631fa0 mov fp, #0
00631fa4 str sl, [sp, #0x24]
00631fa8 mov sb, fp
00631fac ldr sl, [sp, #0xc]
00631fb0 b #0x631fec
00631fb4 ldr ip, [sp, #0xcc]
00631fb8 cmp ip, #0
00631fbc beq #0x631fdc
00631fc0 ldr ip, [sp, #0x24]
00631fc4 ldr r0, [sp, #0xcc]
00631fc8 ldr r1, [sp, #0x1c]
00631fcc mov r2, r5
00631fd0 mov r3, sb
00631fd4 str ip, [sp]
00631fd8 bl #0x65b0fc
00631fdc add sb, sb, #1
00631fe0 cmp sb, sl
00631fe4 add fp, fp, #4
00631fe8 beq #0x631e24
00631fec ldr r3, [r7, #0x14]
00631ff0 add r3, r3, fp
00631ff4 ldr r3, [r3]
00631ff8 str r3, [sp, #0x40]
00631ffc ldr r2, [r3, #-4]
00632000 cmp r2, #0
00632004 beq #0x631e24
00632008 ldrsb r2, [r3]
0063200c cmp r2, #0x23
00632010 bne #0x631fb4
00632014 ldrsb r3, [r3, #1]
00632018 cmp r3, #0
0063201c beq #0x631e24
00632020 b #0x631fb4
00632024 ldr r1, [sp, #0x28]
00632028 ldr r3, [pc, #0x394]
0063202c ldr sb, [sp, #0x30]
00632030 ldr sl, [sp, #0x28]
00632034 ldr r2, [r1, r3]
00632038 ldr r1, [pc, #0x388]
0063203c add r3, sb, #1
00632040 ldr ip, [sp, #0x28]
00632044 ldr r1, [sl, r1]
00632048 ldr sl, [r2, r3, lsl #2]
0063204c ldr r2, [pc, #0x378]
00632050 ldrb r1, [r1, sl]
00632054 ldr r2, [ip, r2]
00632058 ldr ip, [sp, #0x3c]
0063205c ldr lr, [ip, sb, lsl #2]
00632060 ldrb ip, [r2, r3]
00632064 ldr r3, [r7, #0x14]
00632068 mov r2, lr
0063206c mul ip, ip, r1
00632070 mov r1, r5
00632074 str ip, [sp]
00632078 bl #0x5ccf40
0063207c b #0x631e24
00632080 add fp, sp, #0x40
00632084 mov r0, fp
00632088 bl #0x631c14
0063208c ldr sl, [sp, #0x28]
00632090 ldr r3, [pc, #0x32c]
00632094 ldr r2, [r7, #8]
00632098 ldr ip, [sb, #8]
0063209c ldr r1, [sl, r3]
006320a0 ldr r3, [pc, #0x320]
006320a4 add r2, r2, #1
006320a8 ldr r1, [r1, r2, lsl #2]
006320ac ldr r0, [sl, r3]
006320b0 ldr r3, [pc, #0x314]
006320b4 cmp ip, #0
006320b8 ldrb r1, [r0, r1]
006320bc ldr r3, [sl, r3]
006320c0 ldrb r3, [r3, r2]
006320c4 mul r3, r3, r1
006320c8 beq #0x631e24
006320cc mov sb, #0
006320d0 str r4, [sp, #0x24]
006320d4 str r6, [sp, #0x30]
006320d8 mov sl, sb
006320dc mov r6, r5
006320e0 mov r4, r3
006320e4 mov r5, ip
006320e8 b #0x6320fc
006320ec add sl, sl, #1
006320f0 cmp sl, r5
006320f4 add sb, sb, r4
006320f8 beq #0x63239c
006320fc ldr lr, [r7, #0x14]
00632100 mov ip, #0
00632104 strb ip, [sp, #0x80]
00632108 add lr, lr, sb
0063210c mov ip, fp
00632110 ldm lr!, {r0, r1, r2, r3}
00632114 stm ip!, {r0, r1, r2, r3}
00632118 ldm lr!, {r0, r1, r2, r3}
0063211c stm ip!, {r0, r1, r2, r3}
00632120 ldm lr!, {r0, r1, r2, r3}
00632124 stm ip!, {r0, r1, r2, r3}
00632128 ldm lr, {r0, r1, r2, r3}
0063212c stm ip, {r0, r1, r2, r3}
00632130 mov r0, fp
00632134 bl #0x5ba19c
00632138 cmp r0, #0
0063213c bne #0x6320ec
00632140 ldr r3, [sp, #0x1c]
00632144 mov r2, sl
00632148 mov r1, r6
0063214c ldr r0, [r3]
00632150 mov r3, fp
00632154 bl #0x5cb4dc
00632158 b #0x6320ec
0063215c cmp r5, r2
00632160 ldrlo r3, [r3, #0x20]
00632164 movhs r3, #0
00632168 ldr sl, [r7, #0x14]
0063216c addlo r3, r3, r5, lsl #4
00632170 ldr sb, [r3, #8]
00632174 cmp sb, #0
00632178 beq #0x631e24
0063217c str r4, [sp, #0x24]
00632180 ldr r4, [sp, #0x1c]
00632184 mov r7, #0
00632188 add fp, sp, #0x40
0063218c ldr r0, [sl, r7, lsl #2]
00632190 mov r2, r7
00632194 mov r1, r5
00632198 ldr r0, [r0]
0063219c mov r3, fp
006321a0 add r7, r7, #1
006321a4 cmp r0, #0
006321a8 beq #0x6321dc
006321ac ldr r0, [r0, #0x10]
006321b0 cmp r0, #0
006321b4 str r0, [sp, #0x40]
006321b8 ldrne ip, [r0, #4]
006321bc addne ip, ip, #1
006321c0 strne ip, [r0, #4]
006321c4 ldr r0, [r4]
006321c8 bl #0x5cd324
006321cc ldr r0, [sp, #0x40]
006321d0 cmp r0, #0
006321d4 beq #0x6321dc
006321d8 bl #0x31d584
006321dc cmp r7, sb
006321e0 bne #0x63218c
006321e4 ldr r4, [sp, #0x24]
006321e8 b #0x631e24
006321ec cmp r5, r2
006321f0 ldrlo r3, [r3, #0x20]
006321f4 movhs r3, #0
006321f8 ldr sl, [r7, #0x14]
006321fc addlo r3, r3, r5, lsl #4
00632200 ldr sb, [r3, #8]
00632204 cmp sb, #0
00632208 beq #0x631e24
0063220c str r4, [sp, #0x24]
00632210 ldr r4, [sp, #0x1c]
00632214 mov r7, #0
00632218 add fp, sp, #0x40
0063221c ldr r0, [sl, r7, lsl #2]
00632220 mov r2, r7
00632224 mov r1, r5
00632228 ldr r0, [r0]
0063222c mov r3, fp
00632230 add r7, r7, #1
00632234 cmp r0, #0
00632238 beq #0x63226c
0063223c ldr r0, [r0, #0x10]
00632240 cmp r0, #0
00632244 str r0, [sp, #0x40]
00632248 ldrne ip, [r0, #4]
0063224c addne ip, ip, #1
00632250 strne ip, [r0, #4]
00632254 ldr r0, [r4]
00632258 bl #0x5cd324
0063225c ldr r0, [sp, #0x40]
00632260 cmp r0, #0
00632264 beq #0x63226c
00632268 bl #0x31d584
0063226c cmp r7, sb
00632270 bne #0x63221c
00632274 ldr r4, [sp, #0x24]
00632278 b #0x631e24
0063227c cmp r5, r2
00632280 ldrlo r3, [r3, #0x20]
00632284 movhs r3, #0
00632288 ldr sl, [r7, #0x14]
0063228c addlo r3, r3, r5, lsl #4
00632290 ldr sb, [r3, #8]
00632294 cmp sb, #0
00632298 beq #0x631e24
0063229c str r4, [sp, #0x24]
006322a0 ldr r4, [sp, #0x1c]
006322a4 mov r7, #0
006322a8 add fp, sp, #0x40
006322ac ldr r0, [sl, r7, lsl #2]
006322b0 mov r2, r7
006322b4 mov r1, r5
006322b8 ldr r0, [r0]
006322bc mov r3, fp
006322c0 add r7, r7, #1
006322c4 cmp r0, #0
006322c8 beq #0x6322fc
006322cc ldr r0, [r0, #0x10]
006322d0 cmp r0, #0
006322d4 str r0, [sp, #0x40]
006322d8 ldrne ip, [r0, #4]
006322dc addne ip, ip, #1
006322e0 strne ip, [r0, #4]
006322e4 ldr r0, [r4]
006322e8 bl #0x5cd324
006322ec ldr r0, [sp, #0x40]
006322f0 cmp r0, #0
006322f4 beq #0x6322fc
006322f8 bl #0x31d584
006322fc cmp r7, sb
00632300 bne #0x6322ac
00632304 ldr r4, [sp, #0x24]
00632308 b #0x631e24
0063230c cmp r5, r2
00632310 ldrlo r3, [r3, #0x20]
00632314 movhs r3, #0
00632318 ldr sl, [r7, #0x14]
0063231c addlo r3, r3, r5, lsl #4
00632320 ldr sb, [r3, #8]
00632324 cmp sb, #0
00632328 beq #0x631e24
0063232c str r4, [sp, #0x24]
00632330 ldr r4, [sp, #0x1c]
00632334 mov r7, #0
00632338 add fp, sp, #0x40
0063233c ldr r0, [sl, r7, lsl #2]
00632340 mov r2, r7
00632344 mov r1, r5
00632348 ldr r0, [r0]
0063234c mov r3, fp
00632350 add r7, r7, #1
00632354 cmp r0, #0
00632358 beq #0x63238c
0063235c ldr r0, [r0, #0x10]
00632360 cmp r0, #0
00632364 str r0, [sp, #0x40]
00632368 ldrne ip, [r0, #4]
0063236c addne ip, ip, #1
00632370 strne ip, [r0, #4]
00632374 ldr r0, [r4]
00632378 bl #0x5cd324
0063237c ldr r0, [sp, #0x40]
00632380 cmp r0, #0
00632384 beq #0x63238c
00632388 bl #0x31d584
0063238c cmp r7, sb
00632390 bne #0x63233c
00632394 ldr r4, [sp, #0x24]
00632398 b #0x631e24
0063239c ldr r4, [sp, #0x24]
006323a0 ldr r6, [sp, #0x30]
006323a4 b #0x631e24
006323a8 eorseq r2, r6, r0, lsl #27
006323ac strhteq r3, [fp], -ip
006323b0 eoreq r3, fp, ip, lsr #1
006323b4 eorseq r5, r2, r0, asr #15
006323b8 eoreq r3, fp, r0, ror #3
006323bc eoreq r3, fp, r0, asr r1
006323c0 .byte 0xd8, 0x44, 0x29, 0x00
006323c4 andeq r3, r0, ip, lsr #23
006323c8 andeq r1, r0, r0, asr #11
006323cc andeq r2, r0, ip, lsr #23
_ZN6glitch5video6detail18setMatrixParameterEPPNS_4core8CMatrix4IfEERKS4_NS2_10SAllocatorIS4_LNS_6memory13E_MEMORY_HINTE0EEE 0x5badb8
005badb8 push {r4, r5, r6, lr}
005badbc mov r4, r0
005badc0 ldr r0, [r0]
005badc4 ldr r3, [pc, #0x90]
005badc8 mov r5, r1
005badcc cmp r0, #0
005badd0 add r3, pc, r3
005badd4 beq #0x5bae10
005badd8 ldrb r2, [r1, #0x40]
005baddc cmp r2, #0
005bade0 beq #0x5bae04
005bade4 ldr r2, [pc, #0x74]
005bade8 ldr r3, [r3, r2]
005badec ldr r2, [r3]
005badf0 str r2, [r0]
005badf4 str r0, [r3]
005badf8 mov r3, #0
005badfc str r3, [r4]
005bae00 pop {r4, r5, r6, pc}
005bae04 mov r2, #0x41
005bae08 pop {r4, r5, r6, lr}
005bae0c b #0x30e868
005bae10 ldrb r2, [r1, #0x40]
005bae14 cmp r2, #0
005bae18 bne #0x5bae4c
005bae1c ldr r2, [pc, #0x3c]
005bae20 ldr r3, [r3, r2]
005bae24 ldr r6, [r3]
005bae28 cmp r6, #0
005bae2c beq #0x5bae50
005bae30 ldr r2, [r6]
005bae34 str r2, [r3]
005bae38 mov r1, r5
005bae3c mov r0, r6
005bae40 bl #0x5baa94
005bae44 str r6, [r4]
005bae48 pop {r4, r5, r6, pc}
005bae4c pop {r4, r5, r6, pc}
005bae50 bl #0x5bac80
005bae54 mov r6, r0
005bae58 b #0x5bae38
005bae5c eorseq sb, sp, r0, asr #25
005bae60 andeq r3, r0, r0, asr #25
