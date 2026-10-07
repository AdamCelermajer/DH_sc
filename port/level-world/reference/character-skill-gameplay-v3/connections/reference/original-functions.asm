
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

# _ZN15VisualFXManager14DropAnimatedFXERP10AnimatedFX
00494978: push     {r4, r5, r6, r7, r8, lr}
0049497c: ldr      r3, [r1]
00494980: ldr      r6, [pc, #0x124]
00494984: mov      r5, r1
00494988: cmp      r3, #0
0049498c: add      r6, pc, r6
00494990: beq      #0x4949e4
00494994: ldrb     r2, [r0, #4]
00494998: cmp      r2, #0
0049499c: beq      #0x4949e8
004949a0: ldr      r3, [r3, #8]
004949a4: cmp      r3, #0
004949a8: blt      #0x4949d8
004949ac: ldr      r2, [r0, #0x2c]
004949b0: ldr      r0, [r0, #0x28]
004949b4: rsb      r2, r0, r2
004949b8: asr      r2, r2, #3
004949bc: add      r1, r2, r2, lsl #2
004949c0: add      r1, r1, r1, lsl #4
004949c4: add      r1, r1, r1, lsl #8
004949c8: add      r1, r1, r1, lsl #16
004949cc: add      r2, r2, r1, lsl #1
004949d0: cmp      r3, r2
004949d4: blo      #0x4949f0
004949d8: mov      r3, #0
004949dc: str      r3, [r5]
004949e0: pop      {r4, r5, r6, r7, r8, pc}
004949e4: pop      {r4, r5, r6, r7, r8, pc}
004949e8: str      r2, [r1]
004949ec: pop      {r4, r5, r6, r7, r8, pc}
004949f0: mov      r8, #0x18
004949f4: mla      r8, r8, r3, r0
004949f8: mov      r7, r8
004949fc: ldr      r0, [r7, #0x10]!
00494a00: cmp      r7, r0
00494a04: beq      #0x494a28
00494a08: ldr      r3, [r0, #8]
00494a0c: ldr      r2, [r5]
00494a10: ldr      r4, [r0]
00494a14: cmp      r2, r3
00494a18: beq      #0x494a90
00494a1c: mov      r0, r4
00494a20: cmp      r7, r0
00494a24: bne      #0x494a08
00494a28: add      r0, r8, #4
00494a2c: mov      r1, r5
00494a30: bl       #0x494550
00494a34: ldr      r3, [r5]
00494a38: mov      r4, #0
00494a3c: mov      r1, #1
00494a40: mov      r0, r3
00494a44: str      r4, [r3, #0x28]
00494a48: bl       #0x492aa0
00494a4c: ldr      r2, [pc, #0x5c]
00494a50: ldr      r3, [r5]
00494a54: mov      r1, r4
00494a58: ldr      r2, [r6, r2]
00494a5c: mov      r0, r3
00494a60: ldr      lr, [r2]
00494a64: ldr      ip, [r2, #4]
00494a68: ldr      r2, [r2, #8]
00494a6c: str      lr, [r3, #0x34]
00494a70: str      ip, [r3, #0x38]
00494a74: str      r2, [r3, #0x3c]
00494a78: bl       #0x492aa0
00494a7c: ldr      r0, [r5]
00494a80: mov      r1, r4
00494a84: bl       #0x492ef0
00494a88: str      r4, [r5]
00494a8c: pop      {r4, r5, r6, r7, r8, pc}
00494a90: ldr      r3, [r0, #4]
00494a94: mov      r1, #0xc
00494a98: str      r4, [r3]
00494a9c: str      r3, [r4, #4]
00494aa0: bl       #0x708f00
00494aa4: mov      r0, r4
00494aa8: b        #0x494a20
00494aac: subseq   r0, r0, r4, lsl #2
00494ab0: andeq    r3, r0, ip, lsr #30

# _ZN9Character10RaiseEventEiPv
003a4d5c: cmp      r1, #0x36
003a4d60: beq      #0x3a4d6c
003a4d64: add      r0, r0, #0x3c8
003a4d68: b        #0x3cbb34
003a4d6c: add      r0, r0, #0x560
003a4d70: mov      r1, r2
003a4d74: b        #0x3e123c
