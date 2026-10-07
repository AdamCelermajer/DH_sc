_ZN6glitch5video21CProgrammableGLDriverINS0_18CGLSLShaderHandlerEE12setTransformENS0_22E_TRANSFORMATION_STATEERKNS_4core8CMatrix4IfEE
005b1e14 push     {r4, r5, r6, lr}
005b1e18 mov      r3, #0x44
005b1e1c mul      r3, r3, r1
005b1e20 mov      r5, r1
005b1e24 add      r3, r3, #0x288
005b1e28 mov      r1, r2
005b1e2c sub      sp, sp, #0x48
005b1e30 mov      r6, r2
005b1e34 mov      r4, r0
005b1e38 mov      r2, #0x41
005b1e3c add      r0, r0, r3
005b1e40 bl       #0x30e868
005b1e44 cmp      r5, #1
005b1e48 beq      #0x5b1ef8
005b1e4c cmp      r5, #2
005b1e50 beq      #0x5b1e98
005b1e54 cmp      r5, #0
005b1e58 bne      #0x5b1e90
005b1e5c ldr      r3, [r4]
005b1e60 mov      r0, r4
005b1e64 mov      lr, pc
005b1e68 ldr      pc, [r3, #0x1fc]
005b1e6c add      r0, r4, #0x820
005b1e70 add      r0, r0, #8
005b1e74 mov      r1, r6
005b1e78 mov      r2, #0x41
005b1e7c bl       #0x30e868
005b1e80 ldr      r3, [r4, #0xdbc]
005b1e84 orr      r3, r3, #0xef00
005b1e88 orr      r3, r3, #0x7b
005b1e8c str      r3, [r4, #0xdbc]
005b1e90 add      sp, sp, #0x48
005b1e94 pop      {r4, r5, r6, pc}
005b1e98 add      r5, sp, #4
005b1e9c mov      r0, r4
005b1ea0 ldr      r3, [r4]
005b1ea4 mov      lr, pc
005b1ea8 ldr      pc, [r3, #0x1fc]
005b1eac mov      r3, #0
005b1eb0 mov      r2, #0x41
005b1eb4 mov      r1, r6
005b1eb8 mov      r0, r5
005b1ebc strb     r3, [sp, #0x44]
005b1ec0 bl       #0x30e868
005b1ec4 mov      r0, r4
005b1ec8 mov      r1, r5
005b1ecc bl       #0x6dd9d0 ; _ZNK6glitch5video19CCommonGLDriverBase21fixUpProjectionMatrixERNS_4core8CMatrix4IfEE
005b1ed0 mov      r1, r5
005b1ed4 add      r0, r4, #0x8b0
005b1ed8 mov      r2, #0x41
005b1edc bl       #0x30e868
005b1ee0 ldr      r3, [r4, #0xdbc]
005b1ee4 orr      r3, r3, #0x78000
005b1ee8 orr      r3, r3, #0xa50
005b1eec orr      r3, r3, #2
005b1ef0 str      r3, [r4, #0xdbc]
005b1ef4 b        #0x5b1e90
005b1ef8 add      r0, r4, #0x860
005b1efc add      r0, r0, #0xc
005b1f00 mov      r1, r6
005b1f04 mov      r2, #0x41
005b1f08 bl       #0x30e868
005b1f0c ldr      r3, [r4, #0xdbc]
005b1f10 orr      r3, r3, #0xdc00
005b1f14 orr      r3, r3, #0xe7
005b1f18 str      r3, [r4, #0xdbc]
005b1f1c b        #0x5b1e90
_ZNK6glitch5video19CCommonGLDriverBase21fixUpProjectionMatrixERNS_4core8CMatrix4IfEE
006dd9d0 push     {r4, r5, r6, lr}
006dd9d4 mov      r3, #0
006dd9d8 strb     r3, [r1, #0x40]
006dd9dc mov      r4, r1
006dd9e0 mov      r5, r0
006dd9e4 mov      r1, #0
006dd9e8 ldr      r0, [r4, #0x2c]
006dd9ec bl       #0x30df8c
006dd9f0 cmp      r0, #0
006dd9f4 bne      #0x6dda78
006dd9f8 ldr      r0, [r4, #0x28]
006dd9fc mov      r1, r0
006dda00 bl       #0x30eba4
006dda04 mov      r1, #0x3f800000
006dda08 bl       #0x30e3ac
006dda0c ldr      r3, [r4, #0x38]
006dda10 str      r0, [r4, #0x28]
006dda14 mov      r1, r3
006dda18 mov      r0, r3
006dda1c bl       #0x30eba4
006dda20 str      r0, [r4, #0x38]
006dda24 ldrb     r3, [r5, #0x4a0]
006dda28 cmp      r3, #0
006dda2c beq      #0x6dda68
006dda30 ldr      r0, [r4, #4]
006dda34 ldr      r1, [r4, #0x14]
006dda38 ldr      r2, [r4, #0x24]
006dda3c ldr      r3, [r4, #0x34]
006dda40 add      r0, r0, #0x80000000
006dda44 add      r1, r1, #0x80000000
006dda48 add      r2, r2, #0x80000000
006dda4c add      r3, r3, #0x80000000
006dda50 mov      ip, #0
006dda54 strb     ip, [r4, #0x40]
006dda58 str      r0, [r4, #4]
006dda5c str      r1, [r4, #0x14]
006dda60 str      r2, [r4, #0x24]
006dda64 str      r3, [r4, #0x34]
006dda68 mov      r0, r5
006dda6c mov      r1, r4
006dda70 pop      {r4, r5, r6, lr}
006dda74 b        #0x5a8f88
006dda78 ldr      r0, [r4, #0x38]
006dda7c mov      r1, r0
006dda80 bl       #0x30eba4
006dda84 mov      r1, #0x3f800000
006dda88 bl       #0x30e3ac
006dda8c ldr      r3, [r4, #0x28]
006dda90 str      r0, [r4, #0x38]
006dda94 mov      r1, r3
006dda98 mov      r0, r3
006dda9c bl       #0x30eba4
006ddaa0 str      r0, [r4, #0x28]
006ddaa4 b        #0x6dda24
_ZNK6glitch5video12IVideoDriver32fixUpProjectionMatrixOrientationERNS_4core8CMatrix4IfEE
005a8f88 push     {r4, r5, r6, r7, r8, sl}
005a8f8c ldr      r2, [r0, #0xcc]
005a8f90 ldr      r3, [r0, #0xc8]
005a8f94 rsb      r3, r3, r2
005a8f98 asr      r3, r3, #2
005a8f9c cmp      r3, #1
005a8fa0 bls      #0x5a8fac
005a8fa4 pop      {r4, r5, r6, r7, r8, sl}
005a8fa8 bx       lr
005a8fac ldr      r3, [r0, #0x13c]
005a8fb0 cmp      r3, #0
005a8fb4 beq      #0x5a8fa4
005a8fb8 cmp      r3, #1
005a8fbc cmpne    r3, #3
005a8fc0 bne      #0x5a900c
005a8fc4 ldr      r7, [r1]
005a8fc8 ldr      r8, [r1, #4]
005a8fcc ldr      r5, [r1, #0x10]
005a8fd0 ldr      r6, [r1, #0x14]
005a8fd4 ldr      ip, [r1, #0x20]
005a8fd8 ldr      r4, [r1, #0x24]
005a8fdc ldr      r2, [r1, #0x30]
005a8fe0 ldr      r0, [r1, #0x34]
005a8fe4 mov      sl, #0
005a8fe8 strb     sl, [r1, #0x40]
005a8fec str      r8, [r1]
005a8ff0 str      r7, [r1, #4]
005a8ff4 str      r6, [r1, #0x10]
005a8ff8 str      r5, [r1, #0x14]
005a8ffc str      r4, [r1, #0x20]
005a9000 str      ip, [r1, #0x24]
005a9004 str      r0, [r1, #0x30]
005a9008 str      r2, [r1, #0x34]
005a900c sub      r2, r3, #2
005a9010 cmp      r2, #1
005a9014 bls      #0x5a9060
005a9018 sub      r3, r3, #1
005a901c cmp      r3, #1
005a9020 bhi      #0x5a8fa4
005a9024 ldr      ip, [r1, #0x30]
005a9028 ldr      r0, [r1]
005a902c ldr      r2, [r1, #0x10]
005a9030 ldr      r3, [r1, #0x20]
005a9034 add      ip, ip, #0x80000000
005a9038 add      r0, r0, #0x80000000
005a903c add      r2, r2, #0x80000000
005a9040 add      r3, r3, #0x80000000
005a9044 str      ip, [r1, #0x30]
005a9048 mov      ip, #0
005a904c strb     ip, [r1, #0x40]
005a9050 str      r0, [r1]
005a9054 str      r2, [r1, #0x10]
005a9058 str      r3, [r1, #0x20]
005a905c b        #0x5a8fa4
005a9060 ldr      r4, [r1, #4]
005a9064 ldr      ip, [r1, #0x14]
005a9068 ldr      r0, [r1, #0x24]
005a906c ldr      r2, [r1, #0x34]
005a9070 add      r4, r4, #0x80000000
005a9074 add      ip, ip, #0x80000000
005a9078 add      r0, r0, #0x80000000
005a907c add      r2, r2, #0x80000000
005a9080 mov      r5, #0
005a9084 strb     r5, [r1, #0x40]
005a9088 str      r4, [r1, #4]
005a908c str      ip, [r1, #0x14]
005a9090 str      r0, [r1, #0x24]
005a9094 str      r2, [r1, #0x34]
005a9098 b        #0x5a9018
_ZNK6glitch5video21CProgrammableGLDriverINS0_18CGLSLShaderHandlerEE21getTransformForShaderENS0_22E_TRANSFORMATION_STATEE
005af054 mov      r3, #0x44
005af058 mul      r3, r3, r1
005af05c add      r3, r3, #0x820
005af060 add      r3, r3, #8
005af064 add      r0, r0, r3
005af068 bx       lr
