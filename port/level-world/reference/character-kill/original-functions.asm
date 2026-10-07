
# _ZNK11Application15GetCurrentLevelEv
0031f594: ldr      r3, [pc, #0x10]
0031f598: ldr      r2, [pc, #0x10]
0031f59c: add      r3, pc, r3
0031f5a0: ldr      r2, [r3, r2]
0031f5a4: ldr      r0, [r2]
0031f5a8: bx       lr

# _ZNK6CharAI16AI_GetAggroEntryEiRP9CharacterRf
003d72d0: push     {r4, r5, r6, r7, lr}
003d72d4: mov      r5, r1
003d72d8: sub      sp, sp, #0x1c
003d72dc: mov      r1, r0
003d72e0: mov      r6, r2
003d72e4: mov      r0, sp
003d72e8: add      r2, r1, #0x7c
003d72ec: mov      r7, r3
003d72f0: bl       #0x3d7208
003d72f4: ldr      r2, [sp, #8]
003d72f8: mov      r4, sp
003d72fc: mov      r1, #0
003d7300: cmp      r2, r4
003d7304: beq      #0x3d7370
003d7308: cmp      r1, r5
003d730c: beq      #0x3d7398
003d7310: ldr      r0, [r2, #0xc]
003d7314: add      r1, r1, #1
003d7318: cmp      r0, #0
003d731c: beq      #0x3d7338
003d7320: mov      r2, r0
003d7324: ldr      r3, [r2, #8]
003d7328: cmp      r3, #0
003d732c: beq      #0x3d7300
003d7330: mov      r2, r3
003d7334: b        #0x3d7324
003d7338: ldr      r3, [r2, #4]
003d733c: ldr      ip, [r3, #0xc]
003d7340: cmp      r2, ip
003d7344: bne      #0x3d7360
003d7348: mov      r2, r3
003d734c: ldr      r3, [r3, #4]
003d7350: ldr      r0, [r3, #0xc]
003d7354: cmp      r0, r2
003d7358: beq      #0x3d7348
003d735c: ldr      r0, [r2, #0xc]
003d7360: cmp      r0, r3
003d7364: movne    r2, r3
003d7368: cmp      r2, r4
003d736c: bne      #0x3d7308
003d7370: mov      r3, #0
003d7374: str      r3, [r6]
003d7378: ldr      r3, [sp, #0x10]
003d737c: cmp      r3, #0
003d7380: beq      #0x3d7390
003d7384: mov      r0, sp
003d7388: ldr      r1, [sp, #4]
003d738c: bl       #0x3d5d64
003d7390: add      sp, sp, #0x1c
003d7394: pop      {r4, r5, r6, r7, pc}
003d7398: ldr      r3, [r2, #0x14]
003d739c: str      r3, [r6]
003d73a0: ldr      r3, [r2, #0x10]
003d73a4: str      r3, [r7]
003d73a8: b        #0x3d7378

# _ZN14CharProperties12PROPS_AddIntEii
003e0798: lsl      r2, r2, #8
003e079c: b        #0x3e0708

# _ZN9Character10RaiseEventEiPv
003a4d5c: cmp      r1, #0x36
003a4d60: beq      #0x3a4d6c
003a4d64: add      r0, r0, #0x3c8
003a4d68: b        #0x3cbb34
003a4d6c: add      r0, r0, #0x560
003a4d70: mov      r1, r2
003a4d74: b        #0x3e123c

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

# _ZN9Character9Ctrl_KillEP10GameObjectb
003ad528: push     {r4, r5, r6, lr}
003ad52c: ldr      r3, [r0]
003ad530: mov      r4, r0
003ad534: mov      r5, r1
003ad538: mov      r6, r2
003ad53c: mov      lr, pc
003ad540: ldr      pc, [r3, #0x34]
003ad544: cmp      r0, #0
003ad548: beq      #0x3ad550
003ad54c: pop      {r4, r5, r6, pc}
003ad550: mov      r2, r6
003ad554: mov      r1, r5
003ad558: mov      r0, r4
003ad55c: bl       #0x3a5b18
003ad560: mov      r0, r4
003ad564: mov      r2, r5
003ad568: mov      r1, #2
003ad56c: pop      {r4, r5, r6, lr}
003ad570: b        #0x3a4d5c

# _ZNK9Character8DropLootEP10GameObject
003a5ae4: push     {r4, r5, lr}
003a5ae8: mov      r4, r1
003a5aec: sub      sp, sp, #0xc
003a5af0: mov      r5, r0
003a5af4: bl       #0x3a2fcc
003a5af8: mov      ip, #0
003a5afc: mov      r1, r5
003a5b00: mov      r2, r4
003a5b04: mvn      r3, #0
003a5b08: str      ip, [sp]
003a5b0c: bl       #0x3ecba0
003a5b10: add      sp, sp, #0xc
003a5b14: pop      {r4, r5, pc}

# _ZNK6CharAI16AI_GetAggroCountEv
003d4a10: ldr      r0, [r0, #0x8c]
003d4a14: bx       lr

# _ZN13PlayerManager13IsLocalPlayerEPK9Character
0036effc: subs     r3, r1, #0
0036f000: push     {r4, lr}
0036f004: beq      #0x36f020
0036f008: mov      r2, #0
0036f00c: bl       #0x36eea8
0036f010: ldr      r3, [r0]
0036f014: mov      lr, pc
0036f018: ldr      pc, [r3, #0x50]
0036f01c: pop      {r4, pc}
0036f020: mov      r0, r3
0036f024: pop      {r4, pc}

# _ZN9Character12DistributeXPEPS_S0_
003bf828: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
003bf82c: ldr      r4, [pc, #0x73c]
003bf830: ldr      r2, [pc, #0x73c]
003bf834: sub      sp, sp, #0x174
003bf838: add      r4, pc, r4
003bf83c: ldr      r3, [r4, r2]
003bf840: cmp      r1, #0
003bf844: str      r2, [sp, #0x1c]
003bf848: ldr      r3, [r3]
003bf84c: str      r1, [sp, #0x14]
003bf850: str      r0, [sp, #0xc]
003bf854: str      r3, [sp, #0x16c]
003bf858: beq      #0x3bfee0
003bf85c: ldr      r3, [pc, #0x714]
003bf860: ldr      ip, [sp, #0x14]
003bf864: mov      r2, #0x23
003bf868: ldr      r3, [r4, r3]
003bf86c: add      r1, ip, #0xff0
003bf870: add      r0, ip, #0x560
003bf874: ldr      r3, [r3]
003bf878: add      r1, r1, #4
003bf87c: ldr      sb, [pc, #0x6f8]
003bf880: str      r3, [sp, #0x20]
003bf884: bl       #0x3dedb4
003bf888: asr      r0, r0, #8
003bf88c: bl       #0x30e964
003bf890: mov      r1, #0
003bf894: str      r0, [sp, #0x10]
003bf898: bl       #0x30e2f8
003bf89c: ldr      r5, [r4, sb]
003bf8a0: cmp      r0, #0
003bf8a4: moveq    r2, #0
003bf8a8: mov      r0, r5
003bf8ac: streq    r2, [sp, #0x10]
003bf8b0: bl       #0x337888
003bf8b4: ldr      r1, [pc, #0x6c4]
003bf8b8: add      r8, sp, #0x154
003bf8bc: add      r2, sp, #0x78
003bf8c0: mov      r0, r8
003bf8c4: add      r1, pc, r1
003bf8c8: ldr      r6, [pc, #0x6b4]
003bf8cc: bl       #0x3140ec
003bf8d0: mov      r1, r8
003bf8d4: mov      r0, r5
003bf8d8: bl       #0x337a88
003bf8dc: mov      r0, r8
003bf8e0: bl       #0x318254
003bf8e4: add      r7, sp, #0x13c
003bf8e8: add      r6, pc, r6
003bf8ec: mov      r0, r5
003bf8f0: bl       #0x337888
003bf8f4: add      r2, sp, #0x74
003bf8f8: mov      r0, r7
003bf8fc: mov      r1, r6
003bf900: bl       #0x3140ec
003bf904: mov      r1, r7
003bf908: mov      r0, r5
003bf90c: bl       #0x337a88
003bf910: mov      r0, r7
003bf914: bl       #0x318254
003bf918: mov      r0, r5
003bf91c: bl       #0x337888
003bf920: ldr      r3, [pc, #0x660]
003bf924: add      r7, sp, #0x124
003bf928: add      r2, sp, #0x70
003bf92c: mov      r1, r6
003bf930: mov      r0, r7
003bf934: str      r3, [sp, #0x2c]
003bf938: bl       #0x3140ec
003bf93c: mov      r1, r7
003bf940: mov      r0, r5
003bf944: bl       #0x337a88
003bf948: mov      r0, r7
003bf94c: bl       #0x318254
003bf950: ldr      ip, [sp, #0x2c]
003bf954: ldr      r3, [r4, ip]
003bf958: ldr      r8, [r3, #0x40]
003bf95c: ldr      r7, [r8, #0x6c4]
003bf960: cmp      r7, #4
003bf964: bgt      #0x3bfcec
003bf968: ldr      r2, [sp, #0x20]
003bf96c: cmp      r7, #0
003bf970: ldr      r2, [r2, #0xa0]
003bf974: str      r2, [sp, #0x24]
003bf978: ble      #0x3bfd20
003bf97c: ldr      r3, [pc, #0x608]
003bf980: mov      r6, #0
003bf984: str      r6, [sp, #0x18]
003bf988: str      r3, [sp, #0x30]
003bf98c: ldr      r3, [pc, #0x5fc]
003bf990: mov      r5, r6
003bf994: add      r3, pc, r3
003bf998: str      r3, [sp, #0x28]
003bf99c: ldr      r3, [pc, #0x5f0]
003bf9a0: add      r3, pc, r3
003bf9a4: str      r3, [sp, #0x34]
003bf9a8: ldr      r3, [pc, #0x5e8]
003bf9ac: add      r3, pc, r3
003bf9b0: str      r3, [sp, #0x38]
003bf9b4: ldr      r3, [pc, #0x5e0]
003bf9b8: add      r3, pc, r3
003bf9bc: str      r3, [sp, #0x3c]
003bf9c0: mov      r0, r8
003bf9c4: mov      r1, r5
003bf9c8: mov      r2, #1
003bf9cc: bl       #0x36e744
003bf9d0: ldr      sl, [r0, #0x660]
003bf9d4: cmp      sl, #0
003bf9d8: beq      #0x3bfde4
003bf9dc: mov      r1, sl
003bf9e0: ldr      r2, [sp, #0x14]
003bf9e4: ldr      r0, [sp, #0x10]
003bf9e8: bl       #0x3bd918
003bf9ec: add      fp, sp, #0x44
003bf9f0: mov      r1, #0
003bf9f4: str      r0, [fp, r6]
003bf9f8: bl       #0x30e4b4
003bf9fc: cmp      r0, #0
003bfa00: beq      #0x3bfad8
003bfa04: ldr      r2, [sp, #0xc]
003bfa08: cmp      r2, #0
003bfa0c: beq      #0x3bfe78
003bfa10: ldr      r1, [r2, #0x160]
003bfa14: ldr      r0, [sl, #0x160]
003bfa18: bl       #0x30e3ac
003bfa1c: ldr      ip, [sp, #0xc]
003bfa20: mov      r3, r0
003bfa24: ldr      r0, [sl, #0x164]
003bfa28: ldr      r1, [ip, #0x164]
003bfa2c: str      r3, [sp, #8]
003bfa30: bl       #0x30e3ac
003bfa34: ldr      r3, [sp, #8]
003bfa38: mov      r2, r0
003bfa3c: str      r2, [sp, #8]
003bfa40: mov      r1, r3
003bfa44: mov      r0, r3
003bfa48: bl       #0x30ed6c
003bfa4c: ldr      r2, [sp, #8]
003bfa50: mov      r3, r0
003bfa54: str      r3, [sp, #8]
003bfa58: mov      r1, r2
003bfa5c: mov      r0, r2
003bfa60: bl       #0x30ed6c
003bfa64: ldr      r3, [sp, #8]
003bfa68: mov      r1, r0
003bfa6c: mov      r0, r3
003bfa70: bl       #0x30eba4
003bfa74: bl       #0x30e124
003bfa78: ldr      r2, [sp, #0xc]
003bfa7c: cmp      r2, sl
003bfa80: beq      #0x3bfa98
003bfa84: mov      r1, r0
003bfa88: ldr      r0, [sp, #0x24]
003bfa8c: bl       #0x30e4b4
003bfa90: cmp      r0, #0
003bfa94: beq      #0x3bfe30
003bfa98: ldr      fp, [r4, sb]
003bfa9c: ldr      r2, [sp, #0x18]
003bfaa0: add      sl, sp, #0x10c
003bfaa4: mov      r0, fp
003bfaa8: add      r2, r2, #1
003bfaac: str      r2, [sp, #0x18]
003bfab0: bl       #0x337888
003bfab4: add      r2, sp, #0x6c
003bfab8: ldr      r1, [sp, #0x28]
003bfabc: mov      r0, sl
003bfac0: bl       #0x3140ec
003bfac4: mov      r0, fp
003bfac8: mov      r1, sl
003bfacc: bl       #0x337a88
003bfad0: mov      r0, sl
003bfad4: bl       #0x318254
003bfad8: add      r5, r5, #1
003bfadc: cmp      r5, r7
003bfae0: add      r6, r6, #4
003bfae4: bne      #0x3bf9c0
003bfae8: ldr      r3, [sp, #0x18]
003bfaec: cmp      r3, #0
003bfaf0: beq      #0x3bfd20
003bfaf4: sub      r0, r3, #1
003bfaf8: bl       #0x30e964
003bfafc: ldr      ip, [sp, #0x20]
003bfb00: add      r6, sp, #0xdc
003bfb04: ldr      fp, [pc, #0x494]
003bfb08: ldr      r1, [ip, #0xa4]
003bfb0c: bl       #0x30ed6c
003bfb10: ldr      sl, [r4, sb]
003bfb14: str      r0, [sp, #0xc]
003bfb18: mov      r5, #0
003bfb1c: mov      r0, sl
003bfb20: bl       #0x337888
003bfb24: ldr      r1, [pc, #0x478]
003bfb28: add      r2, sp, #0x64
003bfb2c: mov      r0, r6
003bfb30: add      r1, pc, r1
003bfb34: bl       #0x3140ec
003bfb38: mov      r0, sl
003bfb3c: mov      r1, r6
003bfb40: bl       #0x337a88
003bfb44: mov      r0, r6
003bfb48: bl       #0x318254
003bfb4c: ldr      r3, [pc, #0x454]
003bfb50: add      r2, sp, #0x44
003bfb54: add      fp, pc, fp
003bfb58: add      r3, pc, r3
003bfb5c: str      r3, [sp, #0x18]
003bfb60: ldr      r3, [pc, #0x444]
003bfb64: str      r2, [sp, #0x10]
003bfb68: mov      sl, r7
003bfb6c: add      r3, pc, r3
003bfb70: str      r3, [sp, #0x20]
003bfb74: ldr      r3, [pc, #0x434]
003bfb78: add      r3, pc, r3
003bfb7c: str      r3, [sp, #0x24]
003bfb80: b        #0x3bfbc4
003bfb84: ldr      r7, [r4, sb]
003bfb88: add      r6, sp, #0xc4
003bfb8c: mov      r0, r7
003bfb90: bl       #0x337888
003bfb94: add      r2, sp, #0x60
003bfb98: mov      r1, fp
003bfb9c: mov      r0, r6
003bfba0: bl       #0x3140ec
003bfba4: mov      r0, r7
003bfba8: mov      r1, r6
003bfbac: bl       #0x337a88
003bfbb0: mov      r0, r6
003bfbb4: bl       #0x318254
003bfbb8: add      r5, r5, #1
003bfbbc: cmp      r5, sl
003bfbc0: beq      #0x3bfd58
003bfbc4: mov      r0, r8
003bfbc8: mov      r1, r5
003bfbcc: mov      r2, #1
003bfbd0: bl       #0x36e744
003bfbd4: ldr      r6, [r0, #0x660]
003bfbd8: cmp      r6, #0
003bfbdc: beq      #0x3bfbb8
003bfbe0: mov      r0, #0x42000000
003bfbe4: ldr      r1, [sp, #0xc]
003bfbe8: add      r0, r0, #0xc80000
003bfbec: bl       #0x30e3ac
003bfbf0: ldr      r3, [sp, #0x10]
003bfbf4: ldr      r1, [r3, r5, lsl #2]
003bfbf8: bl       #0x30ed6c
003bfbfc: mov      r1, #0x42000000
003bfc00: add      r1, r1, #0xc80000
003bfc04: bl       #0x30ec94
003bfc08: mov      r1, #0
003bfc0c: mov      r7, r0
003bfc10: bl       #0x30e4b4
003bfc14: cmp      r0, #0
003bfc18: beq      #0x3bfb84
003bfc1c: mov      r1, #0x3f800000
003bfc20: mov      r0, r7
003bfc24: bl       #0x30eba4
003bfc28: bl       #0x30e4cc
003bfc2c: lsl      r7, r0, #8
003bfc30: mov      r1, r7
003bfc34: mov      r0, r6
003bfc38: mov      r2, #1
003bfc3c: bl       #0x3bf498
003bfc40: cmp      r0, #0
003bfc44: beq      #0x3bfb84
003bfc48: ldr      ip, [sp, #0x2c]
003bfc4c: mov      r1, r6
003bfc50: ldr      ip, [r4, ip]
003bfc54: ldr      r0, [ip, #0x40]
003bfc58: str      ip, [sp, #0x28]
003bfc5c: bl       #0x36effc
003bfc60: cmp      r0, #0
003bfc64: beq      #0x3bfb84
003bfc68: mov      r0, r6
003bfc6c: bl       #0x3bb918
003bfc70: mov      r3, r0
003bfc74: ldr      r0, [sp, #0x28]
003bfc78: str      r3, [sp, #8]
003bfc7c: bl       #0x31f594
003bfc80: ldr      r3, [sp, #8]
003bfc84: ldr      r2, [r0, #0x118]
003bfc88: cmp      r3, r2
003bfc8c: movlt    r7, #0x100
003bfc90: bl       #0x413e90
003bfc94: mov      r1, r7
003bfc98: mov      r3, r0
003bfc9c: add      r0, r6, #0x560
003bfca0: str      r3, [sp, #8]
003bfca4: bl       #0x3de7ec
003bfca8: ldr      r3, [sp, #8]
003bfcac: mov      r7, r0
003bfcb0: ldr      r1, [sp, #0x18]
003bfcb4: mov      r0, r3
003bfcb8: bl       #0x414678
003bfcbc: ldr      r2, [sp, #0x28]
003bfcc0: mov      r6, r0
003bfcc4: ldr      r1, [sp, #0x20]
003bfcc8: ldr      r0, [r2, #0x2c]
003bfccc: ldr      r2, [sp, #0x24]
003bfcd0: bl       #0x4c4bdc
003bfcd4: asr      r1, r7, #8
003bfcd8: mov      r3, r0
003bfcdc: mov      r2, r6
003bfce0: ldr      r0, [sp, #0x14]
003bfce4: bl       #0x3af0c8
003bfce8: b        #0x3bfb84
003bfcec: ldr      r3, [pc, #0x298]
003bfcf0: ldr      r3, [r4, r3]
003bfcf4: ldr      r3, [r3]
003bfcf8: cmp      r3, #2
003bfcfc: moveq    r3, #0
003bfd00: streq    r3, [r3]
003bfd04: beq      #0x3bfd10
003bfd08: cmp      r3, #1
003bfd0c: beq      #0x3bff38
003bfd10: ldr      r3, [sp, #0x20]
003bfd14: ldr      r3, [r3, #0xa0]
003bfd18: str      r3, [sp, #0x24]
003bfd1c: b        #0x3bf97c
003bfd20: ldr      r6, [r4, sb]
003bfd24: add      r5, sp, #0xac
003bfd28: mov      r0, r6
003bfd2c: bl       #0x337888
003bfd30: ldr      r1, [pc, #0x27c]
003bfd34: add      r2, sp, #0x5c
003bfd38: mov      r0, r5
003bfd3c: add      r1, pc, r1
003bfd40: bl       #0x3140ec
003bfd44: mov      r0, r6
003bfd48: mov      r1, r5
003bfd4c: bl       #0x337a88
003bfd50: mov      r0, r5
003bfd54: bl       #0x318254
003bfd58: ldr      r5, [r4, sb]
003bfd5c: ldr      r6, [pc, #0x254]
003bfd60: add      r7, sp, #0x94
003bfd64: mov      r0, r5
003bfd68: add      r6, pc, r6
003bfd6c: bl       #0x337888
003bfd70: add      r2, sp, #0x58
003bfd74: mov      r0, r7
003bfd78: mov      r1, r6
003bfd7c: bl       #0x3140ec
003bfd80: mov      r1, r7
003bfd84: mov      r0, r5
003bfd88: bl       #0x337a88
003bfd8c: mov      r0, r7
003bfd90: bl       #0x318254
003bfd94: add      r7, sp, #0x7c
003bfd98: mov      r0, r5
003bfd9c: bl       #0x337888
003bfda0: mov      r1, r6
003bfda4: add      r2, sp, #0x54
003bfda8: mov      r0, r7
003bfdac: bl       #0x3140ec
003bfdb0: mov      r0, r5
003bfdb4: mov      r1, r7
003bfdb8: bl       #0x337a88
003bfdbc: mov      r0, r7
003bfdc0: bl       #0x318254
003bfdc4: ldr      ip, [sp, #0x1c]
003bfdc8: ldr      r2, [sp, #0x16c]
003bfdcc: ldr      r3, [r4, ip]
003bfdd0: ldr      r3, [r3]
003bfdd4: cmp      r2, r3
003bfdd8: bne      #0x3bff6c
003bfddc: add      sp, sp, #0x174
003bfde0: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
003bfde4: ldr      ip, [sp, #0x30]
003bfde8: ldr      r3, [r4, ip]
003bfdec: ldr      r3, [r3]
003bfdf0: cmp      r3, #2
003bfdf4: streq    sl, [sl]
003bfdf8: beq      #0x3bfad8
003bfdfc: cmp      r3, #1
003bfe00: bne      #0x3bfad8
003bfe04: ldr      r0, [pc, #0x1b0]
003bfe08: ldr      r3, [pc, #0x1b0]
003bfe0c: movw     ip, #0x103
003bfe10: ldr      r0, [r4, r0]
003bfe14: add      r3, pc, r3
003bfe18: ldr      r1, [sp, #0x38]
003bfe1c: ldr      r2, [sp, #0x3c]
003bfe20: add      r0, r0, #0xa8
003bfe24: str      ip, [sp]
003bfe28: bl       #0x30e004
003bfe2c: b        #0x3bfad8
003bfe30: ldr      r3, [r4, sb]
003bfe34: mov      r2, #0
003bfe38: add      sl, sp, #0xf4
003bfe3c: mov      r0, r3
003bfe40: str      r2, [fp, r6]
003bfe44: str      r3, [sp, #8]
003bfe48: bl       #0x337888
003bfe4c: add      r2, sp, #0x68
003bfe50: ldr      r1, [sp, #0x34]
003bfe54: mov      r0, sl
003bfe58: bl       #0x3140ec
003bfe5c: ldr      r3, [sp, #8]
003bfe60: mov      r1, sl
003bfe64: mov      r0, r3
003bfe68: bl       #0x337a88
003bfe6c: mov      r0, sl
003bfe70: bl       #0x318254
003bfe74: b        #0x3bfad8
003bfe78: ldr      r3, [sp, #0x14]
003bfe7c: ldr      r0, [sl, #0x160]
003bfe80: ldr      r1, [r3, #0x160]
003bfe84: bl       #0x30e3ac
003bfe88: ldr      ip, [sp, #0x14]
003bfe8c: mov      r3, r0
003bfe90: ldr      r0, [sl, #0x164]
003bfe94: ldr      r1, [ip, #0x164]
003bfe98: str      r3, [sp, #8]
003bfe9c: bl       #0x30e3ac
003bfea0: ldr      r3, [sp, #8]
003bfea4: mov      r2, r0
003bfea8: str      r2, [sp, #8]
003bfeac: mov      r1, r3
003bfeb0: mov      r0, r3
003bfeb4: bl       #0x30ed6c
003bfeb8: ldr      r2, [sp, #8]
003bfebc: mov      sl, r0
003bfec0: mov      r1, r2
003bfec4: mov      r0, r2
003bfec8: bl       #0x30ed6c
003bfecc: mov      r1, r0
003bfed0: mov      r0, sl
003bfed4: bl       #0x30eba4
003bfed8: bl       #0x30e124
003bfedc: b        #0x3bfa84
003bfee0: ldr      r3, [pc, #0xa4]
003bfee4: ldr      r3, [r4, r3]
003bfee8: ldr      r3, [r3]
003bfeec: cmp      r3, #2
003bfef0: moveq    r3, r1
003bfef4: streq    r3, [r3]
003bfef8: beq      #0x3bfdc4
003bfefc: cmp      r3, #1
003bff00: bne      #0x3bfdc4
003bff04: ldr      r0, [pc, #0xb0]
003bff08: ldr      r1, [pc, #0xb4]
003bff0c: ldr      r2, [pc, #0xb4]
003bff10: ldr      r0, [r4, r0]
003bff14: ldr      r3, [pc, #0xb0]
003bff18: mov      ip, #0xdb
003bff1c: add      r1, pc, r1
003bff20: add      r2, pc, r2
003bff24: add      r3, pc, r3
003bff28: add      r0, r0, #0xa8
003bff2c: str      ip, [sp]
003bff30: bl       #0x30e004
003bff34: b        #0x3bfdc4
003bff38: ldr      r0, [pc, #0x7c]
003bff3c: ldr      r1, [pc, #0x8c]
003bff40: ldr      r2, [pc, #0x8c]
003bff44: ldr      r0, [r4, r0]
003bff48: ldr      r3, [pc, #0x88]
003bff4c: mov      ip, #0xf7
003bff50: add      r1, pc, r1
003bff54: add      r2, pc, r2
003bff58: add      r3, pc, r3
003bff5c: add      r0, r0, #0xa8
003bff60: str      ip, [sp]
003bff64: bl       #0x30e004
003bff68: b        #0x3bfd10
003bff6c: bl       #0x30e310
003bff70: subseq   r5, sp, r8, asr r2
003bff74: andeq    r4, r0, ip, lsr #1
003bff78: andeq    r3, r0, r8, asr #5
003bff7c: andeq    r0, r0, r4, lsl #17
003bff80: subseq   r5, r0, ip, lsl #4
003bff84: subseq   r5, r0, r0, ror r0
003bff88: strdeq   r3, r4, [r0], -r4
003bff8c: andeq    r3, r0, r0, asr #19
003bff90: subseq   r4, r0, r4, asr #31
003bff94: ldrheq   r4, [r0], #-0xf8
003bff98: subeq    lr, pc, ip, lsr #20
003bff9c: subseq   r5, r0, r0, ror r1
003bffa0: subseq   r4, r0, r4, lsl #28
003bffa4: subseq   r4, r0, r8, lsr #28
003bffa8: subseq   r4, r0, r0, ror #31
003bffac: subseq   r3, r0, r4, lsl #23
003bffb0: ldrsbeq  r4, [r0], #-0xf0
003bffb4: subseq   r4, r0, ip, lsl ip
003bffb8: ldrsheq  r4, [r0], #-0xb0
003bffbc: andeq    r1, r0, r0, asr #19
003bffc0: subseq   r4, r0, ip, ror #21
003bffc4: strheq   lr, [pc], #-0x4c
003bffc8: subseq   r4, r0, r0, lsr #23
003bffcc: ldrsbeq  r4, [r0], #-0x9c
003bffd0: subeq    lr, pc, r8, lsl #9

# _ZN9Character4KillEP10GameObjectb
003a5b18: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
003a5b1c: sub      sp, sp, #0xb4
003a5b20: ldr      r3, [r0]
003a5b24: mov      r6, r2
003a5b28: mov      r4, r0
003a5b2c: mov      r7, r1
003a5b30: mov      lr, pc
003a5b34: ldr      pc, [r3, #0x34]
003a5b38: ldr      r5, [pc, #0x664]
003a5b3c: subs     r2, r0, #0
003a5b40: add      r5, pc, r5
003a5b44: beq      #0x3a5b50
003a5b48: add      sp, sp, #0xb4
003a5b4c: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
003a5b50: movw     r3, #0x1449
003a5b54: mov      sl, #1
003a5b58: add      r8, r4, #0x560
003a5b5c: strb     sl, [r4, r3]
003a5b60: mov      r0, r8
003a5b64: mov      r1, #0x24
003a5b68: bl       #0x3e07a0
003a5b6c: ldr      r3, [r4]
003a5b70: mov      r0, r4
003a5b74: mov      lr, pc
003a5b78: ldr      pc, [r3, #0x28]
003a5b7c: cmp      r0, #0
003a5b80: beq      #0x3a5be0
003a5b84: cmp      r6, #0
003a5b88: bne      #0x3a5be0
003a5b8c: ldr      sb, [pc, #0x614]
003a5b90: mov      r2, sl
003a5b94: mov      r0, r8
003a5b98: mov      r1, #0x19
003a5b9c: bl       #0x3e0798
003a5ba0: ldr      r3, [r5, sb]
003a5ba4: mov      r1, r4
003a5ba8: ldr      r0, [r3, #0x40]
003a5bac: bl       #0x36effc
003a5bb0: cmp      r0, #0
003a5bb4: bne      #0x3a602c
003a5bb8: bl       #0x7fd794
003a5bbc: ldrb     r3, [r0, #5]
003a5bc0: cmp      r3, #0
003a5bc4: beq      #0x3a5b48
003a5bc8: ldr      r3, [r5, sb]
003a5bcc: mov      r1, #0
003a5bd0: mov      r2, #1
003a5bd4: ldr      r0, [r3, #0x40]
003a5bd8: bl       #0x36e478
003a5bdc: b        #0x3a5b48
003a5be0: ldr      sb, [pc, #0x5c0]
003a5be4: ldr      r0, [r5, sb]
003a5be8: bl       #0x31f594
003a5bec: ldr      r3, [r0, #0x150]
003a5bf0: cmp      r3, #0
003a5bf4: beq      #0x3a601c
003a5bf8: cmp      r7, #0
003a5bfc: beq      #0x3a609c
003a5c00: cmp      r6, #0
003a5c04: bne      #0x3a5b48
003a5c08: movw     r3, #0x144c
003a5c0c: str      r7, [r4, r3]
003a5c10: ldr      r3, [pc, #0x594]
003a5c14: ldr      r2, [pc, #0x594]
003a5c18: cmp      r7, r4
003a5c1c: movne    fp, #0
003a5c20: moveq    fp, #1
003a5c24: add      r3, pc, r3
003a5c28: str      r3, [sp, #0x24]
003a5c2c: ldr      r3, [pc, #0x580]
003a5c30: add      r8, r4, #0x3c8
003a5c34: str      r2, [sp, #0x14]
003a5c38: add      r3, pc, r3
003a5c3c: str      r3, [sp, #0x20]
003a5c40: ldr      r3, [pc, #0x570]
003a5c44: mov      sl, r7
003a5c48: add      r3, pc, r3
003a5c4c: str      r3, [sp, #0x1c]
003a5c50: ldr      r3, [pc, #0x564]
003a5c54: add      r3, pc, r3
003a5c58: str      r3, [sp, #0x18]
003a5c5c: mov      r0, r8
003a5c60: bl       #0x3d4a10
003a5c64: cmp      r6, r0
003a5c68: bge      #0x3a5de8
003a5c6c: mov      ip, #0
003a5c70: str      ip, [sp, #0xac]
003a5c74: mov      r0, r8
003a5c78: mov      ip, #0
003a5c7c: mov      r1, r6
003a5c80: add      r2, sp, #0xac
003a5c84: add      r3, sp, #0xa8
003a5c88: str      ip, [sp, #0xa8]
003a5c8c: bl       #0x3d72d0
003a5c90: ldr      r7, [sp, #0xac]
003a5c94: cmp      r7, #0
003a5c98: beq      #0x3a5d40
003a5c9c: movw     r3, #0x14d4
003a5ca0: ldr      r0, [r7, r3]
003a5ca4: cmp      r0, #0
003a5ca8: strne    r0, [sp, #0xac]
003a5cac: beq      #0x3a5de0
003a5cb0: mov      r1, #4
003a5cb4: mov      r2, r4
003a5cb8: bl       #0x3a4d5c
003a5cbc: ldr      r3, [sp, #0xac]
003a5cc0: cmp      r3, #0
003a5cc4: beq      #0x3a5d40
003a5cc8: mov      r0, r3
003a5ccc: ldr      r3, [r3]
003a5cd0: mov      lr, pc
003a5cd4: ldr      pc, [r3, #0x28]
003a5cd8: cmp      r0, #0
003a5cdc: beq      #0x3a5d40
003a5ce0: ldr      r0, [sp, #0xac]
003a5ce4: movw     r3, #0x14a4
003a5ce8: mov      r1, #0x17
003a5cec: ldr      r2, [r0, r3]
003a5cf0: cmp      r4, r2
003a5cf4: moveq    r2, #0
003a5cf8: streq    r2, [r0, r3]
003a5cfc: ldreq    r0, [sp, #0xac]
003a5d00: mov      r2, #1
003a5d04: add      r0, r0, #0x560
003a5d08: bl       #0x3e0798
003a5d0c: ldr      r0, [sp, #0xac]
003a5d10: mov      r1, #0x18
003a5d14: mov      r2, #1
003a5d18: add      r0, r0, #0x560
003a5d1c: bl       #0x3e0798
003a5d20: ldr      r3, [r5, sb]
003a5d24: cmp      sl, r7
003a5d28: ldr      r1, [sp, #0xac]
003a5d2c: ldr      r0, [r3, #0x40]
003a5d30: moveq    fp, #1
003a5d34: bl       #0x36effc
003a5d38: cmp      r0, #0
003a5d3c: bne      #0x3a5d48
003a5d40: add      r6, r6, #1
003a5d44: b        #0x3a5c5c
003a5d48: ldr      r2, [sp, #0x14]
003a5d4c: ldr      r0, [sp, #0xac]
003a5d50: mov      r1, #0x17
003a5d54: ldr      r3, [r5, r2]
003a5d58: add      r0, r0, #0x560
003a5d5c: mov      r2, #0
003a5d60: ldr      r7, [r3]
003a5d64: bl       #0x3df6e0
003a5d68: cmp      r0, #0x64
003a5d6c: beq      #0x3a60fc
003a5d70: ldr      r0, [sp, #0xac]
003a5d74: mov      r1, #0x17
003a5d78: mov      r2, #0
003a5d7c: add      r0, r0, #0x560
003a5d80: bl       #0x3df6e0
003a5d84: cmp      r0, #0x1f4
003a5d88: beq      #0x3a6118
003a5d8c: ldr      r0, [sp, #0xac]
003a5d90: mov      r1, #0x17
003a5d94: mov      r2, #0
003a5d98: add      r0, r0, #0x560
003a5d9c: bl       #0x3df6e0
003a5da0: cmp      r0, #0x3e8
003a5da4: beq      #0x3a60a8
003a5da8: ldr      r0, [sp, #0xac]
003a5dac: mov      r1, #0x17
003a5db0: mov      r2, #0
003a5db4: add      r0, r0, #0x560
003a5db8: bl       #0x3df6e0
003a5dbc: cmp      r0, #0x7d0
003a5dc0: bne      #0x3a5d40
003a5dc4: ldr      r0, [sp, #0x24]
003a5dc8: bl       #0x3a3f70
003a5dcc: mov      r1, r0
003a5dd0: mov      r0, r7
003a5dd4: bl       #0x3813b8
003a5dd8: add      r6, r6, #1
003a5ddc: b        #0x3a5c5c
003a5de0: mov      r0, r7
003a5de4: b        #0x3a5cb0
003a5de8: cmp      fp, #0
003a5dec: mov      r7, sl
003a5df0: beq      #0x3a5e1c
003a5df4: movw     r8, #0x144c
003a5df8: ldr      r3, [r4, r8]
003a5dfc: mov      r0, r3
003a5e00: ldr      r3, [r3]
003a5e04: mov      lr, pc
003a5e08: ldr      pc, [r3, #0x24]
003a5e0c: cmp      r0, #0
003a5e10: bne      #0x3a60c4
003a5e14: mov      r1, r4
003a5e18: bl       #0x3bf828
003a5e1c: ldr      r3, [r4]
003a5e20: mov      r0, r4
003a5e24: mov      lr, pc
003a5e28: ldr      pc, [r3, #0x54]
003a5e2c: cmp      r0, #0
003a5e30: bne      #0x3a5b48
003a5e34: movw     r3, #0x14e4
003a5e38: ldrb     r3, [r4, r3]
003a5e3c: cmp      r3, #0
003a5e40: bne      #0x3a5b48
003a5e44: ldr      r0, [r5, sb]
003a5e48: bl       #0x31f594
003a5e4c: cmp      r0, #0
003a5e50: str      r0, [sp, #0x14]
003a5e54: beq      #0x3a6150
003a5e58: ldr      r8, [pc, #0x360]
003a5e5c: movw     r3, #0x13c8
003a5e60: ldr      sl, [r5, sb]
003a5e64: ldr      r2, [pc, #0x358]
003a5e68: ldrsh    ip, [r4, r3]
003a5e6c: add      r8, pc, r8
003a5e70: ldr      r0, [sl, #0x2c]
003a5e74: add      r2, pc, r2
003a5e78: mov      r1, r8
003a5e7c: ldr      fp, [r4, #0x64]
003a5e80: str      r3, [sp, #0x10]
003a5e84: str      ip, [sp, #0xc]
003a5e88: bl       #0x4c4bdc
003a5e8c: ldr      r2, [pc, #0x334]
003a5e90: ldr      ip, [sp, #0xc]
003a5e94: mov      r6, #0
003a5e98: ldr      r2, [r5, r2]
003a5e9c: mvn      sb, #0
003a5ea0: str      r0, [sp, #0x84]
003a5ea4: add      r2, r2, #8
003a5ea8: ldr      r0, [sp, #0x14]
003a5eac: add      r1, sp, #0x80
003a5eb0: str      fp, [sp, #0x8c]
003a5eb4: ldr      fp, [pc, #0x310]
003a5eb8: str      r2, [sp, #0x80]
003a5ebc: str      ip, [sp, #0x98]
003a5ec0: str      r7, [sp, #0x88]
003a5ec4: strb     r6, [sp, #0x90]
003a5ec8: strb     r6, [sp, #0x91]
003a5ecc: str      sb, [sp, #0x94]
003a5ed0: bl       #0x339090
003a5ed4: ldr      r3, [sp, #0x10]
003a5ed8: ldr      fp, [r5, fp]
003a5edc: ldr      r2, [pc, #0x2ec]
003a5ee0: ldrsh    r3, [r4, r3]
003a5ee4: add      fp, fp, #8
003a5ee8: str      fp, [sp, #0x80]
003a5eec: str      r3, [sp, #0x18]
003a5ef0: ldr      ip, [r4, #0x64]
003a5ef4: add      r2, pc, r2
003a5ef8: mov      r1, r8
003a5efc: ldr      r0, [sl, #0x2c]
003a5f00: str      ip, [sp, #0xc]
003a5f04: bl       #0x4c4bdc
003a5f08: ldr      r3, [pc, #0x2c4]
003a5f0c: ldr      ip, [sp, #0xc]
003a5f10: str      r0, [sp, #0x68]
003a5f14: ldr      r3, [r5, r3]
003a5f18: add      r1, sp, #0x64
003a5f1c: ldr      r0, [sp, #0x14]
003a5f20: add      r3, r3, #8
003a5f24: str      r3, [sp, #0x64]
003a5f28: ldr      r3, [sp, #0x18]
003a5f2c: str      ip, [sp, #0x70]
003a5f30: str      r7, [sp, #0x6c]
003a5f34: str      r3, [sp, #0x7c]
003a5f38: strb     r6, [sp, #0x74]
003a5f3c: strb     r6, [sp, #0x75]
003a5f40: str      sb, [sp, #0x78]
003a5f44: bl       #0x339090
003a5f48: movw     r2, #0x13ca
003a5f4c: ldrsh    ip, [r4, r2]
003a5f50: str      fp, [sp, #0x64]
003a5f54: cmp      ip, sb
003a5f58: beq      #0x3a5b48
003a5f5c: ldr      r2, [pc, #0x274]
003a5f60: ldr      r3, [r4, #0x64]
003a5f64: mov      r1, r8
003a5f68: ldr      r0, [sl, #0x2c]
003a5f6c: add      r2, pc, r2
003a5f70: str      r3, [sp, #0x18]
003a5f74: str      ip, [sp, #0xc]
003a5f78: bl       #0x4c4bdc
003a5f7c: ldr      r3, [pc, #0x258]
003a5f80: ldr      r2, [sp, #0x18]
003a5f84: ldr      ip, [sp, #0xc]
003a5f88: ldr      r3, [r5, r3]
003a5f8c: str      r0, [sp, #0x4c]
003a5f90: add      r1, sp, #0x48
003a5f94: ldr      r0, [sp, #0x14]
003a5f98: add      r3, r3, #8
003a5f9c: str      r2, [sp, #0x54]
003a5fa0: str      r3, [sp, #0x48]
003a5fa4: str      ip, [sp, #0x60]
003a5fa8: str      r7, [sp, #0x50]
003a5fac: strb     r6, [sp, #0x58]
003a5fb0: strb     r6, [sp, #0x59]
003a5fb4: str      sb, [sp, #0x5c]
003a5fb8: bl       #0x339090
003a5fbc: ldr      r2, [pc, #0x21c]
003a5fc0: str      fp, [sp, #0x48]
003a5fc4: mov      r1, r8
003a5fc8: movw     r3, #0x13ca
003a5fcc: ldr      r0, [sl, #0x2c]
003a5fd0: add      r2, pc, r2
003a5fd4: ldrsh    r8, [r4, r3]
003a5fd8: ldr      r4, [r4, #0x64]
003a5fdc: bl       #0x4c4bdc
003a5fe0: ldr      r3, [pc, #0x1fc]
003a5fe4: str      r0, [sp, #0x30]
003a5fe8: add      r1, sp, #0x2c
003a5fec: ldr      r3, [r5, r3]
003a5ff0: ldr      r0, [sp, #0x14]
003a5ff4: str      r7, [sp, #0x34]
003a5ff8: add      r3, r3, #8
003a5ffc: str      r4, [sp, #0x38]
003a6000: strb     r6, [sp, #0x3d]
003a6004: str      sb, [sp, #0x40]
003a6008: str      r3, [sp, #0x2c]
003a600c: str      r8, [sp, #0x44]
003a6010: strb     r6, [sp, #0x3c]
003a6014: bl       #0x339090
003a6018: b        #0x3a5b48
003a601c: mov      r0, r4
003a6020: mov      r1, r7
003a6024: bl       #0x3a5ae4
003a6028: b        #0x3a5bf8
003a602c: ldr      r3, [pc, #0x17c]
003a6030: mov      r0, r8
003a6034: mov      r1, #0x19
003a6038: ldr      r3, [r5, r3]
003a603c: mov      r2, r6
003a6040: ldr      r4, [r3]
003a6044: bl       #0x3df6e0
003a6048: cmp      r0, #0xa
003a604c: beq      #0x3a60e0
003a6050: mov      r0, r8
003a6054: mov      r1, #0x19
003a6058: mov      r2, r6
003a605c: bl       #0x3df6e0
003a6060: cmp      r0, #0x32
003a6064: beq      #0x3a6134
003a6068: mov      r0, r8
003a606c: mov      r2, r6
003a6070: mov      r1, #0x19
003a6074: bl       #0x3df6e0
003a6078: cmp      r0, #0x64
003a607c: bne      #0x3a5bb8
003a6080: ldr      r0, [pc, #0x160]
003a6084: add      r0, pc, r0
003a6088: bl       #0x3a3f70
003a608c: mov      r1, r0
003a6090: mov      r0, r4
003a6094: bl       #0x3813b8
003a6098: b        #0x3a5bb8
003a609c: cmp      r6, #0
003a60a0: bne      #0x3a5b48
003a60a4: b        #0x3a5e1c
003a60a8: ldr      r0, [sp, #0x20]
003a60ac: bl       #0x3a3f70
003a60b0: mov      r1, r0
003a60b4: mov      r0, r7
003a60b8: bl       #0x3813b8
003a60bc: add      r6, r6, #1
003a60c0: b        #0x3a5c5c
003a60c4: add      r6, sp, #0x9c
003a60c8: mov      r0, r6
003a60cc: ldr      r1, [r4, r8]
003a60d0: bl       #0x33dd2c
003a60d4: mov      r0, r6
003a60d8: bl       #0x33ff54
003a60dc: b        #0x3a5e14
003a60e0: ldr      r0, [pc, #0x104]
003a60e4: add      r0, pc, r0
003a60e8: bl       #0x3a3f70
003a60ec: mov      r1, r0
003a60f0: mov      r0, r4
003a60f4: bl       #0x3813b8
003a60f8: b        #0x3a5bb8
003a60fc: ldr      r0, [sp, #0x18]
003a6100: bl       #0x3a3f70
003a6104: mov      r1, r0
003a6108: mov      r0, r7
003a610c: bl       #0x3813b8
003a6110: add      r6, r6, #1
003a6114: b        #0x3a5c5c
003a6118: ldr      r0, [sp, #0x1c]
003a611c: bl       #0x3a3f70
003a6120: mov      r1, r0
003a6124: mov      r0, r7
003a6128: bl       #0x3813b8
003a612c: add      r6, r6, #1
003a6130: b        #0x3a5c5c
003a6134: ldr      r0, [pc, #0xb4]
003a6138: add      r0, pc, r0
003a613c: bl       #0x3a3f70
003a6140: mov      r1, r0
003a6144: mov      r0, r4
003a6148: bl       #0x3813b8
003a614c: b        #0x3a5bb8
003a6150: ldr      r3, [pc, #0x9c]
003a6154: ldr      r3, [r5, r3]
003a6158: ldr      r3, [r3]
003a615c: cmp      r3, #2
003a6160: streq    r0, [r0]
003a6164: beq      #0x3a5e58
003a6168: cmp      r3, #1
003a616c: bne      #0x3a5e58
003a6170: ldr      r0, [pc, #0x80]
003a6174: ldr      r1, [pc, #0x80]
003a6178: ldr      r2, [pc, #0x80]
003a617c: ldr      r0, [r5, r0]
003a6180: ldr      r3, [pc, #0x7c]
003a6184: movw     ip, #0x2d3
003a6188: add      r1, pc, r1
003a618c: add      r2, pc, r2
003a6190: add      r3, pc, r3
003a6194: add      r0, r0, #0xa8
003a6198: str      ip, [sp]
003a619c: bl       #0x30e004
003a61a0: b        #0x3a5e58
003a61a4: subseq   lr, lr, r0, asr pc
003a61a8: strdeq   r3, r4, [r0], -r4
003a61ac: subseq   sp, r1, ip, lsr r7
003a61b0: andeq    r1, r0, r0, ror sp
003a61b4: subseq   sp, r1, r8, lsl r7
003a61b8: ldrsheq  sp, [r1], #-0x68
003a61bc: ldrsbeq  sp, [r1], #-0x6c
003a61c0: ldrsheq  ip, [r1], #-0xac
003a61c4: ldrsheq  sp, [r1], #-0x4c
003a61c8: ldrdeq   r3, r4, [r0], -r4
003a61cc: strheq   r0, [r0], -r0
003a61d0: subseq   sp, r1, ip, lsl #9
003a61d4: strheq   r4, [r0], -r4
003a61d8: subseq   sp, r1, r4, lsr #8
003a61dc: andeq    r2, r0, r0, ror #29
003a61e0: ldrsbeq  sp, [r1], #-0x38
003a61e4: andeq    r4, r0, r4, lsr #11

# _ZN13PlayerManager14GetLocalPlayerEib
0036e478: push     {r4, lr}
0036e47c: mov      r4, r0
0036e480: bl       #0x36e2cc
0036e484: mov      r2, #0
0036e488: mov      r1, r0
0036e48c: mov      r0, r4
0036e490: pop      {r4, lr}
0036e494: b        #0x36dfb0
