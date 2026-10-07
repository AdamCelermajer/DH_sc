
# _Z15NativeUsePotionRKN7gameswf7fn_callE
0043d2c8: push     {r4, r5, r6, r7, lr}
0043d2cc: ldr      r3, [r0, #0xc]
0043d2d0: ldr      r2, [r0, #0x14]
0043d2d4: mov      r0, #0xc
0043d2d8: ldr      r3, [r3]
0043d2dc: sub      sp, sp, #0xc
0043d2e0: ldr      r4, [pc, #0xc0]
0043d2e4: mla      r0, r0, r2, r3
0043d2e8: bl       #0x797a54
0043d2ec: bl       #0x30ea24
0043d2f0: mov      r1, #0
0043d2f4: bl       #0x43c388
0043d2f8: subs     r5, r0, #0
0043d2fc: add      r4, pc, r4
0043d300: beq      #0x43d3a0
0043d304: bl       #0x3a5990
0043d308: cmp      r0, #0
0043d30c: beq      #0x43d3a0
0043d310: mov      r0, r5
0043d314: bl       #0x3bd2dc
0043d318: mov      r7, r0
0043d31c: mov      r0, r5
0043d320: bl       #0x3bd338
0043d324: mov      r6, r0
0043d328: mov      r0, r7
0043d32c: bl       #0x30e4cc
0043d330: bl       #0x30e964
0043d334: mov      r1, #0x3f800000
0043d338: bl       #0x30e70c
0043d33c: cmp      r0, #0
0043d340: bne      #0x43d360
0043d344: mov      r0, r6
0043d348: bl       #0x30e4cc
0043d34c: bl       #0x30e964
0043d350: mov      r1, #0x3f800000
0043d354: bl       #0x30e70c
0043d358: cmp      r0, #0
0043d35c: beq      #0x43d3a0
0043d360: ldr      r0, [r5, #0x378]
0043d364: bl       #0x4056b0
0043d368: ldr      r3, [pc, #0x3c]
0043d36c: ldr      r0, [pc, #0x3c]
0043d370: ldr      r3, [r4, r3]
0043d374: add      r0, pc, r0
0043d378: ldr      r4, [r3]
0043d37c: bl       #0x37ba84
0043d380: mov      ip, #0
0043d384: mov      r1, r0
0043d388: mov      r2, ip
0043d38c: mov      r0, r4
0043d390: mov      r3, ip
0043d394: str      ip, [sp]
0043d398: str      ip, [sp, #4]
0043d39c: bl       #0x36b80c
0043d3a0: add      sp, sp, #0xc
0043d3a4: pop      {r4, r5, r6, r7, pc}

# _Z17NativeAwayFromHudRKN7gameswf7fn_callE
0043ab98: ldr      r3, [pc, #0x40]
0043ab9c: ldr      r2, [pc, #0x40]
0043aba0: push     {r4, lr}
0043aba4: add      r3, pc, r3
0043aba8: ldr      r4, [r3, r2]
0043abac: mov      r0, r4
0043abb0: bl       #0x31f594
0043abb4: cmp      r0, #0
0043abb8: beq      #0x43abdc
0043abbc: mov      r3, #0
0043abc0: strb     r3, [r0, #0x198]
0043abc4: ldr      r0, [r4, #0x50]
0043abc8: bl       #0x3830bc
0043abcc: ldr      r3, [r4, #0x54]
0043abd0: ldr      r0, [r3, #0xf4]
0043abd4: pop      {r4, lr}
0043abd8: b        #0x43799c
0043abdc: pop      {r4, pc}
0043abe0: subseq   sb, r5, ip, ror #29
0043abe4: strdeq   r3, r4, [r0], -r4

# _Z23NativeRefreshHudManagerRKN7gameswf7fn_callE
0043a810: push     {r4, lr}
0043a814: bl       #0x41edd0
0043a818: mov      r3, #0
0043a81c: strb     r3, [r0, #4]
0043a820: bl       #0x41b11c
0043a824: pop      {r4, lr}
0043a828: b        #0x418bec
