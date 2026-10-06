_ZN10GameObject15SetVisualObjectEPKcS1_b
00394d34 cmp      r3, #0
00394d38 push     {r4, r5, r6, r7, r8, lr}
00394d3c mov      r4, r0
00394d40 mov      r5, r1
00394d44 mov      r6, r2
00394d48 beq      #0x394dec
00394d4c cmp      r1, #0
00394d50 add      r8, r0, #0x290
00394d54 beq      #0x394e6c
00394d58 mov      r0, r5
00394d5c bl       #0x30de54
00394d60 mov      r1, r5
00394d64 add      r2, r5, r0
00394d68 mov      r0, r8
00394d6c bl       #0x3109e0 ; _ZNSs9_M_assignEPKcS0_
00394d70 cmp      r6, #0
00394d74 cmpne    r5, #0
00394d78 add      r7, r4, #0x2a8
00394d7c beq      #0x394e28
00394d80 mov      r0, r6
00394d84 bl       #0x30de54
00394d88 add      r2, r6, r0
00394d8c mov      r1, r6
00394d90 mov      r0, r7
00394d94 bl       #0x3109e0 ; _ZNSs9_M_assignEPKcS0_
00394d98 ldr      r2, [r4, #0x2a0]
00394d9c ldr      r3, [r4, #0x2a4]
00394da0 cmp      r2, r3
00394da4 beq      #0x394e50
00394da8 mov      r1, #0
00394dac mov      r0, #0xac
00394db0 bl       #0x310570 ; _Znwj15MemoryHintState
00394db4 mov      r3, r7
00394db8 mov      r5, r0
00394dbc mov      r1, r4
00394dc0 mov      r2, r8
00394dc4 bl       #0x472a0c ; _ZN12VisualObjectC1EP10GameObjectRKSsS3_
00394dc8 ldr      r3, [r5, #8]
00394dcc cmp      r3, #0
00394dd0 beq      #0x394e94
00394dd4 mov      r0, r4
00394dd8 mov      r1, r5
00394ddc bl       #0x394338 ; _ZN10GameObject15SetVisualObjectEP12VisualObject
00394de0 ldr      r3, [r5, #8]
00394de4 str      r4, [r3, #0x204]
00394de8 pop      {r4, r5, r6, r7, r8, pc}
00394dec cmp      r1, #0
00394df0 beq      #0x394e68
00394df4 mov      r0, r1
00394df8 ldr      r1, [r4, #0x2a4]
00394dfc bl       #0x30e31c
00394e00 cmp      r0, #0
00394e04 bne      #0x394e60
00394e08 cmp      r6, #0
00394e0c beq      #0x394e24
00394e10 mov      r0, r6
00394e14 ldr      r1, [r4, #0x2bc]
00394e18 bl       #0x30e31c
00394e1c cmp      r0, #0
00394e20 bne      #0x394e60
00394e24 pop      {r4, r5, r6, r7, r8, pc}
00394e28 ldr      r6, [pc, #0x78]
00394e2c mov      r0, r7
00394e30 add      r6, pc, r6
00394e34 mov      r2, r6
00394e38 mov      r1, r6
00394e3c bl       #0x3109e0 ; _ZNSs9_M_assignEPKcS0_
00394e40 ldr      r2, [r4, #0x2a0]
00394e44 ldr      r3, [r4, #0x2a4]
00394e48 cmp      r2, r3
00394e4c bne      #0x394da8
00394e50 mov      r0, r4
00394e54 mov      r1, #0
00394e58 pop      {r4, r5, r6, r7, r8, lr}
00394e5c b        #0x394338
00394e60 add      r8, r4, #0x290
00394e64 b        #0x394d58
00394e68 add      r8, r0, #0x290
00394e6c ldr      r5, [pc, #0x38]
00394e70 mov      r0, r8
00394e74 add      r7, r4, #0x2a8
00394e78 add      r5, pc, r5
00394e7c mov      r2, r5
00394e80 mov      r1, r5
00394e84 bl       #0x3109e0 ; _ZNSs9_M_assignEPKcS0_
00394e88 mov      r6, r5
00394e8c mov      r2, r5
00394e90 b        #0x394d8c
00394e94 mov      r0, r5
00394e98 ldr      r3, [r5]
00394e9c mov      lr, pc
00394ea0 ldr      pc, [r3, #4]
00394ea4 pop      {r4, r5, r6, r7, r8, pc}
00394ea8 ldrsbeq  r6, [r3], #-0x98
