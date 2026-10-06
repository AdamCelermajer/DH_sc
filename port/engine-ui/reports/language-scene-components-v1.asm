_ZN9Character27UpdateInventoryLocalizationEv
003b36e4 add      r0, r0, #0x37c
003b36e8 b        #0x3fdfa0
_ZN10ItemObject18UpdateLocalizationEv
003ebca8 push     {r4, lr}
003ebcac add      r0, r0, #0x374
003ebcb0 mov      r1, #0
003ebcb4 bl       #0x3fc61c ; _ZN13ItemInventory7GetItemEj
003ebcb8 cmp      r0, #0
003ebcbc beq      #0x3ebcc8
003ebcc0 pop      {r4, lr}
003ebcc4 b        #0x3fc1b4
003ebcc8 pop      {r4, pc}
_ZNK9Character11GetCharTypeEv
003a3054 push     {r4, lr}
003a3058 bl       #0x3a3024 ; _ZNK9Character9GetCharAIEv
003a305c ldr      r0, [r0, #0x38]
003a3060 pop      {r4, pc}
_ZN13ItemInventory26UpdateLocalizationForItemsEv
003fdfa0 push     {r4, r5, r6, lr}
003fdfa4 mov      r6, r0
003fdfa8 bl       #0x3fc608 ; _ZNK13ItemInventory11GetNumItemsEv
003fdfac subs     r5, r0, #0
003fdfb0 ble      #0x3fdfd4
003fdfb4 mov      r4, #0
003fdfb8 mov      r1, r4
003fdfbc mov      r0, r6
003fdfc0 bl       #0x3fc61c ; _ZN13ItemInventory7GetItemEj
003fdfc4 add      r4, r4, #1
003fdfc8 bl       #0x3fc1b4 ; _ZN12ItemInstance18UpdateLocalizationEv
003fdfcc cmp      r5, r4
003fdfd0 bne      #0x3fdfb8
003fdfd4 pop      {r4, r5, r6, pc}
_ZN12ItemInstance18UpdateLocalizationEv
003fc1b4 push     {r4, r5, r6, r7, lr}
003fc1b8 mov      r4, r0
003fc1bc sub      sp, sp, #0xc
003fc1c0 bl       #0x3fb754 ; _ZN12ItemInstance11_UpdateNameEv
003fc1c4 mov      r0, r4
003fc1c8 bl       #0x3fb290 ; _ZN12ItemInstance12_UpdateStatsEv
003fc1cc mov      r0, r4
003fc1d0 bl       #0x3facdc ; _ZN12ItemInstance11_UpdateReqsEv
003fc1d4 mov      r0, r4
003fc1d8 bl       #0x3f9e80 ; _ZNK12ItemInstance12GetNumPowersEv
003fc1dc mov      r1, #0
003fc1e0 mov      r5, r0
003fc1e4 lsl      r0, r0, #2
003fc1e8 bl       #0x31056c ; _Znaj15MemoryHintState
003fc1ec cmp      r5, #0
003fc1f0 mov      r6, r0
003fc1f4 beq      #0x3fc218
003fc1f8 mov      r7, #0
003fc1fc mov      r1, r7
003fc200 mov      r0, r4
003fc204 bl       #0x3fa038 ; _ZNK12ItemInstance10GetPowerIdEj
003fc208 str      r0, [r6, r7, lsl #2]
003fc20c add      r7, r7, #1
003fc210 cmp      r7, r5
003fc214 bne      #0x3fc1fc
003fc218 ldr      r1, [r4, #0x5c]
003fc21c ldr      r2, [r4, #0x60]
003fc220 cmp      r1, r2
003fc224 beq      #0x3fc234
003fc228 add      r0, r4, #0x5c
003fc22c add      r3, sp, #4
003fc230 bl       #0x3fa2c8 ; _ZNSt6vectorIN12ItemInstance9PowerInfoESaIS1_EE8_M_eraseEPS1_S4_RKSt12__false_type
003fc234 cmp      r5, #0
003fc238 beq      #0x3fc25c
003fc23c mov      r7, #0
003fc240 ldr      r1, [r6, r7, lsl #2]
003fc244 mov      r0, r4
003fc248 add      r7, r7, #1
003fc24c mvn      r2, #0
003fc250 bl       #0x3fbc60 ; _ZN12ItemInstance8AddPowerEii
003fc254 cmp      r7, r5
003fc258 bne      #0x3fc240
003fc25c mov      r0, r6
003fc260 bl       #0x310440 ; _Z10CustomFreePv
003fc264 add      sp, sp, #0xc
003fc268 pop      {r4, r5, r6, r7, pc}
