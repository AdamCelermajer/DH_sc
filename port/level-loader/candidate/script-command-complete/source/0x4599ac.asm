004599ac f0412de9 push {r4, r5, r6, r7, r8, lr}
004599b0 08d04de2 sub sp, sp, #8
004599b4 344cffeb bl #0x42ca8c
004599b8 734cffeb bl #0x42cb8c
004599bc 0040a0e1 mov r4, r0
004599c0 314cffeb bl #0x42ca8c
004599c4 704cffeb bl #0x42cb8c
004599c8 b7380deb bl #0x7a7cac
004599cc e0690ceb bl #0x774154
004599d0 14219fe5 ldr r2, [pc, #0x114]
004599d4 0050a0e3 mov r5, #0
004599d8 0010a0e1 mov r1, r0
004599dc 02208fe0 add r2, pc, r2
004599e0 0400a0e1 mov r0, r4
004599e4 0530a0e1 mov r3, r5
004599e8 00419fe5 ldr r4, [pc, #0x100]
004599ec 00508de5 str r5, [sp]
004599f0 05490deb bl #0x7abe0c
004599f4 f8309fe5 ldr r3, [pc, #0xf8]
004599f8 04408fe0 add r4, pc, r4
004599fc 0170a0e3 mov r7, #1
00459a00 032094e7 ldr r2, [r4, r3]
00459a04 ec309fe5 ldr r3, [pc, #0xec]
00459a08 3050c2e5 strb r5, [r2, #0x30]
00459a0c 033094e7 ldr r3, [r4, r3]
00459a10 0070c3e5 strb r7, [r3]
00459a14 5e8f0eeb bl #0x7fd794
00459a18 0530d0e5 ldrb r3, [r0, #5]
00459a1c 050053e1 cmp r3, r5
00459a20 0100001a bne #0x459a2c
00459a24 08d08de2 add sp, sp, #8
00459a28 f081bde8 pop {r4, r5, r6, r7, r8, pc}
00459a2c e2c50eeb bl #0x80b1bc
00459a30 0080a0e1 mov r8, r0
00459a34 c0009fe5 ldr r0, [pc, #0xc0]
00459a38 0710a0e1 mov r1, r7
00459a3c bc609fe5 ldr r6, [pc, #0xbc]
00459a40 00008fe0 add r0, pc, r0
00459a44 fec10eeb bl #0x80a244
00459a48 0130e0e3 mvn r3, #1
00459a4c 543080e5 str r3, [r0, #0x54]
00459a50 0030e0e3 mvn r3, #0
00459a54 583080e5 str r3, [r0, #0x58]
00459a58 0010a0e1 mov r1, r0
00459a5c 507080e5 str r7, [r0, #0x50]
00459a60 0800a0e1 mov r0, r8
00459a64 0ed20eeb bl #0x80e2a4
00459a68 063094e7 ldr r3, [r4, r6]
00459a6c 400093e5 ldr r0, [r3, #0x40]
00459a70 c43690e5 ldr r3, [r0, #0x6c4]
00459a74 050053e1 cmp r3, r5
00459a78 0e0000ca bgt #0x459ab8
00459a7c e8ffffea b #0x459a24
00459a80 603697e5 ldr r3, [r7, #0x660]
00459a84 0110a0e3 mov r1, #1
00459a88 000053e3 cmp r3, #0
00459a8c 0300000a beq #0x459aa0
00459a90 0300a0e1 mov r0, r3
00459a94 003093e5 ldr r3, [r3]
00459a98 0fe0a0e1 mov lr, pc
00459a9c 40f093e5 ldr pc, [r3, #0x40]
00459aa0 063094e7 ldr r3, [r4, r6]
00459aa4 015085e2 add r5, r5, #1
00459aa8 400093e5 ldr r0, [r3, #0x40]
00459aac c43690e5 ldr r3, [r0, #0x6c4]
00459ab0 030055e1 cmp r5, r3
00459ab4 daffffaa bge #0x459a24
00459ab8 0510a0e1 mov r1, r5
00459abc 0020a0e3 mov r2, #0
00459ac0 1f53fceb bl #0x36e744
00459ac4 003090e5 ldr r3, [r0]
00459ac8 0070a0e1 mov r7, r0
00459acc 0fe0a0e1 mov lr, pc
00459ad0 50f093e5 ldr pc, [r3, #0x50]
00459ad4 000050e3 cmp r0, #0
00459ad8 e8ffff0a beq #0x459a80
00459adc 0700a0e1 mov r0, r7
00459ae0 0010a0e3 mov r1, #0
00459ae4 85ffffeb bl #0x459900
00459ae8 ecffffea b #0x459aa0
00459aec 34354700 subeq r3, r7, r4, lsr r5
