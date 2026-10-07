# _ZN10ObjectBase7EnabledEv 0x33de04 48
0033de04: push     {r4, lr}
0033de08: mov      r1, #1
0033de0c: mov      r4, r0
0033de10: ldr      r3, [r0]
0033de14: mov      lr, pc
0033de18: ldr      pc, [r3, #0x40]
0033de1c: mov      r0, r4
0033de20: ldr      r3, [r4]
0033de24: mov      r1, #1
0033de28: mov      lr, pc
0033de2c: ldr      pc, [r3, #0x3c]
0033de30: pop      {r4, pc}
# _ZN11AISExternalC1Eb 0x3dd0e4 68
003dd0e4: push     {r4, r5, r6, lr}
003dd0e8: ldr      r5, [pc, #0x30]
003dd0ec: mov      r4, r0
003dd0f0: bl       #0x3d8fb0 // _ZN12CharAIScriptC2Eb
003dd0f4: ldr      r2, [pc, #0x28]
003dd0f8: add      r5, pc, r5
003dd0fc: mov      r3, #0
003dd100: ldr      r2, [r5, r2]
003dd104: str      r3, [r4, #0xc0]
003dd108: str      r3, [r4, #0xb8]
003dd10c: add      r2, r2, #8
003dd110: str      r2, [r4]
003dd114: str      r3, [r4, #0xbc]
003dd118: mov      r0, r4
003dd11c: pop      {r4, r5, r6, pc}
# _ZN10AISDefault14OnCollisionEndEP10GameObjectb 0x3dbf40 40
003dbf40: ldr      r3, [pc, #0x18]
003dbf44: ldr      r2, [pc, #0x18]
003dbf48: add      r3, pc, r3
003dbf4c: ldr      r2, [r3, r2]
003dbf50: ldr      r3, [r2]
003dbf54: sub      r3, r3, #1
003dbf58: str      r3, [r2]
003dbf5c: bx       lr
003dbf60: subseq   r8, fp, r8, asr #22
003dbf64: andeq    r1, r0, r8, asr #12
# _ZN10GameObject7EnabledEv 0x38ba38 52
0038ba38: push     {r4, lr}
0038ba3c: mov      r4, r0
0038ba40: bl       #0x33de04 // _ZN10ObjectBase7EnabledEv
0038ba44: ldr      r3, [r4, #0x1cc]
0038ba48: ldr      r0, [r4, #0x2dc]
0038ba4c: orr      r3, r3, #8
0038ba50: cmp      r0, #0
0038ba54: str      r3, [r4, #0x1cc]
0038ba58: beq      #0x38ba60
0038ba5c: bl       #0x46ebe4 // _ZN14PhysicalObject12enableFilterEv
0038ba60: mov      r3, #0
0038ba64: strb     r3, [r4, #0x373]
0038ba68: pop      {r4, pc}
# _ZN6CharAI14OnCollisionEndEP10GameObjectb 0x3d0e34 36
003d0e34: push     {r4, lr}
003d0e38: ldr      r3, [r0, #0x1c]
003d0e3c: cmp      r3, #0
003d0e40: beq      #0x3d0e54
003d0e44: mov      r0, r3
003d0e48: ldr      r3, [r3]
003d0e4c: mov      lr, pc
003d0e50: ldr      pc, [r3, #0xc4]
003d0e54: pop      {r4, pc}
# _ZN10ObjectBase11setUpdatingEb 0x33dcf0 8
0033dcf0: strb     r1, [r0, #0x85]
0033dcf4: bx       lr
# _ZN11POCharacter15onCollisionEndsEP18PhysicalBaseObjectRK7Point2DIfEb 0x46fd28 260
0046fd28: push     {r4, r5, r6, r7, r8, sb, sl, lr}
0046fd2c: ldr      r4, [pc, #0xe8]
0046fd30: ldr      r5, [pc, #0xe8]
0046fd34: sub      sp, sp, #0x28
0046fd38: add      r4, pc, r4
0046fd3c: ldr      r2, [r4, r5]
0046fd40: mov      sl, r3
0046fd44: ldr      r2, [r2]
0046fd48: str      r2, [sp, #0x24]
0046fd4c: ldr      r6, [r0, #8]
0046fd50: ldr      r7, [r1, #8]
0046fd54: cmp      r7, #0
0046fd58: cmpne    r6, #0
0046fd5c: bne      #0x46fd7c
0046fd60: ldr      r3, [r4, r5]
0046fd64: ldr      r2, [sp, #0x24]
0046fd68: ldr      r3, [r3]
0046fd6c: cmp      r2, r3
0046fd70: bne      #0x46fe18
0046fd74: add      sp, sp, #0x28
0046fd78: pop      {r4, r5, r6, r7, r8, sb, sl, pc}
0046fd7c: ldr      r3, [pc, #0xa0]
0046fd80: add      r8, sp, #0xc
0046fd84: ldr      sb, [r4, r3]
0046fd88: mov      r0, sb
0046fd8c: bl       #0x337888 // _ZN13DebugSwitches4loadEv
0046fd90: ldr      r1, [pc, #0x90]
0046fd94: mov      r0, r8
0046fd98: str      r8, [sp, #0x1c]
0046fd9c: add      r1, pc, r1
0046fda0: add      r1, r1, #0x19
0046fda4: str      r8, [sp, #0x20]
0046fda8: bl       #0x46fcd8 // 
0046fdac: mov      r0, sb
0046fdb0: mov      r1, r8
0046fdb4: bl       #0x337a88 // _ZN13DebugSwitches9GetSwitchERKSs
0046fdb8: mov      sb, r0
0046fdbc: mov      r0, r8
0046fdc0: bl       #0x3139ac // _ZNSt4priv12_String_baseIcSaIcEE19_M_deallocate_blockEv
0046fdc4: cmp      sb, #0
0046fdc8: bne      #0x46fe04
0046fdcc: mov      r1, r6
0046fdd0: mov      r0, sp
0046fdd4: bl       #0x33dd2c // _ZN10ObjectBase9GetHandleEv
0046fdd8: mov      r0, sp
0046fddc: bl       #0x33ff54 // _ZN12ObjectHandlecvP9CharacterEv
0046fde0: cmp      r0, #0
0046fde4: mov      r8, sp
0046fde8: beq      #0x46fd60
0046fdec: cmp      sl, #0
0046fdf0: movne    r1, #0x3b
0046fdf4: moveq    r1, #0x3c
0046fdf8: mov      r2, r7
0046fdfc: bl       #0x3a4d5c // _ZN9Character10RaiseEventEiPv
0046fe00: b        #0x46fd60 // 
0046fe04: ldr      r3, [r6]
0046fe08: mov      r0, r6
0046fe0c: mov      lr, pc
0046fe10: ldr      pc, [r3, #0x28]
0046fe14: b        #0x46fdcc // 
0046fe18: bl       #0x30e310 // 
0046fe1c: subseq   r4, r2, r8, asr sp
0046fe20: andeq    r4, r0, ip, lsr #1
0046fe24: andeq    r0, r0, r4, lsl #17
0046fe28: subeq    sp, r5, ip, ror r8
# _ZN10ObjectBase10SetVisibleEb 0x33dcf8 16
0033dcf8: cmp      r1, #0
0033dcfc: ldrbne   r1, [r0, #0x8a]
0033dd00: strb     r1, [r0, #0x80]
0033dd04: bx       lr
# _ZN6CharAI18OnCollisionPersistEP10GameObjectb 0x3d0e10 36
003d0e10: push     {r4, lr}
003d0e14: ldr      r3, [r0, #0x1c]
003d0e18: cmp      r3, #0
003d0e1c: beq      #0x3d0e30
003d0e20: mov      r0, r3
003d0e24: ldr      r3, [r3]
003d0e28: mov      lr, pc
003d0e2c: ldr      pc, [r3, #0xc0]
003d0e30: pop      {r4, pc}
# _ZN10GameObject10SetVisibleEb 0x38b0f0 32
0038b0f0: cmp      r1, #0
0038b0f4: ldr      r3, [r0, #0x2d8]
0038b0f8: ldrbne   r1, [r0, #0x8a]
0038b0fc: cmp      r3, #0
0038b100: strb     r1, [r0, #0x80]
0038b104: bxeq     lr
0038b108: mov      r0, r3
0038b10c: b        #0x4713d0 // _ZN12VisualObject14SyncVisibilityEv
# _ZN11POCharacter18onCollisionResultsEP18PhysicalBaseObjectRK7Point2DIfEb 0x46fb14 4
0046fb14: bx       lr
# _ZN6CharAI16OnCollisionBeginEP10GameObjectb 0x3d0dec 36
003d0dec: push     {r4, lr}
003d0df0: ldr      r3, [r0, #0x1c]
003d0df4: cmp      r3, #0
003d0df8: beq      #0x3d0e0c
003d0dfc: mov      r0, r3
003d0e00: ldr      r3, [r3]
003d0e04: mov      lr, pc
003d0e08: ldr      pc, [r3, #0xbc]
003d0e0c: pop      {r4, pc}
# _ZN11POCharacter19onCollisionPersistsEP18PhysicalBaseObjectRK7Point2DIfEb 0x46ff6c 264
0046ff6c: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0046ff70: ldr      r4, [pc, #0xec]
0046ff74: ldr      r5, [pc, #0xec]
0046ff78: sub      sp, sp, #0x2c
0046ff7c: add      r4, pc, r4
0046ff80: ldr      r2, [r4, r5]
0046ff84: mov      sl, r3
0046ff88: ldr      r2, [r2]
0046ff8c: str      r2, [sp, #0x24]
0046ff90: ldr      r6, [r0, #8]
0046ff94: ldr      r7, [r1, #8]
0046ff98: cmp      r7, #0
0046ff9c: cmpne    r6, #0
0046ffa0: bne      #0x46ffc0
0046ffa4: ldr      r3, [r4, r5]
0046ffa8: ldr      r2, [sp, #0x24]
0046ffac: ldr      r3, [r3]
0046ffb0: cmp      r2, r3
0046ffb4: bne      #0x470060
0046ffb8: add      sp, sp, #0x2c
0046ffbc: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
0046ffc0: mov      r1, r6
0046ffc4: mov      r0, sp
0046ffc8: bl       #0x33dd2c // _ZN10ObjectBase9GetHandleEv
0046ffcc: mov      r0, sp
0046ffd0: bl       #0x33ff54 // _ZN12ObjectHandlecvP9CharacterEv
0046ffd4: ldr      r3, [pc, #0x90]
0046ffd8: mov      fp, r0
0046ffdc: add      r8, sp, #0xc
0046ffe0: ldr      sb, [r4, r3]
0046ffe4: mov      r0, sb
0046ffe8: bl       #0x337888 // _ZN13DebugSwitches4loadEv
0046ffec: ldr      r1, [pc, #0x7c]
0046fff0: mov      r0, r8
0046fff4: str      r8, [sp, #0x1c]
0046fff8: add      r1, pc, r1
0046fffc: add      r1, r1, #0x19
00470000: str      r8, [sp, #0x20]
00470004: bl       #0x46fcd8 // 
00470008: mov      r0, sb
0047000c: mov      r1, r8
00470010: bl       #0x337a88 // _ZN13DebugSwitches9GetSwitchERKSs
00470014: mov      sb, r0
00470018: mov      r0, r8
0047001c: bl       #0x3139ac // _ZNSt4priv12_String_baseIcSaIcEE19_M_deallocate_blockEv
00470020: cmp      sb, #0
00470024: bne      #0x47004c
00470028: cmp      fp, #0
0047002c: beq      #0x46ffa4
00470030: cmp      sl, #0
00470034: movne    r1, #0x39
00470038: moveq    r1, #0x3a
0047003c: mov      r0, fp
00470040: mov      r2, r7
00470044: bl       #0x3a4d5c // _ZN9Character10RaiseEventEiPv
00470048: b        #0x46ffa4 // 
0047004c: mov      r0, r6
00470050: ldr      r3, [r6]
00470054: mov      lr, pc
00470058: ldr      pc, [r3, #0x28]
0047005c: b        #0x470028 // 
00470060: bl       #0x30e310 // 
00470064: subseq   r4, r2, r4, lsl fp
00470068: andeq    r4, r0, ip, lsr #1
0047006c: andeq    r0, r0, r4, lsl #17
00470070: subeq    sp, r5, r0, lsr #12
# _ZN11POCharacter17onCollisionBeginsEP18PhysicalBaseObjectRK7Point2DIfEb 0x46fe68 260
0046fe68: push     {r4, r5, r6, r7, r8, sb, sl, lr}
0046fe6c: ldr      r4, [pc, #0xe8]
0046fe70: ldr      r5, [pc, #0xe8]
0046fe74: sub      sp, sp, #0x28
0046fe78: add      r4, pc, r4
0046fe7c: ldr      r2, [r4, r5]
0046fe80: mov      sl, r3
0046fe84: ldr      r2, [r2]
0046fe88: str      r2, [sp, #0x24]
0046fe8c: ldr      r6, [r0, #8]
0046fe90: ldr      r7, [r1, #8]
0046fe94: cmp      r7, #0
0046fe98: cmpne    r6, #0
0046fe9c: bne      #0x46febc
0046fea0: ldr      r3, [r4, r5]
0046fea4: ldr      r2, [sp, #0x24]
0046fea8: ldr      r3, [r3]
0046feac: cmp      r2, r3
0046feb0: bne      #0x46ff58
0046feb4: add      sp, sp, #0x28
0046feb8: pop      {r4, r5, r6, r7, r8, sb, sl, pc}
0046febc: ldr      r3, [pc, #0xa0]
0046fec0: add      r8, sp, #0xc
0046fec4: ldr      sb, [r4, r3]
0046fec8: mov      r0, sb
0046fecc: bl       #0x337888 // _ZN13DebugSwitches4loadEv
0046fed0: ldr      r1, [pc, #0x90]
0046fed4: mov      r0, r8
0046fed8: str      r8, [sp, #0x1c]
0046fedc: add      r1, pc, r1
0046fee0: add      r1, r1, #0x19
0046fee4: str      r8, [sp, #0x20]
0046fee8: bl       #0x46fcd8 // 
0046feec: mov      r0, sb
0046fef0: mov      r1, r8
0046fef4: bl       #0x337a88 // _ZN13DebugSwitches9GetSwitchERKSs
0046fef8: mov      sb, r0
0046fefc: mov      r0, r8
0046ff00: bl       #0x3139ac // _ZNSt4priv12_String_baseIcSaIcEE19_M_deallocate_blockEv
0046ff04: cmp      sb, #0
0046ff08: bne      #0x46ff44
0046ff0c: mov      r1, r6
0046ff10: mov      r0, sp
0046ff14: bl       #0x33dd2c // _ZN10ObjectBase9GetHandleEv
0046ff18: mov      r0, sp
0046ff1c: bl       #0x33ff54 // _ZN12ObjectHandlecvP9CharacterEv
0046ff20: cmp      r0, #0
0046ff24: mov      r8, sp
0046ff28: beq      #0x46fea0
0046ff2c: cmp      sl, #0
0046ff30: movne    r1, #0x37
0046ff34: moveq    r1, #0x38
0046ff38: mov      r2, r7
0046ff3c: bl       #0x3a4d5c // _ZN9Character10RaiseEventEiPv
0046ff40: b        #0x46fea0 // 
0046ff44: ldr      r3, [r6]
0046ff48: mov      r0, r6
0046ff4c: mov      lr, pc
0046ff50: ldr      pc, [r3, #0x28]
0046ff54: b        #0x46ff0c // 
0046ff58: bl       #0x30e310 // 
0046ff5c: subseq   r4, r2, r8, lsl ip
0046ff60: andeq    r4, r0, ip, lsr #1
0046ff64: andeq    r0, r0, r4, lsl #17
0046ff68: subeq    sp, r5, ip, lsr r7
# _ZN9Character7EnabledEv 0x3a598c 4
003a598c: b        #0x38ba38 // _ZN10GameObject7EnabledEv
# _ZN6CharAI17OnCollisionResultEP10GameObjectb 0x3d0e58 36
003d0e58: push     {r4, lr}
003d0e5c: ldr      r3, [r0, #0x1c]
003d0e60: cmp      r3, #0
003d0e64: beq      #0x3d0e78
003d0e68: mov      r0, r3
003d0e6c: ldr      r3, [r3]
003d0e70: mov      lr, pc
003d0e74: ldr      pc, [r3, #0xc8]
003d0e78: pop      {r4, pc}
# _ZNK16CharStateMachine16SM_IsKnockedBackEv 0x3c03a4 24
003c03a4: push     {r4, lr}
003c03a8: bl       #0x3c01ac // _ZNK16CharStateMachine11SM_GetStateEv
003c03ac: cmp      r0, #0xa
003c03b0: movne    r0, #0
003c03b4: moveq    r0, #1
003c03b8: pop      {r4, pc}
# _ZN10AISDefault18OnCollisionPersistEP10GameObjectb 0x3dbfa0 396
003dbfa0: push     {r4, r5, r6, r7, r8, sb, sl, lr}
003dbfa4: mov      r4, r0
003dbfa8: ldr      r0, [r0, #0x98]
003dbfac: mov      r5, r1
003dbfb0: mov      r1, #0
003dbfb4: add      r0, r0, #0x4f0
003dbfb8: add      r0, r0, #0xc
003dbfbc: mov      r7, r2
003dbfc0: bl       #0x3c029c // _ZNK16CharStateMachine11SM_IsMovingEb
003dbfc4: ldr      r6, [pc, #0x158]
003dbfc8: cmp      r0, #0
003dbfcc: add      r6, pc, r6
003dbfd0: beq      #0x3dc004
003dbfd4: ldr      r3, [r4, #0x98]
003dbfd8: ldr      sl, [r3, #0x408]
003dbfdc: cmp      r5, sl
003dbfe0: beq      #0x3dc004
003dbfe4: ldr      r3, [r5]
003dbfe8: mov      r0, r5
003dbfec: mov      lr, pc
003dbff0: ldr      pc, [r3, #0x24]
003dbff4: cmp      r0, #0
003dbff8: bne      #0x3dc060
003dbffc: cmp      r7, #0
003dc000: bne      #0x3dc008
003dc004: pop      {r4, r5, r6, r7, r8, sb, sl, pc}
003dc008: ldr      r3, [r5]
003dc00c: mov      r0, r5
003dc010: mov      lr, pc
003dc014: ldr      pc, [r3, #0x24]
003dc018: cmp      r0, #0
003dc01c: beq      #0x3dc0dc
003dc020: ldr      r2, [pc, #0x100]
003dc024: ldr      r3, [r4, #0xc0]
003dc028: ldr      r0, [r6, r2]
003dc02c: ldr      r2, [r0, #0x74]
003dc030: cmp      r3, r2
003dc034: beq      #0x3dc004
003dc038: ldr      r3, [r4, #0x98]
003dc03c: ldrb     r3, [r3, #0x3e0]
003dc040: cmp      r3, #0
003dc044: bne      #0x3dc004
003dc048: str      r2, [r4, #0xc0]
003dc04c: ldr      r5, [r4, #0xbc]
003dc050: bl       #0x31f66c // _ZN11Application5GetDtEv
003dc054: add      r0, r0, r5
003dc058: str      r0, [r4, #0xbc]
003dc05c: pop      {r4, r5, r6, r7, r8, sb, sl, pc}
003dc060: ldr      r2, [r4, #0x98]
003dc064: mov      r0, r2
003dc068: ldr      r3, [r2]
003dc06c: ldr      sb, [r2, #0x418]
003dc070: mov      lr, pc
003dc074: ldr      pc, [r3, #0x28]
003dc078: subs     r8, r0, #0
003dc07c: bne      #0x3dc09c
003dc080: rsbs     r3, sb, #1
003dc084: movlo    r3, #0
003dc088: cmp      sb, sl
003dc08c: moveq    sl, r3
003dc090: orrne    sl, r3, #1
003dc094: cmp      sl, #0
003dc098: bne      #0x3dc0f4
003dc09c: ldr      r3, [r4, #0x98]
003dc0a0: mov      r0, r3
003dc0a4: ldr      r3, [r3]
003dc0a8: mov      lr, pc
003dc0ac: ldr      pc, [r3, #0x28]
003dc0b0: cmp      r0, #0
003dc0b4: beq      #0x3dbffc
003dc0b8: ldr      r0, [r4, #0x98]
003dc0bc: mov      r1, r5
003dc0c0: add      r0, r0, #0x3c8
003dc0c4: bl       #0x3d574c // _ZNK6CharAI10AI_IsEnemyEPK10GameObject
003dc0c8: cmp      r0, #0
003dc0cc: beq      #0x3dbffc
003dc0d0: ldr      r0, [r4, #0x98]
003dc0d4: bl       #0x3bc6b8 // _ZN9Character14CancelSneakingEv
003dc0d8: b        #0x3dbffc // 
003dc0dc: ldr      r3, [r5, #0xf4]
003dc0e0: cmp      r3, #0x15
003dc0e4: beq      #0x3dc020
003dc0e8: cmp      r3, #2
003dc0ec: bne      #0x3dc004
003dc0f0: b        #0x3dc020 // 
003dc0f4: ldr      r0, [r4, #0x98]
003dc0f8: mov      r1, r5
003dc0fc: add      r0, r0, #0x3c8
003dc100: bl       #0x3d574c // _ZNK6CharAI10AI_IsEnemyEPK10GameObject
003dc104: cmp      r0, #0
003dc108: beq      #0x3dc09c
003dc10c: ldr      r0, [r4, #0x98]
003dc110: mov      r1, r5
003dc114: mov      r2, r8
003dc118: add      r0, r0, #0x3c8
003dc11c: pop      {r4, r5, r6, r7, r8, sb, sl, lr}
003dc120: b        #0x3d6890 // _ZN6CharAI12AI_SetTargetEP10GameObjectb
003dc124: subseq   r8, fp, r4, asr #21
003dc128: strdeq   r3, r4, [r0], -r4
# _ZN10AISDefault16OnCollisionBeginEP10GameObjectb 0x3dbf08 56
003dbf08: ldr      r3, [pc, #0x28]
003dbf0c: ldr      ip, [pc, #0x28]
003dbf10: push     {r4, lr}
003dbf14: add      r3, pc, r3
003dbf18: ldr      ip, [r3, ip]
003dbf1c: ldr      r3, [ip]
003dbf20: add      r3, r3, #1
003dbf24: str      r3, [ip]
003dbf28: ldr      r3, [r0]
003dbf2c: mov      lr, pc
003dbf30: ldr      pc, [r3, #0xc0]
003dbf34: pop      {r4, pc}
003dbf38: subseq   r8, fp, ip, ror fp
003dbf3c: andeq    r1, r0, r8, asr #12
# _ZN10AISDefault17OnCollisionResultEP10GameObjectb 0x3dbf68 4
003dbf68: bx       lr
# _ZN11POCharacter15onCollisionTestEP18PhysicalBaseObjectsttstt 0x46fb4c 176
0046fb4c: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0046fb50: sub      sp, sp, #0x2c
0046fb54: mov      r8, r3
0046fb58: ldrh     r3, [sp, #0x5c]
0046fb5c: mov      r5, r0
0046fb60: add      r4, sp, #0x1c
0046fb64: mov      r0, r4
0046fb68: mov      r7, r1
0046fb6c: ldr      r1, [r5, #8]
0046fb70: mov      sl, r2
0046fb74: str      r3, [sp, #0x14]
0046fb78: ldrh     sb, [sp, #0x50]
0046fb7c: ldrsh    fp, [sp, #0x54]
0046fb80: ldrh     r6, [sp, #0x58]
0046fb84: bl       #0x33dd2c // _ZN10ObjectBase9GetHandleEv
0046fb88: mov      r0, r4
0046fb8c: bl       #0x33ff54 // _ZN12ObjectHandlecvP9CharacterEv
0046fb90: cmp      r0, #0
0046fb94: beq      #0x46fbd4
0046fb98: add      r4, r0, #0x4f0
0046fb9c: add      r4, r4, #0xc
0046fba0: mov      r0, r4
0046fba4: bl       #0x3c01c0 // _ZNK16CharStateMachine13SM_IsInLimbusEv
0046fba8: cmp      r0, #0
0046fbac: beq      #0x46fbbc
0046fbb0: mov      r0, #0
0046fbb4: add      sp, sp, #0x2c
0046fbb8: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
0046fbbc: mov      r0, r4
0046fbc0: bl       #0x3c03a4 // _ZNK16CharStateMachine16SM_IsKnockedBackEv
0046fbc4: cmp      r0, #0
0046fbc8: beq      #0x46fbd4
0046fbcc: tst      r6, #3
0046fbd0: beq      #0x46fbb0
0046fbd4: ldr      ip, [sp, #0x14]
0046fbd8: mov      r0, r5
0046fbdc: mov      r1, r7
0046fbe0: mov      r2, sl
0046fbe4: mov      r3, r8
0046fbe8: stm      sp, {sb, fp}
0046fbec: str      r6, [sp, #8]
0046fbf0: str      ip, [sp, #0xc]
0046fbf4: bl       #0x46e6bc // _ZN14PhysicalObject15onCollisionTestEP18PhysicalBaseObjectsttstt
0046fbf8: b        #0x46fbb4 // 
# _ZN11AISExternalC2Eb 0x3dd128 68
003dd128: push     {r4, r5, r6, lr}
003dd12c: ldr      r5, [pc, #0x30]
003dd130: mov      r4, r0
003dd134: bl       #0x3d8fb0 // _ZN12CharAIScriptC2Eb
003dd138: ldr      r2, [pc, #0x28]
003dd13c: add      r5, pc, r5
003dd140: mov      r3, #0
003dd144: ldr      r2, [r5, r2]
003dd148: str      r3, [r4, #0xc0]
003dd14c: str      r3, [r4, #0xb8]
003dd150: add      r2, r2, #8
003dd154: str      r2, [r4]
003dd158: str      r3, [r4, #0xbc]
003dd15c: mov      r0, r4
003dd160: pop      {r4, r5, r6, pc}
003dd164: subseq   r7, fp, r4, asr sb
003dd168: andeq    r0, r0, r4, ror #18
# _ZNK16CharStateMachine13SM_IsInLimbusEv 0x3c01c0 20
003c01c0: push     {r4, lr}
003c01c4: bl       #0x3c01ac // _ZNK16CharStateMachine11SM_GetStateEv
003c01c8: rsbs     r0, r0, #1
003c01cc: movlo    r0, #0
003c01d0: pop      {r4, pc}
