
# _Z33NativeChangeRolloverInputBehaviorRKN7gameswf7fn_callE
0043cdac: push     {r4, r5, r6, lr}
0043cdb0: ldr      r3, [r0, #0xc]
0043cdb4: mov      r4, r0
0043cdb8: ldr      r0, [r0, #0x14]
0043cdbc: ldr      r3, [r3]
0043cdc0: mov      r5, #0xc
0043cdc4: mla      r0, r5, r0, r3
0043cdc8: bl       #0x797a54
0043cdcc: bl       #0x30ea24
0043cdd0: ldr      r3, [r4, #0xc]
0043cdd4: mov      r6, r0
0043cdd8: ldr      r0, [r4, #0x14]
0043cddc: ldr      r3, [r3]
0043cde0: sub      r0, r0, #1
0043cde4: mla      r0, r5, r0, r3
0043cde8: bl       #0x797960
0043cdec: subs     r4, r0, #0
0043cdf0: beq      #0x43ce18
0043cdf4: bl       #0x42ca8c
0043cdf8: ldr      r3, [r0, #0xf4]
0043cdfc: cmp      r6, #3
0043ce00: movhi    r0, #0
0043ce04: addls    r6, r3, r6, lsl #2
0043ce08: ldrls    r0, [r6, #0x134]
0043ce0c: mov      r1, #0x84
0043ce10: pop      {r4, r5, r6, lr}
0043ce14: b        #0x7a7c98
0043ce18: bl       #0x42ca8c
0043ce1c: ldr      r3, [r0, #0xf4]
0043ce20: cmp      r6, #3
0043ce24: movhi    r0, r4
0043ce28: addls    r6, r3, r6, lsl #2
0043ce2c: ldrls    r0, [r6, #0x134]
0043ce30: mov      r1, #4
0043ce34: pop      {r4, r5, r6, lr}
0043ce38: b        #0x7a7c98

# _ZNK7gameswf8as_value9to_numberEv
00797a54: push     {r4, r6, r7, lr}
00797a58: ldrsb    r3, [r0, #1]
00797a5c: sub      sp, sp, #0x18
00797a60: cmp      r3, #6
00797a64: addls    pc, pc, r3, lsl #2
00797a68: b        #0x797ae0
00797a6c: b        #0x797b38
00797a70: b        #0x797ac8
00797a74: b        #0x797aec
00797a78: b        #0x797a88
00797a7c: b        #0x797a88
00797a80: b        #0x797b48
00797a84: b        #0x797b04
00797a88: ldr      r1, [r0, #4]
00797a8c: add      r0, sp, #0x10
00797a90: ldrsb    r3, [r1]
00797a94: cmn      r3, #1
00797a98: addne    r1, r1, #1
00797a9c: ldreq    r1, [r1, #0xc]
00797aa0: bl       #0x7a59c8
00797aa4: cmp      r0, #0
00797aa8: moveq    r7, #0x80000000
00797aac: asreq    r7, r7, #0xc
00797ab0: moveq    r6, r0
00797ab4: bne      #0x797afc
00797ab8: mov      r0, r6
00797abc: mov      r1, r7
00797ac0: add      sp, sp, #0x18
00797ac4: pop      {r4, r6, r7, pc}
00797ac8: ldrb     r3, [r0, #4]
00797acc: cmp      r3, #0
00797ad0: movne    r7, #0x3fc00000
00797ad4: movne    r6, #0
00797ad8: addne    r7, r7, #0x300000
00797adc: bne      #0x797ab8
00797ae0: mov      r6, #0
00797ae4: mov      r7, #0
00797ae8: b        #0x797ab8
00797aec: ldr      r2, [r0, #8]
00797af0: ldr      r3, [r0, #4]
00797af4: str      r2, [sp, #0x14]
00797af8: str      r3, [sp, #0x10]
00797afc: ldrd     r6, r7, [sp, #0x10]
00797b00: b        #0x797ab8
00797b04: add      r4, sp, #4
00797b08: mov      r3, #0
00797b0c: mov      r1, r4
00797b10: strb     r3, [sp, #5]
00797b14: strb     r3, [sp, #4]
00797b18: bl       #0x797644
00797b1c: mov      r0, r4
00797b20: bl       #0x797a54
00797b24: mov      r6, r0
00797b28: mov      r0, r4
00797b2c: mov      r7, r1
00797b30: bl       #0x797124
00797b34: b        #0x797ab8
00797b38: mov      r7, #0x80000000
00797b3c: mov      r6, #0
00797b40: asr      r7, r7, #0xc
00797b44: b        #0x797ab8
00797b48: ldr      r3, [r0, #4]
00797b4c: cmp      r3, #0
00797b50: moveq    r7, #0x80000000
00797b54: asreq    r7, r7, #0xc
00797b58: moveq    r6, r3
00797b5c: beq      #0x797ab8
00797b60: mov      r0, r3
00797b64: ldr      r3, [r3]
00797b68: mov      lr, pc
00797b6c: ldr      pc, [r3, #0x10]
00797b70: mov      r6, r0
00797b74: mov      r7, r1
00797b78: b        #0x797ab8

# _ZN8RenderFX16SetInputBehaviorEi
007a7c98: str      r1, [r0, #0xf8]
007a7c9c: bx       lr

# _ZNK7gameswf8as_value7to_boolEv
00797960: push     {r4, r5, lr}
00797964: ldrsb    r3, [r0, #1]
00797968: sub      sp, sp, #0x1c
0079796c: sub      r3, r3, #1
00797970: cmp      r3, #5
00797974: addls    pc, pc, r3, lsl #2
00797978: b        #0x7979c0
0079797c: b        #0x7979c8
00797980: b        #0x7979d0
00797984: b        #0x797994
00797988: b        #0x797994
0079798c: b        #0x797a30
00797990: b        #0x797a00
00797994: ldr      r3, [r0, #4]
00797998: ldrsb    r5, [r3]
0079799c: cmn      r5, #1
007979a0: ldreq    r5, [r3, #4]
007979a4: sub      r5, r5, #1
007979a8: cmp      r5, #0
007979ac: movle    r5, #0
007979b0: movgt    r5, #1
007979b4: mov      r0, r5
007979b8: add      sp, sp, #0x1c
007979bc: pop      {r4, r5, pc}
007979c0: mov      r5, #0
007979c4: b        #0x7979b4
007979c8: ldrb     r5, [r0, #4]
007979cc: b        #0x7979b4
007979d0: ldmib    r0, {r1, ip}
007979d4: mov      r2, #0
007979d8: str      ip, [sp, #0x14]
007979dc: str      r1, [sp, #0x10]
007979e0: mov      r3, #0
007979e4: ldrd     r0, r1, [sp, #0x10]
007979e8: bl       #0x30e820
007979ec: cmp      r0, #0
007979f0: mov      r5, #0
007979f4: moveq    r5, #1
007979f8: uxtb     r5, r5
007979fc: b        #0x7979b4
00797a00: add      r4, sp, #4
00797a04: mov      r3, #0
00797a08: mov      r1, r4
00797a0c: strb     r3, [sp, #5]
00797a10: strb     r3, [sp, #4]
00797a14: bl       #0x797644
00797a18: mov      r0, r4
00797a1c: bl       #0x797960
00797a20: mov      r5, r0
00797a24: mov      r0, r4
00797a28: bl       #0x797124
00797a2c: b        #0x7979b4
00797a30: ldr      r3, [r0, #4]
00797a34: cmp      r3, #0
00797a38: beq      #0x7979c0
00797a3c: mov      r0, r3
00797a40: ldr      r3, [r3]
00797a44: mov      lr, pc
00797a48: ldr      pc, [r3, #0x14]
00797a4c: mov      r5, r0
00797a50: b        #0x7979b4
