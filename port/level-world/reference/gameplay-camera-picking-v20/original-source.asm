_ZN10CameraBase13GetWorldCoordERK7Point2DIfER7Point3DIfEf
0040f394 ldr      r3, [pc, #0xb8]
0040f398 push     {r4, r5, r6, r7, r8, lr}
0040f39c mov      r4, r0
0040f3a0 ldr      r0, [pc, #0xb0]
0040f3a4 add      r3, pc, r3
0040f3a8 sub      sp, sp, #8
0040f3ac ldr      r0, [r3, r0]
0040f3b0 mov      ip, #0
0040f3b4 mov      r5, r1
0040f3b8 ldr      lr, [r0, #0x10]
0040f3bc mov      r1, #0x3f800000
0040f3c0 ldr      r0, [r4]
0040f3c4 ldr      lr, [lr, #0x10]
0040f3c8 mov      r7, r2
0040f3cc ldr      r3, [lr, #0xcc]
0040f3d0 ldr      r6, [r3, #-4]
0040f3d4 str      ip, [sp, #4]
0040f3d8 str      ip, [sp]
0040f3dc bl       #0x30eba4
0040f3e0 mov      r1, #0x3f000000
0040f3e4 bl       #0x30ed6c
0040f3e8 mov      r8, r0
0040f3ec ldr      r0, [r6, #0xc]
0040f3f0 bl       #0x30e964
0040f3f4 mov      r1, r0
0040f3f8 mov      r0, r8
0040f3fc bl       #0x30ed6c
0040f400 bl       #0x30e4cc
0040f404 str      r0, [sp]
0040f408 ldr      r0, [r4, #4]
0040f40c mov      r1, #0x3f800000
0040f410 bl       #0x30eba4
0040f414 mov      r1, #0x3f000000
0040f418 bl       #0x30ed6c
0040f41c mov      r4, r0
0040f420 ldr      r0, [r6, #0x10]
0040f424 bl       #0x30e964
0040f428 mov      r1, r0
0040f42c mov      r0, r4
0040f430 bl       #0x30ed6c
0040f434 bl       #0x30e4cc
0040f438 mov      r1, r5
0040f43c str      r0, [sp, #4]
0040f440 mov      r2, r7
0040f444 mov      r0, sp
0040f448 bl       #0x40f2f0 ; _ZN10CameraBase13GetWorldCoordERK7Point2DIiER7Point3DIfEf
0040f44c add      sp, sp, #8
0040f450 pop      {r4, r5, r6, r7, r8, pc}
0040f454 subseq   r5, r8, ip, ror #13
0040f458 strdeq   r3, r4, [r0], -r4
_ZN10CameraBase13GetWorldCoordERK7Point2DIiER7Point3DIfEf
0040f2f0 ldr      ip, [pc, #0x94]
0040f2f4 ldr      r3, [pc, #0x94]
0040f2f8 push     {r4, r5, r6, r7, r8, lr}
0040f2fc add      ip, pc, ip
0040f300 ldr      r3, [ip, r3]
0040f304 mov      r8, r1
0040f308 ldr      lr, [r0, #4]
0040f30c ldr      r3, [r3, #0x10]
0040f310 ldr      r6, [r0]
0040f314 sub      sp, sp, #0x30
0040f318 ldr      r1, [r3, #0x1c]
0040f31c mov      r5, r2
0040f320 mov      r0, sp
0040f324 ldr      r1, [r1, #0x2c]
0040f328 add      r2, sp, #0x28
0040f32c mov      r3, #0
0040f330 ldr      r7, [r1]
0040f334 mov      r4, sp
0040f338 ldr      ip, [r7, #0x14]
0040f33c str      lr, [sp, #0x2c]
0040f340 str      r6, [sp, #0x28]
0040f344 blx      ip
0040f348 mov      r3, #0
0040f34c mov      r1, r3
0040f350 mov      r2, #0x3f800000
0040f354 mov      r0, r5
0040f358 str      r2, [sp, #0x20]
0040f35c str      r3, [sp, #0x18]
0040f360 str      r3, [sp, #0x1c]
0040f364 bl       #0x30eba4
0040f368 mov      r1, sp
0040f36c add      ip, r0, #0x80000000
0040f370 mov      r3, r8
0040f374 add      r0, sp, #0x18
0040f378 add      r2, sp, #0xc
0040f37c str      ip, [sp, #0x24]
0040f380 bl       #0x40f0b4 ; _ZNK6glitch4core7plane3dIfE30getIntersectionWithLimitedLineERKNS0_8vector3dIfEES6_RS4_
0040f384 add      sp, sp, #0x30
0040f388 pop      {r4, r5, r6, r7, r8, pc}
_ZN10CameraBase14GetScreenCoordERK7Point3DIfER7Point2DIfE
0040f714 ldr      r3, [pc, #0xf8]
0040f718 ldr      r2, [pc, #0xf8]
0040f71c push     {r4, r5, r6, r7, r8, sl, lr}
0040f720 add      r3, pc, r3
0040f724 ldr      r8, [r3, r2]
0040f728 sub      sp, sp, #0x5c
0040f72c mov      r6, r0
0040f730 ldr      r2, [r8, #0x10]
0040f734 mov      r7, r1
0040f738 mov      r1, #0
0040f73c ldr      r3, [r2, #0x10]
0040f740 add      r5, sp, #4
0040f744 mov      r4, #0x3f800000
0040f748 mov      r0, r3
0040f74c ldr      r3, [r3]
0040f750 mov      lr, pc
0040f754 ldr      pc, [r3, #0x70]
0040f758 ldr      r3, [r8, #0x10]
0040f75c mov      sl, r0
0040f760 mov      r1, #2
0040f764 ldr      r3, [r3, #0x10]
0040f768 mov      r0, r3
0040f76c ldr      r3, [r3]
0040f770 mov      lr, pc
0040f774 ldr      pc, [r3, #0x70]
0040f778 mov      r1, #0
0040f77c mov      r8, r0
0040f780 mov      r2, #0x40
0040f784 mov      r0, r5
0040f788 bl       #0x30e460
0040f78c ldr      ip, [r6]
0040f790 ldr      r3, [r6, #4]
0040f794 ldr      lr, [r6, #8]
0040f798 mov      r2, sl
0040f79c mov      r1, r8
0040f7a0 mov      r0, r5
0040f7a4 mov      r6, #1
0040f7a8 str      ip, [sp, #0x48]
0040f7ac str      r3, [sp, #0x4c]
0040f7b0 str      lr, [sp, #0x50]
0040f7b4 str      r4, [sp, #4]
0040f7b8 str      r4, [sp, #0x18]
0040f7bc str      r4, [sp, #0x2c]
0040f7c0 str      r4, [sp, #0x40]
0040f7c4 str      r4, [sp, #0x54]
0040f7c8 strb     r6, [sp, #0x44]
0040f7cc bl       #0x40ea54 ; _ZN6glitch4core6detail12CMatrix4BaseIfE20setbyproduct_nocheckERKS3_S5_
0040f7d0 mov      r0, r5
0040f7d4 add      r1, sp, #0x48
0040f7d8 bl       #0x312bf8 ; _ZNK6glitch4core8CMatrix4IfE21multiplyWith1x4MatrixEPf
0040f7dc ldr      r1, [sp, #0x54]
0040f7e0 mov      r0, r4
0040f7e4 bl       #0x30ec94
0040f7e8 ldr      r1, [sp, #0x48]
0040f7ec mov      r5, r0
0040f7f0 bl       #0x30ed6c
0040f7f4 ldr      r1, [sp, #0x4c]
0040f7f8 mov      r4, r0
0040f7fc mov      r0, r5
0040f800 bl       #0x30ed6c
0040f804 str      r4, [r7]
0040f808 str      r0, [r7, #4]
0040f80c add      sp, sp, #0x5c
0040f810 pop      {r4, r5, r6, r7, r8, sl, pc}
0040f814 subseq   r5, r8, r0, ror r3
0040f818 strdeq   r3, r4, [r0], -r4
_ZN6glitch5scene22CSceneCollisionManager27getRayFromScreenCoordinatesENS_4core10position2dIiEEPNS0_16ICameraSceneNodeE
006c59c0 push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
006c59c4 mov      ip, #0
006c59c8 str      ip, [r0, #0x14]
006c59cc str      ip, [r0]
006c59d0 str      ip, [r0, #4]
006c59d4 str      ip, [r0, #8]
006c59d8 str      ip, [r0, #0xc]
006c59dc str      ip, [r0, #0x10]
006c59e0 mov      sl, r1
006c59e4 ldr      r1, [r1, #8]
006c59e8 sub      sp, sp, #0x44
006c59ec mov      r4, r0
006c59f0 cmp      r1, #0
006c59f4 mov      sb, r2
006c59f8 mov      r7, r3
006c59fc beq      #0x6c5c3c
006c5a00 cmp      r3, #0
006c5a04 beq      #0x6c5d30
006c5a08 ldr      r3, [r7]
006c5a0c mov      r0, r7
006c5a10 mov      lr, pc
006c5a14 ldr      pc, [r3, #0x144]
006c5a18 add      ip, r0, #0x2c
006c5a1c add      r8, r0, #0xc
006c5a20 add      fp, r0, #0x5c
006c5a24 mov      r5, #0
006c5a28 mov      r2, ip
006c5a2c mov      r6, r0
006c5a30 mov      r1, fp
006c5a34 add      r3, sp, #0x34
006c5a38 mov      r0, r8
006c5a3c str      ip, [sp, #4]
006c5a40 str      r5, [sp, #0x34]
006c5a44 str      r5, [sp, #0x38]
006c5a48 str      r5, [sp, #0x3c]
006c5a4c bl       #0x3415d8 ; _ZNK6glitch4core7plane3dIfE25getIntersectionWithPlanesERKS2_S4_RNS0_8vector3dIfEE
006c5a50 add      r2, r6, #0x3c
006c5a54 add      r3, sp, #0x28
006c5a58 mov      r1, fp
006c5a5c mov      r0, r8
006c5a60 str      r5, [sp, #0x28]
006c5a64 str      r5, [sp, #0x2c]
006c5a68 str      r5, [sp, #0x30]
006c5a6c bl       #0x3415d8 ; _ZNK6glitch4core7plane3dIfE25getIntersectionWithPlanesERKS2_S4_RNS0_8vector3dIfEE
006c5a70 ldr      r1, [sp, #0x34]
006c5a74 ldr      r0, [sp, #0x28]
006c5a78 bl       #0x30e3ac
006c5a7c ldr      r1, [sp, #0x38]
006c5a80 str      r0, [sp, #0xc]
006c5a84 ldr      r0, [sp, #0x2c]
006c5a88 bl       #0x30e3ac
006c5a8c ldr      r1, [sp, #0x3c]
006c5a90 str      r0, [sp, #0x10]
006c5a94 ldr      r0, [sp, #0x30]
006c5a98 bl       #0x30e3ac
006c5a9c ldr      ip, [sp, #4]
006c5aa0 add      r3, sp, #0x1c
006c5aa4 str      r0, [sp, #0x14]
006c5aa8 mov      r2, ip
006c5aac mov      r0, r8
006c5ab0 add      r1, r6, #0x4c
006c5ab4 str      r5, [sp, #0x24]
006c5ab8 str      r5, [sp, #0x1c]
006c5abc str      r5, [sp, #0x20]
006c5ac0 bl       #0x3415d8 ; _ZNK6glitch4core7plane3dIfE25getIntersectionWithPlanesERKS2_S4_RNS0_8vector3dIfEE
006c5ac4 ldr      r1, [sp, #0x34]
006c5ac8 ldr      r0, [sp, #0x1c]
006c5acc bl       #0x30e3ac
006c5ad0 ldr      r1, [sp, #0x38]
006c5ad4 str      r0, [sp, #8]
006c5ad8 ldr      r0, [sp, #0x20]
006c5adc bl       #0x30e3ac
006c5ae0 ldr      r1, [sp, #0x3c]
006c5ae4 mov      fp, r0
006c5ae8 ldr      r0, [sp, #0x24]
006c5aec bl       #0x30e3ac
006c5af0 ldr      r3, [sl, #0xc]
006c5af4 mov      r8, r0
006c5af8 ldr      r0, [sb]
006c5afc ldr      r3, [r3, #0xcc]
006c5b00 ldr      ip, [r3, #-4]
006c5b04 ldr      r1, [ip, #0x1c]
006c5b08 ldr      r3, [ip, #0x14]
006c5b0c ldr      r2, [ip, #0x18]
006c5b10 ldr      r5, [ip, #0x20]
006c5b14 rsb      r3, r3, r1
006c5b18 str      r3, [sp, #4]
006c5b1c rsb      r5, r2, r5
006c5b20 bl       #0x30e964
006c5b24 ldr      r3, [sp, #4]
006c5b28 mov      sl, r0
006c5b2c mov      r0, r3
006c5b30 bl       #0x30e964
006c5b34 mov      r1, r0
006c5b38 mov      r0, sl
006c5b3c bl       #0x30ec94
006c5b40 mov      sl, r0
006c5b44 ldr      r0, [sb, #4]
006c5b48 bl       #0x30e964
006c5b4c mov      sb, r0
006c5b50 mov      r0, r5
006c5b54 bl       #0x30e964
006c5b58 mov      r1, r0
006c5b5c mov      r0, sb
006c5b60 bl       #0x30ec94
006c5b64 ldr      r3, [r7]
006c5b68 mov      r5, r0
006c5b6c mov      r0, r7
006c5b70 mov      lr, pc
006c5b74 ldr      pc, [r3, #0x150]
006c5b78 cmp      r0, #0
006c5b7c bne      #0x6c5c48
006c5b80 ldr      r3, [r6]
006c5b84 str      r3, [r4]
006c5b88 ldr      r3, [r6, #4]
006c5b8c str      r3, [r4, #4]
006c5b90 ldr      r3, [r6, #8]
006c5b94 str      r3, [r4, #8]
006c5b98 ldr      r1, [sp, #0x10]
006c5b9c mov      r0, sl
006c5ba0 bl       #0x30ed6c
006c5ba4 ldr      r1, [sp, #0x38]
006c5ba8 bl       #0x30eba4
006c5bac mov      r1, fp
006c5bb0 mov      r6, r0
006c5bb4 mov      r0, r5
006c5bb8 bl       #0x30ed6c
006c5bbc mov      r1, r0
006c5bc0 mov      r0, r6
006c5bc4 bl       #0x30eba4
006c5bc8 ldr      r1, [sp, #0x14]
006c5bcc mov      r6, r0
006c5bd0 mov      r0, sl
006c5bd4 bl       #0x30ed6c
006c5bd8 ldr      r1, [sp, #0x3c]
006c5bdc bl       #0x30eba4
006c5be0 mov      r1, r8
006c5be4 mov      r7, r0
006c5be8 mov      r0, r5
006c5bec bl       #0x30ed6c
006c5bf0 mov      r1, r0
006c5bf4 mov      r0, r7
006c5bf8 bl       #0x30eba4
006c5bfc ldr      r1, [sp, #0xc]
006c5c00 mov      r7, r0
006c5c04 mov      r0, sl
006c5c08 bl       #0x30ed6c
006c5c0c ldr      r1, [sp, #0x34]
006c5c10 bl       #0x30eba4
006c5c14 ldr      r1, [sp, #8]
006c5c18 mov      r8, r0
006c5c1c mov      r0, r5
006c5c20 bl       #0x30ed6c
006c5c24 mov      r1, r0
006c5c28 mov      r0, r8
006c5c2c bl       #0x30eba4
006c5c30 str      r6, [r4, #0x10]
006c5c34 str      r0, [r4, #0xc]
006c5c38 str      r7, [r4, #0x14]
006c5c3c mov      r0, r4
006c5c40 add      sp, sp, #0x44
006c5c44 pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
006c5c48 mov      r1, #0x3f000000
006c5c4c mov      r0, sl
006c5c50 bl       #0x30e3ac
006c5c54 mov      r1, #0x3f000000
006c5c58 mov      sb, r0
006c5c5c mov      r0, r5
006c5c60 bl       #0x30e3ac
006c5c64 ldr      r1, [sp, #0x10]
006c5c68 mov      r7, r0
006c5c6c mov      r0, sb
006c5c70 bl       #0x30ed6c
006c5c74 ldr      r1, [r6, #4]
006c5c78 bl       #0x30eba4
006c5c7c mov      r1, fp
006c5c80 mov      r3, r0
006c5c84 mov      r0, r7
006c5c88 str      r3, [sp, #4]
006c5c8c bl       #0x30ed6c
006c5c90 ldr      r3, [sp, #4]
006c5c94 mov      r1, r0
006c5c98 mov      r0, r3
006c5c9c bl       #0x30eba4
006c5ca0 ldr      r1, [sp, #0x14]
006c5ca4 mov      r2, r0
006c5ca8 mov      r0, sb
006c5cac str      r2, [sp]
006c5cb0 bl       #0x30ed6c
006c5cb4 ldr      r1, [r6, #8]
006c5cb8 bl       #0x30eba4
006c5cbc mov      r1, r8
006c5cc0 mov      r3, r0
006c5cc4 mov      r0, r7
006c5cc8 str      r3, [sp, #4]
006c5ccc bl       #0x30ed6c
006c5cd0 ldr      r3, [sp, #4]
006c5cd4 mov      r1, r0
006c5cd8 mov      r0, r3
006c5cdc bl       #0x30eba4
006c5ce0 ldr      r1, [sp, #0xc]
006c5ce4 mov      r3, r0
006c5ce8 mov      r0, sb
006c5cec str      r3, [sp, #4]
006c5cf0 bl       #0x30ed6c
006c5cf4 ldr      r1, [r6]
006c5cf8 bl       #0x30eba4
006c5cfc ldr      r1, [sp, #8]
006c5d00 mov      r6, r0
006c5d04 mov      r0, r7
006c5d08 bl       #0x30ed6c
006c5d0c mov      r1, r0
006c5d10 mov      r0, r6
006c5d14 bl       #0x30eba4
006c5d18 ldr      r2, [sp]
006c5d1c str      r0, [r4]
006c5d20 str      r2, [r4, #4]
006c5d24 ldr      r3, [sp, #4]
006c5d28 str      r3, [r4, #8]
006c5d2c b        #0x6c5b98
006c5d30 ldr      r7, [r1, #0xe4]
006c5d34 cmp      r7, #0
006c5d38 beq      #0x6c5c3c
006c5d3c b        #0x6c5a08
_ZN6glitch5scene12SViewFrustum7setFromERKNS_4core8CMatrix4IfEE
005826c0 push     {r4, r5, r6, r7, r8, sb, sl, lr}
005826c4 mov      r5, r1
005826c8 mov      r6, r0
005826cc ldr      r1, [r1]
005826d0 ldr      r0, [r5, #0xc]
005826d4 bl       #0x30eba4
005826d8 str      r0, [r6, #0x2c]
005826dc ldr      r1, [r5, #0x10]
005826e0 ldr      r0, [r5, #0x1c]
005826e4 bl       #0x30eba4
005826e8 str      r0, [r6, #0x30]
005826ec ldr      r1, [r5, #0x20]
005826f0 ldr      r0, [r5, #0x2c]
005826f4 bl       #0x30eba4
005826f8 str      r0, [r6, #0x34]
005826fc ldr      r1, [r5, #0x30]
00582700 ldr      r0, [r5, #0x3c]
00582704 bl       #0x30eba4
00582708 str      r0, [r6, #0x38]
0058270c ldr      r1, [r5]
00582710 ldr      r0, [r5, #0xc]
00582714 bl       #0x30e3ac
00582718 str      r0, [r6, #0x3c]
0058271c ldr      r1, [r5, #0x10]
00582720 ldr      r0, [r5, #0x1c]
00582724 bl       #0x30e3ac
00582728 str      r0, [r6, #0x40]
0058272c ldr      r1, [r5, #0x20]
00582730 ldr      r0, [r5, #0x2c]
00582734 bl       #0x30e3ac
00582738 str      r0, [r6, #0x44]
0058273c ldr      r1, [r5, #0x30]
00582740 ldr      r0, [r5, #0x3c]
00582744 bl       #0x30e3ac
00582748 str      r0, [r6, #0x48]
0058274c ldr      r1, [r5, #4]
00582750 ldr      r0, [r5, #0xc]
00582754 bl       #0x30e3ac
00582758 str      r0, [r6, #0x5c]
0058275c ldr      r1, [r5, #0x14]
00582760 ldr      r0, [r5, #0x1c]
00582764 bl       #0x30e3ac
00582768 str      r0, [r6, #0x60]
0058276c ldr      r1, [r5, #0x24]
00582770 ldr      r0, [r5, #0x2c]
00582774 bl       #0x30e3ac
00582778 str      r0, [r6, #0x64]
0058277c ldr      r1, [r5, #0x34]
00582780 ldr      r0, [r5, #0x3c]
00582784 bl       #0x30e3ac
00582788 str      r0, [r6, #0x68]
0058278c ldr      r1, [r5, #4]
00582790 ldr      r0, [r5, #0xc]
00582794 bl       #0x30eba4
00582798 str      r0, [r6, #0x4c]
0058279c ldr      r1, [r5, #0x14]
005827a0 ldr      r0, [r5, #0x1c]
005827a4 bl       #0x30eba4
005827a8 str      r0, [r6, #0x50]
005827ac ldr      r1, [r5, #0x24]
005827b0 ldr      r0, [r5, #0x2c]
005827b4 bl       #0x30eba4
005827b8 str      r0, [r6, #0x54]
005827bc ldr      r1, [r5, #0x34]
005827c0 ldr      r0, [r5, #0x3c]
005827c4 bl       #0x30eba4
005827c8 str      r0, [r6, #0x58]
005827cc ldr      r1, [r5, #8]
005827d0 ldr      r0, [r5, #0xc]
005827d4 bl       #0x30e3ac
005827d8 str      r0, [r6, #0xc]
005827dc ldr      r1, [r5, #0x18]
005827e0 mov      sb, r0
005827e4 ldr      r0, [r5, #0x1c]
005827e8 bl       #0x30e3ac
005827ec str      r0, [r6, #0x10]
005827f0 ldr      r1, [r5, #0x28]
005827f4 mov      sl, r0
005827f8 ldr      r0, [r5, #0x2c]
005827fc bl       #0x30e3ac
00582800 str      r0, [r6, #0x14]
00582804 ldr      r1, [r5, #0x38]
00582808 mov      r8, r0
0058280c ldr      r0, [r5, #0x3c]
00582810 bl       #0x30e3ac
00582814 str      r0, [r6, #0x18]
00582818 ldr      r3, [r5, #8]
0058281c mov      r4, r6
00582820 mov      r7, #0
00582824 str      r3, [r6, #0x1c]
00582828 ldr      r3, [r5, #0x18]
0058282c str      r3, [r6, #0x20]
00582830 ldr      r3, [r5, #0x28]
00582834 str      r3, [r6, #0x24]
00582838 ldr      r3, [r5, #0x38]
0058283c str      r3, [r6, #0x28]
00582840 b        #0x582850
00582844 ldr      sb, [r4, #0xc]
00582848 ldr      sl, [r4, #0x10]
0058284c ldr      r8, [r4, #0x14]
00582850 mov      r1, sb
00582854 mov      r0, sb
00582858 bl       #0x30ed6c
0058285c mov      r1, sl
00582860 mov      r5, r0
00582864 mov      r0, sl
00582868 bl       #0x30ed6c
0058286c mov      r1, r0
00582870 mov      r0, r5
00582874 bl       #0x30eba4
00582878 mov      r1, r8
0058287c mov      r5, r0
00582880 mov      r0, r8
00582884 bl       #0x30ed6c
00582888 mov      r1, r0
0058288c mov      r0, r5
00582890 bl       #0x30eba4
00582894 bl       #0x30e124
00582898 mov      r1, r0
0058289c mov      r0, #0x3f800000
005828a0 bl       #0x30ec94
005828a4 add      r5, r0, #0x80000000
005828a8 mov      r1, r5
005828ac ldr      r0, [r4, #0xc]
005828b0 bl       #0x30ed6c
005828b4 mov      r1, r5
005828b8 str      r0, [r4, #0xc]
005828bc ldr      r0, [r4, #0x10]
005828c0 bl       #0x30ed6c
005828c4 mov      r1, r5
005828c8 str      r0, [r4, #0x10]
005828cc ldr      r0, [r4, #0x14]
005828d0 bl       #0x30ed6c
005828d4 mov      r1, r5
005828d8 str      r0, [r4, #0x14]
005828dc ldr      r0, [r4, #0x18]
005828e0 bl       #0x30ed6c
005828e4 add      r7, r7, #1
005828e8 cmp      r7, #6
005828ec str      r0, [r4, #0x18]
005828f0 add      r4, r4, #0x10
005828f4 bne      #0x582844
005828f8 mov      r0, r6
005828fc pop      {r4, r5, r6, r7, r8, sb, sl, lr}
00582900 b        #0x5823d4
_ZNK6glitch4core7plane3dIfE25getIntersectionWithPlanesERKS2_S4_RNS0_8vector3dIfEE
003415d8 push     {r4, r5, r6, r7, lr}
003415dc sub      sp, sp, #0x1c
003415e0 add      r5, sp, #0xc
003415e4 mov      ip, #0
003415e8 mov      r7, r2
003415ec mov      r6, r3
003415f0 mov      r2, r5
003415f4 mov      r3, sp
003415f8 str      ip, [sp, #8]
003415fc str      ip, [sp, #0xc]
00341600 str      ip, [sp, #0x10]
00341604 str      ip, [sp, #0x14]
00341608 str      ip, [sp]
0034160c str      ip, [sp, #4]
00341610 bl       #0x341260 ; _ZNK6glitch4core7plane3dIfE24getIntersectionWithPlaneERKS2_RNS0_8vector3dIfEES7_
00341614 cmp      r0, #0
00341618 mov      r4, sp
0034161c beq      #0x341634
00341620 mov      r0, r7
00341624 mov      r1, r5
00341628 mov      r2, sp
0034162c mov      r3, r6
00341630 bl       #0x34033c ; _ZNK6glitch4core7plane3dIfE23getIntersectionWithLineERKNS0_8vector3dIfEES6_RS4_
00341634 add      sp, sp, #0x1c
00341638 pop      {r4, r5, r6, r7, pc}
_ZNK6glitch4core7plane3dIfE24getIntersectionWithPlaneERKS2_RNS0_8vector3dIfEES7_
00341260 push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
00341264 mov      r4, r0
00341268 ldr      r0, [r0]
0034126c sub      sp, sp, #0x2c
00341270 mov      r5, r1
00341274 mov      r1, r0
00341278 mov      sl, r2
0034127c mov      r7, r3
00341280 bl       #0x30ed6c
00341284 ldr      r6, [r4, #4]
00341288 mov      r8, r0
0034128c mov      r1, r6
00341290 mov      r0, r6
00341294 bl       #0x30ed6c
00341298 mov      r1, r0
0034129c mov      r0, r8
003412a0 bl       #0x30eba4
003412a4 ldr      r8, [r4, #8]
003412a8 mov      r6, r0
003412ac mov      r1, r8
003412b0 mov      r0, r8
003412b4 bl       #0x30ed6c
003412b8 mov      r1, r0
003412bc mov      r0, r6
003412c0 bl       #0x30eba4
003412c4 bl       #0x30e8a4
003412c8 bl       #0x30e1c0
003412cc bl       #0x30e6a0
003412d0 str      r0, [sp, #0x14]
003412d4 ldr      fp, [r5]
003412d8 ldr      r1, [r4]
003412dc ldr      sb, [r5, #4]
003412e0 mov      r0, fp
003412e4 bl       #0x30ed6c
003412e8 ldr      r1, [r4, #4]
003412ec mov      r6, r0
003412f0 mov      r0, sb
003412f4 bl       #0x30ed6c
003412f8 mov      r1, r0
003412fc mov      r0, r6
00341300 bl       #0x30eba4
00341304 ldr      r8, [r5, #8]
00341308 mov      r6, r0
0034130c ldr      r1, [r4, #8]
00341310 mov      r0, r8
00341314 bl       #0x30ed6c
00341318 mov      r1, r0
0034131c mov      r0, r6
00341320 bl       #0x30eba4
00341324 mov      r1, fp
00341328 mov      r6, r0
0034132c mov      r0, fp
00341330 bl       #0x30ed6c
00341334 mov      r1, sb
00341338 mov      fp, r0
0034133c mov      r0, sb
00341340 bl       #0x30ed6c
00341344 mov      r1, r0
00341348 mov      r0, fp
0034134c bl       #0x30eba4
00341350 mov      r1, r8
00341354 mov      sb, r0
00341358 mov      r0, r8
0034135c bl       #0x30ed6c
00341360 mov      r1, r0
00341364 mov      r0, sb
00341368 bl       #0x30eba4
0034136c bl       #0x30e8a4
00341370 bl       #0x30e1c0
00341374 bl       #0x30e6a0
00341378 mov      fp, r0
0034137c mov      r1, fp
00341380 ldr      r0, [sp, #0x14]
00341384 bl       #0x30ed6c
00341388 mov      r1, r6
0034138c mov      r8, r0
00341390 mov      r0, r6
00341394 bl       #0x30ed6c
00341398 mov      r1, r0
0034139c mov      r0, r8
003413a0 bl       #0x30e3ac
003413a4 bl       #0x30e8a4
003413a8 movw     r2, #0x8c3a
003413ac movw     r3, #0x798e
003413b0 mov      sb, r1
003413b4 movt     r2, #0xe230
003413b8 bic      r1, r1, #0x80000000
003413bc movt     r3, #0x3e45
003413c0 mov      r8, r0
003413c4 bl       #0x30e760
003413c8 cmp      r0, #0
003413cc movne    r0, #0
003413d0 bne      #0x3415d0
003413d4 mov      r1, #0x3fc00000
003413d8 mov      r2, r8
003413dc mov      r3, sb
003413e0 mov      r0, #0
003413e4 add      r1, r1, #0x300000
003413e8 bl       #0x30e340
003413ec strd     r0, r1, [sp, #0x20]
003413f0 ldr      r3, [r4, #4]
003413f4 ldr      sb, [r5, #8]
003413f8 ldr      r2, [r5, #4]
003413fc add      r0, r3, #0x80000000
00341400 mov      r1, sb
00341404 str      r3, [sp, #4]
00341408 str      r2, [sp, #0x18]
0034140c bl       #0x30ed6c
00341410 ldr      r1, [sp, #0x18]
00341414 mov      r8, r0
00341418 ldr      r0, [r4, #8]
0034141c bl       #0x30ed6c
00341420 mov      r1, r0
00341424 mov      r0, r8
00341428 bl       #0x30eba4
0034142c ldr      ip, [r4, #8]
00341430 ldr      r2, [r5]
00341434 add      r1, ip, #0x80000000
00341438 ldr      ip, [r4, #0xc]
0034143c str      ip, [sp, #0x1c]
00341440 ldr      ip, [r5, #0xc]
00341444 str      ip, [sp, #0x10]
00341448 ldr      r8, [r4]
0034144c str      r0, [r7]
00341450 mov      r0, r1
00341454 mov      r1, r2
00341458 str      r2, [sp, #8]
0034145c bl       #0x30ed6c
00341460 mov      r1, r8
00341464 mov      ip, r0
00341468 mov      r0, sb
0034146c str      ip, [sp, #0xc]
00341470 bl       #0x30ed6c
00341474 ldr      ip, [sp, #0xc]
00341478 mov      r1, r0
0034147c mov      r0, ip
00341480 bl       #0x30eba4
00341484 str      r0, [r7, #4]
00341488 add      r1, r8, #0x80000000
0034148c ldr      r0, [sp, #0x18]
00341490 bl       #0x30ed6c
00341494 ldr      r3, [sp, #4]
00341498 ldr      r2, [sp, #8]
0034149c mov      r8, r0
003414a0 mov      r0, r3
003414a4 mov      r1, r2
003414a8 bl       #0x30ed6c
003414ac mov      r1, r0
003414b0 mov      r0, r8
003414b4 bl       #0x30eba4
003414b8 ldr      r2, [sp, #0x1c]
003414bc mov      r1, fp
003414c0 str      r0, [r7, #8]
003414c4 add      r3, r2, #0x80000000
003414c8 mov      r0, r3
003414cc bl       #0x30ed6c
003414d0 mov      r1, r6
003414d4 mov      r7, r0
003414d8 ldr      r0, [sp, #0x10]
003414dc bl       #0x30ed6c
003414e0 mov      r1, r0
003414e4 mov      r0, r7
003414e8 bl       #0x30eba4
003414ec bl       #0x30e8a4
003414f0 ldrd     r2, r3, [sp, #0x20]
003414f4 bl       #0x30eab4
003414f8 bl       #0x30e6a0
003414fc ldr      r3, [sp, #0x10]
00341500 mov      r7, r0
00341504 ldr      r1, [sp, #0x14]
00341508 add      r0, r3, #0x80000000
0034150c bl       #0x30ed6c
00341510 mov      r1, r6
00341514 mov      r8, r0
00341518 ldr      r0, [sp, #0x1c]
0034151c bl       #0x30ed6c
00341520 mov      r1, r0
00341524 mov      r0, r8
00341528 bl       #0x30eba4
0034152c bl       #0x30e8a4
00341530 ldrd     r2, r3, [sp, #0x20]
00341534 bl       #0x30eab4
00341538 bl       #0x30e6a0
0034153c ldr      r1, [r4, #4]
00341540 mov      r6, r0
00341544 mov      r0, r7
00341548 bl       #0x30ed6c
0034154c ldr      r1, [r5, #4]
00341550 mov      r8, r0
00341554 mov      r0, r6
00341558 bl       #0x30ed6c
0034155c mov      r1, r0
00341560 mov      r0, r8
00341564 bl       #0x30eba4
00341568 ldr      r1, [r4, #8]
0034156c mov      r8, r0
00341570 mov      r0, r7
00341574 bl       #0x30ed6c
00341578 ldr      r1, [r5, #8]
0034157c mov      sb, r0
00341580 mov      r0, r6
00341584 bl       #0x30ed6c
00341588 mov      r1, r0
0034158c mov      r0, sb
00341590 bl       #0x30eba4
00341594 ldr      r1, [r4]
00341598 mov      sb, r0
0034159c mov      r0, r7
003415a0 bl       #0x30ed6c
003415a4 ldr      r1, [r5]
003415a8 mov      r4, r0
003415ac mov      r0, r6
003415b0 bl       #0x30ed6c
003415b4 mov      r1, r0
003415b8 mov      r0, r4
003415bc bl       #0x30eba4
003415c0 str      r0, [sl]
003415c4 str      sb, [sl, #8]
003415c8 str      r8, [sl, #4]
003415cc mov      r0, #1
003415d0 add      sp, sp, #0x2c
003415d4 pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
_ZNK6glitch4core7plane3dIfE23getIntersectionWithLineERKNS0_8vector3dIfEES6_RS4_
0034033c push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
00340340 mov      r5, r1
00340344 ldr      r6, [r2]
00340348 ldr      r1, [r2, #4]
0034034c ldr      r8, [r0]
00340350 sub      sp, sp, #0x14
00340354 ldr      r7, [r0, #4]
00340358 mov      r4, r0
0034035c str      r1, [sp, #8]
00340360 mov      r0, r8
00340364 mov      r1, r6
00340368 ldr      sb, [r2, #8]
0034036c mov      sl, r3
00340370 bl       #0x30ed6c
00340374 ldr      r1, [sp, #8]
00340378 mov      fp, r0
0034037c mov      r0, r7
00340380 bl       #0x30ed6c
00340384 mov      r1, r0
00340388 mov      r0, fp
0034038c bl       #0x30eba4
00340390 ldr      fp, [r4, #8]
00340394 mov      r3, r0
00340398 mov      r1, sb
0034039c mov      r0, fp
003403a0 str      r3, [sp]
003403a4 bl       #0x30ed6c
003403a8 ldr      r3, [sp]
003403ac mov      r1, r0
003403b0 mov      r0, r3
003403b4 bl       #0x30eba4
003403b8 mov      r1, #0
003403bc str      r0, [sp, #0xc]
003403c0 bl       #0x30df8c
003403c4 cmp      r0, #0
003403c8 movne    r0, #0
003403cc bne      #0x34049c
003403d0 ldr      r3, [r5]
003403d4 mov      r0, r8
003403d8 ldr      r8, [r5, #4]
003403dc mov      r1, r3
003403e0 str      r3, [sp]
003403e4 bl       #0x30ed6c
003403e8 mov      r1, r8
003403ec mov      r2, r0
003403f0 mov      r0, r7
003403f4 ldr      r5, [r5, #8]
003403f8 str      r2, [sp, #4]
003403fc bl       #0x30ed6c
00340400 ldr      r2, [sp, #4]
00340404 mov      r1, r0
00340408 mov      r0, r2
0034040c bl       #0x30eba4
00340410 mov      r1, r5
00340414 mov      r7, r0
00340418 mov      r0, fp
0034041c bl       #0x30ed6c
00340420 mov      r1, r0
00340424 mov      r0, r7
00340428 bl       #0x30eba4
0034042c ldr      r1, [r4, #0xc]
00340430 bl       #0x30eba4
00340434 ldr      r1, [sp, #0xc]
00340438 add      r0, r0, #0x80000000
0034043c bl       #0x30ec94
00340440 mov      r1, r6
00340444 mov      r4, r0
00340448 bl       #0x30ed6c
0034044c ldr      r3, [sp]
00340450 mov      r1, r0
00340454 mov      r0, r3
00340458 bl       #0x30eba4
0034045c str      r0, [sl]
00340460 ldr      r1, [sp, #8]
00340464 mov      r0, r4
00340468 bl       #0x30ed6c
0034046c mov      r1, r0
00340470 mov      r0, r8
00340474 bl       #0x30eba4
00340478 mov      r1, sb
0034047c str      r0, [sl, #4]
00340480 mov      r0, r4
00340484 bl       #0x30ed6c
00340488 mov      r1, r0
0034048c mov      r0, r5
00340490 bl       #0x30eba4
00340494 str      r0, [sl, #8]
00340498 mov      r0, #1
0034049c add      sp, sp, #0x14
003404a0 pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
_ZNK6glitch4core7plane3dIfE30getIntersectionWithLimitedLineERKNS0_8vector3dIfEES6_RS4_
0040f0b4 push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0040f0b8 mov      r4, r1
0040f0bc sub      sp, sp, #0x1c
0040f0c0 ldr      r1, [r1, #4]
0040f0c4 mov      sl, r0
0040f0c8 ldr      r0, [r2, #4]
0040f0cc mov      r5, r2
0040f0d0 mov      r6, r3
0040f0d4 bl       #0x30e3ac
0040f0d8 ldr      r1, [r4, #8]
0040f0dc mov      r7, r0
0040f0e0 ldr      r0, [r5, #8]
0040f0e4 bl       #0x30e3ac
0040f0e8 ldr      r1, [r4]
0040f0ec mov      r8, r0
0040f0f0 ldr      r0, [r5]
0040f0f4 bl       #0x30e3ac
0040f0f8 mov      r1, r4
0040f0fc str      r0, [sp, #0xc]
0040f100 add      r2, sp, #0xc
0040f104 mov      r0, sl
0040f108 mov      r3, r6
0040f10c str      r7, [sp, #0x10]
0040f110 str      r8, [sp, #0x14]
0040f114 bl       #0x34033c ; _ZNK6glitch4core7plane3dIfE23getIntersectionWithLineERKNS0_8vector3dIfEES6_RS4_
0040f118 cmp      r0, #0
0040f11c beq      #0x40f250
0040f120 ldr      r3, [r5]
0040f124 ldr      r8, [r4]
0040f128 mov      r0, r3
0040f12c mov      r1, r8
0040f130 str      r3, [sp]
0040f134 bl       #0x30e3ac
0040f138 ldr      r2, [r5, #4]
0040f13c ldr      r7, [r4, #4]
0040f140 mov      sb, r0
0040f144 mov      r0, r2
0040f148 mov      r1, r7
0040f14c str      r2, [sp, #4]
0040f150 bl       #0x30e3ac
0040f154 ldr      r4, [r4, #8]
0040f158 ldr      fp, [r5, #8]
0040f15c mov      sl, r0
0040f160 mov      r1, r4
0040f164 mov      r0, fp
0040f168 bl       #0x30e3ac
0040f16c mov      r1, sb
0040f170 mov      r5, r0
0040f174 mov      r0, sb
0040f178 bl       #0x30ed6c
0040f17c mov      r1, sl
0040f180 mov      sb, r0
0040f184 mov      r0, sl
0040f188 bl       #0x30ed6c
0040f18c mov      r1, r0
0040f190 mov      r0, sb
0040f194 bl       #0x30eba4
0040f198 mov      r1, r5
0040f19c mov      sl, r0
0040f1a0 mov      r0, r5
0040f1a4 bl       #0x30ed6c
0040f1a8 mov      r1, r0
0040f1ac mov      r0, sl
0040f1b0 bl       #0x30eba4
0040f1b4 ldr      sl, [r6]
0040f1b8 mov      r1, r8
0040f1bc mov      r5, r0
0040f1c0 mov      r0, sl
0040f1c4 bl       #0x30e3ac
0040f1c8 ldr      r8, [r6, #4]
0040f1cc mov      sb, r0
0040f1d0 mov      r1, r7
0040f1d4 mov      r0, r8
0040f1d8 bl       #0x30e3ac
0040f1dc ldr      r6, [r6, #8]
0040f1e0 mov      r7, r0
0040f1e4 mov      r1, r4
0040f1e8 mov      r0, r6
0040f1ec bl       #0x30e3ac
0040f1f0 mov      r1, sb
0040f1f4 mov      r4, r0
0040f1f8 mov      r0, sb
0040f1fc bl       #0x30ed6c
0040f200 mov      r1, r7
0040f204 mov      sb, r0
0040f208 mov      r0, r7
0040f20c bl       #0x30ed6c
0040f210 mov      r1, r0
0040f214 mov      r0, sb
0040f218 bl       #0x30eba4
0040f21c mov      r1, r4
0040f220 mov      r7, r0
0040f224 mov      r0, r4
0040f228 bl       #0x30ed6c
0040f22c mov      r1, r0
0040f230 mov      r0, r7
0040f234 bl       #0x30eba4
0040f238 mov      r1, r0
0040f23c mov      r0, r5
0040f240 bl       #0x30e4b4
0040f244 cmp      r0, #0
0040f248 ldr      r3, [sp]
0040f24c bne      #0x40f25c
0040f250 mov      r0, #0
0040f254 add      sp, sp, #0x1c
0040f258 pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
0040f25c mov      r1, r3
0040f260 mov      r0, sl
0040f264 bl       #0x30e3ac
0040f268 ldr      r1, [sp, #4]
0040f26c mov      r4, r0
0040f270 mov      r0, r8
0040f274 bl       #0x30e3ac
0040f278 mov      r1, fp
0040f27c mov      r7, r0
0040f280 mov      r0, r6
0040f284 bl       #0x30e3ac
0040f288 mov      r1, r4
0040f28c mov      r6, r0
0040f290 mov      r0, r4
0040f294 bl       #0x30ed6c
0040f298 mov      r1, r7
0040f29c mov      r4, r0
0040f2a0 mov      r0, r7
0040f2a4 bl       #0x30ed6c
0040f2a8 mov      r1, r0
0040f2ac mov      r0, r4
0040f2b0 bl       #0x30eba4
0040f2b4 mov      r1, r6
0040f2b8 mov      r4, r0
0040f2bc mov      r0, r6
0040f2c0 bl       #0x30ed6c
0040f2c4 mov      r1, r0
0040f2c8 mov      r0, r4
0040f2cc bl       #0x30eba4
0040f2d0 mov      r1, r0
0040f2d4 mov      r0, r5
0040f2d8 bl       #0x30e4b4
0040f2dc cmp      r0, #0
0040f2e0 mov      r0, #0
0040f2e4 movne    r0, #1
0040f2e8 uxtb     r0, r0
0040f2ec b        #0x40f254
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
