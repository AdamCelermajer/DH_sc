006361e8 push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
006361ec ldr r1, [pc, #0x964]
006361f0 sub sp, sp, #0xac
006361f4 ldr ip, [sp, #0xd0]
006361f8 str r1, [sp, #0x68]
006361fc str r0, [sp, #0x64]
00636200 ldr r1, [ip]
00636204 ldr r0, [sp, #0x68]
00636208 str r3, [sp, #0x40]
0063620c cmp r1, ip
00636210 ldreq r1, [sp, #0x64]
00636214 add r0, pc, r0
00636218 moveq r3, #0
0063621c str r0, [sp, #0x68]
00636220 str r2, [sp, #0x3c]
00636224 streq r3, [r1]
00636228 beq #0x636b00
0063622c ldr r2, [sp, #0x3c]
00636230 ldr r2, [r2, #0xdc]
00636234 str r2, [sp, #0x24]
00636238 bl #0x534254
0063623c str r0, [sp, #0x6c]
00636240 mov r0, #1
00636244 bl #0x534268
00636248 ldr ip, [sp, #0xd0]
0063624c ldr r3, [ip]
00636250 cmp ip, r3
00636254 moveq r4, #0
00636258 moveq r0, r4
0063625c beq #0x6362a4
00636260 ldr ip, [sp, #0xd0]
00636264 mov r4, #0
00636268 mov r0, r4
0063626c ldr r2, [r3, #0x10]
00636270 ldr r3, [r3]
00636274 ldr r1, [r2, #0x20]
00636278 ldr r2, [r2, #0x28]
0063627c cmp r4, r1
00636280 movlo r4, r1
00636284 cmp r0, r2
00636288 movlo r0, r2
0063628c cmp ip, r3
00636290 bne #0x63626c
00636294 cmp r0, #0
00636298 beq #0x6362a4
0063629c lsl r0, r0, #2
006362a0 bl #0x5345f4
006362a4 cmp r4, #0
006362a8 str r0, [sp, #0x2c]
006362ac streq r4, [sp, #0x54]
006362b0 beq #0x6362c0
006362b4 lsl r0, r4, #2
006362b8 bl #0x5345f4
006362bc str r0, [sp, #0x54]
006362c0 ldr r0, [sp, #0x24]
006362c4 ldr r1, [sp, #0x40]
006362c8 mov r2, #1
006362cc bl #0x5ddaec
006362d0 cmp r0, #0
006362d4 str r0, [sp, #0x70]
006362d8 beq #0x6366b4
006362dc ldr r0, [sp, #0xd0]
006362e0 ldr r1, [sp, #0xd0]
006362e4 ldr r0, [r0]
006362e8 cmp r1, r0
006362ec str r0, [sp, #0x60]
006362f0 beq #0x6366b4
006362f4 ldr r3, [pc, #0x860]
006362f8 mov r2, #1
006362fc str r2, [sp, #0x34]
00636300 add r3, pc, r3
00636304 str r3, [sp, #0x44]
00636308 ldr r3, [pc, #0x850]
0063630c add r3, pc, r3
00636310 str r3, [sp, #0x74]
00636314 add r3, sp, #0xa0
00636318 str r3, [sp, #0x4c]
0063631c ldr ip, [sp, #0x60]
00636320 ldr ip, [ip, #0x10]
00636324 str ip, [sp, #0x28]
00636328 ldr r0, [ip, #0x20]
0063632c str r0, [sp, #0x38]
00636330 ldr r3, [ip, #0x28]
00636334 sbfx r2, r0, #0, #0x1e
00636338 cmp r3, #0
0063633c movle r3, #0
00636340 movgt r3, #1
00636344 cmp r2, #0
00636348 str r3, [sp, #0x50]
0063634c ble #0x636378
00636350 ldr r0, [sp, #0x54]
00636354 mov r3, #0
00636358 mov r1, r3
0063635c str r1, [r0, r3, lsl #2]
00636360 add r3, r3, #1
00636364 cmp r3, r2
00636368 bne #0x63635c
0063636c ldr r1, [sp, #0x28]
00636370 ldr r1, [r1, #0x20]
00636374 str r1, [sp, #0x38]
00636378 ldr r2, [sp, #0x38]
0063637c cmp r2, #0
00636380 movle r8, #0
00636384 ble #0x63653c
00636388 ldr ip, [sp, #0x50]
0063638c mov r3, #0
00636390 add r0, sp, #0xa4
00636394 eor ip, ip, #1
00636398 str r3, [sp, #0x18]
0063639c str r3, [sp, #0x1c]
006363a0 str r3, [sp, #0x20]
006363a4 mov r8, r3
006363a8 str ip, [sp, #0x48]
006363ac add sl, sp, #0x78
006363b0 add fp, sp, #0x9c
006363b4 str r0, [sp, #0x14]
006363b8 ldr sb, [sp, #0x24]
006363bc b #0x636410
006363c0 ldr r1, [sp, #0x18]
006363c4 ldr r0, [sp, #0x54]
006363c8 add r0, r0, r1
006363cc str r0, [sp, #0x30]
006363d0 ldr r1, [sp, #0x30]
006363d4 ldr r5, [r1]
006363d8 cmp r5, #0
006363dc beq #0x636460
006363e0 ldr r2, [sp, #0x20]
006363e4 ldr ip, [sp, #0x1c]
006363e8 ldr r0, [sp, #0x18]
006363ec ldr r3, [sp, #0x38]
006363f0 add r2, r2, #1
006363f4 add ip, ip, #0xc
006363f8 add r0, r0, #4
006363fc cmp r2, r3
00636400 str r2, [sp, #0x20]
00636404 str ip, [sp, #0x1c]
00636408 str r0, [sp, #0x18]
0063640c beq #0x63653c
00636410 ldr r1, [sp, #0x28]
00636414 ldr r2, [sp, #0x34]
00636418 ldr ip, [sp, #0x1c]
0063641c ldr r3, [r1, #0x24]
00636420 cmp r2, #0
00636424 add r7, r3, ip
00636428 bne #0x6363c0
0063642c ldr r2, [sp, #0x1c]
00636430 mov r0, sb
00636434 ldr r1, [r3, r2]
00636438 bl #0x5dbb48
0063643c ldr ip, [sp, #0x54]
00636440 ldr r3, [sp, #0x18]
00636444 str r0, [ip, r3]
00636448 add r0, ip, r3
0063644c str r0, [sp, #0x30]
00636450 ldr r1, [sp, #0x30]
00636454 ldr r5, [r1]
00636458 cmp r5, #0
0063645c bne #0x6363e0
00636460 mov r0, sb
00636464 ldr r1, [r7]
00636468 mov r2, #1
0063646c bl #0x5dd80c
00636470 cmp r0, #0
00636474 beq #0x6363e0
00636478 ldr r2, [r7, #4]
0063647c cmp r2, #0
00636480 str r2, [sp, #0x10]
00636484 ble #0x636520
00636488 mov r6, r5
0063648c ldr r4, [r7, #8]
00636490 ldr r3, [sp, #0x3c]
00636494 ldr r0, [sp, #0x4c]
00636498 add r4, r4, r5
0063649c mov r2, r4
006364a0 ldr r1, [r3, #0xd8]
006364a4 bl #0x634b30
006364a8 ldr r3, [sp, #0xa0]
006364ac add r1, r4, #0x1c
006364b0 mov r0, sl
006364b4 cmp r3, #0
006364b8 str r3, [sp, #0x9c]
006364bc ldrne r2, [r3, #4]
006364c0 add r6, r6, #1
006364c4 add r5, r5, #0x74
006364c8 addne r2, r2, #1
006364cc strne r2, [r3, #4]
006364d0 bl #0x5d7a10
006364d4 mov r0, sb
006364d8 mov r1, fp
006364dc mov r2, sl
006364e0 ldr r3, [sp, #0x14]
006364e4 bl #0x5dcf2c
006364e8 ldr r0, [sp, #0x9c]
006364ec cmp r0, #0
006364f0 beq #0x6364f8
006364f4 bl #0x31d584
006364f8 ldr r3, [r4, #0x6c]
006364fc ldr r0, [sp, #0xa0]
00636500 cmp r3, #0
00636504 movgt r8, #1
00636508 cmp r0, #0
0063650c beq #0x636514
00636510 bl #0x31d584
00636514 ldr ip, [sp, #0x10]
00636518 cmp r6, ip
0063651c bne #0x63648c
00636520 ldr r1, [sp, #0x48]
00636524 mov r0, sb
00636528 mov r2, #0
0063652c bl #0x5dd664
00636530 ldr r1, [sp, #0x30]
00636534 str r0, [r1]
00636538 b #0x6363e0
0063653c ldr r1, [sp, #0x50]
00636540 cmp r1, #0
00636544 bne #0x636550
00636548 cmp r8, #0
0063654c beq #0x636694
00636550 ldr r2, [sp, #0x28]
00636554 ldr sl, [r2, #0x28]
00636558 sbfx r2, sl, #0, #0x1e
0063655c cmp r2, #0
00636560 ble #0x636588
00636564 ldr r0, [sp, #0x2c]
00636568 mov r3, #0
0063656c mov r1, r3
00636570 str r1, [r0, r3, lsl #2]
00636574 add r3, r3, #1
00636578 cmp r3, r2
0063657c bne #0x636570
00636580 ldr r3, [sp, #0x28]
00636584 ldr sl, [r3, #0x28]
00636588 cmp sl, #0
0063658c ble #0x636834
00636590 mov r4, #0
00636594 mov r6, r4
00636598 mov r8, r4
0063659c ldr sb, [sp, #0x2c]
006365a0 ldr fp, [sp, #0x74]
006365a4 b #0x6365c8
006365a8 ldr r3, [r7]
006365ac add r8, r8, #1
006365b0 add r6, r6, #0x18
006365b4 cmp r3, #0
006365b8 beq #0x63662c
006365bc cmp r8, sl
006365c0 add r4, r4, #4
006365c4 beq #0x636834
006365c8 ldr ip, [sp, #0x28]
006365cc ldr r2, [sp, #0x40]
006365d0 mov r0, #2
006365d4 ldr r3, [ip, #0x2c]
006365d8 mov r1, fp
006365dc add r7, sb, r4
006365e0 add r5, r3, r6
006365e4 ldr ip, [r5, #0xc]
006365e8 cmp ip, #1
006365ec ble #0x6365f8
006365f0 ldr r3, [r3, r6]
006365f4 bl #0x60b034
006365f8 ldr r0, [sp, #0x34]
006365fc cmp r0, #0
00636600 bne #0x6365a8
00636604 ldr r1, [r5]
00636608 ldr r0, [sp, #0x24]
0063660c bl #0x631b58
00636610 add r7, sb, r4
00636614 str r0, [sb, r4]
00636618 ldr r3, [r7]
0063661c add r8, r8, #1
00636620 add r6, r6, #0x18
00636624 cmp r3, #0
00636628 bne #0x6365bc
0063662c ldr r3, [r5, #0x10]
00636630 ldr r2, [r5, #8]
00636634 ldr r0, [sp, #0x24]
00636638 ldr r3, [r3]
0063663c ldr r1, [r5]
00636640 bl #0x631c50
00636644 str r0, [r7]
00636648 b #0x6365bc
0063664c ldr r0, [sp, #0x30]
00636650 ldr r2, [sp, #0x34]
00636654 ldr r1, [sp, #0x48]
00636658 add r0, r0, #1
0063665c add r2, r2, #0x74
00636660 cmp r0, r1
00636664 str r0, [sp, #0x30]
00636668 str r2, [sp, #0x34]
0063666c bne #0x6368ac
00636670 ldr r3, [sp, #0x50]
00636674 ldr r0, [sp, #0x58]
00636678 ldr ip, [sp, #0x5c]
0063667c add r3, r3, #1
00636680 add r0, r0, #0xc
00636684 cmp r3, ip
00636688 str r3, [sp, #0x50]
0063668c str r0, [sp, #0x58]
00636690 bne #0x636854
00636694 ldr r1, [sp, #0x60]
00636698 ldr r3, [sp, #0xd0]
0063669c mov r2, #0
006366a0 ldr r1, [r1]
006366a4 str r2, [sp, #0x34]
006366a8 cmp r3, r1
006366ac str r1, [sp, #0x60]
006366b0 bne #0x63631c
006366b4 ldr r0, [sp, #0x24]
006366b8 bl #0x5dddb4
006366bc ldr ip, [sp, #0x24]
006366c0 ldr r3, [ip, #0x18]
006366c4 ldr r2, [ip, #0x1c]
006366c8 rsb r2, r3, r2
006366cc cmp r0, r2, asr #3
006366d0 addlo r3, r3, r0, lsl #3
006366d4 blo #0x6366e4
006366d8 ldr r3, [pc, #0x484]
006366dc ldr r0, [sp, #0x68]
006366e0 ldr r3, [r0, r3]
006366e4 ldr r3, [r3]
006366e8 cmp r3, #0
006366ec str r3, [sp, #0x98]
006366f0 ldrne r2, [r3]
006366f4 addne r2, r2, #1
006366f8 strne r2, [r3]
006366fc ldr r1, [sp, #0x70]
00636700 cmp r1, #0
00636704 beq #0x636aa4
00636708 ldr fp, [sp, #0x98]
0063670c cmp fp, #0
00636710 beq #0x636b0c
00636714 ldr r2, [sp, #0xd0]
00636718 ldr ip, [sp, #0xd0]
0063671c ldr r2, [r2]
00636720 str r2, [sp, #0x1c]
00636724 ldrh r3, [fp, #0xe]
00636728 cmp ip, r2
0063672c str r3, [sp, #0x14]
00636730 beq #0x636b44
00636734 add r0, sp, #0x98
00636738 mov r6, #0
0063673c str r0, [sp, #0x10]
00636740 b #0x63675c
00636744 ldr r1, [sp, #0x1c]
00636748 ldr r2, [sp, #0xd0]
0063674c ldr r1, [r1]
00636750 cmp r2, r1
00636754 str r1, [sp, #0x1c]
00636758 beq #0x636b30
0063675c ldr r1, [sp, #0x1c]
00636760 ldr r2, [sp, #0x14]
00636764 ldr r1, [r1, #0x10]
00636768 cmp r6, r2
0063676c str r1, [sp, #0x18]
00636770 ldr r5, [r1, #0x28]
00636774 bhs #0x636744
00636778 mov r4, #0
0063677c ldrh r3, [fp, #0xe]
00636780 cmp r3, r6
00636784 ldrhi r3, [fp, #0x20]
00636788 movls r3, #0
0063678c addhi r3, r3, r6, lsl #4
00636790 cmp r4, r5
00636794 bge #0x636744
00636798 ldr sl, [r3]
0063679c ldr r3, [sp, #0x18]
006367a0 mov ip, #0x18
006367a4 mul r7, ip, r4
006367a8 ldr r8, [r3, #0x2c]
006367ac cmp sl, #0
006367b0 add sb, sl, #4
006367b4 movne r1, sb
006367b8 moveq r1, #0
006367bc ldr r0, [r8, r7]
006367c0 bl #0x30e31c
006367c4 cmp r0, #0
006367c8 add r2, r8, r7
006367cc beq #0x636800
006367d0 add r4, r4, #1
006367d4 cmp r4, r5
006367d8 add r7, r7, #0x18
006367dc beq #0x636744
006367e0 cmp sl, #0
006367e4 movne r1, sb
006367e8 moveq r1, #0
006367ec ldr r0, [r8, r7]
006367f0 bl #0x30e31c
006367f4 cmp r0, #0
006367f8 add r2, r8, r7
006367fc bne #0x6367d0
00636800 cmp r5, r4
00636804 ble #0x636744
00636808 mov r1, r6
0063680c ldr r0, [sp, #0x10]
00636810 ldr r3, [sp, #0xd4]
00636814 bl #0x6324c4
00636818 ldr r0, [sp, #0x14]
0063681c add r6, r6, #1
00636820 uxth r6, r6
00636824 cmp r6, r0
00636828 bhs #0x636a9c
0063682c ldr fp, [sp, #0x98]
00636830 b #0x63677c
00636834 ldr r1, [sp, #0x28]
00636838 ldr r1, [r1, #0x20]
0063683c cmp r1, #0
00636840 str r1, [sp, #0x5c]
00636844 ble #0x636694
00636848 mov r2, #0
0063684c str r2, [sp, #0x58]
00636850 str r2, [sp, #0x50]
00636854 ldr r3, [sp, #0x50]
00636858 ldr ip, [sp, #0x54]
0063685c ldr r1, [ip, r3, lsl #2]
00636860 add r0, ip, r3, lsl #2
00636864 str r0, [sp, #0x20]
00636868 cmp r1, #0
0063686c beq #0x636670
00636870 ldr r0, [sp, #0x24]
00636874 bl #0x5d7cf4
00636878 ldr r1, [sp, #0x28]
0063687c ldr r2, [sp, #0x58]
00636880 mov fp, r0
00636884 ldr r3, [r1, #0x24]
00636888 add r3, r3, r2
0063688c str r3, [sp, #0x38]
00636890 ldr r3, [r3, #4]
00636894 cmp r3, #0
00636898 str r3, [sp, #0x48]
0063689c ble #0x636670
006368a0 mov ip, #0
006368a4 str ip, [sp, #0x34]
006368a8 str ip, [sp, #0x30]
006368ac ldr r0, [sp, #0x38]
006368b0 ldr r1, [sp, #0x34]
006368b4 ldr r3, [r0, #8]
006368b8 add r3, r3, r1
006368bc str r3, [sp, #0x14]
006368c0 ldr r2, [r3, #0x6c]
006368c4 cmp r2, #0
006368c8 str r2, [sp, #0x10]
006368cc ble #0x63664c
006368d0 ldr r3, [sp, #0x30]
006368d4 mov ip, #0x34
006368d8 mov r4, #0
006368dc uxtb r3, r3
006368e0 mul ip, ip, r3
006368e4 str r3, [sp, #0x1c]
006368e8 str ip, [sp, #0x18]
006368ec mov sl, r4
006368f0 b #0x636928
006368f4 ldr r0, [sp, #0x2c]
006368f8 ldr r1, [r0, r3, lsl #2]
006368fc ldr r3, [sp, #0x20]
00636900 ldr r0, [sp, #0x24]
00636904 ldr r2, [r3]
00636908 ldr r3, [sp, #0x1c]
0063690c stm sp, {r6, r8}
00636910 bl #0x631bc4
00636914 ldr ip, [sp, #0x10]
00636918 add sl, sl, #1
0063691c add r4, r4, #0xc
00636920 cmp sl, ip
00636924 beq #0x63664c
00636928 ldr r0, [sp, #0x14]
0063692c ldr r1, [sp, #0x18]
00636930 ldr r2, [fp, #8]
00636934 ldr r7, [r0, #0x70]
00636938 mov r3, #0
0063693c add r2, r2, r1
00636940 add r5, r7, r4
00636944 ldr sb, [r2, #0x20]
00636948 ldrb r8, [r5, #5]
0063694c ldr r1, [r7, r4]
00636950 mov r0, sb
00636954 mov r2, r8
00636958 bl #0x5e4b74
0063695c movw r2, #0xffff
00636960 cmp r0, r2
00636964 mov r6, r0
00636968 beq #0x6369d0
0063696c ldrb r3, [r5, #4]
00636970 cmp r3, #1
00636974 beq #0x6369f4
00636978 ldr ip, [sp, #0x28]
0063697c ldr r3, [r5, #8]
00636980 mov r0, #0x18
00636984 ldr r2, [ip, #0x2c]
00636988 mla r2, r0, r3, r2
0063698c ldr r2, [r2, #4]
00636990 cmp r2, #0x11
00636994 bne #0x6368f4
00636998 ldr ip, [sp, #0x2c]
0063699c add r2, r8, #5
006369a0 ldr r2, [sb, r2, lsl #3]
006369a4 ldr r0, [sp, #0x20]
006369a8 ldr r1, [ip, r3, lsl #2]
006369ac ldr ip, [sp, #0x1c]
006369b0 add r2, r2, r6, lsl #4
006369b4 ldr r3, [r0]
006369b8 ldrh r2, [r2, #4]
006369bc ldr r0, [sp, #0x24]
006369c0 stmib sp, {r6, r8}
006369c4 str ip, [sp]
006369c8 bl #0x5da83c
006369cc b #0x636914
006369d0 ldr ip, [sp, #0x38]
006369d4 mov r0, #3
006369d8 ldr r1, [sp, #0x44]
006369dc ldr r3, [ip]
006369e0 ldr ip, [r7, r4]
006369e4 ldr r2, [sp, #0x40]
006369e8 str ip, [sp]
006369ec bl #0x60b034
006369f0 b #0x636914
006369f4 ldr r1, [sp, #0x3c]
006369f8 ldr r0, [r1, #0xe4]
006369fc ldr r1, [r5, #8]
00636a00 bl #0x5bb378
00636a04 movw r2, #0xffff
00636a08 cmp r0, r2
00636a0c beq #0x636a30
00636a10 ldr r1, [sp, #0x20]
00636a14 ldr r3, [sp, #0x1c]
00636a18 ldr r2, [r1]
00636a1c mov r1, r0
00636a20 ldr r0, [sp, #0x24]
00636a24 stm sp, {r6, r8}
00636a28 bl #0x5da6f0
00636a2c b #0x636914
00636a30 add r3, r8, #5
00636a34 ldr r3, [sb, r3, lsl #3]
00636a38 add r3, r3, r6, lsl #4
00636a3c ldrh r2, [r3, #4]
00636a40 cmp r2, #0x12
00636a44 ble #0x636a80
00636a48 cmp r2, #0x1b
00636a4c bgt #0x636a80
00636a50 ldr ip, [sp, #0x3c]
00636a54 ldr r1, [r5, #8]
00636a58 mov r2, #0x12
00636a5c ldr r0, [ip, #0xe4]
00636a60 mov lr, r2
00636a64 ldr ip, [r3, #8]
00636a68 str ip, [sp]
00636a6c ldrb ip, [r3, #7]
00636a70 mov r3, lr
00636a74 str ip, [sp, #4]
00636a78 bl #0x5bc374
00636a7c b #0x636a10
00636a80 cmp r2, #0x12
00636a84 beq #0x636a50
00636a88 ldr ip, [sp, #0x3c]
00636a8c ldr r1, [r5, #8]
00636a90 ldrb lr, [r3, #6]
00636a94 ldr r0, [ip, #0xe4]
00636a98 b #0x636a64
00636a9c ldr fp, [sp, #0x98]
00636aa0 b #0x636744
00636aa4 ldr fp, [sp, #0x98]
00636aa8 cmp fp, #0
00636aac beq #0x636b0c
00636ab0 ldr r0, [sp, #0x64]
00636ab4 add r1, sp, #0x98
00636ab8 str fp, [r0]
00636abc str r1, [sp, #0x10]
00636ac0 ldr r3, [fp]
00636ac4 add r3, r3, #1
00636ac8 str r3, [fp]
00636acc ldr r0, [sp, #0x10]
00636ad0 bl #0x3522b8
00636ad4 ldr r0, [sp, #0x54]
00636ad8 cmp r0, #0
00636adc beq #0x636ae4
00636ae0 bl #0x534688
00636ae4 ldr r1, [sp, #0x2c]
00636ae8 cmp r1, #0
00636aec beq #0x636af8
00636af0 mov r0, r1
00636af4 bl #0x534688
00636af8 ldr r0, [sp, #0x6c]
00636afc bl #0x534268
00636b00 ldr r0, [sp, #0x64]
00636b04 add sp, sp, #0xac
00636b08 pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
00636b0c ldr r1, [pc, #0x54]
00636b10 ldr r2, [sp, #0x40]
00636b14 mov r0, #3
00636b18 add r1, pc, r1
00636b1c bl #0x60b034
00636b20 add r3, sp, #0xa8
00636b24 str r3, [sp, #0x10]
00636b28 ldr fp, [r3, #-0x10]!
00636b2c str r3, [sp, #0x10]
00636b30 ldr ip, [sp, #0x64]
00636b34 cmp fp, #0
00636b38 str fp, [ip]
00636b3c bne #0x636ac0
00636b40 b #0x636acc
00636b44 ldr r2, [sp, #0x64]
00636b48 add r3, sp, #0x98
00636b4c str fp, [r2]
00636b50 str r3, [sp, #0x10]
00636b54 b #0x636ac0
00636b58 eorseq lr, r5, ip, ror r8
00636b5c eoreq lr, sl, r0, lsl sp
00636b60 eoreq lr, sl, ip, asr #25
00636b64 ldrdeq r3, r4, [r0], -ip
00636b68 eoreq lr, sl, r8, lsl r5
