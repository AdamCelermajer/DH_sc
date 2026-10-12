0045cba0 f04f2de9 push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0045cba4 04429fe5 ldr r4, [pc, #0x204]
0045cba8 04529fe5 ldr r5, [pc, #0x204]
0045cbac 04229fe5 ldr r2, [pc, #0x204]
0045cbb0 04408fe0 add r4, pc, r4
0045cbb4 053094e7 ldr r3, [r4, r5]
0045cbb8 028094e7 ldr r8, [r4, r2]
0045cbbc 44d04de2 sub sp, sp, #0x44
0045cbc0 003093e5 ldr r3, [r3]
0045cbc4 24608de2 add r6, sp, #0x24
0045cbc8 ec719fe5 ldr r7, [pc, #0x1ec]
0045cbcc 3c308de5 str r3, [sp, #0x3c]
0045cbd0 0ca090e5 ldr sl, [r0, #0xc]
0045cbd4 0800a0e1 mov r0, r8
0045cbd8 2a6bfbeb bl #0x337888
0045cbdc dc119fe5 ldr r1, [pc, #0x1dc]
0045cbe0 20208de2 add r2, sp, #0x20
0045cbe4 0600a0e1 mov r0, r6
0045cbe8 01108fe0 add r1, pc, r1
0045cbec 3eddfaeb bl #0x3140ec
0045cbf0 0610a0e1 mov r1, r6
0045cbf4 0800a0e1 mov r0, r8
0045cbf8 a26bfbeb bl #0x337a88
0045cbfc 0600a0e1 mov r0, r6
0045cc00 93edfaeb bl #0x318254
0045cc04 078094e7 ldr r8, [r4, r7]
0045cc08 0800a0e1 mov r0, r8
0045cc0c 600afbeb bl #0x31f594
0045cc10 0010a0e3 mov r1, #0
0045cc14 0060a0e1 mov r6, r0
0045cc18 0120a0e3 mov r2, #1
0045cc1c 400098e5 ldr r0, [r8, #0x40]
0045cc20 1446fceb bl #0x36e478
0045cc24 600690e5 ldr r0, [r0, #0x660]
0045cc28 2d7bfdeb bl #0x3bb8e4
0045cc2c 000056e3 cmp r6, #0
0045cc30 1c008de5 str r0, [sp, #0x1c]
0045cc34 0400000a beq #0x45cc4c
0045cc38 0600a0e1 mov r0, r6
0045cc3c 0010a0e3 mov r1, #0
0045cc40 fe4bfeeb bl #0x3efc40
0045cc44 186196e5 ldr r6, [r6, #0x118]
0045cc48 1c608de5 str r6, [sp, #0x1c]
0045cc4c 70319fe5 ldr r3, [pc, #0x170]
0045cc50 10909ae5 ldr sb, [sl, #0x10]
0045cc54 033094e7 ldr r3, [r4, r3]
0045cc58 006093e5 ldr r6, [r3]
0045cc5c 000056e3 cmp r6, #0
0045cc60 0f00000a beq #0x45cca4
0045cc64 5c319fe5 ldr r3, [pc, #0x15c]
0045cc68 0080a0e3 mov r8, #0
0045cc6c 033094e7 ldr r3, [r4, r3]
0045cc70 00b093e5 ldr fp, [r3]
0045cc74 020000ea b #0x45cc84
0045cc78 018088e2 add r8, r8, #1
0045cc7c 060058e1 cmp r8, r6
0045cc80 0700000a beq #0x45cca4
0045cc84 0900a0e1 mov r0, sb
0045cc88 08119be7 ldr r1, [fp, r8, lsl #2]
0045cc8c a2c5faeb bl #0x30e31c
0045cc90 000050e3 cmp r0, #0
0045cc94 f7ffff1a bne #0x45cc78
0045cc98 48b0a0e3 mov fp, #0x48
0045cc9c 9b080be0 mul fp, fp, r8
0045cca0 010000ea b #0x45ccac
0045cca4 47b0e0e3 mvn fp, #0x47
0045cca8 0080e0e3 mvn r8, #0
0045ccac 18319fe5 ldr r3, [pc, #0x118]
0045ccb0 076094e7 ldr r6, [r4, r7]
0045ccb4 0010a0e3 mov r1, #0
0045ccb8 033094e7 ldr r3, [r4, r3]
0045ccbc 0120a0e3 mov r2, #1
0045ccc0 400096e5 ldr r0, [r6, #0x40]
0045ccc4 009093e5 ldr sb, [r3]
0045ccc8 ea45fceb bl #0x36e478
0045cccc 0810a0e1 mov r1, r8
0045ccd0 600690e5 ldr r0, [r0, #0x660]
0045ccd4 0020e0e3 mvn r2, #0
0045ccd8 dd7afdeb bl #0x3bb854
0045ccdc 0010a0e3 mov r1, #0
0045cce0 0120a0e3 mov r2, #1
0045cce4 400096e5 ldr r0, [r6, #0x40]
0045cce8 e245fceb bl #0x36e478
0045ccec 600690e5 ldr r0, [r0, #0x660]
0045ccf0 ec7dfdeb bl #0x3bc4a8
0045ccf4 a6820eeb bl #0x7fd794
0045ccf8 0530d0e5 ldrb r3, [r0, #5]
0045ccfc 000053e3 cmp r3, #0
0045cd00 1e00001a bne #0x45cd80
0045cd04 076094e7 ldr r6, [r4, r7]
0045cd08 0010a0e3 mov r1, #0
0045cd0c 0120a0e3 mov r2, #1
0045cd10 400096e5 ldr r0, [r6, #0x40]
0045cd14 0b9089e0 add sb, sb, fp
0045cd18 207099e5 ldr r7, [sb, #0x20]
0045cd1c 08809ae5 ldr r8, [sl, #8]
0045cd20 d445fceb bl #0x36e478
0045cd24 600690e5 ldr r0, [r0, #0x660]
0045cd28 7e7afdeb bl #0x3bb728
0045cd2c 0030a0e1 mov r3, r0
0045cd30 0600a0e1 mov r0, r6
0045cd34 1c609de5 ldr r6, [sp, #0x1c]
0045cd38 00c0a0e3 mov ip, #0
0045cd3c 01e0a0e3 mov lr, #1
0045cd40 0710a0e1 mov r1, r7
0045cd44 0820a0e1 mov r2, r8
0045cd48 04e08de5 str lr, [sp, #4]
0045cd4c 08608de5 str r6, [sp, #8]
0045cd50 14c08de5 str ip, [sp, #0x14]
0045cd54 00e08de5 str lr, [sp]
0045cd58 0cc08de5 str ip, [sp, #0xc]
0045cd5c 10c08de5 str ip, [sp, #0x10]
0045cd60 183cfbeb bl #0x32bdc8
0045cd64 053094e7 ldr r3, [r4, r5]
0045cd68 3c209de5 ldr r2, [sp, #0x3c]
0045cd6c 003093e5 ldr r3, [r3]
0045cd70 030052e1 cmp r2, r3
0045cd74 0c00001a bne #0x45cdac
0045cd78 44d08de2 add sp, sp, #0x44
0045cd7c f08fbde8 pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
0045cd80 0010a0e3 mov r1, #0
0045cd84 0120a0e3 mov r2, #1
0045cd88 400096e5 ldr r0, [r6, #0x40]
0045cd8c b945fceb bl #0x36e478
0045cd90 608690e5 ldr r8, [r0, #0x660]
0045cd94 400096e5 ldr r0, [r6, #0x40]
0045cd98 bf44fceb bl #0x36e09c
0045cd9c 603690e5 ldr r3, [r0, #0x660]
0045cda0 080053e1 cmp r3, r8
0045cda4 eeffff1a bne #0x45cd64
0045cda8 d5ffffea b #0x45cd04
0045cdac 57c5faeb bl #0x30e310
0045cdb0 e07e5300 subseq r7, r3, r0, ror #29
0045cdb4 ac400000 andeq r4, r0, ip, lsr #1
0045cdb8 84080000 andeq r0, r0, r4, lsl #17
0045cdbc f4370000 strdeq r3, r4, [r0], -r4
0045cdc0 98044700 umaaleq r0, r7, r8, r4
0045cdc4 c0180000 andeq r1, r0, r0, asr #17
0045cdc8 5c3b0000 andeq r3, r0, ip, asr fp
0045cdcc 74080000 andeq r0, r0, r4, ror r8
