
# _ZN10GameObject14UpdatePFObjectEv
00393ea0: push     {r4, r5, r6, r7, lr}
00393ea4: ldr      r3, [r0, #0x1c8]
00393ea8: ldr      r5, [pc, #0xd8]
00393eac: sub      sp, sp, #0xc
00393eb0: cmp      r3, #0
00393eb4: mov      r4, r0
00393eb8: add      r5, pc, r5
00393ebc: beq      #0x393ee8
00393ec0: ldr      r3, [r0]
00393ec4: mov      lr, pc
00393ec8: ldr      pc, [r3, #0xb4]
00393ecc: cmp      r0, #0
00393ed0: bne      #0x393f38
00393ed4: ldr      r0, [r4, #0x2dc]
00393ed8: cmp      r0, #0
00393edc: beq      #0x393ef0
00393ee0: bl       #0x46e750
00393ee4: str      r0, [r4, #0x1d0]
00393ee8: add      sp, sp, #0xc
00393eec: pop      {r4, r5, r6, r7, pc}
00393ef0: ldr      r1, [r4, #0x144]
00393ef4: ldr      r0, [r4, #0x150]
00393ef8: bl       #0x30e3ac
00393efc: ldr      r1, [r4, #0x148]
00393f00: mov      r5, r0
00393f04: ldr      r0, [r4, #0x154]
00393f08: bl       #0x30e3ac
00393f0c: mov      r6, r0
00393f10: mov      r1, r6
00393f14: mov      r0, r5
00393f18: bl       #0x30e70c
00393f1c: cmp      r0, #0
00393f20: movne    r5, r6
00393f24: mov      r0, r5
00393f28: mov      r1, #0x3f000000
00393f2c: bl       #0x30ed6c
00393f30: str      r0, [r4, #0x1d0]
00393f34: b        #0x393ee8
00393f38: ldr      r3, [r4]
00393f3c: mov      r0, r4
00393f40: ldr      r7, [r4, #0x2dc]
00393f44: mov      lr, pc
00393f48: ldr      pc, [r3, #0xb8]
00393f4c: ldr      r3, [r4]
00393f50: mov      r6, r0
00393f54: mov      r0, r4
00393f58: mov      lr, pc
00393f5c: ldr      pc, [r3, #0xbc]
00393f60: ldr      r3, [pc, #0x24]
00393f64: subs     r7, r7, #0
00393f68: movne    r7, #1
00393f6c: str      r0, [sp]
00393f70: mov      r2, r7
00393f74: ldr      r0, [r5, r3]
00393f78: add      r1, r4, #0x1c8
00393f7c: mov      r3, r6
00393f80: bl       #0x528234
00393f84: b        #0x393ed4

# _ZNK9Container10IsObstacleEv
0039f3a8: mov      r0, #1
0039f3ac: bx       lr

# _ZNK9Container19GetObstacleStrengthEv
0039f3bc: mov      r0, #0x41000000
0039f3c0: add      r0, r0, #0x200000
0039f3c4: bx       lr

# _ZNK11TriggerTrap19GetObstacleStrengthEv
0039cb10: mov      r0, #0x41000000
0039cb14: add      r0, r0, #0x200000
0039cb18: bx       lr

# _ZNK10GameObject17GetObstacleRadiusEv
0034010c: mov      r0, #0
00340110: bx       lr

# _ZNK9Character17GetObstacleRadiusEv
003a2ef0: mov      r0, #0x42000000
003a2ef4: add      r0, r0, #0x480000
003a2ef8: bx       lr

# _ZNK9Container17GetObstacleRadiusEv
0039f3b0: mov      r0, #0x43000000
0039f3b4: add      r0, r0, #0x160000
0039f3b8: bx       lr

# _ZNK14LiftableObject19GetObstacleStrengthEv
003ee420: mov      r0, #0x41000000
003ee424: add      r0, r0, #0x200000
003ee428: bx       lr

# _ZNK9Character10IsObstacleEv
003a2ee8: mov      r0, #1
003a2eec: bx       lr

# _ZNK10GameObject19GetObstacleStrengthEv
00340114: mov      r0, #0
00340118: bx       lr

# _ZNK10GameObject10IsObstacleEv
00340104: mov      r0, #0
00340108: bx       lr

# _ZNK14PhysicalObject9getRadiusEv
0046e750: push     {r4, lr}
0046e754: mov      r1, #0x42000000
0046e758: ldr      r0, [r0, #0xc]
0046e75c: add      r1, r1, #0xc80000
0046e760: bl       #0x30ed6c
0046e764: pop      {r4, pc}

# _ZNK11TriggerTrap10IsObstacleEv
0039cafc: mov      r0, #1
0039cb00: bx       lr

# _ZNK14LiftableObject17GetObstacleRadiusEv
003ee414: mov      r0, #0x43000000
003ee418: add      r0, r0, #0x160000
003ee41c: bx       lr

# _ZNK9Character19GetObstacleStrengthEv
003a2efc: mov      r0, #0x41000000
003a2f00: add      r0, r0, #0xa00000
003a2f04: bx       lr

# _ZNK14LiftableObject10IsObstacleEv
003ee40c: mov      r0, #1
003ee410: bx       lr

# _ZNK11TriggerTrap17GetObstacleRadiusEv
0039cb04: mov      r0, #0x43000000
0039cb08: add      r0, r0, #0x160000
0039cb0c: bx       lr
