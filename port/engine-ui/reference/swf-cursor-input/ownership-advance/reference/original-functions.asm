
# _ZNK7gameswf11ref_counted7add_refEv
00759c64: ldr      r3, [r0, #4]
00759c68: add      r3, r3, #1
00759c6c: str      r3, [r0, #4]
00759c70: bx       lr

# _ZN7gameswf4root7advanceEfb
00775304: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
00775308: mov      r5, r1
0077530c: sub      sp, sp, #0xc
00775310: add      r6, r0, #0xb8
00775314: mov      r4, r0
00775318: mov      r7, r2
0077531c: bl       #0x773d38
00775320: mov      r1, r5
00775324: mov      r0, r6
00775328: bl       #0x760d58
0077532c: ldr      r1, [r4, #0x8c]
00775330: mov      r0, r5
00775334: bl       #0x30eba4
00775338: mov      r1, r5
0077533c: mov      r8, r0
00775340: str      r0, [r4, #0x8c]
00775344: ldr      r0, [r4, #0x94]
00775348: bl       #0x30e3ac
0077534c: ldr      r1, [r4, #0x90]
00775350: str      r0, [r4, #0x94]
00775354: mov      r0, r8
00775358: bl       #0x30e4b4
0077535c: cmp      r0, #0
00775360: bne      #0x775370
00775364: bl       #0x773d38
00775368: add      sp, sp, #0xc
0077536c: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
00775370: bl       #0x7b7898
00775374: ldrb     r3, [r4, #0x84]
00775378: cmp      r3, #0
0077537c: beq      #0x77548c
00775380: ldr      r1, [r4, #0x8c]
00775384: mov      sb, #1
00775388: mov      sl, #0xa
0077538c: mov      fp, sp
00775390: ldr      r0, [r4, #0x90]
00775394: bl       #0x30e9ac
00775398: cmp      r0, #0
0077539c: beq      #0x77545c
007753a0: ldrb     r3, [r4, #0x84]
007753a4: cmp      r3, #0
007753a8: bne      #0x7753f0
007753ac: ldr      r8, [r4, #0x10]
007753b0: cmp      r8, #0
007753b4: beq      #0x775484
007753b8: ldr      r3, [r8]
007753bc: mov      r0, r8
007753c0: mov      r1, #2
007753c4: mov      lr, pc
007753c8: ldr      pc, [r3, #8]
007753cc: cmp      r0, #0
007753d0: movne    r0, r8
007753d4: beq      #0x775484
007753d8: bl       #0x78069c
007753dc: ldr      r3, [r4, #0x10]
007753e0: mov      r0, r3
007753e4: ldr      r3, [r3]
007753e8: mov      lr, pc
007753ec: ldr      pc, [r3, #0x148]
007753f0: ldr      r0, [r4, #0x10]
007753f4: cmp      r7, #0
007753f8: moveq    r1, r5
007753fc: ldr      r3, [r0]
00775400: ldrne    r1, [r4, #0x90]
00775404: ldr      r3, [r3, #0x5c]
00775408: blx      r3
0077540c: ldrb     r2, [r4, #0x84]
00775410: cmp      r2, #0
00775414: bne      #0x775440
00775418: ldr      r0, [r4, #0x10]
0077541c: strb     sb, [r4, #0x84]
00775420: mov      r1, sp
00775424: ldr      r3, [r0]
00775428: ldr      r3, [r3, #0x2c]
0077542c: str      r2, [sp, #4]
00775430: strb     sl, [sp]
00775434: strb     r2, [sp, #1]
00775438: strh     r2, [sp, #2]
0077543c: blx      r3
00775440: ldr      r0, [r4, #0x8c]
00775444: ldr      r1, [r4, #0x90]
00775448: bl       #0x30e3ac
0077544c: cmp      r7, #0
00775450: mov      r1, r0
00775454: str      r0, [r4, #0x8c]
00775458: bne      #0x775390
0077545c: ldr      r0, [r4, #0x94]
00775460: mov      r1, #0
00775464: bl       #0x30e9ac
00775468: cmp      r0, #0
0077546c: bne      #0x7754cc
00775470: ldr      r0, [r4, #0x8c]
00775474: ldr      r1, [r4, #0x90]
00775478: bl       #0x30e7f0
0077547c: str      r0, [r4, #0x8c]
00775480: b        #0x775364
00775484: mov      r0, #0
00775488: b        #0x7753d8
0077548c: ldr      r1, [r4, #0xcc]
00775490: cmp      r1, #0
00775494: beq      #0x7754bc
00775498: ldr      r3, [r4, #0xc8]
0077549c: ldrb     r8, [r3, #4]
007754a0: cmp      r8, #0
007754a4: bne      #0x7754bc
007754a8: mov      r1, r8
007754ac: add      r0, r4, #0xc8
007754b0: bl       #0x41fe84
007754b4: str      r8, [r4, #0xcc]
007754b8: mov      r1, r8
007754bc: add      r1, r1, #0x68
007754c0: mov      r0, r4
007754c4: bl       #0x774660
007754c8: b        #0x775380
007754cc: ldr      r0, [r4, #0xcc]
007754d0: cmp      r0, #0
007754d4: beq      #0x7754e8
007754d8: ldr      r3, [r4, #0xc8]
007754dc: ldrb     r2, [r3, #4]
007754e0: cmp      r2, #0
007754e4: beq      #0x775548
007754e8: bl       #0x76c808
007754ec: mov      r0, r6
007754f0: bl       #0x760940
007754f4: ldr      r3, [r4, #0x10]
007754f8: mov      r0, r3
007754fc: ldr      r3, [r3]
00775500: mov      lr, pc
00775504: ldr      pc, [r3, #0x44]
00775508: ldr      r0, [r4, #0xcc]
0077550c: cmp      r0, #0
00775510: beq      #0x775538
00775514: ldr      r3, [r4, #0xc8]
00775518: ldrb     r5, [r3, #4]
0077551c: cmp      r5, #0
00775520: bne      #0x775538
00775524: add      r0, r4, #0xc8
00775528: mov      r1, r5
0077552c: bl       #0x41fe84
00775530: str      r5, [r4, #0xcc]
00775534: mov      r0, r5
00775538: bl       #0x76d2f8
0077553c: mov      r3, #0x40000000
00775540: str      r3, [r4, #0x94]
00775544: b        #0x775470
00775548: ldr      r1, [r3]
0077554c: sub      r1, r1, #1
00775550: cmp      r1, #0
00775554: str      r1, [r3]
00775558: bne      #0x775564
0077555c: mov      r0, r3
00775560: bl       #0x752b38
00775564: mov      r0, #0
00775568: str      r0, [r4, #0xc8]
0077556c: str      r0, [r4, #0xcc]
00775570: b        #0x7754e8

# _ZN7gameswf11ref_counted8drop_refEv
0075a240: ldr      r2, [r0, #4]
0075a244: sub      r2, r2, #1
0075a248: cmp      r2, #0
0075a24c: str      r2, [r0, #4]
0075a250: bxne     lr
0075a254: b        #0x75a214
