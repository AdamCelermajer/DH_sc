
# _ZN6CharAI14StepLoadScriptEv
003cdf7c: push     {r4, r5, r6, r7, lr}
003cdf80: ldr      r4, [pc, #0xac]
003cdf84: ldr      r6, [pc, #0xac]
003cdf88: ldr      r1, [r0, #0x30]
003cdf8c: add      r4, pc, r4
003cdf90: ldr      r3, [r4, r6]
003cdf94: sub      sp, sp, #0x24
003cdf98: cmp      r1, #0
003cdf9c: ldr      r3, [r3]
003cdfa0: str      r3, [sp, #0x1c]
003cdfa4: beq      #0x3ce00c
003cdfa8: ldr      r0, [r0, #0x20]
003cdfac: bl       #0x37b574
003cdfb0: ldr      r3, [pc, #0x84]
003cdfb4: add      r5, sp, #4
003cdfb8: ldr      r7, [r4, r3]
003cdfbc: mov      r0, r7
003cdfc0: bl       #0x337888
003cdfc4: ldr      r1, [pc, #0x74]
003cdfc8: mov      r2, sp
003cdfcc: mov      r0, r5
003cdfd0: add      r1, pc, r1
003cdfd4: bl       #0x3140ec
003cdfd8: mov      r0, r7
003cdfdc: mov      r1, r5
003cdfe0: bl       #0x337a88
003cdfe4: ldr      r0, [sp, #0x18]
003cdfe8: cmp      r0, r5
003cdfec: beq      #0x3ce00c
003cdff0: cmp      r0, #0
003cdff4: beq      #0x3ce00c
003cdff8: ldr      r1, [sp, #4]
003cdffc: rsb      r1, r0, r1
003ce000: cmp      r1, #0x80
003ce004: bhi      #0x3ce028
003ce008: bl       #0x708f00
003ce00c: ldr      r3, [r4, r6]
003ce010: ldr      r2, [sp, #0x1c]
003ce014: ldr      r3, [r3]
003ce018: cmp      r2, r3
003ce01c: bne      #0x3ce030
003ce020: add      sp, sp, #0x24
003ce024: pop      {r4, r5, r6, r7, pc}
003ce028: bl       #0x310440
003ce02c: b        #0x3ce00c
003ce030: bl       #0x30e310
003ce034: subseq   r6, ip, r4, lsl #22
003ce038: andeq    r4, r0, ip, lsr #1
003ce03c: andeq    r0, r0, r4, lsl #17
003ce040: subeq    r7, pc, r8, lsl #7

# _ZNK9Character6IsDeadEv
003a2ed4: movw     r3, #0x1449
003a2ed8: ldrb     r0, [r0, r3]
003a2edc: bx       lr
