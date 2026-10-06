_ZN12VisualObjectD1Ev
00473884 push     {r4, r5, r6, lr}
00473888 ldr      r5, [pc, #0x204]
0047388c ldr      r3, [pc, #0x204]
00473890 mov      r4, r0
00473894 add      r5, pc, r5
00473898 ldr      r3, [r5, r3]
0047389c mov      r1, #0
004738a0 add      r3, r3, #8
004738a4 str      r3, [r0]
004738a8 bl       #0x470a84 ; _ZN12VisualObject17SetAnimControllerEP14AnimController
004738ac ldr      r3, [r4, #0xc]
004738b0 cmp      r3, #0
004738b4 beq      #0x4738d0
004738b8 ldr      r2, [r3]
004738bc ldr      r0, [r2, #-0xc]
004738c0 add      r0, r3, r0
004738c4 bl       #0x31d584 ; _ZNK6glitch17IReferenceCounted4dropEv
004738c8 mov      r3, #0
004738cc str      r3, [r4, #0xc]
004738d0 ldr      r3, [r4, #8]
004738d4 cmp      r3, #0
004738d8 beq      #0x47391c
004738dc mov      r0, r3
004738e0 ldr      r3, [r3]
004738e4 mov      lr, pc
004738e8 ldr      pc, [r3, #0x74]
004738ec ldr      r3, [r4, #8]
004738f0 mov      r0, r3
004738f4 ldr      r3, [r3]
004738f8 mov      lr, pc
004738fc ldr      pc, [r3, #0x68]
00473900 ldr      r3, [r4, #8]
00473904 ldr      r2, [r3]
00473908 ldr      r0, [r2, #-0xc]
0047390c add      r0, r3, r0
00473910 bl       #0x31d584 ; _ZNK6glitch17IReferenceCounted4dropEv
00473914 mov      r3, #0
00473918 str      r3, [r4, #8]
0047391c ldr      r3, [r4, #0x30]
00473920 cmp      r3, #0
00473924 beq      #0x473968
00473928 mov      r0, r3
0047392c ldr      r3, [r3]
00473930 mov      lr, pc
00473934 ldr      pc, [r3, #0x74]
00473938 ldr      r3, [r4, #0x30]
0047393c mov      r0, r3
00473940 ldr      r3, [r3]
00473944 mov      lr, pc
00473948 ldr      pc, [r3, #0x68]
0047394c ldr      r3, [r4, #0x30]
00473950 ldr      r2, [r3]
00473954 ldr      r0, [r2, #-0xc]
00473958 add      r0, r3, r0
0047395c bl       #0x31d584 ; _ZNK6glitch17IReferenceCounted4dropEv
00473960 mov      r3, #0
00473964 str      r3, [r4, #0x30]
00473968 ldr      r3, [r4, #0x34]
0047396c cmp      r3, #0
00473970 beq      #0x4739b4
00473974 mov      r0, r3
00473978 ldr      r3, [r3]
0047397c mov      lr, pc
00473980 ldr      pc, [r3, #0x74]
00473984 ldr      r3, [r4, #0x34]
00473988 mov      r0, r3
0047398c ldr      r3, [r3]
00473990 mov      lr, pc
00473994 ldr      pc, [r3, #0x68]
00473998 ldr      r3, [r4, #0x34]
0047399c ldr      r2, [r3]
004739a0 ldr      r0, [r2, #-0xc]
004739a4 add      r0, r3, r0
004739a8 bl       #0x31d584 ; _ZNK6glitch17IReferenceCounted4dropEv
004739ac mov      r3, #0
004739b0 str      r3, [r4, #0x34]
004739b4 ldr      r3, [pc, #0xe0]
004739b8 ldr      r3, [r5, r3]
004739bc ldr      r3, [r3, #0x10]
004739c0 ldr      r0, [r3, #0x1c]
004739c4 bl       #0x350ee0 ; _ZN12SceneManager13ForceRegisterEv
004739c8 ldr      r0, [r4, #0x9c]
004739cc add      r3, r4, #0x9c
004739d0 cmp      r0, #0
004739d4 beq      #0x4739f0
004739d8 ldr      r1, [r3, #8]
004739dc rsb      r1, r0, r1
004739e0 bic      r1, r1, #3
004739e4 cmp      r1, #0x80
004739e8 bhi      #0x473a70
004739ec bl       #0x708f00 ; ___ZNSt12__node_alloc13_M_deallocateEPvj_veneer
004739f0 ldr      r0, [r4, #0x8c]
004739f4 add      r3, r4, #0x8c
004739f8 cmp      r0, #0
004739fc beq      #0x473a18
00473a00 ldr      r1, [r3, #8]
00473a04 rsb      r1, r0, r1
00473a08 bic      r1, r1, #3
00473a0c cmp      r1, #0x80
00473a10 bhi      #0x473a8c
00473a14 bl       #0x708f00 ; ___ZNSt12__node_alloc13_M_deallocateEPvj_veneer
00473a18 ldr      r0, [r4, #0x80]
00473a1c add      r3, r4, #0x80
00473a20 cmp      r0, #0
00473a24 beq      #0x473a40
00473a28 ldr      r1, [r3, #8]
00473a2c rsb      r1, r0, r1
00473a30 bic      r1, r1, #3
00473a34 cmp      r1, #0x80
00473a38 bhi      #0x473a84
00473a3c bl       #0x708f00 ; ___ZNSt12__node_alloc13_M_deallocateEPvj_veneer
00473a40 ldr      r0, [r4, #0x44]
00473a44 add      r3, r4, #0x44
00473a48 cmp      r0, #0
00473a4c beq      #0x473a68
00473a50 ldr      r1, [r3, #0x10]
00473a54 rsb      r1, r0, r1
00473a58 bic      r1, r1, #3
00473a5c cmp      r1, #0x80
00473a60 bhi      #0x473a78
00473a64 bl       #0x708f00 ; ___ZNSt12__node_alloc13_M_deallocateEPvj_veneer
00473a68 mov      r0, r4
00473a6c pop      {r4, r5, r6, pc}
00473a70 bl       #0x310440 ; _Z10CustomFreePv
00473a74 b        #0x4739f0
00473a78 bl       #0x310440 ; _Z10CustomFreePv
00473a7c mov      r0, r4
00473a80 pop      {r4, r5, r6, pc}
00473a84 bl       #0x310440 ; _Z10CustomFreePv
00473a88 b        #0x473a40
00473a8c bl       #0x310440 ; _Z10CustomFreePv
00473a90 b        #0x473a18
00473a94 ldrsheq  r1, [r2], #-0x1c
00473a98 muleq    r0, r4, lr
00473a9c strdeq   r3, r4, [r0], -r4
