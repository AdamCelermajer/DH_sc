# _ZN7Structs10Projectile4readEP11IStreamBase
004edbb8 push     {r4, r5, lr}
004edbbc mov      r4, r0
004edbc0 sub      sp, sp, #0xc
004edbc4 mov      r5, r1
004edbc8 mov      r0, r1
004edbcc add      r1, r4, #4
004edbd0 bl       #0x4db89c
004edbd4 mov      r0, r5
004edbd8 add      r1, r4, #5
004edbdc bl       #0x4db89c
004edbe0 mov      r0, r5
004edbe4 add      r1, r4, #8
004edbe8 bl       #0x459090
004edbec mov      r3, #1
004edbf0 cmp      r3, #0
004edbf4 str      r3, [sp, #4]
004edbf8 bne      #0x4edc3c
004edbfc add      r3, r4, #9
004edc00 add      r2, r4, #0xa
004edc04 ldrb     r0, [r2, #1]
004edc08 ldrb     r1, [r3, #-1]
004edc0c cmp      r3, r2
004edc10 eor      r1, r0, r1
004edc14 strb     r1, [r3, #-1]
004edc18 ldrb     r0, [r2, #1]
004edc1c eor      r1, r1, r0
004edc20 strb     r1, [r2, #1]
004edc24 ldrb     r0, [r3, #-1]
004edc28 sub      r2, r2, #1
004edc2c eor      r1, r1, r0
004edc30 strb     r1, [r3, #-1]
004edc34 add      r3, r3, #1
004edc38 blo      #0x4edc04
004edc3c mov      r0, r5
004edc40 add      r1, r4, #0xc
004edc44 bl       #0x459090
004edc48 mov      r3, #1
004edc4c cmp      r3, #0
004edc50 str      r3, [sp, #4]
004edc54 bne      #0x4edc98
004edc58 add      r3, r4, #0xd
004edc5c add      r2, r4, #0xe
004edc60 ldrb     r0, [r2, #1]
004edc64 ldrb     r1, [r3, #-1]
004edc68 cmp      r3, r2
004edc6c eor      r1, r0, r1
004edc70 strb     r1, [r3, #-1]
004edc74 ldrb     r0, [r2, #1]
004edc78 eor      r1, r1, r0
004edc7c strb     r1, [r2, #1]
004edc80 ldrb     r0, [r3, #-1]
004edc84 sub      r2, r2, #1
004edc88 eor      r1, r1, r0
004edc8c strb     r1, [r3, #-1]
004edc90 add      r3, r3, #1
004edc94 blo      #0x4edc60
004edc98 add      r1, r4, #0x10
004edc9c mov      r0, r5
004edca0 bl       #0x4db89c
004edca4 mov      r0, r5
004edca8 add      r1, r4, #0x14
004edcac bl       #0x459090
004edcb0 mov      r3, #1
004edcb4 cmp      r3, #0
004edcb8 str      r3, [sp, #4]
004edcbc bne      #0x4edd00
004edcc0 add      r3, r4, #0x15
004edcc4 add      r2, r4, #0x16
004edcc8 ldrb     r0, [r2, #1]
004edccc ldrb     r1, [r3, #-1]
004edcd0 cmp      r3, r2
004edcd4 eor      r1, r0, r1
004edcd8 strb     r1, [r3, #-1]
004edcdc ldrb     r0, [r2, #1]
004edce0 eor      r1, r1, r0
004edce4 strb     r1, [r2, #1]
004edce8 ldrb     r0, [r3, #-1]
004edcec sub      r2, r2, #1
004edcf0 eor      r1, r1, r0
004edcf4 strb     r1, [r3, #-1]
004edcf8 add      r3, r3, #1
004edcfc blo      #0x4edcc8
004edd00 mov      r0, r5
004edd04 add      r1, r4, #0x18
004edd08 bl       #0x459090
004edd0c mov      r3, #1
004edd10 cmp      r3, #0
004edd14 str      r3, [sp, #4]
004edd18 bne      #0x4edd5c
004edd1c add      r3, r4, #0x19
004edd20 add      r2, r4, #0x1a
004edd24 ldrb     r0, [r2, #1]
004edd28 ldrb     r1, [r3, #-1]
004edd2c cmp      r3, r2
004edd30 eor      r1, r0, r1
004edd34 strb     r1, [r3, #-1]
004edd38 ldrb     r0, [r2, #1]
004edd3c eor      r1, r1, r0
004edd40 strb     r1, [r2, #1]
004edd44 ldrb     r0, [r3, #-1]
004edd48 sub      r2, r2, #1
004edd4c eor      r1, r1, r0
004edd50 strb     r1, [r3, #-1]
004edd54 add      r3, r3, #1
004edd58 blo      #0x4edd24
004edd5c add      r1, r4, #0x1c
004edd60 mov      r0, r5
004edd64 bl       #0x4db89c
004edd68 mov      r0, r5
004edd6c add      r1, r4, #0x1d
004edd70 bl       #0x4db89c
004edd74 mov      r0, r5
004edd78 add      r1, r4, #0x20
004edd7c bl       #0x4db94c
004edd80 mov      r3, #1
004edd84 cmp      r3, #0
004edd88 str      r3, [sp, #4]
004edd8c bne      #0x4eddd0
004edd90 add      r3, r4, #0x21
004edd94 add      r2, r4, #0x22
004edd98 ldrb     r0, [r2, #1]
004edd9c ldrb     r1, [r3, #-1]
004edda0 cmp      r3, r2
004edda4 eor      r1, r0, r1
004edda8 strb     r1, [r3, #-1]
004eddac ldrb     r0, [r2, #1]
004eddb0 eor      r1, r1, r0
004eddb4 strb     r1, [r2, #1]
004eddb8 ldrb     r0, [r3, #-1]
004eddbc sub      r2, r2, #1
004eddc0 eor      r1, r1, r0
004eddc4 strb     r1, [r3, #-1]
004eddc8 add      r3, r3, #1
004eddcc blo      #0x4edd98
004eddd0 mov      r0, r5
004eddd4 add      r1, r4, #0x24
004eddd8 bl       #0x4db94c
004edddc mov      r3, #1
004edde0 cmp      r3, #0
004edde4 str      r3, [sp, #4]
004edde8 bne      #0x4ede2c
004eddec add      r3, r4, #0x25
004eddf0 add      r2, r4, #0x26
004eddf4 ldrb     r0, [r2, #1]
004eddf8 ldrb     r1, [r3, #-1]
004eddfc cmp      r3, r2
004ede00 eor      r1, r0, r1
004ede04 strb     r1, [r3, #-1]
004ede08 ldrb     r0, [r2, #1]
004ede0c eor      r1, r1, r0
004ede10 strb     r1, [r2, #1]
004ede14 ldrb     r0, [r3, #-1]
004ede18 sub      r2, r2, #1
004ede1c eor      r1, r1, r0
004ede20 strb     r1, [r3, #-1]
004ede24 add      r3, r3, #1
004ede28 blo      #0x4eddf4
004ede2c mov      r0, r5
004ede30 add      r1, r4, #0x28
004ede34 bl       #0x459090
004ede38 mov      r3, #1
004ede3c cmp      r3, #0
004ede40 str      r3, [sp, #4]
004ede44 bne      #0x4ede88
004ede48 add      r3, r4, #0x29
004ede4c add      r2, r4, #0x2a
004ede50 ldrb     r0, [r2, #1]
004ede54 ldrb     r1, [r3, #-1]
004ede58 cmp      r3, r2
004ede5c eor      r1, r0, r1
004ede60 strb     r1, [r3, #-1]
004ede64 ldrb     r0, [r2, #1]
004ede68 eor      r1, r1, r0
004ede6c strb     r1, [r2, #1]
004ede70 ldrb     r0, [r3, #-1]
004ede74 sub      r2, r2, #1
004ede78 eor      r1, r1, r0
004ede7c strb     r1, [r3, #-1]
004ede80 add      r3, r3, #1
004ede84 blo      #0x4ede50
004ede88 mov      r0, r5
004ede8c add      r1, r4, #0x2c
004ede90 bl       #0x459090
004ede94 mov      r3, #1
004ede98 cmp      r3, #0
004ede9c str      r3, [sp, #4]
004edea0 bne      #0x4edee4
004edea4 add      r3, r4, #0x2d
004edea8 add      r2, r4, #0x2e
004edeac ldrb     r0, [r2, #1]
004edeb0 ldrb     r1, [r3, #-1]
004edeb4 cmp      r3, r2
004edeb8 eor      r1, r0, r1
004edebc strb     r1, [r3, #-1]
004edec0 ldrb     r0, [r2, #1]
004edec4 eor      r1, r1, r0
004edec8 strb     r1, [r2, #1]
004edecc ldrb     r0, [r3, #-1]
004eded0 sub      r2, r2, #1
004eded4 eor      r1, r1, r0
004eded8 strb     r1, [r3, #-1]
004ededc add      r3, r3, #1
004edee0 blo      #0x4edeac
004edee4 mov      r0, r5
004edee8 add      r1, r4, #0x30
004edeec bl       #0x459090
004edef0 mov      r3, #1
004edef4 cmp      r3, #0
004edef8 str      r3, [sp, #4]
004edefc bne      #0x4edf40
004edf00 add      r3, r4, #0x31
004edf04 add      r2, r4, #0x32
004edf08 ldrb     r0, [r2, #1]
004edf0c ldrb     r1, [r3, #-1]
004edf10 cmp      r3, r2
004edf14 eor      r1, r0, r1
004edf18 strb     r1, [r3, #-1]
004edf1c ldrb     r0, [r2, #1]
004edf20 eor      r1, r1, r0
004edf24 strb     r1, [r2, #1]
004edf28 ldrb     r0, [r3, #-1]
004edf2c sub      r2, r2, #1
004edf30 eor      r1, r1, r0
004edf34 strb     r1, [r3, #-1]
004edf38 add      r3, r3, #1
004edf3c blo      #0x4edf08
004edf40 add      r1, r4, #0x34
004edf44 mov      r0, r5
004edf48 bl       #0x4db89c
004edf4c mov      r0, r5
004edf50 add      r1, r4, #0x35
004edf54 bl       #0x4db89c
004edf58 mov      r0, r5
004edf5c add      r1, r4, #0x38
004edf60 bl       #0x459090
004edf64 mov      r3, #1
004edf68 cmp      r3, #0
004edf6c str      r3, [sp, #4]
004edf70 bne      #0x4edfb4
004edf74 add      r3, r4, #0x39
004edf78 add      r2, r4, #0x3a
004edf7c ldrb     r0, [r2, #1]
004edf80 ldrb     r1, [r3, #-1]
004edf84 cmp      r3, r2
004edf88 eor      r1, r0, r1
004edf8c strb     r1, [r3, #-1]
004edf90 ldrb     r0, [r2, #1]
004edf94 eor      r1, r1, r0
004edf98 strb     r1, [r2, #1]
004edf9c ldrb     r0, [r3, #-1]
004edfa0 sub      r2, r2, #1
004edfa4 eor      r1, r1, r0
004edfa8 strb     r1, [r3, #-1]
004edfac add      r3, r3, #1
004edfb0 blo      #0x4edf7c
004edfb4 mov      r0, r5
004edfb8 add      r1, r4, #0x3c
004edfbc bl       #0x459090
004edfc0 mov      r3, #1
004edfc4 cmp      r3, #0
004edfc8 str      r3, [sp, #4]
004edfcc bne      #0x4ee010
004edfd0 add      r3, r4, #0x3d
004edfd4 add      r2, r4, #0x3e
004edfd8 ldrb     r0, [r2, #1]
004edfdc ldrb     r1, [r3, #-1]
004edfe0 cmp      r3, r2
004edfe4 eor      r1, r0, r1
004edfe8 strb     r1, [r3, #-1]
004edfec ldrb     r0, [r2, #1]
004edff0 eor      r1, r1, r0
004edff4 strb     r1, [r2, #1]
004edff8 ldrb     r0, [r3, #-1]
004edffc sub      r2, r2, #1
004ee000 eor      r1, r1, r0
004ee004 strb     r1, [r3, #-1]
004ee008 add      r3, r3, #1
004ee00c blo      #0x4edfd8
004ee010 mov      r0, r5
004ee014 add      r1, r4, #0x40
004ee018 bl       #0x4db94c
004ee01c mov      r3, #1
004ee020 cmp      r3, #0
004ee024 str      r3, [sp, #4]
004ee028 bne      #0x4ee06c
004ee02c add      r3, r4, #0x41
004ee030 add      r2, r4, #0x42
004ee034 ldrb     r0, [r2, #1]
004ee038 ldrb     r1, [r3, #-1]
004ee03c cmp      r3, r2
004ee040 eor      r1, r0, r1
004ee044 strb     r1, [r3, #-1]
004ee048 ldrb     r0, [r2, #1]
004ee04c eor      r1, r1, r0
004ee050 strb     r1, [r2, #1]
004ee054 ldrb     r0, [r3, #-1]
004ee058 sub      r2, r2, #1
004ee05c eor      r1, r1, r0
004ee060 strb     r1, [r3, #-1]
004ee064 add      r3, r3, #1
004ee068 blo      #0x4ee034
004ee06c mov      r0, r5
004ee070 add      r1, r4, #0x44
004ee074 bl       #0x4db94c
004ee078 mov      r3, #1
004ee07c cmp      r3, #0
004ee080 str      r3, [sp, #4]
004ee084 bne      #0x4ee0c8
004ee088 add      r3, r4, #0x46
004ee08c add      r4, r4, #0x45
004ee090 ldrb     r1, [r3, #1]
004ee094 ldrb     r2, [r4, #-1]
004ee098 cmp      r4, r3
004ee09c eor      r2, r1, r2
004ee0a0 strb     r2, [r4, #-1]
004ee0a4 ldrb     r1, [r3, #1]
004ee0a8 eor      r2, r2, r1
004ee0ac strb     r2, [r3, #1]
004ee0b0 ldrb     r1, [r4, #-1]
004ee0b4 sub      r3, r3, #1
004ee0b8 eor      r2, r2, r1
004ee0bc strb     r2, [r4, #-1]
004ee0c0 add      r4, r4, #1
004ee0c4 blo      #0x4ee090
004ee0c8 add      sp, sp, #0xc
004ee0cc pop      {r4, r5, pc}
