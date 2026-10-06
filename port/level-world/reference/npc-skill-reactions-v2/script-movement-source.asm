_ZN10GameObject6PathToERK7Point3DIfE
003939f0 push     {r4, r5, r6, r7, r8, sb, sl, lr}
003939f4 ldrb     r3, [r0, #0x84]
003939f8 ldr      r4, [pc, #0xdc]
003939fc mov      r6, r0
00393a00 cmp      r3, #0
00393a04 mov      r5, r1
00393a08 add      r4, pc, r4
00393a0c bne      #0x393ad8
00393a10 mov      r2, r0
00393a14 ldr      r3, [r2, #0x200]!
00393a18 cmp      r3, r2
00393a1c beq      #0x393ab4
00393a20 ldr      r3, [r3]
00393a24 cmp      r2, r3
00393a28 bne      #0x393a20
00393a2c ldr      r1, [r5]
00393a30 ldr      r0, [r6, #0x208]
00393a34 bl       #0x30e3ac
00393a38 ldr      r1, [r5, #4]
00393a3c mov      r7, r0
00393a40 ldr      r0, [r6, #0x20c]
00393a44 bl       #0x30e3ac
00393a48 ldr      r1, [r5, #8]
00393a4c mov      sl, r0
00393a50 ldr      r0, [r6, #0x210]
00393a54 bl       #0x30e3ac
00393a58 mov      r1, r7
00393a5c mov      r8, r0
00393a60 mov      r0, r7
00393a64 bl       #0x30ed6c
00393a68 mov      r1, sl
00393a6c mov      r7, r0
00393a70 mov      r0, sl
00393a74 bl       #0x30ed6c
00393a78 mov      r1, r0
00393a7c mov      r0, r7
00393a80 bl       #0x30eba4
00393a84 mov      r1, r8
00393a88 mov      r7, r0
00393a8c mov      r0, r8
00393a90 bl       #0x30ed6c
00393a94 mov      r1, r0
00393a98 mov      r0, r7
00393a9c bl       #0x30eba4
00393aa0 mov      r1, #0x47000000
00393aa4 add      r1, r1, #0x1c4000
00393aa8 bl       #0x30e2f8
00393aac cmp      r0, #0
00393ab0 beq      #0x393ad8
00393ab4 ldr      r2, [pc, #0x24]
00393ab8 ldr      r3, [r6, #0x26c]
00393abc add      r1, r6, #0x1c8
00393ac0 ldr      r0, [r4, r2]
00393ac4 cmp      r3, #0
00393ac8 moveq    r3, #0x1e
00393acc mov      r2, r5
00393ad0 pop      {r4, r5, r6, r7, r8, sb, sl, lr}
00393ad4 b        #0x52db48
00393ad8 pop      {r4, r5, r6, r7, r8, sb, sl, pc}
00393adc rsbeq    r1, r0, r8, lsl #1
00393ae0 andeq    r1, r0, r4, lsl #4
_ZN12v2Controller10Cmd_MoveToERK7Point3DIfE
004054e4 push     {r4, lr}
004054e8 ldrb     r2, [r0, #9]
004054ec ldr      r3, [pc, #0x44]
004054f0 cmp      r2, #0
004054f4 add      r3, pc, r3
004054f8 bne      #0x405520
004054fc ldr      r2, [pc, #0x38]
00405500 ldr      r3, [r3, r2]
00405504 ldrb     r3, [r3]
00405508 cmp      r3, #0
0040550c beq      #0x405514
00405510 pop      {r4, pc}
00405514 ldrb     r3, [r0, #8]
00405518 cmp      r3, #0
0040551c bne      #0x405510
00405520 ldr      r3, [r0, #4]
00405524 mov      r0, r3
00405528 ldr      r3, [r3]
0040552c mov      lr, pc
00405530 ldr      pc, [r3, #0x2c]
00405534 pop      {r4, pc}
_ZN9Character11Ctrl_MoveToERK7Point3DIfE
003ada30 push     {r4, r5, r6, lr}
003ada34 ldr      r3, [r0]
003ada38 mov      r4, r0
003ada3c mov      r5, r1
003ada40 mov      lr, pc
003ada44 ldr      pc, [r3, #0x54]
003ada48 cmp      r0, #0
003ada4c beq      #0x3ada54
003ada50 pop      {r4, r5, r6, pc}
003ada54 mov      r0, r4
003ada58 mov      r1, r5
003ada5c pop      {r4, r5, r6, lr}
003ada60 b        #0x3939f0
_ZN9Character11Ctrl_MoveToEP10GameObject
003ad9e4 push     {r4, r5, r6, lr}
003ad9e8 ldr      r3, [r0]
003ad9ec mov      r4, r0
003ad9f0 mov      r5, r1
003ad9f4 mov      lr, pc
003ad9f8 ldr      pc, [r3, #0x54]
003ad9fc cmp      r0, #0
003ada00 bne      #0x3ada24
003ada04 cmp      r5, #0
003ada08 beq      #0x3ada24
003ada0c mov      r0, r5
003ada10 bl       #0x3935dc ; _ZNK10GameObject17GetTargetPositionEv
003ada14 mov      r1, r0
003ada18 mov      r0, r4
003ada1c pop      {r4, r5, r6, lr}
003ada20 b        #0x3939f0
003ada24 pop      {r4, r5, r6, pc}
_ZNK10GameObject12GetLookAtVecER7Point3DIfE
00393ae4 push     {r4, r5, r6, lr}
00393ae8 ldr      r4, [r0, #0x174]
00393aec mov      r5, r1
00393af0 mov      r0, r4
00393af4 bl       #0x30eb08
00393af8 mov      r6, r0
00393afc mov      r0, r4
00393b00 bl       #0x30e754
00393b04 mov      r3, #0
00393b08 add      r0, r0, #0x80000000
00393b0c str      r6, [r5]
00393b10 str      r3, [r5, #8]
00393b14 str      r0, [r5, #4]
00393b18 pop      {r4, r5, r6, pc}
_ZN8PFObjectC1Ev
00524644 ldr      r3, [pc, #0xf4]
00524648 ldr      r1, [pc, #0xf4]
0052464c push     {r4, r5, r6, r7, r8, lr}
00524650 add      r3, pc, r3
00524654 ldr      lr, [r3, r1]
00524658 mov      r1, #8
0052465c str      r1, [r0, #4]
00524660 mov      r2, #0
00524664 mov      ip, #0
00524668 mov      r5, #0x3f800000
0052466c mov      r1, #2
00524670 str      ip, [r0]
00524674 str      ip, [r0, #0xc]
00524678 str      ip, [r0, #0x10]
0052467c str      r2, [r0, #0x18]
00524680 str      r2, [r0, #0x1c]
00524684 str      r2, [r0, #0x20]
00524688 str      r1, [r0, #0x14]
0052468c str      r5, [r0, #8]
00524690 ldr      r6, [lr]
00524694 mov      r4, r0
00524698 ldr      r0, [pc, #0xa8]
0052469c str      r6, [r4, #0x24]
005246a0 ldr      r6, [lr, #4]
005246a4 ldr      r0, [r3, r0]
005246a8 ldr      r1, [pc, #0x9c]
005246ac str      r6, [r4, #0x28]
005246b0 ldr      r7, [lr, #8]
005246b4 add      r6, r4, #0x38
005246b8 add      lr, r4, #0x8c
005246bc add      r0, r0, #8
005246c0 add      r1, pc, r1
005246c4 str      r0, [r4, #0x4c]
005246c8 str      r2, [r4, #0x34]
005246cc str      r2, [r4, #0x40]
005246d0 str      r2, [r4, #0x44]
005246d4 str      r2, [r4, #0x48]
005246d8 str      ip, [r4, #0x50]
005246dc str      ip, [r4, #0x54]
005246e0 str      r2, [r4, #0x5c]
005246e4 str      r2, [r4, #0x60]
005246e8 str      r2, [r4, #0x64]
005246ec str      r2, [r4, #0x68]
005246f0 mov      r0, lr
005246f4 str      r7, [r4, #0x2c]
005246f8 str      r6, [r4, #0x3c]
005246fc str      r5, [r4, #0x58]
00524700 str      r5, [r4, #0x30]
00524704 str      r6, [r4, #0x38]
00524708 add      r1, r1, #1
0052470c str      r2, [r4, #0x6c]
00524710 str      r2, [r4, #0x70]
00524714 str      ip, [r4, #0x7c]
00524718 str      r2, [r4, #0x88]
0052471c str      r2, [r4, #0x74]
00524720 str      r2, [r4, #0x78]
00524724 str      r2, [r4, #0x80]
00524728 str      r2, [r4, #0x84]
0052472c str      lr, [r4, #0x9c]
00524730 str      lr, [r4, #0xa0]
00524734 bl       #0x5244e8 ; _ZNSs19_M_range_initializeEPKcS0_.clone.4
00524738 mov      r0, r4
0052473c pop      {r4, r5, r6, r7, r8, pc}
00524740 subeq    r0, r7, r0, asr #8
00524744 andeq    r4, r0, r0, asr #6
00524748 andeq    r1, r0, r8, ror r2
0052474c eorseq   lr, ip, r0, lsr #6
_ZN10GameObjectC1EN10ObjectBase6GO_IDSE
0038c130 push     {r4, r5, r6, r7, lr}
0038c134 ldr      r6, [pc, #0x254]
0038c138 sub      sp, sp, #0xc
0038c13c mov      r4, r0
0038c140 bl       #0x33f310 ; _ZN10ObjectBaseC2ENS_6GO_IDSE
0038c144 ldr      r2, [pc, #0x248]
0038c148 add      r6, pc, r6
0038c14c mov      r3, #0
0038c150 ldr      r2, [r6, r2]
0038c154 mov      r5, #0
0038c158 str      r3, [r4, #0x120]
0038c15c add      r1, r2, #0xe4
0038c160 add      r0, r2, #8
0038c164 add      r2, r2, #0xd8
0038c168 str      r2, [r4, #4]
0038c16c str      r1, [r4, #0x24]
0038c170 str      r0, [r4]
0038c174 str      r3, [r4, #0x124]
0038c178 str      r3, [r4, #0x128]
0038c17c str      r3, [r4, #0x12c]
0038c180 str      r3, [r4, #0x130]
0038c184 str      r3, [r4, #0x134]
0038c188 str      r3, [r4, #0x138]
0038c18c str      r3, [r4, #0x13c]
0038c190 str      r3, [r4, #0x140]
0038c194 str      r3, [r4, #0x144]
0038c198 str      r3, [r4, #0x148]
0038c19c str      r3, [r4, #0x14c]
0038c1a0 str      r3, [r4, #0x150]
0038c1a4 str      r3, [r4, #0x154]
0038c1a8 str      r3, [r4, #0x158]
0038c1ac str      r3, [r4, #0x160]
0038c1b0 str      r3, [r4, #0x164]
0038c1b4 str      r3, [r4, #0x168]
0038c1b8 str      r3, [r4, #0x16c]
0038c1bc str      r3, [r4, #0x170]
0038c1c0 str      r3, [r4, #0x174]
0038c1c4 str      r3, [r4, #0x178]
0038c1c8 str      r3, [r4, #0x184]
0038c1cc str      r3, [r4, #0x188]
0038c1d0 str      r3, [r4, #0x18c]
0038c1d4 str      r3, [r4, #0x190]
0038c1d8 str      r3, [r4, #0x194]
0038c1dc strb     r5, [r4, #0x15c]
0038c1e0 str      r5, [r4, #0x180]
0038c1e4 add      r0, r4, #0x1c8
0038c1e8 str      r3, [r4, #0x198]
0038c1ec str      r3, [r4, #0x1c0]
0038c1f0 str      r3, [r4, #0x19c]
0038c1f4 str      r3, [r4, #0x1a0]
0038c1f8 str      r3, [r4, #0x1a4]
0038c1fc str      r3, [r4, #0x1a8]
0038c200 str      r3, [r4, #0x1ac]
0038c204 str      r3, [r4, #0x1b0]
0038c208 strb     r5, [r4, #0x1b4]
0038c20c strb     r5, [r4, #0x1b5]
0038c210 str      r3, [r4, #0x1b8]
0038c214 str      r3, [r4, #0x1bc]
0038c218 strb     r5, [r4, #0x1c4]
0038c21c bl       #0x524644 ; _ZN8PFObjectC1Ev
0038c220 add      r3, r4, #0x278
0038c224 mvn      r7, #0
0038c228 mov      r2, #0x64
0038c22c str      r2, [r4, #0x274]
0038c230 mov      r0, r3
0038c234 str      r3, [r4, #0x288]
0038c238 str      r3, [r4, #0x28c]
0038c23c str      r5, [r4, #0x26c]
0038c240 str      r7, [r4, #0x270]
0038c244 mov      r1, #0x10
0038c248 bl       #0x31167c ; _ZNSt4priv12_String_baseIcSaIcEE17_M_allocate_blockEj
0038c24c ldr      r2, [r4, #0x288]
0038c250 add      r3, r4, #0x290
0038c254 mov      r0, r3
0038c258 strb     r5, [r2]
0038c25c mov      r1, #0x10
0038c260 str      r3, [r4, #0x2a0]
0038c264 str      r3, [r4, #0x2a4]
0038c268 bl       #0x31167c ; _ZNSt4priv12_String_baseIcSaIcEE17_M_allocate_blockEj
0038c26c ldr      r2, [r4, #0x2a0]
0038c270 add      r3, r4, #0x2a8
0038c274 mov      r0, r3
0038c278 strb     r5, [r2]
0038c27c mov      r1, #0x10
0038c280 str      r3, [r4, #0x2b8]
0038c284 str      r3, [r4, #0x2bc]
0038c288 bl       #0x31167c ; _ZNSt4priv12_String_baseIcSaIcEE17_M_allocate_blockEj
0038c28c ldr      r2, [r4, #0x2b8]
0038c290 add      r3, r4, #0x2c0
0038c294 mov      r0, r3
0038c298 strb     r5, [r2]
0038c29c mov      r1, #0x10
0038c2a0 str      r3, [r4, #0x2d0]
0038c2a4 str      r3, [r4, #0x2d4]
0038c2a8 bl       #0x31167c ; _ZNSt4priv12_String_baseIcSaIcEE17_M_allocate_blockEj
0038c2ac ldr      r3, [r4, #0x2d0]
0038c2b0 mov      ip, #1
0038c2b4 add      r6, r4, #0x304
0038c2b8 strb     r5, [r3]
0038c2bc mov      r2, ip
0038c2c0 strb     ip, [r4, #0x2ee]
0038c2c4 strb     ip, [r4, #0x2fb]
0038c2c8 str      r5, [r4, #0x2d8]
0038c2cc str      r5, [r4, #0x2dc]
0038c2d0 str      r5, [r4, #0x2e0]
0038c2d4 str      r5, [r4, #0x2e4]
0038c2d8 str      r5, [r4, #0x2e8]
0038c2dc strb     r5, [r4, #0x2ec]
0038c2e0 strb     r5, [r4, #0x2ed]
0038c2e4 strb     r5, [r4, #0x2ef]
0038c2e8 strb     r5, [r4, #0x2f0]
0038c2ec str      r5, [r4, #0x2f4]
0038c2f0 strb     r5, [r4, #0x2f8]
0038c2f4 strb     r5, [r4, #0x2f9]
0038c2f8 strb     r5, [r4, #0x2fa]
0038c2fc strb     r5, [r4, #0x2fc]
0038c300 str      r5, [r4, #0x300]
0038c304 mov      r3, r5
0038c308 mov      r1, r5
0038c30c mov      r0, r6
0038c310 str      ip, [sp]
0038c314 bl       #0x4a2730 ; _ZN14ObjectSearcher10TargetListC1EP10GameObjectiii
0038c318 add      r3, r4, #0x358
0038c31c mov      r0, r3
0038c320 str      r3, [r4, #0x368]
0038c324 str      r3, [r4, #0x36c]
0038c328 mov      r1, #0x10
0038c32c bl       #0x31167c ; _ZNSt4priv12_String_baseIcSaIcEE17_M_allocate_blockEj
0038c330 ldr      r1, [r4, #0x368]
0038c334 mov      r2, #0xc2000000
0038c338 mov      r3, #0x42000000
0038c33c strb     r5, [r1]
0038c340 add      r2, r2, #0xc80000
0038c344 add      r3, r3, #0xc80000
0038c348 mov      r1, #0x370
0038c34c strh     r7, [r4, r1]
0038c350 mov      r0, r4
0038c354 str      r2, [r4, #0x14c]
0038c358 str      r3, [r4, #0x158]
0038c35c str      r2, [r4, #0x144]
0038c360 str      r2, [r4, #0x148]
0038c364 str      r3, [r4, #0x150]
0038c368 str      r3, [r4, #0x154]
0038c36c strb     r5, [r4, #0x373]
0038c370 strb     r5, [r4, #0x372]
0038c374 bl       #0x38aac8 ; _ZN10GameObject18UpdateAbsoluteAABBEv
0038c378 mov      r0, r6
0038c37c mov      r1, r4
0038c380 bl       #0x4a191c ; _ZN14ObjectSearcher10TargetList12SetRefObjectEP10GameObject
0038c384 mov      r0, r4
0038c388 add      sp, sp, #0xc
0038c38c pop      {r4, r5, r6, r7, pc}
0038c390 rsbeq    r8, r0, r8, asr #18
0038c394 andeq    r2, r0, r0, ror sp
