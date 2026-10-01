; ELF evidence; literal pools retained; not assembler-ready source.

; FUNCTION 0x0060bee4, declared_size=376, range_size=376, mode=arm
; class-group: glitch::collada::SAnimationSegment
; alias: _ZN6glitch7collada17SAnimationSegment7getDataERNS_3res14onDemandReaderE
; demangled: glitch::collada::SAnimationSegment::getData(glitch::res::onDemandReader&)
; decoder-mode: arm
0060bee4  70 40 2d e9                                      push {r4, r5, r6, lr}
0060bee8  08 50 91 e5                                      ldr r5, [r1, #8]
0060beec  10 d0 4d e2                                      sub sp, sp, #0x10
0060bef0  00 40 a0 e1                                      mov r4, r0
0060bef4  00 00 55 e3                                      cmp r5, #0
0060bef8  09 00 00 0a                                      beq #0x60bf24
0060befc  01 00 55 e3                                      cmp r5, #1
0060bf00  2f 00 00 0a                                      beq #0x60bfc4
0060bf04  08 30 81 e2                                      add r3, r1, #8
0060bf08  00 30 80 e5                                      str r3, [r0]
0060bf0c  08 30 91 e5                                      ldr r3, [r1, #8]
0060bf10  01 30 83 e2                                      add r3, r3, #1
0060bf14  08 30 81 e5                                      str r3, [r1, #8]
0060bf18  04 00 a0 e1                                      mov r0, r4
0060bf1c  10 d0 8d e2                                      add sp, sp, #0x10
0060bf20  70 80 bd e8                                      pop {r4, r5, r6, pc}
0060bf24  08 10 81 e2                                      add r1, r1, #8
0060bf28  0c 00 8d e2                                      add r0, sp, #0xc
0060bf2c  ce fd ff eb                                      bl #0x60b66c
0060bf30  0c 60 9d e5                                      ldr r6, [sp, #0xc]
0060bf34  0c 30 96 e5                                      ldr r3, [r6, #0xc]
0060bf38  00 20 93 e5                                      ldr r2, [r3]
0060bf3c  00 00 52 e3                                      cmp r2, #0
0060bf40  15 00 00 ca                                      bgt #0x60bf9c
0060bf44  00 00 56 e3                                      cmp r6, #0
0060bf48  00 60 84 e5                                      str r6, [r4]
0060bf4c  00 30 96 15                                      ldrne r3, [r6]
0060bf50  01 30 83 12                                      addne r3, r3, #1
0060bf54  00 30 86 15                                      strne r3, [r6]
0060bf58  0c 60 9d 15                                      ldrne r6, [sp, #0xc]
0060bf5c  00 00 56 e3                                      cmp r6, #0
0060bf60  ec ff ff 0a                                      beq #0x60bf18
0060bf64  00 30 96 e5                                      ldr r3, [r6]
0060bf68  01 30 43 e2                                      sub r3, r3, #1
0060bf6c  00 00 53 e3                                      cmp r3, #0
0060bf70  00 30 86 e5                                      str r3, [r6]
0060bf74  e7 ff ff 1a                                      bne #0x60bf18
0060bf78  0c 00 96 e5                                      ldr r0, [r6, #0xc]
0060bf7c  00 00 50 e3                                      cmp r0, #0
0060bf80  00 00 00 0a                                      beq #0x60bf88
0060bf84  4b 08 f4 eb                                      bl #0x30e0b8
0060bf88  00 30 a0 e3                                      mov r3, #0
0060bf8c  0c 30 86 e5                                      str r3, [r6, #0xc]
0060bf90  e0 ff ff ea                                      b #0x60bf18
0060bf94  0c 30 9d e5                                      ldr r3, [sp, #0xc]
0060bf98  0c 30 93 e5                                      ldr r3, [r3, #0xc]
0060bf9c  85 31 83 e0                                      add r3, r3, r5, lsl #3
0060bfa0  08 10 93 e5                                      ldr r1, [r3, #8]
0060bfa4  01 50 85 e2                                      add r5, r5, #1
0060bfa8  08 00 83 e2                                      add r0, r3, #8
0060bfac  01 10 80 e0                                      add r1, r0, r1
0060bfb0  02 00 55 e1                                      cmp r5, r2
0060bfb4  08 10 83 e5                                      str r1, [r3, #8]
0060bfb8  f5 ff ff 1a                                      bne #0x60bf94
0060bfbc  0c 60 9d e5                                      ldr r6, [sp, #0xc]
0060bfc0  df ff ff ea                                      b #0x60bf44
0060bfc4  0c 30 91 e5                                      ldr r3, [r1, #0xc]
0060bfc8  02 20 a0 e3                                      mov r2, #2
0060bfcc  08 20 81 e5                                      str r2, [r1, #8]
0060bfd0  00 00 53 e3                                      cmp r3, #0
0060bfd4  08 30 81 e2                                      add r3, r1, #8
0060bfd8  02 00 00 1a                                      bne #0x60bfe8
0060bfdc  10 20 91 e5                                      ldr r2, [r1, #0x10]
0060bfe0  00 00 52 e3                                      cmp r2, #0
0060bfe4  0c 00 00 0a                                      beq #0x60c01c
0060bfe8  00 30 84 e5                                      str r3, [r4]
0060bfec  08 30 91 e5                                      ldr r3, [r1, #8]
0060bff0  00 00 53 e3                                      cmp r3, #0
0060bff4  c7 ff ff 1a                                      bne #0x60bf18
0060bff8  14 00 91 e5                                      ldr r0, [r1, #0x14]
0060bffc  00 00 50 e3                                      cmp r0, #0
0060c000  02 00 00 0a                                      beq #0x60c010
0060c004  04 10 8d e5                                      str r1, [sp, #4]
0060c008  2a 08 f4 eb                                      bl #0x30e0b8
0060c00c  04 10 9d e5                                      ldr r1, [sp, #4]
0060c010  00 30 a0 e3                                      mov r3, #0
0060c014  14 30 81 e5                                      str r3, [r1, #0x14]
0060c018  be ff ff ea                                      b #0x60bf18
0060c01c  14 00 91 e5                                      ldr r0, [r1, #0x14]
0060c020  00 60 90 e5                                      ldr r6, [r0]
0060c024  00 00 56 e3                                      cmp r6, #0
0060c028  08 00 00 da                                      ble #0x60c050
0060c02c  0c 00 93 e5                                      ldr r0, [r3, #0xc]
0060c030  82 01 80 e0                                      add r0, r0, r2, lsl #3
0060c034  08 c0 90 e5                                      ldr ip, [r0, #8]
0060c038  01 20 82 e2                                      add r2, r2, #1
0060c03c  08 50 80 e2                                      add r5, r0, #8
0060c040  0c c0 85 e0                                      add ip, r5, ip
0060c044  06 00 52 e1                                      cmp r2, r6
0060c048  08 c0 80 e5                                      str ip, [r0, #8]
0060c04c  f6 ff ff 1a                                      bne #0x60c02c
0060c050  01 20 a0 e3                                      mov r2, #1
0060c054  10 20 81 e5                                      str r2, [r1, #0x10]
0060c058  e2 ff ff ea                                      b #0x60bfe8

; FUNCTION 0x00645588, declared_size=1504, range_size=1504, mode=arm
; class-group: glitch::collada::CMesh
; alias: _ZN6glitch7collada5CMeshC1ERKNS0_16CColladaDatabaseEPNS_5video12IVideoDriverERKNS0_9SGeometryERKNS0_13SBufferConfigESD_b
; demangled: glitch::collada::CMesh::CMesh(glitch::collada::CColladaDatabase const&, glitch::video::IVideoDriver*, glitch::collada::SGeometry const&, glitch::collada::SBufferConfig const&, glitch::collada::SBufferConfig const&, bool)
; decoder-mode: arm
00645588  f0 4f 2d e9                                      push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0064558c  bc 45 9f e5                                      ldr r4, [pc, #0x5bc]
00645590  bc c5 9f e5                                      ldr ip, [pc, #0x5bc]
00645594  00 b0 a0 e1                                      mov fp, r0
00645598  04 40 8f e0                                      add r4, pc, r4
0064559c  0c c0 94 e7                                      ldr ip, [r4, ip]
006455a0  00 00 a0 e3                                      mov r0, #0
006455a4  04 00 8b e5                                      str r0, [fp, #4]
006455a8  08 c0 8c e2                                      add ip, ip, #8
006455ac  00 c0 8b e5                                      str ip, [fp]
006455b0  01 50 a0 e1                                      mov r5, r1
006455b4  00 10 91 e5                                      ldr r1, [r1]
006455b8  74 d0 4d e2                                      sub sp, sp, #0x74
006455bc  2c 20 8d e5                                      str r2, [sp, #0x2c]
006455c0  34 30 8d e5                                      str r3, [sp, #0x34]
006455c4  0c 10 8b e5                                      str r1, [fp, #0xc]
006455c8  04 30 95 e5                                      ldr r3, [r5, #4]
006455cc  00 00 51 e1                                      cmp r1, r0
006455d0  10 30 8b e5                                      str r3, [fp, #0x10]
006455d4  a0 20 dd e5                                      ldrb r2, [sp, #0xa0]
006455d8  10 20 8d e5                                      str r2, [sp, #0x10]
006455dc  03 00 00 0a                                      beq #0x6455f0
006455e0  04 30 91 e5                                      ldr r3, [r1, #4]
006455e4  00 00 53 e1                                      cmp r3, r0
006455e8  01 30 83 12                                      addne r3, r3, #1
006455ec  04 30 81 15                                      strne r3, [r1, #4]
006455f0  60 05 9f e5                                      ldr r0, [pc, #0x560]
006455f4  60 35 9f e5                                      ldr r3, [pc, #0x560]
006455f8  bf 14 a0 e3                                      mov r1, #0xbf000000
006455fc  00 00 94 e7                                      ldr r0, [r4, r0]
00645600  03 30 94 e7                                      ldr r3, [r4, r3]
00645604  fe 25 a0 e3                                      mov r2, #0x3f800000
00645608  02 15 81 e2                                      add r1, r1, #0x800000
0064560c  08 c0 83 e2                                      add ip, r3, #8
00645610  04 00 80 e2                                      add r0, r0, #4
00645614  00 30 a0 e3                                      mov r3, #0
00645618  08 00 8b e5                                      str r0, [fp, #8]
0064561c  2c 10 8b e5                                      str r1, [fp, #0x2c]
00645620  38 20 8b e5                                      str r2, [fp, #0x38]
00645624  24 10 8b e5                                      str r1, [fp, #0x24]
00645628  28 10 8b e5                                      str r1, [fp, #0x28]
0064562c  30 20 8b e5                                      str r2, [fp, #0x30]
00645630  34 20 8b e5                                      str r2, [fp, #0x34]
00645634  00 c0 8b e5                                      str ip, [fp]
00645638  20 30 8b e5                                      str r3, [fp, #0x20]
0064563c  14 30 8b e5                                      str r3, [fp, #0x14]
00645640  18 30 8b e5                                      str r3, [fp, #0x18]
00645644  1c 30 8b e5                                      str r3, [fp, #0x1c]
00645648  34 c0 9d e5                                      ldr ip, [sp, #0x34]
0064564c  18 10 8b e2                                      add r1, fp, #0x18
00645650  01 00 a0 e1                                      mov r0, r1
00645654  00 30 9c e5                                      ldr r3, [ip]
00645658  1c 10 8d e5                                      str r1, [sp, #0x1c]
0064565c  08 30 8b e5                                      str r3, [fp, #8]
00645660  0c 30 9c e5                                      ldr r3, [ip, #0xc]
00645664  0c 30 93 e5                                      ldr r3, [r3, #0xc]
00645668  03 10 a0 e1                                      mov r1, r3
0064566c  18 30 8d e5                                      str r3, [sp, #0x18]
00645670  b2 fe ff eb                                      bl #0x645140
00645674  00 30 95 e5                                      ldr r3, [r5]
00645678  34 20 9d e5                                      ldr r2, [sp, #0x34]
0064567c  24 30 93 e5                                      ldr r3, [r3, #0x24]
00645680  0c 70 92 e5                                      ldr r7, [r2, #0xc]
00645684  20 20 93 e5                                      ldr r2, [r3, #0x20]
00645688  04 30 92 e5                                      ldr r3, [r2, #4]
0064568c  64 20 92 e5                                      ldr r2, [r2, #0x64]
00645690  00 00 52 e3                                      cmp r2, #0
00645694  00 20 a0 d3                                      movle r2, #0
00645698  01 20 a0 c3                                      movgt r2, #1
0064569c  00 00 53 e3                                      cmp r3, #0
006456a0  14 20 8d e5                                      str r2, [sp, #0x14]
006456a4  38 30 8d 05                                      streq r3, [sp, #0x38]
006456a8  0a 00 00 0a                                      beq #0x6456d8
006456ac  14 10 93 e5                                      ldr r1, [r3, #0x14]
006456b0  a8 34 9f e5                                      ldr r3, [pc, #0x4a8]
006456b4  03 30 94 e7                                      ldr r3, [r4, r3]
006456b8  00 30 93 e5                                      ldr r3, [r3]
006456bc  20 30 93 e5                                      ldr r3, [r3, #0x20]
006456c0  34 30 93 e5                                      ldr r3, [r3, #0x34]
006456c4  03 00 a0 e1                                      mov r0, r3
006456c8  00 30 93 e5                                      ldr r3, [r3]
006456cc  0f e0 a0 e1                                      mov lr, pc
006456d0  0c f0 93 e5                                      ldr pc, [r3, #0xc]
006456d4  38 00 8d e5                                      str r0, [sp, #0x38]
006456d8  84 34 9f e5                                      ldr r3, [pc, #0x484]
006456dc  14 c0 9d e5                                      ldr ip, [sp, #0x14]
006456e0  38 10 9d e5                                      ldr r1, [sp, #0x38]
006456e4  03 30 94 e7                                      ldr r3, [r4, r3]
006456e8  00 00 5c e3                                      cmp ip, #0
006456ec  58 10 8d e5                                      str r1, [sp, #0x58]
006456f0  08 30 83 e2                                      add r3, r3, #8
006456f4  54 30 8d e5                                      str r3, [sp, #0x54]
006456f8  02 00 00 0a                                      beq #0x645708
006456fc  00 30 97 e5                                      ldr r3, [r7]
00645700  00 00 53 e3                                      cmp r3, #0
00645704  f3 00 00 1a                                      bne #0x645ad8
00645708  00 10 a0 e3                                      mov r1, #0
0064570c  30 10 8d e5                                      str r1, [sp, #0x30]
00645710  18 20 9d e5                                      ldr r2, [sp, #0x18]
00645714  00 00 52 e3                                      cmp r2, #0
00645718  c6 00 00 0a                                      beq #0x645a38
0064571c  5c c0 8d e2                                      add ip, sp, #0x5c
00645720  60 10 8d e2                                      add r1, sp, #0x60
00645724  00 80 a0 e3                                      mov r8, #0
00645728  48 30 8d e2                                      add r3, sp, #0x48
0064572c  20 c0 8d e5                                      str ip, [sp, #0x20]
00645730  24 10 8d e5                                      str r1, [sp, #0x24]
00645734  64 20 8d e2                                      add r2, sp, #0x64
00645738  54 c0 8d e2                                      add ip, sp, #0x54
0064573c  68 10 8d e2                                      add r1, sp, #0x68
00645740  44 b0 8d e5                                      str fp, [sp, #0x44]
00645744  08 a0 a0 e1                                      mov sl, r8
00645748  08 90 a0 e1                                      mov sb, r8
0064574c  3c 20 8d e5                                      str r2, [sp, #0x3c]
00645750  28 c0 8d e5                                      str ip, [sp, #0x28]
00645754  40 10 8d e5                                      str r1, [sp, #0x40]
00645758  03 b0 a0 e1                                      mov fp, r3
0064575c  71 00 00 ea                                      b #0x645928
00645760  10 30 97 e5                                      ldr r3, [r7, #0x10]
00645764  ff cf 0f e3                                      movw ip, #0xffff
00645768  08 30 83 e0                                      add r3, r3, r8
0064576c  24 20 93 e5                                      ldr r2, [r3, #0x24]
00645770  0c 00 52 e1                                      cmp r2, ip
00645774  92 00 00 da                                      ble #0x6459c4
00645778  2c 10 93 e5                                      ldr r1, [r3, #0x2c]
0064577c  40 00 9d e5                                      ldr r0, [sp, #0x40]
00645780  28 20 9d e5                                      ldr r2, [sp, #0x28]
00645784  cc fe ff eb                                      bl #0x6452bc
00645788  68 30 9d e5                                      ldr r3, [sp, #0x68]
0064578c  00 00 53 e3                                      cmp r3, #0
00645790  67 00 00 0a                                      beq #0x645934
00645794  00 20 93 e5                                      ldr r2, [r3]
00645798  01 20 82 e2                                      add r2, r2, #1
0064579c  00 20 83 e5                                      str r2, [r3]
006457a0  68 50 9d e5                                      ldr r5, [sp, #0x68]
006457a4  00 00 55 e3                                      cmp r5, #0
006457a8  62 00 00 0a                                      beq #0x645938
006457ac  00 30 95 e5                                      ldr r3, [r5]
006457b0  01 30 43 e2                                      sub r3, r3, #1
006457b4  00 00 53 e3                                      cmp r3, #0
006457b8  00 30 85 e5                                      str r3, [r5]
006457bc  05 00 00 1a                                      bne #0x6457d8
006457c0  0c 00 95 e5                                      ldr r0, [r5, #0xc]
006457c4  00 00 50 e3                                      cmp r0, #0
006457c8  00 00 00 0a                                      beq #0x6457d0
006457cc  39 22 f3 eb                                      bl #0x30e0b8
006457d0  00 10 a0 e3                                      mov r1, #0
006457d4  0c 10 85 e5                                      str r1, [r5, #0xc]
006457d8  68 90 8d e5                                      str sb, [sp, #0x68]
006457dc  09 60 a0 e1                                      mov r6, sb
006457e0  10 30 9d e5                                      ldr r3, [sp, #0x10]
006457e4  00 00 53 e3                                      cmp r3, #0
006457e8  56 00 00 0a                                      beq #0x645948
006457ec  10 30 97 e5                                      ldr r3, [r7, #0x10]
006457f0  08 30 83 e0                                      add r3, r3, r8
006457f4  34 40 93 e5                                      ldr r4, [r3, #0x34]
006457f8  00 00 54 e3                                      cmp r4, #0
006457fc  51 00 00 0a                                      beq #0x645948
00645800  04 30 94 e5                                      ldr r3, [r4, #4]
00645804  01 30 83 e2                                      add r3, r3, #1
00645808  04 30 84 e5                                      str r3, [r4, #4]
0064580c  00 00 54 e3                                      cmp r4, #0
00645810  60 90 8d e5                                      str sb, [sp, #0x60]
00645814  5c 90 8d e5                                      str sb, [sp, #0x5c]
00645818  48 40 8d e5                                      str r4, [sp, #0x48]
0064581c  4c 40 8d 05                                      streq r4, [sp, #0x4c]
00645820  08 00 00 0a                                      beq #0x645848
00645824  04 30 94 e5                                      ldr r3, [r4, #4]
00645828  01 30 83 e2                                      add r3, r3, #1
0064582c  04 30 84 e5                                      str r3, [r4, #4]
00645830  60 30 9d e5                                      ldr r3, [sp, #0x60]
00645834  00 00 53 e3                                      cmp r3, #0
00645838  4c 30 8d e5                                      str r3, [sp, #0x4c]
0064583c  00 20 93 15                                      ldrne r2, [r3]
00645840  01 20 82 12                                      addne r2, r2, #1
00645844  00 20 83 15                                      strne r2, [r3]
00645848  5c 30 9d e5                                      ldr r3, [sp, #0x5c]
0064584c  0b 10 a0 e1                                      mov r1, fp
00645850  00 00 53 e3                                      cmp r3, #0
00645854  50 30 8d e5                                      str r3, [sp, #0x50]
00645858  00 20 93 15                                      ldrne r2, [r3]
0064585c  01 20 82 12                                      addne r2, r2, #1
00645860  00 20 83 15                                      strne r2, [r3]
00645864  1c 00 9d e5                                      ldr r0, [sp, #0x1c]
00645868  c9 fe ff eb                                      bl #0x645394
0064586c  0b 00 a0 e1                                      mov r0, fp
00645870  04 fd ff eb                                      bl #0x644c88
00645874  20 00 9d e5                                      ldr r0, [sp, #0x20]
00645878  7b d2 fc eb                                      bl #0x57a26c
0064587c  24 00 9d e5                                      ldr r0, [sp, #0x24]
00645880  d8 2c f3 eb                                      bl #0x310be8
00645884  10 10 9d e5                                      ldr r1, [sp, #0x10]
00645888  00 00 51 e3                                      cmp r1, #0
0064588c  04 00 00 0a                                      beq #0x6458a4
00645890  10 30 97 e5                                      ldr r3, [r7, #0x10]
00645894  08 30 83 e0                                      add r3, r3, r8
00645898  34 20 93 e5                                      ldr r2, [r3, #0x34]
0064589c  00 00 52 e3                                      cmp r2, #0
006458a0  39 00 00 0a                                      beq #0x64598c
006458a4  00 00 54 e3                                      cmp r4, #0
006458a8  42 00 00 1a                                      bne #0x6459b8
006458ac  00 00 55 e3                                      cmp r5, #0
006458b0  0a 00 00 0a                                      beq #0x6458e0
006458b4  00 30 95 e5                                      ldr r3, [r5]
006458b8  01 30 43 e2                                      sub r3, r3, #1
006458bc  00 00 53 e3                                      cmp r3, #0
006458c0  00 30 85 e5                                      str r3, [r5]
006458c4  05 00 00 1a                                      bne #0x6458e0
006458c8  0c 00 95 e5                                      ldr r0, [r5, #0xc]
006458cc  00 00 50 e3                                      cmp r0, #0
006458d0  00 00 00 0a                                      beq #0x6458d8
006458d4  f7 21 f3 eb                                      bl #0x30e0b8
006458d8  00 20 a0 e3                                      mov r2, #0
006458dc  0c 20 85 e5                                      str r2, [r5, #0xc]
006458e0  00 00 56 e3                                      cmp r6, #0
006458e4  0a 00 00 0a                                      beq #0x645914
006458e8  00 30 96 e5                                      ldr r3, [r6]
006458ec  01 30 43 e2                                      sub r3, r3, #1
006458f0  00 00 53 e3                                      cmp r3, #0
006458f4  00 30 86 e5                                      str r3, [r6]
006458f8  05 00 00 1a                                      bne #0x645914
006458fc  0c 00 96 e5                                      ldr r0, [r6, #0xc]
00645900  00 00 50 e3                                      cmp r0, #0
00645904  00 00 00 0a                                      beq #0x64590c
00645908  ea 21 f3 eb                                      bl #0x30e0b8
0064590c  00 30 a0 e3                                      mov r3, #0
00645910  0c 30 86 e5                                      str r3, [r6, #0xc]
00645914  18 c0 9d e5                                      ldr ip, [sp, #0x18]
00645918  01 a0 8a e2                                      add sl, sl, #1
0064591c  38 80 88 e2                                      add r8, r8, #0x38
00645920  0a 00 5c e1                                      cmp ip, sl
00645924  42 00 00 0a                                      beq #0x645a34
00645928  14 20 9d e5                                      ldr r2, [sp, #0x14]
0064592c  00 00 52 e3                                      cmp r2, #0
00645930  8a ff ff 1a                                      bne #0x645760
00645934  00 50 a0 e3                                      mov r5, #0
00645938  10 30 9d e5                                      ldr r3, [sp, #0x10]
0064593c  05 60 a0 e1                                      mov r6, r5
00645940  00 00 53 e3                                      cmp r3, #0
00645944  a8 ff ff 1a                                      bne #0x6457ec
00645948  00 10 a0 e3                                      mov r1, #0
0064594c  38 00 a0 e3                                      mov r0, #0x38
00645950  15 ba fb eb                                      bl #0x5341ac
00645954  98 c0 9d e5                                      ldr ip, [sp, #0x98]
00645958  00 40 a0 e1                                      mov r4, r0
0064595c  2c 10 9d e5                                      ldr r1, [sp, #0x2c]
00645960  00 c0 8d e5                                      str ip, [sp]
00645964  9c c0 9d e5                                      ldr ip, [sp, #0x9c]
00645968  07 20 a0 e1                                      mov r2, r7
0064596c  0a 30 a0 e1                                      mov r3, sl
00645970  04 c0 8d e5                                      str ip, [sp, #4]
00645974  14 c0 9d e5                                      ldr ip, [sp, #0x14]
00645978  08 c0 8d e5                                      str ip, [sp, #8]
0064597c  1d e0 01 eb                                      bl #0x6bd9f8
00645980  00 00 54 e3                                      cmp r4, #0
00645984  9d ff ff 1a                                      bne #0x645800
00645988  9f ff ff ea                                      b #0x64580c
0064598c  00 00 54 e3                                      cmp r4, #0
00645990  34 40 83 05                                      streq r4, [r3, #0x34]
00645994  c4 ff ff 0a                                      beq #0x6458ac
00645998  04 20 94 e5                                      ldr r2, [r4, #4]
0064599c  01 20 82 e2                                      add r2, r2, #1
006459a0  04 20 84 e5                                      str r2, [r4, #4]
006459a4  34 00 93 e5                                      ldr r0, [r3, #0x34]
006459a8  34 40 83 e5                                      str r4, [r3, #0x34]
006459ac  00 00 50 e3                                      cmp r0, #0
006459b0  00 00 00 0a                                      beq #0x6459b8
006459b4  f2 5e f3 eb                                      bl #0x31d584
006459b8  04 00 a0 e1                                      mov r0, r4
006459bc  f0 5e f3 eb                                      bl #0x31d584
006459c0  b9 ff ff ea                                      b #0x6458ac
006459c4  2c 10 93 e5                                      ldr r1, [r3, #0x2c]
006459c8  3c 00 9d e5                                      ldr r0, [sp, #0x3c]
006459cc  28 20 9d e5                                      ldr r2, [sp, #0x28]
006459d0  54 fe ff eb                                      bl #0x645328
006459d4  64 30 9d e5                                      ldr r3, [sp, #0x64]
006459d8  00 00 53 e3                                      cmp r3, #0
006459dc  d4 ff ff 0a                                      beq #0x645934
006459e0  00 20 93 e5                                      ldr r2, [r3]
006459e4  01 20 82 e2                                      add r2, r2, #1
006459e8  00 20 83 e5                                      str r2, [r3]
006459ec  64 60 9d e5                                      ldr r6, [sp, #0x64]
006459f0  00 00 56 e3                                      cmp r6, #0
006459f4  06 50 a0 01                                      moveq r5, r6
006459f8  78 ff ff 0a                                      beq #0x6457e0
006459fc  00 30 96 e5                                      ldr r3, [r6]
00645a00  01 30 43 e2                                      sub r3, r3, #1
00645a04  00 00 53 e3                                      cmp r3, #0
00645a08  00 30 86 e5                                      str r3, [r6]
00645a0c  05 00 00 1a                                      bne #0x645a28
00645a10  0c 00 96 e5                                      ldr r0, [r6, #0xc]
00645a14  00 00 50 e3                                      cmp r0, #0
00645a18  00 00 00 0a                                      beq #0x645a20
00645a1c  a5 21 f3 eb                                      bl #0x30e0b8
00645a20  00 20 a0 e3                                      mov r2, #0
00645a24  0c 20 86 e5                                      str r2, [r6, #0xc]
00645a28  64 90 8d e5                                      str sb, [sp, #0x64]
00645a2c  09 50 a0 e1                                      mov r5, sb
00645a30  6a ff ff ea                                      b #0x6457e0
00645a34  44 b0 9d e5                                      ldr fp, [sp, #0x44]
00645a38  38 10 9d e5                                      ldr r1, [sp, #0x38]
00645a3c  00 00 51 e3                                      cmp r1, #0
00645a40  01 00 00 0a                                      beq #0x645a4c
00645a44  01 00 a0 e1                                      mov r0, r1
00645a48  cd 5e f3 eb                                      bl #0x31d584
00645a4c  34 20 9d e5                                      ldr r2, [sp, #0x34]
00645a50  30 c0 9d e5                                      ldr ip, [sp, #0x30]
00645a54  0c 30 92 e5                                      ldr r3, [r2, #0xc]
00645a58  00 00 5c e3                                      cmp ip, #0
00645a5c  14 10 83 e2                                      add r1, r3, #0x14
00645a60  20 20 83 e2                                      add r2, r3, #0x20
00645a64  08 40 91 e5                                      ldr r4, [r1, #8]
00645a68  20 c0 93 e5                                      ldr ip, [r3, #0x20]
00645a6c  14 50 93 e5                                      ldr r5, [r3, #0x14]
00645a70  08 00 92 e5                                      ldr r0, [r2, #8]
00645a74  04 10 91 e5                                      ldr r1, [r1, #4]
00645a78  04 30 92 e5                                      ldr r3, [r2, #4]
00645a7c  24 50 8b e5                                      str r5, [fp, #0x24]
00645a80  28 10 8b e5                                      str r1, [fp, #0x28]
00645a84  2c 40 8b e5                                      str r4, [fp, #0x2c]
00645a88  30 c0 8b e5                                      str ip, [fp, #0x30]
00645a8c  34 30 8b e5                                      str r3, [fp, #0x34]
00645a90  38 00 8b e5                                      str r0, [fp, #0x38]
00645a94  0c 00 00 0a                                      beq #0x645acc
00645a98  30 10 9d e5                                      ldr r1, [sp, #0x30]
00645a9c  00 30 91 e5                                      ldr r3, [r1]
00645aa0  01 30 43 e2                                      sub r3, r3, #1
00645aa4  00 00 53 e3                                      cmp r3, #0
00645aa8  00 30 81 e5                                      str r3, [r1]
00645aac  06 00 00 1a                                      bne #0x645acc
00645ab0  0c 00 91 e5                                      ldr r0, [r1, #0xc]
00645ab4  00 00 50 e3                                      cmp r0, #0
00645ab8  00 00 00 0a                                      beq #0x645ac0
00645abc  7d 21 f3 eb                                      bl #0x30e0b8
00645ac0  30 20 9d e5                                      ldr r2, [sp, #0x30]
00645ac4  00 30 a0 e3                                      mov r3, #0
00645ac8  0c 30 82 e5                                      str r3, [r2, #0xc]
00645acc  0b 00 a0 e1                                      mov r0, fp
00645ad0  74 d0 8d e2                                      add sp, sp, #0x74
00645ad4  f0 8f bd e8                                      pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
00645ad8  08 30 97 e5                                      ldr r3, [r7, #8]
00645adc  54 20 8d e2                                      add r2, sp, #0x54
00645ae0  6c 00 8d e2                                      add r0, sp, #0x6c
00645ae4  24 10 93 e5                                      ldr r1, [r3, #0x24]
00645ae8  d8 fd ff eb                                      bl #0x645250
00645aec  6c 20 9d e5                                      ldr r2, [sp, #0x6c]
00645af0  00 00 52 e3                                      cmp r2, #0
00645af4  03 ff ff 0a                                      beq #0x645708
00645af8  00 30 92 e5                                      ldr r3, [r2]
00645afc  01 30 83 e2                                      add r3, r3, #1
00645b00  00 30 82 e5                                      str r3, [r2]
00645b04  6c 20 9d e5                                      ldr r2, [sp, #0x6c]
00645b08  00 00 52 e3                                      cmp r2, #0
00645b0c  30 20 8d e5                                      str r2, [sp, #0x30]
00645b10  fe fe ff 0a                                      beq #0x645710
00645b14  00 30 92 e5                                      ldr r3, [r2]
00645b18  01 30 43 e2                                      sub r3, r3, #1
00645b1c  00 00 53 e3                                      cmp r3, #0
00645b20  00 30 82 e5                                      str r3, [r2]
00645b24  06 00 00 1a                                      bne #0x645b44
00645b28  0c 00 92 e5                                      ldr r0, [r2, #0xc]
00645b2c  00 00 50 e3                                      cmp r0, #0
00645b30  00 00 00 0a                                      beq #0x645b38
00645b34  5f 21 f3 eb                                      bl #0x30e0b8
00645b38  30 c0 9d e5                                      ldr ip, [sp, #0x30]
00645b3c  00 30 a0 e3                                      mov r3, #0
00645b40  0c 30 8c e5                                      str r3, [ip, #0xc]
00645b44  00 30 a0 e3                                      mov r3, #0
00645b48  6c 30 8d e5                                      str r3, [sp, #0x6c]
00645b4c  ef fe ff ea                                      b #0x645710
; mapping-symbol data/literal pool
00645b50  f8 f4 34 00 40 0a 00 00 b4 17 00 00 c0 3d 00 00  .byte 0xf8, 0xf4, 0x34, 0x00, 0x40, 0x0a, 0x00, 0x00, 0xb4, 0x17, 0x00, 0x00, 0xc0, 0x3d, 0x00, 0x00
00645b60  48 44 00 00 fc 46 00 00                          .byte 0x48, 0x44, 0x00, 0x00, 0xfc, 0x46, 0x00, 0x00


; FUNCTION 0x00669e00, declared_size=16, range_size=16, mode=arm
; class-group: glitch::collada::SAnimationAccessor
; alias: _ZNK6glitch7collada18SAnimationAccessor9getTargetEv
; demangled: glitch::collada::SAnimationAccessor::getTarget() const
; decoder-mode: arm
00669e00  00 30 90 e5                                      ldr r3, [r0]
00669e04  10 30 93 e5                                      ldr r3, [r3, #0x10]
00669e08  04 00 93 e5                                      ldr r0, [r3, #4]
00669e0c  1e ff 2f e1                                      bx lr


; FUNCTION 0x00669e10, declared_size=20, range_size=20, mode=arm
; class-group: glitch::collada::SAnimationAccessor
; alias: _ZNK6glitch7collada18SAnimationAccessor7getTypeEi
; demangled: glitch::collada::SAnimationAccessor::getType(int) const
; decoder-mode: arm
00669e10  00 30 90 e5                                      ldr r3, [r0]
00669e14  10 30 93 e5                                      ldr r3, [r3, #0x10]
00669e18  01 32 83 e0                                      add r3, r3, r1, lsl #4
00669e1c  08 00 93 e5                                      ldr r0, [r3, #8]
00669e20  1e ff 2f e1                                      bx lr


; FUNCTION 0x00669e24, declared_size=32, range_size=32, mode=arm
; class-group: glitch::collada::SAnimationAccessor
; alias: _ZNK6glitch7collada18SAnimationAccessor9getOutputEi
; demangled: glitch::collada::SAnimationAccessor::getOutput(int) const
; decoder-mode: arm
00669e24  0c 00 90 e8                                      ldm r0, {r2, r3}
00669e28  1c 00 a0 e3                                      mov r0, #0x1c
00669e2c  08 20 92 e5                                      ldr r2, [r2, #8]
00669e30  90 21 22 e0                                      mla r2, r0, r1, r2
00669e34  18 20 92 e5                                      ldr r2, [r2, #0x18]
00669e38  82 31 83 e0                                      add r3, r3, r2, lsl #3
00669e3c  04 00 83 e2                                      add r0, r3, #4
00669e40  1e ff 2f e1                                      bx lr


; FUNCTION 0x00669e44, declared_size=16, range_size=16, mode=arm
; class-group: glitch::collada::SAnimationAccessor
; alias: _ZNK6glitch7collada18SAnimationAccessor10getChannelEi
; demangled: glitch::collada::SAnimationAccessor::getChannel(int) const
; decoder-mode: arm
00669e44  00 30 90 e5                                      ldr r3, [r0]
00669e48  10 00 93 e5                                      ldr r0, [r3, #0x10]
00669e4c  01 02 80 e0                                      add r0, r0, r1, lsl #4
00669e50  1e ff 2f e1                                      bx lr


; FUNCTION 0x00669e54, declared_size=20, range_size=20, mode=arm
; class-group: glitch::collada::SAnimationAccessor
; alias: _ZNK6glitch7collada18SAnimationAccessor15hasDefaultValueEv
; demangled: glitch::collada::SAnimationAccessor::hasDefaultValue() const
; decoder-mode: arm
00669e54  00 30 90 e5                                      ldr r3, [r0]
00669e58  18 00 93 e5                                      ldr r0, [r3, #0x18]
00669e5c  00 00 50 e2                                      subs r0, r0, #0
00669e60  01 00 a0 13                                      movne r0, #1
00669e64  1e ff 2f e1                                      bx lr


; FUNCTION 0x00669e68, declared_size=16, range_size=16, mode=arm
; class-group: glitch::collada::SAnimationAccessor
; alias: _ZNK6glitch7collada18SAnimationAccessor15getDefaultValueEv
; demangled: glitch::collada::SAnimationAccessor::getDefaultValue() const
; decoder-mode: arm
00669e68  00 30 90 e5                                      ldr r3, [r0]
00669e6c  18 30 93 e5                                      ldr r3, [r3, #0x18]
00669e70  08 00 93 e5                                      ldr r0, [r3, #8]
00669e74  1e ff 2f e1                                      bx lr


; FUNCTION 0x00669e78, declared_size=12, range_size=12, mode=arm
; class-group: glitch::collada::SAnimationAccessor
; alias: _ZNK6glitch7collada18SAnimationAccessor16getChannelsCountEv
; demangled: glitch::collada::SAnimationAccessor::getChannelsCount() const
; decoder-mode: arm
00669e78  00 30 90 e5                                      ldr r3, [r0]
00669e7c  0c 00 93 e5                                      ldr r0, [r3, #0xc]
00669e80  1e ff 2f e1                                      bx lr


; FUNCTION 0x00669e84, declared_size=24, range_size=24, mode=arm
; class-group: glitch::collada::SAnimationAccessor
; alias: _ZNK6glitch7collada18SAnimationAccessor19getTimeInternalTypeEi
; demangled: glitch::collada::SAnimationAccessor::getTimeInternalType(int) const
; decoder-mode: arm
00669e84  00 30 90 e5                                      ldr r3, [r0]
00669e88  1c 20 a0 e3                                      mov r2, #0x1c
00669e8c  08 30 93 e5                                      ldr r3, [r3, #8]
00669e90  92 31 23 e0                                      mla r3, r2, r1, r3
00669e94  04 00 93 e5                                      ldr r0, [r3, #4]
00669e98  1e ff 2f e1                                      bx lr


; FUNCTION 0x00669e9c, declared_size=24, range_size=24, mode=arm
; class-group: glitch::collada::SAnimationAccessor
; alias: _ZNK6glitch7collada18SAnimationAccessor20getInterpolationTypeEi
; demangled: glitch::collada::SAnimationAccessor::getInterpolationType(int) const
; decoder-mode: arm
00669e9c  00 30 90 e5                                      ldr r3, [r0]
00669ea0  1c 20 a0 e3                                      mov r2, #0x1c
00669ea4  92 01 02 e0                                      mul r2, r2, r1
00669ea8  08 30 93 e5                                      ldr r3, [r3, #8]
00669eac  02 00 93 e7                                      ldr r0, [r3, r2]
00669eb0  1e ff 2f e1                                      bx lr


; FUNCTION 0x00669eb4, declared_size=16, range_size=16, mode=arm
; class-group: glitch::collada::SAnimationAccessor
; alias: _ZNK6glitch7collada18SAnimationAccessor10getOffsetsEv
; demangled: glitch::collada::SAnimationAccessor::getOffsets() const
; decoder-mode: arm
00669eb4  00 30 90 e5                                      ldr r3, [r0]
00669eb8  1c 30 93 e5                                      ldr r3, [r3, #0x1c]
00669ebc  08 00 93 e5                                      ldr r0, [r3, #8]
00669ec0  1e ff 2f e1                                      bx lr


; FUNCTION 0x00669ec4, declared_size=16, range_size=16, mode=arm
; class-group: glitch::collada::SAnimationAccessor
; alias: _ZNK6glitch7collada18SAnimationAccessor9getScalesEv
; demangled: glitch::collada::SAnimationAccessor::getScales() const
; decoder-mode: arm
00669ec4  00 30 90 e5                                      ldr r3, [r0]
00669ec8  1c 30 93 e5                                      ldr r3, [r3, #0x1c]
00669ecc  04 00 93 e5                                      ldr r0, [r3, #4]
00669ed0  1e ff 2f e1                                      bx lr


; FUNCTION 0x00669ed4, declared_size=24, range_size=24, mode=arm
; class-group: glitch::collada::SAnimationAccessor
; alias: _ZNK6glitch7collada18SAnimationAccessor18getOffsetScaleTypeEv
; demangled: glitch::collada::SAnimationAccessor::getOffsetScaleType() const
; decoder-mode: arm
00669ed4  00 30 90 e5                                      ldr r3, [r0]
00669ed8  1c 30 93 e5                                      ldr r3, [r3, #0x1c]
00669edc  00 00 53 e3                                      cmp r3, #0
00669ee0  02 00 a0 03                                      moveq r0, #2
00669ee4  00 00 93 15                                      ldrne r0, [r3]
00669ee8  1e ff 2f e1                                      bx lr


; FUNCTION 0x00669eec, declared_size=32, range_size=32, mode=arm
; class-group: glitch::collada::SAnimationAccessor
; alias: _ZNK6glitch7collada18SAnimationAccessor10getKeyTimeEi
; demangled: glitch::collada::SAnimationAccessor::getKeyTime(int) const
; decoder-mode: arm
00669eec  0c 00 90 e8                                      ldm r0, {r2, r3}
00669ef0  1c 00 a0 e3                                      mov r0, #0x1c
00669ef4  08 20 92 e5                                      ldr r2, [r2, #8]
00669ef8  90 21 22 e0                                      mla r2, r0, r1, r2
00669efc  0c 20 92 e5                                      ldr r2, [r2, #0xc]
00669f00  82 31 83 e0                                      add r3, r3, r2, lsl #3
00669f04  04 00 83 e2                                      add r0, r3, #4
00669f08  1e ff 2f e1                                      bx lr


; FUNCTION 0x00669f0c, declared_size=160, range_size=160, mode=arm
; class-group: glitch::collada::SAnimationAccessor
; alias: _ZNK6glitch7collada18SAnimationAccessor10getKeyTimeEii
; demangled: glitch::collada::SAnimationAccessor::getKeyTime(int, int) const
; decoder-mode: arm
00669f0c  70 40 2d e9                                      push {r4, r5, r6, lr}
00669f10  01 40 a0 e1                                      mov r4, r1
00669f14  00 10 a0 e3                                      mov r1, #0
00669f18  02 50 a0 e1                                      mov r5, r2
00669f1c  00 60 a0 e1                                      mov r6, r0
00669f20  d7 ff ff eb                                      bl #0x669e84
00669f24  03 00 50 e3                                      cmp r0, #3
00669f28  18 00 00 0a                                      beq #0x669f90
00669f2c  04 00 50 e3                                      cmp r0, #4
00669f30  10 00 00 0a                                      beq #0x669f78
00669f34  01 00 50 e3                                      cmp r0, #1
00669f38  01 00 00 0a                                      beq #0x669f44
00669f3c  00 00 a0 e3                                      mov r0, #0
00669f40  70 80 bd e8                                      pop {r4, r5, r6, pc}
00669f44  04 10 a0 e1                                      mov r1, r4
00669f48  06 00 a0 e1                                      mov r0, r6
00669f4c  e6 ff ff eb                                      bl #0x669eec
00669f50  04 30 90 e5                                      ldr r3, [r0, #4]
00669f54  05 00 d3 e7                                      ldrb r0, [r3, r5]
00669f58  74 93 f2 eb                                      bl #0x30ed30
00669f5c  ea 2a 05 e3                                      movw r2, #0x5aea
00669f60  aa 3a 0a e3                                      movw r3, #0xaaaa
00669f64  7b 2f 49 e3                                      movt r2, #0x9f7b
00669f68  40 30 44 e3                                      movt r3, #0x4040
00669f6c  d0 92 f2 eb                                      bl #0x30eab4
00669f70  ab 92 f2 eb                                      bl #0x30ea24
00669f74  70 80 bd e8                                      pop {r4, r5, r6, pc}
00669f78  06 00 a0 e1                                      mov r0, r6
00669f7c  04 10 a0 e1                                      mov r1, r4
00669f80  d9 ff ff eb                                      bl #0x669eec
00669f84  04 30 90 e5                                      ldr r3, [r0, #4]
00669f88  05 01 93 e7                                      ldr r0, [r3, r5, lsl #2]
00669f8c  70 80 bd e8                                      pop {r4, r5, r6, pc}
00669f90  04 10 a0 e1                                      mov r1, r4
00669f94  06 00 a0 e1                                      mov r0, r6
00669f98  d3 ff ff eb                                      bl #0x669eec
00669f9c  04 30 90 e5                                      ldr r3, [r0, #4]
00669fa0  85 50 a0 e1                                      lsl r5, r5, #1
00669fa4  b5 00 93 e1                                      ldrh r0, [r3, r5]
00669fa8  ea ff ff ea                                      b #0x669f58


; FUNCTION 0x00669fac, declared_size=8, range_size=8, mode=arm
; class-group: glitch::collada::SAnimationAccessor
; alias: _ZNK6glitch7collada18SAnimationAccessor8getStartEi
; demangled: glitch::collada::SAnimationAccessor::getStart(int) const
; decoder-mode: arm
00669fac  00 20 a0 e3                                      mov r2, #0
00669fb0  d5 ff ff ea                                      b #0x669f0c


; FUNCTION 0x00669fb4, declared_size=40, range_size=40, mode=arm
; class-group: glitch::collada::SAnimationAccessor
; alias: _ZNK6glitch7collada18SAnimationAccessor6getEndEi
; demangled: glitch::collada::SAnimationAccessor::getEnd(int) const
; decoder-mode: arm
00669fb4  70 40 2d e9                                      push {r4, r5, r6, lr}
00669fb8  00 40 a0 e1                                      mov r4, r0
00669fbc  01 50 a0 e1                                      mov r5, r1
00669fc0  c9 ff ff eb                                      bl #0x669eec
00669fc4  00 20 90 e5                                      ldr r2, [r0]
00669fc8  05 10 a0 e1                                      mov r1, r5
00669fcc  04 00 a0 e1                                      mov r0, r4
00669fd0  01 20 42 e2                                      sub r2, r2, #1
00669fd4  70 40 bd e8                                      pop {r4, r5, r6, lr}
00669fd8  cb ff ff ea                                      b #0x669f0c


; FUNCTION 0x00669fdc, declared_size=40, range_size=40, mode=arm
; class-group: glitch::collada::SAnimationAccessor
; alias: _ZNK6glitch7collada18SAnimationAccessor9getLengthEi
; demangled: glitch::collada::SAnimationAccessor::getLength(int) const
; decoder-mode: arm
00669fdc  70 40 2d e9                                      push {r4, r5, r6, lr}
00669fe0  00 60 a0 e1                                      mov r6, r0
00669fe4  01 50 a0 e1                                      mov r5, r1
00669fe8  f1 ff ff eb                                      bl #0x669fb4
00669fec  05 10 a0 e1                                      mov r1, r5
00669ff0  00 40 a0 e1                                      mov r4, r0
00669ff4  06 00 a0 e1                                      mov r0, r6
00669ff8  eb ff ff eb                                      bl #0x669fac
00669ffc  04 00 60 e0                                      rsb r0, r0, r4
0066a000  70 80 bd e8                                      pop {r4, r5, r6, pc}


; FUNCTION 0x0066a004, declared_size=12, range_size=12, mode=arm
; class-group: glitch::collada::SAnimationAccessor
; alias: _ZNK6glitch7collada18SAnimationAccessor11getAnimatorEv
; demangled: glitch::collada::SAnimationAccessor::getAnimator() const
; decoder-mode: arm
0066a004  00 30 90 e5                                      ldr r3, [r0]
0066a008  14 00 93 e5                                      ldr r0, [r3, #0x14]
0066a00c  1e ff 2f e1                                      bx lr


; FUNCTION 0x0066a1f0, declared_size=188, range_size=188, mode=arm
; class-group: bool glitch::collada::SAnimationAccessor
; alias: _ZNK6glitch7collada18SAnimationAccessor16findKeyFrameNoExIiLi1000EEEbiRKNS_3res6vectorIiEEiRi
; demangled: bool glitch::collada::SAnimationAccessor::findKeyFrameNoEx<int, 1000>(int, glitch::res::vector<int> const&, int, int&) const
; decoder-mode: arm
0066a1f0  f8 4f 2d e9                                      push {r3, r4, r5, r6, r7, r8, sb, sl, fp, lr}
0066a1f4  00 50 a0 e1                                      mov r5, r0
0066a1f8  03 00 a0 e1                                      mov r0, r3
0066a1fc  02 40 a0 e1                                      mov r4, r2
0066a200  01 b0 a0 e1                                      mov fp, r1
0066a204  d6 91 f2 eb                                      bl #0x30e964
0066a208  00 70 94 e5                                      ldr r7, [r4]
0066a20c  00 a0 a0 e1                                      mov sl, r0
0066a210  01 70 47 e2                                      sub r7, r7, #1
0066a214  00 00 57 e3                                      cmp r7, #0
0066a218  0d 00 00 da                                      ble #0x66a254
0066a21c  04 90 94 e5                                      ldr sb, [r4, #4]
0066a220  01 80 a0 e3                                      mov r8, #1
0066a224  07 60 88 e0                                      add r6, r8, r7
0066a228  c6 60 a0 e1                                      asr r6, r6, #1
0066a22c  06 01 99 e7                                      ldr r0, [sb, r6, lsl #2]
0066a230  cb 91 f2 eb                                      bl #0x30e964
0066a234  00 10 a0 e1                                      mov r1, r0
0066a238  0a 00 a0 e1                                      mov r0, sl
0066a23c  32 91 f2 eb                                      bl #0x30e70c
0066a240  00 00 50 e3                                      cmp r0, #0
0066a244  01 70 46 12                                      subne r7, r6, #1
0066a248  01 80 86 02                                      addeq r8, r6, #1
0066a24c  07 00 58 e1                                      cmp r8, r7
0066a250  f3 ff ff da                                      ble #0x66a224
0066a254  28 30 9d e5                                      ldr r3, [sp, #0x28]
0066a258  00 70 83 e5                                      str r7, [r3]
0066a25c  04 30 94 e5                                      ldr r3, [r4, #4]
0066a260  07 01 93 e7                                      ldr r0, [r3, r7, lsl #2]
0066a264  be 91 f2 eb                                      bl #0x30e964
0066a268  00 10 a0 e1                                      mov r1, r0
0066a26c  0a 00 a0 e1                                      mov r0, sl
0066a270  45 8f f2 eb                                      bl #0x30df8c
0066a274  00 00 50 e3                                      cmp r0, #0
0066a278  00 70 a0 13                                      movne r7, #0
0066a27c  03 00 00 1a                                      bne #0x66a290
0066a280  00 30 94 e5                                      ldr r3, [r4]
0066a284  01 30 43 e2                                      sub r3, r3, #1
0066a288  03 70 57 e0                                      subs r7, r7, r3
0066a28c  01 70 a0 13                                      movne r7, #1
0066a290  05 00 a0 e1                                      mov r0, r5
0066a294  0b 10 a0 e1                                      mov r1, fp
0066a298  ff fe ff eb                                      bl #0x669e9c
0066a29c  00 00 50 e3                                      cmp r0, #0
0066a2a0  00 00 a0 03                                      moveq r0, #0
0066a2a4  01 00 07 12                                      andne r0, r7, #1
0066a2a8  f8 8f bd e8                                      pop {r3, r4, r5, r6, r7, r8, sb, sl, fp, pc}


; FUNCTION 0x0066a2ac, declared_size=188, range_size=188, mode=arm
; class-group: bool glitch::collada::SAnimationAccessor
; alias: _ZNK6glitch7collada18SAnimationAccessor16findKeyFrameNoExIiLi1000EEEbiRKNS_3res6vectorIiEEiRiRf
; demangled: bool glitch::collada::SAnimationAccessor::findKeyFrameNoEx<int, 1000>(int, glitch::res::vector<int> const&, int, int&, float&) const
; decoder-mode: arm
0066a2ac  f0 4f 2d e9                                      push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0066a2b0  14 d0 4d e2                                      sub sp, sp, #0x14
0066a2b4  38 40 9d e5                                      ldr r4, [sp, #0x38]
0066a2b8  02 70 a0 e1                                      mov r7, r2
0066a2bc  03 60 a0 e1                                      mov r6, r3
0066a2c0  00 40 8d e5                                      str r4, [sp]
0066a2c4  3c 50 9d e5                                      ldr r5, [sp, #0x3c]
0066a2c8  c8 ff ff eb                                      bl #0x66a1f0
0066a2cc  00 80 50 e2                                      subs r8, r0, #0
0066a2d0  1b 00 00 0a                                      beq #0x66a344
0066a2d4  00 b0 94 e5                                      ldr fp, [r4]
0066a2d8  04 90 97 e5                                      ldr sb, [r7, #4]
0066a2dc  00 70 a0 e3                                      mov r7, #0
0066a2e0  fe a5 a0 e3                                      mov sl, #0x3f800000
0066a2e4  0b 01 99 e7                                      ldr r0, [sb, fp, lsl #2]
0066a2e8  9d 91 f2 eb                                      bl #0x30e964
0066a2ec  76 90 f2 eb                                      bl #0x30e4cc
0066a2f0  00 40 a0 e1                                      mov r4, r0
0066a2f4  06 00 60 e0                                      rsb r0, r0, r6
0066a2f8  99 91 f2 eb                                      bl #0x30e964
0066a2fc  01 b0 8b e2                                      add fp, fp, #1
0066a300  00 60 a0 e1                                      mov r6, r0
0066a304  0b 01 99 e7                                      ldr r0, [sb, fp, lsl #2]
0066a308  95 91 f2 eb                                      bl #0x30e964
0066a30c  6e 90 f2 eb                                      bl #0x30e4cc
0066a310  00 00 64 e0                                      rsb r0, r4, r0
0066a314  92 91 f2 eb                                      bl #0x30e964
0066a318  00 10 a0 e1                                      mov r1, r0
0066a31c  06 00 a0 e1                                      mov r0, r6
0066a320  5b 92 f2 eb                                      bl #0x30ec94
0066a324  07 10 a0 e1                                      mov r1, r7
0066a328  00 00 85 e5                                      str r0, [r5]
0066a32c  00 40 a0 e1                                      mov r4, r0
0066a330  f5 90 f2 eb                                      bl #0x30e70c
0066a334  00 00 50 e3                                      cmp r0, #0
0066a338  07 40 a0 11                                      movne r4, r7
0066a33c  03 00 00 0a                                      beq #0x66a350
0066a340  00 40 85 e5                                      str r4, [r5]
0066a344  08 00 a0 e1                                      mov r0, r8
0066a348  14 d0 8d e2                                      add sp, sp, #0x14
0066a34c  f0 8f bd e8                                      pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
0066a350  04 00 a0 e1                                      mov r0, r4
0066a354  0a 10 a0 e1                                      mov r1, sl
0066a358  eb 90 f2 eb                                      bl #0x30e70c
0066a35c  00 00 50 e3                                      cmp r0, #0
0066a360  0a 40 a0 01                                      moveq r4, sl
0066a364  f5 ff ff ea                                      b #0x66a340


; FUNCTION 0x0066a6d0, declared_size=184, range_size=184, mode=arm
; class-group: bool glitch::collada::SAnimationAccessor
; alias: _ZNK6glitch7collada18SAnimationAccessor14findKeyFrameNoIhLi30EEEbRKNS_3res6vectorIiEEiRi
; demangled: bool glitch::collada::SAnimationAccessor::findKeyFrameNo<unsigned char, 30>(glitch::res::vector<int> const&, int, int&) const
; decoder-mode: arm
0066a6d0  f8 4f 2d e9                                      push {r3, r4, r5, r6, r7, r8, sb, sl, fp, lr}
0066a6d4  02 00 a0 e1                                      mov r0, r2
0066a6d8  01 40 a0 e1                                      mov r4, r1
0066a6dc  03 b0 a0 e1                                      mov fp, r3
0066a6e0  9f 90 f2 eb                                      bl #0x30e964
0066a6e4  55 15 05 e3                                      movw r1, #0x5555
0066a6e8  05 12 44 e3                                      movt r1, #0x4205
0066a6ec  00 90 a0 e1                                      mov sb, r0
0066a6f0  67 91 f2 eb                                      bl #0x30ec94
0066a6f4  00 50 94 e5                                      ldr r5, [r4]
0066a6f8  00 80 a0 e1                                      mov r8, r0
0066a6fc  01 50 45 e2                                      sub r5, r5, #1
0066a700  00 00 55 e3                                      cmp r5, #0
0066a704  0c 00 00 da                                      ble #0x66a73c
0066a708  04 a0 94 e5                                      ldr sl, [r4, #4]
0066a70c  01 60 a0 e3                                      mov r6, #1
0066a710  05 70 86 e0                                      add r7, r6, r5
0066a714  c7 00 da e7                                      ldrb r0, [sl, r7, asr #1]
0066a718  91 90 f2 eb                                      bl #0x30e964
0066a71c  08 10 a0 e1                                      mov r1, r8
0066a720  f4 8e f2 eb                                      bl #0x30e2f8
0066a724  c7 70 a0 e1                                      asr r7, r7, #1
0066a728  00 00 50 e3                                      cmp r0, #0
0066a72c  01 50 47 12                                      subne r5, r7, #1
0066a730  01 60 87 02                                      addeq r6, r7, #1
0066a734  05 00 56 e1                                      cmp r6, r5
0066a738  f4 ff ff da                                      ble #0x66a710
0066a73c  00 50 8b e5                                      str r5, [fp]
0066a740  04 30 94 e5                                      ldr r3, [r4, #4]
0066a744  05 00 d3 e7                                      ldrb r0, [r3, r5]
0066a748  85 90 f2 eb                                      bl #0x30e964
0066a74c  55 15 05 e3                                      movw r1, #0x5555
0066a750  05 12 44 e3                                      movt r1, #0x4205
0066a754  84 91 f2 eb                                      bl #0x30ed6c
0066a758  00 10 a0 e1                                      mov r1, r0
0066a75c  09 00 a0 e1                                      mov r0, sb
0066a760  09 8e f2 eb                                      bl #0x30df8c
0066a764  00 00 50 e3                                      cmp r0, #0
0066a768  01 00 00 0a                                      beq #0x66a774
0066a76c  00 00 a0 e3                                      mov r0, #0
0066a770  f8 8f bd e8                                      pop {r3, r4, r5, r6, r7, r8, sb, sl, fp, pc}
0066a774  00 00 94 e5                                      ldr r0, [r4]
0066a778  01 00 40 e2                                      sub r0, r0, #1
0066a77c  00 00 55 e0                                      subs r0, r5, r0
0066a780  01 00 a0 13                                      movne r0, #1
0066a784  f8 8f bd e8                                      pop {r3, r4, r5, r6, r7, r8, sb, sl, fp, pc}


; FUNCTION 0x0066a788, declared_size=60, range_size=60, mode=arm
; class-group: bool glitch::collada::SAnimationAccessor
; alias: _ZNK6glitch7collada18SAnimationAccessor16findKeyFrameNoExIhLi30EEEbiRKNS_3res6vectorIiEEiRi
; demangled: bool glitch::collada::SAnimationAccessor::findKeyFrameNoEx<unsigned char, 30>(int, glitch::res::vector<int> const&, int, int&) const
; decoder-mode: arm
0066a788  70 40 2d e9                                      push {r4, r5, r6, lr}
0066a78c  01 40 a0 e1                                      mov r4, r1
0066a790  02 10 a0 e1                                      mov r1, r2
0066a794  03 20 a0 e1                                      mov r2, r3
0066a798  10 30 9d e5                                      ldr r3, [sp, #0x10]
0066a79c  00 50 a0 e1                                      mov r5, r0
0066a7a0  ca ff ff eb                                      bl #0x66a6d0
0066a7a4  04 10 a0 e1                                      mov r1, r4
0066a7a8  00 60 a0 e1                                      mov r6, r0
0066a7ac  05 00 a0 e1                                      mov r0, r5
0066a7b0  b9 fd ff eb                                      bl #0x669e9c
0066a7b4  00 00 50 e3                                      cmp r0, #0
0066a7b8  00 00 a0 03                                      moveq r0, #0
0066a7bc  01 00 06 12                                      andne r0, r6, #1
0066a7c0  70 80 bd e8                                      pop {r4, r5, r6, pc}


; FUNCTION 0x0066a7c4, declared_size=208, range_size=208, mode=arm
; class-group: bool glitch::collada::SAnimationAccessor
; alias: _ZNK6glitch7collada18SAnimationAccessor16findKeyFrameNoExIhLi30EEEbiRKNS_3res6vectorIiEEiRiRf
; demangled: bool glitch::collada::SAnimationAccessor::findKeyFrameNoEx<unsigned char, 30>(int, glitch::res::vector<int> const&, int, int&, float&) const
; decoder-mode: arm
0066a7c4  f0 47 2d e9                                      push {r4, r5, r6, r7, r8, sb, sl, lr}
0066a7c8  10 d0 4d e2                                      sub sp, sp, #0x10
0066a7cc  30 40 9d e5                                      ldr r4, [sp, #0x30]
0066a7d0  02 70 a0 e1                                      mov r7, r2
0066a7d4  03 60 a0 e1                                      mov r6, r3
0066a7d8  00 40 8d e5                                      str r4, [sp]
0066a7dc  34 50 9d e5                                      ldr r5, [sp, #0x34]
0066a7e0  e8 ff ff eb                                      bl #0x66a788
0066a7e4  00 80 50 e2                                      subs r8, r0, #0
0066a7e8  20 00 00 0a                                      beq #0x66a870
0066a7ec  00 30 94 e5                                      ldr r3, [r4]
0066a7f0  04 70 97 e5                                      ldr r7, [r7, #4]
0066a7f4  00 a0 a0 e3                                      mov sl, #0
0066a7f8  fe 95 a0 e3                                      mov sb, #0x3f800000
0066a7fc  03 00 f7 e7                                      ldrb r0, [r7, r3]!
0066a800  57 90 f2 eb                                      bl #0x30e964
0066a804  55 15 05 e3                                      movw r1, #0x5555
0066a808  05 12 44 e3                                      movt r1, #0x4205
0066a80c  56 91 f2 eb                                      bl #0x30ed6c
0066a810  2d 8f f2 eb                                      bl #0x30e4cc
0066a814  00 40 a0 e1                                      mov r4, r0
0066a818  06 00 60 e0                                      rsb r0, r0, r6
0066a81c  50 90 f2 eb                                      bl #0x30e964
0066a820  00 60 a0 e1                                      mov r6, r0
0066a824  01 00 d7 e5                                      ldrb r0, [r7, #1]
0066a828  4d 90 f2 eb                                      bl #0x30e964
0066a82c  55 15 05 e3                                      movw r1, #0x5555
0066a830  05 12 44 e3                                      movt r1, #0x4205
0066a834  4c 91 f2 eb                                      bl #0x30ed6c
0066a838  23 8f f2 eb                                      bl #0x30e4cc
0066a83c  00 00 64 e0                                      rsb r0, r4, r0
0066a840  47 90 f2 eb                                      bl #0x30e964
0066a844  00 10 a0 e1                                      mov r1, r0
0066a848  06 00 a0 e1                                      mov r0, r6
0066a84c  10 91 f2 eb                                      bl #0x30ec94
0066a850  0a 10 a0 e1                                      mov r1, sl
0066a854  00 00 85 e5                                      str r0, [r5]
0066a858  00 40 a0 e1                                      mov r4, r0
0066a85c  aa 8f f2 eb                                      bl #0x30e70c
0066a860  00 00 50 e3                                      cmp r0, #0
0066a864  0a 40 a0 11                                      movne r4, sl
0066a868  03 00 00 0a                                      beq #0x66a87c
0066a86c  00 40 85 e5                                      str r4, [r5]
0066a870  08 00 a0 e1                                      mov r0, r8
0066a874  10 d0 8d e2                                      add sp, sp, #0x10
0066a878  f0 87 bd e8                                      pop {r4, r5, r6, r7, r8, sb, sl, pc}
0066a87c  04 00 a0 e1                                      mov r0, r4
0066a880  09 10 a0 e1                                      mov r1, sb
0066a884  a0 8f f2 eb                                      bl #0x30e70c
0066a888  00 00 50 e3                                      cmp r0, #0
0066a88c  09 40 a0 01                                      moveq r4, sb
0066a890  f5 ff ff ea                                      b #0x66a86c


; FUNCTION 0x0066abb8, declared_size=192, range_size=192, mode=arm
; class-group: bool glitch::collada::SAnimationAccessor
; alias: _ZNK6glitch7collada18SAnimationAccessor14findKeyFrameNoItLi30EEEbRKNS_3res6vectorIiEEiRi
; demangled: bool glitch::collada::SAnimationAccessor::findKeyFrameNo<unsigned short, 30>(glitch::res::vector<int> const&, int, int&) const
; decoder-mode: arm
0066abb8  f8 4f 2d e9                                      push {r3, r4, r5, r6, r7, r8, sb, sl, fp, lr}
0066abbc  02 00 a0 e1                                      mov r0, r2
0066abc0  01 40 a0 e1                                      mov r4, r1
0066abc4  03 b0 a0 e1                                      mov fp, r3
0066abc8  65 8f f2 eb                                      bl #0x30e964
0066abcc  55 15 05 e3                                      movw r1, #0x5555
0066abd0  05 12 44 e3                                      movt r1, #0x4205
0066abd4  00 90 a0 e1                                      mov sb, r0
0066abd8  2d 90 f2 eb                                      bl #0x30ec94
0066abdc  00 60 94 e5                                      ldr r6, [r4]
0066abe0  00 80 a0 e1                                      mov r8, r0
0066abe4  01 60 46 e2                                      sub r6, r6, #1
0066abe8  00 00 56 e3                                      cmp r6, #0
0066abec  0d 00 00 da                                      ble #0x66ac28
0066abf0  04 a0 94 e5                                      ldr sl, [r4, #4]
0066abf4  01 70 a0 e3                                      mov r7, #1
0066abf8  06 50 87 e0                                      add r5, r7, r6
0066abfc  c5 50 a0 e1                                      asr r5, r5, #1
0066ac00  85 30 a0 e1                                      lsl r3, r5, #1
0066ac04  b3 00 9a e1                                      ldrh r0, [sl, r3]
0066ac08  55 8f f2 eb                                      bl #0x30e964
0066ac0c  08 10 a0 e1                                      mov r1, r8
0066ac10  b8 8d f2 eb                                      bl #0x30e2f8
0066ac14  00 00 50 e3                                      cmp r0, #0
0066ac18  01 60 45 12                                      subne r6, r5, #1
0066ac1c  01 70 85 02                                      addeq r7, r5, #1
0066ac20  06 00 57 e1                                      cmp r7, r6
0066ac24  f3 ff ff da                                      ble #0x66abf8
0066ac28  00 60 8b e5                                      str r6, [fp]
0066ac2c  04 20 94 e5                                      ldr r2, [r4, #4]
0066ac30  86 30 a0 e1                                      lsl r3, r6, #1
0066ac34  b3 00 92 e1                                      ldrh r0, [r2, r3]
0066ac38  49 8f f2 eb                                      bl #0x30e964
0066ac3c  55 15 05 e3                                      movw r1, #0x5555
0066ac40  05 12 44 e3                                      movt r1, #0x4205
0066ac44  48 90 f2 eb                                      bl #0x30ed6c
0066ac48  00 10 a0 e1                                      mov r1, r0
0066ac4c  09 00 a0 e1                                      mov r0, sb
0066ac50  cd 8c f2 eb                                      bl #0x30df8c
0066ac54  00 00 50 e3                                      cmp r0, #0
0066ac58  01 00 00 0a                                      beq #0x66ac64
0066ac5c  00 00 a0 e3                                      mov r0, #0
0066ac60  f8 8f bd e8                                      pop {r3, r4, r5, r6, r7, r8, sb, sl, fp, pc}
0066ac64  00 00 94 e5                                      ldr r0, [r4]
0066ac68  01 00 40 e2                                      sub r0, r0, #1
0066ac6c  00 00 56 e0                                      subs r0, r6, r0
0066ac70  01 00 a0 13                                      movne r0, #1
0066ac74  f8 8f bd e8                                      pop {r3, r4, r5, r6, r7, r8, sb, sl, fp, pc}


; FUNCTION 0x0066ac78, declared_size=60, range_size=60, mode=arm
; class-group: bool glitch::collada::SAnimationAccessor
; alias: _ZNK6glitch7collada18SAnimationAccessor16findKeyFrameNoExItLi30EEEbiRKNS_3res6vectorIiEEiRi
; demangled: bool glitch::collada::SAnimationAccessor::findKeyFrameNoEx<unsigned short, 30>(int, glitch::res::vector<int> const&, int, int&) const
; decoder-mode: arm
0066ac78  70 40 2d e9                                      push {r4, r5, r6, lr}
0066ac7c  01 40 a0 e1                                      mov r4, r1
0066ac80  02 10 a0 e1                                      mov r1, r2
0066ac84  03 20 a0 e1                                      mov r2, r3
0066ac88  10 30 9d e5                                      ldr r3, [sp, #0x10]
0066ac8c  00 50 a0 e1                                      mov r5, r0
0066ac90  c8 ff ff eb                                      bl #0x66abb8
0066ac94  04 10 a0 e1                                      mov r1, r4
0066ac98  00 60 a0 e1                                      mov r6, r0
0066ac9c  05 00 a0 e1                                      mov r0, r5
0066aca0  7d fc ff eb                                      bl #0x669e9c
0066aca4  00 00 50 e3                                      cmp r0, #0
0066aca8  00 00 a0 03                                      moveq r0, #0
0066acac  01 00 06 12                                      andne r0, r6, #1
0066acb0  70 80 bd e8                                      pop {r4, r5, r6, pc}


; FUNCTION 0x0066acb4, declared_size=216, range_size=216, mode=arm
; class-group: bool glitch::collada::SAnimationAccessor
; alias: _ZNK6glitch7collada18SAnimationAccessor16findKeyFrameNoExItLi30EEEbiRKNS_3res6vectorIiEEiRiRf
; demangled: bool glitch::collada::SAnimationAccessor::findKeyFrameNoEx<unsigned short, 30>(int, glitch::res::vector<int> const&, int, int&, float&) const
; decoder-mode: arm
0066acb4  f0 47 2d e9                                      push {r4, r5, r6, r7, r8, sb, sl, lr}
0066acb8  10 d0 4d e2                                      sub sp, sp, #0x10
0066acbc  30 40 9d e5                                      ldr r4, [sp, #0x30]
0066acc0  02 70 a0 e1                                      mov r7, r2
0066acc4  03 60 a0 e1                                      mov r6, r3
0066acc8  00 40 8d e5                                      str r4, [sp]
0066accc  34 50 9d e5                                      ldr r5, [sp, #0x34]
0066acd0  e8 ff ff eb                                      bl #0x66ac78
0066acd4  00 80 50 e2                                      subs r8, r0, #0
0066acd8  22 00 00 0a                                      beq #0x66ad68
0066acdc  00 a0 94 e5                                      ldr sl, [r4]
0066ace0  04 70 97 e5                                      ldr r7, [r7, #4]
0066ace4  00 90 a0 e3                                      mov sb, #0
0066ace8  8a 30 a0 e1                                      lsl r3, sl, #1
0066acec  b3 00 97 e1                                      ldrh r0, [r7, r3]
0066acf0  1b 8f f2 eb                                      bl #0x30e964
0066acf4  55 15 05 e3                                      movw r1, #0x5555
0066acf8  05 12 44 e3                                      movt r1, #0x4205
0066acfc  1a 90 f2 eb                                      bl #0x30ed6c
0066ad00  f1 8d f2 eb                                      bl #0x30e4cc
0066ad04  00 40 a0 e1                                      mov r4, r0
0066ad08  06 00 60 e0                                      rsb r0, r0, r6
0066ad0c  14 8f f2 eb                                      bl #0x30e964
0066ad10  8a 70 87 e0                                      add r7, r7, sl, lsl #1
0066ad14  00 60 a0 e1                                      mov r6, r0
0066ad18  b2 00 d7 e1                                      ldrh r0, [r7, #2]
0066ad1c  10 8f f2 eb                                      bl #0x30e964
0066ad20  55 15 05 e3                                      movw r1, #0x5555
0066ad24  05 12 44 e3                                      movt r1, #0x4205
0066ad28  0f 90 f2 eb                                      bl #0x30ed6c
0066ad2c  e6 8d f2 eb                                      bl #0x30e4cc
0066ad30  00 00 64 e0                                      rsb r0, r4, r0
0066ad34  0a 8f f2 eb                                      bl #0x30e964
0066ad38  00 10 a0 e1                                      mov r1, r0
0066ad3c  06 00 a0 e1                                      mov r0, r6
0066ad40  d3 8f f2 eb                                      bl #0x30ec94
0066ad44  09 10 a0 e1                                      mov r1, sb
0066ad48  00 00 85 e5                                      str r0, [r5]
0066ad4c  00 40 a0 e1                                      mov r4, r0
0066ad50  6d 8e f2 eb                                      bl #0x30e70c
0066ad54  00 00 50 e3                                      cmp r0, #0
0066ad58  fe 65 a0 e3                                      mov r6, #0x3f800000
0066ad5c  09 40 a0 11                                      movne r4, sb
0066ad60  03 00 00 0a                                      beq #0x66ad74
0066ad64  00 40 85 e5                                      str r4, [r5]
0066ad68  08 00 a0 e1                                      mov r0, r8
0066ad6c  10 d0 8d e2                                      add sp, sp, #0x10
0066ad70  f0 87 bd e8                                      pop {r4, r5, r6, r7, r8, sb, sl, pc}
0066ad74  04 00 a0 e1                                      mov r0, r4
0066ad78  06 10 a0 e1                                      mov r1, r6
0066ad7c  62 8e f2 eb                                      bl #0x30e70c
0066ad80  00 00 50 e3                                      cmp r0, #0
0066ad84  06 40 a0 01                                      moveq r4, r6
0066ad88  f5 ff ff ea                                      b #0x66ad64


; FUNCTION 0x006bccf0, declared_size=656, range_size=656, mode=arm
; class-group: glitch::scene
; alias: _ZN6glitch5scene12_GLOBAL__N_19addStreamEPNS_5video12IVideoDriverERNS_7collada5SMeshERNS5_11SMeshBufferEcPNS2_17SVertexStreamDataEhRKNS5_13SBufferConfigE
; demangled: glitch::scene::(anonymous namespace)::addStream(glitch::video::IVideoDriver*, glitch::collada::SMesh&, glitch::collada::SMeshBuffer&, char, glitch::video::SVertexStreamData*, unsigned char, glitch::collada::SBufferConfig const&)
; decoder-mode: arm
006bccf0  f0 4f 2d e9                                      push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
006bccf4  01 50 a0 e1                                      mov r5, r1
006bccf8  00 10 91 e5                                      ldr r1, [r1]
006bccfc  74 62 9f e5                                      ldr r6, [pc, #0x274]
006bcd00  24 d0 4d e2                                      sub sp, sp, #0x24
006bcd04  00 00 51 e3                                      cmp r1, #0
006bcd08  06 60 8f e0                                      add r6, pc, r6
006bcd0c  00 c0 a0 e1                                      mov ip, r0
006bcd10  03 80 a0 e1                                      mov r8, r3
006bcd14  48 a0 9d e5                                      ldr sl, [sp, #0x48]
006bcd18  4c 70 dd e5                                      ldrb r7, [sp, #0x4c]
006bcd1c  38 00 00 0a                                      beq #0x6bce04
006bcd20  08 20 95 e5                                      ldr r2, [r5, #8]
006bcd24  28 40 92 e5                                      ldr r4, [r2, #0x28]
006bcd28  00 00 54 e3                                      cmp r4, #0
006bcd2c  23 00 00 0a                                      beq #0x6bcdc0
006bcd30  04 30 94 e5                                      ldr r3, [r4, #4]
006bcd34  01 30 83 e2                                      add r3, r3, #1
006bcd38  04 30 84 e5                                      str r3, [r4, #4]
006bcd3c  00 c0 95 e5                                      ldr ip, [r5]
006bcd40  00 00 5c e3                                      cmp ip, #0
006bcd44  1c 00 00 1a                                      bne #0x6bcdbc
006bcd48  14 30 a0 e3                                      mov r3, #0x14
006bcd4c  08 20 95 e5                                      ldr r2, [r5, #8]
006bcd50  93 08 08 e0                                      mul r8, r3, r8
006bcd54  20 12 9f e5                                      ldr r1, [pc, #0x220]
006bcd58  08 30 92 e7                                      ldr r3, [r2, r8]
006bcd5c  08 80 82 e0                                      add r8, r2, r8
006bcd60  01 10 96 e7                                      ldr r1, [r6, r1]
006bcd64  04 20 98 e5                                      ldr r2, [r8, #4]
006bcd68  00 00 54 e3                                      cmp r4, #0
006bcd6c  03 00 d1 e7                                      ldrb r0, [r1, r3]
006bcd70  72 10 ff e6                                      uxth r1, r2
006bcd74  07 42 8a e7                                      str r4, [sl, r7, lsl #4]
006bcd78  92 00 02 e0                                      mul r2, r2, r0
006bcd7c  07 a2 8a e0                                      add sl, sl, r7, lsl #4
006bcd80  72 20 ff e6                                      uxth r2, r2
006bcd84  19 00 00 0a                                      beq #0x6bcdf0
006bcd88  04 e0 94 e5                                      ldr lr, [r4, #4]
006bcd8c  04 00 a0 e1                                      mov r0, r4
006bcd90  01 e0 8e e2                                      add lr, lr, #1
006bcd94  04 e0 84 e5                                      str lr, [r4, #4]
006bcd98  be 20 ca e1                                      strh r2, [sl, #0xe]
006bcd9c  04 c0 8a e5                                      str ip, [sl, #4]
006bcda0  08 30 8a e5                                      str r3, [sl, #8]
006bcda4  bc 10 ca e1                                      strh r1, [sl, #0xc]
006bcda8  f5 81 f1 eb                                      bl #0x31d584
006bcdac  01 00 87 e2                                      add r0, r7, #1
006bcdb0  70 00 ef e6                                      uxtb r0, r0
006bcdb4  24 d0 8d e2                                      add sp, sp, #0x24
006bcdb8  f0 8f bd e8                                      pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
006bcdbc  08 20 95 e5                                      ldr r2, [r5, #8]
006bcdc0  18 10 92 e5                                      ldr r1, [r2, #0x18]
006bcdc4  08 00 92 e5                                      ldr r0, [r2, #8]
006bcdc8  10 30 92 e5                                      ldr r3, [r2, #0x10]
006bcdcc  08 11 91 e7                                      ldr r1, [r1, r8, lsl #2]
006bcdd0  00 00 54 e3                                      cmp r4, #0
006bcdd4  08 c1 90 e7                                      ldr ip, [r0, r8, lsl #2]
006bcdd8  08 31 93 e7                                      ldr r3, [r3, r8, lsl #2]
006bcddc  b0 20 d2 e1                                      ldrh r2, [r2]
006bcde0  71 10 ff e6                                      uxth r1, r1
006bcde4  07 42 8a e7                                      str r4, [sl, r7, lsl #4]
006bcde8  07 a2 8a e0                                      add sl, sl, r7, lsl #4
006bcdec  e5 ff ff 1a                                      bne #0x6bcd88
006bcdf0  be 20 ca e1                                      strh r2, [sl, #0xe]
006bcdf4  04 c0 8a e5                                      str ip, [sl, #4]
006bcdf8  08 30 8a e5                                      str r3, [sl, #8]
006bcdfc  bc 10 ca e1                                      strh r1, [sl, #0xc]
006bce00  e9 ff ff ea                                      b #0x6bcdac
006bce04  14 10 a0 e3                                      mov r1, #0x14
006bce08  08 30 95 e5                                      ldr r3, [r5, #8]
006bce0c  91 08 09 e0                                      mul sb, r1, r8
006bce10  09 b0 83 e0                                      add fp, r3, sb
006bce14  10 40 9b e5                                      ldr r4, [fp, #0x10]
006bce18  00 00 54 e3                                      cmp r4, #0
006bce1c  1a 00 00 0a                                      beq #0x6bce8c
006bce20  50 30 9d e5                                      ldr r3, [sp, #0x50]
006bce24  00 b0 93 e5                                      ldr fp, [r3]
006bce28  11 30 d4 e5                                      ldrb r3, [r4, #0x11]
006bce2c  03 00 5b e1                                      cmp fp, r3
006bce30  0e 00 00 0a                                      beq #0x6bce70
006bce34  12 30 d4 e5                                      ldrb r3, [r4, #0x12]
006bce38  08 00 13 e3                                      tst r3, #8
006bce3c  48 00 00 1a                                      bne #0x6bcf64
006bce40  7b b0 ef e6                                      uxtb fp, fp
006bce44  04 00 5b e3                                      cmp fp, #4
006bce48  11 b0 c4 e5                                      strb fp, [r4, #0x11]
006bce4c  04 00 00 0a                                      beq #0x6bce64
006bce50  08 30 94 e5                                      ldr r3, [r4, #8]
006bce54  00 00 53 e3                                      cmp r3, #0
006bce58  12 30 d4 15                                      ldrbne r3, [r4, #0x12]
006bce5c  02 30 83 13                                      orrne r3, r3, #2
006bce60  12 30 c4 15                                      strbne r3, [r4, #0x12]
006bce64  08 30 95 e5                                      ldr r3, [r5, #8]
006bce68  09 30 83 e0                                      add r3, r3, sb
006bce6c  10 40 93 e5                                      ldr r4, [r3, #0x10]
006bce70  50 20 9d e5                                      ldr r2, [sp, #0x50]
006bce74  04 30 d2 e5                                      ldrb r3, [r2, #4]
006bce78  00 00 53 e3                                      cmp r3, #0
006bce7c  27 00 00 1a                                      bne #0x6bcf20
006bce80  00 00 54 e3                                      cmp r4, #0
006bce84  a9 ff ff 1a                                      bne #0x6bcd30
006bce88  ab ff ff ea                                      b #0x6bcd3c
006bce8c  dc 00 d2 e1                                      ldrsb r0, [r2, #0xc]
006bce90  56 25 05 e3                                      movw r2, #0x5556
006bce94  55 25 45 e3                                      movt r2, #0x5555
006bce98  91 30 21 e0                                      mla r1, r1, r0, r3
006bce9c  d8 00 9f e5                                      ldr r0, [pc, #0xd8]
006bcea0  08 10 91 e5                                      ldr r1, [r1, #8]
006bcea4  09 30 93 e7                                      ldr r3, [r3, sb]
006bcea8  00 00 96 e7                                      ldr r0, [r6, r0]
006bceac  92 e1 c2 e0                                      smull lr, r2, r2, r1
006bceb0  04 e0 9b e5                                      ldr lr, [fp, #4]
006bceb4  c1 2f 42 e0                                      sub r2, r2, r1, asr #31
006bceb8  03 00 d0 e7                                      ldrb r0, [r0, r3]
006bcebc  9e 02 0e e0                                      mul lr, lr, r2
006bcec0  50 20 9d e5                                      ldr r2, [sp, #0x50]
006bcec4  90 0e 0e e0                                      mul lr, r0, lr
006bcec8  00 30 92 e5                                      ldr r3, [r2]
006bcecc  1c 20 8d e2                                      add r2, sp, #0x1c
006bced0  14 20 8d e5                                      str r2, [sp, #0x14]
006bced4  00 e0 8d e5                                      str lr, [sp]
006bced8  0c 10 9b e5                                      ldr r1, [fp, #0xc]
006bcedc  08 40 8d e5                                      str r4, [sp, #8]
006bcee0  04 20 a0 e1                                      mov r2, r4
006bcee4  04 10 8d e5                                      str r1, [sp, #4]
006bcee8  14 00 9d e5                                      ldr r0, [sp, #0x14]
006bceec  0c 10 a0 e1                                      mov r1, ip
006bcef0  00 c0 9c e5                                      ldr ip, [ip]
006bcef4  0f e0 a0 e1                                      mov lr, pc
006bcef8  78 f0 9c e5                                      ldr pc, [ip, #0x78]
006bcefc  14 10 9d e5                                      ldr r1, [sp, #0x14]
006bcf00  10 00 8b e2                                      add r0, fp, #0x10
006bcf04  60 fe ff eb                                      bl #0x6bc88c
006bcf08  14 00 9d e5                                      ldr r0, [sp, #0x14]
006bcf0c  1e eb fd eb                                      bl #0x637b8c
006bcf10  08 30 95 e5                                      ldr r3, [r5, #8]
006bcf14  09 30 83 e0                                      add r3, r3, sb
006bcf18  10 40 93 e5                                      ldr r4, [r3, #0x10]
006bcf1c  d3 ff ff ea                                      b #0x6bce70
006bcf20  12 30 d4 e5                                      ldrb r3, [r4, #0x12]
006bcf24  05 10 d2 e5                                      ldrb r1, [r2, #5]
006bcf28  08 00 13 e3                                      tst r3, #8
006bcf2c  01 00 00 0a                                      beq #0x6bcf38
006bcf30  02 00 13 e3                                      tst r3, #2
006bcf34  d1 ff ff 0a                                      beq #0x6bce80
006bcf38  11 30 d4 e5                                      ldrb r3, [r4, #0x11]
006bcf3c  04 00 53 e3                                      cmp r3, #4
006bcf40  ce ff ff 0a                                      beq #0x6bce80
006bcf44  00 30 94 e5                                      ldr r3, [r4]
006bcf48  04 00 a0 e1                                      mov r0, r4
006bcf4c  0f e0 a0 e1                                      mov lr, pc
006bcf50  0c f0 93 e5                                      ldr pc, [r3, #0xc]
006bcf54  08 30 95 e5                                      ldr r3, [r5, #8]
006bcf58  09 90 83 e0                                      add sb, r3, sb
006bcf5c  10 40 99 e5                                      ldr r4, [sb, #0x10]
006bcf60  c6 ff ff ea                                      b #0x6bce80
006bcf64  00 30 94 e5                                      ldr r3, [r4]
006bcf68  04 00 a0 e1                                      mov r0, r4
006bcf6c  0f e0 a0 e1                                      mov lr, pc
006bcf70  10 f0 93 e5                                      ldr pc, [r3, #0x10]
006bcf74  b1 ff ff ea                                      b #0x6bce40
; mapping-symbol data/literal pool
006bcf78  88 7d 2d 00 08 11 00 00                          .byte 0x88, 0x7d, 0x2d, 0x00, 0x08, 0x11, 0x00, 0x00


; FUNCTION 0x006bcf80, declared_size=2680, range_size=2680, mode=arm
; class-group: glitch::scene::CMeshBuffer
; alias: _ZN6glitch5scene11CMeshBufferC2EPNS_5video12IVideoDriverERNS_7collada5SMeshEjRKNS5_13SBufferConfigESA_b
; demangled: glitch::scene::CMeshBuffer::CMeshBuffer(glitch::video::IVideoDriver*, glitch::collada::SMesh&, unsigned int, glitch::collada::SBufferConfig const&, glitch::collada::SBufferConfig const&, bool)
; decoder-mode: arm
006bcf80  64 ca 9f e5                                      ldr ip, [pc, #0xa64]
006bcf84  f0 4f 2d e9                                      push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
006bcf88  60 ea 9f e5                                      ldr lr, [pc, #0xa60]
006bcf8c  0c c0 8f e0                                      add ip, pc, ip
006bcf90  00 40 a0 e1                                      mov r4, r0
006bcf94  0e e0 9c e7                                      ldr lr, [ip, lr]
006bcf98  00 00 a0 e3                                      mov r0, #0
006bcf9c  14 00 84 e5                                      str r0, [r4, #0x14]
006bcfa0  08 e0 8e e2                                      add lr, lr, #8
006bcfa4  00 e0 84 e5                                      str lr, [r4]
006bcfa8  04 00 84 e5                                      str r0, [r4, #4]
006bcfac  08 00 84 e5                                      str r0, [r4, #8]
006bcfb0  0c 00 84 e5                                      str r0, [r4, #0xc]
006bcfb4  10 00 84 e5                                      str r0, [r4, #0x10]
006bcfb8  38 50 a0 e3                                      mov r5, #0x38
006bcfbc  95 03 05 e0                                      mul r5, r5, r3
006bcfc0  02 60 a0 e1                                      mov r6, r2
006bcfc4  10 20 92 e5                                      ldr r2, [r2, #0x10]
006bcfc8  01 90 a0 e1                                      mov sb, r1
006bcfcc  20 3a 9f e5                                      ldr r3, [pc, #0xa20]
006bcfd0  05 10 82 e0                                      add r1, r2, r5
006bcfd4  24 00 91 e5                                      ldr r0, [r1, #0x24]
006bcfd8  05 20 92 e7                                      ldr r2, [r2, r5]
006bcfdc  44 d0 4d e2                                      sub sp, sp, #0x44
006bcfe0  03 30 8f e0                                      add r3, pc, r3
006bcfe4  01 08 50 e3                                      cmp r0, #0x10000
006bcfe8  02 e1 93 e7                                      ldr lr, [r3, r2, lsl #2]
006bcfec  6c 70 9d e5                                      ldr r7, [sp, #0x6c]
006bcff0  70 80 dd e5                                      ldrb r8, [sp, #0x70]
006bcff4  5c 01 00 ba                                      blt #0x6bd56c
006bcff8  28 c0 91 e5                                      ldr ip, [r1, #0x28]
006bcffc  02 20 a0 e3                                      mov r2, #2
006bd000  30 30 91 e5                                      ldr r3, [r1, #0x30]
006bd004  20 10 91 e5                                      ldr r1, [r1, #0x20]
006bd008  01 00 80 e2                                      add r0, r0, #1
006bd00c  00 00 53 e3                                      cmp r3, #0
006bd010  18 30 84 e5                                      str r3, [r4, #0x18]
006bd014  04 a0 93 15                                      ldrne sl, [r3, #4]
006bd018  01 a0 8a 12                                      addne sl, sl, #1
006bd01c  04 a0 83 15                                      strne sl, [r3, #4]
006bd020  00 30 a0 e3                                      mov r3, #0
006bd024  20 c0 84 e5                                      str ip, [r4, #0x20]
006bd028  24 10 84 e5                                      str r1, [r4, #0x24]
006bd02c  28 00 84 e5                                      str r0, [r4, #0x28]
006bd030  bc 22 c4 e1                                      strh r2, [r4, #0x2c]
006bd034  be e2 c4 e1                                      strh lr, [r4, #0x2e]
006bd038  34 30 c4 e5                                      strb r3, [r4, #0x34]
006bd03c  1c 30 84 e5                                      str r3, [r4, #0x1c]
006bd040  30 30 84 e5                                      str r3, [r4, #0x30]
006bd044  10 30 96 e5                                      ldr r3, [r6, #0x10]
006bd048  05 50 83 e0                                      add r5, r3, r5
006bd04c  30 a0 95 e5                                      ldr sl, [r5, #0x30]
006bd050  00 00 5a e3                                      cmp sl, #0
006bd054  51 01 00 0a                                      beq #0x6bd5a0
006bd058  00 b0 97 e5                                      ldr fp, [r7]
006bd05c  11 30 da e5                                      ldrb r3, [sl, #0x11]
006bd060  03 00 5b e1                                      cmp fp, r3
006bd064  0b 00 00 0a                                      beq #0x6bd098
006bd068  12 30 da e5                                      ldrb r3, [sl, #0x12]
006bd06c  08 00 13 e3                                      tst r3, #8
006bd070  85 01 00 1a                                      bne #0x6bd68c
006bd074  7b b0 ef e6                                      uxtb fp, fp
006bd078  04 00 5b e3                                      cmp fp, #4
006bd07c  11 b0 ca e5                                      strb fp, [sl, #0x11]
006bd080  04 00 00 0a                                      beq #0x6bd098
006bd084  08 30 9a e5                                      ldr r3, [sl, #8]
006bd088  00 00 53 e3                                      cmp r3, #0
006bd08c  12 30 da 15                                      ldrbne r3, [sl, #0x12]
006bd090  02 30 83 13                                      orrne r3, r3, #2
006bd094  12 30 ca 15                                      strbne r3, [sl, #0x12]
006bd098  04 30 d7 e5                                      ldrb r3, [r7, #4]
006bd09c  00 00 53 e3                                      cmp r3, #0
006bd0a0  6c 01 00 1a                                      bne #0x6bd658
006bd0a4  00 30 96 e5                                      ldr r3, [r6]
006bd0a8  00 00 53 e3                                      cmp r3, #0
006bd0ac  18 00 00 0a                                      beq #0x6bd114
006bd0b0  08 70 96 e5                                      ldr r7, [r6, #8]
006bd0b4  28 a0 97 e5                                      ldr sl, [r7, #0x28]
006bd0b8  00 00 5a e3                                      cmp sl, #0
006bd0bc  b0 01 00 0a                                      beq #0x6bd784
006bd0c0  68 00 9d e5                                      ldr r0, [sp, #0x68]
006bd0c4  11 30 da e5                                      ldrb r3, [sl, #0x11]
006bd0c8  00 70 90 e5                                      ldr r7, [r0]
006bd0cc  03 00 57 e1                                      cmp r7, r3
006bd0d0  0b 00 00 0a                                      beq #0x6bd104
006bd0d4  12 30 da e5                                      ldrb r3, [sl, #0x12]
006bd0d8  08 00 13 e3                                      tst r3, #8
006bd0dc  f9 01 00 1a                                      bne #0x6bd8c8
006bd0e0  77 70 ef e6                                      uxtb r7, r7
006bd0e4  04 00 57 e3                                      cmp r7, #4
006bd0e8  11 70 ca e5                                      strb r7, [sl, #0x11]
006bd0ec  04 00 00 0a                                      beq #0x6bd104
006bd0f0  08 30 9a e5                                      ldr r3, [sl, #8]
006bd0f4  00 00 53 e3                                      cmp r3, #0
006bd0f8  12 30 da 15                                      ldrbne r3, [sl, #0x12]
006bd0fc  02 30 83 13                                      orrne r3, r3, #2
006bd100  12 30 ca 15                                      strbne r3, [sl, #0x12]
006bd104  68 10 9d e5                                      ldr r1, [sp, #0x68]
006bd108  04 30 d1 e5                                      ldrb r3, [r1, #4]
006bd10c  00 00 53 e3                                      cmp r3, #0
006bd110  d9 01 00 1a                                      bne #0x6bd87c
006bd114  1e 0e a0 e3                                      mov r0, #0x1e0
006bd118  35 dd f9 eb                                      bl #0x5345f4
006bd11c  68 80 9d e5                                      ldr r8, [sp, #0x68]
006bd120  dc 30 d5 e1                                      ldrsb r3, [r5, #0xc]
006bd124  00 70 a0 e1                                      mov r7, r0
006bd128  00 b0 a0 e3                                      mov fp, #0
006bd12c  09 00 a0 e1                                      mov r0, sb
006bd130  06 10 a0 e1                                      mov r1, r6
006bd134  05 20 a0 e1                                      mov r2, r5
006bd138  08 80 8d e5                                      str r8, [sp, #8]
006bd13c  80 08 8d e8                                      stm sp, {r7, fp}
006bd140  ea fe ff eb                                      bl #0x6bccf0
006bd144  01 a0 a0 e3                                      mov sl, #1
006bd148  00 80 a0 e1                                      mov r8, r0
006bd14c  14 50 8d e5                                      str r5, [sp, #0x14]
006bd150  10 50 8d e5                                      str r5, [sp, #0x10]
006bd154  10 00 9d e5                                      ldr r0, [sp, #0x10]
006bd158  06 10 a0 e1                                      mov r1, r6
006bd15c  05 20 a0 e1                                      mov r2, r5
006bd160  d0 c1 d0 e1                                      ldrsb ip, [r0, #0x10]
006bd164  09 00 a0 e1                                      mov r0, sb
006bd168  00 00 5c e3                                      cmp ip, #0
006bd16c  0c 30 a0 e1                                      mov r3, ip
006bd170  0d 00 00 ba                                      blt #0x6bd1ac
006bd174  04 80 8d e5                                      str r8, [sp, #4]
006bd178  68 80 9d e5                                      ldr r8, [sp, #0x68]
006bd17c  02 c0 a0 e3                                      mov ip, #2
006bd180  1c ab 8a e1                                      orr sl, sl, ip, lsl fp
006bd184  08 80 8d e5                                      str r8, [sp, #8]
006bd188  00 70 8d e5                                      str r7, [sp]
006bd18c  d7 fe ff eb                                      bl #0x6bccf0
006bd190  10 c0 9d e5                                      ldr ip, [sp, #0x10]
006bd194  01 b0 8b e2                                      add fp, fp, #1
006bd198  04 00 5b e3                                      cmp fp, #4
006bd19c  01 c0 8c e2                                      add ip, ip, #1
006bd1a0  00 80 a0 e1                                      mov r8, r0
006bd1a4  10 c0 8d e5                                      str ip, [sp, #0x10]
006bd1a8  e9 ff ff 1a                                      bne #0x6bd154
006bd1ac  dd 30 d5 e1                                      ldrsb r3, [r5, #0xd]
006bd1b0  00 00 53 e3                                      cmp r3, #0
006bd1b4  09 00 00 ba                                      blt #0x6bd1e0
006bd1b8  04 80 8d e5                                      str r8, [sp, #4]
006bd1bc  68 80 9d e5                                      ldr r8, [sp, #0x68]
006bd1c0  09 00 a0 e1                                      mov r0, sb
006bd1c4  06 10 a0 e1                                      mov r1, r6
006bd1c8  05 20 a0 e1                                      mov r2, r5
006bd1cc  08 80 8d e5                                      str r8, [sp, #8]
006bd1d0  00 70 8d e5                                      str r7, [sp]
006bd1d4  c5 fe ff eb                                      bl #0x6bccf0
006bd1d8  02 a8 8a e3                                      orr sl, sl, #0x20000
006bd1dc  00 80 a0 e1                                      mov r8, r0
006bd1e0  de 30 d5 e1                                      ldrsb r3, [r5, #0xe]
006bd1e4  00 00 53 e3                                      cmp r3, #0
006bd1e8  07 00 00 ba                                      blt #0x6bd20c
006bd1ec  68 c0 9d e5                                      ldr ip, [sp, #0x68]
006bd1f0  09 00 a0 e1                                      mov r0, sb
006bd1f4  06 10 a0 e1                                      mov r1, r6
006bd1f8  05 20 a0 e1                                      mov r2, r5
006bd1fc  80 11 8d e8                                      stm sp, {r7, r8, ip}
006bd200  ba fe ff eb                                      bl #0x6bccf0
006bd204  01 a7 8a e3                                      orr sl, sl, #0x40000
006bd208  00 80 a0 e1                                      mov r8, r0
006bd20c  df 30 d5 e1                                      ldrsb r3, [r5, #0xf]
006bd210  00 00 53 e3                                      cmp r3, #0
006bd214  09 00 00 ba                                      blt #0x6bd240
006bd218  04 80 8d e5                                      str r8, [sp, #4]
006bd21c  68 80 9d e5                                      ldr r8, [sp, #0x68]
006bd220  09 00 a0 e1                                      mov r0, sb
006bd224  06 10 a0 e1                                      mov r1, r6
006bd228  05 20 a0 e1                                      mov r2, r5
006bd22c  08 80 8d e5                                      str r8, [sp, #8]
006bd230  00 70 8d e5                                      str r7, [sp]
006bd234  ad fe ff eb                                      bl #0x6bccf0
006bd238  02 a7 8a e3                                      orr sl, sl, #0x80000
006bd23c  00 80 a0 e1                                      mov r8, r0
006bd240  10 50 8d e5                                      str r5, [sp, #0x10]
006bd244  00 b0 a0 e3                                      mov fp, #0
006bd248  10 00 9d e5                                      ldr r0, [sp, #0x10]
006bd24c  06 10 a0 e1                                      mov r1, r6
006bd250  05 20 a0 e1                                      mov r2, r5
006bd254  d8 c1 d0 e1                                      ldrsb ip, [r0, #0x18]
006bd258  09 00 a0 e1                                      mov r0, sb
006bd25c  00 00 5c e3                                      cmp ip, #0
006bd260  0c 30 a0 e1                                      mov r3, ip
006bd264  0d 00 00 ba                                      blt #0x6bd2a0
006bd268  04 80 8d e5                                      str r8, [sp, #4]
006bd26c  68 80 9d e5                                      ldr r8, [sp, #0x68]
006bd270  01 c6 a0 e3                                      mov ip, #0x100000
006bd274  1c ab 8a e1                                      orr sl, sl, ip, lsl fp
006bd278  08 80 8d e5                                      str r8, [sp, #8]
006bd27c  00 70 8d e5                                      str r7, [sp]
006bd280  9a fe ff eb                                      bl #0x6bccf0
006bd284  10 c0 9d e5                                      ldr ip, [sp, #0x10]
006bd288  01 b0 8b e2                                      add fp, fp, #1
006bd28c  04 00 5b e3                                      cmp fp, #4
006bd290  01 c0 8c e2                                      add ip, ip, #1
006bd294  00 80 a0 e1                                      mov r8, r0
006bd298  10 c0 8d e5                                      str ip, [sp, #0x10]
006bd29c  e9 ff ff 1a                                      bne #0x6bd248
006bd2a0  10 50 8d e5                                      str r5, [sp, #0x10]
006bd2a4  00 b0 a0 e3                                      mov fp, #0
006bd2a8  10 00 9d e5                                      ldr r0, [sp, #0x10]
006bd2ac  06 10 a0 e1                                      mov r1, r6
006bd2b0  05 20 a0 e1                                      mov r2, r5
006bd2b4  d4 c1 d0 e1                                      ldrsb ip, [r0, #0x14]
006bd2b8  09 00 a0 e1                                      mov r0, sb
006bd2bc  00 00 5c e3                                      cmp ip, #0
006bd2c0  0c 30 a0 e1                                      mov r3, ip
006bd2c4  0d 00 00 ba                                      blt #0x6bd300
006bd2c8  04 80 8d e5                                      str r8, [sp, #4]
006bd2cc  68 80 9d e5                                      ldr r8, [sp, #0x68]
006bd2d0  01 c4 a0 e3                                      mov ip, #0x1000000
006bd2d4  1c ab 8a e1                                      orr sl, sl, ip, lsl fp
006bd2d8  08 80 8d e5                                      str r8, [sp, #8]
006bd2dc  00 70 8d e5                                      str r7, [sp]
006bd2e0  82 fe ff eb                                      bl #0x6bccf0
006bd2e4  10 c0 9d e5                                      ldr ip, [sp, #0x10]
006bd2e8  01 b0 8b e2                                      add fp, fp, #1
006bd2ec  04 00 5b e3                                      cmp fp, #4
006bd2f0  01 c0 8c e2                                      add ip, ip, #1
006bd2f4  00 80 a0 e1                                      mov r8, r0
006bd2f8  10 c0 8d e5                                      str ip, [sp, #0x10]
006bd2fc  e9 ff ff 1a                                      bne #0x6bd2a8
006bd300  dc 31 d5 e1                                      ldrsb r3, [r5, #0x1c]
006bd304  00 00 53 e3                                      cmp r3, #0
006bd308  09 00 00 ba                                      blt #0x6bd334
006bd30c  04 80 8d e5                                      str r8, [sp, #4]
006bd310  68 80 9d e5                                      ldr r8, [sp, #0x68]
006bd314  09 00 a0 e1                                      mov r0, sb
006bd318  06 10 a0 e1                                      mov r1, r6
006bd31c  05 20 a0 e1                                      mov r2, r5
006bd320  08 80 8d e5                                      str r8, [sp, #8]
006bd324  00 70 8d e5                                      str r7, [sp]
006bd328  70 fe ff eb                                      bl #0x6bccf0
006bd32c  01 a2 8a e3                                      orr sl, sl, #0x10000000
006bd330  00 80 a0 e1                                      mov r8, r0
006bd334  dd 31 d5 e1                                      ldrsb r3, [r5, #0x1d]
006bd338  00 00 53 e3                                      cmp r3, #0
006bd33c  07 00 00 ba                                      blt #0x6bd360
006bd340  68 c0 9d e5                                      ldr ip, [sp, #0x68]
006bd344  09 00 a0 e1                                      mov r0, sb
006bd348  06 10 a0 e1                                      mov r1, r6
006bd34c  05 20 a0 e1                                      mov r2, r5
006bd350  80 11 8d e8                                      stm sp, {r7, r8, ip}
006bd354  65 fe ff eb                                      bl #0x6bccf0
006bd358  02 a2 8a e3                                      orr sl, sl, #0x20000000
006bd35c  00 80 a0 e1                                      mov r8, r0
006bd360  0a 10 a0 e1                                      mov r1, sl
006bd364  18 00 8d e2                                      add r0, sp, #0x18
006bd368  fb 8f fb eb                                      bl #0x5a135c
006bd36c  18 30 9d e5                                      ldr r3, [sp, #0x18]
006bd370  00 00 53 e3                                      cmp r3, #0
006bd374  00 20 93 15                                      ldrne r2, [r3]
006bd378  01 20 82 12                                      addne r2, r2, #1
006bd37c  00 20 83 15                                      strne r2, [r3]
006bd380  14 a0 94 e5                                      ldr sl, [r4, #0x14]
006bd384  14 30 84 e5                                      str r3, [r4, #0x14]
006bd388  00 00 5a e3                                      cmp sl, #0
006bd38c  04 00 00 0a                                      beq #0x6bd3a4
006bd390  00 30 9a e5                                      ldr r3, [sl]
006bd394  01 30 43 e2                                      sub r3, r3, #1
006bd398  00 00 53 e3                                      cmp r3, #0
006bd39c  00 30 8a e5                                      str r3, [sl]
006bd3a0  79 00 00 0a                                      beq #0x6bd58c
006bd3a4  18 a0 9d e5                                      ldr sl, [sp, #0x18]
006bd3a8  00 00 5a e3                                      cmp sl, #0
006bd3ac  04 00 00 0a                                      beq #0x6bd3c4
006bd3b0  00 30 9a e5                                      ldr r3, [sl]
006bd3b4  01 30 43 e2                                      sub r3, r3, #1
006bd3b8  00 00 53 e3                                      cmp r3, #0
006bd3bc  00 30 8a e5                                      str r3, [sl]
006bd3c0  6c 00 00 0a                                      beq #0x6bd578
006bd3c4  00 20 e0 e3                                      mvn r2, #0
006bd3c8  00 30 a0 e3                                      mov r3, #0
006bd3cc  14 00 94 e5                                      ldr r0, [r4, #0x14]
006bd3d0  07 10 a0 e1                                      mov r1, r7
006bd3d4  ec 90 fb eb                                      bl #0x5a178c
006bd3d8  00 30 96 e5                                      ldr r3, [r6]
006bd3dc  14 20 94 e5                                      ldr r2, [r4, #0x14]
006bd3e0  00 00 53 e3                                      cmp r3, #0
006bd3e4  04 30 96 15                                      ldrne r3, [r6, #4]
006bd3e8  08 30 82 e5                                      str r3, [r2, #8]
006bd3ec  00 30 96 e5                                      ldr r3, [r6]
006bd3f0  0c 20 d5 e5                                      ldrb r2, [r5, #0xc]
006bd3f4  00 00 53 e3                                      cmp r3, #0
006bd3f8  1a 00 00 0a                                      beq #0x6bd468
006bd3fc  08 30 96 e5                                      ldr r3, [r6, #8]
006bd400  72 20 af e6                                      sxtb r2, r2
006bd404  20 30 93 e5                                      ldr r3, [r3, #0x20]
006bd408  02 31 93 e7                                      ldr r3, [r3, r2, lsl #2]
006bd40c  00 00 53 e3                                      cmp r3, #0
006bd410  14 00 00 0a                                      beq #0x6bd468
006bd414  14 20 94 e5                                      ldr r2, [r4, #0x14]
006bd418  04 10 93 e5                                      ldr r1, [r3, #4]
006bd41c  08 00 93 e5                                      ldr r0, [r3, #8]
006bd420  10 20 92 e5                                      ldr r2, [r2, #0x10]
006bd424  00 c0 93 e5                                      ldr ip, [r3]
006bd428  08 00 82 e5                                      str r0, [r2, #8]
006bd42c  00 c0 82 e5                                      str ip, [r2]
006bd430  04 10 82 e5                                      str r1, [r2, #4]
006bd434  14 20 94 e5                                      ldr r2, [r4, #0x14]
006bd438  14 00 93 e5                                      ldr r0, [r3, #0x14]
006bd43c  0c c0 93 e5                                      ldr ip, [r3, #0xc]
006bd440  10 20 92 e5                                      ldr r2, [r2, #0x10]
006bd444  10 10 93 e5                                      ldr r1, [r3, #0x10]
006bd448  0c 30 82 e2                                      add r3, r2, #0xc
006bd44c  0c c0 82 e5                                      str ip, [r2, #0xc]
006bd450  08 00 83 e5                                      str r0, [r3, #8]
006bd454  04 10 83 e5                                      str r1, [r3, #4]
006bd458  14 30 94 e5                                      ldr r3, [r4, #0x14]
006bd45c  be 20 d3 e1                                      ldrh r2, [r3, #0xe]
006bd460  04 20 82 e3                                      orr r2, r2, #4
006bd464  be 20 c3 e1                                      strh r2, [r3, #0xe]
006bd468  14 00 9d e5                                      ldr r0, [sp, #0x14]
006bd46c  24 20 a0 e3                                      mov r2, #0x24
006bd470  00 10 a0 e3                                      mov r1, #0
006bd474  08 b0 a0 e3                                      mov fp, #8
006bd478  10 70 8d e5                                      str r7, [sp, #0x10]
006bd47c  14 80 8d e5                                      str r8, [sp, #0x14]
006bd480  d0 31 d0 e1                                      ldrsb r3, [r0, #0x10]
006bd484  00 00 53 e3                                      cmp r3, #0
006bd488  23 00 00 ba                                      blt #0x6bd51c
006bd48c  00 c0 96 e5                                      ldr ip, [r6]
006bd490  0c 70 42 e2                                      sub r7, r2, #0xc
006bd494  00 00 5c e3                                      cmp ip, #0
006bd498  1a 00 00 0a                                      beq #0x6bd508
006bd49c  08 c0 96 e5                                      ldr ip, [r6, #8]
006bd4a0  20 c0 9c e5                                      ldr ip, [ip, #0x20]
006bd4a4  03 31 9c e7                                      ldr r3, [ip, r3, lsl #2]
006bd4a8  00 00 53 e3                                      cmp r3, #0
006bd4ac  15 00 00 0a                                      beq #0x6bd508
006bd4b0  14 c0 94 e5                                      ldr ip, [r4, #0x14]
006bd4b4  08 a0 93 e5                                      ldr sl, [r3, #8]
006bd4b8  04 90 93 e5                                      ldr sb, [r3, #4]
006bd4bc  10 50 9c e5                                      ldr r5, [ip, #0x10]
006bd4c0  00 80 93 e5                                      ldr r8, [r3]
006bd4c4  07 c0 85 e0                                      add ip, r5, r7
006bd4c8  07 80 85 e7                                      str r8, [r5, r7]
006bd4cc  08 a0 8c e5                                      str sl, [ip, #8]
006bd4d0  04 90 8c e5                                      str sb, [ip, #4]
006bd4d4  14 c0 94 e5                                      ldr ip, [r4, #0x14]
006bd4d8  14 50 93 e5                                      ldr r5, [r3, #0x14]
006bd4dc  0c 70 93 e5                                      ldr r7, [r3, #0xc]
006bd4e0  10 c0 9c e5                                      ldr ip, [ip, #0x10]
006bd4e4  10 a0 93 e5                                      ldr sl, [r3, #0x10]
006bd4e8  02 30 8c e0                                      add r3, ip, r2
006bd4ec  02 70 8c e7                                      str r7, [ip, r2]
006bd4f0  08 50 83 e5                                      str r5, [r3, #8]
006bd4f4  04 a0 83 e5                                      str sl, [r3, #4]
006bd4f8  14 30 94 e5                                      ldr r3, [r4, #0x14]
006bd4fc  be c0 d3 e1                                      ldrh ip, [r3, #0xe]
006bd500  1b c1 8c e1                                      orr ip, ip, fp, lsl r1
006bd504  be c0 c3 e1                                      strh ip, [r3, #0xe]
006bd508  01 10 81 e2                                      add r1, r1, #1
006bd50c  04 00 51 e3                                      cmp r1, #4
006bd510  01 00 80 e2                                      add r0, r0, #1
006bd514  18 20 82 e2                                      add r2, r2, #0x18
006bd518  d8 ff ff 1a                                      bne #0x6bd480
006bd51c  10 70 9d e5                                      ldr r7, [sp, #0x10]
006bd520  14 80 9d e5                                      ldr r8, [sp, #0x14]
006bd524  08 82 87 e0                                      add r8, r7, r8, lsl #4
006bd528  08 00 57 e1                                      cmp r7, r8
006bd52c  07 50 a0 11                                      movne r5, r7
006bd530  06 00 00 0a                                      beq #0x6bd550
006bd534  00 00 95 e5                                      ldr r0, [r5]
006bd538  10 50 85 e2                                      add r5, r5, #0x10
006bd53c  00 00 50 e3                                      cmp r0, #0
006bd540  00 00 00 0a                                      beq #0x6bd548
006bd544  0e 80 f1 eb                                      bl #0x31d584
006bd548  05 00 58 e1                                      cmp r8, r5
006bd54c  f8 ff ff 1a                                      bne #0x6bd534
006bd550  00 00 57 e3                                      cmp r7, #0
006bd554  01 00 00 0a                                      beq #0x6bd560
006bd558  07 00 a0 e1                                      mov r0, r7
006bd55c  49 dc f9 eb                                      bl #0x534688
006bd560  04 00 a0 e1                                      mov r0, r4
006bd564  44 d0 8d e2                                      add sp, sp, #0x44
006bd568  f0 8f bd e8                                      pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
006bd56c  28 c0 91 e5                                      ldr ip, [r1, #0x28]
006bd570  01 20 a0 e3                                      mov r2, #1
006bd574  a1 fe ff ea                                      b #0x6bd000
006bd578  0a 00 a0 e1                                      mov r0, sl
006bd57c  26 8d fb eb                                      bl #0x5a0a1c
006bd580  0a 00 a0 e1                                      mov r0, sl
006bd584  49 43 f1 eb                                      bl #0x30e2b0
006bd588  8d ff ff ea                                      b #0x6bd3c4
006bd58c  0a 00 a0 e1                                      mov r0, sl
006bd590  21 8d fb eb                                      bl #0x5a0a1c
006bd594  0a 00 a0 e1                                      mov r0, sl
006bd598  44 43 f1 eb                                      bl #0x30e2b0
006bd59c  80 ff ff ea                                      b #0x6bd3a4
006bd5a0  00 00 58 e3                                      cmp r8, #0
006bd5a4  41 00 00 1a                                      bne #0x6bd6b0
006bd5a8  24 10 95 e5                                      ldr r1, [r5, #0x24]
006bd5ac  00 20 99 e5                                      ldr r2, [sb]
006bd5b0  00 30 97 e5                                      ldr r3, [r7]
006bd5b4  01 08 51 e3                                      cmp r1, #0x10000
006bd5b8  78 c0 92 e5                                      ldr ip, [r2, #0x78]
006bd5bc  28 10 95 e5                                      ldr r1, [r5, #0x28]
006bd5c0  2c 20 95 e5                                      ldr r2, [r5, #0x2c]
006bd5c4  24 00 8d e2                                      add r0, sp, #0x24
006bd5c8  01 11 a0 a1                                      lslge r1, r1, #2
006bd5cc  81 10 a0 b1                                      lsllt r1, r1, #1
006bd5d0  04 20 8d e5                                      str r2, [sp, #4]
006bd5d4  00 20 a0 e3                                      mov r2, #0
006bd5d8  00 10 8d e5                                      str r1, [sp]
006bd5dc  08 20 8d e5                                      str r2, [sp, #8]
006bd5e0  09 10 a0 e1                                      mov r1, sb
006bd5e4  01 20 a0 e3                                      mov r2, #1
006bd5e8  3c ff 2f e1                                      blx ip
006bd5ec  24 30 9d e5                                      ldr r3, [sp, #0x24]
006bd5f0  00 00 53 e3                                      cmp r3, #0
006bd5f4  04 20 93 15                                      ldrne r2, [r3, #4]
006bd5f8  01 20 82 12                                      addne r2, r2, #1
006bd5fc  04 20 83 15                                      strne r2, [r3, #4]
006bd600  30 00 95 e5                                      ldr r0, [r5, #0x30]
006bd604  30 30 85 e5                                      str r3, [r5, #0x30]
006bd608  00 00 50 e3                                      cmp r0, #0
006bd60c  00 00 00 0a                                      beq #0x6bd614
006bd610  db 7f f1 eb                                      bl #0x31d584
006bd614  24 00 9d e5                                      ldr r0, [sp, #0x24]
006bd618  00 00 50 e3                                      cmp r0, #0
006bd61c  00 00 00 0a                                      beq #0x6bd624
006bd620  d7 7f f1 eb                                      bl #0x31d584
006bd624  30 30 95 e5                                      ldr r3, [r5, #0x30]
006bd628  00 00 53 e3                                      cmp r3, #0
006bd62c  04 20 93 15                                      ldrne r2, [r3, #4]
006bd630  01 20 82 12                                      addne r2, r2, #1
006bd634  04 20 83 15                                      strne r2, [r3, #4]
006bd638  18 00 94 e5                                      ldr r0, [r4, #0x18]
006bd63c  18 30 84 e5                                      str r3, [r4, #0x18]
006bd640  00 00 50 e3                                      cmp r0, #0
006bd644  93 fe ff 0a                                      beq #0x6bd098
006bd648  cd 7f f1 eb                                      bl #0x31d584
006bd64c  04 30 d7 e5                                      ldrb r3, [r7, #4]
006bd650  00 00 53 e3                                      cmp r3, #0
006bd654  92 fe ff 0a                                      beq #0x6bd0a4
006bd658  30 30 95 e5                                      ldr r3, [r5, #0x30]
006bd65c  05 10 d7 e5                                      ldrb r1, [r7, #5]
006bd660  12 20 d3 e5                                      ldrb r2, [r3, #0x12]
006bd664  08 00 12 e3                                      tst r2, #8
006bd668  93 00 00 1a                                      bne #0x6bd8bc
006bd66c  11 20 d3 e5                                      ldrb r2, [r3, #0x11]
006bd670  04 00 52 e3                                      cmp r2, #4
006bd674  8a fe ff 0a                                      beq #0x6bd0a4
006bd678  03 00 a0 e1                                      mov r0, r3
006bd67c  00 30 93 e5                                      ldr r3, [r3]
006bd680  0f e0 a0 e1                                      mov lr, pc
006bd684  0c f0 93 e5                                      ldr pc, [r3, #0xc]
006bd688  85 fe ff ea                                      b #0x6bd0a4
006bd68c  7b b0 ef e6                                      uxtb fp, fp
006bd690  00 30 9a e5                                      ldr r3, [sl]
006bd694  0a 00 a0 e1                                      mov r0, sl
006bd698  0f e0 a0 e1                                      mov lr, pc
006bd69c  10 f0 93 e5                                      ldr pc, [r3, #0x10]
006bd6a0  04 00 5b e3                                      cmp fp, #4
006bd6a4  11 b0 ca e5                                      strb fp, [sl, #0x11]
006bd6a8  75 fe ff 1a                                      bne #0x6bd084
006bd6ac  79 fe ff ea                                      b #0x6bd098
006bd6b0  24 30 95 e5                                      ldr r3, [r5, #0x24]
006bd6b4  01 08 53 e3                                      cmp r3, #0x10000
006bd6b8  28 30 95 e5                                      ldr r3, [r5, #0x28]
006bd6bc  03 31 a0 a1                                      lslge r3, r3, #2
006bd6c0  83 30 a0 b1                                      lsllt r3, r3, #1
006bd6c4  01 08 53 e3                                      cmp r3, #0x10000
006bd6c8  83 00 00 ba                                      blt #0x6bd8dc
006bd6cc  2c 30 95 e5                                      ldr r3, [r5, #0x2c]
006bd6d0  3c a0 8d e2                                      add sl, sp, #0x3c
006bd6d4  0a 10 a0 e1                                      mov r1, sl
006bd6d8  00 00 53 e3                                      cmp r3, #0
006bd6dc  3c 30 8d e5                                      str r3, [sp, #0x3c]
006bd6e0  00 20 93 15                                      ldrne r2, [r3]
006bd6e4  0c 00 84 e2                                      add r0, r4, #0xc
006bd6e8  00 b0 a0 e3                                      mov fp, #0
006bd6ec  01 20 82 12                                      addne r2, r2, #1
006bd6f0  00 20 83 15                                      strne r2, [r3]
006bd6f4  1c fc ff eb                                      bl #0x6bc76c
006bd6f8  0a 00 a0 e1                                      mov r0, sl
006bd6fc  06 fc ff eb                                      bl #0x6bc71c
006bd700  30 00 85 e2                                      add r0, r5, #0x30
006bd704  24 20 95 e5                                      ldr r2, [r5, #0x24]
006bd708  00 30 99 e5                                      ldr r3, [sb]
006bd70c  10 00 8d e5                                      str r0, [sp, #0x10]
006bd710  28 10 95 e5                                      ldr r1, [r5, #0x28]
006bd714  01 08 52 e3                                      cmp r2, #0x10000
006bd718  0c 20 94 e5                                      ldr r2, [r4, #0xc]
006bd71c  01 11 a0 a1                                      lslge r1, r1, #2
006bd720  81 10 a0 b1                                      lsllt r1, r1, #1
006bd724  78 c0 93 e5                                      ldr ip, [r3, #0x78]
006bd728  00 30 97 e5                                      ldr r3, [r7]
006bd72c  00 10 8d e5                                      str r1, [sp]
006bd730  0c 20 92 e5                                      ldr r2, [r2, #0xc]
006bd734  38 a0 8d e2                                      add sl, sp, #0x38
006bd738  0a 00 a0 e1                                      mov r0, sl
006bd73c  04 20 8d e5                                      str r2, [sp, #4]
006bd740  09 10 a0 e1                                      mov r1, sb
006bd744  01 20 a0 e3                                      mov r2, #1
006bd748  08 b0 8d e5                                      str fp, [sp, #8]
006bd74c  3c ff 2f e1                                      blx ip
006bd750  0a 10 a0 e1                                      mov r1, sl
006bd754  10 00 9d e5                                      ldr r0, [sp, #0x10]
006bd758  4b fc ff eb                                      bl #0x6bc88c
006bd75c  0a 00 a0 e1                                      mov r0, sl
006bd760  40 a0 8d e2                                      add sl, sp, #0x40
006bd764  08 e9 fd eb                                      bl #0x637b8c
006bd768  0c b0 2a e5                                      str fp, [sl, #-0xc]!
006bd76c  08 00 84 e2                                      add r0, r4, #8
006bd770  0a 10 a0 e1                                      mov r1, sl
006bd774  2a fc ff eb                                      bl #0x6bc824
006bd778  0a 00 a0 e1                                      mov r0, sl
006bd77c  14 fc ff eb                                      bl #0x6bc7d4
006bd780  a7 ff ff ea                                      b #0x6bd624
006bd784  00 00 58 e3                                      cmp r8, #0
006bd788  81 00 00 0a                                      beq #0x6bd994
006bd78c  24 70 97 e5                                      ldr r7, [r7, #0x24]
006bd790  00 00 57 e3                                      cmp r7, #0
006bd794  00 30 97 15                                      ldrne r3, [r7]
006bd798  02 30 83 12                                      addne r3, r3, #2
006bd79c  00 30 87 15                                      strne r3, [r7]
006bd7a0  10 80 94 e5                                      ldr r8, [r4, #0x10]
006bd7a4  00 00 58 e3                                      cmp r8, #0
006bd7a8  0a 00 00 0a                                      beq #0x6bd7d8
006bd7ac  00 30 98 e5                                      ldr r3, [r8]
006bd7b0  01 30 43 e2                                      sub r3, r3, #1
006bd7b4  00 00 53 e3                                      cmp r3, #0
006bd7b8  00 30 88 e5                                      str r3, [r8]
006bd7bc  05 00 00 1a                                      bne #0x6bd7d8
006bd7c0  0c 00 98 e5                                      ldr r0, [r8, #0xc]
006bd7c4  00 00 50 e3                                      cmp r0, #0
006bd7c8  00 00 00 0a                                      beq #0x6bd7d0
006bd7cc  39 42 f1 eb                                      bl #0x30e0b8
006bd7d0  00 30 a0 e3                                      mov r3, #0
006bd7d4  0c 30 88 e5                                      str r3, [r8, #0xc]
006bd7d8  00 00 57 e3                                      cmp r7, #0
006bd7dc  10 70 84 e5                                      str r7, [r4, #0x10]
006bd7e0  0a 00 00 0a                                      beq #0x6bd810
006bd7e4  00 30 97 e5                                      ldr r3, [r7]
006bd7e8  01 30 43 e2                                      sub r3, r3, #1
006bd7ec  00 00 53 e3                                      cmp r3, #0
006bd7f0  00 30 87 e5                                      str r3, [r7]
006bd7f4  05 00 00 1a                                      bne #0x6bd810
006bd7f8  0c 00 97 e5                                      ldr r0, [r7, #0xc]
006bd7fc  00 00 50 e3                                      cmp r0, #0
006bd800  00 00 00 0a                                      beq #0x6bd808
006bd804  2b 42 f1 eb                                      bl #0x30e0b8
006bd808  00 30 a0 e3                                      mov r3, #0
006bd80c  0c 30 87 e5                                      str r3, [r7, #0xc]
006bd810  00 10 96 e5                                      ldr r1, [r6]
006bd814  08 20 96 e5                                      ldr r2, [r6, #8]
006bd818  00 30 99 e5                                      ldr r3, [sb]
006bd81c  00 00 51 e3                                      cmp r1, #0
006bd820  04 10 96 15                                      ldrne r1, [r6, #4]
006bd824  28 80 82 e2                                      add r8, r2, #0x28
006bd828  00 20 92 15                                      ldrne r2, [r2]
006bd82c  68 00 9d e5                                      ldr r0, [sp, #0x68]
006bd830  78 c0 93 e5                                      ldr ip, [r3, #0x78]
006bd834  91 02 01 10                                      mulne r1, r1, r2
006bd838  10 20 94 e5                                      ldr r2, [r4, #0x10]
006bd83c  00 30 90 e5                                      ldr r3, [r0]
006bd840  00 10 8d e5                                      str r1, [sp]
006bd844  0c 00 92 e5                                      ldr r0, [r2, #0xc]
006bd848  20 70 8d e2                                      add r7, sp, #0x20
006bd84c  00 10 a0 e3                                      mov r1, #0
006bd850  01 20 a0 e1                                      mov r2, r1
006bd854  03 00 8d e9                                      stmib sp, {r0, r1}
006bd858  07 00 a0 e1                                      mov r0, r7
006bd85c  09 10 a0 e1                                      mov r1, sb
006bd860  3c ff 2f e1                                      blx ip
006bd864  08 00 a0 e1                                      mov r0, r8
006bd868  07 10 a0 e1                                      mov r1, r7
006bd86c  06 fc ff eb                                      bl #0x6bc88c
006bd870  07 00 a0 e1                                      mov r0, r7
006bd874  c4 e8 fd eb                                      bl #0x637b8c
006bd878  21 fe ff ea                                      b #0x6bd104
006bd87c  08 30 96 e5                                      ldr r3, [r6, #8]
006bd880  05 10 d1 e5                                      ldrb r1, [r1, #5]
006bd884  28 30 93 e5                                      ldr r3, [r3, #0x28]
006bd888  12 20 d3 e5                                      ldrb r2, [r3, #0x12]
006bd88c  08 00 12 e3                                      tst r2, #8
006bd890  01 00 00 0a                                      beq #0x6bd89c
006bd894  02 00 12 e3                                      tst r2, #2
006bd898  1d fe ff 0a                                      beq #0x6bd114
006bd89c  11 20 d3 e5                                      ldrb r2, [r3, #0x11]
006bd8a0  04 00 52 e3                                      cmp r2, #4
006bd8a4  1a fe ff 0a                                      beq #0x6bd114
006bd8a8  03 00 a0 e1                                      mov r0, r3
006bd8ac  00 30 93 e5                                      ldr r3, [r3]
006bd8b0  0f e0 a0 e1                                      mov lr, pc
006bd8b4  0c f0 93 e5                                      ldr pc, [r3, #0xc]
006bd8b8  15 fe ff ea                                      b #0x6bd114
006bd8bc  02 00 12 e3                                      tst r2, #2
006bd8c0  f7 fd ff 0a                                      beq #0x6bd0a4
006bd8c4  68 ff ff ea                                      b #0x6bd66c
006bd8c8  00 30 9a e5                                      ldr r3, [sl]
006bd8cc  0a 00 a0 e1                                      mov r0, sl
006bd8d0  0f e0 a0 e1                                      mov lr, pc
006bd8d4  10 f0 93 e5                                      ldr pc, [r3, #0x10]
006bd8d8  00 fe ff ea                                      b #0x6bd0e0
006bd8dc  2c 30 95 e5                                      ldr r3, [r5, #0x2c]
006bd8e0  30 a0 8d e2                                      add sl, sp, #0x30
006bd8e4  0a 10 a0 e1                                      mov r1, sl
006bd8e8  00 00 53 e3                                      cmp r3, #0
006bd8ec  30 30 8d e5                                      str r3, [sp, #0x30]
006bd8f0  00 20 93 15                                      ldrne r2, [r3]
006bd8f4  08 00 84 e2                                      add r0, r4, #8
006bd8f8  00 b0 a0 e3                                      mov fp, #0
006bd8fc  01 20 82 12                                      addne r2, r2, #1
006bd900  00 20 83 15                                      strne r2, [r3]
006bd904  c6 fb ff eb                                      bl #0x6bc824
006bd908  0a 00 a0 e1                                      mov r0, sl
006bd90c  b0 fb ff eb                                      bl #0x6bc7d4
006bd910  30 10 85 e2                                      add r1, r5, #0x30
006bd914  24 20 95 e5                                      ldr r2, [r5, #0x24]
006bd918  00 30 99 e5                                      ldr r3, [sb]
006bd91c  10 10 8d e5                                      str r1, [sp, #0x10]
006bd920  28 10 95 e5                                      ldr r1, [r5, #0x28]
006bd924  01 08 52 e3                                      cmp r2, #0x10000
006bd928  08 20 94 e5                                      ldr r2, [r4, #8]
006bd92c  01 11 a0 a1                                      lslge r1, r1, #2
006bd930  81 10 a0 b1                                      lsllt r1, r1, #1
006bd934  78 c0 93 e5                                      ldr ip, [r3, #0x78]
006bd938  00 30 97 e5                                      ldr r3, [r7]
006bd93c  00 10 8d e5                                      str r1, [sp]
006bd940  0c 20 92 e5                                      ldr r2, [r2, #0xc]
006bd944  2c a0 8d e2                                      add sl, sp, #0x2c
006bd948  0a 00 a0 e1                                      mov r0, sl
006bd94c  04 20 8d e5                                      str r2, [sp, #4]
006bd950  09 10 a0 e1                                      mov r1, sb
006bd954  01 20 a0 e3                                      mov r2, #1
006bd958  08 b0 8d e5                                      str fp, [sp, #8]
006bd95c  3c ff 2f e1                                      blx ip
006bd960  0a 10 a0 e1                                      mov r1, sl
006bd964  10 00 9d e5                                      ldr r0, [sp, #0x10]
006bd968  c7 fb ff eb                                      bl #0x6bc88c
006bd96c  0a 00 a0 e1                                      mov r0, sl
006bd970  40 a0 8d e2                                      add sl, sp, #0x40
006bd974  84 e8 fd eb                                      bl #0x637b8c
006bd978  18 b0 2a e5                                      str fp, [sl, #-0x18]!
006bd97c  0c 00 84 e2                                      add r0, r4, #0xc
006bd980  0a 10 a0 e1                                      mov r1, sl
006bd984  78 fb ff eb                                      bl #0x6bc76c
006bd988  0a 00 a0 e1                                      mov r0, sl
006bd98c  62 fb ff eb                                      bl #0x6bc71c
006bd990  23 ff ff ea                                      b #0x6bd624
006bd994  04 20 96 e5                                      ldr r2, [r6, #4]
006bd998  00 10 97 e5                                      ldr r1, [r7]
006bd99c  68 c0 9d e5                                      ldr ip, [sp, #0x68]
006bd9a0  1c a0 8d e2                                      add sl, sp, #0x1c
006bd9a4  91 02 01 e0                                      mul r1, r1, r2
006bd9a8  00 30 9c e5                                      ldr r3, [ip]
006bd9ac  00 10 8d e5                                      str r1, [sp]
006bd9b0  24 10 97 e5                                      ldr r1, [r7, #0x24]
006bd9b4  08 80 8d e5                                      str r8, [sp, #8]
006bd9b8  08 20 a0 e1                                      mov r2, r8
006bd9bc  04 10 8d e5                                      str r1, [sp, #4]
006bd9c0  0a 00 a0 e1                                      mov r0, sl
006bd9c4  09 10 a0 e1                                      mov r1, sb
006bd9c8  00 c0 99 e5                                      ldr ip, [sb]
006bd9cc  0f e0 a0 e1                                      mov lr, pc
006bd9d0  78 f0 9c e5                                      ldr pc, [ip, #0x78]
006bd9d4  28 00 87 e2                                      add r0, r7, #0x28
006bd9d8  0a 10 a0 e1                                      mov r1, sl
006bd9dc  aa fb ff eb                                      bl #0x6bc88c
006bd9e0  0a 00 a0 e1                                      mov r0, sl
006bd9e4  68 e8 fd eb                                      bl #0x637b8c
006bd9e8  c5 fd ff ea                                      b #0x6bd104
; mapping-symbol data/literal pool
006bd9ec  04 7b 2d 00 54 0c 00 00 50 e3 22 00              .byte 0x04, 0x7b, 0x2d, 0x00, 0x54, 0x0c, 0x00, 0x00, 0x50, 0xe3, 0x22, 0x00

