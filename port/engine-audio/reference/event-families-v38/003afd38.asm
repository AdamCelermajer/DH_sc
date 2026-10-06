# _ZN9Character18F_ApplyCombatSoundERKNS_12AttackResultEP10GameObjectPS_
003afd38 push     {r4, r5, r6, r7, r8, sb, sl, lr}
003afd3c ldr      r4, [pc, #0x188]
003afd40 ldr      r5, [pc, #0x188]
003afd44 sub      sp, sp, #0x48
003afd48 add      r4, pc, r4
003afd4c ldr      r3, [r4, r5]
003afd50 mov      sb, r0
003afd54 mov      r0, r2
003afd58 ldr      r3, [r3]
003afd5c mov      r8, r2
003afd60 add      r6, sp, #0x2c
003afd64 str      r3, [sp, #0x44]
003afd68 bl       #0x3a32d0
003afd6c ldr      r3, [pc, #0x160]
003afd70 mov      sl, r0
003afd74 ldr      r7, [r4, r3]
003afd78 mov      r0, r7
003afd7c bl       #0x337888
003afd80 ldr      r1, [pc, #0x150]
003afd84 add      r2, sp, #0x28
003afd88 mov      r0, r6
003afd8c add      r1, pc, r1
003afd90 bl       #0x3140ec
003afd94 mov      r0, r7
003afd98 mov      r1, r6
003afd9c bl       #0x337a88
003afda0 mov      r7, r0
003afda4 mov      r0, r6
003afda8 bl       #0x3139ac
003afdac cmp      r7, #0
003afdb0 beq      #0x3afdd0
003afdb4 ldr      r3, [r4, r5]
003afdb8 ldr      r2, [sp, #0x44]
003afdbc ldr      r3, [r3]
003afdc0 cmp      r2, r3
003afdc4 bne      #0x3afec8
003afdc8 add      sp, sp, #0x48
003afdcc pop      {r4, r5, r6, r7, r8, sb, sl, pc}
003afdd0 ldr      r3, [r8]
003afdd4 mov      r0, r8
003afdd8 mov      lr, pc
003afddc ldr      pc, [r3, #0x34]
003afde0 subs     r6, r0, #0
003afde4 bne      #0x3afe68
003afde8 ldr      r3, [sb]
003afdec cmp      r3, #0
003afdf0 ble      #0x3afdb4
003afdf4 ldr      r0, [sl, #0xc]
003afdf8 cmp      r0, #0
003afdfc beq      #0x3afdb4
003afe00 ldr      r3, [pc, #0xd4]
003afe04 ldr      r7, [sl, #0x10]
003afe08 ldr      r3, [r4, r3]
003afe0c ldr      sb, [r3]
003afe10 bl       #0x3af6d8
003afe14 ldr      sl, [r7, r0, lsl #2]
003afe18 mov      r0, r8
003afe1c bl       #0x3935dc
003afe20 ldr      r7, [r0]
003afe24 ldr      lr, [r0, #4]
003afe28 ldr      r8, [r0, #8]
003afe2c mov      ip, #0xbf000000
003afe30 add      ip, ip, #0x800000
003afe34 mov      r0, sb
003afe38 mov      r1, sl
003afe3c mov      r3, r6
003afe40 add      r2, sp, #0x10
003afe44 str      r7, [sp, #0x10]
003afe48 str      lr, [sp, #0x14]
003afe4c str      r8, [sp, #0x18]
003afe50 mov      lr, #1
003afe54 str      lr, [sp]
003afe58 str      ip, [sp, #8]
003afe5c str      ip, [sp, #4]
003afe60 bl       #0x36b5d8
003afe64 b        #0x3afdb4
003afe68 ldr      r0, [sl, #4]
003afe6c cmp      r0, #0
003afe70 beq      #0x3afdb4
003afe74 ldr      r3, [pc, #0x60]
003afe78 ldr      r6, [sl, #8]
003afe7c ldr      r3, [r4, r3]
003afe80 ldr      sb, [r3]
003afe84 bl       #0x3af6d8
003afe88 ldr      sl, [r6, r0, lsl #2]
003afe8c mov      r0, r8
003afe90 bl       #0x3935dc
003afe94 ldr      r6, [r0]
003afe98 ldr      lr, [r0, #4]
003afe9c ldr      r8, [r0, #8]
003afea0 mov      ip, #0xbf000000
003afea4 add      ip, ip, #0x800000
003afea8 mov      r0, sb
003afeac mov      r1, sl
003afeb0 mov      r3, r7
003afeb4 add      r2, sp, #0x1c
003afeb8 str      r6, [sp, #0x1c]
003afebc str      lr, [sp, #0x20]
003afec0 str      r8, [sp, #0x24]
003afec4 b        #0x3afe50
003afec8 bl       #0x30e310
003afecc subseq   r4, lr, r8, asr #26
003afed0 andeq    r4, r0, ip, lsr #1
003afed4 andeq    r0, r0, r4, lsl #17
003afed8 subseq   r3, r1, r4, asr lr
003afedc andeq    r0, r0, r4, lsr #27
