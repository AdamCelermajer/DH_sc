
# _ZNK9Character18GetCharSkillListIdEv
003bc5c0: movw     r3, #0x1068
003bc5c4: ldr      r0, [r0, r3]
003bc5c8: ldr      r3, [pc, #0x24]
003bc5cc: cmp      r0, #0
003bc5d0: add      r3, pc, r3
003bc5d4: blt      #0x3bc5ec
003bc5d8: ldr      r2, [pc, #0x18]
003bc5dc: ldr      r3, [r3, r2]
003bc5e0: ldr      r3, [r3]
003bc5e4: cmp      r0, r3
003bc5e8: bxlt     lr
003bc5ec: mov      r0, #3
003bc5f0: bx       lr
003bc5f4: subseq   r8, sp, r0, asr #9
003bc5f8: andeq    r2, r0, r8, ror sp

# _ZNK14PlayerSavegame16SG_GetSkillLevelEj
004668dc: push     {r4, r5, r6, lr}
004668e0: ldr      r3, [r0, #0x84]
004668e4: ldr      r4, [pc, #0xf8]
004668e8: sub      sp, sp, #8
004668ec: cmp      r3, r1
004668f0: mov      r5, r0
004668f4: mov      r6, r1
004668f8: add      r4, pc, r4
004668fc: bhi      #0x466924
00466900: ldr      r3, [pc, #0xe0]
00466904: ldr      r3, [r4, r3]
00466908: ldr      r3, [r3]
0046690c: cmp      r3, #2
00466910: moveq    r3, #0
00466914: streq    r3, [r3]
00466918: beq      #0x466924
0046691c: cmp      r3, #1
00466920: beq      #0x46696c
00466924: ldr      r3, [r5, #0x80]
00466928: cmp      r3, #0
0046692c: beq      #0x466940
00466930: add      r6, r3, r6, lsl #3
00466934: ldrh     r0, [r6, #4]
00466938: add      sp, sp, #8
0046693c: pop      {r4, r5, r6, pc}
00466940: ldr      r2, [pc, #0xa0]
00466944: ldr      r2, [r4, r2]
00466948: ldr      r2, [r2]
0046694c: cmp      r2, #2
00466950: streq    r3, [r3]
00466954: mvneq    r0, #0
00466958: beq      #0x466938
0046695c: cmp      r2, #1
00466960: beq      #0x4669a0
00466964: mvn      r0, #0
00466968: b        #0x466938
0046696c: ldr      r0, [pc, #0x78]
00466970: ldr      r1, [pc, #0x78]
00466974: ldr      r2, [pc, #0x78]
00466978: ldr      r0, [r4, r0]
0046697c: ldr      r3, [pc, #0x74]
00466980: mov      ip, #0xa5
00466984: add      r1, pc, r1
00466988: add      r2, pc, r2
0046698c: add      r3, pc, r3
00466990: add      r0, r0, #0xa8
00466994: str      ip, [sp]
00466998: bl       #0x30e004
0046699c: b        #0x466924
004669a0: ldr      r0, [pc, #0x44]
004669a4: ldr      r1, [pc, #0x50]
004669a8: ldr      r2, [pc, #0x50]
004669ac: ldr      r0, [r4, r0]
004669b0: ldr      r3, [pc, #0x4c]
004669b4: mov      ip, #0xa6
004669b8: add      r1, pc, r1
004669bc: add      r3, pc, r3
004669c0: add      r0, r0, #0xa8
004669c4: add      r2, pc, r2
004669c8: str      ip, [sp]
004669cc: bl       #0x30e004
004669d0: ldr      r3, [r5, #0x80]
004669d4: cmp      r3, #0
004669d8: bne      #0x466930
004669dc: mvn      r0, #0
004669e0: b        #0x466938

# _ZNK3sfc6script3lua5Value9getNumberEv
0031bbf0: push     {r4, r5, r6, lr}
0031bbf4: ldr      r3, [r0, #4]
0031bbf8: mov      r5, r0
0031bbfc: cmp      r3, #0
0031bc00: beq      #0x31bc2c
0031bc04: cmp      r3, #1
0031bc08: beq      #0x31bc38
0031bc0c: cmp      r3, #3
0031bc10: beq      #0x31bc38
0031bc14: cmp      r3, #2
0031bc18: beq      #0x31bc44
0031bc1c: cmp      r3, #7
0031bc20: beq      #0x31bc44
0031bc24: cmp      r3, #4
0031bc28: beq      #0x31bc54
0031bc2c: mov      r5, #0
0031bc30: mov      r0, r5
0031bc34: pop      {r4, r5, r6, pc}
0031bc38: ldr      r5, [r5, #8]
0031bc3c: mov      r0, r5
0031bc40: pop      {r4, r5, r6, pc}
0031bc44: ldr      r0, [r5, #0x6c]
0031bc48: bl       #0x30e2e0
0031bc4c: mov      r5, r0
0031bc50: b        #0x31bc30
0031bc54: bl       #0x84c7e0
0031bc58: ldr      r1, [r5, #0x20]
0031bc5c: mov      r4, r0
0031bc60: bl       #0x84c04c
0031bc64: mov      r0, r4
0031bc68: mvn      r1, #0
0031bc6c: bl       #0x84c450
0031bc70: mov      r5, r0
0031bc74: mov      r0, r4
0031bc78: bl       #0x85797c
0031bc7c: b        #0x31bc30
