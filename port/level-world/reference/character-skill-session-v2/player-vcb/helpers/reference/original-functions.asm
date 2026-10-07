
# _ZNK9LuaScript11IsInVFTableEPKc
0037c2a0: push     {r4, lr}
0037c2a4: mov      r4, r0
0037c2a8: mov      r0, r1
0037c2ac: bl       #0x37c164
0037c2b0: ldr      r3, [r4, #0x38]
0037c2b4: add      r4, r4, #0x34
0037c2b8: cmp      r3, #0
0037c2bc: beq      #0x37c300
0037c2c0: mov      r1, r4
0037c2c4: b        #0x37c2cc
0037c2c8: mov      r3, r2
0037c2cc: ldr      r2, [r3, #0x10]
0037c2d0: cmp      r0, r2
0037c2d4: ldrhi    r2, [r3, #0xc]
0037c2d8: ldrls    r2, [r3, #8]
0037c2dc: movhi    r3, r1
0037c2e0: mov      r1, r3
0037c2e4: cmp      r2, #0
0037c2e8: bne      #0x37c2c8
0037c2ec: cmp      r4, r3
0037c2f0: beq      #0x37c300
0037c2f4: ldr      r2, [r3, #0x10]
0037c2f8: cmp      r0, r2
0037c2fc: bhs      #0x37c308
0037c300: mov      r0, #0
0037c304: pop      {r4, pc}
0037c308: subs     r0, r3, r4
0037c30c: movne    r0, #1
0037c310: pop      {r4, pc}

# _ZN10AISDefault7InitVCBEv
003dc7d8: ldr      r1, [pc, #0x4c]
003dc7dc: mov      r3, #0
003dc7e0: push     {r4, r5, r6, lr}
003dc7e4: add      r1, pc, r1
003dc7e8: str      r3, [r0, #0xb8]
003dc7ec: mov      r4, r0
003dc7f0: bl       #0x37c2a0
003dc7f4: ldr      r1, [pc, #0x34]
003dc7f8: cmp      r0, #0
003dc7fc: movne    r5, #0x800
003dc800: moveq    r5, #0
003dc804: str      r5, [r4, #0xb8]
003dc808: add      r1, pc, r1
003dc80c: mov      r0, r4
003dc810: bl       #0x37c2a0
003dc814: cmp      r0, #0
003dc818: movne    r0, #0x1000
003dc81c: moveq    r0, #0
003dc820: orr      r5, r0, r5
003dc824: str      r5, [r4, #0xb8]
003dc828: pop      {r4, r5, r6, pc}
003dc82c: subeq    sb, lr, ip, ror r1
003dc830: subeq    sb, lr, r8, ror #2
