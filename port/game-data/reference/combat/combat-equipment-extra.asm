
# _ZNK13ItemInventory16HasOffHandWeaponEv
00400158: push     {r4, lr}
0040015c: mov      r1, #2
00400160: mov      r4, r0
00400164: bl       #0x3fc6a8
00400168: mov      r3, #0xc
0040016c: mul      r3, r3, r0
00400170: ldr      r2, [r4, #0x14]
00400174: ldr      r3, [r2, r3]
00400178: ldr      r0, [r3, #8]
0040017c: cmp      r0, #0
00400180: beq      #0x400198
00400184: ldr      r0, [r0]
00400188: bl       #0x3f9e08
0040018c: ldr      r0, [r0, #0x58]
00400190: subs     r0, r0, #6
00400194: movne    r0, #1
00400198: pop      {r4, pc}

# _ZNK13ItemInventory18GetCurrentEquipSetEi
003fc6a8: cmp      r1, #0
003fc6ac: blt      #0x3fc6c0
003fc6b0: sub      r1, r1, #1
003fc6b4: cmp      r1, #1
003fc6b8: movhi    r0, #0
003fc6bc: bxhi     lr
003fc6c0: ldrsb    r0, [r0, #0x2e]
003fc6c4: bx       lr
