
# _ZN7gameswf8listener5aliveEv
00760940: push     {r4, r5, r6, r7, r8, sb, sl, lr}
00760944: ldr      r3, [r0, #4]
00760948: mov      r8, r0
0076094c: cmp      r3, #0
00760950: ble      #0x7609e0
00760954: mov      r4, #0
00760958: mov      sl, r4
0076095c: b        #0x76097c
00760960: ldr      r3, [r2]
00760964: mov      lr, pc
00760968: ldr      pc, [r3, #0x44]
0076096c: ldr      r3, [r8, #4]
00760970: add      r4, r4, #1
00760974: cmp      r4, r3
00760978: bge      #0x7609e0
0076097c: ldr      r5, [r8]
00760980: lsl      r7, r4, #3
00760984: add      r6, r5, r7
00760988: ldr      r2, [r6, #4]
0076098c: cmp      r2, #0
00760990: beq      #0x760970
00760994: ldr      r3, [r5, r4, lsl #3]
00760998: mov      r0, r2
0076099c: ldrb     r1, [r3, #4]
007609a0: cmp      r1, #0
007609a4: bne      #0x760960
007609a8: ldr      r2, [r3]
007609ac: mov      r0, r3
007609b0: sub      r2, r2, #1
007609b4: cmp      r2, #0
007609b8: mov      r1, r2
007609bc: str      r2, [r3]
007609c0: bne      #0x7609c8
007609c4: bl       #0x752b38
007609c8: str      sl, [r5, r7]
007609cc: str      sl, [r6, #4]
007609d0: ldr      r3, [r8, #4]
007609d4: add      r4, r4, #1
007609d8: cmp      r4, r3
007609dc: blt      #0x76097c
007609e0: pop      {r4, r5, r6, r7, r8, sb, sl, pc}
