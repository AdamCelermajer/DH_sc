
# _ZN9Character12_ClearTargetERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv
003b5690: push     {r4, lr}
003b5694: mov      r1, #0
003b5698: add      r4, r2, #0x3c8
003b569c: mov      r0, r4
003b56a0: mov      r2, r1
003b56a4: bl       #0x3d6890
003b56a8: mov      r0, r4
003b56ac: pop      {r4, lr}
003b56b0: b        #0x3d49c4

# _ZN3sfc6script3lua9Arguments12pushUserDataEPNS1_8UserDataE
00386f28: ldr      r3, [pc, #0x58]
00386f2c: ldr      r2, [pc, #0x58]
00386f30: push     {r4, r5, r6, lr}
00386f34: add      r3, pc, r3
00386f38: ldr      r5, [r3, r2]
00386f3c: sub      sp, sp, #0x78
00386f40: add      r4, sp, #4
00386f44: ldr      r3, [r5]
00386f48: str      r3, [sp, #0x74]
00386f4c: ldr      r6, [r0, #4]
00386f50: mov      r0, r4
00386f54: bl       #0x37c978
00386f58: mov      r0, r6
00386f5c: mov      r1, r4
00386f60: bl       #0x3195c0
00386f64: mov      r0, r4
00386f68: bl       #0x3193e8
00386f6c: ldr      r2, [sp, #0x74]
00386f70: ldr      r3, [r5]
00386f74: cmp      r2, r3
00386f78: bne      #0x386f84
00386f7c: add      sp, sp, #0x78
00386f80: pop      {r4, r5, r6, pc}
00386f84: bl       #0x30e310
00386f88: rsbeq    sp, r0, ip, asr fp
00386f8c: andeq    r4, r0, ip, lsr #1

# _ZNK10GameObject17GetTargetPositionEv
003935dc: ldr      r3, [r0, #0x180]
003935e0: cmp      r3, #0
003935e4: beq      #0x3935f8
003935e8: ldrb     r3, [r0, #0x80]
003935ec: cmp      r3, #0
003935f0: addne    r0, r0, #0x184
003935f4: bxne     lr
003935f8: add      r0, r0, #0x160
003935fc: bx       lr

# _ZN9Character10_SetTargetERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv
003b8f38: push     {r4, lr}
003b8f3c: ldr      r3, [r0, #4]
003b8f40: ldm      r3, {r0, r1}
003b8f44: rsb      r3, r0, r1
003b8f48: asr      r3, r3, #4
003b8f4c: add      r1, r3, r3, lsl #3
003b8f50: add      r1, r1, r1, lsl #6
003b8f54: add      r1, r3, r1, lsl #3
003b8f58: add      r1, r1, r1, lsl #15
003b8f5c: add      r3, r3, r1, lsl #3
003b8f60: cmp      r3, #0
003b8f64: bne      #0x3b8f6c
003b8f68: pop      {r4, pc}
003b8f6c: ldr      r3, [r0, #4]
003b8f70: cmp      r3, #2
003b8f74: beq      #0x3b8f80
003b8f78: cmp      r3, #7
003b8f7c: bne      #0x3b8f68
003b8f80: add      r4, r2, #0x3c8
003b8f84: bl       #0x31b5a0
003b8f88: mov      r2, #0
003b8f8c: mov      r1, r0
003b8f90: mov      r0, r4
003b8f94: pop      {r4, lr}
003b8f98: b        #0x3d6890

# _ZNK9Character11GetCharAIIdEv
003a2fec: ldr      r0, [r0, #0xffc]
003a2ff0: ldr      r3, [pc, #0x24]
003a2ff4: cmp      r0, #0
003a2ff8: add      r3, pc, r3
003a2ffc: blt      #0x3a3014
003a3000: ldr      r2, [pc, #0x18]
003a3004: ldr      r3, [r3, r2]
003a3008: ldr      r3, [r3]
003a300c: cmp      r0, r3
003a3010: bxlt     lr
003a3014: mov      r0, #8
003a3018: bx       lr

# _ZN3sfc6script3lua12ReturnValues12pushUserDataEPNS1_8UserDataE
0037c9f8: ldr      r3, [pc, #0x58]
0037c9fc: ldr      r2, [pc, #0x58]
0037ca00: push     {r4, r5, r6, lr}
0037ca04: add      r3, pc, r3
0037ca08: ldr      r5, [r3, r2]
0037ca0c: sub      sp, sp, #0x78
0037ca10: add      r4, sp, #4
0037ca14: ldr      r3, [r5]
0037ca18: str      r3, [sp, #0x74]
0037ca1c: ldr      r6, [r0, #0x24]
0037ca20: mov      r0, r4
0037ca24: bl       #0x37c978
0037ca28: mov      r0, r6
0037ca2c: mov      r1, r4
0037ca30: bl       #0x3195c0
0037ca34: mov      r0, r4
0037ca38: bl       #0x3193e8
0037ca3c: ldr      r2, [sp, #0x74]
0037ca40: ldr      r3, [r5]
0037ca44: cmp      r2, r3
0037ca48: bne      #0x37ca54
0037ca4c: add      sp, sp, #0x78
0037ca50: pop      {r4, r5, r6, pc}
0037ca54: bl       #0x30e310
0037ca58: rsbeq    r8, r1, ip, lsl #1
0037ca5c: andeq    r4, r0, ip, lsr #1

# _ZN9Character10_HasTargetERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv
003b6f50: ldr      r3, [r2, #0x408]
003b6f54: mov      r0, r1
003b6f58: subs     r1, r3, #0
003b6f5c: movne    r1, #1
003b6f60: b        #0x37c7e4

# _ZN6CharAI17AI_SyncLastTargetEv
003d49c4: ldr      r3, [r0, #0x40]
003d49c8: str      r3, [r0, #0x44]
003d49cc: bx       lr

# _ZNK3sfc6script3lua5Value11getUserDataEv
0031b5a0: ldr      r3, [r0, #4]
0031b5a4: cmp      r3, #2
0031b5a8: beq      #0x31b5b8
0031b5ac: cmp      r3, #7
0031b5b0: movne    r0, #0
0031b5b4: bxne     lr
0031b5b8: ldr      r0, [r0, #0x6c]
0031b5bc: bx       lr

# _ZN3sfc6script3lua12ReturnValues11pushBooleanEb
0037c7e4: ldr      r3, [pc, #0x58]
0037c7e8: ldr      r2, [pc, #0x58]
0037c7ec: push     {r4, r5, r6, lr}
0037c7f0: add      r3, pc, r3
0037c7f4: ldr      r5, [r3, r2]
0037c7f8: sub      sp, sp, #0x78
0037c7fc: add      r4, sp, #4
0037c800: ldr      r3, [r5]
0037c804: str      r3, [sp, #0x74]
0037c808: ldr      r6, [r0, #0x24]
0037c80c: mov      r0, r4
0037c810: bl       #0x37c764
0037c814: mov      r0, r6
0037c818: mov      r1, r4
0037c81c: bl       #0x3195c0
0037c820: mov      r0, r4
0037c824: bl       #0x3193e8
0037c828: ldr      r2, [sp, #0x74]
0037c82c: ldr      r3, [r5]
0037c830: cmp      r2, r3
0037c834: bne      #0x37c840
0037c838: add      sp, sp, #0x78
0037c83c: pop      {r4, r5, r6, pc}
0037c840: bl       #0x30e310
0037c844: rsbeq    r8, r1, r0, lsr #5
0037c848: andeq    r4, r0, ip, lsr #1

# _ZN6CharAI12AI_SetTargetEP10GameObjectb
003d6890: push     {r4, r5, r6, r7, r8, sb, sl, lr}
003d6894: ldr      r5, [pc, #0x204]
003d6898: ldr      r7, [pc, #0x204]
003d689c: sub      sp, sp, #0x78
003d68a0: add      r5, pc, r5
003d68a4: ldr      r3, [r5, r7]
003d68a8: mov      r4, r0
003d68ac: cmp      r2, #0
003d68b0: ldr      r3, [r3]
003d68b4: mov      r6, r1
003d68b8: str      r1, [r4, #0x3c]
003d68bc: str      r3, [sp, #0x74]
003d68c0: bne      #0x3d6a20
003d68c4: ldr      r3, [r0, #0x40]
003d68c8: ldr      sb, [pc, #0x1d8]
003d68cc: add      r8, sp, #0x5c
003d68d0: cmp      r3, r1
003d68d4: ldrne    r1, [r0, #4]
003d68d8: ldr      sl, [r5, sb]
003d68dc: movwne   r3, #0x14d0
003d68e0: strhne   r2, [r1, r3]
003d68e4: mov      r0, sl
003d68e8: bl       #0x337888
003d68ec: ldr      r1, [pc, #0x1b8]
003d68f0: add      r2, sp, #0x10
003d68f4: mov      r0, r8
003d68f8: add      r1, pc, r1
003d68fc: bl       #0x3140ec
003d6900: mov      r0, sl
003d6904: mov      r1, r8
003d6908: bl       #0x337a88
003d690c: mov      sl, r0
003d6910: ldr      r0, [sp, #0x70]
003d6914: cmp      r0, r8
003d6918: beq      #0x3d6938
003d691c: cmp      r0, #0
003d6920: beq      #0x3d6938
003d6924: ldr      r1, [sp, #0x5c]
003d6928: rsb      r1, r0, r1
003d692c: cmp      r1, #0x80
003d6930: bhi      #0x3d6a4c
003d6934: bl       #0x708f00
003d6938: cmp      sl, #0
003d693c: beq      #0x3d69a4
003d6940: ldr      r3, [r4, #0x40]
003d6944: cmp      r3, r6
003d6948: beq      #0x3d69a4
003d694c: subs     r3, r3, #0
003d6950: movne    r3, #1
003d6954: subs     r2, r6, #0
003d6958: movne    r2, #1
003d695c: tst      r2, r3
003d6960: bne      #0x3d6a28
003d6964: cmp      r3, #0
003d6968: beq      #0x3d6a18
003d696c: ldr      sl, [r5, sb]
003d6970: add      r8, sp, #0x2c
003d6974: mov      r0, sl
003d6978: bl       #0x337888
003d697c: ldr      r1, [pc, #0x12c]
003d6980: add      r2, sp, #8
003d6984: mov      r0, r8
003d6988: add      r1, pc, r1
003d698c: bl       #0x3140ec
003d6990: mov      r0, sl
003d6994: mov      r1, r8
003d6998: bl       #0x337a88
003d699c: mov      r0, r8
003d69a0: bl       #0x318254
003d69a4: cmp      r6, #0
003d69a8: str      r6, [r4, #0x40]
003d69ac: beq      #0x3d69fc
003d69b0: ldr      r0, [r4, #4]
003d69b4: bl       #0x3a2fec
003d69b8: ldr      r2, [r4, #0x44]
003d69bc: ldr      r3, [r4, #0x40]
003d69c0: cmp      r3, r2
003d69c4: movne    r2, #0
003d69c8: strbne   r2, [r4, #0x4c]
003d69cc: movne    r2, r3
003d69d0: str      r2, [r4, #0x44]
003d69d4: mov      r0, r3
003d69d8: ldr      r3, [r3]
003d69dc: mov      lr, pc
003d69e0: ldr      pc, [r3, #0x34]
003d69e4: eor      r0, r0, #1
003d69e8: strb     r0, [r4, #0x48]
003d69ec: ldr      r1, [r4, #0x40]
003d69f0: mov      r0, r4
003d69f4: bl       #0x3d4ed8
003d69f8: strb     r0, [r4, #0x49]
003d69fc: ldr      r3, [r5, r7]
003d6a00: ldr      r2, [sp, #0x74]
003d6a04: ldr      r3, [r3]
003d6a08: cmp      r2, r3
003d6a0c: bne      #0x3d6a9c
003d6a10: add      sp, sp, #0x78
003d6a14: pop      {r4, r5, r6, r7, r8, sb, sl, pc}
003d6a18: cmp      r2, #0
003d6a1c: bne      #0x3d6a5c
003d6a20: str      r6, [r4, #0x40]
003d6a24: b        #0x3d69fc
003d6a28: ldr      sl, [r5, sb]
003d6a2c: add      r8, sp, #0x44
003d6a30: mov      r0, sl
003d6a34: bl       #0x337888
003d6a38: ldr      r1, [pc, #0x74]
003d6a3c: add      r2, sp, #0xc
003d6a40: mov      r0, r8
003d6a44: add      r1, pc, r1
003d6a48: b        #0x3d698c
003d6a4c: bl       #0x310440
003d6a50: cmp      sl, #0
003d6a54: beq      #0x3d69a4
003d6a58: b        #0x3d6940
003d6a5c: ldr      sl, [r5, sb]
003d6a60: add      r8, sp, #0x14
003d6a64: mov      r0, sl
003d6a68: bl       #0x337888
003d6a6c: ldr      r1, [pc, #0x44]
003d6a70: add      r2, sp, #4
003d6a74: mov      r0, r8
003d6a78: add      r1, pc, r1
003d6a7c: bl       #0x3140ec
003d6a80: mov      r1, r8
003d6a84: mov      r0, sl
003d6a88: bl       #0x337a88
003d6a8c: mov      r0, r8
003d6a90: bl       #0x318254
003d6a94: str      r6, [r4, #0x40]
003d6a98: b        #0x3d69b0
003d6a9c: bl       #0x30e310
003d6aa0: ldrsheq  lr, [fp], #-0x10
003d6aa4: andeq    r4, r0, ip, lsr #1
003d6aa8: andeq    r0, r0, r4, lsl #17
003d6aac: subeq    lr, lr, r0, ror #27
003d6ab0: subeq    lr, lr, r8, ror #26
003d6ab4: subeq    lr, lr, ip, lsr #25
003d6ab8: subeq    lr, lr, r8, ror ip

# _ZNK6CharAI12AI_IsInSightEf
003d4ea0: push     {r4, r5, r6, lr}
003d4ea4: ldr      r0, [r0, #4]
003d4ea8: mov      r5, r1
003d4eac: bl       #0x3a3024
003d4eb0: ldr      r0, [r0, #0x3c]
003d4eb4: mov      r4, #0
003d4eb8: mov      r1, r0
003d4ebc: bl       #0x30ed6c
003d4ec0: mov      r1, r5
003d4ec4: bl       #0x30e2f8
003d4ec8: cmp      r0, #0
003d4ecc: movne    r4, #1
003d4ed0: and      r0, r4, #1
003d4ed4: pop      {r4, r5, r6, pc}

# _ZN9Character10_GetTargetERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv
003b6c7c: mov      r0, r1
003b6c80: ldr      r1, [r2, #0x408]
003b6c84: b        #0x37c9f8

# _ZNK6CharAI12AI_IsInSightEPK10GameObject
003d4ed8: push     {r4, r5, r6, r7, r8, lr}
003d4edc: subs     r5, r1, #0
003d4ee0: mov      r6, r0
003d4ee4: beq      #0x3d4f84
003d4ee8: ldr      r0, [r6, #4]
003d4eec: bl       #0x3935dc
003d4ef0: mov      r4, r0
003d4ef4: mov      r0, r5
003d4ef8: bl       #0x3935dc
003d4efc: mov      r5, r0
003d4f00: ldr      r1, [r0]
003d4f04: ldr      r0, [r4]
003d4f08: bl       #0x30e3ac
003d4f0c: ldr      r1, [r5, #4]
003d4f10: mov      r8, r0
003d4f14: ldr      r0, [r4, #4]
003d4f18: bl       #0x30e3ac
003d4f1c: ldr      r1, [r5, #8]
003d4f20: mov      r7, r0
003d4f24: ldr      r0, [r4, #8]
003d4f28: bl       #0x30e3ac
003d4f2c: mov      r1, r8
003d4f30: mov      r5, r0
003d4f34: mov      r0, r8
003d4f38: bl       #0x30ed6c
003d4f3c: mov      r1, r7
003d4f40: mov      r4, r0
003d4f44: mov      r0, r7
003d4f48: bl       #0x30ed6c
003d4f4c: mov      r1, r0
003d4f50: mov      r0, r4
003d4f54: bl       #0x30eba4
003d4f58: mov      r1, r5
003d4f5c: mov      r4, r0
003d4f60: mov      r0, r5
003d4f64: bl       #0x30ed6c
003d4f68: mov      r1, r0
003d4f6c: mov      r0, r4
003d4f70: bl       #0x30eba4
003d4f74: mov      r1, r0
003d4f78: mov      r0, r6
003d4f7c: pop      {r4, r5, r6, r7, r8, lr}
003d4f80: b        #0x3d4ea0
003d4f84: ldr      r5, [r0, #0x40]
003d4f88: cmp      r5, #0
003d4f8c: bne      #0x3d4ee8
003d4f90: mov      r0, r5
003d4f94: pop      {r4, r5, r6, r7, r8, pc}
