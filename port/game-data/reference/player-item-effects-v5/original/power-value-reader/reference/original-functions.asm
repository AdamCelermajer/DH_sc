
# _ZN12StreamReader6readAsIaEEvP11IStreamBasePT_
004db9fc: str      lr, [sp, #-4]!
004dba00: mov      r3, #0
004dba04: sub      sp, sp, #0xc
004dba08: ldr      ip, [r0]
004dba0c: mov      r2, #1
004dba10: mov      lr, pc
004dba14: ldr      pc, [ip, #0x18]
004dba18: ldr      r3, [pc, #0x74]
004dba1c: cmp      r0, #1
004dba20: add      r3, pc, r3
004dba24: beq      #0x4dba54
004dba28: ldr      r2, [pc, #0x68]
004dba2c: ldr      r2, [r3, r2]
004dba30: ldr      r2, [r2]
004dba34: cmp      r2, #2
004dba38: moveq    r3, #0
004dba3c: streq    r3, [r3]
004dba40: beq      #0x4dba4c
004dba44: cmp      r2, #1
004dba48: beq      #0x4dba60
004dba4c: add      sp, sp, #0xc
004dba50: ldm      sp!, {pc}
004dba54: cmp      r1, #0
004dba58: beq      #0x4dba4c
004dba5c: b        #0x4dba28
004dba60: ldr      r0, [pc, #0x34]
004dba64: ldr      r1, [pc, #0x34]
004dba68: ldr      r2, [pc, #0x34]
004dba6c: ldr      r0, [r3, r0]
004dba70: ldr      r3, [pc, #0x30]
004dba74: mov      ip, #0x50
004dba78: add      r1, pc, r1
004dba7c: add      r2, pc, r2
004dba80: add      r3, pc, r3
004dba84: add      r0, r0, #0xa8
004dba88: str      ip, [sp]
004dba8c: bl       #0x30e004
004dba90: b        #0x4dba4c
004dba94: subeq    sb, fp, r0, ror r0
004dba98: andeq    r3, r0, r0, asr #19
004dba9c: andeq    r1, r0, r0, asr #19
004dbaa0: eorseq   r2, lr, r0, ror #18
004dbaa4: eorseq   r2, lr, r4, lsl #21
004dbaa8: eorseq   r4, lr, r0, asr #5

# _ZN7Structs9LootEntry8finalizeEv
004c6c08: bx       lr

# _ZN12StreamReader6readAsIiEEvP11IStreamBasePT_
00459090: str      lr, [sp, #-4]!
00459094: mov      r3, #0
00459098: sub      sp, sp, #0xc
0045909c: ldr      ip, [r0]
004590a0: mov      r2, #4
004590a4: mov      lr, pc
004590a8: ldr      pc, [ip, #0x18]
004590ac: ldr      r3, [pc, #0x74]
004590b0: cmp      r0, #4
004590b4: add      r3, pc, r3
004590b8: beq      #0x4590e8
004590bc: ldr      r2, [pc, #0x68]
004590c0: ldr      r2, [r3, r2]
004590c4: ldr      r2, [r2]
004590c8: cmp      r2, #2
004590cc: moveq    r3, #0
004590d0: streq    r3, [r3]
004590d4: beq      #0x4590e0
004590d8: cmp      r2, #1
004590dc: beq      #0x4590f4
004590e0: add      sp, sp, #0xc
004590e4: ldm      sp!, {pc}
004590e8: cmp      r1, #0
004590ec: beq      #0x4590e0
004590f0: b        #0x4590bc
004590f4: ldr      r0, [pc, #0x34]
004590f8: ldr      r1, [pc, #0x34]
004590fc: ldr      r2, [pc, #0x34]
00459100: ldr      r0, [r3, r0]
00459104: ldr      r3, [pc, #0x30]
00459108: mov      ip, #0x50
0045910c: add      r1, pc, r1
00459110: add      r2, pc, r2
00459114: add      r3, pc, r3
00459118: add      r0, r0, #0xa8
0045911c: str      ip, [sp]
00459120: bl       #0x30e004
00459124: b        #0x4590e0
00459128: ldrsbeq  fp, [r3], #-0x9c
0045912c: andeq    r3, r0, r0, asr #19
00459130: andeq    r1, r0, r0, asr #19
00459134: subeq    r5, r6, ip, asr #5
00459138: strdeq   r5, r6, [r6], #-0x30
0045913c: subeq    r6, r6, ip, lsr #24
