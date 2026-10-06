_ZN10ItemObject13DropLootTableEiPK10GameObjectS2_ib
003ecba0 push     {r4, r5, r6, r7, r8, sl, lr}
003ecba4 subs     r5, r2, #0
003ecba8 sub      sp, sp, #0x54
003ecbac mov      r8, r0
003ecbb0 mov      r6, r1
003ecbb4 mov      r7, r3
003ecbb8 moveq    r4, r5
003ecbbc beq      #0x3ecbe8
003ecbc0 add      r4, sp, #0x44
003ecbc4 mov      r0, r4
003ecbc8 mov      r1, r5
003ecbcc bl       #0x33dd70 ; _ZNK10ObjectBase9GetHandleEv
003ecbd0 mov      r0, r4
003ecbd4 mov      r1, #0
003ecbd8 bl       #0x33ff8c ; _ZNK12ObjectHandle9GetObjectEb
003ecbdc subs     r4, r0, #0
003ecbe0 bne      #0x3ecc1c
003ecbe4 mov      r4, #0
003ecbe8 cmp      r6, #0
003ecbec beq      #0x3ecc14
003ecbf0 add      sl, sp, #0x38
003ecbf4 mov      r1, r6
003ecbf8 mov      r0, sl
003ecbfc bl       #0x33dd70 ; _ZNK10ObjectBase9GetHandleEv
003ecc00 mov      r0, sl
003ecc04 mov      r1, #0
003ecc08 bl       #0x33ff8c ; _ZNK12ObjectHandle9GetObjectEb
003ecc0c subs     r3, r0, #0
003ecc10 bne      #0x3ecc2c
003ecc14 mov      sl, #0
003ecc18 b        #0x3ecc9c
003ecc1c ldr      r3, [r4, #0xf4]
003ecc20 cmp      r3, #0
003ecc24 beq      #0x3ecbe8
003ecc28 b        #0x3ecbe4
003ecc2c ldr      r2, [r3, #0xf4]
003ecc30 cmp      r2, #0
003ecc34 bne      #0x3ecc14
003ecc38 mov      sl, r3
003ecc3c bl       #0x3a3158 ; _ZNK9Character6IsBossEv
003ecc40 cmp      r0, #0
003ecc44 beq      #0x3ecc8c
003ecc48 mov      r0, sp
003ecc4c bl       #0x3ff200 ; _ZN13ItemInventoryC1Ev
003ecc50 mov      r0, sp
003ecc54 mov      r1, r8
003ecc58 mov      r2, r5
003ecc5c mov      r3, r7
003ecc60 bl       #0x3ecae4 ; _ZN10ItemObject18GetInventoryToDropER13ItemInventoryiPK10GameObjecti
003ecc64 mov      r0, sp
003ecc68 mov      r1, r6
003ecc6c mov      r2, r5
003ecc70 mov      r3, r7
003ecc74 bl       #0x3ec8a0 ; _ZN10ItemObject16DropAndAwardLootER13ItemInventoryPK10GameObjectS4_i
003ecc78 mov      r0, sp
003ecc7c mov      r4, sp
003ecc80 bl       #0x3ff460 ; _ZN13ItemInventoryD1Ev
003ecc84 add      sp, sp, #0x54
003ecc88 pop      {r4, r5, r6, r7, r8, sl, pc}
003ecc8c mov      r0, sl
003ecc90 bl       #0x3a3144 ; _ZNK9Character10IsMiniBossEv
003ecc94 cmp      r0, #0
003ecc98 bne      #0x3ecc48
003ecc9c cmp      r4, #0
003ecca0 beq      #0x3ecc84
003ecca4 ldr      r2, [r4]
003ecca8 mov      r0, r4
003eccac mov      lr, pc
003eccb0 ldr      pc, [r2, #0x28]
003eccb4 cmp      r0, #0
003eccb8 bne      #0x3ecc48
003eccbc cmp      sl, r4
003eccc0 bne      #0x3ecc84
003eccc4 b        #0x3ecc48
_ZN10ItemObject13DropLootTableEiPK10GameObjectS2_PK9Characteri
003eccc8 push     {r4, r5, r6, r7, r8, lr}
003ecccc subs     r5, r2, #0
003eccd0 sub      sp, sp, #0x48
003eccd4 mov      r8, r0
003eccd8 mov      r7, r1
003eccdc mov      r6, r3
003ecce0 beq      #0x3ecd00
003ecce4 add      r4, sp, #0x3c
003ecce8 mov      r1, r5
003eccec mov      r0, r4
003eccf0 bl       #0x33dd70 ; _ZNK10ObjectBase9GetHandleEv
003eccf4 mov      r0, r4
003eccf8 mov      r1, #0
003eccfc bl       #0x33ff8c ; _ZNK12ObjectHandle9GetObjectEb
003ecd00 add      r4, sp, #4
003ecd04 mov      r0, r4
003ecd08 bl       #0x3ff200 ; _ZN13ItemInventoryC1Ev
003ecd0c mov      r0, r4
003ecd10 mov      r1, r8
003ecd14 mov      r2, r5
003ecd18 ldr      r3, [sp, #0x60]
003ecd1c bl       #0x3ecae4 ; _ZN10ItemObject18GetInventoryToDropER13ItemInventoryiPK10GameObjecti
003ecd20 mov      r0, r4
003ecd24 mov      r1, r7
003ecd28 mov      r2, r5
003ecd2c mov      r3, r6
003ecd30 bl       #0x3ec974 ; _ZN10ItemObject13DropInventoryER13ItemInventoryPK10GameObjectS4_PK9Character
003ecd34 mov      r0, r4
003ecd38 bl       #0x3ff460 ; _ZN13ItemInventoryD1Ev
003ecd3c add      sp, sp, #0x48
003ecd40 pop      {r4, r5, r6, r7, r8, pc}
_ZN17OpenableContainer8InteractEP10GameObject
003a1904 push     {r4, r5, r6, r7, r8, sb, sl, lr}
003a1908 sub      sp, sp, #0x30
003a190c mov      r6, r0
003a1910 mov      r5, r1
003a1914 bl       #0x3a18bc ; _ZN17OpenableContainer12TryUnlockingEP10GameObject
003a1918 ldr      r4, [pc, #0x208]
003a191c cmp      r0, #0
003a1920 add      r4, pc, r4
003a1924 bne      #0x3a1930
003a1928 add      sp, sp, #0x30
003a192c pop      {r4, r5, r6, r7, r8, sb, sl, pc}
003a1930 ldr      r7, [pc, #0x1f4]
003a1934 ldr      r0, [r4, r7]
003a1938 bl       #0x31f594 ; _ZNK11Application15GetCurrentLevelEv
003a193c subs     r8, r0, #0
003a1940 beq      #0x3a1acc
003a1944 ldr      sl, [r4, r7]
003a1948 ldr      r0, [sl, #0x40]
003a194c bl       #0x36f074 ; _ZN13PlayerManager20IsLocalPlayerHostingEv
003a1950 cmp      r0, #0
003a1954 bne      #0x3a1a48
003a1958 mov      r0, r6
003a195c mov      r1, r5
003a1960 add      r6, sp, #0x24
003a1964 bl       #0x3a0b38 ; _ZN9Container8InteractEP10GameObject
003a1968 mov      r1, r5
003a196c mov      r0, r6
003a1970 bl       #0x33dd2c ; _ZN10ObjectBase9GetHandleEv
003a1974 mov      r0, r6
003a1978 bl       #0x33ff54 ; _ZN12ObjectHandlecvP9CharacterEv
003a197c subs     r5, r0, #0
003a1980 beq      #0x3a1928
003a1984 ldr      r3, [r5]
003a1988 mov      lr, pc
003a198c ldr      pc, [r3, #0x28]
003a1990 cmp      r0, #0
003a1994 beq      #0x3a1928
003a1998 add      r6, r5, #0x560
003a199c mov      r0, r6
003a19a0 mov      r1, #0xda
003a19a4 mov      r2, #1
003a19a8 bl       #0x3e0798 ; _ZN14CharProperties12PROPS_AddIntEii
003a19ac ldr      r3, [pc, #0x17c]
003a19b0 mov      r0, r6
003a19b4 mov      r1, #0xda
003a19b8 ldr      r3, [r4, r3]
003a19bc mov      r2, #0
003a19c0 ldr      r6, [r3]
003a19c4 bl       #0x3df6e0 ; _ZNK14CharProperties12PROPS_GetIntEib
003a19c8 cmp      r0, #0x63
003a19cc ble      #0x3a1928
003a19d0 ldr      r3, [r4, r7]
003a19d4 mov      r1, r5
003a19d8 ldr      r0, [r3, #0x40]
003a19dc bl       #0x36effc ; _ZN13PlayerManager13IsLocalPlayerEPK9Character
003a19e0 cmp      r0, #0
003a19e4 beq      #0x3a1928
003a19e8 ldr      r3, [pc, #0x144]
003a19ec ldr      r3, [r4, r3]
003a19f0 ldr      r8, [r3]
003a19f4 cmp      r8, #0
003a19f8 beq      #0x3a1b20
003a19fc ldr      r3, [pc, #0x134]
003a1a00 ldr      r7, [pc, #0x134]
003a1a04 mov      r5, #0
003a1a08 ldr      r3, [r4, r3]
003a1a0c add      r7, pc, r7
003a1a10 ldr      r4, [r3]
003a1a14 b        #0x3a1a24
003a1a18 add      r5, r5, #1
003a1a1c cmp      r5, r8
003a1a20 beq      #0x3a1b20
003a1a24 ldr      r1, [r4, r5, lsl #2]
003a1a28 mov      r0, r7
003a1a2c bl       #0x30e31c
003a1a30 cmp      r0, #0
003a1a34 bne      #0x3a1a18
003a1a38 mov      r1, r5
003a1a3c mov      r0, r6
003a1a40 bl       #0x3813b8 ; _ZN13TrophyManager12UnlockTrophyEi
003a1a44 b        #0x3a1928
003a1a48 ldr      r3, [r6]
003a1a4c mov      r0, r6
003a1a50 mov      lr, pc
003a1a54 ldr      pc, [r3, #0xd0]
003a1a58 ldr      r1, [pc, #0xe0]
003a1a5c ldr      r2, [pc, #0xe0]
003a1a60 mov      sb, r0
003a1a64 add      r1, pc, r1
003a1a68 ldr      r0, [sl, #0x2c]
003a1a6c add      r2, pc, r2
003a1a70 ldr      sl, [r6, #0x64]
003a1a74 bl       #0x4c4bdc ; _ZNK15PyDataConstants11getConstantEPKcS1_
003a1a78 ldr      r2, [pc, #0xc8]
003a1a7c add      r1, sp, #0x30
003a1a80 mov      r3, #0
003a1a84 ldr      r2, [r4, r2]
003a1a88 str      r0, [sp, #0xc]
003a1a8c mov      r0, r8
003a1a90 add      r2, r2, #8
003a1a94 str      r2, [r1, #-0x28]!
003a1a98 mvn      r2, #0
003a1a9c strb     r3, [sp, #0x19]
003a1aa0 strb     r3, [sp, #0x18]
003a1aa4 str      sl, [sp, #0x14]
003a1aa8 str      r2, [sp, #0x1c]
003a1aac str      sb, [sp, #0x20]
003a1ab0 str      r5, [sp, #0x10]
003a1ab4 bl       #0x339090 ; _ZNK12EventManager10RaiseAsyncERK6IEvent
003a1ab8 ldr      r3, [pc, #0x8c]
003a1abc ldr      r3, [r4, r3]
003a1ac0 add      r3, r3, #8
003a1ac4 str      r3, [sp, #8]
003a1ac8 b        #0x3a1958
003a1acc ldr      r3, [pc, #0x7c]
003a1ad0 ldr      r3, [r4, r3]
003a1ad4 ldr      r3, [r3]
003a1ad8 cmp      r3, #2
003a1adc streq    r8, [r8]
003a1ae0 beq      #0x3a1944
003a1ae4 cmp      r3, #1
003a1ae8 bne      #0x3a1944
003a1aec ldr      r0, [pc, #0x60]
003a1af0 ldr      r1, [pc, #0x60]
003a1af4 ldr      r2, [pc, #0x60]
003a1af8 ldr      r0, [r4, r0]
003a1afc ldr      r3, [pc, #0x5c]
003a1b00 mov      ip, #0xf9
003a1b04 add      r1, pc, r1
003a1b08 add      r2, pc, r2
003a1b0c add      r3, pc, r3
003a1b10 add      r0, r0, #0xa8
003a1b14 str      ip, [sp]
003a1b18 bl       #0x30e004
003a1b1c b        #0x3a1944
003a1b20 mvn      r1, #0
003a1b24 b        #0x3a1a3c
003a1b28 subseq   r3, pc, r0, ror r1
003a1b2c strdeq   r3, r4, [r0], -r4
003a1b30 andeq    r1, r0, r0, ror sp
003a1b34 strdeq   r0, r1, [r0], -ip
003a1b38 andeq    r1, r0, ip, lsr #32
003a1b3c subseq   r1, r2, ip, ror r6
003a1b40 subseq   r0, r2, r4, lsl #30
003a1b44 subseq   r1, r2, ip, lsl #12
003a1b48 strdeq   r2, r3, [r0], -r8
003a1b4c strheq   r0, [r0], -r0
003a1b50 andeq    r3, r0, r0, asr #19
003a1b54 andeq    r1, r0, r0, asr #19
003a1b58 ldrsbeq  ip, [r1], #-0x84
003a1b5c subseq   sp, r6, r0, asr lr
003a1b60 subseq   r1, r2, r4, lsl r5
