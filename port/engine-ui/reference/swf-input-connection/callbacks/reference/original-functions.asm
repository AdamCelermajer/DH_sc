
# _ZN8RenderFX16InvokeASCallbackEPN7gameswf9characterEPKcPKNS0_8as_valueEi
007abe0c: push     {r4, r5, r6, r7, r8, sl, lr}
007abe10: ldr      r4, [pc, #0x114]
007abe14: ldr      r6, [pc, #0x114]
007abe18: subs     r5, r1, #0
007abe1c: add      r4, pc, r4
007abe20: ldr      r1, [r4, r6]
007abe24: mov      r7, r3
007abe28: sub      sp, sp, #0x24
007abe2c: ldr      r3, [r1]
007abe30: mov      r8, r2
007abe34: str      r3, [sp, #0x1c]
007abe38: beq      #0x7abed8
007abe3c: ldr      r3, [r5]
007abe40: mov      r0, r5
007abe44: mov      r1, #2
007abe48: mov      lr, pc
007abe4c: ldr      pc, [r3, #8]
007abe50: cmp      r0, #0
007abe54: movne    sl, r5
007abe58: beq      #0x7abec4
007abe5c: mov      r0, r5
007abe60: bl       #0x759c64
007abe64: ldr      r3, [sl]
007abe68: mov      r0, sl
007abe6c: mov      lr, pc
007abe70: ldr      pc, [r3, #0x58]
007abe74: ldr      ip, [sp, #0x40]
007abe78: mov      r1, r0
007abe7c: mov      r3, r8
007abe80: add      r0, sp, #8
007abe84: mov      r2, r5
007abe88: stm      sp, {r7, ip}
007abe8c: bl       #0x7bbbfc
007abe90: ldrsb    r3, [sp, #8]
007abe94: cmn      r3, #1
007abe98: beq      #0x7abee0
007abe9c: mov      r0, r5
007abea0: bl       #0x75a240
007abea4: mov      r0, #1
007abea8: ldr      r3, [r4, r6]
007abeac: ldr      r2, [sp, #0x1c]
007abeb0: ldr      r3, [r3]
007abeb4: cmp      r2, r3
007abeb8: bne      #0x7abf28
007abebc: add      sp, sp, #0x24
007abec0: pop      {r4, r5, r6, r7, r8, sl, pc}
007abec4: add      sl, r5, #0x3c
007abec8: mov      r0, sl
007abecc: bl       #0x438224
007abed0: cmp      r0, #0
007abed4: bne      #0x7abef0
007abed8: mov      r0, #0
007abedc: b        #0x7abea8
007abee0: ldr      r0, [sp, #0x14]
007abee4: ldr      r1, [sp, #0x10]
007abee8: bl       #0x752b38
007abeec: b        #0x7abe9c
007abef0: mov      r0, sl
007abef4: bl       #0x438224
007abef8: mov      r1, #2
007abefc: ldr      r3, [r0]
007abf00: mov      lr, pc
007abf04: ldr      pc, [r3, #8]
007abf08: cmp      r0, #0
007abf0c: beq      #0x7abed8
007abf10: mov      r0, sl
007abf14: bl       #0x438224
007abf18: subs     sl, r0, #0
007abf1c: bne      #0x7abe5c
007abf20: mov      r0, #0
007abf24: b        #0x7abea8
007abf28: bl       #0x30e310
007abf2c: andseq   r8, lr, r4, ror ip
007abf30: andeq    r4, r0, ip, lsr #1

# _ZN8MenuBase7OnEventERN8RenderFX5EventE
00423214: push     {r4, r5, r6, r7, r8, lr}
00423218: mov      r5, r0
0042321c: ldr      r0, [r0, #0x5c]
00423220: ldr      r6, [pc, #0xe0]
00423224: mov      r4, r1
00423228: cmp      r0, #0
0042322c: add      r6, pc, r6
00423230: beq      #0x423238
00423234: bl       #0x4130c8
00423238: ldr      r3, [pc, #0xcc]
0042323c: ldr      r3, [r6, r3]
00423240: ldrb     r3, [r3]
00423244: cmp      r3, #0
00423248: beq      #0x4232cc
0042324c: ldr      r3, [r4, #8]
00423250: cmp      r3, #8
00423254: mov      r6, r3
00423258: beq      #0x423274
0042325c: cmp      r3, #6
00423260: beq      #0x4232d8
00423264: mov      r0, r5
00423268: mov      r1, r4
0042326c: pop      {r4, r5, r6, r7, r8, lr}
00423270: b        #0x4231b4
00423274: ldr      r7, [r4, #4]
00423278: ldr      r1, [pc, #0x90]
0042327c: mov      r0, r7
00423280: add      r1, pc, r1
00423284: bl       #0x30ebd4
00423288: cmp      r7, r0
0042328c: bne      #0x423300
00423290: ldr      r3, [r4]
00423294: mov      r0, r3
00423298: ldr      r3, [r3]
0042329c: mov      lr, pc
004232a0: ldr      pc, [r3, #0x170]
004232a4: cmp      r0, #0
004232a8: ldreq    r6, [r4, #8]
004232ac: beq      #0x423300
004232b0: mov      r0, r4
004232b4: ldr      r6, [r5, #4]
004232b8: bl       #0x421e48
004232bc: mov      r2, #0
004232c0: mov      r1, r0
004232c4: mov      r0, r6
004232c8: bl       #0x7ac498
004232cc: ldr      r3, [r4, #8]
004232d0: cmp      r3, #6
004232d4: bne      #0x423264
004232d8: ldr      r1, [pc, #0x34]
004232dc: ldr      r0, [r4, #4]
004232e0: add      r1, pc, r1
004232e4: bl       #0x30ebd4
004232e8: cmp      r0, #0
004232ec: beq      #0x423264
004232f0: ldr      r0, [pc, #0x20]
004232f4: add      r0, pc, r0
004232f8: bl       #0x532c48
004232fc: b        #0x423264
00423300: mov      r3, r6
00423304: b        #0x42325c
00423308: subseq   r1, r7, r4, ror #16
0042330c: andeq    r3, r0, r4, ror #14
00423310: subeq    r5, sl, r0, lsl #28
00423314: subeq    r5, sl, r8, lsr #27
00423318: subeq    r5, sl, ip, lsr #27

# _ZN6MenuFX7OnEventERN8RenderFX5EventE
007adf20: push     {r4, lr}
007adf24: ldr      r3, [r0, #0x118]
007adf28: mov      r4, r1
007adf2c: cmp      r3, #0
007adf30: ble      #0x7adf4c
007adf34: ldr      r1, [r1]
007adf38: bl       #0x7addb0
007adf3c: mov      r1, r4
007adf40: ldr      r3, [r0]
007adf44: mov      lr, pc
007adf48: ldr      pc, [r3, #0x2c]
007adf4c: pop      {r4, pc}

# _ZN6MenuFX14CanHandleEventERN8RenderFX5EventE
007adee0: push     {r4, lr}
007adee4: ldr      r3, [r0, #0x118]
007adee8: mov      r4, r1
007adeec: cmp      r3, #0
007adef0: ble      #0x7adf10
007adef4: ldr      r1, [r1]
007adef8: bl       #0x7addb0
007adefc: mov      r1, r4
007adf00: ldr      r3, [r0]
007adf04: mov      lr, pc
007adf08: ldr      pc, [r3, #0x34]
007adf0c: pop      {r4, pc}
007adf10: mov      r0, #1
007adf14: pop      {r4, pc}

# _ZN11MenuManager7OnEventERN8RenderFX5EventE
0042d060: push     {r4, r5, r6, lr}
0042d064: ldr      r3, [r1, #8]
0042d068: ldr      r5, [pc, #0x70]
0042d06c: mov      r4, r1
0042d070: cmp      r3, #6
0042d074: mov      r6, r0
0042d078: add      r5, pc, r5
0042d07c: beq      #0x42d0b4
0042d080: bl       #0x41b11c
0042d084: mov      r1, r4
0042d088: ldr      r3, [r0]
0042d08c: mov      lr, pc
0042d090: ldr      pc, [r3, #0xc]
0042d094: ldr      r3, [r6, #0xf4]
0042d098: mov      r1, r4
0042d09c: ldr      r3, [r3, #0x140]
0042d0a0: mov      r0, r3
0042d0a4: ldr      r3, [r3]
0042d0a8: mov      lr, pc
0042d0ac: ldr      pc, [r3, #0x18]
0042d0b0: pop      {r4, r5, r6, pc}
0042d0b4: ldr      r0, [r1, #4]
0042d0b8: ldr      r1, [pc, #0x24]
0042d0bc: add      r1, pc, r1
0042d0c0: bl       #0x30ebd4
0042d0c4: cmp      r0, #0
0042d0c8: beq      #0x42d080
0042d0cc: ldr      r3, [pc, #0x14]
0042d0d0: mov      r2, #9
0042d0d4: ldr      r3, [r5, r3]
0042d0d8: str      r2, [r3]
0042d0dc: b        #0x42d080
0042d0e0: subseq   r7, r6, r8, lsl sl
0042d0e4: umaaleq  ip, sb, r4, lr
0042d0e8: andeq    r3, r0, r0, asr r8
