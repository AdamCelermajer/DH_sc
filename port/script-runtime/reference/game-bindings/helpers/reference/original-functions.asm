
# _ZN10CharTimers9TMR_StartEjiiPv
003dbe24: push     {r4, r5, r6, lr}
003dbe28: mov      r6, r3
003dbe2c: mov      r4, r1
003dbe30: mov      r5, r2
003dbe34: bl       #0x3dbd70
003dbe38: subs     r3, r0, #0
003dbe3c: beq      #0x3dbe70
003dbe40: mov      r2, #0
003dbe44: mov      r1, #1
003dbe48: strb     r1, [r3, #0x14]
003dbe4c: str      r5, [r3, #8]
003dbe50: str      r4, [r3, #0xc]
003dbe54: str      r2, [r3, #0x10]
003dbe58: str      r6, [r3, #0x18]
003dbe5c: ldr      r1, [sp, #0x10]
003dbe60: ldr      r0, [r3, #4]
003dbe64: strb     r2, [r3, #0x15]
003dbe68: str      r1, [r3, #0x1c]
003dbe6c: pop      {r4, r5, r6, pc}
003dbe70: mvn      r0, #0
003dbe74: pop      {r4, r5, r6, pc}

# _ZNK3sfc6script3lua5Value11getUIntegerEv
0038d798: push     {r4, lr}
0038d79c: bl       #0x31bbf0
0038d7a0: bl       #0x8be2a0
0038d7a4: pop      {r4, pc}

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

# _ZNK3sfc6script3lua9ArgumentsixEj
0037baf8: push     {r4, r5, r6, lr}
0037bafc: ldr      r4, [r0, #4]
0037bb00: mov      r5, r1
0037bb04: ldm      r4, {r2, r3}
0037bb08: rsb      r3, r2, r3
0037bb0c: asr      r3, r3, #4
0037bb10: add      r1, r3, r3, lsl #3
0037bb14: add      r1, r1, r1, lsl #6
0037bb18: add      r1, r3, r1, lsl #3
0037bb1c: add      r1, r1, r1, lsl #15
0037bb20: add      r3, r3, r1, lsl #3
0037bb24: rsb      r3, r3, #0
0037bb28: cmp      r5, r3
0037bb2c: blo      #0x37bb40
0037bb30: ldr      r0, [pc, #0x14]
0037bb34: add      r0, pc, r0
0037bb38: bl       #0x708eb0
0037bb3c: ldr      r2, [r4]
0037bb40: mov      r0, #0x70
0037bb44: mla      r0, r0, r5, r2
0037bb48: pop      {r4, r5, r6, pc}
0037bb4c: subseq   r2, r4, r4, lsr sb

# _ZN3sfc6script3lua12ReturnValues11pushIntegerEi
0037cb24: ldr      r3, [pc, #0x58]
0037cb28: ldr      r2, [pc, #0x58]
0037cb2c: push     {r4, r5, r6, lr}
0037cb30: add      r3, pc, r3
0037cb34: ldr      r5, [r3, r2]
0037cb38: sub      sp, sp, #0x78
0037cb3c: add      r4, sp, #4
0037cb40: ldr      r3, [r5]
0037cb44: str      r3, [sp, #0x74]
0037cb48: ldr      r6, [r0, #0x24]
0037cb4c: mov      r0, r4
0037cb50: bl       #0x37ca9c
0037cb54: mov      r0, r6
0037cb58: mov      r1, r4
0037cb5c: bl       #0x3195c0
0037cb60: mov      r0, r4
0037cb64: bl       #0x3193e8
0037cb68: ldr      r2, [sp, #0x74]
0037cb6c: ldr      r3, [r5]
0037cb70: cmp      r2, r3
0037cb74: bne      #0x37cb80
0037cb78: add      sp, sp, #0x78
0037cb7c: pop      {r4, r5, r6, pc}
0037cb80: bl       #0x30e310
0037cb84: rsbeq    r7, r1, r0, ror #30
0037cb88: andeq    r4, r0, ip, lsr #1

# _ZN10CharTimers8TMR_StopEj
003db2d8: ldr      r3, [r0, #8]
003db2dc: ldr      r2, [r0, #0xc]
003db2e0: rsb      r2, r3, r2
003db2e4: cmp      r1, r2, asr #5
003db2e8: addlo    r3, r3, r1, lsl #5
003db2ec: movlo    r2, #0
003db2f0: strblo   r2, [r3, #0x14]
003db2f4: bx       lr

# _ZN9LuaScript12BindFunctionEv
0037b5a0: push     {r4, r5, r6, lr}
0037b5a4: add      r4, r0, #4
0037b5a8: mov      r5, r0
0037b5ac: mov      r0, r4
0037b5b0: bl       #0x31b010
0037b5b4: mov      r0, r4
0037b5b8: bl       #0x31b000
0037b5bc: mov      r0, r4
0037b5c0: bl       #0x31aff8
0037b5c4: mov      r0, r4
0037b5c8: ldr      r4, [pc, #0x3a8]
0037b5cc: bl       #0x31b008
0037b5d0: ldr      r3, [pc, #0x3a4]
0037b5d4: ldr      r1, [pc, #0x3a4]
0037b5d8: add      r4, pc, r4
0037b5dc: add      r6, r5, #0x10
0037b5e0: ldr      r2, [r4, r3]
0037b5e4: mov      r0, r6
0037b5e8: mov      r3, r5
0037b5ec: add      r1, pc, r1
0037b5f0: bl       #0x31a4d4
0037b5f4: ldr      r3, [pc, #0x388]
0037b5f8: ldr      r1, [pc, #0x388]
0037b5fc: mov      r0, r6
0037b600: ldr      r2, [r4, r3]
0037b604: add      r1, pc, r1
0037b608: mov      r3, r5
0037b60c: bl       #0x31a4d4
0037b610: ldr      r3, [pc, #0x374]
0037b614: ldr      r1, [pc, #0x374]
0037b618: mov      r0, r6
0037b61c: ldr      r2, [r4, r3]
0037b620: add      r1, pc, r1
0037b624: mov      r3, r5
0037b628: bl       #0x31a4d4
0037b62c: ldr      r3, [pc, #0x360]
0037b630: ldr      r1, [pc, #0x360]
0037b634: mov      r0, r6
0037b638: ldr      r2, [r4, r3]
0037b63c: add      r1, pc, r1
0037b640: mov      r3, r5
0037b644: bl       #0x31a4d4
0037b648: ldr      r3, [pc, #0x34c]
0037b64c: ldr      r1, [pc, #0x34c]
0037b650: mov      r0, r6
0037b654: ldr      r2, [r4, r3]
0037b658: add      r1, pc, r1
0037b65c: mov      r3, r5
0037b660: bl       #0x31a4d4
0037b664: ldr      r3, [pc, #0x338]
0037b668: ldr      r1, [pc, #0x338]
0037b66c: mov      r0, r6
0037b670: ldr      r2, [r4, r3]
0037b674: add      r1, pc, r1
0037b678: mov      r3, r5
0037b67c: bl       #0x31a4d4
0037b680: ldr      r3, [pc, #0x324]
0037b684: ldr      r1, [pc, #0x324]
0037b688: mov      r0, r6
0037b68c: ldr      r2, [r4, r3]
0037b690: add      r1, pc, r1
0037b694: mov      r3, r5
0037b698: bl       #0x31a4d4
0037b69c: ldr      r3, [pc, #0x310]
0037b6a0: ldr      r1, [pc, #0x310]
0037b6a4: mov      r0, r6
0037b6a8: ldr      r2, [r4, r3]
0037b6ac: add      r1, pc, r1
0037b6b0: mov      r3, r5
0037b6b4: bl       #0x31a4d4
0037b6b8: ldr      r3, [pc, #0x2fc]
0037b6bc: ldr      r1, [pc, #0x2fc]
0037b6c0: mov      r0, r6
0037b6c4: ldr      r2, [r4, r3]
0037b6c8: add      r1, pc, r1
0037b6cc: mov      r3, r5
0037b6d0: bl       #0x31a4d4
0037b6d4: ldr      r3, [pc, #0x2e8]
0037b6d8: ldr      r1, [pc, #0x2e8]
0037b6dc: mov      r0, r6
0037b6e0: ldr      r2, [r4, r3]
0037b6e4: add      r1, pc, r1
0037b6e8: mov      r3, r5
0037b6ec: bl       #0x31a4d4
0037b6f0: ldr      r3, [pc, #0x2d4]
0037b6f4: ldr      r1, [pc, #0x2d4]
0037b6f8: mov      r0, r6
0037b6fc: ldr      r2, [r4, r3]
0037b700: add      r1, pc, r1
0037b704: mov      r3, r5
0037b708: bl       #0x31a4d4
0037b70c: ldr      r3, [pc, #0x2c0]
0037b710: ldr      r1, [pc, #0x2c0]
0037b714: mov      r0, r6
0037b718: ldr      r2, [r4, r3]
0037b71c: add      r1, pc, r1
0037b720: mov      r3, r5
0037b724: bl       #0x31a4d4
0037b728: ldr      r3, [pc, #0x2ac]
0037b72c: ldr      r1, [pc, #0x2ac]
0037b730: mov      r0, r6
0037b734: ldr      r2, [r4, r3]
0037b738: add      r1, pc, r1
0037b73c: mov      r3, r5
0037b740: bl       #0x31a4d4
0037b744: ldr      r3, [pc, #0x298]
0037b748: ldr      r1, [pc, #0x298]
0037b74c: mov      r0, r6
0037b750: ldr      r2, [r4, r3]
0037b754: add      r1, pc, r1
0037b758: mov      r3, r5
0037b75c: bl       #0x31a4d4
0037b760: ldr      r3, [pc, #0x284]
0037b764: ldr      r1, [pc, #0x284]
0037b768: mov      r0, r6
0037b76c: ldr      r2, [r4, r3]
0037b770: add      r1, pc, r1
0037b774: mov      r3, r5
0037b778: bl       #0x31a4d4
0037b77c: ldr      r3, [pc, #0x270]
0037b780: ldr      r1, [pc, #0x270]
0037b784: mov      r0, r6
0037b788: ldr      r2, [r4, r3]
0037b78c: add      r1, pc, r1
0037b790: mov      r3, r5
0037b794: bl       #0x31a4d4
0037b798: ldr      r3, [pc, #0x25c]
0037b79c: ldr      r1, [pc, #0x25c]
0037b7a0: mov      r0, r6
0037b7a4: ldr      r2, [r4, r3]
0037b7a8: add      r1, pc, r1
0037b7ac: mov      r3, r5
0037b7b0: bl       #0x31a4d4
0037b7b4: ldr      r3, [pc, #0x248]
0037b7b8: ldr      r1, [pc, #0x248]
0037b7bc: mov      r0, r6
0037b7c0: ldr      r2, [r4, r3]
0037b7c4: add      r1, pc, r1
0037b7c8: mov      r3, r5
0037b7cc: bl       #0x31a4d4
0037b7d0: ldr      r3, [pc, #0x234]
0037b7d4: ldr      r1, [pc, #0x234]
0037b7d8: mov      r0, r6
0037b7dc: ldr      r2, [r4, r3]
0037b7e0: add      r1, pc, r1
0037b7e4: mov      r3, r5
0037b7e8: bl       #0x31a4d4
0037b7ec: ldr      r3, [pc, #0x220]
0037b7f0: ldr      r1, [pc, #0x220]
0037b7f4: mov      r0, r6
0037b7f8: ldr      r2, [r4, r3]
0037b7fc: add      r1, pc, r1
0037b800: mov      r3, r5
0037b804: bl       #0x31a4d4
0037b808: ldr      r3, [pc, #0x20c]
0037b80c: ldr      r1, [pc, #0x20c]
0037b810: mov      r0, r6
0037b814: ldr      r2, [r4, r3]
0037b818: add      r1, pc, r1
0037b81c: mov      r3, r5
0037b820: bl       #0x31a4d4
0037b824: ldr      r3, [pc, #0x1f8]
0037b828: ldr      r1, [pc, #0x1f8]
0037b82c: mov      r0, r6
0037b830: ldr      r2, [r4, r3]
0037b834: add      r1, pc, r1
0037b838: mov      r3, r5
0037b83c: bl       #0x31a4d4
0037b840: ldr      r3, [pc, #0x1e4]
0037b844: ldr      r1, [pc, #0x1e4]
0037b848: mov      r0, r6
0037b84c: ldr      r2, [r4, r3]
0037b850: add      r1, pc, r1
0037b854: mov      r3, r5
0037b858: bl       #0x31a4d4
0037b85c: ldr      r3, [pc, #0x1d0]
0037b860: ldr      r1, [pc, #0x1d0]
0037b864: mov      r0, r6
0037b868: ldr      r2, [r4, r3]
0037b86c: add      r1, pc, r1
0037b870: mov      r3, r5
0037b874: bl       #0x31a4d4
0037b878: ldr      r3, [pc, #0x1bc]
0037b87c: ldr      r1, [pc, #0x1bc]
0037b880: mov      r0, r6
0037b884: ldr      r2, [r4, r3]
0037b888: add      r1, pc, r1
0037b88c: mov      r3, r5
0037b890: bl       #0x31a4d4
0037b894: ldr      r3, [pc, #0x1a8]
0037b898: ldr      r1, [pc, #0x1a8]
0037b89c: mov      r0, r6
0037b8a0: ldr      r2, [r4, r3]
0037b8a4: add      r1, pc, r1
0037b8a8: mov      r3, r5
0037b8ac: bl       #0x31a4d4
0037b8b0: ldr      r3, [pc, #0x194]
0037b8b4: ldr      r1, [pc, #0x194]
0037b8b8: mov      r0, r6
0037b8bc: ldr      r2, [r4, r3]
0037b8c0: add      r1, pc, r1
0037b8c4: mov      r3, r5
0037b8c8: bl       #0x31a4d4
0037b8cc: ldr      r3, [pc, #0x180]
0037b8d0: ldr      r1, [pc, #0x180]
0037b8d4: mov      r0, r6
0037b8d8: ldr      r2, [r4, r3]
0037b8dc: add      r1, pc, r1
0037b8e0: mov      r3, r5
0037b8e4: bl       #0x31a4d4
0037b8e8: ldr      r3, [pc, #0x16c]
0037b8ec: ldr      r1, [pc, #0x16c]
0037b8f0: mov      r0, r6
0037b8f4: ldr      r2, [r4, r3]
0037b8f8: add      r1, pc, r1
0037b8fc: mov      r3, r5
0037b900: bl       #0x31a4d4
0037b904: ldr      r3, [pc, #0x158]
0037b908: ldr      r1, [pc, #0x158]
0037b90c: mov      r0, r6
0037b910: ldr      r2, [r4, r3]
0037b914: add      r1, pc, r1
0037b918: mov      r3, r5
0037b91c: bl       #0x31a4d4
0037b920: ldr      r3, [pc, #0x144]
0037b924: ldr      r1, [pc, #0x144]
0037b928: mov      r0, r6
0037b92c: ldr      r2, [r4, r3]
0037b930: add      r1, pc, r1
0037b934: mov      r3, r5
0037b938: bl       #0x31a4d4
0037b93c: ldr      r3, [pc, #0x130]
0037b940: ldr      r1, [pc, #0x130]
0037b944: mov      r0, r6
0037b948: ldr      r2, [r4, r3]
0037b94c: add      r1, pc, r1
0037b950: mov      r3, r5
0037b954: bl       #0x31a4d4
0037b958: ldr      r3, [pc, #0x11c]
0037b95c: ldr      r1, [pc, #0x11c]
0037b960: mov      r0, r6
0037b964: ldr      r2, [r4, r3]
0037b968: add      r1, pc, r1
0037b96c: mov      r3, r5
0037b970: pop      {r4, r5, r6, lr}
0037b974: b        #0x31a4d4
0037b978: strhteq  sb, [r1], #-0x48
0037b97c: andeq    r3, r0, r0, asr ip
0037b980: subseq   r6, r4, ip, asr r4
0037b984: andeq    r3, r0, r0, asr #20
0037b988: subseq   r6, r4, ip, asr #8
0037b98c: andeq    r1, r0, r4, lsr r7
0037b990: subseq   r6, r4, r8, lsr r4
0037b994: andeq    r0, r0, r0, ror #29
0037b998: subseq   r6, r4, r4, lsr #8
0037b99c: muleq    r0, ip, fp
0037b9a0: subseq   r6, r4, r0, lsl r4
0037b9a4: muleq    r0, r8, ip
0037b9a8: subseq   r6, r4, r4, lsl #8
0037b9ac: muleq    r0, ip, r5
0037b9b0: ldrsheq  r6, [r4], #-0x38
0037b9b4: andeq    r3, r0, r8, ror #15
0037b9b8: subseq   r6, r4, ip, ror #7
0037b9bc: andeq    r4, r0, r4, asr #23
0037b9c0: ldrsbeq  r6, [r4], #-0x38
0037b9c4: muleq    r0, r0, r8
0037b9c8: subseq   r6, r4, ip, asr #7
0037b9cc: andeq    r1, r0, r4, ror #7
0037b9d0: subseq   r6, r4, r0, asr #7
0037b9d4: strdeq   r4, r5, [r0], -r4
0037b9d8: ldrheq   r6, [r4], #-0x34
0037b9dc: andeq    r4, r0, r0, lsl #6
0037b9e0: subseq   r6, r4, r0, lsr #7
0037b9e4: andeq    r3, r0, ip, asr #10
0037b9e8: subseq   r6, r4, ip, lsl #7
0037b9ec: andeq    r0, r0, r8, lsr r7
0037b9f0: subseq   r6, r4, r8, ror r3
0037b9f4: andeq    r4, r0, r4, lsr r6
0037b9f8: subseq   r6, r4, r4, ror #6
0037b9fc: muleq    r0, r0, r0
0037ba00: subseq   r6, r4, r0, asr r3
0037ba04: andeq    r1, r0, ip, lsl #13
0037ba08: subseq   r6, r4, ip, lsr r3
0037ba0c: andeq    r1, r0, ip, lsl #9
0037ba10: subseq   r6, r4, r0, lsr r3
0037ba14: andeq    r2, r0, ip, lsr #29
0037ba18: subseq   r6, r4, r4, lsr #6
0037ba1c: andeq    r2, r0, ip, lsl #5
0037ba20: subseq   r6, r4, r8, lsl r3
0037ba24: ldrdeq   r4, r5, [r0], -ip
0037ba28: subseq   r6, r4, ip, lsl #6
0037ba2c: muleq    r0, r8, pc
0037ba30: subseq   r6, r4, r0, lsl #6
0037ba34: andeq    r1, r0, r0, asr #21
0037ba38: ldrsheq  r6, [r4], #-0x24
0037ba3c: andeq    r3, r0, r8, lsr #11
0037ba40: ldrsheq  r6, [r4], #-0x20
0037ba44: andeq    r4, r0, r0, asr r0
0037ba48: subseq   r6, r4, ip, ror #5
0037ba4c: strdeq   r3, r4, [r0], -r4
0037ba50: subseq   r6, r4, r8, ror #5
0037ba54: andeq    r2, r0, ip, lsl #8
0037ba58: subseq   r6, r4, r4, ror #5
0037ba5c: andeq    r1, r0, ip, ror #15
0037ba60: ldrsbeq  r6, [r4], #-0x28
0037ba64: muleq    r0, r8, sb
0037ba68: subseq   r6, r4, ip, asr #5
0037ba6c: andeq    r3, r0, r8, ror pc
0037ba70: subseq   r6, r4, r0, asr #5
0037ba74: andeq    r2, r0, ip, lsr #13
0037ba78: ldrheq   r6, [r4], #-0x24
0037ba7c: andeq    r2, r0, r8, lsl #30
0037ba80: subseq   r6, r4, r8, lsr #5

# _ZN10CharTimers12SetCharacterEP9Character
003db480: push     {r4, r5, lr}
003db484: ldr      r3, [pc, #0x70]
003db488: subs     r4, r1, #0
003db48c: sub      sp, sp, #0xc
003db490: mov      r5, r0
003db494: add      r3, pc, r3
003db498: beq      #0x3db4a8
003db49c: str      r4, [r5, #4]
003db4a0: add      sp, sp, #0xc
003db4a4: pop      {r4, r5, pc}
003db4a8: ldr      r2, [pc, #0x50]
003db4ac: ldr      r2, [r3, r2]
003db4b0: ldr      r2, [r2]
003db4b4: cmp      r2, #2
003db4b8: streq    r4, [r4]
003db4bc: beq      #0x3db49c
003db4c0: cmp      r2, #1
003db4c4: bne      #0x3db49c
003db4c8: ldr      r0, [pc, #0x34]
003db4cc: ldr      r1, [pc, #0x34]
003db4d0: ldr      r2, [pc, #0x34]
003db4d4: ldr      r0, [r3, r0]
003db4d8: ldr      r3, [pc, #0x30]
003db4dc: mov      ip, #0x3d
003db4e0: add      r1, pc, r1
003db4e4: add      r2, pc, r2
003db4e8: add      r3, pc, r3
003db4ec: add      r0, r0, #0xa8
003db4f0: str      ip, [sp]
003db4f4: bl       #0x30e004
003db4f8: b        #0x3db49c
003db4fc: ldrsheq  sb, [fp], #-0x5c
003db500: andeq    r3, r0, r0, asr #19
003db504: andeq    r1, r0, r0, asr #19
003db508: strdeq   r2, r3, [lr], #-0xe8
003db50c: subseq   r6, r1, r4, lsr #23
003db510: subeq    sl, lr, r8, lsl #8

# _ZN9LuaScript6_TraceERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv
0037ee80: bx       lr
