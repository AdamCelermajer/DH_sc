
ChestInitPost 003a1b64
003a1b64 push {r4, r5, r6, r7, r8, lr}
003a1b68 mov r8, r0
003a1b6c bl #0x39f910
003a1b70 ldr r5, [r8, #0x704]
003a1b74 ldr r2, [r8, #0x700]
003a1b78 ldr r3, [pc, #0x60]
003a1b7c cmp r5, r2
003a1b80 add r3, pc, r3
003a1b84 beq #0x3a1bd4
003a1b88 ldr r2, [pc, #0x54]
003a1b8c ldr r2, [r3, r2]
003a1b90 ldr r6, [r2]
003a1b94 cmp r6, #0
003a1b98 beq #0x3a1bd8
003a1b9c ldr r2, [pc, #0x44]
003a1ba0 mov r4, #0
003a1ba4 ldr r3, [r3, r2]
003a1ba8 ldr r7, [r3]
003a1bac b #0x3a1bbc
003a1bb0 add r4, r4, #1
003a1bb4 cmp r4, r6
003a1bb8 beq #0x3a1bd8
003a1bbc ldr r1, [r7, r4, lsl #2]
003a1bc0 mov r0, r5
003a1bc4 bl #0x30e31c
003a1bc8 cmp r0, #0
003a1bcc bne #0x3a1bb0
003a1bd0 str r4, [r8, #0x710]
003a1bd4 pop {r4, r5, r6, r7, r8, pc}
003a1bd8 mvn r4, #0
003a1bdc b #0x3a1bd0
003a1be0 subseq r2, pc, r0, lsl pc
003a1be4 andeq r0, r0, r0, ror #26
003a1be8 andeq r1, r0, r4, asr ip

ContainerInitPost 0039f910
0039f910 push {r4, r5, r6, r7, r8, lr}
0039f914 sub sp, sp, #8
0039f918 mov r4, r0
0039f91c bl #0x38bd64
0039f920 ldr r3, [r4, #0x274]
0039f924 ldr r5, [pc, #0x200]
0039f928 cmp r0, r3
0039f92c add r5, pc, r5
0039f930 blt #0x39f93c
0039f934 add sp, sp, #8
0039f938 pop {r4, r5, r6, r7, r8, pc}
0039f93c ldr r3, [r4]
0039f940 mov r0, r4
0039f944 mov lr, pc
0039f948 ldr pc, [r3, #0xd0]
0039f94c ldr r3, [r4]
0039f950 str r0, [r4, #0x374]
0039f954 mov r0, r4
0039f958 mov lr, pc
0039f95c ldr pc, [r3, #0xcc]
0039f960 cmn r0, #1
0039f964 beq #0x39f9a4
0039f968 ldr r3, [r4, #0x374]
0039f96c cmn r3, #1
0039f970 beq #0x39f9a4
0039f974 ldr r3, [pc, #0x1b4]
0039f978 mov r2, #0xc
0039f97c ldr r3, [r5, r3]
0039f980 ldr r3, [r3]
0039f984 mla r0, r2, r0, r3
0039f988 ldr r6, [r0, #8]
0039f98c mov r0, r6
0039f990 bl #0x30de54
0039f994 mov r1, r6
0039f998 add r2, r6, r0
0039f99c add r0, r4, #0x290
0039f9a0 bl #0x3109e0
0039f9a4 mov r0, r4
0039f9a8 bl #0x38be5c
0039f9ac mov r0, r4
0039f9b0 bl #0x38ab60
0039f9b4 subs r1, r0, #0
0039f9b8 beq #0x39faf8
0039f9bc ldr r8, [r4, #0x2d8]
0039f9c0 cmp r8, #0
0039f9c4 beq #0x39fa38
0039f9c8 ldr r3, [pc, #0x164]
0039f9cc ldr r6, [r8, #0x38]
0039f9d0 mov r2, r4
0039f9d4 ldr r1, [r5, r3]
0039f9d8 ldr r3, [pc, #0x158]
0039f9dc ldr ip, [r6]
0039f9e0 mov r0, r6
0039f9e4 ldr r3, [r5, r3]
0039f9e8 str r4, [sp]
0039f9ec mov lr, pc
0039f9f0 ldr pc, [ip, #0x2c]
0039f9f4 ldr r1, [pc, #0x140]
0039f9f8 mov r7, #0
0039f9fc ldr ip, [r6]
0039fa00 add r1, pc, r1
0039fa04 str r7, [sp]
0039fa08 mov r0, r6
0039fa0c mov r2, r7
0039fa10 mov r3, r7
0039fa14 mov lr, pc
0039fa18 ldr pc, [ip, #0x20]
0039fa1c cmp r0, #0
0039fa20 beq #0x39fa94
0039fa24 mov r1, r7
0039fa28 mov r0, r4
0039fa2c bl #0x39f3cc
0039fa30 mov r0, r8
0039fa34 bl #0x470a54
0039fa38 ldr r3, [pc, #0x100]
0039fa3c ldr r3, [r5, r3]
0039fa40 ldr r5, [r3]
0039fa44 cmp r5, #0
0039fa48 beq #0x39fa68
0039fa4c ldr r3, [r4]
0039fa50 mov r0, r4
0039fa54 mov lr, pc
0039fa58 ldr pc, [r3, #0xd8]
0039fa5c mov r1, r0
0039fa60 mov r0, r5
0039fa64 bl #0x3699fc
0039fa68 ldr r3, [r4]
0039fa6c mov r0, r4
0039fa70 mov lr, pc
0039fa74 ldr pc, [r3, #0xc8]
0039fa78 ldr r2, [pc, #0xc4]
0039fa7c mov r1, r0
0039fa80 mov r0, r4
0039fa84 add r2, pc, r2
0039fa88 add sp, sp, #8
0039fa8c pop {r4, r5, r6, r7, r8, lr}
0039fa90 b #0x38ef60
0039fa94 ldr r1, [pc, #0xac]
0039fa98 mov r2, r0
0039fa9c ldr ip, [r6]
0039faa0 mov r3, r2
0039faa4 str r0, [sp]
0039faa8 add r1, pc, r1
0039faac mov r0, r6
0039fab0 mov lr, pc
0039fab4 ldr pc, [ip, #0x20]
0039fab8 subs r3, r0, #0
0039fabc bne #0x39fb1c
0039fac0 ldr r1, [pc, #0x84]
0039fac4 ldr ip, [r6]
0039fac8 mov r2, r3
0039facc mov r0, r6
0039fad0 add r1, pc, r1
0039fad4 str r3, [sp]
0039fad8 mov lr, pc
0039fadc ldr pc, [ip, #0x20]
0039fae0 cmp r0, #0
0039fae4 beq #0x39fa30
0039fae8 mov r0, r4
0039faec mov r1, #2
0039faf0 bl #0x39f3cc
0039faf4 b #0x39fa30
0039faf8 mov r0, r4
0039fafc ldr r3, [r4]
0039fb00 mov lr, pc
0039fb04 ldr pc, [r3, #0x40]
0039fb08 mov r0, r4
0039fb0c mov r1, #4
0039fb10 add sp, sp, #8
0039fb14 pop {r4, r5, r6, r7, r8, lr}
0039fb18 b #0x39f3cc
0039fb1c mov r0, r4
0039fb20 mov r1, #1
0039fb24 bl #0x39f3cc
0039fb28 b #0x39fa30
0039fb2c subseq r5, pc, r4, ror #2
0039fb30 andeq r1, r0, r8, lsr #25
0039fb34 andeq r4, r0, r0, lsl #24
0039fb38 andeq r1, r0, ip, ror #30
0039fb3c subseq r3, r2, r0, asr r5
0039fb40 andeq r0, r0, r4, lsr #27
0039fb44 ldrsheq r3, [r2], #-0xc
0039fb48 subseq r3, r2, r0, lsl #8
0039fb4c subseq r2, r2, r0, ror #15
0039fb50 push {r4, r5, r6, lr}
0039fb54 ldr r4, [pc, #0xac]

GameObjectDictRead 004b5544
004b5544 push {r4, r5, r6, r7, r8, sl, lr}
004b5548 sub sp, sp, #0xc
004b554c mov sl, r0
004b5550 bl #0x313a90
004b5554 ldr r6, [pc, #0x124]
004b5558 mov r3, #1
004b555c cmp r3, #0
004b5560 str r0, [sp, #4]
004b5564 str r3, [sp]
004b5568 add r6, pc, r6
004b556c bne #0x4b55b4
004b5570 add r3, sp, #4
004b5574 add r2, r3, #2
004b5578 add r3, r3, #1
004b557c ldrb r0, [r2, #1]
004b5580 ldrb r1, [r3, #-1]
004b5584 cmp r2, r3
004b5588 eor r1, r0, r1
004b558c strb r1, [r3, #-1]
004b5590 ldrb r0, [r2, #1]
004b5594 eor r1, r1, r0
004b5598 strb r1, [r2, #1]
004b559c ldrb r0, [r3, #-1]
004b55a0 sub r2, r2, #1
004b55a4 eor r1, r1, r0
004b55a8 strb r1, [r3, #-1]
004b55ac add r3, r3, #1
004b55b0 bhi #0x4b557c
004b55b4 bl #0x4a3ba4
004b55b8 ldr r7, [pc, #0xc4]
004b55bc ldr r4, [sp, #4]
004b55c0 mov r5, #0xc
004b55c4 ldr r3, [r6, r7]
004b55c8 mul r0, r5, r4
004b55cc str r4, [r3]
004b55d0 add r0, r0, #8
004b55d4 mov r1, #1
004b55d8 bl #0x31056c
004b55dc cmp r4, #0
004b55e0 str r5, [r0]
004b55e4 str r4, [r0, #4]
004b55e8 add r3, r0, #8
004b55ec beq #0x4b561c
004b55f0 ldr r1, [pc, #0x90]
004b55f4 mov r2, #0
004b55f8 mov ip, r2
004b55fc ldr r1, [r6, r1]
004b5600 add r1, r1, #8
004b5604 add r2, r2, #1
004b5608 cmp r2, r4
004b560c str r1, [r0, #8]
004b5610 str ip, [r0, #0x10]
004b5614 add r0, r0, #0xc
004b5618 bne #0x4b5604
004b561c ldr r2, [r6, r7]
004b5620 ldr r8, [pc, #0x64]
004b5624 ldr r1, [r2]
004b5628 ldr r2, [r6, r8]
004b562c cmp r1, #0
004b5630 str r3, [r2]
004b5634 beq #0x4b5678
004b5638 mov r4, #0
004b563c mov r5, r4
004b5640 b #0x4b564c
004b5644 ldr r3, [r6, r8]
004b5648 ldr r3, [r3]
004b564c add r0, r3, r4
004b5650 mov r1, sl
004b5654 ldr r3, [r3, r4]
004b5658 mov lr, pc
004b565c ldr pc, [r3, #0xc]
004b5660 ldr r3, [r6, r7]
004b5664 add r5, r5, #1
004b5668 add r4, r4, #0xc
004b566c ldr r3, [r3]
004b5670 cmp r3, r5
004b5674 bhi #0x4b5644
004b5678 add sp, sp, #0xc
004b567c pop {r4, r5, r6, r7, r8, sl, pc}
004b5680 subeq pc, sp, r8, lsr #10
004b5684 andeq r1, r0, r8, asr #2
004b5688 strheq r0, [r0], -ip
004b568c andeq r1, r0, r8, lsr #25
