
# _ZN7gameswf14movie_def_impl16has_init_actionsEv
007636b4: ldrb     r0, [r0, #0xec]
007636b8: bx       lr

# _ZN7gameswf5arrayINS_9smart_ptrINS_9characterEEEE9push_backIPS2_EEvRKT_
0075581c: push     {r4, r5, r6, lr}
00755820: ldr      r3, [r0, #4]
00755824: ldr      r2, [r0, #8]
00755828: mov      r4, r0
0075582c: add      r5, r3, #1
00755830: cmp      r5, r2
00755834: mov      r6, r1
00755838: bgt      #0x75585c
0075583c: ldr      r0, [r6]
00755840: ldr      r2, [r4]
00755844: cmp      r0, #0
00755848: str      r0, [r2, r3, lsl #2]
0075584c: beq      #0x755854
00755850: bl       #0x759c64
00755854: str      r5, [r4, #4]
00755858: pop      {r4, r5, r6, pc}
0075585c: add      r1, r5, r5, asr #1
00755860: bl       #0x7557a0
00755864: ldr      r3, [r4, #4]
00755868: b        #0x75583c

# _ZN7gameswf5arrayINS_9smart_ptrINS_9characterEEEE6resizeEi
0075586c: push     {r4, r5, r6, r7, r8, lr}
00755870: ldr      r6, [r0, #4]
00755874: mov      r4, r0
00755878: mov      r5, r1
0075587c: cmp      r6, r1
00755880: ble      #0x7558b0
00755884: lsl      r8, r1, #2
00755888: mov      r7, r1
0075588c: ldr      r3, [r4]
00755890: add      r7, r7, #1
00755894: ldr      r0, [r3, r8]
00755898: add      r8, r8, #4
0075589c: cmp      r0, #0
007558a0: beq      #0x7558a8
007558a4: bl       #0x75a240
007558a8: cmp      r7, r6
007558ac: bne      #0x75588c
007558b0: cmp      r5, #0
007558b4: beq      #0x7558c4
007558b8: ldr      r3, [r4, #8]
007558bc: cmp      r5, r3
007558c0: bgt      #0x7558f8
007558c4: cmp      r6, r5
007558c8: bge      #0x7558f0
007558cc: mov      r3, r6
007558d0: mov      r1, #0
007558d4: lsl      r6, r6, #2
007558d8: ldr      r2, [r4]
007558dc: add      r3, r3, #1
007558e0: cmp      r3, r5
007558e4: str      r1, [r2, r6]
007558e8: add      r6, r6, #4
007558ec: bne      #0x7558d8
007558f0: str      r5, [r4, #4]
007558f4: pop      {r4, r5, r6, r7, r8, pc}
007558f8: mov      r0, r4
007558fc: add      r1, r5, r5, asr #1
00755900: bl       #0x7557a0
00755904: b        #0x7558c4

# _ZN7gameswf17sprite_definition16has_init_actionsEv
00783268: mov      r0, #0
0078326c: bx       lr
