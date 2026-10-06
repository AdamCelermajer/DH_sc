_ZN14PhysicalObjectC2EP13PhysicalWorldP10GameObjectbbbbstti
0046f2f0 push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0046f2f4 ldr      r5, [pc, #0x360]
0046f2f8 ldr      r4, [pc, #0x360]
0046f2fc sub      sp, sp, #0xb4
0046f300 add      r5, pc, r5
0046f304 str      r4, [sp, #0x10]
0046f308 ldr      lr, [r5, r4]
0046f30c ldr      ip, [pc, #0x350]
0046f310 ldr      r4, [pc, #0x350]
0046f314 ldrb     sb, [sp, #0xd8]
0046f318 ldr      ip, [r5, ip]
0046f31c ldr      r4, [r5, r4]
0046f320 ldr      lr, [lr]
0046f324 mov      r7, #0
0046f328 str      r4, [sp, #0xc]
0046f32c add      ip, ip, #8
0046f330 mov      r4, r0
0046f334 mov      sl, #0
0046f338 str      r1, [r0, #4]
0046f33c str      ip, [r0]
0046f340 str      r2, [r4, #8]
0046f344 str      sl, [r0, #0xc]
0046f348 strb     sb, [r0, #0x10]
0046f34c str      r7, [r0, #0x14]
0046f350 str      r7, [r0, #0x18]
0046f354 str      r7, [r0, #0x1c]
0046f358 strb     r7, [r0, #0x26]
0046f35c strb     r7, [r0, #0x27]
0046f360 ldrb     ip, [sp, #0xdc]
0046f364 mov      r6, r2
0046f368 str      lr, [sp, #0xac]
0046f36c ldrh     r2, [sp, #0xe8]
0046f370 ldrb     lr, [sp, #0xe0]
0046f374 str      r3, [sp, #0x1c]
0046f378 ldrh     r3, [sp, #0xec]
0046f37c ldr      r0, [sp, #0xc]
0046f380 str      ip, [sp, #0x20]
0046f384 str      lr, [sp, #0x24]
0046f388 str      r3, [sp, #0x18]
0046f38c ldrsh    r8, [sp, #0xe4]
0046f390 str      r2, [sp, #0x14]
0046f394 bl       #0x337888 ; _ZN13DebugSwitches4loadEv
0046f398 ldr      r1, [pc, #0x2cc]
0046f39c add      fp, sp, #0x94
0046f3a0 add      r2, sp, #0x90
0046f3a4 add      r1, pc, r1
0046f3a8 mov      r0, fp
0046f3ac bl       #0x3140ec ; _ZNSsC1EPKcRKSaIcE
0046f3b0 mov      r1, fp
0046f3b4 ldr      r0, [sp, #0xc]
0046f3b8 bl       #0x337a88 ; _ZN13DebugSwitches9GetSwitchERKSs
0046f3bc mov      r3, r0
0046f3c0 mov      r0, fp
0046f3c4 str      r3, [sp, #8]
0046f3c8 bl       #0x3139ac ; _ZNSt4priv12_String_baseIcSaIcEE19_M_deallocate_blockEv
0046f3cc ldr      r3, [sp, #8]
0046f3d0 mvn      r2, #0x298
0046f3d4 sub      r2, r2, #1
0046f3d8 cmp      r3, r7
0046f3dc movne    r8, r2
0046f3e0 cmp      r6, r7
0046f3e4 beq      #0x46f558
0046f3e8 cmp      sb, r7
0046f3ec bne      #0x46f57c
0046f3f0 ldr      r3, [pc, #0x278]
0046f3f4 mov      r2, #1
0046f3f8 ldr      r1, [r6, #0x12c]
0046f3fc ldr      r3, [r5, r3]
0046f400 ldr      r0, [r6, #0x138]
0046f404 str      r2, [sp, #0x30]
0046f408 add      ip, r3, #8
0046f40c movw     r3, #0xcccd
0046f410 movt     r3, #0x3e4c
0046f414 strh     r2, [sp, #0x46]
0046f418 mvn      r2, #0
0046f41c str      ip, [sp, #0x2c]
0046f420 strh     r2, [sp, #0x48]
0046f424 str      r3, [sp, #0x38]
0046f428 str      sl, [sp, #0x40]
0046f42c str      sb, [sp, #0x8c]
0046f430 str      sb, [sp, #0x34]
0046f434 str      sl, [sp, #0x3c]
0046f438 strh     sb, [sp, #0x4a]
0046f43c strb     sb, [sp, #0x44]
0046f440 bl       #0x30e3ac
0046f444 movw     r1, #0xd70a
0046f448 movt     r1, #0x3c23
0046f44c bl       #0x30ed6c
0046f450 ldr      r1, [r6, #0x130]
0046f454 mov      sb, r0
0046f458 ldr      r0, [r6, #0x13c]
0046f45c bl       #0x30e3ac
0046f460 movw     r1, #0xd70a
0046f464 movt     r1, #0x3c23
0046f468 bl       #0x30ed6c
0046f46c movw     r1, #0xd70a
0046f470 mov      r7, r0
0046f474 movt     r1, #0x3c23
0046f478 ldr      r0, [r6, #0x160]
0046f47c bl       #0x30ed6c
0046f480 movw     r1, #0xd70a
0046f484 movt     r1, #0x3c23
0046f488 mov      sl, r0
0046f48c ldr      r0, [r6, #0x164]
0046f490 bl       #0x30ed6c
0046f494 mov      r1, #0x3f000000
0046f498 mov      r6, r0
0046f49c mov      r0, sb
0046f4a0 bl       #0x30ed6c
0046f4a4 mov      r1, #0x3f000000
0046f4a8 mov      r3, r0
0046f4ac mov      r0, r7
0046f4b0 str      r3, [sp, #8]
0046f4b4 bl       #0x30ed6c
0046f4b8 ldr      r3, [sp, #8]
0046f4bc add      fp, sp, #0x2c
0046f4c0 mov      r2, r0
0046f4c4 mov      r1, r3
0046f4c8 mov      r0, fp
0046f4cc bl       #0x7e44b8 ; _ZN12b2PolygonDef8SetAsBoxEff
0046f4d0 mov      r1, r7
0046f4d4 mov      r0, sb
0046f4d8 bl       #0x30e70c
0046f4dc cmp      r0, #0
0046f4e0 moveq    r7, sb
0046f4e4 mov      r0, r7
0046f4e8 mov      r1, #0x3f000000
0046f4ec bl       #0x30ed6c
0046f4f0 ldr      r3, [pc, #0x17c]
0046f4f4 str      r0, [r4, #0xc]
0046f4f8 mov      ip, fp
0046f4fc ldr      r3, [r5, r3]
0046f500 add      r3, r3, #8
0046f504 str      r3, [sp, #0x2c]
0046f508 strh     r8, [r4, #0x24]
0046f50c ldr      lr, [sp, #0x14]
0046f510 mov      r1, ip
0046f514 mov      r3, r6
0046f518 strh     lr, [r4, #0x20]
0046f51c ldr      r2, [sp, #0x18]
0046f520 mov      r0, r4
0046f524 strh     r2, [r4, #0x22]
0046f528 ldr      lr, [sp, #0x20]
0046f52c strh     r8, [ip, #0x1e]
0046f530 mov      r2, sl
0046f534 strb     lr, [ip, #0x18]
0046f538 ldr      lr, [sp, #0x14]
0046f53c strh     lr, [ip, #0x1a]
0046f540 ldr      lr, [sp, #0x18]
0046f544 strh     lr, [ip, #0x1c]
0046f548 ldr      ip, [sp, #0x1c]
0046f54c ldr      lr, [sp, #0x24]
0046f550 stm      sp, {ip, lr}
0046f554 bl       #0x46edf0 ; _ZN14PhysicalObject5_initEP10b2ShapeDefffbb
0046f558 ldr      ip, [sp, #0x10]
0046f55c ldr      r2, [sp, #0xac]
0046f560 mov      r0, r4
0046f564 ldr      r3, [r5, ip]
0046f568 ldr      r3, [r3]
0046f56c cmp      r2, r3
0046f570 bne      #0x46f658
0046f574 add      sp, sp, #0xb4
0046f578 pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
0046f57c movw     r3, #0xcccd
0046f580 ldr      r1, [r6, #0x12c]
0046f584 ldr      r0, [r6, #0x138]
0046f588 movt     r3, #0x3e4c
0046f58c mov      ip, #1
0046f590 mvn      lr, #0
0046f594 str      r3, [sp, #0x38]
0046f598 strh     ip, [sp, #0x46]
0046f59c strh     lr, [sp, #0x48]
0046f5a0 strb     r7, [sp, #0x44]
0046f5a4 str      r7, [sp, #0x30]
0046f5a8 str      sl, [sp, #0x50]
0046f5ac str      r7, [sp, #0x34]
0046f5b0 str      sl, [sp, #0x3c]
0046f5b4 str      sl, [sp, #0x40]
0046f5b8 strh     r7, [sp, #0x4a]
0046f5bc str      sl, [sp, #0x4c]
0046f5c0 bl       #0x30e3ac
0046f5c4 movw     r1, #0xd70a
0046f5c8 movt     r1, #0x3c23
0046f5cc bl       #0x30ed6c
0046f5d0 ldr      r1, [r6, #0x130]
0046f5d4 mov      sb, r0
0046f5d8 ldr      r0, [r6, #0x13c]
0046f5dc bl       #0x30e3ac
0046f5e0 movw     r1, #0xd70a
0046f5e4 movt     r1, #0x3c23
0046f5e8 bl       #0x30ed6c
0046f5ec movw     r1, #0xd70a
0046f5f0 mov      r7, r0
0046f5f4 movt     r1, #0x3c23
0046f5f8 ldr      r0, [r6, #0x160]
0046f5fc bl       #0x30ed6c
0046f600 movw     r1, #0xd70a
0046f604 movt     r1, #0x3c23
0046f608 mov      sl, r0
0046f60c ldr      r0, [r6, #0x164]
0046f610 bl       #0x30ed6c
0046f614 mov      r1, r7
0046f618 mov      r6, r0
0046f61c mov      r0, sb
0046f620 bl       #0x30e70c
0046f624 cmp      r0, #0
0046f628 moveq    r7, sb
0046f62c mov      r0, r7
0046f630 mov      r1, #0x3f000000
0046f634 bl       #0x30ed6c
0046f638 ldr      r3, [pc, #0x34]
0046f63c add      ip, sp, #0xb0
0046f640 str      r0, [r4, #0xc]
0046f644 ldr      r3, [r5, r3]
0046f648 str      r0, [sp, #0x54]
0046f64c add      r3, r3, #8
0046f650 str      r3, [ip, #-0x84]!
0046f654 b        #0x46f508
0046f658 bl       #0x30e310
