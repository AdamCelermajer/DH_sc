
# _ZN6Random9GetRandomEib.clone.1
003af6d8: push     {r4, lr}
003af6dc: ldr      r4, [pc, #0x7c]
003af6e0: cmp      r0, #0
003af6e4: add      r4, pc, r4
003af6e8: beq      #0x3af748
003af6ec: ldr      r2, [pc, #0x70]
003af6f0: mov      r1, r0
003af6f4: movw     r0, #0xe6ab
003af6f8: ldr      r2, [r4, r2]
003af6fc: movw     r3, #0xdb17
003af700: movt     r3, #0x2b52
003af704: ldr      lr, [r2]
003af708: movw     ip, #0xf26b
003af70c: movt     ip, #0xda
003af710: mul      r0, r0, lr
003af714: add      r0, r0, #0x2b000
003af718: add      r0, r0, #0x3fc
003af71c: add      r0, r0, #1
003af720: umull    lr, r3, r3, r0
003af724: rsb      lr, r3, r0
003af728: add      r3, r3, lr, lsr #1
003af72c: lsr      r3, r3, #0x17
003af730: mls      r3, ip, r3, r0
003af734: mov      r0, r3
003af738: str      r3, [r2]
003af73c: bl       #0x30eb2c
003af740: eor      r0, r1, r1, asr #31
003af744: sub      r0, r0, r1, asr #31
003af748: ldr      r3, [pc, #0x18]
003af74c: ldr      r3, [r4, r3]
003af750: ldr      r2, [r3]
003af754: add      r2, r2, #1
003af758: str      r2, [r3]
003af75c: pop      {r4, pc}
003af760: subseq   r5, lr, ip, lsr #7
003af764: muleq    r0, r4, ip
003af768: andeq    r1, r0, r8, lsl #1

# _Z12CF__CalcPushP9CharacterS0_ii
003b0d78: cmp      r3, #0
003b0d7c: push     {r4, r5, r6, r7, r8, sb, sl, lr}
003b0d80: mov      r6, r1
003b0d84: mov      r4, r2
003b0d88: beq      #0x3b0da8
003b0d8c: cmp      r3, #2
003b0d90: subne    r5, r2, #1
003b0d94: beq      #0x3b0e30
003b0d98: cmp      r5, r4
003b0d9c: movle    r0, #0
003b0da0: movgt    r0, #1
003b0da4: pop      {r4, r5, r6, r7, r8, sb, sl, pc}
003b0da8: add      r7, r0, #0xff0
003b0dac: add      r7, r7, #4
003b0db0: add      sl, r0, #0x560
003b0db4: mov      r0, sl
003b0db8: mov      r1, r7
003b0dbc: mov      r2, #0x89
003b0dc0: bl       #0x3dedb4
003b0dc4: subs     r5, r0, #0
003b0dc8: ble      #0x3b0d98
003b0dcc: mov      r1, r7
003b0dd0: mov      r0, sl
003b0dd4: mov      r2, #0x13
003b0dd8: bl       #0x3dedb4
003b0ddc: add      r8, r6, #0xff0
003b0de0: add      r8, r8, #4
003b0de4: add      r6, r6, #0x560
003b0de8: mov      r1, r8
003b0dec: mov      r7, r0
003b0df0: mov      r2, #0x88
003b0df4: mov      r0, r6
003b0df8: bl       #0x3dedb4
003b0dfc: add      r7, r7, r7, lsl #2
003b0e00: rsb      r7, r0, r7
003b0e04: mov      r1, r8
003b0e08: mov      r0, r6
003b0e0c: mov      r2, #0x13
003b0e10: bl       #0x3dedb4
003b0e14: add      r5, r7, r5
003b0e18: add      r0, r0, r0, lsl #2
003b0e1c: rsb      r5, r0, r5
003b0e20: cmp      r5, r4
003b0e24: movle    r0, #0
003b0e28: movgt    r0, #1
003b0e2c: pop      {r4, r5, r6, r7, r8, sb, sl, pc}
003b0e30: add      sl, r0, #0x560
003b0e34: mov      r0, sl
003b0e38: mov      r1, #0xb7
003b0e3c: bl       #0x3af76c
003b0e40: subs     r5, r0, #0
003b0e44: ble      #0x3b0d98
003b0e48: mov      r1, #0x13
003b0e4c: mov      r0, sl
003b0e50: bl       #0x3af76c
003b0e54: add      r6, r6, #0x560
003b0e58: mov      r7, r0
003b0e5c: mov      r1, #0x88
003b0e60: mov      r0, r6
003b0e64: bl       #0x3af76c
003b0e68: add      r7, r7, r7, lsl #2
003b0e6c: rsb      r7, r0, r7
003b0e70: mov      r1, #0x13
003b0e74: mov      r0, r6
003b0e78: bl       #0x3af76c
003b0e7c: add      r5, r7, r5
003b0e80: add      r0, r0, r0, lsl #2
003b0e84: rsb      r5, r0, r5
003b0e88: b        #0x3b0d98

# _Z19CF__CalcMissOrDodgeP9CharacterS0_iiib
003b1df0: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
003b1df4: mov      r5, r3
003b1df8: sub      sp, sp, #0xc
003b1dfc: mov      sb, r0
003b1e00: mov      r0, #0x64
003b1e04: mov      r4, r2
003b1e08: mov      r6, r1
003b1e0c: ldr      sl, [sp, #0x30]
003b1e10: ldrb     r7, [sp, #0x34]
003b1e14: bl       #0x3af6d8
003b1e18: cmp      r5, #0
003b1e1c: add      r1, r4, #1
003b1e20: beq      #0x3b1e88
003b1e24: cmp      r5, #2
003b1e28: subne    r2, r4, #1
003b1e2c: movne    r3, r1
003b1e30: beq      #0x3b1e88
003b1e34: cmp      r3, r4
003b1e38: ble      #0x3b1e98
003b1e3c: cmp      r2, r4
003b1e40: bgt      #0x3b1e7c
003b1e44: cmp      r5, #0
003b1e48: lsl      r8, r0, #8
003b1e4c: beq      #0x3b1eb4
003b1e50: cmp      r5, #2
003b1e54: movne    sb, r1
003b1e58: beq      #0x3b1f50
003b1e5c: add      r3, r8, #1
003b1e60: cmp      r4, sb
003b1e64: bge      #0x3b1e98
003b1e68: cmp      r5, #0
003b1e6c: bne      #0x3b1e7c
003b1e70: cmp      r3, r8
003b1e74: movgt    r3, #1
003b1e78: bgt      #0x3b1ea0
003b1e7c: mov      r3, #0
003b1e80: mov      r5, r3
003b1e84: b        #0x3b1ea0
003b1e88: mov      r3, #0x6200
003b1e8c: cmp      r3, r4
003b1e90: mov      r2, #0x3200
003b1e94: bgt      #0x3b1e3c
003b1e98: mov      r3, #0
003b1e9c: mov      r5, #1
003b1ea0: mov      r0, #0
003b1ea4: bfi      r0, r5, #0, #8
003b1ea8: bfi      r0, r3, #8, #8
003b1eac: add      sp, sp, #0xc
003b1eb0: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
003b1eb4: add      r1, sb, #0xff0
003b1eb8: add      sb, sb, #0x560
003b1ebc: mov      r2, #0x32
003b1ec0: add      r1, r1, #4
003b1ec4: mov      r0, sb
003b1ec8: bl       #0x3dedb4
003b1ecc: mov      r1, r7
003b1ed0: cmp      r0, #0x100
003b1ed4: movge    fp, r0
003b1ed8: movlt    fp, #0x100
003b1edc: mov      r0, sb
003b1ee0: bl       #0x3df81c
003b1ee4: add      sl, r6, #0xff0
003b1ee8: add      sl, sl, #4
003b1eec: add      r6, r6, #0x560
003b1ef0: mov      sb, r0
003b1ef4: mov      r1, sl
003b1ef8: mov      r2, #0x3b
003b1efc: mov      r0, r6
003b1f00: bl       #0x3dedb4
003b1f04: ldr      r7, [pc, #0xa4]
003b1f08: rsb      fp, r0, fp
003b1f0c: mov      r2, #0x33
003b1f10: add      r7, pc, r7
003b1f14: ldr      r3, [r7, #0x24]
003b1f18: add      sb, fp, sb
003b1f1c: mul      sb, r2, sb
003b1f20: lsl      r3, r3, #9
003b1f24: mov      r0, r6
003b1f28: asr      r3, r3, #8
003b1f2c: add      r3, r3, #0x4b00
003b1f30: mov      r1, sl
003b1f34: mov      r2, #0x3c
003b1f38: add      sb, r3, sb, asr #8
003b1f3c: bl       #0x3dedb4
003b1f40: ldr      r3, [r7, #0x28]
003b1f44: sbfx     r3, r3, #0, #0x18
003b1f48: add      r3, r0, r3
003b1f4c: b        #0x3b1e60
003b1f50: add      sb, sb, #0x560
003b1f54: mov      r0, sb
003b1f58: mov      r1, #0x9e
003b1f5c: bl       #0x3af76c
003b1f60: cmn      sl, #1
003b1f64: mov      r7, r0
003b1f68: beq      #0x3b1f7c
003b1f6c: mov      r0, sb
003b1f70: add      r1, sl, #0x9f
003b1f74: bl       #0x3af76c
003b1f78: add      r7, r7, r0
003b1f7c: add      r0, r6, #0x560
003b1f80: mov      r1, #0xa4
003b1f84: bl       #0x3af76c
003b1f88: ldr      r3, [pc, #0x24]
003b1f8c: rsb      sb, r0, r7
003b1f90: lsl      sb, sb, #9
003b1f94: add      r3, pc, r3
003b1f98: ldr      r3, [r3, #0x24]
003b1f9c: lsl      r3, r3, #9
003b1fa0: asr      r3, r3, #8
003b1fa4: add      r3, r3, #0x4b00
003b1fa8: add      sb, r3, sb, asr #8
003b1fac: b        #0x3b1e5c
003b1fb0: subseq   r0, pc, r8, lsl #21
003b1fb4: subseq   r0, pc, r4, lsl #20

# _Z12CF__CalcStunP9CharacterS0_ii
003b0fa0: cmp      r3, #0
003b0fa4: push     {r4, r5, r6, r7, r8, sb, sl, lr}
003b0fa8: mov      r6, r1
003b0fac: mov      r4, r2
003b0fb0: beq      #0x3b0fd0
003b0fb4: cmp      r3, #2
003b0fb8: subne    r5, r2, #1
003b0fbc: beq      #0x3b1058
003b0fc0: cmp      r5, r4
003b0fc4: movle    r0, #0
003b0fc8: movgt    r0, #1
003b0fcc: pop      {r4, r5, r6, r7, r8, sb, sl, pc}
003b0fd0: add      r7, r0, #0xff0
003b0fd4: add      r7, r7, #4
003b0fd8: add      sl, r0, #0x560
003b0fdc: mov      r0, sl
003b0fe0: mov      r1, r7
003b0fe4: mov      r2, #0x8b
003b0fe8: bl       #0x3dedb4
003b0fec: subs     r5, r0, #0
003b0ff0: ble      #0x3b0fc0
003b0ff4: mov      r1, r7
003b0ff8: mov      r0, sl
003b0ffc: mov      r2, #0x13
003b1000: bl       #0x3dedb4
003b1004: add      r8, r6, #0xff0
003b1008: add      r8, r8, #4
003b100c: add      r6, r6, #0x560
003b1010: mov      r1, r8
003b1014: mov      r7, r0
003b1018: mov      r2, #0x8a
003b101c: mov      r0, r6
003b1020: bl       #0x3dedb4
003b1024: add      r7, r7, r7, lsl #2
003b1028: rsb      r7, r0, r7
003b102c: mov      r1, r8
003b1030: mov      r0, r6
003b1034: mov      r2, #0x13
003b1038: bl       #0x3dedb4
003b103c: add      r5, r7, r5
003b1040: add      r0, r0, r0, lsl #2
003b1044: rsb      r5, r0, r5
003b1048: cmp      r5, r4
003b104c: movle    r0, #0
003b1050: movgt    r0, #1
003b1054: pop      {r4, r5, r6, r7, r8, sb, sl, pc}
003b1058: add      sl, r0, #0x560
003b105c: mov      r0, sl
003b1060: mov      r1, #0xb8
003b1064: bl       #0x3af76c
003b1068: subs     r5, r0, #0
003b106c: ble      #0x3b0fc0
003b1070: mov      r1, #0x13
003b1074: mov      r0, sl
003b1078: bl       #0x3af76c
003b107c: add      r6, r6, #0x560
003b1080: mov      r7, r0
003b1084: mov      r1, #0x8a
003b1088: mov      r0, r6
003b108c: bl       #0x3af76c
003b1090: add      r7, r7, r7, lsl #2
003b1094: rsb      r7, r0, r7
003b1098: mov      r1, #0x13
003b109c: mov      r0, r6
003b10a0: bl       #0x3af76c
003b10a4: add      r5, r7, r5
003b10a8: add      r0, r0, r0, lsl #2
003b10ac: rsb      r5, r0, r5
003b10b0: b        #0x3b0fc0

# _Z12CF__CalcFearP9CharacterS0_ii
003b0c64: cmp      r3, #0
003b0c68: push     {r4, r5, r6, r7, r8, sb, sl, lr}
003b0c6c: mov      r6, r1
003b0c70: mov      r4, r2
003b0c74: beq      #0x3b0c94
003b0c78: cmp      r3, #2
003b0c7c: subne    r5, r2, #1
003b0c80: beq      #0x3b0d1c
003b0c84: cmp      r5, r4
003b0c88: movle    r0, #0
003b0c8c: movgt    r0, #1
003b0c90: pop      {r4, r5, r6, r7, r8, sb, sl, pc}
003b0c94: add      r7, r0, #0xff0
003b0c98: add      r7, r7, #4
003b0c9c: add      sl, r0, #0x560
003b0ca0: mov      r0, sl
003b0ca4: mov      r1, r7
003b0ca8: mov      r2, #0x8e
003b0cac: bl       #0x3dedb4
003b0cb0: subs     r5, r0, #0
003b0cb4: ble      #0x3b0c84
003b0cb8: mov      r1, r7
003b0cbc: mov      r0, sl
003b0cc0: mov      r2, #0x13
003b0cc4: bl       #0x3dedb4
003b0cc8: add      r8, r6, #0xff0
003b0ccc: add      r8, r8, #4
003b0cd0: add      r6, r6, #0x560
003b0cd4: mov      r1, r8
003b0cd8: mov      r7, r0
003b0cdc: mov      r2, #0x8d
003b0ce0: mov      r0, r6
003b0ce4: bl       #0x3dedb4
003b0ce8: add      r7, r7, r7, lsl #2
003b0cec: rsb      r7, r0, r7
003b0cf0: mov      r1, r8
003b0cf4: mov      r0, r6
003b0cf8: mov      r2, #0x13
003b0cfc: bl       #0x3dedb4
003b0d00: add      r5, r7, r5
003b0d04: add      r0, r0, r0, lsl #2
003b0d08: rsb      r5, r0, r5
003b0d0c: cmp      r5, r4
003b0d10: movle    r0, #0
003b0d14: movgt    r0, #1
003b0d18: pop      {r4, r5, r6, r7, r8, sb, sl, pc}
003b0d1c: add      sl, r0, #0x560
003b0d20: mov      r0, sl
003b0d24: mov      r1, #0xba
003b0d28: bl       #0x3af76c
003b0d2c: subs     r5, r0, #0
003b0d30: ble      #0x3b0c84
003b0d34: mov      r1, #0x13
003b0d38: mov      r0, sl
003b0d3c: bl       #0x3af76c
003b0d40: add      r6, r6, #0x560
003b0d44: mov      r7, r0
003b0d48: mov      r1, #0x8d
003b0d4c: mov      r0, r6
003b0d50: bl       #0x3af76c
003b0d54: add      r7, r7, r7, lsl #2
003b0d58: rsb      r7, r0, r7
003b0d5c: mov      r1, #0x13
003b0d60: mov      r0, r6
003b0d64: bl       #0x3af76c
003b0d68: add      r5, r7, r5
003b0d6c: add      r0, r0, r0, lsl #2
003b0d70: rsb      r5, r0, r5
003b0d74: b        #0x3b0c84

# _Z12CF__CalcCritP9CharacterS0_ii
003b0868: cmp      r3, #0
003b086c: push     {r4, r5, r6, lr}
003b0870: mov      r4, r2
003b0874: add      r5, r2, #1
003b0878: beq      #0x3b08b8
003b087c: cmp      r3, #2
003b0880: subne    r3, r2, #1
003b0884: beq      #0x3b08e8
003b0888: cmp      r5, r4
003b088c: ble      #0x3b08b0
003b0890: cmp      r3, r4
003b0894: ble      #0x3b08b0
003b0898: ldr      r3, [pc, #0x6c]
003b089c: mov      r2, #1
003b08a0: mov      r0, r2
003b08a4: add      r3, pc, r3
003b08a8: strb     r2, [r3, #0x33]
003b08ac: pop      {r4, r5, r6, pc}
003b08b0: mov      r0, #0
003b08b4: pop      {r4, r5, r6, pc}
003b08b8: add      r1, r0, #0xff0
003b08bc: add      r1, r1, #4
003b08c0: add      r0, r0, #0x560
003b08c4: mov      r2, #0x3f
003b08c8: bl       #0x3dedb4
003b08cc: ldr      r3, [pc, #0x3c]
003b08d0: mov      r5, #0x6200
003b08d4: add      r3, pc, r3
003b08d8: ldr      r3, [r3, #0x24]
003b08dc: sbfx     r3, r3, #0, #0x18
003b08e0: add      r3, r0, r3
003b08e4: b        #0x3b0888
003b08e8: add      r0, r0, #0x560
003b08ec: mov      r1, #0xa5
003b08f0: bl       #0x3af76c
003b08f4: ldr      r3, [pc, #0x18]
003b08f8: add      r3, pc, r3
003b08fc: ldr      r3, [r3, #0x24]
003b0900: sbfx     r3, r3, #0, #0x18
003b0904: add      r3, r0, r3
003b0908: b        #0x3b0888
003b090c: ldrsheq  r2, [pc], #-4
003b0910: subseq   r2, pc, r4, asr #1
003b0914: subseq   r2, pc, r0, lsr #1

# _Z12CF__CalcHurtP9CharacterS0_ii
003b0e8c: cmp      r3, #0
003b0e90: push     {r4, r5, r6, r7, r8, sb, sl, lr}
003b0e94: mov      r6, r1
003b0e98: mov      r4, r2
003b0e9c: beq      #0x3b0ebc
003b0ea0: cmp      r3, #2
003b0ea4: subne    r5, r2, #1
003b0ea8: beq      #0x3b0f44
003b0eac: cmp      r5, r4
003b0eb0: movle    r0, #0
003b0eb4: movgt    r0, #1
003b0eb8: pop      {r4, r5, r6, r7, r8, sb, sl, pc}
003b0ebc: add      r7, r0, #0xff0
003b0ec0: add      r7, r7, #4
003b0ec4: add      sl, r0, #0x560
003b0ec8: mov      r0, sl
003b0ecc: mov      r1, r7
003b0ed0: mov      r2, #0x87
003b0ed4: bl       #0x3dedb4
003b0ed8: subs     r5, r0, #0
003b0edc: ble      #0x3b0eac
003b0ee0: mov      r1, r7
003b0ee4: mov      r0, sl
003b0ee8: mov      r2, #0x13
003b0eec: bl       #0x3dedb4
003b0ef0: add      r8, r6, #0xff0
003b0ef4: add      r8, r8, #4
003b0ef8: add      r6, r6, #0x560
003b0efc: mov      r1, r8
003b0f00: mov      r7, r0
003b0f04: mov      r2, #0x86
003b0f08: mov      r0, r6
003b0f0c: bl       #0x3dedb4
003b0f10: add      r7, r7, r7, lsl #2
003b0f14: rsb      r7, r0, r7
003b0f18: mov      r1, r8
003b0f1c: mov      r0, r6
003b0f20: mov      r2, #0x13
003b0f24: bl       #0x3dedb4
003b0f28: add      r5, r7, r5
003b0f2c: add      r0, r0, r0, lsl #2
003b0f30: rsb      r5, r0, r5
003b0f34: cmp      r5, r4
003b0f38: movle    r0, #0
003b0f3c: movgt    r0, #1
003b0f40: pop      {r4, r5, r6, r7, r8, sb, sl, pc}
003b0f44: add      sl, r0, #0x560
003b0f48: mov      r0, sl
003b0f4c: mov      r1, #0xb6
003b0f50: bl       #0x3af76c
003b0f54: subs     r5, r0, #0
003b0f58: ble      #0x3b0eac
003b0f5c: mov      r1, #0x13
003b0f60: mov      r0, sl
003b0f64: bl       #0x3af76c
003b0f68: add      r6, r6, #0x560
003b0f6c: mov      r7, r0
003b0f70: mov      r1, #0x86
003b0f74: mov      r0, r6
003b0f78: bl       #0x3af76c
003b0f7c: add      r7, r7, r7, lsl #2
003b0f80: rsb      r7, r0, r7
003b0f84: mov      r1, #0x13
003b0f88: mov      r0, r6
003b0f8c: bl       #0x3af76c
003b0f90: add      r5, r7, r5
003b0f94: add      r0, r0, r0, lsl #2
003b0f98: rsb      r5, r0, r5
003b0f9c: b        #0x3b0eac

# _Z12CF__CalcSlowP9CharacterS0_ii
003b0ad4: cmp      r3, #0
003b0ad8: push     {r4, r5, r6, r7, r8, lr}
003b0adc: mov      r6, r1
003b0ae0: mov      r4, r2
003b0ae4: beq      #0x3b0b04
003b0ae8: cmp      r3, #2
003b0aec: subne    r5, r2, #1
003b0af0: beq      #0x3b0be0
003b0af4: cmp      r5, r4
003b0af8: movle    r0, #0
003b0afc: movgt    r0, #1
003b0b00: pop      {r4, r5, r6, r7, r8, pc}
003b0b04: add      r7, r0, #0xff0
003b0b08: add      r7, r7, #4
003b0b0c: add      r8, r0, #0x560
003b0b10: mov      r0, r8
003b0b14: mov      r1, r7
003b0b18: mov      r2, #0x91
003b0b1c: bl       #0x3dedb4
003b0b20: subs     r5, r0, #0
003b0b24: ble      #0x3b0af4
003b0b28: mov      r2, #0x13
003b0b2c: mov      r1, r7
003b0b30: mov      r0, r8
003b0b34: bl       #0x3dedb4
003b0b38: mov      r8, r0
003b0b3c: mov      r0, r5
003b0b40: bl       #0x30e964
003b0b44: mov      r7, r0
003b0b48: mov      r0, r8
003b0b4c: bl       #0x30e964
003b0b50: mov      r1, #0x3e000000
003b0b54: bl       #0x30ed6c
003b0b58: mov      r1, r0
003b0b5c: mov      r0, r7
003b0b60: bl       #0x30eba4
003b0b64: bl       #0x30e4cc
003b0b68: add      r5, r6, #0xff0
003b0b6c: add      r5, r5, #4
003b0b70: add      r6, r6, #0x560
003b0b74: mov      r8, r0
003b0b78: mov      r2, #0x90
003b0b7c: mov      r0, r6
003b0b80: mov      r1, r5
003b0b84: bl       #0x3dedb4
003b0b88: mov      r1, r5
003b0b8c: mov      r7, r0
003b0b90: mov      r2, #0x13
003b0b94: mov      r0, r6
003b0b98: bl       #0x3dedb4
003b0b9c: mov      r6, r0
003b0ba0: rsb      r0, r7, r8
003b0ba4: bl       #0x30e964
003b0ba8: mov      r5, r0
003b0bac: mov      r0, r6
003b0bb0: bl       #0x30e964
003b0bb4: mov      r1, #0x3e000000
003b0bb8: bl       #0x30ed6c
003b0bbc: mov      r1, r0
003b0bc0: mov      r0, r5
003b0bc4: bl       #0x30e3ac
003b0bc8: bl       #0x30e4cc
003b0bcc: mov      r5, r0
003b0bd0: cmp      r5, r4
003b0bd4: movle    r0, #0
003b0bd8: movgt    r0, #1
003b0bdc: pop      {r4, r5, r6, r7, r8, pc}
003b0be0: add      r8, r0, #0x560
003b0be4: mov      r0, r8
003b0be8: mov      r1, #0xbc
003b0bec: bl       #0x3af76c
003b0bf0: subs     r5, r0, #0
003b0bf4: ble      #0x3b0af4
003b0bf8: mov      r1, #0x13
003b0bfc: mov      r0, r8
003b0c00: bl       #0x3af76c
003b0c04: mov      r7, r0
003b0c08: mov      r0, r5
003b0c0c: bl       #0x30e964
003b0c10: mov      r5, r0
003b0c14: mov      r0, r7
003b0c18: bl       #0x30e964
003b0c1c: mov      r1, #0x3e000000
003b0c20: bl       #0x30ed6c
003b0c24: mov      r1, r0
003b0c28: mov      r0, r5
003b0c2c: bl       #0x30eba4
003b0c30: bl       #0x30e4cc
003b0c34: add      r6, r6, #0x560
003b0c38: mov      r7, r0
003b0c3c: mov      r1, #0x90
003b0c40: mov      r0, r6
003b0c44: bl       #0x3af76c
003b0c48: mov      r1, #0x13
003b0c4c: mov      r5, r0
003b0c50: mov      r0, r6
003b0c54: bl       #0x3af76c
003b0c58: mov      r6, r0
003b0c5c: rsb      r0, r5, r7
003b0c60: b        #0x3b0ba4

# _Z16CF_SetCombatantsP9CharacterS0_ibb
003b059c: push     {r4, r5, r6, r7, r8, sb, sl, lr}
003b05a0: ldr      r4, [pc, #0x70]
003b05a4: mov      r5, r1
003b05a8: add      r1, r0, #0xff0
003b05ac: add      r4, pc, r4
003b05b0: mov      r6, r2
003b05b4: add      r1, r1, #4
003b05b8: str      r0, [r4, #0x1c]
003b05bc: mov      r2, #0x13
003b05c0: str      r5, [r4, #0x20]
003b05c4: add      r0, r0, #0x560
003b05c8: mov      r8, r3
003b05cc: ldrb     r7, [sp, #0x20]
003b05d0: bl       #0x3dedb4
003b05d4: add      r1, r5, #0xff0
003b05d8: mov      sl, r0
003b05dc: mov      r2, #0x13
003b05e0: add      r1, r1, #4
003b05e4: add      r0, r5, #0x560
003b05e8: bl       #0x3dedb4
003b05ec: rsb      r0, r0, sl
003b05f0: mov      r3, #0
003b05f4: rsb      r2, r0, #0
003b05f8: strb     r3, [r4, #0x33]
003b05fc: str      r2, [r4, #0x28]
003b0600: str      r6, [r4, #0x2c]
003b0604: strb     r8, [r4, #0x30]
003b0608: strb     r7, [r4, #0x31]
003b060c: str      r0, [r4, #0x24]
003b0610: strb     r3, [r4, #0x32]
003b0614: pop      {r4, r5, r6, r7, r8, sb, sl, pc}
003b0618: subseq   r2, pc, ip, ror #7

# _Z13CF__CalcBlockP9CharacterS0_ii
003b0918: cmp      r3, #0
003b091c: push     {r4, r5, r6, r7, r8, lr}
003b0920: addne    r3, r2, #1
003b0924: mov      r5, r1
003b0928: mov      r4, r2
003b092c: subne    r7, r2, #1
003b0930: beq      #0x3b0964
003b0934: cmp      r3, r4
003b0938: ble      #0x3b095c
003b093c: cmp      r7, r4
003b0940: ble      #0x3b095c
003b0944: ldr      r3, [pc, #0x70]
003b0948: mov      r2, #1
003b094c: mov      r0, r2
003b0950: add      r3, pc, r3
003b0954: strb     r2, [r3, #0x32]
003b0958: pop      {r4, r5, r6, r7, r8, pc}
003b095c: mov      r0, #0
003b0960: pop      {r4, r5, r6, r7, r8, pc}
003b0964: add      r6, r1, #0x560
003b0968: add      r1, r1, #0xff0
003b096c: add      r1, r1, #4
003b0970: mov      r2, #0x3d
003b0974: mov      r0, r6
003b0978: bl       #0x3dedb4
003b097c: ldr      r3, [pc, #0x3c]
003b0980: add      r3, pc, r3
003b0984: ldr      r7, [r3, #0x28]
003b0988: sbfx     r7, r7, #0, #0x18
003b098c: add      r7, r0, r7
003b0990: add      r0, r5, #0x37c
003b0994: bl       #0x400110
003b0998: cmp      r0, #0
003b099c: moveq    r3, #0x6200
003b09a0: beq      #0x3b0934
003b09a4: mov      r0, r6
003b09a8: mov      r1, #0x3e
003b09ac: bl       #0x3af76c
003b09b0: mov      r3, #0x6200
003b09b4: add      r7, r7, r0
003b09b8: b        #0x3b0934
003b09bc: subseq   r2, pc, r8, asr #32
003b09c0: subseq   r2, pc, r8, lsl r0

# _Z14CF__CalcDamageP9CharacterS0_iiibb
003b1fb8: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
003b1fbc: sub      sp, sp, #0x2c
003b1fc0: ldr      ip, [sp, #0x50]
003b1fc4: mov      r4, r0
003b1fc8: mov      r5, r1
003b1fcc: cmp      ip, #1
003b1fd0: mov      sb, r2
003b1fd4: ldrb     fp, [sp, #0x58]
003b1fd8: ldrb     r8, [sp, #0x5c]
003b1fdc: bls      #0x3b201c
003b1fe0: cmp      ip, #2
003b1fe4: beq      #0x3b2234
003b1fe8: cmp      ip, #3
003b1fec: beq      #0x3b23f0
003b1ff0: mov      r3, #0
003b1ff4: str      r3, [r0, #0x18]
003b1ff8: str      r3, [r0]
003b1ffc: str      r3, [r0, #4]
003b2000: str      r3, [r0, #8]
003b2004: str      r3, [r0, #0xc]
003b2008: str      r3, [r0, #0x10]
003b200c: str      r3, [r0, #0x14]
003b2010: mov      r0, r4
003b2014: add      sp, sp, #0x2c
003b2018: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
003b201c: cmp      fp, #0
003b2020: bne      #0x3b23ac
003b2024: add      r7, r1, #0xff0
003b2028: add      r6, r1, #0x560
003b202c: add      r7, r7, #4
003b2030: mov      r2, #0x4f
003b2034: mov      r1, r7
003b2038: mov      r0, r6
003b203c: bl       #0x3dedb4
003b2040: mov      r1, r7
003b2044: mov      r2, #0x50
003b2048: mov      sl, r0
003b204c: mov      r0, r6
003b2050: bl       #0x3dedb4
003b2054: mov      r1, r7
003b2058: mov      ip, r0
003b205c: mov      r2, #0x61
003b2060: mov      r0, r6
003b2064: str      ip, [sp]
003b2068: bl       #0x3dedb4
003b206c: cmp      r8, #0
003b2070: asr      r0, r0, #8
003b2074: str      r0, [sp, #0x10]
003b2078: ldr      ip, [sp]
003b207c: bne      #0x3b256c
003b2080: bic      r3, sl, sl, asr #31
003b2084: bic      ip, ip, ip, asr #31
003b2088: rsb      r0, r3, ip
003b208c: str      ip, [sp, #0x14]
003b2090: str      r3, [sp]
003b2094: bl       #0x3af6d8
003b2098: mov      r1, fp
003b209c: mov      sl, r0
003b20a0: mov      r0, r6
003b20a4: bl       #0x3df8ac
003b20a8: ldr      r3, [sp]
003b20ac: add      sl, sl, r0
003b20b0: add      r0, sb, #0x4f0
003b20b4: add      sl, sl, r3
003b20b8: add      r0, r0, #0xc
003b20bc: str      sl, [sp, #4]
003b20c0: bl       #0x3c01ac
003b20c4: cmp      r0, #9
003b20c8: beq      #0x3b25fc
003b20cc: add      r2, sb, #0xff0
003b20d0: add      r2, r2, #4
003b20d4: add      r3, sb, #0x560
003b20d8: str      r2, [sp, #8]
003b20dc: mov      r1, r7
003b20e0: mov      r2, #0xc6
003b20e4: mov      r0, r6
003b20e8: str      r3, [sp, #0xc]
003b20ec: bl       #0x3dedb4
003b20f0: ldr      r1, [sp, #8]
003b20f4: mov      sl, r0
003b20f8: mov      r2, #0xc7
003b20fc: ldr      r0, [sp, #0xc]
003b2100: bl       #0x3dedb4
003b2104: cmp      sl, r0
003b2108: bgt      #0x3b2550
003b210c: movw     r3, #0x14d0
003b2110: ldrh     sl, [r5, r3]
003b2114: cmp      sl, #0
003b2118: beq      #0x3b2138
003b211c: mov      r2, #0x5e
003b2120: mov      r0, r6
003b2124: mov      r1, r7
003b2128: bl       #0x3dedb4
003b212c: ldr      r2, [sp, #4]
003b2130: mla      r2, sl, r0, r2
003b2134: str      r2, [sp, #4]
003b2138: ldr      r3, [pc, #0x4ec]
003b213c: add      r3, pc, r3
003b2140: ldrb     r3, [r3, #0x32]
003b2144: cmp      r3, #0
003b2148: bne      #0x3b2530
003b214c: ldr      r3, [pc, #0x4dc]
003b2150: mov      r2, #0x47
003b2154: mov      sl, #0x19
003b2158: add      r3, pc, r3
003b215c: ldrb     r3, [r3, #0x33]
003b2160: cmp      r3, #0
003b2164: ldrne    r0, [sp, #4]
003b2168: ldrne    r1, [sp, #0x14]
003b216c: addne    r0, r0, r1
003b2170: strne    r0, [sp, #4]
003b2174: ldr      r1, [sp, #8]
003b2178: ldr      r0, [sp, #0xc]
003b217c: bl       #0x3dedb4
003b2180: ldr      r2, [sp, #4]
003b2184: mul      sl, sl, r0
003b2188: sub      sl, r2, sl, asr #8
003b218c: cmp      sl, #0
003b2190: movle    sl, #0x100
003b2194: cmp      r8, #0
003b2198: movne    r3, #0
003b219c: strne    r3, [sp, #4]
003b21a0: strne    r3, [sp, #8]
003b21a4: beq      #0x3b24d4
003b21a8: ldr      r2, [sp, #0x10]
003b21ac: cmn      r2, #1
003b21b0: beq      #0x3b21ec
003b21b4: cmp      fp, #0
003b21b8: beq      #0x3b2454
003b21bc: mov      r2, #0x62
003b21c0: mov      r1, r7
003b21c4: mov      r0, r6
003b21c8: bl       #0x3dedb4
003b21cc: mov      r1, r7
003b21d0: mov      fp, r0
003b21d4: mov      r2, #0x63
003b21d8: mov      r0, r6
003b21dc: bl       #0x3dedb4
003b21e0: cmp      r0, #0
003b21e4: cmpge    fp, #0
003b21e8: bge      #0x3b247c
003b21ec: mov      r1, r5
003b21f0: mov      r2, sb
003b21f4: mov      r3, r8
003b21f8: add      r0, sp, #0x1c
003b21fc: bl       #0x3b09c4
003b2200: add      r1, sp, #0x1c
003b2204: ldm      r1, {r1, r2, r3}
003b2208: str      sl, [r4]
003b220c: ldr      r0, [sp, #0x10]
003b2210: str      r0, [r4, #4]
003b2214: ldr      r0, [sp, #8]
003b2218: str      r0, [r4, #8]
003b221c: ldr      r0, [sp, #4]
003b2220: str      r1, [r4, #0x10]
003b2224: str      r2, [r4, #0x14]
003b2228: str      r0, [r4, #0xc]
003b222c: str      r3, [r4, #0x18]
003b2230: b        #0x3b2010
003b2234: cmp      r8, #0
003b2238: bne      #0x3b2414
003b223c: add      r7, r1, #0xff0
003b2240: add      r6, r1, #0x560
003b2244: add      r7, r7, #4
003b2248: mov      r2, #0x4f
003b224c: mov      r1, r7
003b2250: mov      r0, r6
003b2254: bl       #0x3dedb4
003b2258: mov      r1, r7
003b225c: mov      r3, r0
003b2260: mov      r2, #0x50
003b2264: mov      r0, r6
003b2268: str      r3, [sp]
003b226c: bl       #0x3dedb4
003b2270: mov      r1, r7
003b2274: mov      sl, r0
003b2278: mov      r2, #0x61
003b227c: mov      r0, r6
003b2280: bl       #0x3dedb4
003b2284: ldr      r3, [sp]
003b2288: asr      r0, r0, #8
003b228c: str      r0, [sp, #0x54]
003b2290: orrs     r7, sl, r3
003b2294: beq      #0x3b2364
003b2298: bic      r7, r3, r3, asr #31
003b229c: bic      sl, sl, sl, asr #31
003b22a0: rsb      r0, r7, sl
003b22a4: str      sl, [sp, #8]
003b22a8: bl       #0x3af6d8
003b22ac: cmp      r8, #0
003b22b0: add      sl, r0, r7
003b22b4: beq      #0x3b2618
003b22b8: ldr      r1, [sp, #0x54]
003b22bc: cmn      r1, #1
003b22c0: addeq    fp, sb, #0x560
003b22c4: beq      #0x3b2324
003b22c8: ldr      r2, [sp, #0x54]
003b22cc: add      fp, sb, #0x560
003b22d0: mov      r0, fp
003b22d4: add      r1, r2, #0x4a
003b22d8: bl       #0x3af76c
003b22dc: movw     r3, #0x851f
003b22e0: asr      r7, r0, #8
003b22e4: movt     r3, #0x51eb
003b22e8: smull    r1, r3, r3, r7
003b22ec: asr      r7, r0, #0x1f
003b22f0: sub      r7, r7, r3, asr #5
003b22f4: adds     r7, r7, #1
003b22f8: bmi      #0x3b25e4
003b22fc: ldr      r2, [sp, #0x54]
003b2300: mov      r0, r6
003b2304: cmp      r7, #1
003b2308: movge    r7, #1
003b230c: add      r1, r2, #0xa6
003b2310: bl       #0x3af76c
003b2314: add      sl, r0, sl
003b2318: mul      sl, r7, sl
003b231c: cmp      sl, #0
003b2320: movle    sl, #0x100
003b2324: ldr      r3, [pc, #0x308]
003b2328: add      r1, sb, #0xff0
003b232c: mov      r0, fp
003b2330: add      r3, pc, r3
003b2334: ldrb     r3, [r3, #0x33]
003b2338: add      r1, r1, #4
003b233c: mov      r2, #0x49
003b2340: cmp      r3, #0
003b2344: ldrne    r3, [sp, #8]
003b2348: mov      r7, #0x33
003b234c: addne    sl, sl, r3
003b2350: bl       #0x3dedb4
003b2354: mul      r7, r7, r0
003b2358: sub      r7, sl, r7, asr #8
003b235c: cmp      r7, #0
003b2360: movle    r7, #0x100
003b2364: cmp      r8, #0
003b2368: beq      #0x3b2590
003b236c: mov      r1, r5
003b2370: mov      r2, sb
003b2374: mov      r3, #1
003b2378: add      r0, sp, #0x1c
003b237c: bl       #0x3b09c4
003b2380: add      r2, sp, #0x1c
003b2384: ldm      r2, {r2, r3, r8}
003b2388: mov      sl, #0
003b238c: mov      r5, sl
003b2390: mov      r1, #0
003b2394: str      r7, [r4]
003b2398: stmib    r4, {r1, r5, sl}
003b239c: str      r2, [r4, #0x10]
003b23a0: str      r3, [r4, #0x14]
003b23a4: str      r8, [r4, #0x18]
003b23a8: b        #0x3b2010
003b23ac: add      r7, r1, #0xff0
003b23b0: add      r6, r1, #0x560
003b23b4: add      r7, r7, #4
003b23b8: mov      r2, #0x51
003b23bc: mov      r1, r7
003b23c0: mov      r0, r6
003b23c4: bl       #0x3dedb4
003b23c8: mov      r1, r7
003b23cc: mov      r2, #0x52
003b23d0: mov      sl, r0
003b23d4: mov      r0, r6
003b23d8: bl       #0x3dedb4
003b23dc: mov      r1, r7
003b23e0: mov      ip, r0
003b23e4: mov      r2, #0x64
003b23e8: mov      r0, r6
003b23ec: b        #0x3b2064
003b23f0: mov      r2, #0
003b23f4: str      r3, [r0]
003b23f8: str      r2, [r0, #0x18]
003b23fc: str      r2, [r0, #4]
003b2400: str      r2, [r0, #8]
003b2404: str      r2, [r0, #0xc]
003b2408: str      r2, [r0, #0x10]
003b240c: str      r2, [r0, #0x14]
003b2410: b        #0x3b2010
003b2414: add      r7, r1, #0xff0
003b2418: add      r6, r1, #0x560
003b241c: add      r7, r7, #4
003b2420: mov      r2, #0xae
003b2424: mov      r1, r7
003b2428: mov      r0, r6
003b242c: bl       #0x3dedb4
003b2430: mov      r1, r7
003b2434: mov      r3, r0
003b2438: mov      r2, #0xaf
003b243c: mov      r0, r6
003b2440: str      r3, [sp]
003b2444: bl       #0x3dedb4
003b2448: ldr      r3, [sp]
003b244c: mov      sl, r0
003b2450: b        #0x3b2290
003b2454: mov      r1, r7
003b2458: mov      r2, #0x5f
003b245c: mov      r0, r6
003b2460: bl       #0x3dedb4
003b2464: mov      r1, r7
003b2468: mov      fp, r0
003b246c: mov      r2, #0x60
003b2470: mov      r0, r6
003b2474: bl       #0x3dedb4
003b2478: b        #0x3b21e0
003b247c: rsb      r0, fp, r0
003b2480: bl       #0x3af6d8
003b2484: ldr      r3, [sp, #0x10]
003b2488: mov      r6, r0
003b248c: ldr      r0, [sp, #0xc]
003b2490: add      r1, r3, #0x4a
003b2494: bl       #0x3af76c
003b2498: movw     r3, #0x851f
003b249c: asr      r2, r0, #8
003b24a0: movt     r3, #0x51eb
003b24a4: smull    r1, r3, r3, r2
003b24a8: asr      r0, r0, #0x1f
003b24ac: sub      r3, r0, r3, asr #5
003b24b0: adds     r3, r3, #1
003b24b4: bmi      #0x3b21ec
003b24b8: cmp      r3, #1
003b24bc: movge    r3, #1
003b24c0: add      r6, r6, fp
003b24c4: mla      sl, r6, r3, sl
003b24c8: cmp      sl, #0
003b24cc: movle    sl, #0x100
003b24d0: b        #0x3b21ec
003b24d4: mov      r1, #0x84
003b24d8: mov      r0, r6
003b24dc: bl       #0x3af76c
003b24e0: mul      r0, sl, r0
003b24e4: movw     r2, #0x851f
003b24e8: movt     r2, #0x51eb
003b24ec: asr      ip, r0, #8
003b24f0: smull    r1, ip, r2, ip
003b24f4: asr      r3, r0, #0x1f
003b24f8: rsb      r3, r3, ip, asr #5
003b24fc: mov      r1, #0x85
003b2500: mov      r0, r6
003b2504: str      r3, [sp, #8]
003b2508: str      r2, [sp]
003b250c: bl       #0x3af76c
003b2510: mul      r0, sl, r0
003b2514: ldr      r2, [sp]
003b2518: asr      r3, r0, #8
003b251c: asr      r0, r0, #0x1f
003b2520: smull    r1, r2, r2, r3
003b2524: rsb      r0, r0, r2, asr #5
003b2528: str      r0, [sp, #4]
003b252c: b        #0x3b21a8
003b2530: ldr      r0, [sp, #0xc]
003b2534: mov      r1, #0x79
003b2538: bl       #0x3af76c
003b253c: ldr      r3, [sp, #4]
003b2540: mul      r0, r3, r0
003b2544: asr      r0, r0, #8
003b2548: str      r0, [sp, #4]
003b254c: b        #0x3b214c
003b2550: mov      r1, #0x5d
003b2554: mov      r0, r6
003b2558: bl       #0x3af76c
003b255c: ldr      r1, [sp, #4]
003b2560: add      r1, r1, r0
003b2564: str      r1, [sp, #4]
003b2568: b        #0x3b210c
003b256c: mov      r1, #0xae
003b2570: mov      r0, r6
003b2574: bl       #0x3af76c
003b2578: mov      r1, #0xaf
003b257c: mov      sl, r0
003b2580: mov      r0, r6
003b2584: bl       #0x3af76c
003b2588: mov      ip, r0
003b258c: b        #0x3b2080
003b2590: mov      r1, #0x84
003b2594: mov      r0, r6
003b2598: bl       #0x3af76c
003b259c: mul      r3, r7, r0
003b25a0: movw     sl, #0x851f
003b25a4: movt     sl, #0x51eb
003b25a8: asr      r5, r3, #8
003b25ac: smull    r0, r5, sl, r5
003b25b0: asr      r3, r3, #0x1f
003b25b4: mov      r1, #0x85
003b25b8: mov      r0, r6
003b25bc: rsb      r5, r3, r5, asr #5
003b25c0: bl       #0x3af76c
003b25c4: mul      r0, r7, r0
003b25c8: mov      r2, r8
003b25cc: asr      r3, r0, #8
003b25d0: smull    r1, sl, sl, r3
003b25d4: asr      r0, r0, #0x1f
003b25d8: rsb      sl, r0, sl, asr #5
003b25dc: mov      r3, r8
003b25e0: b        #0x3b2390
003b25e4: ldr      r2, [sp, #0x54]
003b25e8: mov      r0, r6
003b25ec: mov      sl, #0x100
003b25f0: add      r1, r2, #0xa6
003b25f4: bl       #0x3af76c
003b25f8: b        #0x3b2324
003b25fc: mov      r1, #0x5c
003b2600: mov      r0, r6
003b2604: bl       #0x3af76c
003b2608: ldr      r1, [sp, #4]
003b260c: add      r1, r1, r0
003b2610: str      r1, [sp, #4]
003b2614: b        #0x3b20cc
003b2618: mov      r1, fp
003b261c: mov      r0, r6
003b2620: bl       #0x3df8ac
003b2624: add      sl, sl, r0
003b2628: b        #0x3b22b8
003b262c: subseq   r0, pc, ip, asr r8
003b2630: subseq   r0, pc, r0, asr #16
003b2634: subseq   r0, pc, r8, ror #12
