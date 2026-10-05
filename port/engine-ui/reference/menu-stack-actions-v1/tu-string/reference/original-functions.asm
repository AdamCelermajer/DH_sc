
# _ZNK7gameswf8as_value12to_tu_stringEv
00420a84: push     {r4, r5, r6, lr}
00420a88: ldrsb    r3, [r0, #1]
00420a8c: ldr      r5, [pc, #0xa0]
00420a90: cmp      r3, #3
00420a94: add      r5, pc, r5
00420a98: beq      #0x420b2c
00420a9c: cmp      r3, #4
00420aa0: beq      #0x420b2c
00420aa4: ldr      r3, [pc, #0x8c]
00420aa8: ldr      r4, [r5, r3]
00420aac: ldr      r6, [r4]
00420ab0: ands     r6, r6, #1
00420ab4: beq      #0x420ac4
00420ab8: ldr      r3, [pc, #0x7c]
00420abc: ldr      r0, [r5, r3]
00420ac0: pop      {r4, r5, r6, pc}
00420ac4: mov      r0, r4
00420ac8: bl       #0x30e76c
00420acc: cmp      r0, #0
00420ad0: beq      #0x420ab8
00420ad4: ldr      r3, [pc, #0x60]
00420ad8: mov      r0, r4
00420adc: mov      r2, #1
00420ae0: ldr      r4, [r5, r3]
00420ae4: ldr      r3, [r4, #0x10]
00420ae8: strb     r2, [r4]
00420aec: mvn      r2, #0
00420af0: bfi      r3, r2, #0, #0x18
00420af4: lsr      r2, r3, #0x18
00420af8: bfi      r2, r6, #0, #1
00420afc: str      r3, [r4, #0x10]
00420b00: strb     r6, [r4, #1]
00420b04: strb     r2, [r4, #0x13]
00420b08: bl       #0x30ea3c
00420b0c: ldr      r3, [pc, #0x2c]
00420b10: mov      r0, r4
00420b14: ldr      r1, [r5, r3]
00420b18: ldr      r3, [pc, #0x24]
00420b1c: ldr      r2, [r5, r3]
00420b20: bl       #0x30e304
00420b24: mov      r0, r4
00420b28: pop      {r4, r5, r6, pc}
00420b2c: ldr      r0, [r0, #4]
00420b30: pop      {r4, r5, r6, pc}
00420b34: ldrsheq  r3, [r7], #-0xfc
00420b38: andeq    r3, r0, r8, asr #27
00420b3c: andeq    r3, r0, r4, lsl #26
00420b40: andeq    r1, r0, r4, lsl #31
00420b44: muleq    r0, r0, r8
