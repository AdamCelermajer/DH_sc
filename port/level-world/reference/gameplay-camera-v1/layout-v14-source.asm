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
