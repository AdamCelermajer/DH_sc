
# _ZNK14CharProperties18PROPS_GetFromSheetEiPN7Structs19CharacterPropertiesE
003b55d4: str      lr, [sp, #-4]!
003b55d8: ldr      r3, [pc, #0x80]
003b55dc: subs     ip, r2, #0
003b55e0: sub      sp, sp, #0xc
003b55e4: mov      r2, r1
003b55e8: add      r3, pc, r3
003b55ec: beq      #0x3b5600
003b55f0: mov      r1, ip
003b55f4: add      sp, sp, #0xc
003b55f8: pop      {lr}
003b55fc: b        #0x3dedb4
003b5600: ldr      r2, [pc, #0x5c]
003b5604: ldr      r2, [r3, r2]
003b5608: ldr      r2, [r2]
003b560c: cmp      r2, #2
003b5610: streq    ip, [ip]
003b5614: beq      #0x3b5620
003b5618: cmp      r2, #1
003b561c: beq      #0x3b562c
003b5620: mvn      r0, #0
003b5624: add      sp, sp, #0xc
003b5628: ldm      sp!, {pc}
003b562c: ldr      r0, [pc, #0x34]
003b5630: ldr      r1, [pc, #0x34]
003b5634: ldr      r2, [pc, #0x34]
003b5638: ldr      r0, [r3, r0]
003b563c: ldr      r3, [pc, #0x30]
003b5640: movw     ip, #0x137
003b5644: add      r1, pc, r1
003b5648: add      r2, pc, r2
003b564c: add      r3, pc, r3
003b5650: add      r0, r0, #0xa8
003b5654: str      ip, [sp]
003b5658: bl       #0x30e004
003b565c: b        #0x3b5620
003b5660: subseq   pc, sp, r8, lsr #9
003b5664: andeq    r3, r0, r0, asr #19
003b5668: andeq    r1, r0, r0, asr #19

# _ZNK14CharProperties12_GetPropertyERKN7Structs19CharacterPropertiesEi
003dedb4: str      lr, [sp, #-4]!
003dedb8: ldr      r3, [pc, #0xf0]
003dedbc: cmp      r2, #0
003dedc0: sub      sp, sp, #0xc
003dedc4: add      r3, pc, r3
003dedc8: blt      #0x3dedfc
003dedcc: cmp      r2, #0xdf
003dedd0: ble      #0x3dee20
003dedd4: ldr      r2, [pc, #0xd8]
003dedd8: ldr      r2, [r3, r2]
003deddc: ldr      r2, [r2]
003dede0: cmp      r2, #2
003dede4: beq      #0x3dee10
003dede8: cmp      r2, #1
003dedec: beq      #0x3dee78
003dedf0: mvn      r0, #0
003dedf4: add      sp, sp, #0xc
003dedf8: ldm      sp!, {pc}
003dedfc: ldr      r2, [pc, #0xb0]
003dee00: ldr      r2, [r3, r2]
003dee04: ldr      r2, [r2]
003dee08: cmp      r2, #2
003dee0c: bne      #0x3dee38
003dee10: mov      r3, #0
003dee14: str      r3, [r3]
003dee18: mvn      r0, #0
003dee1c: b        #0x3dedf4
003dee20: ldr      r0, [pc, #0x90]
003dee24: ldr      r3, [r3, r0]
003dee28: ldr      r3, [r3, r2, lsl #2]
003dee2c: add      r1, r1, r3
003dee30: ldr      r0, [r1, #4]
003dee34: b        #0x3dedf4
003dee38: cmp      r2, #1
003dee3c: bne      #0x3dedf0
003dee40: ldr      r0, [pc, #0x74]
003dee44: ldr      r1, [pc, #0x74]
003dee48: ldr      r2, [pc, #0x74]
003dee4c: ldr      r0, [r3, r0]
003dee50: ldr      r3, [pc, #0x70]
003dee54: movw     ip, #0x103
003dee58: add      r1, pc, r1
003dee5c: add      r0, r0, #0xa8
003dee60: add      r2, pc, r2
003dee64: add      r3, pc, r3
003dee68: str      ip, [sp]
003dee6c: bl       #0x30e004
003dee70: mvn      r0, #0
003dee74: b        #0x3dedf4
003dee78: ldr      r0, [pc, #0x3c]
003dee7c: ldr      r1, [pc, #0x48]
003dee80: ldr      r2, [pc, #0x48]
003dee84: ldr      r0, [r3, r0]
003dee88: ldr      r3, [pc, #0x44]
003dee8c: mov      ip, #0x104
003dee90: add      r1, pc, r1
003dee94: add      r0, r0, #0xa8
003dee98: add      r2, pc, r2
003dee9c: add      r3, pc, r3
003deea0: str      ip, [sp]
003deea4: bl       #0x30e004
003deea8: mvn      r0, #0
003deeac: b        #0x3dedf4
003deeb0: subseq   r5, fp, ip, asr #25
003deeb4: andeq    r3, r0, r0, asr #19
003deeb8: andeq    r2, r0, r8, lsr #5
003deebc: andeq    r1, r0, r0, asr #19
003deec0: subeq    pc, sp, r0, lsl #11
003deec4: strheq   r6, [lr], #-0xe0
003deec8: subeq    r6, lr, ip, asr #28
003deecc: subeq    pc, sp, r8, asr #10
003deed0: subeq    r6, lr, r8, lsl #29
003deed4: subeq    r6, lr, r4, lsl lr

# _ZN9Character8_GetPropERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv
003b9d8c: push     {r4, r5, r6, r7, r8, sb, sl, lr}
003b9d90: ldr      r3, [r0, #4]
003b9d94: mov      r7, r1
003b9d98: mov      r6, r2
003b9d9c: ldr      r1, [r3, #4]
003b9da0: ldr      r3, [r3]
003b9da4: ldr      r4, [pc, #0x190]
003b9da8: mov      r5, r0
003b9dac: rsb      r1, r3, r1
003b9db0: asr      r1, r1, #4
003b9db4: add      r4, pc, r4
003b9db8: add      r2, r1, r1, lsl #3
003b9dbc: add      r2, r2, r2, lsl #6
003b9dc0: add      r2, r1, r2, lsl #3
003b9dc4: add      r2, r2, r2, lsl #15
003b9dc8: add      r1, r1, r2, lsl #3
003b9dcc: cmp      r1, #0
003b9dd0: bne      #0x3b9dd8
003b9dd4: pop      {r4, r5, r6, r7, r8, sb, sl, pc}
003b9dd8: ldr      r3, [r3, #4]
003b9ddc: cmp      r3, #3
003b9de0: bne      #0x3b9dd4
003b9de4: mov      r1, #0
003b9de8: bl       #0x37baf8
003b9dec: bl       #0x38d798
003b9df0: mov      r1, #0
003b9df4: mov      r0, r5
003b9df8: bl       #0x37baf8
003b9dfc: bl       #0x38d798
003b9e00: cmp      r0, #0xdf
003b9e04: bhi      #0x3b9dd4
003b9e08: ldr      r3, [r5, #4]
003b9e0c: ldr      r2, [r3, #4]
003b9e10: ldr      r3, [r3]
003b9e14: rsb      r2, r3, r2
003b9e18: asr      r2, r2, #4
003b9e1c: add      r1, r2, r2, lsl #3
003b9e20: add      r1, r1, r1, lsl #6
003b9e24: add      r1, r2, r1, lsl #3
003b9e28: add      r1, r1, r1, lsl #15
003b9e2c: add      r2, r2, r1, lsl #3
003b9e30: rsb      r2, r2, #0
003b9e34: cmp      r2, #1
003b9e38: bls      #0x3b9e60
003b9e3c: ldr      r3, [r3, #0x74]
003b9e40: cmp      r3, #2
003b9e44: beq      #0x3b9e94
003b9e48: mov      r0, r5
003b9e4c: mov      r1, #1
003b9e50: bl       #0x37baf8
003b9e54: ldr      r8, [r0, #4]
003b9e58: cmp      r8, #1
003b9e5c: beq      #0x3b9ef0
003b9e60: mov      r1, #0
003b9e64: mov      r0, r5
003b9e68: bl       #0x37baf8
003b9e6c: bl       #0x38d798
003b9e70: add      r1, r6, #0xff0
003b9e74: mov      r2, r0
003b9e78: add      r1, r1, #4
003b9e7c: add      r0, r6, #0x560
003b9e80: bl       #0x3dedb4
003b9e84: mov      r1, r0
003b9e88: mov      r0, r7
003b9e8c: pop      {r4, r5, r6, r7, r8, sb, sl, lr}
003b9e90: b        #0x37cb24
003b9e94: mov      r1, #1
003b9e98: mov      r0, r5
003b9e9c: bl       #0x37baf8
003b9ea0: bl       #0x31b580
003b9ea4: cmp      r0, #0
003b9ea8: beq      #0x3b9dd4
003b9eac: mov      r1, #0
003b9eb0: mov      r0, r5
003b9eb4: bl       #0x37baf8
003b9eb8: bl       #0x38d798
003b9ebc: mov      r1, #1
003b9ec0: mov      r4, r0
003b9ec4: mov      r0, r5
003b9ec8: bl       #0x37baf8
003b9ecc: bl       #0x31b580
003b9ed0: mov      r1, r4
003b9ed4: mov      r2, r0
003b9ed8: add      r0, r6, #0x560
003b9edc: bl       #0x3b55d4
003b9ee0: mov      r1, r0
003b9ee4: mov      r0, r7
003b9ee8: pop      {r4, r5, r6, r7, r8, sb, sl, lr}
003b9eec: b        #0x37cb24
003b9ef0: mov      r1, #0
003b9ef4: mov      r0, r5
003b9ef8: bl       #0x37baf8
003b9efc: bl       #0x38d798
003b9f00: mov      r1, r8
003b9f04: mov      sl, r0
003b9f08: mov      r0, r5
003b9f0c: bl       #0x37baf8
003b9f10: bl       #0x31bc80
003b9f14: cmp      r0, #0
003b9f18: addeq    r1, r6, #0xff0
003b9f1c: add      r0, r6, #0x560
003b9f20: addeq    r1, r1, #4
003b9f24: moveq    r2, sl
003b9f28: beq      #0x3b9e80
003b9f2c: ldr      r3, [pc, #0xc]
003b9f30: mov      r2, sl
003b9f34: ldr      r1, [r4, r3]
003b9f38: b        #0x3b9e80
003b9f3c: ldrsbeq  sl, [sp], #-0xcc
003b9f40: andeq    r1, r0, ip, asr #32
