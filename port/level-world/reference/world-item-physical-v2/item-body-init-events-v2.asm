_ZN14PhysicalObject5_initEP10b2ShapeDefffbb
0046edf0 push     {r4, r5, r6, r7, lr}
0046edf4 sub      sp, sp, #0x34
0046edf8 ldrb     lr, [sp, #0x4c]
0046edfc mov      r4, #0
0046ee00 mov      r6, r1
0046ee04 mov      r5, r0
0046ee08 mov      ip, #1
0046ee0c ldr      r0, [r0, #4]
0046ee10 add      r1, sp, #4
0046ee14 str      r4, [sp, #8]
0046ee18 str      r4, [sp, #0xc]
0046ee1c str      r4, [sp, #4]
0046ee20 str      r4, [sp, #0x10]
0046ee24 str      r4, [sp, #0x20]
0046ee28 str      r4, [sp, #0x24]
0046ee2c str      r4, [sp, #0x28]
0046ee30 ldrb     r7, [sp, #0x48]
0046ee34 str      r3, [sp, #0x1c]
0046ee38 strb     lr, [sp, #0x2f]
0046ee3c strb     ip, [sp, #0x2e]
0046ee40 str      r2, [sp, #0x18]
0046ee44 strb     ip, [sp, #0x2c]
0046ee48 str      r5, [sp, #0x14]
0046ee4c strb     ip, [sp, #0x2d]
0046ee50 bl       #0x34bcf0 ; _ZN13PhysicalWorld10createBodyEP9b2BodyDef
0046ee54 mov      r3, #0x3f800000
0046ee58 str      r0, [r5, #0x14]
0046ee5c str      r5, [r0, #0x90]
0046ee60 str      r3, [r6, #0xc]
0046ee64 ldrb     r3, [r6, #0x18]
0046ee68 cmp      r7, #0
0046ee6c str      r4, [r6, #0x10]
0046ee70 movweq   r4, #0xd70a
0046ee74 movteq   r4, #0x4133
0046ee78 cmp      r3, #0
0046ee7c str      r5, [r6, #8]
0046ee80 str      r4, [r6, #0x14]
0046ee84 bne      #0x46eea4
0046ee88 mov      r1, r6
0046ee8c mov      r0, r5
0046ee90 mov      r2, #1
0046ee94 bl       #0x46edb8 ; _ZN14PhysicalObject9_addShapeEP10b2ShapeDefb
0046ee98 str      r0, [r5, #0x18]
0046ee9c add      sp, sp, #0x34
0046eea0 pop      {r4, r5, r6, r7, pc}
0046eea4 mov      r1, r6
0046eea8 mov      r0, r5
0046eeac mov      r2, #1
0046eeb0 bl       #0x46edb8 ; _ZN14PhysicalObject9_addShapeEP10b2ShapeDefb
0046eeb4 str      r0, [r5, #0x1c]
0046eeb8 b        #0x46ee9c
_ZN6POItem17onCollisionBeginsEP18PhysicalBaseObjectRK7Point2DIfEb
004702a8 push     {r4, r5, r6, lr}
004702ac ldr      r3, [r0, #8]
004702b0 sub      sp, sp, #0x18
004702b4 add      r4, sp, #0xc
004702b8 mov      r0, r4
004702bc mov      r5, r1
004702c0 mov      r1, r3
004702c4 bl       #0x33dd2c ; _ZN10ObjectBase9GetHandleEv
004702c8 mov      r0, r4
004702cc mov      r1, #0
004702d0 bl       #0x33fdc0 ; _ZN12ObjectHandle9GetObjectEb
004702d4 subs     r4, r0, #0
004702d8 beq      #0x4702e8
004702dc ldr      r3, [r4, #0xf4]
004702e0 cmp      r3, #3
004702e4 beq      #0x4702ec
004702e8 mov      r4, #0
004702ec cmp      r5, #0
004702f0 beq      #0x470304
004702f4 ldr      r5, [r5, #8]
004702f8 cmp      r4, #0
004702fc cmpne    r5, #0
00470300 bne      #0x47030c
00470304 add      sp, sp, #0x18
00470308 pop      {r4, r5, r6, pc}
0047030c ldr      r3, [r4]
00470310 mov      r0, r4
00470314 mov      r1, #0
00470318 mov      lr, pc
0047031c ldr      pc, [r3, #0x88]
00470320 cmp      r0, #0
00470324 beq      #0x470304
00470328 mov      r1, r5
0047032c mov      r0, sp
00470330 bl       #0x33dd2c ; _ZN10ObjectBase9GetHandleEv
00470334 mov      r0, sp
00470338 bl       #0x33ff54 ; _ZN12ObjectHandlecvP9CharacterEv
0047033c subs     r1, r0, #0
00470340 mov      r6, sp
00470344 beq      #0x470304
00470348 mov      r0, r4
0047034c bl       #0x3ec048 ; _ZN10ItemObject17OnCollisionBeginsEP9Character
00470350 b        #0x470304
_ZN6POItem15onCollisionEndsEP18PhysicalBaseObjectRK7Point2DIfEb
004701fc push     {r4, r5, r6, lr}
00470200 ldr      r3, [r0, #8]
00470204 sub      sp, sp, #0x18
00470208 add      r4, sp, #0xc
0047020c mov      r0, r4
00470210 mov      r5, r1
00470214 mov      r1, r3
00470218 bl       #0x33dd2c ; _ZN10ObjectBase9GetHandleEv
0047021c mov      r0, r4
00470220 mov      r1, #0
00470224 bl       #0x33fdc0 ; _ZN12ObjectHandle9GetObjectEb
00470228 subs     r4, r0, #0
0047022c beq      #0x47023c
00470230 ldr      r3, [r4, #0xf4]
00470234 cmp      r3, #3
00470238 beq      #0x470240
0047023c mov      r4, #0
00470240 cmp      r5, #0
00470244 beq      #0x470258
00470248 ldr      r5, [r5, #8]
0047024c cmp      r4, #0
00470250 cmpne    r5, #0
00470254 bne      #0x470260
00470258 add      sp, sp, #0x18
0047025c pop      {r4, r5, r6, pc}
00470260 ldr      r3, [r4]
00470264 mov      r0, r4
00470268 mov      r1, #0
0047026c mov      lr, pc
00470270 ldr      pc, [r3, #0x88]
00470274 cmp      r0, #0
00470278 beq      #0x470258
0047027c mov      r1, r5
00470280 mov      r0, sp
00470284 bl       #0x33dd2c ; _ZN10ObjectBase9GetHandleEv
00470288 mov      r0, sp
0047028c bl       #0x33ff54 ; _ZN12ObjectHandlecvP9CharacterEv
00470290 subs     r1, r0, #0
00470294 mov      r6, sp
00470298 beq      #0x470258
0047029c mov      r0, r4
004702a0 bl       #0x3ebd2c ; _ZN10ItemObject15OnCollisionEndsEP9Character
004702a4 b        #0x470258
