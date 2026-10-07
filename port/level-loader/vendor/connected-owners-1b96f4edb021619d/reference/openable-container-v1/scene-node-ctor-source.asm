_ZN6glitch5scene10ISceneNodeC2EiRKNS_4core8vector3dIfEERKNS2_10quaternionES6_
005990c0 push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
005990c4 mov      r5, #0
005990c8 sub      sp, sp, #0x14
005990cc mov      r8, r1
005990d0 str      r5, [r0, #4]
005990d4 str      r5, [r0, #8]
005990d8 mov      r4, r0
005990dc mov      sl, r3
005990e0 ldr      r7, [sp, #0x3c]
005990e4 str      r2, [sp, #0xc]
005990e8 bl       #0x6a118c ; _ZN6glitch7IObjectC2Ev
005990ec ldr      r2, [r8]
005990f0 add      r3, r4, #0xc
005990f4 mov      r0, r3
005990f8 str      r2, [r4]
005990fc ldr      r1, [r8, #4]
00599100 ldr      r2, [r2, #-0x1c]
00599104 mov      sb, #0x40
00599108 mov      r6, #0x3f800000
0059910c str      r1, [r4, r2]
00599110 ldr      r2, [r4]
00599114 ldr      r1, [r8, #8]
00599118 mov      r8, #1
0059911c ldr      r2, [r2, #-0xc]
00599120 str      r1, [r4, r2]
00599124 str      r3, [r4, #0x1c]
00599128 str      r3, [r4, #0x20]
0059912c bl       #0x598ee0 ; _ZNSt4priv12_String_baseIcN6glitch4core10SAllocatorIcLNS1_6memory13E_MEMORY_HINTE0EEEE17_M_allocate_blockEj.clone.2
00599130 ldr      r3, [r4, #0x1c]
00599134 mov      r1, r5
00599138 mov      r2, sb
0059913c strb     r5, [r3]
00599140 add      r0, r4, #0x24
00599144 strb     r5, [r4, #0x64]
00599148 bl       #0x30e460
0059914c mov      r2, sb
00599150 mov      r1, r5
00599154 str      r6, [r4, #0x24]
00599158 str      r6, [r4, #0x38]
0059915c str      r6, [r4, #0x4c]
00599160 str      r6, [r4, #0x60]
00599164 strb     r8, [r4, #0x64]
00599168 strb     r5, [r4, #0xa8]
0059916c add      r0, r4, #0x68
00599170 bl       #0x30e460
00599174 str      r6, [r4, #0x68]
00599178 str      r6, [r4, #0x7c]
0059917c str      r6, [r4, #0x90]
00599180 str      r6, [r4, #0xa4]
00599184 strb     r8, [r4, #0xa8]
00599188 ldr      r3, [sl]
0059918c add      r2, r4, #0xb8
00599190 str      r2, [sp, #4]
00599194 str      r3, [r4, #0xac]
00599198 ldr      r3, [sl, #4]
0059919c mov      ip, #0xbf000000
005991a0 add      ip, ip, #0x800000
005991a4 str      r3, [r4, #0xb0]
005991a8 ldr      r3, [sl, #8]
005991ac add      lr, r4, #0xfc
005991b0 add      sb, r4, #0xf4
005991b4 str      r3, [r4, #0xb4]
005991b8 ldr      fp, [sp, #0x38]
005991bc add      sl, r4, #0x104
005991c0 ldm      fp, {r0, r1, r2, r3}
005991c4 ldr      fp, [sp, #4]
005991c8 stm      fp, {r0, r1, r2, r3}
005991cc ldr      r3, [r7]
005991d0 mov      r0, r4
005991d4 mov      r1, r5
005991d8 str      r3, [r4, #0xc8]
005991dc ldr      r3, [r7, #4]
005991e0 str      r3, [r4, #0xcc]
005991e4 ldr      r3, [r7, #8]
005991e8 str      ip, [r4, #0xdc]
005991ec str      ip, [r4, #0xd4]
005991f0 str      r3, [r4, #0xd0]
005991f4 str      ip, [r4, #0xd8]
005991f8 str      r6, [r4, #0xe8]
005991fc str      r6, [r4, #0xe0]
00599200 str      r6, [r4, #0xe4]
00599204 str      r5, [r4, #0xec]
00599208 str      r5, [r4, #0xf0]
0059920c str      sb, [r4, #0xf4]
00599210 str      sb, [r4, #0xf8]
00599214 str      lr, [r4, #0x100]
00599218 str      sl, [r4, #0x108]
0059921c ldr      r2, [sp, #0xc]
00599220 movw     r3, #0x60f
00599224 str      r3, [r4, #0x11c]
00599228 mov      r3, #0
0059922c str      r2, [r4, #0x10c]
00599230 strb     r8, [r4, #0x121]
00599234 str      r3, [r4, #0x128]
00599238 str      lr, [r4, #0xfc]
0059923c str      sl, [r4, #0x104]
00599240 str      r5, [r4, #0x110]
00599244 str      r5, [r4, #0x114]
00599248 str      r5, [r4, #0x118]
0059924c strb     r8, [r4, #0x120]
00599250 str      r5, [r4, #0x124]
00599254 str      r5, [r4, #0x12c]
00599258 bl       #0x597c60 ; _ZN6glitch5scene10ISceneNode22updateAbsolutePositionEb
0059925c mov      r0, r4
00599260 add      sp, sp, #0x14
00599264 pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
