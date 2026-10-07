
# _ZNK3sfc6script3lua5Value7getBoolEv
0031bc80: push     {r4, r5, r6, lr}
0031bc84: ldr      r3, [r0, #4]
0031bc88: mov      r5, r0
0031bc8c: cmp      r3, #0
0031bc90: beq      #0x31bcbc
0031bc94: cmp      r3, #1
0031bc98: beq      #0x31bcc8
0031bc9c: cmp      r3, #3
0031bca0: beq      #0x31bcc8
0031bca4: cmp      r3, #2
0031bca8: beq      #0x31bcec
0031bcac: cmp      r3, #7
0031bcb0: beq      #0x31bcec
0031bcb4: cmp      r3, #4
0031bcb8: beq      #0x31bcfc
0031bcbc: mov      r5, #0
0031bcc0: mov      r0, r5
0031bcc4: pop      {r4, r5, r6, pc}
0031bcc8: ldr      r0, [r5, #8]
0031bccc: mov      r1, #0
0031bcd0: bl       #0x30df8c
0031bcd4: cmp      r0, #0
0031bcd8: mov      r5, #0
0031bcdc: moveq    r5, #1
0031bce0: uxtb     r5, r5
0031bce4: mov      r0, r5
0031bce8: pop      {r4, r5, r6, pc}
0031bcec: ldr      r5, [r5, #0x6c]
0031bcf0: subs     r5, r5, #0
0031bcf4: movne    r5, #1
0031bcf8: b        #0x31bcc0
0031bcfc: bl       #0x84c7e0
0031bd00: ldr      r1, [r5, #0x20]
0031bd04: mov      r4, r0
0031bd08: bl       #0x84c04c
0031bd0c: mov      r0, r4
0031bd10: mvn      r1, #0
0031bd14: bl       #0x84b320
0031bd18: subs     r5, r0, #0
0031bd1c: movne    r5, #1
0031bd20: mov      r0, r4
0031bd24: bl       #0x85797c
0031bd28: b        #0x31bcc0

# _ZNK10GameObject12GetLookAtVecER7Point3DIfE
00393ae4: push     {r4, r5, r6, lr}
00393ae8: ldr      r4, [r0, #0x174]
00393aec: mov      r5, r1
00393af0: mov      r0, r4
00393af4: bl       #0x30eb08
00393af8: mov      r6, r0
00393afc: mov      r0, r4
00393b00: bl       #0x30e754
00393b04: mov      r3, #0
00393b08: add      r0, r0, #0x80000000
00393b0c: str      r6, [r5]
00393b10: str      r3, [r5, #8]
00393b14: str      r0, [r5, #4]
00393b18: pop      {r4, r5, r6, pc}

# _ZNK3sfc6script3lua5Value9getNumberEv
0031bbf0: push     {r4, r5, r6, lr}
0031bbf4: ldr      r3, [r0, #4]
0031bbf8: mov      r5, r0
0031bbfc: cmp      r3, #0
0031bc00: beq      #0x31bc2c
0031bc04: cmp      r3, #1
0031bc08: beq      #0x31bc38
0031bc0c: cmp      r3, #3
0031bc10: beq      #0x31bc38
0031bc14: cmp      r3, #2
0031bc18: beq      #0x31bc44
0031bc1c: cmp      r3, #7
0031bc20: beq      #0x31bc44
0031bc24: cmp      r3, #4
0031bc28: beq      #0x31bc54
0031bc2c: mov      r5, #0
0031bc30: mov      r0, r5
0031bc34: pop      {r4, r5, r6, pc}
0031bc38: ldr      r5, [r5, #8]
0031bc3c: mov      r0, r5
0031bc40: pop      {r4, r5, r6, pc}
0031bc44: ldr      r0, [r5, #0x6c]
0031bc48: bl       #0x30e2e0
0031bc4c: mov      r5, r0
0031bc50: b        #0x31bc30
0031bc54: bl       #0x84c7e0
0031bc58: ldr      r1, [r5, #0x20]
0031bc5c: mov      r4, r0
0031bc60: bl       #0x84c04c
0031bc64: mov      r0, r4
0031bc68: mvn      r1, #0
0031bc6c: bl       #0x84c450
0031bc70: mov      r5, r0
0031bc74: mov      r0, r4
0031bc78: bl       #0x85797c
0031bc7c: b        #0x31bc30

# _ZNK3sfc6script3lua9ArgumentsixEj
0037baf8: push     {r4, r5, r6, lr}
0037bafc: ldr      r4, [r0, #4]
0037bb00: mov      r5, r1
0037bb04: ldm      r4, {r2, r3}
0037bb08: rsb      r3, r2, r3
0037bb0c: asr      r3, r3, #4
0037bb10: add      r1, r3, r3, lsl #3
0037bb14: add      r1, r1, r1, lsl #6
0037bb18: add      r1, r3, r1, lsl #3
0037bb1c: add      r1, r1, r1, lsl #15
0037bb20: add      r3, r3, r1, lsl #3
0037bb24: rsb      r3, r3, #0
0037bb28: cmp      r5, r3
0037bb2c: blo      #0x37bb40
0037bb30: ldr      r0, [pc, #0x14]
0037bb34: add      r0, pc, r0
0037bb38: bl       #0x708eb0
0037bb3c: ldr      r2, [r4]
0037bb40: mov      r0, #0x70
0037bb44: mla      r0, r0, r5, r2
0037bb48: pop      {r4, r5, r6, pc}
0037bb4c: subseq   r2, r4, r4, lsr sb

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
