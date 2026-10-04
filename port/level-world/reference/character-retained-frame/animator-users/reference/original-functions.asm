
# _ZN12CharAnimator15IncAnimSetUsersEv
003c9168: str      lr, [sp, #-4]!
003c916c: ldr      r3, [r0, #4]
003c9170: sub      sp, sp, #0xc
003c9174: ldr      r3, [r3, #0x2d8]
003c9178: cmp      r3, #0
003c917c: beq      #0x3c91c0
003c9180: ldr      r3, [r3, #0x38]
003c9184: add      r0, sp, #4
003c9188: mov      r1, r3
003c918c: ldr      r3, [r3]
003c9190: mov      lr, pc
003c9194: ldr      pc, [r3, #8]
003c9198: ldr      r3, [sp, #4]
003c919c: cmp      r3, #0
003c91a0: beq      #0x3c91c0
003c91a4: ldr      r2, [r3, #0x24]
003c91a8: add      r2, r2, #1
003c91ac: str      r2, [r3, #0x24]
003c91b0: ldr      r0, [sp, #4]
003c91b4: cmp      r0, #0
003c91b8: beq      #0x3c91c0
003c91bc: bl       #0x31d584
003c91c0: add      sp, sp, #0xc
003c91c4: ldm      sp!, {pc}

# _ZN12CharAnimator15DecAnimSetUsersEv
003c91c8: str      lr, [sp, #-4]!
003c91cc: ldr      r3, [r0, #4]
003c91d0: sub      sp, sp, #0xc
003c91d4: ldr      r3, [r3, #0x2d8]
003c91d8: cmp      r3, #0
003c91dc: beq      #0x3c9220
003c91e0: ldr      r3, [r3, #0x38]
003c91e4: add      r0, sp, #4
003c91e8: mov      r1, r3
003c91ec: ldr      r3, [r3]
003c91f0: mov      lr, pc
003c91f4: ldr      pc, [r3, #8]
003c91f8: ldr      r3, [sp, #4]
003c91fc: cmp      r3, #0
003c9200: beq      #0x3c9220
003c9204: ldr      r2, [r3, #0x24]
003c9208: sub      r2, r2, #1
003c920c: str      r2, [r3, #0x24]
003c9210: ldr      r0, [sp, #4]
003c9214: cmp      r0, #0
003c9218: beq      #0x3c9220
003c921c: bl       #0x31d584
003c9220: add      sp, sp, #0xc
003c9224: ldm      sp!, {pc}
