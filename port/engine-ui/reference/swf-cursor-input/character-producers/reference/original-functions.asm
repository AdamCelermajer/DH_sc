
# _ZN7gameswf15sprite_instance10set_memberERKNS_10tu_stringiERKNS_8as_valueE
00780968: push     {r4, r5, r6, r7, r8, sb, sl, lr}
0078096c: ldr      r4, [pc, #0x22c]
00780970: ldr      r5, [pc, #0x22c]
00780974: sub      sp, sp, #0x38
00780978: add      r4, pc, r4
0078097c: ldr      r3, [r4, r5]
00780980: mov      r6, r0
00780984: mov      r0, r1
00780988: ldr      r3, [r3]
0078098c: mov      r7, r1
00780990: mov      r8, r2
00780994: str      r3, [sp, #0x34]
00780998: bl       #0x772030
0078099c: cmp      r0, #0x28
007809a0: beq      #0x7809d8
007809a4: cmp      r0, #0x29
007809a8: beq      #0x7809ec
007809ac: mov      r0, r6
007809b0: mov      r1, r7
007809b4: mov      r2, r8
007809b8: bl       #0x75361c
007809bc: ldr      r3, [r4, r5]
007809c0: ldr      r2, [sp, #0x34]
007809c4: ldr      r3, [r3]
007809c8: cmp      r2, r3
007809cc: bne      #0x780b9c
007809d0: add      sp, sp, #0x38
007809d4: pop      {r4, r5, r6, r7, r8, sb, sl, pc}
007809d8: mov      r0, r8
007809dc: bl       #0x797960
007809e0: strb     r0, [r6, #0xea]
007809e4: mov      r0, #1
007809e8: b        #0x7809bc
007809ec: ldrsb    r3, [r8, #1]
007809f0: cmp      r3, #5
007809f4: beq      #0x780a00
007809f8: mov      r0, #1
007809fc: b        #0x7809bc
00780a00: ldr      r7, [r8, #4]
00780a04: cmp      r7, #0
00780a08: beq      #0x7809f8
00780a0c: ldr      r3, [r7]
00780a10: mov      r0, r7
00780a14: mov      r1, #0x1b
00780a18: mov      lr, pc
00780a1c: ldr      pc, [r3, #8]
00780a20: cmp      r0, #0
00780a24: beq      #0x7809f8
00780a28: ldr      r3, [r7]
00780a2c: mov      r0, r6
00780a30: add      sb, sp, #0x20
00780a34: ldr      r8, [r3, #0x48]
00780a38: bl       #0x77ff40
00780a3c: mov      r1, r0
00780a40: mov      r0, r7
00780a44: blx      r8
00780a48: mov      r0, r6
00780a4c: bl       #0x77ff40
00780a50: mov      r1, r6
00780a54: add      r0, r0, #0x38
00780a58: bl       #0x427ba8
00780a5c: mov      r3, #0
00780a60: strb     r3, [sp, #1]
00780a64: strb     r3, [sp]
00780a68: ldr      r1, [pc, #0x138]
00780a6c: ldr      r3, [r7]
00780a70: mov      r0, sb
00780a74: add      r1, pc, r1
00780a78: ldr      sl, [r3, #0x20]
00780a7c: bl       #0x413a7c
00780a80: mov      r1, sb
00780a84: mov      r0, r7
00780a88: mov      r2, sp
00780a8c: blx      sl
00780a90: ldrsb    r3, [sp, #0x20]
00780a94: mov      r8, sp
00780a98: cmn      r3, #1
00780a9c: bne      #0x780aac
00780aa0: ldr      r0, [sp, #0x2c]
00780aa4: ldr      r1, [sp, #0x28]
00780aa8: bl       #0x752b38
00780aac: ldrsb    r3, [sp, #1]
00780ab0: cmp      r3, #5
00780ab4: beq      #0x780b1c
00780ab8: mov      r0, sp
00780abc: bl       #0x797124
00780ac0: mov      r3, #0
00780ac4: strb     r3, [sp, #1]
00780ac8: ldr      r1, [pc, #0xdc]
00780acc: ldr      r3, [r7]
00780ad0: add      sb, sp, #0xc
00780ad4: add      r1, pc, r1
00780ad8: mov      r0, sb
00780adc: ldr      sl, [r3, #0x20]
00780ae0: bl       #0x413a7c
00780ae4: mov      r0, r7
00780ae8: mov      r1, sb
00780aec: mov      r2, sp
00780af0: blx      sl
00780af4: ldrsb    r3, [sp, #0xc]
00780af8: cmn      r3, #1
00780afc: beq      #0x780b8c
00780b00: ldrsb    r3, [sp, #1]
00780b04: cmp      r3, #5
00780b08: beq      #0x780b54
00780b0c: mov      r0, sp
00780b10: bl       #0x797124
00780b14: mov      r0, #1
00780b18: b        #0x7809bc
00780b1c: ldr      sl, [sp, #4]
00780b20: cmp      sl, #0
00780b24: beq      #0x780ab8
00780b28: ldr      r3, [sl]
00780b2c: mov      r0, sl
00780b30: mov      r1, #0x1a
00780b34: mov      lr, pc
00780b38: ldr      pc, [r3, #8]
00780b3c: cmp      r0, #0
00780b40: beq      #0x780ab8
00780b44: add      r1, sl, #0x38
00780b48: mov      r0, r6
00780b4c: bl       #0x4121f8
00780b50: b        #0x780ab8
00780b54: ldr      r7, [sp, #4]
00780b58: cmp      r7, #0
00780b5c: beq      #0x780b0c
00780b60: ldr      r3, [r7]
00780b64: mov      r0, r7
00780b68: mov      r1, #0x1c
00780b6c: mov      lr, pc
00780b70: ldr      pc, [r3, #8]
00780b74: cmp      r0, #0
00780b78: beq      #0x780b0c
00780b7c: mov      r0, r6
00780b80: add      r1, r7, #0x38
00780b84: bl       #0x75355c
00780b88: b        #0x780b0c
00780b8c: ldr      r0, [sp, #0x18]
00780b90: ldr      r1, [sp, #0x14]
00780b94: bl       #0x752b38
00780b98: b        #0x780b00
00780b9c: bl       #0x30e310
00780ba0: eoreq    r4, r1, r8, lsl r1
00780ba4: andeq    r4, r0, ip, lsr #1
00780ba8: andseq   r1, r6, ip, asr #28
00780bac: ldrsbeq  r8, [r8], -r4

# _ZN7gameswf15sprite_instanceC1EPNS_6playerEPNS_20movie_definition_subEPNS_4rootEPNS_9characterEi
00780718: push     {r4, r5, r6, r7, r8, lr}
0078071c: sub      sp, sp, #8
00780720: mov      r6, r2
00780724: mov      ip, #2
00780728: mov      r7, r3
0078072c: ldr      r5, [pc, #0x22c]
00780730: ldr      r3, [sp, #0x24]
00780734: ldr      r2, [sp, #0x20]
00780738: mov      r4, r0
0078073c: str      ip, [sp]
00780740: bl       #0x754b28
00780744: ldr      r3, [pc, #0x218]
00780748: add      r5, pc, r5
0078074c: cmp      r6, #0
00780750: ldr      r3, [r5, r3]
00780754: str      r6, [r4, #0xa0]
00780758: add      r3, r3, #8
0078075c: str      r3, [r4]
00780760: beq      #0x78076c
00780764: mov      r0, r6
00780768: bl       #0x759c64
0078076c: ldr      r3, [r4, #0xa0]
00780770: mov      r5, #0
00780774: mov      r2, #1
00780778: str      r7, [r4, #0xa4]
0078077c: strb     r2, [r4, #0xea]
00780780: str      r5, [r4, #0xa8]
00780784: str      r5, [r4, #0xac]
00780788: str      r5, [r4, #0xb0]
0078078c: strb     r5, [r4, #0xb4]
00780790: str      r5, [r4, #0xb8]
00780794: str      r5, [r4, #0xbc]
00780798: str      r5, [r4, #0xc0]
0078079c: str      r5, [r4, #0xc4]
007807a0: strb     r5, [r4, #0xc8]
007807a4: str      r5, [r4, #0xcc]
007807a8: str      r5, [r4, #0xd0]
007807ac: str      r5, [r4, #0xd4]
007807b0: strb     r5, [r4, #0xd8]
007807b4: str      r5, [r4, #0xdc]
007807b8: str      r5, [r4, #0xe0]
007807bc: strh     r5, [r4, #0xe4]
007807c0: strb     r5, [r4, #0xe6]
007807c4: strb     r5, [r4, #0xe7]
007807c8: strb     r2, [r4, #0xe8]
007807cc: strb     r5, [r4, #0xe9]
007807d0: strb     r5, [r4, #0xeb]
007807d4: strb     r5, [r4, #0xec]
007807d8: str      r5, [r4, #0xf0]
007807dc: str      r5, [r4, #0xf4]
007807e0: str      r5, [r4, #0xf8]
007807e4: str      r5, [r4, #0xfc]
007807e8: mov      r0, r3
007807ec: ldr      r3, [r3]
007807f0: mov      lr, pc
007807f4: ldr      pc, [r3, #0x88]
007807f8: cmp      r0, r5
007807fc: bne      #0x780858
00780800: ldr      r1, [r4, #0x30]
00780804: cmp      r1, #0
00780808: mov      r3, r1
0078080c: beq      #0x780820
00780810: ldr      r0, [r4, #0x2c]
00780814: ldrb     r2, [r0, #4]
00780818: cmp      r2, #0
0078081c: beq      #0x780914
00780820: ldr      r3, [r3, #0x30]
00780824: cmp      r1, #0
00780828: str      r3, [r4, #0x34]
0078082c: beq      #0x780840
00780830: ldr      r0, [r4, #0x2c]
00780834: ldrb     r3, [r0, #4]
00780838: cmp      r3, #0
0078083c: beq      #0x7808ec
00780840: mov      r0, r4
00780844: add      r1, r1, #0x88
00780848: bl       #0x768d08
0078084c: mov      r0, r4
00780850: add      sp, sp, #8
00780854: pop      {r4, r5, r6, r7, r8, pc}
00780858: mov      r1, r5
0078085c: mov      r0, #0x20
00780860: bl       #0x752ba8
00780864: strb     r5, [r0, #0x1c]
00780868: str      r5, [r0]
0078086c: str      r5, [r0, #4]
00780870: str      r5, [r0, #8]
00780874: strb     r5, [r0, #0xc]
00780878: str      r5, [r0, #0x10]
0078087c: str      r5, [r0, #0x14]
00780880: str      r5, [r0, #0x18]
00780884: ldr      r3, [r4, #0xa0]
00780888: str      r0, [r4, #0xdc]
0078088c: mov      r7, r0
00780890: add      r8, r0, #0x10
00780894: mov      r0, r3
00780898: ldr      r3, [r3]
0078089c: mov      lr, pc
007808a0: ldr      pc, [r3, #0x38]
007808a4: subs     r6, r0, #0
007808a8: ldr      r5, [r7, #0x14]
007808ac: bne      #0x780944
007808b0: cmp      r6, r5
007808b4: ble      #0x7808d0
007808b8: mov      r2, #0
007808bc: ldr      r3, [r8]
007808c0: strb     r2, [r3, r5]
007808c4: add      r5, r5, #1
007808c8: cmp      r5, r6
007808cc: bne      #0x7808bc
007808d0: str      r6, [r7, #0x14]
007808d4: ldr      r3, [r4, #0xdc]
007808d8: mov      r1, #0
007808dc: ldr      r2, [r3, #0x14]
007808e0: ldr      r0, [r3, #0x10]
007808e4: bl       #0x30e460
007808e8: b        #0x780800
007808ec: ldr      r1, [r0]
007808f0: sub      r1, r1, #1
007808f4: cmp      r1, #0
007808f8: str      r1, [r0]
007808fc: bne      #0x780904
00780900: bl       #0x752b38
00780904: mov      r1, #0
00780908: str      r1, [r4, #0x2c]
0078090c: str      r1, [r4, #0x30]
00780910: b        #0x780840
00780914: ldr      r1, [r0]
00780918: sub      r1, r1, #1
0078091c: cmp      r1, #0
00780920: str      r1, [r0]
00780924: bne      #0x78092c
00780928: bl       #0x752b38
0078092c: mov      r2, #0
00780930: mov      r3, r2
00780934: str      r2, [r4, #0x2c]
00780938: str      r2, [r4, #0x30]
0078093c: mov      r1, r2
00780940: b        #0x780820
00780944: ldr      r3, [r7, #0x18]
00780948: cmp      r6, r3
0078094c: ble      #0x7808b0
00780950: mov      r0, r8
00780954: add      r1, r6, r6, asr #1
00780958: bl       #0x77e930
0078095c: b        #0x7808b0
00780960: eoreq    r4, r1, r8, asr #6
00780964: andeq    r4, r0, r4, ror fp

# _ZNK7gameswf9character10is_enabledEv
00752dd4: mov      r0, #1
00752dd8: bx       lr

# _ZNK7gameswf15sprite_instance10is_enabledEv
00782de4: push     {r4, lr}
00782de8: mov      r4, r0
00782dec: ldrb     r0, [r0, #0xea]
00782df0: cmp      r0, #0
00782df4: bne      #0x782dfc
00782df8: pop      {r4, pc}
00782dfc: ldr      r3, [r4, #0x40]
00782e00: cmp      r3, #0
00782e04: beq      #0x782e58
00782e08: ldr      r0, [r4, #0x3c]
00782e0c: ldrb     r2, [r0, #4]
00782e10: cmp      r2, #0
00782e14: beq      #0x782e2c
00782e18: mov      r0, r3
00782e1c: ldr      r3, [r3]
00782e20: mov      lr, pc
00782e24: ldr      pc, [r3, #0x170]
00782e28: pop      {r4, pc}
00782e2c: ldr      r1, [r0]
00782e30: sub      r1, r1, #1
00782e34: cmp      r1, #0
00782e38: str      r1, [r0]
00782e3c: bne      #0x782e44
00782e40: bl       #0x752b38
00782e44: mov      r3, #0
00782e48: str      r3, [r4, #0x40]
00782e4c: str      r3, [r4, #0x3c]
00782e50: mov      r0, #1
00782e54: pop      {r4, pc}
00782e58: mov      r0, #1
00782e5c: pop      {r4, pc}

# _ZN7gameswf9characterC1EPNS_6playerEPS0_ii
00754a1c: push     {r4, r5, r6, r7, r8, lr}
00754a20: ldr      r4, [pc, #0xe8]
00754a24: mov      r6, r0
00754a28: mov      r5, r2
00754a2c: mov      r8, r3
00754a30: bl       #0x76bce0
00754a34: ldr      r3, [pc, #0xd8]
00754a38: add      r4, pc, r4
00754a3c: mov      r7, #0
00754a40: ldr      r3, [r4, r3]
00754a44: str      r8, [r6, #0x38]
00754a48: mov      r1, r5
00754a4c: add      r3, r3, #8
00754a50: str      r3, [r6]
00754a54: add      r0, r6, #0x3c
00754a58: str      r7, [r6, #0x3c]
00754a5c: str      r7, [r6, #0x40]
00754a60: bl       #0x427ba8
00754a64: ldr      r3, [pc, #0xac]
00754a68: ldr      r1, [pc, #0xac]
00754a6c: mov      r2, #0
00754a70: ldr      r5, [r4, r3]
00754a74: ldr      r3, [pc, #0xa4]
00754a78: add      r1, pc, r1
00754a7c: add      r8, r1, #0xc
00754a80: ldr      ip, [r4, r3]
00754a84: ldr      r3, [pc, #0x98]
00754a88: str      r8, [r6, #0x44]
00754a8c: str      r5, [r6, #0x48]
00754a90: ldr      r4, [r4, r3]
00754a94: mov      r3, #0x3f800000
00754a98: str      ip, [r6, #0x50]
00754a9c: str      r4, [r6, #0x4c]
00754aa0: str      r3, [r6, #0x88]
00754aa4: str      r2, [r6, #0x90]
00754aa8: strh     r7, [r6, #0x96]
00754aac: ldr      ip, [sp, #0x18]
00754ab0: mov      r1, #1
00754ab4: strb     r7, [r6, #0x9c]
00754ab8: strb     ip, [r6, #0x98]
00754abc: strb     r1, [r6, #0x9d]
00754ac0: str      r7, [r6, #0x54]
00754ac4: str      r3, [r6, #0x58]
00754ac8: str      r3, [r6, #0x60]
00754acc: str      r3, [r6, #0x68]
00754ad0: str      r3, [r6, #0x70]
00754ad4: str      r2, [r6, #0x5c]
00754ad8: str      r2, [r6, #0x64]
00754adc: str      r2, [r6, #0x6c]
00754ae0: str      r2, [r6, #0x74]
00754ae4: str      r7, [r6, #0x7c]
00754ae8: str      r7, [r6, #0x80]
00754aec: str      r7, [r6, #0x84]
00754af0: str      r7, [r6, #0x8c]
00754af4: str      r3, [r6, #0x78]
00754af8: strh     r7, [r6, #0x94]
00754afc: strb     r1, [r6, #0x99]
00754b00: strb     r1, [r6, #0x9a]
00754b04: strb     r1, [r6, #0x9b]
00754b08: mov      r0, r6
00754b0c: pop      {r4, r5, r6, r7, r8, pc}
00754b10: eoreq    r0, r4, r8, asr r0
00754b14: andeq    r3, r0, r4, ror #3
00754b18: andeq    r3, r0, r4, lsl #9
00754b1c: eoreq    r7, sl, r8, lsl #20
00754b20: andeq    r3, r0, r4, ror #27
00754b24: andeq    r4, r0, r8, asr #1
