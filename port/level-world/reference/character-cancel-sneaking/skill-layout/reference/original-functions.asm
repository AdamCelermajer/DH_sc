
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
