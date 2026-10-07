
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

# _ZN8RenderFX8SetFocusEPN7gameswf9characterEi
007ac228: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
007ac22c: ldr      r4, [pc, #0x1cc]
007ac230: ldr      sl, [pc, #0x1cc]
007ac234: mov      r8, r2
007ac238: add      r4, pc, r4
007ac23c: ldr      r3, [r4, sl]
007ac240: sub      sp, sp, #0x134
007ac244: mov      r5, r0
007ac248: ldr      r2, [r3]
007ac24c: mov      r3, #0x28
007ac250: mla      r3, r3, r8, r0
007ac254: str      r2, [sp, #0x12c]
007ac258: ldr      r7, [r3, #0x68]
007ac25c: mov      r6, r1
007ac260: cmp      r1, r7
007ac264: beq      #0x7ac3b8
007ac268: ldr      sb, [r0, #0xf8]
007ac26c: ands     sb, sb, #0x40
007ac270: bne      #0x7ac314
007ac274: cmp      r7, #0
007ac278: beq      #0x7ac314
007ac27c: ldr      r3, [r7]
007ac280: mov      r0, r7
007ac284: mov      r1, #2
007ac288: mov      lr, pc
007ac28c: ldr      pc, [r3, #8]
007ac290: cmp      r0, #0
007ac294: beq      #0x7ac314
007ac298: ldrb     r3, [r7, #0xea]
007ac29c: cmp      r3, #0
007ac2a0: beq      #0x7ac314
007ac2a4: ldr      r2, [pc, #0x15c]
007ac2a8: mov      r1, r7
007ac2ac: mov      r3, sb
007ac2b0: add      r2, pc, r2
007ac2b4: mov      r0, r5
007ac2b8: bl       #0x7aba04
007ac2bc: mov      r3, #0
007ac2c0: mov      r2, #1
007ac2c4: str      sb, [sp, #0x1c]
007ac2c8: str      r3, [sp, #0x18]
007ac2cc: str      r2, [sp, #0xc]
007ac2d0: str      r3, [sp, #0x14]
007ac2d4: str      r3, [sp, #0x10]
007ac2d8: str      r7, [sp, #4]
007ac2dc: ldr      r2, [r7, #0x44]
007ac2e0: mov      r0, r5
007ac2e4: add      r1, sp, #4
007ac2e8: ldrsb    r3, [r2]
007ac2ec: cmn      r3, #1
007ac2f0: ldreq    r2, [r2, #0xc]
007ac2f4: mov      r3, #0
007ac2f8: addne    r2, r2, #1
007ac2fc: str      r2, [sp, #8]
007ac300: strb     r3, [sp, #0x28]
007ac304: strb     r3, [sp, #0x29]
007ac308: str      r3, [sp, #0x20]
007ac30c: str      r8, [sp, #0x24]
007ac310: bl       #0x7abf34
007ac314: mov      fp, #0x28
007ac318: mul      fp, fp, r8
007ac31c: mov      r1, r6
007ac320: add      fp, fp, #0x68
007ac324: add      fp, r5, fp
007ac328: mov      r0, fp
007ac32c: bl       #0x75518c
007ac330: ldr      r3, [r5, #0xf8]
007ac334: ands     r3, r3, #0x40
007ac338: bne      #0x7ac3b8
007ac33c: cmp      r6, #0
007ac340: beq      #0x7ac3b8
007ac344: ldr      r1, [r6, #0x44]
007ac348: mov      r2, #0
007ac34c: str      r3, [sp, #0xc]
007ac350: str      r2, [sp, #0x18]
007ac354: str      r2, [sp, #0x14]
007ac358: str      r2, [sp, #0x10]
007ac35c: str      r3, [sp, #0x1c]
007ac360: str      r6, [sp, #4]
007ac364: ldrsb    r3, [r1]
007ac368: mov      sb, #0
007ac36c: add      r7, sp, #4
007ac370: cmn      r3, #1
007ac374: ldreq    r1, [r1, #0xc]
007ac378: ldr      r3, [r5, #0xfc]
007ac37c: addne    r1, r1, #1
007ac380: str      r1, [sp, #8]
007ac384: str      r8, [sp, #0x24]
007ac388: strb     sb, [sp, #0x29]
007ac38c: str      sb, [sp, #0x20]
007ac390: strb     sb, [sp, #0x28]
007ac394: mov      r0, r3
007ac398: mov      r1, r7
007ac39c: ldr      r3, [r3]
007ac3a0: mov      lr, pc
007ac3a4: ldr      pc, [r3, #8]
007ac3a8: subs     r1, r0, #0
007ac3ac: bne      #0x7ac3d4
007ac3b0: mov      r0, fp
007ac3b4: bl       #0x75518c
007ac3b8: ldr      r3, [r4, sl]
007ac3bc: ldr      r2, [sp, #0x12c]
007ac3c0: ldr      r3, [r3]
007ac3c4: cmp      r2, r3
007ac3c8: bne      #0x7ac3fc
007ac3cc: add      sp, sp, #0x134
007ac3d0: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
007ac3d4: ldr      r2, [pc, #0x30]
007ac3d8: mov      r1, r6
007ac3dc: mov      r3, sb
007ac3e0: add      r2, pc, r2
007ac3e4: mov      r0, r5
007ac3e8: bl       #0x7aba04
007ac3ec: mov      r0, r5
007ac3f0: mov      r1, r7
007ac3f4: bl       #0x7abf34
007ac3f8: b        #0x7ac3b8
007ac3fc: bl       #0x30e310
007ac400: andseq   r8, lr, r8, asr r8
007ac404: andeq    r4, r0, ip, lsr #1
007ac408: ldrsheq  pc, [r1], -r0
007ac40c: andseq   pc, r1, r8, lsr r7

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

# _ZN8RenderFX9GotoFrameEPN7gameswf9characterEPKcb
007ab924: push     {r4, r5, r6, r7, r8, sl, lr}
007ab928: ldr      r4, [pc, #0xcc]
007ab92c: ldr      r6, [pc, #0xcc]
007ab930: mov      r8, r3
007ab934: add      r4, pc, r4
007ab938: ldr      r0, [r4, r6]
007ab93c: sub      sp, sp, #0x1c
007ab940: subs     r5, r1, #0
007ab944: ldr      r3, [r0]
007ab948: mov      r7, r2
007ab94c: str      r3, [sp, #0x14]
007ab950: beq      #0x7ab9c8
007ab954: ldr      r2, [r5]
007ab958: mov      r0, r5
007ab95c: mov      r1, #2
007ab960: mov      lr, pc
007ab964: ldr      pc, [r2, #8]
007ab968: cmp      r0, #0
007ab96c: beq      #0x7ab9c8
007ab970: ldr      r3, [r5]
007ab974: mov      r1, r7
007ab978: mov      r0, sp
007ab97c: ldr      r7, [r3, #0x9c]
007ab980: bl       #0x413a7c
007ab984: mov      r0, r5
007ab988: mov      r1, sp
007ab98c: blx      r7
007ab990: ldrsb    r3, [sp]
007ab994: mov      sl, sp
007ab998: mov      r7, r0
007ab99c: cmn      r3, #1
007ab9a0: beq      #0x7ab9e8
007ab9a4: cmp      r7, #0
007ab9a8: beq      #0x7ab9c8
007ab9ac: mov      r0, r5
007ab9b0: eor      r1, r8, #1
007ab9b4: ldr      r3, [r5]
007ab9b8: mov      lr, pc
007ab9bc: ldr      pc, [r3, #0x94]
007ab9c0: mov      r0, #1
007ab9c4: b        #0x7ab9cc
007ab9c8: mov      r0, #0
007ab9cc: ldr      r3, [r4, r6]
007ab9d0: ldr      r2, [sp, #0x14]
007ab9d4: ldr      r3, [r3]
007ab9d8: cmp      r2, r3
007ab9dc: bne      #0x7ab9f8
007ab9e0: add      sp, sp, #0x1c
007ab9e4: pop      {r4, r5, r6, r7, r8, sl, pc}
007ab9e8: ldr      r0, [sp, #0xc]
007ab9ec: ldr      r1, [sp, #8]
007ab9f0: bl       #0x752b38
007ab9f4: b        #0x7ab9a4
007ab9f8: bl       #0x30e310
007ab9fc: andseq   sb, lr, ip, asr r1
007aba00: andeq    r4, r0, ip, lsr #1
