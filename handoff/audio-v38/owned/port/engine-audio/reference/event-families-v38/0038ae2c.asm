# _ZN10GameObject15UpdateIdleSoundEv
0038ae2c push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0038ae30 ldr      r4, [pc, #0x2a0]
0038ae34 ldr      r6, [pc, #0x2a0]
0038ae38 sub      sp, sp, #0x2c
0038ae3c add      r4, pc, r4
0038ae40 ldr      r3, [r4, r6]
0038ae44 mov      r5, r0
0038ae48 ldr      r2, [r3]
0038ae4c cmp      r2, #0
0038ae50 beq      #0x38aea8
0038ae54 ldrb     r8, [r0, #0x373]
0038ae58 cmp      r8, #0
0038ae5c bne      #0x38aeb0
0038ae60 ldr      r3, [pc, #0x278]
0038ae64 ldr      sl, [r4, r3]
0038ae68 mov      r0, sl
0038ae6c bl       #0x31f594
0038ae70 cmp      r0, #0
0038ae74 beq      #0x38aea8
0038ae78 ldr      r0, [sl, #0x40]
0038ae7c mov      r1, r8
0038ae80 mov      r2, #1
0038ae84 bl       #0x36e478
0038ae88 ldr      r3, [r0, #0x660]
0038ae8c cmp      r3, #0
0038ae90 beq      #0x38aea8
0038ae94 mov      r0, sl
0038ae98 bl       #0x31f594
0038ae9c ldr      r3, [r0, #0x130]
0038aea0 cmp      r3, #0x26
0038aea4 beq      #0x38aedc
0038aea8 add      sp, sp, #0x2c
0038aeac pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
0038aeb0 ldrb     r2, [r0, #0x372]
0038aeb4 cmp      r2, #0
0038aeb8 beq      #0x38aea8
0038aebc mov      r2, #0
0038aec0 strb     r2, [r0, #0x372]
0038aec4 mov      r2, #0x370
0038aec8 ldrsh    r1, [r0, r2]
0038aecc ldr      r0, [r3]
0038aed0 mov      r2, #0x3e8
0038aed4 bl       #0x369fec
0038aed8 b        #0x38aea8
0038aedc ldr      r1, [pc, #0x200]
0038aee0 ldr      r2, [pc, #0x200]
0038aee4 ldr      r0, [sl, #0x2c]
0038aee8 add      r1, pc, r1
0038aeec add      r2, pc, r2
0038aef0 bl       #0x4c4bdc
0038aef4 bl       #0x30e964
0038aef8 ldr      r3, [pc, #0x1ec]
0038aefc mov      r7, r0
0038af00 mov      r1, r8
0038af04 ldr      r3, [r4, r3]
0038af08 ldr      r0, [sl, #0x40]
0038af0c mov      r2, #1
0038af10 ldr      ip, [r3, #8]
0038af14 ldr      lr, [r3]
0038af18 ldr      r3, [r3, #4]
0038af1c str      ip, [sp, #0x24]
0038af20 str      lr, [sp, #0x1c]
0038af24 str      r3, [sp, #0x20]
0038af28 bl       #0x36e478
0038af2c ldr      r3, [r0, #0x660]
0038af30 cmp      r3, #0
0038af34 beq      #0x38b0c8
0038af38 mov      r1, r8
0038af3c ldr      r0, [sl, #0x40]
0038af40 mov      r2, #1
0038af44 bl       #0x36e478
0038af48 ldr      r3, [r0, #0x660]
0038af4c ldr      r1, [r3, #0x160]
0038af50 str      r1, [sp, #0x1c]
0038af54 ldr      sl, [r3, #0x164]
0038af58 str      sl, [sp, #0x20]
0038af5c ldr      r8, [r3, #0x168]
0038af60 str      r8, [sp, #0x24]
0038af64 ldr      r0, [r5, #0x160]
0038af68 bl       #0x30e3ac
0038af6c mov      r1, sl
0038af70 mov      sb, r0
0038af74 ldr      r0, [r5, #0x164]
0038af78 bl       #0x30e3ac
0038af7c mov      r1, r8
0038af80 mov      fp, r0
0038af84 ldr      r0, [r5, #0x168]
0038af88 bl       #0x30e3ac
0038af8c mov      r1, sb
0038af90 mov      sl, r0
0038af94 mov      r0, sb
0038af98 bl       #0x30ed6c
0038af9c mov      r1, fp
0038afa0 mov      r8, r0
0038afa4 mov      r0, fp
0038afa8 bl       #0x30ed6c
0038afac mov      r1, r0
0038afb0 mov      r0, r8
0038afb4 bl       #0x30eba4
0038afb8 mov      r1, sl
0038afbc mov      r8, r0
0038afc0 mov      r0, sl
0038afc4 bl       #0x30ed6c
0038afc8 mov      r1, r0
0038afcc mov      r0, r8
0038afd0 bl       #0x30eba4
0038afd4 bl       #0x30e8a4
0038afd8 bl       #0x30e1c0
0038afdc bl       #0x30e6a0
0038afe0 ldrb     r3, [r5, #0x372]
0038afe4 mov      r8, r0
0038afe8 cmp      r3, #0
0038afec beq      #0x38b04c
0038aff0 mov      r0, r7
0038aff4 mov      r1, r8
0038aff8 bl       #0x30e9ac
0038affc cmp      r0, #0
0038b000 beq      #0x38aea8
0038b004 mov      r0, r7
0038b008 mov      r1, #0
0038b00c bl       #0x30e2f8
0038b010 cmp      r0, #0
0038b014 beq      #0x38aea8
0038b018 ldr      r3, [r4, r6]
0038b01c mov      r2, #0
0038b020 strb     r2, [r5, #0x372]
0038b024 ldr      r0, [r3]
0038b028 mov      r3, #0x370
0038b02c ldrsh    r1, [r5, r3]
0038b030 mov      r2, #0x3e8
0038b034 add      r3, sp, #0x1c
0038b038 str      r7, [sp]
0038b03c bl       #0x36a218
0038b040 ldrb     r3, [r5, #0x372]
0038b044 cmp      r3, #0
0038b048 bne      #0x38aea8
0038b04c mov      r1, r8
0038b050 mov      r0, r7
0038b054 bl       #0x30e2f8
0038b058 cmp      r0, #0
0038b05c beq      #0x38aea8
0038b060 mov      r0, r7
0038b064 mov      r1, #0
0038b068 bl       #0x30e2f8
0038b06c cmp      r0, #0
0038b070 beq      #0x38aea8
0038b074 ldr      r3, [r4, r6]
0038b078 mov      ip, #1
0038b07c strb     ip, [r5, #0x372]
0038b080 ldr      r7, [r5, #0x160]
0038b084 ldr      r6, [r5, #0x164]
0038b088 ldr      r4, [r5, #0x168]
0038b08c ldr      r0, [r3]
0038b090 mov      lr, #0xbf000000
0038b094 mov      r3, #0x370
0038b098 ldrsh    r1, [r5, r3]
0038b09c add      lr, lr, #0x800000
0038b0a0 mov      r3, ip
0038b0a4 add      r2, sp, #0x10
0038b0a8 str      r7, [sp, #0x10]
0038b0ac str      r6, [sp, #0x14]
0038b0b0 str      r4, [sp, #0x18]
0038b0b4 str      lr, [sp, #8]
0038b0b8 str      ip, [sp]
0038b0bc str      lr, [sp, #4]
0038b0c0 bl       #0x36b5d8
0038b0c4 b        #0x38aea8
0038b0c8 ldr      r1, [sp, #0x1c]
0038b0cc ldr      sl, [sp, #0x20]
0038b0d0 ldr      r8, [sp, #0x24]
0038b0d4 b        #0x38af64
0038b0d8 rsbeq    sb, r0, r4, asr ip
0038b0dc andeq    r0, r0, r4, lsr #27
0038b0e0 strdeq   r3, r4, [r0], -r4
0038b0e4 subseq   r7, r3, r0, ror r4
0038b0e8 subseq   r7, r3, ip, ror r4
0038b0ec andeq    r3, r0, ip, lsr #30
