
# _ZN12ItemInstance9PowerInfo4swapERS0_
003fb570: push     {r4, r5, r6, r7, r8, sl, lr}
003fb574: ldr      r6, [pc, #0xbc]
003fb578: ldr      sl, [pc, #0xbc]
003fb57c: sub      sp, sp, #0x24
003fb580: add      r6, pc, r6
003fb584: ldr      r3, [r6, sl]
003fb588: mov      r4, r0
003fb58c: add      r7, sp, #4
003fb590: ldr      r3, [r3]
003fb594: ldr      r2, [r4, #0x18]
003fb598: mov      r5, r1
003fb59c: mov      r0, r7
003fb5a0: ldr      r1, [r4, #0x1c]
003fb5a4: add      r8, r5, #8
003fb5a8: str      r3, [sp, #0x1c]
003fb5ac: str      r7, [sp, #0x14]
003fb5b0: str      r7, [sp, #0x18]
003fb5b4: bl       #0x3116e8
003fb5b8: add      r0, r4, #8
003fb5bc: cmp      r0, r8
003fb5c0: beq      #0x3fb5d0
003fb5c4: ldr      r1, [r5, #0x1c]
003fb5c8: ldr      r2, [r5, #0x18]
003fb5cc: bl       #0x3109e0
003fb5d0: cmp      r8, r7
003fb5d4: beq      #0x3fb5e8
003fb5d8: mov      r0, r8
003fb5dc: ldr      r1, [sp, #0x18]
003fb5e0: ldr      r2, [sp, #0x14]
003fb5e4: bl       #0x3109e0
003fb5e8: ldr      r2, [r5]
003fb5ec: ldr      r3, [r4]
003fb5f0: mov      r0, r7
003fb5f4: eor      r3, r2, r3
003fb5f8: str      r3, [r4]
003fb5fc: ldr      r2, [r5]
003fb600: eor      r3, r3, r2
003fb604: str      r3, [r5]
003fb608: ldr      r2, [r4]
003fb60c: eor      r3, r2, r3
003fb610: str      r3, [r4]
003fb614: bl       #0x3139ac
003fb618: ldr      r3, [r6, sl]
003fb61c: ldr      r2, [sp, #0x1c]
003fb620: ldr      r3, [r3]
003fb624: cmp      r2, r3
003fb628: bne      #0x3fb634
003fb62c: add      sp, sp, #0x24
003fb630: pop      {r4, r5, r6, r7, r8, sl, pc}
003fb634: bl       #0x30e310
003fb638: subseq   sb, sb, r0, lsl r5
003fb63c: andeq    r4, r0, ip, lsr #1
