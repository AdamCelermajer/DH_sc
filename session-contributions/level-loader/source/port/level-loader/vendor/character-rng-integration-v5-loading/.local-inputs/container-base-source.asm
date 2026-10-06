_ZN9ContainerC2EN10ObjectBase6GO_IDSE
003a0788 push     {r4, r5, r6, r7, r8, lr}
003a078c ldr      r5, [pc, #0x98]
003a0790 mov      r4, r0
003a0794 bl       #0x38c398 ; _ZN10GameObjectC2EN10ObjectBase6GO_IDSE
003a0798 ldr      r3, [pc, #0x90]
003a079c add      r5, pc, r5
003a07a0 add      r2, r4, #0x378
003a07a4 ldr      r3, [r5, r3]
003a07a8 mov      r0, r2
003a07ac str      r2, [r4, #0x388]
003a07b0 add      ip, r3, #8
003a07b4 add      r1, r3, #0x100
003a07b8 add      r3, r3, #0xf4
003a07bc str      ip, [r4]
003a07c0 str      r2, [r4, #0x38c]
003a07c4 str      r3, [r4, #4]
003a07c8 str      r1, [r4, #0x24]
003a07cc mov      r1, #0x10
003a07d0 bl       #0x31167c ; _ZNSt4priv12_String_baseIcSaIcEE17_M_allocate_blockEj
003a07d4 ldr      r3, [r4, #0x388]
003a07d8 mov      r6, #0
003a07dc mov      r7, #2
003a07e0 add      r8, r4, #0x3a0
003a07e4 add      r5, r4, #0x540
003a07e8 strb     r6, [r3]
003a07ec add      r5, r5, #8
003a07f0 strb     r6, [r4, #0x390]
003a07f4 str      r7, [r4, #0x394]
003a07f8 str      r6, [r4, #0x398]
003a07fc mov      r0, r8
003a0800 bl       #0x39ff58 ; _ZN9Container18NetStructContainerC1Ev
003a0804 mov      r0, r5
003a0808 bl       #0x39ff58 ; _ZN9Container18NetStructContainerC1Ev
003a080c mov      r3, #1
003a0810 strb     r3, [r4, #0x28]
003a0814 strb     r6, [r4, #0x84]
003a0818 str      r8, [r4, #0x100]
003a081c str      r5, [r4, #0x104]
003a0820 strb     r7, [r4, #0xf8]
003a0824 mov      r0, r4
003a0828 pop      {r4, r5, r6, r7, r8, pc}
003a082c ldrsheq  r4, [pc], #-0x24
003a0830 ldrdeq   r2, r3, [r0], -r4
_ZN9Container8InitPostEv
0039f910 push     {r4, r5, r6, r7, r8, lr}
0039f914 sub      sp, sp, #8
0039f918 mov      r4, r0
0039f91c bl       #0x38bd64 ; _ZN10GameObject21CheckSpawnProbabilityEv
0039f920 ldr      r3, [r4, #0x274]
0039f924 ldr      r5, [pc, #0x200]
0039f928 cmp      r0, r3
0039f92c add      r5, pc, r5
0039f930 blt      #0x39f93c
0039f934 add      sp, sp, #8
0039f938 pop      {r4, r5, r6, r7, r8, pc}
0039f93c ldr      r3, [r4]
0039f940 mov      r0, r4
0039f944 mov      lr, pc
0039f948 ldr      pc, [r3, #0xd0]
0039f94c ldr      r3, [r4]
0039f950 str      r0, [r4, #0x374]
0039f954 mov      r0, r4
0039f958 mov      lr, pc
0039f95c ldr      pc, [r3, #0xcc]
0039f960 cmn      r0, #1
0039f964 beq      #0x39f9a4
0039f968 ldr      r3, [r4, #0x374]
0039f96c cmn      r3, #1
0039f970 beq      #0x39f9a4
0039f974 ldr      r3, [pc, #0x1b4]
0039f978 mov      r2, #0xc
0039f97c ldr      r3, [r5, r3]
0039f980 ldr      r3, [r3]
0039f984 mla      r0, r2, r0, r3
0039f988 ldr      r6, [r0, #8]
0039f98c mov      r0, r6
0039f990 bl       #0x30de54
0039f994 mov      r1, r6
0039f998 add      r2, r6, r0
0039f99c add      r0, r4, #0x290
0039f9a0 bl       #0x3109e0 ; _ZNSs9_M_assignEPKcS0_
0039f9a4 mov      r0, r4
0039f9a8 bl       #0x38be5c ; _ZN10GameObject8InitPostEv
0039f9ac mov      r0, r4
0039f9b0 bl       #0x38ab60 ; _ZNK10GameObject13MeetConditionEv
0039f9b4 subs     r1, r0, #0
0039f9b8 beq      #0x39faf8
0039f9bc ldr      r8, [r4, #0x2d8]
0039f9c0 cmp      r8, #0
0039f9c4 beq      #0x39fa38
0039f9c8 ldr      r3, [pc, #0x164]
0039f9cc ldr      r6, [r8, #0x38]
0039f9d0 mov      r2, r4
0039f9d4 ldr      r1, [r5, r3]
0039f9d8 ldr      r3, [pc, #0x158]
0039f9dc ldr      ip, [r6]
0039f9e0 mov      r0, r6
0039f9e4 ldr      r3, [r5, r3]
0039f9e8 str      r4, [sp]
0039f9ec mov      lr, pc
0039f9f0 ldr      pc, [ip, #0x2c]
0039f9f4 ldr      r1, [pc, #0x140]
0039f9f8 mov      r7, #0
0039f9fc ldr      ip, [r6]
0039fa00 add      r1, pc, r1
0039fa04 str      r7, [sp]
0039fa08 mov      r0, r6
0039fa0c mov      r2, r7
0039fa10 mov      r3, r7
0039fa14 mov      lr, pc
0039fa18 ldr      pc, [ip, #0x20]
0039fa1c cmp      r0, #0
0039fa20 beq      #0x39fa94
0039fa24 mov      r1, r7
0039fa28 mov      r0, r4
0039fa2c bl       #0x39f3cc ; _ZN9Container8SetStateENS_5StateE
0039fa30 mov      r0, r8
0039fa34 bl       #0x470a54 ; _ZN12VisualObject12ApplyMeshBoxEv
0039fa38 ldr      r3, [pc, #0x100]
0039fa3c ldr      r3, [r5, r3]
0039fa40 ldr      r5, [r3]
0039fa44 cmp      r5, #0
0039fa48 beq      #0x39fa68
0039fa4c ldr      r3, [r4]
0039fa50 mov      r0, r4
0039fa54 mov      lr, pc
0039fa58 ldr      pc, [r3, #0xd8]
0039fa5c mov      r1, r0
0039fa60 mov      r0, r5
0039fa64 bl       #0x3699fc ; _ZN15VoxSoundManager9LoadSoundEi
0039fa68 ldr      r3, [r4]
0039fa6c mov      r0, r4
0039fa70 mov      lr, pc
0039fa74 ldr      pc, [r3, #0xc8]
0039fa78 ldr      r2, [pc, #0xc4]
0039fa7c mov      r1, r0
0039fa80 mov      r0, r4
0039fa84 add      r2, pc, r2
0039fa88 add      sp, sp, #8
0039fa8c pop      {r4, r5, r6, r7, r8, lr}
0039fa90 b        #0x38ef60
0039fa94 ldr      r1, [pc, #0xac]
0039fa98 mov      r2, r0
0039fa9c ldr      ip, [r6]
0039faa0 mov      r3, r2
0039faa4 str      r0, [sp]
0039faa8 add      r1, pc, r1
0039faac mov      r0, r6
0039fab0 mov      lr, pc
0039fab4 ldr      pc, [ip, #0x20]
0039fab8 subs     r3, r0, #0
0039fabc bne      #0x39fb1c
0039fac0 ldr      r1, [pc, #0x84]
0039fac4 ldr      ip, [r6]
0039fac8 mov      r2, r3
0039facc mov      r0, r6
0039fad0 add      r1, pc, r1
0039fad4 str      r3, [sp]
0039fad8 mov      lr, pc
0039fadc ldr      pc, [ip, #0x20]
0039fae0 cmp      r0, #0
0039fae4 beq      #0x39fa30
0039fae8 mov      r0, r4
0039faec mov      r1, #2
0039faf0 bl       #0x39f3cc ; _ZN9Container8SetStateENS_5StateE
0039faf4 b        #0x39fa30
0039faf8 mov      r0, r4
0039fafc ldr      r3, [r4]
0039fb00 mov      lr, pc
0039fb04 ldr      pc, [r3, #0x40]
0039fb08 mov      r0, r4
0039fb0c mov      r1, #4
0039fb10 add      sp, sp, #8
0039fb14 pop      {r4, r5, r6, r7, r8, lr}
0039fb18 b        #0x39f3cc
0039fb1c mov      r0, r4
0039fb20 mov      r1, #1
0039fb24 bl       #0x39f3cc ; _ZN9Container8SetStateENS_5StateE
0039fb28 b        #0x39fa30
0039fb2c subseq   r5, pc, r4, ror #2
0039fb30 andeq    r1, r0, r8, lsr #25
0039fb34 andeq    r4, r0, r0, lsl #24
0039fb38 andeq    r1, r0, ip, ror #30
0039fb3c subseq   r3, r2, r0, asr r5
0039fb40 andeq    r0, r0, r4, lsr #27
0039fb44 ldrsheq  r3, [r2], #-0xc
0039fb48 subseq   r3, r2, r0, lsl #8
0039fb4c subseq   r2, r2, r0, ror #15
_ZN9Container9InitFinalEv
0039fc98 push     {r4, r5, r6, lr}
0039fc9c mov      r4, r0
0039fca0 bl       #0x38bd64 ; _ZN10GameObject21CheckSpawnProbabilityEv
0039fca4 ldr      r3, [r4, #0x274]
0039fca8 ldr      r5, [pc, #0x80]
0039fcac cmp      r0, r3
0039fcb0 add      r5, pc, r5
0039fcb4 blt      #0x39fcbc
0039fcb8 pop      {r4, r5, r6, pc}
0039fcbc mov      r0, r4
0039fcc0 bl       #0x38cd48 ; _ZN10GameObject9InitFinalEv
0039fcc4 ldr      r3, [r4, #0x394]
0039fcc8 sub      r3, r3, #3
0039fccc cmp      r3, #1
0039fcd0 bls      #0x39fce4
0039fcd4 ldr      r3, [r4]
0039fcd8 mov      r0, r4
0039fcdc mov      lr, pc
0039fce0 ldr      pc, [r3, #0x2c]
0039fce4 mov      r0, r4
0039fce8 bl       #0x38ab60 ; _ZNK10GameObject13MeetConditionEv
0039fcec cmp      r0, #0
0039fcf0 beq      #0x39fcb8
0039fcf4 ldr      r3, [pc, #0x38]
0039fcf8 mov      r1, #0
0039fcfc mov      r0, #0x28
0039fd00 ldr      r3, [r5, r3]
0039fd04 ldr      r6, [r3, #0x44]
0039fd08 bl       #0x310570 ; _Znwj15MemoryHintState
0039fd0c mov      r1, r6
0039fd10 mov      r2, r4
0039fd14 mov      r5, r0
0039fd18 bl       #0x39fc2c ; _ZN7PODecorC1EP13PhysicalWorldP10GameObjectb.clone.2
0039fd1c mov      r0, r4
0039fd20 mov      r1, r5
0039fd24 mov      r2, #0
0039fd28 pop      {r4, r5, r6, lr}
0039fd2c b        #0x394bf8
0039fd30 subseq   r4, pc, r0, ror #27
0039fd34 strdeq   r3, r4, [r0], -r4
_ZN9Container6DoOpenEv
003a0a98 push     {r4, r5, r6, lr}
003a0a9c sub      sp, sp, #0x10
003a0aa0 ldr      r3, [r0]
003a0aa4 mov      r4, r0
003a0aa8 mov      lr, pc
003a0aac ldr      pc, [r3, #0xd4]
003a0ab0 ldr      r3, [r4]
003a0ab4 mov      r5, r0
003a0ab8 mov      r0, r4
003a0abc ldr      r6, [r4, #0x398]
003a0ac0 mov      lr, pc
003a0ac4 ldr      pc, [r3, #0xe0]
003a0ac8 mov      ip, #0
003a0acc mov      r3, r0
003a0ad0 mov      r2, r6
003a0ad4 mov      r0, r5
003a0ad8 mov      r1, r4
003a0adc str      ip, [sp]
003a0ae0 bl       #0x3ecba0 ; _ZN10ItemObject13DropLootTableEiPK10GameObjectS2_ib
003a0ae4 ldr      r3, [r4, #0x300]
003a0ae8 cmp      r3, #0
003a0aec beq      #0x3a0b2c
003a0af0 add      r5, sp, #8
003a0af4 mov      r0, r5
003a0af8 bl       #0x3192b4 ; _ZN3sfc6script3lua9ArgumentsC1Ev
003a0afc ldr      r1, [r4, #0x398]
003a0b00 cmp      r1, #0
003a0b04 beq      #0x3a0b10
003a0b08 mov      r0, r5
003a0b0c bl       #0x386f28 ; _ZN3sfc6script3lua9Arguments12pushUserDataEPNS1_8UserDataE
003a0b10 ldr      r1, [pc, #0x1c]
003a0b14 ldr      r0, [r4, #0x300]
003a0b18 mov      r2, r5
003a0b1c add      r1, pc, r1
003a0b20 bl       #0x37c41c ; _ZN9LuaScript4CallEPKcRKN3sfc6script3lua9ArgumentsE
003a0b24 mov      r0, r5
003a0b28 bl       #0x319228 ; _ZN3sfc6script3lua9ArgumentsD1Ev
003a0b2c add      sp, sp, #0x10
003a0b30 pop      {r4, r5, r6, pc}
003a0b34 subseq   r2, r2, r4, ror #8
_ZN9Container8InteractEP10GameObject
003a0b38 push     {r4, r5, r6, r7, lr}
003a0b3c ldr      r3, [r0, #0x394]
003a0b40 ldr      r5, [pc, #0x110]
003a0b44 sub      sp, sp, #0x24
003a0b48 sub      r3, r3, #3
003a0b4c cmp      r3, #1
003a0b50 mov      r4, r0
003a0b54 add      r5, pc, r5
003a0b58 bls      #0x3a0c34
003a0b5c str      r1, [r0, #0x398]
003a0b60 mov      r1, #4
003a0b64 bl       #0x39f3cc ; _ZN9Container8SetStateENS_5StateE
003a0b68 ldr      r3, [r4]
003a0b6c mov      r0, r4
003a0b70 mov      lr, pc
003a0b74 ldr      pc, [r3, #0xdc]
003a0b78 subs     r1, r0, #0
003a0b7c beq      #0x3a0c3c
003a0b80 ldr      r3, [r4, #0x2d8]
003a0b84 cmp      r3, #0
003a0b88 beq      #0x3a0c4c
003a0b8c mov      r0, r4
003a0b90 mov      r1, #3
003a0b94 bl       #0x39f3cc ; _ZN9Container8SetStateENS_5StateE
003a0b98 ldr      r2, [r4, #0x2d8]
003a0b9c ldr      r1, [pc, #0xb8]
003a0ba0 mov      r3, #0
003a0ba4 ldr      ip, [r2, #0x38]
003a0ba8 add      r1, pc, r1
003a0bac mov      r2, r3
003a0bb0 mov      r0, ip
003a0bb4 ldr      ip, [ip]
003a0bb8 str      r3, [sp]
003a0bbc mov      lr, pc
003a0bc0 ldr      pc, [ip, #0x20]
003a0bc4 ldr      r2, [pc, #0x94]
003a0bc8 ldr      r3, [r4]
003a0bcc mov      r0, r4
003a0bd0 ldr      r2, [r5, r2]
003a0bd4 ldr      r7, [r2]
003a0bd8 mov      lr, pc
003a0bdc ldr      pc, [r3, #0xd8]
003a0be0 ldr      lr, [r4, #0x168]
003a0be4 ldr      r5, [r4, #0x164]
003a0be8 ldr      r6, [r4, #0x160]
003a0bec mov      ip, #0xbf000000
003a0bf0 add      ip, ip, #0x800000
003a0bf4 mov      r1, r0
003a0bf8 mov      r3, #0
003a0bfc str      lr, [sp, #0x1c]
003a0c00 mov      r0, r7
003a0c04 mov      lr, #1
003a0c08 add      r2, sp, #0x14
003a0c0c str      r6, [sp, #0x14]
003a0c10 str      r5, [sp, #0x18]
003a0c14 str      lr, [sp]
003a0c18 str      ip, [sp, #8]
003a0c1c str      ip, [sp, #4]
003a0c20 bl       #0x36b5d8 ; _ZN15VoxSoundManager6Play3DEiRKN6glitch4core8vector3dIfEEbiff
003a0c24 mov      r0, r4
003a0c28 ldr      r3, [r4]
003a0c2c mov      lr, pc
003a0c30 ldr      pc, [r3, #0x2c]
003a0c34 add      sp, sp, #0x24
003a0c38 pop      {r4, r5, r6, r7, pc}
003a0c3c mov      r0, r4
003a0c40 mov      r2, r1
003a0c44 bl       #0x394bf8 ; _ZN10GameObject17SetPhysicalObjectEP14PhysicalObjectb
003a0c48 b        #0x3a0b80
003a0c4c mov      r0, r4
003a0c50 bl       #0x3a0a98 ; _ZN9Container6DoOpenEv
003a0c54 b        #0x3a0bc4
003a0c58 subseq   r3, pc, ip, lsr pc
003a0c5c subseq   r1, r2, r0, lsr pc
003a0c60 andeq    r0, r0, r4, lsr #27
_ZN9Container10__CallbackEPN6glitch5scene19ITimelineControllerEPv
0039f44c push     {r4, lr}
0039f450 ldr      r3, [r1, #0x394]
0039f454 sub      sp, sp, #8
0039f458 mov      r4, r1
0039f45c cmp      r3, #1
0039f460 beq      #0x39f4d4
0039f464 cmp      r3, #3
0039f468 beq      #0x39f498
0039f46c ldr      r3, [r0]
0039f470 mov      lr, pc
0039f474 ldr      pc, [r3, #0x44]
0039f478 cmp      r0, #0
0039f47c ldreq    r3, [r4, #0x2d8]
0039f480 ldreq    r3, [r3, #8]
0039f484 ldreq    r2, [r3, #0x11c]
0039f488 biceq    r2, r2, #0x200
0039f48c streq    r2, [r3, #0x11c]
0039f490 add      sp, sp, #8
0039f494 pop      {r4, pc}
0039f498 mov      r0, r1
0039f49c mov      r1, #4
0039f4a0 bl       #0x39f3cc ; _ZN9Container8SetStateENS_5StateE
0039f4a4 ldr      r2, [r4, #0x2d8]
0039f4a8 ldr      r1, [pc, #0x60]
0039f4ac mov      r3, #0
0039f4b0 ldr      ip, [r2, #0x38]
0039f4b4 add      r1, pc, r1
0039f4b8 mov      r2, r3
0039f4bc mov      r0, ip
0039f4c0 ldr      ip, [ip]
0039f4c4 str      r3, [sp]
0039f4c8 mov      lr, pc
0039f4cc ldr      pc, [ip, #0x20]
0039f4d0 b        #0x39f490
0039f4d4 mov      r0, r1
0039f4d8 mov      r1, #2
0039f4dc bl       #0x39f3cc ; _ZN9Container8SetStateENS_5StateE
0039f4e0 ldr      r2, [r4, #0x2d8]
0039f4e4 ldr      r1, [pc, #0x28]
0039f4e8 mov      r3, #0
0039f4ec ldr      ip, [r2, #0x38]
0039f4f0 add      r1, pc, r1
0039f4f4 mov      r2, r3
0039f4f8 mov      r0, ip
0039f4fc ldr      ip, [ip]
0039f500 str      r3, [sp]
0039f504 mov      lr, pc
0039f508 ldr      pc, [ip, #0x20]
0039f50c b        #0x39f490
0039f510 subseq   r3, r2, r4, lsl r6
0039f514 subseq   r2, r2, r0, asr #27
_ZN9Container15__EventCallbackERKN6glitch7collada15STriggeredEventEPv
003a0c64 push     {r4, r5, r6, lr}
003a0c68 mov      r6, r1
003a0c6c ldr      r1, [pc, #0x6c]
003a0c70 sub      sp, sp, #8
003a0c74 mov      r5, r0
003a0c78 add      r1, pc, r1
003a0c7c ldr      r0, [r0, #4]
003a0c80 bl       #0x30e31c
003a0c84 cmp      r0, #0
003a0c88 beq      #0x3a0cd4
003a0c8c ldr      r3, [r6, #0x300]
003a0c90 cmp      r3, #0
003a0c94 beq      #0x3a0ccc
003a0c98 mov      r0, sp
003a0c9c bl       #0x3192b4 ; _ZN3sfc6script3lua9ArgumentsC1Ev
003a0ca0 mov      r0, sp
003a0ca4 ldr      r1, [r5, #4]
003a0ca8 bl       #0x39ec10 ; _ZN3sfc6script3lua9Arguments10pushStringEPKc
003a0cac ldr      r1, [pc, #0x30]
003a0cb0 ldr      r0, [r6, #0x300]
003a0cb4 mov      r2, sp
003a0cb8 add      r1, pc, r1
003a0cbc bl       #0x37c41c ; _ZN9LuaScript4CallEPKcRKN3sfc6script3lua9ArgumentsE
003a0cc0 mov      r0, sp
003a0cc4 mov      r4, sp
003a0cc8 bl       #0x319228 ; _ZN3sfc6script3lua9ArgumentsD1Ev
003a0ccc add      sp, sp, #8
003a0cd0 pop      {r4, r5, r6, pc}
003a0cd4 mov      r0, r6
003a0cd8 bl       #0x3a0a98 ; _ZN9Container6DoOpenEv
003a0cdc b        #0x3a0ccc
003a0ce0 subseq   r2, r2, r0, lsl r3
003a0ce4 subseq   r2, r2, r8, ror r2
_ZN9Container8SetStateENS_5StateE
0039f3cc ldr      r3, [r0, #0x2d8]
0039f3d0 cmp      r1, #3
0039f3d4 ldr      r3, [r3, #8]
0039f3d8 ldr      r2, [r3, #0x11c]
0039f3dc biceq    r2, r2, #0x400
0039f3e0 orrne    r2, r2, #0x400
0039f3e4 str      r2, [r3, #0x11c]
0039f3e8 str      r1, [r0, #0x394]
0039f3ec bx       lr
_ZN9Container5SpawnEv
0039f3f0 push     {r4, r5, lr}
0039f3f4 ldr      r5, [r0, #0x2d8]
0039f3f8 sub      sp, sp, #0xc
0039f3fc cmp      r5, #0
0039f400 beq      #0x39f440
0039f404 ldr      r4, [r0, #0x394]
0039f408 cmp      r4, #0
0039f40c bne      #0x39f440
0039f410 mov      r1, #1
0039f414 bl       #0x39f3cc ; _ZN9Container8SetStateENS_5StateE
0039f418 ldr      r3, [r5, #0x38]
0039f41c ldr      r1, [pc, #0x24]
0039f420 mov      r2, r4
0039f424 ldr      ip, [r3]
0039f428 mov      r0, r3
0039f42c add      r1, pc, r1
0039f430 str      r4, [sp]
0039f434 mov      r3, r4
0039f438 mov      lr, pc
0039f43c ldr      pc, [ip, #0x20]
0039f440 add      sp, sp, #0xc
0039f444 pop      {r4, r5, pc}
0039f448 subseq   r3, r2, ip, ror sl
_ZNK9Container13IsInteractiveEP10GameObject
0039f384 ldrb     r3, [r0, #0x81]
0039f388 cmp      r3, #0
0039f38c movne    r0, #0
0039f390 bxne     lr
0039f394 ldr      r0, [r0, #0x394]
0039f398 cmp      r0, #2
0039f39c movne    r0, #0
0039f3a0 moveq    r0, #1
0039f3a4 bx       lr
_ZNK17OpenableContainer11KeepPhysicsEv
003a177c push     {r4, lr}
003a1780 ldr      r0, [r0, #0x38c]
003a1784 bl       #0x3a1708 ; _ZN6Arrays19GetMemberIDByStringINS_18OpenableContainersEEEiPKc
003a1788 ldr      r4, [pc, #0x2c]
003a178c cmn      r0, #1
003a1790 add      r4, pc, r4
003a1794 beq      #0x3a17b4
003a1798 ldr      r3, [pc, #0x20]
003a179c mov      r2, #0x28
003a17a0 ldr      r3, [r4, r3]
003a17a4 ldr      r3, [r3]
003a17a8 mla      r0, r2, r0, r3
003a17ac ldrb     r0, [r0, #0xc]
003a17b0 pop      {r4, pc}
003a17b4 mov      r0, #0
003a17b8 pop      {r4, pc}
003a17bc subseq   r3, pc, r0, lsl #6
003a17c0 ldrdeq   r3, r4, [r0], -ip
_ZN7Structs17OpenableContainer4readEP11IStreamBase
004fdb90 push     {r4, r5, r6, lr}
004fdb94 mov      r4, r0
004fdb98 sub      sp, sp, #8
004fdb9c mov      r0, r1
004fdba0 mov      r5, r1
004fdba4 add      r1, r4, #4
004fdba8 bl       #0x459090 ; _ZN12StreamReader6readAsIiEEvP11IStreamBasePT_
004fdbac mov      r3, #1
004fdbb0 cmp      r3, #0
004fdbb4 str      r3, [sp, #4]
004fdbb8 bne      #0x4fdbfc
004fdbbc add      r3, r4, #5
004fdbc0 add      r2, r4, #6
004fdbc4 ldrb     r0, [r2, #1]
004fdbc8 ldrb     r1, [r3, #-1]
004fdbcc cmp      r3, r2
004fdbd0 eor      r1, r0, r1
004fdbd4 strb     r1, [r3, #-1]
004fdbd8 ldrb     r0, [r2, #1]
004fdbdc eor      r1, r1, r0
004fdbe0 strb     r1, [r2, #1]
004fdbe4 ldrb     r0, [r3, #-1]
004fdbe8 sub      r2, r2, #1
004fdbec eor      r1, r1, r0
004fdbf0 strb     r1, [r3, #-1]
004fdbf4 add      r3, r3, #1
004fdbf8 blo      #0x4fdbc4
004fdbfc mov      r0, r5
004fdc00 add      r1, r4, #8
004fdc04 bl       #0x459090 ; _ZN12StreamReader6readAsIiEEvP11IStreamBasePT_
004fdc08 mov      r3, #1
004fdc0c cmp      r3, #0
004fdc10 str      r3, [sp, #4]
004fdc14 bne      #0x4fdc58
004fdc18 add      r3, r4, #9
004fdc1c add      r2, r4, #0xa
004fdc20 ldrb     r0, [r2, #1]
004fdc24 ldrb     r1, [r3, #-1]
004fdc28 cmp      r3, r2
004fdc2c eor      r1, r0, r1
004fdc30 strb     r1, [r3, #-1]
004fdc34 ldrb     r0, [r2, #1]
004fdc38 eor      r1, r1, r0
004fdc3c strb     r1, [r2, #1]
004fdc40 ldrb     r0, [r3, #-1]
004fdc44 sub      r2, r2, #1
004fdc48 eor      r1, r1, r0
004fdc4c strb     r1, [r3, #-1]
004fdc50 add      r3, r3, #1
004fdc54 blo      #0x4fdc20
004fdc58 add      r1, r4, #0xc
004fdc5c mov      r0, r5
004fdc60 bl       #0x4db89c ; _ZN12StreamReader6readAsIbEEvP11IStreamBasePT_
004fdc64 mov      r0, r5
004fdc68 add      r1, r4, #0x10
004fdc6c bl       #0x459090 ; _ZN12StreamReader6readAsIiEEvP11IStreamBasePT_
004fdc70 mov      r3, #1
004fdc74 cmp      r3, #0
004fdc78 str      r3, [sp, #4]
004fdc7c bne      #0x4fdcc0
004fdc80 add      r3, r4, #0x11
004fdc84 add      r2, r4, #0x12
004fdc88 ldrb     r0, [r2, #1]
004fdc8c ldrb     r1, [r3, #-1]
004fdc90 cmp      r3, r2
004fdc94 eor      r1, r0, r1
004fdc98 strb     r1, [r3, #-1]
004fdc9c ldrb     r0, [r2, #1]
004fdca0 eor      r1, r1, r0
004fdca4 strb     r1, [r2, #1]
004fdca8 ldrb     r0, [r3, #-1]
004fdcac sub      r2, r2, #1
004fdcb0 eor      r1, r1, r0
004fdcb4 strb     r1, [r3, #-1]
004fdcb8 add      r3, r3, #1
004fdcbc blo      #0x4fdc88
004fdcc0 mov      r0, r5
004fdcc4 add      r1, r4, #0x14
004fdcc8 bl       #0x3df1a0 ; _ZN12StreamReader6readAsIjEEvP11IStreamBasePT_
004fdccc mov      r3, #1
004fdcd0 cmp      r3, #0
004fdcd4 str      r3, [sp, #4]
004fdcd8 bne      #0x4fdd1c
004fdcdc add      r3, r4, #0x15
004fdce0 add      r2, r4, #0x16
004fdce4 ldrb     r0, [r2, #1]
004fdce8 ldrb     r1, [r3, #-1]
004fdcec cmp      r3, r2
004fdcf0 eor      r1, r0, r1
004fdcf4 strb     r1, [r3, #-1]
004fdcf8 ldrb     r0, [r2, #1]
004fdcfc eor      r1, r1, r0
004fdd00 strb     r1, [r2, #1]
004fdd04 ldrb     r0, [r3, #-1]
004fdd08 sub      r2, r2, #1
004fdd0c eor      r1, r1, r0
004fdd10 strb     r1, [r3, #-1]
004fdd14 add      r3, r3, #1
004fdd18 blo      #0x4fdce4
004fdd1c ldr      r0, [r4, #0x18]
004fdd20 cmp      r0, #0
004fdd24 beq      #0x4fdd2c
004fdd28 bl       #0x310440 ; _Z10CustomFreePv
004fdd2c ldr      r0, [r4, #0x14]
004fdd30 mov      r1, #1
004fdd34 mov      r6, #0
004fdd38 add      r0, r0, r1
004fdd3c bl       #0x31056c ; _Znaj15MemoryHintState
004fdd40 ldr      r2, [r4, #0x14]
004fdd44 mov      r1, r0
004fdd48 str      r0, [r4, #0x18]
004fdd4c mov      r3, r6
004fdd50 mov      r0, r5
004fdd54 bl       #0x317454 ; _ZN12StreamReader12readStringExEP11IStreamBasePcy
004fdd58 ldr      r3, [r4, #0x14]
004fdd5c ldr      r2, [r4, #0x18]
004fdd60 mov      r0, r5
004fdd64 add      r1, r4, #0x1c
004fdd68 strb     r6, [r2, r3]
004fdd6c bl       #0x459090 ; _ZN12StreamReader6readAsIiEEvP11IStreamBasePT_
004fdd70 mov      r3, #1
004fdd74 cmp      r3, r6
004fdd78 str      r3, [sp, #4]
004fdd7c bne      #0x4fddc0
004fdd80 add      r3, r4, #0x1d
004fdd84 add      r2, r4, #0x1e
004fdd88 ldrb     r0, [r2, #1]
004fdd8c ldrb     r1, [r3, #-1]
004fdd90 cmp      r3, r2
004fdd94 eor      r1, r0, r1
004fdd98 strb     r1, [r3, #-1]
004fdd9c ldrb     r0, [r2, #1]
004fdda0 eor      r1, r1, r0
004fdda4 strb     r1, [r2, #1]
004fdda8 ldrb     r0, [r3, #-1]
004fddac sub      r2, r2, #1
004fddb0 eor      r1, r1, r0
004fddb4 strb     r1, [r3, #-1]
004fddb8 add      r3, r3, #1
004fddbc blo      #0x4fdd88
004fddc0 mov      r0, r5
004fddc4 add      r1, r4, #0x20
004fddc8 bl       #0x459090 ; _ZN12StreamReader6readAsIiEEvP11IStreamBasePT_
004fddcc mov      r3, #1
004fddd0 cmp      r3, #0
004fddd4 str      r3, [sp, #4]
004fddd8 bne      #0x4fde1c
004fdddc add      r3, r4, #0x21
004fdde0 add      r2, r4, #0x22
004fdde4 ldrb     r0, [r2, #1]
004fdde8 ldrb     r1, [r3, #-1]
004fddec cmp      r3, r2
004fddf0 eor      r1, r0, r1
004fddf4 strb     r1, [r3, #-1]
004fddf8 ldrb     r0, [r2, #1]
004fddfc eor      r1, r1, r0
004fde00 strb     r1, [r2, #1]
004fde04 ldrb     r0, [r3, #-1]
004fde08 sub      r2, r2, #1
004fde0c eor      r1, r1, r0
004fde10 strb     r1, [r3, #-1]
004fde14 add      r3, r3, #1
004fde18 blo      #0x4fdde4
004fde1c mov      r0, r5
004fde20 add      r1, r4, #0x24
004fde24 bl       #0x459090 ; _ZN12StreamReader6readAsIiEEvP11IStreamBasePT_
004fde28 mov      r3, #1
004fde2c cmp      r3, #0
004fde30 str      r3, [sp, #4]
004fde34 bne      #0x4fde78
004fde38 add      r3, r4, #0x26
004fde3c add      r4, r4, #0x25
004fde40 ldrb     r1, [r3, #1]
004fde44 ldrb     r2, [r4, #-1]
004fde48 cmp      r4, r3
004fde4c eor      r2, r1, r2
004fde50 strb     r2, [r4, #-1]
004fde54 ldrb     r1, [r3, #1]
004fde58 eor      r2, r2, r1
004fde5c strb     r2, [r3, #1]
004fde60 ldrb     r1, [r4, #-1]
004fde64 sub      r3, r3, #1
004fde68 eor      r2, r2, r1
004fde6c strb     r2, [r4, #-1]
004fde70 add      r4, r4, #1
004fde74 blo      #0x4fde40
004fde78 add      sp, sp, #8
004fde7c pop      {r4, r5, r6, pc}
