
# _ZN6glitch7collada21CSceneNodeAnimatorSet19setCurrentAnimationEi
0065f8c8: push     {r4, r5, r6, r7, r8, sb, sl, lr}
0065f8cc: mov      r4, r0
0065f8d0: mov      r5, r1
0065f8d4: bl       #0x65f0fc
0065f8d8: ldr      r3, [r4, #0x24]
0065f8dc: str      r0, [r4, #0x14]
0065f8e0: mov      r1, r5
0065f8e4: ldr      r2, [r3, #0x3c]
0065f8e8: mov      r0, r3
0065f8ec: str      r5, [r4, #0x50]
0065f8f0: mul      r3, r2, r5
0065f8f4: str      r3, [r4, #0x4c]
0065f8f8: bl       #0x65f0b4
0065f8fc: bl       #0x60e334
0065f900: ldr      r3, [r4]
0065f904: mov      r6, r0
0065f908: mov      r0, r4
0065f90c: mov      lr, pc
0065f910: ldr      pc, [r3, #0x44]
0065f914: cmp      r0, #0
0065f918: beq      #0x65f9fc
0065f91c: ldr      r8, [r6]
0065f920: cmp      r8, #0
0065f924: beq      #0x65f968
0065f928: ldr      r3, [r4]
0065f92c: mov      r0, r4
0065f930: mov      lr, pc
0065f934: ldr      pc, [r3, #0x44]
0065f938: str      r6, [r0, #0x34]
0065f93c: ldr      r2, [r6]
0065f940: cmp      r2, #0
0065f944: moveq    r1, #1
0065f948: streq    r1, [r0, #0x14]
0065f94c: streq    r2, [r0, #0x10]
0065f950: beq      #0x65f9d4
0065f954: ldr      r3, [r0]
0065f958: mov      r1, #0
0065f95c: mov      lr, pc
0065f960: ldr      pc, [r3, #0x10]
0065f964: b        #0x65f9d4
0065f968: ldr      r3, [r4]
0065f96c: mov      r0, r4
0065f970: mov      lr, pc
0065f974: ldr      pc, [r3, #0x44]
0065f978: mov      r7, #1
0065f97c: str      r8, [r0, #0x10]
0065f980: str      r8, [r0, #0x34]
0065f984: str      r7, [r0, #0x14]
0065f988: ldr      r3, [r4]
0065f98c: mov      r0, r4
0065f990: mov      lr, pc
0065f994: ldr      pc, [r3, #0x44]
0065f998: ldr      r3, [r0]
0065f99c: mov      r8, r0
0065f9a0: mov      r1, r5
0065f9a4: mov      r0, r4
0065f9a8: ldr      r6, [r3, #0x50]
0065f9ac: bl       #0x65f104
0065f9b0: mov      r1, r5
0065f9b4: mov      sl, r0
0065f9b8: mov      r0, r4
0065f9bc: bl       #0x65f10c
0065f9c0: mov      r1, sl
0065f9c4: mov      r2, r0
0065f9c8: mov      r3, r7
0065f9cc: mov      r0, r8
0065f9d0: blx      r6
0065f9d4: mov      r1, r5
0065f9d8: ldr      r0, [r4, #0x24]
0065f9dc: bl       #0x65f0b4
0065f9e0: ldr      r3, [r0]
0065f9e4: mov      r0, r4
0065f9e8: ldr      r3, [r3, #0x24]
0065f9ec: ldr      r3, [r3, #0x20]
0065f9f0: ldr      r1, [r3, #0x2c]
0065f9f4: pop      {r4, r5, r6, r7, r8, sb, sl, lr}
0065f9f8: b        #0x60fab8
0065f9fc: pop      {r4, r5, r6, r7, r8, sb, sl, pc}

# _ZNK6glitch7collada16CColladaDatabase17constructAnimatorEv
0060fb54: push     {r4, r5, r6, lr}
0060fb58: ldr      r3, [r0]
0060fb5c: mov      r4, r0
0060fb60: ldr      r3, [r3, #0x24]
0060fb64: ldr      r2, [r3, #0x20]
0060fb68: ldr      r3, [r2, #0x4c]
0060fb6c: cmp      r3, #0
0060fb70: movle    r6, #0
0060fb74: ble      #0x60fbb4
0060fb78: mov      r5, #0
0060fb7c: mov      r6, r5
0060fb80: mov      r1, r5
0060fb84: mov      r0, r4
0060fb88: bl       #0x60e3c8
0060fb8c: ldr      r3, [r0, #0xc]
0060fb90: add      r5, r5, #1
0060fb94: cmp      r3, #1
0060fb98: ldr      r3, [r4]
0060fb9c: addeq    r6, r6, #1
0060fba0: ldr      r3, [r3, #0x24]
0060fba4: ldr      r2, [r3, #0x20]
0060fba8: ldr      r3, [r2, #0x4c]
0060fbac: cmp      r5, r3
0060fbb0: blt      #0x60fb80
0060fbb4: ldr      r3, [r2, #0x24]
0060fbb8: cmp      r3, #0
0060fbbc: bne      #0x60fbc8
0060fbc0: cmp      r6, #0
0060fbc4: beq      #0x60fc60
0060fbc8: ldr      r3, [r4, #4]
0060fbcc: add      r2, r2, #0x34
0060fbd0: mov      r1, r4
0060fbd4: mov      r0, r3
0060fbd8: ldr      r3, [r3]
0060fbdc: mov      lr, pc
0060fbe0: ldr      pc, [r3, #8]
0060fbe4: ldr      r3, [r4]
0060fbe8: mov      r6, r0
0060fbec: ldr      r3, [r3, #0x24]
0060fbf0: ldr      r3, [r3, #0x20]
0060fbf4: ldr      r2, [r3, #0x24]
0060fbf8: cmp      r2, #0
0060fbfc: movgt    r5, #0
0060fc00: ble      #0x60fc4c
0060fc04: mov      r1, r5
0060fc08: mov      r0, r4
0060fc0c: bl       #0x60e35c
0060fc10: ldr      r3, [r0, #0x14]
0060fc14: mov      r1, r0
0060fc18: add      r5, r5, #1
0060fc1c: cmp      r3, #0
0060fc20: mov      r0, r6
0060fc24: beq      #0x60fc34
0060fc28: ldr      r3, [r6]
0060fc2c: mov      lr, pc
0060fc30: ldr      pc, [r3, #0x88]
0060fc34: ldr      r3, [r4]
0060fc38: ldr      r3, [r3, #0x24]
0060fc3c: ldr      r3, [r3, #0x20]
0060fc40: ldr      r2, [r3, #0x24]
0060fc44: cmp      r5, r2
0060fc48: blt      #0x60fc04
0060fc4c: ldr      r1, [r3, #0x2c]
0060fc50: mov      r0, r6
0060fc54: bl       #0x60fab8
0060fc58: mov      r0, r6
0060fc5c: pop      {r4, r5, r6, pc}
0060fc60: ldr      r3, [r2, #0x2c]
0060fc64: cmp      r3, #0
0060fc68: bne      #0x60fbc8
0060fc6c: b        #0x60fc58
