_ZN7PODecorC1EP13PhysicalWorldP10GameObjectb.clone.2
0039fc2c push     {r4, r5, lr}
0039fc30 mov      lr, #1
0039fc34 sub      sp, sp, #0x24
0039fc38 mov      r5, #2
0039fc3c mov      ip, #0
0039fc40 mov      r3, lr
0039fc44 str      r5, [sp, #0x10]
0039fc48 ldr      r4, [pc, #0x40]
0039fc4c movw     r5, #0xffff
0039fc50 str      r5, [sp, #0x14]
0039fc54 str      ip, [sp, #0xc]
0039fc58 mov      r5, r0
0039fc5c str      ip, [sp]
0039fc60 str      ip, [sp, #4]
0039fc64 str      ip, [sp, #8]
0039fc68 str      lr, [sp, #0x18]
0039fc6c bl       #0x46f2f0 ; _ZN14PhysicalObjectC2EP13PhysicalWorldP10GameObjectbbbbstti
0039fc70 ldr      r3, [pc, #0x1c]
0039fc74 add      r4, pc, r4
0039fc78 mov      r0, r5
0039fc7c ldr      r3, [r4, r3]
0039fc80 add      r3, r3, #8
0039fc84 str      r3, [r5]
0039fc88 add      sp, sp, #0x24
0039fc8c pop      {r4, r5, pc}
0039fc90 subseq   r4, pc, ip, lsl lr
0039fc94 andeq    r2, r0, r8, lsl r4
_ZN10GameObject17SetPhysicalObjectEP14PhysicalObjectb
00394bf8 push     {r4, r5, r6, r7, r8, sb, sl, lr}
00394bfc ldr      r4, [pc, #0x120]
00394c00 ldr      r7, [pc, #0x120]
00394c04 ldr      ip, [pc, #0x120]
00394c08 add      r4, pc, r4
00394c0c ldr      r3, [r4, r7]
00394c10 ldr      r8, [r4, ip]
00394c14 sub      sp, sp, #0x20
00394c18 ldr      r3, [r3]
00394c1c add      r5, sp, #4
00394c20 mov      sl, r0
00394c24 mov      r0, r8
00394c28 str      r3, [sp, #0x1c]
00394c2c mov      sb, r2
00394c30 mov      r6, r1
00394c34 bl       #0x337888 ; _ZN13DebugSwitches4loadEv
00394c38 mov      r0, r5
00394c3c mov      r1, #0xd
00394c40 str      r5, [sp, #0x14]
00394c44 str      r5, [sp, #0x18]
00394c48 bl       #0x31167c ; _ZNSt4priv12_String_baseIcSaIcEE17_M_allocate_blockEj
00394c4c ldr      r1, [pc, #0xdc]
00394c50 mov      r2, #0xc
00394c54 ldr      r0, [sp, #0x18]
00394c58 add      r1, pc, r1
00394c5c bl       #0x30e868
00394c60 add      r3, r0, #0xc
00394c64 str      r3, [sp, #0x14]
00394c68 mov      r3, #0
00394c6c strb     r3, [r0, #0xc]
00394c70 mov      r1, r5
00394c74 mov      r0, r8
00394c78 bl       #0x337a88 ; _ZN13DebugSwitches9GetSwitchERKSs
00394c7c mov      r8, r0
00394c80 mov      r0, r5
00394c84 bl       #0x3139ac ; _ZNSt4priv12_String_baseIcSaIcEE19_M_deallocate_blockEv
00394c88 cmp      r8, #0
00394c8c beq      #0x394cc4
00394c90 cmp      r6, #0
00394c94 beq      #0x394ca8
00394c98 mov      r0, r6
00394c9c ldr      r3, [r6]
00394ca0 mov      lr, pc
00394ca4 ldr      pc, [r3, #4]
00394ca8 ldr      r3, [r4, r7]
00394cac ldr      r2, [sp, #0x1c]
00394cb0 ldr      r3, [r3]
00394cb4 cmp      r2, r3
00394cb8 bne      #0x394d20
00394cbc add      sp, sp, #0x20
00394cc0 pop      {r4, r5, r6, r7, r8, sb, sl, pc}
00394cc4 ldr      r3, [sl, #0x2dc]
00394cc8 cmp      r3, r6
00394ccc beq      #0x394d00
00394cd0 cmp      r3, #0
00394cd4 beq      #0x394cec
00394cd8 mov      r0, r3
00394cdc ldr      r3, [r3]
00394ce0 mov      lr, pc
00394ce4 ldr      pc, [r3, #4]
00394ce8 str      r8, [sl, #0x2dc]
00394cec cmp      r6, #0
00394cf0 str      r6, [sl, #0x2dc]
00394cf4 beq      #0x394d00
00394cf8 cmp      sb, #0
00394cfc bne      #0x394d0c
00394d00 mov      r0, sl
00394d04 bl       #0x393ea0 ; _ZN10GameObject14UpdatePFObjectEv
00394d08 b        #0x394ca8
00394d0c mov      r0, r6
00394d10 bl       #0x46eb20 ; _ZN14PhysicalObject3pinEv
00394d14 mov      r0, sl
00394d18 bl       #0x393ea0 ; _ZN10GameObject14UpdatePFObjectEv
00394d1c b        #0x394ca8
00394d20 bl       #0x30e310
00394d24 subseq   pc, pc, r8, lsl #29
00394d28 andeq    r4, r0, ip, lsr #1
00394d2c andeq    r0, r0, r4, lsl #17
00394d30 subseq   sp, r2, r8, lsr #24
