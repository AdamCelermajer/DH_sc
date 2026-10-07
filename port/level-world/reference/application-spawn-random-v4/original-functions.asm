_GLOBAL__I_.._.._sources_Utils_Random.cpp
003136dc ldr      r2, [pc, #0x30]
003136e0 ldr      r1, [pc, #0x30]
003136e4 ldr      r3, [pc, #0x30]
003136e8 add      r2, pc, r2
003136ec ldr      r0, [r2, r1]
003136f0 add      r3, pc, r3
003136f4 mov      r1, #0x3f000000
003136f8 mov      ip, #0
003136fc str      r1, [r3, #8]
00313700 str      ip, [r0, #4]
00313704 str      r1, [r3]
00313708 str      r1, [r3, #4]
0031370c str      ip, [r0]
00313710 bx       lr
00313714 rsbeq    r1, r8, r8, lsr #7
00313718 andeq    r1, r0, r8, lsl #1
0031371c mlseq    r8, r8, r1, ip
_ZN6Random9GetRandomEib
0033ff90 push     {r4, r5, r6, lr}
0033ff94 ldr      r4, [pc, #0xa8]
0033ff98 subs     r5, r1, #0
0033ff9c add      r4, pc, r4
0033ffa0 beq      #0x340034
0033ffa4 ldr      r3, [pc, #0x9c]
0033ffa8 ldr      r2, [r4, r3]
0033ffac mov      r3, r2
0033ffb0 ldr      r2, [r2]
0033ffb4 cmp      r0, #0
0033ffb8 beq      #0x34000c
0033ffbc movw     r1, #0xe6ab
0033ffc0 mul      r2, r1, r2
0033ffc4 movw     ip, #0xdb17
0033ffc8 add      r1, r2, #0x2b000
0033ffcc add      r1, r1, #0x3fc
0033ffd0 add      r1, r1, #1
0033ffd4 movt     ip, #0x2b52
0033ffd8 umull    lr, r2, ip, r1
0033ffdc movw     ip, #0xf26b
0033ffe0 rsb      lr, r2, r1
0033ffe4 add      r2, r2, lr, lsr #1
0033ffe8 movt     ip, #0xda
0033ffec lsr      r2, r2, #0x17
0033fff0 mls      r2, ip, r2, r1
0033fff4 mov      r1, r0
0033fff8 str      r2, [r3]
0033fffc mov      r0, r2
00340000 bl       #0x30eb2c
00340004 eor      r0, r1, r1, asr #31
00340008 sub      r0, r0, r1, asr #31
0034000c ldr      r3, [pc, #0x38]
00340010 cmp      r5, #0
00340014 ldr      r3, [r4, r3]
00340018 ldrne    r2, [r3, #4]
0034001c ldreq    r2, [r3]
00340020 addne    r2, r2, #1
00340024 addeq    r2, r2, #1
00340028 strne    r2, [r3, #4]
0034002c streq    r2, [r3]
00340030 pop      {r4, r5, r6, pc}
00340034 ldr      r3, [pc, #0x14]
00340038 ldr      r3, [r4, r3]
0034003c ldr      r2, [r3]
00340040 b        #0x33ffb4
_ZN10ObjectBase6DeleteEv
0033ddb4 mov      r3, #2
0033ddb8 strb     r3, [r0, #0x82]
0033ddbc mov      r3, #1
0033ddc0 strb     r3, [r0, #0x81]
0033ddc4 bx       lr
_ZN11ApplicationC1Ev
0032d79c push     {r4, r5, r6, r7, r8, lr}
0032d7a0 ldr      r6, [pc, #0x1bc]
0032d7a4 ldr      r3, [pc, #0x1bc]
0032d7a8 mov      r2, r0
0032d7ac add      r6, pc, r6
0032d7b0 ldr      r3, [r6, r3]
0032d7b4 mov      r1, #0x1e
0032d7b8 mov      r4, r0
0032d7bc add      r3, r3, #8
0032d7c0 str      r3, [r2], #8
0032d7c4 str      r1, [r0, #0x6c]
0032d7c8 mov      r1, #0x3f800000
0032d7cc mov      r5, #0
0032d7d0 str      r1, [r0, #0x9c]
0032d7d4 mov      r7, #1
0032d7d8 add      r3, r0, #0xbc
0032d7dc mvn      r1, #0
0032d7e0 str      r2, [r0, #0xc]
0032d7e4 str      r2, [r0, #8]
0032d7e8 str      r1, [r0, #0xa0]
0032d7ec mov      r0, r3
0032d7f0 str      r5, [r4, #0x10]
0032d7f4 str      r5, [r4, #0x14]
0032d7f8 str      r5, [r4, #0x18]
0032d7fc str      r5, [r4, #0x1c]
0032d800 str      r5, [r4, #0x20]
0032d804 str      r5, [r4, #0x24]
0032d808 str      r5, [r4, #0x28]
0032d80c str      r5, [r4, #0x2c]
0032d810 str      r5, [r4, #0x30]
0032d814 str      r5, [r4, #0x34]
0032d818 str      r5, [r4, #0x38]
0032d81c str      r5, [r4, #0x3c]
0032d820 str      r5, [r4, #0x40]
0032d824 str      r5, [r4, #0x44]
0032d828 str      r5, [r4, #0x48]
0032d82c str      r5, [r4, #0x4c]
0032d830 str      r5, [r4, #0x50]
0032d834 str      r5, [r4, #0x74]
0032d838 strb     r5, [r4, #0x78]
0032d83c strb     r5, [r4, #0x79]
0032d840 strb     r5, [r4, #0x7a]
0032d844 str      r5, [r4, #0x88]
0032d848 strb     r5, [r4, #0xa4]
0032d84c strb     r5, [r4, #0xa5]
0032d850 strb     r5, [r4, #0xa6]
0032d854 mov      r1, #0x10
0032d858 strb     r5, [r4, #0xa7]
0032d85c strb     r5, [r4, #0xa8]
0032d860 strb     r5, [r4, #0xa9]
0032d864 strb     r5, [r4, #0xaa]
0032d868 strb     r5, [r4, #0xad]
0032d86c str      r3, [r4, #0xcc]
0032d870 str      r3, [r4, #0xd0]
0032d874 strb     r7, [r4, #0xab]
0032d878 strb     r7, [r4, #0xb4]
0032d87c bl       #0x31167c ; _ZNSt4priv12_String_baseIcSaIcEE17_M_allocate_blockEj
0032d880 ldr      r2, [r4, #0xcc]
0032d884 add      r3, r4, #0xd4
0032d888 mov      r0, r3
0032d88c strb     r5, [r2]
0032d890 mov      r1, #0x10
0032d894 str      r3, [r4, #0xe4]
0032d898 str      r3, [r4, #0xe8]
0032d89c bl       #0x31167c ; _ZNSt4priv12_String_baseIcSaIcEE17_M_allocate_blockEj
0032d8a0 ldr      r3, [pc, #0xc4]
0032d8a4 ldr      r2, [r4, #0xe4]
0032d8a8 mov      r0, #0x10
0032d8ac ldr      r3, [r6, r3]
0032d8b0 strb     r5, [r2]
0032d8b4 strb     r5, [r4, #0xee]
0032d8b8 strb     r5, [r4, #0xed]
0032d8bc strb     r5, [r4, #0x10e]
0032d8c0 strb     r7, [r4, #0x10f]
0032d8c4 strb     r7, [r3]
0032d8c8 bl       #0x310454 ; _Z11CustomAllocj
0032d8cc mov      r1, r7
0032d8d0 mov      r5, r0
0032d8d4 bl       #0x317e44 ; _ZN16updateJob_threadC1Ei
0032d8d8 str      r5, [r4, #4]
0032d8dc mov      r0, #0x10
0032d8e0 bl       #0x310454 ; _Z11CustomAllocj
0032d8e4 mov      r1, #2
0032d8e8 mov      r5, r0
0032d8ec bl       #0x317e44 ; _ZN16updateJob_threadC1Ei
0032d8f0 ldr      r3, [pc, #0x78]
0032d8f4 mov      r0, #0x10
0032d8f8 ldr      r3, [r6, r3]
0032d8fc str      r5, [r3]
0032d900 bl       #0x310454 ; _Z11CustomAllocj
0032d904 mov      r1, #3
0032d908 mov      r5, r0
0032d90c bl       #0x317e44 ; _ZN16updateJob_threadC1Ei
0032d910 ldr      r3, [pc, #0x5c]
0032d914 mov      r0, #0x10
0032d918 ldr      r3, [r6, r3]
0032d91c str      r5, [r3]
0032d920 bl       #0x310454 ; _Z11CustomAllocj
0032d924 mov      r1, #4
0032d928 mov      r5, r0
0032d92c bl       #0x317e44 ; _ZN16updateJob_threadC1Ei
0032d930 ldr      r3, [pc, #0x40]
0032d934 mov      r0, #0x10
0032d938 ldr      r3, [r6, r3]
0032d93c str      r5, [r3]
0032d940 bl       #0x310454 ; _Z11CustomAllocj
0032d944 mov      r1, #5
0032d948 mov      r5, r0
0032d94c bl       #0x317e44 ; _ZN16updateJob_threadC1Ei
0032d950 ldr      r3, [pc, #0x24]
0032d954 mov      r0, r4
0032d958 ldr      r3, [r6, r3]
0032d95c str      r5, [r3]
0032d960 pop      {r4, r5, r6, r7, r8, pc}
0032d964 rsbeq    r7, r6, r4, ror #5
0032d968 andeq    r0, r0, ip, asr #27
0032d96c andeq    r3, r0, r8, asr sp
0032d970 andeq    r4, r0, r0, asr #16
0032d974 strheq   r0, [r0], -ip
0032d978 muleq    r0, r8, r5
0032d97c andeq    r1, r0, r0, asr #26
_ZN10GameObject21CheckSpawnProbabilityEv
0038bd64 push     {r4, r5, lr}
0038bd68 sub      sp, sp, #0x14
0038bd6c add      r4, sp, #4
0038bd70 mov      r1, r0
0038bd74 mov      r5, r0
0038bd78 mov      r0, r4
0038bd7c bl       #0x33dd2c ; _ZN10ObjectBase9GetHandleEv
0038bd80 mov      r0, r4
0038bd84 bl       #0x33ff54 ; _ZN12ObjectHandlecvP9CharacterEv
0038bd88 ldr      r4, [pc, #0xc4]
0038bd8c subs     r3, r0, #0
0038bd90 add      r4, pc, r4
0038bd94 beq      #0x38bdbc
0038bd98 ldr      r3, [r3]
0038bd9c mov      lr, pc
0038bda0 ldr      pc, [r3, #0x28]
0038bda4 cmp      r0, #0
0038bda8 beq      #0x38bdbc
0038bdac mvn      r0, #1
0038bdb0 str      r0, [r5, #0x270]
0038bdb4 add      sp, sp, #0x14
0038bdb8 pop      {r4, r5, pc}
0038bdbc ldr      r0, [r5, #0x270]
0038bdc0 cmn      r0, #1
0038bdc4 bne      #0x38bdb4
0038bdc8 bl       #0x7fd794 ; _Z9GetOnlinev
0038bdcc ldrb     r3, [r0, #5]
0038bdd0 cmp      r3, #0
0038bdd4 beq      #0x38be44
0038bdd8 ldr      r3, [r5, #0x108]
0038bddc cmn      r3, #1
0038bde0 beq      #0x38be44
0038bde4 ldr      r0, [r5, #0xfc]
0038bde8 subs     r0, r0, #0
0038bdec movne    r0, #1
0038bdf0 bl       #0x38bc3c ; _ZN6Random9GetRandomEib.clone.2
0038bdf4 str      r0, [r5, #0x270]
0038bdf8 ldr      r3, [r5, #0x274]
0038bdfc cmp      r0, r3
0038be00 blt      #0x38bdac
0038be04 mov      r1, #0
0038be08 ldr      r3, [r5]
0038be0c mov      r0, r5
0038be10 mov      lr, pc
0038be14 ldr      pc, [r3, #0x40]
0038be18 mov      r0, r5
0038be1c bl       #0x33ddb4 ; _ZN10ObjectBase6DeleteEv
0038be20 mov      r3, #0
0038be24 strb     r3, [r5, #0x82]
0038be28 ldr      r3, [pc, #0x28]
0038be2c mov      r1, r5
0038be30 ldr      r3, [r4, r3]
0038be34 ldr      r0, [r3, #0x38]
0038be38 bl       #0x3432f8 ; _ZN13ObjectManager15MarkForDeletionEP10ObjectBase
0038be3c ldr      r0, [r5, #0x270]
0038be40 b        #0x38bdb4
0038be44 mov      r0, #0
0038be48 bl       #0x38bc3c ; _ZN6Random9GetRandomEib.clone.2
0038be4c str      r0, [r5, #0x270]
0038be50 b        #0x38bdf8
0038be54 rsbeq    r8, r0, r0, lsl #26
0038be58 strdeq   r3, r4, [r0], -r4
