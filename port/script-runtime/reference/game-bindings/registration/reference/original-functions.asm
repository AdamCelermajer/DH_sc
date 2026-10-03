
# _ZN12CharAIScript24CharAIScriptBindFunctionEv
003d8ec8: push     {r4, r5, r6, lr}
003d8ecc: ldr      r4, [pc, #0x44]
003d8ed0: ldr      r3, [pc, #0x44]
003d8ed4: ldr      r1, [pc, #0x44]
003d8ed8: mov      r5, r0
003d8edc: add      r4, pc, r4
003d8ee0: add      r6, r0, #0x10
003d8ee4: ldr      r2, [r4, r3]
003d8ee8: mov      r0, r6
003d8eec: mov      r3, r5
003d8ef0: add      r1, pc, r1
003d8ef4: bl       #0x31a4d4
003d8ef8: ldr      r3, [pc, #0x24]
003d8efc: ldr      r1, [pc, #0x24]
003d8f00: mov      r0, r6
003d8f04: ldr      r2, [r4, r3]
003d8f08: add      r1, pc, r1
003d8f0c: mov      r3, r5
003d8f10: pop      {r4, r5, r6, lr}
003d8f14: b        #0x31a4d4
003d8f18: ldrheq   fp, [fp], #-0xb4
003d8f1c: andeq    r4, r0, ip, asr r6
003d8f20: subeq    ip, lr, r8, ror #17
003d8f24: andeq    r1, r0, r0, ror sb
003d8f28: subeq    ip, lr, r0, ror #17

# _ZN12CharAIScript12BindFunctionEv
003d8f2c: push     {r4, lr}
003d8f30: mov      r4, r0
003d8f34: bl       #0x37b5a0
003d8f38: mov      r0, r4
003d8f3c: pop      {r4, lr}
003d8f40: b        #0x3d8ec8

# _ZN3sfc6script3lua12ReturnValues9_doReturnEP9lua_State
0031b308: push     {r4, r5, r6, r7, r8, lr}
0031b30c: ldr      r3, [r0, #0x24]
0031b310: mov      r7, r1
0031b314: mov      r6, r0
0031b318: ldr      r1, [r3, #4]
0031b31c: ldr      r2, [r3]
0031b320: rsb      r3, r2, r1
0031b324: asr      r3, r3, #4
0031b328: add      r0, r3, r3, lsl #3
0031b32c: add      r0, r0, r0, lsl #6
0031b330: add      r0, r3, r0, lsl #3
0031b334: add      r0, r0, r0, lsl #15
0031b338: add      r0, r3, r0, lsl #3
0031b33c: rsb      r0, r0, #0
0031b340: cmp      r0, #0
0031b344: beq      #0x31b394
0031b348: mov      r4, #0
0031b34c: mov      r5, r4
0031b350: add      r0, r2, r4
0031b354: mov      r1, r7
0031b358: bl       #0x31cac4
0031b35c: ldr      r2, [r6, #0x24]
0031b360: add      r5, r5, #1
0031b364: add      r4, r4, #0x70
0031b368: ldm      r2, {r2, r3}
0031b36c: rsb      r3, r2, r3
0031b370: asr      r3, r3, #4
0031b374: add      r1, r3, r3, lsl #3
0031b378: add      r1, r1, r1, lsl #6
0031b37c: add      r1, r3, r1, lsl #3
0031b380: add      r1, r1, r1, lsl #15
0031b384: add      r3, r3, r1, lsl #3
0031b388: rsb      r0, r3, #0
0031b38c: cmp      r5, r0
0031b390: blo      #0x31b350
0031b394: pop      {r4, r5, r6, r7, r8, pc}

# _ZN3sfc6script3lua5Value13_setFromStackEP9lua_Statei
0031c9c8: push     {r4, r5, r6, lr}
0031c9cc: mov      r5, r1
0031c9d0: mov      r4, r0
0031c9d4: mov      r1, r2
0031c9d8: mov      r0, r5
0031c9dc: mov      r6, r2
0031c9e0: bl       #0x84b264
0031c9e4: str      r0, [r4, #4]
0031c9e8: cmp      r0, #5
0031c9ec: addls    pc, pc, r0, lsl #2
0031c9f0: b        #0x31ca0c
0031c9f4: b        #0x31ca14
0031c9f8: b        #0x31ca54
0031c9fc: b        #0x31ca6c
0031ca00: b        #0x31ca80
0031ca04: b        #0x31ca94
0031ca08: b        #0x31ca18
0031ca0c: mov      r3, #0
0031ca10: str      r3, [r4, #4]
0031ca14: pop      {r4, r5, r6, pc}
0031ca18: ldr      r2, [pc, #0xa0]
0031ca1c: mov      r1, r6
0031ca20: mov      r0, r5
0031ca24: add      r2, pc, r2
0031ca28: bl       #0x84c1ec
0031ca2c: mvn      r1, #0
0031ca30: mov      r0, r5
0031ca34: bl       #0x84b390
0031ca38: mvn      r1, #1
0031ca3c: str      r0, [r4, #0x6c]
0031ca40: mov      r0, r5
0031ca44: bl       #0x84b140
0031ca48: mov      r3, #7
0031ca4c: str      r3, [r4, #4]
0031ca50: pop      {r4, r5, r6, pc}
0031ca54: mov      r1, r6
0031ca58: mov      r0, r5
0031ca5c: bl       #0x84b320
0031ca60: bl       #0x30e964
0031ca64: str      r0, [r4, #8]
0031ca68: pop      {r4, r5, r6, pc}
0031ca6c: mov      r0, r5
0031ca70: mov      r1, r6
0031ca74: bl       #0x84b390
0031ca78: str      r0, [r4, #0x6c]
0031ca7c: pop      {r4, r5, r6, pc}
0031ca80: mov      r0, r5
0031ca84: mov      r1, r6
0031ca88: bl       #0x84c450
0031ca8c: str      r0, [r4, #8]
0031ca90: pop      {r4, r5, r6, pc}
0031ca94: mov      r1, r6
0031ca98: mov      r2, #0
0031ca9c: mov      r0, r5
0031caa0: bl       #0x84c384
0031caa4: mov      r5, r0
0031caa8: bl       #0x30de54
0031caac: mov      r1, r5
0031cab0: add      r2, r5, r0
0031cab4: add      r0, r4, #0xc
0031cab8: pop      {r4, r5, r6, lr}
0031cabc: b        #0x3109e0
0031cac0: subseq   r1, sl, ip, lsr #28
