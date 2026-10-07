_ZN9Character26INV_CheckItemsRequirementsEv
003a9d10 push     {r4, r5, r6, r7, r8, lr}
003a9d14 add      r5, r0, #0x37c
003a9d18 mov      r6, r0
003a9d1c mov      r0, r5
003a9d20 bl       #0x3ffd20 ; _ZNK13ItemInventory20GetNumEquipmentSlotsEv
003a9d24 subs     r7, r0, #0
003a9d28 beq      #0x3a9d8c
003a9d2c mov      r4, #0
003a9d30 mov      r8, r4
003a9d34 b        #0x3a9d44
003a9d38 add      r4, r4, #1
003a9d3c cmp      r4, r7
003a9d40 beq      #0x3a9d84
003a9d44 mov      r1, r4
003a9d48 mov      r0, r5
003a9d4c bl       #0x3ffe3c ; _ZN13ItemInventory15GetEquippedItemEj
003a9d50 mov      r1, r0
003a9d54 mov      r0, r6
003a9d58 bl       #0x3a4930 ; _ZN9Character24INV_DoesMeetRequirementsEP12ItemInstance
003a9d5c cmp      r0, #0
003a9d60 bne      #0x3a9d38
003a9d64 mov      r1, r4
003a9d68 mov      r0, r5
003a9d6c mvn      r2, #0
003a9d70 add      r4, r4, #1
003a9d74 bl       #0x40050c ; _ZN13ItemInventory19UnEquipItemFromSlotEji
003a9d78 cmp      r4, r7
003a9d7c mov      r8, #1
003a9d80 bne      #0x3a9d44
003a9d84 cmp      r8, #0
003a9d88 bne      #0x3a9d90
003a9d8c pop      {r4, r5, r6, r7, r8, pc}
003a9d90 add      r0, r6, #0x560
003a9d94 bl       #0x3e08a8 ; _ZN14CharProperties21UpdateGearsPropertiesEv
003a9d98 mov      r0, r6
003a9d9c bl       #0x3a9d10 ; _ZN9Character26INV_CheckItemsRequirementsEv
003a9da0 mov      r0, r6
003a9da4 bl       #0x3a999c ; _ZN9Character14INV_UpdateSkinEv
003a9da8 mov      r0, r6
003a9dac pop      {r4, r5, r6, r7, r8, lr}
003a9db0 b        #0x3bd140
_ZN14CharProperties16RecalcPropertiesEb
003e0810 cmp      r1, #0
003e0814 push     {r4, r5, r6, lr}
003e0818 mov      r4, r0
003e081c bne      #0x3e0840
003e0820 mov      r5, #0
003e0824 mov      r1, r5
003e0828 mov      r0, r4
003e082c add      r5, r5, #1
003e0830 bl       #0x3dfe60 ; _ZN14CharProperties14RecalcPropertyEi
003e0834 cmp      r5, #0xe0
003e0838 bne      #0x3e0824
003e083c pop      {r4, r5, r6, pc}
003e0840 add      r1, r0, #8
003e0844 ldr      r2, [r0, #0x74]
003e0848 mov      r3, #0
003e084c bl       #0x3e2e20 ; _ZN14CharProperties10_LoadClassERN7Structs19CharacterPropertiesEib
003e0850 b        #0x3e0820
_ZN14CharProperties20PROPS_RemoveAllBuffsEv
003e0af8 push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
003e0afc ldr      r2, [pc, #0x160]
003e0b00 sub      sp, sp, #0x34
003e0b04 add      r3, r0, #0xe10
003e0b08 add      r2, pc, r2
003e0b0c str      r2, [sp, #8]
003e0b10 ldr      r2, [pc, #0x150]
003e0b14 add      r3, r3, #8
003e0b18 str      r3, [sp, #4]
003e0b1c ldr      sl, [r0, #0xe20]
003e0b20 mov      r7, r0
003e0b24 add      sb, sp, #0x20
003e0b28 add      r4, sp, #0x10
003e0b2c str      r2, [sp, #0xc]
003e0b30 ldr      r3, [sp, #4]
003e0b34 cmp      r3, sl
003e0b38 beq      #0x3e0bec
003e0b3c add      r5, sl, #0x34
003e0b40 ldm      r5, {r0, r1, r2, r3}
003e0b44 stm      sb, {r0, r1, r2, r3}
003e0b48 add      r0, sl, #0x44
003e0b4c mov      r1, sb
003e0b50 bl       #0x3de870 ; _ZNKSt4priv20_Deque_iterator_baseIPN14CharProperties8BuffInstEE11_M_subtractERKS4_
003e0b54 subs     r8, r0, #0
003e0b58 beq      #0x3e0ba8
003e0b5c mov      r6, #0
003e0b60 ldm      r5, {r0, r1, r2, r3}
003e0b64 stm      r4, {r0, r1, r2, r3}
003e0b68 mov      r1, r6
003e0b6c mov      r0, r4
003e0b70 bl       #0x3de8b4 ; _ZNSt4priv20_Deque_iterator_baseIPN14CharProperties8BuffInstEE10_M_advanceEi
003e0b74 ldr      r3, [sp, #0x10]
003e0b78 ldr      r0, [r7, #4]
003e0b7c add      r6, r6, #1
003e0b80 ldr      fp, [r3]
003e0b84 add      r0, r0, #0x3b4
003e0b88 ldr      r1, [fp, #0x388]
003e0b8c bl       #0x3db2d8 ; _ZN10CharTimers8TMR_StopEj
003e0b90 mov      r0, fp
003e0b94 bl       #0x4c5740 ; _ZN7Structs19CharacterPropertiesD1Ev
003e0b98 mov      r0, fp
003e0b9c bl       #0x310440 ; _Z10CustomFreePv
003e0ba0 cmp      r6, r8
003e0ba4 bne      #0x3e0b60
003e0ba8 ldr      r2, [sp, #8]
003e0bac ldr      r3, [sp, #0xc]
003e0bb0 add      r1, sl, #0x18
003e0bb4 ldr      r0, [r2, r3]
003e0bb8 bl       #0x494978 ; _ZN15VisualFXManager14DropAnimatedFXERP10AnimatedFX
003e0bbc ldr      r2, [sl, #0xc]
003e0bc0 cmp      r2, #0
003e0bc4 bne      #0x3e0bd0
003e0bc8 b        #0x3e0c30
003e0bcc mov      r2, r3
003e0bd0 ldr      r3, [r2, #8]
003e0bd4 cmp      r3, #0
003e0bd8 bne      #0x3e0bcc
003e0bdc ldr      r3, [sp, #4]
003e0be0 mov      sl, r2
003e0be4 cmp      r3, sl
003e0be8 bne      #0x3e0b3c
003e0bec ldr      r3, [r7, #0xe28]
003e0bf0 cmp      r3, #0
003e0bf4 beq      #0x3e0c1c
003e0bf8 ldr      r0, [sp, #4]
003e0bfc ldr      r1, [r7, #0xe1c]
003e0c00 bl       #0x3e0ab8 ; _ZNSt4priv8_Rb_treeIiSt4lessIiESt4pairIKiN14CharProperties8BuffDeclEENS_10_Select1stIS7_EENS_11_MapTraitsTIS7_EESaIS7_EE8_M_eraseEPNS_18_Rb_tree_node_baseE
003e0c04 ldr      r2, [sp, #4]
003e0c08 mov      r3, #0
003e0c0c str      r3, [r7, #0xe28]
003e0c10 str      r2, [r7, #0xe24]
003e0c14 str      r2, [r7, #0xe20]
003e0c18 str      r3, [r7, #0xe1c]
003e0c1c mov      r0, r7
003e0c20 mov      r1, #1
003e0c24 bl       #0x3e0810 ; _ZN14CharProperties16RecalcPropertiesEb
003e0c28 add      sp, sp, #0x34
003e0c2c pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
003e0c30 ldr      r3, [sl, #4]
003e0c34 ldr      r1, [r3, #0xc]
003e0c38 cmp      r1, sl
003e0c3c bne      #0x3e0c58
003e0c40 mov      sl, r3
003e0c44 ldr      r3, [r3, #4]
003e0c48 ldr      r2, [r3, #0xc]
003e0c4c cmp      r2, sl
003e0c50 beq      #0x3e0c40
003e0c54 ldr      r2, [sl, #0xc]
003e0c58 cmp      r3, r2
003e0c5c movne    sl, r3
003e0c60 b        #0x3e0b30
003e0c64 subseq   r3, fp, r8, lsl #31
003e0c68 andeq    r1, r0, r8, lsl #22
