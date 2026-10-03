
# _ZN10LuaManager18FlushBufferedFilesEv
00379fe8: push     {r4, r5, r6, lr}
00379fec: ldr      r6, [r0, #0xc]
00379ff0: mov      r5, r0
00379ff4: add      r4, r0, #4
00379ff8: cmp      r4, r6
00379ffc: beq      #0x37a048
0037a000: ldr      r3, [r6, #0x28]
0037a004: cmp      r3, #0
0037a008: beq      #0x37a01c
0037a00c: mov      r0, r3
0037a010: ldr      r3, [r3]
0037a014: mov      lr, pc
0037a018: ldr      pc, [r3, #4]
0037a01c: ldr      r2, [r6, #0xc]
0037a020: cmp      r2, #0
0037a024: bne      #0x37a030
0037a028: b        #0x37a078
0037a02c: mov      r2, r3
0037a030: ldr      r3, [r2, #8]
0037a034: cmp      r3, #0
0037a038: bne      #0x37a02c
0037a03c: mov      r6, r2
0037a040: cmp      r4, r6
0037a044: bne      #0x37a000
0037a048: ldr      r3, [r5, #0x14]
0037a04c: cmp      r3, #0
0037a050: beq      #0x37a074
0037a054: mov      r0, r4
0037a058: ldr      r1, [r5, #8]
0037a05c: bl       #0x379fa8
0037a060: mov      r3, #0
0037a064: str      r3, [r5, #0x14]
0037a068: str      r4, [r5, #0x10]
0037a06c: str      r4, [r5, #0xc]
0037a070: str      r3, [r5, #8]
0037a074: pop      {r4, r5, r6, pc}
0037a078: ldr      r3, [r6, #4]
0037a07c: ldr      r1, [r3, #0xc]
0037a080: cmp      r6, r1
0037a084: bne      #0x37a0a0
0037a088: mov      r6, r3
0037a08c: ldr      r3, [r3, #4]
0037a090: ldr      r2, [r3, #0xc]
0037a094: cmp      r2, r6
0037a098: beq      #0x37a088
0037a09c: ldr      r2, [r6, #0xc]
0037a0a0: cmp      r2, r3
0037a0a4: movne    r6, r3
0037a0a8: b        #0x379ff8
