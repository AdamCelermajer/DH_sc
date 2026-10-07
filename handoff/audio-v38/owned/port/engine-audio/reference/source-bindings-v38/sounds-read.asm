004b5690 push {r4, r5, r6, r7, r8, sl, lr}
004b5694 sub sp, sp, #0xc
004b5698 mov sl, r0
004b569c bl #0x313a90
004b56a0 ldr r6, [pc, #0x11c]
004b56a4 mov r3, #1
004b56a8 cmp r3, #0
004b56ac str r0, [sp, #4]
004b56b0 str r3, [sp]
004b56b4 add r6, pc, r6
004b56b8 bne #0x4b5700
004b56bc add r3, sp, #4
004b56c0 add r2, r3, #2
004b56c4 add r3, r3, #1
004b56c8 ldrb r0, [r2, #1]
004b56cc ldrb r1, [r3, #-1]
004b56d0 cmp r2, r3
004b56d4 eor r1, r0, r1
004b56d8 strb r1, [r3, #-1]
004b56dc ldrb r0, [r2, #1]
004b56e0 eor r1, r1, r0
004b56e4 strb r1, [r2, #1]
004b56e8 ldrb r0, [r3, #-1]
004b56ec sub r2, r2, #1
004b56f0 eor r1, r1, r0
004b56f4 strb r1, [r3, #-1]
004b56f8 add r3, r3, #1
004b56fc bhi #0x4b56c8
004b5700 bl #0x4a38a4
004b5704 ldr r7, [pc, #0xbc]
004b5708 ldr r4, [sp, #4]
004b570c mov r5, #0xc
004b5710 ldr r3, [r6, r7]
004b5714 mul r0, r5, r4
004b5718 str r4, [r3]
004b571c add r0, r0, #8
004b5720 mov r1, #1
004b5724 bl #0x31056c
004b5728 cmp r4, #0
004b572c str r5, [r0]
004b5730 str r4, [r0, #4]
004b5734 add r3, r0, #8
004b5738 beq #0x4b5760
004b573c ldr r1, [pc, #0x88]
004b5740 mov r2, #0
004b5744 ldr r1, [r6, r1]
004b5748 add r1, r1, #8
004b574c add r2, r2, #1
004b5750 cmp r2, r4
004b5754 str r1, [r0, #8]
004b5758 add r0, r0, #0xc
004b575c bne #0x4b574c
004b5760 ldr r2, [r6, r7]
004b5764 ldr r8, [pc, #0x64]
004b5768 ldr r1, [r2]
004b576c ldr r2, [r6, r8]
004b5770 cmp r1, #0
004b5774 str r3, [r2]
004b5778 beq #0x4b57bc
004b577c mov r4, #0
004b5780 mov r5, r4
004b5784 b #0x4b5790
004b5788 ldr r3, [r6, r8]
004b578c ldr r3, [r3]
004b5790 add r0, r3, r4
004b5794 mov r1, sl
004b5798 ldr r3, [r3, r4]
004b579c mov lr, pc
004b57a0 ldr pc, [r3, #0xc]
004b57a4 ldr r3, [r6, r7]
004b57a8 add r5, r5, #1
004b57ac add r4, r4, #0xc
004b57b0 ldr r3, [r3]
004b57b4 cmp r3, r5
004b57b8 bhi #0x4b5788
004b57bc add sp, sp, #0xc
004b57c0 pop {r4, r5, r6, r7, r8, sl, pc}
004b57c4 .byte 0xdc, 0xf3, 0x4d, 0x00
004b57c8 andeq r3, r0, r8, lsr sp
004b57cc andeq r0, r0, r0, asr fp
004b57d0 andeq r3, r0, ip, lsr lr
