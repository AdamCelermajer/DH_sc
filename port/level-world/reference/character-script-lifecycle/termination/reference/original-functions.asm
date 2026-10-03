
# _ZN6CharAI13_SkillCleanUpEv
003d8ae0: push     {r4, r5, r6, lr}
003d8ae4: ldr      r3, [r0, #0xb4]
003d8ae8: ldr      r6, [r0, #0xb8]
003d8aec: mov      r5, r0
003d8af0: rsb      r6, r3, r6
003d8af4: asrs     r6, r6, #2
003d8af8: beq      #0x3d8b24
003d8afc: mov      r4, #0
003d8b00: b        #0x3d8b08
003d8b04: ldr      r3, [r5, #0xb4]
003d8b08: ldr      r0, [r3, r4, lsl #2]
003d8b0c: add      r4, r4, #1
003d8b10: cmp      r0, #0
003d8b14: beq      #0x3d8b1c
003d8b18: bl       #0x3daafc
003d8b1c: cmp      r4, r6
003d8b20: bne      #0x3d8b04
003d8b24: pop      {r4, r5, r6, pc}

# _ZN12CharAIScript12BindFunctionEv
003d8f2c: push     {r4, lr}
003d8f30: mov      r4, r0
003d8f34: bl       #0x37b5a0
003d8f38: mov      r0, r4
003d8f3c: pop      {r4, lr}
003d8f40: b        #0x3d8ec8

# _ZN6CharAI13_SpellCleanUpEv
003d8a98: push     {r4, r5, r6, lr}
003d8a9c: ldr      r3, [r0, #0xc0]
003d8aa0: ldr      r6, [r0, #0xc4]
003d8aa4: mov      r5, r0
003d8aa8: rsb      r6, r3, r6
003d8aac: asrs     r6, r6, #2
003d8ab0: beq      #0x3d8adc
003d8ab4: mov      r4, #0
003d8ab8: b        #0x3d8ac0
003d8abc: ldr      r3, [r5, #0xc0]
003d8ac0: ldr      r0, [r3, r4, lsl #2]
003d8ac4: add      r4, r4, #1
003d8ac8: cmp      r0, #0
003d8acc: beq      #0x3d8ad4
003d8ad0: bl       #0x3daafc
003d8ad4: cmp      r4, r6
003d8ad8: bne      #0x3d8abc
003d8adc: pop      {r4, r5, r6, pc}

# _ZN10AISDefault11OnTerminateEv
003dbe84: bx       lr

# _ZN9AISPlayer7InitVCBEv
003dd884: push     {r4, r5, r6, lr}
003dd888: mov      r4, r0
003dd88c: bl       #0x3dc7d8
003dd890: ldr      r1, [pc, #0x24]
003dd894: mov      r0, r4
003dd898: ldr      r5, [r4, #0xb8]
003dd89c: add      r1, pc, r1
003dd8a0: bl       #0x37c2a0
003dd8a4: cmp      r0, #0
003dd8a8: movne    r0, #0x400
003dd8ac: moveq    r0, #0
003dd8b0: orr      r5, r0, r5
003dd8b4: str      r5, [r4, #0xb8]
003dd8b8: pop      {r4, r5, r6, pc}
003dd8bc: strdeq   r8, sb, [lr], #-0x24

# _ZN6CharAI11OnTerminateEv
003d11bc: push     {r4, r5, r6, lr}
003d11c0: mov      r5, r0
003d11c4: bl       #0x3cfd7c
003d11c8: mov      r0, r5
003d11cc: bl       #0x3d5fa8
003d11d0: mov      r0, r5
003d11d4: mov      r1, #0
003d11d8: bl       #0x3d6abc
003d11dc: ldr      r0, [r5, #0x58]
003d11e0: cmp      r0, #0
003d11e4: beq      #0x3d11f8
003d11e8: ldr      r2, [r5, #4]
003d11ec: ldr      r3, [r0, #0x418]
003d11f0: cmp      r2, r3
003d11f4: beq      #0x3d12a0
003d11f8: ldr      r4, [r5, #0x64]
003d11fc: mov      r3, #0
003d1200: str      r3, [r5, #0x58]
003d1204: add      r6, r5, #0x5c
003d1208: cmp      r6, r4
003d120c: beq      #0x3d1258
003d1210: ldr      r0, [r4, #0x14]
003d1214: cmp      r0, #0
003d1218: beq      #0x3d122c
003d121c: ldr      r2, [r5, #4]
003d1220: ldr      r3, [r0, #0x418]
003d1224: cmp      r2, r3
003d1228: beq      #0x3d1290
003d122c: ldr      r2, [r4, #0xc]
003d1230: cmp      r2, #0
003d1234: bne      #0x3d1240
003d1238: b        #0x3d125c
003d123c: mov      r2, r3
003d1240: ldr      r3, [r2, #8]
003d1244: cmp      r3, #0
003d1248: bne      #0x3d123c
003d124c: mov      r4, r2
003d1250: cmp      r6, r4
003d1254: bne      #0x3d1210
003d1258: pop      {r4, r5, r6, pc}
003d125c: ldr      r3, [r4, #4]
003d1260: ldr      r1, [r3, #0xc]
003d1264: cmp      r1, r4
003d1268: bne      #0x3d1284
003d126c: mov      r4, r3
003d1270: ldr      r3, [r3, #4]
003d1274: ldr      r2, [r3, #0xc]
003d1278: cmp      r2, r4
003d127c: beq      #0x3d126c
003d1280: ldr      r2, [r4, #0xc]
003d1284: cmp      r3, r2
003d1288: movne    r4, r3
003d128c: b        #0x3d1208
003d1290: add      r0, r0, #0x3c8
003d1294: mov      r1, #0
003d1298: bl       #0x3d4d80
003d129c: b        #0x3d122c
003d12a0: add      r0, r0, #0x3c8
003d12a4: mov      r1, #0
003d12a8: bl       #0x3d4d80
003d12ac: b        #0x3d11f8
