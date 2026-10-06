# _ZN10Projectile14HandleImpactFXENS_10ImpactTypeERK7Point3DIfE
003e4db0 push     {r4, r5, r6, r7, lr}
003e4db4 ldr      r4, [pc, #0x120]
003e4db8 sub      sp, sp, #0x24
003e4dbc mov      r5, r2
003e4dc0 add      r4, pc, r4
003e4dc4 cmp      r1, #3
003e4dc8 addls    pc, pc, r1, lsl #2
003e4dcc b        #0x3e4eb8
003e4dd0 b        #0x3e4e78
003e4dd4 b        #0x3e4de0
003e4dd8 b        #0x3e4e80
003e4ddc b        #0x3e4e80
003e4de0 ldr      r2, [pc, #0xf8]
003e4de4 ldr      r3, [r0, #0x374]
003e4de8 mov      r1, #0x48
003e4dec ldr      r2, [r4, r2]
003e4df0 ldr      r2, [r2]
003e4df4 mla      r3, r1, r3, r2
003e4df8 ldr      r6, [r3, #0xc]
003e4dfc ldr      r1, [r3, #8]
003e4e00 cmn      r1, #1
003e4e04 beq      #0x3e4e24
003e4e08 ldr      r3, [pc, #0xd4]
003e4e0c mov      ip, #0
003e4e10 mov      r2, r5
003e4e14 ldr      r0, [r4, r3]
003e4e18 mov      r3, ip
003e4e1c str      ip, [sp]
003e4e20 bl       #0x495d14
003e4e24 cmn      r6, #1
003e4e28 beq      #0x3e4e78
003e4e2c ldr      r3, [pc, #0xb4]
003e4e30 ldr      lr, [r5, #8]
003e4e34 ldr      r7, [r5]
003e4e38 ldr      r3, [r4, r3]
003e4e3c ldr      r4, [r5, #4]
003e4e40 mov      ip, #0xbf000000
003e4e44 ldr      r0, [r3]
003e4e48 add      ip, ip, #0x800000
003e4e4c str      lr, [sp, #0x1c]
003e4e50 mov      r1, r6
003e4e54 mov      lr, #1
003e4e58 add      r2, sp, #0x14
003e4e5c mov      r3, #0
003e4e60 str      r7, [sp, #0x14]
003e4e64 str      r4, [sp, #0x18]
003e4e68 str      lr, [sp]
003e4e6c str      ip, [sp, #8]
003e4e70 str      ip, [sp, #4]
003e4e74 bl       #0x36b5d8
003e4e78 add      sp, sp, #0x24
003e4e7c pop      {r4, r5, r6, r7, pc}
003e4e80 ldr      r2, [pc, #0x58]
003e4e84 ldr      r3, [r0, #0x374]
003e4e88 mov      r1, #0x48
003e4e8c ldr      r2, [r4, r2]
003e4e90 ldr      r2, [r2]
003e4e94 mla      r3, r1, r3, r2
003e4e98 ldr      r6, [r3, #0x30]
003e4e9c ldr      r1, [r3, #0x2c]
003e4ea0 cmn      r6, #1
003e4ea4 beq      #0x3e4ed0
003e4ea8 cmn      r1, #1
003e4eac bne      #0x3e4e08
003e4eb0 ldr      r1, [r3, #0x14]
003e4eb4 b        #0x3e4e00
003e4eb8 ldr      r2, [pc, #0x20]
003e4ebc ldr      r3, [r0, #0x374]
003e4ec0 mov      r1, #0x48
003e4ec4 ldr      r2, [r4, r2]
003e4ec8 ldr      r2, [r2]
003e4ecc mla      r3, r1, r3, r2
003e4ed0 ldr      r6, [r3, #0x18]
003e4ed4 ldr      r1, [r3, #0x14]
003e4ed8 b        #0x3e4e00
003e4edc ldrsbeq  pc, [sl], #-0xc0
003e4ee0 andeq    r2, r0, r0, asr #6
003e4ee4 andeq    r1, r0, r8, lsl #22
003e4ee8 andeq    r0, r0, r4, lsr #27
