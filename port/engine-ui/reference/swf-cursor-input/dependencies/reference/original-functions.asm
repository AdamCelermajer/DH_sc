
# _ZN7gameswf19edit_text_character24get_topmost_mouse_entityEff
0078bb58: push     {r4, r5, r6, r7, lr}
0078bb5c: mov      r4, r0
0078bb60: ldrb     r0, [r0, #0x9b]
0078bb64: sub      sp, sp, #0x14
0078bb68: mov      ip, r1
0078bb6c: cmp      r0, #0
0078bb70: mov      r3, r2
0078bb74: bne      #0x78bb84
0078bb78: mov      r0, #0
0078bb7c: add      sp, sp, #0x14
0078bb80: pop      {r4, r5, r6, r7, pc}
0078bb84: ldr      r0, [r4, #0x4c]
0078bb88: mov      lr, #0
0078bb8c: add      r1, sp, #8
0078bb90: mov      r2, sp
0078bb94: str      lr, [sp, #0xc]
0078bb98: str      ip, [sp]
0078bb9c: str      r3, [sp, #4]
0078bba0: str      lr, [sp, #8]
0078bba4: bl       #0x753d7c
0078bba8: ldr      r5, [r4, #0xa0]
0078bbac: ldr      r6, [sp, #8]
0078bbb0: ldr      r7, [sp, #0xc]
0078bbb4: ldr      r1, [r5, #0x24]
0078bbb8: mov      r0, r6
0078bbbc: bl       #0x30e70c
0078bbc0: cmp      r0, #0
0078bbc4: bne      #0x78bb78
0078bbc8: mov      r0, r6
0078bbcc: ldr      r1, [r5, #0x28]
0078bbd0: bl       #0x30e2f8
0078bbd4: cmp      r0, #0
0078bbd8: bne      #0x78bb78
0078bbdc: mov      r0, r7
0078bbe0: ldr      r1, [r5, #0x2c]
0078bbe4: bl       #0x30e70c
0078bbe8: cmp      r0, #0
0078bbec: bne      #0x78bb78
0078bbf0: mov      r0, r7
0078bbf4: ldr      r1, [r5, #0x30]
0078bbf8: bl       #0x30e2f8
0078bbfc: cmp      r0, #0
0078bc00: moveq    r0, r4
0078bc04: beq      #0x78bb7c
0078bc08: b        #0x78bb78

# _Z19CanHandleMouseEventPN7gameswf9characterE
007a8c6c: push     {r4, lr}
007a8c70: subs     r4, r0, #0
007a8c74: beq      #0x7a8ccc
007a8c78: ldr      r0, [r4, #0x44]
007a8c7c: ldr      r1, [pc, #0x50]
007a8c80: ldrsb    r3, [r0]
007a8c84: add      r1, pc, r1
007a8c88: cmn      r3, #1
007a8c8c: addne    r0, r0, #1
007a8c90: ldreq    r0, [r0, #0xc]
007a8c94: bl       #0x30ebd4
007a8c98: cmp      r0, #0
007a8c9c: beq      #0x7a8cc4
007a8ca0: ldr      r3, [r4]
007a8ca4: mov      r0, r4
007a8ca8: mov      r1, #2
007a8cac: mov      lr, pc
007a8cb0: ldr      pc, [r3, #8]
007a8cb4: cmp      r0, #0
007a8cb8: moveq    r0, #1
007a8cbc: ldrbne   r0, [r4, #0xea]
007a8cc0: pop      {r4, pc}
007a8cc4: ldrb     r0, [r4, #0x9c]
007a8cc8: pop      {r4, pc}
007a8ccc: mov      r0, r4
007a8cd0: pop      {r4, pc}
007a8cd4: ldrsheq  r0, [r2], -ip

# _ZN7gameswf25button_character_instance24get_topmost_mouse_entityEff
007c6e50: push     {r4, r5, r6, r7, r8, sb, sl, lr}
007c6e54: mov      r7, r0
007c6e58: ldrb     r0, [r0, #0x9b]
007c6e5c: sub      sp, sp, #0x18
007c6e60: mov      ip, r1
007c6e64: cmp      r0, #0
007c6e68: mov      r3, r2
007c6e6c: bne      #0x7c6e7c
007c6e70: mov      r0, #0
007c6e74: add      sp, sp, #0x18
007c6e78: pop      {r4, r5, r6, r7, r8, sb, sl, pc}
007c6e7c: add      sl, sp, #0x10
007c6e80: ldr      r0, [r7, #0x4c]
007c6e84: mov      r8, #0
007c6e88: add      r2, sp, #8
007c6e8c: mov      r1, sl
007c6e90: str      r3, [sp, #0xc]
007c6e94: str      ip, [sp, #8]
007c6e98: str      r8, [sp, #0x10]
007c6e9c: str      r8, [sp, #0x14]
007c6ea0: bl       #0x753d7c
007c6ea4: ldr      r3, [r7, #0xa0]
007c6ea8: ldr      r2, [r3, #0x28]
007c6eac: cmp      r2, #0
007c6eb0: ble      #0x7c6e70
007c6eb4: mov      r5, #0
007c6eb8: mov      r6, r5
007c6ebc: mov      sb, sp
007c6ec0: ldr      r4, [r3, #0x24]
007c6ec4: mov      r1, sp
007c6ec8: mov      r2, sl
007c6ecc: add      r4, r4, r5
007c6ed0: ldr      ip, [r4, #8]
007c6ed4: add      r0, r4, #0x14
007c6ed8: cmp      ip, #0
007c6edc: blt      #0x7c6f20
007c6ee0: ldrb     ip, [r4, #2]
007c6ee4: cmp      ip, #0
007c6ee8: beq      #0x7c6f20
007c6eec: str      r8, [sp]
007c6ef0: str      r8, [sp, #4]
007c6ef4: bl       #0x753d7c
007c6ef8: ldr      r3, [r4, #0xc]
007c6efc: ldr      r1, [sp]
007c6f00: ldr      r2, [sp, #4]
007c6f04: mov      r0, r3
007c6f08: ldr      r3, [r3]
007c6f0c: mov      lr, pc
007c6f10: ldr      pc, [r3, #0x10]
007c6f14: cmp      r0, #0
007c6f18: bne      #0x7c6f38
007c6f1c: ldr      r3, [r7, #0xa0]
007c6f20: ldr      r2, [r3, #0x28]
007c6f24: add      r6, r6, #1
007c6f28: add      r5, r5, #0x64
007c6f2c: cmp      r6, r2
007c6f30: blt      #0x7c6ec0
007c6f34: b        #0x7c6e70
007c6f38: mov      r0, r7
007c6f3c: b        #0x7c6e74

# _ZN7gameswf9smart_ptrINS_9characterEE7set_refEPS1_
0075518c: push     {r4, r5, r6, lr}
00755190: mov      r4, r0
00755194: ldr      r0, [r0]
00755198: mov      r5, r1
0075519c: cmp      r0, r1
007551a0: beq      #0x7551c8
007551a4: cmp      r0, #0
007551a8: beq      #0x7551b0
007551ac: bl       #0x75a240
007551b0: cmp      r5, #0
007551b4: str      r5, [r4]
007551b8: beq      #0x7551c8
007551bc: mov      r0, r5
007551c0: pop      {r4, r5, r6, lr}
007551c4: b        #0x759c64
007551c8: pop      {r4, r5, r6, pc}

# _ZN7gameswf6matrix18set_scale_rotationEfff
00796920: push     {r4, r5, r6, r7, r8, sb, sl, lr}
00796924: mov      r4, r0
00796928: mov      r0, r3
0079692c: mov      r5, r3
00796930: mov      r6, r2
00796934: mov      r8, r1
00796938: bl       #0x30e754
0079693c: mov      r7, r0
00796940: mov      r0, r5
00796944: bl       #0x30eb08
00796948: mov      r1, r7
0079694c: mov      sl, r0
00796950: mov      r0, r8
00796954: bl       #0x30ed6c
00796958: mvn      r1, #0x800000
0079695c: mov      r5, r0
00796960: bl       #0x30e4b4
00796964: cmp      r0, #0
00796968: beq      #0x796a50
0079696c: mvn      r1, #0x80000000
00796970: mov      r0, r5
00796974: sub      r1, r1, #0x800000
00796978: bl       #0x30e9ac
0079697c: cmp      r0, #0
00796980: beq      #0x796a50
00796984: str      r5, [r4]
00796988: mov      r1, r6
0079698c: add      r0, sl, #0x80000000
00796990: bl       #0x30ed6c
00796994: mvn      r1, #0x800000
00796998: mov      r5, r0
0079699c: bl       #0x30e4b4
007969a0: cmp      r0, #0
007969a4: beq      #0x796a48
007969a8: mvn      r1, #0x80000000
007969ac: mov      r0, r5
007969b0: sub      r1, r1, #0x800000
007969b4: bl       #0x30e9ac
007969b8: cmp      r0, #0
007969bc: beq      #0x796a48
007969c0: str      r5, [r4, #4]
007969c4: mov      r1, sl
007969c8: mov      r0, r8
007969cc: bl       #0x30ed6c
007969d0: mvn      r1, #0x800000
007969d4: mov      r5, r0
007969d8: bl       #0x30e4b4
007969dc: cmp      r0, #0
007969e0: beq      #0x796a40
007969e4: mvn      r1, #0x80000000
007969e8: mov      r0, r5
007969ec: sub      r1, r1, #0x800000
007969f0: bl       #0x30e9ac
007969f4: cmp      r0, #0
007969f8: beq      #0x796a40
007969fc: str      r5, [r4, #0xc]
00796a00: mov      r1, r7
00796a04: mov      r0, r6
00796a08: bl       #0x30ed6c
00796a0c: mvn      r1, #0x800000
00796a10: mov      r5, r0
00796a14: bl       #0x30e4b4
00796a18: cmp      r0, #0
00796a1c: beq      #0x796a58
00796a20: mvn      r1, #0x80000000
00796a24: mov      r0, r5
00796a28: sub      r1, r1, #0x800000
00796a2c: bl       #0x30e9ac
00796a30: cmp      r0, #0
00796a34: beq      #0x796a58
00796a38: str      r5, [r4, #0x10]
00796a3c: pop      {r4, r5, r6, r7, r8, sb, sl, pc}
00796a40: mov      r5, #0
00796a44: b        #0x7969fc
00796a48: mov      r5, #0
00796a4c: b        #0x7969c0
00796a50: mov      r5, #0
00796a54: b        #0x796984
00796a58: mov      r5, #0
00796a5c: str      r5, [r4, #0x10]
00796a60: pop      {r4, r5, r6, r7, r8, sb, sl, pc}

# _ZN8RenderFX14FindCharactersEPN7gameswf9characterEPKci
007a8c08: push     {r4, r5, r6, lr}
007a8c0c: mov      r4, r0
007a8c10: ldr      r0, [r0, #8]
007a8c14: cmp      r0, #0
007a8c18: ble      #0x7a8c34
007a8c1c: mov      r0, #0
007a8c20: str      r0, [r4, #8]
007a8c24: mov      r0, r4
007a8c28: bl       #0x7a8acc
007a8c2c: add      r0, r4, #4
007a8c30: pop      {r4, r5, r6, pc}
007a8c34: bge      #0x7a8c1c
007a8c38: lsl      ip, r0, #2
007a8c3c: mov      r5, #0
007a8c40: ldr      lr, [r4, #4]
007a8c44: adds     r0, r0, #1
007a8c48: str      r5, [lr, ip]
007a8c4c: add      ip, ip, #4
007a8c50: bne      #0x7a8c40
007a8c54: mov      r0, #0
007a8c58: str      r0, [r4, #8]
007a8c5c: mov      r0, r4
007a8c60: bl       #0x7a8acc
007a8c64: add      r0, r4, #4
007a8c68: pop      {r4, r5, r6, pc}

# _ZN7gameswf15sprite_instance24get_topmost_mouse_entityEff
0077f00c: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0077f010: mov      r4, r0
0077f014: ldrb     r0, [r0, #0x9b]
0077f018: sub      sp, sp, #0x1c
0077f01c: str      r1, [sp, #4]
0077f020: cmp      r0, #0
0077f024: str      r2, [sp]
0077f028: moveq    r5, r0
0077f02c: beq      #0x77f138
0077f030: ldr      r3, [r4, #0x54]
0077f034: cmp      r3, #0
0077f038: beq      #0x77f058
0077f03c: ldr      r0, [r3, #0x68]
0077f040: cmp      r0, #0
0077f044: beq      #0x77f058
0077f048: mov      r1, r4
0077f04c: add      r2, sp, #4
0077f050: mov      r3, sp
0077f054: bl       #0x777514
0077f058: ldr      ip, [sp, #4]
0077f05c: ldr      r0, [r4, #0x4c]
0077f060: mov      r3, #0
0077f064: str      ip, [sp, #8]
0077f068: ldr      ip, [sp]
0077f06c: add      r1, sp, #0x10
0077f070: add      r2, sp, #8
0077f074: str      r3, [sp, #0x14]
0077f078: str      ip, [sp, #0xc]
0077f07c: str      r3, [sp, #0x10]
0077f080: bl       #0x753d7c
0077f084: ldr      r8, [r4, #0xac]
0077f088: subs     r7, r8, #1
0077f08c: bmi      #0x77f178
0077f090: ldr      sl, [pc, #0xec]
0077f094: mov      r6, #0
0077f098: lsl      r7, r7, #2
0077f09c: add      sl, pc, sl
0077f0a0: mov      sb, r6
0077f0a4: mov      fp, r6
0077f0a8: ldr      r3, [r4, #0xa8]
0077f0ac: ldr      r5, [r3, r7]
0077f0b0: subs     r0, r5, #0
0077f0b4: beq      #0x77f11c
0077f0b8: ldrb     r3, [r5, #0x9b]
0077f0bc: cmp      r3, #0
0077f0c0: beq      #0x77f11c
0077f0c4: ldr      r1, [sp, #0x10]
0077f0c8: ldr      r2, [sp, #0x14]
0077f0cc: ldr      r3, [r5]
0077f0d0: mov      lr, pc
0077f0d4: ldr      pc, [r3, #0x68]
0077f0d8: subs     fp, r0, #0
0077f0dc: beq      #0x77f0f8
0077f0e0: ldr      r3, [fp]
0077f0e4: mov      lr, pc
0077f0e8: ldr      pc, [r3, #0x15c]
0077f0ec: cmp      r0, #0
0077f0f0: bne      #0x77f170
0077f0f4: mov      sb, #1
0077f0f8: ldr      r3, [r5, #0x44]
0077f0fc: mov      r1, sl
0077f100: ldrsb    r2, [r3]
0077f104: add      r0, r3, #1
0077f108: cmn      r2, #1
0077f10c: ldreq    r0, [r3, #0xc]
0077f110: bl       #0x30e31c
0077f114: cmp      r0, #0
0077f118: beq      #0x77f12c
0077f11c: add      r6, r6, #1
0077f120: cmp      r6, r8
0077f124: sub      r7, r7, #4
0077f128: bne      #0x77f0a8
0077f12c: cmp      sb, #0
0077f130: bne      #0x77f144
0077f134: mov      r5, fp
0077f138: mov      r0, r5
0077f13c: add      sp, sp, #0x1c
0077f140: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
0077f144: mov      r5, #0
0077f148: ldr      r3, [r4]
0077f14c: mov      r0, r4
0077f150: mov      lr, pc
0077f154: ldr      pc, [r3, #0x15c]
0077f158: cmp      r0, #0
0077f15c: movne    r5, r4
0077f160: bne      #0x77f138
0077f164: cmp      r5, #0
0077f168: bne      #0x77f138
0077f16c: b        #0x77f134
0077f170: mov      r5, fp
0077f174: b        #0x77f148
0077f178: mov      fp, #0
0077f17c: mov      r5, fp
0077f180: b        #0x77f138
0077f184: andseq   sl, r8, r4, ror #20

# _ZN7gameswf17generic_character24get_topmost_mouse_entityEff
0075b6fc: push     {r4, lr}
0075b700: sub      sp, sp, #0x10
0075b704: mov      r4, r0
0075b708: mov      r3, #0
0075b70c: ldr      r0, [r0, #0x4c]
0075b710: str      r1, [sp]
0075b714: str      r2, [sp, #4]
0075b718: add      r1, sp, #8
0075b71c: mov      r2, sp
0075b720: str      r3, [sp, #0xc]
0075b724: str      r3, [sp, #8]
0075b728: bl       #0x753d7c
0075b72c: ldr      r3, [r4, #0xa0]
0075b730: ldr      r1, [sp, #8]
0075b734: ldr      r2, [sp, #0xc]
0075b738: mov      r0, r3
0075b73c: ldr      r3, [r3]
0075b740: mov      lr, pc
0075b744: ldr      pc, [r3, #0x10]
0075b748: cmp      r0, #0
0075b74c: movne    r0, r4
0075b750: moveq    r0, #0
0075b754: add      sp, sp, #0x10
0075b758: pop      {r4, pc}

# _ZN8RenderFX8PlayAnimEPN7gameswf9characterEPKci
007aba04: mov      r3, #1
007aba08: b        #0x7ab924

# _Z16GetLocalPositionPN7gameswf9characterEff
007aa2e0: push     {r4, r5, r6, r7, lr}
007aa2e4: mov      r7, r1
007aa2e8: mov      r1, #0x41000000
007aa2ec: sub      sp, sp, #0x34
007aa2f0: mov      r4, r0
007aa2f4: add      r1, r1, #0xa00000
007aa2f8: mov      r0, r2
007aa2fc: mov      r5, r3
007aa300: bl       #0x30ed6c
007aa304: mov      r1, #0x41000000
007aa308: mov      r6, r0
007aa30c: add      r1, r1, #0xa00000
007aa310: mov      r0, r5
007aa314: bl       #0x30ed6c
007aa318: mov      r3, #0
007aa31c: mov      r5, r0
007aa320: str      r3, [r4, #4]
007aa324: str      r3, [r4]
007aa328: mov      r0, r7
007aa32c: bl       #0x753f74
007aa330: add      ip, sp, #0x18
007aa334: mov      lr, r0
007aa338: ldm      lr!, {r0, r1, r2, r3}
007aa33c: stm      ip!, {r0, r1, r2, r3}
007aa340: mov      r2, #0
007aa344: add      r3, sp, #8
007aa348: ldm      lr, {r0, r1}
007aa34c: str      r2, [r3], #4
007aa350: str      r2, [r3], #4
007aa354: str      r2, [r3], #4
007aa358: stm      ip, {r0, r1}
007aa35c: mov      lr, #0x3f800000
007aa360: str      r2, [r3]
007aa364: mov      r0, sp
007aa368: add      r1, sp, #0x18
007aa36c: str      lr, [sp, #0x10]
007aa370: str      r2, [sp, #4]
007aa374: str      lr, [sp]
007aa378: bl       #0x795adc
007aa37c: ldr      r1, [sp]
007aa380: mov      r0, r6
007aa384: bl       #0x30ed6c
007aa388: ldr      r1, [sp, #4]
007aa38c: mov      r7, r0
007aa390: mov      r0, r5
007aa394: bl       #0x30ed6c
007aa398: mov      r1, r0
007aa39c: mov      r0, r7
007aa3a0: bl       #0x30eba4
007aa3a4: ldr      r1, [sp, #8]
007aa3a8: bl       #0x30eba4
007aa3ac: str      r0, [r4]
007aa3b0: ldr      r1, [sp, #0xc]
007aa3b4: mov      r0, r6
007aa3b8: bl       #0x30ed6c
007aa3bc: ldr      r1, [sp, #0x10]
007aa3c0: mov      r6, r0
007aa3c4: mov      r0, r5
007aa3c8: bl       #0x30ed6c
007aa3cc: mov      r1, r0
007aa3d0: mov      r0, r6
007aa3d4: bl       #0x30eba4
007aa3d8: ldr      r1, [sp, #0x14]
007aa3dc: bl       #0x30eba4
007aa3e0: str      r0, [r4, #4]
007aa3e4: mov      r0, r4
007aa3e8: add      sp, sp, #0x34
007aa3ec: pop      {r4, r5, r6, r7, pc}
