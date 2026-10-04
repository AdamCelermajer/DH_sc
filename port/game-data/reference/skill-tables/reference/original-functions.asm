
# _ZN12StreamReader6readAsIbEEvP11IStreamBasePT_
004db89c: str      lr, [sp, #-4]!
004db8a0: mov      r3, #0
004db8a4: sub      sp, sp, #0xc
004db8a8: ldr      ip, [r0]
004db8ac: mov      r2, #1
004db8b0: mov      lr, pc
004db8b4: ldr      pc, [ip, #0x18]
004db8b8: ldr      r3, [pc, #0x74]
004db8bc: cmp      r0, #1
004db8c0: add      r3, pc, r3
004db8c4: beq      #0x4db8f4
004db8c8: ldr      r2, [pc, #0x68]
004db8cc: ldr      r2, [r3, r2]
004db8d0: ldr      r2, [r2]
004db8d4: cmp      r2, #2
004db8d8: moveq    r3, #0
004db8dc: streq    r3, [r3]
004db8e0: beq      #0x4db8ec
004db8e4: cmp      r2, #1
004db8e8: beq      #0x4db900
004db8ec: add      sp, sp, #0xc
004db8f0: ldm      sp!, {pc}
004db8f4: cmp      r1, #0
004db8f8: beq      #0x4db8ec
004db8fc: b        #0x4db8c8
004db900: ldr      r0, [pc, #0x34]
004db904: ldr      r1, [pc, #0x34]
004db908: ldr      r2, [pc, #0x34]
004db90c: ldr      r0, [r3, r0]
004db910: ldr      r3, [pc, #0x30]
004db914: mov      ip, #0x50
004db918: add      r1, pc, r1
004db91c: add      r2, pc, r2
004db920: add      r3, pc, r3
004db924: add      r0, r0, #0xa8
004db928: str      ip, [sp]
004db92c: bl       #0x30e004
004db930: b        #0x4db8ec
004db934: ldrdeq   sb, sl, [fp], #-0x10
004db938: andeq    r3, r0, r0, asr #19
004db93c: andeq    r1, r0, r0, asr #19
004db940: eorseq   r2, lr, r0, asr #21
004db944: eorseq   r2, lr, r4, ror #23
004db948: eorseq   r4, lr, r0, lsr #8

# _ZN6Arrays14SkillListTable9readNamesEP11IStreamBase
004b0464: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
004b0468: mov      r7, r0
004b046c: sub      sp, sp, #0x1c
004b0470: bl       #0x4a55e4
004b0474: mov      r0, r7
004b0478: bl       #0x313a90
004b047c: ldr      r6, [pc, #0x16c]
004b0480: mov      r3, #1
004b0484: cmp      r3, #0
004b0488: add      r6, pc, r6
004b048c: str      r0, [sp, #0x14]
004b0490: str      r3, [sp, #0xc]
004b0494: bne      #0x4b04e4
004b0498: add      r3, sp, #0x14
004b049c: add      r2, r3, #2
004b04a0: add      r3, r3, #1
004b04a4: ldrb     r0, [r2, #1]
004b04a8: ldrb     r1, [r3, #-1]
004b04ac: cmp      r2, r3
004b04b0: mov      r4, r2
004b04b4: eor      r1, r0, r1
004b04b8: strb     r1, [r3, #-1]
004b04bc: ldrb     r0, [r2, #1]
004b04c0: eor      r1, r1, r0
004b04c4: strb     r1, [r2, #1]
004b04c8: ldrb     r0, [r3, #-1]
004b04cc: sub      r2, r2, #1
004b04d0: eor      r1, r1, r0
004b04d4: strb     r1, [r3, #-1]
004b04d8: add      r3, r3, #1
004b04dc: bhi      #0x4b04a4
004b04e0: ldr      r0, [sp, #0x14]
004b04e4: ldr      r3, [pc, #0x108]
004b04e8: ldr      r3, [r6, r3]
004b04ec: ldr      r3, [r3]
004b04f0: cmp      r3, r0
004b04f4: beq      #0x4b0500
004b04f8: add      sp, sp, #0x1c
004b04fc: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
004b0500: lsl      r0, r0, #2
004b0504: mov      r1, #1
004b0508: bl       #0x31056c
004b050c: ldr      sb, [pc, #0xe4]
004b0510: ldr      r2, [sp, #0x14]
004b0514: ldr      r3, [r6, sb]
004b0518: cmp      r2, #0
004b051c: str      r0, [r3]
004b0520: beq      #0x4b04f8
004b0524: add      sl, sp, #0x10
004b0528: mov      r8, #1
004b052c: add      r1, sl, r8
004b0530: add      r3, sl, #2
004b0534: mov      r4, #0
004b0538: stm      sp, {r1, r3}
004b053c: mov      r0, r7
004b0540: mov      r1, sl
004b0544: bl       #0x3df1a0
004b0548: cmp      r8, #0
004b054c: str      r8, [sp, #0xc]
004b0550: bne      #0x4b0594
004b0554: ldr      r3, [sp]
004b0558: ldr      r2, [sp, #4]
004b055c: ldrb     r0, [r2, #1]
004b0560: ldrb     r1, [r3, #-1]
004b0564: cmp      r2, r3
004b0568: eor      r1, r0, r1
004b056c: strb     r1, [r3, #-1]
004b0570: ldrb     r0, [r2, #1]
004b0574: eor      r1, r1, r0
004b0578: strb     r1, [r2, #1]
004b057c: ldrb     r0, [r3, #-1]
004b0580: sub      r2, r2, #1
004b0584: eor      r1, r1, r0
004b0588: strb     r1, [r3, #-1]
004b058c: add      r3, r3, #1
004b0590: bhi      #0x4b055c
004b0594: ldr      r0, [sp, #0x10]
004b0598: ldr      r5, [r6, sb]
004b059c: mov      r1, #1
004b05a0: add      r0, r0, r1
004b05a4: ldr      fp, [r5]
004b05a8: bl       #0x31056c
004b05ac: str      r0, [fp, r4, lsl #2]
004b05b0: ldr      r3, [r5]
004b05b4: ldr      r2, [sp, #0x10]
004b05b8: mov      r0, r7
004b05bc: ldr      r1, [r3, r4, lsl #2]
004b05c0: mov      r3, #0
004b05c4: bl       #0x317454
004b05c8: ldr      r3, [r5]
004b05cc: mov      r1, #0
004b05d0: ldr      r2, [r3, r4, lsl #2]
004b05d4: ldr      r3, [sp, #0x10]
004b05d8: add      r4, r4, #1
004b05dc: strb     r1, [r2, r3]
004b05e0: ldr      r3, [sp, #0x14]
004b05e4: cmp      r3, r4
004b05e8: bhi      #0x4b053c
004b05ec: b        #0x4b04f8
004b05f0: subeq    r4, lr, r8, lsl #12
004b05f4: andeq    r2, r0, r8, ror sp
004b05f8: andeq    r1, r0, ip, lsr r0

# _ZN6Arrays10SkillTable13finalizeNamesEv
004a5464: push     {r4, r5, r6, r7, r8, lr}
004a5468: ldr      r5, [pc, #0x84]
004a546c: ldr      r6, [pc, #0x84]
004a5470: add      r5, pc, r5
004a5474: ldr      r3, [r5, r6]
004a5478: ldr      r3, [r3]
004a547c: cmp      r3, #0
004a5480: beq      #0x4a54f0
004a5484: ldr      r7, [pc, #0x70]
004a5488: ldr      r2, [r5, r7]
004a548c: ldr      r2, [r2]
004a5490: cmp      r2, #0
004a5494: beq      #0x4a54dc
004a5498: mov      r4, #0
004a549c: b        #0x4a54a8
004a54a0: ldr      r3, [r5, r6]
004a54a4: ldr      r3, [r3]
004a54a8: ldr      r0, [r3, r4, lsl #2]
004a54ac: add      r4, r4, #1
004a54b0: cmp      r0, #0
004a54b4: beq      #0x4a54c4
004a54b8: bl       #0x310440
004a54bc: ldr      r3, [r5, r6]
004a54c0: ldr      r3, [r3]
004a54c4: ldr      r2, [r5, r7]
004a54c8: ldr      r2, [r2]
004a54cc: cmp      r2, r4
004a54d0: bhi      #0x4a54a0
004a54d4: cmp      r3, #0
004a54d8: beq      #0x4a54e4
004a54dc: mov      r0, r3
004a54e0: bl       #0x310440
004a54e4: ldr      r3, [r5, r6]
004a54e8: mov      r2, #0
004a54ec: str      r2, [r3]
004a54f0: pop      {r4, r5, r6, r7, r8, pc}
004a54f4: subeq    pc, lr, r0, lsr #12
004a54f8: ldrdeq   r3, r4, [r0], -r8
004a54fc: andeq    r2, r0, r8, lsr #14

# _ZN7Structs9SkillList4readEP11IStreamBase
004ea830: push     {r4, r5, r6, r7, r8, lr}
004ea834: mov      r5, r0
004ea838: sub      sp, sp, #8
004ea83c: mov      r0, r1
004ea840: mov      r8, r1
004ea844: add      r1, r5, #4
004ea848: bl       #0x3df1a0
004ea84c: mov      r3, #1
004ea850: cmp      r3, #0
004ea854: str      r3, [sp, #4]
004ea858: bne      #0x4ea89c
004ea85c: add      r3, r5, #5
004ea860: add      r2, r5, #6
004ea864: ldrb     r0, [r2, #1]
004ea868: ldrb     r1, [r3, #-1]
004ea86c: cmp      r3, r2
004ea870: eor      r1, r0, r1
004ea874: strb     r1, [r3, #-1]
004ea878: ldrb     r0, [r2, #1]
004ea87c: eor      r1, r1, r0
004ea880: strb     r1, [r2, #1]
004ea884: ldrb     r0, [r3, #-1]
004ea888: sub      r2, r2, #1
004ea88c: eor      r1, r1, r0
004ea890: strb     r1, [r3, #-1]
004ea894: add      r3, r3, #1
004ea898: blo      #0x4ea864
004ea89c: ldr      r0, [r5, #8]
004ea8a0: cmp      r0, #0
004ea8a4: beq      #0x4ea8ac
004ea8a8: bl       #0x310440
004ea8ac: ldr      r0, [r5, #4]
004ea8b0: mov      r1, #1
004ea8b4: lsl      r0, r0, #2
004ea8b8: bl       #0x31056c
004ea8bc: ldr      r3, [r5, #4]
004ea8c0: str      r0, [r5, #8]
004ea8c4: cmp      r3, #0
004ea8c8: beq      #0x4ea94c
004ea8cc: mov      r4, #0
004ea8d0: mov      r7, #1
004ea8d4: lsl      r6, r4, #2
004ea8d8: add      r1, r0, r6
004ea8dc: mov      r0, r8
004ea8e0: bl       #0x459090
004ea8e4: str      r7, [sp, #4]
004ea8e8: cmp      r7, #0
004ea8ec: ldr      r3, [r5, #8]
004ea8f0: bne      #0x4ea938
004ea8f4: add      r6, r3, r6
004ea8f8: add      r3, r6, #2
004ea8fc: add      r6, r6, #1
004ea900: ldrb     r1, [r3, #1]
004ea904: ldrb     r2, [r6, #-1]
004ea908: cmp      r3, r6
004ea90c: eor      r2, r1, r2
004ea910: strb     r2, [r6, #-1]
004ea914: ldrb     r1, [r3, #1]
004ea918: eor      r2, r2, r1
004ea91c: strb     r2, [r3, #1]
004ea920: ldrb     r1, [r6, #-1]
004ea924: sub      r3, r3, #1
004ea928: eor      r2, r2, r1
004ea92c: strb     r2, [r6, #-1]
004ea930: add      r6, r6, #1
004ea934: bhi      #0x4ea900
004ea938: ldr      r3, [r5, #4]
004ea93c: add      r4, r4, #1
004ea940: cmp      r3, r4
004ea944: ldrhi    r0, [r5, #8]
004ea948: bhi      #0x4ea8d4
004ea94c: add      sp, sp, #8
004ea950: pop      {r4, r5, r6, r7, r8, pc}

# _ZN7Structs5Skill4readEP11IStreamBase
004ebeb0: push     {r4, r5, r6, r7, r8, lr}
004ebeb4: mov      r4, r0
004ebeb8: sub      sp, sp, #8
004ebebc: mov      r0, r1
004ebec0: mov      r5, r1
004ebec4: add      r1, r4, #4
004ebec8: bl       #0x459090
004ebecc: mov      r3, #1
004ebed0: cmp      r3, #0
004ebed4: str      r3, [sp, #4]
004ebed8: bne      #0x4ebf1c
004ebedc: add      r3, r4, #5
004ebee0: add      r2, r4, #6
004ebee4: ldrb     r0, [r2, #1]
004ebee8: ldrb     r1, [r3, #-1]
004ebeec: cmp      r3, r2
004ebef0: eor      r1, r0, r1
004ebef4: strb     r1, [r3, #-1]
004ebef8: ldrb     r0, [r2, #1]
004ebefc: eor      r1, r1, r0
004ebf00: strb     r1, [r2, #1]
004ebf04: ldrb     r0, [r3, #-1]
004ebf08: sub      r2, r2, #1
004ebf0c: eor      r1, r1, r0
004ebf10: strb     r1, [r3, #-1]
004ebf14: add      r3, r3, #1
004ebf18: blo      #0x4ebee4
004ebf1c: add      r1, r4, #8
004ebf20: mov      r0, r5
004ebf24: bl       #0x4db89c
004ebf28: mov      r0, r5
004ebf2c: add      r1, r4, #0xc
004ebf30: bl       #0x3df1a0
004ebf34: mov      r3, #1
004ebf38: cmp      r3, #0
004ebf3c: str      r3, [sp, #4]
004ebf40: bne      #0x4ebf84
004ebf44: add      r3, r4, #0xd
004ebf48: add      r2, r4, #0xe
004ebf4c: ldrb     r0, [r2, #1]
004ebf50: ldrb     r1, [r3, #-1]
004ebf54: cmp      r3, r2
004ebf58: eor      r1, r0, r1
004ebf5c: strb     r1, [r3, #-1]
004ebf60: ldrb     r0, [r2, #1]
004ebf64: eor      r1, r1, r0
004ebf68: strb     r1, [r2, #1]
004ebf6c: ldrb     r0, [r3, #-1]
004ebf70: sub      r2, r2, #1
004ebf74: eor      r1, r1, r0
004ebf78: strb     r1, [r3, #-1]
004ebf7c: add      r3, r3, #1
004ebf80: blo      #0x4ebf4c
004ebf84: ldr      r0, [r4, #0x10]
004ebf88: cmp      r0, #0
004ebf8c: beq      #0x4ebf94
004ebf90: bl       #0x310440
004ebf94: ldr      r0, [r4, #0xc]
004ebf98: mov      r1, #1
004ebf9c: lsl      r0, r0, #2
004ebfa0: bl       #0x31056c
004ebfa4: ldr      r3, [r4, #0xc]
004ebfa8: str      r0, [r4, #0x10]
004ebfac: cmp      r3, #0
004ebfb0: beq      #0x4ec034
004ebfb4: mov      r6, #0
004ebfb8: mov      r8, #1
004ebfbc: lsl      r7, r6, #2
004ebfc0: add      r1, r0, r7
004ebfc4: mov      r0, r5
004ebfc8: bl       #0x459090
004ebfcc: str      r8, [sp, #4]
004ebfd0: cmp      r8, #0
004ebfd4: ldr      r3, [r4, #0x10]
004ebfd8: bne      #0x4ec020
004ebfdc: add      r7, r3, r7
004ebfe0: add      r3, r7, #2
004ebfe4: add      r7, r7, #1
004ebfe8: ldrb     r1, [r3, #1]
004ebfec: ldrb     r2, [r7, #-1]
004ebff0: cmp      r7, r3
004ebff4: eor      r2, r1, r2
004ebff8: strb     r2, [r7, #-1]
004ebffc: ldrb     r1, [r3, #1]
004ec000: eor      r2, r2, r1
004ec004: strb     r2, [r3, #1]
004ec008: ldrb     r1, [r7, #-1]
004ec00c: sub      r3, r3, #1
004ec010: eor      r2, r2, r1
004ec014: strb     r2, [r7, #-1]
004ec018: add      r7, r7, #1
004ec01c: blo      #0x4ebfe8
004ec020: ldr      r3, [r4, #0xc]
004ec024: add      r6, r6, #1
004ec028: cmp      r3, r6
004ec02c: ldrhi    r0, [r4, #0x10]
004ec030: bhi      #0x4ebfbc
004ec034: mov      r0, r5
004ec038: add      r1, r4, #0x14
004ec03c: bl       #0x459090
004ec040: mov      r3, #1
004ec044: cmp      r3, #0
004ec048: str      r3, [sp, #4]
004ec04c: bne      #0x4ec090
004ec050: add      r3, r4, #0x15
004ec054: add      r2, r4, #0x16
004ec058: ldrb     r0, [r2, #1]
004ec05c: ldrb     r1, [r3, #-1]
004ec060: cmp      r3, r2
004ec064: eor      r1, r0, r1
004ec068: strb     r1, [r3, #-1]
004ec06c: ldrb     r0, [r2, #1]
004ec070: eor      r1, r1, r0
004ec074: strb     r1, [r2, #1]
004ec078: ldrb     r0, [r3, #-1]
004ec07c: sub      r2, r2, #1
004ec080: eor      r1, r1, r0
004ec084: strb     r1, [r3, #-1]
004ec088: add      r3, r3, #1
004ec08c: blo      #0x4ec058
004ec090: add      r1, r4, #0x18
004ec094: mov      r0, r5
004ec098: bl       #0x4db89c
004ec09c: mov      r0, r5
004ec0a0: add      r1, r4, #0x1c
004ec0a4: bl       #0x459090
004ec0a8: mov      r3, #1
004ec0ac: cmp      r3, #0
004ec0b0: str      r3, [sp, #4]
004ec0b4: bne      #0x4ec0f8
004ec0b8: add      r3, r4, #0x1d
004ec0bc: add      r2, r4, #0x1e
004ec0c0: ldrb     r0, [r2, #1]
004ec0c4: ldrb     r1, [r3, #-1]
004ec0c8: cmp      r3, r2
004ec0cc: eor      r1, r0, r1
004ec0d0: strb     r1, [r3, #-1]
004ec0d4: ldrb     r0, [r2, #1]
004ec0d8: eor      r1, r1, r0
004ec0dc: strb     r1, [r2, #1]
004ec0e0: ldrb     r0, [r3, #-1]
004ec0e4: sub      r2, r2, #1
004ec0e8: eor      r1, r1, r0
004ec0ec: strb     r1, [r3, #-1]
004ec0f0: add      r3, r3, #1
004ec0f4: blo      #0x4ec0c0
004ec0f8: mov      r0, r5
004ec0fc: add      r1, r4, #0x20
004ec100: bl       #0x459090
004ec104: mov      r3, #1
004ec108: cmp      r3, #0
004ec10c: str      r3, [sp, #4]
004ec110: bne      #0x4ec154
004ec114: add      r3, r4, #0x21
004ec118: add      r2, r4, #0x22
004ec11c: ldrb     r0, [r2, #1]
004ec120: ldrb     r1, [r3, #-1]
004ec124: cmp      r3, r2
004ec128: eor      r1, r0, r1
004ec12c: strb     r1, [r3, #-1]
004ec130: ldrb     r0, [r2, #1]
004ec134: eor      r1, r1, r0
004ec138: strb     r1, [r2, #1]
004ec13c: ldrb     r0, [r3, #-1]
004ec140: sub      r2, r2, #1
004ec144: eor      r1, r1, r0
004ec148: strb     r1, [r3, #-1]
004ec14c: add      r3, r3, #1
004ec150: blo      #0x4ec11c
004ec154: mov      r0, r5
004ec158: add      r1, r4, #0x24
004ec15c: bl       #0x3df1a0
004ec160: mov      r3, #1
004ec164: cmp      r3, #0
004ec168: str      r3, [sp, #4]
004ec16c: bne      #0x4ec1b0
004ec170: add      r3, r4, #0x25
004ec174: add      r2, r4, #0x26
004ec178: ldrb     r0, [r2, #1]
004ec17c: ldrb     r1, [r3, #-1]
004ec180: cmp      r3, r2
004ec184: eor      r1, r0, r1
004ec188: strb     r1, [r3, #-1]
004ec18c: ldrb     r0, [r2, #1]
004ec190: eor      r1, r1, r0
004ec194: strb     r1, [r2, #1]
004ec198: ldrb     r0, [r3, #-1]
004ec19c: sub      r2, r2, #1
004ec1a0: eor      r1, r1, r0
004ec1a4: strb     r1, [r3, #-1]
004ec1a8: add      r3, r3, #1
004ec1ac: blo      #0x4ec178
004ec1b0: ldr      r0, [r4, #0x28]
004ec1b4: cmp      r0, #0
004ec1b8: beq      #0x4ec1c0
004ec1bc: bl       #0x310440
004ec1c0: ldr      r0, [r4, #0x24]
004ec1c4: mov      r1, #1
004ec1c8: mov      r6, #0
004ec1cc: add      r0, r0, r1
004ec1d0: bl       #0x31056c
004ec1d4: ldr      r2, [r4, #0x24]
004ec1d8: mov      r1, r0
004ec1dc: str      r0, [r4, #0x28]
004ec1e0: mov      r3, r6
004ec1e4: mov      r0, r5
004ec1e8: bl       #0x317454
004ec1ec: ldr      r3, [r4, #0x24]
004ec1f0: ldr      r2, [r4, #0x28]
004ec1f4: add      r1, r4, #0x2c
004ec1f8: mov      r0, r5
004ec1fc: strb     r6, [r2, r3]
004ec200: bl       #0x4db89c
004ec204: mov      r0, r5
004ec208: add      r1, r4, #0x30
004ec20c: bl       #0x459090
004ec210: mov      r3, #1
004ec214: cmp      r3, r6
004ec218: str      r3, [sp, #4]
004ec21c: bne      #0x4ec260
004ec220: add      r3, r4, #0x31
004ec224: add      r2, r4, #0x32
004ec228: ldrb     r0, [r2, #1]
004ec22c: ldrb     r1, [r3, #-1]
004ec230: cmp      r3, r2
004ec234: eor      r1, r0, r1
004ec238: strb     r1, [r3, #-1]
004ec23c: ldrb     r0, [r2, #1]
004ec240: eor      r1, r1, r0
004ec244: strb     r1, [r2, #1]
004ec248: ldrb     r0, [r3, #-1]
004ec24c: sub      r2, r2, #1
004ec250: eor      r1, r1, r0
004ec254: strb     r1, [r3, #-1]
004ec258: add      r3, r3, #1
004ec25c: blo      #0x4ec228
004ec260: mov      r0, r5
004ec264: add      r1, r4, #0x34
004ec268: bl       #0x459090
004ec26c: mov      r3, #1
004ec270: cmp      r3, #0
004ec274: str      r3, [sp, #4]
004ec278: bne      #0x4ec2bc
004ec27c: add      r3, r4, #0x35
004ec280: add      r2, r4, #0x36
004ec284: ldrb     r0, [r2, #1]
004ec288: ldrb     r1, [r3, #-1]
004ec28c: cmp      r3, r2
004ec290: eor      r1, r0, r1
004ec294: strb     r1, [r3, #-1]
004ec298: ldrb     r0, [r2, #1]
004ec29c: eor      r1, r1, r0
004ec2a0: strb     r1, [r2, #1]
004ec2a4: ldrb     r0, [r3, #-1]
004ec2a8: sub      r2, r2, #1
004ec2ac: eor      r1, r1, r0
004ec2b0: strb     r1, [r3, #-1]
004ec2b4: add      r3, r3, #1
004ec2b8: blo      #0x4ec284
004ec2bc: mov      r0, r5
004ec2c0: add      r1, r4, #0x38
004ec2c4: bl       #0x3df1a0
004ec2c8: mov      r3, #1
004ec2cc: cmp      r3, #0
004ec2d0: str      r3, [sp, #4]
004ec2d4: bne      #0x4ec318
004ec2d8: add      r3, r4, #0x39
004ec2dc: add      r2, r4, #0x3a
004ec2e0: ldrb     r0, [r2, #1]
004ec2e4: ldrb     r1, [r3, #-1]
004ec2e8: cmp      r3, r2
004ec2ec: eor      r1, r0, r1
004ec2f0: strb     r1, [r3, #-1]
004ec2f4: ldrb     r0, [r2, #1]
004ec2f8: eor      r1, r1, r0
004ec2fc: strb     r1, [r2, #1]
004ec300: ldrb     r0, [r3, #-1]
004ec304: sub      r2, r2, #1
004ec308: eor      r1, r1, r0
004ec30c: strb     r1, [r3, #-1]
004ec310: add      r3, r3, #1
004ec314: blo      #0x4ec2e0
004ec318: ldr      r0, [r4, #0x3c]
004ec31c: cmp      r0, #0
004ec320: beq      #0x4ec328
004ec324: bl       #0x310440
004ec328: ldr      r0, [r4, #0x38]
004ec32c: mov      r1, #1
004ec330: mov      r6, #0
004ec334: add      r0, r0, r1
004ec338: bl       #0x31056c
004ec33c: ldr      r2, [r4, #0x38]
004ec340: mov      r1, r0
004ec344: str      r0, [r4, #0x3c]
004ec348: mov      r3, r6
004ec34c: mov      r0, r5
004ec350: bl       #0x317454
004ec354: ldr      r3, [r4, #0x38]
004ec358: ldr      r2, [r4, #0x3c]
004ec35c: mov      r0, r5
004ec360: add      r1, r4, #0x40
004ec364: strb     r6, [r2, r3]
004ec368: bl       #0x459090
004ec36c: mov      r3, #1
004ec370: cmp      r3, r6
004ec374: str      r3, [sp, #4]
004ec378: bne      #0x4ec3bc
004ec37c: add      r3, r4, #0x41
004ec380: add      r2, r4, #0x42
004ec384: ldrb     r0, [r2, #1]
004ec388: ldrb     r1, [r3, #-1]
004ec38c: cmp      r3, r2
004ec390: eor      r1, r0, r1
004ec394: strb     r1, [r3, #-1]
004ec398: ldrb     r0, [r2, #1]
004ec39c: eor      r1, r1, r0
004ec3a0: strb     r1, [r2, #1]
004ec3a4: ldrb     r0, [r3, #-1]
004ec3a8: sub      r2, r2, #1
004ec3ac: eor      r1, r1, r0
004ec3b0: strb     r1, [r3, #-1]
004ec3b4: add      r3, r3, #1
004ec3b8: blo      #0x4ec384
004ec3bc: mov      r0, r5
004ec3c0: add      r1, r4, #0x44
004ec3c4: bl       #0x459090
004ec3c8: mov      r3, #1
004ec3cc: cmp      r3, #0
004ec3d0: str      r3, [sp, #4]
004ec3d4: bne      #0x4ec418
004ec3d8: add      r3, r4, #0x45
004ec3dc: add      r2, r4, #0x46
004ec3e0: ldrb     r0, [r2, #1]
004ec3e4: ldrb     r1, [r3, #-1]
004ec3e8: cmp      r3, r2
004ec3ec: eor      r1, r0, r1
004ec3f0: strb     r1, [r3, #-1]
004ec3f4: ldrb     r0, [r2, #1]
004ec3f8: eor      r1, r1, r0
004ec3fc: strb     r1, [r2, #1]
004ec400: ldrb     r0, [r3, #-1]
004ec404: sub      r2, r2, #1
004ec408: eor      r1, r1, r0
004ec40c: strb     r1, [r3, #-1]
004ec410: add      r3, r3, #1
004ec414: blo      #0x4ec3e0
004ec418: mov      r0, r5
004ec41c: add      r1, r4, #0x48
004ec420: bl       #0x459090
004ec424: mov      r3, #1
004ec428: cmp      r3, #0
004ec42c: str      r3, [sp, #4]
004ec430: bne      #0x4ec474
004ec434: add      r3, r4, #0x4a
004ec438: add      r4, r4, #0x49
004ec43c: ldrb     r1, [r3, #1]
004ec440: ldrb     r2, [r4, #-1]
004ec444: cmp      r4, r3
004ec448: eor      r2, r1, r2
004ec44c: strb     r2, [r4, #-1]
004ec450: ldrb     r1, [r3, #1]
004ec454: eor      r2, r2, r1
004ec458: strb     r2, [r3, #1]
004ec45c: ldrb     r1, [r4, #-1]
004ec460: sub      r3, r3, #1
004ec464: eor      r2, r2, r1
004ec468: strb     r2, [r4, #-1]
004ec46c: add      r4, r4, #1
004ec470: blo      #0x4ec43c
004ec474: add      sp, sp, #8
004ec478: pop      {r4, r5, r6, r7, r8, pc}

# _ZN12StreamReader12readStringExEP11IStreamBasePcy
00317454: push     {r4, r5, r6, lr}
00317458: ldr      ip, [r0]
0031745c: mov      r4, r2
00317460: mov      r5, r3
00317464: mov      lr, pc
00317468: ldr      pc, [ip, #0x18]
0031746c: cmp      r0, r4
00317470: mov      r0, #0
00317474: beq      #0x317480
00317478: and      r0, r0, #1
0031747c: pop      {r4, r5, r6, pc}
00317480: cmp      r1, r5
00317484: moveq    r0, #1
00317488: and      r0, r0, #1
0031748c: pop      {r4, r5, r6, pc}

# _ZN6Arrays10SkillTable4readEP11IStreamBase
004b9810: push     {r4, r5, r6, r7, r8, sl, lr}
004b9814: sub      sp, sp, #0xc
004b9818: mov      sl, r0
004b981c: bl       #0x313a90
004b9820: ldr      r6, [pc, #0x12c]
004b9824: mov      r3, #1
004b9828: cmp      r3, #0
004b982c: str      r0, [sp, #4]
004b9830: str      r3, [sp]
004b9834: add      r6, pc, r6
004b9838: bne      #0x4b9880
004b983c: add      r3, sp, #4
004b9840: add      r2, r3, #2
004b9844: add      r3, r3, #1
004b9848: ldrb     r0, [r2, #1]
004b984c: ldrb     r1, [r3, #-1]
004b9850: cmp      r2, r3
004b9854: eor      r1, r0, r1
004b9858: strb     r1, [r3, #-1]
004b985c: ldrb     r0, [r2, #1]
004b9860: eor      r1, r1, r0
004b9864: strb     r1, [r2, #1]
004b9868: ldrb     r0, [r3, #-1]
004b986c: sub      r2, r2, #1
004b9870: eor      r1, r1, r0
004b9874: strb     r1, [r3, #-1]
004b9878: add      r3, r3, #1
004b987c: bhi      #0x4b9848
004b9880: bl       #0x4a5500
004b9884: ldr      r7, [pc, #0xcc]
004b9888: ldr      r4, [sp, #4]
004b988c: mov      r5, #0x4c
004b9890: ldr      r3, [r6, r7]
004b9894: mul      r0, r5, r4
004b9898: str      r4, [r3]
004b989c: add      r0, r0, #8
004b98a0: mov      r1, #1
004b98a4: bl       #0x31056c
004b98a8: cmp      r4, #0
004b98ac: str      r5, [r0]
004b98b0: str      r4, [r0, #4]
004b98b4: add      r3, r0, #8
004b98b8: beq      #0x4b98f0
004b98bc: ldr      r1, [pc, #0x98]
004b98c0: mov      r2, #0
004b98c4: ldr      ip, [r6, r1]
004b98c8: mov      r1, r2
004b98cc: add      ip, ip, #8
004b98d0: add      r2, r2, #1
004b98d4: cmp      r2, r4
004b98d8: str      ip, [r0, #8]
004b98dc: str      r1, [r0, #0x18]
004b98e0: str      r1, [r0, #0x30]
004b98e4: str      r1, [r0, #0x44]
004b98e8: add      r0, r0, #0x4c
004b98ec: bne      #0x4b98d0
004b98f0: ldr      r2, [r6, r7]
004b98f4: ldr      r8, [pc, #0x64]
004b98f8: ldr      r1, [r2]
004b98fc: ldr      r2, [r6, r8]
004b9900: cmp      r1, #0
004b9904: str      r3, [r2]
004b9908: beq      #0x4b994c
004b990c: mov      r4, #0
004b9910: mov      r5, r4
004b9914: b        #0x4b9920
004b9918: ldr      r3, [r6, r8]
004b991c: ldr      r3, [r3]
004b9920: add      r0, r3, r4
004b9924: mov      r1, sl
004b9928: ldr      r3, [r3, r4]
004b992c: mov      lr, pc
004b9930: ldr      pc, [r3, #0xc]
004b9934: ldr      r3, [r6, r7]
004b9938: add      r5, r5, #1
004b993c: add      r4, r4, #0x4c
004b9940: ldr      r3, [r3]
004b9944: cmp      r3, r5
004b9948: bhi      #0x4b9918
004b994c: add      sp, sp, #0xc
004b9950: pop      {r4, r5, r6, r7, r8, sl, pc}
004b9954: subeq    fp, sp, ip, asr r2
004b9958: andeq    r2, r0, r8, lsr #14
004b995c: andeq    r3, r0, ip, ror r6
004b9960: andeq    r4, r0, ip, lsl r4

# _ZN7Structs5Skill8finalizeEv
004d0fa4: push     {r4, lr}
004d0fa8: mov      r4, r0
004d0fac: ldr      r0, [r0, #0x10]
004d0fb0: cmp      r0, #0
004d0fb4: beq      #0x4d0fc8
004d0fb8: bl       #0x310440
004d0fbc: mov      r3, #0
004d0fc0: str      r3, [r4, #0xc]
004d0fc4: str      r3, [r4, #0x10]
004d0fc8: ldr      r0, [r4, #0x28]
004d0fcc: cmp      r0, #0
004d0fd0: beq      #0x4d0fe4
004d0fd4: bl       #0x310440
004d0fd8: mov      r3, #0
004d0fdc: str      r3, [r4, #0x24]
004d0fe0: str      r3, [r4, #0x28]
004d0fe4: ldr      r0, [r4, #0x3c]
004d0fe8: cmp      r0, #0
004d0fec: beq      #0x4d1000
004d0ff0: bl       #0x310440
004d0ff4: mov      r3, #0
004d0ff8: str      r3, [r4, #0x38]
004d0ffc: str      r3, [r4, #0x3c]
004d1000: pop      {r4, pc}

# _ZN6Arrays14SkillListTable8finalizeEv
004a5680: push     {r4, r5, r6, r7, r8, lr}
004a5684: ldr      r5, [pc, #0xcc]
004a5688: ldr      r7, [pc, #0xcc]
004a568c: add      r5, pc, r5
004a5690: ldr      r3, [r5, r7]
004a5694: ldr      r3, [r3]
004a5698: cmp      r3, #0
004a569c: beq      #0x4a5754
004a56a0: ldr      r8, [pc, #0xb8]
004a56a4: ldr      r2, [r5, r8]
004a56a8: ldr      r2, [r2]
004a56ac: cmp      r2, #0
004a56b0: beq      #0x4a5700
004a56b4: mov      r4, #0
004a56b8: mov      r6, r4
004a56bc: b        #0x4a56c8
004a56c0: ldr      r3, [r5, r7]
004a56c4: ldr      r3, [r3]
004a56c8: add      r0, r3, r4
004a56cc: ldr      r3, [r3, r4]
004a56d0: mov      lr, pc
004a56d4: ldr      pc, [r3, #8]
004a56d8: ldr      r3, [r5, r8]
004a56dc: add      r6, r6, #1
004a56e0: add      r4, r4, #0xc
004a56e4: ldr      r3, [r3]
004a56e8: cmp      r3, r6
004a56ec: bhi      #0x4a56c0
004a56f0: ldr      r3, [r5, r7]
004a56f4: ldr      r3, [r3]
004a56f8: cmp      r3, #0
004a56fc: beq      #0x4a5748
004a5700: ldr      r2, [r3, #-4]
004a5704: mov      r0, #0xc
004a5708: mla      r0, r0, r2, r3
004a570c: cmp      r3, r0
004a5710: bne      #0x4a571c
004a5714: b        #0x4a5740
004a5718: mov      r0, r4
004a571c: sub      r4, r0, #0xc
004a5720: ldr      r3, [r0, #-0xc]
004a5724: mov      r0, r4
004a5728: mov      lr, pc
004a572c: ldr      pc, [r3]
004a5730: ldr      r3, [r5, r7]
004a5734: ldr      r0, [r3]
004a5738: cmp      r0, r4
004a573c: bne      #0x4a5718
004a5740: sub      r0, r0, #8
004a5744: bl       #0x310440
004a5748: ldr      r3, [r5, r7]
004a574c: mov      r2, #0
004a5750: str      r2, [r3]
004a5754: pop      {r4, r5, r6, r7, r8, pc}
004a5758: subeq    pc, lr, r4, lsl #8
004a575c: andeq    r1, r0, r8, asr #3
004a5760: andeq    r2, r0, r8, ror sp

# _ZN12StreamReader6readAsIjEEvP11IStreamBasePT_
003df1a0: str      lr, [sp, #-4]!
003df1a4: mov      r3, #0
003df1a8: sub      sp, sp, #0xc
003df1ac: ldr      ip, [r0]
003df1b0: mov      r2, #4
003df1b4: mov      lr, pc
003df1b8: ldr      pc, [ip, #0x18]
003df1bc: ldr      r3, [pc, #0x74]
003df1c0: cmp      r0, #4
003df1c4: add      r3, pc, r3
003df1c8: beq      #0x3df1f8
003df1cc: ldr      r2, [pc, #0x68]
003df1d0: ldr      r2, [r3, r2]
003df1d4: ldr      r2, [r2]
003df1d8: cmp      r2, #2
003df1dc: moveq    r3, #0
003df1e0: streq    r3, [r3]
003df1e4: beq      #0x3df1f0
003df1e8: cmp      r2, #1
003df1ec: beq      #0x3df204
003df1f0: add      sp, sp, #0xc
003df1f4: ldm      sp!, {pc}
003df1f8: cmp      r1, #0
003df1fc: beq      #0x3df1f0
003df200: b        #0x3df1cc
003df204: ldr      r0, [pc, #0x34]
003df208: ldr      r1, [pc, #0x34]
003df20c: ldr      r2, [pc, #0x34]
003df210: ldr      r0, [r3, r0]
003df214: ldr      r3, [pc, #0x30]
003df218: mov      ip, #0x50
003df21c: add      r1, pc, r1
003df220: add      r2, pc, r2
003df224: add      r3, pc, r3
003df228: add      r0, r0, #0xa8
003df22c: str      ip, [sp]
003df230: bl       #0x30e004
003df234: b        #0x3df1f0
003df238: subseq   r5, fp, ip, asr #17
003df23c: andeq    r3, r0, r0, asr #19
003df240: andeq    r1, r0, r0, asr #19
003df244: strheq   pc, [sp], #-0x1c
003df248: subeq    pc, sp, r0, ror #5
003df24c: subeq    r0, lr, ip, lsl fp

# _ZN6Arrays10SkillTable9readNamesEP11IStreamBase
004b0938: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
004b093c: mov      r7, r0
004b0940: sub      sp, sp, #0x1c
004b0944: bl       #0x4a5464
004b0948: mov      r0, r7
004b094c: bl       #0x313a90
004b0950: ldr      r6, [pc, #0x16c]
004b0954: mov      r3, #1
004b0958: cmp      r3, #0
004b095c: add      r6, pc, r6
004b0960: str      r0, [sp, #0x14]
004b0964: str      r3, [sp, #0xc]
004b0968: bne      #0x4b09b8
004b096c: add      r3, sp, #0x14
004b0970: add      r2, r3, #2
004b0974: add      r3, r3, #1
004b0978: ldrb     r0, [r2, #1]
004b097c: ldrb     r1, [r3, #-1]
004b0980: cmp      r2, r3
004b0984: mov      r4, r2
004b0988: eor      r1, r0, r1
004b098c: strb     r1, [r3, #-1]
004b0990: ldrb     r0, [r2, #1]
004b0994: eor      r1, r1, r0
004b0998: strb     r1, [r2, #1]
004b099c: ldrb     r0, [r3, #-1]
004b09a0: sub      r2, r2, #1
004b09a4: eor      r1, r1, r0
004b09a8: strb     r1, [r3, #-1]
004b09ac: add      r3, r3, #1
004b09b0: bhi      #0x4b0978
004b09b4: ldr      r0, [sp, #0x14]
004b09b8: ldr      r3, [pc, #0x108]
004b09bc: ldr      r3, [r6, r3]
004b09c0: ldr      r3, [r3]
004b09c4: cmp      r3, r0
004b09c8: beq      #0x4b09d4
004b09cc: add      sp, sp, #0x1c
004b09d0: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
004b09d4: lsl      r0, r0, #2
004b09d8: mov      r1, #1
004b09dc: bl       #0x31056c
004b09e0: ldr      sb, [pc, #0xe4]
004b09e4: ldr      r2, [sp, #0x14]
004b09e8: ldr      r3, [r6, sb]
004b09ec: cmp      r2, #0
004b09f0: str      r0, [r3]
004b09f4: beq      #0x4b09cc
004b09f8: add      sl, sp, #0x10
004b09fc: mov      r8, #1
004b0a00: add      r1, sl, r8
004b0a04: add      r3, sl, #2
004b0a08: mov      r4, #0
004b0a0c: stm      sp, {r1, r3}
004b0a10: mov      r0, r7
004b0a14: mov      r1, sl
004b0a18: bl       #0x3df1a0
004b0a1c: cmp      r8, #0
004b0a20: str      r8, [sp, #0xc]
004b0a24: bne      #0x4b0a68
004b0a28: ldr      r3, [sp]
004b0a2c: ldr      r2, [sp, #4]
004b0a30: ldrb     r0, [r2, #1]
004b0a34: ldrb     r1, [r3, #-1]
004b0a38: cmp      r2, r3
004b0a3c: eor      r1, r0, r1
004b0a40: strb     r1, [r3, #-1]
004b0a44: ldrb     r0, [r2, #1]
004b0a48: eor      r1, r1, r0
004b0a4c: strb     r1, [r2, #1]
004b0a50: ldrb     r0, [r3, #-1]
004b0a54: sub      r2, r2, #1
004b0a58: eor      r1, r1, r0
004b0a5c: strb     r1, [r3, #-1]
004b0a60: add      r3, r3, #1
004b0a64: bhi      #0x4b0a30
004b0a68: ldr      r0, [sp, #0x10]
004b0a6c: ldr      r5, [r6, sb]
004b0a70: mov      r1, #1
004b0a74: add      r0, r0, r1
004b0a78: ldr      fp, [r5]
004b0a7c: bl       #0x31056c
004b0a80: str      r0, [fp, r4, lsl #2]
004b0a84: ldr      r3, [r5]
004b0a88: ldr      r2, [sp, #0x10]
004b0a8c: mov      r0, r7
004b0a90: ldr      r1, [r3, r4, lsl #2]
004b0a94: mov      r3, #0
004b0a98: bl       #0x317454
004b0a9c: ldr      r3, [r5]
004b0aa0: mov      r1, #0
004b0aa4: ldr      r2, [r3, r4, lsl #2]
004b0aa8: ldr      r3, [sp, #0x10]
004b0aac: add      r4, r4, #1
004b0ab0: strb     r1, [r2, r3]
004b0ab4: ldr      r3, [sp, #0x14]
004b0ab8: cmp      r3, r4
004b0abc: bhi      #0x4b0a10
004b0ac0: b        #0x4b09cc
004b0ac4: subeq    r4, lr, r4, lsr r1
004b0ac8: andeq    r2, r0, r8, lsr #14
004b0acc: ldrdeq   r3, r4, [r0], -r8

# _ZN6Arrays19GetMemberIDByStringINS_10SkillTableEEEiPKc
004ad0dc: ldr      r3, [pc, #0x60]
004ad0e0: ldr      r2, [pc, #0x60]
004ad0e4: push     {r4, r5, r6, r7, r8, lr}
004ad0e8: add      r3, pc, r3
004ad0ec: ldr      r2, [r3, r2]
004ad0f0: mov      r6, r0
004ad0f4: ldr      r5, [r2]
004ad0f8: cmp      r5, #0
004ad0fc: beq      #0x4ad13c
004ad100: ldr      r2, [pc, #0x44]
004ad104: mov      r4, #0
004ad108: ldr      r3, [r3, r2]
004ad10c: ldr      r7, [r3]
004ad110: b        #0x4ad120
004ad114: add      r4, r4, #1
004ad118: cmp      r4, r5
004ad11c: beq      #0x4ad13c
004ad120: ldr      r1, [r7, r4, lsl #2]
004ad124: mov      r0, r6
004ad128: bl       #0x30e31c
004ad12c: cmp      r0, #0
004ad130: bne      #0x4ad114
004ad134: mov      r0, r4
004ad138: pop      {r4, r5, r6, r7, r8, pc}
004ad13c: mvn      r0, #0
004ad140: pop      {r4, r5, r6, r7, r8, pc}
004ad144: subeq    r7, lr, r8, lsr #19
004ad148: andeq    r2, r0, r8, lsr #14
004ad14c: ldrdeq   r3, r4, [r0], -r8

# _ZN7Structs19GetMemberIDByStringINS_5SkillEEEiPKc
004ad150: ldr      r3, [pc, #0x48]
004ad154: ldr      r2, [pc, #0x48]
004ad158: push     {r4, r5, r6, lr}
004ad15c: add      r3, pc, r3
004ad160: mov      r6, r0
004ad164: ldr      r5, [r3, r2]
004ad168: mov      r4, #0
004ad16c: ldr      r1, [r5, #0x14]
004ad170: mov      r0, r6
004ad174: bl       #0x30e31c
004ad178: cmp      r0, #0
004ad17c: beq      #0x4ad198
004ad180: add      r4, r4, #1
004ad184: cmp      r4, #0xf
004ad188: add      r5, r5, #0x18
004ad18c: bne      #0x4ad16c
004ad190: mvn      r0, #0
004ad194: pop      {r4, r5, r6, pc}
004ad198: mov      r0, r4
004ad19c: pop      {r4, r5, r6, pc}
004ad1a0: subeq    r7, lr, r4, lsr sb
004ad1a4: muleq    r0, ip, pc

# _ZN6Arrays14SkillListTable4readEP11IStreamBase
004b9964: push     {r4, r5, r6, r7, r8, sl, lr}
004b9968: sub      sp, sp, #0xc
004b996c: mov      sl, r0
004b9970: bl       #0x313a90
004b9974: ldr      r6, [pc, #0x124]
004b9978: mov      r3, #1
004b997c: cmp      r3, #0
004b9980: str      r0, [sp, #4]
004b9984: str      r3, [sp]
004b9988: add      r6, pc, r6
004b998c: bne      #0x4b99d4
004b9990: add      r3, sp, #4
004b9994: add      r2, r3, #2
004b9998: add      r3, r3, #1
004b999c: ldrb     r0, [r2, #1]
004b99a0: ldrb     r1, [r3, #-1]
004b99a4: cmp      r2, r3
004b99a8: eor      r1, r0, r1
004b99ac: strb     r1, [r3, #-1]
004b99b0: ldrb     r0, [r2, #1]
004b99b4: eor      r1, r1, r0
004b99b8: strb     r1, [r2, #1]
004b99bc: ldrb     r0, [r3, #-1]
004b99c0: sub      r2, r2, #1
004b99c4: eor      r1, r1, r0
004b99c8: strb     r1, [r3, #-1]
004b99cc: add      r3, r3, #1
004b99d0: bhi      #0x4b999c
004b99d4: bl       #0x4a5680
004b99d8: ldr      r7, [pc, #0xc4]
004b99dc: ldr      r4, [sp, #4]
004b99e0: mov      r5, #0xc
004b99e4: ldr      r3, [r6, r7]
004b99e8: mul      r0, r5, r4
004b99ec: str      r4, [r3]
004b99f0: add      r0, r0, #8
004b99f4: mov      r1, #1
004b99f8: bl       #0x31056c
004b99fc: cmp      r4, #0
004b9a00: str      r5, [r0]
004b9a04: str      r4, [r0, #4]
004b9a08: add      r3, r0, #8
004b9a0c: beq      #0x4b9a3c
004b9a10: ldr      r1, [pc, #0x90]
004b9a14: mov      r2, #0
004b9a18: mov      ip, r2
004b9a1c: ldr      r1, [r6, r1]
004b9a20: add      r1, r1, #8
004b9a24: add      r2, r2, #1
004b9a28: cmp      r2, r4
004b9a2c: str      r1, [r0, #8]
004b9a30: str      ip, [r0, #0x10]
004b9a34: add      r0, r0, #0xc
004b9a38: bne      #0x4b9a24
004b9a3c: ldr      r2, [r6, r7]
004b9a40: ldr      r8, [pc, #0x64]
004b9a44: ldr      r1, [r2]
004b9a48: ldr      r2, [r6, r8]
004b9a4c: cmp      r1, #0
004b9a50: str      r3, [r2]
004b9a54: beq      #0x4b9a98
004b9a58: mov      r4, #0
004b9a5c: mov      r5, r4
004b9a60: b        #0x4b9a6c
004b9a64: ldr      r3, [r6, r8]
004b9a68: ldr      r3, [r3]
004b9a6c: add      r0, r3, r4
004b9a70: mov      r1, sl
004b9a74: ldr      r3, [r3, r4]
004b9a78: mov      lr, pc
004b9a7c: ldr      pc, [r3, #0xc]
004b9a80: ldr      r3, [r6, r7]
004b9a84: add      r5, r5, #1
004b9a88: add      r4, r4, #0xc
004b9a8c: ldr      r3, [r3]
004b9a90: cmp      r3, r5
004b9a94: bhi      #0x4b9a64
004b9a98: add      sp, sp, #0xc
004b9a9c: pop      {r4, r5, r6, r7, r8, sl, pc}
004b9aa0: subeq    fp, sp, r8, lsl #2
004b9aa4: andeq    r2, r0, r8, ror sp
004b9aa8: andeq    r1, r0, r4, asr #13
004b9aac: andeq    r1, r0, r8, asr #3

# _ZN6Arrays10SkillTable8finalizeEv
004a5500: push     {r4, r5, r6, r7, r8, lr}
004a5504: ldr      r5, [pc, #0xcc]
004a5508: ldr      r7, [pc, #0xcc]
004a550c: add      r5, pc, r5
004a5510: ldr      r3, [r5, r7]
004a5514: ldr      r3, [r3]
004a5518: cmp      r3, #0
004a551c: beq      #0x4a55d4
004a5520: ldr      r8, [pc, #0xb8]
004a5524: ldr      r2, [r5, r8]
004a5528: ldr      r2, [r2]
004a552c: cmp      r2, #0
004a5530: beq      #0x4a5580
004a5534: mov      r4, #0
004a5538: mov      r6, r4
004a553c: b        #0x4a5548
004a5540: ldr      r3, [r5, r7]
004a5544: ldr      r3, [r3]
004a5548: add      r0, r3, r4
004a554c: ldr      r3, [r3, r4]
004a5550: mov      lr, pc
004a5554: ldr      pc, [r3, #8]
004a5558: ldr      r3, [r5, r8]
004a555c: add      r6, r6, #1
004a5560: add      r4, r4, #0x4c
004a5564: ldr      r3, [r3]
004a5568: cmp      r3, r6
004a556c: bhi      #0x4a5540
004a5570: ldr      r3, [r5, r7]
004a5574: ldr      r3, [r3]
004a5578: cmp      r3, #0
004a557c: beq      #0x4a55c8
004a5580: ldr      r2, [r3, #-4]
004a5584: mov      r0, #0x4c
004a5588: mla      r0, r0, r2, r3
004a558c: cmp      r3, r0
004a5590: bne      #0x4a559c
004a5594: b        #0x4a55c0
004a5598: mov      r0, r4
004a559c: sub      r4, r0, #0x4c
004a55a0: ldr      r3, [r0, #-0x4c]
004a55a4: mov      r0, r4
004a55a8: mov      lr, pc
004a55ac: ldr      pc, [r3]
004a55b0: ldr      r3, [r5, r7]
004a55b4: ldr      r0, [r3]
004a55b8: cmp      r0, r4
004a55bc: bne      #0x4a5598
004a55c0: sub      r0, r0, #8
004a55c4: bl       #0x310440
004a55c8: ldr      r3, [r5, r7]
004a55cc: mov      r2, #0
004a55d0: str      r2, [r3]
004a55d4: pop      {r4, r5, r6, r7, r8, pc}
004a55d8: subeq    pc, lr, r4, lsl #11
004a55dc: andeq    r4, r0, ip, lsl r4
004a55e0: andeq    r2, r0, r8, lsr #14

# _ZN6Arrays14SkillListTable13finalizeNamesEv
004a55e4: push     {r4, r5, r6, r7, r8, lr}
004a55e8: ldr      r5, [pc, #0x84]
004a55ec: ldr      r6, [pc, #0x84]
004a55f0: add      r5, pc, r5
004a55f4: ldr      r3, [r5, r6]
004a55f8: ldr      r3, [r3]
004a55fc: cmp      r3, #0
004a5600: beq      #0x4a5670
004a5604: ldr      r7, [pc, #0x70]
004a5608: ldr      r2, [r5, r7]
004a560c: ldr      r2, [r2]
004a5610: cmp      r2, #0
004a5614: beq      #0x4a565c
004a5618: mov      r4, #0
004a561c: b        #0x4a5628
004a5620: ldr      r3, [r5, r6]
004a5624: ldr      r3, [r3]
004a5628: ldr      r0, [r3, r4, lsl #2]
004a562c: add      r4, r4, #1
004a5630: cmp      r0, #0
004a5634: beq      #0x4a5644
004a5638: bl       #0x310440
004a563c: ldr      r3, [r5, r6]
004a5640: ldr      r3, [r3]
004a5644: ldr      r2, [r5, r7]
004a5648: ldr      r2, [r2]
004a564c: cmp      r2, r4
004a5650: bhi      #0x4a5620
004a5654: cmp      r3, #0
004a5658: beq      #0x4a5664
004a565c: mov      r0, r3
004a5660: bl       #0x310440
004a5664: ldr      r3, [r5, r6]
004a5668: mov      r2, #0
004a566c: str      r2, [r3]
004a5670: pop      {r4, r5, r6, r7, r8, pc}
004a5674: subeq    pc, lr, r0, lsr #9
004a5678: andeq    r1, r0, ip, lsr r0
004a567c: andeq    r2, r0, r8, ror sp

# _ZN7Structs19GetMemberIDByStringINS_9SkillListEEEiPKc
004ad21c: ldr      r3, [pc, #0x20]
004ad220: ldr      r2, [pc, #0x20]
004ad224: push     {r4, lr}
004ad228: add      r3, pc, r3
004ad22c: ldr      r2, [r3, r2]
004ad230: ldr      r1, [r2, #0x14]
004ad234: bl       #0x30e31c
004ad238: cmp      r0, #0
004ad23c: mvnne    r0, #0
004ad240: pop      {r4, pc}
004ad244: subeq    r7, lr, r8, ror #16
004ad248: andeq    r3, r0, r0, ror pc

# _ZN7Structs9SkillList8finalizeEv
004d10e0: push     {r4, lr}
004d10e4: mov      r4, r0
004d10e8: ldr      r0, [r0, #8]
004d10ec: cmp      r0, #0
004d10f0: beq      #0x4d1104
004d10f4: bl       #0x310440
004d10f8: mov      r3, #0
004d10fc: str      r3, [r4, #4]
004d1100: str      r3, [r4, #8]
004d1104: pop      {r4, pc}

# _ZN6Arrays19GetMemberIDByStringINS_14SkillListTableEEEiPKc
004ad1a8: ldr      r3, [pc, #0x60]
004ad1ac: ldr      r2, [pc, #0x60]
004ad1b0: push     {r4, r5, r6, r7, r8, lr}
004ad1b4: add      r3, pc, r3
004ad1b8: ldr      r2, [r3, r2]
004ad1bc: mov      r6, r0
004ad1c0: ldr      r5, [r2]
004ad1c4: cmp      r5, #0
004ad1c8: beq      #0x4ad208
004ad1cc: ldr      r2, [pc, #0x44]
004ad1d0: mov      r4, #0
004ad1d4: ldr      r3, [r3, r2]
004ad1d8: ldr      r7, [r3]
004ad1dc: b        #0x4ad1ec
004ad1e0: add      r4, r4, #1
004ad1e4: cmp      r4, r5
004ad1e8: beq      #0x4ad208
004ad1ec: ldr      r1, [r7, r4, lsl #2]
004ad1f0: mov      r0, r6
004ad1f4: bl       #0x30e31c
004ad1f8: cmp      r0, #0
004ad1fc: bne      #0x4ad1e0
004ad200: mov      r0, r4
004ad204: pop      {r4, r5, r6, r7, r8, pc}
004ad208: mvn      r0, #0
004ad20c: pop      {r4, r5, r6, r7, r8, pc}
004ad210: ldrdeq   r7, r8, [lr], #-0x8c
004ad214: andeq    r2, r0, r8, ror sp
004ad218: andeq    r1, r0, ip, lsr r0

# _ZN12StreamReader6readAsIjEET_P11IStreamBase
00313a90: str      lr, [sp, #-4]!
00313a94: sub      sp, sp, #0x14
00313a98: mov      r3, #0
00313a9c: ldr      ip, [r0]
00313aa0: add      r1, sp, #0xc
00313aa4: mov      r2, #4
00313aa8: mov      lr, pc
00313aac: ldr      pc, [ip, #0x18]
00313ab0: ldr      r3, [pc, #0x78]
00313ab4: cmp      r0, #4
00313ab8: add      r3, pc, r3
00313abc: beq      #0x313af0
00313ac0: ldr      r2, [pc, #0x6c]
00313ac4: ldr      r2, [r3, r2]
00313ac8: ldr      r2, [r2]
00313acc: cmp      r2, #2
00313ad0: moveq    r3, #0
00313ad4: streq    r3, [r3]
00313ad8: beq      #0x313ae4
00313adc: cmp      r2, #1
00313ae0: beq      #0x313afc
00313ae4: ldr      r0, [sp, #0xc]
00313ae8: add      sp, sp, #0x14
00313aec: ldm      sp!, {pc}
00313af0: cmp      r1, #0
00313af4: beq      #0x313ae4
00313af8: b        #0x313ac0
00313afc: ldr      r0, [pc, #0x34]
00313b00: ldr      r1, [pc, #0x34]
00313b04: ldr      r2, [pc, #0x34]
00313b08: ldr      r0, [r3, r0]
00313b0c: ldr      r3, [pc, #0x30]
00313b10: mov      ip, #0x44
00313b14: add      r1, pc, r1
00313b18: add      r2, pc, r2
00313b1c: add      r3, pc, r3
00313b20: add      r0, r0, #0xa8
00313b24: str      ip, [sp]
00313b28: bl       #0x30e004
00313b2c: b        #0x313ae4

# _ZN12StreamReader6readAsIiEEvP11IStreamBasePT_
00459090: str      lr, [sp, #-4]!
00459094: mov      r3, #0
00459098: sub      sp, sp, #0xc
0045909c: ldr      ip, [r0]
004590a0: mov      r2, #4
004590a4: mov      lr, pc
004590a8: ldr      pc, [ip, #0x18]
004590ac: ldr      r3, [pc, #0x74]
004590b0: cmp      r0, #4
004590b4: add      r3, pc, r3
004590b8: beq      #0x4590e8
004590bc: ldr      r2, [pc, #0x68]
004590c0: ldr      r2, [r3, r2]
004590c4: ldr      r2, [r2]
004590c8: cmp      r2, #2
004590cc: moveq    r3, #0
004590d0: streq    r3, [r3]
004590d4: beq      #0x4590e0
004590d8: cmp      r2, #1
004590dc: beq      #0x4590f4
004590e0: add      sp, sp, #0xc
004590e4: ldm      sp!, {pc}
004590e8: cmp      r1, #0
004590ec: beq      #0x4590e0
004590f0: b        #0x4590bc
004590f4: ldr      r0, [pc, #0x34]
004590f8: ldr      r1, [pc, #0x34]
004590fc: ldr      r2, [pc, #0x34]
00459100: ldr      r0, [r3, r0]
00459104: ldr      r3, [pc, #0x30]
00459108: mov      ip, #0x50
0045910c: add      r1, pc, r1
00459110: add      r2, pc, r2
00459114: add      r3, pc, r3
00459118: add      r0, r0, #0xa8
0045911c: str      ip, [sp]
00459120: bl       #0x30e004
00459124: b        #0x4590e0
00459128: ldrsbeq  fp, [r3], #-0x9c
0045912c: andeq    r3, r0, r0, asr #19
00459130: andeq    r1, r0, r0, asr #19
00459134: subeq    r5, r6, ip, asr #5
00459138: strdeq   r5, r6, [r6], #-0x30
0045913c: subeq    r6, r6, ip, lsr #24
