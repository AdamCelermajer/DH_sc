
# _ZN9Character17SG_GetPlayerLevelEv
003bb828: movw     r3, #0x14e8
003bb82c: ldr      r3, [r0, r3]
003bb830: cmp      r3, #0
003bb834: mvneq    r0, #0
003bb838: ldrne    r0, [r3, #0x30]
003bb83c: bx       lr

# _ZN13PlayerManager16GetHostingPlayerEv
0036e09c: push     {r4, lr}
0036e0a0: mov      r4, r0
0036e0a4: bl       #0x7fd794
0036e0a8: ldrb     r3, [r0, #5]
0036e0ac: cmp      r3, #0
0036e0b0: bne      #0x36e0c8
0036e0b4: mov      r1, #0
0036e0b8: mov      r0, r4
0036e0bc: mov      r2, #0
0036e0c0: pop      {r4, lr}
0036e0c4: b        #0x36dfb0
0036e0c8: bl       #0x320e98
0036e0cc: ldrb     r3, [r0, #0x24]
0036e0d0: cmp      r3, #0
0036e0d4: beq      #0x36e0b4
0036e0d8: bl       #0x800f8c
0036e0dc: ldr      r3, [r0]
0036e0e0: mov      lr, pc
0036e0e4: ldr      pc, [r3, #0x64]
0036e0e8: cmp      r0, #0
0036e0ec: beq      #0x36e0b4
0036e0f0: bl       #0x8100dc
0036e0f4: bl       #0x8100e0
0036e0f8: cmp      r0, #0
0036e0fc: beq      #0x36e0b4
0036e100: bl       #0x8100dc
0036e104: ldr      r1, [r0, #0x170]
0036e108: b        #0x36e0b8

# _ZNK11Application15GetCurrentLevelEv
0031f594: ldr      r3, [pc, #0x10]
0031f598: ldr      r2, [pc, #0x10]
0031f59c: add      r3, pc, r3
0031f5a0: ldr      r2, [r3, r2]
0031f5a4: ldr      r0, [r2]
0031f5a8: bx       lr

# _ZN9Character17SG_SetPlayerLevelEi
003bb840: movw     r3, #0x14e8
003bb844: ldr      r3, [r0, r3]
003bb848: cmp      r3, #0
003bb84c: strne    r1, [r3, #0x30]
003bb850: bx       lr

# _ZN14PlayerSavegame17__SavePlayerLevelEP11IStreamBasePv
00468930: add      r1, r1, #0x30
00468934: b        #0x38b808

# _ZN14PlayerSavegame17__LoadPlayerLevelEP11IStreamBasePv
004689a0: add      r1, r1, #0x30
004689a4: b        #0x38b758

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
