_ZN10ItemObject9InitAgainER13ItemInventoryjPK9Character
003ec0f0 push     {r4, r5, r6, r7, r8, sl, lr}
003ec0f4 add      r7, r0, #0x374
003ec0f8 sub      sp, sp, #0x34
003ec0fc mov      r5, #0
003ec100 mov      r4, r0
003ec104 mov      r6, r3
003ec108 mov      r0, r1
003ec10c mov      r3, #1
003ec110 mov      r1, r2
003ec114 mov      r2, r7
003ec118 str      r5, [sp]
003ec11c bl       #0x3ffa44 ; _ZN13ItemInventory14TransferItemToEjRS_bb
003ec120 mov      r0, r7
003ec124 mov      r1, r5
003ec128 bl       #0x3fc61c ; _ZN13ItemInventory7GetItemEj
003ec12c ldr      r5, [pc, #0x1bc]
003ec130 subs     r7, r0, #0
003ec134 add      r5, pc, r5
003ec138 beq      #0x3ec29c
003ec13c bl       #0x3f9e08 ; _ZNK12ItemInstance7GetItemEv
003ec140 ldr      r3, [r0, #0x54]
003ec144 cmn      r3, #1
003ec148 beq      #0x3ec184
003ec14c ldr      r3, [pc, #0x1a0]
003ec150 mov      r0, r7
003ec154 ldr      r3, [r5, r3]
003ec158 ldr      r8, [r3]
003ec15c bl       #0x3f9e08 ; _ZNK12ItemInstance7GetItemEv
003ec160 ldr      r3, [r0, #0x54]
003ec164 mov      r2, #0x14
003ec168 mla      r8, r2, r3, r8
003ec16c mov      r3, #0x3b4
003ec170 ldrh     r2, [r8, #4]
003ec174 strh     r2, [r4, r3]
003ec178 ldrh     r8, [r8, #8]
003ec17c movw     r3, #0x3b6
003ec180 strh     r8, [r4, r3]
003ec184 ldr      r8, [r4, #0x2d8]
003ec188 cmp      r8, #0
003ec18c beq      #0x3ec1bc
003ec190 mov      r0, r7
003ec194 bl       #0x3fa710 ; _ZNK12ItemInstance8GetColorEv
003ec198 ldr      r2, [r8, #0x38]
003ec19c mov      r3, #0
003ec1a0 mov      r1, r3
003ec1a4 ldr      ip, [r2]
003ec1a8 mov      r0, r2
003ec1ac str      r3, [sp]
003ec1b0 mov      r2, r3
003ec1b4 mov      lr, pc
003ec1b8 ldr      pc, [ip, #0x1c]
003ec1bc cmp      r6, #0
003ec1c0 strne    r6, [r4, #0x3bc]
003ec1c4 mov      r3, #0x3b4
003ec1c8 ldrsh    r1, [r4, r3]
003ec1cc ldr      r3, [pc, #0x124]
003ec1d0 ldr      lr, [r4, #0x1b0]
003ec1d4 ldr      r6, [r4, #0x1ac]
003ec1d8 ldr      r3, [r5, r3]
003ec1dc ldr      r8, [r4, #0x1a8]
003ec1e0 mov      ip, #0xbf000000
003ec1e4 ldr      r0, [r3]
003ec1e8 add      ip, ip, #0x800000
003ec1ec mov      r7, #1
003ec1f0 add      r2, sp, #0x24
003ec1f4 mov      r3, #0
003ec1f8 str      lr, [sp, #0x2c]
003ec1fc str      ip, [sp, #8]
003ec200 str      ip, [sp, #4]
003ec204 str      r8, [sp, #0x24]
003ec208 str      r6, [sp, #0x28]
003ec20c str      r7, [sp]
003ec210 bl       #0x36b5d8 ; _ZN15VoxSoundManager6Play3DEiRKN6glitch4core8vector3dIfEEbiff
003ec214 ldr      r3, [pc, #0xe0]
003ec218 mov      r6, #0
003ec21c mov      r1, r6
003ec220 ldr      r3, [r5, r3]
003ec224 mov      r0, #0x28
003ec228 ldr      sl, [r3, #0x44]
003ec22c bl       #0x310570 ; _Znwj15MemoryHintState
003ec230 mvn      ip, #2
003ec234 str      ip, [sp, #0xc]
003ec238 mov      ip, #0x40
003ec23c mov      r1, sl
003ec240 mov      r2, r4
003ec244 mov      r3, r6
003ec248 str      ip, [sp, #0x10]
003ec24c mov      ip, #4
003ec250 mov      r8, r0
003ec254 str      ip, [sp, #0x14]
003ec258 str      r7, [sp, #4]
003ec25c str      r7, [sp]
003ec260 str      r6, [sp, #8]
003ec264 str      r6, [sp, #0x18]
003ec268 bl       #0x46f2f0 ; _ZN14PhysicalObjectC2EP13PhysicalWorldP10GameObjectbbbbstti
003ec26c ldr      r3, [pc, #0x8c]
003ec270 mov      r0, r4
003ec274 mov      r1, r8
003ec278 ldr      r3, [r5, r3]
003ec27c mov      r2, r6
003ec280 add      r3, r3, #8
003ec284 str      r3, [r8]
003ec288 bl       #0x394bf8 ; _ZN10GameObject17SetPhysicalObjectEP14PhysicalObjectb
003ec28c mov      r0, r4
003ec290 bl       #0x3ebca4 ; _ZN10ItemObject8ShowGlowEv
003ec294 add      sp, sp, #0x34
003ec298 pop      {r4, r5, r6, r7, r8, sl, pc}
003ec29c ldr      r3, [pc, #0x60]
003ec2a0 ldr      r3, [r5, r3]
003ec2a4 ldr      r3, [r3]
003ec2a8 cmp      r3, #2
003ec2ac streq    r7, [r7]
003ec2b0 beq      #0x3ec1c4
003ec2b4 cmp      r3, #1
003ec2b8 bne      #0x3ec1c4
003ec2bc ldr      r0, [pc, #0x44]
003ec2c0 ldr      r1, [pc, #0x44]
003ec2c4 ldr      r2, [pc, #0x44]
003ec2c8 ldr      r0, [r5, r0]
003ec2cc ldr      r3, [pc, #0x40]
003ec2d0 movw     ip, #0x1c7
003ec2d4 add      r1, pc, r1
003ec2d8 add      r2, pc, r2
003ec2dc add      r3, pc, r3
003ec2e0 add      r0, r0, #0xa8
003ec2e4 str      ip, [sp]
003ec2e8 bl       #0x30e004
003ec2ec b        #0x3ec1c4
003ec2f0 subseq   r8, sl, ip, asr sb
003ec2f4 andeq    r0, r0, ip, lsr sb
003ec2f8 andeq    r0, r0, r4, lsr #27
003ec2fc strdeq   r3, r4, [r0], -r4
003ec300 muleq    r0, r0, r6
003ec304 andeq    r3, r0, r0, asr #19
003ec308 andeq    r1, r0, r0, asr #19
003ec30c subeq    r2, sp, r4, lsl #2
003ec310 subeq    sb, sp, r0, asr pc
003ec314 subeq    sb, sp, r4, asr pc
