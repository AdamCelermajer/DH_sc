
# _ZNK14CharProperties9PROPS_GetEib.clone.3
003af76c: mov      r2, r1
003af770: add      r1, r0, #0xa90
003af774: add      r1, r1, #4
003af778: b        #0x3dedb4

# _Z16CF_CalcDotDamageP9CharacterS0_b
003b09c4: push     {r4, r5, r6, r7, r8, sb, sl, lr}
003b09c8: add      r4, r1, #0xff0
003b09cc: subs     r7, r3, #0
003b09d0: add      r4, r4, #4
003b09d4: add      r6, r1, #0x560
003b09d8: mov      sl, r2
003b09dc: mov      r1, r4
003b09e0: movne    r2, #0xb5
003b09e4: moveq    r2, #0x7d
003b09e8: mov      r5, r0
003b09ec: mov      r0, r6
003b09f0: bl       #0x3dedb4
003b09f4: cmp      r0, #0
003b09f8: mov      r8, r0
003b09fc: mvnle    r4, #0
003b0a00: movle    r7, #0
003b0a04: ble      #0x3b0a78
003b0a08: cmp      r7, #0
003b0a0c: beq      #0x3b0a8c
003b0a10: mov      r1, r4
003b0a14: mov      r2, #0xb3
003b0a18: mov      r0, r6
003b0a1c: bl       #0x3dedb4
003b0a20: mov      r1, r4
003b0a24: mov      r7, r0
003b0a28: mov      r2, #0xb4
003b0a2c: mov      r0, r6
003b0a30: bl       #0x3dedb4
003b0a34: rsb      r0, r7, r0
003b0a38: bl       #0x3af6d8
003b0a3c: mov      r1, r4
003b0a40: add      r7, r0, r7
003b0a44: mov      r2, #0xb2
003b0a48: mov      r0, r6
003b0a4c: bl       #0x3dedb4
003b0a50: asr      r4, r0, #8
003b0a54: cmn      r4, #1
003b0a58: beq      #0x3b0a78
003b0a5c: add      r1, sl, #0xff0
003b0a60: add      r1, r1, #4
003b0a64: add      r0, sl, #0x560
003b0a68: add      r2, r4, #0x4a
003b0a6c: bl       #0x3dedb4
003b0a70: rsb      r7, r0, r7
003b0a74: bic      r7, r7, r7, asr #31
003b0a78: str      r8, [r5]
003b0a7c: str      r7, [r5, #4]
003b0a80: str      r4, [r5, #8]
003b0a84: mov      r0, r5
003b0a88: pop      {r4, r5, r6, r7, r8, sb, sl, pc}
003b0a8c: mov      r1, r4
003b0a90: mov      r2, #0x7b
003b0a94: mov      r0, r6
003b0a98: bl       #0x3dedb4
003b0a9c: mov      r1, r4
003b0aa0: mov      r7, r0
003b0aa4: mov      r2, #0x7c
003b0aa8: mov      r0, r6
003b0aac: bl       #0x3dedb4
003b0ab0: rsb      r0, r7, r0
003b0ab4: bl       #0x3af6d8
003b0ab8: mvn      r4, #0
003b0abc: add      r7, r7, r0
003b0ac0: str      r8, [r5]
003b0ac4: str      r7, [r5, #4]
003b0ac8: str      r4, [r5, #8]
003b0acc: mov      r0, r5
003b0ad0: pop      {r4, r5, r6, r7, r8, sb, sl, pc}
