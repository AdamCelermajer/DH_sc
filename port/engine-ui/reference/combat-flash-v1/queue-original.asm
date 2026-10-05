[('_ZN6Arrays13CharAnimTable13m_memberNamesE', '0x9a6448'), ('_ZN6Arrays13CharAnimTable4sizeE', '0x9a6440'), ('_ZN6Arrays13CharAnimTable7membersE', '0x9a6444')]
{'0x31f66c': '_ZN11Application5GetDtEv', '0x30e2e0': None, '0x30e3ac': None}
0x30e2e0 __aeabi_ui2f
0x30e3ac __aeabi_fsub

# _ZN16FlashAnimManagerC2Ev
413748 mov r3, r0
41374c add ip, r0, #0x3c0
413750 mov r2, #0
413754 mvn r1, #0
413758 str r2, [r3]
41375c str r2, [r3, #4]
413760 str r2, [r3, #8]
413764 str r2, [r3, #0xc]
413768 str r2, [r3, #0x10]
41376c str r2, [r3, #0x14]
413770 str r1, [r3, #0x18]
413774 str r2, [r3, #0x1c]
413778 strb r2, [r3, #0x20]
41377c add r3, r3, #0x50
413780 cmp r3, ip
413784 bne #0x413758
413788 mov r3, r0
41378c str r2, [r0, #0x3c0]
413790 str r2, [r0, #0x3c4]
413794 str r2, [r0, #0x3c8]
413798 str r2, [r0, #0x3cc]
41379c str r2, [r0, #0x3d4]
4137a0 strb r2, [r3, #0x3d0]!
4137a4 str r3, [r0, #0x3dc]
4137a8 str r2, [r0, #0x3e0]
4137ac str r3, [r0, #0x3d8]
4137b0 bx lr

# _ZN16FlashAnimManager26FindAvailableAnimContextIDEv
413880 mov r3, #0
413884 ldr r2, [r0, #0x14]
413888 add r0, r0, #0x50
41388c tst r2, #1
413890 beq #0x4138a4
413894 add r3, r3, #1
413898 cmp r3, #0xc
41389c bne #0x413884
4138a0 mvn r3, #0
4138a4 mov r0, r3
4138a8 bx lr

# _ZN16FlashAnimManager6UpdateEv
4138ac push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
4138b0 ldr r4, [pc, #0x168]
4138b4 ldr r6, [pc, #0x168]
4138b8 sub sp, sp, #0xc
4138bc add r4, pc, r4
4138c0 mov sl, r0
4138c4 ldr r0, [r4, r6]
4138c8 bl #0x31f594 ; _ZNK11Application15GetCurrentLevelEv
4138cc cmp r0, #0
4138d0 beq #0x413a18
4138d4 ldr r3, [r0, #0x130]
4138d8 cmp r3, #1
4138dc ble #0x413a18
4138e0 ldr r3, [r0, #0x130]
4138e4 cmp r3, #0x1a
4138e8 movgt r3, #0
4138ec movle r3, #1
4138f0 ldr r0, [sl, #0x3c0]
4138f4 cmp r0, #0
4138f8 beq #0x413904
4138fc cmp r3, #0
413900 beq #0x4139f4
413904 mov r5, #0x21
413908 ldr r0, [r4, r6]
41390c bl #0x31f66c ; _ZN11Application5GetDtEv
413910 mov r8, #0
413914 mov r4, sl
413918 mov fp, #0x60
41391c mov sb, #0xc
413920 str r0, [sp, #4]
413924 ldr r7, [r4, #0x14]
413928 tst r7, #1
41392c beq #0x4139a0
413930 ldr r1, [sp, #4]
413934 ldr r3, [r4, #0x10]
413938 ldr r6, [r4, #0x18]
41393c ldr r2, [r4, #0x1c]
413940 add r3, r1, r3
413944 str r3, [r4, #0x10]
413948 ldr r1, [sl, #0x3c4]
41394c cmp r3, r5
413950 and r7, r7, #2
413954 mla r6, fp, r6, r1
413958 mla r6, sb, r2, r6
41395c rsb r2, r5, r3
413960 ble #0x4139a0
413964 ldr r3, [r4, #0xc]
413968 str r2, [r4, #0x10]
41396c add r3, r3, #1
413970 str r3, [r4, #0xc]
413974 ldr r3, [r6]
413978 mov r0, r3
41397c ldr r3, [r3]
413980 mov lr, pc
413984 ldr pc, [r3, #0x13c]
413988 cmp r7, #0
41398c bne #0x4139b8
413990 ldr r3, [r4, #0x10]
413994 cmp r3, r5
413998 rsb r2, r5, r3
41399c bgt #0x413964
4139a0 add r8, r8, #1
4139a4 cmp r8, #0xc
4139a8 add r4, r4, #0x50
4139ac bne #0x413924
4139b0 add sp, sp, #0xc
4139b4 pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
4139b8 ldr r3, [r4, #0xc]
4139bc cmp r0, r3
4139c0 bgt #0x413990
4139c4 mov r1, r8
4139c8 mov r0, sl
4139cc bl #0x413868 ; _ZN16FlashAnimManager13StopFlashAnimEi
4139d0 ldr r1, [r4, #0x18]
4139d4 ldr r2, [sl, #0x3c4]
4139d8 ldr r3, [r4, #0x1c]
4139dc mla r2, fp, r1, r2
4139e0 mla r3, sb, r3, r2
4139e4 mov r2, #0
4139e8 strb r2, [r3, #8]
4139ec ldr r3, [r4, #0x10]
4139f0 b #0x413994
4139f4 bl #0x7a7cac ; _ZNK8RenderFX7GetRootEv
4139f8 bl #0x7741a0 ; _ZNK7gameswf4root14get_frame_rateEv
4139fc mov r1, r0
413a00 mov r0, #0x44000000
413a04 add r0, r0, #0x7a0000
413a08 bl #0x30ec94 ; 
413a0c bl #0x30e4cc ; 
413a10 mov r5, r0
413a14 b #0x413908
413a18 mov r3, #0
413a1c b #0x4138f0
413a20 ldrsbeq r1, [r8], #-0x14
413a24 strdeq r3, r4, [r0], -r4

# _ZN16FlashAnimManager18UpdateAnimInstanceEP16FlashAnimContext
413a28 str r4, [sp, #-4]!
413a2c ldr r2, [r1, #0x18]
413a30 ldr r4, [r0, #0x3c4]
413a34 ldr ip, [r1, #0x1c]
413a38 mov r3, r1
413a3c mov r1, #0x60
413a40 mla r1, r1, r2, r4
413a44 mov r2, #0xc
413a48 mla r2, r2, ip, r1
413a4c ldr r1, [r2, #4]
413a50 cmp r1, #0
413a54 beq #0x413a70
413a58 ldr r2, [pc, #0x18]
413a5c ldr r0, [r0, #0x3c0]
413a60 add r3, r3, #0x20
413a64 add r2, pc, r2
413a68 ldm sp!, {r4}
413a6c b #0x7a947c
413a70 ldm sp!, {r4}
413a74 bx lr
413a78 subeq fp, sp, ip, lsl #7

# _ZN16FlashAnimManager16FindAnimInstanceEi
413b1c push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
413b20 ldr r5, [pc, #0x134]
413b24 ldr r7, [pc, #0x134]
413b28 mov r8, #0x60
413b2c add r5, pc, r5
413b30 ldr r3, [r5, r7]
413b34 ldr sl, [r0, #0x3c4]
413b38 mul r8, r8, r1
413b3c ldr r3, [r3]
413b40 sub sp, sp, #0x64
413b44 add r2, sl, r8
413b48 mov sb, r0
413b4c mov r4, #0
413b50 str r3, [sp, #0x5c]
413b54 mov r6, r2
413b58 ldrb r3, [r6, #8]
413b5c cmp r3, #0
413b60 beq #0x413b7c
413b64 add r4, r4, #1
413b68 cmp r4, #8
413b6c add r6, r6, #0xc
413b70 bne #0x413b58
413b74 add r6, r2, #0x54
413b78 mov r4, #7
413b7c ldr r3, [r6]
413b80 cmp r3, #0
413b84 beq #0x413bb0
413b88 ldr r3, [r5, r7]
413b8c mov r2, #1
413b90 strb r2, [r6, #8]
413b94 ldr r2, [sp, #0x5c]
413b98 ldr r3, [r3]
413b9c mov r0, r4
413ba0 cmp r2, r3
413ba4 bne #0x413c58
413ba8 add sp, sp, #0x64
413bac pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
413bb0 ldr r1, [pc, #0xac]
413bb4 add r3, sp, #8
413bb8 mov r2, r4
413bbc add r1, pc, r1
413bc0 mov r0, r3
413bc4 str r3, [sp, #4]
413bc8 bl #0x30eae4 ; 
413bcc ldr r2, [sl, r8]
413bd0 ldr r3, [sp, #4]
413bd4 add fp, sp, #0x48
413bd8 mov r0, fp
413bdc mov r1, r3
413be0 ldr r3, [r2]
413be4 ldr r3, [r3, #0xd4]
413be8 str r3, [sp, #4]
413bec bl #0x413a7c ; _ZN7gameswf9tu_stringC1EPKc
413bf0 ldr r8, [sl, r8]
413bf4 add r0, r8, #0x3c
413bf8 bl #0x386144 ; _ZNK7gameswf8weak_ptrINS_9characterEE11check_proxyEv
413bfc ldr r0, [r8, #0x40]
413c00 bl #0x77ed2c ; _ZN7gameswf15sprite_instance17get_highest_depthEv
413c04 ldr r3, [sp, #4]
413c08 mov r2, r0
413c0c mov r1, fp
413c10 mov r0, r8
413c14 blx r3
413c18 str r0, [r6]
413c1c ldrsb r3, [sp, #0x48]
413c20 mov r2, r0
413c24 cmn r3, #1
413c28 beq #0x413c44
413c2c ldr r1, [pc, #0x34]
413c30 ldr r0, [sb, #0x3c0]
413c34 add r1, pc, r1
413c38 bl #0x7a8a84 ; _ZN8RenderFX4FindEPKcPN7gameswf9characterE
413c3c str r0, [r6, #4]
413c40 b #0x413b88
413c44 ldr r0, [sp, #0x54]
413c48 ldr r1, [sp, #0x50]
413c4c bl #0x752b38 ; _ZN7gameswf13free_internalEPvj
413c50 ldr r2, [r6]
413c54 b #0x413c2c
413c58 bl #0x30e310 ; 
413c5c subseq r0, r8, r4, ror #30
413c60 andeq r4, r0, ip, lsr #1
413c64 subeq r4, fp, ip, lsr #9
413c68 subeq r4, fp, r4, asr #8

# _ZN16FlashAnimManager13PlayFlashAnimEiiiii
413c6c push {r4, r5, r6, r7, r8, sb, sl, lr}
413c70 mov r4, r1
413c74 mov r5, r2
413c78 mov sb, r3
413c7c mov r6, r0
413c80 bl #0x413880 ; _ZN16FlashAnimManager26FindAvailableAnimContextIDEv
413c84 subs r8, r0, #0
413c88 movlt r7, #0
413c8c blt #0x413cec
413c90 mov sl, #0x50
413c94 mul sl, sl, r8
413c98 mov r1, r4
413c9c add r7, r6, sl
413ca0 str r4, [r7, #0x18]
413ca4 mov r0, r6
413ca8 bl #0x413b1c ; _ZN16FlashAnimManager16FindAnimInstanceEi
413cac mov r1, #0xa
413cb0 ldr r2, [sp, #0x24]
413cb4 mul r1, r1, r8
413cb8 str r0, [r7, #0x1c]
413cbc str r5, [r6, sl]
413cc0 str sb, [r7, #4]
413cc4 ldr r0, [sp, #0x20]
413cc8 add r8, r8, r8, lsl #2
413ccc add r8, r8, #1
413cd0 add r1, r6, r1, lsl #3
413cd4 str r0, [r1, #0xc]
413cd8 add r3, r6, r8, lsl #4
413cdc orr r2, r2, #1
413ce0 mov r1, #0
413ce4 str r1, [r6, r8, lsl #4]
413ce8 str r2, [r3, #4]
413cec mov r0, r7
413cf0 pop {r4, r5, r6, r7, r8, sb, sl, pc}

# _ZN16FlashAnimManager13PlayFlashAnimEiRKN6glitch4core8vector3dIfEEii
413cf4 push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
413cf8 sub sp, sp, #0x14
413cfc mov r4, r0
413d00 mov r5, r1
413d04 mov r0, r2
413d08 add r1, sp, #8
413d0c mov r2, #0
413d10 mov r7, #0x60
413d14 str r2, [sp, #0xc]
413d18 str r2, [sp, #8]
413d1c mov r6, r3
413d20 mul r7, r7, r5
413d24 bl #0x50e830 ; _Z24GetScreenPosFromWorldPosRKN6glitch4core8vector3dIfEERNS0_10position2dIiEE
413d28 ldr r8, [r4, #0x3c4]
413d2c ldr fp, [sp, #8]
413d30 ldr sl, [sp, #0xc]
413d34 ldr r3, [r8, r7]
413d38 mov r0, r3
413d3c ldr r3, [r3]
413d40 mov lr, pc
413d44 ldr pc, [r3, #0x54]
413d48 bl #0x416538 ; _ZN12GameSWFUtils17GetInvPixelScaleXEPN7gameswf4rootE
413d4c ldr r3, [r8, r7]
413d50 mov sb, r0
413d54 mov r0, r3
413d58 ldr r3, [r3]
413d5c mov lr, pc
413d60 ldr pc, [r3, #0x54]
413d64 bl #0x416578 ; _ZN12GameSWFUtils17GetInvPixelScaleYEPN7gameswf4rootE
413d68 mov r8, r0
413d6c mov r0, fp
413d70 bl #0x30e964 ; 
413d74 mov r1, r0
413d78 mov r0, sb
413d7c bl #0x30ed6c ; 
413d80 bl #0x30e4cc ; 
413d84 mov r7, r0
413d88 mov r0, sl
413d8c bl #0x30e964 ; 
413d90 mov r1, r0
413d94 mov r0, r8
413d98 bl #0x30ed6c ; 
413d9c bl #0x30e4cc ; 
413da0 ldr ip, [sp, #0x38]
413da4 mov r3, r0
413da8 mov r1, r5
413dac mov r0, r4
413db0 mov r2, r7
413db4 stm sp, {r6, ip}
413db8 bl #0x413c6c ; _ZN16FlashAnimManager13PlayFlashAnimEiiiii
413dbc add sp, sp, #0x14
413dc0 pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}

# _ZN16FlashAnimManager23PlayScrollingCombatTextEiRKN6glitch4core8vector3dIfEEPKci
413dc4 push {r4, r5, r6, lr}
413dc8 mov ip, #2
413dcc sub sp, sp, #8
413dd0 mov r5, r3
413dd4 mov r3, #0
413dd8 str ip, [sp]
413ddc mov r4, r0
413de0 bl #0x413cf4 ; _ZN16FlashAnimManager13PlayFlashAnimEiRKN6glitch4core8vector3dIfEEii
413de4 subs r6, r0, #0
413de8 beq #0x413e08
413dec ldr r3, [sp, #0x18]
413df0 mov r1, r5
413df4 str r3, [r6, #8]
413df8 bl #0x413aec ; _ZN16FlashAnimContext7SetTextEPKc
413dfc mov r0, r4
413e00 mov r1, r6
413e04 bl #0x413a28 ; _ZN16FlashAnimManager18UpdateAnimInstanceEP16FlashAnimContext
413e08 mov r0, r6
413e0c add sp, sp, #8
413e10 pop {r4, r5, r6, pc}

# _ZN16FlashAnimContext15SetTextToNumberEi
413f14 push {r4, r5, r6, r7, r8, lr}
413f18 ldr r4, [pc, #0x74]
413f1c subs r5, r1, #0
413f20 movlt r3, #0
413f24 strblt r3, [r0, #0x20]
413f28 add r4, pc, r4
413f2c ldr r3, [r4, #0x3ec]
413f30 mov r6, r0
413f34 tst r3, #1
413f38 beq #0x413f6c
413f3c ldr r3, [pc, #0x54]
413f40 add r3, pc, r3
413f44 ldr r3, [r3, #0x3f0]
413f48 cmp r5, r3
413f4c blt #0x413f54
413f50 pop {r4, r5, r6, r7, r8, pc}
413f54 ldr r1, [pc, #0x40]
413f58 add r0, r6, #0x20
413f5c mov r2, r5
413f60 add r1, pc, r1
413f64 pop {r4, r5, r6, r7, r8, lr}
413f68 b #0x30eae4
413f6c add r7, r4, #0x3ec
413f70 mov r0, r7
413f74 bl #0x30e76c ; 
413f78 cmp r0, #0
413f7c beq #0x413f3c
413f80 mvn r3, #0x80000001
413f84 str r3, [r4, #0x3f0]
413f88 mov r0, r7
413f8c bl #0x30ea3c ; 
413f90 b #0x413f3c
413f94 subseq pc, r8, r4, lsr r3
413f98 subseq pc, r8, ip, lsl r3
413f9c subeq sp, sl, r0, asr pc

# _ZN16FlashAnimManager24PlayScrollingCombatValueEiRKN6glitch4core8vector3dIfEEii
413fa0 push {r4, r5, r6, lr}
413fa4 mov ip, #2
413fa8 sub sp, sp, #8
413fac mov r5, r3
413fb0 mov r3, #0
413fb4 str ip, [sp]
413fb8 mov r4, r0
413fbc bl #0x413cf4 ; _ZN16FlashAnimManager13PlayFlashAnimEiRKN6glitch4core8vector3dIfEEii
413fc0 subs r6, r0, #0
413fc4 beq #0x413fe4
413fc8 ldr r3, [sp, #0x18]
413fcc mov r1, r5
413fd0 str r3, [r6, #8]
413fd4 bl #0x413f14 ; _ZN16FlashAnimContext15SetTextToNumberEi
413fd8 mov r0, r4
413fdc mov r1, r6
413fe0 bl #0x413a28 ; _ZN16FlashAnimManager18UpdateAnimInstanceEP16FlashAnimContext
413fe4 mov r0, r6
413fe8 add sp, sp, #8
413fec pop {r4, r5, r6, pc}

# _ZN16FlashAnimManager4DrawEv
414260 push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
414264 ldr r1, [pc, #0x208]
414268 ldr r2, [pc, #0x208]
41426c sub sp, sp, #0x3c
414270 add r1, pc, r1
414274 ldr r3, [r1, r2]
414278 str r2, [sp, #0x14]
41427c ldr r2, [pc, #0x1f8]
414280 ldr r3, [r3]
414284 mov r6, r0
414288 ldr r5, [r1, r2]
41428c str r3, [sp, #0x34]
414290 str r1, [sp, #0xc]
414294 mov r0, r5
414298 bl #0x337888 ; _ZN13DebugSwitches4loadEv
41429c ldr r1, [pc, #0x1dc]
4142a0 add r4, sp, #0x1c
4142a4 add r2, sp, #0x18
4142a8 add r1, pc, r1
4142ac mov r0, r4
4142b0 bl #0x3140ec ; _ZNSsC1EPKcRKSaIcE
4142b4 mov r0, r5
4142b8 mov r1, r4
4142bc bl #0x337a88 ; _ZN13DebugSwitches9GetSwitchERKSs
4142c0 mov r5, r0
4142c4 ldr r0, [sp, #0x30]
4142c8 cmp r0, r4
4142cc beq #0x4142ec
4142d0 cmp r0, #0
4142d4 beq #0x4142ec
4142d8 ldr r1, [sp, #0x1c]
4142dc rsb r1, r0, r1
4142e0 cmp r1, #0x80
4142e4 bhi #0x414468
4142e8 bl #0x708f00 ; 
4142ec cmp r5, #0
4142f0 bne #0x414444
4142f4 ldr r0, [r6, #0x3c0]
4142f8 cmp r0, #0
4142fc beq #0x414444
414300 mov r1, r5
414304 bl #0x7a7cb4 ; _ZN8RenderFX23SetTextBufferingEnabledEb
414308 ldr r0, [r6, #0x3c0]
41430c bl #0x7a9afc ; _ZN8RenderFX12BeginDisplayEv
414310 mov r4, r6
414314 add sl, r6, #0x3c0
414318 ldr r3, [r4, #0x14]
41431c tst r3, #1
414320 beq #0x414424
414324 ldr r5, [r4, #0x18]
414328 ldr r3, [r6, #0x3c4]
41432c ldr r8, [r4, #0x1c]
414330 mov r0, #0x60
414334 mov r1, #0xc
414338 mla r5, r0, r5, r3
41433c mul r7, r1, r8
414340 ldr fp, [r5, r7]
414344 add ip, r5, r7
414348 cmp fp, #0
41434c beq #0x414424
414350 ldr r3, [fp, #0x4c]
414354 mov r1, #0x41000000
414358 add r1, r1, #0xa00000
41435c ldr r0, [r3, #8]
414360 str ip, [sp, #4]
414364 str r3, [sp, #8]
414368 bl #0x30ec94 ; 
41436c bl #0x30e4cc ; 
414370 ldr r3, [sp, #8]
414374 mov r1, #0x41000000
414378 add r1, r1, #0xa00000
41437c mov sb, r0
414380 ldr r0, [r3, #0x14]
414384 bl #0x30ec94 ; 
414388 bl #0x30e4cc ; 
41438c str r0, [sp, #0x10]
414390 ldm r4, {r2, r3}
414394 mov r1, fp
414398 add r2, sb, r2
41439c add r3, r0, r3
4143a0 ldr r0, [r6, #0x3c0]
4143a4 bl #0x7aa3f0 ; _ZN8RenderFX11SetPositionEPN7gameswf9characterEii
4143a8 ldr r1, [r5, r7]
4143ac ldr r0, [r6, #0x3c0]
4143b0 ldr r2, [r4, #0xc]
4143b4 mov r3, #0
4143b8 bl #0x7a7d34 ; _ZN8RenderFX9GotoFrameEPN7gameswf9characterEib
4143bc ldr ip, [sp, #4]
4143c0 ldr r1, [ip, #4]
4143c4 cmp r1, #0
4143c8 beq #0x4143dc
4143cc ldr r0, [r6, #0x3c0]
4143d0 mov r2, #0xff000000
4143d4 ldr r3, [r4, #8]
4143d8 bl #0x7a9dd0 ; _ZN8RenderFX17SetColorTransformEPN7gameswf9characterEjj
4143dc mov r2, #0xc
4143e0 mul r8, r2, r8
4143e4 mov r0, #1
4143e8 ldr r3, [r5, r8]
4143ec strb r0, [r3, #0x9b]
4143f0 ldr r3, [r5, r8]
4143f4 mov r0, r3
4143f8 ldr r3, [r3]
4143fc mov lr, pc
414400 ldr pc, [r3, #0x120]
414404 ldr r1, [r5, r8]
414408 mov r0, #0
41440c ldr r3, [sp, #0x10]
414410 strb r0, [r1, #0x9b]
414414 mov r2, sb
414418 ldr r1, [r5, r8]
41441c ldr r0, [r6, #0x3c0]
414420 bl #0x7aa3f0 ; _ZN8RenderFX11SetPositionEPN7gameswf9characterEii
414424 add r4, r4, #0x50
414428 cmp r4, sl
41442c bne #0x414318
414430 ldr r0, [r6, #0x3c0]
414434 mov r1, #1
414438 bl #0x7a7cb4 ; _ZN8RenderFX23SetTextBufferingEnabledEb
41443c ldr r0, [r6, #0x3c0]
414440 bl #0x7a9ac8 ; _ZN8RenderFX10EndDisplayEv
414444 ldr r2, [sp, #0xc]
414448 ldr r1, [sp, #0x14]
41444c ldr r3, [r2, r1]
414450 ldr r2, [sp, #0x34]
414454 ldr r3, [r3]
414458 cmp r2, r3
41445c bne #0x414470
414460 add sp, sp, #0x3c
414464 pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
414468 bl #0x310440 ; _Z10CustomFreePv
41446c b #0x4142ec
414470 bl #0x30e310 ; 
414474 subseq r0, r8, r0, lsr #16
414478 andeq r4, r0, ip, lsr #1
41447c andeq r0, r0, r4, lsl #17
414480 ldrdeq r3, r4, [fp], #-0xd8

# _ZNK16FlashAnimManager18GetStyleIdFromNameEPKc
414678 push {r4, lr}
41467c sub sp, sp, #8
414680 add r3, sp, #8
414684 str r1, [r3, #-4]!
414688 add r4, r0, #0x3d0
41468c mov r1, r3
414690 mov r0, r4
414694 bl #0x414484 ; _ZNKSt4priv8_Rb_treeISsSt4lessISsESt4pairIKSsiENS_10_Select1stIS5_EENS_11_MapTraitsTIS5_EESaIS5_EE7_M_findIPKcEEPNS_18_Rb_tree_node_baseERKT_
414698 cmp r0, r4
41469c moveq r0, #0
4146a0 ldrne r0, [r0, #0x28]
4146a4 add sp, sp, #8
4146a8 pop {r4, pc}
