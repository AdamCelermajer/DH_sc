
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

# _ZNK3sfc6script3lua5Value10getPointerEv
0031b580: ldr      r3, [r0, #4]
0031b584: cmp      r3, #2
0031b588: beq      #0x31b598
0031b58c: cmp      r3, #7
0031b590: movne    r0, #0
0031b594: bxne     lr
0031b598: ldr      r0, [r0, #0x6c]
0031b59c: bx       lr

# _ZNK3sfc6script3lua5Value11getUIntegerEv
0038d798: push     {r4, lr}
0038d79c: bl       #0x31bbf0
0038d7a0: bl       #0x8be2a0
0038d7a4: pop      {r4, pc}

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

# _ZN3sfc6script3lua12ReturnValues11pushIntegerEi
0037cb24: ldr      r3, [pc, #0x58]
0037cb28: ldr      r2, [pc, #0x58]
0037cb2c: push     {r4, r5, r6, lr}
0037cb30: add      r3, pc, r3
0037cb34: ldr      r5, [r3, r2]
0037cb38: sub      sp, sp, #0x78
0037cb3c: add      r4, sp, #4
0037cb40: ldr      r3, [r5]
0037cb44: str      r3, [sp, #0x74]
0037cb48: ldr      r6, [r0, #0x24]
0037cb4c: mov      r0, r4
0037cb50: bl       #0x37ca9c
0037cb54: mov      r0, r6
0037cb58: mov      r1, r4
0037cb5c: bl       #0x3195c0
0037cb60: mov      r0, r4
0037cb64: bl       #0x3193e8
0037cb68: ldr      r2, [sp, #0x74]
0037cb6c: ldr      r3, [r5]
0037cb70: cmp      r2, r3
0037cb74: bne      #0x37cb80
0037cb78: add      sp, sp, #0x78
0037cb7c: pop      {r4, r5, r6, pc}
0037cb80: bl       #0x30e310
0037cb84: rsbeq    r7, r1, r0, ror #30
0037cb88: andeq    r4, r0, ip, lsr #1
