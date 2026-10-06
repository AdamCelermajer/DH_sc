_ZN17OpenableContainerC1EN10ObjectBase6GO_IDSE
003a1cb0 push     {r4, r5, r6, lr}
003a1cb4 ldr      r5, [pc, #0x68]
003a1cb8 mov      r4, r0
003a1cbc bl       #0x3a0788 ; _ZN9ContainerC2EN10ObjectBase6GO_IDSE
003a1cc0 ldr      r3, [pc, #0x60]
003a1cc4 add      r5, pc, r5
003a1cc8 add      r2, r4, #0x6f0
003a1ccc ldr      r3, [r5, r3]
003a1cd0 mov      r0, r2
003a1cd4 str      r2, [r4, #0x700]
003a1cd8 add      ip, r3, #8
003a1cdc add      r1, r3, #0x100
003a1ce0 add      r3, r3, #0xf4
003a1ce4 str      r3, [r4, #4]
003a1ce8 str      r1, [r4, #0x24]
003a1cec str      r2, [r4, #0x704]
003a1cf0 str      ip, [r4]
003a1cf4 mov      r1, #0x10
003a1cf8 bl       #0x31167c ; _ZNSt4priv12_String_baseIcSaIcEE17_M_allocate_blockEj
003a1cfc ldr      r2, [r4, #0x700]
003a1d00 mov      r1, #0
003a1d04 mov      r3, #1
003a1d08 strb     r1, [r2]
003a1d0c mvn      r2, #0
003a1d10 strb     r3, [r4, #0x70c]
003a1d14 str      r2, [r4, #0x710]
003a1d18 str      r3, [r4, #0x708]
003a1d1c mov      r0, r4
003a1d20 pop      {r4, r5, r6, pc}
003a1d24 subseq   r2, pc, ip, asr #27
003a1d28 strdeq   r0, r1, [r0], -ip
_ZN17OpenableContainer8InitPostEv
003a1b64 push     {r4, r5, r6, r7, r8, lr}
003a1b68 mov      r8, r0
003a1b6c bl       #0x39f910 ; _ZN9Container8InitPostEv
003a1b70 ldr      r5, [r8, #0x704]
003a1b74 ldr      r2, [r8, #0x700]
003a1b78 ldr      r3, [pc, #0x60]
003a1b7c cmp      r5, r2
003a1b80 add      r3, pc, r3
003a1b84 beq      #0x3a1bd4
003a1b88 ldr      r2, [pc, #0x54]
003a1b8c ldr      r2, [r3, r2]
003a1b90 ldr      r6, [r2]
003a1b94 cmp      r6, #0
003a1b98 beq      #0x3a1bd8
003a1b9c ldr      r2, [pc, #0x44]
003a1ba0 mov      r4, #0
003a1ba4 ldr      r3, [r3, r2]
003a1ba8 ldr      r7, [r3]
003a1bac b        #0x3a1bbc
003a1bb0 add      r4, r4, #1
003a1bb4 cmp      r4, r6
003a1bb8 beq      #0x3a1bd8
003a1bbc ldr      r1, [r7, r4, lsl #2]
003a1bc0 mov      r0, r5
003a1bc4 bl       #0x30e31c
003a1bc8 cmp      r0, #0
003a1bcc bne      #0x3a1bb0
003a1bd0 str      r4, [r8, #0x710]
003a1bd4 pop      {r4, r5, r6, r7, r8, pc}
003a1bd8 mvn      r4, #0
003a1bdc b        #0x3a1bd0
003a1be0 subseq   r2, pc, r0, lsl pc
003a1be4 andeq    r0, r0, r0, ror #26
003a1be8 andeq    r1, r0, r4, asr ip
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
_ZNK17OpenableContainer9GetVisualEv
003a1670 ldr      r0, [r0, #0x374]
003a1674 ldr      r3, [pc, #0x24]
003a1678 cmn      r0, #1
003a167c add      r3, pc, r3
003a1680 bxeq     lr
003a1684 ldr      r2, [pc, #0x18]
003a1688 ldr      r3, [r3, r2]
003a168c mov      r2, #0x28
003a1690 ldr      r3, [r3]
003a1694 mla      r0, r2, r0, r3
003a1698 ldr      r0, [r0, #0x24]
003a169c bx       lr
003a16a0 subseq   r3, pc, r4, lsl r4
003a16a4 ldrdeq   r3, r4, [r0], -ip
_ZNK17OpenableContainer9GetScriptEv
003a1634 ldr      r2, [r0, #0x374]
003a1638 ldr      r3, [pc, #0x28]
003a163c cmn      r2, #1
003a1640 add      r3, pc, r3
003a1644 moveq    r0, #0
003a1648 bxeq     lr
003a164c ldr      r1, [pc, #0x18]
003a1650 ldr      r3, [r3, r1]
003a1654 mov      r1, #0x28
003a1658 ldr      r3, [r3]
003a165c mla      r2, r1, r2, r3
003a1660 ldr      r0, [r2, #0x18]
003a1664 bx       lr
003a1668 subseq   r3, pc, r0, asr r4
003a166c ldrdeq   r3, r4, [r0], -ip
_ZN17OpenableContainer17DeclarePropertiesEv
003a1e8c push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
003a1e90 ldr      r4, [pc, #0x1d4]
003a1e94 ldr      r3, [pc, #0x1d4]
003a1e98 sub      sp, sp, #0x4c
003a1e9c add      r4, pc, r4
003a1ea0 ldr      ip, [r4, r3]
003a1ea4 add      r7, sp, #0x2c
003a1ea8 mov      fp, r0
003a1eac ldr      r3, [ip]
003a1eb0 str      ip, [sp]
003a1eb4 mov      sb, #0
003a1eb8 str      r3, [sp, #0x44]
003a1ebc bl       #0x3a083c ; _ZN9Container17DeclarePropertiesEv
003a1ec0 mov      r0, r7
003a1ec4 mov      r1, #0x10
003a1ec8 str      r7, [sp, #0x3c]
003a1ecc str      r7, [sp, #0x40]
003a1ed0 bl       #0x31167c ; _ZNSt4priv12_String_baseIcSaIcEE17_M_allocate_blockEj
003a1ed4 ldr      r3, [sp, #0x3c]
003a1ed8 add      r8, sp, #0x14
003a1edc mov      r0, r8
003a1ee0 strb     sb, [r3]
003a1ee4 ldr      r2, [sp, #0x3c]
003a1ee8 ldr      r1, [sp, #0x40]
003a1eec str      r8, [sp, #0x24]
003a1ef0 str      r8, [sp, #0x28]
003a1ef4 bl       #0x3116e8 ; _ZNSs19_M_range_initializeEPKcS0_
003a1ef8 mov      r1, sb
003a1efc mov      r0, #0x38
003a1f00 bl       #0x310570 ; _Znwj15MemoryHintState
003a1f04 ldr      r3, [pc, #0x168]
003a1f08 ldr      sl, [pc, #0x168]
003a1f0c mov      r6, r0
003a1f10 ldr      r3, [r4, r3]
003a1f14 add      sl, pc, sl
003a1f18 mov      r1, sl
003a1f1c add      r3, r3, #8
003a1f20 str      r3, [r0], #8
003a1f24 add      r2, sp, #0x10
003a1f28 str      r3, [sp, #4]
003a1f2c bl       #0x3140ec ; _ZNSsC1EPKcRKSaIcE
003a1f30 ldr      r2, [pc, #0x144]
003a1f34 add      r5, fp, #4
003a1f38 add      r1, fp, #0x6f0
003a1f3c ldr      r2, [r4, r2]
003a1f40 mov      r0, r6
003a1f44 rsb      r1, r5, r1
003a1f48 add      r2, r2, #8
003a1f4c str      r1, [r6, #4]
003a1f50 str      r2, [r0], #0x20
003a1f54 str      r0, [r6, #0x30]
003a1f58 str      r0, [r6, #0x34]
003a1f5c ldr      r1, [sp, #0x28]
003a1f60 ldr      r2, [sp, #0x24]
003a1f64 bl       #0x3116e8 ; _ZNSs19_M_range_initializeEPKcS0_
003a1f68 mov      r2, r6
003a1f6c mov      r1, sl
003a1f70 mov      r0, r5
003a1f74 bl       #0x513ce4 ; _ZN11PropertyMap11AddPropertyEPKcP8Property
003a1f78 mov      r0, r8
003a1f7c bl       #0x3139ac ; _ZNSt4priv12_String_baseIcSaIcEE19_M_deallocate_blockEv
003a1f80 mov      r0, r7
003a1f84 bl       #0x3139ac ; _ZNSt4priv12_String_baseIcSaIcEE19_M_deallocate_blockEv
003a1f88 mov      r1, sb
003a1f8c mov      r0, #0x24
003a1f90 bl       #0x310570 ; _Znwj15MemoryHintState
003a1f94 ldr      r7, [pc, #0xe4]
003a1f98 ldr      r3, [sp, #4]
003a1f9c mov      r6, r0
003a1fa0 add      r7, pc, r7
003a1fa4 str      r3, [r0], #8
003a1fa8 mov      r1, r7
003a1fac add      r2, sp, #0xc
003a1fb0 str      r3, [sp, #4]
003a1fb4 bl       #0x3140ec ; _ZNSsC1EPKcRKSaIcE
003a1fb8 ldr      r2, [pc, #0xc4]
003a1fbc add      fp, fp, #0x700
003a1fc0 add      r1, fp, #8
003a1fc4 ldr      r2, [r4, r2]
003a1fc8 rsb      r1, r5, r1
003a1fcc mov      r8, #1
003a1fd0 add      r2, r2, #8
003a1fd4 str      r1, [r6, #4]
003a1fd8 str      r2, [r6]
003a1fdc mov      r1, r7
003a1fe0 mov      r2, r6
003a1fe4 str      r8, [r6, #0x20]
003a1fe8 mov      r0, r5
003a1fec bl       #0x513ce4 ; _ZN11PropertyMap11AddPropertyEPKcP8Property
003a1ff0 mov      r1, sb
003a1ff4 mov      r0, #0x24
003a1ff8 bl       #0x310570 ; _Znwj15MemoryHintState
003a1ffc ldr      r7, [pc, #0x84]
003a2000 ldr      r3, [sp, #4]
003a2004 mov      r6, r0
003a2008 add      r7, pc, r7
003a200c str      r3, [r0], #8
003a2010 mov      r1, r7
003a2014 add      r2, sp, #8
003a2018 bl       #0x3140ec ; _ZNSsC1EPKcRKSaIcE
003a201c ldr      r3, [pc, #0x68]
003a2020 add      fp, fp, #0xc
003a2024 rsb      fp, r5, fp
003a2028 ldr      r3, [r4, r3]
003a202c mov      r2, r6
003a2030 str      fp, [r6, #4]
003a2034 add      r3, r3, #8
003a2038 str      r3, [r6]
003a203c strb     r8, [r6, #0x20]
003a2040 mov      r0, r5
003a2044 mov      r1, r7
003a2048 bl       #0x513ce4 ; _ZN11PropertyMap11AddPropertyEPKcP8Property
003a204c ldr      ip, [sp]
003a2050 ldr      r2, [sp, #0x44]
003a2054 ldr      r3, [ip]
003a2058 cmp      r2, r3
003a205c bne      #0x3a2068
003a2060 add      sp, sp, #0x4c
003a2064 pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
003a2068 bl       #0x30e310
003a206c ldrsheq  r2, [pc], #-0xb4
003a2070 andeq    r4, r0, ip, lsr #1
003a2074 andeq    r2, r0, r0, lsr r3
003a2078 subseq   r1, r2, r4, lsl #3
003a207c muleq    r0, r4, r4
003a2080 subseq   r1, r2, r8, lsl #2
003a2084 muleq    r0, r0, r5
003a2088 subseq   r1, r2, r8, lsr #1
003a208c andeq    r3, r0, ip, asr #28
_ZNK17OpenableContainer8IsLockedEv
003a16a8 ldr      r3, [r0, #0x394]
003a16ac sub      r3, r3, #3
003a16b0 cmp      r3, #1
003a16b4 movls    r0, #0
003a16b8 bxls     lr
003a16bc ldr      r0, [r0, #0x710]
003a16c0 adds     r0, r0, #1
003a16c4 movne    r0, #1
003a16c8 bx       lr
_ZNK17OpenableContainer7GetLootEv
003a1804 push     {r4, lr}
003a1808 ldr      r0, [r0, #0x38c]
003a180c bl       #0x3a1708 ; _ZN6Arrays19GetMemberIDByStringINS_18OpenableContainersEEEiPKc
003a1810 ldr      r4, [pc, #0x24]
003a1814 cmn      r0, #1
003a1818 add      r4, pc, r4
003a181c beq      #0x3a1838
003a1820 ldr      r3, [pc, #0x18]
003a1824 mov      r2, #0x28
003a1828 ldr      r3, [r4, r3]
003a182c ldr      r3, [r3]
003a1830 mla      r0, r2, r0, r3
003a1834 ldr      r0, [r0, #0x10]
003a1838 pop      {r4, pc}
003a183c subseq   r3, pc, r8, ror r2
003a1840 ldrdeq   r3, r4, [r0], -ip
_ZNK17OpenableContainer8GetSoundEv
003a17c4 push     {r4, lr}
003a17c8 ldr      r0, [r0, #0x38c]
003a17cc bl       #0x3a1708 ; _ZN6Arrays19GetMemberIDByStringINS_18OpenableContainersEEEiPKc
003a17d0 ldr      r4, [pc, #0x24]
003a17d4 cmn      r0, #1
003a17d8 add      r4, pc, r4
003a17dc beq      #0x3a17f8
003a17e0 ldr      r3, [pc, #0x18]
003a17e4 mov      r2, #0x28
003a17e8 ldr      r3, [r4, r3]
003a17ec ldr      r3, [r3]
003a17f0 mla      r0, r2, r0, r3
003a17f4 ldr      r0, [r0, #8]
003a17f8 pop      {r4, pc}
003a17fc ldrheq   r3, [pc], #-0x28
003a1800 ldrdeq   r3, r4, [r0], -ip
_ZN17OpenableContainer6UnlockEP9Character
003a1860 cmp      r1, #0
003a1864 push     {r4, r5, r6, lr}
003a1868 mov      r4, r0
003a186c beq      #0x3a18a4
003a1870 add      r5, r1, #0x37c
003a1874 mov      r0, r5
003a1878 ldr      r1, [r4, #0x710]
003a187c bl       #0x3fd15c ; _ZN13ItemInventory8FindItemEi
003a1880 cmp      r0, #0
003a1884 beq      #0x3a18a4
003a1888 ldrsh    r3, [r0, #0x50]
003a188c ldr      r2, [r4, #0x708]
003a1890 cmp      r2, r3
003a1894 bgt      #0x3a18a4
003a1898 ldrb     r3, [r4, #0x70c]
003a189c cmp      r3, #0
003a18a0 bne      #0x3a18ac
003a18a4 mov      r0, #0
003a18a8 pop      {r4, r5, r6, pc}
003a18ac ldr      r1, [r4, #0x710]
003a18b0 mov      r0, r5
003a18b4 pop      {r4, r5, r6, lr}
003a18b8 b        #0x3fe658
_ZN17OpenableContainer12TryUnlockingEP10GameObject
003a18bc push     {r4, r5, r6, lr}
003a18c0 mov      r5, r1
003a18c4 mov      r4, r0
003a18c8 bl       #0x3a16a8 ; _ZNK17OpenableContainer8IsLockedEv
003a18cc cmp      r0, #0
003a18d0 bne      #0x3a18dc
003a18d4 mov      r0, #1
003a18d8 pop      {r4, r5, r6, pc}
003a18dc ldr      r3, [r5]
003a18e0 mov      r0, r5
003a18e4 mov      lr, pc
003a18e8 ldr      pc, [r3, #0x24]
003a18ec cmp      r0, #0
003a18f0 movne    r1, r5
003a18f4 moveq    r1, #0
003a18f8 mov      r0, r4
003a18fc pop      {r4, r5, r6, lr}
003a1900 b        #0x3a1860
