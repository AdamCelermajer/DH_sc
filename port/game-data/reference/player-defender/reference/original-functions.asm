
# _ZNK16CharStateMachine9SM_IsIdleEb
003c0260: push     {r4, lr}
003c0264: mov      r4, r1
003c0268: bl       #0x3c01ac
003c026c: cmp      r0, #0xd
003c0270: beq      #0x3c028c
003c0274: cmp      r0, #0x12
003c0278: beq      #0x3c0294
003c027c: cmp      r0, #3
003c0280: beq      #0x3c028c
003c0284: mov      r0, #0
003c0288: pop      {r4, pc}
003c028c: mov      r0, #1
003c0290: pop      {r4, pc}
003c0294: eor      r0, r4, #1
003c0298: pop      {r4, pc}

# _ZN14CharProperties12PROPS_AddIntEii
003e0798: lsl      r2, r2, #8
003e079c: b        #0x3e0708

# _ZNK14CharProperties12PROPS_GetIntEib
003df6e0: ldr      r3, [pc, #0x28]
003df6e4: cmp      r2, #0
003df6e8: mov      r2, r1
003df6ec: addeq    r1, r0, #0xa90
003df6f0: add      r3, pc, r3
003df6f4: push     {r4, lr}
003df6f8: addeq    r1, r1, #4
003df6fc: ldrne    r1, [pc, #0x10]
003df700: ldrne    r1, [r3, r1]
003df704: bl       #0x3dedb4
003df708: asr      r0, r0, #8
003df70c: pop      {r4, pc}
003df710: subseq   r5, fp, r0, lsr #7
003df714: andeq    r1, r0, ip, asr #32

# _ZN12v2Controller8Cmd_KillEP10GameObjectb
0040570c: push     {r4, lr}
00405710: ldr      r3, [r0, #4]
00405714: mov      r0, r3
00405718: ldr      r3, [r3]
0040571c: mov      lr, pc
00405720: ldr      pc, [r3, #0x58]
00405724: pop      {r4, pc}

# _ZNK16CharStateMachine11SM_GetStateEv
003c01ac: ldr      r3, [r0, #0x20]
003c01b0: cmp      r3, #0
003c01b4: mvneq    r0, #0
003c01b8: ldrne    r0, [r3]
003c01bc: bx       lr

# _ZN13PlayerManager20GetPlayerByCharacterEPK9Characterb
0036eea8: push     {r4, r5, r6, r7, r8, lr}
0036eeac: mov      r5, r0
0036eeb0: mov      r6, r1
0036eeb4: mov      r7, r2
0036eeb8: bl       #0x7fd794
0036eebc: ldrb     r3, [r0, #5]
0036eec0: cmp      r3, #0
0036eec4: bne      #0x36ef50
0036eec8: ldr      r3, [r5, #0x698]
0036eecc: add      ip, r5, #0x690
0036eed0: cmp      ip, r3
0036eed4: beq      #0x36ef14
0036eed8: ldr      r2, [r3, #0x678]
0036eedc: add      r0, r3, #0x18
0036eee0: cmp      r6, r2
0036eee4: beq      #0x36ef18
0036eee8: ldr      r2, [r3, #0xc]
0036eeec: cmp      r2, #0
0036eef0: bne      #0x36eefc
0036eef4: b        #0x36ef1c
0036eef8: mov      r2, r3
0036eefc: ldr      r3, [r2, #8]
0036ef00: cmp      r3, #0
0036ef04: bne      #0x36eef8
0036ef08: mov      r3, r2
0036ef0c: cmp      ip, r3
0036ef10: bne      #0x36eed8
0036ef14: add      r0, r5, #8
0036ef18: pop      {r4, r5, r6, r7, r8, pc}
0036ef1c: ldr      r1, [r3, #4]
0036ef20: ldr      r0, [r1, #0xc]
0036ef24: cmp      r0, r3
0036ef28: bne      #0x36ef44
0036ef2c: mov      r3, r1
0036ef30: ldr      r1, [r1, #4]
0036ef34: ldr      r2, [r1, #0xc]
0036ef38: cmp      r3, r2
0036ef3c: beq      #0x36ef2c
0036ef40: ldr      r2, [r3, #0xc]
0036ef44: cmp      r1, r2
0036ef48: movne    r3, r1
0036ef4c: b        #0x36eed0
0036ef50: bl       #0x320e98
0036ef54: ldrb     r3, [r0, #0x24]
0036ef58: cmp      r3, #0
0036ef5c: beq      #0x36eec8
0036ef60: bl       #0x800f8c
0036ef64: ldr      r3, [r0]
0036ef68: mov      lr, pc
0036ef6c: ldr      pc, [r3, #0x64]
0036ef70: cmp      r0, #0
0036ef74: beq      #0x36eec8
0036ef78: bl       #0x8100dc
0036ef7c: bl       #0x8100e0
0036ef80: cmp      r0, #0
0036ef84: beq      #0x36eec8
0036ef88: ldr      r2, [r5, #0x6a8]
0036ef8c: ldr      r3, [r5, #0x6ac]
0036ef90: rsb      r3, r2, r3
0036ef94: lsrs     r3, r3, #2
0036ef98: beq      #0x36ef14
0036ef9c: mov      r3, #0
0036efa0: mov      r4, r3
0036efa4: b        #0x36efbc
0036efa8: ldr      r2, [r5, #0x6a8]
0036efac: ldr      r1, [r5, #0x6ac]
0036efb0: rsb      r1, r2, r1
0036efb4: cmp      r4, r1, asr #2
0036efb8: bhs      #0x36ef14
0036efbc: ldr      r1, [r2, r3, lsl #2]
0036efc0: mov      r0, r5
0036efc4: mov      r2, r7
0036efc8: lsl      r8, r3, #2
0036efcc: bl       #0x36dfb0
0036efd0: ldr      r2, [r0, #0x660]
0036efd4: add      r4, r4, #1
0036efd8: mov      r3, r4
0036efdc: cmp      r6, r2
0036efe0: bne      #0x36efa8
0036efe4: ldr      r3, [r5, #0x6a8]
0036efe8: mov      r0, r5
0036efec: mov      r2, r7
0036eff0: ldr      r1, [r3, r8]
0036eff4: pop      {r4, r5, r6, r7, r8, lr}
0036eff8: b        #0x36dec4

# _ZN17PlayerStatManager13IncrementStatE9EStatTypei
003790ec: mov      r3, r2
003790f0: mov      r2, #1
003790f4: b        #0x3790e0
