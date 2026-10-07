
# _ZN7gameswf9character19notify_need_advanceEv
007750e8: push     {r4, lr}
007750ec: mov      r4, r0
007750f0: ldr      r3, [r4, #0x40]
007750f4: mov      r1, #1
007750f8: strb     r1, [r4, #0x9d]
007750fc: cmp      r3, #0
00775100: beq      #0x775128
00775104: ldr      r0, [r4, #0x3c]
00775108: ldrb     r2, [r0, #4]
0077510c: cmp      r2, #0
00775110: beq      #0x77512c
00775114: mov      r4, r3
00775118: ldr      r3, [r4, #0x40]
0077511c: strb     r1, [r4, #0x9d]
00775120: cmp      r3, #0
00775124: bne      #0x775104
00775128: pop      {r4, pc}
0077512c: ldr      r1, [r0]
00775130: sub      r1, r1, #1
00775134: cmp      r1, #0
00775138: str      r1, [r0]
0077513c: bne      #0x775144
00775140: bl       #0x752b38
00775144: mov      r3, #0
00775148: str      r3, [r4, #0x40]
0077514c: str      r3, [r4, #0x3c]
00775150: pop      {r4, pc}

# _ZN7gameswf9character7recycleEPS0_i
00754958: mov      r3, #0
0075495c: push     {r4, r5, r6, lr}
00754960: str      r3, [r0, #0x90]
00754964: mov      r3, #0
00754968: mov      r4, r0
0075496c: str      r2, [r0, #0x38]
00754970: strh     r3, [r0, #0x94]
00754974: strh     r3, [r0, #0x96]
00754978: add      r0, r0, #0x3c
0075497c: bl       #0x427ba8
00754980: ldr      r1, [pc, #0x80]
00754984: ldr      r5, [pc, #0x80]
00754988: mov      r0, r4
0075498c: add      r1, pc, r1
00754990: add      r1, r1, #0xc
00754994: bl       #0x7535b8
00754998: ldr      r3, [pc, #0x70]
0075499c: add      r5, pc, r5
007549a0: ldr      r2, [r4, #0x4c]
007549a4: ldr      r3, [r5, r3]
007549a8: cmp      r2, r3
007549ac: strne    r3, [r4, #0x4c]
007549b0: movne    r3, #1
007549b4: strbne   r3, [r4, #0x99]
007549b8: ldr      r3, [pc, #0x54]
007549bc: ldr      r2, [r4, #0x48]
007549c0: ldr      r3, [r5, r3]
007549c4: cmp      r2, r3
007549c8: strne    r3, [r4, #0x48]
007549cc: movne    r3, #1
007549d0: strbne   r3, [r4, #0x9a]
007549d4: ldr      r3, [pc, #0x3c]
007549d8: ldr      r2, [r4, #0x50]
007549dc: ldr      r3, [r5, r3]
007549e0: cmp      r2, r3
007549e4: strne    r3, [r4, #0x50]
007549e8: mov      r2, #0
007549ec: mov      r3, #1
007549f0: strb     r3, [r4, #0x99]
007549f4: strb     r2, [r4, #0x9c]
007549f8: strb     r3, [r4, #0x9b]
007549fc: strb     r3, [r4, #0x9d]
00754a00: strb     r3, [r4, #0x9a]
00754a04: pop      {r4, r5, r6, pc}
