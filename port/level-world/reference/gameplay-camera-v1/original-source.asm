
_GLOBAL__I_.._.._sources_Game_Cameras_CameraBase.cpp
0040f81c push     {r4, r5, r6, lr}
0040f820 ldr      r4, [pc, #0xac]
0040f824 ldr      r2, [pc, #0xac]
0040f828 ldr      r3, [pc, #0xac]
0040f82c add      r4, pc, r4
0040f830 ldr      r1, [r4, r2]
0040f834 add      r3, pc, r3
0040f838 mov      r2, #0x3f000000
0040f83c ldr      r0, [r1]
0040f840 str      r2, [r3, #8]
0040f844 str      r2, [r3]
0040f848 tst      r0, #1
0040f84c str      r2, [r3, #4]
0040f850 beq      #0x40f8a0
0040f854 ldr      r3, [pc, #0x84]
0040f858 ldr      r3, [r4, r3]
0040f85c ldr      r2, [r3]
0040f860 tst      r2, #1
0040f864 beq      #0x40f86c
0040f868 pop      {r4, r5, r6, pc}
0040f86c mov      r2, #1
0040f870 str      r2, [r3]
0040f874 ldr      r3, [pc, #0x68]
0040f878 ldr      r5, [r4, r3]
0040f87c mov      r0, r5
0040f880 bl       #0x32d79c ; _ZN11ApplicationC1Ev
0040f884 ldr      r3, [pc, #0x5c]
0040f888 mov      r0, r5
0040f88c ldr      r1, [r4, r3]
0040f890 ldr      r3, [pc, #0x54]
0040f894 ldr      r2, [r4, r3]
0040f898 pop      {r4, r5, r6, lr}
0040f89c b        #0x30e304
0040f8a0 mov      r3, #1
0040f8a4 str      r3, [r1]
0040f8a8 ldr      r3, [pc, #0x40]
0040f8ac ldr      r5, [r4, r3]
0040f8b0 mov      r0, r5
0040f8b4 bl       #0x3790a8 ; _ZN17PlayerStatManagerC1Ev
0040f8b8 ldr      r3, [pc, #0x34]
0040f8bc mov      r0, r5
0040f8c0 ldr      r1, [r4, r3]
0040f8c4 ldr      r3, [pc, #0x20]
0040f8c8 ldr      r2, [r4, r3]
0040f8cc bl       #0x30e304
0040f8d0 b        #0x40f854
0040f8d4 subseq   r5, r8, r4, ror #4
0040f8d8 strdeq   r0, r1, [r0], -r4
0040f8dc ldrsbeq  r3, [sb], #-0x98
0040f8e0 andeq    r0, r0, ip, lsr #31
0040f8e4 strdeq   r3, r4, [r0], -r4
0040f8e8 andeq    r0, r0, r0, asr #17
0040f8ec muleq    r0, r0, r8
0040f8f0 andeq    r2, r0, r4, lsl r7
0040f8f4 muleq    r0, ip, r5

_GLOBAL__I_.._.._sources_Game_Cameras_CameraLevel.cpp
004100e8 push     {r4, r5, r6, lr}
004100ec ldr      r4, [pc, #0xf4]
004100f0 ldr      r2, [pc, #0xf4]
004100f4 ldr      r3, [pc, #0xf4]
004100f8 add      r4, pc, r4
004100fc ldr      r1, [r4, r2]
00410100 add      r3, pc, r3
00410104 mov      r2, #0x3f000000
00410108 ldr      r0, [r1]
0041010c str      r2, [r3, #8]
00410110 str      r2, [r3]
00410114 tst      r0, #1
00410118 str      r2, [r3, #4]
0041011c beq      #0x4101b4
00410120 ldr      r3, [pc, #0xcc]
00410124 ldr      r3, [r4, r3]
00410128 ldr      r2, [r3]
0041012c tst      r2, #1
00410130 beq      #0x410180
00410134 ldr      r3, [pc, #0xbc]
00410138 ldr      r3, [r4, r3]
0041013c ldr      r2, [r3]
00410140 tst      r2, #1
00410144 beq      #0x41014c
00410148 pop      {r4, r5, r6, pc}
0041014c mov      r2, #1
00410150 str      r2, [r3]
00410154 ldr      r3, [pc, #0xa0]
00410158 ldr      r5, [r4, r3]
0041015c mov      r0, r5
00410160 bl       #0x4753b4 ; _ZN14AnimSetManagerC1Ev
00410164 ldr      r3, [pc, #0x94]
00410168 mov      r0, r5
0041016c ldr      r1, [r4, r3]
00410170 ldr      r3, [pc, #0x8c]
00410174 ldr      r2, [r4, r3]
00410178 pop      {r4, r5, r6, lr}
0041017c b        #0x30e304
00410180 mov      r2, #1
00410184 str      r2, [r3]
00410188 ldr      r3, [pc, #0x78]
0041018c ldr      r5, [r4, r3]
00410190 mov      r0, r5
00410194 bl       #0x32d79c ; _ZN11ApplicationC1Ev
00410198 ldr      r3, [pc, #0x6c]
0041019c mov      r0, r5
004101a0 ldr      r1, [r4, r3]
004101a4 ldr      r3, [pc, #0x58]
004101a8 ldr      r2, [r4, r3]
004101ac bl       #0x30e304
004101b0 b        #0x410134
004101b4 mov      r3, #1
004101b8 str      r3, [r1]
004101bc ldr      r3, [pc, #0x4c]
004101c0 ldr      r5, [r4, r3]
004101c4 mov      r0, r5
004101c8 bl       #0x3790a8 ; _ZN17PlayerStatManagerC1Ev
004101cc ldr      r3, [pc, #0x40]
004101d0 mov      r0, r5
004101d4 ldr      r1, [r4, r3]
004101d8 ldr      r3, [pc, #0x24]
004101dc ldr      r2, [r4, r3]
004101e0 bl       #0x30e304
004101e4 b        #0x410120

_GLOBAL__I_.._.._sources_Game_Cameras_CameraTarget.cpp
00411d78 push     {r4, r5, r6, lr}
00411d7c ldr      r4, [pc, #0xac]
00411d80 ldr      r2, [pc, #0xac]
00411d84 ldr      r3, [pc, #0xac]
00411d88 add      r4, pc, r4
00411d8c ldr      r1, [r4, r2]
00411d90 add      r3, pc, r3
00411d94 mov      r2, #0x3f000000
00411d98 ldr      r0, [r1]
00411d9c str      r2, [r3, #8]
00411da0 str      r2, [r3]
00411da4 tst      r0, #1
00411da8 str      r2, [r3, #4]
00411dac beq      #0x411dfc
00411db0 ldr      r3, [pc, #0x84]
00411db4 ldr      r3, [r4, r3]
00411db8 ldr      r2, [r3]
00411dbc tst      r2, #1
00411dc0 beq      #0x411dc8
00411dc4 pop      {r4, r5, r6, pc}
00411dc8 mov      r2, #1
00411dcc str      r2, [r3]
00411dd0 ldr      r3, [pc, #0x68]
00411dd4 ldr      r5, [r4, r3]
00411dd8 mov      r0, r5
00411ddc bl       #0x32d79c ; _ZN11ApplicationC1Ev
00411de0 ldr      r3, [pc, #0x5c]
00411de4 mov      r0, r5
00411de8 ldr      r1, [r4, r3]
00411dec ldr      r3, [pc, #0x54]
00411df0 ldr      r2, [r4, r3]
00411df4 pop      {r4, r5, r6, lr}
00411df8 b        #0x30e304
00411dfc mov      r3, #1
00411e00 str      r3, [r1]
00411e04 ldr      r3, [pc, #0x40]
00411e08 ldr      r5, [r4, r3]
00411e0c mov      r0, r5
00411e10 bl       #0x3790a8 ; _ZN17PlayerStatManagerC1Ev
00411e14 ldr      r3, [pc, #0x34]
00411e18 mov      r0, r5
00411e1c ldr      r1, [r4, r3]
00411e20 ldr      r3, [pc, #0x20]
00411e24 ldr      r2, [r4, r3]
00411e28 bl       #0x30e304
00411e2c b        #0x411db0
00411e30 subseq   r2, r8, r8, lsl #26
00411e34 strdeq   r0, r1, [r0], -r4
00411e38 ldrheq   r1, [sb], #-0x44
00411e3c andeq    r0, r0, ip, lsr #31
00411e40 strdeq   r3, r4, [r0], -r4
00411e44 andeq    r0, r0, r0, asr #17
00411e48 muleq    r0, r0, r8
00411e4c andeq    r2, r0, r4, lsl r7
00411e50 muleq    r0, ip, r5

_GLOBAL__I_.._.._sources_Game_ObjectsSub_Visual_Animations_AnimSetController.cpp
004752dc push     {r4, r5, r6, lr}
004752e0 ldr      r4, [pc, #0x64]
004752e4 ldr      r2, [pc, #0x64]
004752e8 ldr      r3, [pc, #0x64]
004752ec add      r4, pc, r4
004752f0 ldr      r1, [r4, r2]
004752f4 add      r3, pc, r3
004752f8 mov      r2, #0x3f000000
004752fc ldr      r0, [r1]
00475300 str      r2, [r3, #8]
00475304 str      r2, [r3]
00475308 tst      r0, #1
0047530c str      r2, [r3, #4]
00475310 beq      #0x475318
00475314 pop      {r4, r5, r6, pc}
00475318 mov      r3, #1
0047531c str      r3, [r1]
00475320 ldr      r3, [pc, #0x30]
00475324 ldr      r5, [r4, r3]
00475328 mov      r0, r5
0047532c bl       #0x4753b4 ; _ZN14AnimSetManagerC1Ev
00475330 ldr      r3, [pc, #0x24]
00475334 mov      r0, r5
00475338 ldr      r1, [r4, r3]
0047533c ldr      r3, [pc, #0x1c]
00475340 ldr      r2, [r4, r3]
00475344 pop      {r4, r5, r6, lr}
00475348 b        #0x30e304
0047534c subseq   pc, r1, r4, lsr #15
00475350 andeq    r1, r0, ip, ror sb
00475354 subseq   r0, r3, r0, lsr lr
00475358 andeq    r4, r0, r8, lsr r8
0047535c andeq    r1, r0, r4, lsl #7
00475360 muleq    r0, r0, r8

_GLOBAL__I_.._.._sources_Game_ObjectsSub_Visual_Animations_AnimSetManager.cpp
0047557c ldr      r3, [pc, #0x14]
00475580 mov      r2, #0x3f000000
00475584 add      r3, pc, r3
00475588 str      r2, [r3, #8]
0047558c str      r2, [r3]
00475590 str      r2, [r3, #4]
00475594 bx       lr
00475598 subseq   r0, r3, ip, lsr #23

_GLOBAL__I_.._.._sources_Game_ObjectsSub_Visual_Animations_BlendedAnimSetController.cpp
00476a14 push     {r4, r5, r6, lr}
00476a18 ldr      r4, [pc, #0x64]
00476a1c ldr      r2, [pc, #0x64]
00476a20 ldr      r3, [pc, #0x64]
00476a24 add      r4, pc, r4
00476a28 ldr      r1, [r4, r2]
00476a2c add      r3, pc, r3
00476a30 mov      r2, #0x3f000000
00476a34 ldr      r0, [r1]
00476a38 str      r2, [r3, #8]
00476a3c str      r2, [r3]
00476a40 tst      r0, #1
00476a44 str      r2, [r3, #4]
00476a48 beq      #0x476a50
00476a4c pop      {r4, r5, r6, pc}
00476a50 mov      r3, #1
00476a54 str      r3, [r1]
00476a58 ldr      r3, [pc, #0x30]
00476a5c ldr      r5, [r4, r3]
00476a60 mov      r0, r5
00476a64 bl       #0x4753b4 ; _ZN14AnimSetManagerC1Ev
00476a68 ldr      r3, [pc, #0x24]
00476a6c mov      r0, r5
00476a70 ldr      r1, [r4, r3]
00476a74 ldr      r3, [pc, #0x1c]
00476a78 ldr      r2, [r4, r3]
00476a7c pop      {r4, r5, r6, lr}
00476a80 b        #0x30e304
00476a84 subseq   lr, r1, ip, rrx
00476a88 andeq    r1, r0, ip, ror sb
00476a8c subseq   pc, r2, r0, lsl r7
00476a90 andeq    r4, r0, r8, lsr r8
00476a94 andeq    r1, r0, r4, lsl #7
00476a98 muleq    r0, r0, r8

_ZN12CameraTarget6UpdateEv
00411a7c push     {r4, r5, lr}
00411a80 ldr      r3, [r0, #4]
00411a84 sub      sp, sp, #0x1c
00411a88 mov      r4, r0
00411a8c cmp      r3, #0
00411a90 beq      #0x411b2c
00411a94 ldr      r3, [r0, #0xc]
00411a98 cmp      r3, #0
00411a9c beq      #0x411b2c
00411aa0 bl       #0x411830 ; _ZN12CameraTarget16HandleTransitionEv
00411aa4 cmp      r0, #0
00411aa8 beq      #0x411ab4
00411aac add      sp, sp, #0x1c
00411ab0 pop      {r4, r5, pc}
00411ab4 mov      r0, r4
00411ab8 bl       #0x411a74 ; _ZN12CameraTarget17GetTargetPositionEv
00411abc ldr      r2, [r0]
00411ac0 mov      r3, r0
00411ac4 add      r5, sp, #0xc
00411ac8 str      r2, [sp, #0xc]
00411acc ldr      r2, [r3, #4]
00411ad0 mov      r1, r5
00411ad4 mov      r0, r4
00411ad8 str      r2, [sp, #0x10]
00411adc ldr      r3, [r3, #8]
00411ae0 str      r3, [sp, #0x14]
00411ae4 bl       #0x4117bc ; _ZN12CameraTarget12HandleOffsetER7Point3DIfE
00411ae8 mov      r1, r5
00411aec mov      r0, r4
00411af0 bl       #0x4115d8 ; _ZN12CameraTarget14HandleGhostCamER7Point3DIfE
00411af4 mov      r1, r5
00411af8 mov      r0, r4
00411afc bl       #0x41165c ; _ZN12CameraTarget13HandleDampingER7Point3DIfE
00411b00 ldr      r0, [r4, #4]
00411b04 ldr      r2, [sp, #0xc]
00411b08 mov      r1, sp
00411b0c ldr      r3, [r0]
00411b10 ldr      r3, [r3, #0xa4]
00411b14 str      r2, [sp]
00411b18 ldr      r2, [sp, #0x10]
00411b1c str      r2, [sp, #4]
00411b20 ldr      r2, [sp, #0x14]
00411b24 str      r2, [sp, #8]
00411b28 blx      r3
00411b2c mov      r0, r4
00411b30 bl       #0x40ea50 ; _ZN10CameraBase6UpdateEv
00411b34 b        #0x411aac

_ZN11CameraLevel8PlayAnimEiib
0040f904 push     {r4, r5, lr}
0040f908 mov      r4, r0
0040f90c ldr      r0, [r0, #0x44]
0040f910 mov      r5, r3
0040f914 sub      sp, sp, #0xc
0040f918 ldr      r3, [r0, #0x38]
0040f91c mov      ip, r2
0040f920 cmp      r3, #0
0040f924 beq      #0x40f978
0040f928 mov      lr, #0
0040f92c str      ip, [r3, #0xc]
0040f930 strb     lr, [r3, #0x10]
0040f934 ldr      ip, [r3]
0040f938 mov      r0, r3
0040f93c mov      r2, lr
0040f940 str      lr, [sp]
0040f944 mov      r3, lr
0040f948 mov      lr, pc
0040f94c ldr      pc, [ip, #0x1c]
0040f950 cmp      r0, #0
0040f954 beq      #0x40f978
0040f958 mov      r3, #1
0040f95c cmp      r5, #0
0040f960 strb     r3, [r4, #0x84]
0040f964 moveq    r3, #0
0040f968 streq    r3, [r4, #0x88]
0040f96c moveq    r3, #0x3f800000
0040f970 strb     r5, [r4, #0xa4]
0040f974 streq    r3, [r4, #0x8c]
0040f978 add      sp, sp, #0xc
0040f97c pop      {r4, r5, pc}

_ZNK17AnimSetController7HasClipEPKcj
00474f58 mov      r0, #0
00474f5c bx       lr

_ZN11CameraLevel15HandleCenteringER7Point3DIfE
0040fa68 push     {r4, r5, r6, r7, lr}
0040fa6c ldr      r7, [pc, #0x288]
0040fa70 ldr      r3, [pc, #0x288]
0040fa74 sub      sp, sp, #0x5c
0040fa78 add      r7, pc, r7
0040fa7c ldr      r3, [r7, r3]
0040fa80 mov      r4, r1
0040fa84 mov      r0, r3
0040fa88 ldr      r6, [r3, #0x40]
0040fa8c bl       #0x31f594 ; _ZNK11Application15GetCurrentLevelEv
0040fa90 mov      r1, #1
0040fa94 mov      r5, r0
0040fa98 mov      r0, r6
0040fa9c bl       #0x36ead0 ; _ZN13PlayerManager18GetNumLocalPlayersEb
0040faa0 cmp      r5, #0
0040faa4 mov      r6, r0
0040faa8 beq      #0x40fc88
0040faac cmp      r6, #2
0040fab0 beq      #0x40fbd0
0040fab4 ble      #0x40fbc8
0040fab8 mov      r7, #0
0040fabc str      r7, [sp, #0x14]
0040fac0 str      r7, [sp, #0x18]
0040fac4 str      r7, [sp, #0x1c]
0040fac8 str      r7, [sp, #8]
0040facc str      r7, [sp, #0xc]
0040fad0 str      r7, [sp, #0x10]
0040fad4 ldrb     r3, [r5, #0x190]
0040fad8 cmp      r3, #0
0040fadc ldrb     r3, [r5, #0x191]
0040fae0 movne    r7, #0xbe000000
0040fae4 addne    r7, r7, #0x800000
0040fae8 cmp      r3, #0
0040faec beq      #0x40fb00
0040faf0 mov      r0, r7
0040faf4 mov      r1, #0x3e800000
0040faf8 bl       #0x30eba4
0040fafc mov      r7, r0
0040fb00 ldrb     r3, [r5, #0x192]
0040fb04 cmp      r3, #0
0040fb08 beq      #0x40fb1c
0040fb0c mov      r0, r7
0040fb10 mov      r1, #0x3e800000
0040fb14 bl       #0x30e3ac
0040fb18 mov      r7, r0
0040fb1c cmp      r6, #4
0040fb20 beq      #0x40fcdc
0040fb24 mov      r5, #0
0040fb28 mov      r0, r7
0040fb2c mov      r1, r5
0040fb30 bl       #0x30df8c
0040fb34 cmp      r0, #0
0040fb38 bne      #0x40fbc8
0040fb3c ldr      r2, [r4, #8]
0040fb40 add      r0, sp, #0x40
0040fb44 add      r1, sp, #0x14
0040fb48 str      r5, [sp, #0x40]
0040fb4c str      r5, [sp, #0x44]
0040fb50 bl       #0x40f394 ; _ZN10CameraBase13GetWorldCoordERK7Point2DIfER7Point3DIfEf
0040fb54 ldr      r2, [r4, #8]
0040fb58 add      r0, sp, #0x38
0040fb5c add      r1, sp, #8
0040fb60 str      r5, [sp, #0x3c]
0040fb64 str      r7, [sp, #0x38]
0040fb68 bl       #0x40f394 ; _ZN10CameraBase13GetWorldCoordERK7Point2DIfER7Point3DIfEf
0040fb6c ldr      r1, [sp, #0x18]
0040fb70 ldr      r0, [sp, #0xc]
0040fb74 bl       #0x30e3ac
0040fb78 ldr      r1, [sp, #0x1c]
0040fb7c mov      r6, r0
0040fb80 ldr      r0, [sp, #0x10]
0040fb84 bl       #0x30e3ac
0040fb88 ldr      r1, [sp, #0x14]
0040fb8c mov      r5, r0
0040fb90 ldr      r0, [sp, #8]
0040fb94 bl       #0x30e3ac
0040fb98 mov      r1, r0
0040fb9c ldr      r0, [r4]
0040fba0 bl       #0x30eba4
0040fba4 mov      r1, r6
0040fba8 str      r0, [r4]
0040fbac ldr      r0, [r4, #4]
0040fbb0 bl       #0x30eba4
0040fbb4 mov      r1, r5
0040fbb8 str      r0, [r4, #4]
0040fbbc ldr      r0, [r4, #8]
0040fbc0 bl       #0x30eba4
0040fbc4 str      r0, [r4, #8]
0040fbc8 add      sp, sp, #0x5c
0040fbcc pop      {r4, r5, r6, r7, pc}
0040fbd0 mov      r6, #0
0040fbd4 str      r6, [sp, #0x2c]
0040fbd8 str      r6, [sp, #0x30]
0040fbdc str      r6, [sp, #0x34]
0040fbe0 str      r6, [sp, #0x20]
0040fbe4 str      r6, [sp, #0x24]
0040fbe8 str      r6, [sp, #0x28]
0040fbec ldrb     r3, [r5, #0x190]
0040fbf0 cmp      r3, #0
0040fbf4 ldrb     r3, [r5, #0x191]
0040fbf8 movne    r6, #0xbf000000
0040fbfc cmp      r3, #0
0040fc00 beq      #0x40fc14
0040fc04 mov      r0, r6
0040fc08 mov      r1, #0x3f000000
0040fc0c bl       #0x30eba4
0040fc10 mov      r6, r0
0040fc14 mov      r5, #0
0040fc18 mov      r0, r6
0040fc1c mov      r1, r5
0040fc20 bl       #0x30df8c
0040fc24 cmp      r0, #0
0040fc28 bne      #0x40fbc8
0040fc2c ldr      r2, [r4, #8]
0040fc30 add      r0, sp, #0x50
0040fc34 add      r1, sp, #0x2c
0040fc38 str      r5, [sp, #0x50]
0040fc3c str      r5, [sp, #0x54]
0040fc40 bl       #0x40f394 ; _ZN10CameraBase13GetWorldCoordERK7Point2DIfER7Point3DIfEf
0040fc44 ldr      r2, [r4, #8]
0040fc48 add      r0, sp, #0x48
0040fc4c add      r1, sp, #0x20
0040fc50 str      r6, [sp, #0x48]
0040fc54 str      r5, [sp, #0x4c]
0040fc58 bl       #0x40f394 ; _ZN10CameraBase13GetWorldCoordERK7Point2DIfER7Point3DIfEf
0040fc5c ldr      r1, [sp, #0x30]
0040fc60 ldr      r0, [sp, #0x24]
0040fc64 bl       #0x30e3ac
0040fc68 ldr      r1, [sp, #0x34]
0040fc6c mov      r6, r0
0040fc70 ldr      r0, [sp, #0x28]
0040fc74 bl       #0x30e3ac
0040fc78 ldr      r1, [sp, #0x2c]
0040fc7c mov      r5, r0
0040fc80 ldr      r0, [sp, #0x20]
0040fc84 b        #0x40fb94
0040fc88 ldr      r3, [pc, #0x74]
0040fc8c ldr      r3, [r7, r3]
0040fc90 ldr      r3, [r3]
0040fc94 cmp      r3, #2
0040fc98 streq    r5, [r5]
0040fc9c beq      #0x40faac
0040fca0 cmp      r3, #1
0040fca4 bne      #0x40faac
0040fca8 ldr      r0, [pc, #0x58]
0040fcac ldr      r1, [pc, #0x58]
0040fcb0 ldr      r2, [pc, #0x58]
0040fcb4 ldr      r0, [r7, r0]
0040fcb8 ldr      r3, [pc, #0x54]
0040fcbc mov      ip, #0x66
0040fcc0 add      r1, pc, r1
0040fcc4 add      r2, pc, r2
0040fcc8 add      r3, pc, r3
0040fccc add      r0, r0, #0xa8
0040fcd0 str      ip, [sp]
0040fcd4 bl       #0x30e004
0040fcd8 b        #0x40faac
0040fcdc ldrb     r3, [r5, #0x193]
0040fce0 cmp      r3, #0
0040fce4 beq      #0x40fb24
0040fce8 mov      r0, r7
0040fcec mov      r1, #0x3e800000
0040fcf0 bl       #0x30eba4
0040fcf4 mov      r7, r0
0040fcf8 b        #0x40fb24
0040fcfc subseq   r5, r8, r8, lsl r0
0040fd00 strdeq   r3, r4, [r0], -r4
0040fd04 andeq    r3, r0, r0, asr #19
0040fd08 andeq    r1, r0, r0, asr #19
0040fd0c subeq    lr, sl, r8, lsl r7
0040fd10 umaaleq  pc, pc, r4, ip
0040fd14 subeq    r8, fp, r8, ror #3

_Z27GetNewScriptCmdImplInstanceI22Script_SetCameraTargetEP13ScriptCmdImplv
00458b00 push     {r4, lr}
00458b04 mov      r1, #0
00458b08 mov      r0, #0x10
00458b0c bl       #0x310570 ; _Znwj15MemoryHintState
00458b10 ldr      r4, [pc, #0x28]
00458b14 ldr      r2, [pc, #0x28]
00458b18 mov      r1, #0
00458b1c add      r4, pc, r4
00458b20 ldr      r2, [r4, r2]
00458b24 str      r1, [r0, #0xc]
00458b28 strb     r1, [r0, #4]
00458b2c add      r2, r2, #8
00458b30 str      r2, [r0]
00458b34 mvn      r2, #0
00458b38 str      r2, [r0, #8]
00458b3c pop      {r4, pc}
00458b40 subseq   fp, r3, r4, ror pc
00458b44 andeq    r1, r0, ip, lsl #21

_ZN11CameraLevelC1Ev
0040ff70 push     {r4, r5, r6, lr}
0040ff74 ldr      r6, [pc, #0xa8]
0040ff78 mov      r4, r0
0040ff7c bl       #0x411cdc ; _ZN12CameraTargetC2Ev
0040ff80 ldr      r2, [pc, #0xa0]
0040ff84 add      r6, pc, r6
0040ff88 mov      r5, #0
0040ff8c ldr      r2, [r6, r2]
0040ff90 add      r3, r4, #0x4c
0040ff94 mov      r0, r3
0040ff98 add      r2, r2, #8
0040ff9c str      r2, [r4]
0040ffa0 str      r3, [r4, #0x5c]
0040ffa4 str      r3, [r4, #0x60]
0040ffa8 str      r5, [r4, #0x44]
0040ffac str      r5, [r4, #0x48]
0040ffb0 mov      r1, #0x10
0040ffb4 bl       #0x31167c ; _ZNSt4priv12_String_baseIcSaIcEE17_M_allocate_blockEj
0040ffb8 ldr      r2, [r4, #0x5c]
0040ffbc add      r3, r4, #0x64
0040ffc0 mov      r0, r3
0040ffc4 strb     r5, [r2]
0040ffc8 mov      r1, #0x10
0040ffcc str      r3, [r4, #0x74]
0040ffd0 str      r3, [r4, #0x78]
0040ffd4 bl       #0x31167c ; _ZNSt4priv12_String_baseIcSaIcEE17_M_allocate_blockEj
0040ffd8 ldr      r2, [r4, #0x74]
0040ffdc mov      r3, #0
0040ffe0 mov      r0, r4
0040ffe4 strb     r5, [r2]
0040ffe8 mvn      r2, #0
0040ffec str      r2, [r4, #0x7c]
0040fff0 mov      r2, #0x3f800000
0040fff4 str      r2, [r4, #0x8c]
0040fff8 str      r3, [r4, #0xa0]
0040fffc strb     r5, [r4, #0xa4]
00410000 strb     r5, [r4, #0x84]
00410004 strb     r5, [r4, #0x85]
00410008 strb     r5, [r4, #0x86]
0041000c str      r3, [r4, #0x88]
00410010 str      r3, [r4, #0x90]
00410014 str      r3, [r4, #0x94]
00410018 str      r3, [r4, #0x98]
0041001c str      r3, [r4, #0x9c]
00410020 pop      {r4, r5, r6, pc}
00410024 subseq   r4, r8, ip, lsl #22
00410028 andeq    r4, r0, r8, lsr #4

_ZNK22Script_SetCameraTarget10IsBlockingEv
004592cc push     {r4, lr}
004592d0 ldr      r2, [r0, #0xc]
004592d4 ldr      r3, [pc, #0x48]
004592d8 ldrb     r2, [r2, #0x14]
004592dc add      r3, pc, r3
004592e0 cmp      r2, #0
004592e4 bne      #0x4592f0
004592e8 mov      r0, #0
004592ec pop      {r4, pc}
004592f0 ldr      r2, [pc, #0x30]
004592f4 ldr      r0, [r3, r2]
004592f8 bl       #0x31f594 ; _ZNK11Application15GetCurrentLevelEv
004592fc cmp      r0, #0
00459300 beq      #0x4592e8
00459304 ldr      r3, [r0, #0x128]
00459308 cmp      r3, #0
0045930c beq      #0x4592e8
00459310 ldr      r0, [r3, #0x20]
00459314 cmp      r0, #0
00459318 movle    r0, #0
0045931c movgt    r0, #1
00459320 pop      {r4, pc}
00459324 ldrheq   fp, [r3], #-0x74
00459328 strdeq   r3, r4, [r0], -r4

_ZN11CameraLevel30CalculateDefaultTargetDistanceEv
0040fd18 push     {r4, r5, r6, r7, r8, sl, lr}
0040fd1c ldr      r4, [r0, #8]
0040fd20 ldr      r3, [pc, #0x13c]
0040fd24 sub      sp, sp, #0xc
0040fd28 cmp      r4, #0
0040fd2c mov      r5, r0
0040fd30 add      r3, pc, r3
0040fd34 beq      #0x40fe10
0040fd38 mov      r0, r4
0040fd3c bl       #0x597290 ; _ZNK6glitch5scene10ISceneNode9getParentEv
0040fd40 ldr      r3, [r0]
0040fd44 mov      lr, pc
0040fd48 ldr      pc, [r3, #0xa0]
0040fd4c ldr      r3, [r4]
0040fd50 mov      r2, r0
0040fd54 mov      r0, r4
0040fd58 ldr      r6, [r2, #8]
0040fd5c ldr      r4, [r2]
0040fd60 ldr      r7, [r2, #4]
0040fd64 mov      lr, pc
0040fd68 ldr      pc, [r3, #0x10c]
0040fd6c ldr      r3, [r0]
0040fd70 mov      lr, pc
0040fd74 ldr      pc, [r3, #0xa0]
0040fd78 mov      r1, r4
0040fd7c mov      r3, r0
0040fd80 ldr      r0, [r0]
0040fd84 ldr      sl, [r3, #4]
0040fd88 ldr      r8, [r3, #8]
0040fd8c bl       #0x30e3ac
0040fd90 mov      r1, r7
0040fd94 mov      r4, r0
0040fd98 mov      r0, sl
0040fd9c bl       #0x30e3ac
0040fda0 mov      r1, r6
0040fda4 mov      r7, r0
0040fda8 mov      r0, r8
0040fdac bl       #0x30e3ac
0040fdb0 mov      r1, r4
0040fdb4 mov      r6, r0
0040fdb8 mov      r0, r4
0040fdbc bl       #0x30ed6c
0040fdc0 mov      r1, r7
0040fdc4 mov      r4, r0
0040fdc8 mov      r0, r7
0040fdcc bl       #0x30ed6c
0040fdd0 mov      r1, r0
0040fdd4 mov      r0, r4
0040fdd8 bl       #0x30eba4
0040fddc mov      r1, r6
0040fde0 mov      r4, r0
0040fde4 mov      r0, r6
0040fde8 bl       #0x30ed6c
0040fdec mov      r1, r0
0040fdf0 mov      r0, r4
0040fdf4 bl       #0x30eba4
0040fdf8 bl       #0x30e8a4
0040fdfc bl       #0x30e1c0
0040fe00 bl       #0x30e6a0
0040fe04 str      r0, [r5, #0x94]
0040fe08 add      sp, sp, #0xc
0040fe0c pop      {r4, r5, r6, r7, r8, sl, pc}
0040fe10 ldr      r2, [pc, #0x50]
0040fe14 ldr      r2, [r3, r2]
0040fe18 ldr      r2, [r2]
0040fe1c cmp      r2, #2
0040fe20 streq    r4, [r4]
0040fe24 beq      #0x40fd38
0040fe28 cmp      r2, #1
0040fe2c bne      #0x40fd38
0040fe30 ldr      r0, [pc, #0x34]
0040fe34 ldr      r1, [pc, #0x34]
0040fe38 ldr      r2, [pc, #0x34]
0040fe3c ldr      r0, [r3, r0]
0040fe40 ldr      r3, [pc, #0x30]
0040fe44 mov      ip, #0x54
0040fe48 add      r1, pc, r1
0040fe4c add      r2, pc, r2
0040fe50 add      r3, pc, r3
0040fe54 add      r0, r0, #0xa8
0040fe58 str      ip, [sp]
0040fe5c bl       #0x30e004
0040fe60 b        #0x40fd38
0040fe64 subseq   r4, r8, r0, ror #26
0040fe68 andeq    r3, r0, r0, asr #19
0040fe6c andeq    r1, r0, r0, asr #19
0040fe70 umaaleq  lr, sl, r0, r5
0040fe74 subeq    r8, fp, ip, lsr #1
0040fe78 subeq    r8, fp, r0, rrx

_ZN7Structs15SetCameraTargetD1Ev
004d373c push     {r4, lr}
004d3740 ldr      r3, [pc, #0x34]
004d3744 ldr      r2, [pc, #0x34]
004d3748 mov      r4, r0
004d374c add      r3, pc, r3
004d3750 ldr      r0, [r0, #0x10]
004d3754 ldr      r2, [r3, r2]
004d3758 cmp      r0, #0
004d375c add      r2, r2, #8
004d3760 str      r2, [r4]
004d3764 beq      #0x4d376c
004d3768 bl       #0x310440 ; _Z10CustomFreePv
004d376c mov      r0, r4
004d3770 bl       #0x4c6c60 ; _ZN7Structs9ScriptCmdD2Ev
004d3774 mov      r0, r4
004d3778 pop      {r4, pc}
004d377c subeq    r1, ip, r4, asr #6
004d3780 andeq    r3, r0, r0, lsr r0

_ZN12CameraTarget16HandleTransitionEv
00411830 push     {r4, r5, r6, r7, r8, sl, lr}
00411834 ldr      r2, [r0, #0x20]
00411838 ldr      r3, [pc, #0x17c]
0041183c sub      sp, sp, #0x1c
00411840 cmp      r2, #0
00411844 mov      r4, r0
00411848 add      r3, pc, r3
0041184c blt      #0x4119b4
00411850 ldr      r2, [r0, #4]
00411854 cmp      r2, #0
00411858 beq      #0x4119b4
0041185c ldr      r2, [r0, #0xc]
00411860 cmp      r2, #0
00411864 beq      #0x4119b4
00411868 ldr      r2, [pc, #0x150]
0041186c ldr      r0, [r3, r2]
00411870 bl       #0x31f66c ; _ZN11Application5GetDtEv
00411874 ldr      r3, [r4, #0x20]
00411878 rsb      r3, r0, r3
0041187c cmp      r3, #0
00411880 str      r3, [r4, #0x20]
00411884 ble      #0x411974
00411888 ldr      r0, [r4, #0xc]
0041188c bl       #0x3943b8 ; _ZNK10GameObject23GetCameraAnchorPositionEv
00411890 mov      r6, r0
00411894 ldr      r0, [r4, #0x20]
00411898 bl       #0x30e964
0041189c mov      r5, r0
004118a0 ldr      r0, [r4, #0x1c]
004118a4 bl       #0x30e964
004118a8 mov      r1, r0
004118ac mov      r0, r5
004118b0 bl       #0x30ec94
004118b4 mov      r1, r0
004118b8 mov      r0, #0x3f800000
004118bc bl       #0x30e3ac
004118c0 ldr      r8, [r4, #0x14]
004118c4 mov      r5, r0
004118c8 ldr      r0, [r6, #4]
004118cc mov      r1, r8
004118d0 bl       #0x30e3ac
004118d4 mov      r1, r0
004118d8 mov      r0, r5
004118dc bl       #0x30ed6c
004118e0 mov      r1, r0
004118e4 mov      r0, r8
004118e8 bl       #0x30eba4
004118ec ldr      r7, [r4, #0x18]
004118f0 mov      r8, r0
004118f4 ldr      r0, [r6, #8]
004118f8 mov      r1, r7
004118fc bl       #0x30e3ac
00411900 mov      r1, r0
00411904 mov      r0, r5
00411908 bl       #0x30ed6c
0041190c mov      r1, r0
00411910 mov      r0, r7
00411914 bl       #0x30eba4
00411918 ldr      r7, [r4, #4]
0041191c ldr      r4, [r4, #0x10]
00411920 mov      sl, r0
00411924 ldr      r3, [r7]
00411928 ldr      r0, [r6]
0041192c mov      r1, r4
00411930 ldr      r6, [r3, #0xa4]
00411934 bl       #0x30e3ac
00411938 mov      r1, r0
0041193c mov      r0, r5
00411940 bl       #0x30ed6c
00411944 mov      r1, r0
00411948 mov      r0, r4
0041194c bl       #0x30eba4
00411950 str      r8, [sp, #0x10]
00411954 str      r0, [sp, #0xc]
00411958 str      sl, [sp, #0x14]
0041195c mov      r0, r7
00411960 add      r1, sp, #0xc
00411964 blx      r6
00411968 mov      r0, #1
0041196c add      sp, sp, #0x1c
00411970 pop      {r4, r5, r6, r7, r8, sl, pc}
00411974 ldr      r5, [r4, #4]
00411978 ldr      r0, [r4, #0xc]
0041197c ldr      r3, [r5]
00411980 ldr      r4, [r3, #0xa4]
00411984 bl       #0x3943b8 ; _ZNK10GameObject23GetCameraAnchorPositionEv
00411988 ldr      r1, [r0]
0041198c ldr      r2, [r0, #4]
00411990 ldr      r3, [r0, #8]
00411994 str      r1, [sp]
00411998 mov      r0, r5
0041199c str      r2, [sp, #4]
004119a0 str      r3, [sp, #8]
004119a4 mov      r1, sp
004119a8 blx      r4
004119ac mov      r0, #1
004119b0 b        #0x41196c
004119b4 mov      r0, #0
004119b8 b        #0x41196c
004119bc subseq   r3, r8, r8, asr #4
004119c0 strdeq   r3, r4, [r0], -r4

_ZN14AnimSetManager23GetSynchronizedAnimatorEi
004761cc push     {r4, r5, r6, r7, lr}
004761d0 sub      sp, sp, #0x1c
004761d4 str      r1, [sp, #4]
004761d8 mov      r5, r0
004761dc bl       #0x475404 ; _ZNK14AnimSetManager6ExistsEi
004761e0 subs     r4, r0, #0
004761e4 bne      #0x4761f4
004761e8 mov      r0, r4
004761ec add      sp, sp, #0x1c
004761f0 pop      {r4, r5, r6, r7, pc}
004761f4 add      r0, r5, #4
004761f8 add      r1, sp, #4
004761fc bl       #0x476058 ; _ZNSt3mapIi12AnimationSetSt4lessIiESaISt4pairIKiS0_EEEixIiEERS0_RKT_
00476200 ldr      r3, [r0, #0x20]
00476204 mov      r5, r0
00476208 ldrb     r2, [r3, #0x70]
0047620c cmp      r2, #0
00476210 bne      #0x4762b0
00476214 mov      r3, #0
00476218 str      r3, [sp, #0x10]
0047621c str      r3, [sp, #8]
00476220 str      r3, [sp, #0xc]
00476224 ldr      r3, [r5, #0x20]
00476228 mov      r1, #0
0047622c mov      r0, #0xc8
00476230 cmp      r3, #0
00476234 str      r3, [sp, #0x14]
00476238 ldrne    r2, [r3, #4]
0047623c add      r6, sp, #8
00476240 addne    r2, r2, #1
00476244 strne    r2, [r3, #4]
00476248 bl       #0x310570 ; _Znwj15MemoryHintState
0047624c add      r1, sp, #0x14
00476250 mov      r2, r6
00476254 mov      r4, r0
00476258 bl       #0x36904c ; _ZN23AnimatorSynchronizedSetC1ERKN5boost13intrusive_ptrIN6glitch7collada13CAnimationSetEEERKSt6vectorISsNS2_4core10SAllocatorISsLNS2_6memory13E_MEMORY_HINTE0EEEE
0047625c ldr      r0, [sp, #0x14]
00476260 cmp      r0, #0
00476264 beq      #0x47626c
00476268 bl       #0x31d584 ; _ZNK6glitch17IReferenceCounted4dropEv
0047626c ldr      r3, [r4]
00476270 mov      r0, r4
00476274 mov      lr, pc
00476278 ldr      pc, [r3, #0x44]
0047627c mov      r7, r0
00476280 mov      r0, r5
00476284 bl       #0x3649bc ; _ZN12AnimationSet18CalculateCacheSizeEv
00476288 cmp      r7, #0
0047628c beq      #0x4762a4
00476290 mov      r0, r7
00476294 ldr      r3, [r7]
00476298 mov      r1, #0
0047629c mov      lr, pc
004762a0 ldr      pc, [r3, #0x40]
004762a4 mov      r0, r6
004762a8 bl       #0x47578c ; _ZNSt6vectorISsN6glitch4core10SAllocatorISsLNS0_6memory13E_MEMORY_HINTE0EEEED1Ev
004762ac b        #0x4761e8
004762b0 mov      r0, r3
004762b4 ldr      r3, [r3]
004762b8 mov      lr, pc
004762bc ldr      pc, [r3, #0x38]
004762c0 b        #0x476214

_ZN12CameraTargetD0Ev
00411bf0 push     {r4, lr}
00411bf4 mov      r4, r0
00411bf8 bl       #0x411bbc ; _ZN12CameraTargetD1Ev
00411bfc mov      r0, r4
00411c00 bl       #0x310440 ; _Z10CustomFreePv
00411c04 mov      r0, r4
00411c08 pop      {r4, pc}

_ZN14AnimSetManagerC1Ev
004753b4 ldr      r1, [pc, #0x40]
004753b8 str      r4, [sp, #-4]!
004753bc ldr      r4, [pc, #0x3c]
004753c0 add      r1, pc, r1
004753c4 mov      ip, #0
004753c8 ldr      r4, [r1, r4]
004753cc mov      r2, r0
004753d0 str      ip, [r0, #8]
004753d4 add      r4, r4, #8
004753d8 str      r4, [r0]
004753dc strb     ip, [r2, #4]!
004753e0 mvn      r4, #0
004753e4 str      r4, [r0, #0x1c]
004753e8 str      r2, [r0, #0x10]
004753ec str      ip, [r0, #0x14]
004753f0 str      r2, [r0, #0xc]
004753f4 ldm      sp!, {r4}
004753f8 bx       lr
004753fc ldrsbeq  pc, [r1], #-0x60
00475400 andeq    r4, r0, ip, ror #6

_ZN24BlendedAnimSetController8StopClipEbj
00476780 push     {r4, r5, r6, lr}
00476784 mov      r4, r1
00476788 mov      r1, r2
0047678c bl       #0x4748b8 ; _ZN14AnimController7GetAnimEj
00476790 ldr      r2, [r0, #0x70]
00476794 ldr      r3, [r0, #0x28]
00476798 mov      r6, r0
0047679c ldr      r3, [r3, r2, lsl #2]
004767a0 cmp      r3, #0
004767a4 beq      #0x476800
004767a8 mov      r0, r3
004767ac ldr      r3, [r3]
004767b0 mov      lr, pc
004767b4 ldr      pc, [r3, #0x44]
004767b8 mov      r1, #0
004767bc mov      r5, r0
004767c0 mov      r0, r6
004767c4 bl       #0x3666d8 ; _ZN15AnimatorBlender8SetScaleEf
004767c8 cmp      r5, #0
004767cc beq      #0x4767d8
004767d0 cmp      r4, #0
004767d4 bne      #0x4767dc
004767d8 pop      {r4, r5, r6, pc}
004767dc ldr      r3, [r5]
004767e0 mov      r0, r5
004767e4 ldr      r4, [r3, #0xc]
004767e8 mov      lr, pc
004767ec ldr      pc, [r3, #0x30]
004767f0 mov      r1, r0
004767f4 mov      r0, r5
004767f8 blx      r4
004767fc pop      {r4, r5, r6, pc}
00476800 mov      r1, #0
00476804 pop      {r4, r5, r6, lr}
00476808 b        #0x3666d8 ; _ZN15AnimatorBlender8SetScaleEf

_ZN14AnimSetManagerD2Ev
00475728 ldr      r3, [pc, #0x54]
0047572c ldr      r2, [pc, #0x54]
00475730 push     {r4, r5, r6, lr}
00475734 add      r3, pc, r3
00475738 ldr      r2, [r3, r2]
0047573c mov      r4, r0
00475740 add      r2, r2, #8
00475744 str      r2, [r0]
00475748 bl       #0x475664 ; _ZN14AnimSetManager5FlushEv
0047574c ldr      r3, [r4, #0x14]
00475750 cmp      r3, #0
00475754 beq      #0x47577c
00475758 add      r5, r4, #4
0047575c mov      r0, r5
00475760 ldr      r1, [r4, #8]
00475764 bl       #0x475624 ; _ZNSt4priv8_Rb_treeIiSt4lessIiESt4pairIKi12AnimationSetENS_10_Select1stIS6_EENS_11_MapTraitsTIS6_EESaIS6_EE8_M_eraseEPNS_18_Rb_tree_node_baseE
00475768 mov      r3, #0
0047576c str      r5, [r4, #0x10]
00475770 str      r3, [r4, #0x14]
00475774 str      r5, [r4, #0xc]
00475778 str      r3, [r4, #8]
0047577c mov      r0, r4
00475780 pop      {r4, r5, r6, pc}
00475784 subseq   pc, r1, ip, asr r3
00475788 andeq    r4, r0, ip, ror #6

_ZN14AnimSetManager15AddTemplateAnimEii
00476398 push     {r4, r5, r6, r7, r8, lr}
0047639c sub      sp, sp, #0x10
004763a0 mov      r5, r0
004763a4 str      r1, [sp, #4]
004763a8 mov      r7, r2
004763ac bl       #0x475404 ; _ZNK14AnimSetManager6ExistsEi
004763b0 ldr      r6, [pc, #0xa0]
004763b4 cmp      r0, #0
004763b8 addne    r5, r5, #4
004763bc add      r6, pc, r6
004763c0 addne    r4, sp, #4
004763c4 bne      #0x476408
004763c8 ldr      r3, [sp, #4]
004763cc cmp      r3, #0
004763d0 blt      #0x476450
004763d4 add      r5, r5, #4
004763d8 add      r4, sp, #4
004763dc mov      r1, r4
004763e0 mov      r0, r5
004763e4 bl       #0x476058 ; _ZNSt3mapIi12AnimationSetSt4lessIiESaISt4pairIKiS0_EEEixIiEERS0_RKT_
004763e8 mov      r8, r0
004763ec bl       #0x364ca0 ; _ZN12AnimationSet13CreateAnimSetEv
004763f0 ldr      r3, [pc, #0x64]
004763f4 ldr      r3, [r6, r3]
004763f8 ldrb     r3, [r3]
004763fc cmp      r3, #0
00476400 movne    r3, #1
00476404 strbne   r3, [r8, #0x3c]
00476408 mov      r1, r4
0047640c mov      r0, r5
00476410 bl       #0x476058 ; _ZNSt3mapIi12AnimationSetSt4lessIiESaISt4pairIKiS0_EEEixIiEERS0_RKT_
00476414 mov      r1, r7
00476418 mov      r5, r0
0047641c bl       #0x3659ec ; _ZN12AnimationSet13LoadAnimationEi
00476420 ldr      r3, [pc, #0x38]
00476424 ldr      r5, [r5, #0x20]
00476428 add      r4, sp, #8
0047642c ldr      r1, [r0, #0x14]
00476430 ldr      r2, [r6, r3]
00476434 mov      r0, r4
00476438 bl       #0x60f25c ; _ZN6glitch7collada16CColladaDatabaseC1EPKcPNS0_15CColladaFactoryE
0047643c mov      r0, r5
00476440 mov      r1, r4
00476444 bl       #0x62fc90 ; _ZN6glitch7collada20CDynamicAnimationSet26setDefaultAnimationLibraryERKNS0_16CColladaDatabaseE
00476448 mov      r0, r4
0047644c bl       #0x619474 ; _ZN6glitch7collada16CColladaDatabaseD1Ev
00476450 add      sp, sp, #0x10
00476454 pop      {r4, r5, r6, r7, r8, pc}
00476458 ldrsbeq  lr, [r1], #-0x64
0047645c andeq    r4, r0, r8, lsr #9
00476460 andeq    r4, r0, r0, lsl r7

_ZN10CameraBase9SetActiveEv
0040f45c push     {r4, r5, r6, lr}
0040f460 ldr      r5, [pc, #0x68]
0040f464 ldr      r4, [pc, #0x68]
0040f468 mov      r6, r0
0040f46c add      r5, pc, r5
0040f470 ldr      r3, [r5, r4]
0040f474 ldr      r3, [r3]
0040f478 cmp      r3, r0
0040f47c beq      #0x40f4cc
0040f480 cmp      r3, #0
0040f484 beq      #0x40f498
0040f488 mov      r0, r3
0040f48c ldr      r3, [r3]
0040f490 mov      lr, pc
0040f494 ldr      pc, [r3, #0xc]
0040f498 ldr      r3, [pc, #0x38]
0040f49c ldr      r4, [r5, r4]
0040f4a0 ldr      r3, [r5, r3]
0040f4a4 str      r6, [r4]
0040f4a8 ldr      r1, [r6, #8]
0040f4ac ldr      r3, [r3, #0x10]
0040f4b0 ldr      r0, [r3, #0x1c]
0040f4b4 bl       #0x5890c0 ; _ZN6glitch5scene13CSceneManager15setActiveCameraEPNS0_16ICameraSceneNodeE
0040f4b8 ldr      r3, [r4]
0040f4bc mov      r0, r3
0040f4c0 ldr      r3, [r3]
0040f4c4 mov      lr, pc
0040f4c8 ldr      pc, [r3, #8]
0040f4cc pop      {r4, r5, r6, pc}
0040f4d0 subseq   r5, r8, r4, lsr #12
0040f4d4 strheq   r4, [r0], -r0
0040f4d8 strdeq   r3, r4, [r0], -r4

_ZN12CameraTarget12HandleOffsetER7Point3DIfE
004117bc push     {r4, lr}
004117c0 ldrb     r3, [r0, #0x24]
004117c4 sub      sp, sp, #0x10
004117c8 mov      r4, r1
004117cc cmp      r3, #0
004117d0 moveq    r0, r3
004117d4 beq      #0x411828
004117d8 ldr      r2, [r1, #8]
004117dc mov      r3, #0
004117e0 add      r1, sp, #4
004117e4 str      r3, [sp, #0xc]
004117e8 str      r3, [sp, #4]
004117ec str      r3, [sp, #8]
004117f0 bl       #0x40f4dc ; _ZNK10CameraBase15GetCenterOffsetER7Point3DIfEf
004117f4 ldr      r1, [sp, #4]
004117f8 ldr      r0, [r4]
004117fc bl       #0x30eba4
00411800 str      r0, [r4]
00411804 ldr      r1, [sp, #8]
00411808 ldr      r0, [r4, #4]
0041180c bl       #0x30eba4
00411810 str      r0, [r4, #4]
00411814 ldr      r1, [sp, #0xc]
00411818 ldr      r0, [r4, #8]
0041181c bl       #0x30eba4
00411820 str      r0, [r4, #8]
00411824 mov      r0, #1
00411828 add      sp, sp, #0x10
0041182c pop      {r4, pc}

_ZNK10CameraBase14GetCameraUpVecEv
0040e928 push     {r4, lr}
0040e92c ldr      r2, [r1, #8]
0040e930 ldr      r3, [pc, #0x68]
0040e934 mov      r4, r0
0040e938 cmp      r2, #0
0040e93c add      r3, pc, r3
0040e940 beq      #0x40e978
0040e944 ldr      r3, [r2]
0040e948 mov      r0, r2
0040e94c mov      lr, pc
0040e950 ldr      pc, [r3, #0x38]
0040e954 add      r3, r0, #0x20
0040e958 ldr      r1, [r3, #8]
0040e95c ldr      r2, [r0, #0x20]
0040e960 ldr      r3, [r3, #4]
0040e964 mov      r0, r4
0040e968 str      r1, [r4, #8]
0040e96c str      r2, [r4]
0040e970 str      r3, [r4, #4]
0040e974 pop      {r4, pc}
0040e978 ldr      r2, [pc, #0x24]
0040e97c ldr      r3, [r3, r2]
0040e980 ldr      r2, [r3]
0040e984 str      r2, [r0]
0040e988 ldr      r2, [r3, #4]
0040e98c str      r2, [r0, #4]
0040e990 ldr      r3, [r3, #8]
0040e994 str      r3, [r0, #8]
0040e998 mov      r0, r4
0040e99c pop      {r4, pc}
0040e9a0 subseq   r6, r8, r4, asr r1
0040e9a4 andeq    r3, r0, ip, lsr #30

_ZN12CameraTargetD1Ev
00411bbc ldr      r3, [pc, #0x24]
00411bc0 ldr      r2, [pc, #0x24]
00411bc4 push     {r4, lr}
00411bc8 add      r3, pc, r3
00411bcc ldr      r2, [r3, r2]
00411bd0 mov      r4, r0
00411bd4 add      r2, r2, #8
00411bd8 str      r2, [r0]
00411bdc bl       #0x40e780 ; _ZN10CameraBaseD2Ev
00411be0 mov      r0, r4
00411be4 pop      {r4, pc}
00411be8 subseq   r2, r8, r8, asr #29
00411bec ldrdeq   r0, r1, [r0], -r4

_ZN14AnimSetManager5FlushEv
00475664 push     {r4, r5, r6, lr}
00475668 ldr      r3, [r0, #0x14]
0047566c mov      r4, r0
00475670 cmp      r3, #0
00475674 beq      #0x47569c
00475678 add      r5, r0, #4
0047567c mov      r0, r5
00475680 ldr      r1, [r4, #8]
00475684 bl       #0x475624 ; _ZNSt4priv8_Rb_treeIiSt4lessIiESt4pairIKi12AnimationSetENS_10_Select1stIS6_EENS_11_MapTraitsTIS6_EESaIS6_EE8_M_eraseEPNS_18_Rb_tree_node_baseE
00475688 mov      r3, #0
0047568c str      r5, [r4, #0x10]
00475690 str      r3, [r4, #0x14]
00475694 str      r5, [r4, #0xc]
00475698 str      r3, [r4, #8]
0047569c mvn      r3, #0
004756a0 str      r3, [r4, #0x1c]
004756a4 pop      {r4, r5, r6, pc}

_ZN5Level16UpdateCameraZoomEv
003f9870 push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
003f9874 ldr      r5, [pc, #0x2dc]
003f9878 ldr      r6, [pc, #0x2dc]
003f987c sub      sp, sp, #0x14
003f9880 add      r5, pc, r5
003f9884 ldr      r3, [r5, r6]
003f9888 mov      r7, r0
003f988c ldr      r3, [r3, #0x40]
003f9890 ldr      r3, [r3, #0x6c4]
003f9894 cmp      r3, #1
003f9898 beq      #0x3f98ac
003f989c bl       #0x7fd794 ; _Z9GetOnlinev
003f98a0 ldrb     r8, [r0, #5]
003f98a4 cmp      r8, #0
003f98a8 beq      #0x3f98b4
003f98ac add      sp, sp, #0x14
003f98b0 pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
003f98b4 ldr      r3, [pc, #0x2a4]
003f98b8 mov      r1, #0
003f98bc str      r1, [sp, #8]
003f98c0 ldr      r3, [r5, r3]
003f98c4 str      r1, [sp, #0xc]
003f98c8 ldr      r4, [r3]
003f98cc ldr      r0, [r4, #0x1c]
003f98d0 bl       #0x30e4b4
003f98d4 cmp      r0, #0
003f98d8 beq      #0x3f9a78
003f98dc ldr      r0, [r4, #0x20]
003f98e0 mov      r1, #0
003f98e4 bl       #0x30e4b4
003f98e8 cmp      r0, #0
003f98ec bne      #0x3f9914
003f98f0 ldr      r3, [pc, #0x26c]
003f98f4 ldr      r3, [r5, r3]
003f98f8 ldr      r3, [r3]
003f98fc cmp      r3, #2
003f9900 moveq    r3, #0
003f9904 streq    r3, [r3]
003f9908 beq      #0x3f9914
003f990c cmp      r3, #1
003f9910 beq      #0x3f9af0
003f9914 ldr      r0, [r4, #0x18]
003f9918 mov      r1, #0
003f991c bl       #0x30e4b4
003f9920 cmp      r0, #0
003f9924 bne      #0x3f994c
003f9928 ldr      r3, [pc, #0x234]
003f992c ldr      r3, [r5, r3]
003f9930 ldr      r3, [r3]
003f9934 cmp      r3, #2
003f9938 moveq    r3, #0
003f993c streq    r3, [r3]
003f9940 beq      #0x3f994c
003f9944 cmp      r3, #1
003f9948 beq      #0x3f9b24
003f994c ldr      r8, [r5, r6]
003f9950 mvn      r6, #0x80000000
003f9954 sub      r6, r6, #0x800000
003f9958 mov      r5, #0
003f995c add      sl, sp, #8
003f9960 mov      r1, r5
003f9964 mov      r2, #0
003f9968 ldr      r0, [r8, #0x40]
003f996c bl       #0x36e744 ; _ZN13PlayerManager9GetPlayerEib
003f9970 ldr      sb, [r0, #0x660]
003f9974 add      r5, r5, #1
003f9978 subs     r0, sb, #0
003f997c beq      #0x3f9a10
003f9980 ldr      r3, [sb]
003f9984 mov      lr, pc
003f9988 ldr      pc, [r3, #0x34]
003f998c cmp      r0, #0
003f9990 mov      r1, sl
003f9994 add      r0, sb, #0x160
003f9998 bne      #0x3f9a10
003f999c bl       #0x40f714 ; _ZN10CameraBase14GetScreenCoordERK7Point3DIfER7Point2DIfE
003f99a0 ldr      r1, [sp, #8]
003f99a4 ldr      r0, [r4, #0x1c]
003f99a8 bic      r1, r1, #0x80000000
003f99ac bl       #0x30e3ac
003f99b0 mov      r1, r6
003f99b4 mov      fp, r0
003f99b8 bl       #0x30e70c
003f99bc ldr      sb, [sp, #0xc]
003f99c0 cmp      r0, #0
003f99c4 ldr      r0, [r4, #0x20]
003f99c8 mov      r1, sb
003f99cc movne    r6, fp
003f99d0 bl       #0x30e3ac
003f99d4 mov      fp, r0
003f99d8 mov      r1, fp
003f99dc mov      r0, r6
003f99e0 bl       #0x30e2f8
003f99e4 ldr      r1, [r4, #0x18]
003f99e8 cmp      r0, #0
003f99ec mov      r0, sb
003f99f0 movne    r6, fp
003f99f4 bl       #0x30eba4
003f99f8 mov      sb, r0
003f99fc mov      r1, sb
003f9a00 mov      r0, r6
003f9a04 bl       #0x30e2f8
003f9a08 cmp      r0, #0
003f9a0c movne    r6, sb
003f9a10 cmp      r5, #4
003f9a14 bne      #0x3f9960
003f9a18 ldr      r5, [r4, #0xc]
003f9a1c mov      r1, r6
003f9a20 mov      r0, r5
003f9a24 bl       #0x30e3ac
003f9a28 mov      r1, r6
003f9a2c bic      r8, r0, #0x80000000
003f9a30 mov      r0, r5
003f9a34 bl       #0x30e2f8
003f9a38 cmp      r0, #0
003f9a3c bne      #0x3f9acc
003f9a40 mov      r0, r5
003f9a44 mov      r1, r6
003f9a48 bl       #0x30e70c
003f9a4c cmp      r0, #0
003f9a50 beq      #0x3f98ac
003f9a54 ldr      r1, [r4, #0x10]
003f9a58 mov      r0, r8
003f9a5c bl       #0x30ed6c
003f9a60 ldr      r5, [r7, #0x128]
003f9a64 mov      r1, r0
003f9a68 ldr      r0, [r5, #0x8c]
003f9a6c bl       #0x30eba4
003f9a70 str      r0, [r5, #0x8c]
003f9a74 b        #0x3f98ac
003f9a78 ldr      r3, [pc, #0xe4]
003f9a7c ldr      r3, [r5, r3]
003f9a80 ldr      r3, [r3]
003f9a84 cmp      r3, #2
003f9a88 streq    r8, [r8]
003f9a8c beq      #0x3f98dc
003f9a90 cmp      r3, #1
003f9a94 bne      #0x3f98dc
003f9a98 ldr      r0, [pc, #0xc8]
003f9a9c ldr      r1, [pc, #0xc8]
003f9aa0 ldr      r2, [pc, #0xc8]
003f9aa4 ldr      r0, [r5, r0]
003f9aa8 ldr      r3, [pc, #0xc4]
003f9aac mov      ip, #0x24
003f9ab0 add      r1, pc, r1
003f9ab4 add      r2, pc, r2
003f9ab8 add      r3, pc, r3
003f9abc add      r0, r0, #0xa8
003f9ac0 str      ip, [sp]
003f9ac4 bl       #0x30e004
003f9ac8 b        #0x3f98dc
003f9acc ldr      r1, [r4, #0x10]
003f9ad0 mov      r0, r8
003f9ad4 bl       #0x30ed6c
003f9ad8 ldr      r5, [r7, #0x128]
003f9adc mov      r1, r0
003f9ae0 ldr      r0, [r5, #0x8c]
003f9ae4 bl       #0x30e3ac
003f9ae8 str      r0, [r5, #0x8c]
003f9aec b        #0x3f98ac
003f9af0 ldr      r0, [pc, #0x70]
003f9af4 ldr      r1, [pc, #0x7c]
003f9af8 ldr      r2, [pc, #0x7c]
003f9afc ldr      r0, [r5, r0]
003f9b00 ldr      r3, [pc, #0x78]
003f9b04 mov      ip, #0x25
003f9b08 add      r1, pc, r1
003f9b0c add      r2, pc, r2
003f9b10 add      r3, pc, r3
003f9b14 add      r0, r0, #0xa8
003f9b18 str      ip, [sp]
003f9b1c bl       #0x30e004
003f9b20 b        #0x3f9914
003f9b24 ldr      r0, [pc, #0x3c]
003f9b28 ldr      r1, [pc, #0x54]
003f9b2c ldr      r2, [pc, #0x54]
003f9b30 ldr      r0, [r5, r0]
003f9b34 ldr      r3, [pc, #0x50]
003f9b38 mov      ip, #0x26
003f9b3c add      r1, pc, r1
003f9b40 add      r2, pc, r2
003f9b44 add      r3, pc, r3
003f9b48 add      r0, r0, #0xa8
003f9b4c str      ip, [sp]
003f9b50 bl       #0x30e004
003f9b54 b        #0x3f994c
003f9b58 subseq   fp, sb, r0, lsl r2
003f9b5c strdeq   r3, r4, [r0], -r4
003f9b60 andeq    r3, r0, r8, asr #5
003f9b64 andeq    r3, r0, r0, asr #19
003f9b68 andeq    r1, r0, r0, asr #19
003f9b6c subeq    r4, ip, r8, lsr #18
003f9b70 strheq   sp, [ip], #-0x34
003f9b74 ldrdeq   sp, lr, [ip], #-0x38
003f9b78 ldrdeq   r4, r5, [ip], #-0x80
003f9b7c subeq    sp, ip, ip, asr #7
003f9b80 subeq    sp, ip, r0, lsl #7
003f9b84 umaaleq  r4, ip, ip, r8
003f9b88 subeq    sp, ip, r0, asr #7
003f9b8c subeq    sp, ip, ip, asr #6

_ZN10CameraBase13GetWorldCoordERK7Point2DIiER7Point3DIfEf
0040f2f0 ldr      ip, [pc, #0x94]
0040f2f4 ldr      r3, [pc, #0x94]
0040f2f8 push     {r4, r5, r6, r7, r8, lr}
0040f2fc add      ip, pc, ip
0040f300 ldr      r3, [ip, r3]
0040f304 mov      r8, r1
0040f308 ldr      lr, [r0, #4]
0040f30c ldr      r3, [r3, #0x10]
0040f310 ldr      r6, [r0]
0040f314 sub      sp, sp, #0x30
0040f318 ldr      r1, [r3, #0x1c]
0040f31c mov      r5, r2
0040f320 mov      r0, sp
0040f324 ldr      r1, [r1, #0x2c]
0040f328 add      r2, sp, #0x28
0040f32c mov      r3, #0
0040f330 ldr      r7, [r1]
0040f334 mov      r4, sp
0040f338 ldr      ip, [r7, #0x14]
0040f33c str      lr, [sp, #0x2c]
0040f340 str      r6, [sp, #0x28]
0040f344 blx      ip
0040f348 mov      r3, #0
0040f34c mov      r1, r3
0040f350 mov      r2, #0x3f800000
0040f354 mov      r0, r5
0040f358 str      r2, [sp, #0x20]
0040f35c str      r3, [sp, #0x18]
0040f360 str      r3, [sp, #0x1c]
0040f364 bl       #0x30eba4
0040f368 mov      r1, sp
0040f36c add      ip, r0, #0x80000000
0040f370 mov      r3, r8
0040f374 add      r0, sp, #0x18
0040f378 add      r2, sp, #0xc
0040f37c str      ip, [sp, #0x24]
0040f380 bl       #0x40f0b4 ; _ZNK6glitch4core7plane3dIfE30getIntersectionWithLimitedLineERKNS0_8vector3dIfEES6_RS4_
0040f384 add      sp, sp, #0x30
0040f388 pop      {r4, r5, r6, r7, r8, pc}

_ZN24BlendedAnimSetControllerC1EP13RootSceneNodei
00476ddc push     {r4, r5, r6, r7, r8, sl, lr}
00476de0 mov      r5, r2
00476de4 sub      sp, sp, #0x14
00476de8 mov      r2, #1
00476dec ldr      r7, [pc, #0x258]
00476df0 mov      r4, r0
00476df4 bl       #0x474e44 ; _ZN14AnimControllerC2EP13RootSceneNodeb
00476df8 ldr      r3, [pc, #0x250]
00476dfc add      r7, pc, r7
00476e00 ldr      r2, [pc, #0x24c]
00476e04 ldr      r3, [r7, r3]
00476e08 mov      r8, #0
00476e0c ldr      r6, [r7, r2]
00476e10 add      r3, r3, #8
00476e14 str      r3, [r4]
00476e18 mov      r3, #1
00476e1c strb     r3, [r4, #0x10]
00476e20 mov      r1, r5
00476e24 str      r5, [r4, #8]
00476e28 mov      r0, r6
00476e2c str      r8, [r4, #0xc]
00476e30 str      r8, [r4, #0x14]
00476e34 bl       #0x4762c4 ; _ZN14AnimSetManager11GetAnimatorEi
00476e38 mov      r5, r0
00476e3c ldr      r1, [r4, #8]
00476e40 mov      r0, r6
00476e44 bl       #0x4762c4 ; _ZN14AnimSetManager11GetAnimatorEi
00476e48 subs     r3, r5, r8
00476e4c movne    r3, #1
00476e50 subs     sl, r0, r8
00476e54 movne    sl, #1
00476e58 tst      sl, r3
00476e5c mov      r6, r0
00476e60 bne      #0x476ea0
00476e64 cmp      r3, #0
00476e68 beq      #0x476e7c
00476e6c ldr      r3, [r5]
00476e70 ldr      r0, [r3, #-0xc]
00476e74 add      r0, r5, r0
00476e78 bl       #0x31d584 ; _ZNK6glitch17IReferenceCounted4dropEv
00476e7c cmp      sl, #0
00476e80 beq      #0x476e94
00476e84 ldr      r3, [r6]
00476e88 ldr      r0, [r3, #-0xc]
00476e8c add      r0, r6, r0
00476e90 bl       #0x31d584 ; _ZNK6glitch17IReferenceCounted4dropEv
00476e94 mov      r0, r4
00476e98 add      sp, sp, #0x14
00476e9c pop      {r4, r5, r6, r7, r8, sl, pc}
00476ea0 mov      r0, r5
00476ea4 bl       #0x65f11c ; _ZNK6glitch7collada21CSceneNodeAnimatorSet17getAnimationCountEv
00476ea8 cmp      r0, r8
00476eac ble      #0x476fd8
00476eb0 mov      r1, #0
00476eb4 mov      r0, #0xd0
00476eb8 bl       #0x5341ac ; _ZnwjN6glitch6memory13E_MEMORY_HINTE
00476ebc mov      r7, r0
00476ec0 bl       #0x367018 ; _ZN15AnimatorBlenderC1Ev
00476ec4 mov      r3, #1
00476ec8 str      r5, [sp, #0xc]
00476ecc strb     r3, [r7, #0x24]
00476ed0 ldr      r3, [sp, #0xc]
00476ed4 add      r8, r7, #0x28
00476ed8 ldr      r2, [r3]
00476edc ldr      r2, [r2, #-0xc]
00476ee0 add      r3, r3, r2
00476ee4 ldr      r2, [r3, #4]
00476ee8 add      r2, r2, #1
00476eec str      r2, [r3, #4]
00476ef0 ldr      r1, [r7, #0x2c]
00476ef4 ldr      r3, [r7, #0x30]
00476ef8 cmp      r1, r3
00476efc beq      #0x47702c
00476f00 ldr      r3, [sp, #0xc]
00476f04 str      r3, [r1]
00476f08 ldr      r3, [r7, #0x2c]
00476f0c add      r3, r3, #4
00476f10 str      r3, [r7, #0x2c]
00476f14 mov      r3, #1
00476f18 str      r6, [sp, #0xc]
00476f1c strb     r3, [r7, #0x24]
00476f20 ldr      r3, [sp, #0xc]
00476f24 ldr      r2, [r3]
00476f28 ldr      r2, [r2, #-0xc]
00476f2c add      r3, r3, r2
00476f30 ldr      r2, [r3, #4]
00476f34 add      r2, r2, #1
00476f38 str      r2, [r3, #4]
00476f3c ldr      r1, [r7, #0x2c]
00476f40 ldr      r3, [r7, #0x30]
00476f44 cmp      r1, r3
00476f48 beq      #0x47703c
00476f4c ldr      r3, [sp, #0xc]
00476f50 str      r3, [r1]
00476f54 ldr      r3, [r7, #0x2c]
00476f58 add      r3, r3, #4
00476f5c str      r3, [r7, #0x2c]
00476f60 mov      r0, r7
00476f64 ldr      r3, [r7]
00476f68 mov      r1, #0
00476f6c mov      lr, pc
00476f70 ldr      pc, [r3, #0x88]
00476f74 ldr      r3, [r7, #0x34]
00476f78 mov      r2, #0x3f800000
00476f7c mov      r1, r7
00476f80 str      r2, [r3]
00476f84 ldr      r3, [r7, #0x34]
00476f88 mov      r2, #0
00476f8c str      r2, [r3, #4]
00476f90 ldr      r3, [r4, #4]
00476f94 mov      r0, r3
00476f98 ldr      r3, [r3]
00476f9c mov      lr, pc
00476fa0 ldr      pc, [r3, #0x6c]
00476fa4 ldr      r3, [r5]
00476fa8 ldr      r0, [r3, #-0xc]
00476fac add      r0, r5, r0
00476fb0 bl       #0x31d584 ; _ZNK6glitch17IReferenceCounted4dropEv
00476fb4 ldr      r3, [r6]
00476fb8 ldr      r0, [r3, #-0xc]
00476fbc add      r0, r6, r0
00476fc0 bl       #0x31d584 ; _ZNK6glitch17IReferenceCounted4dropEv
00476fc4 ldr      r3, [r7]
00476fc8 ldr      r0, [r3, #-0xc]
00476fcc add      r0, r7, r0
00476fd0 bl       #0x31d584 ; _ZNK6glitch17IReferenceCounted4dropEv
00476fd4 b        #0x476e94
00476fd8 ldr      r3, [pc, #0x78]
00476fdc ldr      r3, [r7, r3]
00476fe0 ldr      r3, [r3]
00476fe4 cmp      r3, #2
00476fe8 streq    r8, [r8]
00476fec beq      #0x476eb0
00476ff0 cmp      r3, #1
00476ff4 bne      #0x476eb0
00476ff8 ldr      r0, [pc, #0x5c]
00476ffc ldr      r1, [pc, #0x5c]
00477000 ldr      r2, [pc, #0x5c]
00477004 ldr      r0, [r7, r0]
00477008 ldr      r3, [pc, #0x58]
0047700c mov      ip, #0x45
00477010 add      r1, pc, r1
00477014 add      r2, pc, r2
00477018 add      r3, pc, r3
0047701c add      r0, r0, #0xa8
00477020 str      ip, [sp]
00477024 bl       #0x30e004
00477028 b        #0x476eb0
0047702c mov      r0, r8
00477030 add      r2, sp, #0xc
00477034 bl       #0x476a9c ; _ZNSt6vectorIPN6glitch7collada18ISceneNodeAnimatorENS0_4core10SAllocatorIS3_LNS0_6memory13E_MEMORY_HINTE0EEEE18_M_insert_overflowEPS3_RKS3_RKSt11__true_typejb.clone.1
00477038 b        #0x476f14
0047703c mov      r0, r8
00477040 add      r2, sp, #0xc
00477044 bl       #0x476a9c ; _ZNSt6vectorIPN6glitch7collada18ISceneNodeAnimatorENS0_4core10SAllocatorIS3_LNS0_6memory13E_MEMORY_HINTE0EEEE18_M_insert_overflowEPS3_RKS3_RKSt11__true_typejb.clone.1
00477048 b        #0x476f60

_ZN14AnimSetManagerD1Ev
004756a8 ldr      r3, [pc, #0x54]
004756ac ldr      r2, [pc, #0x54]
004756b0 push     {r4, r5, r6, lr}
004756b4 add      r3, pc, r3
004756b8 ldr      r2, [r3, r2]
004756bc mov      r4, r0
004756c0 add      r2, r2, #8
004756c4 str      r2, [r0]
004756c8 bl       #0x475664 ; _ZN14AnimSetManager5FlushEv
004756cc ldr      r3, [r4, #0x14]
004756d0 cmp      r3, #0
004756d4 beq      #0x4756fc
004756d8 add      r5, r4, #4
004756dc mov      r0, r5
004756e0 ldr      r1, [r4, #8]
004756e4 bl       #0x475624 ; _ZNSt4priv8_Rb_treeIiSt4lessIiESt4pairIKi12AnimationSetENS_10_Select1stIS6_EENS_11_MapTraitsTIS6_EESaIS6_EE8_M_eraseEPNS_18_Rb_tree_node_baseE
004756e8 mov      r3, #0
004756ec str      r5, [r4, #0x10]
004756f0 str      r3, [r4, #0x14]
004756f4 str      r5, [r4, #0xc]
004756f8 str      r3, [r4, #8]
004756fc mov      r0, r4
00475700 pop      {r4, r5, r6, pc}
00475704 ldrsbeq  pc, [r1], #-0x3c
00475708 andeq    r4, r0, ip, ror #6

_ZN5Level11_LoadCameraEv
003f1008 push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
003f100c mov      r1, #0
003f1010 sub      sp, sp, #0x14
003f1014 mov      r4, r0
003f1018 mov      r0, #0x28
003f101c bl       #0x310570 ; _Znwj15MemoryHintState
003f1020 mov      r6, r0
003f1024 bl       #0x41106c ; _ZN14CameraOverviewC1Ev
003f1028 str      r6, [r4, #0x12c]
003f102c mov      r1, #0
003f1030 mov      r0, #0xa8
003f1034 bl       #0x310570 ; _Znwj15MemoryHintState
003f1038 ldr      r5, [pc, #0x308]
003f103c mov      r6, r0
003f1040 bl       #0x40ff70 ; _ZN11CameraLevelC1Ev
003f1044 cmp      r6, #0
003f1048 str      r6, [r4, #0x128]
003f104c add      r5, pc, r5
003f1050 beq      #0x3f12f0
003f1054 ldr      r7, [r4, #0x38]
003f1058 cmp      r7, #0
003f105c beq      #0x3f1240
003f1060 ldr      r3, [r7, #0x260]
003f1064 str      r3, [sp, #0xc]
003f1068 ldr      r3, [pc, #0x2dc]
003f106c ldr      sb, [r7, #0x278]
003f1070 ldr      r3, [r5, r3]
003f1074 ldr      sl, [r3]
003f1078 cmp      sl, #0
003f107c beq      #0x3f1130
003f1080 ldr      r3, [pc, #0x2c8]
003f1084 mov      r8, #0
003f1088 ldr      r3, [r5, r3]
003f108c ldr      fp, [r3]
003f1090 b        #0x3f10a0
003f1094 add      r8, r8, #1
003f1098 cmp      r8, sl
003f109c beq      #0x3f1130
003f10a0 ldr      r1, [fp, r8, lsl #2]
003f10a4 mov      r0, sb
003f10a8 bl       #0x30e31c
003f10ac cmp      r0, #0
003f10b0 bne      #0x3f1094
003f10b4 mov      r0, r6
003f10b8 ldr      r3, [r7, #0x290]
003f10bc mov      r2, r8
003f10c0 ldr      r1, [sp, #0xc]
003f10c4 bl       #0x41068c ; _ZN11CameraLevel4LoadEPKciS1_
003f10c8 ldr      r7, [r4, #0x38]
003f10cc ldr      r6, [r4, #0x128]
003f10d0 cmp      r7, #0
003f10d4 bne      #0x3f1154
003f10d8 ldr      r3, [pc, #0x274]
003f10dc ldr      r3, [r5, r3]
003f10e0 ldr      r3, [r3]
003f10e4 cmp      r3, #2
003f10e8 streq    r7, [r7]
003f10ec beq      #0x3f1154
003f10f0 cmp      r3, #1
003f10f4 bne      #0x3f1154
003f10f8 ldr      r0, [pc, #0x258]
003f10fc ldr      r1, [pc, #0x258]
003f1100 ldr      r2, [pc, #0x258]
003f1104 ldr      r0, [r5, r0]
003f1108 ldr      r3, [pc, #0x254]
003f110c mov      ip, #0x1dc
003f1110 add      r1, pc, r1
003f1114 add      r0, r0, #0xa8
003f1118 add      r2, pc, r2
003f111c add      r3, pc, r3
003f1120 str      ip, [sp]
003f1124 bl       #0x30e004
003f1128 ldr      r7, [r4, #0x38]
003f112c b        #0x3f1154
003f1130 mov      r0, r6
003f1134 ldr      r3, [r7, #0x290]
003f1138 mvn      r2, #0
003f113c ldr      r1, [sp, #0xc]
003f1140 bl       #0x41068c ; _ZN11CameraLevel4LoadEPKciS1_
003f1144 ldr      r7, [r4, #0x38]
003f1148 ldr      r6, [r4, #0x128]
003f114c cmp      r7, #0
003f1150 beq      #0x3f10d8
003f1154 ldr      r0, [r7, #0x294]
003f1158 bl       #0x30e964
003f115c mov      r8, r0
003f1160 ldr      r0, [r7, #0x298]
003f1164 bl       #0x30e964
003f1168 movw     r1, #0xf877
003f116c movw     r2, #0x78e9
003f1170 mov      r3, r8
003f1174 movt     r1, #0x3edb
003f1178 movt     r2, #0x3fd5
003f117c str      r0, [sp]
003f1180 mov      r0, r6
003f1184 mov      r6, #0
003f1188 str      r6, [sp, #4]
003f118c bl       #0x40e9a8 ; _ZN10CameraBase7SetDataEffffb
003f1190 ldr      r0, [r4, #0x128]
003f1194 bl       #0x40f45c ; _ZN10CameraBase9SetActiveEv
003f1198 ldr      r3, [pc, #0x1c8]
003f119c ldr      r0, [r4, #0x128]
003f11a0 mov      lr, #0x1c
003f11a4 ldr      r3, [r5, r3]
003f11a8 ldr      r1, [r0, #0x80]
003f11ac mov      r2, r6
003f11b0 ldr      ip, [r3]
003f11b4 mov      r3, r6
003f11b8 mla      r1, lr, r1, ip
003f11bc ldr      r1, [r1, #0x10]
003f11c0 bl       #0x40f904 ; _ZN11CameraLevel8PlayAnimEiib
003f11c4 ldr      r3, [pc, #0x1a0]
003f11c8 mov      r1, r6
003f11cc mov      r2, #1
003f11d0 ldr      r7, [r5, r3]
003f11d4 ldr      r8, [r4, #0x128]
003f11d8 ldr      r0, [r7, #0x40]
003f11dc bl       #0x36e478 ; _ZN13PlayerManager14GetLocalPlayerEib
003f11e0 mov      r2, r6
003f11e4 ldr      r1, [r0, #0x660]
003f11e8 mov      r0, r8
003f11ec bl       #0x4119c4 ; _ZN12CameraTarget9SetTargetEP10GameObjecti
003f11f0 ldr      r0, [r7, #0x50]
003f11f4 ldr      r1, [r4, #0x128]
003f11f8 bl       #0x381fb0 ; _ZN11ZoomHandler9setCameraEP11CameraLevel
003f11fc ldr      r3, [r4, #0x128]
003f1200 mov      r2, #0x3f800000
003f1204 str      r2, [r3, #0x8c]
003f1208 ldr      r3, [r4, #0x128]
003f120c mov      r2, #0
003f1210 str      r2, [r3, #0x88]
003f1214 ldr      r3, [r4, #0x38]
003f1218 ldr      r2, [r7, #0x10]
003f121c cmp      r3, r6
003f1220 ldr      r6, [r2, #0x1c]
003f1224 beq      #0x3f1298
003f1228 ldr      r1, [r3, #0x248]
003f122c mov      r0, r6
003f1230 mov      r2, #0
003f1234 add      sp, sp, #0x14
003f1238 pop      {r4, r5, r6, r7, r8, sb, sl, fp, lr}
003f123c b        #0x359a38 ; _ZN12SceneManager18AddSkyBoxSceneNodeEPKcS1_
003f1240 ldr      r3, [pc, #0x10c]
003f1244 ldr      r3, [r5, r3]
003f1248 ldr      r3, [r3]
003f124c cmp      r3, #2
003f1250 streq    r7, [r7]
003f1254 beq      #0x3f1060
003f1258 cmp      r3, #1
003f125c bne      #0x3f1060
003f1260 ldr      r0, [pc, #0xf0]
003f1264 ldr      r1, [pc, #0x104]
003f1268 ldr      r2, [pc, #0x104]
003f126c ldr      r0, [r5, r0]
003f1270 ldr      r3, [pc, #0x100]
003f1274 mov      ip, #0x1dc
003f1278 add      r1, pc, r1
003f127c add      r0, r0, #0xa8
003f1280 add      r2, pc, r2
003f1284 add      r3, pc, r3
003f1288 str      ip, [sp]
003f128c bl       #0x30e004
003f1290 ldr      r7, [r4, #0x38]
003f1294 b        #0x3f1060
003f1298 ldr      r2, [pc, #0xb4]
003f129c ldr      r2, [r5, r2]
003f12a0 ldr      r2, [r2]
003f12a4 cmp      r2, #2
003f12a8 streq    r3, [r3]
003f12ac beq      #0x3f1228
003f12b0 cmp      r2, #1
003f12b4 bne      #0x3f1228
003f12b8 ldr      r0, [pc, #0x98]
003f12bc ldr      r1, [pc, #0xb8]
003f12c0 ldr      r2, [pc, #0xb8]
003f12c4 ldr      r0, [r5, r0]
003f12c8 ldr      r3, [pc, #0xb4]
003f12cc mov      ip, #0x1dc
003f12d0 add      r1, pc, r1
003f12d4 add      r3, pc, r3
003f12d8 add      r0, r0, #0xa8
003f12dc add      r2, pc, r2
003f12e0 str      ip, [sp]
003f12e4 bl       #0x30e004
003f12e8 ldr      r3, [r4, #0x38]
003f12ec b        #0x3f1228
003f12f0 ldr      r3, [pc, #0x5c]
003f12f4 ldr      r3, [r5, r3]
003f12f8 ldr      r3, [r3]
003f12fc cmp      r3, #2
003f1300 streq    r6, [r6]
003f1304 beq      #0x3f1054
003f1308 cmp      r3, #1
003f130c bne      #0x3f1054
003f1310 ldr      r0, [pc, #0x40]
003f1314 ldr      r1, [pc, #0x6c]
003f1318 ldr      r2, [pc, #0x6c]
003f131c ldr      r0, [r5, r0]
003f1320 ldr      r3, [pc, #0x68]
003f1324 mov      ip, #0x9b0
003f1328 add      r1, pc, r1
003f132c add      r0, r0, #0xa8
003f1330 add      r2, pc, r2
003f1334 add      r3, pc, r3
003f1338 str      ip, [sp]
003f133c bl       #0x30e004
003f1340 ldr      r6, [r4, #0x128]
003f1344 b        #0x3f1054
003f1348 subseq   r3, sl, r4, asr #20
003f134c andeq    r3, r0, r4, ror #17
003f1350 andeq    r3, r0, ip, asr sl
003f1354 andeq    r3, r0, r0, asr #19
003f1358 andeq    r1, r0, r0, asr #19
003f135c subeq    sp, ip, r8, asr #5
003f1360 ldrdeq   r4, r5, [sp], #-0xa8
003f1364 subeq    r0, sp, ip, lsr fp
003f1368 ldrdeq   r3, r4, [r0], -r4
003f136c strdeq   r3, r4, [r0], -r4
003f1370 subeq    sp, ip, r0, ror #2
003f1374 subeq    r4, sp, r0, ror sb
003f1378 ldrdeq   r0, r1, [sp], #-0x94
003f137c subeq    sp, ip, r8, lsl #2
003f1380 subeq    r4, sp, r4, lsl sb
003f1384 subeq    r0, sp, r4, lsl #19
003f1388 strheq   sp, [ip], #-0
003f138c subeq    r5, sp, r8, lsr #5
003f1390 ldrdeq   r5, r6, [sp], #-0x1c

_ZN10CameraBaseC1Ev
0040e750 ldr      r3, [pc, #0x20]
0040e754 ldr      r1, [pc, #0x20]
0040e758 mov      ip, #0
0040e75c add      r3, pc, r3
0040e760 ldr      r1, [r3, r1]
0040e764 str      ip, [r0, #8]
0040e768 str      ip, [r0, #4]
0040e76c add      r1, r1, #8
0040e770 str      r1, [r0]
0040e774 bx       lr
0040e778 subseq   r6, r8, r4, lsr r3
0040e77c andeq    r2, r0, ip, ror #3

_ZN22Script_SetCameraTarget7ExecuteEbi
00460004 push     {r4, r5, r6, r7, r8, sb, sl, lr}
00460008 ldr      r4, [pc, #0xec]
0046000c ldr      r6, [pc, #0xec]
00460010 ldr      r2, [pc, #0xec]
00460014 add      r4, pc, r4
00460018 ldr      r3, [r4, r6]
0046001c ldr      r7, [r4, r2]
00460020 sub      sp, sp, #0x20
00460024 ldr      r3, [r3]
00460028 mov      sl, r1
0046002c add      r5, sp, #4
00460030 str      r3, [sp, #0x1c]
00460034 ldr      r8, [r0, #0xc]
00460038 mov      r0, r7
0046003c bl       #0x337888 ; _ZN13DebugSwitches4loadEv
00460040 ldr      r1, [pc, #0xc0]
00460044 mov      r2, sp
00460048 mov      r0, r5
0046004c add      r1, pc, r1
00460050 bl       #0x3140ec ; _ZNSsC1EPKcRKSaIcE
00460054 mov      r1, r5
00460058 mov      r0, r7
0046005c bl       #0x337a88 ; _ZN13DebugSwitches9GetSwitchERKSs
00460060 mov      r0, r5
00460064 bl       #0x318254 ; _ZNSsD1Ev
00460068 ldr      r3, [pc, #0x9c]
0046006c ldr      sb, [r4, r3]
00460070 mov      r0, sb
00460074 bl       #0x31f594 ; _ZNK11Application15GetCurrentLevelEv
00460078 cmp      r0, #0
0046007c beq      #0x4600b0
00460080 ldr      r5, [r0, #0x128]
00460084 cmp      r5, #0
00460088 beq      #0x4600b0
0046008c ldr      r1, [r8, #0x10]
00460090 ldrb     r7, [r1]
00460094 cmp      r7, #0
00460098 beq      #0x4600cc
0046009c cmp      sl, #0
004600a0 movne    r2, #0
004600a4 ldreq    r2, [r8, #8]
004600a8 mov      r0, r5
004600ac bl       #0x411b38 ; _ZN12CameraTarget9SetTargetEPKci
004600b0 ldr      r3, [r4, r6]
004600b4 ldr      r2, [sp, #0x1c]
004600b8 ldr      r3, [r3]
004600bc cmp      r2, r3
004600c0 bne      #0x4600f8
004600c4 add      sp, sp, #0x20
004600c8 pop      {r4, r5, r6, r7, r8, sb, sl, pc}
004600cc mov      r2, #1
004600d0 mov      r1, r7
004600d4 ldr      r0, [sb, #0x40]
004600d8 bl       #0x36e478 ; _ZN13PlayerManager14GetLocalPlayerEib
004600dc cmp      sl, #0
004600e0 ldr      r1, [r0, #0x660]
004600e4 movne    r2, r7
004600e8 ldreq    r2, [r8, #8]
004600ec mov      r0, r5
004600f0 bl       #0x4119c4 ; _ZN12CameraTarget9SetTargetEP10GameObjecti
004600f4 b        #0x4600b0
004600f8 bl       #0x30e310
004600fc subseq   r4, r3, ip, ror sl
00460100 andeq    r4, r0, ip, lsr #1
00460104 andeq    r0, r0, r4, lsl #17
00460108 subeq    sp, r6, r4, lsr r0
0046010c strdeq   r3, r4, [r0], -r4

_ZN17AnimSetControllerC2EP13RootSceneNodei
00475248 push     {r4, r5, r6, lr}
0047524c mov      r6, r2
00475250 ldr      r5, [pc, #0x78]
00475254 mov      r2, #1
00475258 mov      r4, r0
0047525c bl       #0x474e44 ; _ZN14AnimControllerC2EP13RootSceneNodeb
00475260 ldr      r3, [pc, #0x6c]
00475264 add      r5, pc, r5
00475268 str      r6, [r4, #8]
0047526c ldr      r3, [r5, r3]
00475270 mov      r1, r6
00475274 add      r3, r3, #8
00475278 str      r3, [r4]
0047527c mov      r3, #0
00475280 str      r3, [r4, #0xc]
00475284 mov      r3, #1
00475288 strb     r3, [r4, #0x10]
0047528c ldr      r3, [pc, #0x44]
00475290 ldr      r0, [r5, r3]
00475294 bl       #0x4762c4 ; _ZN14AnimSetManager11GetAnimatorEi
00475298 subs     r5, r0, #0
0047529c beq      #0x4752c8
004752a0 ldr      r3, [r4, #4]
004752a4 mov      r1, r5
004752a8 mov      r0, r3
004752ac ldr      r3, [r3]
004752b0 mov      lr, pc
004752b4 ldr      pc, [r3, #0x6c]
004752b8 ldr      r3, [r5]
004752bc ldr      r0, [r3, #-0xc]
004752c0 add      r0, r5, r0
004752c4 bl       #0x31d584 ; _ZNK6glitch17IReferenceCounted4dropEv
004752c8 mov      r0, r4
004752cc pop      {r4, r5, r6, pc}
004752d0 subseq   pc, r1, ip, lsr #16
004752d4 andeq    r3, r0, ip, lsr #10
004752d8 andeq    r4, r0, r8, lsr r8

_ZN17AnimSetControllerC1EP13RootSceneNodei
004751b4 push     {r4, r5, r6, lr}
004751b8 mov      r6, r2
004751bc ldr      r5, [pc, #0x78]
004751c0 mov      r2, #1
004751c4 mov      r4, r0
004751c8 bl       #0x474e44 ; _ZN14AnimControllerC2EP13RootSceneNodeb
004751cc ldr      r3, [pc, #0x6c]
004751d0 add      r5, pc, r5
004751d4 str      r6, [r4, #8]
004751d8 ldr      r3, [r5, r3]
004751dc mov      r1, r6
004751e0 add      r3, r3, #8
004751e4 str      r3, [r4]
004751e8 mov      r3, #0
004751ec str      r3, [r4, #0xc]
004751f0 mov      r3, #1
004751f4 strb     r3, [r4, #0x10]
004751f8 ldr      r3, [pc, #0x44]
004751fc ldr      r0, [r5, r3]
00475200 bl       #0x4762c4 ; _ZN14AnimSetManager11GetAnimatorEi
00475204 subs     r5, r0, #0
00475208 beq      #0x475234
0047520c ldr      r3, [r4, #4]
00475210 mov      r1, r5
00475214 mov      r0, r3
00475218 ldr      r3, [r3]
0047521c mov      lr, pc
00475220 ldr      pc, [r3, #0x6c]
00475224 ldr      r3, [r5]
00475228 ldr      r0, [r3, #-0xc]
0047522c add      r0, r5, r0
00475230 bl       #0x31d584 ; _ZNK6glitch17IReferenceCounted4dropEv
00475234 mov      r0, r4
00475238 pop      {r4, r5, r6, pc}
0047523c subseq   pc, r1, r0, asr #17
00475240 andeq    r3, r0, ip, lsr #10
00475244 andeq    r4, r0, r8, lsr r8

_ZNK14AnimSetManager12DBG_DumpInfoEv
00475488 push     {r4, r5}
0047548c ldr      ip, [r0, #0xc]
00475490 add      r4, r0, #4
00475494 cmp      ip, r4
00475498 beq      #0x475504
0047549c ldr      r3, [ip, #0x24]
004754a0 add      r0, ip, #0x1c
004754a4 cmp      r3, r0
004754a8 beq      #0x4754d8
004754ac ldr      r2, [r3, #0xc]
004754b0 cmp      r2, #0
004754b4 bne      #0x4754c0
004754b8 b        #0x47550c
004754bc mov      r2, r3
004754c0 ldr      r3, [r2, #8]
004754c4 cmp      r3, #0
004754c8 bne      #0x4754bc
004754cc mov      r3, r2
004754d0 cmp      r0, r3
004754d4 bne      #0x4754ac
004754d8 ldr      r2, [ip, #0xc]
004754dc cmp      r2, #0
004754e0 beq      #0x475540
004754e4 mov      ip, r2
004754e8 b        #0x4754f0
004754ec mov      ip, r3
004754f0 ldr      r3, [ip, #8]
004754f4 cmp      r3, #0
004754f8 bne      #0x4754ec
004754fc cmp      r4, ip
00475500 bne      #0x47549c
00475504 pop      {r4, r5}
00475508 bx       lr
0047550c ldr      r1, [r3, #4]
00475510 ldr      r5, [r1, #0xc]
00475514 cmp      r5, r3
00475518 bne      #0x475534
0047551c mov      r3, r1
00475520 ldr      r1, [r1, #4]
00475524 ldr      r2, [r1, #0xc]
00475528 cmp      r2, r3
0047552c beq      #0x47551c
00475530 ldr      r2, [r3, #0xc]
00475534 cmp      r2, r1
00475538 movne    r3, r1
0047553c b        #0x4754d0
00475540 ldr      r3, [ip, #4]
00475544 ldr      r1, [r3, #0xc]
00475548 cmp      r1, ip
0047554c bne      #0x475568
00475550 mov      ip, r3
00475554 ldr      r3, [r3, #4]
00475558 ldr      r2, [r3, #0xc]
0047555c cmp      r2, ip
00475560 beq      #0x475550
00475564 ldr      r2, [ip, #0xc]
00475568 cmp      r3, r2
0047556c movne    ip, r3
00475570 cmp      r4, ip
00475574 bne      #0x47549c
00475578 b        #0x475504

_ZN7Structs15SetCameraTargetD2Ev
004d37a0 push     {r4, lr}
004d37a4 ldr      r3, [pc, #0x34]
004d37a8 ldr      r2, [pc, #0x34]
004d37ac mov      r4, r0
004d37b0 add      r3, pc, r3
004d37b4 ldr      r0, [r0, #0x10]
004d37b8 ldr      r2, [r3, r2]
004d37bc cmp      r0, #0
004d37c0 add      r2, r2, #8
004d37c4 str      r2, [r4]
004d37c8 beq      #0x4d37d0
004d37cc bl       #0x310440 ; _Z10CustomFreePv
004d37d0 mov      r0, r4
004d37d4 bl       #0x4c6c60 ; _ZN7Structs9ScriptCmdD2Ev
004d37d8 mov      r0, r4
004d37dc pop      {r4, pc}
004d37e0 subeq    r1, ip, r0, ror #5
004d37e4 andeq    r3, r0, r0, lsr r0

_ZN24BlendedAnimSetController8PlayClipEPKcbij
00476690 mov      r0, #0
00476694 bx       lr

_ZN11CameraLevelD0Ev
0040fee8 push     {r4, lr}
0040feec mov      r4, r0
0040fef0 bl       #0x40fe7c ; _ZN11CameraLevelD1Ev
0040fef4 mov      r0, r4
0040fef8 bl       #0x310440 ; _Z10CustomFreePv
0040fefc mov      r0, r4
0040ff00 pop      {r4, pc}

_ZN10CameraBase6UpdateEv
0040ea50 bx       lr

_ZNK17AnimSetController15GetClipDurationEj
00474f60 mvn      r0, #0
00474f64 bx       lr

_ZN24BlendedAnimSetControllerD0Ev
004769c4 push     {r4, lr}
004769c8 mov      r4, r0
004769cc bl       #0x476990 ; _ZN24BlendedAnimSetControllerD1Ev
004769d0 mov      r0, r4
004769d4 bl       #0x310440 ; _Z10CustomFreePv
004769d8 mov      r0, r4
004769dc pop      {r4, pc}

_ZN12CameraTargetC2Ev
00411cdc push     {r4, r5, r6, lr}
00411ce0 ldr      r5, [pc, #0x84]
00411ce4 mov      r4, r0
00411ce8 bl       #0x40e720 ; _ZN10CameraBaseC2Ev
00411cec ldr      r1, [pc, #0x7c]
00411cf0 add      r5, pc, r5
00411cf4 ldr      r2, [pc, #0x78]
00411cf8 ldr      r1, [r5, r1]
00411cfc mov      r3, #0
00411d00 ldr      r2, [r5, r2]
00411d04 add      r0, r1, #8
00411d08 str      r0, [r4]
00411d0c mov      r0, #1
00411d10 strb     r0, [r4, #0x25]
00411d14 movw     r0, #0x3333
00411d18 mov      r1, #0
00411d1c movt     r0, #0x3f33
00411d20 str      r0, [r4, #0x28]
00411d24 strb     r1, [r4, #0x24]
00411d28 str      r1, [r4, #0xc]
00411d2c str      r3, [r4, #0x10]
00411d30 str      r3, [r4, #0x14]
00411d34 str      r3, [r4, #0x18]
00411d38 str      r1, [r4, #0x1c]
00411d3c str      r1, [r4, #0x20]
00411d40 ldr      r1, [r2]
00411d44 mov      r0, r4
00411d48 str      r1, [r4, #0x2c]
00411d4c ldr      r1, [r2, #4]
00411d50 str      r1, [r4, #0x30]
00411d54 ldr      r2, [r2, #8]
00411d58 str      r3, [r4, #0x40]
00411d5c str      r3, [r4, #0x38]
00411d60 str      r2, [r4, #0x34]
00411d64 str      r3, [r4, #0x3c]
00411d68 pop      {r4, r5, r6, pc}
00411d6c subseq   r2, r8, r0, lsr #27
00411d70 ldrdeq   r0, r1, [r0], -r4
00411d74 andeq    r3, r0, ip, lsr #30

_ZN11CameraLevel6UpdateEv
00410390 ldr      r3, [pc, #0x2e0]
00410394 ldr      r2, [pc, #0x2e0]
00410398 push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0041039c add      r3, pc, r3
004103a0 ldr      r2, [r3, r2]
004103a4 sub      sp, sp, #0x2c
004103a8 mov      r4, r0
004103ac ldr      r3, [r2]
004103b0 cmp      r0, r3
004103b4 beq      #0x4103c8
004103b8 mov      r0, r4
004103bc bl       #0x40ea50 ; _ZN10CameraBase6UpdateEv
004103c0 add      sp, sp, #0x2c
004103c4 pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
004103c8 ldr      r3, [r0, #0xc]
004103cc cmp      r3, #0
004103d0 beq      #0x4103b8
004103d4 ldr      r3, [r0, #8]
004103d8 cmp      r3, #0
004103dc beq      #0x4103b8
004103e0 ldr      r3, [r0, #4]
004103e4 cmp      r3, #0
004103e8 beq      #0x4103b8
004103ec bl       #0x411830 ; _ZN12CameraTarget16HandleTransitionEv
004103f0 cmp      r0, #0
004103f4 bne      #0x4103c0
004103f8 ldrb     r3, [r4, #0x85]
004103fc cmp      r3, #0
00410400 beq      #0x410610
00410404 ldr      r3, [r4, #0xc]
00410408 add      r3, r3, #0x160
0041040c ldr      r2, [r3]
00410410 add      r5, sp, #0x1c
00410414 mov      r1, r5
00410418 str      r2, [sp, #0x1c]
0041041c ldr      r2, [r3, #4]
00410420 mov      r0, r4
00410424 str      r2, [sp, #0x20]
00410428 ldr      r3, [r3, #8]
0041042c str      r3, [sp, #0x24]
00410430 bl       #0x4117bc ; _ZN12CameraTarget12HandleOffsetER7Point3DIfE
00410434 mov      r0, r4
00410438 mov      r1, r5
0041043c bl       #0x40fa68 ; _ZN11CameraLevel15HandleCenteringER7Point3DIfE
00410440 ldrb     r3, [r4, #0x85]
00410444 ldr      r7, [r4, #0x98]
00410448 ldr      r6, [r4, #0x9c]
0041044c cmp      r3, #0
00410450 ldr      sl, [r4, #0xa0]
00410454 moveq    fp, r7
00410458 beq      #0x4104ec
0041045c ldr      r8, [pc, #0x21c]
00410460 add      r8, pc, r8
00410464 ldr      r3, [r8, #0xc]
00410468 tst      r3, #1
0041046c beq      #0x410620
00410470 ldr      r8, [pc, #0x20c]
00410474 add      r8, pc, r8
00410478 ldr      r3, [r8, #0x14]
0041047c tst      r3, #1
00410480 beq      #0x41064c
00410484 ldr      r8, [pc, #0x1fc]
00410488 mov      r1, r7
0041048c add      r8, pc, r8
00410490 ldr      sb, [r8, #0x10]
00410494 mov      r0, sb
00410498 bl       #0x30ed6c
0041049c ldr      r8, [r8, #0x18]
004104a0 mov      fp, r0
004104a4 mov      r1, r6
004104a8 mov      r0, r8
004104ac bl       #0x30ed6c
004104b0 mov      r1, r0
004104b4 mov      r0, fp
004104b8 bl       #0x30e3ac
004104bc mov      r1, r6
004104c0 mov      fp, r0
004104c4 mov      r0, sb
004104c8 bl       #0x30ed6c
004104cc mov      r1, r7
004104d0 mov      r6, r0
004104d4 mov      r0, r8
004104d8 bl       #0x30ed6c
004104dc mov      r1, r0
004104e0 mov      r0, r6
004104e4 bl       #0x30eba4
004104e8 mov      r6, r0
004104ec mov      r1, fp
004104f0 ldr      r0, [sp, #0x1c]
004104f4 bl       #0x30eba4
004104f8 mov      r1, r6
004104fc str      r0, [sp, #0x1c]
00410500 ldr      r0, [sp, #0x20]
00410504 bl       #0x30eba4
00410508 mov      r1, sl
0041050c str      r0, [sp, #0x20]
00410510 ldr      r0, [sp, #0x24]
00410514 bl       #0x30eba4
00410518 ldrb     r3, [r4, #0x84]
0041051c str      r0, [sp, #0x24]
00410520 cmp      r3, #0
00410524 bne      #0x4105c4
00410528 mov      r0, r4
0041052c bl       #0x410218 ; _ZN11CameraLevel10HandleZoomEv
00410530 ldr      r0, [r4, #0x90]
00410534 add      r0, r0, #0x80000000
00410538 ldr      r1, [r4, #0x94]
0041053c bl       #0x30ed6c
00410540 ldr      r3, [r4, #8]
00410544 mov      r2, #0
00410548 str      r2, [sp, #0x14]
0041054c str      r2, [sp, #0x10]
00410550 str      r0, [sp, #0x18]
00410554 add      r1, sp, #0x10
00410558 mov      r0, r3
0041055c ldr      r3, [r3]
00410560 mov      lr, pc
00410564 ldr      pc, [r3, #0xa4]
00410568 mov      r1, r5
0041056c mov      r0, r4
00410570 bl       #0x4115d8 ; _ZN12CameraTarget14HandleGhostCamER7Point3DIfE
00410574 mov      r1, r5
00410578 mov      r0, r4
0041057c bl       #0x41165c ; _ZN12CameraTarget13HandleDampingER7Point3DIfE
00410580 ldr      r0, [r4, #4]
00410584 ldr      r2, [sp, #0x1c]
00410588 add      r1, sp, #4
0041058c ldr      r3, [r0]
00410590 ldr      r3, [r3, #0xa4]
00410594 str      r2, [sp, #4]
00410598 ldr      r2, [sp, #0x20]
0041059c str      r2, [sp, #8]
004105a0 ldr      r2, [sp, #0x24]
004105a4 str      r2, [sp, #0xc]
004105a8 blx      r3
004105ac ldr      r3, [r4, #0xc]
004105b0 ldrb     r3, [r3, #0x81]
004105b4 cmp      r3, #0
004105b8 movne    r3, #0
004105bc strne    r3, [r4, #0xc]
004105c0 b        #0x4103b8
004105c4 ldrb     r3, [r4, #0xa4]
004105c8 cmp      r3, #0
004105cc bne      #0x410528
004105d0 ldr      r6, [r4, #0x90]
004105d4 movw     r1, #0xcccd
004105d8 movt     r1, #0x3dcc
004105dc bic      r0, r6, #0x80000000
004105e0 bl       #0x30e70c
004105e4 cmp      r0, #0
004105e8 movne    r3, #0
004105ec movne    r0, #0x80000000
004105f0 strne    r3, [r4, #0x90]
004105f4 bne      #0x410538
004105f8 mov      r0, r6
004105fc mov      r1, #0x3f400000
00410600 bl       #0x30ed6c
00410604 str      r0, [r4, #0x90]
00410608 add      r0, r0, #0x80000000
0041060c b        #0x410538
00410610 mov      r0, r4
00410614 bl       #0x411a74 ; _ZN12CameraTarget17GetTargetPositionEv
00410618 mov      r3, r0
0041061c b        #0x41040c
00410620 add      sb, r8, #0xc
00410624 mov      r0, sb
00410628 bl       #0x30e76c
0041062c cmp      r0, #0
00410630 beq      #0x410470
00410634 movw     r3, #0x4f3
00410638 movt     r3, #0x3f35
0041063c mov      r0, sb
00410640 str      r3, [r8, #0x10]
00410644 bl       #0x30ea3c
00410648 b        #0x410470
0041064c add      sb, r8, #0x14
00410650 mov      r0, sb
00410654 bl       #0x30e76c
00410658 cmp      r0, #0
0041065c beq      #0x410484
00410660 movw     r3, #0x4f3
00410664 movt     r3, #0x3f35
00410668 mov      r0, sb
0041066c str      r3, [r8, #0x18]
00410670 bl       #0x30ea3c
00410674 b        #0x410484
00410678 ldrsheq  r4, [r8], #-0x64
0041067c strheq   r4, [r0], -r0
00410680 ldrheq   r2, [sb], #-0xdc
00410684 subseq   r2, sb, r8, lsr #27

_ZN17AnimSetController12SetCallbacksEPFvPN6glitch5scene19ITimelineControllerEPvES4_PFvRKNS0_7collada15STriggeredEventES4_ES4_
00474fb0 push     {r4, r5, r6, r7, r8, lr}
00474fb4 mov      r5, r1
00474fb8 mov      r1, #0
00474fbc mov      r7, r2
00474fc0 mov      r6, r3
00474fc4 ldr      r4, [sp, #0x18]
00474fc8 bl       #0x4748b8 ; _ZN14AnimController7GetAnimEj
00474fcc cmp      r0, #0
00474fd0 beq      #0x474fec
00474fd4 mov      r1, r5
00474fd8 mov      r2, r7
00474fdc mov      r3, r6
00474fe0 str      r4, [sp, #0x18]
00474fe4 pop      {r4, r5, r6, r7, r8, lr}
00474fe8 b        #0x367510 ; _ZN11AnimatorSet12SetCallbacksEPFvPN6glitch5scene19ITimelineControllerEPvES4_PFvRKNS0_7collada15STriggeredEventES4_ES4_
00474fec pop      {r4, r5, r6, r7, r8, pc}

_ZN14AnimSetManager7AddAnimEii
0047653c push     {r4, r5, r6, r7, r8, sl, lr}
00476540 ldr      r4, [pc, #0x124]
00476544 ldr      r5, [pc, #0x124]
00476548 sub      sp, sp, #0x2c
0047654c add      r4, pc, r4
00476550 ldr      r3, [r4, r5]
00476554 mov      r7, r0
00476558 str      r1, [sp, #4]
0047655c ldr      r3, [r3]
00476560 mov      r8, r2
00476564 str      r3, [sp, #0x24]
00476568 bl       #0x475404 ; _ZNK14AnimSetManager6ExistsEi
0047656c cmp      r0, #0
00476570 addne    r7, r7, #4
00476574 addne    r6, sp, #4
00476578 bne      #0x4765bc
0047657c ldr      r3, [sp, #4]
00476580 cmp      r3, #0
00476584 blt      #0x4765d4
00476588 add      r7, r7, #4
0047658c add      r6, sp, #4
00476590 mov      r1, r6
00476594 mov      r0, r7
00476598 bl       #0x476058 ; _ZNSt3mapIi12AnimationSetSt4lessIiESaISt4pairIKiS0_EEEixIiEERS0_RKT_
0047659c mov      sl, r0
004765a0 bl       #0x364ca0 ; _ZN12AnimationSet13CreateAnimSetEv
004765a4 ldr      r3, [pc, #0xc8]
004765a8 ldr      r3, [r4, r3]
004765ac ldrb     r3, [r3]
004765b0 cmp      r3, #0
004765b4 movne    r3, #1
004765b8 strbne   r3, [sl, #0x3c]
004765bc mov      r0, r7
004765c0 mov      r1, r6
004765c4 bl       #0x476058 ; _ZNSt3mapIi12AnimationSetSt4lessIiESaISt4pairIKiS0_EEEixIiEERS0_RKT_
004765c8 ldrb     r3, [r0, #0x3c]
004765cc cmp      r3, #0
004765d0 beq      #0x4765f0
004765d4 ldr      r3, [r4, r5]
004765d8 ldr      r2, [sp, #0x24]
004765dc ldr      r3, [r3]
004765e0 cmp      r2, r3
004765e4 bne      #0x476668
004765e8 add      sp, sp, #0x2c
004765ec pop      {r4, r5, r6, r7, r8, sl, pc}
004765f0 mov      r1, r8
004765f4 bl       #0x3659ec ; _ZN12AnimationSet13LoadAnimationEi
004765f8 ldr      r3, [pc, #0x78]
004765fc add      r6, sp, #0xc
00476600 ldr      r7, [r4, r3]
00476604 mov      r0, r7
00476608 bl       #0x337888 ; _ZN13DebugSwitches4loadEv
0047660c ldr      r1, [pc, #0x68]
00476610 mov      r0, r6
00476614 str      r6, [sp, #0x1c]
00476618 add      r1, pc, r1
0047661c add      r2, r1, #0x17
00476620 str      r6, [sp, #0x20]
00476624 bl       #0x3116e8 ; _ZNSs19_M_range_initializeEPKcS0_
00476628 mov      r0, r7
0047662c mov      r1, r6
00476630 bl       #0x337a88 ; _ZN13DebugSwitches9GetSwitchERKSs
00476634 ldr      r0, [sp, #0x20]
00476638 cmp      r0, r6
0047663c beq      #0x4765d4
00476640 cmp      r0, #0
00476644 beq      #0x4765d4
00476648 ldr      r1, [sp, #0xc]
0047664c rsb      r1, r0, r1
00476650 cmp      r1, #0x80
00476654 bhi      #0x476660
00476658 bl       #0x708f00 ; ___ZNSt12__node_alloc13_M_deallocateEPvj_veneer
0047665c b        #0x4765d4
00476660 bl       #0x310440 ; _Z10CustomFreePv
00476664 b        #0x4765d4
00476668 bl       #0x30e310
0047666c subseq   lr, r1, r4, asr #10
00476670 andeq    r4, r0, ip, lsr #1
00476674 andeq    r4, r0, r8, lsr #9
00476678 andeq    r0, r0, r4, lsl #17
0047667c subeq    r7, r5, r0, lsr r2

_ZNK10CameraBase18GetCameraLookAtVecEv
0040e8a8 push     {r4, lr}
0040e8ac ldr      r2, [r1, #8]
0040e8b0 ldr      r3, [pc, #0x68]
0040e8b4 mov      r4, r0
0040e8b8 cmp      r2, #0
0040e8bc add      r3, pc, r3
0040e8c0 beq      #0x40e8f8
0040e8c4 ldr      r3, [r2]
0040e8c8 mov      r0, r2
0040e8cc mov      lr, pc
0040e8d0 ldr      pc, [r3, #0x38]
0040e8d4 add      r3, r0, #0x10
0040e8d8 ldr      r1, [r3, #8]
0040e8dc ldr      r2, [r0, #0x10]
0040e8e0 ldr      r3, [r3, #4]
0040e8e4 mov      r0, r4
0040e8e8 str      r1, [r4, #8]
0040e8ec str      r2, [r4]
0040e8f0 str      r3, [r4, #4]
0040e8f4 pop      {r4, pc}
0040e8f8 ldr      r2, [pc, #0x24]
0040e8fc ldr      r3, [r3, r2]
0040e900 ldr      r2, [r3]
0040e904 str      r2, [r0]
0040e908 ldr      r2, [r3, #4]
0040e90c str      r2, [r0, #4]
0040e910 ldr      r3, [r3, #8]
0040e914 str      r3, [r0, #8]
0040e918 mov      r0, r4
0040e91c pop      {r4, pc}
0040e920 ldrsbeq  r6, [r8], #-0x14
0040e924 andeq    r3, r0, ip, lsr #30

_ZN17AnimSetController8PlayClipEjbij
00474ff0 cmn      r1, #1
00474ff4 push     {r4, r5, r6, r7, r8, sb, sl, lr}
00474ff8 mov      r4, r1
00474ffc mov      r8, r2
00475000 mov      r7, r0
00475004 beq      #0x4750bc
00475008 ldr      r1, [sp, #0x20]
0047500c bl       #0x4748b8 ; _ZN14AnimController7GetAnimEj
00475010 subs     r5, r0, #0
00475014 beq      #0x4750c4
00475018 ldr      r3, [r5]
0047501c mov      lr, pc
00475020 ldr      pc, [r3, #0x44]
00475024 mov      r6, r0
00475028 mov      r0, r5
0047502c bl       #0x65f114 ; _ZNK6glitch7collada21CSceneNodeAnimatorSet19getCurrentAnimationEv
00475030 mov      sl, r0
00475034 mov      r0, r5
00475038 bl       #0x369160 ; _Z13GetApplicatorPN6glitch5scene18ISceneNodeAnimatorE
0047503c mov      r1, r4
00475040 mov      sb, r0
00475044 mov      r0, r5
00475048 bl       #0x3674ac ; _ZN11AnimatorSet19SetCurrentAnimationEi
0047504c cmn      r0, #1
00475050 mov      r4, r0
00475054 beq      #0x4750bc
00475058 ldr      r3, [r6, #0x34]
0047505c cmp      r3, #0
00475060 beq      #0x475078
00475064 mov      r0, r5
00475068 ldr      r3, [r5]
0047506c ldr      r1, [r7, #0xc]
00475070 mov      lr, pc
00475074 ldr      pc, [r3, #0x30]
00475078 cmp      sl, r4
0047507c beq      #0x4750d8
00475080 mov      r1, r8
00475084 mov      r0, r6
00475088 ldr      r3, [r6]
0047508c mov      lr, pc
00475090 ldr      pc, [r3, #0x40]
00475094 mov      r0, r6
00475098 mov      r1, #0x3f800000
0047509c ldr      r3, [r6]
004750a0 mov      lr, pc
004750a4 ldr      pc, [r3, #0x48]
004750a8 ldr      r0, [r7, #4]
004750ac ldrb     r1, [r7, #0x10]
004750b0 bl       #0x35d624 ; _ZN13RootSceneNode7NewAnimEb
004750b4 mov      r0, #1
004750b8 pop      {r4, r5, r6, r7, r8, sb, sl, pc}
004750bc mov      r0, #0
004750c0 pop      {r4, r5, r6, r7, r8, sb, sl, pc}
004750c4 bl       #0x65f114 ; _ZNK6glitch7collada21CSceneNodeAnimatorSet19getCurrentAnimationEv
004750c8 mov      r0, r5
004750cc bl       #0x369160 ; _Z13GetApplicatorPN6glitch5scene18ISceneNodeAnimatorE
004750d0 mov      r0, r5
004750d4 pop      {r4, r5, r6, r7, r8, sb, sl, pc}
004750d8 ldr      r3, [r6]
004750dc mov      r0, r6
004750e0 mov      lr, pc
004750e4 ldr      pc, [r3, #0x44]
004750e8 cmp      r0, #0
004750ec bne      #0x475080
004750f0 cmp      sb, #0
004750f4 ldr      r3, [r6]
004750f8 ldr      r1, [r6, #0x10]
004750fc ldrne    sb, [sb, #0x10]
00475100 ldr      r3, [r3, #0xc]
00475104 mov      r0, r6
00475108 add      r1, sb, r1
0047510c blx      r3
00475110 b        #0x475080

_ZN17AnimSetController8PlayClipEPKcbij
00474f68 mov      r0, #0
00474f6c bx       lr

_ZN12CameraTarget17GetTargetPositionEv
00411a74 ldr      r0, [r0, #0xc]
00411a78 b        #0x3943b8 ; _ZNK10GameObject23GetCameraAnchorPositionEv

_ZN7Structs15SetCameraTargetD0Ev
004d3784 push     {r4, lr}
004d3788 mov      r4, r0
004d378c bl       #0x4d373c ; _ZN7Structs15SetCameraTargetD1Ev
004d3790 mov      r0, r4
004d3794 bl       #0x310440 ; _Z10CustomFreePv
004d3798 mov      r0, r4
004d379c pop      {r4, pc}

_Z27GetNewScriptCmdDataInstanceIN7Structs15SetCameraTargetEEPNS0_9ScriptCmdEv
004574dc push     {r4, lr}
004574e0 mov      r1, #0
004574e4 mov      r0, #0x18
004574e8 bl       #0x310570 ; _Znwj15MemoryHintState
004574ec ldr      r4, [pc, #0x1c]
004574f0 ldr      r3, [pc, #0x1c]
004574f4 mov      r1, #0
004574f8 add      r4, pc, r4
004574fc ldr      r3, [r4, r3]
00457500 str      r1, [r0, #0x10]
00457504 add      r3, r3, #8
00457508 str      r3, [r0]
0045750c pop      {r4, pc}

_ZN12CameraTarget13HandleDampingER7Point3DIfE
0041165c push     {r3, r4, r5, r6, r7, r8, sb, sl, fp, lr}
00411660 ldrb     r3, [r0, #0x25]
00411664 ldr      r5, [pc, #0x148]
00411668 mov      r4, r0
0041166c cmp      r3, #0
00411670 mov      r6, r1
00411674 add      r5, pc, r5
00411678 beq      #0x4117ac
0041167c ldr      r7, [r0, #4]
00411680 cmp      r7, #0
00411684 beq      #0x4117ac
00411688 ldr      r1, [r1]
0041168c ldr      r0, [r0, #0x2c]
00411690 bl       #0x30eba4
00411694 ldr      r1, [r6, #4]
00411698 mov      sl, r0
0041169c ldr      r0, [r4, #0x30]
004116a0 bl       #0x30eba4
004116a4 ldr      r1, [r6, #8]
004116a8 mov      sb, r0
004116ac ldr      r0, [r4, #0x34]
004116b0 bl       #0x30eba4
004116b4 ldr      r3, [r7]
004116b8 mov      r8, r0
004116bc mov      r0, r7
004116c0 mov      lr, pc
004116c4 ldr      pc, [r3, #0xa0]
004116c8 ldr      r7, [r4, #0x28]
004116cc mov      r3, r0
004116d0 ldr      r1, [r0]
004116d4 mov      r0, sl
004116d8 ldr      fp, [r3, #4]
004116dc ldr      sl, [r3, #8]
004116e0 bl       #0x30e3ac
004116e4 mov      r1, r7
004116e8 bl       #0x30ed6c
004116ec mov      r1, fp
004116f0 str      r0, [r4, #0x2c]
004116f4 mov      r0, sb
004116f8 bl       #0x30e3ac
004116fc mov      r1, r7
00411700 bl       #0x30ed6c
00411704 mov      r1, sl
00411708 str      r0, [r4, #0x30]
0041170c mov      r0, r8
00411710 bl       #0x30e3ac
00411714 mov      r1, r7
00411718 bl       #0x30ed6c
0041171c ldr      r3, [r4, #4]
00411720 str      r0, [r4, #0x34]
00411724 mov      r0, r3
00411728 ldr      r3, [r3]
0041172c mov      lr, pc
00411730 ldr      pc, [r3, #0xa0]
00411734 ldr      r3, [pc, #0x7c]
00411738 mov      r7, r0
0041173c ldr      r0, [r5, r3]
00411740 bl       #0x31f66c ; _ZN11Application5GetDtEv
00411744 bl       #0x30e2e0
00411748 movw     r1, #0x126f
0041174c movt     r1, #0x3a83
00411750 bl       #0x30ed6c
00411754 ldr      r1, [r4, #0x30]
00411758 mov      r5, r0
0041175c bl       #0x30ed6c
00411760 ldr      r1, [r7, #4]
00411764 bl       #0x30eba4
00411768 ldr      r1, [r4, #0x34]
0041176c mov      r8, r0
00411770 mov      r0, r5
00411774 bl       #0x30ed6c
00411778 ldr      r1, [r7, #8]
0041177c bl       #0x30eba4
00411780 ldr      r1, [r4, #0x2c]
00411784 mov      sl, r0
00411788 mov      r0, r5
0041178c bl       #0x30ed6c
00411790 ldr      r1, [r7]
00411794 bl       #0x30eba4
00411798 str      sl, [r6, #8]
0041179c str      r0, [r6]
004117a0 str      r8, [r6, #4]
004117a4 mov      r0, #1
004117a8 pop      {r3, r4, r5, r6, r7, r8, sb, sl, fp, pc}
004117ac mov      r0, #0
004117b0 pop      {r3, r4, r5, r6, r7, r8, sb, sl, fp, pc}
004117b4 subseq   r3, r8, ip, lsl r4
004117b8 strdeq   r3, r4, [r0], -r4

_ZN10CameraBaseD2Ev
0040e780 ldr      r3, [pc, #0x80]
0040e784 ldr      r2, [pc, #0x80]
0040e788 ldr      r1, [pc, #0x80]
0040e78c add      r3, pc, r3
0040e790 push     {r4, lr}
0040e794 ldr      r2, [r3, r2]
0040e798 ldr      r1, [r3, r1]
0040e79c mov      r4, r0
0040e7a0 add      r2, r2, #8
0040e7a4 str      r2, [r0]
0040e7a8 ldr      r2, [r1]
0040e7ac cmp      r2, r0
0040e7b0 moveq    r3, #0
0040e7b4 streq    r3, [r1]
0040e7b8 ldr      r3, [r0, #4]
0040e7bc cmp      r3, #0
0040e7c0 beq      #0x40e7dc
0040e7c4 ldr      r2, [r3]
0040e7c8 ldr      r0, [r2, #-0xc]
0040e7cc add      r0, r3, r0
0040e7d0 bl       #0x31d584 ; _ZNK6glitch17IReferenceCounted4dropEv
0040e7d4 mov      r3, #0
0040e7d8 str      r3, [r4, #4]
0040e7dc ldr      r3, [r4, #8]
0040e7e0 cmp      r3, #0
0040e7e4 beq      #0x40e800
0040e7e8 ldr      r2, [r3]
0040e7ec ldr      r0, [r2, #-0xc]
0040e7f0 add      r0, r3, r0
0040e7f4 bl       #0x31d584 ; _ZNK6glitch17IReferenceCounted4dropEv
0040e7f8 mov      r3, #0
0040e7fc str      r3, [r4, #8]
0040e800 mov      r0, r4
0040e804 pop      {r4, pc}
0040e808 subseq   r6, r8, r4, lsl #6
0040e80c andeq    r2, r0, ip, ror #3
0040e810 strheq   r4, [r0], -r0

_ZN17AnimSetControllerD2Ev
00475180 ldr      r3, [pc, #0x24]
00475184 ldr      r2, [pc, #0x24]
00475188 push     {r4, lr}
0047518c add      r3, pc, r3
00475190 ldr      r2, [r3, r2]
00475194 mov      r4, r0
00475198 add      r2, r2, #8
0047519c str      r2, [r0]
004751a0 bl       #0x474684 ; _ZN14AnimControllerD2Ev
004751a4 mov      r0, r4
004751a8 pop      {r4, pc}
004751ac subseq   pc, r1, r4, lsl #18
004751b0 andeq    r3, r0, ip, lsr #10

_ZN11CameraLevelD2Ev
0040ff04 push     {r4, lr}
0040ff08 ldr      r3, [pc, #0x58]
0040ff0c ldr      r2, [pc, #0x58]
0040ff10 ldr      r1, [r0, #0x44]
0040ff14 add      r3, pc, r3
0040ff18 ldr      r2, [r3, r2]
0040ff1c cmp      r1, #0
0040ff20 mov      r4, r0
0040ff24 add      r2, r2, #8
0040ff28 str      r2, [r0]
0040ff2c beq      #0x40ff48
0040ff30 ldr      r3, [r1]
0040ff34 mov      r0, r1
0040ff38 mov      lr, pc
0040ff3c ldr      pc, [r3, #4]
0040ff40 mov      r3, #0
0040ff44 str      r3, [r4, #0x44]
0040ff48 add      r0, r4, #0x64
0040ff4c bl       #0x3139ac ; _ZNSt4priv12_String_baseIcSaIcEE19_M_deallocate_blockEv
0040ff50 add      r0, r4, #0x4c
0040ff54 bl       #0x3139ac ; _ZNSt4priv12_String_baseIcSaIcEE19_M_deallocate_blockEv
0040ff58 mov      r0, r4
0040ff5c bl       #0x411c0c ; _ZN12CameraTargetD2Ev
0040ff60 mov      r0, r4
0040ff64 pop      {r4, pc}
0040ff68 subseq   r4, r8, ip, ror fp
0040ff6c andeq    r4, r0, r8, lsr #4

_ZN10CameraBase14GetScreenCoordERK7Point3DIfER7Point2DIfE
0040f714 ldr      r3, [pc, #0xf8]
0040f718 ldr      r2, [pc, #0xf8]
0040f71c push     {r4, r5, r6, r7, r8, sl, lr}
0040f720 add      r3, pc, r3
0040f724 ldr      r8, [r3, r2]
0040f728 sub      sp, sp, #0x5c
0040f72c mov      r6, r0
0040f730 ldr      r2, [r8, #0x10]
0040f734 mov      r7, r1
0040f738 mov      r1, #0
0040f73c ldr      r3, [r2, #0x10]
0040f740 add      r5, sp, #4
0040f744 mov      r4, #0x3f800000
0040f748 mov      r0, r3
0040f74c ldr      r3, [r3]
0040f750 mov      lr, pc
0040f754 ldr      pc, [r3, #0x70]
0040f758 ldr      r3, [r8, #0x10]
0040f75c mov      sl, r0
0040f760 mov      r1, #2
0040f764 ldr      r3, [r3, #0x10]
0040f768 mov      r0, r3
0040f76c ldr      r3, [r3]
0040f770 mov      lr, pc
0040f774 ldr      pc, [r3, #0x70]
0040f778 mov      r1, #0
0040f77c mov      r8, r0
0040f780 mov      r2, #0x40
0040f784 mov      r0, r5
0040f788 bl       #0x30e460
0040f78c ldr      ip, [r6]
0040f790 ldr      r3, [r6, #4]
0040f794 ldr      lr, [r6, #8]
0040f798 mov      r2, sl
0040f79c mov      r1, r8
0040f7a0 mov      r0, r5
0040f7a4 mov      r6, #1
0040f7a8 str      ip, [sp, #0x48]
0040f7ac str      r3, [sp, #0x4c]
0040f7b0 str      lr, [sp, #0x50]
0040f7b4 str      r4, [sp, #4]
0040f7b8 str      r4, [sp, #0x18]
0040f7bc str      r4, [sp, #0x2c]
0040f7c0 str      r4, [sp, #0x40]
0040f7c4 str      r4, [sp, #0x54]
0040f7c8 strb     r6, [sp, #0x44]
0040f7cc bl       #0x40ea54 ; _ZN6glitch4core6detail12CMatrix4BaseIfE20setbyproduct_nocheckERKS3_S5_
0040f7d0 mov      r0, r5
0040f7d4 add      r1, sp, #0x48
0040f7d8 bl       #0x312bf8 ; _ZNK6glitch4core8CMatrix4IfE21multiplyWith1x4MatrixEPf
0040f7dc ldr      r1, [sp, #0x54]
0040f7e0 mov      r0, r4
0040f7e4 bl       #0x30ec94
0040f7e8 ldr      r1, [sp, #0x48]
0040f7ec mov      r5, r0
0040f7f0 bl       #0x30ed6c
0040f7f4 ldr      r1, [sp, #0x4c]
0040f7f8 mov      r4, r0
0040f7fc mov      r0, r5
0040f800 bl       #0x30ed6c
0040f804 str      r4, [r7]
0040f808 str      r0, [r7, #4]
0040f80c add      sp, sp, #0x5c
0040f810 pop      {r4, r5, r6, r7, r8, sl, pc}
0040f814 subseq   r5, r8, r0, ror r3
0040f818 strdeq   r3, r4, [r0], -r4

_ZN10CameraBase11DeactivatedEv
0040e71c bx       lr

_ZN11CameraLevelD1Ev
0040fe7c push     {r4, lr}
0040fe80 ldr      r3, [pc, #0x58]
0040fe84 ldr      r2, [pc, #0x58]
0040fe88 ldr      r1, [r0, #0x44]
0040fe8c add      r3, pc, r3
0040fe90 ldr      r2, [r3, r2]
0040fe94 cmp      r1, #0
0040fe98 mov      r4, r0
0040fe9c add      r2, r2, #8
0040fea0 str      r2, [r0]
0040fea4 beq      #0x40fec0
0040fea8 ldr      r3, [r1]
0040feac mov      r0, r1
0040feb0 mov      lr, pc
0040feb4 ldr      pc, [r3, #4]
0040feb8 mov      r3, #0
0040febc str      r3, [r4, #0x44]
0040fec0 add      r0, r4, #0x64
0040fec4 bl       #0x3139ac ; _ZNSt4priv12_String_baseIcSaIcEE19_M_deallocate_blockEv
0040fec8 add      r0, r4, #0x4c
0040fecc bl       #0x3139ac ; _ZNSt4priv12_String_baseIcSaIcEE19_M_deallocate_blockEv
0040fed0 mov      r0, r4
0040fed4 bl       #0x411c0c ; _ZN12CameraTargetD2Ev
0040fed8 mov      r0, r4
0040fedc pop      {r4, pc}
0040fee0 subseq   r4, r8, r4, lsl #24
0040fee4 andeq    r4, r0, r8, lsr #4

_ZNK14AnimSetManager6ExistsEi
00475404 ldr      r3, [r0, #8]
00475408 add      r0, r0, #4
0047540c cmp      r3, #0
00475410 beq      #0x475458
00475414 mov      ip, r0
00475418 b        #0x475420
0047541c mov      r3, r2
00475420 ldr      r2, [r3, #0x10]
00475424 cmp      r1, r2
00475428 ldrgt    r2, [r3, #0xc]
0047542c ldrle    r2, [r3, #8]
00475430 movgt    r3, ip
00475434 mov      ip, r3
00475438 cmp      r2, #0
0047543c bne      #0x47541c
00475440 cmp      r0, r3
00475444 beq      #0x475458
00475448 ldr      r3, [r3, #0x10]
0047544c cmp      r1, r3
00475450 movge    r0, #1
00475454 bxge     lr
00475458 mov      r0, #0
0047545c bx       lr

_ZN11CameraLevel4LoadEPKciS1_
0041068c push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
00410690 ldr      r5, [pc, #0x278]
00410694 ldr      r8, [pc, #0x278]
00410698 mov      r6, r1
0041069c add      r5, pc, r5
004106a0 ldr      r1, [r5, r8]
004106a4 sub      sp, sp, #0x2c
004106a8 mov      r4, r0
004106ac ldr      r1, [r1]
004106b0 mov      r0, r6
004106b4 mov      r7, r3
004106b8 mov      sb, r2
004106bc str      r1, [sp, #0x24]
004106c0 bl       #0x30de54
004106c4 add      sl, r4, #0x4c
004106c8 add      r2, r6, r0
004106cc mov      r1, r6
004106d0 mov      r0, sl
004106d4 bl       #0x3109e0 ; _ZNSs9_M_assignEPKcS0_
004106d8 str      sb, [r4, #0x80]
004106dc mov      r0, r7
004106e0 bl       #0x30de54
004106e4 add      r6, sp, #0xc
004106e8 add      r2, r7, r0
004106ec mov      r1, r7
004106f0 add      r0, r4, #0x64
004106f4 bl       #0x3109e0 ; _ZNSs9_M_assignEPKcS0_
004106f8 mov      r0, r6
004106fc mov      r1, #0x10
00410700 str      r6, [sp, #0x1c]
00410704 str      r6, [sp, #0x20]
00410708 bl       #0x31167c ; _ZNSt4priv12_String_baseIcSaIcEE17_M_allocate_blockEj
0041070c ldr      r3, [sp, #0x1c]
00410710 mov      fp, #0
00410714 mov      r1, fp
00410718 strb     fp, [r3]
0041071c mov      r0, #0xac
00410720 bl       #0x310570 ; _Znwj15MemoryHintState
00410724 mov      r1, fp
00410728 mov      sb, r0
0041072c mov      r2, sl
00410730 mov      r3, r6
00410734 bl       #0x472a0c ; _ZN12VisualObjectC1EP10GameObjectRKSsS3_
00410738 mov      r0, r6
0041073c str      sb, [r4, #0x44]
00410740 bl       #0x3139ac ; _ZNSt4priv12_String_baseIcSaIcEE19_M_deallocate_blockEv
00410744 ldr      r0, [r4, #0x44]
00410748 cmp      r0, fp
0041074c beq      #0x4108f0
00410750 ldr      r3, [r0, #8]
00410754 cmp      r3, fp
00410758 beq      #0x4108f0
0041075c mov      r1, r7
00410760 bl       #0x470a18 ; _ZNK12VisualObject15GetSpecificNodeEPKc
00410764 subs     r6, r0, #0
00410768 streq    r6, [r4, #8]
0041076c beq      #0x4107c0
00410770 movw     r1, #0x6164
00410774 movt     r1, #0x6365
00410778 bl       #0x596f6c ; _ZN6glitch5scene10ISceneNode20getSceneNodeFromTypeENS0_17E_SCENE_NODE_TYPEE
0041077c cmp      r0, fp
00410780 mov      r3, r0
00410784 str      r0, [r4, #8]
00410788 beq      #0x4107ac
0041078c ldr      r2, [r0]
00410790 mov      r0, r4
00410794 ldr      r2, [r2, #-0xc]
00410798 add      r3, r3, r2
0041079c ldr      r2, [r3, #4]
004107a0 add      r2, r2, #1
004107a4 str      r2, [r3, #4]
004107a8 bl       #0x40fd18 ; _ZN11CameraLevel30CalculateDefaultTargetDistanceEv
004107ac ldr      r1, [pc, #0x164]
004107b0 mov      r0, r6
004107b4 add      r1, pc, r1
004107b8 bl       #0x5984f4 ; _ZN6glitch5scene10ISceneNode20getSceneNodeFromNameEPKc
004107bc mov      r6, r0
004107c0 ldr      r3, [r4, #0x44]
004107c4 str      r6, [r4, #0x48]
004107c8 ldr      r3, [r3, #8]
004107cc cmp      r3, #0
004107d0 str      r3, [r4, #4]
004107d4 beq      #0x4107f0
004107d8 ldr      r2, [r3]
004107dc ldr      r2, [r2, #-0xc]
004107e0 add      r3, r3, r2
004107e4 ldr      r2, [r3, #4]
004107e8 add      r2, r2, #1
004107ec str      r2, [r3, #4]
004107f0 ldr      r3, [pc, #0x124]
004107f4 mov      r7, #0x1c
004107f8 ldr      sl, [r5, r3]
004107fc mov      r0, sl
00410800 bl       #0x476464 ; _ZN14AnimSetManager6CreateEv
00410804 ldr      r3, [pc, #0x114]
00410808 str      r0, [r4, #0x7c]
0041080c ldr      r2, [r4, #0x80]
00410810 ldr      r3, [r5, r3]
00410814 mov      r1, r0
00410818 mov      r0, sl
0041081c ldr      r3, [r3]
00410820 mla      r7, r7, r2, r3
00410824 ldr      r2, [r7, #0x18]
00410828 bl       #0x476398 ; _ZN14AnimSetManager15AddTemplateAnimEii
0041082c mov      r0, sl
00410830 ldr      r1, [r4, #0x7c]
00410834 ldr      r2, [r7, #0x10]
00410838 bl       #0x47653c ; _ZN14AnimSetManager7AddAnimEii
0041083c mov      r0, sl
00410840 ldr      r1, [r4, #0x7c]
00410844 ldr      r2, [r7, #0x14]
00410848 bl       #0x47653c ; _ZN14AnimSetManager7AddAnimEii
0041084c mov      r0, sl
00410850 ldr      r1, [r4, #0x7c]
00410854 ldr      r2, [r7, #0xc]
00410858 bl       #0x47653c ; _ZN14AnimSetManager7AddAnimEii
0041085c ldr      r3, [r7, #4]
00410860 cmp      r3, #0
00410864 beq      #0x410890
00410868 mov      r6, #0
0041086c ldr      r3, [r7, #8]
00410870 mov      r0, sl
00410874 ldr      r1, [r4, #0x7c]
00410878 ldr      r2, [r3, r6, lsl #2]
0041087c bl       #0x47653c ; _ZN14AnimSetManager7AddAnimEii
00410880 ldr      r3, [r7, #4]
00410884 add      r6, r6, #1
00410888 cmp      r3, r6
0041088c bhi      #0x41086c
00410890 ldr      r6, [r4, #0x44]
00410894 mov      r1, #0
00410898 mov      r0, #0x14
0041089c ldr      sl, [r6, #8]
004108a0 bl       #0x310570 ; _Znwj15MemoryHintState
004108a4 ldr      r2, [r4, #0x7c]
004108a8 mov      r7, r0
004108ac mov      r1, sl
004108b0 bl       #0x4751b4 ; _ZN17AnimSetControllerC1EP13RootSceneNodei
004108b4 mov      r0, r6
004108b8 mov      r1, r7
004108bc bl       #0x470a84 ; _ZN12VisualObject17SetAnimControllerEP14AnimController
004108c0 ldr      r3, [r4, #0x44]
004108c4 mov      lr, #0
004108c8 mov      r2, r4
004108cc ldr      ip, [r3, #0x38]
004108d0 ldr      r3, [pc, #0x4c]
004108d4 mov      r0, ip
004108d8 ldr      r1, [r5, r3]
004108dc ldr      ip, [ip]
004108e0 mov      r3, lr
004108e4 str      lr, [sp]
004108e8 mov      lr, pc
004108ec ldr      pc, [ip, #0x2c]
004108f0 ldr      r3, [r5, r8]
004108f4 ldr      r2, [sp, #0x24]
004108f8 ldr      r3, [r3]
004108fc cmp      r2, r3
00410900 bne      #0x41090c
00410904 add      sp, sp, #0x2c
00410908 pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
0041090c bl       #0x30e310
00410910 ldrsheq  r4, [r8], #-0x34
00410914 andeq    r4, r0, ip, lsr #1
00410918 subeq    r7, fp, r4, ror #14
0041091c andeq    r4, r0, r8, lsr r8
00410920 ldrdeq   r3, r4, [r0], -r4
00410924 andeq    r3, r0, r8, lsr #18

_ZN14AnimSetManager9GetClipIdEii
0047559c push     {r4, r5, r6, lr}
004755a0 mov      r6, r2
004755a4 mov      r5, r0
004755a8 mov      r4, r1
004755ac bl       #0x475404 ; _ZNK14AnimSetManager6ExistsEi
004755b0 cmp      r0, #0
004755b4 beq      #0x47561c
004755b8 ldr      r3, [r5, #8]
004755bc add      r5, r5, #4
004755c0 cmp      r3, #0
004755c4 beq      #0x475608
004755c8 mov      r1, r5
004755cc b        #0x4755d4
004755d0 mov      r3, r2
004755d4 ldr      r2, [r3, #0x10]
004755d8 cmp      r4, r2
004755dc ldrgt    r2, [r3, #0xc]
004755e0 ldrle    r2, [r3, #8]
004755e4 movgt    r3, r1
004755e8 mov      r1, r3
004755ec cmp      r2, #0
004755f0 bne      #0x4755d0
004755f4 cmp      r5, r3
004755f8 beq      #0x475608
004755fc ldr      r2, [r3, #0x10]
00475600 cmp      r4, r2
00475604 movge    r5, r3
00475608 add      r0, r5, #0x14
0047560c mov      r1, r6
00475610 bl       #0x3660f4 ; _ZN12AnimationSet12GetAnimationEi
00475614 ldr      r0, [r0, #0x20]
00475618 pop      {r4, r5, r6, pc}
0047561c mvn      r0, #0
00475620 pop      {r4, r5, r6, pc}

_ZN14AnimSetManager11DBG_SetNameEiPKc
00475460 ldr      r3, [r0, #8]
00475464 cmp      r3, #0
00475468 bxeq     lr
0047546c ldr      r2, [r3, #0x10]
00475470 cmp      r1, r2
00475474 ldrle    r3, [r3, #8]
00475478 ldrgt    r3, [r3, #0xc]
0047547c cmp      r3, #0
00475480 bne      #0x47546c
00475484 bx       lr

_ZN10CameraBaseD1Ev
0040e814 ldr      r3, [pc, #0x80]
0040e818 ldr      r2, [pc, #0x80]
0040e81c ldr      r1, [pc, #0x80]
0040e820 add      r3, pc, r3
0040e824 push     {r4, lr}
0040e828 ldr      r2, [r3, r2]
0040e82c ldr      r1, [r3, r1]
0040e830 mov      r4, r0
0040e834 add      r2, r2, #8
0040e838 str      r2, [r0]
0040e83c ldr      r2, [r1]
0040e840 cmp      r2, r0
0040e844 moveq    r3, #0
0040e848 streq    r3, [r1]
0040e84c ldr      r3, [r0, #4]
0040e850 cmp      r3, #0
0040e854 beq      #0x40e870
0040e858 ldr      r2, [r3]
0040e85c ldr      r0, [r2, #-0xc]
0040e860 add      r0, r3, r0
0040e864 bl       #0x31d584 ; _ZNK6glitch17IReferenceCounted4dropEv
0040e868 mov      r3, #0
0040e86c str      r3, [r4, #4]
0040e870 ldr      r3, [r4, #8]
0040e874 cmp      r3, #0
0040e878 beq      #0x40e894
0040e87c ldr      r2, [r3]
0040e880 ldr      r0, [r2, #-0xc]
0040e884 add      r0, r3, r0
0040e888 bl       #0x31d584 ; _ZNK6glitch17IReferenceCounted4dropEv
0040e88c mov      r3, #0
0040e890 str      r3, [r4, #8]
0040e894 mov      r0, r4
0040e898 pop      {r4, pc}
0040e89c subseq   r6, r8, r0, ror r2
0040e8a0 andeq    r2, r0, ip, ror #3
0040e8a4 strheq   r4, [r0], -r0

_ZN14AnimSetManager11GetAnimatorEi
004762c4 push     {r4, r5, r6, lr}
004762c8 sub      sp, sp, #0x10
004762cc str      r1, [sp, #4]
004762d0 mov      r5, r0
004762d4 bl       #0x475404 ; _ZNK14AnimSetManager6ExistsEi
004762d8 subs     r4, r0, #0
004762dc bne      #0x4762ec
004762e0 mov      r0, r4
004762e4 add      sp, sp, #0x10
004762e8 pop      {r4, r5, r6, pc}
004762ec add      r0, r5, #4
004762f0 add      r1, sp, #4
004762f4 bl       #0x476058 ; _ZNSt3mapIi12AnimationSetSt4lessIiESaISt4pairIKiS0_EEEixIiEERS0_RKT_
004762f8 ldr      r3, [r0, #0x20]
004762fc mov      r5, r0
00476300 ldrb     r2, [r3, #0x70]
00476304 cmp      r2, #0
00476308 bne      #0x476384
0047630c add      r6, sp, #0x10
00476310 str      r5, [r6, #-4]!
00476314 ldr      r3, [r5, #4]
00476318 mov      r1, #0
0047631c mov      r0, #0xa4
00476320 add      r3, r3, #1
00476324 str      r3, [r5, #4]
00476328 bl       #0x310570 ; _Znwj15MemoryHintState
0047632c mov      r1, r6
00476330 mov      r4, r0
00476334 bl       #0x3676b8 ; _ZN11AnimatorSetC1ERKN5boost13intrusive_ptrI12AnimationSetEE
00476338 ldr      r0, [sp, #0xc]
0047633c cmp      r0, #0
00476340 beq      #0x476348
00476344 bl       #0x31d584 ; _ZNK6glitch17IReferenceCounted4dropEv
00476348 ldr      r3, [r4]
0047634c mov      r0, r4
00476350 mov      lr, pc
00476354 ldr      pc, [r3, #0x44]
00476358 mov      r6, r0
0047635c mov      r0, r5
00476360 bl       #0x3649bc ; _ZN12AnimationSet18CalculateCacheSizeEv
00476364 cmp      r6, #0
00476368 beq      #0x4762e0
0047636c mov      r0, r6
00476370 ldr      r3, [r6]
00476374 mov      r1, #0
00476378 mov      lr, pc
0047637c ldr      pc, [r3, #0x40]
00476380 b        #0x4762e0
00476384 mov      r0, r3
00476388 ldr      r3, [r3]
0047638c mov      lr, pc
00476390 ldr      pc, [r3, #0x38]
00476394 b        #0x47630c

_ZN12CameraTarget14HandleGhostCamER7Point3DIfE
004115d8 push     {r4, r5, r6, lr}
004115dc mov      r4, r1
004115e0 mov      r5, r0
004115e4 ldr      r1, [r0, #0x38]
004115e8 ldr      r0, [r4]
004115ec bl       #0x30eba4
004115f0 str      r0, [r4]
004115f4 ldr      r1, [r5, #0x3c]
004115f8 ldr      r0, [r4, #4]
004115fc bl       #0x30eba4
00411600 str      r0, [r4, #4]
00411604 ldr      r1, [r5, #0x40]
00411608 ldr      r0, [r4, #8]
0041160c bl       #0x30eba4
00411610 str      r0, [r4, #8]
00411614 mov      r0, #1
00411618 pop      {r4, r5, r6, pc}

_ZN24BlendedAnimSetControllerC2EP13RootSceneNodei
00476b4c push     {r4, r5, r6, r7, r8, sl, lr}
00476b50 mov      r5, r2
00476b54 sub      sp, sp, #0x14
00476b58 mov      r2, #1
00476b5c ldr      r7, [pc, #0x258]
00476b60 mov      r4, r0
00476b64 bl       #0x474e44 ; _ZN14AnimControllerC2EP13RootSceneNodeb
00476b68 ldr      r3, [pc, #0x250]
00476b6c add      r7, pc, r7
00476b70 ldr      r2, [pc, #0x24c]
00476b74 ldr      r3, [r7, r3]
00476b78 mov      r8, #0
00476b7c ldr      r6, [r7, r2]
00476b80 add      r3, r3, #8
00476b84 str      r3, [r4]
00476b88 mov      r3, #1
00476b8c strb     r3, [r4, #0x10]
00476b90 mov      r1, r5
00476b94 str      r5, [r4, #8]
00476b98 mov      r0, r6
00476b9c str      r8, [r4, #0xc]
00476ba0 str      r8, [r4, #0x14]
00476ba4 bl       #0x4762c4 ; _ZN14AnimSetManager11GetAnimatorEi
00476ba8 mov      r5, r0
00476bac ldr      r1, [r4, #8]
00476bb0 mov      r0, r6
00476bb4 bl       #0x4762c4 ; _ZN14AnimSetManager11GetAnimatorEi
00476bb8 subs     r3, r5, r8
00476bbc movne    r3, #1
00476bc0 subs     sl, r0, r8
00476bc4 movne    sl, #1
00476bc8 tst      sl, r3
00476bcc mov      r6, r0
00476bd0 bne      #0x476c10
00476bd4 cmp      r3, #0
00476bd8 beq      #0x476bec
00476bdc ldr      r3, [r5]
00476be0 ldr      r0, [r3, #-0xc]
00476be4 add      r0, r5, r0
00476be8 bl       #0x31d584 ; _ZNK6glitch17IReferenceCounted4dropEv
00476bec cmp      sl, #0
00476bf0 beq      #0x476c04
00476bf4 ldr      r3, [r6]
00476bf8 ldr      r0, [r3, #-0xc]
00476bfc add      r0, r6, r0
00476c00 bl       #0x31d584 ; _ZNK6glitch17IReferenceCounted4dropEv
00476c04 mov      r0, r4
00476c08 add      sp, sp, #0x14
00476c0c pop      {r4, r5, r6, r7, r8, sl, pc}
00476c10 mov      r0, r5
00476c14 bl       #0x65f11c ; _ZNK6glitch7collada21CSceneNodeAnimatorSet17getAnimationCountEv
00476c18 cmp      r0, r8
00476c1c ble      #0x476d48
00476c20 mov      r1, #0
00476c24 mov      r0, #0xd0
00476c28 bl       #0x5341ac ; _ZnwjN6glitch6memory13E_MEMORY_HINTE
00476c2c mov      r7, r0
00476c30 bl       #0x367018 ; _ZN15AnimatorBlenderC1Ev
00476c34 mov      r3, #1
00476c38 str      r5, [sp, #0xc]
00476c3c strb     r3, [r7, #0x24]
00476c40 ldr      r3, [sp, #0xc]
00476c44 add      r8, r7, #0x28
00476c48 ldr      r2, [r3]
00476c4c ldr      r2, [r2, #-0xc]
00476c50 add      r3, r3, r2
00476c54 ldr      r2, [r3, #4]
00476c58 add      r2, r2, #1
00476c5c str      r2, [r3, #4]
00476c60 ldr      r1, [r7, #0x2c]
00476c64 ldr      r3, [r7, #0x30]
00476c68 cmp      r1, r3
00476c6c beq      #0x476d9c
00476c70 ldr      r3, [sp, #0xc]
00476c74 str      r3, [r1]
00476c78 ldr      r3, [r7, #0x2c]
00476c7c add      r3, r3, #4
00476c80 str      r3, [r7, #0x2c]
00476c84 mov      r3, #1
00476c88 str      r6, [sp, #0xc]
00476c8c strb     r3, [r7, #0x24]
00476c90 ldr      r3, [sp, #0xc]
00476c94 ldr      r2, [r3]
00476c98 ldr      r2, [r2, #-0xc]
00476c9c add      r3, r3, r2
00476ca0 ldr      r2, [r3, #4]
00476ca4 add      r2, r2, #1
00476ca8 str      r2, [r3, #4]
00476cac ldr      r1, [r7, #0x2c]
00476cb0 ldr      r3, [r7, #0x30]
00476cb4 cmp      r1, r3
00476cb8 beq      #0x476dac
00476cbc ldr      r3, [sp, #0xc]
00476cc0 str      r3, [r1]
00476cc4 ldr      r3, [r7, #0x2c]
00476cc8 add      r3, r3, #4
00476ccc str      r3, [r7, #0x2c]
00476cd0 mov      r0, r7
00476cd4 ldr      r3, [r7]
00476cd8 mov      r1, #0
00476cdc mov      lr, pc
00476ce0 ldr      pc, [r3, #0x88]
00476ce4 ldr      r3, [r7, #0x34]
00476ce8 mov      r2, #0x3f800000
00476cec mov      r1, r7
00476cf0 str      r2, [r3]
00476cf4 ldr      r3, [r7, #0x34]
00476cf8 mov      r2, #0
00476cfc str      r2, [r3, #4]
00476d00 ldr      r3, [r4, #4]
00476d04 mov      r0, r3
00476d08 ldr      r3, [r3]
00476d0c mov      lr, pc
00476d10 ldr      pc, [r3, #0x6c]
00476d14 ldr      r3, [r5]
00476d18 ldr      r0, [r3, #-0xc]
00476d1c add      r0, r5, r0
00476d20 bl       #0x31d584 ; _ZNK6glitch17IReferenceCounted4dropEv
00476d24 ldr      r3, [r6]
00476d28 ldr      r0, [r3, #-0xc]
00476d2c add      r0, r6, r0
00476d30 bl       #0x31d584 ; _ZNK6glitch17IReferenceCounted4dropEv
00476d34 ldr      r3, [r7]
00476d38 ldr      r0, [r3, #-0xc]
00476d3c add      r0, r7, r0
00476d40 bl       #0x31d584 ; _ZNK6glitch17IReferenceCounted4dropEv
00476d44 b        #0x476c04
00476d48 ldr      r3, [pc, #0x78]
00476d4c ldr      r3, [r7, r3]
00476d50 ldr      r3, [r3]
00476d54 cmp      r3, #2
00476d58 streq    r8, [r8]
00476d5c beq      #0x476c20
00476d60 cmp      r3, #1
00476d64 bne      #0x476c20
00476d68 ldr      r0, [pc, #0x5c]
00476d6c ldr      r1, [pc, #0x5c]
00476d70 ldr      r2, [pc, #0x5c]
00476d74 ldr      r0, [r7, r0]
00476d78 ldr      r3, [pc, #0x58]
00476d7c mov      ip, #0x45
00476d80 add      r1, pc, r1
00476d84 add      r2, pc, r2
00476d88 add      r3, pc, r3
00476d8c add      r0, r0, #0xa8
00476d90 str      ip, [sp]
00476d94 bl       #0x30e004
00476d98 b        #0x476c20
00476d9c mov      r0, r8
00476da0 add      r2, sp, #0xc
00476da4 bl       #0x476a9c ; _ZNSt6vectorIPN6glitch7collada18ISceneNodeAnimatorENS0_4core10SAllocatorIS3_LNS0_6memory13E_MEMORY_HINTE0EEEE18_M_insert_overflowEPS3_RKS3_RKSt11__true_typejb.clone.1
00476da8 b        #0x476c84
00476dac mov      r0, r8
00476db0 add      r2, sp, #0xc
00476db4 bl       #0x476a9c ; _ZNSt6vectorIPN6glitch7collada18ISceneNodeAnimatorENS0_4core10SAllocatorIS3_LNS0_6memory13E_MEMORY_HINTE0EEEE18_M_insert_overflowEPS3_RKS3_RKSt11__true_typejb.clone.1
00476db8 b        #0x476cd0
00476dbc subseq   sp, r1, r4, lsr #30
00476dc0 andeq    r2, r0, r8, lsr #7
00476dc4 andeq    r4, r0, r8, lsr r8
00476dc8 andeq    r3, r0, r0, asr #19
00476dcc andeq    r1, r0, r0, asr #19
00476dd0 subeq    r7, r4, r8, asr r6
00476dd4 ldrdeq   r6, r7, [r5], #-0xac
00476dd8 strdeq   r6, r7, [r5], #-0xa8

_ZN12CameraTargetD2Ev
00411c0c ldr      r3, [pc, #0x24]
00411c10 ldr      r2, [pc, #0x24]
00411c14 push     {r4, lr}
00411c18 add      r3, pc, r3
00411c1c ldr      r2, [r3, r2]
00411c20 mov      r4, r0
00411c24 add      r2, r2, #8
00411c28 str      r2, [r0]
00411c2c bl       #0x40e780 ; _ZN10CameraBaseD2Ev
00411c30 mov      r0, r4
00411c34 pop      {r4, pc}
00411c38 subseq   r2, r8, r8, ror lr
00411c3c ldrdeq   r0, r1, [r0], -r4

_ZN24BlendedAnimSetController12SetCallbacksEPFvPN6glitch5scene19ITimelineControllerEPvES4_PFvRKNS0_7collada15STriggeredEventES4_ES4_
004766fc push     {r4, r5, r6, r7, r8, lr}
00476700 mov      r5, r1
00476704 mov      r1, #0
00476708 mov      r7, r2
0047670c mov      r6, r3
00476710 ldr      r4, [sp, #0x18]
00476714 bl       #0x4748b8 ; _ZN14AnimController7GetAnimEj
00476718 cmp      r0, #0
0047671c beq      #0x476738
00476720 mov      r1, r5
00476724 mov      r2, r7
00476728 mov      r3, r6
0047672c str      r4, [sp, #0x18]
00476730 pop      {r4, r5, r6, r7, r8, lr}
00476734 b        #0x366eb8 ; _ZN15AnimatorBlender12SetCallbacksEPFvPN6glitch5scene19ITimelineControllerEPvES4_PFvRKNS0_7collada15STriggeredEventES4_ES4_
00476738 pop      {r4, r5, r6, r7, r8, pc}

_ZN14AnimSetManagerD0Ev
0047570c push     {r4, lr}
00475710 mov      r4, r0
00475714 bl       #0x4756a8 ; _ZN14AnimSetManagerD1Ev
00475718 mov      r0, r4
0047571c bl       #0x310440 ; _Z10CustomFreePv
00475720 mov      r0, r4
00475724 pop      {r4, pc}

_ZN12CameraTarget13EnableDampingEb
0041161c ldr      r3, [pc, #0x28]
00411620 ldr      r2, [pc, #0x28]
00411624 strb     r1, [r0, #0x25]
00411628 add      r3, pc, r3
0041162c ldr      r2, [r3, r2]
00411630 ldr      r3, [r2]
00411634 str      r3, [r0, #0x2c]
00411638 ldr      r3, [r2, #4]
0041163c str      r3, [r0, #0x30]
00411640 ldr      r3, [r2, #8]
00411644 str      r3, [r0, #0x34]
00411648 bx       lr
0041164c subseq   r3, r8, r8, ror #8
00411650 andeq    r3, r0, ip, lsr #30

_ZN17AnimSetControllerD1Ev
00475130 ldr      r3, [pc, #0x24]
00475134 ldr      r2, [pc, #0x24]
00475138 push     {r4, lr}
0047513c add      r3, pc, r3
00475140 ldr      r2, [r3, r2]
00475144 mov      r4, r0
00475148 add      r2, r2, #8
0047514c str      r2, [r0]
00475150 bl       #0x474684 ; _ZN14AnimControllerD2Ev
00475154 mov      r0, r4
00475158 pop      {r4, pc}
0047515c subseq   pc, r1, r4, asr sb
00475160 andeq    r3, r0, ip, lsr #10

_ZN12CameraTargetC1Ev
00411c40 push     {r4, r5, r6, lr}
00411c44 ldr      r5, [pc, #0x84]
00411c48 mov      r4, r0
00411c4c bl       #0x40e720 ; _ZN10CameraBaseC2Ev
00411c50 ldr      r1, [pc, #0x7c]
00411c54 add      r5, pc, r5
00411c58 ldr      r2, [pc, #0x78]
00411c5c ldr      r1, [r5, r1]
00411c60 mov      r3, #0
00411c64 ldr      r2, [r5, r2]
00411c68 add      r0, r1, #8
00411c6c str      r0, [r4]
00411c70 mov      r0, #1
00411c74 strb     r0, [r4, #0x25]
00411c78 movw     r0, #0x3333
00411c7c mov      r1, #0
00411c80 movt     r0, #0x3f33
00411c84 str      r0, [r4, #0x28]
00411c88 strb     r1, [r4, #0x24]
00411c8c str      r1, [r4, #0xc]
00411c90 str      r3, [r4, #0x10]
00411c94 str      r3, [r4, #0x14]
00411c98 str      r3, [r4, #0x18]
00411c9c str      r1, [r4, #0x1c]
00411ca0 str      r1, [r4, #0x20]
00411ca4 ldr      r1, [r2]
00411ca8 mov      r0, r4
00411cac str      r1, [r4, #0x2c]
00411cb0 ldr      r1, [r2, #4]
00411cb4 str      r1, [r4, #0x30]
00411cb8 ldr      r2, [r2, #8]
00411cbc str      r3, [r4, #0x40]
00411cc0 str      r3, [r4, #0x38]
00411cc4 str      r2, [r4, #0x34]
00411cc8 str      r3, [r4, #0x3c]
00411ccc pop      {r4, r5, r6, pc}
00411cd0 subseq   r2, r8, ip, lsr lr
00411cd4 ldrdeq   r0, r1, [r0], -r4
00411cd8 andeq    r3, r0, ip, lsr #30

_ZN10CameraBase9ActivatedEv
0040e718 bx       lr

_ZN11CameraLevel10HandleZoomEv
00410218 push     {r4, r5, r6, r7, r8, sb, sl, lr}
0041021c ldr      r4, [pc, #0x158]
00410220 ldr      r7, [pc, #0x158]
00410224 ldr      r2, [pc, #0x158]
00410228 add      r4, pc, r4
0041022c ldr      r3, [r4, r7]
00410230 ldr      r8, [r4, r2]
00410234 sub      sp, sp, #0x20
00410238 ldr      r3, [r3]
0041023c add      r5, sp, #4
00410240 mov      r6, r0
00410244 mov      r0, r8
00410248 str      r3, [sp, #0x1c]
0041024c bl       #0x337888 ; _ZN13DebugSwitches4loadEv
00410250 mov      r0, r5
00410254 mov      r1, #0xd
00410258 str      r5, [sp, #0x14]
0041025c str      r5, [sp, #0x18]
00410260 bl       #0x31167c ; _ZNSt4priv12_String_baseIcSaIcEE17_M_allocate_blockEj
00410264 ldr      r1, [pc, #0x11c]
00410268 mov      r2, #0xc
0041026c ldr      r0, [sp, #0x18]
00410270 add      r1, pc, r1
00410274 bl       #0x30e868
00410278 add      r3, r0, #0xc
0041027c str      r3, [sp, #0x14]
00410280 mov      r3, #0
00410284 strb     r3, [r0, #0xc]
00410288 mov      r1, r5
0041028c mov      r0, r8
00410290 bl       #0x337a88 ; _ZN13DebugSwitches9GetSwitchERKSs
00410294 mov      r8, r0
00410298 mov      r0, r5
0041029c bl       #0x3139ac ; _ZNSt4priv12_String_baseIcSaIcEE19_M_deallocate_blockEv
004102a0 cmp      r8, #0
004102a4 bne      #0x41036c
004102a8 ldrb     r3, [r6, #0x86]
004102ac cmp      r3, #0
004102b0 bne      #0x410350
004102b4 ldr      r3, [pc, #0xd0]
004102b8 ldrb     r2, [r6, #0x85]
004102bc ldr      r8, [r6, #0x88]
004102c0 ldr      r3, [r4, r3]
004102c4 cmp      r2, #0
004102c8 mov      r0, r8
004102cc ldr      r3, [r3]
004102d0 ldr      sb, [r3, #0xac]
004102d4 ldrne    sb, [r3, #0x4c]
004102d8 ldr      sl, [r3, #0xa8]
004102dc ldrne    sl, [r3, #0x48]
004102e0 mov      r1, sb
004102e4 bl       #0x30e2f8
004102e8 cmp      r0, #0
004102ec moveq    r8, sb
004102f0 mov      r0, r8
004102f4 mov      r1, sl
004102f8 bl       #0x30e70c
004102fc ldr      r5, [r6, #0x8c]
00410300 cmp      r0, #0
00410304 moveq    r8, sl
00410308 mov      r0, r5
0041030c mov      r1, sb
00410310 str      r8, [r6, #0x88]
00410314 bl       #0x30e2f8
00410318 cmp      r0, #0
0041031c moveq    r5, sb
00410320 mov      r0, r5
00410324 mov      r1, sl
00410328 bl       #0x30e70c
0041032c cmp      r0, #0
00410330 moveq    r5, sl
00410334 str      r5, [r6, #0x8c]
00410338 mov      r1, r5
0041033c mov      r0, r8
00410340 bl       #0x30e2f8
00410344 cmp      r0, #0
00410348 moveq    r5, r8
0041034c str      r5, [r6, #0x90]
00410350 ldr      r3, [r4, r7]
00410354 ldr      r2, [sp, #0x1c]
00410358 ldr      r3, [r3]
0041035c cmp      r2, r3
00410360 bne      #0x410378
00410364 add      sp, sp, #0x20
00410368 pop      {r4, r5, r6, r7, r8, sb, sl, pc}
0041036c ldr      r3, [r6, #0x88]
00410370 str      r3, [r6, #0x90]
00410374 b        #0x410350
00410378 bl       #0x30e310
0041037c subseq   r4, r8, r8, ror #16
00410380 andeq    r4, r0, ip, lsr #1
00410384 andeq    r0, r0, r4, lsl #17
00410388 umaaleq  r7, fp, r8, ip
0041038c andeq    r3, r0, r8, asr #5

_ZNK10CameraBase15GetCenterOffsetER7Point3DIfEf
0040f4dc push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0040f4e0 ldr      r3, [r0, #8]
0040f4e4 ldr      r4, [pc, #0x204]
0040f4e8 mov      r6, r0
0040f4ec cmp      r3, #0
0040f4f0 add      r4, pc, r4
0040f4f4 sub      sp, sp, #0x34
0040f4f8 mov      r5, r1
0040f4fc mov      fp, r2
0040f500 moveq    r0, r3
0040f504 beq      #0x40f588
0040f508 mov      r0, r3
0040f50c ldr      r3, [r3]
0040f510 mov      lr, pc
0040f514 ldr      pc, [r3, #0x38]
0040f518 add      r3, r0, #0x10
0040f51c ldr      r7, [r3, #4]
0040f520 ldr      r8, [r0, #0x10]
0040f524 ldr      r3, [r3, #8]
0040f528 mov      sl, #0
0040f52c mov      r1, r8
0040f530 str      r3, [sp, #4]
0040f534 mov      r0, r8
0040f538 str      r8, [r5]
0040f53c str      r7, [r5, #4]
0040f540 str      sl, [r5, #8]
0040f544 bl       #0x30ed6c
0040f548 mov      r1, r7
0040f54c mov      sb, r0
0040f550 mov      r0, r7
0040f554 bl       #0x30ed6c
0040f558 mov      r1, r0
0040f55c mov      r0, sb
0040f560 bl       #0x30eba4
0040f564 mov      r1, sl
0040f568 bl       #0x30eba4
0040f56c movw     r1, #0xb717
0040f570 bic      r0, r0, #0x80000000
0040f574 movt     r1, #0x38d1
0040f578 bl       #0x30e70c
0040f57c cmp      r0, #0
0040f580 beq      #0x40f590
0040f584 mov      r0, #1
0040f588 add      sp, sp, #0x34
0040f58c pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
0040f590 ldr      r1, [r6, #8]
0040f594 add      r0, sp, #0x24
0040f598 bl       #0x597180 ; _ZNK6glitch5scene10ISceneNode19getAbsolutePositionEv
0040f59c mov      r1, fp
0040f5a0 ldr      r0, [sp, #0x2c]
0040f5a4 bl       #0x30e3ac
0040f5a8 ldr      r3, [pc, #0x144]
0040f5ac ldr      lr, [sp, #4]
0040f5b0 add      r1, sp, #0x18
0040f5b4 ldr      r3, [r4, r3]
0040f5b8 mov      sl, r0
0040f5bc add      r0, sp, #0xc
0040f5c0 ldr      ip, [r3]
0040f5c4 ldmib    r3, {r2, r3}
0040f5c8 add      ip, ip, #0x80000000
0040f5cc add      r2, r2, #0x80000000
0040f5d0 add      r3, r3, #0x80000000
0040f5d4 str      lr, [sp, #0x14]
0040f5d8 str      ip, [sp, #0x18]
0040f5dc str      r2, [sp, #0x1c]
0040f5e0 str      r8, [sp, #0xc]
0040f5e4 str      r7, [sp, #0x10]
0040f5e8 str      r3, [sp, #0x20]
0040f5ec bl       #0x313058 ; _ZNK7Point3DIfE5angleERKS0_
0040f5f0 ldr      r3, [r6, #8]
0040f5f4 bic      r4, r0, #0x80000000
0040f5f8 mov      r0, r3
0040f5fc ldr      r3, [r3]
0040f600 mov      lr, pc
0040f604 ldr      pc, [r3, #0x128]
0040f608 ldr      r3, [r6, #8]
0040f60c mov      r8, r0
0040f610 mov      r0, r3
0040f614 ldr      r3, [r3]
0040f618 mov      lr, pc
0040f61c ldr      pc, [r3, #0x128]
0040f620 mov      r6, r0
0040f624 mov      r0, r4
0040f628 bl       #0x30df2c
0040f62c mov      r1, #0x3f000000
0040f630 mov      r7, r0
0040f634 mov      r0, r8
0040f638 bl       #0x30ed6c
0040f63c mov      r1, r4
0040f640 bl       #0x30eba4
0040f644 bl       #0x30df2c
0040f648 mov      r1, #0xbf000000
0040f64c mov      r8, r0
0040f650 mov      r0, r6
0040f654 bl       #0x30ed6c
0040f658 mov      r1, r4
0040f65c bl       #0x30eba4
0040f660 bl       #0x30df2c
0040f664 mov      r1, r0
0040f668 mov      r0, r7
0040f66c bl       #0x30e3ac
0040f670 mov      r1, sl
0040f674 bl       #0x30ed6c
0040f678 mov      r6, r0
0040f67c mov      r0, r5
0040f680 bl       #0x34d0b0 ; _ZN7Point3DIfE9normalizeEv
0040f684 mov      r1, r7
0040f688 mov      r4, r0
0040f68c mov      r0, r8
0040f690 bl       #0x30e3ac
0040f694 mov      r1, sl
0040f698 bl       #0x30ed6c
0040f69c mov      r1, r0
0040f6a0 mov      r0, r6
0040f6a4 bl       #0x30eba4
0040f6a8 mov      r1, #0x3f000000
0040f6ac bl       #0x30ed6c
0040f6b0 mov      r1, r6
0040f6b4 bl       #0x30e3ac
0040f6b8 mov      r5, r0
0040f6bc mov      r1, r0
0040f6c0 ldr      r0, [r4]
0040f6c4 bl       #0x30ed6c
0040f6c8 mov      r1, r5
0040f6cc str      r0, [r4]
0040f6d0 ldr      r0, [r4, #4]
0040f6d4 bl       #0x30ed6c
0040f6d8 mov      r1, r5
0040f6dc str      r0, [r4, #4]
0040f6e0 ldr      r0, [r4, #8]
0040f6e4 bl       #0x30ed6c
0040f6e8 str      r0, [r4, #8]
0040f6ec b        #0x40f584
0040f6f0 subseq   r5, r8, r0, lsr #11
0040f6f4 andeq    r4, r0, r0, asr #6

_ZN14AnimSetManager6CreateEv
00476464 push     {r4, r5, r6, lr}
00476468 ldr      r1, [r0, #0x1c]
0047646c sub      sp, sp, #8
00476470 mov      r4, r0
00476474 sub      r1, r1, #1
00476478 str      r1, [r0, #0x1c]
0047647c bl       #0x475404 ; _ZNK14AnimSetManager6ExistsEi
00476480 ldr      r5, [pc, #0x98]
00476484 cmp      r0, #0
00476488 add      r5, pc, r5
0047648c beq      #0x4764b4
00476490 ldr      r3, [pc, #0x8c]
00476494 ldr      r3, [r5, r3]
00476498 ldr      r3, [r3]
0047649c cmp      r3, #2
004764a0 moveq    r3, #0
004764a4 streq    r3, [r3]
004764a8 beq      #0x4764b4
004764ac cmp      r3, #1
004764b0 beq      #0x4764ec
004764b4 add      r1, r4, #0x1c
004764b8 add      r0, r4, #4
004764bc bl       #0x476058 ; _ZNSt3mapIi12AnimationSetSt4lessIiESaISt4pairIKiS0_EEEixIiEERS0_RKT_
004764c0 mov      r6, r0
004764c4 bl       #0x364ca0 ; _ZN12AnimationSet13CreateAnimSetEv
004764c8 ldr      r3, [pc, #0x58]
004764cc ldr      r3, [r5, r3]
004764d0 ldrb     r3, [r3]
004764d4 cmp      r3, #0
004764d8 movne    r3, #1
004764dc strbne   r3, [r6, #0x3c]
004764e0 ldr      r0, [r4, #0x1c]
004764e4 add      sp, sp, #8
004764e8 pop      {r4, r5, r6, pc}
004764ec ldr      r0, [pc, #0x38]
004764f0 ldr      r1, [pc, #0x38]
004764f4 ldr      r2, [pc, #0x38]
004764f8 ldr      r0, [r5, r0]
004764fc ldr      r3, [pc, #0x34]
00476500 mov      ip, #0x48
00476504 add      r1, pc, r1
00476508 add      r2, pc, r2
0047650c add      r3, pc, r3
00476510 add      r0, r0, #0xa8
00476514 str      ip, [sp]
00476518 bl       #0x30e004
0047651c b        #0x4764b4
00476520 subseq   lr, r1, r8, lsl #12
00476524 andeq    r3, r0, r0, asr #19
00476528 andeq    r4, r0, r8, lsr #9
0047652c andeq    r1, r0, r0, asr #19
00476530 ldrdeq   r7, r8, [r4], #-0xe4
00476534 subeq    r7, r5, r0, asr #5
00476538 ldrdeq   r7, r8, [r5], #-0x2c

_ZN14AnimSetManagerC2Ev
00475364 ldr      r1, [pc, #0x40]
00475368 str      r4, [sp, #-4]!
0047536c ldr      r4, [pc, #0x3c]
00475370 add      r1, pc, r1
00475374 mov      ip, #0
00475378 ldr      r4, [r1, r4]
0047537c mov      r2, r0
00475380 str      ip, [r0, #8]
00475384 add      r4, r4, #8
00475388 str      r4, [r0]
0047538c strb     ip, [r2, #4]!
00475390 mvn      r4, #0
00475394 str      r4, [r0, #0x1c]
00475398 str      r2, [r0, #0x10]
0047539c str      ip, [r0, #0x14]
004753a0 str      r2, [r0, #0xc]
004753a4 ldm      sp!, {r4}
004753a8 bx       lr
004753ac subseq   pc, r1, r0, lsr #14
004753b0 andeq    r4, r0, ip, ror #6

_ZN7Structs15SetCameraTarget4readEP11IStreamBase
005026c8 push     {r4, r5, r6, lr}
005026cc mov      r4, r0
005026d0 sub      sp, sp, #8
005026d4 mov      r5, r1
005026d8 bl       #0x4ff828 ; _ZN7Structs9ScriptCmd4readEP11IStreamBase
005026dc mov      r0, r5
005026e0 add      r1, r4, #8
005026e4 bl       #0x459090 ; _ZN12StreamReader6readAsIiEEvP11IStreamBasePT_
005026e8 mov      r3, #1
005026ec cmp      r3, #0
005026f0 str      r3, [sp, #4]
005026f4 bne      #0x502738
005026f8 add      r3, r4, #9
005026fc add      r2, r4, #0xa
00502700 ldrb     r0, [r2, #1]
00502704 ldrb     r1, [r3, #-1]
00502708 cmp      r2, r3
0050270c eor      r1, r0, r1
00502710 strb     r1, [r3, #-1]
00502714 ldrb     r0, [r2, #1]
00502718 eor      r1, r1, r0
0050271c strb     r1, [r2, #1]
00502720 ldrb     r0, [r3, #-1]
00502724 sub      r2, r2, #1
00502728 eor      r1, r1, r0
0050272c strb     r1, [r3, #-1]
00502730 add      r3, r3, #1
00502734 bhi      #0x502700
00502738 mov      r0, r5
0050273c add      r1, r4, #0xc
00502740 bl       #0x3df1a0 ; _ZN12StreamReader6readAsIjEEvP11IStreamBasePT_
00502744 mov      r3, #1
00502748 cmp      r3, #0
0050274c str      r3, [sp, #4]
00502750 bne      #0x502794
00502754 add      r3, r4, #0xd
00502758 add      r2, r4, #0xe
0050275c ldrb     r0, [r2, #1]
00502760 ldrb     r1, [r3, #-1]
00502764 cmp      r2, r3
00502768 eor      r1, r0, r1
0050276c strb     r1, [r3, #-1]
00502770 ldrb     r0, [r2, #1]
00502774 eor      r1, r1, r0
00502778 strb     r1, [r2, #1]
0050277c ldrb     r0, [r3, #-1]
00502780 sub      r2, r2, #1
00502784 eor      r1, r1, r0
00502788 strb     r1, [r3, #-1]
0050278c add      r3, r3, #1
00502790 bhi      #0x50275c
00502794 ldr      r0, [r4, #0x10]
00502798 cmp      r0, #0
0050279c beq      #0x5027a4
005027a0 bl       #0x310440 ; _Z10CustomFreePv
005027a4 ldr      r0, [r4, #0xc]
005027a8 mov      r1, #1
005027ac mov      r6, #0
005027b0 add      r0, r0, r1
005027b4 bl       #0x31056c ; _Znaj15MemoryHintState
005027b8 ldr      r2, [r4, #0xc]
005027bc mov      r1, r0
005027c0 str      r0, [r4, #0x10]
005027c4 mov      r3, r6
005027c8 mov      r0, r5
005027cc bl       #0x317454 ; _ZN12StreamReader12readStringExEP11IStreamBasePcy
005027d0 ldr      r2, [r4, #0x10]
005027d4 ldr      r3, [r4, #0xc]
005027d8 mov      r0, r5
005027dc add      r1, r4, #0x14
005027e0 strb     r6, [r2, r3]
005027e4 bl       #0x4db89c ; _ZN12StreamReader6readAsIbEEvP11IStreamBasePT_
005027e8 add      sp, sp, #8
005027ec pop      {r4, r5, r6, pc}

_ZN24BlendedAnimSetController8SetScaleEfj
0047673c ldr      r3, [pc, #0x34]
00476740 push     {r4, lr}
00476744 mov      r4, r1
00476748 ldr      r1, [pc, #0x2c]
0047674c add      r3, pc, r3
00476750 ldr      ip, [r3, r1]
00476754 ldrb     r3, [ip]
00476758 cmp      r3, #0
0047675c bne      #0x476764
00476760 pop      {r4, pc}
00476764 mov      r1, r2
00476768 bl       #0x4748b8 ; _ZN14AnimController7GetAnimEj
0047676c mov      r1, r4
00476770 pop      {r4, lr}
00476774 b        #0x3666d8 ; _ZN15AnimatorBlender8SetScaleEf
00476778 subseq   lr, r1, r4, asr #6
0047677c andeq    r3, r0, ip, ror #27

_ZN24BlendedAnimSetControllerD1Ev
00476990 ldr      r3, [pc, #0x24]
00476994 ldr      r2, [pc, #0x24]
00476998 push     {r4, lr}
0047699c add      r3, pc, r3
004769a0 ldr      r2, [r3, r2]
004769a4 mov      r4, r0
004769a8 add      r2, r2, #8
004769ac str      r2, [r0]
004769b0 bl       #0x474684 ; _ZN14AnimControllerD2Ev
004769b4 mov      r0, r4
004769b8 pop      {r4, pc}
004769bc ldrsheq  lr, [r1], #-4
004769c0 andeq    r2, r0, r8, lsr #7

_ZN11CameraLevel10__CallbackEPN6glitch5scene19ITimelineControllerEPv
0040f8f8 mov      r3, #0
0040f8fc strb     r3, [r1, #0x84]
0040f900 bx       lr

_ZN12CameraTarget15SetDampingRatioEf
00411654 str      r1, [r0, #0x28]
00411658 bx       lr

_ZNK24BlendedAnimSetController15GetClipDurationEj
00476688 mvn      r0, #0
0047668c bx       lr

_ZN12CameraTarget9SetTargetEP10GameObjecti
004119c4 ldr      r3, [pc, #0xa0]
004119c8 push     {r4, r5, r6, lr}
004119cc subs     r5, r1, #0
004119d0 mov      r4, r0
004119d4 add      r3, pc, r3
004119d8 mov      r6, r2
004119dc beq      #0x411a2c
004119e0 cmp      r2, #0
004119e4 ble      #0x411a30
004119e8 ldr      r0, [r0, #0xc]
004119ec cmp      r0, #0
004119f0 beq      #0x411a60
004119f4 bl       #0x3943b8 ; _ZNK10GameObject23GetCameraAnchorPositionEv
004119f8 ldr      r3, [r0]
004119fc str      r3, [r4, #0x10]
00411a00 ldr      r3, [r0, #4]
00411a04 str      r3, [r4, #0x14]
00411a08 ldr      r3, [r0, #8]
00411a0c str      r6, [r4, #0x20]
00411a10 str      r6, [r4, #0x1c]
00411a14 str      r3, [r4, #0x18]
00411a18 mov      r3, #0
00411a1c str      r5, [r4, #0xc]
00411a20 str      r3, [r4, #0x40]
00411a24 str      r3, [r4, #0x38]
00411a28 str      r3, [r4, #0x3c]
00411a2c pop      {r4, r5, r6, pc}
00411a30 ldr      r1, [pc, #0x38]
00411a34 mov      r2, #0
00411a38 ldr      r3, [r3, r1]
00411a3c ldr      r1, [r3]
00411a40 str      r1, [r0, #0x10]
00411a44 ldr      r1, [r3, #4]
00411a48 str      r1, [r0, #0x14]
00411a4c ldr      r3, [r3, #8]
00411a50 str      r2, [r0, #0x20]
00411a54 str      r2, [r0, #0x1c]
00411a58 str      r3, [r0, #0x18]
00411a5c b        #0x411a18
00411a60 ldr      r2, [pc, #8]
00411a64 ldr      r0, [r3, r2]
00411a68 b        #0x4119f8
00411a6c ldrheq   r3, [r8], #-0xc
00411a70 andeq    r3, r0, ip, lsr #30

_ZN24BlendedAnimSetControllerD2Ev
004769e0 ldr      r3, [pc, #0x24]
004769e4 ldr      r2, [pc, #0x24]
004769e8 push     {r4, lr}
004769ec add      r3, pc, r3
004769f0 ldr      r2, [r3, r2]
004769f4 mov      r4, r0
004769f8 add      r2, r2, #8
004769fc str      r2, [r0]
00476a00 bl       #0x474684 ; _ZN14AnimControllerD2Ev
00476a04 mov      r0, r4
00476a08 pop      {r4, pc}
00476a0c subseq   lr, r1, r4, lsr #1
00476a10 andeq    r2, r0, r8, lsr #7

_ZN10CameraBaseC2Ev
0040e720 ldr      r3, [pc, #0x20]
0040e724 ldr      r1, [pc, #0x20]
0040e728 mov      ip, #0
0040e72c add      r3, pc, r3
0040e730 ldr      r1, [r3, r1]
0040e734 str      ip, [r0, #8]
0040e738 str      ip, [r0, #4]
0040e73c add      r1, r1, #8
0040e740 str      r1, [r0]
0040e744 bx       lr
0040e748 subseq   r6, r8, r4, ror #6
0040e74c andeq    r2, r0, ip, ror #3

_ZN10CameraBase7SetDataEffffb
0040e9a8 push     {r4, r5, r6, lr}
0040e9ac ldr      ip, [r0, #8]
0040e9b0 sub      sp, sp, #0x10
0040e9b4 mov      r4, r0
0040e9b8 cmp      ip, #0
0040e9bc mov      r5, r2
0040e9c0 mov      r6, r3
0040e9c4 beq      #0x40ea48
0040e9c8 mov      r0, ip
0040e9cc ldr      r3, [ip]
0040e9d0 mov      lr, pc
0040e9d4 ldr      pc, [r3, #0x13c]
0040e9d8 ldr      r3, [r4, #8]
0040e9dc mov      r1, r5
0040e9e0 mov      r0, r3
0040e9e4 ldr      r3, [r3]
0040e9e8 mov      lr, pc
0040e9ec ldr      pc, [r3, #0x138]
0040e9f0 ldr      r3, [r4, #8]
0040e9f4 mov      r1, r6
0040e9f8 mov      r0, r3
0040e9fc ldr      r3, [r3]
0040ea00 mov      lr, pc
0040ea04 ldr      pc, [r3, #0x130]
0040ea08 ldr      r3, [r4, #8]
0040ea0c ldr      r1, [sp, #0x20]
0040ea10 mov      r0, r3
0040ea14 ldr      r3, [r3]
0040ea18 mov      lr, pc
0040ea1c ldr      pc, [r3, #0x134]
0040ea20 ldr      r0, [r4, #8]
0040ea24 mov      r2, #0
0040ea28 mov      ip, #0x3f800000
0040ea2c ldr      r3, [r0]
0040ea30 add      r1, sp, #4
0040ea34 ldr      r3, [r3, #0x114]
0040ea38 str      r2, [sp, #8]
0040ea3c str      ip, [sp, #0xc]
0040ea40 str      r2, [sp, #4]
0040ea44 blx      r3
0040ea48 add      sp, sp, #0x10
0040ea4c pop      {r4, r5, r6, pc}

_ZN24BlendedAnimSetController15GetAnimationSetEv
00476698 push     {r4, lr}
0047669c mov      r4, r0
004766a0 mov      r0, r1
004766a4 mov      r1, #0
004766a8 bl       #0x4748b8 ; _ZN14AnimController7GetAnimEj
004766ac cmp      r0, #0
004766b0 beq      #0x4766ec
004766b4 ldr      r3, [r0, #0x28]
004766b8 ldr      r2, [r0, #0x70]
004766bc ldr      r3, [r3, r2, lsl #2]
004766c0 cmp      r3, #0
004766c4 beq      #0x4766ec
004766c8 ldr      r3, [r3, #0x94]
004766cc cmp      r3, #0
004766d0 str      r3, [r4]
004766d4 beq      #0x4766e4
004766d8 ldr      r2, [r3, #4]
004766dc add      r2, r2, #1
004766e0 str      r2, [r3, #4]
004766e4 mov      r0, r4
004766e8 pop      {r4, pc}
004766ec mov      r3, #0
004766f0 str      r3, [r4]
004766f4 mov      r0, r4
004766f8 pop      {r4, pc}

_ZNK24BlendedAnimSetController10GetNumClipEj
0047695c push     {r4, lr}
00476960 bl       #0x474770 ; _ZNK14AnimController7GetAnimEj
00476964 cmp      r0, #0
00476968 beq      #0x476988
0047696c ldr      r3, [r0, #0x28]
00476970 ldr      r2, [r0, #0x70]
00476974 ldr      r0, [r3, r2, lsl #2]
00476978 cmp      r0, #0
0047697c beq      #0x476988
00476980 pop      {r4, lr}
00476984 b        #0x65f11c ; _ZNK6glitch7collada21CSceneNodeAnimatorSet17getAnimationCountEv
00476988 mov      r0, #0
0047698c pop      {r4, pc}

_ZN11CameraLevelC2Ev
0041002c push     {r4, r5, r6, lr}
00410030 ldr      r6, [pc, #0xa8]
00410034 mov      r4, r0
00410038 bl       #0x411cdc ; _ZN12CameraTargetC2Ev
0041003c ldr      r2, [pc, #0xa0]
00410040 add      r6, pc, r6
00410044 mov      r5, #0
00410048 ldr      r2, [r6, r2]
0041004c add      r3, r4, #0x4c
00410050 mov      r0, r3
00410054 add      r2, r2, #8
00410058 str      r2, [r4]
0041005c str      r3, [r4, #0x5c]
00410060 str      r3, [r4, #0x60]
00410064 str      r5, [r4, #0x44]
00410068 str      r5, [r4, #0x48]
0041006c mov      r1, #0x10
00410070 bl       #0x31167c ; _ZNSt4priv12_String_baseIcSaIcEE17_M_allocate_blockEj
00410074 ldr      r2, [r4, #0x5c]
00410078 add      r3, r4, #0x64
0041007c mov      r0, r3
00410080 strb     r5, [r2]
00410084 mov      r1, #0x10
00410088 str      r3, [r4, #0x74]
0041008c str      r3, [r4, #0x78]
00410090 bl       #0x31167c ; _ZNSt4priv12_String_baseIcSaIcEE17_M_allocate_blockEj
00410094 ldr      r2, [r4, #0x74]
00410098 mov      r3, #0
0041009c mov      r0, r4
004100a0 strb     r5, [r2]
004100a4 mvn      r2, #0
004100a8 str      r2, [r4, #0x7c]
004100ac mov      r2, #0x3f800000
004100b0 str      r2, [r4, #0x8c]
004100b4 str      r3, [r4, #0xa0]
004100b8 strb     r5, [r4, #0xa4]
004100bc strb     r5, [r4, #0x84]
004100c0 strb     r5, [r4, #0x85]
004100c4 strb     r5, [r4, #0x86]
004100c8 str      r3, [r4, #0x88]
004100cc str      r3, [r4, #0x90]
004100d0 str      r3, [r4, #0x94]
004100d4 str      r3, [r4, #0x98]
004100d8 str      r3, [r4, #0x9c]
004100dc pop      {r4, r5, r6, pc}
004100e0 subseq   r4, r8, r0, asr sl
004100e4 andeq    r4, r0, r8, lsr #4

_ZN17AnimSetController15GetAnimationSetEv
00474f70 push     {r4, lr}
00474f74 mov      r4, r0
00474f78 mov      r0, r1
00474f7c mov      r1, #0
00474f80 bl       #0x4748b8 ; _ZN14AnimController7GetAnimEj
00474f84 cmp      r0, #0
00474f88 streq    r0, [r4]
00474f8c beq      #0x474fa8
00474f90 ldr      r3, [r0, #0x94]
00474f94 cmp      r3, #0
00474f98 str      r3, [r4]
00474f9c ldrne    r2, [r3, #4]
00474fa0 addne    r2, r2, #1
00474fa4 strne    r2, [r3, #4]
00474fa8 mov      r0, r4
00474fac pop      {r4, pc}

_ZN7Structs15SetCameraTarget8finalizeEv
004d370c push     {r4, lr}
004d3710 mov      r4, r0
004d3714 ldr      r0, [r0, #0x10]
004d3718 cmp      r0, #0
004d371c beq      #0x4d3730
004d3720 bl       #0x310440 ; _Z10CustomFreePv
004d3724 mov      r3, #0
004d3728 str      r3, [r4, #0xc]
004d372c str      r3, [r4, #0x10]
004d3730 mov      r0, r4
004d3734 pop      {r4, lr}
004d3738 b        #0x4c6c68 ; _ZN7Structs9ScriptCmd8finalizeEv

_ZN10CameraBaseD0Ev
0040f6f8 push     {r4, lr}
0040f6fc mov      r4, r0
0040f700 bl       #0x40e814 ; _ZN10CameraBaseD1Ev
0040f704 mov      r0, r4
0040f708 bl       #0x310440 ; _Z10CustomFreePv
0040f70c mov      r0, r4
0040f710 pop      {r4, pc}

_ZN10CameraBase13GetWorldCoordERK7Point2DIfER7Point3DIfEf
0040f394 ldr      r3, [pc, #0xb8]
0040f398 push     {r4, r5, r6, r7, r8, lr}
0040f39c mov      r4, r0
0040f3a0 ldr      r0, [pc, #0xb0]
0040f3a4 add      r3, pc, r3
0040f3a8 sub      sp, sp, #8
0040f3ac ldr      r0, [r3, r0]
0040f3b0 mov      ip, #0
0040f3b4 mov      r5, r1
0040f3b8 ldr      lr, [r0, #0x10]
0040f3bc mov      r1, #0x3f800000
0040f3c0 ldr      r0, [r4]
0040f3c4 ldr      lr, [lr, #0x10]
0040f3c8 mov      r7, r2
0040f3cc ldr      r3, [lr, #0xcc]
0040f3d0 ldr      r6, [r3, #-4]
0040f3d4 str      ip, [sp, #4]
0040f3d8 str      ip, [sp]
0040f3dc bl       #0x30eba4
0040f3e0 mov      r1, #0x3f000000
0040f3e4 bl       #0x30ed6c
0040f3e8 mov      r8, r0
0040f3ec ldr      r0, [r6, #0xc]
0040f3f0 bl       #0x30e964
0040f3f4 mov      r1, r0
0040f3f8 mov      r0, r8
0040f3fc bl       #0x30ed6c
0040f400 bl       #0x30e4cc
0040f404 str      r0, [sp]
0040f408 ldr      r0, [r4, #4]
0040f40c mov      r1, #0x3f800000
0040f410 bl       #0x30eba4
0040f414 mov      r1, #0x3f000000
0040f418 bl       #0x30ed6c
0040f41c mov      r4, r0
0040f420 ldr      r0, [r6, #0x10]
0040f424 bl       #0x30e964
0040f428 mov      r1, r0
0040f42c mov      r0, r4
0040f430 bl       #0x30ed6c
0040f434 bl       #0x30e4cc
0040f438 mov      r1, r5
0040f43c str      r0, [sp, #4]
0040f440 mov      r2, r7
0040f444 mov      r0, sp
0040f448 bl       #0x40f2f0 ; _ZN10CameraBase13GetWorldCoordERK7Point2DIiER7Point3DIfEf
0040f44c add      sp, sp, #8
0040f450 pop      {r4, r5, r6, r7, r8, pc}
0040f454 subseq   r5, r8, ip, ror #13
0040f458 strdeq   r3, r4, [r0], -r4

_ZNK24BlendedAnimSetController7HasClipEPKcj
00476680 mov      r0, #0
00476684 bx       lr

_ZN12CameraTarget9SetTargetEPKci
00411b38 push     {r4, r5, r6, r7, lr}
00411b3c ldr      ip, [pc, #0x70]
00411b40 mov      r3, r1
00411b44 ldr      r1, [pc, #0x6c]
00411b48 add      ip, pc, ip
00411b4c sub      sp, sp, #0x1c
00411b50 ldr      r1, [ip, r1]
00411b54 add      r5, sp, #0xc
00411b58 mov      r4, #0
00411b5c ldr      r1, [r1, #0x38]
00411b60 mov      r7, r0
00411b64 mov      r6, r2
00411b68 mov      r0, r5
00411b6c mov      r2, r3
00411b70 mvn      r3, #0
00411b74 str      r4, [sp]
00411b78 str      r4, [sp, #4]
00411b7c bl       #0x34aca0 ; _ZN13ObjectManager15GetObjectByNameEPKcibS1_
00411b80 mov      r0, r5
00411b84 mov      r1, r4
00411b88 bl       #0x33fdc0 ; _ZN12ObjectHandle9GetObjectEb
00411b8c cmp      r0, r4
00411b90 beq      #0x411bac
00411b94 mov      r0, r5
00411b98 bl       #0x33fee4 ; _ZN12ObjectHandlecvP10GameObjectEv
00411b9c mov      r2, r6
00411ba0 mov      r1, r0
00411ba4 mov      r0, r7
00411ba8 bl       #0x4119c4 ; _ZN12CameraTarget9SetTargetEP10GameObjecti
00411bac add      sp, sp, #0x1c
00411bb0 pop      {r4, r5, r6, r7, pc}
00411bb4 subseq   r2, r8, r8, asr #30
00411bb8 strdeq   r3, r4, [r0], -r4

_ZNK11CameraLevel16CanPlayShakeAnimEP9Character
0040f980 push     {r4, r5, r6, lr}
0040f984 ldr      r5, [pc, #0xc0]
0040f988 subs     r6, r1, #0
0040f98c sub      sp, sp, #8
0040f990 add      r5, pc, r5
0040f994 beq      #0x40f9f0
0040f998 ldr      r3, [r6]
0040f99c mov      r0, r6
0040f9a0 mov      lr, pc
0040f9a4 ldr      pc, [r3, #0x28]
0040f9a8 cmp      r0, #0
0040f9ac bne      #0x40f9bc
0040f9b0 mov      r0, #1
0040f9b4 add      sp, sp, #8
0040f9b8 pop      {r4, r5, r6, pc}
0040f9bc ldr      r3, [pc, #0x8c]
0040f9c0 mov      r1, #0
0040f9c4 ldr      r4, [r5, r3]
0040f9c8 ldr      r0, [r4, #0x40]
0040f9cc bl       #0x36ead0 ; _ZN13PlayerManager18GetNumLocalPlayersEb
0040f9d0 cmp      r0, #1
0040f9d4 movne    r0, #0
0040f9d8 bne      #0x40f9b4
0040f9dc ldr      r0, [r4, #0x40]
0040f9e0 mov      r1, r6
0040f9e4 add      sp, sp, #8
0040f9e8 pop      {r4, r5, r6, lr}
0040f9ec b        #0x36effc ; _ZN13PlayerManager13IsLocalPlayerEPK9Character
0040f9f0 ldr      r3, [pc, #0x5c]
0040f9f4 ldr      r3, [r5, r3]
0040f9f8 ldr      r4, [r3]
0040f9fc cmp      r4, #2
0040fa00 streq    r6, [r6]
0040fa04 moveq    r0, #1
0040fa08 beq      #0x40f9b4
0040fa0c cmp      r4, #1
0040fa10 bne      #0x40f9b0
0040fa14 ldr      r0, [pc, #0x3c]
0040fa18 ldr      r1, [pc, #0x3c]
0040fa1c ldr      r2, [pc, #0x3c]
0040fa20 ldr      r0, [r5, r0]
0040fa24 ldr      r3, [pc, #0x38]
0040fa28 mov      ip, #0xf6
0040fa2c add      r1, pc, r1
0040fa30 add      r0, r0, #0xa8
0040fa34 add      r2, pc, r2
0040fa38 add      r3, pc, r3
0040fa3c str      ip, [sp]
0040fa40 bl       #0x30e004
0040fa44 mov      r0, r4
0040fa48 b        #0x40f9b4
0040fa4c subseq   r5, r8, r0, lsl #2
0040fa50 strdeq   r3, r4, [r0], -r4
0040fa54 andeq    r3, r0, r0, asr #19
0040fa58 andeq    r1, r0, r0, asr #19
0040fa5c subeq    lr, sl, ip, lsr #19
0040fa60 subeq    r8, fp, ip, ror #8
0040fa64 subeq    r8, fp, r8, ror r4

_ZN11ZoomHandler9setCameraEP11CameraLevel
00381fb0 ldr      r3, [pc, #0x5c]
00381fb4 ldr      r2, [pc, #0x5c]
00381fb8 push     {r4, lr}
00381fbc add      r3, pc, r3
00381fc0 mov      r4, r0
00381fc4 ldr      r0, [r3, r2]
00381fc8 mov      r2, #0
00381fcc str      r1, [r4, #0x20]
00381fd0 strb     r2, [r4, #0x24]
00381fd4 strb     r2, [r4, #0x34]
00381fd8 ldr      r2, [r0, #0x10]
00381fdc ldr      r3, [r2, #0x1c]
00381fe0 ldr      r3, [r3, #0x14]
00381fe4 ldr      r3, [r3, #0xcc]
00381fe8 ldr      r3, [r3, #-4]
00381fec ldr      r2, [r3, #0xc]
00381ff0 ldr      r0, [r3, #0x10]
00381ff4 cmp      r0, r2
00381ff8 movlt    r0, r2
00381ffc bl       #0x30e964
00382000 mov      r1, r0
00382004 mov      r0, #0x3f800000
00382008 bl       #0x30ec94
0038200c str      r0, [r4, #0x28]
00382010 pop      {r4, pc}

_ZN24BlendedAnimSetController8PlayClipEjbij
0047680c push     {r3, r4, r5, r6, r7, r8, sb, sl, fp, lr}
00476810 mov      r4, r1
00476814 ldr      r1, [sp, #0x28]
00476818 mov      sl, r2
0047681c mov      r7, r0
00476820 bl       #0x4748b8 ; _ZN14AnimController7GetAnimEj
00476824 subs     r6, r0, #0
00476828 beq      #0x476900
0047682c ldr      r1, [r7, #0x14]
00476830 bl       #0x36679c ; _ZN15AnimatorBlender5BlendEi
00476834 cmn      r4, #1
00476838 beq      #0x476900
0047683c ldr      r2, [r6, #0x70]
00476840 ldr      r3, [r6, #0x28]
00476844 ldr      r5, [r3, r2, lsl #2]
00476848 cmp      r5, #0
0047684c beq      #0x476908
00476850 ldr      r3, [r5]
00476854 mov      r0, r5
00476858 mov      lr, pc
0047685c ldr      pc, [r3, #0x44]
00476860 mov      r8, r0
00476864 mov      r0, r5
00476868 bl       #0x65f114 ; _ZNK6glitch7collada21CSceneNodeAnimatorSet19getCurrentAnimationEv
0047686c mov      sb, r0
00476870 mov      r0, r6
00476874 bl       #0x369160 ; _Z13GetApplicatorPN6glitch5scene18ISceneNodeAnimatorE
00476878 mov      r1, r4
0047687c mov      fp, r0
00476880 mov      r0, r5
00476884 bl       #0x3674ac ; _ZN11AnimatorSet19SetCurrentAnimationEi
00476888 cmn      r0, #1
0047688c mov      r4, r0
00476890 beq      #0x476900
00476894 ldr      r3, [r8, #0x34]
00476898 cmp      r3, #0
0047689c beq      #0x4768b4
004768a0 mov      r0, r5
004768a4 ldr      r3, [r5]
004768a8 ldr      r1, [r7, #0xc]
004768ac mov      lr, pc
004768b0 ldr      pc, [r3, #0x30]
004768b4 cmp      sb, r4
004768b8 beq      #0x476920
004768bc mov      r1, sl
004768c0 mov      r0, r8
004768c4 ldr      r3, [r8]
004768c8 mov      lr, pc
004768cc ldr      pc, [r3, #0x40]
004768d0 ldr      r3, [r8]
004768d4 mov      r0, r8
004768d8 mov      r1, #0x3f800000
004768dc mov      lr, pc
004768e0 ldr      pc, [r3, #0x48]
004768e4 ldrb     r1, [r7, #0x10]
004768e8 ldr      r0, [r7, #4]
004768ec bl       #0x35d624 ; _ZN13RootSceneNode7NewAnimEb
004768f0 mov      r0, r6
004768f4 bl       #0x366740 ; _ZN15AnimatorBlender9BlendPostEv
004768f8 mov      r0, #1
004768fc pop      {r3, r4, r5, r6, r7, r8, sb, sl, fp, pc}
00476900 mov      r0, #0
00476904 pop      {r3, r4, r5, r6, r7, r8, sb, sl, fp, pc}
00476908 mov      r0, r5
0047690c bl       #0x65f114 ; _ZNK6glitch7collada21CSceneNodeAnimatorSet19getCurrentAnimationEv
00476910 mov      r0, r6
00476914 bl       #0x369160 ; _Z13GetApplicatorPN6glitch5scene18ISceneNodeAnimatorE
00476918 mov      r0, r5
0047691c pop      {r3, r4, r5, r6, r7, r8, sb, sl, fp, pc}
00476920 ldr      r3, [r8]
00476924 mov      r0, r8
00476928 mov      lr, pc
0047692c ldr      pc, [r3, #0x44]
00476930 cmp      r0, #0
00476934 bne      #0x4768bc
00476938 cmp      fp, #0
0047693c ldr      r3, [r8]
00476940 ldr      r1, [r8, #0x10]
00476944 ldrne    fp, [fp, #0x10]
00476948 ldr      r3, [r3, #0xc]
0047694c mov      r0, r8
00476950 add      r1, fp, r1
00476954 blx      r3
00476958 b        #0x4768bc

_ZN17AnimSetControllerD0Ev
00475164 push     {r4, lr}
00475168 mov      r4, r0
0047516c bl       #0x475130 ; _ZN17AnimSetControllerD1Ev
00475170 mov      r0, r4
00475174 bl       #0x310440 ; _Z10CustomFreePv
00475178 mov      r0, r4
0047517c pop      {r4, pc}

_ZNK17AnimSetController10GetNumClipEj
00475114 push     {r4, lr}
00475118 bl       #0x474770 ; _ZNK14AnimController7GetAnimEj
0047511c cmp      r0, #0
00475120 beq      #0x47512c
00475124 pop      {r4, lr}
00475128 b        #0x65f11c ; _ZNK6glitch7collada21CSceneNodeAnimatorSet17getAnimationCountEv
0047512c pop      {r4, pc}