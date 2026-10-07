_ZNK10CameraBase15GetCenterOffsetER7Point3DIfEf
0040f4dc push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0040f4e0 ldr      r3, [r0, #8]
0040f4e4 ldr      r4, [pc, #0x204]
0040f4e8 mov      r6, r0
0040f4ec cmp      r3, #0
0040f4f0 add      r4, pc, r4
0040f4f4 sub      sp, sp, #0x34
0040f4f8 mov      r5, r1
0040f4fc mov      fp, r2
0040f500 moveq    r0, r3
0040f504 beq      #0x40f588
0040f508 mov      r0, r3
0040f50c ldr      r3, [r3]
0040f510 mov      lr, pc
0040f514 ldr      pc, [r3, #0x38]
0040f518 add      r3, r0, #0x10
0040f51c ldr      r7, [r3, #4]
0040f520 ldr      r8, [r0, #0x10]
0040f524 ldr      r3, [r3, #8]
0040f528 mov      sl, #0
0040f52c mov      r1, r8
0040f530 str      r3, [sp, #4]
0040f534 mov      r0, r8
0040f538 str      r8, [r5]
0040f53c str      r7, [r5, #4]
0040f540 str      sl, [r5, #8]
0040f544 bl       #0x30ed6c
0040f548 mov      r1, r7
0040f54c mov      sb, r0
0040f550 mov      r0, r7
0040f554 bl       #0x30ed6c
0040f558 mov      r1, r0
0040f55c mov      r0, sb
0040f560 bl       #0x30eba4
0040f564 mov      r1, sl
0040f568 bl       #0x30eba4
0040f56c movw     r1, #0xb717
0040f570 bic      r0, r0, #0x80000000
0040f574 movt     r1, #0x38d1
0040f578 bl       #0x30e70c
0040f57c cmp      r0, #0
0040f580 beq      #0x40f590
0040f584 mov      r0, #1
0040f588 add      sp, sp, #0x34
0040f58c pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
0040f590 ldr      r1, [r6, #8]
0040f594 add      r0, sp, #0x24
0040f598 bl       #0x597180 ; _ZNK6glitch5scene10ISceneNode19getAbsolutePositionEv
0040f59c mov      r1, fp
0040f5a0 ldr      r0, [sp, #0x2c]
0040f5a4 bl       #0x30e3ac
0040f5a8 ldr      r3, [pc, #0x144]
0040f5ac ldr      lr, [sp, #4]
0040f5b0 add      r1, sp, #0x18
0040f5b4 ldr      r3, [r4, r3]
0040f5b8 mov      sl, r0
0040f5bc add      r0, sp, #0xc
0040f5c0 ldr      ip, [r3]
0040f5c4 ldmib    r3, {r2, r3}
0040f5c8 add      ip, ip, #0x80000000
0040f5cc add      r2, r2, #0x80000000
0040f5d0 add      r3, r3, #0x80000000
0040f5d4 str      lr, [sp, #0x14]
0040f5d8 str      ip, [sp, #0x18]
0040f5dc str      r2, [sp, #0x1c]
0040f5e0 str      r8, [sp, #0xc]
0040f5e4 str      r7, [sp, #0x10]
0040f5e8 str      r3, [sp, #0x20]
0040f5ec bl       #0x313058 ; _ZNK7Point3DIfE5angleERKS0_
0040f5f0 ldr      r3, [r6, #8]
0040f5f4 bic      r4, r0, #0x80000000
0040f5f8 mov      r0, r3
0040f5fc ldr      r3, [r3]
0040f600 mov      lr, pc
0040f604 ldr      pc, [r3, #0x128]
0040f608 ldr      r3, [r6, #8]
0040f60c mov      r8, r0
0040f610 mov      r0, r3
0040f614 ldr      r3, [r3]
0040f618 mov      lr, pc
0040f61c ldr      pc, [r3, #0x128]
0040f620 mov      r6, r0
0040f624 mov      r0, r4
0040f628 bl       #0x30df2c
0040f62c mov      r1, #0x3f000000
0040f630 mov      r7, r0
0040f634 mov      r0, r8
0040f638 bl       #0x30ed6c
0040f63c mov      r1, r4
0040f640 bl       #0x30eba4
0040f644 bl       #0x30df2c
0040f648 mov      r1, #0xbf000000
0040f64c mov      r8, r0
0040f650 mov      r0, r6
0040f654 bl       #0x30ed6c
0040f658 mov      r1, r4
0040f65c bl       #0x30eba4
0040f660 bl       #0x30df2c
0040f664 mov      r1, r0
0040f668 mov      r0, r7
0040f66c bl       #0x30e3ac
0040f670 mov      r1, sl
0040f674 bl       #0x30ed6c
0040f678 mov      r6, r0
0040f67c mov      r0, r5
0040f680 bl       #0x34d0b0 ; _ZN7Point3DIfE9normalizeEv
0040f684 mov      r1, r7
0040f688 mov      r4, r0
0040f68c mov      r0, r8
0040f690 bl       #0x30e3ac
0040f694 mov      r1, sl
0040f698 bl       #0x30ed6c
0040f69c mov      r1, r0
0040f6a0 mov      r0, r6
0040f6a4 bl       #0x30eba4
0040f6a8 mov      r1, #0x3f000000
0040f6ac bl       #0x30ed6c
0040f6b0 mov      r1, r6
0040f6b4 bl       #0x30e3ac
0040f6b8 mov      r5, r0
0040f6bc mov      r1, r0
0040f6c0 ldr      r0, [r4]
0040f6c4 bl       #0x30ed6c
0040f6c8 mov      r1, r5
0040f6cc str      r0, [r4]
0040f6d0 ldr      r0, [r4, #4]
0040f6d4 bl       #0x30ed6c
0040f6d8 mov      r1, r5
0040f6dc str      r0, [r4, #4]
0040f6e0 ldr      r0, [r4, #8]
0040f6e4 bl       #0x30ed6c
0040f6e8 str      r0, [r4, #8]
0040f6ec b        #0x40f584
0040f6f0 subseq   r5, r8, r0, lsr #11
0040f6f4 andeq    r4, r0, r0, asr #6
_ZNK10CameraBase18GetCameraLookAtVecEv
0040e8a8 push     {r4, lr}
0040e8ac ldr      r2, [r1, #8]
0040e8b0 ldr      r3, [pc, #0x68]
0040e8b4 mov      r4, r0
0040e8b8 cmp      r2, #0
0040e8bc add      r3, pc, r3
0040e8c0 beq      #0x40e8f8
0040e8c4 ldr      r3, [r2]
0040e8c8 mov      r0, r2
0040e8cc mov      lr, pc
0040e8d0 ldr      pc, [r3, #0x38]
0040e8d4 add      r3, r0, #0x10
0040e8d8 ldr      r1, [r3, #8]
0040e8dc ldr      r2, [r0, #0x10]
0040e8e0 ldr      r3, [r3, #4]
0040e8e4 mov      r0, r4
0040e8e8 str      r1, [r4, #8]
0040e8ec str      r2, [r4]
0040e8f0 str      r3, [r4, #4]
0040e8f4 pop      {r4, pc}
0040e8f8 ldr      r2, [pc, #0x24]
0040e8fc ldr      r3, [r3, r2]
0040e900 ldr      r2, [r3]
0040e904 str      r2, [r0]
0040e908 ldr      r2, [r3, #4]
0040e90c str      r2, [r0, #4]
0040e910 ldr      r3, [r3, #8]
0040e914 str      r3, [r0, #8]
0040e918 mov      r0, r4
0040e91c pop      {r4, pc}
0040e920 ldrsbeq  r6, [r8], #-0x14
0040e924 andeq    r3, r0, ip, lsr #30
_ZNK10CameraBase14GetCameraUpVecEv
0040e928 push     {r4, lr}
0040e92c ldr      r2, [r1, #8]
0040e930 ldr      r3, [pc, #0x68]
0040e934 mov      r4, r0
0040e938 cmp      r2, #0
0040e93c add      r3, pc, r3
0040e940 beq      #0x40e978
0040e944 ldr      r3, [r2]
0040e948 mov      r0, r2
0040e94c mov      lr, pc
0040e950 ldr      pc, [r3, #0x38]
0040e954 add      r3, r0, #0x20
0040e958 ldr      r1, [r3, #8]
0040e95c ldr      r2, [r0, #0x20]
0040e960 ldr      r3, [r3, #4]
0040e964 mov      r0, r4
0040e968 str      r1, [r4, #8]
0040e96c str      r2, [r4]
0040e970 str      r3, [r4, #4]
0040e974 pop      {r4, pc}
0040e978 ldr      r2, [pc, #0x24]
0040e97c ldr      r3, [r3, r2]
0040e980 ldr      r2, [r3]
0040e984 str      r2, [r0]
0040e988 ldr      r2, [r3, #4]
0040e98c str      r2, [r0, #4]
0040e990 ldr      r3, [r3, #8]
0040e994 str      r3, [r0, #8]
0040e998 mov      r0, r4
0040e99c pop      {r4, pc}
0040e9a0 subseq   r6, r8, r4, asr r1
0040e9a4 andeq    r3, r0, ip, lsr #30
_ZNK7Point3DIfE5angleERKS0_
00313058 push     {r4, lr}
0031305c bl       #0x312f40 ; _ZNK7Point3DIfE8angleCosERKS0_
00313060 pop      {r4, lr}
00313064 b        #0x30e3dc
_ZNK7Point3DIfE8angleCosERKS0_
00312f40 push     {r4, r5, r6, r7, r8, lr}
00312f44 ldr      r7, [r0]
00312f48 mov      r3, r0
00312f4c mov      r4, r1
00312f50 ldr      r6, [r0, #4]
00312f54 ldr      r1, [r1]
00312f58 mov      r0, r7
00312f5c ldr      r5, [r3, #8]
00312f60 bl       #0x30ed6c
00312f64 ldr      r1, [r4, #4]
00312f68 mov      r8, r0
00312f6c mov      r0, r6
00312f70 bl       #0x30ed6c
00312f74 mov      r1, r0
00312f78 mov      r0, r8
00312f7c bl       #0x30eba4
00312f80 ldr      r1, [r4, #8]
00312f84 mov      r8, r0
00312f88 mov      r0, r5
00312f8c bl       #0x30ed6c
00312f90 mov      r1, r0
00312f94 mov      r0, r8
00312f98 bl       #0x30eba4
00312f9c mov      r1, r7
00312fa0 mov      r8, r0
00312fa4 mov      r0, r7
00312fa8 bl       #0x30ed6c
00312fac mov      r1, r6
00312fb0 mov      r7, r0
00312fb4 mov      r0, r6
00312fb8 bl       #0x30ed6c
00312fbc mov      r1, r0
00312fc0 mov      r0, r7
00312fc4 bl       #0x30eba4
00312fc8 mov      r1, r5
00312fcc mov      r6, r0
00312fd0 mov      r0, r5
00312fd4 bl       #0x30ed6c
00312fd8 mov      r1, r0
00312fdc mov      r0, r6
00312fe0 bl       #0x30eba4
00312fe4 bl       #0x30e124
00312fe8 mov      r5, r0
00312fec ldr      r0, [r4]
00312ff0 ldr      r7, [r4, #4]
00312ff4 ldr      r6, [r4, #8]
00312ff8 mov      r1, r0
00312ffc bl       #0x30ed6c
00313000 mov      r1, r7
00313004 mov      r4, r0
00313008 mov      r0, r7
0031300c bl       #0x30ed6c
00313010 mov      r1, r0
00313014 mov      r0, r4
00313018 bl       #0x30eba4
0031301c mov      r1, r6
00313020 mov      r4, r0
00313024 mov      r0, r6
00313028 bl       #0x30ed6c
0031302c mov      r1, r0
00313030 mov      r0, r4
00313034 bl       #0x30eba4
00313038 bl       #0x30e124
0031303c mov      r1, r0
00313040 mov      r0, r5
00313044 bl       #0x30ed6c
00313048 mov      r1, r0
0031304c mov      r0, r8
00313050 bl       #0x30ec94
00313054 pop      {r4, r5, r6, r7, r8, pc}
_ZN7Point3DIfE9normalizeEv
0034d0b0 push     {r4, r5, r6, r7, lr}
0034d0b4 mov      r4, r0
0034d0b8 ldr      r0, [r0]
0034d0bc sub      sp, sp, #0xc
0034d0c0 ldr      r7, [r4, #4]
0034d0c4 mov      r1, r0
0034d0c8 bl       #0x30ed6c
0034d0cc mov      r1, r7
0034d0d0 mov      r5, r0
0034d0d4 mov      r0, r7
0034d0d8 bl       #0x30ed6c
0034d0dc mov      r1, r0
0034d0e0 mov      r0, r5
0034d0e4 bl       #0x30eba4
0034d0e8 ldr      r6, [r4, #8]
0034d0ec mov      r5, r0
0034d0f0 mov      r1, r6
0034d0f4 mov      r0, r6
0034d0f8 bl       #0x30ed6c
0034d0fc mov      r1, r0
0034d100 mov      r0, r5
0034d104 bl       #0x30eba4
0034d108 bl       #0x30e124
0034d10c add      r1, sp, #8
0034d110 str      r0, [r1, #-4]!
0034d114 mov      r0, r4
0034d118 bl       #0x34d04c ; _ZN7Point3DIfEdVERKf
0034d11c add      sp, sp, #0xc
0034d120 pop      {r4, r5, r6, r7, pc}
_GLOBAL__I_.._.._sources_Utils_Point3D.cpp
00312e00 ldr      r2, [pc, #0x7c]
00312e04 ldr      r3, [pc, #0x7c]
00312e08 push     {r4, r5, r6, r7}
00312e0c add      r2, pc, r2
00312e10 ldr      r0, [r2, r3]
00312e14 ldr      r3, [pc, #0x70]
00312e18 ldr      r1, [pc, #0x70]
00312e1c mov      r7, #0x3f000000
00312e20 ldr      r6, [r2, r3]
00312e24 ldr      r3, [pc, #0x68]
00312e28 mov      r4, #0x3f800000
00312e2c add      r1, pc, r1
00312e30 ldr      r5, [r2, r3]
00312e34 ldr      r3, [pc, #0x5c]
00312e38 str      r7, [r1, #8]
00312e3c str      r4, [r0, #8]
00312e40 ldr      ip, [r2, r3]
00312e44 mov      r3, #0
00312e48 str      r3, [r6, #8]
00312e4c str      r3, [r5, #8]
00312e50 str      r3, [ip, #8]
00312e54 str      r3, [r0, #4]
00312e58 str      r7, [r1]
00312e5c str      r7, [r1, #4]
00312e60 str      r3, [r6]
00312e64 str      r3, [r6, #4]
00312e68 str      r4, [r5]
00312e6c str      r3, [r5, #4]
00312e70 str      r3, [ip]
00312e74 str      r4, [ip, #4]
00312e78 str      r3, [r0]
00312e7c pop      {r4, r5, r6, r7}
00312e80 bx       lr
00312e84 rsbeq    r1, r8, r4, lsl #25
00312e88 andeq    r4, r0, r0, asr #6
00312e8c andeq    r3, r0, ip, lsr #30
00312e90 rsbeq    ip, r8, r4, lsl sl
00312e94 andeq    r3, r0, r8, lsr r2
00312e98 ldrdeq   r4, r5, [r0], -r8
