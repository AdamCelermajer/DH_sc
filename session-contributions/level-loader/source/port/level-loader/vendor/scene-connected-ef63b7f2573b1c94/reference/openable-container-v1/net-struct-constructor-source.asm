_ZN9Container18NetStructContainerC1Ev
0039ff58 push     {r3, r4, r5, r6, r7, r8, sb, sl, fp, lr}
0039ff5c ldr      r5, [pc, #0x190]
0039ff60 mov      r4, r0
0039ff64 bl       #0x8138f4 ; _ZN9NetStructC2Ev
0039ff68 ldr      r3, [pc, #0x188]
0039ff6c ldr      r6, [pc, #0x188]
0039ff70 add      r5, pc, r5
0039ff74 ldr      r3, [r5, r3]
0039ff78 ldr      r2, [r4, #0x150]
0039ff7c ldr      r0, [r5, r6]
0039ff80 add      r3, r3, #8
0039ff84 mov      r8, #0
0039ff88 mov      sb, #0
0039ff8c mov      ip, #0x138
0039ff90 strd     r8, sb, [r4, ip]
0039ff94 cmp      r2, #0
0039ff98 mvn      r1, #0
0039ff9c mov      r2, #0
0039ffa0 add      r0, r0, #8
0039ffa4 str      r3, [r4]
0039ffa8 mov      r3, #1
0039ffac str      r3, [r4, #0x134]
0039ffb0 str      r1, [r4, #0x144]
0039ffb4 str      r0, [r4, #0x130]
0039ffb8 str      r1, [r4, #0x140]
0039ffbc str      r2, [r4, #0x148]
0039ffc0 strb     r2, [r4, #0x14c]
0039ffc4 addeq    r8, r4, #0x130
0039ffc8 beq      #0x39ffdc
0039ffcc add      r8, r4, #0x130
0039ffd0 str      r2, [r4, #0x150]
0039ffd4 mov      r0, r8
0039ffd8 bl       #0x814f84 ; _ZN15NetStructMember10SetChangedEv
0039ffdc ldr      r2, [pc, #0x11c]
0039ffe0 ldr      r3, [pc, #0x11c]
0039ffe4 ldr      r1, [r4, #0x178]
0039ffe8 ldr      r2, [r5, r2]
0039ffec ldr      r3, [r5, r3]
0039fff0 mov      sl, #0
0039fff4 add      r2, r2, #8
0039fff8 mov      fp, #0
0039fffc mov      ip, #0x160
003a0000 strd     sl, fp, [r4, ip]
003a0004 cmp      r1, #0
003a0008 mvn      r0, #0
003a000c mov      r1, #0
003a0010 add      r3, r3, #8
003a0014 str      r2, [r4, #0x130]
003a0018 mov      r2, #0x10
003a001c str      r2, [r4, #0x15c]
003a0020 str      r0, [r4, #0x16c]
003a0024 str      r3, [r4, #0x158]
003a0028 str      r0, [r4, #0x168]
003a002c str      r1, [r4, #0x170]
003a0030 strb     r1, [r4, #0x174]
003a0034 addeq    r7, r4, #0x158
003a0038 beq      #0x3a004c
003a003c add      r7, r4, #0x158
003a0040 str      r1, [r4, #0x178]
003a0044 mov      r0, r7
003a0048 bl       #0x814f84 ; _ZN15NetStructMember10SetChangedEv
003a004c ldr      r3, [pc, #0xb4]
003a0050 ldr      r2, [r4, #0x1a0]
003a0054 ldr      r0, [r5, r6]
003a0058 ldr      r3, [r5, r3]
003a005c mov      sl, #0
003a0060 mov      fp, #0
003a0064 add      r3, r3, #8
003a0068 mov      ip, #0x188
003a006c strd     sl, fp, [r4, ip]
003a0070 cmp      r2, #0
003a0074 mvn      r1, #0
003a0078 mov      r2, #0
003a007c add      r0, r0, #8
003a0080 str      r3, [r4, #0x158]
003a0084 mov      r3, #0x20
003a0088 str      r3, [r4, #0x184]
003a008c str      r1, [r4, #0x194]
003a0090 str      r0, [r4, #0x180]
003a0094 str      r1, [r4, #0x190]
003a0098 str      r2, [r4, #0x198]
003a009c strb     r2, [r4, #0x19c]
003a00a0 addeq    r6, r4, #0x180
003a00a4 beq      #0x3a00b8
003a00a8 add      r6, r4, #0x180
003a00ac str      r2, [r4, #0x1a0]
003a00b0 mov      r0, r6
003a00b4 bl       #0x814f84 ; _ZN15NetStructMember10SetChangedEv
003a00b8 ldr      r3, [pc, #0x4c]
003a00bc mov      r1, r8
003a00c0 mov      r0, r4
003a00c4 ldr      r3, [r5, r3]
003a00c8 add      r3, r3, #8
003a00cc str      r3, [r4, #0x180]
003a00d0 bl       #0x81324c ; _ZN9NetStruct13DeclareMemberEP15NetStructMember
003a00d4 mov      r0, r4
003a00d8 mov      r1, r7
003a00dc bl       #0x81324c ; _ZN9NetStruct13DeclareMemberEP15NetStructMember
003a00e0 mov      r0, r4
003a00e4 mov      r1, r6
003a00e8 bl       #0x81324c ; _ZN9NetStruct13DeclareMemberEP15NetStructMember
003a00ec mov      r0, r4
003a00f0 pop      {r3, r4, r5, r6, r7, r8, sb, sl, fp, pc}
003a00f4 subseq   r4, pc, r0, lsr #22
003a00f8 strdeq   r0, r1, [r0], -r0
003a00fc andeq    r4, r0, r8, rrx
003a0100 strdeq   r3, r4, [r0], -r4
003a0104 andeq    r2, r0, r4, lsl #19
003a0108 andeq    r3, r0, ip, lsr r5
003a010c strdeq   r3, r4, [r0], -r0
