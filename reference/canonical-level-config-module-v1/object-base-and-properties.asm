_ZN10ObjectBaseC2ENS_6GO_IDSE
0033f310 push     {r4, r5, r6, r7, r8, lr}
0033f314 ldr      r6, [pc, #0x198]
0033f318 ldr      r2, [pc, #0x198]
0033f31c ldr      r3, [pc, #0x198]
0033f320 add      r6, pc, r6
0033f324 ldr      r2, [r6, r2]
0033f328 ldr      r3, [r6, r3]
0033f32c mov      r4, r0
0033f330 add      r2, r2, #8
0033f334 add      r0, r3, #8
0033f338 add      r3, r4, #8
0033f33c str      r2, [r4]
0033f340 str      r0, [r4, #4]
0033f344 mov      r7, r1
0033f348 mov      r0, r3
0033f34c str      r3, [r4, #0x18]
0033f350 str      r3, [r4, #0x1c]
0033f354 mov      r1, #0x10
0033f358 bl       #0x31167c ; _ZNSt4priv12_String_baseIcSaIcEE17_M_allocate_blockEj
0033f35c ldr      r2, [pc, #0x15c]
0033f360 ldr      r1, [r4, #0x18]
0033f364 mov      r5, #0
0033f368 ldr      r2, [r6, r2]
0033f36c strb     r5, [r1]
0033f370 add      r3, r4, #0x30
0033f374 add      r1, r2, #0x74
0033f378 add      r0, r2, #8
0033f37c add      r2, r2, #0x68
0033f380 stm      r4, {r0, r2}
0033f384 str      r1, [r4, #0x24]
0033f388 mov      r0, r3
0033f38c str      r5, [r4, #0x20]
0033f390 strb     r5, [r4, #0x28]
0033f394 strb     r5, [r4, #0x29]
0033f398 str      r5, [r4, #0x2c]
0033f39c str      r3, [r4, #0x40]
0033f3a0 str      r3, [r4, #0x44]
0033f3a4 mov      r1, #0x10
0033f3a8 bl       #0x31167c ; _ZNSt4priv12_String_baseIcSaIcEE17_M_allocate_blockEj
0033f3ac ldr      r2, [r4, #0x40]
0033f3b0 add      r3, r4, #0x48
0033f3b4 mov      r0, r3
0033f3b8 strb     r5, [r2]
0033f3bc mov      r1, #0x10
0033f3c0 str      r3, [r4, #0x58]
0033f3c4 str      r3, [r4, #0x5c]
0033f3c8 bl       #0x31167c ; _ZNSt4priv12_String_baseIcSaIcEE17_M_allocate_blockEj
0033f3cc ldr      r2, [r4, #0x58]
0033f3d0 add      r3, r4, #0x68
0033f3d4 mvn      r6, #0
0033f3d8 strb     r5, [r2]
0033f3dc mov      r1, #0x10
0033f3e0 mov      r0, r3
0033f3e4 strb     r5, [r4, #0x60]
0033f3e8 str      r3, [r4, #0x78]
0033f3ec str      r3, [r4, #0x7c]
0033f3f0 str      r6, [r4, #0x64]
0033f3f4 bl       #0x31167c ; _ZNSt4priv12_String_baseIcSaIcEE17_M_allocate_blockEj
0033f3f8 ldr      r3, [r4, #0x78]
0033f3fc add      r0, r4, #0x8c
0033f400 strb     r5, [r3]
0033f404 mov      r3, #1
0033f408 strb     r3, [r4, #0x8a]
0033f40c strb     r5, [r4, #0x81]
0033f410 strb     r5, [r4, #0x84]
0033f414 strb     r5, [r4, #0x85]
0033f418 strb     r5, [r4, #0x86]
0033f41c strb     r5, [r4, #0x88]
0033f420 strb     r5, [r4, #0x89]
0033f424 bl       #0x33ed7c ; _ZN13ConditionDataC1Ev
0033f428 add      r0, r4, #0xb0
0033f42c bl       #0x33ed7c ; _ZN13ConditionDataC1Ev
0033f430 add      r3, r4, #0xd4
0033f434 mov      r0, r3
0033f438 str      r3, [r4, #0xe4]
0033f43c str      r3, [r4, #0xe8]
0033f440 mov      r1, #0x10
0033f444 bl       #0x31167c ; _ZNSt4priv12_String_baseIcSaIcEE17_M_allocate_blockEj
0033f448 ldr      r3, [r4, #0xe4]
0033f44c mov      r1, r5
0033f450 mov      r0, #0xc
0033f454 strb     r5, [r3]
0033f458 mov      r3, #0
0033f45c str      r3, [r4, #0x114]
0033f460 strb     r5, [r4, #0xf0]
0033f464 strb     r5, [r4, #0xf1]
0033f468 strb     r5, [r4, #0xf8]
0033f46c str      r5, [r4, #0xfc]
0033f470 str      r5, [r4, #0x100]
0033f474 str      r5, [r4, #0x104]
0033f478 strb     r5, [r4, #0x10c]
0033f47c strb     r5, [r4, #0x118]
0033f480 strb     r5, [r4, #0x119]
0033f484 str      r5, [r4, #0x11c]
0033f488 str      r7, [r4, #0xf4]
0033f48c str      r6, [r4, #0x110]
0033f490 str      r6, [r4, #0xec]
0033f494 str      r6, [r4, #0x108]
0033f498 bl       #0x310570 ; _Znwj15MemoryHintState
0033f49c mov      r5, r0
0033f4a0 bl       #0x33f50c ; _ZN12ObjectHandleC1Ev
0033f4a4 str      r5, [r4, #0x2c]
0033f4a8 mov      r0, r4
0033f4ac str      r4, [r5, #4]
0033f4b0 pop      {r4, r5, r6, r7, r8, pc}
0033f4b4 rsbeq    r5, r5, r0, ror r7
0033f4b8 andeq    r1, r0, ip, lsl #1
0033f4bc ldrdeq   r3, r4, [r0], -ip
0033f4c0 andeq    r3, r0, r4, lsl #23
_ZN11LevelConfig17DeclarePropertiesEv
003f4b9c push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
003f4ba0 ldr      sb, [pc, #0x54c]
003f4ba4 ldr      r3, [pc, #0x54c]
003f4ba8 ldr      r1, [pc, #0x54c]
003f4bac add      sb, pc, sb
003f4bb0 ldr      ip, [sb, r3]
003f4bb4 mov      r4, r0
003f4bb8 add      r5, r0, #4
003f4bbc ldr      r3, [ip]
003f4bc0 sub      sp, sp, #0x13c
003f4bc4 add      r1, pc, r1
003f4bc8 mov      r0, r5
003f4bcc add      r2, r4, #0x150
003f4bd0 str      ip, [sp, #4]
003f4bd4 str      r3, [sp, #0x134]
003f4bd8 bl       #0x33ef7c ; _ZN11PropertyMap11AddPropertyISsEEvPKcRT_
003f4bdc ldr      r1, [pc, #0x51c]
003f4be0 mov      r0, r5
003f4be4 add      r2, r4, #0x24c
003f4be8 add      r1, pc, r1
003f4bec bl       #0x33ef7c ; _ZN11PropertyMap11AddPropertyISsEEvPKcRT_
003f4bf0 ldr      r1, [pc, #0x50c]
003f4bf4 add      r6, sp, #0x11c
003f4bf8 add      r2, sp, #0x88
003f4bfc add      r1, pc, r1
003f4c00 mov      r0, r6
003f4c04 bl       #0x3140ec ; _ZNSsC1EPKcRKSaIcE
003f4c08 ldr      r1, [pc, #0x4f8]
003f4c0c mov      r3, r6
003f4c10 add      r2, r4, #0x264
003f4c14 add      r1, pc, r1
003f4c18 mov      r0, r5
003f4c1c bl       #0x33e404 ; _ZN11PropertyMap11AddPropertyISsEEvPKcRT_S3_
003f4c20 mov      r0, r6
003f4c24 bl       #0x3139ac ; _ZNSt4priv12_String_baseIcSaIcEE19_M_deallocate_blockEv
003f4c28 ldr      r1, [pc, #0x4dc]
003f4c2c mov      r0, r5
003f4c30 add      r2, r4, #0x27c
003f4c34 add      r1, pc, r1
003f4c38 bl       #0x33ef7c ; _ZN11PropertyMap11AddPropertyISsEEvPKcRT_
003f4c3c ldr      r1, [pc, #0x4cc]
003f4c40 mov      r0, r5
003f4c44 add      r2, r4, #0x294
003f4c48 add      r1, pc, r1
003f4c4c mov      r3, #0x258
003f4c50 bl       #0x398878 ; _ZN11PropertyMap11AddPropertyIiEEvPKcRT_S3_
003f4c54 ldr      r1, [pc, #0x4b8]
003f4c58 movw     r3, #0x2710
003f4c5c mov      r0, r5
003f4c60 add      r1, pc, r1
003f4c64 add      r2, r4, #0x298
003f4c68 bl       #0x398878 ; _ZN11PropertyMap11AddPropertyIiEEvPKcRT_S3_
003f4c6c ldr      r1, [pc, #0x4a4]
003f4c70 mov      r0, r5
003f4c74 add      r2, r4, #0x234
003f4c78 add      r1, pc, r1
003f4c7c bl       #0x33ef7c ; _ZN11PropertyMap11AddPropertyISsEEvPKcRT_
003f4c80 ldr      r1, [pc, #0x494]
003f4c84 mov      r0, r5
003f4c88 add      r2, r4, #0x168
003f4c8c add      r1, pc, r1
003f4c90 bl       #0x33ef7c ; _ZN11PropertyMap11AddPropertyISsEEvPKcRT_
003f4c94 ldr      r1, [pc, #0x484]
003f4c98 mov      r0, r5
003f4c9c add      r2, r4, #0x198
003f4ca0 add      r1, pc, r1
003f4ca4 bl       #0x33ef7c ; _ZN11PropertyMap11AddPropertyISsEEvPKcRT_
003f4ca8 ldr      r1, [pc, #0x474]
003f4cac mov      r0, r5
003f4cb0 add      r2, r4, #0x1b0
003f4cb4 add      r1, pc, r1
003f4cb8 bl       #0x33ef7c ; _ZN11PropertyMap11AddPropertyISsEEvPKcRT_
003f4cbc ldr      r1, [pc, #0x464]
003f4cc0 mov      r0, r5
003f4cc4 add      r2, r4, #0x180
003f4cc8 add      r1, pc, r1
003f4ccc bl       #0x33ef7c ; _ZN11PropertyMap11AddPropertyISsEEvPKcRT_
003f4cd0 ldr      r1, [pc, #0x454]
003f4cd4 mov      r0, r5
003f4cd8 add      r2, r4, #0x1c8
003f4cdc add      r1, pc, r1
003f4ce0 mov      r3, #1
003f4ce4 bl       #0x33e4ac ; _ZN11PropertyMap11AddPropertyIbEEvPKcRT_S3_
003f4ce8 ldr      r1, [pc, #0x440]
003f4cec mov      r6, #0x43000000
003f4cf0 add      r6, r6, #0x7f0000
003f4cf4 mov      r0, r5
003f4cf8 add      r1, pc, r1
003f4cfc add      r2, r4, #0x1cc
003f4d00 add      r3, sp, #0x60
003f4d04 str      r6, [sp, #0x60]
003f4d08 str      r6, [sp, #0x64]
003f4d0c str      r6, [sp, #0x68]
003f4d10 bl       #0x389894 ; _ZN11PropertyMap11AddPropertyI7Point3DIfEEEvPKcRT_S5_
003f4d14 ldr      r1, [pc, #0x418]
003f4d18 mov      r0, r5
003f4d1c add      r2, r4, #0x1d8
003f4d20 add      r1, pc, r1
003f4d24 mov      r3, #1
003f4d28 bl       #0x398878 ; _ZN11PropertyMap11AddPropertyIiEEvPKcRT_S3_
003f4d2c ldr      r1, [pc, #0x404]
003f4d30 mov      r0, r5
003f4d34 add      r2, r4, #0x1dc
003f4d38 add      r1, pc, r1
003f4d3c mov      r3, #0
003f4d40 bl       #0x398878 ; _ZN11PropertyMap11AddPropertyIiEEvPKcRT_S3_
003f4d44 ldr      r1, [pc, #0x3f0]
003f4d48 mov      r0, r5
003f4d4c add      r2, r4, #0x1e0
003f4d50 add      r1, pc, r1
003f4d54 add      r3, sp, #0x54
003f4d58 str      r6, [sp, #0x5c]
003f4d5c str      r6, [sp, #0x54]
003f4d60 str      r6, [sp, #0x58]
003f4d64 bl       #0x389894 ; _ZN11PropertyMap11AddPropertyI7Point3DIfEEEvPKcRT_S5_
003f4d68 ldr      r1, [pc, #0x3d0]
003f4d6c mov      r6, #0
003f4d70 mov      lr, #0x3f800000
003f4d74 add      r1, pc, r1
003f4d78 mov      r0, r5
003f4d7c add      r2, r4, #0x1f8
003f4d80 add      r3, sp, #0x48
003f4d84 str      lr, [sp, #0x50]
003f4d88 str      r6, [sp, #0x48]
003f4d8c str      r6, [sp, #0x4c]
003f4d90 bl       #0x389894 ; _ZN11PropertyMap11AddPropertyI7Point3DIfEEEvPKcRT_S5_
003f4d94 ldr      r1, [pc, #0x3a8]
003f4d98 mov      r0, r5
003f4d9c add      r2, r4, #0x1ec
003f4da0 add      r1, pc, r1
003f4da4 add      r3, sp, #0x3c
003f4da8 str      r6, [sp, #0x3c]
003f4dac str      r6, [sp, #0x40]
003f4db0 str      r6, [sp, #0x44]
003f4db4 bl       #0x389894 ; _ZN11PropertyMap11AddPropertyI7Point3DIfEEEvPKcRT_S5_
003f4db8 ldr      r1, [pc, #0x388]
003f4dbc movw     lr, #0x2400
003f4dc0 movt     lr, #0xc974
003f4dc4 mov      r0, r5
003f4dc8 add      r1, pc, r1
003f4dcc add      r2, r4, #0x218
003f4dd0 add      r3, sp, #0x30
003f4dd4 str      lr, [sp, #0x38]
003f4dd8 str      lr, [sp, #0x30]
003f4ddc str      lr, [sp, #0x34]
003f4de0 bl       #0x389894 ; _ZN11PropertyMap11AddPropertyI7Point3DIfEEEvPKcRT_S5_
003f4de4 ldr      r1, [pc, #0x360]
003f4de8 mov      r0, r5
003f4dec add      r2, r4, #0x224
003f4df0 add      r1, pc, r1
003f4df4 add      r3, sp, #0x24
003f4df8 str      r6, [sp, #0x24]
003f4dfc str      r6, [sp, #0x28]
003f4e00 str      r6, [sp, #0x2c]
003f4e04 bl       #0x389894 ; _ZN11PropertyMap11AddPropertyI7Point3DIfEEEvPKcRT_S5_
003f4e08 ldr      r1, [pc, #0x340]
003f4e0c mov      r3, r6
003f4e10 mov      r0, r5
003f4e14 add      r1, pc, r1
003f4e18 add      r2, r4, #0x230
003f4e1c bl       #0x39503c ; _ZN11PropertyMap11AddPropertyIfEEvPKcRT_S3_
003f4e20 ldr      r1, [pc, #0x32c]
003f4e24 add      r6, sp, #0x104
003f4e28 add      r2, sp, #0x84
003f4e2c add      r1, pc, r1
003f4e30 mov      r0, r6
003f4e34 bl       #0x3140ec ; _ZNSsC1EPKcRKSaIcE
003f4e38 ldr      r1, [pc, #0x318]
003f4e3c mov      r3, r6
003f4e40 add      r2, r4, #0x120
003f4e44 add      r1, pc, r1
003f4e48 mov      r0, r5
003f4e4c bl       #0x33e404 ; _ZN11PropertyMap11AddPropertyISsEEvPKcRT_S3_
003f4e50 mov      r0, r6
003f4e54 bl       #0x3139ac ; _ZNSt4priv12_String_baseIcSaIcEE19_M_deallocate_blockEv
003f4e58 ldr      r1, [pc, #0x2fc]
003f4e5c add      r7, sp, #0xec
003f4e60 add      r2, sp, #0x80
003f4e64 add      r1, pc, r1
003f4e68 mov      r0, r7
003f4e6c bl       #0x3140ec ; _ZNSsC1EPKcRKSaIcE
003f4e70 ldr      r1, [pc, #0x2e8]
003f4e74 ldr      r6, [pc, #0x2e8]
003f4e78 mov      r3, r7
003f4e7c add      r2, r4, #0x138
003f4e80 add      r1, pc, r1
003f4e84 mov      r0, r5
003f4e88 bl       #0x33e404 ; _ZN11PropertyMap11AddPropertyISsEEvPKcRT_S3_
003f4e8c add      r6, pc, r6
003f4e90 mov      r0, r7
003f4e94 add      r7, sp, #0xd4
003f4e98 bl       #0x3139ac ; _ZNSt4priv12_String_baseIcSaIcEE19_M_deallocate_blockEv
003f4e9c mov      r1, r6
003f4ea0 add      r2, sp, #0x7c
003f4ea4 mov      r0, r7
003f4ea8 bl       #0x3140ec ; _ZNSsC1EPKcRKSaIcE
003f4eac ldr      r1, [pc, #0x2b4]
003f4eb0 mov      r3, r7
003f4eb4 add      r2, r4, #0x2b8
003f4eb8 add      r1, pc, r1
003f4ebc mov      r0, r5
003f4ec0 bl       #0x33e404 ; _ZN11PropertyMap11AddPropertyISsEEvPKcRT_S3_
003f4ec4 mov      r0, r7
003f4ec8 add      r7, sp, #0xbc
003f4ecc bl       #0x3139ac ; _ZNSt4priv12_String_baseIcSaIcEE19_M_deallocate_blockEv
003f4ed0 mov      r1, r6
003f4ed4 add      r2, sp, #0x78
003f4ed8 mov      r0, r7
003f4edc bl       #0x3140ec ; _ZNSsC1EPKcRKSaIcE
003f4ee0 ldr      r1, [pc, #0x284]
003f4ee4 mov      r3, r7
003f4ee8 add      r2, r4, #0x2d0
003f4eec add      r1, pc, r1
003f4ef0 mov      r0, r5
003f4ef4 bl       #0x33e404 ; _ZN11PropertyMap11AddPropertyISsEEvPKcRT_S3_
003f4ef8 mov      r0, r7
003f4efc add      r7, sp, #0xa4
003f4f00 bl       #0x3139ac ; _ZNSt4priv12_String_baseIcSaIcEE19_M_deallocate_blockEv
003f4f04 mov      r1, r6
003f4f08 add      r2, sp, #0x74
003f4f0c mov      r0, r7
003f4f10 bl       #0x3140ec ; _ZNSsC1EPKcRKSaIcE
003f4f14 ldr      r1, [pc, #0x254]
003f4f18 mov      r3, r7
003f4f1c add      r2, r4, #0x300
003f4f20 add      r1, pc, r1
003f4f24 mov      r0, r5
003f4f28 bl       #0x33e404 ; _ZN11PropertyMap11AddPropertyISsEEvPKcRT_S3_
003f4f2c mov      r0, r7
003f4f30 bl       #0x3139ac ; _ZNSt4priv12_String_baseIcSaIcEE19_M_deallocate_blockEv
003f4f34 ldr      r1, [pc, #0x238]
003f4f38 add      r6, sp, #0x8c
003f4f3c add      r2, sp, #0x70
003f4f40 add      r1, pc, r1
003f4f44 mov      r0, r6
003f4f48 bl       #0x3140ec ; _ZNSsC1EPKcRKSaIcE
003f4f4c ldr      r1, [pc, #0x224]
003f4f50 mov      r3, r6
003f4f54 add      r2, r4, #0x2e8
003f4f58 add      r1, pc, r1
003f4f5c mov      r0, r5
003f4f60 bl       #0x33e404 ; _ZN11PropertyMap11AddPropertyISsEEvPKcRT_S3_
003f4f64 add      sl, sp, #0xc
003f4f68 mov      r0, r6
003f4f6c add      fp, sp, #0x18
003f4f70 bl       #0x3139ac ; _ZNSt4priv12_String_baseIcSaIcEE19_M_deallocate_blockEv
003f4f74 mov      r6, #0
003f4f78 mov      r1, fp
003f4f7c mov      r0, sl
003f4f80 str      r6, [sp, #0x18]
003f4f84 str      r6, [sp, #0x1c]
003f4f88 str      r6, [sp, #0x20]
003f4f8c bl       #0x3424b8 ; _ZNSt6vectorI7Point3DIfESaIS1_EEC1ERKS3_
003f4f90 mov      r1, r6
003f4f94 mov      r0, #0x2c
003f4f98 bl       #0x310570 ; _Znwj15MemoryHintState
003f4f9c ldr      r3, [pc, #0x1d8]
003f4fa0 ldr      r8, [pc, #0x1d8]
003f4fa4 mov      r7, r0
003f4fa8 ldr      r3, [sb, r3]
003f4fac add      r8, pc, r8
003f4fb0 mov      r1, r8
003f4fb4 add      r3, r3, #8
003f4fb8 str      r3, [r0], #8
003f4fbc add      r2, sp, #0x6c
003f4fc0 bl       #0x3140ec ; _ZNSsC1EPKcRKSaIcE
003f4fc4 ldr      r3, [pc, #0x1b8]
003f4fc8 add      r2, r4, #0x204
003f4fcc rsb      r2, r5, r2
003f4fd0 ldr      r3, [sb, r3]
003f4fd4 mov      r0, r7
003f4fd8 str      r2, [r7, #4]
003f4fdc add      r3, r3, #8
003f4fe0 str      r3, [r0], #0x20
003f4fe4 mov      r1, sl
003f4fe8 bl       #0x3424b8 ; _ZNSt6vectorI7Point3DIfESaIS1_EEC1ERKS3_
003f4fec mov      r1, r8
003f4ff0 mov      r2, r7
003f4ff4 mov      r0, r5
003f4ff8 bl       #0x513ce4 ; _ZN11PropertyMap11AddPropertyEPKcP8Property
003f4ffc mov      r0, sl
003f5000 bl       #0x34611c ; _ZNSt6vectorI7Point3DIfESaIS1_EED1Ev
003f5004 mov      r0, fp
003f5008 bl       #0x34611c ; _ZNSt6vectorI7Point3DIfESaIS1_EED1Ev
003f500c ldr      r1, [pc, #0x174]
003f5010 mov      r3, #0x43000000
003f5014 mov      r0, r5
003f5018 add      r2, r4, #0x210
003f501c add      r1, pc, r1
003f5020 add      r3, r3, #0x480000
003f5024 bl       #0x39503c ; _ZN11PropertyMap11AddPropertyIfEEvPKcRT_S3_
003f5028 ldr      r1, [pc, #0x15c]
003f502c mov      r3, #0x42000000
003f5030 mov      r0, r5
003f5034 add      r2, r4, #0x214
003f5038 add      r1, pc, r1
003f503c add      r3, r3, #0xc80000
003f5040 bl       #0x39503c ; _ZN11PropertyMap11AddPropertyIfEEvPKcRT_S3_
003f5044 ldr      r1, [pc, #0x144]
003f5048 mov      r0, r5
003f504c add      r2, r4, #0x2a0
003f5050 add      r1, pc, r1
003f5054 mov      r3, #0x500000
003f5058 bl       #0x398878 ; _ZN11PropertyMap11AddPropertyIiEEvPKcRT_S3_
003f505c ldr      r1, [pc, #0x130]
003f5060 mov      r0, r5
003f5064 add      r2, r4, #0x2a4
003f5068 add      r1, pc, r1
003f506c mov      r3, #0xa0000
003f5070 bl       #0x398878 ; _ZN11PropertyMap11AddPropertyIiEEvPKcRT_S3_
003f5074 ldr      r1, [pc, #0x11c]
003f5078 mov      r0, r5
003f507c add      r2, r4, #0x2a8
003f5080 add      r1, pc, r1
003f5084 mov      r3, r6
003f5088 bl       #0x33e4ac ; _ZN11PropertyMap11AddPropertyIbEEvPKcRT_S3_
003f508c ldr      r1, [pc, #0x108]
003f5090 mov      r0, r5
003f5094 add      r2, r4, #0x2ac
003f5098 add      r1, pc, r1
003f509c mov      r3, #0x500000
003f50a0 bl       #0x398878 ; _ZN11PropertyMap11AddPropertyIiEEvPKcRT_S3_
003f50a4 ldr      r1, [pc, #0xf4]
003f50a8 mov      r0, r5
003f50ac add      r2, r4, #0x2b0
003f50b0 add      r1, pc, r1
003f50b4 mov      r3, #0xa0000
003f50b8 bl       #0x398878 ; _ZN11PropertyMap11AddPropertyIiEEvPKcRT_S3_
003f50bc ldr      r1, [pc, #0xe0]
003f50c0 add      r2, r4, #0x2b4
003f50c4 mov      r3, r6
003f50c8 mov      r0, r5
003f50cc add      r1, pc, r1
003f50d0 bl       #0x33e4ac ; _ZN11PropertyMap11AddPropertyIbEEvPKcRT_S3_
003f50d4 ldr      ip, [sp, #4]
003f50d8 ldr      r2, [sp, #0x134]
003f50dc ldr      r3, [ip]
003f50e0 cmp      r2, r3
003f50e4 bne      #0x3f50f0
003f50e8 add      sp, sp, #0x13c
003f50ec pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
003f50f0 bl       #0x30e310
003f50f4 subseq   pc, sb, r4, ror #29
003f50f8 andeq    r4, r0, ip, lsr #1
003f50fc subeq    r1, sp, ip, lsl #25
003f5100 subeq    r1, sp, r8, ror ip
003f5104 subeq    r1, sp, r4, ror ip
003f5108 subeq    r1, sp, r4, ror #24
003f510c subeq    r1, sp, r4, asr ip
003f5110 subeq    r1, sp, r0, asr ip
003f5114 subeq    r1, sp, r8, asr #24
003f5118 subeq    r1, sp, r0, asr #24
003f511c subeq    r1, sp, ip, asr #24
003f5120 subeq    r1, sp, r0, lsr #24
003f5124 subeq    r1, sp, ip, lsl ip
003f5128 subeq    r1, sp, r8, lsl ip
003f512c subeq    r1, sp, r4, lsl ip
003f5130 subeq    r1, sp, r0, lsl ip
003f5134 strdeq   r1, r2, [sp], #-0xb8
003f5138 strdeq   r1, r2, [sp], #-0xb0
003f513c strdeq   sp, lr, [ip], #-0x50
003f5140 strheq   r1, [sp], #-0xbc
003f5144 subeq    r1, sp, r8, lsr #23
003f5148 umaaleq  r1, sp, r0, fp
003f514c subeq    r1, sp, r8, ror fp
003f5150 subeq    r1, sp, r4, ror #22
003f5154 subeq    fp, ip, r4, asr #12
003f5158 subeq    fp, ip, r4, lsr r3
003f515c subeq    r1, sp, r4, lsr #22
003f5160 subeq    ip, lr, r8, ror #4
003f5164 subeq    r6, sp, ip, ror sb
003f5168 subeq    r1, sp, r0, ror #21
003f516c strheq   r1, [sp], #-0xac
003f5170 umaaleq  r1, sp, r8, sl
003f5174 subeq    r1, sp, r8, lsl #21
003f5178 subeq    r1, sp, r8, lsl #21
003f517c andeq    r2, r0, r0, lsr r3
003f5180 subeq    r1, sp, r4, asr #20
003f5184 muleq    r0, r8, r6
003f5188 subeq    r1, sp, r4, ror #19
003f518c subeq    r1, sp, r0, ror #19
003f5190 ldrdeq   r1, r2, [sp], #-0x98
003f5194 ldrdeq   r1, r2, [sp], #-0x98
003f5198 ldrdeq   r1, r2, [sp], #-0x98
003f519c ldrdeq   r1, r2, [sp], #-0x90
003f51a0 ldrdeq   r1, r2, [sp], #-0x90
003f51a4 subeq    r1, sp, ip, asr #19
