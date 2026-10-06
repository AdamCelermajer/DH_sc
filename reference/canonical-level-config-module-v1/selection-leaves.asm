_ZSt7getlineIcSt11char_traitsIcESaIcEERSt13basic_istreamIT_T0_ES7_RSbIS4_S5_T1_ES4_.clone.8
0038a258 push     {r4, r5, r6, r7, r8, lr}
0038a25c mov      r6, r1
0038a260 mov      r7, r0
0038a264 bl       #0x30f6e8 ; _ZSt14_M_init_noskipIcSt11char_traitsIcEEbRSt13basic_istreamIT_T0_E
0038a268 cmp      r0, #0
0038a26c bne      #0x38a2a8
0038a270 ldr      r3, [r7]
0038a274 ldr      r0, [r3, #-0xc]
0038a278 add      r0, r7, r0
0038a27c ldr      r3, [r0, #0x48]
0038a280 ldr      r2, [r0, #8]
0038a284 cmp      r3, #0
0038a288 orr      r3, r2, #4
0038a28c orreq    r3, r2, #5
0038a290 ldr      r2, [r0, #0x14]
0038a294 str      r3, [r0, #8]
0038a298 tst      r3, r2
0038a29c bne      #0x38a380
0038a2a0 mov      r0, r7
0038a2a4 pop      {r4, r5, r6, r7, r8, pc}
0038a2a8 ldr      r2, [r7]
0038a2ac ldr      r3, [r6, #0x14]
0038a2b0 ldr      r1, [r6, #0x10]
0038a2b4 ldr      r2, [r2, #-0xc]
0038a2b8 mov      r5, #0
0038a2bc cmp      r3, r1
0038a2c0 add      r2, r7, r2
0038a2c4 ldr      r4, [r2, #0x48]
0038a2c8 movne    r2, #0
0038a2cc strbne   r2, [r3]
0038a2d0 ldrne    r3, [r6, #0x14]
0038a2d4 strne    r3, [r6, #0x10]
0038a2d8 b        #0x38a308
0038a2dc str      r1, [r4, #8]
0038a2e0 ldrb     r0, [r3]
0038a2e4 sxtb     r3, r0
0038a2e8 cmp      r3, #0x2c
0038a2ec mov      r1, r3
0038a2f0 mov      r0, r6
0038a2f4 add      r5, r5, #1
0038a2f8 beq      #0x38a370
0038a2fc bl       #0x32a25c ; _ZNSs9push_backEc
0038a300 cmn      r5, #2
0038a304 beq      #0x38a370
0038a308 ldr      r3, [r4, #8]
0038a30c ldr      r2, [r4, #0xc]
0038a310 add      r1, r3, #1
0038a314 cmp      r3, r2
0038a318 blo      #0x38a2dc
0038a31c ldr      r3, [r4]
0038a320 mov      r0, r4
0038a324 mov      lr, pc
0038a328 ldr      pc, [r3, #0x24]
0038a32c cmn      r0, #1
0038a330 bne      #0x38a2e4
0038a334 ldr      r3, [r7]
0038a338 ldr      r0, [r3, #-0xc]
0038a33c add      r0, r7, r0
0038a340 ldr      r3, [r0, #0x48]
0038a344 ldr      r2, [r0, #8]
0038a348 cmp      r3, #0
0038a34c orr      r3, r2, #2
0038a350 orreq    r3, r2, #3
0038a354 ldr      r2, [r0, #0x14]
0038a358 str      r3, [r0, #8]
0038a35c tst      r3, r2
0038a360 beq      #0x38a368
0038a364 bl       #0x708f20 ; ___ZNSt8ios_base16_M_throw_failureEv_veneer
0038a368 cmp      r5, #0
0038a36c beq      #0x38a270
0038a370 cmn      r5, #3
0038a374 bhi      #0x38a270
0038a378 mov      r0, r7
0038a37c pop      {r4, r5, r6, r7, r8, pc}
0038a380 bl       #0x708f20 ; ___ZNSt8ios_base16_M_throw_failureEv_veneer
0038a384 mov      r0, r7
0038a388 pop      {r4, r5, r6, r7, r8, pc}
_ZN6Random9GetRandomEib.clone.3
00388c58 push     {r4, lr}
00388c5c ldr      r4, [pc, #0x7c]
00388c60 cmp      r0, #0
00388c64 add      r4, pc, r4
00388c68 beq      #0x388cc8
00388c6c ldr      r2, [pc, #0x70]
00388c70 mov      r1, r0
00388c74 movw     r0, #0xe6ab
00388c78 ldr      r2, [r4, r2]
00388c7c movw     r3, #0xdb17
00388c80 movt     r3, #0x2b52
00388c84 ldr      lr, [r2]
00388c88 movw     ip, #0xf26b
00388c8c movt     ip, #0xda
00388c90 mul      r0, r0, lr
00388c94 add      r0, r0, #0x2b000
00388c98 add      r0, r0, #0x3fc
00388c9c add      r0, r0, #1
00388ca0 umull    lr, r3, r3, r0
00388ca4 rsb      lr, r3, r0
00388ca8 add      r3, r3, lr, lsr #1
00388cac lsr      r3, r3, #0x17
00388cb0 mls      r3, ip, r3, r0
00388cb4 mov      r0, r3
00388cb8 str      r3, [r2]
00388cbc bl       #0x30eb2c
00388cc0 eor      r0, r1, r1, asr #31
00388cc4 sub      r0, r0, r1, asr #31
00388cc8 ldr      r3, [pc, #0x18]
00388ccc ldr      r3, [r4, r3]
00388cd0 ldr      r2, [r3]
00388cd4 add      r2, r2, #1
00388cd8 str      r2, [r3]
00388cdc pop      {r4, pc}
00388ce0 rsbeq    fp, r0, ip, lsr #28
00388ce4 muleq    r0, r4, ip
00388ce8 andeq    r1, r0, r8, lsl #1
_Z12Color255To01R7Point3DIfE
003ef1b8 push     {r4, lr}
003ef1bc mov      r1, #0x43000000
003ef1c0 mov      r4, r0
003ef1c4 add      r1, r1, #0x7f0000
003ef1c8 ldr      r0, [r0]
003ef1cc bl       #0x30ec94
003ef1d0 mov      r1, #0x43000000
003ef1d4 str      r0, [r4]
003ef1d8 add      r1, r1, #0x7f0000
003ef1dc ldr      r0, [r4, #4]
003ef1e0 bl       #0x30ec94
003ef1e4 mov      r1, #0x43000000
003ef1e8 str      r0, [r4, #4]
003ef1ec add      r1, r1, #0x7f0000
003ef1f0 ldr      r0, [r4, #8]
003ef1f4 bl       #0x30ec94
003ef1f8 str      r0, [r4, #8]
003ef1fc pop      {r4, pc}
_ZN5Level17SetObjectModuleIdEi
003ef278 str      r1, [r0, #0x18c]
003ef27c bx       lr
