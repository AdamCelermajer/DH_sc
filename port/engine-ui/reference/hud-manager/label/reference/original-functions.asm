
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
