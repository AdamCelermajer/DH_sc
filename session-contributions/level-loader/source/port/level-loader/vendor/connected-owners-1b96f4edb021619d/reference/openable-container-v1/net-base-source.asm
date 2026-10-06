_ZN9Container25PopulateOutgoingNetStructEb
003a0110 push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
003a0114 ldr      r4, [pc, #0x1e8]
003a0118 ldr      r6, [pc, #0x1e8]
003a011c ldr      r3, [r0, #0x394]
003a0120 sub      sp, sp, #0x7c
003a0124 add      r4, pc, r4
003a0128 ldr      r2, [sp, #0x70]
003a012c ldr      ip, [r4, r6]
003a0130 sub      r3, r3, #3
003a0134 cmp      r3, #1
003a0138 movhi    r3, #0
003a013c movls    r3, #1
003a0140 cmp      r2, r3
003a0144 mov      r8, #0
003a0148 mov      r2, #0
003a014c mov      r5, r0
003a0150 add      ip, ip, #8
003a0154 mvn      r0, #0
003a0158 mov      lr, #1
003a015c mov      sb, #0
003a0160 strd     r8, sb, [sp, #0x58]
003a0164 str      lr, [sp, #0x54]
003a0168 str      r0, [sp, #0x64]
003a016c strb     r2, [sp, #0x6c]
003a0170 str      ip, [sp, #0x50]
003a0174 mov      r8, r1
003a0178 str      r0, [sp, #0x60]
003a017c str      r2, [sp, #0x68]
003a0180 addeq    r7, sp, #0x50
003a0184 beq      #0x3a0198
003a0188 add      r7, sp, #0x50
003a018c mov      r0, r7
003a0190 str      r3, [sp, #0x70]
003a0194 bl       #0x814f84 ; _ZN15NetStructMember10SetChangedEv
003a0198 ldr      r2, [pc, #0x16c]
003a019c add      r1, r7, #0x20
003a01a0 ldr      r3, [r5, #0x4d0]
003a01a4 ldr      r2, [r4, r2]
003a01a8 add      r0, r5, #0x4d0
003a01ac ldr      r7, [pc, #0x15c]
003a01b0 add      r2, r2, #8
003a01b4 str      r2, [sp, #0x50]
003a01b8 mov      lr, pc
003a01bc ldr      pc, [r3, #0x1c]
003a01c0 ldr      r3, [pc, #0x14c]
003a01c4 ldr      r2, [r4, r7]
003a01c8 ldr      r1, [r5, #0x398]
003a01cc ldr      r3, [r4, r3]
003a01d0 add      r2, r2, #8
003a01d4 str      r2, [sp, #0x50]
003a01d8 ldr      r0, [r3, #0x38]
003a01dc bl       #0x3402f4 ; _ZN13ObjectManager24GetNetworkIdByObjectBaseEPK10ObjectBase
003a01e0 ldr      r2, [pc, #0x130]
003a01e4 ldr      r1, [sp, #0x48]
003a01e8 mov      r3, r0
003a01ec ldr      r2, [r4, r2]
003a01f0 mvn      r0, #0
003a01f4 cmp      r3, r1
003a01f8 mov      sl, #0
003a01fc mov      r1, #0
003a0200 add      r2, r2, #8
003a0204 mov      ip, #0x10
003a0208 mov      fp, #0
003a020c strd     sl, fp, [sp, #0x30]
003a0210 str      ip, [sp, #0x2c]
003a0214 str      r0, [sp, #0x3c]
003a0218 strb     r1, [sp, #0x44]
003a021c str      r2, [sp, #0x28]
003a0220 str      r0, [sp, #0x38]
003a0224 str      r1, [sp, #0x40]
003a0228 addeq    sl, sp, #0x28
003a022c beq      #0x3a0240
003a0230 add      sl, sp, #0x28
003a0234 mov      r0, sl
003a0238 str      r3, [sp, #0x48]
003a023c bl       #0x814f84 ; _ZN15NetStructMember10SetChangedEv
003a0240 ldr      r2, [pc, #0xd4]
003a0244 add      r0, r5, #0x4f0
003a0248 ldr      r3, [r5, #0x4f8]
003a024c ldr      r2, [r4, r2]
003a0250 add      r0, r0, #8
003a0254 add      r1, sl, #0x20
003a0258 add      r2, r2, #8
003a025c str      r2, [sp, #0x28]
003a0260 mov      lr, pc
003a0264 ldr      pc, [r3, #0x1c]
003a0268 cmp      r8, #0
003a026c beq      #0x3a02fc
003a0270 ldr      ip, [r4, r7]
003a0274 ldr      r3, [r5, #0xfc]
003a0278 ldr      r0, [r4, r6]
003a027c ldr      r2, [sp, #0x20]
003a0280 add      ip, ip, #8
003a0284 mvn      r1, #0
003a0288 cmp      r3, r2
003a028c mov      r6, #0
003a0290 mov      r2, #0
003a0294 add      r0, r0, #8
003a0298 str      ip, [sp, #0x28]
003a029c mov      r7, #0
003a02a0 mov      ip, #0x20
003a02a4 strd     r6, r7, [sp, #8]
003a02a8 str      ip, [sp, #4]
003a02ac str      r1, [sp, #0x14]
003a02b0 strb     r2, [sp, #0x1c]
003a02b4 str      r0, [sp]
003a02b8 str      r1, [sp, #0x10]
003a02bc str      r2, [sp, #0x18]
003a02c0 moveq    r6, sp
003a02c4 beq      #0x3a02d8
003a02c8 mov      r0, sp
003a02cc mov      r6, sp
003a02d0 str      r3, [sp, #0x20]
003a02d4 bl       #0x814f84 ; _ZN15NetStructMember10SetChangedEv
003a02d8 ldr      r2, [pc, #0x40]
003a02dc ldr      r3, [r5, #0x520]
003a02e0 add      r0, r5, #0x520
003a02e4 ldr      r2, [r4, r2]
003a02e8 add      r1, r6, #0x20
003a02ec add      r2, r2, #8
003a02f0 str      r2, [sp]
003a02f4 mov      lr, pc
003a02f8 ldr      pc, [r3, #0x1c]
003a02fc add      sp, sp, #0x7c
003a0300 pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
003a0304 subseq   r4, pc, ip, ror #18
003a0308 andeq    r4, r0, r8, rrx
003a030c strdeq   r3, r4, [r0], -r4
003a0310 andeq    r1, r0, r8, lsr #1
003a0314 strdeq   r3, r4, [r0], -r4
003a0318 andeq    r2, r0, r4, lsl #19
003a031c andeq    r3, r0, ip, lsr r5
003a0320 strdeq   r3, r4, [r0], -r0
_ZN9NetStructC2Ev
008138f4 ldr      r3, [pc, #0x58]
008138f8 ldr      r2, [pc, #0x58]
008138fc push     {r4, lr}
00813900 add      r3, pc, r3
00813904 ldr      r2, [r3, r2]
00813908 mov      r4, r0
0081390c mov      r1, #0
00813910 add      r2, r2, #8
00813914 str      r2, [r4]
00813918 str      r1, [r4, #0x104]
0081391c strb     r1, [r4, #0x108]
00813920 str      r1, [r4, #0x110]
00813924 strb     r1, [r0, #0x10c]!
00813928 str      r0, [r4, #0x118]
0081392c str      r0, [r4, #0x114]
00813930 str      r1, [r4, #0x11c]
00813934 strb     r1, [r4, #0x124]
00813938 strb     r1, [r4, #0x125]
0081393c str      r1, [r4, #0x128]
00813940 add      r0, r4, #4
00813944 mov      r2, #0x100
00813948 bl       #0x30e460
0081394c mov      r0, r4
00813950 pop      {r4, pc}
00813954 mulseq   r8, r0, r1
00813958 andeq    r4, r0, r4, asr #7
_ZN9NetStruct13DeclareMemberEP15NetStructMember
0081324c ldr      r3, [r0, #0x104]
00813250 add      r2, r3, #1
00813254 add      r3, r0, r3, lsl #2
00813258 str      r1, [r3, #4]
0081325c str      r2, [r0, #0x104]
00813260 bx       lr
