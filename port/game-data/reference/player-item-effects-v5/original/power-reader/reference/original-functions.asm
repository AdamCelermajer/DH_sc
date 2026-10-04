
# _ZN6Arrays14ItemPowerTable9readNamesEP11IStreamBase
004b6630: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
004b6634: mov      r7, r0
004b6638: sub      sp, sp, #0x1c
004b663c: bl       #0x4a6ad8
004b6640: mov      r0, r7
004b6644: bl       #0x313a90
004b6648: ldr      r6, [pc, #0x16c]
004b664c: mov      r3, #1
004b6650: cmp      r3, #0
004b6654: add      r6, pc, r6
004b6658: str      r0, [sp, #0x14]
004b665c: str      r3, [sp, #0xc]
004b6660: bne      #0x4b66b0
004b6664: add      r3, sp, #0x14
004b6668: add      r2, r3, #2
004b666c: add      r3, r3, #1
004b6670: ldrb     r0, [r2, #1]
004b6674: ldrb     r1, [r3, #-1]
004b6678: cmp      r2, r3
004b667c: mov      r4, r2
004b6680: eor      r1, r0, r1
004b6684: strb     r1, [r3, #-1]
004b6688: ldrb     r0, [r2, #1]
004b668c: eor      r1, r1, r0
004b6690: strb     r1, [r2, #1]
004b6694: ldrb     r0, [r3, #-1]
004b6698: sub      r2, r2, #1
004b669c: eor      r1, r1, r0
004b66a0: strb     r1, [r3, #-1]
004b66a4: add      r3, r3, #1
004b66a8: bhi      #0x4b6670
004b66ac: ldr      r0, [sp, #0x14]
004b66b0: ldr      r3, [pc, #0x108]
004b66b4: ldr      r3, [r6, r3]
004b66b8: ldr      r3, [r3]
004b66bc: cmp      r3, r0
004b66c0: beq      #0x4b66cc
004b66c4: add      sp, sp, #0x1c
004b66c8: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
004b66cc: lsl      r0, r0, #2
004b66d0: mov      r1, #1
004b66d4: bl       #0x31056c
004b66d8: ldr      sb, [pc, #0xe4]
004b66dc: ldr      r2, [sp, #0x14]
004b66e0: ldr      r3, [r6, sb]
004b66e4: cmp      r2, #0
004b66e8: str      r0, [r3]
004b66ec: beq      #0x4b66c4
004b66f0: add      sl, sp, #0x10
004b66f4: mov      r8, #1
004b66f8: add      r1, sl, r8
004b66fc: add      r3, sl, #2
004b6700: mov      r4, #0
004b6704: stm      sp, {r1, r3}
004b6708: mov      r0, r7
004b670c: mov      r1, sl
004b6710: bl       #0x3df1a0
004b6714: cmp      r8, #0
004b6718: str      r8, [sp, #0xc]
004b671c: bne      #0x4b6760
004b6720: ldr      r3, [sp]
004b6724: ldr      r2, [sp, #4]
004b6728: ldrb     r0, [r2, #1]
004b672c: ldrb     r1, [r3, #-1]
004b6730: cmp      r2, r3
004b6734: eor      r1, r0, r1
004b6738: strb     r1, [r3, #-1]
004b673c: ldrb     r0, [r2, #1]
004b6740: eor      r1, r1, r0
004b6744: strb     r1, [r2, #1]
004b6748: ldrb     r0, [r3, #-1]
004b674c: sub      r2, r2, #1
004b6750: eor      r1, r1, r0
004b6754: strb     r1, [r3, #-1]
004b6758: add      r3, r3, #1
004b675c: bhi      #0x4b6728
004b6760: ldr      r0, [sp, #0x10]
004b6764: ldr      r5, [r6, sb]
004b6768: mov      r1, #1
004b676c: add      r0, r0, r1
004b6770: ldr      fp, [r5]
004b6774: bl       #0x31056c
004b6778: str      r0, [fp, r4, lsl #2]
004b677c: ldr      r3, [r5]
004b6780: ldr      r2, [sp, #0x10]
004b6784: mov      r0, r7
004b6788: ldr      r1, [r3, r4, lsl #2]
004b678c: mov      r3, #0
004b6790: bl       #0x317454
004b6794: ldr      r3, [r5]
004b6798: mov      r1, #0
004b679c: ldr      r2, [r3, r4, lsl #2]
004b67a0: ldr      r3, [sp, #0x10]
004b67a4: add      r4, r4, #1
004b67a8: strb     r1, [r2, r3]
004b67ac: ldr      r3, [sp, #0x14]
004b67b0: cmp      r3, r4
004b67b4: bhi      #0x4b6708
004b67b8: b        #0x4b66c4
004b67bc: subeq    lr, sp, ip, lsr r4
004b67c0: andeq    r1, r0, r8, lsl #3
004b67c4: andeq    r1, r0, ip, asr #5

# _ZN6Arrays14ItemPowerTable8finalizeEv
004a6b74: push     {r4, r5, r6, r7, r8, lr}
004a6b78: ldr      r5, [pc, #0xcc]
004a6b7c: ldr      r7, [pc, #0xcc]
004a6b80: add      r5, pc, r5
004a6b84: ldr      r3, [r5, r7]
004a6b88: ldr      r3, [r3]
004a6b8c: cmp      r3, #0
004a6b90: beq      #0x4a6c48
004a6b94: ldr      r8, [pc, #0xb8]
004a6b98: ldr      r2, [r5, r8]
004a6b9c: ldr      r2, [r2]
004a6ba0: cmp      r2, #0
004a6ba4: beq      #0x4a6bf4
004a6ba8: mov      r4, #0
004a6bac: mov      r6, r4
004a6bb0: b        #0x4a6bbc
004a6bb4: ldr      r3, [r5, r7]
004a6bb8: ldr      r3, [r3]
004a6bbc: add      r0, r3, r4
004a6bc0: ldr      r3, [r3, r4]
004a6bc4: mov      lr, pc
004a6bc8: ldr      pc, [r3, #8]
004a6bcc: ldr      r3, [r5, r8]
004a6bd0: add      r6, r6, #1
004a6bd4: add      r4, r4, #0x28
004a6bd8: ldr      r3, [r3]
004a6bdc: cmp      r3, r6
004a6be0: bhi      #0x4a6bb4
004a6be4: ldr      r3, [r5, r7]
004a6be8: ldr      r3, [r3]
004a6bec: cmp      r3, #0
004a6bf0: beq      #0x4a6c3c
004a6bf4: ldr      r2, [r3, #-4]
004a6bf8: mov      r0, #0x28
004a6bfc: mla      r0, r0, r2, r3
004a6c00: cmp      r3, r0
004a6c04: bne      #0x4a6c10
004a6c08: b        #0x4a6c34
004a6c0c: mov      r0, r4
004a6c10: sub      r4, r0, #0x28
004a6c14: ldr      r3, [r0, #-0x28]
004a6c18: mov      r0, r4
004a6c1c: mov      lr, pc
004a6c20: ldr      pc, [r3]
004a6c24: ldr      r3, [r5, r7]
004a6c28: ldr      r0, [r3]
004a6c2c: cmp      r0, r4
004a6c30: bne      #0x4a6c0c
004a6c34: sub      r0, r0, #8
004a6c38: bl       #0x310440
004a6c3c: ldr      r3, [r5, r7]
004a6c40: mov      r2, #0
004a6c44: str      r2, [r3]
004a6c48: pop      {r4, r5, r6, r7, r8, pc}
004a6c4c: subeq    sp, lr, r0, lsl pc
004a6c50: andeq    r3, r0, r8, ror #23
004a6c54: andeq    r1, r0, r8, lsl #3

# _ZN7Structs12ItemPowerRef8finalizeEv
004d573c: push     {r4, r5, r6, lr}
004d5740: ldr      r3, [r0, #0x10]
004d5744: mov      r5, r0
004d5748: cmp      r3, #0
004d574c: beq      #0x4d579c
004d5750: ldr      r0, [r3, #-4]
004d5754: add      r0, r3, r0, lsl #4
004d5758: cmp      r3, r0
004d575c: bne      #0x4d5768
004d5760: b        #0x4d5788
004d5764: mov      r0, r4
004d5768: sub      r4, r0, #0x10
004d576c: ldr      r3, [r0, #-0x10]
004d5770: mov      r0, r4
004d5774: mov      lr, pc
004d5778: ldr      pc, [r3]
004d577c: ldr      r0, [r5, #0x10]
004d5780: cmp      r0, r4
004d5784: bne      #0x4d5764
004d5788: sub      r0, r0, #8
004d578c: bl       #0x310440
004d5790: mov      r3, #0
004d5794: str      r3, [r5, #0xc]
004d5798: str      r3, [r5, #0x10]
004d579c: pop      {r4, r5, r6, pc}

# _ZN7Structs12ItemPowerRef4readEP11IStreamBase
004ecdc8: push     {r4, r5, r6, r7, lr}
004ecdcc: mov      r5, r0
004ecdd0: sub      sp, sp, #0xc
004ecdd4: mov      r0, r1
004ecdd8: mov      r6, r1
004ecddc: add      r1, r5, #4
004ecde0: bl       #0x4db9fc
004ecde4: ldr      r7, [pc, #0x364]
004ecde8: mov      r0, r6
004ecdec: add      r1, r5, #8
004ecdf0: bl       #0x459090
004ecdf4: mov      r3, #1
004ecdf8: cmp      r3, #0
004ecdfc: str      r3, [sp, #4]
004ece00: add      r7, pc, r7
004ece04: bne      #0x4ece48
004ece08: add      r3, r5, #9
004ece0c: add      r2, r5, #0xa
004ece10: ldrb     r0, [r2, #1]
004ece14: ldrb     r1, [r3, #-1]
004ece18: cmp      r3, r2
004ece1c: eor      r1, r0, r1
004ece20: strb     r1, [r3, #-1]
004ece24: ldrb     r0, [r2, #1]
004ece28: eor      r1, r1, r0
004ece2c: strb     r1, [r2, #1]
004ece30: ldrb     r0, [r3, #-1]
004ece34: sub      r2, r2, #1
004ece38: eor      r1, r1, r0
004ece3c: strb     r1, [r3, #-1]
004ece40: add      r3, r3, #1
004ece44: blo      #0x4ece10
004ece48: mov      r0, r6
004ece4c: add      r1, r5, #0xc
004ece50: bl       #0x3df1a0
004ece54: mov      r3, #1
004ece58: cmp      r3, #0
004ece5c: str      r3, [sp, #4]
004ece60: bne      #0x4ecea4
004ece64: add      r3, r5, #0xd
004ece68: add      r2, r5, #0xe
004ece6c: ldrb     r0, [r2, #1]
004ece70: ldrb     r1, [r3, #-1]
004ece74: cmp      r3, r2
004ece78: eor      r1, r0, r1
004ece7c: strb     r1, [r3, #-1]
004ece80: ldrb     r0, [r2, #1]
004ece84: eor      r1, r1, r0
004ece88: strb     r1, [r2, #1]
004ece8c: ldrb     r0, [r3, #-1]
004ece90: sub      r2, r2, #1
004ece94: eor      r1, r1, r0
004ece98: strb     r1, [r3, #-1]
004ece9c: add      r3, r3, #1
004ecea0: blo      #0x4ece6c
004ecea4: ldr      r3, [r5, #0x10]
004ecea8: cmp      r3, #0
004eceac: beq      #0x4ecef0
004eceb0: ldr      r0, [r3, #-4]
004eceb4: add      r0, r3, r0, lsl #4
004eceb8: cmp      r3, r0
004ecebc: bne      #0x4ecec8
004ecec0: b        #0x4ecee8
004ecec4: mov      r0, r4
004ecec8: sub      r4, r0, #0x10
004ececc: ldr      r3, [r0, #-0x10]
004eced0: mov      r0, r4
004eced4: mov      lr, pc
004eced8: ldr      pc, [r3]
004ecedc: ldr      r0, [r5, #0x10]
004ecee0: cmp      r0, r4
004ecee4: bne      #0x4ecec4
004ecee8: sub      r0, r0, #8
004eceec: bl       #0x310440
004ecef0: ldr      r4, [r5, #0xc]
004ecef4: mov      r1, #1
004ecef8: lsl      r0, r4, #4
004ecefc: add      r0, r0, #8
004ecf00: bl       #0x31056c
004ecf04: mov      r3, #0x10
004ecf08: cmp      r4, #0
004ecf0c: stm      r0, {r3, r4}
004ecf10: add      r3, r0, #8
004ecf14: beq      #0x4ecf3c
004ecf18: ldr      r1, [pc, #0x234]
004ecf1c: mov      r2, #0
004ecf20: ldr      r1, [r7, r1]
004ecf24: add      r1, r1, #8
004ecf28: add      r2, r2, #1
004ecf2c: cmp      r2, r4
004ecf30: str      r1, [r0, #8]
004ecf34: add      r0, r0, #0x10
004ecf38: bne      #0x4ecf28
004ecf3c: ldr      r2, [r5, #0xc]
004ecf40: str      r3, [r5, #0x10]
004ecf44: cmp      r2, #0
004ecf48: beq      #0x4ecf7c
004ecf4c: mov      r4, #0
004ecf50: b        #0x4ecf58
004ecf54: ldr      r3, [r5, #0x10]
004ecf58: add      r0, r3, r4, lsl #4
004ecf5c: mov      r1, r6
004ecf60: ldr      r3, [r3, r4, lsl #4]
004ecf64: mov      lr, pc
004ecf68: ldr      pc, [r3, #0xc]
004ecf6c: ldr      r3, [r5, #0xc]
004ecf70: add      r4, r4, #1
004ecf74: cmp      r3, r4
004ecf78: bhi      #0x4ecf54
004ecf7c: mov      r0, r6
004ecf80: add      r1, r5, #0x14
004ecf84: bl       #0x459090
004ecf88: mov      r3, #1
004ecf8c: cmp      r3, #0
004ecf90: str      r3, [sp, #4]
004ecf94: bne      #0x4ecfd8
004ecf98: add      r3, r5, #0x15
004ecf9c: add      r2, r5, #0x16
004ecfa0: ldrb     r0, [r2, #1]
004ecfa4: ldrb     r1, [r3, #-1]
004ecfa8: cmp      r3, r2
004ecfac: eor      r1, r0, r1
004ecfb0: strb     r1, [r3, #-1]
004ecfb4: ldrb     r0, [r2, #1]
004ecfb8: eor      r1, r1, r0
004ecfbc: strb     r1, [r2, #1]
004ecfc0: ldrb     r0, [r3, #-1]
004ecfc4: sub      r2, r2, #1
004ecfc8: eor      r1, r1, r0
004ecfcc: strb     r1, [r3, #-1]
004ecfd0: add      r3, r3, #1
004ecfd4: blo      #0x4ecfa0
004ecfd8: mov      r0, r6
004ecfdc: add      r1, r5, #0x18
004ecfe0: bl       #0x459090
004ecfe4: mov      r3, #1
004ecfe8: cmp      r3, #0
004ecfec: str      r3, [sp, #4]
004ecff0: bne      #0x4ed034
004ecff4: add      r3, r5, #0x19
004ecff8: add      r2, r5, #0x1a
004ecffc: ldrb     r0, [r2, #1]
004ed000: ldrb     r1, [r3, #-1]
004ed004: cmp      r3, r2
004ed008: eor      r1, r0, r1
004ed00c: strb     r1, [r3, #-1]
004ed010: ldrb     r0, [r2, #1]
004ed014: eor      r1, r1, r0
004ed018: strb     r1, [r2, #1]
004ed01c: ldrb     r0, [r3, #-1]
004ed020: sub      r2, r2, #1
004ed024: eor      r1, r1, r0
004ed028: strb     r1, [r3, #-1]
004ed02c: add      r3, r3, #1
004ed030: blo      #0x4ecffc
004ed034: mov      r0, r6
004ed038: add      r1, r5, #0x1c
004ed03c: bl       #0x459090
004ed040: mov      r3, #1
004ed044: cmp      r3, #0
004ed048: str      r3, [sp, #4]
004ed04c: bne      #0x4ed090
004ed050: add      r3, r5, #0x1d
004ed054: add      r2, r5, #0x1e
004ed058: ldrb     r0, [r2, #1]
004ed05c: ldrb     r1, [r3, #-1]
004ed060: cmp      r3, r2
004ed064: eor      r1, r0, r1
004ed068: strb     r1, [r3, #-1]
004ed06c: ldrb     r0, [r2, #1]
004ed070: eor      r1, r1, r0
004ed074: strb     r1, [r2, #1]
004ed078: ldrb     r0, [r3, #-1]
004ed07c: sub      r2, r2, #1
004ed080: eor      r1, r1, r0
004ed084: strb     r1, [r3, #-1]
004ed088: add      r3, r3, #1
004ed08c: blo      #0x4ed058
004ed090: mov      r0, r6
004ed094: add      r1, r5, #0x20
004ed098: bl       #0x459090
004ed09c: mov      r3, #1
004ed0a0: cmp      r3, #0
004ed0a4: str      r3, [sp, #4]
004ed0a8: bne      #0x4ed0ec
004ed0ac: add      r3, r5, #0x21
004ed0b0: add      r2, r5, #0x22
004ed0b4: ldrb     r0, [r2, #1]
004ed0b8: ldrb     r1, [r3, #-1]
004ed0bc: cmp      r3, r2
004ed0c0: eor      r1, r0, r1
004ed0c4: strb     r1, [r3, #-1]
004ed0c8: ldrb     r0, [r2, #1]
004ed0cc: eor      r1, r1, r0
004ed0d0: strb     r1, [r2, #1]
004ed0d4: ldrb     r0, [r3, #-1]
004ed0d8: sub      r2, r2, #1
004ed0dc: eor      r1, r1, r0
004ed0e0: strb     r1, [r3, #-1]
004ed0e4: add      r3, r3, #1
004ed0e8: blo      #0x4ed0b4
004ed0ec: mov      r0, r6
004ed0f0: add      r1, r5, #0x24
004ed0f4: bl       #0x459090
004ed0f8: mov      r3, #1
004ed0fc: cmp      r3, #0
004ed100: str      r3, [sp, #4]
004ed104: bne      #0x4ed148
004ed108: add      r3, r5, #0x26
004ed10c: add      r5, r5, #0x25
004ed110: ldrb     r1, [r3, #1]
004ed114: ldrb     r2, [r5, #-1]
004ed118: cmp      r5, r3
004ed11c: eor      r2, r1, r2
004ed120: strb     r2, [r5, #-1]
004ed124: ldrb     r1, [r3, #1]
004ed128: eor      r2, r2, r1
004ed12c: strb     r2, [r3, #1]
004ed130: ldrb     r1, [r5, #-1]
004ed134: sub      r3, r3, #1
004ed138: eor      r2, r2, r1
004ed13c: strb     r2, [r5, #-1]
004ed140: add      r5, r5, #1
004ed144: blo      #0x4ed110
004ed148: add      sp, sp, #0xc
004ed14c: pop      {r4, r5, r6, r7, pc}
004ed150: umaaleq  r7, sl, r0, ip
004ed154: strheq   r1, [r0], -r8

# _ZN6Arrays14ItemPowerTable4readEP11IStreamBase
004bab7c: push     {r4, r5, r6, r7, r8, sl, lr}
004bab80: sub      sp, sp, #0xc
004bab84: mov      sl, r0
004bab88: bl       #0x313a90
004bab8c: ldr      r6, [pc, #0x124]
004bab90: mov      r3, #1
004bab94: cmp      r3, #0
004bab98: str      r0, [sp, #4]
004bab9c: str      r3, [sp]
004baba0: add      r6, pc, r6
004baba4: bne      #0x4babec
004baba8: add      r3, sp, #4
004babac: add      r2, r3, #2
004babb0: add      r3, r3, #1
004babb4: ldrb     r0, [r2, #1]
004babb8: ldrb     r1, [r3, #-1]
004babbc: cmp      r2, r3
004babc0: eor      r1, r0, r1
004babc4: strb     r1, [r3, #-1]
004babc8: ldrb     r0, [r2, #1]
004babcc: eor      r1, r1, r0
004babd0: strb     r1, [r2, #1]
004babd4: ldrb     r0, [r3, #-1]
004babd8: sub      r2, r2, #1
004babdc: eor      r1, r1, r0
004babe0: strb     r1, [r3, #-1]
004babe4: add      r3, r3, #1
004babe8: bhi      #0x4babb4
004babec: ldr      r7, [pc, #0xc8]
004babf0: bl       #0x4a6b74
004babf4: ldr      r4, [sp, #4]
004babf8: ldr      r3, [r6, r7]
004babfc: mov      r1, #1
004bac00: add      r0, r4, r4, lsl #2
004bac04: add      r0, r0, r1
004bac08: str      r4, [r3]
004bac0c: lsl      r0, r0, #3
004bac10: bl       #0x31056c
004bac14: mov      r3, #0x28
004bac18: cmp      r4, #0
004bac1c: stm      r0, {r3, r4}
004bac20: add      r3, r0, #8
004bac24: beq      #0x4bac54
004bac28: ldr      r1, [pc, #0x90]
004bac2c: mov      r2, #0
004bac30: mov      ip, r2
004bac34: ldr      r1, [r6, r1]
004bac38: add      r1, r1, #8
004bac3c: add      r2, r2, #1
004bac40: cmp      r2, r4
004bac44: str      r1, [r0, #8]
004bac48: str      ip, [r0, #0x18]
004bac4c: add      r0, r0, #0x28
004bac50: bne      #0x4bac3c
004bac54: ldr      r2, [r6, r7]
004bac58: ldr      r8, [pc, #0x64]
004bac5c: ldr      r1, [r2]
004bac60: ldr      r2, [r6, r8]
004bac64: cmp      r1, #0
004bac68: str      r3, [r2]
004bac6c: beq      #0x4bacb0
004bac70: mov      r4, #0
004bac74: mov      r5, r4
004bac78: b        #0x4bac84
004bac7c: ldr      r3, [r6, r8]
004bac80: ldr      r3, [r3]
004bac84: add      r0, r3, r4
004bac88: mov      r1, sl
004bac8c: ldr      r3, [r3, r4]
004bac90: mov      lr, pc
004bac94: ldr      pc, [r3, #0xc]
004bac98: ldr      r3, [r6, r7]
004bac9c: add      r5, r5, #1
004baca0: add      r4, r4, #0x28
004baca4: ldr      r3, [r3]
004baca8: cmp      r3, r5
004bacac: bhi      #0x4bac7c
004bacb0: add      sp, sp, #0xc
004bacb4: pop      {r4, r5, r6, r7, r8, sl, pc}
004bacb8: strdeq   sb, sl, [sp], #-0xe0
004bacbc: andeq    r1, r0, r8, lsl #3
004bacc0: andeq    r2, r0, r8, asr #3
004bacc4: andeq    r3, r0, r8, ror #23
