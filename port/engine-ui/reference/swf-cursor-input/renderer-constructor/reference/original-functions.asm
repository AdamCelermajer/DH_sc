
# _ZN8RenderFXC1Ev
007a850c: push     {r4, r5, r6, r7, r8, sb, sl, lr}
007a8510: ldr      r3, [pc, #0xd8]
007a8514: ldr      r1, [pc, #0xd8]
007a8518: ldr      r2, [r0, #0x54]
007a851c: add      r3, pc, r3
007a8520: mov      r5, r0
007a8524: ldr      r1, [r3, r1]
007a8528: mvn      r0, #0
007a852c: bfi      r2, r0, #0, #0x18
007a8530: mov      r6, #0
007a8534: lsr      r0, r2, #0x18
007a8538: add      r1, r1, #8
007a853c: bfi      r0, r6, #0, #1
007a8540: mov      sl, #1
007a8544: str      r2, [r5, #0x54]
007a8548: mov      r7, #0
007a854c: str      r1, [r5]
007a8550: strb     r0, [r5, #0x57]
007a8554: str      r6, [r5, #4]
007a8558: str      r6, [r5, #8]
007a855c: str      r6, [r5, #0xc]
007a8560: strb     r6, [r5, #0x10]
007a8564: str      r6, [r5, #0x14]
007a8568: str      r6, [r5, #0x18]
007a856c: str      r6, [r5, #0x1c]
007a8570: str      r6, [r5, #0x20]
007a8574: strb     r6, [r5, #0x24]
007a8578: str      r6, [r5, #0x28]
007a857c: str      r6, [r5, #0x2c]
007a8580: str      r6, [r5, #0x30]
007a8584: strb     r6, [r5, #0x34]
007a8588: str      r6, [r5, #0x38]
007a858c: str      r6, [r5, #0x3c]
007a8590: str      r6, [r5, #0x40]
007a8594: strb     sl, [r5, #0x44]
007a8598: strb     r6, [r5, #0x45]
007a859c: add      r4, r5, #0x58
007a85a0: add      r8, r5, #0xf8
007a85a4: str      r7, [r4, #4]
007a85a8: str      r7, [r4]
007a85ac: str      r7, [r4, #8]
007a85b0: str      r6, [r4, #0xc]
007a85b4: str      r6, [r4, #0x10]
007a85b8: str      r6, [r4, #0x14]
007a85bc: str      r6, [r4, #0x18]
007a85c0: str      r6, [r4, #0x1c]
007a85c4: str      r6, [r4, #0x20]
007a85c8: strb     sl, [r4, #0x24]
007a85cc: mov      r0, r4
007a85d0: add      r4, r4, #0x28
007a85d4: bl       #0x7a84c4
007a85d8: cmp      r4, r8
007a85dc: bne      #0x7a85a4
007a85e0: str      r6, [r5, #0xfc]
007a85e4: str      r6, [r5, #0xf8]
007a85e8: mov      r0, r5
007a85ec: pop      {r4, r5, r6, r7, r8, sb, sl, pc}
007a85f0: andseq   ip, lr, r4, ror r5
007a85f4: andeq    r4, r0, r0, ror #18
