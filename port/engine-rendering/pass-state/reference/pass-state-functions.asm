; Selected ARM functions for render-pass state flow. Byte rows checked against the supplied APK ELF.

; Source: glitch_video_detail_renderpass_SRenderState-2777ca965cb6-001.asm
; FUNCTION 0x005d7a10, declared_size=564, range_size=564, mode=arm
; class-group: glitch::video::detail::renderpass::SRenderState
; alias: _ZN6glitch5video6detail10renderpass12SRenderStateC1ERKNS0_12SRenderStateE
; demangled: glitch::video::detail::renderpass::SRenderState::SRenderState(glitch::video::SRenderState const&)
; decoder-mode: arm
005d7a10  30 00 2d e9                                      push {r4, r5}
005d7a14  15 c0 d1 e5                                      ldrb ip, [r1, #0x15]
005d7a18  14 20 d1 e5                                      ldrb r2, [r1, #0x14]
005d7a1c  17 40 d1 e5                                      ldrb r4, [r1, #0x17]
005d7a20  16 50 d1 e5                                      ldrb r5, [r1, #0x16]
005d7a24  08 20 c0 e5                                      strb r2, [r0, #8]
005d7a28  0b 40 c0 e5                                      strb r4, [r0, #0xb]
005d7a2c  0a 50 c0 e5                                      strb r5, [r0, #0xa]
005d7a30  09 c0 c0 e5                                      strb ip, [r0, #9]
005d7a34  00 30 a0 e1                                      mov r3, r0
005d7a38  28 00 91 e5                                      ldr r0, [r1, #0x28]
005d7a3c  00 20 a0 e3                                      mov r2, #0
005d7a40  0c 00 83 e5                                      str r0, [r3, #0xc]
005d7a44  2c 00 91 e5                                      ldr r0, [r1, #0x2c]
005d7a48  10 00 83 e5                                      str r0, [r3, #0x10]
005d7a4c  38 00 91 e5                                      ldr r0, [r1, #0x38]
005d7a50  04 20 83 e5                                      str r2, [r3, #4]
005d7a54  00 20 83 e5                                      str r2, [r3]
005d7a58  1c 00 83 e5                                      str r0, [r3, #0x1c]
005d7a5c  0c 20 91 e5                                      ldr r2, [r1, #0xc]
005d7a60  02 07 12 e3                                      tst r2, #0x80000
005d7a64  01 28 a0 13                                      movne r2, #0x10000
005d7a68  04 20 83 15                                      strne r2, [r3, #4]
005d7a6c  08 c0 91 e5                                      ldr ip, [r1, #8]
005d7a70  5c c6 e2 e7                                      ubfx ip, ip, #0xc, #3
005d7a74  0c cc a0 e1                                      lsl ip, ip, #0x18
005d7a78  00 c0 83 e5                                      str ip, [r3]
005d7a7c  00 00 d1 e5                                      ldrb r0, [r1]
005d7a80  00 c0 8c e1                                      orr ip, ip, r0
005d7a84  00 c0 83 e5                                      str ip, [r3]
005d7a88  0c 20 91 e5                                      ldr r2, [r1, #0xc]
005d7a8c  01 06 12 e3                                      tst r2, #0x100000
005d7a90  04 20 93 e5                                      ldr r2, [r3, #4]
005d7a94  02 28 82 13                                      orrne r2, r2, #0x20000
005d7a98  02 28 c2 03                                      biceq r2, r2, #0x20000
005d7a9c  04 20 83 e5                                      str r2, [r3, #4]
005d7aa0  08 00 91 e5                                      ldr r0, [r1, #8]
005d7aa4  01 27 c2 e3                                      bic r2, r2, #0x40000
005d7aa8  03 01 00 e2                                      and r0, r0, #0xc0000000
005d7aac  00 00 8c e1                                      orr r0, ip, r0
005d7ab0  00 00 83 e5                                      str r0, [r3]
005d7ab4  0c c0 91 e5                                      ldr ip, [r1, #0xc]
005d7ab8  dc ca e0 e7                                      ubfx ip, ip, #0x15, #1
005d7abc  0c 29 82 e1                                      orr r2, r2, ip, lsl #18
005d7ac0  04 20 83 e5                                      str r2, [r3, #4]
005d7ac4  0c c0 91 e5                                      ldr ip, [r1, #0xc]
005d7ac8  01 05 1c e3                                      tst ip, #0x400000
005d7acc  02 27 82 13                                      orrne r2, r2, #0x80000
005d7ad0  02 27 c2 03                                      biceq r2, r2, #0x80000
005d7ad4  04 20 83 e5                                      str r2, [r3, #4]
005d7ad8  0c 20 91 e5                                      ldr r2, [r1, #0xc]
005d7adc  52 26 e2 e7                                      ubfx r2, r2, #0xc, #3
005d7ae0  82 0d 80 e1                                      orr r0, r0, r2, lsl #27
005d7ae4  00 00 83 e5                                      str r0, [r3]
005d7ae8  0c 20 91 e5                                      ldr r2, [r1, #0xc]
005d7aec  02 05 12 e3                                      tst r2, #0x800000
005d7af0  04 20 93 e5                                      ldr r2, [r3, #4]
005d7af4  01 26 82 13                                      orrne r2, r2, #0x100000
005d7af8  01 26 c2 03                                      biceq r2, r2, #0x100000
005d7afc  04 20 83 e5                                      str r2, [r3, #4]
005d7b00  0c 00 91 e5                                      ldr r0, [r1, #0xc]
005d7b04  03 2a c2 e3                                      bic r2, r2, #0x3000
005d7b08  d0 07 e1 e7                                      ubfx r0, r0, #0xf, #2
005d7b0c  00 26 82 e1                                      orr r2, r2, r0, lsl #12
005d7b10  04 20 83 e5                                      str r2, [r3, #4]
005d7b14  0c 00 91 e5                                      ldr r0, [r1, #0xc]
005d7b18  03 29 c2 e3                                      bic r2, r2, #0xc000
005d7b1c  d0 08 e1 e7                                      ubfx r0, r0, #0x11, #2
005d7b20  00 27 82 e1                                      orr r2, r2, r0, lsl #14
005d7b24  04 20 83 e5                                      str r2, [r3, #4]
005d7b28  0c 00 91 e5                                      ldr r0, [r1, #0xc]
005d7b2c  02 04 10 e3                                      tst r0, #0x2000000
005d7b30  02 c6 82 13                                      orrne ip, r2, #0x200000
005d7b34  02 c6 c2 03                                      biceq ip, r2, #0x200000
005d7b38  04 c0 83 e5                                      str ip, [r3, #4]
005d7b3c  0c 20 91 e5                                      ldr r2, [r1, #0xc]
005d7b40  01 03 12 e3                                      tst r2, #0x4000000
005d7b44  01 c5 8c 13                                      orrne ip, ip, #0x400000
005d7b48  01 c5 cc 03                                      biceq ip, ip, #0x400000
005d7b4c  04 c0 83 e5                                      str ip, [r3, #4]
005d7b50  0c 20 91 e5                                      ldr r2, [r1, #0xc]
005d7b54  02 03 12 e3                                      tst r2, #0x8000000
005d7b58  02 c5 8c 13                                      orrne ip, ip, #0x800000
005d7b5c  02 c5 cc 03                                      biceq ip, ip, #0x800000
005d7b60  04 c0 83 e5                                      str ip, [r3, #4]
005d7b64  0c 20 91 e5                                      ldr r2, [r1, #0xc]
005d7b68  01 02 12 e3                                      tst r2, #0x10000000
005d7b6c  01 c4 8c 13                                      orrne ip, ip, #0x1000000
005d7b70  01 c4 cc 03                                      biceq ip, ip, #0x1000000
005d7b74  04 c0 83 e5                                      str ip, [r3, #4]
005d7b78  0c 20 91 e5                                      ldr r2, [r1, #0xc]
005d7b7c  02 02 12 e3                                      tst r2, #0x20000000
005d7b80  02 c4 8c 13                                      orrne ip, ip, #0x2000000
005d7b84  02 c4 cc 03                                      biceq ip, ip, #0x2000000
005d7b88  04 c0 83 e5                                      str ip, [r3, #4]
005d7b8c  0c 20 91 e5                                      ldr r2, [r1, #0xc]
005d7b90  01 01 12 e3                                      tst r2, #0x40000000
005d7b94  01 c3 8c 13                                      orrne ip, ip, #0x4000000
005d7b98  01 c3 cc 03                                      biceq ip, ip, #0x4000000
005d7b9c  04 c0 83 e5                                      str ip, [r3, #4]
005d7ba0  10 20 91 e5                                      ldr r2, [r1, #0x10]
005d7ba4  01 00 12 e3                                      tst r2, #1
005d7ba8  02 c3 8c 13                                      orrne ip, ip, #0x8000000
005d7bac  02 c3 cc 03                                      biceq ip, ip, #0x8000000
005d7bb0  04 c0 83 e5                                      str ip, [r3, #4]
005d7bb4  08 00 91 e5                                      ldr r0, [r1, #8]
005d7bb8  07 c0 cc e3                                      bic ip, ip, #7
005d7bbc  00 20 93 e5                                      ldr r2, [r3]
005d7bc0  50 09 e2 e7                                      ubfx r0, r0, #0x12, #3
005d7bc4  00 c0 8c e1                                      orr ip, ip, r0
005d7bc8  04 c0 83 e5                                      str ip, [r3, #4]
005d7bcc  02 00 d1 e5                                      ldrb r0, [r1, #2]
005d7bd0  ff 2c c2 e3                                      bic r2, r2, #0xff00
005d7bd4  38 c0 cc e3                                      bic ip, ip, #0x38
005d7bd8  00 24 82 e1                                      orr r2, r2, r0, lsl #8
005d7bdc  00 20 83 e5                                      str r2, [r3]
005d7be0  03 40 d1 e5                                      ldrb r4, [r1, #3]
005d7be4  ff 28 c2 e3                                      bic r2, r2, #0xff0000
005d7be8  03 00 a0 e1                                      mov r0, r3
005d7bec  04 28 82 e1                                      orr r2, r2, r4, lsl #16
005d7bf0  00 20 83 e5                                      str r2, [r3]
005d7bf4  08 20 91 e5                                      ldr r2, [r1, #8]
005d7bf8  d2 2a e2 e7                                      ubfx r2, r2, #0x15, #3
005d7bfc  82 21 8c e1                                      orr r2, ip, r2, lsl #3
005d7c00  04 20 83 e5                                      str r2, [r3, #4]
005d7c04  08 c0 91 e5                                      ldr ip, [r1, #8]
005d7c08  07 2d c2 e3                                      bic r2, r2, #0x1c0
005d7c0c  5c cc e2 e7                                      ubfx ip, ip, #0x18, #3
005d7c10  0c 23 82 e1                                      orr r2, r2, ip, lsl #6
005d7c14  04 20 83 e5                                      str r2, [r3, #4]
005d7c18  08 c0 91 e5                                      ldr ip, [r1, #8]
005d7c1c  0e 2c c2 e3                                      bic r2, r2, #0xe00
005d7c20  dc cd e2 e7                                      ubfx ip, ip, #0x1b, #3
005d7c24  8c 24 82 e1                                      orr r2, r2, ip, lsl #9
005d7c28  04 20 83 e5                                      str r2, [r3, #4]
005d7c2c  30 c0 91 e5                                      ldr ip, [r1, #0x30]
005d7c30  34 20 91 e5                                      ldr r2, [r1, #0x34]
005d7c34  14 c0 83 e5                                      str ip, [r3, #0x14]
005d7c38  18 20 83 e5                                      str r2, [r3, #0x18]
005d7c3c  30 00 bd e8                                      pop {r4, r5}
005d7c40  1e ff 2f e1                                      bx lr

; Source: glitch_video_CMaterialRendererManager-a4bf824a9f3f-001.asm
; FUNCTION 0x005dcf2c, declared_size=100, range_size=100, mode=arm
; class-group: glitch::video::CMaterialRendererManager
; alias: _ZN6glitch5video24CMaterialRendererManager13addRenderPassERKN5boost13intrusive_ptrIKNS0_7IShaderEEERKNS0_6detail10renderpass12SRenderStateERKNS9_8material12SRenderStateE
; demangled: glitch::video::CMaterialRendererManager::addRenderPass(boost::intrusive_ptr<glitch::video::IShader const> const&, glitch::video::detail::renderpass::SRenderState const&, glitch::video::detail::material::SRenderState const&)
; decoder-mode: arm
005dcf2c  f0 41 2d e9                                      push {r4, r5, r6, r7, r8, lr}
005dcf30  01 40 a0 e1                                      mov r4, r1
005dcf34  50 10 9f e5                                      ldr r1, [pc, #0x50]
005dcf38  02 60 a0 e1                                      mov r6, r2
005dcf3c  03 50 a0 e1                                      mov r5, r3
005dcf40  01 10 8f e0                                      add r1, pc, r1
005dcf44  00 70 a0 e1                                      mov r7, r0
005dcf48  42 fc ff eb                                      bl #0x5dc058
005dcf4c  00 00 50 e3                                      cmp r0, #0
005dcf50  08 00 00 0a                                      beq #0x5dcf78
005dcf54  00 80 94 e5                                      ldr r8, [r4]
005dcf58  00 00 58 e3                                      cmp r8, #0
005dcf5c  06 00 00 0a                                      beq #0x5dcf7c
005dcf60  90 00 97 e5                                      ldr r0, [r7, #0x90]
005dcf64  04 10 a0 e1                                      mov r1, r4
005dcf68  06 20 a0 e1                                      mov r2, r6
005dcf6c  05 30 a0 e1                                      mov r3, r5
005dcf70  1c f1 ff eb                                      bl #0x5d93e8
005dcf74  01 00 a0 e3                                      mov r0, #1
005dcf78  f0 81 bd e8                                      pop {r4, r5, r6, r7, r8, pc}
005dcf7c  07 00 a0 e1                                      mov r0, r7
005dcf80  93 ff ff eb                                      bl #0x5dcdd4
005dcf84  08 00 a0 e1                                      mov r0, r8
005dcf88  f0 81 bd e8                                      pop {r4, r5, r6, r7, r8, pc}
; mapping-symbol data/literal pool
005dcf8c  b8 3f 30 00                                      .byte 0xb8, 0x3f, 0x30, 0x00

; Source: void_glitch_video_detail-1d8f95ea9161-001.asm
; FUNCTION 0x005b71d8, declared_size=452, range_size=452, mode=arm
; class-group: void glitch::video::detail
; alias: _ZN6glitch5video6detail5applyILb1ENS1_10renderpass12SRenderStateENS0_15CCommonGLDriverINS0_21CProgrammableGLDriverINS0_18CGLSLShaderHandlerEEENS1_33CProgrammableGLFunctionPointerSetEEEEEvRKT0_PT1_
; demangled: void glitch::video::detail::apply<true, glitch::video::detail::renderpass::SRenderState, glitch::video::CCommonGLDriver<glitch::video::CProgrammableGLDriver<glitch::video::CGLSLShaderHandler>, glitch::video::detail::CProgrammableGLFunctionPointerSet> >(glitch::video::detail::renderpass::SRenderState const&, glitch::video::CCommonGLDriver<glitch::video::CProgrammableGLDriver<glitch::video::CGLSLShaderHandler>, glitch::video::detail::CProgrammableGLFunctionPointerSet>*)
; decoder-mode: arm
005b71d8  70 40 2d e9                                      push {r4, r5, r6, lr}
005b71dc  04 30 90 e5                                      ldr r3, [r0, #4]
005b71e0  00 50 a0 e1                                      mov r5, r0
005b71e4  01 40 a0 e1                                      mov r4, r1
005b71e8  53 68 e0 e7                                      ubfx r6, r3, #0x10, #1
005b71ec  00 00 56 e3                                      cmp r6, #0
005b71f0  42 00 00 1a                                      bne #0x5b7300
005b71f4  c4 21 d1 e5                                      ldrb r2, [r1, #0x1c4]
005b71f8  00 00 52 e3                                      cmp r2, #0
005b71fc  5c 00 00 1a                                      bne #0x5b7374
005b7200  d3 68 e0 e7                                      ubfx r6, r3, #0x11, #1
005b7204  00 00 56 e3                                      cmp r6, #0
005b7208  43 00 00 1a                                      bne #0x5b731c
005b720c  c5 21 d4 e5                                      ldrb r2, [r4, #0x1c5]
005b7210  00 00 52 e3                                      cmp r2, #0
005b7214  4c 00 00 1a                                      bne #0x5b734c
005b7218  d3 69 e0 e7                                      ubfx r6, r3, #0x13, #1
005b721c  00 00 56 e3                                      cmp r6, #0
005b7220  44 00 00 1a                                      bne #0x5b7338
005b7224  c6 21 d4 e5                                      ldrb r2, [r4, #0x1c6]
005b7228  00 00 52 e3                                      cmp r2, #0
005b722c  55 00 00 1a                                      bne #0x5b7388
005b7230  02 06 13 e3                                      tst r3, #0x200000
005b7234  13 00 00 0a                                      beq #0x5b7288
005b7238  04 00 a0 e1                                      mov r0, r4
005b723c  05 10 a0 e1                                      mov r1, r5
005b7240  57 e1 ff eb                                      bl #0x5af7a4
005b7244  04 30 95 e5                                      ldr r3, [r5, #4]
005b7248  d3 6c e0 e7                                      ubfx r6, r3, #0x19, #1
005b724c  00 00 56 e3                                      cmp r6, #0
005b7250  1b 00 00 1a                                      bne #0x5b72c4
005b7254  d1 21 d4 e5                                      ldrb r2, [r4, #0x1d1]
005b7258  00 00 52 e3                                      cmp r2, #0
005b725c  3f 00 00 1a                                      bne #0x5b7360
005b7260  d3 6d e0 e7                                      ubfx r6, r3, #0x1b, #1
005b7264  00 00 56 e3                                      cmp r6, #0
005b7268  1c 00 00 1a                                      bne #0x5b72e0
005b726c  d4 31 d4 e5                                      ldrb r3, [r4, #0x1d4]
005b7270  00 00 53 e3                                      cmp r3, #0
005b7274  1d 00 00 1a                                      bne #0x5b72f0
005b7278  04 00 a0 e1                                      mov r0, r4
005b727c  05 10 a0 e1                                      mov r1, r5
005b7280  70 40 bd e8                                      pop {r4, r5, r6, lr}
005b7284  90 ff ff ea                                      b #0x5b70cc
005b7288  01 05 13 e3                                      tst r3, #0x400000
005b728c  e9 ff ff 1a                                      bne #0x5b7238
005b7290  d3 6b e0 e7                                      ubfx r6, r3, #0x17, #1
005b7294  00 00 56 e3                                      cmp r6, #0
005b7298  e6 ff ff 1a                                      bne #0x5b7238
005b729c  cc 21 94 e5                                      ldr r2, [r4, #0x1cc]
005b72a0  00 00 52 e3                                      cmp r2, #0
005b72a4  e7 ff ff 0a                                      beq #0x5b7248
005b72a8  37 00 08 e3                                      movw r0, #0x8037
005b72ac  2d 5b f5 eb                                      bl #0x30df68
005b72b0  cc 61 84 e5                                      str r6, [r4, #0x1cc]
005b72b4  04 30 95 e5                                      ldr r3, [r5, #4]
005b72b8  d3 6c e0 e7                                      ubfx r6, r3, #0x19, #1
005b72bc  00 00 56 e3                                      cmp r6, #0
005b72c0  e3 ff ff 0a                                      beq #0x5b7254
005b72c4  04 00 a0 e1                                      mov r0, r4
005b72c8  05 10 a0 e1                                      mov r1, r5
005b72cc  6e e1 ff eb                                      bl #0x5af88c
005b72d0  04 30 95 e5                                      ldr r3, [r5, #4]
005b72d4  d3 6d e0 e7                                      ubfx r6, r3, #0x1b, #1
005b72d8  00 00 56 e3                                      cmp r6, #0
005b72dc  e2 ff ff 0a                                      beq #0x5b726c
005b72e0  04 00 a0 e1                                      mov r0, r4
005b72e4  05 10 a0 e1                                      mov r1, r5
005b72e8  e2 e1 ff eb                                      bl #0x5afa78
005b72ec  e1 ff ff ea                                      b #0x5b7278
005b72f0  b9 0e a0 e3                                      mov r0, #0xb90
005b72f4  1b 5b f5 eb                                      bl #0x30df68
005b72f8  d4 61 c4 e5                                      strb r6, [r4, #0x1d4]
005b72fc  dd ff ff ea                                      b #0x5b7278
005b7300  01 00 a0 e1                                      mov r0, r1
005b7304  05 10 a0 e1                                      mov r1, r5
005b7308  3a e0 ff eb                                      bl #0x5af3f8
005b730c  04 30 95 e5                                      ldr r3, [r5, #4]
005b7310  d3 68 e0 e7                                      ubfx r6, r3, #0x11, #1
005b7314  00 00 56 e3                                      cmp r6, #0
005b7318  bb ff ff 0a                                      beq #0x5b720c
005b731c  04 00 a0 e1                                      mov r0, r4
005b7320  05 10 a0 e1                                      mov r1, r5
005b7324  ca e0 ff eb                                      bl #0x5af654
005b7328  04 30 95 e5                                      ldr r3, [r5, #4]
005b732c  d3 69 e0 e7                                      ubfx r6, r3, #0x13, #1
005b7330  00 00 56 e3                                      cmp r6, #0
005b7334  ba ff ff 0a                                      beq #0x5b7224
005b7338  04 00 a0 e1                                      mov r0, r4
005b733c  05 10 a0 e1                                      mov r1, r5
005b7340  ed e0 ff eb                                      bl #0x5af6fc
005b7344  04 30 95 e5                                      ldr r3, [r5, #4]
005b7348  b8 ff ff ea                                      b #0x5b7230
005b734c  44 0b 00 e3                                      movw r0, #0xb44
005b7350  04 5b f5 eb                                      bl #0x30df68
005b7354  c5 61 c4 e5                                      strb r6, [r4, #0x1c5]
005b7358  04 30 95 e5                                      ldr r3, [r5, #4]
005b735c  ad ff ff ea                                      b #0x5b7218
005b7360  a0 00 08 e3                                      movw r0, #0x80a0
005b7364  ff 5a f5 eb                                      bl #0x30df68
005b7368  d1 61 c4 e5                                      strb r6, [r4, #0x1d1]
005b736c  04 30 95 e5                                      ldr r3, [r5, #4]
005b7370  ba ff ff ea                                      b #0x5b7260
005b7374  e2 0b 00 e3                                      movw r0, #0xbe2
005b7378  fa 5a f5 eb                                      bl #0x30df68
005b737c  c4 61 c4 e5                                      strb r6, [r4, #0x1c4]
005b7380  04 30 95 e5                                      ldr r3, [r5, #4]
005b7384  9d ff ff ea                                      b #0x5b7200
005b7388  71 0b 00 e3                                      movw r0, #0xb71
005b738c  f5 5a f5 eb                                      bl #0x30df68
005b7390  c6 61 c4 e5                                      strb r6, [r4, #0x1c6]
005b7394  04 30 95 e5                                      ldr r3, [r5, #4]
005b7398  a4 ff ff ea                                      b #0x5b7230

; Source: void_glitch_video_detail-1d8f95ea9161-001.asm
; FUNCTION 0x005b73e0, declared_size=264, range_size=264, mode=arm
; class-group: void glitch::video::detail
; alias: _ZN6glitch5video6detail17applyRenderStatesINS0_15CCommonGLDriverINS0_21CProgrammableGLDriverINS0_18CGLSLShaderHandlerEEENS1_33CProgrammableGLFunctionPointerSetEEEEEvPNS0_9CMaterialEhhPT_
; demangled: void glitch::video::detail::applyRenderStates<glitch::video::CCommonGLDriver<glitch::video::CProgrammableGLDriver<glitch::video::CGLSLShaderHandler>, glitch::video::detail::CProgrammableGLFunctionPointerSet> >(glitch::video::CMaterial*, unsigned char, unsigned char, glitch::video::CCommonGLDriver<glitch::video::CProgrammableGLDriver<glitch::video::CGLSLShaderHandler>, glitch::video::detail::CProgrammableGLFunctionPointerSet>*)
; decoder-mode: arm
005b73e0  f0 47 2d e9                                      push {r4, r5, r6, r7, r8, sb, sl, lr}
005b73e4  f0 c0 93 e5                                      ldr ip, [r3, #0xf0]
005b73e8  ec 40 9f e5                                      ldr r4, [pc, #0xec]
005b73ec  01 50 a0 e1                                      mov r5, r1
005b73f0  00 00 5c e3                                      cmp ip, #0
005b73f4  04 40 8f e0                                      add r4, pc, r4
005b73f8  02 70 a0 e1                                      mov r7, r2
005b73fc  04 60 90 e5                                      ldr r6, [r0, #4]
005b7400  02 00 00 0a                                      beq #0x5b7410
005b7404  04 20 9c e5                                      ldr r2, [ip, #4]
005b7408  02 00 56 e1                                      cmp r6, r2
005b740c  14 00 00 0a                                      beq #0x5b7464
005b7410  0c 80 a0 e3                                      mov r8, #0xc
005b7414  98 05 08 e0                                      mul r8, r8, r5
005b7418  18 20 96 e5                                      ldr r2, [r6, #0x18]
005b741c  bc a0 9f e5                                      ldr sl, [pc, #0xbc]
005b7420  bc 90 9f e5                                      ldr sb, [pc, #0xbc]
005b7424  08 20 82 e0                                      add r2, r2, r8
005b7428  08 20 92 e5                                      ldr r2, [r2, #8]
005b742c  34 00 a0 e3                                      mov r0, #0x34
005b7430  90 27 20 e0                                      mla r0, r0, r7, r2
005b7434  03 10 a0 e1                                      mov r1, r3
005b7438  66 ff ff eb                                      bl #0x5b71d8
005b743c  18 30 96 e5                                      ldr r3, [r6, #0x18]
005b7440  00 20 a0 e3                                      mov r2, #0
005b7444  08 80 83 e0                                      add r8, r3, r8
005b7448  08 30 98 e5                                      ldr r3, [r8, #8]
005b744c  30 20 c3 e5                                      strb r2, [r3, #0x30]
005b7450  0a 20 94 e7                                      ldr r2, [r4, sl]
005b7454  09 30 94 e7                                      ldr r3, [r4, sb]
005b7458  00 50 c2 e5                                      strb r5, [r2]
005b745c  00 70 c3 e5                                      strb r7, [r3]
005b7460  f0 87 bd e8                                      pop {r4, r5, r6, r7, r8, sb, sl, pc}
005b7464  0c 80 a0 e3                                      mov r8, #0xc
005b7468  98 01 08 e0                                      mul r8, r8, r1
005b746c  18 20 96 e5                                      ldr r2, [r6, #0x18]
005b7470  08 20 82 e0                                      add r2, r2, r8
005b7474  04 10 d2 e5                                      ldrb r1, [r2, #4]
005b7478  01 00 51 e3                                      cmp r1, #1
005b747c  03 00 00 9a                                      bls #0x5b7490
005b7480  08 20 92 e5                                      ldr r2, [r2, #8]
005b7484  54 a0 9f e5                                      ldr sl, [pc, #0x54]
005b7488  54 90 9f e5                                      ldr sb, [pc, #0x54]
005b748c  e6 ff ff ea                                      b #0x5b742c
005b7490  08 20 92 e5                                      ldr r2, [r2, #8]
005b7494  30 10 d2 e5                                      ldrb r1, [r2, #0x30]
005b7498  00 00 51 e3                                      cmp r1, #0
005b749c  02 00 00 0a                                      beq #0x5b74ac
005b74a0  38 a0 9f e5                                      ldr sl, [pc, #0x38]
005b74a4  38 90 9f e5                                      ldr sb, [pc, #0x38]
005b74a8  df ff ff ea                                      b #0x5b742c
005b74ac  30 90 9f e5                                      ldr sb, [pc, #0x30]
005b74b0  09 10 94 e7                                      ldr r1, [r4, sb]
005b74b4  00 10 d1 e5                                      ldrb r1, [r1]
005b74b8  07 00 51 e1                                      cmp r1, r7
005b74bc  1c a0 9f 15                                      ldrne sl, [pc, #0x1c]
005b74c0  d9 ff ff 1a                                      bne #0x5b742c
005b74c4  14 a0 9f e5                                      ldr sl, [pc, #0x14]
005b74c8  0a 10 94 e7                                      ldr r1, [r4, sl]
005b74cc  00 10 d1 e5                                      ldrb r1, [r1]
005b74d0  05 00 51 e1                                      cmp r1, r5
005b74d4  d4 ff ff 1a                                      bne #0x5b742c
005b74d8  dc ff ff ea                                      b #0x5b7450
; mapping-symbol data/literal pool
005b74dc  9c d6 3d 00 08 16 00 00 70 15 00 00              .byte 0x9c, 0xd6, 0x3d, 0x00, 0x08, 0x16, 0x00, 0x00, 0x70, 0x15, 0x00, 0x00

; Source: void_glitch_video_CCommonGLDriver_glitch_video_CProgrammableGLDriver_glitch_video_CGLSLSha-8379cfb2ad39-001.asm
; FUNCTION 0x005af3f8, declared_size=380, range_size=380, mode=arm
; class-group: void glitch::video::CCommonGLDriver<glitch::video::CProgrammableGLDriver<glitch::video::CGLSLShaderHandler>, glitch::video::detail::CProgrammableGLFunctionPointerSet>
; alias: _ZN6glitch5video15CCommonGLDriverINS0_21CProgrammableGLDriverINS0_18CGLSLShaderHandlerEEENS0_6detail33CProgrammableGLFunctionPointerSetEE21applyRenderStateBlendINS5_10renderpass12SRenderStateEEEvRKT_
; demangled: void glitch::video::CCommonGLDriver<glitch::video::CProgrammableGLDriver<glitch::video::CGLSLShaderHandler>, glitch::video::detail::CProgrammableGLFunctionPointerSet>::applyRenderStateBlend<glitch::video::detail::renderpass::SRenderState>(glitch::video::detail::renderpass::SRenderState const&)
; decoder-mode: arm
005af3f8  f0 4f 2d e9                                      push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
005af3fc  c4 31 d0 e5                                      ldrb r3, [r0, #0x1c4]
005af400  0c d0 4d e2                                      sub sp, sp, #0xc
005af404  00 40 a0 e1                                      mov r4, r0
005af408  00 00 53 e3                                      cmp r3, #0
005af40c  01 80 a0 e1                                      mov r8, r1
005af410  50 00 00 0a                                      beq #0x5af558
005af414  00 20 98 e5                                      ldr r2, [r8]
005af418  fc 31 94 e5                                      ldr r3, [r4, #0x1fc]
005af41c  52 5c e2 e7                                      ubfx r5, r2, #0x18, #3
005af420  03 00 55 e1                                      cmp r5, r3
005af424  06 00 00 0a                                      beq #0x5af444
005af428  3c 31 9f e5                                      ldr r3, [pc, #0x13c]
005af42c  03 30 8f e0                                      add r3, pc, r3
005af430  3c 30 83 e2                                      add r3, r3, #0x3c
005af434  05 01 93 e7                                      ldr r0, [r3, r5, lsl #2]
005af438  b5 7a f5 eb                                      bl #0x30df14
005af43c  fc 51 84 e5                                      str r5, [r4, #0x1fc]
005af440  00 20 98 e5                                      ldr r2, [r8]
005af444  0f 30 02 e2                                      and r3, r2, #0xf
005af448  00 50 a0 e3                                      mov r5, #0
005af44c  52 22 e3 e7                                      ubfx r2, r2, #4, #4
005af450  13 50 c7 e7                                      bfi r5, r3, #0, #8
005af454  00 12 94 e5                                      ldr r1, [r4, #0x200]
005af458  12 54 cf e7                                      bfi r5, r2, #8, #8
005af45c  1f 58 df e7                                      bfc r5, #0x10, #0x10
005af460  01 00 55 e1                                      cmp r5, r1
005af464  05 00 00 0a                                      beq #0x5af480
005af468  00 01 9f e5                                      ldr r0, [pc, #0x100]
005af46c  00 00 8f e0                                      add r0, pc, r0
005af470  02 11 90 e7                                      ldr r1, [r0, r2, lsl #2]
005af474  03 01 90 e7                                      ldr r0, [r0, r3, lsl #2]
005af478  e6 7b f5 eb                                      bl #0x30e418
005af47c  00 52 84 e5                                      str r5, [r4, #0x200]
005af480  08 70 d8 e5                                      ldrb r7, [r8, #8]
005af484  0b 60 d8 e5                                      ldrb r6, [r8, #0xb]
005af488  0a 50 d8 e5                                      ldrb r5, [r8, #0xa]
005af48c  04 32 d4 e5                                      ldrb r3, [r4, #0x204]
005af490  07 22 d4 e5                                      ldrb r2, [r4, #0x207]
005af494  09 80 d8 e5                                      ldrb r8, [r8, #9]
005af498  06 02 d4 e5                                      ldrb r0, [r4, #0x206]
005af49c  05 12 d4 e5                                      ldrb r1, [r4, #0x205]
005af4a0  03 20 cd e5                                      strb r2, [sp, #3]
005af4a4  02 00 cd e5                                      strb r0, [sp, #2]
005af4a8  01 10 cd e5                                      strb r1, [sp, #1]
005af4ac  00 30 cd e5                                      strb r3, [sp]
005af4b0  07 60 cd e5                                      strb r6, [sp, #7]
005af4b4  06 50 cd e5                                      strb r5, [sp, #6]
005af4b8  05 80 cd e5                                      strb r8, [sp, #5]
005af4bc  04 70 cd e5                                      strb r7, [sp, #4]
005af4c0  00 30 9d e5                                      ldr r3, [sp]
005af4c4  04 20 9d e5                                      ldr r2, [sp, #4]
005af4c8  03 00 52 e1                                      cmp r2, r3
005af4cc  1f 00 00 0a                                      beq #0x5af550
005af4d0  07 00 a0 e1                                      mov r0, r7
005af4d4  22 7d f5 eb                                      bl #0x30e964
005af4d8  81 10 08 e3                                      movw r1, #0x8081
005af4dc  80 1b 43 e3                                      movt r1, #0x3b80
005af4e0  21 7e f5 eb                                      bl #0x30ed6c
005af4e4  00 90 a0 e1                                      mov sb, r0
005af4e8  08 00 a0 e1                                      mov r0, r8
005af4ec  1c 7d f5 eb                                      bl #0x30e964
005af4f0  81 10 08 e3                                      movw r1, #0x8081
005af4f4  80 1b 43 e3                                      movt r1, #0x3b80
005af4f8  1b 7e f5 eb                                      bl #0x30ed6c
005af4fc  00 a0 a0 e1                                      mov sl, r0
005af500  05 00 a0 e1                                      mov r0, r5
005af504  16 7d f5 eb                                      bl #0x30e964
005af508  81 10 08 e3                                      movw r1, #0x8081
005af50c  80 1b 43 e3                                      movt r1, #0x3b80
005af510  15 7e f5 eb                                      bl #0x30ed6c
005af514  00 b0 a0 e1                                      mov fp, r0
005af518  06 00 a0 e1                                      mov r0, r6
005af51c  10 7d f5 eb                                      bl #0x30e964
005af520  81 10 08 e3                                      movw r1, #0x8081
005af524  80 1b 43 e3                                      movt r1, #0x3b80
005af528  0f 7e f5 eb                                      bl #0x30ed6c
005af52c  0a 10 a0 e1                                      mov r1, sl
005af530  00 30 a0 e1                                      mov r3, r0
005af534  0b 20 a0 e1                                      mov r2, fp
005af538  09 00 a0 e1                                      mov r0, sb
005af53c  88 7b f5 eb                                      bl #0x30e364
005af540  04 72 c4 e5                                      strb r7, [r4, #0x204]
005af544  07 62 c4 e5                                      strb r6, [r4, #0x207]
005af548  06 52 c4 e5                                      strb r5, [r4, #0x206]
005af54c  05 82 c4 e5                                      strb r8, [r4, #0x205]
005af550  0c d0 8d e2                                      add sp, sp, #0xc
005af554  f0 8f bd e8                                      pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
005af558  e2 0b 00 e3                                      movw r0, #0xbe2
005af55c  f8 7b f5 eb                                      bl #0x30e544
005af560  01 30 a0 e3                                      mov r3, #1
005af564  c4 31 c4 e5                                      strb r3, [r4, #0x1c4]
005af568  a9 ff ff ea                                      b #0x5af414
; mapping-symbol data/literal pool
005af56c  08 0c 33 00 c8 0b 33 00                          .byte 0x08, 0x0c, 0x33, 0x00, 0xc8, 0x0b, 0x33, 0x00

; Source: void_glitch_video_CCommonGLDriver_glitch_video_CProgrammableGLDriver_glitch_video_CGLSLSha-8379cfb2ad39-001.asm
; FUNCTION 0x005af654, declared_size=100, range_size=100, mode=arm
; class-group: void glitch::video::CCommonGLDriver<glitch::video::CProgrammableGLDriver<glitch::video::CGLSLShaderHandler>, glitch::video::detail::CProgrammableGLFunctionPointerSet>
; alias: _ZN6glitch5video15CCommonGLDriverINS0_21CProgrammableGLDriverINS0_18CGLSLShaderHandlerEEENS0_6detail33CProgrammableGLFunctionPointerSetEE24applyRenderStateCullFaceINS5_10renderpass12SRenderStateEEEvRKT_
; demangled: void glitch::video::CCommonGLDriver<glitch::video::CProgrammableGLDriver<glitch::video::CGLSLShaderHandler>, glitch::video::detail::CProgrammableGLFunctionPointerSet>::applyRenderStateCullFace<glitch::video::detail::renderpass::SRenderState>(glitch::video::detail::renderpass::SRenderState const&)
; decoder-mode: arm
005af654  70 40 2d e9                                      push {r4, r5, r6, lr}
005af658  c5 31 d0 e5                                      ldrb r3, [r0, #0x1c5]
005af65c  00 40 a0 e1                                      mov r4, r0
005af660  01 50 a0 e1                                      mov r5, r1
005af664  00 00 53 e3                                      cmp r3, #0
005af668  0c 00 00 0a                                      beq #0x5af6a0
005af66c  00 30 95 e5                                      ldr r3, [r5]
005af670  d8 21 94 e5                                      ldr r2, [r4, #0x1d8]
005af674  23 3f a0 e1                                      lsr r3, r3, #0x1e
005af678  02 00 53 e1                                      cmp r3, r2
005af67c  06 00 00 0a                                      beq #0x5af69c
005af680  2c 20 9f e5                                      ldr r2, [pc, #0x2c]
005af684  d8 31 84 e5                                      str r3, [r4, #0x1d8]
005af688  02 20 8f e0                                      add r2, pc, r2
005af68c  50 20 82 e2                                      add r2, r2, #0x50
005af690  03 01 92 e7                                      ldr r0, [r2, r3, lsl #2]
005af694  70 40 bd e8                                      pop {r4, r5, r6, lr}
005af698  84 7c f5 ea                                      b #0x30e8b0
005af69c  70 80 bd e8                                      pop {r4, r5, r6, pc}
005af6a0  44 0b 00 e3                                      movw r0, #0xb44
005af6a4  a6 7b f5 eb                                      bl #0x30e544
005af6a8  01 30 a0 e3                                      mov r3, #1
005af6ac  c5 31 c4 e5                                      strb r3, [r4, #0x1c5]
005af6b0  ed ff ff ea                                      b #0x5af66c
; mapping-symbol data/literal pool
005af6b4  ac 09 33 00                                      .byte 0xac, 0x09, 0x33, 0x00

; Source: void_glitch_video_CCommonGLDriver_glitch_video_CProgrammableGLDriver_glitch_video_CGLSLSha-8379cfb2ad39-001.asm
; FUNCTION 0x005af6fc, declared_size=100, range_size=100, mode=arm
; class-group: void glitch::video::CCommonGLDriver<glitch::video::CProgrammableGLDriver<glitch::video::CGLSLShaderHandler>, glitch::video::detail::CProgrammableGLFunctionPointerSet>
; alias: _ZN6glitch5video15CCommonGLDriverINS0_21CProgrammableGLDriverINS0_18CGLSLShaderHandlerEEENS0_6detail33CProgrammableGLFunctionPointerSetEE25applyRenderStateDepthTestINS5_10renderpass12SRenderStateEEEvRKT_
; demangled: void glitch::video::CCommonGLDriver<glitch::video::CProgrammableGLDriver<glitch::video::CGLSLShaderHandler>, glitch::video::detail::CProgrammableGLFunctionPointerSet>::applyRenderStateDepthTest<glitch::video::detail::renderpass::SRenderState>(glitch::video::detail::renderpass::SRenderState const&)
; decoder-mode: arm
005af6fc  70 40 2d e9                                      push {r4, r5, r6, lr}
005af700  c6 31 d0 e5                                      ldrb r3, [r0, #0x1c6]
005af704  00 40 a0 e1                                      mov r4, r0
005af708  01 50 a0 e1                                      mov r5, r1
005af70c  00 00 53 e3                                      cmp r3, #0
005af710  0c 00 00 0a                                      beq #0x5af748
005af714  00 30 95 e5                                      ldr r3, [r5]
005af718  e0 21 94 e5                                      ldr r2, [r4, #0x1e0]
005af71c  d3 3d e2 e7                                      ubfx r3, r3, #0x1b, #3
005af720  02 00 53 e1                                      cmp r3, r2
005af724  06 00 00 0a                                      beq #0x5af744
005af728  2c 20 9f e5                                      ldr r2, [pc, #0x2c]
005af72c  e0 31 84 e5                                      str r3, [r4, #0x1e0]
005af730  02 20 8f e0                                      add r2, pc, r2
005af734  5c 20 82 e2                                      add r2, r2, #0x5c
005af738  03 01 92 e7                                      ldr r0, [r2, r3, lsl #2]
005af73c  70 40 bd e8                                      pop {r4, r5, r6, lr}
005af740  d4 7a f5 ea                                      b #0x30e298
005af744  70 80 bd e8                                      pop {r4, r5, r6, pc}
005af748  71 0b 00 e3                                      movw r0, #0xb71
005af74c  7c 7b f5 eb                                      bl #0x30e544
005af750  01 30 a0 e3                                      mov r3, #1
005af754  c6 31 c4 e5                                      strb r3, [r4, #0x1c6]
005af758  ed ff ff ea                                      b #0x5af714
; mapping-symbol data/literal pool
005af75c  04 09 33 00                                      .byte 0x04, 0x09, 0x33, 0x00

; Source: void_glitch_video_CCommonGLDriver_glitch_video_CProgrammableGLDriver_glitch_video_CGLSLSha-8379cfb2ad39-001.asm
; FUNCTION 0x005af7a4, declared_size=132, range_size=132, mode=arm
; class-group: void glitch::video::CCommonGLDriver<glitch::video::CProgrammableGLDriver<glitch::video::CGLSLShaderHandler>, glitch::video::detail::CProgrammableGLFunctionPointerSet>
; alias: _ZN6glitch5video15CCommonGLDriverINS0_21CProgrammableGLDriverINS0_18CGLSLShaderHandlerEEENS0_6detail33CProgrammableGLFunctionPointerSetEE29applyRenderStatePolygonOffsetINS5_10renderpass12SRenderStateEEEvRKT_
; demangled: void glitch::video::CCommonGLDriver<glitch::video::CProgrammableGLDriver<glitch::video::CGLSLShaderHandler>, glitch::video::detail::CProgrammableGLFunctionPointerSet>::applyRenderStatePolygonOffset<glitch::video::detail::renderpass::SRenderState>(glitch::video::detail::renderpass::SRenderState const&)
; decoder-mode: arm
005af7a4  70 40 2d e9                                      push {r4, r5, r6, lr}
005af7a8  cc 31 d0 e5                                      ldrb r3, [r0, #0x1cc]
005af7ac  00 40 a0 e1                                      mov r4, r0
005af7b0  01 50 a0 e1                                      mov r5, r1
005af7b4  00 00 53 e3                                      cmp r3, #0
005af7b8  02 00 00 1a                                      bne #0x5af7c8
005af7bc  04 30 91 e5                                      ldr r3, [r1, #4]
005af7c0  02 06 13 e3                                      tst r3, #0x200000
005af7c4  12 00 00 1a                                      bne #0x5af814
005af7c8  14 60 95 e5                                      ldr r6, [r5, #0x14]
005af7cc  20 12 94 e5                                      ldr r1, [r4, #0x220]
005af7d0  18 50 95 e5                                      ldr r5, [r5, #0x18]
005af7d4  06 00 a0 e1                                      mov r0, r6
005af7d8  eb 79 f5 eb                                      bl #0x30df8c
005af7dc  00 00 50 e3                                      cmp r0, #0
005af7e0  05 00 00 1a                                      bne #0x5af7fc
005af7e4  06 00 a0 e1                                      mov r0, r6
005af7e8  05 10 a0 e1                                      mov r1, r5
005af7ec  bf 7c f5 eb                                      bl #0x30eaf0
005af7f0  24 52 84 e5                                      str r5, [r4, #0x224]
005af7f4  20 62 84 e5                                      str r6, [r4, #0x220]
005af7f8  70 80 bd e8                                      pop {r4, r5, r6, pc}
005af7fc  05 00 a0 e1                                      mov r0, r5
005af800  24 12 94 e5                                      ldr r1, [r4, #0x224]
005af804  e0 79 f5 eb                                      bl #0x30df8c
005af808  00 00 50 e3                                      cmp r0, #0
005af80c  f4 ff ff 0a                                      beq #0x5af7e4
005af810  70 80 bd e8                                      pop {r4, r5, r6, pc}
005af814  37 00 08 e3                                      movw r0, #0x8037
005af818  49 7b f5 eb                                      bl #0x30e544
005af81c  01 30 a0 e3                                      mov r3, #1
005af820  cc 31 c4 e5                                      strb r3, [r4, #0x1cc]
005af824  e7 ff ff ea                                      b #0x5af7c8

; Source: void_glitch_video_CCommonGLDriver_glitch_video_CProgrammableGLDriver_glitch_video_CGLSLSha-8379cfb2ad39-001.asm
; FUNCTION 0x005af88c, declared_size=108, range_size=108, mode=arm
; class-group: void glitch::video::CCommonGLDriver<glitch::video::CProgrammableGLDriver<glitch::video::CGLSLShaderHandler>, glitch::video::detail::CProgrammableGLFunctionPointerSet>
; alias: _ZN6glitch5video15CCommonGLDriverINS0_21CProgrammableGLDriverINS0_18CGLSLShaderHandlerEEENS0_6detail33CProgrammableGLFunctionPointerSetEE30applyRenderStateSampleCoverageINS5_10renderpass12SRenderStateEEEvRKT_
; demangled: void glitch::video::CCommonGLDriver<glitch::video::CProgrammableGLDriver<glitch::video::CGLSLShaderHandler>, glitch::video::detail::CProgrammableGLFunctionPointerSet>::applyRenderStateSampleCoverage<glitch::video::detail::renderpass::SRenderState>(glitch::video::detail::renderpass::SRenderState const&)
; decoder-mode: arm
005af88c  70 40 2d e9                                      push {r4, r5, r6, lr}
005af890  d1 31 d0 e5                                      ldrb r3, [r0, #0x1d1]
005af894  00 40 a0 e1                                      mov r4, r0
005af898  01 60 a0 e1                                      mov r6, r1
005af89c  00 00 53 e3                                      cmp r3, #0
005af8a0  11 00 00 0a                                      beq #0x5af8ec
005af8a4  1c 50 96 e5                                      ldr r5, [r6, #0x1c]
005af8a8  28 12 94 e5                                      ldr r1, [r4, #0x228]
005af8ac  04 60 96 e5                                      ldr r6, [r6, #4]
005af8b0  05 00 a0 e1                                      mov r0, r5
005af8b4  b4 79 f5 eb                                      bl #0x30df8c
005af8b8  00 00 50 e3                                      cmp r0, #0
005af8bc  56 6d e0 e7                                      ubfx r6, r6, #0x1a, #1
005af8c0  05 00 00 1a                                      bne #0x5af8dc
005af8c4  05 00 a0 e1                                      mov r0, r5
005af8c8  06 10 a0 e1                                      mov r1, r6
005af8cc  d2 61 c4 e5                                      strb r6, [r4, #0x1d2]
005af8d0  28 52 84 e5                                      str r5, [r4, #0x228]
005af8d4  70 40 bd e8                                      pop {r4, r5, r6, lr}
005af8d8  3f 79 f5 ea                                      b #0x30dddc
005af8dc  d2 31 d4 e5                                      ldrb r3, [r4, #0x1d2]
005af8e0  06 00 53 e1                                      cmp r3, r6
005af8e4  f6 ff ff 1a                                      bne #0x5af8c4
005af8e8  70 80 bd e8                                      pop {r4, r5, r6, pc}
005af8ec  a0 00 08 e3                                      movw r0, #0x80a0
005af8f0  13 7b f5 eb                                      bl #0x30e544
005af8f4  ea ff ff ea                                      b #0x5af8a4

; Source: void_glitch_video_CCommonGLDriver_glitch_video_CProgrammableGLDriver_glitch_video_CGLSLSha-8379cfb2ad39-001.asm
; FUNCTION 0x005afa78, declared_size=212, range_size=212, mode=arm
; class-group: void glitch::video::CCommonGLDriver<glitch::video::CProgrammableGLDriver<glitch::video::CGLSLShaderHandler>, glitch::video::detail::CProgrammableGLFunctionPointerSet>
; alias: _ZN6glitch5video15CCommonGLDriverINS0_21CProgrammableGLDriverINS0_18CGLSLShaderHandlerEEENS0_6detail33CProgrammableGLFunctionPointerSetEE23applyRenderStateStencilINS5_10renderpass12SRenderStateEEEvRKT_
; demangled: void glitch::video::CCommonGLDriver<glitch::video::CProgrammableGLDriver<glitch::video::CGLSLShaderHandler>, glitch::video::detail::CProgrammableGLFunctionPointerSet>::applyRenderStateStencil<glitch::video::detail::renderpass::SRenderState>(glitch::video::detail::renderpass::SRenderState const&)
; decoder-mode: arm
005afa78  70 40 2d e9                                      push {r4, r5, r6, lr}
005afa7c  d4 31 d0 e5                                      ldrb r3, [r0, #0x1d4]
005afa80  00 50 a0 e1                                      mov r5, r0
005afa84  01 60 a0 e1                                      mov r6, r1
005afa88  00 00 53 e3                                      cmp r3, #0
005afa8c  27 00 00 0a                                      beq #0x5afb30
005afa90  04 00 96 e5                                      ldr r0, [r6, #4]
005afa94  f0 41 95 e5                                      ldr r4, [r5, #0x1f0]
005afa98  00 20 96 e5                                      ldr r2, [r6]
005afa9c  07 30 00 e2                                      and r3, r0, #7
005afaa0  04 c0 a0 e1                                      mov ip, r4
005afaa4  52 14 e7 e7                                      ubfx r1, r2, #8, #8
005afaa8  13 40 c7 e7                                      bfi r4, r3, #0, #8
005afaac  11 44 cf e7                                      bfi r4, r1, #8, #8
005afab0  52 28 e7 e7                                      ubfx r2, r2, #0x10, #8
005afab4  12 48 d7 e7                                      bfi r4, r2, #0x10, #8
005afab8  0c 00 54 e1                                      cmp r4, ip
005afabc  06 00 00 0a                                      beq #0x5afadc
005afac0  7c 00 9f e5                                      ldr r0, [pc, #0x7c]
005afac4  00 00 8f e0                                      add r0, pc, r0
005afac8  03 31 80 e0                                      add r3, r0, r3, lsl #2
005afacc  5c 00 93 e5                                      ldr r0, [r3, #0x5c]
005afad0  ce 7a f5 eb                                      bl #0x30e610
005afad4  f0 41 85 e5                                      str r4, [r5, #0x1f0]
005afad8  04 00 96 e5                                      ldr r0, [r6, #4]
005afadc  f4 41 95 e5                                      ldr r4, [r5, #0x1f4]
005afae0  d0 11 e2 e7                                      ubfx r1, r0, #3, #3
005afae4  50 23 e2 e7                                      ubfx r2, r0, #6, #3
005afae8  04 30 a0 e1                                      mov r3, r4
005afaec  11 40 c7 e7                                      bfi r4, r1, #0, #8
005afaf0  12 44 cf e7                                      bfi r4, r2, #8, #8
005afaf4  d0 04 e2 e7                                      ubfx r0, r0, #9, #3
005afaf8  10 48 d7 e7                                      bfi r4, r0, #0x10, #8
005afafc  03 00 54 e1                                      cmp r4, r3
005afb00  09 00 00 0a                                      beq #0x5afb2c
005afb04  3c c0 9f e5                                      ldr ip, [pc, #0x3c]
005afb08  0c c0 8f e0                                      add ip, pc, ip
005afb0c  00 31 8c e0                                      add r3, ip, r0, lsl #2
005afb10  01 11 8c e0                                      add r1, ip, r1, lsl #2
005afb14  02 21 8c e0                                      add r2, ip, r2, lsl #2
005afb18  7c 00 91 e5                                      ldr r0, [r1, #0x7c]
005afb1c  7c 10 92 e5                                      ldr r1, [r2, #0x7c]
005afb20  7c 20 93 e5                                      ldr r2, [r3, #0x7c]
005afb24  16 7b f5 eb                                      bl #0x30e784
005afb28  f0 41 85 e5                                      str r4, [r5, #0x1f0]
005afb2c  70 80 bd e8                                      pop {r4, r5, r6, pc}
005afb30  b9 0e a0 e3                                      mov r0, #0xb90
005afb34  82 7a f5 eb                                      bl #0x30e544
005afb38  01 30 a0 e3                                      mov r3, #1
005afb3c  d4 31 c5 e5                                      strb r3, [r5, #0x1d4]
005afb40  d2 ff ff ea                                      b #0x5afa90
; mapping-symbol data/literal pool
005afb44  70 05 33 00 2c 05 33 00                          .byte 0x70, 0x05, 0x33, 0x00, 0x2c, 0x05, 0x33, 0x00

; Source: void_glitch_video_CCommonGLDriver_glitch_video_CProgrammableGLDriver_glitch_video_CGLSLSha-8379cfb2ad39-001.asm
; FUNCTION 0x005b70cc, declared_size=268, range_size=268, mode=arm
; class-group: void glitch::video::CCommonGLDriver<glitch::video::CProgrammableGLDriver<glitch::video::CGLSLShaderHandler>, glitch::video::detail::CProgrammableGLFunctionPointerSet>
; alias: _ZN6glitch5video15CCommonGLDriverINS0_21CProgrammableGLDriverINS0_18CGLSLShaderHandlerEEENS0_6detail33CProgrammableGLFunctionPointerSetEE26applyRenderStateNonGroupedINS5_10renderpass12SRenderStateEEEvRKT_
; demangled: void glitch::video::CCommonGLDriver<glitch::video::CProgrammableGLDriver<glitch::video::CGLSLShaderHandler>, glitch::video::detail::CProgrammableGLFunctionPointerSet>::applyRenderStateNonGrouped<glitch::video::detail::renderpass::SRenderState>(glitch::video::detail::renderpass::SRenderState const&)
; decoder-mode: arm
005b70cc  70 40 2d e9                                      push {r4, r5, r6, lr}
005b70d0  00 40 a0 e1                                      mov r4, r0
005b70d4  04 00 91 e5                                      ldr r0, [r1, #4]
005b70d8  dc 31 94 e5                                      ldr r3, [r4, #0x1dc]
005b70dc  01 50 a0 e1                                      mov r5, r1
005b70e0  50 69 e0 e7                                      ubfx r6, r0, #0x12, #1
005b70e4  03 00 56 e1                                      cmp r6, r3
005b70e8  0a 00 00 0a                                      beq #0x5b7118
005b70ec  a0 34 d4 e5                                      ldrb r3, [r4, #0x4a0]
005b70f0  00 00 53 e3                                      cmp r3, #0
005b70f4  d8 30 9f e5                                      ldr r3, [pc, #0xd8]
005b70f8  01 20 66 12                                      rsbne r2, r6, #1
005b70fc  06 20 a0 01                                      moveq r2, r6
005b7100  03 30 8f e0                                      add r3, pc, r3
005b7104  02 31 83 e0                                      add r3, r3, r2, lsl #2
005b7108  9c 00 93 e5                                      ldr r0, [r3, #0x9c]
005b710c  59 5b f5 eb                                      bl #0x30de78
005b7110  dc 61 84 e5                                      str r6, [r4, #0x1dc]
005b7114  04 00 95 e5                                      ldr r0, [r5, #4]
005b7118  c7 31 d4 e5                                      ldrb r3, [r4, #0x1c7]
005b711c  50 0a e0 e7                                      ubfx r0, r0, #0x14, #1
005b7120  00 00 53 e1                                      cmp r3, r0
005b7124  01 00 00 0a                                      beq #0x5b7130
005b7128  c7 01 c4 e5                                      strb r0, [r4, #0x1c7]
005b712c  41 5c f5 eb                                      bl #0x30e238
005b7130  0c 60 95 e5                                      ldr r6, [r5, #0xc]
005b7134  18 12 94 e5                                      ldr r1, [r4, #0x218]
005b7138  06 00 a0 e1                                      mov r0, r6
005b713c  92 5b f5 eb                                      bl #0x30df8c
005b7140  00 00 50 e3                                      cmp r0, #0
005b7144  1e 00 00 0a                                      beq #0x5b71c4
005b7148  10 60 95 e5                                      ldr r6, [r5, #0x10]
005b714c  1c 12 94 e5                                      ldr r1, [r4, #0x21c]
005b7150  06 00 a0 e1                                      mov r0, r6
005b7154  8c 5b f5 eb                                      bl #0x30df8c
005b7158  00 00 50 e3                                      cmp r0, #0
005b715c  1c 62 84 05                                      streq r6, [r4, #0x21c]
005b7160  04 30 95 e5                                      ldr r3, [r5, #4]
005b7164  e4 11 94 e5                                      ldr r1, [r4, #0x1e4]
005b7168  53 26 e1 e7                                      ubfx r2, r3, #0xc, #2
005b716c  01 00 52 e1                                      cmp r2, r1
005b7170  e4 21 84 15                                      strne r2, [r4, #0x1e4]
005b7174  04 30 95 15                                      ldrne r3, [r5, #4]
005b7178  e8 11 94 e5                                      ldr r1, [r4, #0x1e8]
005b717c  53 27 e1 e7                                      ubfx r2, r3, #0xe, #2
005b7180  01 00 52 e1                                      cmp r2, r1
005b7184  e8 21 84 15                                      strne r2, [r4, #0x1e8]
005b7188  04 30 95 15                                      ldrne r3, [r5, #4]
005b718c  d0 21 d4 e5                                      ldrb r2, [r4, #0x1d0]
005b7190  53 3c e0 e7                                      ubfx r3, r3, #0x18, #1
005b7194  03 00 52 e1                                      cmp r2, r3
005b7198  08 00 00 0a                                      beq #0x5b71c0
005b719c  00 00 53 e3                                      cmp r3, #0
005b71a0  d0 31 c4 e5                                      strb r3, [r4, #0x1d0]
005b71a4  02 00 00 1a                                      bne #0x5b71b4
005b71a8  9e 00 08 e3                                      movw r0, #0x809e
005b71ac  70 40 bd e8                                      pop {r4, r5, r6, lr}
005b71b0  6c 5b f5 ea                                      b #0x30df68
005b71b4  9e 00 08 e3                                      movw r0, #0x809e
005b71b8  70 40 bd e8                                      pop {r4, r5, r6, lr}
005b71bc  e0 5c f5 ea                                      b #0x30e544
005b71c0  70 80 bd e8                                      pop {r4, r5, r6, pc}
005b71c4  18 62 84 e5                                      str r6, [r4, #0x218]
005b71c8  06 00 a0 e1                                      mov r0, r6
005b71cc  c2 5e f5 eb                                      bl #0x30ecdc
005b71d0  dc ff ff ea                                      b #0x5b7148
; mapping-symbol data/literal pool
005b71d4  34 8f 32 00                                      .byte 0x34, 0x8f, 0x32, 0x00
