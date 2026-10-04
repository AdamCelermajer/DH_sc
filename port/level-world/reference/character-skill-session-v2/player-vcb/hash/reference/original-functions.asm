
# _ZN6glitch4core10hashStringEPKc
0037c164: push     {r4, r5, r6, r7, lr}
0037c168: ldr      r4, [pc, #0x118]
0037c16c: ldr      r3, [pc, #0x118]
0037c170: ldr      r7, [pc, #0x118]
0037c174: add      r4, pc, r4
0037c178: ldr      r5, [r4, r3]
0037c17c: ldr      r3, [r4, r7]
0037c180: sub      sp, sp, #0x24
0037c184: ldr      r2, [r5]
0037c188: ldr      r3, [r3]
0037c18c: mov      r6, r0
0037c190: tst      r2, #1
0037c194: str      r3, [sp, #0x1c]
0037c198: beq      #0x37c244
0037c19c: add      r5, sp, #4
0037c1a0: mov      r0, r6
0037c1a4: str      r5, [sp, #0x14]
0037c1a8: str      r5, [sp, #0x18]
0037c1ac: bl       #0x30de54
0037c1b0: mov      r1, r6
0037c1b4: add      r2, r6, r0
0037c1b8: mov      r0, r5
0037c1bc: bl       #0x3116e8
0037c1c0: ldr      r0, [sp, #0x18]
0037c1c4: ldr      ip, [sp, #0x14]
0037c1c8: cmp      r0, ip
0037c1cc: moveq    r6, #0
0037c1d0: beq      #0x37c200
0037c1d4: mov      r2, r0
0037c1d8: mov      r6, #0
0037c1dc: ldrsb    r1, [r2], #1
0037c1e0: movw     r3, #0x79b9
0037c1e4: movt     r3, #0x9e37
0037c1e8: add      r3, r1, r3
0037c1ec: add      r3, r3, r6, lsl #6
0037c1f0: add      r3, r3, r6, lsr #2
0037c1f4: cmp      r2, ip
0037c1f8: eor      r6, r6, r3
0037c1fc: bne      #0x37c1dc
0037c200: cmp      r0, r5
0037c204: beq      #0x37c224
0037c208: cmp      r0, #0
0037c20c: beq      #0x37c224
0037c210: ldr      r1, [sp, #4]
0037c214: rsb      r1, r0, r1
0037c218: cmp      r1, #0x80
0037c21c: bhi      #0x37c27c
0037c220: bl       #0x708f00
0037c224: ldr      r3, [r4, r7]
0037c228: ldr      r2, [sp, #0x1c]
0037c22c: mov      r0, r6
0037c230: ldr      r3, [r3]
0037c234: cmp      r2, r3
0037c238: bne      #0x37c284
0037c23c: add      sp, sp, #0x24
0037c240: pop      {r4, r5, r6, r7, pc}
0037c244: mov      r0, r5
0037c248: bl       #0x30e76c
0037c24c: cmp      r0, #0
0037c250: beq      #0x37c19c
0037c254: mov      r0, r5
0037c258: bl       #0x30ea3c
0037c25c: ldr      r3, [pc, #0x30]
0037c260: ldr      r0, [r4, r3]
0037c264: ldr      r3, [pc, #0x2c]
0037c268: ldr      r1, [r4, r3]
0037c26c: ldr      r3, [pc, #0x28]
0037c270: ldr      r2, [r4, r3]
0037c274: bl       #0x30e304
0037c278: b        #0x37c19c
0037c27c: bl       #0x310440
0037c280: b        #0x37c224
0037c284: bl       #0x30e310
0037c288: rsbeq    r8, r1, ip, lsl sb
0037c28c: muleq    r0, ip, sb
0037c290: andeq    r4, r0, ip, lsr #1
0037c294: andeq    r1, r0, r4, ror #13
0037c298: andeq    r2, r0, r4, lsr #14
0037c29c: muleq    r0, r0, r8
