
# _ZNSt4priv8_Rb_treeIiSt4lessIiESt4pairIKi14ObjectListItemENS_10_Select1stIS6_EENS_11_MapTraitsTIS6_EESaIS6_EE13insert_uniqueENS_17_Rb_tree_iteratorIS6_SA_EERKS6_
0033f914: push     {r4, r5, r6, r7, r8, sl, lr}
0033f918: ldr      r4, [r2]
0033f91c: ldr      r2, [r1, #8]
0033f920: sub      sp, sp, #0x2c
0033f924: mov      r5, r1
0033f928: cmp      r4, r2
0033f92c: mov      r7, r0
0033f930: mov      r6, r3
0033f934: beq      #0x33faa4
0033f938: cmp      r4, r1
0033f93c: beq      #0x33fb24
0033f940: ldrb     r3, [r4]
0033f944: cmp      r3, #0
0033f948: beq      #0x33fa38
0033f94c: ldr      ip, [r4, #8]
0033f950: cmp      ip, #0
0033f954: bne      #0x33f960
0033f958: b        #0x33fa58
0033f95c: mov      ip, r3
0033f960: ldr      r3, [ip, #0xc]
0033f964: cmp      r3, #0
0033f968: bne      #0x33f95c
0033f96c: ldr      r2, [r6]
0033f970: ldr      r0, [r4, #0x10]
0033f974: cmp      r2, r0
0033f978: movge    r1, #0
0033f97c: movlt    r1, #1
0033f980: cmp      r1, #0
0033f984: bne      #0x33f9f8
0033f988: ldr      r8, [r4, #0xc]
0033f98c: cmp      r8, #0
0033f990: beq      #0x33fb8c
0033f994: mov      ip, r8
0033f998: b        #0x33f9a0
0033f99c: mov      ip, r3
0033f9a0: ldr      r3, [ip, #8]
0033f9a4: cmp      r3, #0
0033f9a8: bne      #0x33f99c
0033f9ac: cmp      r1, #0
0033f9b0: bne      #0x33fa88
0033f9b4: cmp      r2, r0
0033f9b8: ble      #0x33fb4c
0033f9bc: cmp      r5, ip
0033f9c0: beq      #0x33f9d0
0033f9c4: ldr      r3, [ip, #0x10]
0033f9c8: cmp      r2, r3
0033f9cc: bge      #0x33fa88
0033f9d0: cmp      r8, #0
0033f9d4: bne      #0x33fb04
0033f9d8: mov      r1, r5
0033f9dc: mov      r2, r4
0033f9e0: mov      r3, r6
0033f9e4: mov      r0, r7
0033f9e8: str      r8, [sp]
0033f9ec: str      r4, [sp, #4]
0033f9f0: bl       #0x33f6b4
0033f9f4: b        #0x33fa2c
0033f9f8: ldr      r3, [ip, #0x10]
0033f9fc: cmp      r2, r3
0033fa00: ble      #0x33f988
0033fa04: ldr      lr, [ip, #0xc]
0033fa08: cmp      lr, #0
0033fa0c: beq      #0x33fb6c
0033fa10: mov      ip, #0
0033fa14: mov      r1, r5
0033fa18: mov      r2, r4
0033fa1c: mov      r3, r6
0033fa20: mov      r0, r7
0033fa24: stm      sp, {r4, ip}
0033fa28: bl       #0x33f6b4
0033fa2c: mov      r0, r7
0033fa30: add      sp, sp, #0x2c
0033fa34: pop      {r4, r5, r6, r7, r8, sl, pc}
0033fa38: ldr      r3, [r4, #4]
0033fa3c: ldr      r3, [r3, #4]
0033fa40: cmp      r4, r3
0033fa44: ldreq    ip, [r4, #0xc]
0033fa48: beq      #0x33f96c
0033fa4c: ldr      ip, [r4, #8]
0033fa50: cmp      ip, #0
0033fa54: bne      #0x33f960
0033fa58: ldr      ip, [r4, #4]
0033fa5c: ldr      r3, [ip, #8]
0033fa60: cmp      r4, r3
0033fa64: beq      #0x33fa70
0033fa68: b        #0x33f96c
0033fa6c: mov      ip, r3
0033fa70: ldr      r3, [ip, #4]
0033fa74: ldr      r2, [r3, #8]
0033fa78: cmp      r2, ip
0033fa7c: beq      #0x33fa6c
0033fa80: mov      ip, r3
0033fa84: b        #0x33f96c
0033fa88: mov      r1, r5
0033fa8c: mov      r2, r6
0033fa90: add      r0, sp, #8
0033fa94: bl       #0x33f78c
0033fa98: ldr      r3, [sp, #8]
0033fa9c: str      r3, [r7]
0033faa0: b        #0x33fa2c
0033faa4: ldr      r2, [r1, #0x10]
0033faa8: cmp      r2, #0
0033faac: beq      #0x33fbfc
0033fab0: ldr      r2, [r3]
0033fab4: ldr      ip, [r4, #0x10]
0033fab8: cmp      r2, ip
0033fabc: blt      #0x33fc14
0033fac0: ble      #0x33fb4c
0033fac4: ldr      lr, [r4, #0xc]
0033fac8: cmp      lr, #0
0033facc: beq      #0x33fbc4
0033fad0: mov      ip, lr
0033fad4: b        #0x33fadc
0033fad8: mov      ip, r3
0033fadc: ldr      r3, [ip, #8]
0033fae0: cmp      r3, #0
0033fae4: bne      #0x33fad8
0033fae8: cmp      r5, ip
0033faec: beq      #0x33fc64
0033faf0: ldr      r3, [ip, #0x10]
0033faf4: cmp      r2, r3
0033faf8: bge      #0x33fc28
0033fafc: cmp      lr, #0
0033fb00: beq      #0x33fc44
0033fb04: mov      lr, #0
0033fb08: mov      r1, r5
0033fb0c: mov      r2, ip
0033fb10: mov      r3, r6
0033fb14: mov      r0, r7
0033fb18: stm      sp, {ip, lr}
0033fb1c: bl       #0x33f6b4
0033fb20: b        #0x33fa2c
0033fb24: ldr      r2, [r4, #0xc]
0033fb28: ldr      ip, [r3]
0033fb2c: ldr      lr, [r2, #0x10]
0033fb30: cmp      lr, ip
0033fb34: bge      #0x33fb54
0033fb38: mov      ip, #0
0033fb3c: str      ip, [sp]
0033fb40: str      r4, [sp, #4]
0033fb44: bl       #0x33f6b4
0033fb48: b        #0x33fa2c
0033fb4c: str      r4, [r7]
0033fb50: b        #0x33fa2c
0033fb54: mov      r2, r3
0033fb58: add      r0, sp, #0x10
0033fb5c: bl       #0x33f78c
0033fb60: ldr      r3, [sp, #0x10]
0033fb64: str      r3, [r7]
0033fb68: b        #0x33fa2c
0033fb6c: mov      r1, r5
0033fb70: mov      r2, ip
0033fb74: mov      r3, r6
0033fb78: mov      r0, r7
0033fb7c: str      lr, [sp]
0033fb80: str      ip, [sp, #4]
0033fb84: bl       #0x33f6b4
0033fb88: b        #0x33fa2c
0033fb8c: ldr      r3, [r4, #4]
0033fb90: ldr      ip, [r3, #0xc]
0033fb94: cmp      r4, ip
0033fb98: movne    ip, r4
0033fb9c: bne      #0x33fbb4
0033fba0: mov      ip, r3
0033fba4: ldr      r3, [r3, #4]
0033fba8: ldr      sl, [r3, #0xc]
0033fbac: cmp      ip, sl
0033fbb0: beq      #0x33fba0
0033fbb4: ldr      sl, [ip, #0xc]
0033fbb8: cmp      r3, sl
0033fbbc: movne    ip, r3
0033fbc0: b        #0x33f9ac
0033fbc4: ldr      r3, [r4, #4]
0033fbc8: ldr      r1, [r3, #0xc]
0033fbcc: cmp      r4, r1
0033fbd0: movne    ip, r4
0033fbd4: bne      #0x33fbec
0033fbd8: mov      ip, r3
0033fbdc: ldr      r3, [r3, #4]
0033fbe0: ldr      r1, [r3, #0xc]
0033fbe4: cmp      r1, ip
0033fbe8: beq      #0x33fbd8
0033fbec: ldr      r1, [ip, #0xc]
0033fbf0: cmp      r3, r1
0033fbf4: movne    ip, r3
0033fbf8: b        #0x33fae8
0033fbfc: mov      r2, r3
0033fc00: add      r0, sp, #0x20
0033fc04: bl       #0x33f78c
0033fc08: ldr      r3, [sp, #0x20]
0033fc0c: str      r3, [r7]
0033fc10: b        #0x33fa2c
0033fc14: mov      ip, #0
0033fc18: mov      r2, r4
0033fc1c: stm      sp, {r4, ip}
0033fc20: bl       #0x33f6b4
0033fc24: b        #0x33fa2c
0033fc28: mov      r1, r5
0033fc2c: mov      r2, r6
0033fc30: add      r0, sp, #0x18
0033fc34: bl       #0x33f78c
0033fc38: ldr      r3, [sp, #0x18]
0033fc3c: str      r3, [r7]
0033fc40: b        #0x33fa2c
0033fc44: mov      r1, r5
0033fc48: mov      r2, r4
0033fc4c: mov      r3, r6
0033fc50: mov      r0, r7
0033fc54: str      lr, [sp]
0033fc58: str      r4, [sp, #4]
0033fc5c: bl       #0x33f6b4
0033fc60: b        #0x33fa2c
0033fc64: mov      ip, #0
0033fc68: mov      r1, r5
0033fc6c: mov      r2, r4
0033fc70: mov      r3, r6
0033fc74: mov      r0, r7
0033fc78: str      ip, [sp]
0033fc7c: str      r4, [sp, #4]
0033fc80: bl       #0x33f6b4
0033fc84: b        #0x33fa2c

# _ZNK9Character18GetCharAIFactionIdEv
003a3180: ldr      r0, [r0, #0xff8]
003a3184: ldr      r3, [pc, #0x24]
003a3188: cmp      r0, #0
003a318c: add      r3, pc, r3
003a3190: blt      #0x3a31a8
003a3194: ldr      r2, [pc, #0x18]
003a3198: ldr      r3, [r3, r2]
003a319c: ldr      r3, [r3]
003a31a0: cmp      r0, r3
003a31a4: bxlt     lr
003a31a8: mov      r0, #0xa
003a31ac: bx       lr
003a31b0: subseq   r1, pc, r4, lsl #18
003a31b4: andeq    r2, r0, r4, asr #4

# _ZNK12ObjectHandle9GetObjectEb
0033ff8c: b        #0x33fdc0

# _ZNK10ObjectBase9GetHandleEv
0033dd70: ldr      r3, [pc, #0x34]
0033dd74: ldr      r2, [pc, #0x34]
0033dd78: push     {r4, lr}
0033dd7c: add      r3, pc, r3
0033dd80: ldr      r2, [r3, r2]
0033dd84: ldr      ip, [r1, #0x2c]
0033dd88: mov      r4, r0
0033dd8c: ldr      lr, [r2, #0x38]
0033dd90: mov      r2, #0xc
0033dd94: ldr      r3, [lr, #0x78]
0033dd98: str      r3, [ip, #8]
0033dd9c: ldr      r1, [r1, #0x2c]
0033dda0: bl       #0x30df38
0033dda4: mov      r0, r4
0033dda8: pop      {r4, pc}
0033ddac: rsbeq    r6, r5, r4, lsl sp
0033ddb0: strdeq   r3, r4, [r0], -r4
