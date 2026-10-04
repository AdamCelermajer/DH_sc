
# _ZN16CharStateMachine15SM_SetIdleStateEb
003c1a00: strb     r1, [r0, #0x3c]
003c1a04: mvn      r2, #0
003c1a08: mov      r1, #3
003c1a0c: mov      r3, #0
003c1a10: b        #0x3c1938

# _ZN16CharStateMachine9_SetStateEiiPv
003c1938: push     {r4, r5, r6, r7, r8, sl, lr}
003c193c: ldr      ip, [r0, #0x20]
003c1940: sub      sp, sp, #0x14
003c1944: mov      r4, r0
003c1948: cmp      ip, #0
003c194c: mvneq    r7, #0
003c1950: mov      r5, r1
003c1954: mov      r8, r2
003c1958: mov      sl, r3
003c195c: moveq    r6, r7
003c1960: beq      #0x3c1990
003c1964: ldr      r3, [ip, #4]
003c1968: ldr      r6, [ip]
003c196c: ldr      r2, [r0, #4]
003c1970: ldr      ip, [r3]
003c1974: mov      r0, r3
003c1978: str      r1, [sp]
003c197c: mov      r3, r4
003c1980: mov      r1, r6
003c1984: mov      lr, pc
003c1988: ldr      pc, [ip, #0x10]
003c198c: mov      r7, r6
003c1990: mov      r0, r4
003c1994: mov      r1, r5
003c1998: bl       #0x3c0084
003c199c: cmp      r0, #0
003c19a0: streq    r0, [r4, #0x20]
003c19a4: bne      #0x3c19c0
003c19a8: ldr      r0, [r4, #4]
003c19ac: mov      r2, r7
003c19b0: mov      r1, #0x1d
003c19b4: add      sp, sp, #0x14
003c19b8: pop      {r4, r5, r6, r7, r8, sl, lr}
003c19bc: b        #0x3a4d5c
003c19c0: mov      r1, r5
003c19c4: mov      r0, r4
003c19c8: bl       #0x3c184c
003c19cc: cmp      r6, r5
003c19d0: movne    r3, #0
003c19d4: str      r0, [r4, #0x20]
003c19d8: strne    r3, [r4, #0x60]
003c19dc: ldm      r0, {r1, r3}
003c19e0: ldr      r2, [r4, #4]
003c19e4: ldr      ip, [r3]
003c19e8: mov      r0, r3
003c19ec: stm      sp, {r6, r8, sl}
003c19f0: mov      r3, r4
003c19f4: mov      lr, pc
003c19f8: ldr      pc, [ip, #0xc]
003c19fc: b        #0x3c19a8

# _ZN5Level15_LoadCharStatesEv
003eff98: ldr      r3, [pc, #0x70]
003eff9c: ldr      r2, [pc, #0x70]
003effa0: push     {r4, r5, r6, lr}
003effa4: add      r3, pc, r3
003effa8: ldr      r2, [r3, r2]
003effac: ldr      r6, [r2, #0x38]
003effb0: ldr      r4, [r6, #0x60]!
003effb4: cmp      r6, r4
003effb8: beq      #0x3efff8
003effbc: ldr      r5, [r4, #8]
003effc0: subs     r0, r5, #0
003effc4: beq      #0x3effec
003effc8: bl       #0x3a5784
003effcc: subs     r3, r0, #0
003effd0: beq      #0x3efffc
003effd4: add      r0, r5, #0x4f0
003effd8: cmp      r3, #0x11
003effdc: add      r0, r0, #0xc
003effe0: mov      r1, #0
003effe4: beq      #0x3efffc
003effe8: bl       #0x3c1a00
003effec: ldr      r4, [r4]
003efff0: cmp      r6, r4
003efff4: bne      #0x3effbc
003efff8: pop      {r4, r5, r6, pc}
003efffc: add      r0, r5, #0x4f0
003f0000: add      r0, r0, #0xc
003f0004: bl       #0x3c1a64
003f0008: ldr      r4, [r4]
003f000c: b        #0x3efff0
003f0010: subseq   r4, sl, ip, ror #21
003f0014: strdeq   r3, r4, [r0], -r4

# _ZN16CharStateMachine12SetCharacterEP9Character
003c1600: push     {r4, r5, lr}
003c1604: ldr      r3, [pc, #0x70]
003c1608: subs     r4, r1, #0
003c160c: sub      sp, sp, #0xc
003c1610: mov      r5, r0
003c1614: add      r3, pc, r3
003c1618: beq      #0x3c1628
003c161c: str      r4, [r5, #4]
003c1620: add      sp, sp, #0xc
003c1624: pop      {r4, r5, pc}
003c1628: ldr      r2, [pc, #0x50]
003c162c: ldr      r2, [r3, r2]
003c1630: ldr      r2, [r2]
003c1634: cmp      r2, #2
003c1638: streq    r4, [r4]
003c163c: beq      #0x3c161c
003c1640: cmp      r2, #1
003c1644: bne      #0x3c161c
003c1648: ldr      r0, [pc, #0x34]
003c164c: ldr      r1, [pc, #0x34]
003c1650: ldr      r2, [pc, #0x34]
003c1654: ldr      r0, [r3, r0]
003c1658: ldr      r3, [pc, #0x30]
003c165c: mov      ip, #0xe1
003c1660: add      r1, pc, r1
003c1664: add      r2, pc, r2
003c1668: add      r3, pc, r3
003c166c: add      r0, r0, #0xa8
003c1670: str      ip, [sp]
003c1674: bl       #0x30e004
003c1678: b        #0x3c161c
003c167c: subseq   r3, sp, ip, ror r4
003c1680: andeq    r3, r0, r0, asr #19
003c1684: andeq    r1, r0, r0, asr #19
003c1688: subeq    ip, pc, r8, ror sp
003c168c: subseq   r0, r3, r4, lsr #20
003c1690: subseq   r3, r0, r0, lsl #11

# _ZN16CharStateMachine19SM_SetPreSpawnStateEv
003c1a64: mov      r1, #0x11
003c1a68: mvn      r2, #0
003c1a6c: mov      r3, #0
003c1a70: b        #0x3c1938

# _ZN16CharStateMachineC1Ev
003c1ac4: ldr      ip, [pc, #0x84]
003c1ac8: str      r4, [sp, #-4]!
003c1acc: ldr      r4, [pc, #0x80]
003c1ad0: add      ip, pc, ip
003c1ad4: mov      r2, #0
003c1ad8: ldr      r4, [ip, r4]
003c1adc: mov      r1, r0
003c1ae0: str      r2, [r0, #4]
003c1ae4: add      r4, r4, #8
003c1ae8: str      r4, [r0]
003c1aec: str      r2, [r0, #0xc]
003c1af0: mvn      r4, #0
003c1af4: strb     r2, [r1, #8]!
003c1af8: str      r1, [r0, #0x14]
003c1afc: str      r4, [r0, #0x28]
003c1b00: str      r2, [r0, #0x5c]
003c1b04: str      r1, [r0, #0x10]
003c1b08: str      r2, [r0, #0x18]
003c1b0c: str      r2, [r0, #0x20]
003c1b10: str      r2, [r0, #0x24]
003c1b14: str      r2, [r0, #0x60]
003c1b18: str      r2, [r0, #0x2c]
003c1b1c: str      r2, [r0, #0x30]
003c1b20: str      r2, [r0, #0x34]
003c1b24: str      r2, [r0, #0x38]
003c1b28: str      r2, [r0, #0x3c]
003c1b2c: str      r2, [r0, #0x40]
003c1b30: str      r2, [r0, #0x44]
003c1b34: str      r2, [r0, #0x48]
003c1b38: str      r2, [r0, #0x4c]
003c1b3c: str      r2, [r0, #0x50]
003c1b40: str      r2, [r0, #0x54]
003c1b44: str      r2, [r0, #0x58]
003c1b48: ldm      sp!, {r4}
003c1b4c: bx       lr
003c1b50: subseq   r2, sp, r0, asr #31
003c1b54: strdeq   r2, r3, [r0], -r8

# _ZN6CSIdle7OnFocusEiP9CharacterP16CharStateMachineiiPv
003c3020: push     {r4, r5, r6, r7, r8, lr}
003c3024: ldr      r4, [pc, #0xf4]
003c3028: ldr      r7, [pc, #0xf4]
003c302c: ldr      r1, [pc, #0xf4]
003c3030: add      r4, pc, r4
003c3034: ldr      r3, [r4, r7]
003c3038: ldr      r8, [r4, r1]
003c303c: sub      sp, sp, #0x20
003c3040: ldr      r3, [r3]
003c3044: mov      r0, r8
003c3048: mov      r6, r2
003c304c: str      r3, [sp, #0x1c]
003c3050: bl       #0x337888
003c3054: ldr      r1, [pc, #0xd0]
003c3058: add      r5, sp, #4
003c305c: mov      r2, sp
003c3060: add      r1, pc, r1
003c3064: mov      r0, r5
003c3068: bl       #0x3140ec
003c306c: mov      r1, r5
003c3070: mov      r0, r8
003c3074: bl       #0x337a88
003c3078: mov      r0, r5
003c307c: bl       #0x318254
003c3080: ldrb     r3, [r6, #0x538]
003c3084: cmp      r3, #0
003c3088: beq      #0x3c30a8
003c308c: ldr      r3, [r4, r7]
003c3090: ldr      r2, [sp, #0x1c]
003c3094: ldr      r3, [r3]
003c3098: cmp      r2, r3
003c309c: bne      #0x3c311c
003c30a0: add      sp, sp, #0x20
003c30a4: pop      {r4, r5, r6, r7, r8, pc}
003c30a8: mov      r3, #0x2380
003c30ac: str      r3, [r6, #0x520]
003c30b0: ldr      r3, [pc, #0x78]
003c30b4: mov      r0, r6
003c30b8: add      r5, r6, #0x490
003c30bc: ldr      r3, [r4, r3]
003c30c0: add      r5, r5, #0xc
003c30c4: ldr      r8, [r3]
003c30c8: bl       #0x3a3228
003c30cc: ldr      r3, [pc, #0x60]
003c30d0: ldr      r1, [pc, #0x60]
003c30d4: ldr      r2, [r4, r3]
003c30d8: mov      r3, #0xa0
003c30dc: mla      r3, r3, r0, r8
003c30e0: ldr      r0, [r2, #0x2c]
003c30e4: ldr      r2, [pc, #0x50]
003c30e8: add      r1, pc, r1
003c30ec: ldr      r8, [r3, #0x28]
003c30f0: add      r2, pc, r2
003c30f4: bl       #0x4c4bdc
003c30f8: ands     r0, r0, #2
003c30fc: bne      #0x3c3110
003c3100: add      r1, r0, r8
003c3104: mov      r0, r5
003c3108: bl       #0x3cacb0
003c310c: b        #0x3c308c
003c3110: mov      r0, r6
003c3114: bl       #0x3a53e0
003c3118: b        #0x3c3100
003c311c: bl       #0x30e310
003c3120: subseq   r1, sp, r0, ror #20
003c3124: andeq    r4, r0, ip, lsr #1
003c3128: andeq    r0, r0, r4, lsl #17
003c312c: ldrsheq  r1, [r0], #-0xd0
003c3130: andeq    r4, r0, r4, asr #16
003c3134: strdeq   r3, r4, [r0], -r4
003c3138: ldrsbeq  r1, [r0], #-0xa0
003c313c: ldrsbeq  r1, [r0], #-0xa8
