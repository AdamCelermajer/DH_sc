
# _ZNK9Character13CTRLIsAllowedEv 0x3ad430
003ad430 push {r4, lr}
003ad434 add r4, r0, #0x4f0
003ad438 add r4, r4, #0xc
003ad43c mov r0, r4
003ad440 mov r1, #1
003ad444 bl #0x3c034c
003ad448 cmp r0, #0
003ad44c beq #0x3ad458
003ad450 mov r0, #0
003ad454 pop {r4, pc}
003ad458 mov r0, r4
003ad45c mov r1, #1
003ad460 bl #0x3c0378
003ad464 eor r0, r0, #1
003ad468 uxtb r0, r0
003ad46c pop {r4, pc}

# _ZN9Character17GetFaeryCharacterEv 0x3ae60c
003ae60c ldr r0, [r0, #0x420]
003ae610 bx lr

# _ZNK9Character17GetFaeryCharacterEv 0x3ae614
003ae614 ldr r0, [r0, #0x420]
003ae618 bx lr

# _ZN9Character11ChangeFaeryEj 0x3ae99c
003ae99c push {r4, r5, r6, lr}
003ae9a0 sub sp, sp, #8
003ae9a4 mov r6, r1
003ae9a8 mov r4, r0
003ae9ac bl #0x3bb8e4
003ae9b0 mov r5, r0
003ae9b4 mov r1, r5
003ae9b8 mov r0, r4
003ae9bc bl #0x3bba20
003ae9c0 ldr r3, [pc, #0xbc]
003ae9c4 cmp r0, r6
003ae9c8 add r3, pc, r3
003ae9cc bhi #0x3ae9f4
003ae9d0 ldr r2, [pc, #0xb0]
003ae9d4 ldr r2, [r3, r2]
003ae9d8 ldr r2, [r2]
003ae9dc cmp r2, #2
003ae9e0 moveq r3, #0
003ae9e4 streq r3, [r3]
003ae9e8 beq #0x3ae9f4
003ae9ec cmp r2, #1
003ae9f0 beq #0x3aea50
003ae9f4 mov r0, r4
003ae9f8 mov r1, r6
003ae9fc mov r2, r5
003aea00 bl #0x3bb9d8
003aea04 add r0, r4, #0x3c8
003aea08 bl #0x3d8894
003aea0c ldr r4, [r4, #0x420]
003aea10 cmp r4, #0
003aea14 beq #0x3aea48
003aea18 mov r0, r4
003aea1c bl #0x3a54d4
003aea20 mov r2, #0
003aea24 mov r1, r0
003aea28 mov r3, #1
003aea2c mov r0, r4
003aea30 bl #0x394d34
003aea34 add r0, r4, #0x490
003aea38 add r0, r0, #0xc
003aea3c add sp, sp, #8
003aea40 pop {r4, r5, r6, lr}
003aea44 b #0x3c99a0
003aea48 add sp, sp, #8
003aea4c pop {r4, r5, r6, pc}
003aea50 ldr r0, [pc, #0x34]
003aea54 ldr r1, [pc, #0x34]
003aea58 ldr r2, [pc, #0x34]
003aea5c ldr r0, [r3, r0]
003aea60 ldr r3, [pc, #0x30]
003aea64 mov ip, #0x6b
003aea68 add r1, pc, r1
003aea6c add r2, pc, r2
003aea70 add r3, pc, r3
003aea74 add r0, r0, #0xa8
003aea78 str ip, [sp]
003aea7c bl #0x30e004
003aea80 b #0x3ae9f4
003aea84 subseq r6, lr, r8, asr #1
003aea88 andeq r3, r0, r0, asr #19
003aea8c andeq r1, r0, r0, asr #19
003aea90 subseq pc, r0, r0, ror sb
003aea94 subseq r4, r1, r4, ror #26
003aea98 subseq r4, r1, r8, lsl #27

# _ZNK9Character21SG_GetCurrentFaerieIdEi 0x3bb98c
003bb98c movw r3, #0x14e8
003bb990 ldr r0, [r0, r3]
003bb994 ldr r3, [pc, #0x34]
003bb998 cmp r0, #0
003bb99c add r3, pc, r3
003bb9a0 bxeq lr
003bb9a4 cmn r1, #1
003bb9a8 beq #0x3bb9b8
003bb9ac add r0, r0, r1, lsl #2
003bb9b0 ldr r0, [r0, #0xac]
003bb9b4 bx lr
003bb9b8 ldr r2, [pc, #0x14]
003bb9bc ldr r3, [r3, r2]
003bb9c0 ldr r3, [r3]
003bb9c4 add r0, r0, r3, lsl #2
003bb9c8 ldr r0, [r0, #0xac]
003bb9cc bx lr
003bb9d0 ldrsheq sb, [sp], #-4
003bb9d4 muleq r0, ip, sl

# _ZN9Character19SG_SetCurrentFaerieEji 0x3bb9d8
003bb9d8 movw r3, #0x14e8
003bb9dc ldr r0, [r0, r3]
003bb9e0 ldr r3, [pc, #0x30]
003bb9e4 cmp r0, #0
003bb9e8 add r3, pc, r3
003bb9ec bxeq lr
003bb9f0 cmn r2, #1
003bb9f4 addne r0, r0, r2, lsl #2
003bb9f8 strne r1, [r0, #0xac]
003bb9fc bxne lr
003bba00 ldr r2, [pc, #0x14]
003bba04 ldr r3, [r3, r2]
003bba08 ldr r3, [r3]
003bba0c add r0, r0, r3, lsl #2
003bba10 str r1, [r0, #0xac]
003bba14 bx lr
003bba18 subseq sb, sp, r8, lsr #1
003bba1c muleq r0, ip, sl

# _ZNK16CharStateMachine15SM_IsUsingSkillEv 0x3c02e8
003c02e8 push {r4, lr}
003c02ec bl #0x3c01ac
003c02f0 cmp r0, #6
003c02f4 movne r0, #0
003c02f8 moveq r0, #1
003c02fc pop {r4, pc}

# _ZNK16CharStateMachine12SM_IsCastingEv 0x3c0334
003c0334 push {r4, lr}
003c0338 bl #0x3c01ac
003c033c cmp r0, #7
003c0340 movne r0, #0
003c0344 moveq r0, #1
003c0348 pop {r4, pc}

# _ZN16CharStateMachine15SM_SetCastStateEjPvb 0x3c6394
003c6394 push {r4, r5, r6, r7, r8, lr}
003c6398 mov r4, r0
003c639c ldr r0, [r0, #4]
003c63a0 mov r7, r3
003c63a4 mov r5, r1
003c63a8 mov r6, r2
003c63ac bl #0x3a3228
003c63b0 ldr r3, [pc, #0xb8]
003c63b4 cmp r0, #0
003c63b8 add r3, pc, r3
003c63bc blt #0x3c63f4
003c63c0 ldr r2, [pc, #0xac]
003c63c4 ldr r2, [r3, r2]
003c63c8 ldr r2, [r2]
003c63cc cmp r0, r2
003c63d0 bge #0x3c63f4
003c63d4 ldr r2, [pc, #0x9c]
003c63d8 mov r1, #0xa0
003c63dc ldr r2, [r3, r2]
003c63e0 ldr r2, [r2]
003c63e4 mla r0, r1, r0, r2
003c63e8 ldr r2, [r0, #0x84]
003c63ec cmp r2, r5
003c63f0 bhi #0x3c63f8
003c63f4 pop {r4, r5, r6, r7, r8, pc}
003c63f8 ldr r2, [pc, #0x7c]
003c63fc ldr r1, [pc, #0x7c]
003c6400 ldr r2, [r3, r2]
003c6404 ldr r3, [r0, #0x88]
003c6408 add r1, pc, r1
003c640c ldr r0, [r2, #0x2c]
003c6410 ldr r2, [pc, #0x6c]
003c6414 ldr r5, [r3, r5, lsl #2]
003c6418 add r2, pc, r2
003c641c bl #0x4c4bdc
003c6420 ands r0, r0, #0x400000
003c6424 bne #0x3c6464
003c6428 add r5, r0, r5
003c642c cmp r7, #0
003c6430 str r5, [r4, #0x28]
003c6434 bne #0x3c644c
003c6438 mov r0, r4
003c643c mov r2, r6
003c6440 movw r1, #0xc356
003c6444 pop {r4, r5, r6, r7, r8, lr}
003c6448 b #0x3c5684
003c644c mov r0, r4
003c6450 mov r3, r6
003c6454 mov r1, #7
003c6458 movw r2, #0xc356
003c645c pop {r4, r5, r6, r7, r8, lr}
003c6460 b #0x3c1938
003c6464 ldr r0, [r4, #4]
003c6468 bl #0x3a53e0
003c646c b #0x3c6428
003c6470 ldrsbeq lr, [ip], #-0x68
003c6474 andeq r2, r0, r0, asr #17
003c6478 andeq r4, r0, r4, asr #16
003c647c strdeq r3, r4, [r0], -r4
003c6480 strheq lr, [pc], #-0x70
003c6484 strheq lr, [pc], #-0x70

# _ZNK6CharAI21IsScriptProcessLoadedEv 0x3cb458
003cb458 ldr r0, [r0, #0x28]
003cb45c cmp r0, #6
003cb460 movle r0, #0
003cb464 movgt r0, #1
003cb468 bx lr

# _ZNK6CharAI16AI_IsSpellActiveEv 0x3d7d9c
003d7d9c mov r0, #0
003d7da0 bx lr

# _ZNK6CharAI12AI_SpellInfoERf 0x3d7da8
003d7da8 push {r4, r5, r6, lr}
003d7dac mov r4, r0
003d7db0 sub sp, sp, #8
003d7db4 mov r6, r1
003d7db8 ldr r0, [r0, #4]
003d7dbc mvn r1, #0
003d7dc0 bl #0x3bb98c
003d7dc4 ldr r2, [r4, #0xc0]
003d7dc8 ldr r1, [r4, #0xc4]
003d7dcc ldr r3, [pc, #0x9c]
003d7dd0 mov r5, r0
003d7dd4 rsb r1, r2, r1
003d7dd8 cmp r0, r1, asr #2
003d7ddc add r3, pc, r3
003d7de0 blo #0x3d7e08
003d7de4 ldr r1, [pc, #0x88]
003d7de8 ldr r1, [r3, r1]
003d7dec ldr r1, [r1]
003d7df0 cmp r1, #2
003d7df4 moveq r3, #0
003d7df8 streq r3, [r3]
003d7dfc beq #0x3d7e08
003d7e00 cmp r1, #1
003d7e04 beq #0x3d7e38
003d7e08 ldr r0, [r2, r5, lsl #2]
003d7e0c cmp r0, #0
003d7e10 beq #0x3d7e28
003d7e14 mov r2, r6
003d7e18 mov r1, #0
003d7e1c add sp, sp, #8
003d7e20 pop {r4, r5, r6, lr}
003d7e24 b #0x3daca8
003d7e28 mov r3, #0
003d7e2c str r3, [r6]
003d7e30 add sp, sp, #8
003d7e34 pop {r4, r5, r6, pc}
003d7e38 ldr r0, [pc, #0x38]
003d7e3c ldr r1, [pc, #0x38]
003d7e40 ldr r2, [pc, #0x38]
003d7e44 ldr r0, [r3, r0]
003d7e48 ldr r3, [pc, #0x34]
003d7e4c add r2, pc, r2
003d7e50 movw ip, #0x1c9
003d7e54 add r1, pc, r1
003d7e58 add r0, r0, #0xa8
003d7e5c add r3, pc, r3
003d7e60 str ip, [sp]
003d7e64 bl #0x30e004
003d7e68 ldr r2, [r4, #0xc0]
003d7e6c b #0x3d7e08
003d7e70 ldrheq ip, [fp], #-0xc4
003d7e74 andeq r3, r0, r0, asr #19
003d7e78 andeq r1, r0, r0, asr #19
003d7e7c subeq r6, lr, r4, lsl #11
003d7e80 strheq sp, [lr], #-0x8c
003d7e84 subeq sp, lr, ip, asr #17

# _ZN6CharAI11AI_EndSpellEb 0x3d7f60
003d7f60 push {r4, r5, r6, lr}
003d7f64 mov r4, r0
003d7f68 ldr r0, [r0, #4]
003d7f6c mov r6, r1
003d7f70 add r0, r0, #0x4f0
003d7f74 add r0, r0, #0xc
003d7f78 bl #0x3c0334
003d7f7c cmp r0, #0
003d7f80 bne #0x3d7f88
003d7f84 pop {r4, r5, r6, pc}
003d7f88 ldr r5, [r4, #4]
003d7f8c mvn r1, #0
003d7f90 mov r0, r5
003d7f94 bl #0x3bb98c
003d7f98 mov r1, r0
003d7f9c mov r0, r5
003d7fa0 bl #0x3aeac0
003d7fa4 ldr r3, [r0, #0x1c]
003d7fa8 cmp r3, #2
003d7fac bne #0x3d7f84
003d7fb0 ldrb r3, [r4, #0xd0]
003d7fb4 cmp r3, #0
003d7fb8 moveq r3, #1
003d7fbc strbeq r3, [r4, #0xd1]
003d7fc0 bne #0x3d801c
003d7fc4 bl #0x7fd794
003d7fc8 ldrb r3, [r0, #5]
003d7fcc cmp r3, #0
003d7fd0 beq #0x3d7f84
003d7fd4 cmp r6, #0
003d7fd8 bne #0x3d7f84
003d7fdc bl #0x80b1bc
003d7fe0 mov r5, r0
003d7fe4 ldr r0, [pc, #0x48]
003d7fe8 ldr r3, [r4, #4]
003d7fec mov r1, #1
003d7ff0 add r0, pc, r0
003d7ff4 ldrb r4, [r3, #0x108]
003d7ff8 bl #0x80a244
003d7ffc mov r3, #4
003d8000 mov r1, r0
003d8004 strb r4, [r0, #0x54]
003d8008 strb r3, [r0, #0x50]
003d800c strh r6, [r0, #0x52]
003d8010 mov r0, r5
003d8014 pop {r4, r5, r6, lr}
003d8018 b #0x80e2a4
003d801c ldr r0, [r4, #4]
003d8020 mov r1, #1
003d8024 add r0, r0, #0x490
003d8028 add r0, r0, #0xc
003d802c bl #0x3c948c
003d8030 b #0x3d7fc4
003d8034 subeq r6, lr, r8, lsl pc

# _ZNK6CharAI16AI_IsSpellUsableEv 0x3d80b4
003d80b4 push {r4, r5, r6, r7, lr}
003d80b8 mov r4, r0
003d80bc ldr r0, [r0, #4]
003d80c0 sub sp, sp, #0xc
003d80c4 ldr r6, [pc, #0xdc]
003d80c8 add r0, r0, #0x4f0
003d80cc add r0, r0, #0xc
003d80d0 bl #0x3c02e8
003d80d4 cmp r0, #0
003d80d8 add r6, pc, r6
003d80dc beq #0x3d80ec
003d80e0 mov r0, #0
003d80e4 add sp, sp, #0xc
003d80e8 pop {r4, r5, r6, r7, pc}
003d80ec ldr r0, [r4, #4]
003d80f0 add r0, r0, #0x4f0
003d80f4 add r0, r0, #0xc
003d80f8 bl #0x3c0334
003d80fc subs r7, r0, #0
003d8100 bne #0x3d80e0
003d8104 mov r0, r4
003d8108 bl #0x3cb458
003d810c cmp r0, #0
003d8110 beq #0x3d80e0
003d8114 ldr r0, [r4, #4]
003d8118 mvn r1, #0
003d811c bl #0x3bb98c
003d8120 ldr r2, [r4, #0xc0]
003d8124 ldr r3, [r4, #0xc4]
003d8128 mov r5, r0
003d812c rsb r3, r2, r3
003d8130 cmp r0, r3, asr #2
003d8134 blt #0x3d8158
003d8138 ldr r3, [pc, #0x6c]
003d813c ldr r3, [r6, r3]
003d8140 ldr r3, [r3]
003d8144 cmp r3, #2
003d8148 streq r7, [r7]
003d814c beq #0x3d8158
003d8150 cmp r3, #1
003d8154 beq #0x3d8170
003d8158 ldr r0, [r2, r5, lsl #2]
003d815c cmp r0, #0
003d8160 beq #0x3d80e0
003d8164 add sp, sp, #0xc
003d8168 pop {r4, r5, r6, r7, lr}
003d816c b #0x3da9dc
003d8170 ldr r0, [pc, #0x38]
003d8174 ldr r1, [pc, #0x38]
003d8178 ldr r2, [pc, #0x38]
003d817c ldr r0, [r6, r0]
003d8180 ldr r3, [pc, #0x34]
003d8184 add r2, pc, r2
003d8188 movw ip, #0x141
003d818c add r1, pc, r1
003d8190 add r0, r0, #0xa8
003d8194 add r3, pc, r3
003d8198 str ip, [sp]
003d819c bl #0x30e004
003d81a0 ldr r2, [r4, #0xc0]
003d81a4 b #0x3d8158
003d81a8 ldrheq ip, [fp], #-0x98
003d81ac andeq r3, r0, r0, asr #19
003d81b0 andeq r1, r0, r0, asr #19
003d81b4 subeq r6, lr, ip, asr #4
003d81b8 subeq sp, lr, ip, lsl r6
003d81bc umaaleq sp, lr, r4, r5

# _ZN6CharAI13AI_BeginSpellEb 0x3d81c0
003d81c0 push {r4, r5, r6, r7, r8, lr}
003d81c4 mov r4, r0
003d81c8 mov r6, r1
003d81cc ldr r0, [r0, #4]
003d81d0 mvn r1, #0
003d81d4 bl #0x3bb98c
003d81d8 mov r1, r0
003d81dc mov r5, r0
003d81e0 ldr r0, [r4, #4]
003d81e4 bl #0x3aeac0
003d81e8 ldr r7, [r0, #0x1c]
003d81ec cmp r7, #1
003d81f0 beq #0x3d82a4
003d81f4 mov r0, r4
003d81f8 bl #0x3d80b4
003d81fc cmp r0, #0
003d8200 bne #0x3d8208
003d8204 pop {r4, r5, r6, r7, r8, pc}
003d8208 ldr r7, [r4, #4]
003d820c mov r8, #0
003d8210 mvn r1, #0
003d8214 strb r8, [r4, #0xd0]
003d8218 strb r8, [r4, #0xd1]
003d821c mov r0, r7
003d8220 bl #0x3bb98c
003d8224 mov r1, r0
003d8228 add r0, r7, #0x4f0
003d822c mov r3, r8
003d8230 mov r2, r8
003d8234 add r0, r0, #0xc
003d8238 bl #0x3c6394
003d823c bl #0x7fd794
003d8240 ldrb r3, [r0, #5]
003d8244 cmp r3, r8
003d8248 beq #0x3d8290
003d824c cmp r6, r8
003d8250 bne #0x3d8290
003d8254 bl #0x80b1bc
003d8258 mov r6, r0
003d825c ldr r0, [pc, #0xc0]
003d8260 ldr r3, [r4, #4]
003d8264 mov r1, #1
003d8268 add r0, pc, r0
003d826c ldrb r7, [r3, #0x108]
003d8270 bl #0x80a244
003d8274 mov r3, #3
003d8278 mov r1, r0
003d827c strb r7, [r0, #0x54]
003d8280 strb r3, [r0, #0x50]
003d8284 strh r5, [r0, #0x52]
003d8288 mov r0, r6
003d828c bl #0x80e2a4
003d8290 ldr r0, [r4, #4]
003d8294 add r0, r0, #0x4f0
003d8298 add r0, r0, #0xc
003d829c pop {r4, r5, r6, r7, r8, lr}
003d82a0 b #0x3c0334
003d82a4 mov r0, r4
003d82a8 bl #0x3d7d9c
003d82ac cmp r0, #0
003d82b0 beq #0x3d81f4
003d82b4 ldr r3, [r4, #0xc0]
003d82b8 ldr r0, [r3, r5, lsl #2]
003d82bc bl #0x3da8b8
003d82c0 bl #0x7fd794
003d82c4 ldrb r3, [r0, #5]
003d82c8 cmp r3, #0
003d82cc beq #0x3d831c
003d82d0 cmp r6, #0
003d82d4 bne #0x3d831c
003d82d8 bl #0x80b1bc
003d82dc mov r6, r0
003d82e0 ldr r0, [pc, #0x40]
003d82e4 ldr r3, [r4, #4]
003d82e8 mov r1, r7
003d82ec add r0, pc, r0
003d82f0 ldrb r4, [r3, #0x108]
003d82f4 bl #0x80a244
003d82f8 mov r3, #3
003d82fc mov r1, r0
003d8300 strb r4, [r0, #0x54]
003d8304 strb r3, [r0, #0x50]
003d8308 strh r5, [r0, #0x52]
003d830c mov r0, r6
003d8310 bl #0x80e2a4
003d8314 mov r0, r7
003d8318 pop {r4, r5, r6, r7, r8, pc}
003d831c mov r0, #1
003d8320 b #0x3d8204
003d8324 subeq r6, lr, r0, lsr #25
003d8328 subeq r6, lr, ip, lsl ip

# _ZN6CharAI12AI_CastSpellEv 0x3d832c
003d832c push {r4, lr}
003d8330 mov r1, #0
003d8334 mov r4, r0
003d8338 bl #0x3d81c0
003d833c cmp r0, #0
003d8340 beq #0x3d8354
003d8344 mov r0, r4
003d8348 mov r1, #0
003d834c bl #0x3d7f60
003d8350 mov r0, #1
003d8354 pop {r4, pc}

# _ZN12v2Controller13Cmd_BeginCastEb 0x4055f8
004055f8 push {r4, lr}
004055fc ldrb r2, [r0, #9]
00405600 ldr r3, [pc, #0x44]
00405604 cmp r2, #0
00405608 add r3, pc, r3
0040560c bne #0x405634
00405610 ldr r2, [pc, #0x38]
00405614 ldr r3, [r3, r2]
00405618 ldrb r3, [r3]
0040561c cmp r3, #0
00405620 beq #0x405628
00405624 pop {r4, pc}
00405628 ldrb r3, [r0, #8]
0040562c cmp r3, #0
00405630 bne #0x405624
00405634 ldr r3, [r0, #4]
00405638 mov r0, r3
0040563c ldr r3, [r3]
00405640 mov lr, pc
00405644 ldr pc, [r3, #0x44]
00405648 pop {r4, pc}
0040564c subseq pc, r8, r8, lsl #9
00405650 andeq r3, r0, r0, asr r6

# _ZN12v2Controller11Cmd_EndCastEb 0x405654
00405654 push {r4, lr}
00405658 ldrb r2, [r0, #9]
0040565c ldr r3, [pc, #0x44]
00405660 cmp r2, #0
00405664 add r3, pc, r3
00405668 bne #0x405690
0040566c ldr r2, [pc, #0x38]
00405670 ldr r3, [r3, r2]
00405674 ldrb r3, [r3]
00405678 cmp r3, #0
0040567c beq #0x405684
00405680 pop {r4, pc}
00405684 ldrb r3, [r0, #8]
00405688 cmp r3, #0
0040568c bne #0x405680
00405690 ldr r3, [r0, #4]
00405694 mov r0, r3
00405698 ldr r3, [r3]
0040569c mov lr, pc
004056a0 ldr pc, [r3, #0x48]
004056a4 pop {r4, pc}
004056a8 subseq pc, r8, ip, lsr #8
004056ac andeq r3, r0, r0, asr r6

# _Z14NativeHUDSpellRKN7gameswf7fn_callE 0x43cabc
0043cabc ldr r3, [pc, #0x58]
0043cac0 ldr r2, [pc, #0x58]
0043cac4 push {r4, lr}
0043cac8 add r3, pc, r3
0043cacc ldr r0, [r3, r2]
0043cad0 mov r1, #0
0043cad4 mov r2, #1
0043cad8 ldr r0, [r0, #0x40]
0043cadc bl #0x36e478
0043cae0 ldr r4, [r0, #0x660]
0043cae4 cmp r4, #0
0043cae8 beq #0x43cb18
0043caec mov r0, r4
0043caf0 bl #0x3ad430
0043caf4 cmp r0, #0
0043caf8 beq #0x43cb18
0043cafc ldr r0, [r4, #0x378]
0043cb00 mov r1, #0
0043cb04 bl #0x4055f8
0043cb08 ldr r0, [r4, #0x378]
0043cb0c mov r1, #0
0043cb10 pop {r4, lr}
0043cb14 b #0x405654
0043cb18 pop {r4, pc}
0043cb1c subseq r7, r5, r8, asr #31
0043cb20 strdeq r3, r4, [r0], -r4

# _Z23NativeHUDSetActiveFaeryRKN7gameswf7fn_callE 0x43ee40
0043ee40 push {r4, r5, r6, r7, r8, sl, fp, lr}
0043ee44 ldr r3, [r0, #0x10]
0043ee48 ldr r4, [pc, #0x10c]
0043ee4c sub sp, sp, #0x10
0043ee50 cmp r3, #2
0043ee54 mov r5, r0
0043ee58 add r4, pc, r4
0043ee5c beq #0x43ee68
0043ee60 add sp, sp, #0x10
0043ee64 pop {r4, r5, r6, r7, r8, sl, fp, pc}
0043ee68 ldr r7, [r0, #0xc]
0043ee6c ldr r8, [r0, #0x14]
0043ee70 mov r6, #0xc
0043ee74 ldr r3, [r7]
0043ee78 mla r3, r6, r8, r3
0043ee7c ldrsb r2, [r3, #1]
0043ee80 cmp r2, #2
0043ee84 bne #0x43ee60
0043ee88 ldr r2, [r3, #8]
0043ee8c ldr r3, [r3, #4]
0043ee90 str r2, [sp, #0xc]
0043ee94 str r3, [sp, #8]
0043ee98 ldrd sl, fp, [sp, #8]
0043ee9c mov r0, sl
0043eea0 mov r2, sl
0043eea4 mov r1, fp
0043eea8 mov r3, fp
0043eeac strd sl, fp, [sp]
0043eeb0 bl #0x30e2bc
0043eeb4 subs sl, r0, #0
0043eeb8 bne #0x43ee60
0043eebc ldr r3, [r7]
0043eec0 sub r0, r8, #1
0043eec4 mla r0, r6, r0, r3
0043eec8 bl #0x439d8c
0043eecc cmp r0, #0
0043eed0 beq #0x43ee60
0043eed4 ldr r3, [r5, #0xc]
0043eed8 ldr r0, [r5, #0x14]
0043eedc ldr r3, [r3]
0043eee0 mla r0, r6, r0, r3
0043eee4 bl #0x43a1b8
0043eee8 ldr r3, [r5, #0xc]
0043eeec mov r7, r0
0043eef0 ldr r0, [r5, #0x14]
0043eef4 ldr r3, [r3]
0043eef8 sub r0, r0, #1
0043eefc mla r0, r6, r0, r3
0043ef00 bl #0x43a1b8
0043ef04 mov r1, sl
0043ef08 bl #0x43c388
0043ef0c cmp r0, #0
0043ef10 beq #0x43ee60
0043ef14 mov r1, r7
0043ef18 bl #0x3ae99c
0043ef1c ldr r3, [pc, #0x3c]
0043ef20 ldr r4, [r4, r3]
0043ef24 mov r0, r4
0043ef28 bl #0x31f594
0043ef2c cmp r0, #0
0043ef30 beq #0x43ef44
0043ef34 mov r0, r4
0043ef38 bl #0x31f594
0043ef3c mov r1, sl
0043ef40 bl #0x3f0898
0043ef44 ldr r4, [r5]
0043ef48 mov r0, r4
0043ef4c bl #0x797124
0043ef50 mov r3, #0
0043ef54 strb r3, [r4, #1]
0043ef58 b #0x43ee60
0043ef5c subseq r5, r5, r8, lsr ip
0043ef60 strdeq r3, r4, [r0], -r4

